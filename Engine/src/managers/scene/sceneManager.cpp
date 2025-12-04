#include "sceneManager.h"
#include <memory>

void SceneManager::SetCurrentScene(std::unique_ptr<Scene> scene)
{
	currentScene = std::move(scene);
	currentScene->Init();
}

Scene* SceneManager::GetCurrentScene() const
{
	return currentScene.get();
}

void SceneManager::Tick(float deltaTime) const
{
	currentScene->Tick(deltaTime);
}

void SceneManager::Render() const
{
	currentScene->Render();
}
