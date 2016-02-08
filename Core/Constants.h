#pragma once

#include "Configuration.h"

namespace c{
	extern Configuration config;

	extern File dir;
	extern File textureDir;
	extern File fontDir;
	extern File backgroundDir;
	extern File soundDir;
	extern File musicDir;
	extern File animationDir;

	extern float masterVolume;
	extern float musicVolume;

	const void initialize();
}