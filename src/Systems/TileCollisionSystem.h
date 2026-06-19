#pragma once

#include "../ECS/Systems/IUpdateSystem.h"

class TileCollisionSystem final : public IUpdateSystem
{
public:
	void Update(World& world, float dt) override;
};