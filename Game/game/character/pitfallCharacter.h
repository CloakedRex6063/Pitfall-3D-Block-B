#pragma once
#include "engine/actor/character/character.h"

class PitfallCharacter : public Character
{
public:
	~PitfallCharacter() override;
	void Init() override;
	void Tick(float deltaTime) override;
	void Render() override;

	void SetupInputs(InputManager* manager) override;
	void SetupAnimations(SkeletalMeshComponent* meshComponent) override;

	void MoveLeft(void* val);
	void MoveRight(void* value);

	void Jump(void* value);
	void Slide(void* value);

	void HandleAnimationEnd();

	void OnCollisionEnter(Actor* other) override;
	void OnCollisionStay(Actor* other) override;

	void Die();

	void AddScore(int value);
	void ResetScore();
	void SetScore(int value);
	int GetScore() const;

private:
	int score = 0;
	bool bDead = false;

};

