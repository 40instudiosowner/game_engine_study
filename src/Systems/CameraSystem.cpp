#include "CameraSystem.h"
#include "../Components/DefaultCameraComponent.h"
#include "../Components/FollowXCameraComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/World.h"
#include "../ECS/FilterBuilder.h"
#include "../GameEngine.h"

CameraSystem::CameraSystem(sf::RenderWindow& window, GameEngine& engine)
	: _window(window), _engine(engine)
{
}

void CameraSystem::Update(World& world, float)
{
	auto* followPool = world.GetPool<FollowXCameraComponent>();
	auto* defaultPool = world.GetPool<DefaultCameraComponent>();
	auto* transformPool = world.GetPool<TransformComponent>();

	// FollowX camera: track X position of target entity
	if (followPool && followPool->Size() > 0)
	{
		const auto& entities = followPool->GetDenseEntities();
		for (auto entity : entities)
		{
			auto& follow = followPool->Get(entity);
			uint32_t targetId = follow.targetEntityId;

			if (targetId == 0 && transformPool && transformPool->Size() > 0)
			{
				// Auto-find first entity (player) if no target set
				targetId = static_cast<uint32_t>(transformPool->GetDenseEntities()[0]);
			}

			if (transformPool && transformPool->Has(targetId))
			{
				auto& targetTransform = transformPool->Get(targetId);

				// Camera follows player's X, Y is fixed at center
				sf::Vector2f windowCenter(
					_window.getSize().x / 2.f,
					_window.getSize().y / 2.f
				);

				sf::View view;
				view.setSize(static_cast<sf::Vector2f>(_window.getSize()));
				view.setCenter({
					targetTransform.position.x,
					windowCenter.y
				});
				_window.setView(view);
				return;
			}
		}
	}

	// Default camera (no follow-X active)
	if (defaultPool && defaultPool->Size() > 0)
	{
		_window.setView(_window.getDefaultView());
	}
}