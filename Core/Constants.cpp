#include "Constants.h"

namespace c{
	Configuration config;

	File dir;
	File textureDir;

	const void initialize(){
		config = Configuration();
		dir = File();
		config.load(dir.child("config.txt"));
		textureDir = dir.child(config.stringValue("directories.textureDir"));
	}
}