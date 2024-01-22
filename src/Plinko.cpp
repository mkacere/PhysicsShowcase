#include "Plinko.h"
#include "MainMenu.h"

Plinko::Plinko(std::shared_ptr<Context>& context) : m_context(context), numRows(7),
    kPegOffset(155.f, 100.f), kPegScale(0.87f),
    kGraphOffset(20.f, 110.f), kGraphScale(1.f){
    createGraph();
    createPegs();
}

Plinko::~Plinko() {
}

void Plinko::Init()
{   

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

        float yThreshold = numRows * 40.f * kPegScale + kPegOffset.y;
        if ((ball.getPosition().y > yThreshold) && !ball.getHasBeenScanned()) {
            ball.setHasBeenScanned(true);

            // Calculate slot index based on ball's x position
            size_t slotIndex = calculateSlotIndex(ball.getPosition().x);

            if (slotIndex < m_slotValues.size()) {
                m_slotValues[slotIndex]++;
            }
        }


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

    drawGraph();

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
            for (size_t i = 0; i < 2; i++)
            {
                addBall(10.f);
            }
            
            break;
        default:
            break;
    }
}

void Plinko::addBall(const float ballRadius) {
    // Add a new PlinkoBall to the vector
    PlinkoBall newBall(ballRadius * kPegScale);

    //float randomXVelocity = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * 10.0f - 5.f;
    //randomXVelocity *= kPegScale;  // Scale the random velocity

    newBall.setVelocity(sf::Vector2f(0.f, 0.0f)); // Start with zero velocity

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

size_t Plinko::calculateSlotIndex(float xPosition)
{
    size_t slotIndex = static_cast<size_t>((xPosition - kPegOffset.x + (numRows * 40.0f * kPegScale / 2.0f)) / (40.0f * kPegScale));
    
    // Ensure the slotIndex is within bounds
    return slotIndex - 7;
}


void Plinko::createGraph()
{
    size_t numSlots = numRows + 3;
    m_slotValues.resize(numSlots, 0);
}

void Plinko::drawGraph() {
    sf::RenderWindow& window = *m_context->m_window;
    sf::VertexArray graph(sf::LinesStrip);

    // Calculate the position and spacing for each point on the graph
    float graphWidth = 250.0f;
    float graphHeight = 175.0f;

    // Adjust multiplier as needed for scaling
    float yScale = graphHeight / static_cast<float>(std::max<float>(1, *std::max_element(m_slotValues.begin(), m_slotValues.end())));

    float xSpacing = graphWidth / (m_slotValues.size() - 1);

    for (size_t i = 0; i < m_slotValues.size(); ++i) {
        float xPos = kGraphOffset.x + i * xSpacing;
        float yPos = kGraphOffset.y + graphHeight - (m_slotValues[i] * yScale);

        // Add the point to the graph
        graph.append(sf::Vertex(sf::Vector2f(xPos, yPos), sf::Color::Cyan));

        // Draw the value next to the point (optional)
        sf::Text text(std::to_string(m_slotValues[i]), m_context->m_assets->GetFont(0));
        text.setCharacterSize(12);
        text.setFillColor(sf::Color::White);
        text.setPosition(xPos, yPos - 15.0f); // Adjust as needed
        window.draw(text);
    }

    // Draw the lines connecting the points
    window.draw(graph);
}
