#include "RenderSystem.h"

#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CircleColliderComponent.h"
#include "../Components/CircleShapeComponent.h"
#include "../Components/RectangleShapeComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/TileComponent.h"
#include "../Components/DecorComponent.h"
#include "../Components/BulletComponent.h"

#include "../ECS/World.h"

#include <SFML/Graphics/RenderWindow.hpp>

static constexpr int TILE_SIZE = 64;

RenderSystem::RenderSystem(
	sf::RenderWindow& window,
	GameState& gameState,
	AssetManager& assets,
	int& renderMode)
	: _window(window)
	, _gameState(gameState)
	, _assets(assets)
	, _renderMode(renderMode)
{
}

void RenderSystem::Update(World&, float)
{
}

void RenderSystem::Render(World& world, sf::RenderWindow& window, AssetManager& assets, int renderMode)
{
	switch (renderMode)
	{
	case 0: RenderTextures(world); break;
	case 1: RenderColliders(world); break;
	case 2: RenderGrid(); break;
	default: RenderTextures(world); break;
	}
}

void RenderSystem::RenderTextures(World& world)
{
	auto* transformPool = world.GetPool<TransformComponent>();
	auto* spritePool = world.GetPool<SpriteComponent>();
	auto* decorPool = world.GetPool<DecorComponent>();
	auto* tilePool = world.GetPool<TileComponent>();
	auto* playerPool = world.GetPool<PlayerComponent>();

	if (!transformPool || !spritePool)
		return;

	// Layer 1: Decorations (background)
	if (decorPool)
	{
		for (auto entity : spritePool->GetDenseEntities())
		{
			if (!decorPool->Has(entity) || !transformPool->Has(entity))
				continue;
			DrawSprite(entity, *transformPool, *spritePool);
		}
	}

	// Layer 2: Tiles (level geometry)
	if (tilePool)
	{
		for (auto entity : spritePool->GetDenseEntities())
		{
			if (!tilePool->Has(entity) || !transformPool->Has(entity))
				continue;
			DrawSprite(entity, *transformPool, *spritePool);
		}
	}

	// Layer 3: Player + Bullets + everything else
	for (auto entity : spritePool->GetDenseEntities())
	{
		if (!transformPool->Has(entity))
			continue;

		// Skip decor and tiles (already drawn)
		if (decorPool && decorPool->Has(entity)) continue;
		if (tilePool && tilePool->Has(entity)) continue;

		DrawSprite(entity, *transformPool, *spritePool);
	}

	// Circle shapes (backward compat)
	auto* circlePool = world.GetPool<CircleShapeComponent>();
	if (circlePool)
	{
		for (auto entity : circlePool->GetDenseEntities())
		{
			if (!transformPool->Has(entity)) continue;
			auto& shape = circlePool->Get(entity);
			shape.shape.setPosition(transformPool->Get(entity).position);
			_window.draw(shape.shape);
		}
	}

	// Rectangle shapes
	auto* rectPool = world.GetPool<RectangleShapeComponent>();
	if (rectPool)
	{
		for (auto entity : rectPool->GetDenseEntities())
		{
			if (!transformPool->Has(entity)) continue;
			auto& shape = rectPool->Get(entity);
			shape.shape.setPosition(transformPool->Get(entity).position);
			_window.draw(shape.shape);
		}
	}
}

void RenderSystem::DrawSprite(size_t entity, ComponentPool<TransformComponent>& transformPool, ComponentPool<SpriteComponent>& spritePool)
{
	auto& transform = transformPool.Get(entity);
	auto& spriteComp = spritePool.Get(entity);

	if (!spriteComp.visible || !spriteComp.texture)
		return;

	auto sprite = spriteComp.CreateSprite();
	sprite.setPosition(transform.position);
	_window.draw(sprite);
}

void RenderSystem::RenderColliders(World& world)
{
	auto* transformPool = world.GetPool<TransformComponent>();
	auto* boxPool = world.GetPool<BoxColliderComponent>();
	auto* circleColliderPool = world.GetPool<CircleColliderComponent>();

	if (!transformPool)
		return;

	if (boxPool)
	{
		for (auto entity : boxPool->GetDenseEntities())
		{
			if (!transformPool->Has(entity)) continue;

			auto& transform = transformPool->Get(entity);
			auto& box = boxPool->Get(entity);

			sf::RectangleShape rect;
			rect.setSize(box.size);
			rect.setPosition(transform.position);
			rect.setFillColor(sf::Color::Transparent);
			rect.setOutlineColor(sf::Color::Green);
			rect.setOutlineThickness(2.f);
			_window.draw(rect);
		}
	}

	if (circleColliderPool)
	{
		for (auto entity : circleColliderPool->GetDenseEntities())
		{
			if (!transformPool->Has(entity)) continue;

			auto& transform = transformPool->Get(entity);
			auto& circle = circleColliderPool->Get(entity);

			sf::CircleShape shape;
			shape.setRadius(circle.radius);
			shape.setPosition(transform.position);
			shape.setFillColor(sf::Color::Transparent);
			shape.setOutlineColor(sf::Color::Red);
			shape.setOutlineThickness(2.f);
			_window.draw(shape);
		}
	}
}

void RenderSystem::RenderGrid()
{
	sf::Vector2u windowSize = _window.getSize();

	for (int x = 0; x < static_cast<int>(windowSize.x); x += TILE_SIZE)
	{
		sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(static_cast<float>(x), 0.f), sf::Color(100, 100, 100, 100)),
			sf::Vertex(sf::Vector2f(static_cast<float>(x), static_cast<float>(windowSize.y)), sf::Color(100, 100, 100, 100))
		};
		_window.draw(line, 2, sf::PrimitiveType::Lines);
	}

	for (int y = 0; y < static_cast<int>(windowSize.y); y += TILE_SIZE)
	{
		sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(0.f, static_cast<float>(y)), sf::Color(100, 100, 100, 100)),
			sf::Vertex(sf::Vector2f(static_cast<float>(windowSize.x), static_cast<float>(y)), sf::Color(100, 100, 100, 100))
		};
		_window.draw(line, 2, sf::PrimitiveType::Lines);
	}
}