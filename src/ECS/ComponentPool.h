#pragma once

#include "IComponentPool.h"

#include <vector>
#include <cassert>

template<typename T>
class ComponentPool final : public IComponentPool
{
public:

	void Add(size_t entity, const T& component)
	{
		if (Has(entity))
			return;

		if (entity >= sparse.size())
			sparse.resize(entity + 1, INVALID_INDEX);

		sparse[entity] = denseEntities.size();

		denseEntities.push_back(entity);
		denseComponents.push_back(component);
	}

	void Remove(size_t entity) override
	{
		if (!Has(entity))
			return;

		size_t denseIndex = sparse[entity];
		size_t lastIndex = denseEntities.size() - 1;

		denseEntities[denseIndex] = denseEntities[lastIndex];
		denseComponents[denseIndex] = denseComponents[lastIndex];

		sparse[denseEntities[denseIndex]] = denseIndex;

		denseEntities.pop_back();
		denseComponents.pop_back();

		sparse[entity] = INVALID_INDEX;
	}

	bool Has(size_t entity) const override
	{
		if (entity >= sparse.size())
			return false;

		return sparse[entity] != INVALID_INDEX;
	}

	T& Get(size_t entity)
	{
		assert(Has(entity));

		return denseComponents[sparse[entity]];
	}

	const std::vector<size_t>& GetDenseEntities() const
	{
		return denseEntities;
	}

	const std::vector<T>& GetDenseComponents() const
	{
		return denseComponents;
	}

	size_t Size() const override
	{
		return denseEntities.size();
	}

	const std::vector<size_t>&
		GetEntities() const override
	{
		return denseEntities;
	}


private:

	static constexpr size_t INVALID_INDEX = SIZE_MAX;

	std::vector<size_t> sparse;

	std::vector<size_t> denseEntities;

	std::vector<T> denseComponents;
};