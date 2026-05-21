#pragma once

#include "../ECS/Systems/IUpdateSystem.h"

class InputSystem final : public IUpdateSystem
{
public:

	void Update(World& world, float dt) override;
};