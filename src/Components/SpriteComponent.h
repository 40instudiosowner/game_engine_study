#pragma once

#include <SFML/Graphics.hpp>


struct SpriteComponent
{
	sf::RectangleShape shape;

	std::vector<sf::Texture> textures;

	int currentTexture = 0;

	bool visible = true;

	sf::Color color = sf::Color::White;
};