#pragma once

#include "../ECS/World.h"
#include "../ECS/FilterBuilder.h"
#include "../ECS/Filter.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"

namespace Editor
{

class GameObjectController
{
    World& _world;

    int _draggableEntity = -1;
    sf::Vector2f _dragOffset;

public:
    explicit GameObjectController(World& world);

    bool IsEntityDragging() const;

    bool StartDragEntity(const sf::Vector2f& worldPos);
    void DragEntity(const sf::Vector2f& worldPos) const;
    void StopDragEntity();
};

}
