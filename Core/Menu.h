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

	MenuType type = VERTICAL;

	std::vector<MenuItem*> items;
};