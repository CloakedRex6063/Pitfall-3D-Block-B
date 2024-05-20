#pragma once
#include "engine/scene/scene.h"
#include "string"

class SkeletalModel;
class Animator;

class MainMenuScene : public Scene
{
public:
	~MainMenuScene() override;
	void Init() override;
	void Tick(float deltaTime) override;
	void Render() override;

private:
	SkeletalModel* model = nullptr;
	Animator* animator = nullptr;
	std::string name1 = "";
	std::string name2 = "";
	std::string name3 = "";
};

