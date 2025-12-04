#pragma once

/* Container for bone data */

#include <vector>
#include <assimp/scene.h>
#include <list>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "util/assimpGLMHelpers.h"

// Referenced from LearnOpenGL.com

struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};

struct KeyRotation
{
	glm::quat orientation;
	float timeStamp;
};

struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};

class Bone
{
public:
	Bone(const std::string& name, int ID, const aiNodeAnim* channel);

	void Update(float animationTime);

	void UpdateRoot(float animationTime, bool bYLock, float offset);

	glm::mat4 GetLocalTransform() const;
	std::string GetBoneName() const;
	int GetBoneID() const;

	int GetPositionIndex(float animationTime) const;

	int GetRotationIndex(float animationTime) const;

	int GetScaleIndex(float animationTime) const;

private:

	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

	glm::mat4 InterpolatePosition(float animationTime);

	glm::mat4 InterpolatePositionRoot(float animationTime, bool bYLock, float offset);

	glm::mat4 InterpolateRotation(float animationTime);

	glm::mat4 InterpolateScaling(float animationTime);

	std::vector<KeyPosition> positions;
	std::vector<KeyRotation> rotations;
	std::vector<KeyScale> scales;
	unsigned numPositions;
	unsigned numRotations;
	unsigned numScalings;

	glm::mat4 localTransform;
	std::string name;
	int ID = 0;
};
