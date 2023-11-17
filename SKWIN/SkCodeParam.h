#ifndef _SKCODE_PARAM_H_
#define _SKCODE_PARAM_H_

#include <SkVersionControl.h>

class SkCodeParam
{
public:
	//--- Specific game setup
	static bool bDM1Mode;				// dungeon loaded is a native DM1 dungeon
	static bool bDM1TQMode;				// dungeon runs as TQ DM1 dungeon
	static bool bBWMode;				// experimental Bloodwych mode

	//--- Options
	static bool bUseDM2ExtendedMode;
	static bool bUsePowerDebug;
	static bool bUseFixedMode;
	static bool bUseIngameDebug;
	static bool bUseSuperMode;
	static bool bUseSuperInfoEye;

	static bool bMIDIMusicEnabled;
	static bool bUseExtendedSound;
	static bool bUseMultilanguageExtended;

	static bool bUseVaryingPlaybackFrequency;
	static bool bUsePlayerWalkSound;

	static bool bFullLight;

	static bool bForceSaveGameReadOK;	// to allow return from gameload

	//--- Specific parameters, for testing/debugging
	static bool bDisableFogEffect;
	static bool bWeakDoors;
	static bool bWalkOverPits;
	static bool bUseCustomSpells;	// reads spells from GDAT
	static bool bUnlimitedCharges;
	static bool bUseScrollIDType;

	static bool bForceOrnateSound;

	static bool bDebugInfoMapInit;

	//--- Very specific for debugging
	static bool bDebugNoImageDecodingAssert;
	static bool bDebugBypassNullPointers;

public:
	SkCodeParam();
};

#endif // _SKCODE_PARAM_H_