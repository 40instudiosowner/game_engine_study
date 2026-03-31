#ifndef TEXT_H
#define TEXT_H

#include <iostream>
#include <SFML/Graphics.hpp>

class Text
{
    sf::Font _font; 

    sf::Text _text = sf::Text(_font);

    void Transfer(const Text& other);
public:
    Text() = default;
    Text(const Text &other);
    Text(Text &&other);
    Text(const std::filesystem::path& fontPath, const sf::String text, const int fontSize);

    const int GetCharacterSize() const;
    void SetPosition(const sf::Vector2f position);

    void Draw(sf::RenderTarget& target);

    Text& operator =(const Text &other);
    Text& operator =(Text &&other);
};

#endif //TEXT_H