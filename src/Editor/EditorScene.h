#pragma once

#include "../Scenes/Scene.h"
#include <nlohmann/json.hpp>
#include "CameraService.h"
#include "RenderService.h"
#include "GameObjectFactory.h"
#include "GameObjectController.h"
#include "../Serialization/ComponentSerializer.h"
#include "../Serialization/ComponentRegistrations.h"
#include "JsonSerializer.h"

#include <memory>
#include <string>
#include <unordered_map>

using json = nlohmann::json;

namespace Editor
{

class EditorScene : public Scene
{
public:
    void Init(GameEngine* engine) override;
    void Update(float dt) override;
    void Render() override;
    void HandleEvent(const sf::Event& event) override;

private:
    void SetupInputActions();
    void StartTexturePlacement(const std::string& texName);
    void StartAnimationPlacement(const std::string& animName);
    void PlaceEntityAt(const sf::Vector2f& worldPos);
    void CancelPlacement();
    void SaveScene();
    void LoadScene();

    std::unique_ptr<CameraService> _camera;
    std::unique_ptr<RenderService> _renderService;
    std::unique_ptr<GameObjectFactory> _factory;
    std::shared_ptr<GameObjectController> _goController;
    std::unique_ptr<ComponentSerializer> _serializer;
    std::unique_ptr<JsonSerializer> _jsonSerializer;

    int _pendingEntityId = -1;
    std::string _pendingType;
    std::unordered_map<int, std::string> _entityAssetNames;
};

}
