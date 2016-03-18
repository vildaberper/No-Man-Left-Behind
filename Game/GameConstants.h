#pragma once

#include "Constants.h"
#include "Configuration.h"

namespace gc{
	extern Configuration config;

	extern File levelDir;
	extern File journalDir;
	extern File transitionDir;

	extern File handBookFile;
	extern File restartFile;

	extern bool developerMode;

	extern int inventorySize;
	extern int stackSizePennicillin;
	extern int stackSizeForceps;
	extern int stackSizeAlcohol;
	extern int stackSizeMorphine;
	extern int stackSizeSutureKit;
	extern int stackSizeScalpel;
	extern int stackSizeGauze;

	extern float timerRateNotSeen;
	extern float survive0Rate;
	extern float survive1Rate;
	extern float survive2Rate;
	extern float survive3Rate;
	extern float survive4Rate;

	extern float stressCivil;
	extern float stressSoldier;
	extern float stressGeneral;

	extern float stressForA;
	extern float stressForB;

	extern float playerSpeedLower;
	extern float playerSpeedUpper;

	extern int resourcesFromGenerals;
	extern float timeBonusFromSoldiers;

	extern float criticalTimer;

	extern float armyMarchStart;
	extern float armyScreamLength;

	extern float hearingDistance;

	extern float injuredVoiceLower;
	extern float injuredVoiceUpper;

	extern float armyMarchVolume;
	extern float armyScreamVolume;
	extern float injuredVoiceVolume;
	extern float bagVolume;

	extern std::vector<std::string> levelProgression;

	const void initialize();
}