#pragma once

#include "SFMLI.h"

class TexBar{
public:
	TexBar();

	TexBar(sf::Texture* left, sf::Texture* middle, sf::Texture* right);

	~TexBar();

	bool isValid();

	sf::Texture* left = nullptr;
	sf::Texture* middle = nullptr;
	sf::Texture* right = nullptr;
};