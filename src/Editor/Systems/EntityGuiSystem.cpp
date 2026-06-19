#include "EntityGuiSystem.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/CollisionComponent.h"
#include "../../Components/PlayerComponent.h"
#include "../../Components/TileComponent.h"
#include "../../Components/EnemyComponent.h"
#include "../../Components/MovementComponent.h"

#include <imgui.h>

namespace Editor
{

void EntityGuiSystem::Update(World& world, float dt)
{
    ImGui::Begin("Entities");

    const auto& entities = world.GetEntities();
    for (const auto& entity : entities)
    {
        auto* tPool = world.GetPool<TransformComponent>();
        if (!tPool || !tPool->Has(entity.id))
            continue;

        ImGui::Separator();
        ImGui::Text("Entity %u (gen %u)", entity.id, entity.generation);

        auto& t = tPool->Get(entity.id);
        ImGui::Text("  Pos: %.1f, %.1f", t.position.x, t.position.y);

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
        {
            ImGui::OpenPopup("EntityEdit");
        }

        if (ImGui::BeginPopup("EntityEdit"))
        {
            if (ImGui::Button("Delete"))
            {
                world.DestroyEntity(entity);
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    ImGui::End();
}

}
