#pragma once

#include "Scene.h"
#include <nlohmann/json.hpp>
#include <SFML/System/Clock.hpp>

using json = nlohmann::json;

class GameScene : public Scene
{
public:
	void Init(GameEngine* engine) override;
	void Update(float dt) override;
	void Render() override;
	void HandleEvent(const sf::Event& event) override;
	void OnActivate() override;

	float GetLevelTime() const { return _levelTime; }
	void SetBestTime(float t) { _bestTime = t; }

private:
	void LoadLevelConfig(const std::string& path);
	void LoadGameConfig(const std::string& path);
	void CreatePlayer(const json& playerConfig, int gridX, int gridY);
	void CreateTile(int gridX, int gridY, const std::string& type);
	void CreateDecor(int gridX, int gridY, const std::string& name);
	void CreateFinish(int gridX, int gridY);
	void CreateGoomba(int gridX, int gridY, float patrolLeft, float patrolRight);
	void SetupInputActions();

	float _levelTime = 0.f;
	float _bestTime = 0.f;

	// Render mode: 0=textures, 1=colliders, 2=grid
	int _renderMode = 0;

	// Game config data
	float _playerSpeedX = 5.f;
	float _playerJumpY = 12.f;
	float _playerMaxSpeed = 15.f;
	float _playerGravity = 0.6f;
	sf::Vector2f _playerBBox = { 24.f, 32.f };
	std::string _playerBaseAnim;
	std::vector<std::string> _playerAnimations;

	float _bulletSpeedX = 10.f;
	float _bulletRadius = 4.f;

	bool _isPaused = false;

	sf::Clock _imguiDeltaClock;
};