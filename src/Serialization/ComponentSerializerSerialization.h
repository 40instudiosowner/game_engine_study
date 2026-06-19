#pragma once

#include "ComponentSerializer.h"
#include "ComponentsMetadata.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#include <type_traits>

template<typename T>
void ComponentSerializer::Serialized(const T& component, SerializedComponent& out)
{
    ForEachField<T>([&](auto&& field) {
        SerializedField sf;
        sf.Name = field.name;
        sf.Value = AnyFromField(component.*(field.member));
        out.Fields.push_back(std::move(sf));
    });
}

template<typename T>
void ComponentSerializer::Deserialized(const SerializedComponent& in, T& component)
{
    for (const auto& field : in.Fields)
    {
        ForEachField<T>([&](auto&& f) {
            if (f.name == field.Name)
            {
                FieldFromAny(component.*(f.member), field.Value);
            }
        });
    }
}

template<typename U>
std::any ComponentSerializer::AnyFromField(const U& val)
{
    if constexpr (std::is_enum_v<U>)
        return std::any(static_cast<int>(val));
    else if constexpr (std::is_same_v<U, sf::Vector2f>)
        return std::any(val);
    else if constexpr (std::is_same_v<U, sf::IntRect>)
        return std::any(val);
    else if constexpr (std::is_same_v<U, sf::Color>)
        return std::any(val);
    else if constexpr (std::is_arithmetic_v<U> || std::is_same_v<U, std::string>)
        return std::any(val);
    else
        return std::any();
}

template<typename U>
void ComponentSerializer::FieldFromAny(U& val, const std::any& a)
{
    if (!a.has_value()) return;
    if constexpr (std::is_enum_v<U>)
    {
        if (a.type() == typeid(int))
            val = static_cast<U>(std::any_cast<int>(a));
    }
    else if constexpr (std::is_same_v<U, sf::Vector2f>)
    {
        if (a.type() == typeid(sf::Vector2f))
            val = std::any_cast<sf::Vector2f>(a);
    }
    else if constexpr (std::is_same_v<U, sf::IntRect>)
    {
        if (a.type() == typeid(sf::IntRect))
            val = std::any_cast<sf::IntRect>(a);
    }
    else if constexpr (std::is_same_v<U, sf::Color>)
    {
        if (a.type() == typeid(sf::Color))
            val = std::any_cast<sf::Color>(a);
    }
    else if constexpr (std::is_same_v<U, int>)
    {
        if (a.type() == typeid(int))
            val = std::any_cast<int>(a);
    }
    else if constexpr (std::is_same_v<U, float>)
    {
        if (a.type() == typeid(float))
            val = std::any_cast<float>(a);
    }
    else if constexpr (std::is_same_v<U, double>)
    {
        if (a.type() == typeid(double))
            val = std::any_cast<double>(a);
    }
    else if constexpr (std::is_same_v<U, bool>)
    {
        if (a.type() == typeid(bool))
            val = std::any_cast<bool>(a);
    }
    else if constexpr (std::is_same_v<U, std::string>)
    {
        if (a.type() == typeid(std::string))
            val = std::any_cast<std::string>(a);
    }
    else if constexpr (std::is_same_v<U, uint32_t>)
    {
        if (a.type() == typeid(uint32_t))
            val = std::any_cast<uint32_t>(a);
    }
}