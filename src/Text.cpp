#include "Text.h"

Text::Text(const Text &other) 
{
    Transfer(other);
}

Text::Text(Text &&other)
{
    Transfer(other);
}

Text::Text(const std::filesystem::path& fontPath, const sf::String text, const int fontSize)
{
    if (!_font.openFromFile(fontPath))
    {
        std::cerr << "Could not load font!\n";
        exit(-1);
    }

    _text = sf::Text(_font, text, fontSize);
}

void Text::Draw(sf::RenderTarget& target)
{
    target.draw(_text);
}

const int Text::GetCharacterSize() const
{
    return _text.getCharacterSize();
}

void Text::SetPosition(const sf::Vector2f position)
{
    _text.setPosition(position);
}

Text& Text::operator =(const Text &other)
{
    Transfer(other);

    return *this;
}

Text& Text::operator =(Text &&other)
{
    Transfer(other);

    return *this;
}

void Text::Transfer(const Text& other)
{
    _font = other._font;
    _text = other._text;

    // It is important to note that the sf::Text instance doesn't copy the font that it uses, 
    // it only keeps a reference to it. Thus, a sf::Font must not be destructed while it is 
    // used by a sf::Text (i.e. never write a function that uses a local sf::Font instance 
    // for creating a text). - https://www.sfml-dev.org/documentation/3.0.0/classsf_1_1Text.html
    _text.setFont(_font);
}