#pragma once
#include "engine/actor/actor.h"
#include "engine/math/vector3.h"

class InputManager;
class SkeletalMeshComponent;

class Character : public Actor
{
public:
	Character();
	~Character() override;
	void Init() override;
	void Tick(float deltaTime) override;
	void Render() override;

	virtual void SetupInputs(InputManager* manager) = 0;
	virtual void SetupAnimations(SkeletalMeshComponent* meshComponent) = 0;

	Vector3 GetPosition();
};

