#pragma once
#include "engine/scene/scene.h"
#include <memory>

enum SceneType
{
	Easy,
	Medium,
	Hard,
	MainMenu
};

class LoadScene : public Scene
{
public:
	void Init() override;
	void Render() override;
	SceneType type;

private:
	bool bIsLoaded = false;
};

