#include "gameInstance.h"
#include "managers/input/inputManager.h"
#include "managers/model/modelManager.h"
#include "managers/physics/physicsManager.h"
#include "managers/scene/sceneManager.h"
#include "managers/timer/timerManager.h"
#include "util/util.h"

GameInstance::GameInstance()
{
	Util::SetGameInstance(this);
	inputManager = new InputManager();
	physicsManager = new PhysicsManager();
	sceneManager = new SceneManager();
	timerManager = new TimerManager();
	modelManager = new ModelManager();
}

GameInstance::~GameInstance()
{
	delete inputManager;
	delete sceneManager;
	delete physicsManager;
	delete modelManager;
}

void GameInstance::Tick(float deltaTime)
{
	inputManager->Tick();
	sceneManager->Tick(deltaTime);
	physicsManager->Tick(deltaTime);
	timerManager->Tick(deltaTime);
}

void GameInstance::Render()
{
	sceneManager->Render();
	physicsManager->Render();
	modelManager->Render();
}

InputManager* GameInstance::GetInputManager() const
{
	return inputManager;
}

SceneManager* GameInstance::GetSceneManager() const
{
	return sceneManager;
}

PhysicsManager* GameInstance::GetPhysicsManager() const
{
	return physicsManager;
}

TimerManager* GameInstance::GetTimerManager() const
{
	return timerManager;
}

ModelManager* GameInstance::GetModelManager() const
{
	return modelManager;
}
