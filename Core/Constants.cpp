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

	float masterVolume;
	float musicVolume;

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

		masterVolume = 1.0f;
		musicVolume = 1.0f;
	}
}