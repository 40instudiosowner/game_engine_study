#pragma once

#include "../ECS/Systems/IUpdateSystem.h"
#include "../Game/GameState.h"
#include "../AssetManager.h"

class ShooterSystem final : public IUpdateSystem
{
public:
	explicit ShooterSystem(GameState& gameState, AssetManager& assets);

	void Update(World& world, float dt) override;

private:
	GameState& _gameState;
	AssetManager& _assets;
	float _cooldown = 0.f;
};