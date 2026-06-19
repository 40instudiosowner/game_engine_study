#include "GuiSystem.h"
#include "../RenderState.h"

#include <iostream>
#include <imgui.h>

namespace Editor
{

GuiSystem::GuiSystem(AssetManager& assets,
                     std::function<void(const std::string&)> onCreateTexture,
                     std::function<void(const std::string&)> onCreateAnimation,
                     std::function<void()> onSaveScene,
                     std::function<void()> onLoadScene)
    : _assets(assets)
    , _onCreateTexture(onCreateTexture)
    , _onCreateAnimation(onCreateAnimation)
    , _onSaveScene(onSaveScene)
    , _onLoadScene(onLoadScene)
{
}

void GuiSystem::Update(World& world, float dt)
{
    ImGui::Begin("Editor");

    if (ImGui::BeginTabBar("Tab"))
    {
        if (ImGui::BeginTabItem("Debug"))
        {
            ImGui::Checkbox("Draw Grid (G)", &RenderState::IsGrid);
            ImGui::Checkbox("Draw Colliders (C)", &RenderState::IsCollider);
            ImGui::Checkbox("Draw Textures (T)", &RenderState::IsTexture);
            ImGui::Spacing();
            ImGui::Checkbox("Snap to Grid (S)", &RenderState::SnapToGrid);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Assets"))
        {
            ImGui::TextWrapped("Click a texture to create on the scene, then click to place.");

            ImGui::Spacing();
            ImGui::SeparatorText("Textures");
            ImGui::Spacing();

            int btnIdx = 0;
            for (const auto& [name, tex] : _assets.GetAllTextures())
            {
                ImGui::PushID(btnIdx++);
                ImTextureID texId = (ImTextureID)(uintptr_t)(tex->getNativeHandle());
                float w = static_cast<float>(tex->getSize().x);
                float h = static_cast<float>(tex->getSize().y);
                float scale = std::min(64.f / w, 64.f / h);
                if (ImGui::ImageButton(name.c_str(), texId,
                    ImVec2(w * scale, h * scale)))
                {
                    _onCreateTexture(name);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("%s\n%dx%d", name.c_str(), tex->getSize().x, tex->getSize().y);
                ImGui::PopID();
                if (btnIdx % 4 != 0)
                    ImGui::SameLine();
            }

            ImGui::Spacing();
            ImGui::SeparatorText("Animations");
            ImGui::Spacing();

            btnIdx = 0;
            for (const auto& [name, anim] : _assets.GetAllAnimations())
            {
                ImGui::PushID(btnIdx++);
                if (anim.texture)
                {
                    ImTextureID texId = (ImTextureID)(uintptr_t)(anim.texture->getNativeHandle());
                    float w = static_cast<float>(anim.frameWidth);
                    float h = static_cast<float>(anim.frameHeight);
                    float scale = std::min(64.f / w, 64.f / h);
                    if (ImGui::ImageButton(name.c_str(), texId,
                        ImVec2(w * scale, h * scale),
                        ImVec2(0, 0),
                        ImVec2(1.f / anim.frameCount, 1.f)))
                    {
                        _onCreateAnimation(name);
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetTooltip("%s\n%d frames, %dx%d",
                            name.c_str(), anim.frameCount, anim.frameWidth, anim.frameHeight);
                }
                else
                {
                    ImGui::Text("%s (no texture)", name.c_str());
                }
                ImGui::PopID();
                if (btnIdx % 4 != 0)
                    ImGui::SameLine();
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Save Scene"))
    {
        if (_onSaveScene)
            _onSaveScene();
    }
    ImGui::SameLine();
    if (ImGui::Button("Load Scene"))
    {
        if (_onLoadScene)
            _onLoadScene();
    }

    ImGui::End();
}

}
