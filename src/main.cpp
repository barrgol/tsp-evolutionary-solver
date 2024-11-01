#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <random>

#include "configuration.hpp"
#include "events.hpp"
#include "city.hpp"
#include "network.hpp"

const int NUM_NODES = 10;

//struct NodeRender {
//    sf::CircleShape shape;
//    sf::Text label;
//
//    NodeRender(Node& n, sf::Font& font) {
//        this->shape = sf::CircleShape(conf::NODE_RADIUS);
//        this->shape.setPosition(n.position);
//        if (n.id == 0) {
//            this->shape.setFillColor(sf::Color::Green);
//        } else {
//            this->shape.setFillColor(sf::Color::Red);
//        }
//
//        this->label = sf::Text(n.name, font, conf::LABEL_SIZE);
//        label.setPosition(this->shape.getPosition() + sf::Vector2f{ conf::NODE_RADIUS / 2, 0.0f });
//        label.setFillColor(sf::Color::Black);
//    }
//
//    void render(sf::RenderWindow& window) {
//        window.draw(this->shape);
//        window.draw(this->label);
//    }
//};
//
//struct TSPNetworkRender {
//    std::vector<NodeRender> nodeRenders;
//
//    TSPNetworkRender(TSPNetwork& tn, sf::Font& font) {
//        for (Node &n : tn.nodes) {
//            nodeRenders.push_back(NodeRender(n, font));
//        }
//    }
//
//    void render(sf::RenderWindow& window) {
//        for (NodeRender& nr : this->nodeRenders) {
//            nr.render(window);
//        }
//    }
//};

int main()
{
    // Set up rendering window
    auto window = sf::RenderWindow{ { conf::WINDOW_SIZE.x, conf::WINDOW_SIZE.y }, "TSP Evolutionary solver" };
    window.setFramerateLimit(conf::MAX_FRAMERATE);

    // Load the font
    sf::Font font;
    if (!font.loadFromFile("res/cour.ttf"))
    {
        std::cerr << "Error: Could not open 'cour.ttf'" << std::endl;
        return EXIT_FAILURE;
    }

    // Load the background
    // Source: https://commons.wikimedia.org/wiki/File:Template_europe_map.png
    // Author: San Jose, 2 April 2006
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

    // Set a random seed
    std::srand(std::time(NULL));

    // Shuffle the city list to obtain random choice of cities
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(allCities.begin(), allCities.end(), g);
    std::vector<City> cities(allCities.begin(), allCities.begin() + NUM_NODES);

    // Pick the random starting point
    std::uniform_int_distribution<> unif(0, cities.size() - 1);
    int randomIndex = unif(g);

    City start = cities.at(randomIndex);

    // Initialize the TSP network
    for (City& c : cities) {
        std::cout << c.name << std::endl;
    }

    std::cout << "START = " << start.name << std::endl;

    while (window.isOpen())
    {   
        processEvents(window);

        window.draw(background);
        
        window.display();
    }
}