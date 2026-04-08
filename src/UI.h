#pragma once

#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "VisualObject.h"

class UI
{
public:

    sf::Clock _deltaClock;

    UI(sf::RenderWindow& window);
    UI(const UI&) = default;
    UI& operator=(UI&) = default;
    UI(const UI&&) = delete;
    UI& operator=(UI&&) = delete;

	void Update(sf::RenderWindow& window, const std::vector<std::unique_ptr<VisualObject>>& visualObjects, bool& isRun);
    void Shutdown();
private:
    void UpdateUserInput(sf::RenderWindow& window, const std::vector<std::unique_ptr<VisualObject>>& visualObjects, bool& isRun);
    void UpdateLogic(const std::vector<std::unique_ptr<VisualObject>>& visualObjects);
    void UpdateGui(const std::vector<std::unique_ptr<VisualObject>>& visualObjects);
    void Render(sf::RenderWindow& window, const std::vector<std::unique_ptr<VisualObject>>& visualObjects);
};