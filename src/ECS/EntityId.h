#pragma once

#include <bitset>
#include <cstdint>

constexpr size_t MAX_COMPONENTS = 64;

struct EntityId
{
	uint32_t id = 0;
	uint32_t generation = 0;

	std::bitset<MAX_COMPONENTS> mask;

	bool operator==(const EntityId& other) const
	{
		return id == other.id &&
			generation == other.generation;
	}
};