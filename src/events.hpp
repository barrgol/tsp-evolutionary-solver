#pragma once
#include <SFML/Window.hpp>
#include "solver.hpp"

struct AppState {
	TSPSolver& solver;
	bool& paused;
};

void processEvents(sf::Window& window, AppState& state);
