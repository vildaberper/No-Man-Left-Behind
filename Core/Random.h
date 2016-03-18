#pragma once

#include <cstdlib>
#include <time.h>

namespace random{
	void initialize(unsigned int seed = unsigned int(time(0)));

	int randomInt();
	int randomInt(const int& upper, const int& lower = 0);

	float randomFloat();
	float randomFloat(const float& upper, const float& lower = 0);
}