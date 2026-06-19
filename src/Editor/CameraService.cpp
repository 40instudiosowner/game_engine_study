#include "CameraService.h"
#include <iostream>

namespace Editor
{

CameraService::CameraService(sf::RenderWindow& window)
    : _window(window)
    , _view(window.getDefaultView())
{
}

sf::Vector2f CameraService::WindowToWorldPosition(const sf::Vector2i& mousePos) const
{
    sf::View prev = _window.getView();
    _window.setView(_view);
    sf::Vector2f result = _window.mapPixelToCoords(mousePos);
    _window.setView(prev);
    return result;
}

void CameraService::StartMoving(const sf::Vector2i& mousePos)
{
    _isMoving = true;
    _lastMousePos = mousePos;
}

void CameraService::StopMoving()
{
    _isMoving = false;
}

void CameraService::Drag(const sf::Vector2i& mousePos, float deltaTime)
{
    if (!_isMoving)
        return;

    sf::Vector2i delta = mousePos - _lastMousePos;
    _view.move(sf::Vector2f(-delta));
    _window.setView(_view);
    _lastMousePos = mousePos;
}

void CameraService::Zoom(float scrollDelta, const sf::Vector2i& mousePos)
{
    sf::Vector2f beforeZoom = WindowToWorldPosition(mousePos);

    float factor = (scrollDelta > 0.f) ? ZoomStep : (1.f / ZoomStep);
    _view.zoom(factor);

    // Clamp zoom
    if (_view.getSize().x < 1280.f / ZoomMax || _view.getSize().x > 1280.f / ZoomMin)
    {
        _view.zoom(1.f / factor);
        return;
    }

    sf::Vector2f afterZoom = WindowToWorldPosition(mousePos);
    _view.move(beforeZoom - afterZoom);
    _window.setView(_view);
}

}
