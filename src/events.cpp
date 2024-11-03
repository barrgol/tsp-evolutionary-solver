#include "events.hpp"


void processEvents(sf::Window& window, AppState& state)
{
    for (auto event = sf::Event{}; window.pollEvent(event);)
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }
            else if (event.key.code == sf::Keyboard::Space)
            {
                state.paused = !state.paused;
            }
            else if (event.key.code == sf::Keyboard::R)
            {
                state.paused = true;
                state.solver.initialize();
            }
        }
    }
}