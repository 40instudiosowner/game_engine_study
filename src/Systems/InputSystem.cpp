#include "InputSystem.h"

#include "../Components/PlayerComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/ShooterComponent.h"

#include "../ECS/World.h"

#include <SFML/Window/Keyboard.hpp>

void InputSystem::Update(
	World& world,
	float)
{
	auto* playerPool =
		world.GetPool<PlayerComponent>();

	auto* movementPool =
		world.GetPool<MovementComponent>();

	auto* shooterPool =
		world.GetPool<ShooterComponent>();

	if (!playerPool ||
		!movementPool ||
		!shooterPool)
	{
		return;
	}

	const auto& entities =
		playerPool->GetDenseEntities();

	for (auto entity : entities)
	{
		auto& movement =
			movementPool->Get(entity);

		auto& shooter =
			shooterPool->Get(entity);

		movement.direction = { 0.f, 0.f };

		if (sf::Keyboard::isKeyPressed(
			sf::Keyboard::Key::A))
		{
			movement.direction.x = -1.f;
		}

		if (sf::Keyboard::isKeyPressed(
			sf::Keyboard::Key::D))
		{
			movement.direction.x = 1.f;
		}

		if (sf::Keyboard::isKeyPressed(
			sf::Keyboard::Key::Space))
		{
			shooter.wantsShoot = true;
		}
	}
}