#include "PlayerAnimationSystem.h"
#include "../Components/PlayerComponent.h"
#include "../Components/AnimationStateComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../ECS/World.h"
#include "../ECS/FilterBuilder.h"
#include <cmath>

void PlayerAnimationSystem::Update(World& world, float)
{
	auto filter = FilterBuilder(world)
		.With<PlayerComponent>()
		.With<AnimationStateComponent>()
		.With<MovementComponent>()
		.Build();

	auto* playerPool = world.GetPool<PlayerComponent>();
	auto* statePool = world.GetPool<AnimationStateComponent>();
	auto* movementPool = world.GetPool<MovementComponent>();
	auto* spritePool = world.GetPool<SpriteComponent>();

	if (!playerPool || !statePool || !movementPool)
		return;

	for (auto entity : filter)
	{
		auto& player = playerPool->Get(entity);
		auto& state = statePool->Get(entity);
		auto& movement = movementPool->Get(entity);

		bool isMoving = std::abs(movement.direction.x) > 0.1f;
		bool isInAir = !player.isGrounded;
		bool isShooting = player.shootAnimTimer > 0;

		std::string newAnim;

		if (isShooting)
		{
			if (isInAir)
				newAnim = "MegaShootJump";
			else if (isMoving)
				newAnim = "MegaShootRun";
			else
				newAnim = "MegaShootIdle";
		}
		else
		{
			if (isInAir)
				newAnim = "MegaJump";
			else if (isMoving)
				newAnim = "MegaRun";
			else
				newAnim = "MegaIdle";
		}

		if (!newAnim.empty())
			state.nextAnimation = newAnim;

		// Flip sprite based on facing direction
		if (spritePool && spritePool->Has(entity))
		{
			auto& sprite = spritePool->Get(entity);
			if (player.facingRight)
				sprite.scale = { std::abs(sprite.scale.x), sprite.scale.y };
			else
				sprite.scale = { -std::abs(sprite.scale.x), sprite.scale.y };
		}

		// Reset shoot flag after processing
		player.wantsShoot = false;
	}
}