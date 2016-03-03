#pragma once

#include "Constants.h"
#include "Configuration.h"

namespace gc{
	extern Configuration config;

	extern File levelDir;

	extern std::vector<std::string> levelProgression;

	const void initialize();
}