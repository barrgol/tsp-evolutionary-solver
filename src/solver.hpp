#pragma once
#include <numeric>
#include <random>
#include "city.hpp"
#include "network.hpp"
#include "configuration.hpp"

using Permutation = std::vector<int>;

struct TSPSolver {
	TSPNetwork& net;
	std::vector<Permutation> population;
	Permutation bestSolution;

	std::mt19937 g;
	std::uniform_int_distribution<> unif;

	TSPSolver(TSPNetwork& net)
		: net(net)
		, population{ std::vector<Permutation>(conf::POPULATION_SIZE, Permutation(net.cities.size() - 1)) }
		, unif{0, net.cities.size() - 1}
	{
		std::random_device rd;
		this->g = std::mt19937(rd());

		for (auto& sol : population) {
			std::iota(sol.begin(), sol.end(), 1);
			std::shuffle(sol.begin(), sol.end(), g);
			inversion_mutation(sol);
		}

		this->bestSolution = population[0];

		// TODO: DELETE
		Permutation test1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		Permutation test2 = { 9, 3, 7, 8, 2, 6, 5, 1, 4 };
		pmx_crossover(test1, test2);
	}

	void inversion_mutation(Permutation& p) {
		int x = unif(g);
		int y = unif(g);
		std::reverse(p.begin() + std::min(x,y), p.begin() + std::max(x,y));
	}

	Permutation pmx_crossover(Permutation& p1, Permutation& p2) {
		// Create a map of indices for each gene in both parents
		Permutation ind1(p1.size() + 1);
		Permutation ind2(p2.size() + 1);

		for (size_t i = 0; i < p1.size(); i++) {
			ind1[p1[i]] = i;
			ind2[p2[i]] = i;
		}

		// Initialize the child
		Permutation child(p1.size());

		// Copy part of parent 1 into child
		int x = unif(g);
		int y = unif(g);

		std::copy(p1.begin() + std::min(x, y), p1.begin() + std::max(x, y), child.begin() + std::min(x, y));
		
		// Create a map to keep track of genes that are already present in child
		std::vector<bool> occ(p1.size() + 1, false);
		
		for (size_t i = std::min(x, y); i < std::max(x, y); i++) {
			occ[p1[i]] = true;
		}

		// Copy part of parent 2 according to positions in parent 1
		for (size_t i = std::min(x, y); i < std::max(x, y); i++) {
			if (!occ[p2[i]]) {
				// Look for the target position based on order in p1
				int tpos = ind2[p1[ind2[p2[i]]]];
				while (tpos >= std::min(x, y) && tpos < std::max(x, y)) {
					tpos = ind2[p1[tpos]];
				}

				child[tpos] = p2[i];
				occ[p2[i]] = true;
			}
		}

		// Copy the remaining elements from parent 2
		for (size_t i = 0; i < p2.size(); i++) {
			if (!occ[p2[i]]) {
				// Look for the target position based on order in p1
				child[i] = p2[i];
			}
		}
		
		return child;
	}


};