#pragma once

#include "../ECS/Systems/IUpdateSystem.h"

#include "../Game/GameState.h"

class GameOverSystem final :
	public IUpdateSystem
{
public:

	explicit GameOverSystem(GameState& gameState);

	void Update(World& world, float dt) override;

private:

	GameState& _gameState;
};