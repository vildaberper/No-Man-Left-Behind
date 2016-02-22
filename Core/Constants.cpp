#include "Constants.h"

namespace c{
	Configuration config;

	File dir;
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
	int frameLimit;
	int resX;
	int resY;

	const void initialize(){
		config = Configuration();
		dir = File();
		config.load(dir.child("config.txt"));

		textureDir = dir.child(config.stringValue("directories.textureDir"));
		fontDir = dir.child(config.stringValue("directories.fontDir"));
		backgroundDir = dir.child(config.stringValue("directories.backgroundDir"));
		soundDir = dir.child(config.stringValue("directories.soundDir"));
		musicDir = dir.child(config.stringValue("directories.musicDir"));
		animationDir = dir.child(config.stringValue("directories.animationDir"));
		worldDir = dir.child(config.stringValue("directories.worldDir"));

		collisionBoxFile = dir.child(config.stringValue("files.collisionBoxFile"));

		autoResolution = config.boolValue("settings.autoResolution");
		fullscreen = config.boolValue("settings.fullscreen");
		verticalSync = config.boolValue("settings.verticalSync");
		frameLimit = config.intValue("settings.frameLimit");
		resX = config.intValue("settings.resX");
		resY = config.intValue("settings.resY");

		masterVolume = 1.0f;
		musicVolume = 1.0f;
	}
}