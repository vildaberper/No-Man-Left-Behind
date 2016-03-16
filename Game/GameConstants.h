#pragma once

#include "Constants.h"
#include "Configuration.h"

namespace gc{
	extern Configuration config;

	extern File levelDir;
	extern File journalDir;

	extern File handBookFile;

	extern int inventorySize;

	extern float timerRateNotSeen;
	extern float survive0Rate;
	extern float survive1Rate;
	extern float survive2Rate;
	extern float survive3Rate;
	extern float survive4Rate;

	extern int resourcesFromGenerals;

	extern float criticalTimer;

	extern std::vector<std::string> levelProgression;

	const void initialize();
}