#include "Random.h"

namespace random{
	void initialize(unsigned int seed){
		srand(seed);
	}

	int randomInt(){
		return rand();
	}
	int randomInt(const int& upper, const int& lower){
		if(upper == lower){
			return upper;
		}
		return lower + randomInt() % (upper + 1 - lower);
	}


	float randomFloat(){
		return float(randomInt()) / float(RAND_MAX);
	}
	float randomFloat(const float& upper, const float& lower){
		return lower + randomFloat() * (upper - lower);
	}
}