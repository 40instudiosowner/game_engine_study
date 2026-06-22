#include "JsonSerializer.h"

#include <iostream>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>

json JsonSerializer::SerializeAny(const std::any& value)
{
    if (!value.has_value())
        return nullptr;

    if (value.type() == typeid(int))
        return json{{"type", "int"}, {"value", std::any_cast<int>(value)}};
    if (value.type() == typeid(float))
        return json{{"type", "float"}, {"value", std::any_cast<float>(value)}};
    if (value.type() == typeid(double))
        return json{{"type", "double"}, {"value", std::any_cast<double>(value)}};
    if (value.type() == typeid(bool))
        return json{{"type", "bool"}, {"value", std::any_cast<bool>(value)}};
    if (value.type() == typeid(uint32_t))
        return json{{"type", "uint32"}, {"value", std::any_cast<uint32_t>(value)}};
    if (value.type() == typeid(std::string))
        return json{{"type", "string"}, {"value", std::any_cast<std::string>(value)}};
    if (value.type() == typeid(sf::Vector2f))
    {
        auto v = std::any_cast<sf::Vector2f>(value);
        return json{{"type", "vec2f"}, {"value", {v.x, v.y}}};
    }
    if (value.type() == typeid(sf::IntRect))
    {
        auto r = std::any_cast<sf::IntRect>(value);
        return json{{"type", "intrect"}, {"value", {{"left", r.position.x}, {"top", r.position.y}, {"width", r.size.x}, {"height", r.size.y}}}};
    }
    if (value.type() == typeid(sf::Color))
    {
        auto c = std::any_cast<sf::Color>(value);
        return json{{"type", "color"}, {"value", {c.r, c.g, c.b, c.a}}};
    }

    return nullptr;
}

std::any JsonSerializer::DeserializeAny(const json& j)
{
    if (j.is_null() || !j.contains("type"))
        return {};

    const std::string& type = j["type"];
    const auto& val = j["value"];

    if (type == "int")
        return std::any(static_cast<int>(val));
    if (type == "float")
        return std::any(static_cast<float>(val));
    if (type == "double")
        return std::any(static_cast<double>(val));
    if (type == "bool")
        return std::any(static_cast<bool>(val));
    if (type == "uint32")
        return std::any(static_cast<uint32_t>(val));
    if (type == "string")
        return std::any(val.get<std::string>());
    if (type == "vec2f")
        return std::any(sf::Vector2f(
            static_cast<float>(val[0]),
            static_cast<float>(val[1])));
    if (type == "intrect")
        return std::any(sf::IntRect(
            sf::Vector2i(static_cast<int>(val["left"]), static_cast<int>(val["top"])),
            sf::Vector2i(static_cast<int>(val["width"]), static_cast<int>(val["height"]))));
    if (type == "color")
        return std::any(sf::Color(
            static_cast<uint8_t>(val[0]),
            static_cast<uint8_t>(val[1]),
            static_cast<uint8_t>(val[2]),
            static_cast<uint8_t>(val[3])));

    return {};
}

json JsonSerializer::SerializeField(const SerializedField& field)
{
    return json{
        {"name", field.Name},
        {"value", SerializeAny(field.Value)}
    };
}

SerializedField JsonSerializer::DeserializeField(const json& j)
{
    SerializedField field;
    field.Name = j["name"];
    field.Value = DeserializeAny(j["value"]);
    return field;
}

json JsonSerializer::SerializeComponent(const SerializedComponent& comp)
{
    json j;
    j["type_name"] = comp.Type.name();
    j["type_hash"] = comp.Type.hash_code();
    for (const auto& field : comp.Fields)
        j["fields"].push_back(SerializeField(field));
    return j;
}

SerializedComponent JsonSerializer::DeserializeComponent(const json& j)
{
    SerializedComponent comp;
    size_t hash = j["type_hash"].get<size_t>();
    auto& types = _serializer.SerializableTypes();
    auto it = types.find(hash);
    if (it != types.end())
        comp.Type = it->second;
    for (const auto& field : j["fields"])
        comp.Fields.push_back(DeserializeField(field));
    return comp;
}

json JsonSerializer::SerializeEntity(const SerializedEntity& entity)
{
    json j = json::array();
    for (const auto& [type, comp] : entity.Components)
    {
        json compJson;
        compJson["type_name"] = type.name();
        compJson["type_hash"] = type.hash_code();
        for (const auto& field : comp.Fields)
            compJson["fields"].push_back(SerializeField(field));
        j.push_back(compJson);
    }
    return j;
}

SerializedEntity JsonSerializer::DeserializeEntity(const json& j)
{
    SerializedEntity entity;
    for (const auto& compJson : j)
    {
        SerializedComponent comp;
        size_t hash = compJson["type_hash"].get<size_t>();
        auto& types = _serializer.SerializableTypes();
        auto it = types.find(hash);
        if (it != types.end())
            comp.Type = it->second;
        for (const auto& field : compJson["fields"])
            comp.Fields.push_back(DeserializeField(field));
        entity.Components[comp.Type] = std::move(comp);
    }
    return entity;
}

json JsonSerializer::SerializeWorld(const World& world)
{
    json j = json::array();

    auto& serializer = _serializer;
    const auto& serializableTypes = serializer.SerializableTypes();

    for (const auto& entity : world.GetEntities())
    {
        json entityJson;
        entityJson["id"] = entity.id;
        entityJson["generation"] = entity.generation;

        for (const auto& [hash, typeIdx] : serializableTypes)
        {
            SerializedComponent comp;
            if (serializer.Serialize(static_cast<int>(entity.id), typeIdx, comp))
            {
                entityJson["components"].push_back(SerializeComponent(comp));
            }
        }

        if (entityJson.contains("components") && !entityJson["components"].empty())
            j.push_back(entityJson);
    }

    return j;
}

void JsonSerializer::DeserializeWorld(const json& j, World& world)
{
    auto& serializer = _serializer;

    for (const auto& entityJson : j)
    {
        size_t entityId = entityJson["id"];

        // Ensure the entity slot exists in the new world
        while (entityId >= world.GetEntities().size())
            world.CreateEntity();

        for (const auto& compJson : entityJson["components"])
        {
            SerializedComponent comp = DeserializeComponent(compJson);
            serializer.Deserialize(static_cast<int>(entityId), comp);
        }
    }
}

void JsonSerializer::SaveWorldToFile(const World& world)
{
    json j = SerializeWorld(world);
    std::ofstream file(_filePath);
    if (file.is_open())
    {
        file << j.dump(2);
        std::cout << "World saved to " << _filePath << "\n";
    }
    else
    {
        std::cerr << "Failed to save world to " << _filePath << "\n";
    }
}

void JsonSerializer::LoadWorldFromFile(World& world)
{
    std::ifstream file(_filePath);
    if (!file.is_open())
    {
        std::cerr << "Failed to load world from " << _filePath << "\n";
        return;
    }

    json j;
    file >> j;
    DeserializeWorld(j, world);
    std::cout << "World loaded from " << _filePath << "\n";
}