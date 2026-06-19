#pragma once

#include <vector>
#include <functional>
#include <unordered_map>

#include "ComponentSerializerFuncs.h"

class ComponentSerializer;

class IComponentSerializer
{
public:
    using LateRegistrationFunc = std::function<void(ComponentSerializer&)>;

    virtual bool Serialize(int entity, const std::type_index& type, SerializedComponent& out) = 0;
    virtual bool Deserialize(int entity, const SerializedComponent& in) = 0;
    virtual const std::unordered_map<size_t, std::type_index>& SerializableTypes() const = 0;

    static void AddLateRegistration(LateRegistrationFunc func)
    {
        LateRegistrators().push_back(std::move(func));
    }

protected:
    virtual ~IComponentSerializer() = default;

    static std::vector<LateRegistrationFunc>& LateRegistrators()
    {
        static std::vector<LateRegistrationFunc> late;
        return late;
    }
};
