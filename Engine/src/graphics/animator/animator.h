#pragma once

#include "functional"
#include "glm/glm.hpp"
#include "map"
#include "vector"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "graphics/bone/bone.h"
#include "graphics/animation/animation.h"

// Referenced from LearnOpenGL.com

class Animator
{
public:
	Animator();

	~Animator();

	void UpdateAnimation(float dt);

	std::string GetCurrentAnimation() const;

	void PlayAnimation(const std::string& name, bool bNewLoop, float newAnimSpeed, bool bNewYLock, float newOffset);

	void AddAnimation(Animation* animation, const std::string& name);

	void CalculateBoneTransform(const AssimpNodeData* node, const glm::mat4& parentTransform);

	std::vector<glm::mat4> GetFinalBoneMatrices();

	function<void()> onAnimationFinished;
private:
	std::vector<glm::mat4> m_FinalBoneMatrices;
	Animation* m_CurrentAnimation = nullptr;
	std::unordered_map<std::string,Animation*> animations;
	float m_CurrentTime;
	float animSpeed = 1;
	float offset = 0;
	bool bLoop = true;
	bool bYLock = false;
	bool bAnimationFinished = false;
};
