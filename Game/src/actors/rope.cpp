#include "rope.h"

#include "components/mesh/staticMeshComponent.h"
#include "components/physics/physicsComponent.h"

void Rope::Init()
{
	Actor::Init();
	AddComponent<PhysicsComponent>();
	AddComponent<StaticMeshComponent>();

}
