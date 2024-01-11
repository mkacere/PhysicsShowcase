#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Button
{
private:
    static constexpr float m_buttonWidth = 200.0f;
    static constexpr float m_buttonHeight = 50.0f;
    static constexpr unsigned int m_textSize = 20;

    sf::Vector2f m_position;
    sf::Color m_color;
    sf::RectangleShape m_buttonShape;
    sf::Text m_buttonText;
    static sf::Font m_font;

    bool m_isSelected;

    static sf::Font& getFont();

public:
    Button(const sf::Vector2f &position, const sf::Color &color, const std::string &buttonText);
    ~Button();

    void draw(sf::RenderWindow &window);

    bool getSelected() const;
    void setSelected(bool selected);

    sf::Vector2f getPosition() const;

    sf::Vector2f getOriginalPosition() const;

    void updatePosition(const sf::Vector2f& newPosition);

    sf::Vector2f getSize() const;
};
