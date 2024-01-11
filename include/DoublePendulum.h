#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "State.h"
#include "MainMenu.h"

class DoublePendulum : public Engine::State
{
private:
    struct TrailPoint
    {
        sf::Vector2f position;
        sf::Color color;
    };

    std::shared_ptr<Context> m_context;

    // Variables for double pendulum simulation
    float m_length1;
    float m_length2;
    float m_angle1;
    float m_angle2;
    float m_angularVelocity1;
    float m_angularVelocity2;
    float m_mass1;
    float m_mass2;

    // SFML graphics components
    sf::CircleShape m_pendulum0;
    sf::CircleShape m_pendulum1;
    sf::CircleShape m_pendulum2;
    sf::VertexArray m_pendulumLines;

    // Trail for fading effect
    sf::VertexArray m_trail;

    const float g = 9.8f; // Acceleration due to gravity

public:
    DoublePendulum(std::shared_ptr<Context> &context);
    ~DoublePendulum();

    void Init() override;
    void ProcessInput() override;
    void Update(sf::Time deltaTime) override;
    void Draw() override;

private:
    void calculatePendulumState(float deltaTime);
    void updatePendulumGraphics();
    void updateTrail();
    void handleKeyPressed(sf::Keyboard::Key key);
};
