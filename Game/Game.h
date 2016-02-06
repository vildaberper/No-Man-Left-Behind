#pragma once

#include "GI.h"
#include "SFMLI.h"
#include "World.h"
#include "Player.h"
#include "Manager.h"
#include "Constants.h"
#include "Controller.h"
#include "MathHelper.h"

class Game{
public:
	Game();
	~Game();

	void run();
private:
	sf::RenderWindow* window;

	Manager* manager;

	World* world;

	File file;

	Controller* controller;

	Player* player;
};