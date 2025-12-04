#pragma once

#include "string"
#include "vector"
#include "components/component.h"

class Actor
{
public:
	Actor();
	virtual ~Actor();

	virtual void Init() {}
	virtual void Tick(float deltaTime);
	virtual void Render();
	virtual void OnCollisionEnter(Actor* other) {}
	virtual void OnCollisionStay(Actor* other) {}

	std::string GetName();
	void SetName(const std::string& name);

	void AddTag(const std::string& tag);
	void RemoveTag(const std::string& tag);
	bool HasTag(const std::string& tag);

	template <typename T>
	void AddComponent()
	{
		T* component = new T(this);
		dynamic_cast<Component*>(component)->Init();
		components.push_back(component);
	}

	template <typename T>
	T* GetComponent()
	{
		for (auto& component : components)
		{
			if (dynamic_cast<T*>(component))
			{
				return dynamic_cast<T*>(component);
			}
		}
		return nullptr;
	}

private:
	std::vector<std::string> tags;
	std::vector<Component*> components;
	std::string name;
};

