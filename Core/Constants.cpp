#include "Constants.h"

namespace c{
	Configuration config;

	File dir;
	File textureDir;
	File fontDir;

	const void initialize(){
		config = Configuration();
		dir = File();
		config.load(dir.child("config.txt"));
		textureDir = dir.child(config.stringValue("directories.textureDir"));
		fontDir = dir.child(config.stringValue("directories.fontDir"));
	}
}