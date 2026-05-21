#pragma once

#include "../ECS/Systems/IUpdateSystem.h"

namespace sf
{
	class RenderWindow;
}

class RenderSystem final : public IUpdateSystem
{
public:

	explicit RenderSystem(sf::RenderWindow& window);

	void Update(World& world, float dt) override;

private:

	sf::RenderWindow& _window;
};