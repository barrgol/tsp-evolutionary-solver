#pragma once

namespace conf
{
	// Window configuration
	sf::Vector2u const WINDOW_SIZE = { 1550u, 1200u };
	sf::Vector2f const WINDOW_SIZE_F = static_cast<sf::Vector2f>(WINDOW_SIZE);
	uint32_t const MAX_FRAMERATE = 60;
	float const DT = 1.0f / static_cast<float>(MAX_FRAMERATE);

	// Rendering configuration
	sf::Vector2f const LAT_BOUNDS = { 74.9f, 35.6f };
	sf::Vector2f const LON_BOUNDS = { -11.5f, 40.8f };

	const float NODE_RADIUS = 5.0f;
	const unsigned int LABEL_SIZE = 16;

	// Logic configuration
	const int NUM_NODES = 16;

	// Evolutionary solver configuration
	const int POPULATION_SIZE = 100;

}