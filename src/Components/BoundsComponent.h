#pragma once

#include <SFML/System/Vector2.hpp>

struct BoundsComponent
{
	sf::Vector2f min{ 0.f, 0.f };

	sf::Vector2f max{ 1920.f, 1080.f };

	bool bounce = true;
};
