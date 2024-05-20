#pragma once
#include "engine/util/common.h"
WARNING_DISABLE
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
WARNING_ENABLE
#include "../component.h"
#include "functional"

class CollisionComponent : public Component
{
public:
	CollisionComponent(Actor* owner);
	~CollisionComponent() override;

	void Init() override;
	void Tick(float dt) override;
	void Render() override;

private:
	btCollisionObject* collisionObject;
	std::vector<btCollisionObject*> overlappingObjects;
};

