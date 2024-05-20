#include "enemy.h"
#include "engine/components/mesh/staticMeshComponent.h"
#include "engine/components/physics/physicsComponent.h"

Enemy::Enemy()
{
	AddTag("Enemy");
}

void Enemy::Init()
{
	Actor::Init();
	AddComponent<PhysicsComponent>();
	AddComponent<StaticMeshComponent>();
}
