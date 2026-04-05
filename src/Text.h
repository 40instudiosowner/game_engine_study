#ifndef TEXT_H
#define TEXT_H

#include "VisualObject.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <filesystem>

#define DEFAULT_TEXT_SIZE 255

class Text : public VisualObject
{
private:
    sf::Font _font;
    sf::Text _text = sf::Text(_font);
    
    const size_t _contentSize = DEFAULT_TEXT_SIZE;
    char _content[DEFAULT_TEXT_SIZE];
    
    void Transfer(const Text& other);
    
public:
    Text() = default;
    Text(const Text &other);
    Text(Text &&other);
    Text(const std::filesystem::path& fontPath, const sf::String text, const int fontSize);
    
    ~Text() override = default;
    
    void Draw(sf::RenderTarget& target) override;
    void Update() override;
    void SetPosition(const sf::Vector2f position) override;
    sf::Vector2f GetPosition() const override { return _text.getPosition(); }
    
    void SetScale(float scale) override { 
        _text.setScale(sf::Vector2f(scale, scale));
    }
    float GetScale() const override { return _text.getScale().x; }
    float* GetScaleRef() override { return nullptr; }
    const int GetCharacterSize() const;
    void SetContent(const std::string& content);
    char* GetContent();
    size_t GetContentSize();
    
    Text& operator =(const Text &other);
    Text& operator =(Text &&other);
};

#endif //TEXT_H