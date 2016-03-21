#pragma once

#include "Truck.h"
#include "World.h"
#include "Player.h"
#include "Random.h"
#include "Injured.h"
#include "Handbook.h"
#include "CursorSet.h"
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

extern float stepDistance;

typedef std::pair<drawable::Drawable*, ItemStack> ResourcePair;

class Level : InputListener{
public:
	Level(Manager* manager, Controller* controller, JournalManager* jmanager, CursorSet* cursorSet);
	~Level();

	void stopMusic();

	void load(File& file);

	void save(File& file);

	void begin(const float& stress, const float& timeBonus);

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
	sf::Time time;
	Animatable* timerHud;

	bool canControl;
	bool canUseResource;

	bool usingResource = false;
	sf::Time resourceUsedTime;
	sf::Time resourceUsedTimer;
	ProgressBar* resourceUseBar;
	Injured* resourceUseInjured = nullptr;

	std::string musicIntro;
	std::string musicMain;
	unsigned long introId;
	unsigned long mainId;

	float warSoundLower;
	float warSoundUpper;
	float warSoundVolume;
	sf::Time nextWarSound;

	unsigned long armyMarchId = 0;
	unsigned long armyScreamId = 0;

	float stress = 1.0f;

	bool useTruck = true;
	Truck* truck;
	bool nearTruck_ = false;
	Vector spawn;
	Vector truckspawn;
	float timeBeforeBreak;

	std::vector<Vector> injuredPositions;

	std::vector<std::string> journals;
	std::set<std::string> usedJournals;

	Player* player = nullptr;
	PlayerInventory* playerInventory;

	Handbook* handBook;

	bool firstFrame = true;
	LevelState state;
	float fadeValue;

	std::vector<Injured*> injured;
	Injured* closest = nullptr;

	std::vector<ResourceBox*> resourceBoxes;
	ResourceBox* closestBox = nullptr;

	std::vector<ResourcePair> resources;

	Animatable* journal;
	float dist = 1400.0f;
	float target = dist;
	float actual = dist;
	float d = 5.0f;
	CoreSprite* skull;

	size_t extraResources = 0;

	int totalCivil = 0;
	int totalSoldier = 0;
	int totalGeneral = 0;
	int savedCivil = 0;
	int savedSoldier = 0;
	int savedGeneral = 0;

	unsigned long listenerId;

	CompleteState completeState = IN_GAME;

	CursorSet* cursorSet;

	bool hasUsedResource = false;

	bool wasUsingController;

	float relativeOffset;
};