#pragma once

#include "../../ECS/Systems/IUpdateSystem.h"
#include "../GameObjectController.h"
#include "../CameraService.h"

#include <SFML/Window/Mouse.hpp>
#include <functional>
#include <memory>

namespace Editor
{

class MouseInputSystem final : public IUpdateSystem
{
    std::shared_ptr<GameObjectController> _goController;
    CameraService& _camera;
    sf::RenderWindow& _window;
    bool _leftPressed = false;
    bool _middlePressed = false;
    bool _rightPressed = false;
    bool _cameraDragging = false;

    std::function<void(const sf::Vector2f&)> _onPlaceEntity;
    std::function<bool()> _isPlacementMode;
    std::function<void()> _onCancelPlacement;

public:
    MouseInputSystem(std::shared_ptr<GameObjectController> goController,
                     CameraService& camera,
                     sf::RenderWindow& window,
                     std::function<void(const sf::Vector2f&)> onPlaceEntity,
                     std::function<bool()> isPlacementMode,
                     std::function<void()> onCancelPlacement);

    void Update(World& world, float dt) override;
};

}
