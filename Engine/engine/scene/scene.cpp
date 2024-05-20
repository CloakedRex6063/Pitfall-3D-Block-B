#include "scene.h"
#include "engine/actor/character/character.h"
#include "engine/camera/camera.h"
#include "engine/components/mesh/staticMeshComponent.h"
#include "engine/framework/gameInstance.h"
#include "engine/graphics/shader/shader.h"
#include "engine/math/vector3.h"
#include "engine/util/util.h"

Scene::~Scene()
{
	for (const auto& actor : actors)
	{
		delete actor.second;
	}
	actors.clear();
	delete skybox;
	delete skyboxShader;
}

void Scene::Init()
{
	skyboxShader = new Shader("Linux/shaders/skybox.vert", "Linux/shaders/skybox.frag");
	skybox = new Skybox(skyboxShader);
}

void Scene::Tick(float deltaTime)
{
	for (const auto& actor : actors)
	{
		actor.second->Tick(deltaTime);
	}
	if (character)
	{
		Util::GetCamera()->FollowTarget(Vector3(character->GetPosition().x,0, character->GetPosition().z),Vector3(0,4,-10));
	}
	else
	{
		Util::GetCamera()->FollowTarget(Vector3(0,0,0),Vector3(0,1,-5));
	}
}

void Scene::Render()
{
	if (bLoadSkybox)
	{
		skybox->Draw();
	}
	Util::GetBasicShader()->Bind();
	for (const auto& actor : actors)
	{
		actor.second->Render();
	}
}

void Scene::AddActor(const std::string& name, Actor* actor)
{
	if (actor->HasTag("Character"))
	{
		character = dynamic_cast<Character*>(actor);
	}
	actors.insert(std::pair(name,actor));
	actor->Init();
	actor->SetName(name);
}

Actor* Scene::GetActor(const std::string& name)
{
	return actors[name];
}

std::unordered_map<std::string,Actor*> Scene::GetActors() const
{
	return actors;
}

std::vector<Actor*> Scene::GetActorsWithTag(const std::string& tag) const
{
	std::vector<Actor*> actorsWithTag;
	for (const auto& actor : actors)
	{
		if (actor.second->HasTag(tag))
		{
			actorsWithTag.push_back(actor.second);
		}
	}
	return actorsWithTag;
}

void Scene::RemoveActor(const std::string& name)
{
	delete actors[name];
	actors.erase(name);
}
