#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <algorithm>
#include "city.hpp"

struct TSPNetwork {
    std::vector<City> cities;

    TSPNetwork(std::vector<City>& allCities, const int NUM_NODES) {
        // Shuffle the city list to obtain random choice of cities
        std::random_device rd;
        std::mt19937 g(rd());

        std::shuffle(allCities.begin(), allCities.end(), g);
        this->cities = std::vector<City>(allCities.begin(), allCities.begin() + NUM_NODES);

        // Pick the random starting point
        std::uniform_int_distribution<> unif(0, cities.size() - 1);
        int start = unif(g);

        // Swap for simplicity
        if (start != 0) {
            std::swap(this->cities[0], this->cities[start]);
        }
    }
};