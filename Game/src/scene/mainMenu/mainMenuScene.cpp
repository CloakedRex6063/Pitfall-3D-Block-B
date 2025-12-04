#include "mainMenuScene.h"
#include "imgui.h"
#include "framework/gameInstance.h"
#include "graphics/animator/animator.h"
#include "graphics/model/skeletalModel.h"
#include "managers/scene/sceneManager.h"
#include "managers/timer/timerManager.h"
#include "util/util.h"
#include "procGen/randomName.h"
#include "scene/load/loadScene.h"
#include "score/scoreManager.h"

void MainMenuScene::Init()
{
	Scene::Init();
	model = Util::GetGameInstance()->GetModelManager()->playerModel;
	animator = new Animator();
	animator->AddAnimation(new Animation("assets/dance.dae",model),"Dance");
	animator->PlayAnimation("Dance",true,1,false,0);
	constexpr RandomName name;
	name1 = name.GenerateRandomName(10);
	name2 = name.GenerateRandomName(6);
	name3 = name.GenerateRandomName(8);
	cout << name1 << endl;
}

void MainMenuScene::Tick(float deltaTime)
{
	Scene::Tick(deltaTime);
	animator->UpdateAnimation(deltaTime);

	constexpr int flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;

	ImGui::SetNextWindowPos(ImVec2(540, 100));
	ImGui::SetNextWindowSize(ImVec2(200,500));
	ImGui::Begin("Pitfall Text",nullptr,flags);
	ImGui::Button("Pitfall3D",ImVec2(200,50));
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 200, 100));
	ImGui::SetNextWindowSize(ImVec2(200,500));
	ImGui::Begin("Pitfall Score",nullptr,flags);
	const int score = ScoreManager::GetScore();
	const int highScore = ScoreManager::GetHighScore();
	const int totalScore = ScoreManager::GetTotalScore();
	ImGui::Text("Score: %05d", score);
	ImGui::Text("High Score: %05d", highScore);
	ImGui::Text("Total Score: %05d", totalScore);
	ImGui::End();


	ImGui::SetNextWindowPos(ImVec2(240, 600));
	ImGui::SetNextWindowSize(ImVec2(800,100));
	ImGui::Begin("Main Menu",nullptr,flags);
	if (ImGui::Button("Easy",ImVec2(200,50)))
	{
		Util::GetGameInstance()->GetSceneManager()->SetCurrentScene(std::make_unique<LoadScene>());
		const auto loadScene = dynamic_cast<LoadScene*>(Util::GetGameInstance()->GetSceneManager()->GetCurrentScene());
		if (loadScene)
		{
			loadScene->type = Easy;
		}
	}
	ImGui::SameLine(0, 100);
	if (ImGui::Button("Medium",ImVec2(200,50)))
	{
		Util::GetGameInstance()->GetSceneManager()->SetCurrentScene(std::make_unique<LoadScene>());
		const auto loadScene = dynamic_cast<LoadScene*>(Util::GetGameInstance()->GetSceneManager()->GetCurrentScene());
		if (loadScene)
		{
			loadScene->type = Medium;
		}
	}
	ImGui::SameLine(0, 100);
	if (ImGui::Button("Hard",ImVec2(200,50)))
	{
		Util::GetGameInstance()->GetSceneManager()->SetCurrentScene(std::make_unique<LoadScene>());
		const auto loadScene = dynamic_cast<LoadScene*>(Util::GetGameInstance()->GetSceneManager()->GetCurrentScene());
		if (loadScene)
		{
			loadScene->type = Hard;
		}
	}
	ImGui::End();
}

void MainMenuScene::Render()
{
	Scene::Render();

	const auto transforms = animator->GetFinalBoneMatrices();
	for (unsigned i = 0; i < transforms.size(); ++i)
	{
		std::string name = "finalBonesMatrices[" + std::to_string(i) + "]";
		Util::GetBasicShader()->SetMat4(name.c_str(), transforms[i]);
	}

	auto modelMat = glm::mat4(1.0f);
	modelMat = glm::rotate(modelMat,glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
	Util::GetBasicShader()->SetMat4("model", modelMat);
	model->Draw();
}

MainMenuScene::~MainMenuScene()
{
	delete animator;
}
