#pragma once

#include <SFML/System/Vector2.hpp>

struct MovementComponent
{
	sf::Vector2f direction{ 0.f, 0.f };

	float speed = 0.f;
};