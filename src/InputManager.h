#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

enum class ActionType
{
	Press,
	Release,
	Hold
};

struct ActionBinding
{
	sf::Keyboard::Key key = sf::Keyboard::Key::Unknown;
	sf::Mouse::Button mouseButton = sf::Mouse::Button::Middle;  // arbitrary fallback
	ActionType type = ActionType::Press;
	bool useMouse = false;
};

struct Action
{
	std::string name;
	std::function<void()> callback;
	ActionBinding binding;
	bool isActive = false;
};

class InputManager
{
public:
	void RegisterAction(
		const std::string& name,
		sf::Keyboard::Key key,
		ActionType type,
		std::function<void()> callback);

	void RegisterMouseAction(
		const std::string& name,
		sf::Mouse::Button button,
		ActionType type,
		std::function<void()> callback);

	void Update();

	void Clear();

private:
	std::unordered_map<std::string, Action> _actions;
	std::unordered_map<int, std::vector<std::string>> _keyMappings;
	std::unordered_map<int, std::vector<std::string>> _mouseMappings;

	// Track previous frame key states
	std::unordered_map<int, bool> _prevKeyStates;
	std::unordered_map<int, bool> _prevMouseStates;
};