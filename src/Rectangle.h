#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <SFML/Graphics.hpp>

class Rectangle
{
    sf::RectangleShape _rectangle;

    // Все дефолтные данные надо будет прочитать из конфигурационного файла
    bool _shouldDraw = true;
    float _color[3] = { 1.0f, 0.0f, 0.0f }; 

    float _xSpeed = 1.0f; 
    float _ySpeed = 0.5f;  

public:
    Rectangle() = default;
    Rectangle(sf::Vector2f size);

    void ReverseMove();
    float* GetColors();
    bool& GetShouldDraw();
    void Draw(sf::RenderTarget& target);
    void Update();
    void SetPosition(const sf::Vector2f position);
};

#endif //RECTANGLE_H