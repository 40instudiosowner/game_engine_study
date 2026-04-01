#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <SFML/Graphics.hpp>

class Rectangle
{
    sf::RectangleShape _rectangle;
    sf::Texture        _recTexture;

    // Все дефолтные данные надо будет прочитать из конфигурационного файла
    bool _shouldDraw = true;
    bool _stopped = false;

    float _color[3] = { 1.0f, 0.0f, 0.0f }; 

    const float _xSpeed = 1.0f; 
    const float _ySpeed = 0.5f;  

    float _speed = 1.0f;
    float _speedSave = 0.0f;

    sf::Vector2f _position;

public:
    Rectangle() = default;
    Rectangle(sf::Vector2f size);

    void ReverseMove();
    float* GetColors();
    bool& GetShouldDraw();
    void Draw(sf::RenderTarget& target);
    void Update();
    void SetPosition(const sf::Vector2f position);
    void SaveCurrentPosition();
    void SaveCurrentSpeed();

    void SetSpeed(const float speed);
    float* GetSpeed();

    bool IsStopped();

    void SetTexture(const std::string& filepath);

    void StopMovement();
    void ContinueMovement();

};

#endif //RECTANGLE_H