#include "GameOverSystem.h"

#include <SFML/Window/Keyboard.hpp>

GameOverSystem::GameOverSystem(
	GameState& gameState)
	:
	_gameState(gameState)
{
}

void GameOverSystem::Update(
	World&,
	float)
{
	if (!_gameState.isGameOver)
		return;

	if (sf::Keyboard::isKeyPressed(
		sf::Keyboard::Key::Enter))
	{
		_gameState.isGameOver = false;

		_gameState.score = 0;
	}
}