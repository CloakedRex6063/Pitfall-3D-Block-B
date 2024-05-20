#include "collectable.h"

#include <iostream>

#include "engine/components/physics/physicsComponent.h"
#include "engine/components/mesh/staticMeshComponent.h"

Collectable::Collectable()
{
	AddTag("Collectable");
}

void Collectable::Init()
{
	Actor::Init();
	AddComponent<PhysicsComponent>();
	AddComponent<StaticMeshComponent>();
}

float rot = 0.0f;

void Collectable::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);
}
