#pragma once

#include <vector>
#include <string>

#include "Item.h"

class Journal{
public:
	Journal();
	~Journal();

	std::vector<std::string> injured;

	std::vector<std::string> lines;

	std::vector<Resource> requirements;
};