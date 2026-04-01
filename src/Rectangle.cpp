#include "Rectangle.h"

Rectangle::Rectangle(sf::Vector2f size)
{
    _rectangle = sf::RectangleShape(size);

}

void Rectangle::SetTexture(const std::string& filepath)
{
    _recTexture.loadFromFile(filepath);
    _rectangle.setTexture(&_recTexture);

}

void Rectangle::ReverseMove() 
{
    //_xSpeed *= -1.0f;
    //_ySpeed *= -1.0f;
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
    //_rectangle.setFillColor(sf::Color(_color[0]*255, _color[1]*255, _color[2]*255));
    _rectangle.setPosition({_rectangle.getPosition().x + _xSpeed * _speed, _rectangle.getPosition().y + _ySpeed * _speed});
    
}

void Rectangle::SetPosition(const sf::Vector2f position)
{
    _rectangle.setPosition(position);
    //_position = position;
}

void Rectangle::SaveCurrentPosition()
{
    _position = { _rectangle.getPosition().x, _rectangle.getPosition().y };
}

void Rectangle::SaveCurrentSpeed()
{
    _speedSave = _speed;
}

void Rectangle::SetSpeed(const float speed)
{
    _speed = speed;
}

float* Rectangle::GetSpeed()
{
    return &_speed;
}

void Rectangle::StopMovement()
{

    this->SaveCurrentPosition();
    this->SaveCurrentSpeed();

    this->SetSpeed(0.f);
    _stopped = true;
}

void Rectangle::ContinueMovement()
{
    _stopped = false;
    
    this->SetSpeed(_speedSave);
}

bool Rectangle::IsStopped()
{
    return _stopped;
}