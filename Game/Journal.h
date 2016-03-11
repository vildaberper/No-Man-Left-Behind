#pragma once

#include <vector>
#include <string>

#include "Item.h"
#include "SFMLI.h"

class Journal{
public:
	Journal();
	~Journal();

	sf::Time deathTimer;

	std::vector<std::string> injured;

	std::vector<std::string> lines;

	std::vector<Resource> requirements;
};