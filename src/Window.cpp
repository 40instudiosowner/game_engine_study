#include "Window.h"

Window::Window(const unsigned int wWidth, const unsigned int wHeight) 
{
    _window.create(sf::VideoMode({wWidth, wHeight}), "Demo");

    auto desktop = sf::VideoMode::getDesktopMode();
    _window.setPosition({ (int) (desktop.size.x / 2 - wWidth / 2), (int) (desktop.size.y / 2 - wHeight / 2) });

    _window.setFramerateLimit(60);

    ImGui::SFML::Init(_window);

    ImGui::GetStyle().ScaleAllSizes(1.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;

    Initialize();
}

void Window::Initialize()
{
    // Все эти дефолтные данные надо будет прочитать из конфигурационного файла
    _rect = std::make_shared<Rectangle>(sf::Vector2f{120.f, 50.f});
    _rect->SetPosition({100.0f, 10.0f});

    _text = std::make_shared<Text>("../fonts/futura.ttf", L"Текст", 24);
    _text->SetPosition({0, _window.getSize().y - (float) _text->GetCharacterSize()});
}

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

            if (keyPressed->code == sf::Keyboard::Key::B)
            {
                _rect->ReverseMove();
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

    ImGui::Begin("Window Title");
    ImGui::Text("Press B to invert movement");

    ImGui::ColorEdit3("Color", _rect->GetColors());
    ImGui::SameLine();
    ImGui::Checkbox("Draw Rectangle", &_rect->GetShouldDraw());

    if (ImGui::Button("Reset Rectangle"))
        _rect->SetPosition({0, 0});

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
