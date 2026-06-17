#pragma once

#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <functional>

// Simple button helper
struct MenuButton
{
	sf::RectangleShape bg;
	std::optional<sf::Text> text;
	std::function<void()> onClick;
	bool isHovered = false;
	sf::Color normalColor;
	sf::Color hoverColor;

	void Init(sf::Font& font, const std::string& label, float x, float y,
		sf::Color normal, sf::Color hover, std::function<void()> callback)
	{
		normalColor = normal;
		hoverColor = hover;

		bg.setSize({ 300.f, 60.f });
		bg.setFillColor(normal);
		bg.setOrigin({ 150.f, 30.f });
		bg.setPosition({ x, y });

		text.emplace(font, label, 36);
		text->setFillColor(sf::Color::White);
		auto bounds = text->getLocalBounds();
		text->setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
		text->setPosition({ x, y });

		onClick = callback;
	}

	void UpdateHover(const sf::Vector2f& mousePos)
	{
		sf::FloatRect rect(bg.getPosition() - sf::Vector2f(150.f, 30.f), sf::Vector2f(300.f, 60.f));
		isHovered = rect.contains(mousePos);
		bg.setFillColor(isHovered ? hoverColor : normalColor);
	}

	void Draw(sf::RenderWindow& window)
	{
		window.draw(bg);
		if (text) window.draw(*text);
	}
};

class MenuScene : public Scene
{
public:
	void Init(GameEngine* engine) override;
	void Update(float dt) override;
	void Render() override;
	void HandleEvent(const sf::Event& event) override;
	void OnActivate() override;

private:
	std::optional<sf::Text> _titleText;
	std::optional<sf::Text> _bestTimeText;
	float _bestTime = 0.f;

	MenuButton _playButton;
	MenuButton _exitButton;
};