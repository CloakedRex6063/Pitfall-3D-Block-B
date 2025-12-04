#include "actor.h"
#include "algorithm"
#include "graphics/model/skeletalModel.h"

Actor::Actor()
{
}

Actor::~Actor()
{
	for (const auto& component : components)
	{
		delete component;
	}
	components.clear();
}

void Actor::Tick(float deltaTime)
{
	for (const auto& component : components)
	{
		component->Tick(deltaTime);
	}
}

void Actor::Render()
{
	for (const auto& component : components)
	{
		component->Render();
	}
}

std::string Actor::GetName()
{
	return name;
}

void Actor::SetName(const std::string& name)
{
	this->name = name;
}

void Actor::AddTag(const std::string& tag)
{
	tags.push_back(tag);
}

void Actor::RemoveTag(const std::string& tag)
{
	const auto it = std::find(tags.begin(), tags.end(), tag);
	if (it != tags.end())
	{
		tags.erase(it); 
	}
}

bool Actor::HasTag(const std::string& tag)
{
	const auto it = std::find(tags.begin(), tags.end(), tag);
	return (it != tags.end());
}
