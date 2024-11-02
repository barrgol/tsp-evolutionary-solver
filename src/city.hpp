#pragma once
#include <string>
#include <sstream>
#include <numbers>
#include <cmath>

struct City
{
	std::string country;
	std::string name;
	float latitude;
	float longitude;

	City() {};

	City(std::string country, std::string name, float latitude, float longitude)
		: country{ country }
		, name{ name }
		, latitude{ latitude }
		, longitude{ longitude }
	{}

	static City parse(std::string str) {
		// Helper containers
		std::stringstream ss(str);
		std::string cell;

		std::string country, name;
		float latitude, longitude;

		// Country & City name
		std::getline(ss, cell, ',');
		country = cell;

		std::getline(ss, cell, ',');
		name = cell;

		// Latitude & Longitude
		std::getline(ss, cell, ',');
		latitude = std::stof(cell);

		std::getline(ss, cell, ',');
		longitude = std::stof(cell);
		
		return City(country, name, latitude, longitude);
	}

	float distance(City& c) {
		// For converting degrees to radians
		auto rad = [](float deg) { return deg * (std::numbers::pi_v<float> / 180.0f);  };

		// Haversine formula
		float lat_A = rad(this->latitude);
		float lat_B = rad(c.latitude);
		float lon_A = rad(this->longitude);
		float lon_B = rad(c.longitude);

		float res = 3440.1 * std::acos(
			(std::sin(lat_A) * std::sin(lat_B)) +
			std::cos(lat_A) * std::cos(lat_B) * std::cos(lon_A - lon_B)
		);

		// Switch from NM to kilometers
		return std::round(1.852 * res);
	}
};