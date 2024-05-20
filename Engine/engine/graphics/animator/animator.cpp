#include "animator.h"

#include <iostream>

Animator::Animator()
{
	m_CurrentTime = 0.0;
	m_FinalBoneMatrices.reserve(100);

	for (int i = 0; i < 100; i++)
		m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
}

Animator::~Animator()
{
	for (const auto& anim : animations)
		delete anim.second;
	animations.clear();
}

void Animator::UpdateAnimation(float dt)
{
	if (m_CurrentAnimation)
	{
		if (m_CurrentTime >= m_CurrentAnimation->GetDuration() - 1.f && !bLoop)
		{
			if (!bAnimationFinished)
			{
				bAnimationFinished = true;
				onAnimationFinished();
			}
			return;
		}

		// Update animation time based on elapsed time (dt)
		m_CurrentTime += static_cast<float>(m_CurrentAnimation->GetTicksPerSecond()) * dt * animSpeed;

		// Check for looping
		if (bLoop)
		{
			m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
		}
		else
		{
			// Ensure that the current time does not exceed the animation duration
			m_CurrentTime = glm::clamp(m_CurrentTime, 0.0f, m_CurrentAnimation->GetDuration() - 1.f);
		}

		// Calculate bone transformations
		CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
	}	
}

std::string Animator::GetCurrentAnimation() const
{
	for (const auto& anim : animations)
	{
		if (anim.second == m_CurrentAnimation)
		{	
			return anim.first;
		}
	}
	return "";
}

void Animator::PlayAnimation(const std::string& name, bool bNewLoop, float newAnimSpeed, bool bNewYLock, float newOffset)
{
	bAnimationFinished = false;
	bLoop = bNewLoop;
	bYLock = bNewYLock;
	animSpeed = newAnimSpeed;
	offset = newOffset;
	if (animations.find(name) != animations.end())
	{
		m_CurrentAnimation = animations[name];
		m_CurrentTime = 0.f;
	}
	else
	{
		std::cout << "Animation not found" << std::endl;
	}
}

void Animator::AddAnimation(Animation* animation, const std::string& name)
{
	animations.insert(std::pair(name,animation));
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, const glm::mat4& parentTransform)
{
	const std::string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	if (const auto bone = m_CurrentAnimation->FindBone(nodeName))
	{
		// In place animation fix for  root animation
		if (nodeName == "mixamorig_Hips" && m_CurrentTime > 0.f)
		{
			bone->UpdateRoot(m_CurrentTime, bYLock, offset);
		}
		else
		{
			bone->Update(m_CurrentTime);
		}
		nodeTransform = bone->GetLocalTransform();
	}

	const glm::mat4 globalTransformation = parentTransform * nodeTransform;

	auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		const int index = boneInfoMap[nodeName].id;
		const glm::mat4 boneOffset = boneInfoMap[nodeName].offset;
		m_FinalBoneMatrices[index] = globalTransformation * boneOffset;
	}

	for (unsigned i = 0; i < node->childrenCount; i++)
		CalculateBoneTransform(&node->children[i], globalTransformation);
}

std::vector<glm::mat4> Animator::GetFinalBoneMatrices()
{
	return m_FinalBoneMatrices;
}
