#pragma once

#include "IComponentSerializer.h"
#include "SerializedEntity.h"
#include "../ECS/World.h"

class ComponentSerializer final : public IComponentSerializer
{
    std::unordered_map<std::type_index, ComponentSerializerFuncs::SerializeFunc> _serializer;
    std::unordered_map<std::type_index, ComponentSerializerFuncs::DeserializeFunc> _deserializer;
    std::unordered_map<size_t, std::type_index> _hashToTypeIdxMap;
    World& _world;

public:
    explicit ComponentSerializer(World& world);

    template<typename T>
    void Register()
    {
        _hashToTypeIdxMap.emplace(GetComponentTypeId<T>(), std::type_index(typeid(T)));

        _serializer[std::type_index(typeid(T))] = [this](int entity, SerializedComponent& out) -> bool
        {
            if (!_world.GetPool<T>() || !_world.GetPool<T>()->Has(static_cast<size_t>(entity)))
                return false;
            auto& comp = _world.GetPool<T>()->Get(static_cast<size_t>(entity));
            out.Type = std::type_index(typeid(T));
            Serialized(comp, out);
            return true;
        };

        _deserializer[std::type_index(typeid(T))] = [this](int entity, const SerializedComponent& in) -> bool
        {
            T comp{};
            Deserialized(in, comp);
            _world.AddComponent(EntityId{ static_cast<uint32_t>(entity), 0 }, comp);
            return true;
        };
    }

    bool Serialize(int entity, const std::type_index& type, SerializedComponent& out) override;
    bool Deserialize(int entity, const SerializedComponent& in) override;

    const std::unordered_map<size_t, std::type_index>& SerializableTypes() const override
    {
        return _hashToTypeIdxMap;
    }

private:
    template<typename T>
    void Serialized(const T& component, SerializedComponent& out);

    template<typename T>
    void Deserialized(const SerializedComponent& in, T& component);

    template<typename U>
    static std::any AnyFromField(const U& val);

    template<typename U>
    static void FieldFromAny(U& val, const std::any& a);
};
