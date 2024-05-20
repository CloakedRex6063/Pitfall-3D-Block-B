#pragma once
#include "engine/actor/actor.h"

class Collectable : public Actor
{
public:
	Collectable();
	void Init() override;
	void Tick(float deltaTime) override;
};

