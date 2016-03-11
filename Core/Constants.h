#pragma once

#include "Configuration.h"

namespace c{
	static const std::string WINDOW_TITLE = "No Man Left Behind";

	extern Configuration config;

	extern File baseDir;
	extern File textureDir;
	extern File fontDir;
	extern File backgroundDir;
	extern File soundDir;
	extern File musicDir;
	extern File animationDir;
	extern File worldDir;

	extern File collisionBoxFile;

	extern bool autoResolution;
	extern bool fullscreen;
	extern bool verticalSync;
	extern int frameLimit;
	extern int resX;
	extern int resY;
	extern bool showLog;

	extern float masterVolume;
	extern float musicVolume;

	extern File menuFont;
	extern File textFont;

	const void initialize();
}