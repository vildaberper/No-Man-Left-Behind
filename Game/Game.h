#pragma once

#include "GI.h"
#include "Level.h"
#include "SFMLI.h"
#include "World.h"
#include "Player.h"
#include "Manager.h"
#include "Constants.h"
#include "Controller.h"
#include "MathHelper.h"
#include "ProgressBar.h"

class Game{
public:
	Game();
	~Game();

	void run();
private:
	sf::RenderWindow* window;

	Manager* manager;

	Controller* controller;

	bool managerInitialized = false;
};