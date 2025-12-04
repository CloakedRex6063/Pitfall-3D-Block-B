#pragma once
#include <memory>
#include "scene/scene.h"

class SceneManager
{
public:
	void SetCurrentScene(std::unique_ptr<Scene> scene);
	Scene* GetCurrentScene() const;

	void Tick(float deltaTime) const;
	void Render() const;

private:
	std::unique_ptr<Scene> currentScene = {};
};

