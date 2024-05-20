#include "floor.h"

#include "engine/components/mesh/staticMeshComponent.h"
#include "engine/components/physics/physicsComponent.h"
#include "engine/util/util.h"
#include "game/framework/pitfallGameInstance.h"

Floor::Floor()
{
	AddTag("Floor");
}

void Floor::Init()
{
	Actor::Init();
	AddComponent<PhysicsComponent>();
	AddComponent<StaticMeshComponent>();
	const auto model = Util::GetGameInstance()->GetModelManager()->GetModel("Floor");
	GetComponent<StaticMeshComponent>()->SetModel(model,
		Vector3(0, 0, 0));
	GetComponent<PhysicsComponent>()->SetGravity(0.f);
	GetComponent<PhysicsComponent>()->SetMass(0.0f);
}
