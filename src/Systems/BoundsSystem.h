#pragma once

#include "../ECS/Systems/IUpdateSystem.h"

class BoundsSystem final : public IUpdateSystem
{
public:

	void Update(World& world, float dt) override;
};