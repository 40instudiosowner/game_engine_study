#include "TileCollisionSystem.h"
#include "../Components/PlayerComponent.h"
#include "../Components/TileComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/VelocityComponent.h"
#include "../ECS/World.h"
#include <algorithm>

void TileCollisionSystem::Update(World& world, float)
{
	auto* playerPool = world.GetPool<PlayerComponent>();
	auto* tilePool = world.GetPool<TileComponent>();
	auto* transformPool = world.GetPool<TransformComponent>();
	auto* boxPool = world.GetPool<BoxColliderComponent>();
	auto* collisionPool = world.GetPool<CollisionComponent>();
	auto* velocityPool = world.GetPool<VelocityComponent>();

	if (!playerPool || !tilePool || !transformPool || !boxPool || !collisionPool)
		return;

	for (auto playerEntity : playerPool->GetDenseEntities())
	{
		if (!collisionPool->Has(playerEntity) || !transformPool->Has(playerEntity) || !boxPool->Has(playerEntity))
			continue;

		auto& playerTransform = transformPool->Get(playerEntity);
		auto& playerBox = boxPool->Get(playerEntity);
		auto& collisions = collisionPool->Get(playerEntity).collidedEntities;
		auto& playerComp = playerPool->Get(playerEntity);

		playerComp.isGrounded = false;

		for (auto tileEntity : collisions)
		{
			if (!tilePool->Has(tileEntity) || !transformPool->Has(tileEntity) || !boxPool->Has(tileEntity))
				continue;

			auto& tileTransform = transformPool->Get(tileEntity);
			auto& tileBox = boxPool->Get(tileEntity);

			sf::FloatRect playerRect(playerTransform.position + playerBox.offset, playerBox.size);
			sf::FloatRect tileRect(tileTransform.position + tileBox.offset, tileBox.size);

			// Calculate overlap on each axis
			float overlapLeft = (playerRect.position.x + playerRect.size.x) - tileRect.position.x;
			float overlapRight = (tileRect.position.x + tileRect.size.x) - playerRect.position.x;
			float overlapTop = (playerRect.position.y + playerRect.size.y) - tileRect.position.y;
			float overlapBottom = (tileRect.position.y + tileRect.size.y) - playerRect.position.y;

			// Find minimum overlap axis (MTV)
			float minOverlapX = std::min(overlapLeft, overlapRight);
			float minOverlapY = std::min(overlapTop, overlapBottom);

			if (minOverlapX < minOverlapY)
			{
				// Resolve horizontally (adjust transform, accounting for offset)
				if (overlapLeft < overlapRight)
					playerTransform.position.x = tileRect.position.x - playerBox.size.x - playerBox.offset.x;
				else
					playerTransform.position.x = tileRect.position.x + tileRect.size.x - playerBox.offset.x;

				if (velocityPool && velocityPool->Has(playerEntity))
					velocityPool->Get(playerEntity).velocity.x = 0.f;
			}
			else
			{
				// Resolve vertically
				if (overlapTop < overlapBottom)
				{
					// Player lands ON TOP of tile
					playerTransform.position.y = tileRect.position.y - playerBox.size.y - playerBox.offset.y;
					playerComp.isGrounded = true;
					if (velocityPool && velocityPool->Has(playerEntity))
					{
						auto& vel = velocityPool->Get(playerEntity);
						if (vel.velocity.y > 0.f)
							vel.velocity.y = 0.f;
					}
				}
				else
				{
					// Player hits tile from below (head bump)
					playerTransform.position.y = tileRect.position.y + tileRect.size.y - playerBox.offset.y;
					if (velocityPool && velocityPool->Has(playerEntity))
					{
						auto& vel = velocityPool->Get(playerEntity);
						if (vel.velocity.y < 0.f)
							vel.velocity.y = 0.f;
					}
				}
			}
		}
	}
}