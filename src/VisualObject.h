#ifndef VISUAL_OBJECT_H
#define VISUAL_OBJECT_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class VisualObject
{
protected:
    bool _shouldDraw = true;
    sf::Vector2f _position{ 0.0f, 0.0f };
    sf::Vector2f _constrains{ 1920.0f, 1080.0f };
    
public:
    VisualObject() = default;
    virtual ~VisualObject() = default;
    
    virtual void Draw(sf::RenderTarget& target) = 0;
    virtual void Update() = 0;
    virtual void SetPosition(const sf::Vector2f position) = 0;
    virtual sf::Vector2f GetPosition() const = 0;
    
    virtual bool GetShouldDraw() const { return _shouldDraw; }
    virtual bool* GetShouldDrawRef() { return &_shouldDraw; }
    virtual void SetShouldDraw(bool shouldDraw) { _shouldDraw = shouldDraw; }
    
    virtual sf::Vector2f GetConstrains() const { return _constrains; }
    virtual void SetConstrains(const sf::Vector2f constrains) { _constrains = constrains; }
    
    virtual void Move(const sf::Vector2f offset) {
        _position += offset;
        ClampPosition();
    }
    
    virtual void SetScale(float scale) = 0;
    virtual float GetScale() const = 0;
    virtual float* GetScaleRef() = 0;
    
protected:
    virtual void ClampPosition() {
        if (_position.x < 0) _position.x = 0;
        if (_position.y < 0) _position.y = 0;
        if (_position.x > _constrains.x) _position.x = _constrains.x;
        if (_position.y > _constrains.y) _position.y = _constrains.y;
    }
};

#endif // VisualObject_H