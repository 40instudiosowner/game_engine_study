#include "RenderSystem.h"

#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/CircleShapeComponent.h"
#include "../Components/RectangleShapeComponent.h"

#include "../ECS/World.h"

#include <SFML/Graphics/RenderWindow.hpp>

RenderSystem::RenderSystem(sf::RenderWindow& window, GameState& gameState)
	: _window(window), _gameState(gameState)
{
}

void RenderSystem::Update(World& world, float)
{
	auto* transformPool =
		world.GetPool<TransformComponent>();

	auto* spritePool =
		world.GetPool<SpriteComponent>();

	auto* circlePool =
		world.GetPool<CircleShapeComponent>();

	auto* rectPool =
		world.GetPool<RectangleShapeComponent>();

	if (circlePool)
	{
		const auto& entities =
			circlePool->GetDenseEntities();

		for (auto entity : entities)
		{
			auto& transform =
				transformPool->Get(entity);

			auto& shape =
				circlePool->Get(entity);

			shape.shape.setPosition(
				transform.position);

			_window.draw(shape.shape);
		}
	}

	if (rectPool)
	{
		const auto& entities =
			rectPool->GetDenseEntities();

		for (auto entity : entities)
		{
			auto& transform =
				transformPool->Get(entity);

			auto& shape =
				rectPool->Get(entity);

			shape.shape.setPosition(
				transform.position);

			_window.draw(
				shape.shape);
		}
	}

	if (spritePool)
	{
		const auto& entities =
			spritePool->GetDenseEntities();

		for (auto entity : entities)
		{
			auto& transform =
				transformPool->Get(entity);

			auto& sprite =
				spritePool->Get(entity);

			sprite.sprite.setPosition(
				transform.position);

			if (sprite.visible)
			{
				_window.draw(sprite.sprite);
			}
		}
	}
	
}