#include "CollisionSystem.h"

#include "../ECS/World.h"

#include "../Components/TransformComponent.h"

#include "../Components/BoxColliderComponent.h"
#include "../Components/CircleColliderComponent.h"

#include "../Components/CollisionComponent.h"

#include <cmath>
#include <algorithm>

static float DistanceSquared(
	const sf::Vector2f& a,
	const sf::Vector2f& b)
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;

	return dx * dx + dy * dy;
}

static bool AABBvsAABB(
	const sf::Vector2f& aPos,
	const sf::Vector2f& aSize,
	const sf::Vector2f& bPos,
	const sf::Vector2f& bSize)
{
	return
		aPos.x < bPos.x + bSize.x &&
		aPos.x + aSize.x > bPos.x &&
		aPos.y < bPos.y + bSize.y &&
		aPos.y + aSize.y > bPos.y;
}

void CollisionSystem::Update(
	World& world,
	float)
{
	auto* collisionPool =
		world.GetPool<CollisionComponent>();

	auto* transformPool =
		world.GetPool<TransformComponent>();

	auto* circlePool =
		world.GetPool<CircleColliderComponent>();

	auto* boxPool =
		world.GetPool<BoxColliderComponent>();

	if (!collisionPool ||
		!transformPool)
	{
		return;
	}

	//
	// Clear collisions
	//

	const auto& collisionEntities =
		collisionPool->GetDenseEntities();

	for (auto entity : collisionEntities)
	{
		collisionPool
			->Get(entity)
			.collidedEntities
			.clear();
	}

	//
	// Circle vs Circle
	//

	if (circlePool)
	{
		const auto& circles =
			circlePool->GetDenseEntities();

		for (size_t i = 0;
			i < circles.size();
			++i)
		{
			for (size_t j = i + 1;
				j < circles.size();
				++j)
			{
				size_t a = circles[i];
				size_t b = circles[j];

				auto& ta =
					transformPool->Get(a);

				auto& tb =
					transformPool->Get(b);

				auto& ca =
					circlePool->Get(a);

				auto& cb =
					circlePool->Get(b);

				sf::Vector2f centerA =
				{
					ta.position.x + ca.radius,
					ta.position.y + ca.radius
				};

				sf::Vector2f centerB =
				{
					tb.position.x + cb.radius,
					tb.position.y + cb.radius
				};

				float radius =
					ca.radius +
					cb.radius;

				if (
					DistanceSquared(
						centerA,
						centerB)
					<=
					radius * radius)
				{
					collisionPool
						->Get(a)
						.collidedEntities
						.push_back(b);

					collisionPool
						->Get(b)
						.collidedEntities
						.push_back(a);
				}
			}
		}
	}

	//
	// AABB vs AABB
	//

	if (boxPool)
	{
		const auto& boxes =
			boxPool->GetDenseEntities();

		for (size_t i = 0;
			i < boxes.size();
			++i)
		{
			for (size_t j = i + 1;
				j < boxes.size();
				++j)
			{
				size_t a = boxes[i];
				size_t b = boxes[j];

				auto& ta =
					transformPool->Get(a);

				auto& tb =
					transformPool->Get(b);

				auto& ba =
					boxPool->Get(a);

				auto& bb =
					boxPool->Get(b);

				if (
					AABBvsAABB(
						ta.position,
						ba.size,
						tb.position,
						bb.size))
				{
					collisionPool
						->Get(a)
						.collidedEntities
						.push_back(b);

					collisionPool
						->Get(b)
						.collidedEntities
						.push_back(a);
				}
			}
		}
	}

	//
	// Box vs Circle
	//

	if (boxPool && circlePool)
	{
		const auto& boxes =
			boxPool->GetDenseEntities();

		const auto& circles =
			circlePool->GetDenseEntities();

		for (auto boxEntity : boxes)
		{
			for (auto circleEntity : circles)
			{
				if (boxEntity == circleEntity)
					continue;

				auto& bt =
					transformPool
					->Get(boxEntity);

				auto& ct =
					transformPool
					->Get(circleEntity);

				auto& box =
					boxPool
					->Get(boxEntity);

				auto& circle =
					circlePool
					->Get(circleEntity);

				sf::Vector2f circleCenter =
				{
					ct.position.x + circle.radius,
					ct.position.y + circle.radius
				};

				float closestX =
					std::clamp(
						circleCenter.x,
						bt.position.x,
						bt.position.x +
						box.size.x);

				float closestY =
					std::clamp(
						circleCenter.y,
						bt.position.y,
						bt.position.y +
						box.size.y);

				float dx =
					circleCenter.x -
					closestX;

				float dy =
					circleCenter.y -
					closestY;

				if (
					dx * dx +
					dy * dy
					<=
					circle.radius *
					circle.radius)
				{
					collisionPool
						->Get(boxEntity)
						.collidedEntities
						.push_back(circleEntity);

					collisionPool
						->Get(circleEntity)
						.collidedEntities
						.push_back(boxEntity);
				}
			}
		}
	}
}