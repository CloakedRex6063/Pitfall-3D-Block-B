#pragma once
#include "engine/actor/character/character.h"
#include "engine/framework/gameInstance.h"

class Actor;

class PitfallGameInstance : public GameInstance
{
public:
	PitfallGameInstance();
	~PitfallGameInstance() override;

	void Tick(float deltaTime) override;
	void Render() override;

	Character* character;
};

