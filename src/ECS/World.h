#pragma once

#include "EntityId.h"
#include "ComponentPool.h"
#include "ComponentType.h"

#include <memory>
#include <unordered_map>
#include <vector>
#include <queue>

class World
{
public:

	EntityId CreateEntity()
	{
		EntityId entity;

		entity.id = nextEntityId++;

		entities.push_back(entity);

		return entity;
	}

	void DestroyEntity(EntityId entity)
	{
		for (auto& [id, pool] : componentPools)
			pool->Remove(entity.id);

		entities.erase(
			std::remove_if(
				entities.begin(),
				entities.end(),
				[&](const EntityId& e)
				{
					return e.id == entity.id;
				}),
			entities.end());

	}

	template<typename T>
	void AddComponent(EntityId entity, const T& component)
	{
		auto pool = GetOrCreatePool<T>();

		pool->Add(entity.id, component);

		// entity.mask.set(GetComponentTypeId<T>(), true);

		entities[entity.id].mask.set(GetComponentTypeId<T>());
	}

	template<typename T>
	void RemoveComponent(EntityId entity)
	{
		for (auto& [id, pool] : componentPools)
			pool->Remove(entity.id);

		entities[entity.id].mask.reset(GetComponentTypeId<T>());
	}

	template<typename T>
	bool HasComponent(EntityId entity)
	{
		auto pool = GetOrCreatePool<T>();

		return pool->Has(entity.id);
	}

	template<typename T>
	T& GetComponent(EntityId entity)
	{
		auto pool = GetOrCreatePool<T>();

		return pool->Get(entity.id);
	}

	template<typename T>
	ComponentPool<T>* GetPool()
	{
		size_t type = GetComponentTypeId<T>();

		auto it = componentPools.find(type);

		if (it == componentPools.end())
			return nullptr;

		return static_cast<ComponentPool<T>*>(it->second.get());
	}

	IComponentPool* GetPoolByType(size_t type)
	{
		auto it =
			componentPools.find(type);

		if (it == componentPools.end())
			return nullptr;

		return it->second.get();
	}

	void DestroyEntityById(size_t entityId)
	{
		for (auto& [id, pool]
			: componentPools)
		{
			pool->Remove(entityId);
		}

		entities[entityId].mask.reset();

		entities[entityId].generation++;

		freeEntities.push(entityId);
	}

	const std::vector<EntityId>& GetEntities() const
	{
		return entities;
	}

	const EntityId& GetEntity(uint32_t id) const
	{
		return entities[id];
	}

	bool IsAlive(const EntityId& entity) const
	{
		if (entity.id >= entities.size())
		{
			return false;
		}

		return entities[entity.id].generation == entity.generation;
	}


private:

	template<typename T>
	ComponentPool<T>* GetOrCreatePool()
	{
		size_t type = GetComponentTypeId<T>();

		auto it = componentPools.find(type);

		if (it == componentPools.end())
		{
			auto pool = std::make_unique<ComponentPool<T>>();

			auto* ptr = pool.get();

			componentPools[type] = std::move(pool);

			return ptr;
		}

		return static_cast<ComponentPool<T>*>(it->second.get());
	}

private:

	uint32_t nextEntityId = 0;

	std::vector<EntityId> entities;
	std::queue<uint32_t> freeEntities;

	std::unordered_map<size_t, std::unique_ptr<IComponentPool>> componentPools;
};