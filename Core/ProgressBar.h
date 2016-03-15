#pragma once

#include "SFMLI.h"
#include "TexBar.h"
#include "Vector.h"

class ProgressBar{
public:
	ProgressBar();
	~ProgressBar();

	Vector position;
	Vector size;

	TexBar* background = NULL;
	TexBar* progressbar = NULL;

	sf::Color bgColor;
	sf::Color pbColor;
	sf::Color oColor;
	bool customColors = false;

	float progress = 0.0f;
};