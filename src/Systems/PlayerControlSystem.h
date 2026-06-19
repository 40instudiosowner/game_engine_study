#pragma once

#include "../ECS/Systems/IUpdateSystem.h"
#include "../InputManager.h"

class PlayerControlSystem final : public IUpdateSystem
{
public:
	explicit PlayerControlSystem(InputManager& inputManager);

	void Update(World& world, float dt) override;

private:
	InputManager& _inputManager;
};