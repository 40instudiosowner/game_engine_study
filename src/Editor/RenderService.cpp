#include "RenderService.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <cmath>

namespace Editor
{

RenderService::RenderService(sf::RenderWindow& window, const sf::Font& baseFont)
    : _window(window)
    , _baseFont(baseFont)
    , _gridText(baseFont, "", 10)
{
}

sf::Vector2f RenderService::SnapToGrid(const sf::Vector2f& pos)
{
    float gx = std::floor(pos.x / GridSize.x) * GridSize.x;
    float gy = std::floor(pos.y / GridSize.y) * GridSize.y;
    return {gx, gy};
}

void RenderService::Clear(const sf::Color& color) const
{
    _window.clear(color);
}

void RenderService::SetView(const sf::View& view) const
{
    _window.setView(view);
}

void RenderService::DrawLine(const sf::Vector2f& p1, const sf::Vector2f& p2) const
{
    sf::Vertex line[] = {
        sf::Vertex(p1, sf::Color(100, 100, 100, 100)),
        sf::Vertex(p2, sf::Color(100, 100, 100, 100))
    };
    _window.draw(line, 2, sf::PrimitiveType::Lines);
}

void RenderService::RenderGrid(float currentZoom)
{
    // Get visible world area from current view
    sf::View view = _window.getView();
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();

    sf::Vector2f topLeft = viewCenter - viewSize / 2.f;
    sf::Vector2f bottomRight = viewCenter + viewSize / 2.f;

    // Adjust grid step based on zoom: minimum 4px screen distance
    int step = GridSize.x;
    if (step * currentZoom < 4.f)
        step = static_cast<int>(4.f / currentZoom);
    if (step < 1) step = 1;

    // Round to grid
    float startX = std::floor(topLeft.x / step) * step;
    float startY = std::floor(topLeft.y / step) * step;

    for (float x = startX; x <= bottomRight.x; x += step)
    {
        DrawLine(
            sf::Vector2f(x, topLeft.y),
            sf::Vector2f(x, bottomRight.y));
    }

    for (float y = startY; y <= bottomRight.y; y += step)
    {
        DrawLine(
            sf::Vector2f(topLeft.x, y),
            sf::Vector2f(bottomRight.x, y));
    }
}

void RenderService::RenderColliders(const BoxColliderComponent& bbox, sf::Vector2f& position) const
{
    sf::RectangleShape rect;
    rect.setSize(bbox.size);
    rect.setPosition(position + bbox.offset);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Green);
    rect.setOutlineThickness(2.f);
    _window.draw(rect);
}

void RenderService::RenderTexture(const sf::Sprite& sprite) const
{
    _window.draw(sprite);
}

}
