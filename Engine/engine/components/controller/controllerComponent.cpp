#include "controllerComponent.h"

#include "engine/actor/actor.h"
#include "engine/components/mesh/skeletalMeshComponent.h"
#include "engine/framework/gameInstance.h"
#include "engine/graphics/model/skeletalModel.h"
#include "engine/managers/physics/physicsManager.h"
#include "engine/util/util.h"

ControllerComponent::ControllerComponent(Actor* owner) : Component(owner)
{
}

ControllerComponent::~ControllerComponent()
{
	// Remove the character from the physics manager
	Util::GetGameInstance()->GetPhysicsManager()->RemoveCharacter(ghostObject,controller);
	delete controller;
	delete ghostObject;
}

void ControllerComponent::Init()
{
	// Get MeshComponent from owner
	const auto meshComp = GetOwner()->GetComponent<SkeletalMeshComponent>();

	// Set controller component for MeshComponent
	meshComp->controllerComponent = this;

	// Create btPairCachingGhostObject
	ghostObject = new btPairCachingGhostObject();

	// Get capsule collider shape from MeshComponent's model
	const auto shape = meshComp->GetModel()->GetCapsuleCollider();
	capsuleHalfHeight = shape->getImplicitShapeDimensions().getX();

	// Set the collision shape for the ghost object
	ghostObject->setCollisionShape(shape);

	// Set owner as user pointer for the ghost object
	ghostObject->setUserPointer(GetOwner());

	// Create btKinematicCharacterController
	controller = new btKinematicCharacterController(ghostObject, shape, 0.35f, Vector3(0, 1, 0));
	controller->setGravity(btVector3(0, -9.81f, 0));

	// Add the character to the physics manager
	Util::GetGameInstance()->GetPhysicsManager()->AddCharacter(ghostObject,
		btBroadphaseProxy::CharacterFilter | btBroadphaseProxy::DefaultFilter,
		btBroadphaseProxy::AllFilter, controller);

	// Set transform for the ghost object
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(Vector3(0, 0, 0));
	ghostObject->setWorldTransform(transform);
}

void ControllerComponent::Tick(float deltaTime)
{
	controller->setWalkDirection(bCanMove ? velocity * deltaTime : Vector3());
	velocity = Vector3();
	LerpLanes(deltaTime);
}

void ControllerComponent::Render()
{
}

void ControllerComponent::Move(Vector3 direction, float scale)
{
	speed = walkSpeed;
	velocity += direction * speed * scale;
}

void ControllerComponent::Rotate(Vector3 axis, float angle) const
{
	const float angleInRad = btRadians(angle);
	ghostObject->getWorldTransform().setRotation(btQuaternion(axis,angleInRad));
}

void ControllerComponent::Jump()
{
	if (controller->canJump())
	{
		GetOwner()->GetComponent<SkeletalMeshComponent>()->PlayAnimation("Jump", false, 1, true);
		controller->jump(btVector3(0,jumpSpeed,0));
	}
}

void ControllerComponent::Slide() 
{
	const auto capsuleShape = dynamic_cast<btCapsuleShape*>(ghostObject->getCollisionShape());
	if (capsuleShape && controller->canJump() && bCanSlide)
	{
		bCanSlide = false;
		speed = slideSpeed;
		capsuleShape->setLocalScaling(Vector3(1.f, capsuleHalfHeight/2, 1.f));
		GetOwner()->GetComponent<SkeletalMeshComponent>()->PlayAnimation("Slide", false, 1, false, 75);
	}
}

void ControllerComponent::ResetSlide()
{
	if (const auto capsuleShape = dynamic_cast<btCapsuleShape*>(ghostObject->getCollisionShape()))
	{
		bCanSlide = true;
		speed = walkSpeed;
		capsuleShape->setLocalScaling(Vector3(1.f, 1.f, 1.f));
	}
}

void ControllerComponent::StopMovement()
{
	bCanMove = false;
}

void ControllerComponent::SetGravity(float gravity) const
{
	controller->setGravity(btVector3(0, gravity, 0));
}

void ControllerComponent::SetPosition(Vector3 pos) const
{
	controller->warp(pos);
}

void ControllerComponent::SwitchLane(int lane)
{
	currentLane = lane;
}

void ControllerComponent::LerpLanes(float deltaTime) const
{
	const Vector3 pos = ghostObject->getWorldTransform().getOrigin();
	const Vector3 targetPosition = Vector3(static_cast<float>(currentLane) * laneWidth, pos.y, pos.z);
	const auto lerpPos = Vector3::Lerp(pos,targetPosition,laneChangeSpeed * deltaTime);
	controller->warp(lerpPos);
}

btTransform& ControllerComponent::GetTransform() const
{
	return ghostObject->getWorldTransform();
}

btPairCachingGhostObject* ControllerComponent::GetGhostObject() const
{
	return ghostObject;
}
