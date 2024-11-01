#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "city.hpp"

struct TSPNetwork {
    std::vector<City>& cities;
    City& start;

    TSPNetwork(std::vector<City>& cities, City& start)
        : cities{ cities }
        , start{ start }
    {}
};