#include "DoublePendulum.h"
#include <math.h>

DoublePendulum::DoublePendulum(std::shared_ptr<Context> &context)
    : m_context(context), m_length1(100.0f), m_length2(400.0f/ 3.0f),
      m_angle1(-(5.0 * M_PI)/4.0), m_angle2(1), m_angularVelocity1(0.0f), m_angularVelocity2(0.0f),
      m_mass1(3.0f), m_mass2(2.0f)
{
    m_trail.setPrimitiveType(sf::Points);
    m_trail.resize(500); 
}

DoublePendulum::~DoublePendulum()
{
}

void DoublePendulum::Init()
{

    // nodes
    m_pendulum0.setRadius(7.25f);
    m_pendulum0.setFillColor(sf::Color::White);
    m_pendulum1.setRadius(15.0f);
    m_pendulum1.setFillColor(sf::Color::Blue);
    m_pendulum2.setRadius(10.0f);
    m_pendulum2.setFillColor(sf::Color::Red);


    // Emplace back two vertices in the pendulumLines array
    m_pendulumLines.setPrimitiveType(sf::LinesStrip);
    m_pendulumLines.resize(3);  // Ensure it has at least two vertices

    // Set the initial positions of pendulum balls based on angles and lengths
    updatePendulumGraphics();
}

void DoublePendulum::ProcessInput()
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

void DoublePendulum::Update(sf::Time deltaTime)
{
    calculatePendulumState(deltaTime.asSeconds());
    updatePendulumGraphics();
    updateTrail();
}

void DoublePendulum::Draw()
{
    m_context->m_window->clear(sf::Color::Black);

    //trail
    m_context->m_window->draw(m_trail);

    // Draw lines connecting pendulum balls
    m_context->m_window->draw(m_pendulumLines);

    // Draw the center node
    m_context->m_window->draw(m_pendulum0);

    // Draw pendulum balls
    m_context->m_window->draw(m_pendulum1);
    m_context->m_window->draw(m_pendulum2);

    m_context->m_window->display();
}


void DoublePendulum::calculatePendulumState(float deltaTime)
{
    const float dt = 0.01f; // Time step for numerical integration

    // Euler's method for numerical integration
    for (int i = 0; i < 10; ++i)
    {
        // Equations of motion for a double pendulum
        float numerator1 = -g * (2 * m_mass1 + m_mass2) * std::sin(m_angle1);
        float numerator2 = -m_mass2 * g * std::sin(m_angle1 - 2 * m_angle2);
        float numerator3 = -2 * std::sin(m_angle1 - m_angle2) * m_mass2;
        float numerator4 = m_angularVelocity2 * m_angularVelocity2 * m_length2 +
                           m_angularVelocity1 * m_angularVelocity1 * m_length1 * std::cos(m_angle1 - m_angle2);
        float denominator = m_length1 * (2 * m_mass1 + m_mass2 - m_mass2 * std::cos(2 * m_angle1 - 2 * m_angle2));

        float alpha1 = (numerator1 + numerator2 + numerator3 * numerator4) / denominator;

        numerator1 = 2 * std::sin(m_angle1 - m_angle2);
        numerator2 = (m_angularVelocity1 * m_angularVelocity1 * m_length1 * (m_mass1 + m_mass2));
        numerator3 = g * (m_mass1 + m_mass2) * std::cos(m_angle1);
        numerator4 = m_angularVelocity2 * m_angularVelocity2 * m_length2 * m_mass2 * std::cos(m_angle1 - m_angle2);
        denominator = m_length2 * (2 * m_mass1 + m_mass2 - m_mass2 * std::cos(2 * m_angle1 - 2 * m_angle2));

        float alpha2 = (numerator1 * (numerator2 + numerator3 + numerator4)) / denominator;

        // Update angles and angular velocities
        m_angle1 += m_angularVelocity1 * dt;
        m_angle2 += m_angularVelocity2 * dt;
        m_angularVelocity1 += alpha1 * dt;
        m_angularVelocity2 += alpha2 * dt;
    }
}

void DoublePendulum::updatePendulumGraphics()
{
    m_pendulum0.setOrigin(m_pendulum0.getRadius(), m_pendulum0.getRadius());

    // Update the positions of pendulum balls based on angles and lengths
    float x1 = m_length1 * std::sin(m_angle1);
    float y1 = m_length1 * std::cos(m_angle1);
    float x2 = x1 + m_length2 * std::sin(m_angle2);
    float y2 = y1 + m_length2 * std::cos(m_angle2);

    m_pendulum1.setPosition(m_context->m_window->getSize().x / 2 + x1, 150 + y1);
    m_pendulum2.setPosition(m_context->m_window->getSize().x / 2 + x2, 150 + y2);

    // Set the origin of pendulum balls to their centers
    m_pendulum1.setOrigin(m_pendulum1.getRadius(), m_pendulum1.getRadius());
    m_pendulum2.setOrigin(m_pendulum2.getRadius(), m_pendulum2.getRadius());

    m_pendulum0.setPosition(m_context->m_window->getSize().x / 2, 150); // Center position for pendulum0

    // Set the positions of the lines
    for (int i = 0; i < 3; ++i)
    {
        if (i == 0)
        {
            // Line from the center node to the first pendulum
            m_pendulumLines[i].position = m_pendulum0.getPosition();
        }
        else
        {
            // Lines connecting pendulum balls
            m_pendulumLines[i].position = sf::Vector2f(m_context->m_window->getSize().x / 2 + (i == 1 ? x1 : x2), 150 + (i == 1 ? y1 : y2));
        }
    }
}

void DoublePendulum::updateTrail()
{
    sf::VertexArray newTrail;

    // Add a new point to the trail
    sf::Vertex trailPoint;
    trailPoint.position = m_pendulum2.getPosition();
    trailPoint.color = sf::Color(255, 255, 255, 200); // Adjust alpha as needed
    m_trail.append(trailPoint);

    for (std::size_t i = 0; i < m_trail.getVertexCount(); ++i)
    {
        sf::Vertex& oldPoint = m_trail[i];
        oldPoint.color.a = static_cast<sf::Uint8>(std::max(0, oldPoint.color.a - 1)); // Adjust the fading speed
        newTrail.append(oldPoint);
    }

    // Swap the new trail with the old one
    std::swap(m_trail, newTrail);
}



void DoublePendulum::handleKeyPressed(sf::Keyboard::Key key)
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