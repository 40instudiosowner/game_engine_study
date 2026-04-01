#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include "Rectangle.h"
#include "Text.h"
#include "ConfigReader.h"

class Window {
    sf::RenderWindow _window;

    sf::Clock _deltaClock;

    bool _isRun;

    std::shared_ptr<Rectangle> _rect;
    std::shared_ptr<Text> _text;

    std::shared_ptr<ConfigReader> _config;
    std::vector<std::string> _logoPaths;

    void Initialize();

    void UpdateUserInput();
    void UpdateLogic();
    void UpdateGui();
    void Render();

    void setConfig(ConfigReader config);

public:

    Window(const unsigned int wWidth, const unsigned int wHeight, ConfigReader config);
    
    void Run();
};

#endif //WINDOW_H
