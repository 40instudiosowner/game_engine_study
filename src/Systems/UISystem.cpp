#include "UISystem.h"

#include "../ECS/World.h"

#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/AsteroidComponent.h"
#include "../Components/BulletComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/CollisionComponent.h"

#include <imgui.h>
#include <imgui-SFML.h>

#include <iostream>

UISystem::UISystem(sf::RenderWindow& window, World& world, GameState& gameState)
	: _window(window), _world(world), _gameState(gameState)
{
	if (!ImGui::SFML::Init(window))
		std::cerr << "ImGui init error\n";

	ImGui::GetStyle().ScaleAllSizes(1.f);

	ImGui::GetIO().FontGlobalScale = 2.f;
}

void UISystem::Update(
	World& world,
	bool& isRun)
{
	ImGui::SFML::Update(
		_window,
		_deltaClock.restart());

	UpdateInput(world, isRun);

	UpdateGui(world);

	ImGui::Begin("Game");

	ImGui::Text(
		"Score: %d",
		_gameState.score);

	ImGui::SliderFloat(
		"Asteroid Spawn Interval",
		&_gameState.asteroidSpawnInterval,
		0.1f,
		5.0f);

	if (ImGui::Button("Spawn Asteroid"))
	{
		_gameState.spawnAsteroidRequest = true;
	}

	ImGui::End();


	if (_gameState.isGameOver)
	{
		ImGui::SetNextWindowPos(
			ImVec2(
				_window.getSize().x / 2.f - 150.f,
				_window.getSize().y / 2.f - 100.f));

		ImGui::Begin(
			"Game Over",
			nullptr,
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse);

		ImGui::Text("GAME OVER");

		ImGui::Separator();

		ImGui::Text(
			"Final score: %d",
			_gameState.score);

		ImGui::Text(
			"Press Enter to restart");

		ImGui::End();
	}

}

void UISystem::UpdateInput(World&, bool& isRun)
{
	while (const std::optional event =
		_window.pollEvent())
	{
		ImGui::SFML::ProcessEvent(
			_window,
			*event);

		if (event->is<sf::Event::Closed>())
		{
			isRun = false;
		}

		if (_gameState.isGameOver)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
			{
				_gameState.shouldRestart = true;
			}

			continue;
		}

	}
}

void UISystem::UpdateGui(World& world)
{
	ImGui::Begin("Entities");

	const auto& entities =
		_world.GetEntities();

	for (const auto& entity : entities)
	{
		auto* transformPool = _world.GetPool<TransformComponent>();

		if (!transformPool || !transformPool->Has(entity.id))
		{
			continue; // не выводим id без компонента TransformComponent
		}

		ImGui::Separator();
		ImGui::Text(
			"Entity ID: %zu",
			entity.id);


		if (transformPool && transformPool->Has(entity.id))
		{
			auto& transform =
				transformPool->Get(entity.id);

			ImGui::Text(
				"Position: %.1f %.1f",
				transform.position.x,
				transform.position.y);
		}

		//
		// Components
		//

		ImGui::Text("Components:");

		if (_world.GetPool<PlayerComponent>() &&
			_world.GetPool<PlayerComponent>()->Has(entity.id))
		{
			ImGui::BulletText(
				"%s",
				typeid(PlayerComponent)
				.name());
		}

		if (_world.GetPool<AsteroidComponent>() &&
			_world.GetPool<AsteroidComponent>()
			->Has(entity.id))
		{
			ImGui::BulletText(
				"%s",
				typeid(AsteroidComponent)
				.name());
		}

		if (_world.GetPool<BulletComponent>() &&
			_world.GetPool<BulletComponent>()->Has(entity.id))
		{
			ImGui::BulletText(
				"%s",
				typeid(BulletComponent)
				.name());
		}

		if (_world.GetPool<MovementComponent>() &&
			_world.GetPool<MovementComponent>()->Has(entity.id))
		{
			ImGui::BulletText(
				"%s",
				typeid(MovementComponent)
				.name());
		}

		if (_world.GetPool<CollisionComponent>() &&
			_world.GetPool<CollisionComponent>()->Has(entity.id))
		{
			ImGui::BulletText(
				"%s",
				typeid(CollisionComponent)
				.name());
		}
	}

	ImGui::End();



	auto* spritePool =
		world.GetPool<SpriteComponent>();

	auto* transformPool =
		world.GetPool<TransformComponent>();


	if (!spritePool ||
		!transformPool)
		return;		// !!!!!!!!!! 

	//const auto& entities =
	//	spritePool->GetDenseEntities();

	if (entities.empty())
		return;

	auto entity = entities.front();

	if (spritePool && spritePool->Has(entity.id) && transformPool && transformPool->Has(entity.id))
	{

		auto& sprite =
			spritePool->Get(entity.id);


		auto& transform =
			transformPool->Get(entity.id);

		ImGui::Begin("Settings");


		ImGui::SliderFloat2(
			"Position",
			&transform.position.x,
			0.f,
			1500.f);

		//ImGui::ColorEdit4(
		//	"Color",
		//	reinterpret_cast<float*>(&sprite.color));

		ImGui::Checkbox(
			"Visible",
			&sprite.visible);

		ImGui::End();

	}
}

void UISystem::Shutdown()
{
	ImGui::SFML::Shutdown();
}

void UISystem::Render(sf::RenderWindow& window)
{
	ImGui::SFML::Render(window);
}
