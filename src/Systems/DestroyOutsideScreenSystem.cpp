#include "DestroyOutsideScreenSystem.h"

#include "../ECS/World.h"
#include "../ECS/FilterBuilder.h"

#include "../Components/TransformComponent.h"

#include "../Components/BulletComponent.h"
#include "../Components/AsteroidComponent.h"

DestroyOutsideScreenSystem::DestroyOutsideScreenSystem(sf::Vector2u windowSize, GameState& gameState)
	:_windowSize(windowSize), _gameState(gameState)
{
}

void DestroyOutsideScreenSystem::Update(
	World& world,
	float)
{
	auto* transformPool =
		world.GetPool<TransformComponent>();

	if (!transformPool)
		return;

	if (_gameState.isGameOver)
		return;

	//
	// Bullets — remove by distance from spawn point (standard Mega Man / run'n'gun)
	//
	auto* bulletPool =
		world.GetPool<BulletComponent>();

	if (bulletPool)
	{
		auto filter = FilterBuilder(world)
			.With<BulletComponent>()
			.With<TransformComponent>()
			.Build();

		for (auto entity : filter)
		{
			auto& transform = transformPool->Get(entity);
			auto& bullet = bulletPool->Get(entity);

			float dx = transform.position.x - bullet.spawnPosition.x;
			float dy = transform.position.y - bullet.spawnPosition.y;
			float distSq = dx * dx + dy * dy;

			if (distSq > bullet.maxDistance * bullet.maxDistance)
				world.DestroyEntityById(entity);
		}
	}

	//
	// Asteroids
	//

	auto* asteroidPool =
		world.GetPool<AsteroidComponent>();

	if (asteroidPool)
	{
		auto asteroids =
			asteroidPool->GetDenseEntities();

		for (auto entity : asteroids)
		{
			auto& transform =
				transformPool->Get(entity);

			if (transform.position.y > _windowSize.y + 200.f ||
				transform.position.x < -200.f ||
				transform.position.x > _windowSize.x + 200.f)
			{
				world.DestroyEntityById(entity);
			}
		}
	}
}
