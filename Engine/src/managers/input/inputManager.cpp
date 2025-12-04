#include "inputManager.h"
#include "utility"
#include "math/vector2.h"

void InputManager::Tick()
{
	for (auto& binding : bindings)
	{
		const auto action = actions.find(binding.name);
		if (action == actions.end()) continue;
		InputModifier modifier;
		switch (binding.event)
		{
		case IE_Pressed:
			if (IsPressed(binding.name, modifier) && binding.pressedCallback)
			{
				if (action->second.type == IT_Button)
				{
					binding.pressedCallback(nullptr);
				}
			}

			break;
		case IE_Held:
			if (IsHeld(binding.name, modifier) && binding.heldCallback)
			{
				if (action->second.type == IT_Button)
				{
					binding.heldCallback(nullptr);
				}
			}
			break;
		case IE_Released:
			if (IsReleased(binding.name, modifier) && binding.releasedCallback)
			{
				if (action->second.type == IT_Button)
				{
					binding.releasedCallback(nullptr);
				}
			}
			break;
		case IE_Axis:
			for (auto& key : action->second.keys)
			{
				if (keyStates[key.first])
				{
					if (action->second.type == IT_Axis)
					{
						delete static_cast<float*>(binding.value);
						binding.value = new float(key.second == IM_Negate ? -1.f : 1.f);
					}
					break;
				}
				if (action->second.type == IT_Axis)
				{
					delete static_cast<float*>(binding.value);
					binding.value = new float(0.f);
				}
			}
			if (binding.axisCallback)
			{
				binding.axisCallback(binding.value);
			}
			break;
		}
	}
}

bool InputManager::IsPressed(const std::string& name, InputModifier& modifier)
{
	static std::unordered_map<Keys, bool> prevKeyStates;
	const auto& action = actions.find(name);

	if (action == actions.end())
	{
		modifier = IM_None;
		return false;
	}

	for (const auto& key : action->second.keys)
	{
		if (keyStates[key.first] && !prevKeyStates[key.first])
		{
			modifier = key.second;
			prevKeyStates[key.first] = true;
			return true;
		}
		prevKeyStates[key.first] = keyStates[key.first];
	}
	modifier = IM_None;
	return false;
}

bool InputManager::IsHeld(const std::string& name, InputModifier& modifier)
{
	const auto& action = actions.find(name);

	if (action == actions.end())
	{
		modifier = IM_None;
		return false;
	}

	for (const auto& key : action->second.keys)
	{
		if (keyStates[key.first])
		{
			modifier = key.second;
			return true;
		}
	}
	modifier = IM_None;
	return false;
}

bool InputManager::IsReleased(const std::string& name, InputModifier& modifier)
{
	static std::unordered_map<Keys, bool> prevKeyStates;
	const auto& action = actions.find(name);

	if (action == actions.end())
	{
		modifier = IM_None;
		return false;
	}

	for (const auto& key : action->second.keys)
	{
		if (!keyStates[key.first] && prevKeyStates[key.first])
		{
			modifier = key.second;
			prevKeyStates[key.first] = false;
			return true;
		}
		prevKeyStates[key.first] = keyStates[key.first];
	}
	modifier = IM_None;
	return false;
}


void InputManager::AddAction(const std::string& name, const InputAction& action)
{
	actions.insert(std::make_pair(name, action));
}

void InputManager::BindAction(const std::string& name, InputEvent event, const std::function<void(void*)>& callback)
{
	InputBinding binding;
	binding.name = name;
	binding.event = event;
	binding.value = nullptr;

	const auto& action = actions.find(name);
	if (action == actions.end()) return;

	switch (event)
	{
	case IE_Pressed:
		binding.pressedCallback = callback;
		break;
	case IE_Held:
		binding.heldCallback = callback;
		break;
	case IE_Released:
		binding.releasedCallback = callback;
		break;
	case IE_Axis:
		break;
	}

	bindings.push_back(binding);
}

void InputManager::BindAxis(const std::string& name, const std::function<void(void*)>& callback)
{
	InputBinding binding;
	binding.name = name;
	binding.event = IE_Axis;
	binding.value = new float(0.f);
	binding.axisCallback = callback;
	bindings.push_back(binding);
}

void InputManager::ClearBinding(const std::string& name, InputEvent event)
{
	for (auto it = bindings.begin(); it != bindings.end();)
	{
		if (it->name == name && it->event == event)
		{
			it = bindings.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void* InputManager::GetInputValue(const std::string& name) const
{
	for(const auto& binding : bindings)
	{
		if (binding.name == name && binding.value)
		{
			return binding.value;
		}
	}
	return nullptr;
}
