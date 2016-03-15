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
	LEVEL_MENU,
	LEVEL,
	TRANSITION,
	OPTIONS,
	CREDITS,
	COMPLETE,
	CLOSE
};

class Game : public InputListener{
public:
	Game();
	~Game();

	virtual void on(KeyboardEvent& event);

	void run();

	void newGame();
private:
	Menu* mainMenu;
	Menu* pauseMenu;

	std::string* menuCommand;
	unsigned long introId;
	unsigned long mainId;
	bool playingMenuMusic = false;
	bool shouldPlayMenuMusic = true;

	Level* level = NULL;
	ItemStack lastItemInHand;
	ItemStack* lastContent = NULL;
	size_t lastExtraResources = 0;

	int totalCivil = 0;
	int totalSoldier = 0;
	int totalGeneral = 0;
	int savedCivil = 0;
	int savedSoldier = 0;
	int savedGeneral = 0;

	sf::Time transitionTime;
	bool transtionDone = false;

	bool restartLevel = false;

	sf::Time creditTime;
	sf::Time creditLogoTime;
	CoreSprite* nmlbb;
	CoreSprite* morsol;
	CoreSprite* creditbg;
	std::vector<std::string> credits;

	CoreSprite* mainmenu;

	float a;
	float at;
	float as = 1.0f;

	GameState state = MAIN_MENU;

	GameState nextState;

	GameState lastState;

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