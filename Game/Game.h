#pragma once

#include "GI.h"
#include "Level.h"
#include "SFMLI.h"
#include "World.h"
#include "Player.h"
#include "Manager.h"
#include "Handbook.h"
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
	CoreSprite* mainmenu;
	float a;
	float at;

	GameState state = MAIN_MENU;

	size_t currentLevel = 0;

	PlayerInventory* playerInventory;

	Handbook* handBook;

	unsigned long inputListenerId;

	sf::RenderWindow* window;

	Manager* manager;

	JournalManager* jmanager;

	Controller* controller;

	bool managerInitialized = false;
};