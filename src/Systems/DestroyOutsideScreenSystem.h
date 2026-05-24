#pragma once

#include "../ECS/Systems/IUpdateSystem.h"

#include <SFML/System/Vector2.hpp>
#include "../Game/GameState.h"

class DestroyOutsideScreenSystem final :
	public IUpdateSystem
{
public:

	explicit DestroyOutsideScreenSystem(
		sf::Vector2u windowSize, GameState& gameState);

	void Update(
		World& world,
		float dt) override;

private:

	sf::Vector2u _windowSize;
	GameState& _gameState;
};