#pragma once
#include <numeric>
#include <random>
#include "city.hpp"
#include "network.hpp"
#include "configuration.hpp"

using Permutation = std::vector<int>;
const auto argmax = [](std::vector<int> v) { return std::distance(v.begin(), std::max_element(v.begin(), v.end())); };

struct TSPSolver {
	TSPNetwork& net;
	std::vector<Permutation> population;
	std::vector<int> pop_fitnesses;

	Permutation bestSolution;
	int bestFitness;

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

		this->pop_fitnesses = evaluate(population);

		int indmax = argmax(pop_fitnesses);
		this->bestSolution = population[indmax];
		this->bestFitness = pop_fitnesses[indmax];
	}

	Permutation step() {
		std::uniform_real_distribution<> distr(0.0, 1.0f);

		std::vector<Permutation> parents = tournament_selection(population, pop_fitnesses);
		std::vector<Permutation> children(population.size());

		for (size_t i = 1; i < parents.size(); i += 2) {
			Permutation p1 = parents[i-1];
			Permutation p2 = parents[i];
			Permutation child(net.cities.size() - 1);

			// Crossover
			if (distr(g) < conf::P_C) {
				child = pmx_crossover(p1, p2);
			} else {
				if (distr(g) < 0.5) {
					std::copy(p1.begin(), p1.end(), child.begin());
				}
				else {
					std::copy(p2.begin(), p2.end(), child.begin());
				}
			}

			// Mutation
			if (distr(g) < conf::P_M) {
				inversion_mutation(child);
			}

			// Add child to the pool
			children[i / 2] = child;
		}

		// Complete generational replacement
		std::copy(children.begin(), children.end(), population.begin());

		// Return the best individual
		this->pop_fitnesses = evaluate(population);
		int indmax = argmax(pop_fitnesses);

		if (pop_fitnesses[indmax] > bestFitness) {
			this->bestSolution = population[indmax];
			this->bestFitness = pop_fitnesses[indmax];
		}

		return this->bestSolution;
	}

	int fitness(Permutation& p) {
		int res = net.cities[0].distance(net.cities[p[0]]);
		for (size_t i = 1; i < p.size(); i++) {
			res += net.cities[p[i - 1]].distance(net.cities[p[i]]);
		}
		res += net.cities[p.back()].distance(net.cities[0]);

		return -res;
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
		std::uniform_int_distribution<> distr(0, individuals.size() - 1);
		std::vector<Permutation> parents(2 * individuals.size());
		std::vector<int> indices(conf::K, 0);
		std::vector<int> tourFitnesses(conf::K, 0);

		for (size_t i = 0; i < 2 * individuals.size(); i++) {
			for (size_t j = 0; j < conf::K; j++) {
				indices[j] = distr(g);
				tourFitnesses[j] = fitnesses[indices[j]];
			}
			parents[i] = individuals[indices[argmax(tourFitnesses)]];
		}

		return parents;
	}
};