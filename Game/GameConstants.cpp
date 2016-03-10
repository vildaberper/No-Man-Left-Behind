#include "GameConstants.h"

namespace gc{
	Configuration config;

	File levelDir;
	File journalDir;

	File handBookFile;

	int inventorySize;

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

		levelProgression = config.stringVector("levels.progression");

		logger::timing("Game constants initialized in " + std::to_string(cl.getElapsedTime().asSeconds()) + " seconds");
	}
}