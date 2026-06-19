#pragma once

#include <SFML/System/Vector2.hpp>

struct BulletComponent
{
	sf::Vector2f spawnPosition{ 0.f, 0.f };
	float maxDistance = 900.f;
};