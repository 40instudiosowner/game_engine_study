#include <iostream>

#include "UI.h"
#include "Rectangle.h"
#include "Text.h"

UI::UI(sf::RenderWindow& window)
{
    if (!ImGui::SFML::Init(window))
        std::cerr << "Init error";

    ImGui::GetStyle().ScaleAllSizes(1.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;
}

void UI::Update(sf::RenderWindow& window, const std::vector<std::unique_ptr<VisualObject>>& visualObjects, bool& isRun)
{
    ImGui::SFML::Update(window, _deltaClock.restart());

    UpdateUserInput(window, visualObjects, isRun);
    UpdateLogic(visualObjects);
    UpdateGui(visualObjects);
    Render(window, visualObjects);
}

void UI::UpdateUserInput(sf::RenderWindow& window, const std::vector<std::unique_ptr<VisualObject>>& visualObjects, bool& isRun)
{
    auto* tempRect = dynamic_cast<Rectangle*>(visualObjects.front().get());
    auto* tempText = dynamic_cast<Text*>(visualObjects.back().get());

    while (const std::optional event = window.pollEvent())
    {
        ImGui::SFML::ProcessEvent(window, *event);

        if (event->is<sf::Event::Closed>())
        {
            std::cout << "Window closed\n";
            isRun = false;
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
#ifdef DEBUG
            std::wcout << L"Key pressed with code = " << sf::Keyboard::getDescription(keyPressed->scancode).toWideString() << "\n";
#endif

            if (keyPressed->code == sf::Keyboard::Key::Space)
            {
                if (tempRect->IsStopped())
                {
                    tempText->SetShouldDraw(false);
                    tempRect->ContinueMovement();
                }
                else
                {
                    tempText->SetShouldDraw(true);
                    tempRect->StopMovement();
                }
            }
        }
    }
}

void UI::UpdateLogic(const std::vector<std::unique_ptr<VisualObject>>& visualObjects)
{
    for (auto& obj : visualObjects)
        obj->Update();
}

void UI::UpdateGui(const std::vector<std::unique_ptr<VisualObject>>& visualObjects)
{
    auto* tempRect = dynamic_cast<Rectangle*>(visualObjects.front().get());
    auto* tempText = dynamic_cast<Text*>(visualObjects.back().get());

    ImGui::Begin("Settings");

    // Выпадающий список для выбора лого
    if (ImGui::BeginCombo("Logo", tempRect->GetLogos()[tempRect->GetCurrentLogoIndex()].c_str()))
    {
        for (int i = 0; i < tempRect->GetLogos().size(); i++)
        {
            bool isSelected = (tempRect->GetCurrentLogoIndex() == i);
            if (ImGui::Selectable(tempRect->GetLogos()[i].c_str(), isSelected))
                tempRect->SetCurrentLogoIndex(static_cast<Logo>(i));

            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    // Скорость
    ImGui::SliderFloat("Speed", tempRect->GetSpeedRef(), 0.0f, 10.0f);

    // Масштаб
    ImGui::SliderFloat("Scale", tempRect->GetScaleRef(), 0.1f, 5.0f);

    ImGui::SliderFloat("R", &tempRect->GetColorRef()[0], 0.0f, 1.0f);
    ImGui::SliderFloat("G", &tempRect->GetColorRef()[1], 0.0f, 1.0f);
    ImGui::SliderFloat("B", &tempRect->GetColorRef()[2], 0.0f, 1.0f);


    // Кнопка сброса позиции
    if (ImGui::Button("Reset Rectangle"))
        tempRect->SetPosition({ 0, 0 });


    // Текст вместо "Пауза"
    ImGui::InputText("Pause Text", tempText->GetContent(), tempText->GetContentSize());

    //ImGui::SameLine();
    ImGui::Checkbox("Draw Rectangle", tempRect->GetShouldDrawRef());

    ImGui::Text("Press Space to pause...");


    ImGui::End();
}

void UI::Render(sf::RenderWindow& window, const std::vector<std::unique_ptr<VisualObject>>& visualObjects)
{
    window.clear();

    for (auto& obj : visualObjects)
        obj->Draw(window);

    ImGui::SFML::Render(window);
    window.display();
}

void UI::Shutdown()
{
    ImGui::SFML::Shutdown();
}