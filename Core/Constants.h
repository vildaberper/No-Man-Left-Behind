#pragma once

#include "Configuration.h"

namespace c{
	static const std::string WINDOW_TITLE = "No Man Left Behind";

	extern Configuration config;

	extern File dir;
	extern File textureDir;
	extern File fontDir;
	extern File backgroundDir;
	extern File soundDir;
	extern File musicDir;
	extern File animationDir;
	extern File worldDir;

	extern File collisionBoxFile;

	extern bool fullscreen;
	extern bool verticalSync;
	extern int frameLimit;
	extern int resX;
	extern int resY;

	extern float masterVolume;
	extern float musicVolume;

	const void initialize();
}