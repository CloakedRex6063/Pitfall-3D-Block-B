#pragma once
#include "engine/scene/scene.h"

class Animator;
class SkeletalModel;

class GameOverScene : public Scene
{
public:
	void Init() override;
	void Tick(float deltaTime) override;
	void Render() override;

private:
	SkeletalModel* model = nullptr;
	Animator* animator = nullptr;

	bool newHighScore = false;
};

