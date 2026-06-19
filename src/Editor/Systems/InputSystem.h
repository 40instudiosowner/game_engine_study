#pragma once

#include "../../ECS/Systems/IUpdateSystem.h"

namespace Editor
{

class InputSystem final : public IUpdateSystem
{
public:
    InputSystem() = default;
    void Update(World& world, float dt) override;
};

}
