#pragma once
#include <array>
#include "network.hpp"
#include "solver.hpp"
#include "configuration.hpp"

using Line = std::array<sf::Vertex, 2>;

struct Renderer
{
	TSPSolver& solver;
	TSPNetwork& network;
	std::vector<sf::CircleShape> nodes;
	std::vector<sf::Text> labels;
	std::vector<Line> lines;

	Renderer(TSPSolver& sol, sf::Font &f)
		: solver{sol}
		, network{sol.net}
	{
		for (size_t i = 0; i < network.cities.size(); i++) {
			City c = network.cities[i];
			nodes.push_back(sf::CircleShape(conf::NODE_RADIUS));

			// Scale map coordinates to screen coordinates
			float lon = conf::WINDOW_SIZE_F.x * (abs(c.longitude - conf::LON_BOUNDS.x) / abs(conf::LON_BOUNDS.x - conf::LON_BOUNDS.y));
			float lat = conf::WINDOW_SIZE_F.y * (abs(c.latitude - conf::LAT_BOUNDS.x) / abs(conf::LAT_BOUNDS.x - conf::LAT_BOUNDS.y));

			// Render the dot corresponding to the city location
			nodes[i].setPosition({ lon, lat });
			nodes[i].setFillColor(i == 0 ? sf::Color{ 40, 150, 40 } : sf::Color::Blue);

			// Render the label with a city name
			labels.push_back(sf::Text(c.name, f, conf::LABEL_SIZE));
			labels[i].setPosition(nodes[i].getPosition() + sf::Vector2f{ labels[i].getString().getSize() * (-4.5f), conf::NODE_RADIUS + 3.0f });
			labels[i].setFillColor(i == 0 ? sf::Color{ 40, 150, 40 } : sf::Color::Blue);
			labels[i].setStyle(sf::Text::Bold);
		}

		// Render the path between the nodes
		lines.push_back({ sf::Vertex(nodes[0].getPosition() + sf::Vector2f{ conf::NODE_RADIUS / 2, conf::NODE_RADIUS / 2 }, sf::Color::Black),
						  sf::Vertex(nodes[solver.bestSolution.front()].getPosition() + sf::Vector2f{ conf::NODE_RADIUS / 2, conf::NODE_RADIUS / 2 }, sf::Color::Black) });
		for (int i = 1; i < solver.bestSolution.size(); i++) {
			lines.push_back({ sf::Vertex(nodes[solver.bestSolution[i-1]].getPosition() + sf::Vector2f{ conf::NODE_RADIUS / 2, conf::NODE_RADIUS / 2 }, sf::Color::Black),
							  sf::Vertex(nodes[solver.bestSolution[i]].getPosition() + sf::Vector2f{ conf::NODE_RADIUS / 2, conf::NODE_RADIUS / 2 }, sf::Color::Black) });
		}
		lines.push_back({ sf::Vertex(nodes[solver.bestSolution.back()].getPosition() + sf::Vector2f{ conf::NODE_RADIUS / 2, conf::NODE_RADIUS / 2 }, sf::Color::Black),
					      sf::Vertex(nodes[0].getPosition() + sf::Vector2f{ conf::NODE_RADIUS / 2, conf::NODE_RADIUS / 2 }, sf::Color::Black) });
	}

	void update() {
		lines.front() = { sf::Vertex(nodes[0].getPosition() + sf::Vector2f{ conf::NODE_RADIUS / 2, conf::NODE_RADIUS / 2 }, sf::Color::Black),
						  sf::Vertex(nodes[solver.bestSolution.front()].getPosition() + sf::Vector2f{ conf::NODE_RADIUS / 2, conf::NODE_RADIUS / 2 }, sf::Color::Black) };
		for (int i = 1; i < solver.bestSolution.size(); i++) {
			lines[i] = { sf::Vertex(nodes[solver.bestSolution[i - 1]].getPosition() + sf::Vector2f{ conf::NODE_RADIUS / 2, conf::NODE_RADIUS / 2 }, sf::Color::Black),
						 sf::Vertex(nodes[solver.bestSolution[i]].getPosition() + sf::Vector2f{ conf::NODE_RADIUS / 2, conf::NODE_RADIUS / 2 }, sf::Color::Black) };
		}
		lines.back() = { sf::Vertex(nodes[solver.bestSolution.back()].getPosition() + sf::Vector2f{ conf::NODE_RADIUS / 2, conf::NODE_RADIUS / 2 }, sf::Color::Black),
						 sf::Vertex(nodes[0].getPosition() + sf::Vector2f{ conf::NODE_RADIUS / 2, conf::NODE_RADIUS / 2 }, sf::Color::Black) };
	}

	void render(sf::RenderWindow& context) {
		this->update();
		context.draw(lines.back().data(), 2, sf::Lines);
		for (size_t i = 0; i < network.cities.size(); i++) {
			context.draw(lines[i].data(), 2, sf::Lines);
			context.draw(nodes[i]);
			context.draw(labels[i]);
		}
	}
};