#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Game.h"
#include "State.h"
#include "MainMenu.h"


class GravitySource
{
    sf::Vector2f pos;
    float strength;
    sf::CircleShape s;


public:
    GravitySource(float pos_x, float pos_y, float strength)
    {
        pos.x = pos_x;
        pos.y = pos_y;
        this->strength = strength;

        s.setPosition(pos);
        s.setFillColor(sf::Color::White);
        s.setRadius(10);

    }

    void render(sf::RenderWindow& wind)
    {
        wind.draw(s);
    }

    sf::Vector2f get_pos()
    {
        return pos;
    }

    float get_strength()
    {
        return strength;
    }
};

class Particle
{
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::CircleShape s;


public:
    Particle(float pos_x, float pos_y, float vel_x, float vel_y)
    {
        pos.x = pos_x;
        pos.y = pos_y;

        vel.x = vel_x;
        vel.y = vel_y;

        s.setPosition(pos);
        s.setFillColor(sf::Color::White);
        s.setRadius(5);

    }

    void render(sf::RenderWindow& wind)
    {
        s.setPosition(pos);
        wind.draw(s);
    }

    void set_color(sf::Color col)
    {
        s.setFillColor(col);
    }

    void update_physcis(GravitySource &s, float deltaTime)
    {
        float speed = 10.f;
        float distance_x = s.get_pos().x - pos.x;
        float distance_y = s.get_pos().y - pos.y;

        float distance = sqrt(distance_x * distance_x + distance_y * distance_y);
        
        float inverse_distance = 1.f / distance;
        
        float normalized_x = inverse_distance * distance_x;
        float normalized_y = inverse_distance * distance_y;

        float inverse_square_dropoff = inverse_distance * inverse_distance;

        float acceleration_x = normalized_x * s.get_strength() * inverse_square_dropoff;
        float acceleration_y = normalized_y * s.get_strength() * inverse_square_dropoff;

        vel.x += acceleration_x * deltaTime * speed;
        vel.y += acceleration_y * deltaTime * speed;

        pos.x += vel.x * deltaTime * speed;
        pos.y += vel.y * deltaTime * speed;

    }
};


class GravitySim : public Engine::State
{
private:
    std::shared_ptr<Context> m_context;

    std::vector<GravitySource> sources;
    std::vector<Particle> particles;

public:
    GravitySim(std::shared_ptr<Context>& context);
    ~GravitySim();

    void Init() override;
    void ProcessInput() override;
    void Update(sf::Time deltaTime) override;
    void Draw() override;

private:
    sf::Color map_val_to_color(float value);
    void handleKeyPressed(sf::Keyboard::Key key);

};
