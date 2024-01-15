#pragma once

#include <SFML/Graphics.hpp>

class PlinkoBall{
private:
    sf::CircleShape m_ball;
    sf::Vector2f m_velocity;

public:
    PlinkoBall(float radius = 10.0f) : m_ball(radius) {
        m_ball.setFillColor(sf::Color::Red);
        m_ball.setOrigin(radius, radius);
        m_velocity = sf::Vector2f(0.0f, 0.0f); // Start with zero velocity
    }

    void applyForce(const sf::Vector2f& force){
        m_velocity += force;
    }

    float getRadius() const {
        return m_ball.getRadius();
    }

    void setPosition(const sf::Vector2f& position) {
        m_ball.setPosition(position);
    }

    sf::Vector2f getPosition() const {
        return m_ball.getPosition();
    }

    void setVelocity(const sf::Vector2f& velocity) {
        m_velocity = velocity;
    }

    sf::Vector2f getVelocity() const {
        return m_velocity;
    }

    void update(float deltaTime, float gravity, float maxFallSpeed) {
        // Accelerate the ball in freefall
        m_velocity.y += gravity * deltaTime;

        // Cap the fall speed
        m_velocity.y = std::min(m_velocity.y, maxFallSpeed);

        // Update position based on velocity
        sf::Vector2f position = m_ball.getPosition();
        position += m_velocity * deltaTime;
        m_ball.setPosition(position);
    }

    void draw(sf::RenderWindow& window) const
    {
        window.draw(m_ball);
    }
};
