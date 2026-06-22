#include "MouseInputSystem.h"
#include <SFML/Window/Mouse.hpp>
#include <imgui.h>

namespace Editor
{

MouseInputSystem::MouseInputSystem(std::shared_ptr<GameObjectController> goController,
                                   CameraService& camera,
                                   sf::RenderWindow& window,
                                   std::function<void(const sf::Vector2f&)> onPlaceEntity,
                                   std::function<bool()> isPlacementMode,
                                   std::function<void()> onCancelPlacement)
    : _goController(goController)
    , _camera(camera)
    , _window(window)
    , _onPlaceEntity(onPlaceEntity)
    , _isPlacementMode(isPlacementMode)
    , _onCancelPlacement(onCancelPlacement)
{
}

void MouseInputSystem::Update(World& world, float dt)
{
    // Don't process mouse input when ImGui is capturing it
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    sf::Vector2i mousePos = sf::Mouse::getPosition(_window);
    sf::Vector2f worldPos = _camera.WindowToWorldPosition(mousePos);

    bool inPlacement = _isPlacementMode ? _isPlacementMode() : false;

    // Left mouse button
    bool leftDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    if (leftDown)
    {
        if (!_leftPressed)
        {
            _leftPressed = true;

            if (inPlacement)
            {
                if (_onPlaceEntity)
                    _onPlaceEntity(worldPos);
            }
            else
            {
                if (!_goController->StartDragEntity(worldPos))
                {
                    _camera.StartMoving(mousePos);
                    _cameraDragging = true;
                }
            }
        }
        else
        {
            if (inPlacement)
            {
            }
            else if (_goController->IsEntityDragging())
            {
                _goController->DragEntity(worldPos);
            }
            else if (_cameraDragging)
            {
                _camera.Drag(mousePos, dt);
            }
        }
    }
    else
    {
        if (_leftPressed)
        {
            if (_goController->IsEntityDragging())
                _goController->StopDragEntity();
            if (_cameraDragging)
            {
                _camera.StopMoving();
                _cameraDragging = false;
            }
            _leftPressed = false;
        }
    }

    // Right mouse button — camera pan (or cancel placement)
    bool rightDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
    if (rightDown)
    {
        if (!_rightPressed)
        {
            _rightPressed = true;
            if (inPlacement)
            {
                if (_onCancelPlacement)
                    _onCancelPlacement();
            }
            else
            {
                _camera.StartMoving(mousePos);
            }
        }
        else if (!inPlacement)
        {
            _camera.Drag(mousePos, dt);
        }
    }
    else
    {
        if (_rightPressed)
        {
            _camera.StopMoving();
            _rightPressed = false;
        }
    }

    // Middle mouse button — camera pan
    bool middleDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle);
    if (middleDown)
    {
        if (!_middlePressed)
        {
            _camera.StartMoving(mousePos);
            _middlePressed = true;
        }
        else
        {
            _camera.Drag(mousePos, dt);
        }
    }
    else
    {
        if (_middlePressed)
        {
            _camera.StopMoving();
            _middlePressed = false;
        }
    }
}

}
