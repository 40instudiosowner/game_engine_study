#pragma once

#include <SFML/System/Clock.hpp>
#include "../Game/GameState.h"

class World;

namespace sf
{
	class RenderWindow;
}

class UISystem
{
public:

	explicit UISystem(sf::RenderWindow& window, GameState& gameState);

	void Update(
		World& world,
		bool& isRun);

	void Shutdown();

	void Render(sf::RenderWindow& window);

private:

	void UpdateInput(World& world, bool& isRun);

	void UpdateGui(World& world);

private:

	sf::RenderWindow& _window;

	sf::Clock _deltaClock;
	GameState& _gameState;
};