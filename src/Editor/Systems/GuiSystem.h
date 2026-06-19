#pragma once

#include "../../ECS/Systems/IUpdateSystem.h"
#include "../../AssetManager.h"
#include <functional>

namespace Editor
{

class GuiSystem final : public IUpdateSystem
{
    AssetManager& _assets;
    std::function<void(const std::string&)> _onCreateTexture;
    std::function<void(const std::string&)> _onCreateAnimation;
    std::function<void()> _onSaveScene;
    std::function<void()> _onLoadScene;

public:
    GuiSystem(AssetManager& assets,
              std::function<void(const std::string&)> onCreateTexture,
              std::function<void(const std::string&)> onCreateAnimation,
              std::function<void()> onSaveScene,
              std::function<void()> onLoadScene);

    void Update(World& world, float dt) override;
};

}
