#include "Window.h"
#include <iostream>

Window::Window(const unsigned int wWidth, const unsigned int wHeight, ConfigReader config) 
{
    _config = std::make_shared<ConfigReader>(config);

    _window.create(sf::VideoMode({wWidth, wHeight}), "Slider");

    auto desktop = sf::VideoMode::getDesktopMode();
    _window.setPosition({ (int) (desktop.size.x / 2 - wWidth / 2), (int) (desktop.size.y / 2 - wHeight / 2) });

    _window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(_window))
        std::cerr << "Init error";

    ImGui::GetStyle().ScaleAllSizes(1.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;

    Initialize();
}

void Window::Initialize()
{
    // Все эти дефолтные данные надо будет прочитать из конфигурационного файла
    _rect = std::make_shared<Rectangle>(sf::Vector2f{static_cast<float>(_config->getLogoWidth()),
                                                     static_cast<float>(_config->getLogoHeight()) });
    _rect->SetPosition({ _config->getLogoPositionX(), _config->getLogoPositionY() });
    _logoPaths = _config->getLogoPaths();

    if (_logoPaths.size() > 0)
        _rect->SetTexture(_logoPaths[0]);
    
    std::cout << "fact path -------" <<  _config->getFontPath();
    _text = std::make_shared<Text>(_config->getFontPath(), L"Pause", 24);
    _text->SetPosition({ _window.getSize().x / 2.f, 
                         _window.getSize().y / 2.f - static_cast<float>(_text->GetCharacterSize())});
}

void Window::setConfig(ConfigReader config)
{
    _config = std::make_shared<ConfigReader>(config);
};

void Window::Run()
{
    while (_isRun) 
    {
        sf::Time delta = _deltaClock.restart();
        ImGui::SFML::Update(_window, delta);

        UpdateUserInput();
        UpdateLogic();
        UpdateGui();
        Render();
    }

    _window.close();
    ImGui::SFML::Shutdown();
}

void Window::UpdateUserInput()
{
    while (const std::optional event = _window.pollEvent())
    {
        ImGui::SFML::ProcessEvent(_window, *event);

        if (event->is<sf::Event::Closed>())
        {
            _isRun = false;
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) 
        {
            std::wcout << L"Key pressed with code = " << sf::Keyboard::getDescription(keyPressed->scancode).toWideString() << "\n";

            if (keyPressed->code == sf::Keyboard::Key::Space)
            {
                //_rect->ReverseMove();
                if (_rect->IsStopped())
                {
                    _text->SetShouldDraw(false);
                    _rect->ContinueMovement();
                }
                else
                {
                    _text->SetShouldDraw(true);
                    _rect->StopMovement();
                }
            }
        }
    }
}

void Window::UpdateLogic()
{
    _rect->Update();
}

void Window::UpdateGui()
{
    //ImGui::ShowDemoWindow();

    ImGui::Begin("Settings");
    ImGui::Text("Press Space to pause...");

    ImGui::ColorEdit3("Color", _rect->GetColors());
    ImGui::SameLine();
    ImGui::Checkbox("Draw Rectangle", &_rect->GetShouldDraw());

    if (ImGui::Button("Reset Rectangle"))
        _rect->SetPosition({0, 0});

    ImGui::InputFloat("Speed", _rect->GetSpeed());

    ImGui::End();
}

void Window::Render()
{
    _window.clear();
    
    _rect->Draw(_window);
    _text->Draw(_window);

    ImGui::SFML::Render(_window);
    _window.display();
}
