#pragma once

#include "Vector.h"
#include "SFMLI.h"

class ProgressBar{
public:
	ProgressBar();
	~ProgressBar();

	Vector position;
	Vector size;

	sf::Sprite* bleft;
	sf::Sprite* bright;
	sf::Sprite* bmiddle;

	float progress;
};