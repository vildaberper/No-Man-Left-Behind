#pragma once

#include "Configuration.h"

namespace c{
	extern Configuration config;

	extern File dir;
	extern File textureDir;
	extern File fontDir;
	extern File backgroundDir;
	extern File soundDir;

	const void initialize();
}