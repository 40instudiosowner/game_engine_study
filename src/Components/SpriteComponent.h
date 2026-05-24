#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>

struct SpriteComponent
{
	sf::Sprite sprite;
	sf::Color color = sf::Color::White;

	bool visible = true;

};