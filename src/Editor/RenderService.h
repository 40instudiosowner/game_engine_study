#pragma once

#include <SFML/Graphics.hpp>
#include "../Components/BoxColliderComponent.h"

namespace Editor
{

class RenderService
{
    static constexpr sf::Vector2i GridSize = {64, 64};

    sf::RenderWindow& _window;
    const sf::Font& _baseFont;
    sf::Text _gridText;

    void DrawLine(const sf::Vector2f& p1, const sf::Vector2f& p2) const;

public:
    RenderService(sf::RenderWindow& window, const sf::Font& baseFont);

    static sf::Vector2f SnapToGrid(const sf::Vector2f& pos);

    void Clear(const sf::Color& color) const;
    void SetView(const sf::View& view) const;
    void RenderColliders(const BoxColliderComponent& bbox, sf::Vector2f& position) const;
    void RenderGrid(float currentZoom);
    void RenderTexture(const sf::Sprite& sprite) const;
};

}
