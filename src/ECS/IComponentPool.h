#pragma once

#include <vector>

class IComponentPool
{
public:

	virtual ~IComponentPool() = default;

	virtual void Remove(size_t entity) = 0;

	virtual bool Has(size_t entity) const = 0;

	virtual size_t Size() const = 0;

	virtual const std::vector<size_t>&
		GetEntities() const = 0;
};