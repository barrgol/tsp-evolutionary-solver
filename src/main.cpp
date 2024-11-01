#include <SFML/Graphics.hpp>
#include <iostream>
#include "configuration.hpp"
#include "events.hpp"

typedef struct Node Node;
typedef struct NodeRender NodeRender;

typedef struct TSPNetwork TSPNetwork;
typedef struct TSPNetworkRender TSPNetworkRender;

struct Node
{
    int id;
    sf::Vector2f position;
    std::string name;

    Node(int id, sf::Vector2f position, std::string name) {
        this->id = id;
        this->position = position;
        this->name = name;
    }

    float distance(Node& n2) {
        return std::sqrt(std::pow(n2.position.x - this->position.x, 2) +
            std::pow(n2.position.y - this->position.y, 2));
    }
};

struct NodeRender {
    sf::CircleShape shape;
    sf::Text label;

    NodeRender(Node& n, sf::Font& font) {
        this->shape = sf::CircleShape(conf::NODE_RADIUS);
        this->shape.setPosition(n.position);
        if (n.id == 0) {
            this->shape.setFillColor(sf::Color::Green);
        } else {
            this->shape.setFillColor(sf::Color::Red);
        }

        this->label = sf::Text(n.name, font, conf::LABEL_SIZE);
        label.setPosition(this->shape.getPosition() + sf::Vector2f{ conf::NODE_RADIUS / 2, 0.0f });
        label.setFillColor(sf::Color::Black);
    }

    void render(sf::RenderWindow& window) {
        sf::CircleShape circle = sf::CircleShape(100.0f);
        circle.setFillColor(sf::Color::Green);
        // window.draw(circle);

        window.draw(this->shape);
        window.draw(this->label);
    }
};

struct TSPNetwork {
    std::vector<Node> nodes;

    TSPNetwork(int numNodes) {
        for (int i = 0; i < numNodes; i++) {
            Node n = Node(i, {}, std::string(1, 65 + i));
            n.position = sf::Vector2f(1.0f * (rand() % ((int)(conf::WINDOW_SIZE.x - 2 * conf::NODE_RADIUS)) + conf::NODE_RADIUS),
                                      1.0f * (rand() % ((int)(conf::WINDOW_SIZE.y - 2 * conf::NODE_RADIUS)) + conf::NODE_RADIUS));

            bool tooClose = true;
            while (tooClose) {
                int j = 0;
                for (; j < i; j++) {
                    if (nodes[j].distance(n) < 3 * conf::NODE_RADIUS) {
                        break;
                    }
                }

                if (j >= i) {
                    tooClose = false;
                }
                else {
                    n.position = sf::Vector2f(1.0f * (rand() % ((int) (conf::WINDOW_SIZE.x - 2 * conf::NODE_RADIUS)) + conf::NODE_RADIUS),
                                              1.0f * (rand() % ((int) (conf::WINDOW_SIZE.y - 2 * conf::NODE_RADIUS)) + conf::NODE_RADIUS));
                }
            }

            nodes.push_back(n);
        }
    }
};

struct TSPNetworkRender {
    std::vector<NodeRender> nodeRenders;

    TSPNetworkRender(TSPNetwork& tn, sf::Font& font) {
        for (Node &n : tn.nodes) {
            nodeRenders.push_back(NodeRender(n, font));
        }
    }

    void render(sf::RenderWindow& window) {
        for (NodeRender& nr : this->nodeRenders) {
            nr.render(window);
        }
    }
};

int main()
{
    // Set up rendering window
    auto window = sf::RenderWindow{ { conf::WINDOW_SIZE.x, conf::WINDOW_SIZE.y }, "TSP Evolutionary solver" };
    window.setFramerateLimit(conf::MAX_FRAMERATE);

    // Load the font
    sf::Font font;
    if (!font.loadFromFile("res/cour.ttf"))
    {
        std::cerr << "Error while loading the font 'cour.ttf'" << std::endl;
        return EXIT_FAILURE;
    }

    // Load the background
    // Source: https://commons.wikimedia.org/wiki/File:Template_europe_map.png
    // Author: San Jose, 2 April 2006
    sf::Texture europeMap;
    if (!europeMap.loadFromFile("res/background.png")) {
        std::cerr << "Error while loading 'background.png'" << std::endl;
        return EXIT_FAILURE;
    }

    sf::Sprite background;
    background.setTexture(europeMap);

    // Set a random seed
    std::srand(std::time(NULL));

    // Initialize the TSP Network
    TSPNetwork tn = TSPNetwork(conf::NUM_NODES);
    TSPNetworkRender tnr = TSPNetworkRender(tn, font);

    sf::CircleShape shape = sf::CircleShape(50);
    shape.setPosition({ 100, 100 });
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {   
        processEvents(window);

        window.draw(background);
        tnr.render(window);
        
        window.display();
    }
}