#include "UISystem.h"

#include "../ECS/World.h"

#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/SpriteComponent.h"

#include <imgui.h>
#include <imgui-SFML.h>

#include <iostream>

UISystem::UISystem(sf::RenderWindow& window, GameState& gameState)
	: _window(window), _gameState(gameState)
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
			"Press any key to restart");

		ImGui::End();
	}

}

void UISystem::UpdateInput(
	World&,
	bool& isRun)
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
			if (event->is<sf::Event::KeyPressed>())
			{
				//RestartGame();
			}

			continue;
		}

	}
}

void UISystem::UpdateGui(World& world)
{
	auto* spritePool =
		world.GetPool<SpriteComponent>();

	auto* velocityPool =
		world.GetPool<VelocityComponent>();

	auto* transformPool =
		world.GetPool<TransformComponent>();

	if (!spritePool ||
		!velocityPool ||
		!transformPool)
		return;

	const auto& entities =
		spritePool->GetDenseEntities();

	if (entities.empty())
		return;

	auto entity = entities.front();

	auto& sprite =
		spritePool->Get(entity);

	auto& velocity =
		velocityPool->Get(entity);

	auto& transform =
		transformPool->Get(entity);

	ImGui::Begin("Settings");

	ImGui::SliderFloat(
		"Velocity X",
		&velocity.velocity.x,
		-500.f,
		500.f);

	ImGui::SliderFloat(
		"Velocity Y",
		&velocity.velocity.y,
		-500.f,
		500.f);

	ImGui::SliderFloat2(
		"Position",
		&transform.position.x,
		0.f,
		1500.f);

	ImGui::ColorEdit4(
		"Color",
		reinterpret_cast<float*>(&sprite.color));

	ImGui::Checkbox(
		"Visible",
		&sprite.visible);

	ImGui::End();
}

void UISystem::Shutdown()
{
	ImGui::SFML::Shutdown();
}

void UISystem::Render(sf::RenderWindow& window)
{
	ImGui::SFML::Render(window);
}
