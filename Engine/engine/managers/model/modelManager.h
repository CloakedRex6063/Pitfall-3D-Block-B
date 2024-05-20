#pragma once
#include <unordered_map>
#include "engine/graphics/model/staticModel.h"

class SkeletalModel;

class ModelManager
{
public:
	void AddModel(const std::string& name, const std::string& path, Vector3 scale, Vector3 offset, Vector3 rot, Vector2 texScale);
	StaticModel* GetModel(const std::string& name);
	void Render();

	SkeletalModel* playerModel;

	unordered_map<std::string, std::vector<glm::mat4>> modelMatrices;
	std::unordered_map<std::string, StaticModel*> models;
};

