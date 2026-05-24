#include "GameOverSystem.h"
#include "../Components/AsteroidComponent.h"
#include "../ECS/World.h"

#include <SFML/Window/Keyboard.hpp>

GameOverSystem::GameOverSystem(GameState& gameState)
	:_gameState(gameState)
{
}

void GameOverSystem::Update(World& world, float)
{
	if (!_gameState.isGameOver)
		return;

	if (sf::Keyboard::isKeyPressed(
		sf::Keyboard::Key::Enter))
	{
		_gameState.isGameOver = false;

		_gameState.score = 0;
	}

	// удаляем астероиды при завершении игры
	auto* asteroidPool = world.GetPool<AsteroidComponent>();

	if (asteroidPool)
	{
		auto asteroids =
			asteroidPool->GetDenseEntities();

		for (auto entity : asteroids)
		{
			world.DestroyEntityById(entity);
		}
	}

}