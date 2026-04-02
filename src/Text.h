#ifndef TEXT_H
#define TEXT_H

#include <iostream>
#include <SFML/Graphics.hpp>

#define DEFAULT_TEXT_SIZE 255

class Text
{
    sf::Font _font; 
    sf::Text _text = sf::Text(_font);

	const size_t _contentSize = DEFAULT_TEXT_SIZE;
	char _content[DEFAULT_TEXT_SIZE];

    bool _shouldDraw = false;

    void Transfer(const Text& other);
public:
    Text() = default;
    Text(const Text &other);
    Text(Text &&other);
    Text(const std::filesystem::path& fontPath, const sf::String text, const int fontSize);

    const int GetCharacterSize() const;
    void SetPosition(const sf::Vector2f position);

    void Draw(sf::RenderTarget& target);
    void SetShouldDraw(bool shouldDraw);

    Text& operator =(const Text &other);
    Text& operator =(Text &&other);

	void SetContent(const std::string& content);
	char* GetContent();

	void Update();
    size_t GetContentSize();
};

#endif //TEXT_H