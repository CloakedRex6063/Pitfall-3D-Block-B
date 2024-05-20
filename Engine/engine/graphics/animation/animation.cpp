#include "animation.h"

Animation::Animation(const std::string& animationPath, SkeletalModel* model)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
	assert(scene && scene->mRootNode);
	const auto animation = scene->mAnimations[0];
	duration = static_cast<float>(animation->mDuration);
	ticksPerSecond = static_cast<int>(animation->mTicksPerSecond);
	aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
	globalTransformation = globalTransformation.Inverse();
	ReadHierarchyData(rootNode, scene->mRootNode);
	ReadMissingBones(animation, *model);
}

Bone* Animation::FindBone(const std::string& name)
{
	const auto iter = std::find_if(bones.begin(), bones.end(),
	                               [&](const Bone& Bone)
	                               {
		                               return Bone.GetBoneName() == name;
	                               });
	if (iter == bones.end()) return nullptr;
	return &(*iter);
}

int Animation::GetTicksPerSecond() const
{
	return ticksPerSecond;
}

float Animation::GetDuration() const
{
	return duration;
}

const AssimpNodeData& Animation::GetRootNode()
{
	return rootNode;
}

const std::map<std::string, BoneInfo>& Animation::GetBoneIDMap()
{ 
	return boneInfoMap;
}

void Animation::ReadMissingBones(const aiAnimation* animation, SkeletalModel& model)
{
	const unsigned size = animation->mNumChannels;

	auto& boneMap = model.GetBoneInfoMap();//getting m_boneMap from Model class
	int& boneCount = model.GetBoneCount(); //getting the m_BoneCounter from Model class

	//reading channels(bones engaged in an animation and their keyframes)
	for (unsigned i = 0; i < size; i++)
	{
		const auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;

		if (boneMap.find(boneName) == boneMap.end())
		{
			boneMap[boneName].id = boneCount;
			boneCount++;
		}
		bones.push_back(Bone(channel->mNodeName.data,
		                       boneMap[channel->mNodeName.data].id, channel));
	}

	boneInfoMap = boneMap;
}

void Animation::ReadHierarchyData(AssimpNodeData& dest, const aiNode* src)
{
	assert(src);

	dest.name = src->mName.data;
	dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
	dest.childrenCount = src->mNumChildren;

	for (unsigned i = 0; i < src->mNumChildren; i++)
	{
		AssimpNodeData newData;
		ReadHierarchyData(newData, src->mChildren[i]);
		dest.children.push_back(newData);
	}
}
