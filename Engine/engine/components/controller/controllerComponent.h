#pragma once
#include "engine/components/component.h"
#include "engine/util/common.h"
WARNING_DISABLE
#include "bullet/btBulletDynamicsCommon.h"
#include "bullet/BulletCollision/CollisionDispatch/btGhostObject.h"
#include "bullet/BulletDynamics/Character/btKinematicCharacterController.h"
WARNING_ENABLE
#include "engine/math/vector3.h"

class ControllerComponent : public Component
{
public:
	ControllerComponent(Actor* owner);
	~ControllerComponent() override;
	void Init() override;
	void Tick(float deltaTime) override;
	void Render() override;

	void Move(Vector3 direction, float scale);
	void Rotate(Vector3 axis, float angle) const;
	void Jump();
	void Slide();
	void ResetSlide();
	void StopMovement();
	void SetGravity(float gravity) const;
	void SetPosition(Vector3 pos) const;

	void SwitchLane(int lane);
	void LerpLanes(float dt) const;

	float speed = 20;
	float walkSpeed = 20;
	float jumpSpeed = 7;
	float slideSpeed = 30;
	Vector3 velocity;

	int currentLane = 0;
	float laneWidth = 2.5;
	float laneChangeSpeed = 20;

	btTransform& GetTransform() const;
	btPairCachingGhostObject* GetGhostObject() const;

private:
	btKinematicCharacterController* controller = nullptr;
	btPairCachingGhostObject* ghostObject = nullptr;

	float capsuleHalfHeight = 0;
	bool bCanSlide = true;
	bool bCanMove = true;
};

