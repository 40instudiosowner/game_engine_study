#pragma once

#include "../ECS/Systems/IUpdateSystem.h"
#include "../Game/GameState.h"
#include <SFML/Graphics.hpp>

class AsteroidSpawnSystem final :
	public IUpdateSystem
{
public:

	explicit AsteroidSpawnSystem(sf::Vector2u windowSize, GameState& gameState);

	void Update(World& world, float dt) override;

private:
	GameState& _gameState;

	sf::Vector2u _windowSize;

	float _timer = 0.f;

	float _spawnDelay = 1.f;
};