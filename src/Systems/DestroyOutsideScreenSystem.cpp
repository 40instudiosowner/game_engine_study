#include "DestroyOutsideScreenSystem.h"

#include "../ECS/World.h"

#include "../Components/TransformComponent.h"

#include "../Components/BulletComponent.h"
#include "../Components/AsteroidComponent.h"

DestroyOutsideScreenSystem::DestroyOutsideScreenSystem(sf::Vector2u windowSize)
	:_windowSize(windowSize)
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

	//
	// Bullets
	//

	auto* bulletPool =
		world.GetPool<BulletComponent>();

	if (bulletPool)
	{
		auto bullets =
			bulletPool->GetDenseEntities();

		for (auto entity : bullets)
		{
			auto& transform =
				transformPool->Get(entity);

			if (transform.position.y < -50.f)
			{
				world.DestroyEntityById(entity);
			}
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

			if (
				transform.position.y >
				_windowSize.y + 200.f ||

				transform.position.x < -200.f ||

				transform.position.x >
				_windowSize.x + 200.f)
			{
				world.DestroyEntityById(entity);
			}
		}
	}
}