#include "pitfallGameInstance.h"
#include "engine/graphics/model/skeletalModel.h"
#include "engine/managers/input/inputManager.h"
#include "engine/managers/physics/physicsManager.h"
#include "engine/managers/scene/sceneManager.h"
#include "game/scene/mainMenu/mainMenuScene.h"
#include "game/score/scoreManager.h"

PitfallGameInstance::PitfallGameInstance()
{
	InputAction debugPhysics(IT_Button);
	debugPhysics.keys[KEY_P] = IM_None;
	GetInputManager()->AddAction("DebugPhysics",debugPhysics);
	GetInputManager()->BindAction("DebugPhysics", IE_Pressed, [this](const auto& val) {GetPhysicsManager()->ToggleDebugDraw(val); });

	InputAction moveLeft(IT_Button);
	moveLeft.keys[KEY_A] = IM_None;
	GetInputManager()->AddAction("MoveLeft",moveLeft);

	InputAction moveRight(IT_Button);
	moveRight.keys[KEY_D] = IM_None;
	GetInputManager()->AddAction("MoveRight",moveRight);

	InputAction jump(IT_Button);
	jump.keys[KEY_Space] = IM_None;
	GetInputManager()->AddAction("Jump",jump);

	InputAction slide(IT_Button);
	slide.keys[KEY_Ctrl_Left] = IM_None;
	GetInputManager()->AddAction("Slide", slide);

	GetModelManager()->AddModel("Rock", "Game/assets/Rock.obj",Vector3(0.6f),Vector3(),Vector3(),Vector2(1,1));
	GetModelManager()->AddModel("Coin", "Game/assets/coin.obj",Vector3(0.5f), Vector3(),Vector3(),Vector2(1,1));
	GetModelManager()->AddModel("Floor", "Game/assets/Floor.obj", Vector3(1.f), Vector3(), Vector3(), Vector2(0.3f,3.f));

	GetModelManager()->playerModel = new SkeletalModel("Game/assets/player.dae");

	ScoreManager::LoadScores();
	GetSceneManager()->SetCurrentScene(std::make_unique<MainMenuScene>());
}

PitfallGameInstance::~PitfallGameInstance()
{

}

void PitfallGameInstance::Tick(float deltaTime)
{
	GameInstance::Tick(deltaTime);
}

void PitfallGameInstance::Render()
{
	GameInstance::Render();
}
