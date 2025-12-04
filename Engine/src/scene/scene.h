#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

#include "graphics/skybox/skybox.h"

class Character;
class Actor;

class Scene
{
public:
	virtual ~Scene();
	virtual void Init();
	virtual void Tick(float deltaTime);
	virtual void Render();

	void AddActor(const std::string& name, Actor* actor);
	void RemoveActor(const std::string& name);
	Actor* GetActor(const std::string& name);
	std::unordered_map<std::string,Actor*> GetActors() const;
	std::vector<Actor*> GetActorsWithTag(const std::string& tag) const;

	Character* character = nullptr;

private:
	std::unordered_map<std::string,Actor*> actors = {};
	Skybox* skybox = nullptr;
	Shader* skyboxShader = nullptr;

protected:
	bool bLoadSkybox = false;
};

