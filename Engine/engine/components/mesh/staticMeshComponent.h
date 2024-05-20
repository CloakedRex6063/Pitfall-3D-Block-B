#pragma once
#include <glm/glm.hpp>
#include "engine/components/component.h"
#include "engine/math/vector3.h"

class PhysicsComponent;
class StaticModel;

class StaticMeshComponent : public Component
{
public:
	StaticMeshComponent(Actor* owner);
	~StaticMeshComponent() override;

	void Init() override;
	void Tick(float deltaTime) override;
	void Render() override;

	void SetModel(StaticModel* newModel, Vector3 pos);
	StaticModel* GetModel() const { return model; }

	glm::mat4 GetModelMatrix() const;

	void IsInstanced(bool value) { bInstanced = value; }

	Vector3 rot;

private:
	StaticModel* model = nullptr;
	PhysicsComponent* physicsComponent = nullptr;
	glm::mat4 modelMat;
	bool bInstanced = true;
};

