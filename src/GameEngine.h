#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <unordered_map>

#include "AssetManager.h"
#include "InputManager.h"
#include "ECS/World.h"
#include "Game/GameState.h"

class Scene;

class GameEngine
{
public:
	GameEngine(unsigned int width, unsigned int height);
	~GameEngine();

	void Run();
	void Stop();

	void ChangeScene(const std::string& name);
	void RegisterScene(const std::string& name, std::unique_ptr<Scene> scene);

	sf::RenderWindow& GetWindow() { return _window; }
	AssetManager& GetAssets() { return _assets; }
	InputManager& GetInputManager() { return _inputManager; }
	const GameState& GetGameState() const { return _gameState; }

	void SetBestTime(float time) { _bestTime = time; }
	float GetBestTime() const { return _bestTime; }

private:
	void ProcessEvents();
	void Update(float dt);
	void Render();

	sf::RenderWindow _window;
	bool _isRunning = true;

	AssetManager _assets;
	InputManager _inputManager;
	GameState _gameState;

	float _bestTime = 0.f;

	std::unordered_map<std::string, std::unique_ptr<Scene>> _scenes;
	Scene* _currentScene = nullptr;
	std::string _nextSceneName;

	sf::Clock _deltaClock;
};