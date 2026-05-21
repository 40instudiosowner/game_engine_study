#pragma once

#include <SFML/Graphics.hpp>

#include <memory>

#include "ConfigReader.h"

#include "ECS/World.h"
#include "ECS/Systems/SystemManager.h"
#include "Game/GameState.h"

class UISystem;

class Window
{
public:

	~Window();

	Window(
		unsigned int width,
		unsigned int height,
		ConfigReader& config);

	void Run();

	void RestartGame();

private:

	void Initialize();

	void CreatePlayerEntity();

private:

	sf::RenderWindow _window;

	bool _isRun = true;

	std::unique_ptr<ConfigReader> _config;

	World _world;

	SystemManager _systems;

	std::unique_ptr<UISystem> _uiSystem;

	GameState _gameState;

	std::vector<std::unique_ptr<sf::Texture>> _textures;
};