#pragma once

#include "Truck.h"
#include "World.h"
#include "Player.h"
#include "Injured.h"
#include "Controller.h"
#include "JournalManager.h"

enum LevelState{
	TRUCKMOVING,
	TRUCKBREAKING,
	PLAYING
};

class Level{
public:
	Level(Manager* manager, Controller* controller, JournalManager* jmanager);
	~Level();

	void load(File& file);

	void save(File& file);

	void begin();

	void tick();

	bool done();

	Injured* nearestInjured(const float& maxDistance);

	void updateInventoryMenu();
	std::vector<MenuItem*>* mis;

	Manager* manager;
	Controller* controller;
	JournalManager* jmanager;

	std::string levelFileName;

	std::string worldFileName;
	World* world;

	std::string musicIntro;
	std::string musicMain;
	unsigned long introId;
	unsigned long mainId;

	bool useTruck;
	Truck* truck;
	Vector spawn;
	float timeBeforeBreak;

	Player* player = NULL;
	int selectedSlot = 0;

	Menu* invM;
	bool firstInventoryFrame = true;
	bool firstFrame = true;
	LevelState state;
	float fadeValue;

	std::vector<Injured*> injured;
	Injured* closest = NULL;

	Animatable* journal;
	float dist = 1250.0f;
	float target = dist;
	float actual = dist;
	float d = 5.0f;
};