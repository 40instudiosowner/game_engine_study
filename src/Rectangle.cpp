#include "Rectangle.h"
#include <iostream>

Rectangle::Rectangle(sf::Vector2f size)
{
    _rectangle = sf::RectangleShape(size);
    _size = size;
}

void Rectangle::SetTexture(const std::string& filepath)
{
	sf::Texture newTexture;
	if (!newTexture.loadFromFile(filepath))
        std::cerr << "Error loading texture " << filepath << std::endl;

	_recTextures.push_back(newTexture);

    //_rectangle.setTexture(&_recTextures.back());

}

void Rectangle::SetConstrains(const sf::Vector2f constrains)
{
    _constrains = constrains;
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

bool Rectangle::CheckCollisionX()
{
    if (_rectangle.getPosition().x + _size.x >= _constrains.x || _rectangle.getPosition().x <= 0.0f)
		return true;
	else return false;
}

bool Rectangle::CheckCollisionY()
{
    if (_rectangle.getPosition().y + _size.y >= _constrains.y || _rectangle.getPosition().y <= 0.0f)
        return true;
    else return false;
}

void Rectangle::InvertDirectionIfCollided()
{
    auto pos = _rectangle.getPosition();
    auto bounds = _rectangle.getGlobalBounds(); // границы с учетом scale

    // X
    if (pos.x <= 0.0f)
    {
        pos.x = 0.0f;
        _xSpeed = std::abs(_xSpeed); // вправо
    }
    else if (pos.x + bounds.size.x >= _constrains.x)
    {
        pos.x = _constrains.x - bounds.size.x;
        _xSpeed = -_xSpeed; // влево
    }

    // Y
    if (pos.y <= 0.0f)
    {
        pos.y = 0.0f;
        _ySpeed = std::abs(_ySpeed); // вниз
    }
    else if (pos.y + bounds.size.y >= _constrains.y)
    {
        pos.y = _constrains.y - bounds.size.y;
        _ySpeed = -_ySpeed; // вверх
    }

	_rectangle.setPosition(pos); // корректируем позицию, если она вышла за границы
}

void Rectangle::Update()
{
    //_rectangle.setFillColor(sf::Color(_color[0]*255, _color[1]*255, _color[2]*255));
    _rectangle.setPosition({_rectangle.getPosition().x + _xSpeed * _speed, _rectangle.getPosition().y + _ySpeed * _speed});
    if (_updateTexture)
    {
		auto texture = &_recTextures[static_cast<int>(_currentLogo)];
		sf::Vector2f newSize = { static_cast<float>(texture->getSize().x), 
                                 static_cast<float>(texture->getSize().y) };
        _rectangle.setTexture(texture);
        _rectangle.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(texture->getSize().x, texture->getSize().y)));
		_rectangle.setSize(newSize); // устанавливаем размер в соответствии с размером текстуры

	    _updateTexture = false;
    }
    _rectangle.setScale({ _scale, _scale });


	this->InvertDirectionIfCollided();
}

void Rectangle::SetPosition(const sf::Vector2f position)
{
    _rectangle.setPosition(position);
    _position = position;
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

void Rectangle::SetScale(const float scale)
{
    _scale = scale;
	//_rectangle.setScale(_scale, _scale);
}

float* Rectangle::GetScale()
{
	return &_scale;
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

std::vector<std::string>& Rectangle::GetLogos()
{
	return _logos;
}

int Rectangle::GetCurrentLogoIndex() const
{
	return static_cast<int>(_currentLogo);
}

void Rectangle::SetCurrentLogoIndex(Logo idx)
{
    _currentLogo = idx;
	_updateTexture = true;
}