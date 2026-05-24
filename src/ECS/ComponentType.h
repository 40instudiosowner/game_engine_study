#pragma once

#include <cstddef>

inline size_t GetUniqueComponentTypeId()
{
	static size_t lastId = 0;
	return lastId++;
}

template<typename T>
inline size_t GetComponentTypeId()
{
	static size_t typeId = GetUniqueComponentTypeId();
	return typeId;
}