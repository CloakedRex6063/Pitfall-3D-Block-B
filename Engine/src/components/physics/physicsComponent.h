#pragma once
#include "components/component.h"
#include "util/common.h"
WARNING_DISABLE
#include "btBulletDynamicsCommon.h"
WARNING_ENABLE
#include "math/vector3.h"

class PhysicsComponent : public Component
{
public:
	PhysicsComponent(Actor* owner);
	~PhysicsComponent() override;
	void Init() override;
	void Tick(float deltaTime) override;
	void Render() override;

	void CreateRigidBody(btCollisionShape* shape, Vector3 initialPos = Vector3());
	void SetPosition(Vector3 position);
	void SetRotation(Vector3 rotation);
	void Rotate(Vector3 rotation) const;
	Vector3 GetRotation() const;
	Vector3 GetPosition() const;
	void SetGravity(float gravity);
	void SetMass(float mass) const;
	btRigidBody* GetBody() const;
	btTransform& GetTransform();

private:
	btRigidBody* body = nullptr;
	float mass = 5;
	btTransform transform;
};

