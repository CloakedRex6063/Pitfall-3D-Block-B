#pragma once
#include "actor/character/character.h"
#include "framework/gameInstance.h"

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

