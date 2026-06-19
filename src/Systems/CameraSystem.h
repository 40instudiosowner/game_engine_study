#pragma once

#include "../ECS/Systems/IUpdateSystem.h"
#include "../AssetManager.h"
#include <SFML/Graphics.hpp>

class GameEngine;

class CameraSystem final : public IUpdateSystem
{
public:
	CameraSystem(sf::RenderWindow& window, GameEngine& engine);

	void Update(World& world, float dt) override;

private:
	sf::RenderWindow& _window;
	GameEngine& _engine;
};