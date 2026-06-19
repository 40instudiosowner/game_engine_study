#include "BrickSystem.h"
#include "../Components/BrickComponent.h"
#include "../Components/TileComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/BulletComponent.h"
#include "../Components/FinishComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/AnimationStateComponent.h"
#include "../Components/AnimatorComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/MovementComponent.h"
#include "../ECS/World.h"
#include <algorithm>

BrickSystem::BrickSystem(AssetManager& assets, sf::RenderWindow& window,
	GameState& gameState, std::function<void(const std::string&)> sceneChangeCallback)
	: _assets(assets), _window(window), _gameState(gameState), _sceneChangeCallback(sceneChangeCallback)
{
}

void BrickSystem::Update(World& world, float)
{
	auto* collisionPool = world.GetPool<CollisionComponent>();
	auto* tilePool = world.GetPool<TileComponent>();
	auto* playerPool = world.GetPool<PlayerComponent>();
	auto* bulletPool = world.GetPool<BulletComponent>();
	auto* transformPool = world.GetPool<TransformComponent>();
	auto* finishPool = world.GetPool<FinishComponent>();

	if (!collisionPool || !tilePool || !transformPool)
		return;

	std::vector<uint32_t> entitiesToDestroy;

	for (auto entity : tilePool->GetDenseEntities())
	{
		if (!collisionPool->Has(entity))
			continue;

		auto& tile = tilePool->Get(entity);
		auto& collisions = collisionPool->Get(entity).collidedEntities;

		if (tile.type == TileType::Brick)
		{
			for (auto other : collisions)
			{
				if (bulletPool && bulletPool->Has(other))
				{
					auto explosion = world.CreateEntity();
					TransformComponent expTransform;
					if (transformPool->Has(entity))
						expTransform = transformPool->Get(entity);
					world.AddComponent(explosion, expTransform);

					SpriteComponent expSprite;
					const auto* explosionAnim = _assets.GetAnimation(AssetManager::ANIM_EXPLOSION);
					if (explosionAnim)
					{
						expSprite.texture = explosionAnim->texture;
						expSprite.textureRect = sf::IntRect({{0, 0}, {explosionAnim->frameWidth, explosionAnim->frameHeight}});
						expSprite.origin = {explosionAnim->frameWidth / 2.f, explosionAnim->frameHeight / 2.f};
					}
					world.AddComponent(explosion, expSprite);

					AnimationStateComponent expState;
					expState.currentAnimation = AssetManager::ANIM_EXPLOSION;
					expState.nextAnimation = AssetManager::ANIM_EXPLOSION;
					expState.isLooping = false;
					world.AddComponent(explosion, expState);

					AnimatorComponent expAnimator;
					if (explosionAnim)
						expAnimator.animations[AssetManager::ANIM_EXPLOSION] = *explosionAnim;
					world.AddComponent(explosion, expAnimator);

					entitiesToDestroy.push_back(static_cast<uint32_t>(entity));
					entitiesToDestroy.push_back(static_cast<uint32_t>(other));
				}

				if (playerPool && playerPool->Has(other))
				{
					auto& playerTransform = transformPool->Get(other);
					auto& brickTransform = transformPool->Get(entity);

					if (playerTransform.position.y > brickTransform.position.y)
					{
						auto explosion = world.CreateEntity();
						TransformComponent expTransform;
						expTransform = brickTransform;
						world.AddComponent(explosion, expTransform);

						SpriteComponent expSprite;
						const auto* explosionAnim = _assets.GetAnimation(AssetManager::ANIM_EXPLOSION);
						if (explosionAnim)
						{
							expSprite.texture = explosionAnim->texture;
							expSprite.textureRect = sf::IntRect({{0, 0}, {explosionAnim->frameWidth, explosionAnim->frameHeight}});
							expSprite.origin = {explosionAnim->frameWidth / 2.f, explosionAnim->frameHeight / 2.f};
						}
						world.AddComponent(explosion, expSprite);

						AnimationStateComponent expState;
						expState.currentAnimation = AssetManager::ANIM_EXPLOSION;
						expState.nextAnimation = AssetManager::ANIM_EXPLOSION;
						expState.isLooping = false;
						world.AddComponent(explosion, expState);

						AnimatorComponent expAnimator;
						if (explosionAnim)
							expAnimator.animations[AssetManager::ANIM_EXPLOSION] = *explosionAnim;
						world.AddComponent(explosion, expAnimator);

						entitiesToDestroy.push_back(static_cast<uint32_t>(entity));

						auto* movementPool = world.GetPool<MovementComponent>();
						if (movementPool && movementPool->Has(other))
						{
							auto& movement = movementPool->Get(other);
							movement.direction.y = 0.f;
						}
					}
				}
			}
		}

		if (tile.type == TileType::Question)
		{
			for (auto other : collisions)
			{
				if (playerPool && playerPool->Has(other) && transformPool->Has(entity))
				{
					auto& questionTransform = transformPool->Get(entity);
					if (transformPool->Has(other))
					{
						auto& playerTransform = transformPool->Get(other);
						if (playerTransform.position.y > questionTransform.position.y)
						{
							const auto* inactiveTex = _assets.GetTexture(AssetManager::TEX_QUESTION_INACTIVE);
							auto* spritePool = world.GetPool<SpriteComponent>();
							if (inactiveTex && spritePool && spritePool->Has(entity))
							{
								auto& sprite = spritePool->Get(entity);
								sprite.texture = inactiveTex;
								sprite.textureRect = sf::IntRect({{0, 0}, sf::Vector2i(inactiveTex->getSize())});
							}

							auto coin = world.CreateEntity();
							TransformComponent coinTransform;
							coinTransform.position = {
								questionTransform.position.x,
								questionTransform.position.y - 64.f
							};
							world.AddComponent(coin, coinTransform);

							SpriteComponent coinSprite;
							const auto* coinAnim = _assets.GetAnimation(AssetManager::ANIM_COIN);
							if (coinAnim)
							{
								coinSprite.texture = coinAnim->texture;
								coinSprite.textureRect = sf::IntRect({{0, 0}, {coinAnim->frameWidth, coinAnim->frameHeight}});
								coinSprite.origin = {coinAnim->frameWidth / 2.f, coinAnim->frameHeight / 2.f};
							}
							world.AddComponent(coin, coinSprite);

							AnimationStateComponent coinState;
							coinState.currentAnimation = AssetManager::ANIM_COIN;
							coinState.nextAnimation = AssetManager::ANIM_COIN;
							coinState.isLooping = false;
							world.AddComponent(coin, coinState);

							AnimatorComponent coinAnimator;
							if (coinAnim)
								coinAnimator.animations[AssetManager::ANIM_COIN] = *coinAnim;
							world.AddComponent(coin, coinAnimator);

							tile.type = TileType::Normal;
							_gameState.score++;
						}
					}
				}
			}
		}
	}

	for (auto id : entitiesToDestroy)
		world.DestroyEntityById(id);

	// Destroy finished non-looping animations
	auto* statePool = world.GetPool<AnimationStateComponent>();
	if (statePool)
	{
		std::vector<uint32_t> finishedAnims;
		for (auto entity : statePool->GetDenseEntities())
		{
			auto& state = statePool->Get(entity);
			if (state.isFinished && !state.isLooping)
			{
				if (!tilePool->Has(entity) && !playerPool->Has(entity))
					finishedAnims.push_back(static_cast<uint32_t>(entity));
			}
		}
		for (auto id : finishedAnims)
			world.DestroyEntityById(id);
	}

	// Check finish collision
	if (playerPool && finishPool && collisionPool)
	{
		for (auto finish : finishPool->GetDenseEntities())
		{
			if (!collisionPool->Has(finish))
				continue;

			auto& finishCollisions = collisionPool->Get(finish).collidedEntities;
			for (auto other : finishCollisions)
			{
				if (playerPool->Has(other))
				{
					if (_sceneChangeCallback)
						_sceneChangeCallback("menu");
					return;
				}
			}
		}
	}
}