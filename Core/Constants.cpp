#include "Constants.h"

namespace c{
	Configuration config;

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

	int frameLimit;
	int resX;
	int resY;

	const void initialize(){
		File dir = File();

		config = Configuration();
		config.load(dir.child("config.txt"));

		baseDir = dir.child(config.stringValue("directories.baseDir"));
		textureDir = baseDir.child(config.stringValue("directories.textureDir"));
		fontDir = baseDir.child(config.stringValue("directories.fontDir"));
		backgroundDir = baseDir.child(config.stringValue("directories.backgroundDir"));
		soundDir = baseDir.child(config.stringValue("directories.soundDir"));
		musicDir = baseDir.child(config.stringValue("directories.musicDir"));
		animationDir = baseDir.child(config.stringValue("directories.animationDir"));
		worldDir = baseDir.child(config.stringValue("directories.worldDir"));

		collisionBoxFile = baseDir.child(config.stringValue("files.collisionBoxFile"));

		autoResolution = config.boolValue("settings.autoResolution");
		fullscreen = config.boolValue("settings.fullscreen");
		verticalSync = config.boolValue("settings.verticalSync");
		frameLimit = config.intValue("settings.frameLimit");
		resX = config.intValue("settings.resX");
		resY = config.intValue("settings.resY");

		masterVolume = config.floatValue("volume.masterVolume");
		musicVolume = config.floatValue("volume.musicVolume");
	}
}