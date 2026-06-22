#include "EditorScene.h"
#include "../GameEngine.h"
#include "../AssetManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/TileComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/DecorComponent.h"
#include "../Components/FinishComponent.h"
#include "../Components/EnemyComponent.h"
#include "../Components/AnimationStateComponent.h"
#include "../Components/AnimatorComponent.h"
#include "Constants.h"
#include "RenderState.h"
#include "Systems/GuiSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/MouseInputSystem.h"
#include "Systems/EntityGuiSystem.h"

#include <imgui.h>
#include <imgui-SFML.h>
#include <fstream>
#include <iostream>

namespace Editor
{

static constexpr int TILE_SIZE = 64;

void EditorScene::Init(GameEngine* engine)
{
    _engine = engine;
    auto& window = engine->GetWindow();
    auto& assets = engine->GetAssets();

    _camera = std::make_unique<CameraService>(window);

    sf::Font* font = assets.GetFont(AssetManager::FONT_MAIN);
    sf::Font baseFont = font ? *font : sf::Font();
    _renderService = std::make_unique<RenderService>(window, baseFont);

    _factory = std::make_unique<GameObjectFactory>(window, _world);
    _goController = std::make_shared<GameObjectController>(_world);
    _serializer = std::make_unique<ComponentSerializer>(_world);
    _jsonSerializer = std::make_unique<JsonSerializer>(*_serializer, "assets/configs/scene.json");

    _pendingEntityId = -1;

    SetupInputActions();

    _systems.AddSystem<GuiSystem>(assets,
        [this](const std::string& name) { StartTexturePlacement(name); },
        [this](const std::string& name) { StartAnimationPlacement(name); },
        [this]() { SaveScene(); },
        [this]() { LoadScene(); });
    _systems.AddSystem<InputSystem>();
    _systems.AddSystem<MouseInputSystem>(_goController, *_camera, window,
        [this](const sf::Vector2f& pos) { PlaceEntityAt(pos); },
        [this]() { return _pendingEntityId >= 0; },
        [this]() { CancelPlacement(); });
    _systems.AddSystem<EntityGuiSystem>(_goController, *_serializer);

    std::cout << "EditorScene initialized" << std::endl;
}

void EditorScene::SetupInputActions()
{
    auto& input = _engine->GetInputManager();
    input.Clear();
    input.RegisterAction("LeftMouse", sf::Keyboard::Key::Unknown, ActionType::Press, []{});
}

void EditorScene::StartTexturePlacement(const std::string& texName)
{
    const auto* tex = _engine->GetAssets().GetTexture(texName);
    if (!tex)
    {
        std::cerr << "Texture not found: " << texName << "\n";
        return;
    }

    // Destroy any previous pending entity
    if (_pendingEntityId >= 0)
        CancelPlacement();

    int id = _factory->CreateGameObjectEntity(*tex);
    _pendingEntityId = id;
    _pendingType = texName;
    _entityAssetNames[id] = texName;

    // Store texture name on the sprite for serialization round-trip
    auto* spritePool = _world.GetPool<SpriteComponent>();
    if (spritePool && spritePool->Has(static_cast<size_t>(id)))
        spritePool->Get(static_cast<size_t>(id)).textureName = texName;

    // Place at camera center
    auto* transformPool = _world.GetPool<TransformComponent>();
    if (transformPool)
    {
        auto& t = transformPool->Get(static_cast<size_t>(id));
        t.position = _camera->WindowToWorldPosition(
            sf::Vector2i(_engine->GetWindow().getSize().x / 2,
                          _engine->GetWindow().getSize().y / 2));
    }

    // If it's a tile texture, add TileComponent
    if (texName.find("Tile") != std::string::npos || texName.find("Brick") != std::string::npos ||
        texName.find("Question") != std::string::npos || texName.find("Pipe") != std::string::npos)
    {
        _world.AddComponent(EntityId{ static_cast<uint32_t>(id), 0 }, TileComponent{ TileType::Normal });
    }

    std::cout << "Placing: " << texName << " (entity " << id << ")\n";
}

void EditorScene::StartAnimationPlacement(const std::string& animName)
{
    const auto* anim = _engine->GetAssets().GetAnimation(animName);
    if (!anim)
    {
        std::cerr << "Animation not found: " << animName << "\n";
        return;
    }

    if (_pendingEntityId >= 0)
        CancelPlacement();

    int id = _factory->CreateGameObjectEntity(*anim);
    _pendingEntityId = id;
    _pendingType = animName;
    _entityAssetNames[id] = animName;

    // Store texture name on the sprite for serialization round-trip
    auto* spritePool = _world.GetPool<SpriteComponent>();
    if (spritePool && spritePool->Has(static_cast<size_t>(id)))
        spritePool->Get(static_cast<size_t>(id)).textureName = animName;

    // Place at camera center
    auto* transformPool = _world.GetPool<TransformComponent>();
    if (transformPool)
    {
        auto& t = transformPool->Get(static_cast<size_t>(id));
        t.position = _camera->WindowToWorldPosition(
            sf::Vector2i(_engine->GetWindow().getSize().x / 2,
                          _engine->GetWindow().getSize().y / 2));
    }

    std::cout << "Placing animation: " << animName << " (entity " << id << ")\n";
}

void EditorScene::PlaceEntityAt(const sf::Vector2f& worldPos)
{
    if (_pendingEntityId < 0)
        return;

    auto* transformPool = _world.GetPool<TransformComponent>();
    if (transformPool)
    {
        auto& transform = transformPool->Get(static_cast<size_t>(_pendingEntityId));
        if (RenderState::SnapToGrid)
            transform.position = RenderService::SnapToGrid(worldPos);
        else
            transform.position = worldPos;
    }

    _pendingEntityId = -1;
    _pendingType.clear();
}

void EditorScene::CancelPlacement()
{
    if (_pendingEntityId >= 0)
    {
        _entityAssetNames.erase(_pendingEntityId);
        _world.DestroyEntityById(static_cast<size_t>(_pendingEntityId));
        _pendingEntityId = -1;
        _pendingType.clear();
    }
}

static std::string AssetNameToEntityType(const std::string& name)
{
    if (name.starts_with("Mega"))
        return "Player";
    if (name == "Bullet")
        return "Bullet";
    if (name == "Finish")
        return "Finish";
    if (name == "BigHill" || name == "SmallHill" || name == "Cloud")
        return "Decor";
    if (name.starts_with("Goomba"))
        return "Goomba";
    if (name == "CoinAnim")
        return "Coin";
    if (name == "Explosion")
        return "Explosion";
    if (name.starts_with("Pipe"))
        return "Pipe";
    return "Tile";
}

void EditorScene::SaveScene()
{
    // Save full scene via JsonSerializer
    if (_jsonSerializer)
        _jsonSerializer->SaveWorldToFile(_world);

    // Save simplified level config for game use (determine type by texture name)
    json level = json::array();

    auto* transformPool = _world.GetPool<TransformComponent>();
    auto* spritePool = _world.GetPool<SpriteComponent>();

    if (!transformPool)
        return;

    const auto& entities = _world.GetEntities();
    for (const auto& entity : entities)
    {
        if (!transformPool->Has(entity.id))
            continue;

        auto& t = transformPool->Get(entity.id);
        int gx = static_cast<int>(t.position.x) / TILE_SIZE;
        int gy = static_cast<int>(t.position.y) / TILE_SIZE;

        json entry;
        entry["x"] = gx;
        entry["y"] = gy;

        // Determine type by stored asset name (texture type)
        auto it = _entityAssetNames.find(static_cast<int>(entity.id));
        if (it != _entityAssetNames.end())
        {
            entry["name"] = AssetNameToEntityType(it->second);
        }
        else
        {
            // Fallback: check animation name in AnimationStateComponent
            auto* animStatePool = _world.GetPool<AnimationStateComponent>();
            if (animStatePool && animStatePool->Has(entity.id))
            {
                auto& animState = animStatePool->Get(entity.id);
                entry["name"] = AssetNameToEntityType(animState.currentAnimation);
            }
            else
            {
                entry["name"] = "Tile";
            }
        }

        // For enemies, add patrol data
        if (entry["name"] == "Goomba")
        {
            auto* enemyPool = _world.GetPool<EnemyComponent>();
            if (enemyPool && enemyPool->Has(entity.id))
            {
                auto& enemy = enemyPool->Get(entity.id);
                entry["patrolLeft"] = enemy.patrolLeft / TILE_SIZE;
                entry["patrolRight"] = enemy.patrolRight / TILE_SIZE;
            }
        }

        level.push_back(entry);
    }

    std::ofstream file("assets/configs/level_editor.json");
    if (file.is_open())
    {
        file << level.dump(2);
        std::cout << "Level saved to assets/configs/level_editor.json\n";
    }
    else
    {
        std::cerr << "Failed to save level\n";
    }
}

void EditorScene::LoadScene()
{
    _pendingReload = true;
}

void EditorScene::RestoreTexturePointers()
{
    auto& assets = _engine->GetAssets();
    auto* spritePool = _world.GetPool<SpriteComponent>();

    if (!spritePool)
        return;

    for (auto entity : spritePool->GetDenseEntities())
    {
        auto& sprite = spritePool->Get(entity);
        if (sprite.texture || sprite.textureName.empty())
            continue;

        // Try texture first
        const auto* tex = assets.GetTexture(sprite.textureName);
        if (tex)
        {
            sprite.texture = tex;
            continue;
        }

        // Try animation (use its texture)
        const auto* anim = assets.GetAnimation(sprite.textureName);
        if (anim && anim->texture)
        {
            sprite.texture = anim->texture;
            continue;
        }
    }
}

void EditorScene::DoLoadScene()
{
    _pendingReload = false;

    // Reset render states that might have been toggled during previous session
    RenderState::SnapToGrid = false;
    RenderState::IsTexture = true;
    RenderState::IsGrid = false;
    RenderState::IsCollider = false;
    _entityAssetNames.clear();
    _pendingEntityId = -1;
    _pendingType.clear();
    _world = World();
    _serializer = std::make_unique<ComponentSerializer>(_world);
    _jsonSerializer = std::make_unique<JsonSerializer>(*_serializer, "assets/configs/scene.json");

    _jsonSerializer->LoadWorldFromFile(_world);
    _goController = std::make_shared<GameObjectController>(_world);

    // Rebuild systems with the new world and controller
    _systems.Clear();
    auto& window = _engine->GetWindow();
    auto& assets = _engine->GetAssets();
    _systems.AddSystem<GuiSystem>(assets,
        [this](const std::string& name) { StartTexturePlacement(name); },
        [this](const std::string& name) { StartAnimationPlacement(name); },
        [this]() { SaveScene(); },
        [this]() { LoadScene(); });
    _systems.AddSystem<InputSystem>();
    _systems.AddSystem<MouseInputSystem>(_goController, *_camera, window,
        [this](const sf::Vector2f& pos) { PlaceEntityAt(pos); },
        [this]() { return _pendingEntityId >= 0; },
        [this]() { CancelPlacement(); });
    _systems.AddSystem<EntityGuiSystem>(_goController, *_serializer);

    // Restore texture pointers from stored names
    RestoreTexturePointers();

    std::cout << "Scene loaded\n";
}

void EditorScene::Update(float dt)
{
    // Deferred scene reload (must happen outside _systems.Update loop)
    if (_pendingReload)
        DoLoadScene();

    // If in placement mode, update pending entity position to follow mouse
    if (_pendingEntityId >= 0)
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(_engine->GetWindow());
        sf::Vector2f worldPos = _camera->WindowToWorldPosition(mousePos);

        auto* transformPool = _world.GetPool<TransformComponent>();
        if (transformPool)
        {
            auto& transform = transformPool->Get(static_cast<size_t>(_pendingEntityId));
            if (RenderState::SnapToGrid)
                transform.position = RenderService::SnapToGrid(worldPos);
            else
                transform.position = worldPos;
        }
    }

}

