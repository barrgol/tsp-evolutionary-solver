#pragma once

namespace conf
{
	// Window configuration
	sf::Vector2u const WINDOW_SIZE = { 1200u, 1200u };
	sf::Vector2f const WINDOW_SIZE_F = static_cast<sf::Vector2f>(WINDOW_SIZE);
	uint32_t const MAX_FRAMERATE = 60;
	float const DT = 1.0f / static_cast<float>(MAX_FRAMERATE);

	// Logic configuration
	const int NUM_NODES = 10;

	// Node drawing configuration
	const float NODE_RADIUS = 20.0f;
	const unsigned int LABEL_SIZE = 28;
}