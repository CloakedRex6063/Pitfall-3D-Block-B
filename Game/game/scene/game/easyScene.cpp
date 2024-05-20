#include "easyScene.h"
#include "imgui.h"
#include "engine/components/controller/controllerComponent.h"
#include "engine/components/mesh/staticMeshComponent.h"
#include "engine/components/physics/physicsComponent.h"
#include "engine/framework/gameInstance.h"
#include "engine/managers/physics/physicsManager.h"
#include "engine/managers/scene/sceneManager.h"
#include "engine/util/util.h"
#include "game/actors/floor.h"
#include "game/character/pitfallCharacter.h"
#include "game/procGen/proceduralGenerator.h"
#include "game/scene/load/loadScene.h"

void EasyScene::Init()
{
	bLoadSkybox = true;
	Scene::Init();

	AddActor("Floor", new Floor());
	AddActor("Character",new PitfallCharacter());

	proceduralGenerator = new ProceduralGenerator(this,0.1f,0.4f,10);
	proceduralGenerator->GenerateEnemies(80, 200);
	proceduralGenerator->GenerateCoins(30, 200);
}

void EasyScene::Tick(float deltaTime)
{
	Scene::Tick(deltaTime);
	const auto floorPos = GetActor("Floor")->GetComponent<PhysicsComponent>()->GetPosition();
	if (character->GetPosition().z > floorPos.z + 100.f)
	{
		character->GetComponent<ControllerComponent>()->walkSpeed += 1.f;
		character->GetComponent<ControllerComponent>()->walkSpeed = std::min(character->GetComponent<ControllerComponent>()->walkSpeed, maxSpeed);
		GetActor("Floor")->GetComponent<PhysicsComponent>()->SetPosition(floorPos + Vector3(0, 0, 200.f));
		proceduralGenerator->RemoveActors(0, character->GetPosition().z - 50.f);
		proceduralGenerator->GenerateEnemies(character->GetPosition().z + 100.f, character->GetPosition().z + 300.f);
		proceduralGenerator->GenerateCoins(character->GetPosition().z + 100.f, character->GetPosition().z + 300.f);
	}
}

void EasyScene::Render()
{
	Scene::Render();

	constexpr int flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;
	const auto io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 225, 25));
	ImGui::SetNextWindowSize(ImVec2(200,100));
	ImGui::Begin("Main Menu",nullptr,flags);
	if (ImGui::Button("Main Menu",ImVec2(200,50)))
	{
		Util::GetGameInstance()->GetSceneManager()->SetCurrentScene(std::make_unique<LoadScene>());
		const auto loadScene = dynamic_cast<LoadScene*>(Util::GetGameInstance()->GetSceneManager()->GetCurrentScene());
		if (loadScene)
		{
			loadScene->type = MainMenu;
		}
	}
	ImGui::End();
}