void EditorScene::Render()
{
    auto& window = _engine->GetWindow();
    window.clear(sf::Color(0x2E, 0x2E, 0x2E));

    _renderService->SetView(_camera->View());

    float currentZoom = 1280.f / _camera->View().getSize().x;

    if (RenderState::IsGrid)
        _renderService->RenderGrid(currentZoom);

    if (RenderState::IsTexture)
    {
        auto* transformPool = _world.GetPool<TransformComponent>();
        auto* spritePool = _world.GetPool<SpriteComponent>();

        if (transformPool && spritePool)
        {
            for (auto entity : spritePool->GetDenseEntities())
            {
                if (!transformPool->Has(entity))
                    continue;

                auto& transform = transformPool->Get(entity);
                auto& spriteComp = spritePool->Get(entity);

                if (!spriteComp.visible || !spriteComp.texture)
                    continue;

                auto sprite = spriteComp.CreateSprite();
                sprite.setPosition(transform.position);
                _renderService->RenderTexture(sprite);
            }
        }
    }

    if (RenderState::IsCollider)
    {
        auto* transformPool = _world.GetPool<TransformComponent>();
        auto* bboxPool = _world.GetPool<BoxColliderComponent>();

        if (transformPool && bboxPool)
        {
            for (auto entity : bboxPool->GetDenseEntities())
            {
                if (!transformPool->Has(entity))
                    continue;

                auto& transform = transformPool->Get(entity);
                auto& bbox = bboxPool->Get(entity);
                _renderService->RenderColliders(bbox, transform.position);
            }
        }
    }

    // Start ImGui frame, then run all editor systems (they use ImGui)
    ImGui::SFML::Update(window, _imguiDeltaClock.restart());

    _systems.Update(_world, 0.f);

    ImGui::SFML::Render(window);
}

void EditorScene::HandleEvent(const sf::Event& event)
{
    auto& window = _engine->GetWindow();
    ImGui::SFML::ProcessEvent(window, event);

    if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard)
        return;

    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        {
            if (_pendingEntityId >= 0)
                CancelPlacement();
            else
                _engine->ChangeScene("game");
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::Delete)
        {
            // Delete selected entity (handled in EntityGuiSystem)
        }
    }

    if (const auto* scroll = event.getIf<sf::Event::MouseWheelScrolled>())
    {
        if (scroll->wheel == sf::Mouse::Wheel::Vertical)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            _camera->Zoom(scroll->delta, mousePos);
        }
    }
}

}
