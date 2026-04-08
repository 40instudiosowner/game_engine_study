#include "Window.h"
#include <iostream>

Window::Window(const unsigned int wWidth, const unsigned int wHeight, ConfigReader& config) 
{
    _window.create(sf::VideoMode({wWidth, wHeight}), "Slider");

    auto desktop = sf::VideoMode::getDesktopMode();
    _window.setPosition({ (int) (desktop.size.x / 2 - wWidth / 2), (int) (desktop.size.y / 2 - wHeight / 2) });

    _window.setFramerateLimit(60);

    _ui = std::make_unique<UI>(_window);

    setConfig(config);
    Initialize();
}

int Window::Initialize()
{
    if (!_config)
    {
        std::cerr << "INIT ERROR! Config is not set!";
        return 1;
    }
 
    // ЛОГО 
    _visualObjects.push_back(std::make_unique<Rectangle>(sf::Vector2f{ static_cast<float>(_config->getLogoWidth()),
                                                                       static_cast<float>(_config->getLogoHeight()) }));

    _visualObjects.front()->SetPosition({_config->getLogoPositionX(), _config->getLogoPositionY()});
    _visualObjects.front()->SetShouldDraw(true);

    _logoPaths = _config->getLogoPaths();
    // загрузка всех текстур из доступных путей и установка их в Rectangle
    auto* tempRec = dynamic_cast<Rectangle*>(_visualObjects.front().get());
    for (auto& path : _logoPaths)
    {
        tempRec->SetTexture(path);
    }

    _visualObjects.front()->SetScale(_config->getLogoScaleMultiplier());
    _visualObjects.front()->SetConstrains({ static_cast<float>(_window.getSize().x), static_cast<float>(_window.getSize().y) });


    // ТЕКСТ 
    _visualObjects.push_back(std::make_unique<Text>(_config->getFontPath(), _config->getText(), _config->getFontSize()));
    auto* tempText = dynamic_cast<Text*>(_visualObjects.back().get());
    _visualObjects.back()->SetPosition({_window.getSize().x / 2.f,
                                        _window.getSize().y / 2.f - static_cast<float>(tempText->GetCharacterSize()) });

    _initialized = true;
    return 0;
}

void Window::setConfig(ConfigReader& config)
{
    _config = std::make_unique<ConfigReader>(config);
};

void Window::Run()
{
    while (_isRun) 
    {
        _ui->Update(_window, _visualObjects, _isRun);
    }

    _window.close();

    _ui->Shutdown();
}
