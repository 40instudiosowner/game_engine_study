#include "Rectangle.h"

Rectangle::Rectangle(sf::Vector2f size)
{
    _rectangle = sf::RectangleShape(size);
}

void Rectangle::ReverseMove() 
{
    _xSpeed *= -1.0f;
    _ySpeed *= -1.0f;
}

float* Rectangle::GetColors()
{
    return _color;
}

bool& Rectangle::GetShouldDraw()
{
    return _shouldDraw;
}

void Rectangle::Draw(sf::RenderTarget& target)
{
    if (_shouldDraw)
        target.draw(_rectangle);
}


void Rectangle::Update()
{
    _rectangle.setFillColor(sf::Color(_color[0]*255, _color[1]*255, _color[2]*255));
    _rectangle.setPosition({_rectangle.getPosition().x + _xSpeed, _rectangle.getPosition().y + _ySpeed});
}

void Rectangle::SetPosition(const sf::Vector2f position)
{
    _rectangle.setPosition(position);
}