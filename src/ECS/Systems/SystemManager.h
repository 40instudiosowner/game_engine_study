#pragma once

#include "IInitSystem.h"
#include "IUpdateSystem.h"

#include <memory>
#include <vector>
#include <cstdint>

class SystemManager
{
public:

	template<typename T, typename... Args>
	T* AddSystem(Args&&... args)
	{
		auto system = std::make_unique<T>(std::forward<Args>(args)...);

		auto* ptr = system.get();

		if constexpr (std::is_base_of_v<IInitSystem, T>)
			initSystems.push_back(ptr);

		if constexpr (std::is_base_of_v<IUpdateSystem, T>)
			updateSystems.push_back(ptr);

		systems.push_back(std::move(system));

		return ptr;
	}

	void Init(World& world)
	{
		for (auto* system : initSystems)
			system->Init(world);
	}

	void Update(World& world, float dt)
	{
		for (auto* system : updateSystems)
			system->Update(world, dt);
	}

	template<typename T>
	T* GetSystem()
	{
		for (auto& sys : systems)
		{
			auto* ptr = dynamic_cast<T*>(sys.get());
			if (ptr)
				return ptr;
		}
		return nullptr;
	}

private:

	std::vector<std::unique_ptr<void, void(*)(void*)>> dummy;

	std::vector<std::unique_ptr<IUpdateSystem>> systems;

	std::vector<IInitSystem*> initSystems;

	std::vector<IUpdateSystem*> updateSystems;
};