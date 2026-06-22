#pragma once

#include <SFML/Graphics.hpp>

struct SpriteComponent
{
	const sf::Texture* texture = nullptr;
	std::string textureName;
	sf::IntRect textureRect;
	sf::Color color = sf::Color::White;
	bool visible = true;
	sf::Vector2f origin{ 0.f, 0.f };
	sf::Vector2f scale{ 1.f, 1.f };
	float rotation = 0.f;

	sf::Sprite CreateSprite() const
	{
		if (!texture)
			return sf::Sprite(*texture); // will crash if null - caller must check

		sf::Sprite s(*texture);
		if (textureRect.size.x == 0 || textureRect.size.y == 0)
		{
			auto size = texture->getSize();
			s.setTextureRect(sf::IntRect({ {0, 0}, sf::Vector2i(static_cast<int>(size.x), static_cast<int>(size.y)) }));
		}
		else
		{
			s.setTextureRect(textureRect);
		}
		s.setColor(color);
		s.setOrigin(origin);
		s.setScale(scale);
		s.setRotation(sf::degrees(rotation));
		return s;
	}
};