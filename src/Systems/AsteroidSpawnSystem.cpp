#include "AsteroidSpawnSystem.h"

#include "../Components/AsteroidComponent.h"
#include "../Components/CircleShapeComponent.h"
#include "../Components/CircleColliderComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/MovementComponent.h"

#include "../ECS/World.h"

#include <random>

AsteroidSpawnSystem::AsteroidSpawnSystem(sf::Vector2u windowSize, GameState& gameState):
	_windowSize(windowSize), _gameState(gameState)
{
}

void AsteroidSpawnSystem::Update(
	World& world,
	float dt)
{
	_timer += dt;

	if (_timer < _spawnDelay)
		return;

	_timer = 0.f;

	static std::random_device rd;

	static std::mt19937 gen(rd());

	std::uniform_real_distribution<float>
		xDist(
			0.f,
			static_cast<float>(
				_windowSize.x));

	std::uniform_real_distribution<float>
		radiusDist(20.f, 60.f);

	std::uniform_real_distribution<float>
		speedDist(100.f, 250.f);

	float radius =
		radiusDist(gen);

	EntityId asteroid =
		world.CreateEntity();

	TransformComponent transform;

	transform.position =
	{
		xDist(gen),
		-radius * 2.f
	};

	MovementComponent movement;

	movement.direction =
	{
		0.f,
		1.f
	};

	movement.speed =
		speedDist(gen);

	CircleShapeComponent shape;

	shape.shape.setRadius(radius);

	shape.shape.setPointCount(12);

	shape.shape.setFillColor(
		sf::Color(120, 120, 120));

	CircleColliderComponent collider;

	collider.radius = radius;

	world.AddComponent(
		asteroid,
		transform);

	world.AddComponent(
		asteroid,
		movement);

	world.AddComponent(
		asteroid,
		shape);

	world.AddComponent(
		asteroid,
		collider);

	world.AddComponent(
		asteroid,
		CollisionComponent());

	world.AddComponent(
		asteroid,
		AsteroidComponent());
}