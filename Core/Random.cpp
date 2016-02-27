#include "Random.h"

namespace random{
	void initialize(unsigned int seed){
		srand(seed);
	}

	float random(){
		return float(rand()) / float(RAND_MAX);
	}

	int random(const int& upper, const int& lower){
		return lower + rand() % (upper + 1 - lower);
	}
}