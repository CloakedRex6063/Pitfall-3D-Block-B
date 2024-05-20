#pragma once
#include "engine/util/common.h"
WARNING_DISABLE
#include "bullet/btBulletDynamicsCommon.h"
WARNING_ENABLE

class DebugDrawer;

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();
	void Tick(float deltaTime);
	void Render();

	void AddRigidBody(btRigidBody* body);
	void AddCharacter(btCollisionObject* object, int filter, int filterMask, btActionInterface* character);
	void SetGravity(const btVector3& gravity);

	void ToggleDebugDraw(void* val);
	void RemoveRigidBody(btRigidBody* body);
	void RemoveCharacter(btCollisionObject* object, btActionInterface* character);
	bool bDrawDebug = true;

private:
	btBroadphaseInterface* broadPhase = nullptr;
	btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
	btCollisionDispatcher* dispatcher = nullptr;
	btSequentialImpulseConstraintSolver* solver = nullptr;
	btDiscreteDynamicsWorld* dynamicsWorld = nullptr;
	DebugDrawer* physicsDebugDraw = nullptr;
};

