#pragma once

#include "World.h"
#include "ComponentType.h"

#include <vector>

class Filter
{
public:

	class Iterator
	{
	public:

		Iterator(
			Filter* filter,
			size_t index)
			:
			_filter(filter),
			_index(index)
		{
			SkipInvalid();
		}

		size_t operator*() const
		{
			return _filter->_sourceEntities[_index];
		}

		Iterator& operator++()
		{
			++_index;

			SkipInvalid();

			return *this;
		}

		bool operator!=(const Iterator& other) const
		{
			return _index != other._index;
		}

	private:

		void SkipInvalid()
		{
			while (_index < _filter->_sourceEntities.size())
			{
				size_t entity =
					_filter->_sourceEntities[_index];

				if (_filter->Matches(entity))
					return;

				++_index;
			}
		}

	private:

		Filter* _filter;

		size_t _index;
	};

public:

	explicit Filter(World& world)
		: _world(world)
	{
	}

	void AddRequiredComponent(size_t component)
	{
		_required.push_back(component);
		_requiredMask.set(component);
	}

	void Build()
	{
		size_t minSize = SIZE_MAX;

		IComponentPool* smallest = nullptr;

		for (auto component : _required)
		{
			auto* pool =
				_world.GetPoolByType(component);

			if (!pool)
				continue;

			size_t size =
				pool->Size();

			if (size < minSize)
			{
				minSize = size;

				smallest = pool;
			}
		}

		if (smallest)
		{
			_sourceEntities = smallest->GetEntities();
		}
	}

	Iterator begin()
	{
		return Iterator(this, 0);
	}

	Iterator end()
	{
		return Iterator(
			this,
			_sourceEntities.size());
	}

private:

	bool Matches(size_t entity)
	{
		const auto& entityData =
			_world.GetEntity(
				static_cast<uint32_t>(entity));

		return
			(
				entityData.mask &
				_requiredMask
			)
			==
			_requiredMask;
	}

private:

	World& _world;

	std::vector<size_t> _required;
	std::bitset<MAX_COMPONENTS> _requiredMask;

	std::vector<size_t> _sourceEntities;
};