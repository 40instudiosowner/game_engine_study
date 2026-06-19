#pragma once

#include "../Serialization/SerializedEntity.h"
#include "../Serialization/IComponentSerializer.h"
#include "../ECS/World.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

class JsonSerializer
{
    IComponentSerializer& _serializer;
    std::string _filePath;

public:
    JsonSerializer(IComponentSerializer& serializer, const std::string& filePath)
        : _serializer(serializer), _filePath(filePath)
    {}

    json SerializeAny(const std::any& value);
    json SerializeField(const SerializedField& field);
    json SerializeComponent(const SerializedComponent& comp);
    json SerializeEntity(const SerializedEntity& entity);
    json SerializeWorld(const World& world);

    std::any DeserializeAny(const json& j);
    SerializedField DeserializeField(const json& j);
    SerializedComponent DeserializeComponent(const json& j);
    SerializedEntity DeserializeEntity(const json& j);
    void DeserializeWorld(const json& j, World& world);

    void SaveWorldToFile(const World& world);
    void LoadWorldFromFile(World& world);
};