#pragma once

#include "Truck.h"
#include "World.h"
#include "Player.h"
#include "Random.h"
#include "Injured.h"
#include "Controller.h"
#include "JournalManager.h"
#include "PlayerInventory.h"

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

	std::vector<Vector> injuredPositions;

	std::vector<std::string> journals;

	Player* player = NULL;
	PlayerInventory* playerInventory;

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