#pragma once

#include "../ECS/Systems/SystemManager.h"
#include "../ECS/World.h"
#include "../Game/GameState.h"
#include "../InputManager.h"
#include <SFML/Window/Event.hpp>

class GameEngine;

class Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;

	virtual void Init(GameEngine* engine) = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	virtual void HandleEvent(const sf::Event& event) = 0;
	virtual void OnActivate() {}
	virtual void OnDeactivate() {}

	SystemManager& GetSystems() { return _systems; }
	World& GetWorld() { return _world; }
	GameState& GetGameState() { return _gameState; }

protected:
	SystemManager _systems;
	World _world;
	GameState _gameState;
	GameEngine* _engine = nullptr;
};