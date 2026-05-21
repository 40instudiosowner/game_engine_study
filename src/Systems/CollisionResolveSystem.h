#pragma once

#include "../ECS/Systems/IUpdateSystem.h"
#include "../Game/GameState.h"

class CollisionResolveSystem final :
	public IUpdateSystem
{
public:

	explicit CollisionResolveSystem(GameState& gameState);

	void Update(World& world, float dt) override;

private:

	GameState& _gameState;
};