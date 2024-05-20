#include "rope.h"

#include "engine/components/mesh/staticMeshComponent.h"
#include "engine/components/physics/physicsComponent.h"

void Rope::Init()
{
	Actor::Init();
	AddComponent<PhysicsComponent>();
	AddComponent<StaticMeshComponent>();

}
