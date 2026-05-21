#pragma once

#include <SFML/System/Vector2.hpp>

struct BoxColliderComponent
{
	float radius = 0.f;
	sf::Vector2f size{ 0.f, 0.f };
};