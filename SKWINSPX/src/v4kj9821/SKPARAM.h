#ifndef _SKCODE_PARAM_H_
#define _SKCODE_PARAM_H_

#include <SkVer.h>

class SkCodeParam
{
public:

	static bool bUseBigEnd;				// tell to read data files in big-end mode, specially for Amiga and Sega-CD
	static bool bNoAudio;				// some debug flag to disable audio

	//--- Specific game setup
	static bool bDM1Mode;				// dungeon loaded is a native DM1 dungeon
	static bool bDM1ReadSavegame;		// read savegame as a native DM1 savegame (DM1 retrocompatibility)
	static bool bDM1TQMode;				// dungeon runs as TQ DM1 dungeon
	static bool bBWMode;				// experimental Bloodwych mode
	static bool bEOBMode;				// experimental Eye of the Beholder mode
	static bool bRandomInitChampion;	// experimental Dungeon generation
	static bool bAllowFreeObjectMove;

	static bool bDM2V5Mode;

	//--- Options
	static bool bUseDM2ExtendedMode;
	static bool bUsePowerDebug;
	static bool bUseFixedMode;
	static bool bUseIngameDebug;
	static bool bUseSuperMode;
	static bool bUseSuperInfoEye;
	static bool bShowLuck;

	static bool bMIDIMusicEnabled;
	static bool bWAVMusicEnabled;
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
	static bool bInfiniteSpells;
	static bool bUseScrollIDType;
	static bool bNoTimer;			// allow to flush/clean active timers (or auto-cycling traps)

	static bool bForceOrnateSound;

	static bool bDebugInfoMapInit;

	//--- Very specific for debugging
	static bool bDebugNoImageDecodingAssert;
	static bool bDebugBypassNullPointers;

public:
	SkCodeParam();
};

#endif // _SKCODE_PARAM_H_