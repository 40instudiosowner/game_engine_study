#include "MenuScene.h"
#include "../GameEngine.h"
#include "../AssetManager.h"
#include <cstdio>
#include <iostream>

void MenuScene::Init(GameEngine* engine)
{
	_engine = engine;

	auto* font = engine->GetAssets().GetFont(AssetManager::FONT_MAIN);
	if (!font) return;

	_titleText.emplace(*font, "MEGA MAN - ECS", 72);
	_titleText->setFillColor(sf::Color::White);
	auto titleBounds = _titleText->getLocalBounds();
	_titleText->setOrigin({ titleBounds.size.x / 2.f, titleBounds.size.y / 2.f });
	_titleText->setPosition({ 640.f, 200.f });

	_bestTimeText.emplace(*font, "", 24);
	_bestTimeText->setFillColor(sf::Color::Yellow);
	_bestTimeText->setPosition({ 20.f, 20.f });

	_playButton.Init(*font, "PLAY", 640.f, 400.f,
		sf::Color(100, 100, 200), sf::Color(150, 150, 255),
		[this]() { _engine->ChangeScene("game"); });

	_exitButton.Init(*font, "EXIT", 640.f, 500.f,
		sf::Color(150, 50, 50), sf::Color(200, 80, 80),
		[this]() { _engine->Stop(); });
}

void MenuScene::OnActivate()
{
#ifdef DEBUG
	std::cout << "[MenuScene] Activated\n";
#endif
}

void MenuScene::Update(float)
{
	if (_bestTime > 0.f && _bestTimeText)
	{
		char buf[64];
		int minutes = static_cast<int>(_bestTime) / 60;
		float seconds = _bestTime - minutes * 60;
		snprintf(buf, sizeof(buf), "Best Time: %d:%05.2f", minutes, seconds);
		_bestTimeText->setString(buf);
	}
}

void MenuScene::Render()
{
	auto& window = _engine->GetWindow();
	window.clear(sf::Color::Black);

	if (_titleText) window.draw(*_titleText);
	_playButton.Draw(window);
	_exitButton.Draw(window);
	if (_bestTimeText && _bestTime > 0.f) window.draw(*_bestTimeText);
}

void MenuScene::HandleEvent(const sf::Event& event)
{
	auto& window = _engine->GetWindow();

	if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>())
	{
		// Use mapPixelToCoords for cross-platform correctness (HiDPI, Wayland, etc.)
		auto worldPos = window.mapPixelToCoords(
			{ mouseMoved->position.x, mouseMoved->position.y });

		_playButton.UpdateHover(worldPos);
		_exitButton.UpdateHover(worldPos);
	}

	if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (mousePressed->button == sf::Mouse::Button::Left)
		{
			auto worldPos = window.mapPixelToCoords(
				{ mousePressed->position.x, mousePressed->position.y });

			_playButton.UpdateHover(worldPos);
			_exitButton.UpdateHover(worldPos);

			if (_playButton.isHovered) _playButton.onClick();
			if (_exitButton.isHovered) _exitButton.onClick();
		}
	}

	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
	{
		if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
			_engine->Stop();
	}
}