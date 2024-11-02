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

		for (size_t i = 1; i < net.cities.size(); i++) {
			std::cout << net.cities[i - 1].name << " - " << net.cities[i].name << ": " << net.cities[i - 1].distance(net.cities[i]) << " km" << std::endl;
		}

		std::cout << "Path length = " << fitness(bestSolution) << " km" << std::endl;
	}

	int fitness(Permutation& p) {
		int res = net.cities[0].distance(net.cities[p[0]]);
		for (size_t i = 1; i < p.size(); i++) {
			res += net.cities[p[i - 1]].distance(net.cities[p[i]]);
		}
		res += net.cities[p.back()].distance(net.cities[0]);

		return res;
	}

	std::vector<int> evaluate(std::vector<Permutation> individuals) {
		std::vector<int> fitnesses(individuals.size(), 0);
		for (size_t i = 0; i < individuals.size(); i++) {
			fitnesses[i] = fitness(individuals[i]);
		}

		return fitnesses;
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

	std::vector<Permutation> tournament_selection(std::vector<Permutation> individuals, std::vector<int> fitnesses) {
		std::uniform_int_distribution<> distr(0, individuals.size());
		std::vector<Permutation> parents{};
		std::vector<int> indices(0, conf::K);
		std::vector<int> tourFitnesses(0, conf::K);

		auto argmax = [](std::vector<int> v) { return std::distance(v.begin(), std::max_element(v.begin(), v.end())); };

		for (size_t i = 0; i < 2 * individuals.size(); i++) {
			for (size_t j = 0; j < conf::K; j++) {
				indices[j] = distr(g);
				tourFitnesses[j] = fitnesses[indices[j]];
			}
			parents.push_back(individuals[indices[argmax(tourFitnesses)]]);
		}

		return parents;
	}
};