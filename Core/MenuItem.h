#pragma once

#include <string>

#include "SFMLI.h"
#include "TexBar.h"
#include "CoreSprite.h"

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
	CoreSprite* sprite;
	MenuItemType type = TEXT;
	bool alternativeText = false;
	Menu* toggle = NULL;
	bool closeOnClick;
	bool highlight = false;
	bool darkenOnMouseOver = false;

	TexBar* background = NULL;

	std::string selectedPrefix;
	std::string* selectedString = NULL;
};