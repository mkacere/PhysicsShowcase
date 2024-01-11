#include "DVD.h"

DVD::DVD(std::shared_ptr<Context> &context) : m_context(context)
{
}

DVD::~DVD()
{
}

void DVD::Init()
{
    m_dvdLogoTexture.loadFromFile("../assets/images/dvd_logo_white.png");
    m_dvdLogo.setTexture(m_dvdLogoTexture);
    m_dvdLogo.scale(.125f, .125f);
    m_dvdLogo.setOrigin(m_dvdLogoTexture.getSize().x / 2.0f, m_dvdLogoTexture.getSize().y / 2.0f);
    setRandomNeonColor();

    sf::Vector2u windowSize = m_context->m_window->getSize();

    float margin = 75.0f;

    // Set initial position randomly within the window boundaries, with a margin
    float randomX = static_cast<float>(std::rand() % (int)(windowSize.x - 2 * margin) + margin);
    float randomY = static_cast<float>(std::rand() % (int)(windowSize.y - 2 * margin) + margin);
    m_dvdLogo.setPosition(randomX, randomY);

    // rand set direction
    float randomDirectionX = (std::rand() % 2 == 0) ? 1.0f : -1.0f; // Either 1.0 or -1.0
    float randomDirectionY = (std::rand() % 2 == 0) ? 1.0f : -1.0f; // Either 1.0 or -1.0
    m_velocity = sf::Vector2f(randomDirectionX * 100.0f, randomDirectionY * 100.0f);
}

void DVD::ProcessInput()
{
    sf::Event event;
    while (m_context->m_window->pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            m_context->m_window->close();
            break;

        case sf::Event::KeyPressed:
            handleKeyPressed(event.key.code);
            break;
        }
    }
}

void DVD::Update(sf::Time deltaTime)
{
    // Update logo position based on velocity
    m_dvdLogo.move(m_velocity * deltaTime.asSeconds());

    float halfWidth = (m_dvdLogo.getLocalBounds().width * m_dvdLogo.getScale().x) / 2.0f;
    float halfHeight = (m_dvdLogo.getLocalBounds().height * m_dvdLogo.getScale().y) / 2.0f;

    // Simple bouncing off the walls
    if (m_dvdLogo.getPosition().x - halfWidth < 0 || m_dvdLogo.getPosition().x + halfWidth > m_context->m_window->getSize().x)
    {
        m_velocity.x = -m_velocity.x;
        // Change color to a random neon color when bouncing
        setRandomNeonColor();
    }

    if (m_dvdLogo.getPosition().y - halfHeight < 0 || m_dvdLogo.getPosition().y + halfHeight > m_context->m_window->getSize().y)
    {
        m_velocity.y = -m_velocity.y;
        // Change color to a random neon color when bouncing
        setRandomNeonColor();
    }
}

void DVD::Draw()
{
    m_context->m_window->clear(sf::Color::Black);
    m_context->m_window->draw(m_dvdLogo);
    m_context->m_window->display();
}

void DVD::setRandomNeonColor()
{
    std::vector<sf::Color> neonColors = {
        sf::Color(255, 0, 0),       // Neon Red
        sf::Color(255, 165, 0),     // Neon Orange
        sf::Color(255, 255, 0),     // Neon Yellow
        sf::Color(0, 255, 0),       // Neon Green
        sf::Color(0, 0, 255),       // Neon Blue
        sf::Color(255, 0, 255),     // Neon Magenta
        sf::Color(255, 69, 0),      // Neon Red-Orange
        sf::Color(255, 215, 0),     // Neon Gold
        sf::Color(50, 205, 50),     // Neon Lime Green
        sf::Color(0, 128, 255),     // Neon Deep Blue
        sf::Color(138, 43, 226),    // Neon Blue-Violet
        sf::Color(255, 20, 147)     // Neon Deep Pink
    };

    m_dvdLogo.setColor(neonColors[std::rand() % neonColors.size()]);
}

void DVD::handleKeyPressed(sf::Keyboard::Key key)
{
    switch (key)
    {
    case sf::Keyboard::Escape:
    
        m_context->m_states->Add(std::make_unique<MainMenu>(m_context), true);
        break;

    // Add more cases for other keys as needed

    default:
        break;
    }
}
