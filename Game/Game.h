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
#include "GameConstants.h"
#include "JournalManager.h"

enum GameState{
	MAIN_MENU,
	LEVEL,
	COMPLETE
};

class Game : public InputListener{
public:
	Game();
	~Game();

	virtual void on(KeyboardEvent& event);

	void run();
private:
	GameState state = MAIN_MENU;

	size_t currentLevel = 0;

	unsigned long inputListenerId;

	sf::RenderWindow* window;

	Manager* manager;

	JournalManager* jmanager;

	Controller* controller;

	bool managerInitialized = false;
};