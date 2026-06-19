#include "GameEngine.h"
#include "Scenes/Scene.h"

#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>

GameEngine::GameEngine(unsigned int width, unsigned int height)
{
	_window.create(
		sf::VideoMode({ width, height }),
		"Mega Man - ECS Game Engine");

	auto desktop = sf::VideoMode::getDesktopMode();
	_window.setPosition({
		static_cast<int>(desktop.size.x / 2 - width / 2),
		static_cast<int>(desktop.size.y / 2 - height / 2)
	});
	_window.setFramerateLimit(60);

	// Load assets
	if (!_assets.LoadAssets("assets/configs/assets.json"))
	{
		std::cerr << "Failed to load assets\n";
	}

	if (!ImGui::SFML::Init(_window))
		std::cerr << "ImGui-SFML init failed\n";

	std::cout << "GameEngine initialized\n";
}

GameEngine::~GameEngine()
{
	ImGui::SFML::Shutdown();
	_window.close();
}

void GameEngine::Run()
{
	while (_isRunning)
	{
		float dt = _deltaClock.restart().asSeconds();
		// Clamp dt to avoid physics explosions when window is dragged or focus changes
		if (dt > 0.05f)
			dt = 0.05f;

		ProcessEvents();
		Update(dt);
		Render();
	}
}

void GameEngine::Stop()
{
	_isRunning = false;
}

void GameEngine::ProcessEvents()
{
	while (const std::optional event = _window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			_isRunning = false;
			return;
		}

		if (_currentScene)
		{
			_currentScene->HandleEvent(*event);
		}
	}

	// Update input manager
	_inputManager.Update();
}

void GameEngine::Update(float dt)
{
	// Scene switching
	if (!_nextSceneName.empty())
	{
		auto it = _scenes.find(_nextSceneName);
		if (it != _scenes.end())
		{
			if (_currentScene)
				_currentScene->OnDeactivate();

			_currentScene = it->second.get();
			_currentScene->OnActivate();
		}
		_nextSceneName.clear();
	}

	if (_currentScene)
	{
		_currentScene->Update(dt);
	}
}

void GameEngine::Render()
{
	_window.clear();

	if (_currentScene)
	{
		_currentScene->Render();
	}

	_window.display();
}

void GameEngine::ChangeScene(const std::string& name)
{
	_nextSceneName = name;
}

void GameEngine::RegisterScene(const std::string& name, std::unique_ptr<Scene> scene)
{
	scene->Init(this);
	_scenes[name] = std::move(scene);
}