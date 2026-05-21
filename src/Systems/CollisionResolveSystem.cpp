#include "CollisionResolveSystem.h"

#include "../Game/GameState.h"

#include "../Components/PlayerComponent.h"
#include "../Components/BulletComponent.h"
#include "../Components/AsteroidComponent.h"
#include "../Components/CollisionComponent.h"

#include "../ECS/World.h"


CollisionResolveSystem::CollisionResolveSystem(GameState& gameState)
	: _gameState(gameState)
{
}


void CollisionResolveSystem::Update(World& world, float)
{
	auto* collisionPool =
		world.GetPool<CollisionComponent>();

	auto* bulletPool =
		world.GetPool<BulletComponent>();

	auto* asteroidPool =
		world.GetPool<AsteroidComponent>();

	auto* playerPool =
		world.GetPool<PlayerComponent>();

	if (!collisionPool)
		return;

	//
	// Bullet vs Asteroid
	//

	if (bulletPool && asteroidPool)
	{
		const auto& bullets =
			bulletPool->GetDenseEntities();

		for (auto bullet : bullets)
		{
			auto& collisions =
				collisionPool
				->Get(bullet)
				.collidedEntities;

			for (auto other : collisions)
			{
				if (!asteroidPool->Has(other))
					continue;

				world.DestroyEntityById(
					bullet);

				world.DestroyEntityById(
					other);

				_gameState.score++;
			}
		}
	}

	//
	// Player vs Asteroid
	//

	if (playerPool && asteroidPool)
	{
		const auto& players =
			playerPool->GetDenseEntities();

		for (auto player : players)
		{
			auto& collisions = collisionPool->Get(player).collidedEntities;

			for (auto other : collisions)
			{
				if (!asteroidPool->Has(other))
					continue;

				_gameState.isGameOver = true;

				world.DestroyEntityById(player);
				world.DestroyEntityById(other);

			}
		}
	}
}