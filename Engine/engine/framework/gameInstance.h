#pragma once
#include "engine/managers/model/modelManager.h"

class TimerManager;
class PhysicsManager;
class SceneManager;
class InputManager;

class GameInstance
{
public:
	GameInstance();
	virtual ~GameInstance();

	virtual void Tick(float deltaTime);
	virtual void Render();

	InputManager* GetInputManager() const;
	SceneManager* GetSceneManager() const;
	PhysicsManager* GetPhysicsManager() const;
	TimerManager* GetTimerManager() const;
	ModelManager* GetModelManager() const;

private:
	InputManager* inputManager;
	SceneManager* sceneManager;
	PhysicsManager* physicsManager;
	TimerManager* timerManager;
	ModelManager* modelManager;
};

