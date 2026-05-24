#include "ShooterSystem.h"

#include "../Components/PlayerComponent.h"
#include "../Components/ShooterComponent.h"
#include "../Components/TransformComponent.h"

#include "../Components/BulletComponent.h"
#include "../Components/RectangleShapeComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/MovementComponent.h"

#include "../ECS/World.h"

void ShooterSystem::Update(
	World& world,
	float dt)
{
	auto* playerPool =
		world.GetPool<PlayerComponent>();

	auto* shooterPool =
		world.GetPool<ShooterComponent>();

	auto* transformPool =
		world.GetPool<TransformComponent>();

	if (!playerPool ||
		!shooterPool ||
		!transformPool)
	{
		return;
	}

	const auto& entities =
		playerPool->GetDenseEntities();

	for (auto entity : entities)
	{
		auto& shooter = shooterPool->Get(entity);

		shooter.currentCooldown -= dt;

		if (!shooter.wantsShoot)
			continue; // игрок не хочет стрелять

		shooter.wantsShoot = false;

		if (shooter.currentCooldown > 0.f)
			continue;

		shooter.currentCooldown = shooter.cooldown;

		auto& transform = transformPool->Get(entity);

		EntityId bullet = world.CreateEntity(); // создали пулю

		TransformComponent bulletTransform;

		bulletTransform.position =
		{
			transform.position.x + 40.f - 6.f / 2.f, // 40 - радиус игрока, 6 - ширина пули
			transform.position.y
		};

		MovementComponent movement;

		movement.direction = { 0.f, -1.f };

		movement.speed = 700.f;

		RectangleShapeComponent shape;

		shape.shape.setSize({ 6.f, 20.f });

		shape.shape.setFillColor(
			sf::Color::Red);

		BoxColliderComponent collider;

		collider.size = { 6.f, 20.f };

		world.AddComponent(
			bullet,
			bulletTransform);

		world.AddComponent(
			bullet,
			movement);

		world.AddComponent(
			bullet,
			shape);

		world.AddComponent(
			bullet,
			collider);

		world.AddComponent(
			bullet,
			CollisionComponent());

		world.AddComponent(
			bullet,
			BulletComponent());
	}
}