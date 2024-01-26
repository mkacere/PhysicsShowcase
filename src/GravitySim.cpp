#include "GravitySim.h"

sf::Color GravitySim::map_val_to_color(float value)
{
    if (value < 0.0f) value = 0;
    if (value > 1.0f) value = 1;

    int r = 0, g = 0, b = 0;

    if (value < 0.5f){
        b = 255* (1.0f - 2 * value);
        g = 255 * 2 * value;
    } else {
        g = 255 * (2.0f - 2 * value);
        r = 255 * (2 * value - 1);
    }
    return sf::Color(r, g, b);}

void GravitySim::handleKeyPressed(sf::Keyboard::Key key)
{
    switch (key) {
        case sf::Keyboard::Escape:
            m_context->m_states->Add(std::make_unique<MainMenu>(m_context), true);
            break;
    }
}

GravitySim::GravitySim(std::shared_ptr<Context>& context) : m_context(context)
{
    m_context->m_window->setFramerateLimit(60);
}


GravitySim::~GravitySim()
{
}

void GravitySim::Init()
{
    // Clear existing sources and particles
    sources.clear();
    particles.clear();

    // Add sources with confined positions and reduced strength
   sources.push_back(GravitySource(500 * 600 / 1600, 500 * 400 / 1000, 7000));  // Adjusted position
    sources.push_back(GravitySource(1200 * 600 / 1600, 1200 * 400 / 1000, 7000)); 
    // Adjust the number of particles based on the confined window size
    int num_particles = 10000;

    for (int i = 0; i < num_particles; i++)
    {
        particles.push_back(Particle(600 * 600 / 1600, 700 * 400 / 1000, 1, 0.05 + (0.025 / num_particles) * i));  // Adjusted initial position

        float val = (float)i / (float)num_particles;

        sf::Color col = map_val_to_color(val);

        particles[i].set_color(col);
    }
}




void GravitySim::ProcessInput()
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

void GravitySim::Update(sf::Time deltaTime)
{
    float seconds = deltaTime.asSeconds();

    for (int i = 0; i < sources.size(); i++){
        for (int j = 0; j <particles.size(); j++){
            particles[j].update_physcis(sources[i], seconds);
        }
    }
}

void GravitySim::Draw()
{
    m_context->m_window->clear(sf::Color::Black);

    for (int i = 0; i < sources.size(); i++){
        sources[i].render(*m_context->m_window);
    }

    for (int j = 0; j < particles.size(); j++){
        particles[j].render(*m_context->m_window);
    }
    m_context->m_window->display();
}
