#pragma once
#include "scene/scene.h"

class StaticModel;
class ProceduralGenerator;
class btRigidBody;

class EasyScene : public Scene
{
public:
	void Init() override;
	void Tick(float deltaTime) override;
	void Render() override;

	ProceduralGenerator* proceduralGenerator = nullptr;

	float maxSpeed = 35.f;
};

