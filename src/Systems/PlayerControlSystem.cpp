#include "PlayerControlSystem.h"
#include "../Components/PlayerComponent.h"
#include "../Components/MovementComponent.h"
#include "../ECS/World.h"
#include "../ECS/FilterBuilder.h"
#include <SFML/Window/Keyboard.hpp>

PlayerControlSystem::PlayerControlSystem(InputManager& inputManager)
	: _inputManager(inputManager)
{
}

void PlayerControlSystem::Update(World& world, float)
{
	auto filter = FilterBuilder(world)
		.With<PlayerComponent>()
		.With<MovementComponent>()
		.Build();

	auto* playerPool = world.GetPool<PlayerComponent>();
	auto* movementPool = world.GetPool<MovementComponent>();

	if (!playerPool || !movementPool)
		return;

	for (auto entity : filter)
	{
		auto& player = playerPool->Get(entity);
		auto& movement = movementPool->Get(entity);

		// Horizontal movement
		float dir = 0.f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			dir = -1.f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			dir = 1.f;

		movement.direction.x = dir;
		movement.direction.y = 0.f;

		if (dir > 0.f) player.facingRight = true;
		if (dir < 0.f) player.facingRight = false;

		// Jump (only when grounded) - use Press action to prevent held-key repeat
		if (_inputManager.WasActionJustPressed("Jump") && player.isGrounded)
		{
			player.wantsJump = true;
		}

		// Shoot - use Press action
		if (_inputManager.WasActionJustPressed("Shoot"))
		{
			player.wantsShoot = true;
			player.shootAnimTimer = 10;
		}

		// Decay shoot timer
		if (player.shootAnimTimer > 0)
			player.shootAnimTimer--;
	}
}