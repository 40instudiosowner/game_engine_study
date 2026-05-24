#include "BoundsSystem.h"

#include "../ECS/World.h"

#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoundsComponent.h"

#include <cmath>

void BoundsSystem::Update(
	World& world,
	float)
{
	auto* transformPool =
		world.GetPool<TransformComponent>();

	auto* velocityPool =
		world.GetPool<VelocityComponent>();

	auto* spritePool =
		world.GetPool<SpriteComponent>();

	auto* boundsPool =
		world.GetPool<BoundsComponent>();

	if (!transformPool ||
		!velocityPool ||
		!spritePool ||
		!boundsPool)
	{
		return;
	}

	//
	// Итерируемся по smallest pool
	//

	const auto& entities =
		boundsPool->GetDenseEntities();

	for (auto entity : entities)
	{
		if (!transformPool->Has(entity) ||
			!velocityPool->Has(entity) ||
			!spritePool->Has(entity))
		{
			continue;
		}

		auto& transform =
			transformPool->Get(entity);

		auto& velocity =
			velocityPool->Get(entity);

		auto& sprite =
			spritePool->Get(entity);

		auto& bounds =
			boundsPool->Get(entity);

		auto position =
			transform.position;

		auto size = sprite.sprite.getGlobalBounds().size;

		//
		// LEFT
		//

		if (position.x <= bounds.min.x)
		{
			position.x = bounds.min.x;

			if (bounds.bounce)
			{
				velocity.velocity.x = std::abs(velocity.velocity.x);
			}
		}

		//
		// RIGHT
		//

		else if (
			position.x + size.x >=
			bounds.max.x)
		{
			position.x = bounds.max.x - size.x;

			if (bounds.bounce)
			{
				velocity.velocity.x = -std::abs(velocity.velocity.x);
			}
		}

		//
		// TOP
		//

		if (position.y <= bounds.min.y)
		{
			position.y = bounds.min.y;

			if (bounds.bounce)
			{
				velocity.velocity.y =
					std::abs(
						velocity.velocity.y);
			}
		}

		//
		// BOTTOM
		//

		else if (
			position.y + size.y >=
			bounds.max.y)
		{
			position.y =
				bounds.max.y - size.y;

			if (bounds.bounce)
			{
				velocity.velocity.y =
					-std::abs(
						velocity.velocity.y);
			}
		}

		transform.position =
			position;
	}
}