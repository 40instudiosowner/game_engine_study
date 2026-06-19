#pragma once

#include "../Animation.h"
#include "../ECS/World.h"
#include <SFML/Graphics/RenderWindow.hpp>

namespace Editor
{

class GameObjectFactory
{
    sf::RenderWindow& _window;
    World& _world;

public:
    GameObjectFactory(sf::RenderWindow& window, World& world);

    int CreateGameObjectEntity(const sf::Texture& tex) const;
    int CreateGameObjectEntity(const Animation& anim) const;
};

}
