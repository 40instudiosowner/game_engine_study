#include "MovementSystem.h"

#include "../Components/TransformComponent.h"
#include "../Components/MovementComponent.h"

#include "../ECS/World.h"
#include "../ECS/FilterBuilder.h"

void MovementSystem::Update(
	World& world,
	float dt)
{
	auto filter =		// строим фильтр с нужными компонентами
		FilterBuilder(world)
		.With<TransformComponent>()
		.With<MovementComponent>()
		.Build();


	auto* transformPool =
		world.GetPool<TransformComponent>();

	auto* movementPool =
		world.GetPool<MovementComponent>();

	if (!transformPool ||
		!movementPool)
	{
		return;
	}

	for (auto entity : filter)
	{
		auto& transform = transformPool->Get(entity);

		auto& movement = movementPool->Get(entity);

		transform.position += movement.direction * movement.speed * dt;
	}
	//const auto& entities =
	//	movementPool->GetDenseEntities();

	//for (auto entity : entities)
	//{
	//	if (!transformPool->Has(entity))
	//		continue;


	//	auto& transform = transformPool->Get(entity);

	//	//if (transform.position.y < 0)
	//	//{
	//	//	world.DestroyEntityById(entity);
	//	//  continue;
	//	//}

	//	auto& movement = movementPool->Get(entity);

	//	transform.position += movement.direction * movement.speed * dt;
	//}

}