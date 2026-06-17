#include "CollisionResolveSystem.h"

#include "../Game/GameState.h"

#include "../Components/PlayerComponent.h"
#include "../Components/BulletComponent.h"
#include "../Components/AsteroidComponent.h"
#include "../Components/CollisionComponent.h"

#include "../ECS/World.h"
#include "../ECS/FilterBuilder.h"


CollisionResolveSystem::CollisionResolveSystem(GameState& gameState)
	: _gameState(gameState)
{
}


void CollisionResolveSystem::Update(World& world, float)
{
	if (_gameState.isGameOver)
		return;


	auto* collisionPool =
		world.GetPool<CollisionComponent>();

	auto* bulletPool =
		world.GetPool<BulletComponent>();

	auto* asteroidPool =
		world.GetPool<AsteroidComponent>();

	auto* playerPool =
		world.GetPool<PlayerComponent>();

	if (!collisionPool || !asteroidPool || !playerPool)
		return;


	//
	// Bullet vs Asteroid
	//


    {
        auto bulletFilter =
            FilterBuilder(world)
            .With<CollisionComponent>()
            .With<BulletComponent>()
            .Build();

        std::vector<uint32_t> bulletsToDestroy;
        std::vector<uint32_t> asteroidsToDestroy;

        for (auto bullet : bulletFilter)
        {
            auto& collisions =
                collisionPool
                ->Get(bullet)
                .collidedEntities;

            for (auto other : collisions)
            {
                if (!asteroidPool->Has(other))
                {
                    continue;
                }

                bulletsToDestroy.push_back(
                    static_cast<uint32_t>(bullet));

                asteroidsToDestroy.push_back(
                    static_cast<uint32_t>(other));

                _gameState.score++;
            }
        }

        for (auto entity : bulletsToDestroy)
        {
            world.DestroyEntityById(entity);
        }

        for (auto entity : asteroidsToDestroy)
        {
            world.DestroyEntityById(entity);
        }
    }

    //
    // Player vs Asteroid
    //

    {
        auto playerFilter =
            FilterBuilder(world)
            .With<CollisionComponent>()
            .With<PlayerComponent>()
            .Build();

        for (auto player : playerFilter)
        {
            auto& collisions = collisionPool
                ->Get(player)
                .collidedEntities;

            for (auto other : collisions)
            {
                if (!asteroidPool->Has(other))
                {
                    continue;
                }

                _gameState.isGameOver = true;

                world.DestroyEntityById(
                    static_cast<uint32_t>(player));

                world.DestroyEntityById(
                    static_cast<uint32_t>(other));

                return;
            }
        }
    }


	//if (bulletPool && asteroidPool)
	//{
	//	const auto& bullets =
	//		bulletPool->GetDenseEntities();

	//	for (auto bullet : bullets)
	//	{
	//		auto& collisions = collisionPool->Get(bullet).collidedEntities;

	//		for (auto other : collisions)
	//		{
	//			if (!asteroidPool->Has(other))
	//				continue;

	//			world.DestroyEntityById(bullet);

	//			world.DestroyEntityById(other);

	//			_gameState.score++;
	//		}
	//	}
	//}

	////
	//// Player vs Asteroid
	////

	//if (playerPool && asteroidPool)
	//{
	//	const auto& players = playerPool->GetDenseEntities();

	//	for (auto player : players)
	//	{
	//		auto& collisions = collisionPool->Get(player).collidedEntities;

	//		for (auto other : collisions)
	//		{
	//			if (!asteroidPool->Has(other))
	//				continue;

	//			_gameState.isGameOver = true;

	//			world.DestroyEntityById(player);
	//			world.DestroyEntityById(other);

	//		}
	//	}
	//}
}