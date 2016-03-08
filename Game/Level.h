#pragma once

#include "Truck.h"
#include "World.h"
#include "Player.h"
#include "Random.h"
#include "Injured.h"
#include "Controller.h"
#include "ResourceBox.h"
#include "JournalManager.h"
#include "PlayerInventory.h"

enum LevelState{
	TRUCKMOVING,
	TRUCKBREAKING,
	PLAYING
};

enum CompleteState{
	IN_GAME,
	TIME_RAN_OUT,
	IN_TRUCK
};

class Level : InputListener{
public:
	Level(Manager* manager, Controller* controller, JournalManager* jmanager);
	~Level();

	void load(File& file);

	void save(File& file);

	void begin();

	void tick();

	bool done();

	void on(MouseButtonEvent& event);

	Injured* nearestInjured(const float& maxDistance);

	ResourceBox* nearestResourceBox(const float& maxDistance);

	bool nearTruck(const float& maxDistance);

	std::vector<MenuItem*>* mis;

	Manager* manager;
	Controller* controller;
	JournalManager* jmanager;

	std::string levelFileName;

	std::string worldFileName;
	World* world;

	sf::Time timer;
	sf::Clock clock;
	Animatable* timerHud;

	std::string musicIntro;
	std::string musicMain;
	unsigned long introId;
	unsigned long mainId;

	bool useTruck;
	Truck* truck;
	bool nearTruck_ = false;
	Vector spawn;
	Vector truckspawn;
	float timeBeforeBreak;

	std::vector<Vector> injuredPositions;

	std::vector<std::string> journals;
	std::set<std::string> usedJournals;

	Player* player = NULL;
	PlayerInventory* playerInventory;

	bool firstFrame = true;
	LevelState state;
	float fadeValue;

	std::vector<Injured*> injured;
	Injured* closest = NULL;

	std::vector<ResourceBox*> resourceBoxes;
	ResourceBox* closestBox = NULL;

	Animatable* journal;
	float dist = 1400.0f;
	float target = dist;
	float actual = dist;
	float d = 5.0f;

	unsigned long listenerId;

	CompleteState completeState = IN_GAME;
};