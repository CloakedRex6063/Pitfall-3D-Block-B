#pragma once
#include "components/component.h"
#include "math/vector3.h"

class SkeletalModel;
class PhysicsComponent;
class ControllerComponent;
class Animation;
class Animator;

class SkeletalMeshComponent : public Component
{
public:
	SkeletalMeshComponent(Actor* owner);
	~SkeletalMeshComponent() override;

	void Init() override;
	void Tick(float deltaTime) override;
	void Render() override;

	void AddAnimation(const std::string& path, const std::string& name) const;
	void PlayAnimation(const std::string& name, bool bNewLoop = true, float animSpeed = 1, bool bYLock = false, float newOffset = 0) const;

	std::string GetCurrentAnimation() const;

	void SetModel(SkeletalModel* newModel);
	SkeletalModel* GetModel() const;

	Vector3 offset;
	Animator* animator = nullptr;
	ControllerComponent* controllerComponent = nullptr;

private:
	SkeletalModel* model = nullptr;
	PhysicsComponent* physicsComponent = nullptr;
};

