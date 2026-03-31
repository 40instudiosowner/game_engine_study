#include <iostream>
#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

int main()
{
    setlocale(LC_ALL, "");

    // Создадим новое окно размером w*h пикселей.
    // Верхний левый угол окна - (0,0), а нижний правый - (w,h)
    const int wWidth = 1280;
    const int wHeight = 720;
    sf::RenderWindow window(sf::VideoMode({wWidth, wHeight}), "Demo");

    auto desktop = sf::VideoMode::getDesktopMode();
    window.setPosition({ ((int) desktop.size.x) / 2 - wWidth / 2, ((int) desktop.size.y) / 2 - wHeight / 2 });

    window.setFramerateLimit(60);

    // Инициализируем ImGui и создадим таймер для внутреннего отсчета дельты фрейма
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

    // Масштабируем ImGui на указанный множитель
    ImGui::GetStyle().ScaleAllSizes(1.0f);
    // Для текста свой множитель масштаба
    //ImGui::GetIO().FontGlobalScale = 2.0f;

    // Создаем SFML прямоугольник 
    sf::RectangleShape rectangle({120.f, 50.f});
    rectangle.setPosition({10.0f, 10.0f});

    bool drawRectangle = true;

    // Цвет (r, g, b) в ImGui задается отрезком [0,1] вместо [0-255], как в SFML
    float rectangleColor[3] = {1.0f, 0.0f, 0.0f};

    // Будем использовать эти значения  позже для того, чтобы двигать прямоугольник
    float rectangleSpeedX = 1.0f;  
    float rectangleSpeedY = 0.5f; 

    // Загрузим шрифт, чтобы отрисовать текст
    sf::Font myFont;

    // Читаем шрифт из файла
    if (!myFont.openFromFile("../fonts/futura.ttf"))
    {
        // Если не смогли прочесть шрифт - выводим ошибку в stderr и выходим
        std::cerr << "Could not load font!\n";
        exit(-1);
    }

    // Подготоваливаем объект текста для отрисовки на экране
    sf::Text text = sf::Text(myFont, L"Текст", 24);

    // Разместим верхний левый угол текста так, чтобы текст выравнивался по нижней стороне окна.
    // Размер символа текста указан в пикселях, поэтому сдвигаем текст вверх от нижней границы на его высоту
    text.setPosition({0, wHeight - (float) text.getCharacterSize()});

    // Основной цикл - крутится каждый кадр, пока открыто окно 
    while (window.isOpen())
    {
        // Обработка событий
        while (const std::optional event = window.pollEvent())
        {
            // Передача события в ImGui для обработки им
            ImGui::SFML::ProcessEvent(window, *event);

            // Это событие дергается, если окно было закрыто по кнопке
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())  // Это событие дергается, если была нажата клавиша
            {
                // Печатаем, что за клавиша в консоль
                std::wcout << L"Key pressed with code = " << sf::Keyboard::getDescription(keyPressed->scancode).toWideString() << "\n";

                // Например, при нажатой английской B
                if (keyPressed->code == sf::Keyboard::Key::B)
                {
                    // Меняем направление движения прямоугольника
                    rectangleSpeedX *= -1.0f;
                    rectangleSpeedY *= -1.0f;
                }
            }
        }

        // Обновляем ImGui на этом фрейме с временем, прошедшим с прошлого фрейма
        ImGui::SFML::Update(window, deltaClock.restart());

        //ImGui::ShowDemoWindow();

        // Рисуем UI
        ImGui::Begin("Window Title");
        ImGui::Text("Press B to invert movement");

        ImGui::ColorEdit3("Color", rectangleColor);
        ImGui::SameLine();
        ImGui::Checkbox("Draw Rectangle", &drawRectangle);

        if (ImGui::Button("Reset Rectangle"))
            rectangle.setPosition({0, 0});

        ImGui::End();

        // Устанавливаем цвет прямоугольника, т.к. он мог измениться через UI
        rectangle.setFillColor(sf::Color(rectangleColor[0]*255, rectangleColor[1]*255, rectangleColor[2]*255));
        // Базовая анимация - двигаем прямоугольник
        rectangle.setPosition({rectangle.getPosition().x + rectangleSpeedX, rectangle.getPosition().y + rectangleSpeedY});

        // Вызовы методов отрисовки
        window.clear(); // Очищаем окно от предыдущей отрисовки (очистка буфера)
        
        if (drawRectangle)  // Рисуем прямоугольник, если true
            window.draw(rectangle);

        window.draw(text);

        ImGui::SFML::Render(window);  // Отрисовываем UI в самом конце, т.к. он должен быть поверх
        window.display();  // Вызываем метод отрисовки всего окна
    }

    ImGui::SFML::Shutdown();

    return 0;
}