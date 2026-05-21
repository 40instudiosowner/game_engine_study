#pragma once

class World;

class IUpdateSystem
{
public:
	virtual ~IUpdateSystem() = default;

	virtual void Update(World& world, float dt) = 0;
};