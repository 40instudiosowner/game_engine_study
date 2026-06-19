#include "InputManager.h"

void InputManager::RegisterAction(
	const std::string& name,
	sf::Keyboard::Key key,
	ActionType type,
	std::function<void()> callback)
{
	Action action;
	action.name = name;
	action.binding.key = key;
	action.binding.type = type;
	action.binding.useMouse = false;
	action.callback = callback;

	_actions[name] = action;
	_keyMappings[static_cast<int>(key)].push_back(name);
}

void InputManager::RegisterMouseAction(
	const std::string& name,
	sf::Mouse::Button button,
	ActionType type,
	std::function<void()> callback)
{
	Action action;
	action.name = name;
	action.binding.mouseButton = button;
	action.binding.type = type;
	action.binding.useMouse = true;
	action.callback = callback;

	_actions[name] = action;
	_mouseMappings[static_cast<int>(button)].push_back(name);
}

void InputManager::Update()
{
	// Reset per-frame flags
	for (auto& [name, action] : _actions)
	{
		action.justPressed = false;
		action.justReleased = false;
	}

	// Process keyboard
	for (auto& [key, actionNames] : _keyMappings)
	{
		sf::Keyboard::Key k = static_cast<sf::Keyboard::Key>(key);
		bool current = sf::Keyboard::isKeyPressed(k);
		bool previous = _prevKeyStates[key];

		for (const auto& name : actionNames)
		{
			auto& action = _actions[name];

			if (action.binding.type == ActionType::Press && current && !previous)
			{
				action.callback();
				action.justPressed = true;
			}
			else if (action.binding.type == ActionType::Release && !current && previous)
			{
				action.callback();
				action.justReleased = true;
			}
			else if (action.binding.type == ActionType::Hold && current)
			{
				action.callback();
			}

			action.isActive = current;
		}

		_prevKeyStates[key] = current;
	}

	// Process mouse
	for (auto& [button, actionNames] : _mouseMappings)
	{
		sf::Mouse::Button b = static_cast<sf::Mouse::Button>(button);
		bool current = sf::Mouse::isButtonPressed(b);
		bool previous = _prevMouseStates[button];

		for (const auto& name : actionNames)
		{
			auto& action = _actions[name];

			if (action.binding.type == ActionType::Press && current && !previous)
			{
				action.callback();
				action.justPressed = true;
			}
			else if (action.binding.type == ActionType::Release && !current && previous)
			{
				action.callback();
				action.justReleased = true;
			}
			else if (action.binding.type == ActionType::Hold && current)
			{
				action.callback();
			}

			action.isActive = current;
		}

		_prevMouseStates[button] = current;
	}
}

bool InputManager::IsActionActive(const std::string& name) const
{
	auto it = _actions.find(name);
	if (it != _actions.end())
		return it->second.isActive;
	return false;
}

bool InputManager::WasActionJustPressed(const std::string& name) const
{
	auto it = _actions.find(name);
	if (it != _actions.end())
		return it->second.justPressed;
	return false;
}

bool InputManager::WasActionJustReleased(const std::string& name) const
{
	auto it = _actions.find(name);
	if (it != _actions.end())
		return it->second.justReleased;
	return false;
}

void InputManager::Clear()
{
	_actions.clear();
	_keyMappings.clear();
	_mouseMappings.clear();
	_prevKeyStates.clear();
	_prevMouseStates.clear();
}