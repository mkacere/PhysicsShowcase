#include "Button.h"

sf::Font Button::m_font;

sf::Font &Button::getFont()
{
    if (!m_font.loadFromFile("../assets/fonts/Arial.ttf"))
    {
        std::cerr << "Error loading font\n";
    }
    return m_font;
}

Button::Button(const sf::Vector2f& position, const sf::Color& color, const std::string& buttonText)
    : m_position(position), m_color(color), m_isSelected(false)
{
    m_buttonShape.setSize(sf::Vector2f(m_buttonWidth, m_buttonHeight));
    m_buttonShape.setPosition(m_position);
    m_buttonShape.setOutlineThickness(2);
    m_buttonShape.setOutlineColor(m_color);
    m_buttonShape.setFillColor(sf::Color::Black);

    m_buttonText.setFont(getFont());
    m_buttonText.setString(buttonText);
    m_buttonText.setCharacterSize(m_textSize);
    m_buttonText.setFillColor(sf::Color::Cyan);
    m_buttonText.setPosition(m_position.x + (m_buttonWidth - m_buttonText.getGlobalBounds().width) / 2, 
                            m_position.y + (m_buttonHeight - m_buttonText.getGlobalBounds().height) / 2.5);
}

Button::~Button()
{
}

void Button::draw(sf::RenderWindow &window)
{
    m_buttonShape.setOutlineThickness(2);

    if(m_isSelected)
    {
        m_buttonShape.setOutlineColor(sf::Color::White);
    } else {
        m_buttonShape.setOutlineColor(m_color);
    }

    window.draw(m_buttonShape);
    window.draw(m_buttonText);
}

bool Button::getSelected() const 
{
    return m_isSelected;
}

void Button::setSelected(bool selected)
{
    m_isSelected = selected;
}

sf::Vector2f Button::getPosition() const
{
    return m_position;
}

sf::Vector2f Button::getOriginalPosition() const
{
    return m_position;
}

void Button::updatePosition(const sf::Vector2f &newPosition)
{
    m_position = newPosition;
    m_buttonShape.setPosition(m_position);
    m_buttonText.setPosition(
        m_position.x + (m_buttonWidth - m_buttonText.getGlobalBounds().width) / 2,
        m_position.y + (m_buttonHeight - m_buttonText.getGlobalBounds().height) / 2.5);

}

sf::Vector2f Button::getSize() const
{
    return sf::Vector2f(m_buttonWidth, m_buttonHeight);
}