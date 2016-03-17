#include "Constants.h"

namespace c{
	Configuration* config = NULL;

	File baseDir;
	File textureDir;
	File fontDir;
	File backgroundDir;
	File soundDir;
	File musicDir;
	File animationDir;
	File worldDir;

	File collisionBoxFile;

	bool autoResolution;
	bool fullscreen;
	bool verticalSync;

	float masterVolume;
	float musicVolume;

	File menuFont;
	File textFont;

	int frameLimit;
	int resX;
	int resY;
	bool showLog;

	const void initialize(){
		sf::Clock cl;

		if(config != NULL){
			delete config;
		}

		File dir = File();

		config = new Configuration();
		config->load(dir.child("config.txt"));

		std::string baseDirPath = config->stringValue("directories.baseDir");
		if(baseDirPath.find_first_of(':') == std::string::npos){
			baseDir = dir.child(baseDirPath);
		}
		else{
			baseDir = File(baseDirPath);
		}
		textureDir = baseDir.child(config->stringValue("directories.textureDir"));
		fontDir = baseDir.child(config->stringValue("directories.fontDir"));
		backgroundDir = baseDir.child(config->stringValue("directories.backgroundDir"));
		soundDir = baseDir.child(config->stringValue("directories.soundDir"));
		musicDir = baseDir.child(config->stringValue("directories.musicDir"));
		animationDir = baseDir.child(config->stringValue("directories.animationDir"));
		worldDir = baseDir.child(config->stringValue("directories.worldDir"));

		collisionBoxFile = baseDir.child(config->stringValue("files.collisionBoxFile"));

		autoResolution = config->boolValue("settings.autoResolution");
		fullscreen = config->boolValue("settings.fullscreen");
		verticalSync = config->boolValue("settings.verticalSync");
		frameLimit = config->intValue("settings.frameLimit");
		resX = config->intValue("settings.resX");
		resY = config->intValue("settings.resY");
		showLog = config->boolValue("settings.showLog");

		masterVolume = config->floatValue("volume.masterVolume");
		musicVolume = config->floatValue("volume.musicVolume");

		menuFont = fontDir.child(config->stringValue("fonts.menuFont"));
		textFont = fontDir.child(config->stringValue("fonts.textFont"));

		logger::timing("Constants initialized in " + std::to_string(cl.getElapsedTime().asSeconds()) + " seconds");
	}

	void save(){
		sf::Clock cl;

		c::config->set("settings.fullscreen", c::fullscreen);
		c::config->set("settings.verticalSync", c::verticalSync);

		config->save(File().child("config.txt"));

		logger::timing("Constants saved in " + std::to_string(cl.getElapsedTime().asSeconds()) + " seconds");
	}
}