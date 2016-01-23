#pragma once

#include "SFMLI.h"

class Game{
public:
	Game();
	~Game();

	void run();
private:
	sf::RenderWindow window;
};