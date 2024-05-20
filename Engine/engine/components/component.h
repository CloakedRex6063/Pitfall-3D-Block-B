#pragma once

class Actor;

class Component
{
public:
	Component(Actor* owner);
	virtual ~Component();

	virtual void Init() = 0;
	virtual void Tick(float deltaTime) = 0;
	virtual void Render() = 0;

	Actor* GetOwner() const;
private:
	Actor* owner;
};

