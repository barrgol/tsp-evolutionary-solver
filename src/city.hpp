#pragma once
#include <string>
#include <sstream>

struct City
{
	std::string country;
	std::string name;
	float latitude;
	float longitude;

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
};