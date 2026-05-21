#pragma once

#include "Filter.h"

class FilterBuilder
{
public:

	explicit FilterBuilder(World& world)
		:
		_filter(world)
	{
	}

	template<typename T>
	FilterBuilder& With()
	{
		_filter.AddRequiredComponent(
			GetComponentTypeId<T>());

		return *this;
	}

	Filter Build()
	{
		_filter.Build();

		return _filter;
	}

private:

	Filter _filter;
};