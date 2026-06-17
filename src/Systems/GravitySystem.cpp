#include "GravitySystem.h"
#include "../Components/GravityComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/PlayerComponent.h"
#include "../ECS/World.h"
#include "../ECS/FilterBuilder.h"

void GravitySystem::Update(World& world, float dt)
{
	auto filter = FilterBuilder(world)
		.With<GravityComponent>()
		.Build();

	auto* gravityPool = world.GetPool<GravityComponent>();
	auto* transformPool = world.GetPool<TransformComponent>();
	auto* velocityPool = world.GetPool<VelocityComponent>();
	auto* movementPool = world.GetPool<MovementComponent>();
	auto* playerPool = world.GetPool<PlayerComponent>();

	if (!gravityPool || !transformPool)
		return;

	for (auto entity : filter)
	{
		auto& gravity = gravityPool->Get(entity);

		// Primary path: use VelocityComponent (proper physics)
		if (velocityPool && velocityPool->Has(entity))
		{
			auto& velocity = velocityPool->Get(entity);
			velocity.velocity.y += gravity.gravity * dt;

			auto& transform = transformPool->Get(entity);
			transform.position += velocity.velocity * dt;

			// Handle player jump
			if (playerPool && playerPool->Has(entity))
			{
				auto& player = playerPool->Get(entity);
				if (player.wantsJump && player.isGrounded)
				{
					velocity.velocity.y = player.jumpVelocity;
					player.wantsJump = false;
					player.isGrounded = false;
				}
			}
		}
	}
}