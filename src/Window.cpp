#include "Window.h"

#include "Systems/BoundsSystem.h"
#include "Systems/MovementSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/UISystem.h"
#include "Systems/AsteroidSpawnSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/ShooterSystem.h"
#include "Systems/CollisionSystem.h"
#include "Systems/CollisionResolveSystem.h"
#include "Systems/GameOverSystem.h"
#include "Systems/DestroyOutsideScreenSystem.h"

#include "Components/TransformComponent.h"
#include "Components/VelocityComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/BoundsComponent.h"
#include "Components/CollisionComponent.h"
#include "Components/MovementComponent.h"
#include "Components/CircleShapeComponent.h"
#include "Components/ShooterComponent.h"
#include "Components/CircleColliderComponent.h"
#include "Components/PlayerComponent.h"

#include <iostream>
#include <imgui-SFML.h>

Window::Window(
	unsigned int width,
	unsigned int height,
	ConfigReader& config)
{
	_gameState.isGameOver = false;
	_gameState.score = 0;

	_window.create(
		sf::VideoMode({ width, height }),
		"Space Invaders ECS");

	auto desktop =
		sf::VideoMode::getDesktopMode();

	_window.setPosition(
		{
			static_cast<int>(
				desktop.size.x / 2 - width / 2),

			static_cast<int>(
				desktop.size.y / 2 - height / 2)
		});

	_window.setFramerateLimit(60);

	_config = std::make_unique<ConfigReader>(config);

	RegisterSystems();

	// UI SYSTEM
	_uiSystem = std::make_unique<UISystem>(_window, _world, _gameState);

	// ENTITIES

	Initialize();
}

void Window::RegisterSystems()
{
	// ECS SYSTEMS
	_systems.AddSystem<MovementSystem>();

	_systems.AddSystem<BoundsSystem>();

	_systems.AddSystem<RenderSystem>(_window, _gameState);

	_systems.AddSystem<InputSystem>();

	_systems.AddSystem<ShooterSystem>();

	_systems.AddSystem<AsteroidSpawnSystem>(_window.getSize(), _gameState);

	_systems.AddSystem<CollisionSystem>();

	_systems.AddSystem<CollisionResolveSystem>(_gameState);

	_systems.AddSystem<GameOverSystem>(_gameState);

	_systems.AddSystem<DestroyOutsideScreenSystem>(_window.getSize(), _gameState);
}

Window::~Window() {};

void Window::RestartGame()
{
	_world = World();

	_systems = SystemManager();

	_gameState = {};

	CreatePlayerEntity();

	RegisterSystems();
}

void Window::Initialize()
{
	if (!_config)
	{
		std::cerr
			<< "Config is null\n";

		return;
	}

	CreatePlayerEntity();
}

void Window::CreatePlayerEntity()
{
	EntityId player =
		_world.CreateEntity();

	//
	// Transform
	//

	TransformComponent transform;

	transform.position =
	{
		_window.getSize().x / 2.f,
		_window.getSize().y - 120.f
	};

	//
	// Movement
	MovementComponent movement;

	movement.speed = 500.f;

	// Shape
	CircleShapeComponent shape;

	shape.shape.setRadius(40.f);

	shape.shape.setPointCount(3);

	shape.shape.setRotation(
		sf::degrees(90.f));

	shape.shape.setFillColor(
		sf::Color::Green);

	// Collider
	CircleColliderComponent collider;

	collider.radius = 40.f;

	// Shooter
	ShooterComponent shooter;

	shooter.cooldown = 0.25f;

	// Player
	PlayerComponent playerComponent;

	// Collision
	CollisionComponent collision;

	// Add components
	_world.AddComponent(
		player,
		transform);

	_world.AddComponent(
		player,
		movement);

	_world.AddComponent(
		player,
		shape);

	_world.AddComponent(
		player,
		collider);

	_world.AddComponent(
		player,
		shooter);

	_world.AddComponent(
		player,
		collision);

	_world.AddComponent(
		player,
		playerComponent);
}


void Window::Run()
{
	sf::Clock deltaClock;

	while (_isRun)
	{
		float dt =
			deltaClock
			.restart()
			.asSeconds();

		// Clear frame
		_window.clear();

		// UI
		_uiSystem->Update(_world,_isRun);

		// ECS Systems
		_systems.Update(_world, dt);

		// ImGui render

		ImGui::SFML::Render(_window);

		// Present frame
		_window.display();

		// Restart
		if (_gameState.shouldRestart)
		{
			RestartGame();

			_gameState.shouldRestart = false;
		}
	}

	_uiSystem->Shutdown();

	_window.close();
}
