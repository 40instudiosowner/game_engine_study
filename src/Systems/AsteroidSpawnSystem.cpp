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

void AsteroidSpawnSystem::SpawnAsteroid(
	World& world)
{
	auto asteroid =
		world.CreateEntity();

	TransformComponent transform;

	transform.position =
	{
		static_cast<float>(
			rand() % _windowSize.x),
		-100.f
	};

	world.AddComponent(
		asteroid,
		transform);

	
	// Random radius
	float radius =
		20.f +
		static_cast<float>(
			rand() % 40);

	CircleColliderComponent collider;
	collider.radius = radius;

	world.AddComponent(
		asteroid,
		collider);

	CircleShapeComponent shape;

	shape.shape.setRadius(radius);

	shape.shape.setPointCount(
		8 + rand() % 8);

	shape.shape.setFillColor(
		sf::Color::Transparent);

	shape.shape.setOutlineColor(
		sf::Color::White);

	shape.shape.setOutlineThickness(
		2.f);

	world.AddComponent(
		asteroid,
		shape);

	MovementComponent movement;

	movement.direction =
	{
		-0.5f +
		static_cast<float>(
			rand()) / RAND_MAX,

		0.5f +
		static_cast<float>(
			rand()) / RAND_MAX
	};

	movement.speed =
		50.f +
		static_cast<float>(
			rand() % 250);

	world.AddComponent(
		asteroid,
		movement);

	world.AddComponent(
		asteroid,
		CollisionComponent());

	world.AddComponent(
		asteroid,
		AsteroidComponent());
}

void AsteroidSpawnSystem::Update(
	World& world,
	float dt)
{
	_timer += dt;

	if (_gameState.spawnAsteroidRequest)
	{
		SpawnAsteroid(world);

		_gameState.spawnAsteroidRequest = false;
	}


	if (_timer < _gameState.asteroidSpawnInterval)
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

	if (_gameState.isGameOver)
		return;

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