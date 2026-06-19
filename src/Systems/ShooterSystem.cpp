#include "ShooterSystem.h"
#include "../Components/PlayerComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/BulletComponent.h"
#include "../ECS/World.h"
#include "../ECS/FilterBuilder.h"

ShooterSystem::ShooterSystem(GameState& gameState, AssetManager& assets)
	: _gameState(gameState), _assets(assets)
{
}

void ShooterSystem::Update(World& world, float dt)
{
	_cooldown -= dt;

	auto filter = FilterBuilder(world)
		.With<PlayerComponent>()
		.With<TransformComponent>()
		.Build();

	auto* playerPool = world.GetPool<PlayerComponent>();
	auto* transformPool = world.GetPool<TransformComponent>();

	if (!playerPool || !transformPool)
		return;

	for (auto entity : filter)
	{
		auto& player = playerPool->Get(entity);

		if (!player.wantsShoot)
			continue;

		player.wantsShoot = false;

		if (_cooldown > 0.f)
			continue;

		_cooldown = 0.25f;

		auto& playerTransform = transformPool->Get(entity);

		auto bullet = world.CreateEntity();

		// Position
		TransformComponent bulletTransform;
		float offsetX = player.facingRight ? 28.f : -28.f;
		bulletTransform.position = {
			playerTransform.position.x + offsetX,
			playerTransform.position.y
		};
		world.AddComponent(bullet, bulletTransform);

		// Sprite
		SpriteComponent bulletSprite;
		const auto* bulletTex = _assets.GetTexture("Bullet");
		if (bulletTex)
		{
			bulletSprite.texture = bulletTex;
			auto texSize = bulletTex->getSize();
			bulletSprite.textureRect = sf::IntRect({{0, 0}, sf::Vector2i(static_cast<int>(texSize.x), static_cast<int>(texSize.y))});
			bulletSprite.origin = { texSize.x / 2.f, texSize.y / 2.f };
		}
		world.AddComponent(bullet, bulletSprite);

		// Movement
		MovementComponent movement;
		movement.direction = { player.facingRight ? 1.f : -1.f, 0.f };
		movement.speed = 600.f;
		world.AddComponent(bullet, movement);

		// Collider
		BoxColliderComponent collider;
		collider.size = { 8.f, 8.f };
		world.AddComponent(bullet, collider);

		world.AddComponent(bullet, CollisionComponent{});

		BulletComponent bulletComp;
		bulletComp.spawnPosition = bulletTransform.position;
		world.AddComponent(bullet, bulletComp);
	}
}