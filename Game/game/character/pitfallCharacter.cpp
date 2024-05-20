#include "pitfallCharacter.h"

#include "game/actors/collectable.h"
#include "imgui.h"
#include "engine/components/controller/controllerComponent.h"
#include "engine/components/mesh/skeletalMeshComponent.h"
#include "engine/framework/gameInstance.h"
#include "engine/graphics/animator/animator.h"
#include "engine/managers/input/inputManager.h"
#include "engine/managers/scene/sceneManager.h"
#include "engine/managers/timer/timerManager.h"
#include "game/actors/enemy.h"
#include "game/scene/gameOver/gameOverScene.h"
#include "game/score/scoreManager.h"

PitfallCharacter::~PitfallCharacter()
{
	Util::GetGameInstance()->GetInputManager()->ClearBinding("MoveLeft", IE_Pressed);
	Util::GetGameInstance()->GetInputManager()->ClearBinding("MoveRight", IE_Pressed);
	Util::GetGameInstance()->GetInputManager()->ClearBinding("Jump", IE_Pressed);
	Util::GetGameInstance()->GetInputManager()->ClearBinding("Slide", IE_Pressed);
}

void PitfallCharacter::Init()
{
	Character::Init();
	GetComponent<SkeletalMeshComponent>()->animator->onAnimationFinished = [this] { this->HandleAnimationEnd(); };
}

void PitfallCharacter::Tick(float deltaTime)
{
	Character::Tick(deltaTime);
	GetComponent<ControllerComponent>()->Move(Vector3::Forward(),1.0f);
}

void PitfallCharacter::Render()
{
	Character::Render();

	const Vector3 position = GetComponent<ControllerComponent>()->GetTransform().getOrigin();
	float yaw, pitch, roll;
	GetComponent<ControllerComponent>()->GetTransform().getRotation().getEulerZYX(yaw, pitch, roll);
	yaw = btDegrees(yaw); pitch = btDegrees(pitch); roll = btDegrees(roll);
	float pos[3] = {position.x,position.y,position.z};
	float rot[3] = { roll,pitch,yaw };

	const ImGuiIO& io = ImGui::GetIO();

	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 450, 200), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(450, 175), ImGuiCond_Always);
	ImGui::Begin("Character Details");
	ImGui::InputFloat3("Position",pos);
	ImGui::InputFloat3("Rotation", rot);

	ImGui::SliderFloat("Walk Speed", &GetComponent<ControllerComponent>()->walkSpeed,0.1f,100);
	ImGui::SliderFloat("Jump Speed", &GetComponent<ControllerComponent>()->jumpSpeed,0.1f,10);
	ImGui::SliderFloat("Lane Change Speed", &GetComponent<ControllerComponent>()->laneChangeSpeed,1,100);
	ImGui::SliderInt("Lane", &GetComponent<ControllerComponent>()->currentLane,-1,1);
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x/2, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Always);
	ImGui::Begin("Button", nullptr,
	             ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
	             ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse |
	             ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);
	ImGui::Text("Score: %05d", score);
	ImGui::End();
}

void PitfallCharacter::SetupInputs(InputManager* manager)
{
	manager->BindAction("MoveRight",IE_Pressed, [this](const auto&& val) {this->MoveRight(val); });
	manager->BindAction("MoveLeft", IE_Pressed, [this] (const auto&& val) {this->MoveLeft(val); });
	manager->BindAction("Jump", IE_Pressed, [this](const auto&& val) {this->Jump(val); });
	manager->BindAction("Slide", IE_Pressed, [this](const auto&& val) {this->Slide(val); });
}

void PitfallCharacter::SetupAnimations(SkeletalMeshComponent* meshComponent)
{
	meshComponent->offset = glm::vec3(0.0f, -0.9f, 0.0f);
	meshComponent->SetModel(Util::GetGameInstance()->GetModelManager()->playerModel);
	meshComponent->AddAnimation("Game/assets/idle.dae","Idle");
	meshComponent->AddAnimation("Game/assets/run.dae","Run");
	meshComponent->AddAnimation("Game/assets/jump.dae","Jump");
	meshComponent->AddAnimation("Game/assets/slide.dae", "Slide");
	meshComponent->AddAnimation("Game/assets/death.dae","Death");
	meshComponent->AddAnimation("Game/assets/strafeLeft.dae","Left");
	meshComponent->AddAnimation("Game/assets/strafeRight.dae","Right");
	meshComponent->PlayAnimation("Run");
}

void PitfallCharacter::MoveLeft(void* val)
{
	if (!bDead)
	{
		auto currentLane = GetComponent<ControllerComponent>()->currentLane;
		currentLane++;
		if (currentLane <= 1)
		{
			GetComponent<SkeletalMeshComponent>()->PlayAnimation("Left",false, 1.5);
		}
		currentLane = glm::clamp(currentLane, -1, 1);
		GetComponent<ControllerComponent>()->SwitchLane(currentLane);
		GetComponent<ControllerComponent>()->ResetSlide();
	}
}

void PitfallCharacter::MoveRight(void* value)
{
	if (!bDead)
	{
		auto currentLane = GetComponent<ControllerComponent>()->currentLane;
		currentLane--;
		if (currentLane >= -1)
		{
			GetComponent<SkeletalMeshComponent>()->PlayAnimation("Right",false, 1.5);
		}
		currentLane = glm::clamp(currentLane, -1, 1);
		GetComponent<ControllerComponent>()->SwitchLane(currentLane);
		GetComponent<ControllerComponent>()->ResetSlide();
	}
}

void PitfallCharacter::Jump(void* value)
{
	if (!bDead)
	{
		GetComponent<ControllerComponent>()->Jump();
	}
}

void PitfallCharacter::Slide(void* value)
{
	if (!bDead)
	{
		GetComponent<ControllerComponent>()->Slide();
	}
}

void PitfallCharacter::HandleAnimationEnd()
{
	const auto currentAnim = GetComponent<SkeletalMeshComponent>()->GetCurrentAnimation();
	if (currentAnim == "Slide")
	{
		GetComponent<ControllerComponent>()->ResetSlide();
		GetComponent<SkeletalMeshComponent>()->PlayAnimation("Run");
	}
	if (currentAnim == "Left" || currentAnim == "Right" || currentAnim == "Jump")
	{
		GetComponent<SkeletalMeshComponent>()->PlayAnimation("Run");
	}
}

void PitfallCharacter::OnCollisionEnter(Actor* other)
{
	if (dynamic_cast<Enemy*>(other))
	{
		bDead = true;
		GetComponent<ControllerComponent>()->ResetSlide();
		GetComponent<SkeletalMeshComponent>()->PlayAnimation("Death",false);
		GetComponent<ControllerComponent>()->StopMovement();
		Util::GetGameInstance()->GetTimerManager()->CreateTimer("Die",2,false,[this]{this->Die();});
	}

	if (dynamic_cast<Collectable*>(other))
	{
		Util::GetGameInstance()->GetSceneManager()->GetCurrentScene()->RemoveActor(other->GetName());
		AddScore(50);
	}
}

void PitfallCharacter::OnCollisionStay(Actor* other)
{

}

void PitfallCharacter::Die()
{
	ScoreManager::SaveScore(score);
	Util::GetGameInstance()->GetSceneManager()->SetCurrentScene(std::make_unique<GameOverScene>());
}

void PitfallCharacter::AddScore(int value)
{
	score += value;
}

void PitfallCharacter::ResetScore()
{
	score = 0;
}

void PitfallCharacter::SetScore(int value)
{
	score = value;
}

int PitfallCharacter::GetScore() const
{
	return score;
}
