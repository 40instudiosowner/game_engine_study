#pragma once

#include "../ECS/Systems/IUpdateSystem.h"
#include "../Game/GameState.h"
#include "../AssetManager.h"
#include "../ECS/ComponentPool.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"

namespace sf
{
	class RenderWindow;
}

class RenderSystem final : public IUpdateSystem
{
public:

	RenderSystem(
		sf::RenderWindow& window,
		GameState& gameState,
		AssetManager& assets,
		int& renderMode);

	void Update(World& world, float dt) override;
	void Render(World& world, sf::RenderWindow& window, AssetManager& assets, int renderMode);

private:

	void RenderTextures(World& world);
	void RenderColliders(World& world);
	void RenderGrid();
	void DrawSprite(size_t entity, ComponentPool<TransformComponent>& transformPool, ComponentPool<SpriteComponent>& spritePool);

	sf::RenderWindow& _window;
	GameState& _gameState;
	AssetManager& _assets;
	int& _renderMode;
};