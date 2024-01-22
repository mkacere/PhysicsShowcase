#include "MainMenu.h"

#include "DVD.h"
#include "DoublePendulum.h"
#include "Plinko.h"

#include <memory>

#include <unistd.h>

#include <SFML/Window/Event.hpp>

MainMenu::MainMenu(std::shared_ptr<Context> &context) 
    : m_context(context), m_selectedButtonIndex(0)
{
}

MainMenu::~MainMenu()
{
}

void MainMenu::Init()
{
    m_context->m_assets->AddFont(MAIN_FONT, "../assets/fonts/Arial.ttf");
    m_mainTitle.setFont(m_context->m_assets->GetFont(MAIN_FONT));
    m_mainTitle.setString("Physics Showcase");
    m_mainTitle.setPosition(m_context->m_window->getSize().x / 2 - m_mainTitle.getLocalBounds().width / 2, 20);
    
    titleBanner.setSize(sf::Vector2f(m_context->m_window->getSize().x, 70));
    titleBanner.setFillColor(sf::Color::Black);  // Set the banner color
    titleBanner.setOutlineThickness(3);
    titleBanner.setOutlineColor(sf::Color::Cyan);

    m_imageSprite.setPosition(m_context->m_window->getSize().x / 2, 115);

    m_imageOutline.setFillColor(sf::Color(0, 0, 0, 0));
    m_imageOutline.setPosition(m_imageSprite.getPosition());
    m_imageOutline.setSize(sf::Vector2f(250, 250));
    m_imageOutline.setOutlineThickness(4);
    m_imageOutline.setOutlineColor(sf::Color::Cyan);

    m_buttonNames = {"DVD", "Double Pendulum", "Plinko", "2", "1", "josh", "is", "zestyyyyy af"};
    for (size_t i = 0; i < m_buttonNames.size(); ++i)
    {
        m_buttons.emplace_back(sf::Vector2f(50, (m_context->m_window->getSize().y / 2) + i * 60), sf::Color::Blue, m_buttonNames[i]);
    }

    if (!m_buttons.empty())
    {
        m_buttons[0].setSelected(true);
    }

    m_imageLinks = {"../assets/previews/Screenshot 2024-01-07 at 12.41.19 AM.png", 
                    "../assets/previews/DoublePendPrev.jpg", 
                    "../assets/previews/plinkoprev.png", 
                    "../assets/previews/download.jpeg", 
                    "",
                    "",
                    ""};

    if (!m_imageTexture.loadFromFile(m_imageLinks[0]))
    {
        std::cerr << "Failed to load texture: " << m_imageLinks[m_selectedButtonIndex] << "\n";
    }
    else
    {
        sf::Vector2u originalSize = m_imageTexture.getSize();

        // Calculate the scaling factors to fit within a 250x250 rectangle
        float scaleX = 250.0f / originalSize.x;
        float scaleY = 250.0f / originalSize.y;
        m_imageSprite.setTexture(m_imageTexture);
        m_imageSprite.scale(scaleX, scaleY);
    }
}

void MainMenu::ProcessInput()
{
    sf::Event event;
    while (m_context->m_window->pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            m_context->m_window->close();
        else if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::W:
            case sf::Keyboard::Up:
            {
                moveButtons(-1);
                break;
            }
            case sf::Keyboard::S:
            case sf::Keyboard::Down:
            {
                moveButtons(1);
                break;
            }
            case sf::Keyboard::Return:
                returnedIndex = m_selectedButtonIndex;
                //std::cout << "Button Clicked: " <<m_buttonNames[m_selectedButtonIndex] << "\n";
                handleStateTransition();
                break;
            }
        }
    }
}

void MainMenu::Update(sf::Time deltaTime) 
{
}



void MainMenu::Draw()
{
    m_context->m_window->clear(sf::Color::Black);

    for (size_t i = 0; i < m_buttons.size(); ++i) 
    {
        m_buttons[i].draw(*m_context->m_window);
    }
    m_context->m_window->draw(titleBanner);

    m_context->m_window->draw(m_mainTitle);

    m_context->m_window->draw(m_imageSprite);

    m_context->m_window->draw(m_imageOutline);

    m_context->m_window->display();
}

void MainMenu::moveButtons(int direction)
{
    // fixed spacing
    const float buttonSpacing = 60.0f;

    size_t numButtons = m_buttons.size();

    m_buttons[m_selectedButtonIndex].setSelected(false);
    m_selectedButtonIndex = (m_selectedButtonIndex + direction + numButtons) % numButtons;
    m_buttons[m_selectedButtonIndex].setSelected(true);

    // load image texture
    if (!m_imageLinks[m_selectedButtonIndex].empty())
    {
        if (m_imageTexture.loadFromFile(m_imageLinks[m_selectedButtonIndex]))
        {
            sf::Vector2u originalSize = m_imageTexture.getSize();

            float scaleX = 250.0f / originalSize.x;
            float scaleY = scaleX;

            // Set the scale and position of the sprite
            m_imageSprite.setScale(sf::Vector2f(scaleX, scaleY));
            //m_imageSprite.setPosition(m_context->m_window->getSize().x / 2 - m_imageSprite.getGlobalBounds().width / 2, 115);
        }
        else
        {
            std::cerr << "Failed to load texture: " << m_imageLinks[m_selectedButtonIndex] << std::endl;
            // Use a default blank texture
            if (!m_imageTexture.loadFromFile("../assets/previews/blank.png"))
            {
                std::cerr << "Failed to load default blank texture\n";
            }
        }
    }
    else
    {
        // Use a default blank texture
        if (!m_imageTexture.loadFromFile("../assets/previews/blank.png"))
        {
            std::cerr << "Failed to load default blank texture\n";
        }
    }

    m_imageSprite.setTexture(m_imageTexture);

    for (size_t i = 0; i < numButtons; ++i)
    {
        float newY = m_context->m_window->getSize().y / 2 + i * buttonSpacing - m_selectedButtonIndex * buttonSpacing;
        m_buttons[i].updatePosition(sf::Vector2f(50, newY));
    }

}

void MainMenu::handleStateTransition()
{
    if (m_context->m_states)
    {
        //stateAdded = false;
        //m_context->m_states->PopCurrent();

        switch (returnedIndex)
        {
        case 0:
            std::cout << "case " << returnedIndex << " was called\n";
            m_context->m_states->Add(std::make_unique<DVD>(m_context), true);
            break;
        case 1:
            std::cout << "case " << returnedIndex << " was called\n";
            m_context->m_states->Add(std::make_unique<DoublePendulum>(m_context), true);
            break;
        case 2:
            std::cout << "case " << returnedIndex << " was called\n";
            m_context->m_states->Add(std::make_unique<Plinko>(m_context), true);
            break;
        }
    }
}
