#include "AnimationSystem.h"
#include "../Components/AnimationStateComponent.h"
#include "../Components/AnimatorComponent.h"
#include "../Components/SpriteComponent.h"
#include "../ECS/World.h"
#include "../ECS/FilterBuilder.h"

void AnimationSystem::Update(World& world, float)
{
	auto filter = FilterBuilder(world)
		.With<AnimationStateComponent>()
		.With<AnimatorComponent>()
		.With<SpriteComponent>()
		.Build();

	auto* statePool = world.GetPool<AnimationStateComponent>();
	auto* animatorPool = world.GetPool<AnimatorComponent>();
	auto* spritePool = world.GetPool<SpriteComponent>();

	if (!statePool || !animatorPool || !spritePool)
		return;

	for (auto entity : filter)
	{
		auto& state = statePool->Get(entity);
		auto& animator = animatorPool->Get(entity);
		auto& sprite = spritePool->Get(entity);

		// Check if animation needs to change
		if (!state.nextAnimation.empty() && state.nextAnimation != state.currentAnimation)
		{
			auto it = animator.animations.find(state.nextAnimation);
			if (it != animator.animations.end())
			{
				state.currentAnimation = state.nextAnimation;
				state.currentFrame = 0;
				state.frameTimer = 0;
				state.isFinished = false;

				const auto& anim = it->second;
				sprite.texture = anim.texture;
				sprite.textureRect = sf::IntRect({
					{0, 0},
					{anim.frameWidth, anim.frameHeight}
				});
				sprite.origin = {
					anim.frameWidth / 2.f,
					anim.frameHeight / 2.f
				};
			}
		}

		auto animIt = animator.animations.find(state.currentAnimation);
		if (animIt == animator.animations.end())
			continue;

		const auto& anim = animIt->second;

		// Update frame
		state.frameTimer++;

		if (state.frameTimer >= anim.frameDuration)
		{
			state.frameTimer = 0;
			state.currentFrame++;

			if (state.currentFrame >= anim.frameCount)
			{
				if (state.isLooping)
				{
					state.currentFrame = 0;
				}
				else
				{
					state.currentFrame = anim.frameCount - 1;
					state.isFinished = true;
				}
			}

			// Apply frame to sprite
			int col = state.currentFrame % anim.frameCount;
			sprite.textureRect = sf::IntRect({
				{col * anim.frameWidth, 0},
				{anim.frameWidth, anim.frameHeight}
			});
			sprite.texture = anim.texture;
		}
	}
}