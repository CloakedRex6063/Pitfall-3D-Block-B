#include "gameOverScene.h"
#include <memory>
#include "imgui.h"
#include "engine/framework/gameInstance.h"
#include "engine/graphics/animation/animation.h"
#include "engine/graphics/animator/animator.h"
#include "engine/managers/scene/sceneManager.h"
#include "engine/util/util.h"
#include "game/scene/load/loadScene.h"
#include "game/score/scoreManager.h"


void GameOverScene::Init()
{
	Scene::Init();
	model = Util::GetGameInstance()->GetModelManager()->playerModel;
	animator = new Animator();
	animator->AddAnimation(new Animation("Game/assets/crying.dae",model),"Cry");
	animator->PlayAnimation("Cry",true,1,false,0);

	newHighScore = ScoreManager::GetNewHighScore();
}

void GameOverScene::Tick(float deltaTime)
{
	Scene::Tick(deltaTime);
	if (animator)
	{
		animator->UpdateAnimation(deltaTime);
	}

	constexpr int flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;

	ImGui::SetNextWindowPos(ImVec2(540, 100));
	ImGui::SetNextWindowSize(ImVec2(200,200));
	ImGui::Begin("Score",nullptr,flags);
	const std::string score = (newHighScore ? "New High Score:" : "Score: ") + std::to_string(ScoreManager::GetScore());
	ImGui::Button("Game Over",ImVec2(200,50));
	ImGui::Button(score.c_str(),ImVec2(200,50));
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(540, 600));
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

void GameOverScene::Render()
{
	Scene::Render();

	if (animator)
	{
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
}
