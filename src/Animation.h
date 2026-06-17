#pragma once

#include <SFML/Graphics.hpp>
#include <string>

struct Animation
{
	std::string name;
	int frameCount = 1;
	int frameDuration = 1;
	int frameWidth = 0;
	int frameHeight = 0;
	const sf::Texture* texture = nullptr;

	sf::Sprite MakeSprite() const
	{
		sf::Sprite s(*texture);
		s.setTextureRect(sf::IntRect({
			{0, 0},
			{frameWidth, frameHeight}
		}));
		s.setOrigin({
			frameWidth / 2.f,
			frameHeight / 2.f
		});
		return s;
	}

	void ApplyFrame(sf::Sprite& s, int frame) const
	{
		if (frameCount <= 1)
			return;
		int col = frame % frameCount;
		s.setTextureRect(sf::IntRect({
			{col * frameWidth, 0},
			{frameWidth, frameHeight}
		}));
	}
};