#pragma once
#include "network.hpp"
#include "configuration.hpp"

struct Renderer
{
	TSPNetwork& network;
	std::vector<sf::CircleShape> nodes;
	std::vector<sf::Text> labels;

	Renderer(TSPNetwork& net, sf::Font &f)
		: network{net}
		, nodes{}
	{
		for (size_t i = 0; i < network.cities.size(); i++) {
			nodes.push_back(sf::CircleShape(conf::NODE_RADIUS));

			// Scale map coordinates to screen coordinates
			float lon = conf::WINDOW_SIZE_F.x * (abs(network.cities[i].longitude - conf::LON_BOUNDS.x) / abs(conf::LON_BOUNDS.x - conf::LON_BOUNDS.y));
			float lat = conf::WINDOW_SIZE_F.y * (abs(network.cities[i].latitude - conf::LAT_BOUNDS.x) / abs(conf::LAT_BOUNDS.x - conf::LAT_BOUNDS.y));

			// Render the dot corresponding to the city location
			nodes[i].setPosition({ lon, lat });
			nodes[i].setFillColor(sf::Color::Blue);

			// Render the label with a city name
			labels.push_back(sf::Text(network.cities[i].name, f, conf::LABEL_SIZE));
			labels[i].setPosition(nodes[i].getPosition() + sf::Vector2f{ labels[i].getString().getSize() * (-4.5f), conf::NODE_RADIUS + 3.0f });
			labels[i].setFillColor(sf::Color::Blue);
			labels[i].setStyle(sf::Text::Bold);
		}
	}

	void render(sf::RenderWindow& context) {
		for (size_t i = 0; i < network.cities.size(); i++) {
			context.draw(nodes[i]);
			context.draw(labels[i]);
		}
	}
};