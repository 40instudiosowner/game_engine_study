#pragma once

#include "../ECS/Systems/IUpdateSystem.h"

class CollisionSystem final : public IUpdateSystem
{
public:

	void Update(World& world, float dt) override;
};