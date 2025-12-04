#include "bone.h"

Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel):
	localTransform(1.0f),
	name(name),
	ID(ID)
{
	numPositions = channel->mNumPositionKeys;

	for (unsigned positionIndex = 0; positionIndex < numPositions; ++positionIndex)
	{
		aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
		float timeStamp = static_cast<float>(channel->mPositionKeys[positionIndex].mTime);
		KeyPosition data;
		data.position = AssimpGLMHelpers::GetGLMVec(aiPosition);
		data.timeStamp = timeStamp;
		positions.push_back(data);
	}

	numRotations = channel->mNumRotationKeys;
	for (unsigned rotationIndex = 0; rotationIndex < numRotations; ++rotationIndex)
	{
		aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
		float timeStamp = static_cast<float>(channel->mRotationKeys[rotationIndex].mTime);
		KeyRotation data;
		data.orientation = AssimpGLMHelpers::GetGLMQuat(aiOrientation);
		data.timeStamp = timeStamp;
		rotations.push_back(data);
	}

	numScalings = channel->mNumScalingKeys;
	for (unsigned keyIndex = 0; keyIndex < numScalings; ++keyIndex)
	{
		aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
		float timeStamp = static_cast<float>(channel->mScalingKeys[keyIndex].mTime);
		KeyScale data;
		data.scale = AssimpGLMHelpers::GetGLMVec(scale);
		data.timeStamp = timeStamp;
		scales.push_back(data);
	}
}

void Bone::Update(float animationTime)
{
	const glm::mat4 translation = InterpolatePosition(animationTime);
	const glm::mat4 rotation = InterpolateRotation(animationTime);
	const glm::mat4 scale = InterpolateScaling(animationTime);
	localTransform = translation * rotation * scale;
}

void Bone::UpdateRoot(float animationTime, bool bYLock, float offset)
{
	const glm::mat4 translation = InterpolatePositionRoot(animationTime, bYLock, offset);
	const glm::mat4 rotation = InterpolateRotation(animationTime);
	const glm::mat4 scale = InterpolateScaling(animationTime);
	localTransform = translation * rotation * scale;
}

glm::mat4 Bone::GetLocalTransform() const
{
	return localTransform;
}

std::string Bone::GetBoneName() const
{
	return name;
}

int Bone::GetBoneID() const
{
	return ID;
}

int Bone::GetPositionIndex(float animationTime) const
{
	for (unsigned index = 0; index < numPositions - 1; ++index)
	{
		if (animationTime < positions[index + 1].timeStamp)
			return static_cast<int>(index);
	}
	assert(0);
}

int Bone::GetRotationIndex(float animationTime) const
{
	for (unsigned index = 0; index < numRotations - 1; ++index)
	{
		if (animationTime < rotations[index + 1].timeStamp)
			return static_cast<int>(index);
	}
	assert(0);
}

int Bone::GetScaleIndex(float animationTime) const
{
	for (unsigned index = 0; index < numScalings - 1; ++index)
	{
		if (animationTime < scales[index + 1].timeStamp)
			return static_cast<int>(index);
	}
	assert(0);
}

float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
	const float midWayLength = animationTime - lastTimeStamp;
	const float framesDiff = nextTimeStamp - lastTimeStamp;
	const float scaleFactor = midWayLength / framesDiff;
	return scaleFactor;
}

glm::mat4 Bone::InterpolatePosition(float animationTime)
{
	if (1 == numPositions)
	{
		return glm::translate(glm::mat4(1.0f), positions[0].position);
	}

	const int p0Index = GetPositionIndex(animationTime);
	const int p1Index = p0Index + 1;
	const float scaleFactor = GetScaleFactor(positions[p0Index].timeStamp,
	                                         positions[p1Index].timeStamp, animationTime);
	const glm::vec3 finalPosition = glm::mix(positions[p0Index].position, positions[p1Index].position
	                                         , scaleFactor);
	return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::InterpolatePositionRoot(float animationTime, bool bYLock, float offset)
{
	if (1 == numPositions)
	{
		return glm::translate(glm::mat4(1.0f), positions[0].position);
	}

	const int p0Index = GetPositionIndex(animationTime);
	const int p1Index = p0Index + 1;
	const float scaleFactor = GetScaleFactor(positions[p0Index].timeStamp,
	                                         positions[p1Index].timeStamp, animationTime);
	const glm::vec3 finalPosition = glm::mix(glm::vec3(positions[p0Index].position.x,
	                                                   positions[bYLock ? 0 : p0Index].position.y + offset,
	                                                   positions[0].position.z),
	                                         glm::vec3(positions[p1Index].position.x,
	                                                   positions[bYLock ? 0 : p1Index].position.y + offset,
	                                                   positions[0].position.z),
	                                         scaleFactor);
	return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::InterpolateRotation(float animationTime)
{
	if (1 == numRotations)
	{
		const auto rotation = glm::normalize(rotations[0].orientation);
		return glm::toMat4(rotation);
	}

	const int p0Index = GetRotationIndex(animationTime);
	const int p1Index = p0Index + 1;
	const float scaleFactor = GetScaleFactor(rotations[p0Index].timeStamp,
	                                         rotations[p1Index].timeStamp, animationTime);
	glm::quat finalRotation = glm::slerp(rotations[p0Index].orientation, rotations[p1Index].orientation
	                                     , scaleFactor);
	finalRotation = glm::normalize(finalRotation);
	return glm::toMat4(finalRotation);
}

glm::mat4 Bone::InterpolateScaling(float animationTime)
{
	if (1 == numScalings)
		return glm::scale(glm::mat4(1.0f), scales[0].scale);

	const int p0Index = GetScaleIndex(animationTime);
	const int p1Index = p0Index + 1;
	const float scaleFactor = GetScaleFactor(scales[p0Index].timeStamp,
	                                         scales[p1Index].timeStamp, animationTime);
	const glm::vec3 finalScale = glm::mix(scales[p0Index].scale, scales[p1Index].scale
	                                      , scaleFactor);
	return glm::scale(glm::mat4(1.0f), finalScale);
}
