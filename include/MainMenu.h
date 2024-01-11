#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics/Text.hpp>

#include "State.h"
#include "Game.h"
#include "Button.h"

class MainMenu : public Engine::State
{
private:
    std::shared_ptr<Context> m_context;
    sf::Text m_mainTitle;
    sf::RectangleShape titleBanner;
    std::vector<Button> m_buttons;
    int m_selectedButtonIndex; 
    int returnedIndex;
    std::vector<std::string> m_buttonNames;

    std::vector<std::string> m_imageLinks;
    sf::Texture m_imageTexture;
    sf::Sprite m_imageSprite;
    sf::RectangleShape m_imageOutline;

    bool stateAdded;

public:
    MainMenu(std::shared_ptr<Context>& context);
    ~MainMenu();

    void Init() override;
    void ProcessInput() override;
    void Update(sf::Time deltaTime) override;
    void Draw() override;

    void moveButtons(int direction);

    void handleStateTransition();
};