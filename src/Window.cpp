#include "Window.h"
#include <iostream>

Window::Window(const unsigned int wWidth, const unsigned int wHeight, ConfigReader& config) 
{

    _window.create(sf::VideoMode({wWidth, wHeight}), "Slider");

    auto desktop = sf::VideoMode::getDesktopMode();
    _window.setPosition({ (int) (desktop.size.x / 2 - wWidth / 2), (int) (desktop.size.y / 2 - wHeight / 2) });

    _window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(_window))
        std::cerr << "Init error";

    ImGui::GetStyle().ScaleAllSizes(1.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;

    setConfig(config);
}

void Window::Initialize()
{
    // Все эти дефолтные данные надо будет прочитать из конфигурационного файла

    _rect = std::make_shared<Rectangle>(sf::Vector2f{static_cast<float>(_config->getLogoWidth()),
                                                     static_cast<float>(_config->getLogoHeight()) });
    _rect->SetPosition({ _config->getLogoPositionX(), _config->getLogoPositionY() });

    _logoPaths = _config->getLogoPaths();
	// загрузка всех текстур из доступных путей и установка их в Rectangle
	for (auto& path : _logoPaths)
        _rect->SetTexture(path);

    _rect->SetScale(_config->getLogoScaleMultiplier());
    _rect->SetConstrains({ static_cast<float>(_window.getSize().x), static_cast<float>(_window.getSize().y) });

    //std::cout << "fact path -------" <<  _config->getFontPath();
    _text = std::make_shared<Text>(_config->getFontPath(), 
                                   _config->getText(), 
                                   _config->getFontSize());

    _text->SetPosition({ _window.getSize().x / 2.f, 
                         _window.getSize().y / 2.f - static_cast<float>(_text->GetCharacterSize())});
}

void Window::setConfig(ConfigReader& config)
{
    _config = std::make_unique<ConfigReader>(config);
    Initialize();
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
            std::cout<< "Window closed\n";
            _isRun = false;
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) 
        {
            #ifdef DEBUG
            std::wcout << L"Key pressed with code = " << sf::Keyboard::getDescription(keyPressed->scancode).toWideString() << "\n";
            #endif

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
	_text->Update();
}

void Window::UpdateGui()
{

    ImGui::Begin("Settings");

    // Выпадающий список для выбора лого
    if (ImGui::BeginCombo("Logo", _rect->GetLogos()[_rect->GetCurrentLogoIndex()].c_str()))
    {
        for (int i = 0; i < _rect->GetLogos().size(); i++)
        {
            bool isSelected = (_rect->GetCurrentLogoIndex() == i);
            if (ImGui::Selectable(_rect->GetLogos()[i].c_str(), isSelected))
                _rect->SetCurrentLogoIndex(static_cast<Logo>(i));

            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    // Скорость
    ImGui::SliderFloat("Speed", _rect->GetSpeed(), 0.0f, 10.0f);

    // Масштаб
    ImGui::SliderFloat("Scale", _rect->GetScaleRef(), 0.1f, 5.0f);

    ImGui::SliderFloat("R", &_rect->GetColorRef()[0], 0.1f, 1.0f);
    ImGui::SliderFloat("G", &_rect->GetColorRef()[1], 0.1f, 1.0f);
    ImGui::SliderFloat("B", &_rect->GetColorRef()[2], 0.1f, 1.0f);


    // Кнопка сброса позиции
    if (ImGui::Button("Reset Rectangle"))
        _rect->SetPosition({0, 0});


    // Текст вместо "Пауза"
    ImGui::InputText("Pause Text", _text->GetContent(), _text->GetContentSize());


    //ImGui::ColorEdit3("Color", _rect->GetColors());
    //ImGui::SameLine();
    ImGui::Checkbox("Draw Rectangle", _rect->GetShouldDrawRef());

    ImGui::Text("Press Space to pause...");

    //ImGui::InputFloat("Speed", _rect->GetSpeed());

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