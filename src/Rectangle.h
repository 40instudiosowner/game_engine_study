#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <SFML/Graphics.hpp>

enum class Logo { BluRay, CD, DVD };

class Rectangle
{
    sf::RectangleShape _rectangle;
	std::vector<sf::Texture>  _recTextures; // Вектор текстур для разных лого

    sf::Vector2f _size{ 120.0f, 50.0f };
    sf::Vector2f _constrains{ 1920.0f, 1080.0f };

    // Все дефолтные данные надо будет прочитать из конфигурационного файла
    bool _shouldDraw = true;
    bool _stopped = false;
	bool _updateTexture = true; // Флаг для обновления текстуры при смене лого

    float _color[3] = { 1.0f, 0.0f, 0.0f }; 

    float _xSpeed = 1.0f; 
    float _ySpeed = 0.5f;  

    float _speed = 1.0f;
	float _speedSave = 0.0f;    // Сохранение скорости для возможности остановки и продолжения движения

	float _scale = 1.0f;

    sf::Vector2f _position {};

    std::vector<std::string> _logos = { "BluRay", "CD", "DVD" };
	Logo _currentLogo = Logo::BluRay;

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

	void SetScale(const float scale);
	float* GetScale();

    bool IsStopped();

    void SetTexture(const std::string& filepath);

    void StopMovement();
    void ContinueMovement();

	std::vector<std::string>& GetLogos();
	int GetCurrentLogoIndex() const;
	void SetCurrentLogoIndex(Logo idx);

    bool CheckCollisionX();
    bool CheckCollisionY();

	void InvertDirectionIfCollided();

	void SetConstrains(const sf::Vector2f constrains);
};

#endif //RECTANGLE_H