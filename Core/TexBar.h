#pragma once

#include "SFMLI.h"

class TexBar{
public:
	TexBar();

	~TexBar();

	bool isValid();

	sf::Texture* left = NULL;
	sf::Texture* middle = NULL;
	sf::Texture* right = NULL;
};