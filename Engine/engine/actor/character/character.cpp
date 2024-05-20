#include "character.h"
#include "engine/components/collision/collisionComponent.h"
#include "engine/components/controller/controllerComponent.h"
#include "engine/components/mesh/skeletalMeshComponent.h"
#include "engine/framework/gameInstance.h"
#include "engine/util/util.h"

Character::Character()
{
	AddTag("Character");
}

Character::~Character()
{
}

void Character::Init()
{
	Actor::Init();
	AddComponent<SkeletalMeshComponent>();
	SetupAnimations(GetComponent<SkeletalMeshComponent>());
	SetupInputs(Util::GetGameInstance()->GetInputManager());
	AddComponent<ControllerComponent>();
	AddComponent<CollisionComponent>();
}

void Character::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);
}

void Character::Render()
{
	Actor::Render();
}

Vector3 Character::GetPosition() 
{
	return GetComponent<ControllerComponent>()->GetTransform().getOrigin();
}
