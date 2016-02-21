#pragma once

#include "Truck.h"
#include "World.h"
#include "Player.h"
#include "Injured.h"
#include "Controller.h"

enum LevelState{
	TRUCKMOVING,
	TRUCKBREAKING,
	PLAYING
};

class Level{
public:
	Level(Manager* manager, Controller* controller);
	~Level();

	void begin();

	void tick();

	bool done();

	Manager* manager;
	Controller* controller;

	std::string levelFileName;

	std::string worldFileName;
	World* world;

	Truck* truck;
	Vector spawn;
	float timeBeforeBreak;

	Player* player = NULL;

	bool firstFrame = true;
	LevelState state;
	float fadeValue;

	std::vector<Injured*> injured;
};