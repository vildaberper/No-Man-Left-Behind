#include "GameConstants.h"

namespace gc{
	Configuration config;

	File levelDir;
	File journalDir;
	File transitionDir;

	File handBookFile;
	File restartFile;

	bool developerMode;

	int inventorySize;
	int stackSizePennicillin;
	int stackSizeForceps;
	int stackSizeAlcohol;
	int stackSizeMorphine;
	int stackSizeSutureKit;
	int stackSizeScalpel;
	int stackSizeGauze;

	float timerRateNotSeen;

	float survive0Rate;
	float survive1Rate;
	float survive2Rate;
	float survive3Rate;
	float survive4Rate;

	float stressCivil;
	float stressSoldier;
	float stressGeneral;

	float stressForA;
	float stressForB;

	float playerSpeedLower;
	float playerSpeedUpper;

	int resourcesFromGenerals;
	float timeBonusFromSoldiers;

	float criticalTimer;

	float armyMarchStart;
	float armyScreamLength;

	float armyMarchVolume;
	float armyScreamVolume;

	std::vector<std::string> levelProgression;

	const void initialize(){
		sf::Clock cl;

		File dir = File();

		config = Configuration();
		config.load(dir.child("gameconfig.txt"));

		levelDir = c::baseDir.child(config.stringValue("directories.levelDir"));
		journalDir = c::baseDir.child(config.stringValue("directories.journalDir"));
		transitionDir = c::baseDir.child(config.stringValue("directories.transitionDir"));

		handBookFile = c::baseDir.child(config.stringValue("files.handBookFile"));
		restartFile = c::baseDir.child(config.stringValue("files.restartFile"));

		developerMode = config.boolValue("general.developerMode");

		inventorySize = config.intValue("general.inventorySize");
		stackSizePennicillin = config.intValue("general.stackSizePennicillin");
		stackSizeForceps = config.intValue("general.stackSizeForceps");
		stackSizeAlcohol = config.intValue("general.stackSizeAlcohol");
		stackSizeMorphine = config.intValue("general.stackSizeMorphine");
		stackSizeSutureKit = config.intValue("general.stackSizeSutureKit");
		stackSizeScalpel = config.intValue("general.stackSizeScalpel");
		stackSizeGauze = config.intValue("general.stackSizeGauze");

		timerRateNotSeen = config.floatValue("general.timerRateNotSeen");
		survive0Rate = config.floatValue("general.survive0Rate");
		survive1Rate = config.floatValue("general.survive1Rate");
		survive2Rate = config.floatValue("general.survive2Rate");
		survive3Rate = config.floatValue("general.survive3Rate");
		survive4Rate = config.floatValue("general.survive4Rate");
		stressCivil = config.floatValue("general.stressCivil");
		stressSoldier = config.floatValue("general.stressSoldier");
		stressGeneral = config.floatValue("general.stressGeneral");
		stressForA = config.floatValue("general.stressForA");
		stressForB = config.floatValue("general.stressForB");
		playerSpeedLower = config.floatValue("general.playerSpeedLower");
		playerSpeedUpper = config.floatValue("general.playerSpeedUpper");
		resourcesFromGenerals = config.intValue("general.resourcesFromGenerals");
		timeBonusFromSoldiers = config.floatValue("general.timeBonusFromSoldiers");
		criticalTimer = config.floatValue("general.criticalTimer");

		armyMarchStart = config.floatValue("general.armyMarchStart");
		armyScreamLength = config.floatValue("general.armyScreamLength");

		armyMarchVolume = config.floatValue("sound.armyMarchVolume");
		armyScreamVolume = config.floatValue("sound.armyScreamVolume");

		levelProgression = config.stringVector("levels.progression");

		logger::timing("Game constants initialized in " + std::to_string(cl.getElapsedTime().asSeconds()) + " seconds");
	}
}