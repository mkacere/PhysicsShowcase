#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "State.h"
#include "MainMenu.h"

class DVD : public Engine::State
{
private:
    std::shared_ptr<Context> m_context;
    sf::Texture m_dvdLogoTexture;
    sf::Sprite m_dvdLogo;
    sf::Vector2f m_velocity;

public:
    DVD(std::shared_ptr<Context> &context);
    ~DVD();

    void Init() override;
    void ProcessInput() override;
    void Update(sf::Time deltaTime) override;
    void Draw() override;

    void setRandomNeonColor();

private:
    void handleKeyPressed(sf::Keyboard::Key key);
};