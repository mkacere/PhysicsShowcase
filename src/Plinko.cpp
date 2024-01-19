#include "Plinko.h"
#include "MainMenu.h"

Plinko::Plinko(std::shared_ptr<Context>& context) : m_context(context), 
    kPegOffset(135.f, 100.f), kPegScale(0.87f){
    createPegs();
}

Plinko::~Plinko() {
}

void Plinko::Init()
{
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
    const float gravity = 1250.0f * kPegScale;      // Adjust gravity as needed
    const float maxFallSpeed = 450.0f * kPegScale; // Adjust maximum fall speed as needed
    const float bounceFactor = 0.5f * kPegScale;
    const float minBounceMagnitude = 78.5f * kPegScale;


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
                sf::Vector2f newVelocity = (ball.getVelocity() - 2.0f * dotProduct * normal) * bounceFactor;


                // Check if the magnitude is below the threshold
                float newVelocityMagnitude = std::sqrt(newVelocity.x * newVelocity.x + newVelocity.y * newVelocity.y);
                if (newVelocityMagnitude < minBounceMagnitude) {
                    // Normalize the new velocity and multiply by the minimum bounce magnitude
                    newVelocity = normalize(newVelocity) * minBounceMagnitude;
                }

                // Set the new velocity
                ball.setVelocity(newVelocity);

                // Move the ball slightly away from the peg to avoid immediate re-collision
                sf::Vector2f newPosition = ball.getPosition() + normal * (combinedRadius - distance) * 1.001f;
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
    numRows = 7;
    const float xSpacing = 40.0f * kPegScale;
    const float ySpacing = 40.0f * kPegScale;

    for (size_t row = 0; row < numRows; ++row) {
        size_t numPegsInRow = 3 + 1 * row; // Increase by 2 for each row

        for (size_t i = 0; i < numPegsInRow; ++i) {
            sf::CircleShape peg(5.0f * kPegScale);
            peg.setFillColor(sf::Color::White);

            float xOffset = (m_context->m_window->getSize().x / 2) - (numPegsInRow * xSpacing / 2.0f) + (i * xSpacing);
            float yOffset = row * ySpacing;

            peg.setPosition(xOffset + kPegOffset.x, yOffset + kPegOffset.y);

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
            addBall(10.0f);
            break;
        default:
            break;
    }
}

void Plinko::addBall(const float ballRadius) {
    // Add a new PlinkoBall to the vector
    PlinkoBall newBall(ballRadius * kPegScale);

    float randomXVelocity = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * 10.0f - 5.f;
    randomXVelocity *= kPegScale;  // Scale the random velocity

    newBall.setVelocity(sf::Vector2f(randomXVelocity, 0.0f)); // Start with zero velocity

    const float xSpan = 22.f * kPegScale;

    // Define the x-range based on totalOffset from createPegs
    const float minX = (m_context->m_window->getSize().x / 2) - xSpan;
    const float maxX = (m_context->m_window->getSize().x / 2) + xSpan;

    // Randomly set the x-position within the defined range
    float randomX = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (maxX - minX) + minX;
    const float scaledRadius = ballRadius * kPegScale;

    // Adjust the position to ensure it stays within screen bounds
    randomX = std::max(scaledRadius, std::min(randomX, m_context->m_window->getSize().x - scaledRadius));

    newBall.setPosition(sf::Vector2f(randomX, -ballRadius * kPegScale) + sf::Vector2f(kPegOffset.x - scaledRadius, 0));

    m_balls.push_back(newBall);
}

sf::Vector2f Plinko::normalize(const sf::Vector2f &vector)
{
    float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
        if (length != 0.0f) {
            return vector / length;
        } else {
            return vector;
        }
}