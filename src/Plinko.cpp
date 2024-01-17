#include "Plinko.h"
#include "MainMenu.h"

Plinko::Plinko(std::shared_ptr<Context>& context) : m_context(context) {
    createPegs();
}

Plinko::~Plinko() {
}

void Plinko::Init() {
    // Initialize any necessary components here
}

void Plinko::ProcessInput() {
    sf::Event event;
    while (m_context->m_window->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                m_context->m_window->close();
                break;
            case sf::Event::KeyPressed:
                handleKeyPressed(event.key.code);
                break;
        }
    }
}

void Plinko::Update(sf::Time deltaTime) {
    const float gravity = 1500.0f;      // Adjust gravity as needed
    const float maxFallSpeed = 2000.0f; // Adjust maximum fall speed as needed

    for (auto& ball : m_balls) {
        ball.update(deltaTime.asSeconds(), gravity, maxFallSpeed);


        // Check for collision with pegs
        for (auto& peg : m_pegs) {
            sf::Vector2f pegCenter = peg.getPosition() + sf::Vector2f(peg.getRadius(), peg.getRadius());
            float combinedRadius = peg.getRadius() + ball.getRadius();

            // Calculate the relative position based on the center of the ball
            sf::Vector2f relativePosition = ball.getPosition() - pegCenter;

            // Check for collision by comparing the distance between centers and the sum of radii
            float distance = std::sqrt(relativePosition.x * relativePosition.x + relativePosition.y * relativePosition.y);

            if (distance < combinedRadius) {
                // Resolve collision
                sf::Vector2f normal = relativePosition / distance;

                // Calculate the new velocity after collision (reflect the velocity)
                float dotProduct = ball.getVelocity().x * normal.x + ball.getVelocity().y * normal.y;
                sf::Vector2f newVelocity = (ball.getVelocity() - 2.0f * dotProduct * normal) * 0.5f;

                // Set the new velocity
                ball.setVelocity(newVelocity);

                // Move the ball slightly away from the peg to avoid immediate re-collision
                sf::Vector2f newPosition = ball.getPosition() + normal * (combinedRadius - distance) * 2.5f;
                ball.setPosition(newPosition);
            }
        }
    }

    // Remove balls that have fallen below the window
    m_balls.erase(
        std::remove_if(m_balls.begin(), m_balls.end(),
            [&](const PlinkoBall& ball) {
                return ball.getPosition().y > m_context->m_window->getSize().y;
            }),
        m_balls.end()
    );
}




void Plinko::Draw() {
    m_context->m_window->clear(sf::Color::Black);

    // Draw pegs
    for (const auto& peg : m_pegs) {
        m_context->m_window->draw(peg);
    }

    // Draw balls
    for (int i = 0; i < m_balls.size(); i++) {
        m_balls[i].draw(*m_context->m_window);
    }

    m_context->m_window->display();
}

void Plinko::createPegs() {
    m_pegs.clear(); // Clear any existing pegs

    // Rows (expandable)
    size_t numRows = 7;
    float xSpacing = 40.0f;
    float ySpacing = 40.0f;
    sf::Vector2f totalOffset(125.f, -65.f);

    for (size_t row = 0; row < numRows; ++row) {
        size_t numPegsInRow = 3 + 1 * row; // Increase by 2 for each row

        for (size_t i = 0; i < numPegsInRow; ++i) {
            sf::CircleShape peg(5.0f);
            peg.setFillColor(sf::Color::White);

            float xOffset = (m_context->m_window->getSize().x / 2) - (numPegsInRow * xSpacing / 2.0f) + (i * xSpacing); // Center horizontally
            float yOffset = 100.0f + row * ySpacing;

            peg.setPosition(xOffset + totalOffset.x, yOffset + totalOffset.y);

            m_pegs.push_back(peg);
        }
    }
}


void Plinko::handleKeyPressed(sf::Keyboard::Key key) {
    // Implementation of handleKeyPressed
    switch (key) {
        case sf::Keyboard::Escape:
            m_context->m_states->Add(std::make_unique<MainMenu>(m_context), true);
            break;
        case sf::Keyboard::Space:
            addBall();
            break;
        default:
            break;
    }
}

void Plinko::addBall() {
    // Add a new PlinkoBall to the vector
    PlinkoBall newBall(10.0f);

    float randomXVelocity = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * 10.0f - 5.f;

    newBall.setVelocity(sf::Vector2f(randomXVelocity, 0.0f)); // Start with zero velocity

    float stretch = 20.f;
    float offsetx = 125.f;

    // Define the x-range based on totalOffset from createPegs
    float minX = (m_context->m_window->getSize().x / 2) - stretch;
    float maxX = (m_context->m_window->getSize().x / 2) + stretch;

    // Randomly set the x-position within the defined range
    float randomX = static_cast<float>(std::rand() % static_cast<int>(maxX - minX)) + minX;
    newBall.setPosition(sf::Vector2f(randomX, 0.f) + sf::Vector2f(offsetx - 10.0f, 0));

    m_balls.push_back(newBall);
}
