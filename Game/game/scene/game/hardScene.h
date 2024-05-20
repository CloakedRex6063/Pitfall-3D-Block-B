#pragma once
#include "engine/scene/scene.h"

class StaticModel;
class ProceduralGenerator;
class btRigidBody;

class HardScene : public Scene
{
public:
	void Init() override;
	void Tick(float deltaTime) override;
	void Render() override;

	ProceduralGenerator* proceduralGenerator = nullptr;

	float maxSpeed = 50.f;
};

