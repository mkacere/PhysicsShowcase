#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "PlinkoBall.h"
#include "Game.h"
#include "State.h"

using BallId = int;

class Plinko : public Engine::State {
private:
    class
    std::shared_ptr<Context> m_context;
    
    std::vector<PlinkoBall> m_balls;
    std::vector<sf::CircleShape> m_pegs;
    size_t numRows;

    std::vector<size_t> m_slotValues;
    sf::Vector2f kGraphOffset;
    float kGraphScale;

    const float kPegScale;
    const sf::Vector2f kPegOffset;

public:
    Plinko(std::shared_ptr<Context>& context);
    ~Plinko();

    void Init() override;
    void ProcessInput() override;
    void Update(sf::Time deltaTime) override;
    void Draw() override;

private:
    void createPegs();
    void handleKeyPressed(sf::Keyboard::Key key);
    void addBall(const float ballRadius);

    sf::Vector2f normalize(const sf::Vector2f& vector);

    size_t calculateSlotIndex(float xPosition);
    void createGraph();
    void drawGraph();
};

