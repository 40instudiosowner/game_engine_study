#pragma once

#include "../../ECS/Systems/IUpdateSystem.h"
#include "../GameObjectController.h"
#include "../../Serialization/IComponentSerializer.h"

namespace Editor
{

class EntityGuiSystem final : public IUpdateSystem
{
    std::shared_ptr<GameObjectController> _goController;
    IComponentSerializer& _serializer;

public:
    EntityGuiSystem(std::shared_ptr<GameObjectController> goController, IComponentSerializer& serializer)
        : _goController(goController)
        , _serializer(serializer)
    {
    }

    void Update(World& world, float dt) override;
};

}
