#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "VisualObject.h"
#include <SFML/Graphics.hpp>
#include <vector>

enum class Logo { BluRay, CD, DVD };

class Rectangle : public VisualObject
{
private:
    sf::RectangleShape _rectangle;
    std::vector<sf::Texture> _recTextures;
    
    sf::Vector2f _size{ 120.0f, 50.0f };
    
    bool _stopped = false;
    bool _updateTexture = true;
    
    float _color[3] = { 1.0f, 0.0f, 0.0f };
    float _xSpeed = 1.0f;
    float _ySpeed = 0.5f;
    float _speed = 1.0f;
    float _speedSave = 0.0f;
    float _scale = 1.0f;
    
    std::vector<std::string> _logos = { "BluRay", "CD", "DVD" };
    Logo _currentLogo = Logo::BluRay;
    
public:
    Rectangle() = default;
    Rectangle(sf::Vector2f size);
    
    void Draw(sf::RenderTarget& target) override;
    void Update() override;
    void SetPosition(const sf::Vector2f position) override;
    sf::Vector2f GetPosition() const override { return _rectangle.getPosition(); }
    
    void SetScale(float scale) override { 
        _scale = scale;
        _rectangle.setScale(sf::Vector2f(scale, scale));
    }

    void SetColor(const float r, const float g, const float b) { 
        _color[0] = r;
        _color[1] = g;
        _color[2] = b;
    }

    float* GetColorRef() { return _color; }


    float GetScale() const override { return _scale; }
    float* GetScaleRef() override { return &_scale; }
    
    void ReverseMove();
    float* GetColors();
    void SaveCurrentPosition();
    void SaveCurrentSpeed();
    
    void SetSpeed(const float speed);
    float* GetSpeed();
    
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
};

#endif //RECTANGLE_H