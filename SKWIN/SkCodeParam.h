#ifndef _SKCODE_PARAM_H_
#define _SKCODE_PARAM_H_

#include <SkVersionControl.h>

class SkCodeParam
{
public:
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

	//--- Specific parameters, for testing/debugging
	static bool bDisableFogEffect;
	static bool bWeakDoors;
	static bool bWalkOverPits;
	static bool bUseCustomSpells;	// reads spells from GDAT
	static bool bUnlimitedCharges;
	static bool bUseScrollIDType;

	static bool bForceOrnateSound;

	//--- Very specific for debugging
	static bool bDebugNoImageDecodingAssert;
	static bool bDebugBypassNullPointers;

public:
	SkCodeParam();
};

#endif // _SKCODE_PARAM_H_