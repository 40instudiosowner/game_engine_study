#pragma once

#include "../ECS/Systems/IUpdateSystem.h"
#include "../AssetManager.h"
#include "../Game/GameState.h"
#include <SFML/Graphics.hpp>
#include <functional>

class BrickSystem final : public IUpdateSystem
{
public:
	BrickSystem(AssetManager& assets, sf::RenderWindow& window,
		GameState& gameState, std::function<void(const std::string&)> sceneChangeCallback);

	void Update(World& world, float dt) override;

private:
	AssetManager& _assets;
	sf::RenderWindow& _window;
	GameState& _gameState;
	std::function<void(const std::string&)> _sceneChangeCallback;
};