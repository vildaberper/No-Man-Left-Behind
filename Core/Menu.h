#pragma once

#include <vector>

#include "Vector.h"
#include "MenuItem.h"

enum MenuType{
	VERTICAL,
	HORIZONTAL
};

class Menu{
public:
	Menu();
	~Menu();

	Vector position;
	Vector size;

	bool hidden = true;

	bool drawElementBackgrounds = true;

	float topOffset = 0.0f;
	float bottomOffset = 0.0f;
	float leftOffset = 0.0f;
	float rightOffset = 0.0f;

	TexBar* background = nullptr;

	MenuType type = VERTICAL;

	std::vector<MenuItem*> items;
};