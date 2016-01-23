#pragma once

#include "SFMLI.h"
#include "File.h"

class Editor{
public:
	Editor();
	~Editor();

	void run();
private:
	sf::RenderWindow window;
};