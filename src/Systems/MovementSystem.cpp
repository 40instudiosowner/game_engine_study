#include "MovementSystem.h"

#include "../Components/TransformComponent.h"
#include "../Components/MovementComponent.h"

#include "../ECS/World.h"

void MovementSystem::Update(
	World& world,
	float dt)
{
	auto* transformPool =
		world.GetPool<TransformComponent>();

	auto* movementPool =
		world.GetPool<MovementComponent>();

	if (!transformPool ||
		!movementPool)
	{
		return;
	}

	const auto& entities =
		movementPool->GetDenseEntities();

	for (auto entity : entities)
	{
		if (!transformPool->Has(entity))
			continue;


		auto& transform =
			transformPool->Get(entity);

		//if (transform.position.y < 0)
		//{
		//	world.DestroyEntityById(entity);
		//  continue;
		//}

		auto& movement =
			movementPool->Get(entity);

		transform.position +=
			movement.direction *
			movement.speed *
			dt;
	}
}