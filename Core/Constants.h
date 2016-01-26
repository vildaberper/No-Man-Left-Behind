#include "Configuration.h"

namespace c{
	static Configuration config = Configuration();

	static File dir = File();
	static File textureDir;

	static void initialize(){
		config.load(dir.child("config.txt"));
		textureDir = dir.child(config.stringValue("directories.textureDir"));
	}
}