#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <random>

#include "configuration.hpp"
#include "events.hpp"
#include "city.hpp"
#include "network.hpp"
#include "renderer.hpp"
#include "solver.hpp"

int main()
{    
    // Set a random seed
    std::srand(std::time(NULL));

    // Set up rendering window
    auto window = sf::RenderWindow{ { conf::WINDOW_SIZE.x, conf::WINDOW_SIZE.y }, "TSP Evolutionary solver" };
    window.setFramerateLimit(conf::MAX_FRAMERATE);

    // Load the font
    sf::Font font;
    if (!font.loadFromFile("res/arialbd.ttf"))
    {
        std::cerr << "Error: Could not open 'arialbd.ttf'" << std::endl;
        return EXIT_FAILURE;
    }

    // Load the background
    // Source: https://commons.m.wikimedia.org/wiki/File:Blank_Map_of_The_World_Equirectangular_Projection.png
    sf::Texture europeMap;
    if (!europeMap.loadFromFile("res/background.png")) {
        std::cerr << "Error: Could not open 'background.png'" << std::endl;
        return EXIT_FAILURE;
    }

    sf::Sprite background;
    background.setTexture(europeMap);

    // Load the cities data
    std::ifstream file("res/european_cities.csv");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open 'european_cities.csv'" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<City> allCities;
    std::string line;

    std::getline(file, line); // Omit the column line

    while (std::getline(file, line)) {
        allCities.push_back(City::parse(line));
    }

    // Initialize the TSP network and problem solver
    TSPNetwork net = TSPNetwork(allCities, conf::NUM_NODES);
    TSPSolver solver = TSPSolver(net);
    int ngen = 0;

    // Initialize Renderer
    Renderer r = Renderer(solver, font);

    window.draw(background);
    r.render(window);

    window.display();

    bool paused = true;

    AppState state{ solver, paused };

    while (window.isOpen())
    {   
        processEvents(window, state);

        if (!paused && solver.ngen < conf::NGEN) {
            solver.step();
            ngen++;
        }

        window.draw(background);
        r.render(window);
        
        window.display();

        sf::Clock clock;
        while (clock.getElapsedTime().asMilliseconds() <= conf::FREEZE_DURATION_MS) {};
    }
}