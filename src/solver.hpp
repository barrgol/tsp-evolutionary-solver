#pragma once
#include <numeric>
#include <random>
#include "city.hpp"
#include "network.hpp"
#include "configuration.hpp"


struct TSPSolver {
	TSPNetwork& net;
	std::vector<std::vector<int>> population;
	std::vector<int> bestSolution;

	TSPSolver(TSPNetwork& net)
		: net(net)
		, population{ std::vector<std::vector<int>>(conf::POPULATION_SIZE, std::vector<int>(conf::NUM_NODES - 1)) }
	{
		std::random_device rd;
		std::mt19937 g(rd());

		for (auto& sol : population) {
			std::iota(sol.begin(), sol.end(), 1);
			std::shuffle(sol.begin(), sol.end(), g);
		}

		this->bestSolution = population[0];
	}
};