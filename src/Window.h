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
#include "UI.h"

class Window 
{
    sf::RenderWindow _window;

    bool _isRun = true;
    bool _initialized = false;
 
    std::unique_ptr<UI> _ui;
    std::vector<std::unique_ptr<VisualObject>> _visualObjects;    // список объектов отрисовки

    std::unique_ptr<ConfigReader> _config;
    std::vector<std::string> _logoPaths;

    int Initialize();

    void setConfig(ConfigReader& config);

public:

    Window(const unsigned int wWidth, const unsigned int wHeight, ConfigReader& config);
    
    void Run();
};

#endif //WINDOW_H
