#pragma once

#include <SFML/System/Vector2.hpp>

struct BoundsComponent
{
	sf::Vector2f min{ 0.f, 0.f };

	sf::Vector2f max{ 1280.f, 720.f };

	bool bounce = true;
};
