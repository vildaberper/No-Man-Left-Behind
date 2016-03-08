#pragma once

#include "Constants.h"
#include "Configuration.h"

namespace gc{
	extern Configuration config;

	extern File levelDir;
	extern File journalDir;

	extern int inventorySize;

	extern std::vector<std::string> levelProgression;

	const void initialize();
}