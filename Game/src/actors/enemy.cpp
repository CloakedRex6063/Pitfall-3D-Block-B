#include "enemy.h"
#include "components/mesh/staticMeshComponent.h"
#include "components/physics/physicsComponent.h"

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
