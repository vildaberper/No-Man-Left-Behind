#pragma once

#include <string>

#include "SFMLI.h"

class Menu;

enum MenuItemType{
	TEXT,
	TEXTURE,
	INPUT_STRING,
	INPUT_FLOAT,
	INPUT_INT
};

class MenuItem{
public:
	MenuItem();
	~MenuItem();
	std::string title;
	std::string value;
	sf::Sprite* sprite;
	MenuItemType type = TEXT;
	Menu* toggle = NULL;
	bool closeOnClick;
	bool highlight = false;

	std::string selectedPrefix;
	std::string* selectedString = NULL;
};