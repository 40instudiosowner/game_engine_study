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
    this->SetContent(text.toAnsiString());

}

void Text::Draw(sf::RenderTarget& target)
{
    if (_shouldDraw)
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

void Text::SetContent(const std::string& content)
{
    //_content = content.c_str();
	if (content.size() + 1 > _contentSize)
    {
        std::cerr << "Content size exceeds the maximum allowed size of " << _contentSize << " characters.\n";
        return;
    }
	std::copy(content.c_str(), content.c_str() + content.size() + 1, _content);
    //_text.setString(content);
}

char* Text::GetContent() 
{
    return _content;
}

void Text::Update()
{
	_text.setString(_content);
}

size_t Text::GetContentSize()
{
	return _contentSize;
}