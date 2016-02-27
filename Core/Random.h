#pragma once

#include <cstdlib>
#include <time.h>

namespace random{
	void initialize(unsigned int seed = unsigned int(time(0)));

	float random();

	int random(const int& upper, const int& lower = 0);
}