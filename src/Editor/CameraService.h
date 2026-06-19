#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

namespace Editor
{

class CameraService
{
    static constexpr sf::Vector2f DragSpeed = {500.f, 500.f};
    static constexpr float ZoomStep = 1.1f;
    static constexpr float ZoomMin = 0.5f;
    static constexpr float ZoomMax = 3.f;

    sf::RenderWindow& _window;
    sf::View _view;
    bool _isMoving = false;
    sf::Vector2i _lastMousePos;

public:
    explicit CameraService(sf::RenderWindow& window);

    const sf::View& View() const { return _view; }

    sf::Vector2f WindowToWorldPosition(const sf::Vector2i& mousePos) const;

    void StartMoving(const sf::Vector2i& mousePos);
    void StopMoving();
    void Drag(const sf::Vector2i& mousePos, float deltaTime);

    void Zoom(float scrollDelta, const sf::Vector2i& mousePos);
};

}
