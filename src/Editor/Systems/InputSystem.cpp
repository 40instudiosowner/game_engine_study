#include "InputSystem.h"
#include "../Constants.h"
#include "../RenderState.h"
#include <SFML/Window/Keyboard.hpp>
#include <imgui.h>

namespace Editor
{

void InputSystem::Update(World& world, float dt)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G))
        RenderState::IsGrid = !RenderState::IsGrid;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C))
        RenderState::IsCollider = !RenderState::IsCollider;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T))
        RenderState::IsTexture = !RenderState::IsTexture;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        RenderState::SnapToGrid = !RenderState::SnapToGrid;
}

}
