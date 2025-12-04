#include "collisionComponent.h"
WARNING_ENABLE
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
WARNING_DISABLE
#include "actor/actor.h"
#include "components/controller/controllerComponent.h"
#include "components/physics/physicsComponent.h"
#include "algorithm"

CollisionComponent::CollisionComponent(Actor* owner) : Component(owner)
{
}

CollisionComponent::~CollisionComponent()
{
}

void CollisionComponent::Init()
{
	if (GetOwner()->GetComponent<ControllerComponent>())
	{
		collisionObject = GetOwner()->GetComponent<ControllerComponent>()->GetGhostObject();
	}
	else if (GetOwner()->GetComponent<PhysicsComponent>()->GetBody())
	{
		collisionObject = GetOwner()->GetComponent<PhysicsComponent>()->GetBody();
	}
	else
	{
		collisionObject = nullptr;
	}
}

void CollisionComponent::Tick(float dt)
{
	if (collisionObject)
	{
		const auto ghostObject = dynamic_cast<btGhostObject*>(collisionObject);
		if(ghostObject->getNumOverlappingObjects() > 0)
		{
			for (int i = 0; i < ghostObject->getNumOverlappingObjects(); i++)
			{
				btCollisionObject* overlappingObject = ghostObject->getOverlappingObject(i);
				const auto actor = static_cast<Actor*>(overlappingObject->getUserPointer());
				// Get the actor associated with the object
				if (actor)
				{
					// Call the actor's OnCollisionEnter
					GetOwner()->OnCollisionStay(actor);
				}
				else
				{
					GetOwner()->OnCollisionStay(nullptr);
				}
				if (std::find(overlappingObjects.begin(), overlappingObjects.end(), overlappingObject) == overlappingObjects.end())
				{
					if (actor)
					{
						GetOwner()->OnCollisionEnter(actor);
					}
					else
					{
						GetOwner()->OnCollisionEnter(nullptr);
					}
				}
			}
		}

		overlappingObjects.clear();
		for (int i = 0; i < ghostObject->getNumOverlappingObjects(); i++)
		{
			btCollisionObject* overlappingObject = ghostObject->getOverlappingObject(i);
			overlappingObjects.push_back(overlappingObject);
		}
	}
}

void CollisionComponent::Render()
{

}
