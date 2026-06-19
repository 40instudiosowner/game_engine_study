#include "GameObjectController.h"
#include "../Components/CollisionComponent.h"
#include "../Components/SpriteComponent.h"

namespace Editor
{

GameObjectController::GameObjectController(World& world)
    : _world(world)
{
}

bool GameObjectController::IsEntityDragging() const
{
    return _draggableEntity >= 0;
}

bool GameObjectController::StartDragEntity(const sf::Vector2f& worldPos)
{
    // Find entity under cursor by checking collision components
    auto* transformPool = _world.GetPool<TransformComponent>();
    auto* bboxPool = _world.GetPool<BoxColliderComponent>();
    auto* collisionPool = _world.GetPool<CollisionComponent>();

    if (!transformPool || !bboxPool)
        return false;

    const auto& dense = bboxPool->GetDenseEntities();
    for (auto it = dense.rbegin(); it != dense.rend(); ++it)
    {
        size_t entityId = *it;
        if (!transformPool->Has(entityId))
            continue;

        auto& transform = transformPool->Get(entityId);
        auto& bbox = bboxPool->Get(entityId);

        sf::FloatRect rect(
            transform.position + bbox.offset,
            bbox.size);

        if (rect.contains(worldPos))
        {
            _draggableEntity = static_cast<int>(entityId);
            _dragOffset = transform.position - worldPos;
            return true;
        }
    }

    return false;
}

void GameObjectController::DragEntity(const sf::Vector2f& worldPos) const
{
    if (_draggableEntity < 0)
        return;

    auto* transformPool = _world.GetPool<TransformComponent>();
    if (!transformPool)
        return;

    auto& transform = transformPool->Get(static_cast<size_t>(_draggableEntity));
    transform.position = worldPos + _dragOffset;
}

void GameObjectController::StopDragEntity()
{
    _draggableEntity = -1;
}

}
