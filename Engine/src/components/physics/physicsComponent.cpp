#include "physicsComponent.h"
#include "framework/gameInstance.h"
#include "managers/physics/physicsManager.h"
#include "math/vector3.h"
#include "util/util.h"

PhysicsComponent::PhysicsComponent(Actor* owner) : Component(owner)
{

}

PhysicsComponent::~PhysicsComponent()
{
	if (body)
	{
		Util::GetGameInstance()->GetPhysicsManager()->RemoveRigidBody(body);
		delete body->getMotionState();
		delete body;
	}
}

void PhysicsComponent::Init()
{
	transform.setIdentity();
}

void PhysicsComponent::Tick(float deltaTime)
{

}

void PhysicsComponent::CreateRigidBody(btCollisionShape* shape, Vector3 initialPos)
{
	const bool isDynamic = (mass != 0.f);
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
	{
		shape->calculateLocalInertia(mass, localInertia);
	}
	transform.setOrigin(initialPos);
	const auto motionState = new btDefaultMotionState(transform);
	const btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
	body = new btRigidBody(rbInfo);
	body->setAngularFactor(btVector3(0,1,0));
	body->setUserPointer(GetOwner());

	Util::GetGameInstance()->GetPhysicsManager()->AddRigidBody(body);
}

void PhysicsComponent::SetPosition(Vector3 position)
{
	if (body)
	{
		body->getWorldTransform().setOrigin(position);	
	}
}

void PhysicsComponent::SetRotation(Vector3 rotation)
{
	rotation.x = btRadians(rotation.x);
	rotation.y = btRadians(rotation.y);
	rotation.z = btRadians(rotation.z);

	if (body)
	{
		body->getWorldTransform().setRotation(btQuaternion(rotation.z, rotation.y, rotation.x));
	}
}

void PhysicsComponent::Rotate(Vector3 rotation) const
{
	rotation.x = btRadians(rotation.x);
	rotation.y = btRadians(rotation.y);
	rotation.z = btRadians(rotation.z);

	if (body)
	{
		const auto rot = body->getWorldTransform().getRotation();
		body->getWorldTransform().setRotation(rot + btQuaternion(rotation.z, rotation.y, rotation.x));
	}
}

Vector3 PhysicsComponent::GetRotation() const
{
	if (body)
	{
		const auto rot = body->getWorldTransform().getRotation();
		return Vector3(btDegrees(rot.x()), btDegrees(rot.y()), btDegrees(rot.z()));
	}
	return Vector3();
}

Vector3 PhysicsComponent::GetPosition() const
{
	if (body)
	{
		return body->getWorldTransform().getOrigin();
	}
	return Vector3();
}

void PhysicsComponent::SetGravity(float gravity)
{
	if (body)
	{
		body->setGravity(btVector3(0, gravity, 0));
	}
}

void PhysicsComponent::SetMass(const float mass) const
{
	if (body)
	{
		body->setMassProps(mass, Vector3(0, 0, 0));
	}
}

btRigidBody* PhysicsComponent::GetBody() const
{
	return body;
}

btTransform& PhysicsComponent::GetTransform()
{
	return body->getWorldTransform();
}

void PhysicsComponent::Render()
{
}
