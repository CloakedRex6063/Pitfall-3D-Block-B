#pragma once

#include "vector"
#include "map"
#include "glm/glm.hpp"
#include "assimp/scene.h"
#include "engine/graphics/animData/animData.h"
#include "engine/graphics/bone/bone.h"
#include "engine/graphics/model/skeletalModel.h"
#include "engine/util/assimpGLMHelpers.h"

// Referenced from LearnOpenGL.com

struct AssimpNodeData
{
	std::vector<AssimpNodeData> children;
	glm::mat4 transformation;
	std::string name;
	unsigned childrenCount;
};

class Animation
{
public:
	Animation() = default;
	Animation(const std::string& animationPath, SkeletalModel* model);
	Bone* FindBone(const std::string& name);

	int GetTicksPerSecond() const;
	float GetDuration() const;
	const AssimpNodeData& GetRootNode();

	const std::map<std::string,BoneInfo>& GetBoneIDMap();

private:
	void ReadMissingBones(const aiAnimation* animation, SkeletalModel& model);

	static void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);
	float duration;
	int ticksPerSecond;
	std::vector<Bone> bones;
	AssimpNodeData rootNode;
	std::map<std::string, BoneInfo> boneInfoMap;
};
