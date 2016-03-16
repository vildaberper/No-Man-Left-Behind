#include "GameConstants.h"

namespace gc{
	Configuration config;

	File levelDir;
	File journalDir;

	File handBookFile;

	int inventorySize;

	float timerRateNotSeen;

	float survive0Rate;
	float survive1Rate;
	float survive2Rate;
	float survive3Rate;
	float survive4Rate;

	int resourcesFromGenerals;

	float criticalTimer;

	std::vector<std::string> levelProgression;

	const void initialize(){
		sf::Clock cl;

		File dir = File();

		config = Configuration();
		config.load(dir.child("gameconfig.txt"));

		levelDir = c::baseDir.child(config.stringValue("directories.levelDir"));
		journalDir = c::baseDir.child(config.stringValue("directories.journalDir"));

		handBookFile = c::baseDir.child(config.stringValue("files.handBookFile"));

		inventorySize = config.intValue("general.inventorySize");
		timerRateNotSeen = config.floatValue("general.timerRateNotSeen");
		survive0Rate = config.floatValue("general.survive0Rate");
		survive1Rate = config.floatValue("general.survive1Rate");
		survive2Rate = config.floatValue("general.survive2Rate");
		survive3Rate = config.floatValue("general.survive3Rate");
		survive4Rate = config.floatValue("general.survive4Rate");
		resourcesFromGenerals = config.intValue("general.resourcesFromGenerals");
		criticalTimer = config.floatValue("general.criticalTimer");

		levelProgression = config.stringVector("levels.progression");

		logger::timing("Game constants initialized in " + std::to_string(cl.getElapsedTime().asSeconds()) + " seconds");
	}
}