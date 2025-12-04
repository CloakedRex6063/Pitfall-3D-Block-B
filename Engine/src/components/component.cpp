#include "component.h"

Component::Component(Actor* owner): owner(owner)
{
	
}

Component::~Component()
{
	
}

Actor* Component::GetOwner() const
{
	return owner;
}
