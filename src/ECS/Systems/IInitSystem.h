#pragma once

class World;

class IInitSystem
{
public:
	virtual ~IInitSystem() = default;

	virtual void Init(World& world) = 0;
};