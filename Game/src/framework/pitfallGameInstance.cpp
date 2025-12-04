#include "pitfallGameInstance.h"
#include "graphics/model/skeletalModel.h"
#include "managers/input/inputManager.h"
#include "managers/physics/physicsManager.h"
#include "managers/scene/sceneManager.h"
#include "scene/mainMenu/mainMenuScene.h"
#include "score/scoreManager.h"

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

	GetModelManager()->AddModel("Rock", "assets/Rock.obj",Vector3(0.6f),Vector3(),Vector3(),Vector2(1,1));
	GetModelManager()->AddModel("Coin", "assets/coin.obj",Vector3(0.5f), Vector3(),Vector3(),Vector2(1,1));
	GetModelManager()->AddModel("Floor", "assets/Floor.obj", Vector3(1.f), Vector3(), Vector3(), Vector2(0.3f,3.f));

	GetModelManager()->playerModel = new SkeletalModel("assets/player.dae");

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
