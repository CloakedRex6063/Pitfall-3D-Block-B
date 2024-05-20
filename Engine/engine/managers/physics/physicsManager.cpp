#include "physicsManager.h"
WARNING_DISABLE
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
WARNING_ENABLE
#include "engine/graphics/shader/shader.h"
#include "engine/physics/debug/debugDrawer.h"

PhysicsManager::PhysicsManager()
{
	broadPhase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

	const auto debugShader = new Shader("Linux/shaders/debug.vert", "Linux/shaders/debug.frag");
	physicsDebugDraw = new DebugDrawer(debugShader);
	physicsDebugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	dynamicsWorld->setDebugDrawer(physicsDebugDraw);
	dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
}

PhysicsManager::~PhysicsManager()
{
	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadPhase;
	delete physicsDebugDraw;
}

void PhysicsManager::Tick(float deltaTime)
{
	dynamicsWorld->stepSimulation(1/60.f, 10);
}

void PhysicsManager::Render()
{
	if (bDrawDebug)
	{
		physicsDebugDraw->Draw();
		dynamicsWorld->debugDrawWorld();
	}
}

void PhysicsManager::AddRigidBody(btRigidBody* body)
{
	dynamicsWorld->addRigidBody(body);
}

void PhysicsManager::AddCharacter(btCollisionObject* object, int filter, int filterMask, btActionInterface* character)
{
	dynamicsWorld->addCollisionObject(object,filter,filterMask);
	dynamicsWorld->addAction(character);
}

void PhysicsManager::SetGravity(const btVector3& gravity)
{
	dynamicsWorld->setGravity(gravity);
}

void PhysicsManager::ToggleDebugDraw(void*)
{
	bDrawDebug = !bDrawDebug;
}

void PhysicsManager::RemoveRigidBody(btRigidBody* body)
{
	dynamicsWorld->removeRigidBody(body);
}

void PhysicsManager::RemoveCharacter(btCollisionObject* object, btActionInterface* character)
{
	dynamicsWorld->removeCollisionObject(object);
	dynamicsWorld->removeAction(character);
}
