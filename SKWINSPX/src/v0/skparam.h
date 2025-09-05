/*
 * Filename:    skparam.cpp
 * Author:      Sphenx
 * Created:     201x
 * Description: SKParam controls ingame options that can be activated/deactivated
 *              for gameplay, debug purpose, and engine compatibility-mode purpose.
 *
 */

#ifndef _SKCODE_PARAM_H_
#define _SKCODE_PARAM_H_

#include <sktypes.h>

class SkCodeParam
{
public:

	static bool bCompilation64bits;

	// mode
	static bool bRenderingEngineSDL;				// SDL renderer - Windows / Linux
	static bool bRenderingEngineDOS;				// DOS renderer - DOS only
	static bool bRenderingEngineMFC;				// MFC renderer - Windows only
	static bool bEngineNoDisplay;

	static UINT bOptionNewGame;
	static int bOptionResumeSaveGame;	// not UINT else there is trouble to get the resume game screen

	//

	static bool bUseBigEnd;				// tell to read data files in big-end mode, specially for Amiga and Sega-CD
	static bool bNoAudio;				// some debug flag to disable audio
	static bool bNoMusic;
	static bool bChampionImmune;		// if activated, champions do not take any damage

	static bool bUseAudioSDL;

	//--- Specific game setup
	static bool bDM1Mode;				// dungeon loaded is a native DM1 dungeon
	static bool bGFXFixModeDM1;
	static bool bDM1ReadSavegame;		// read savegame as a native DM1 savegame (DM1 retrocompatibility)
	static bool bDM1TQMode;				// dungeon runs as TQ DM1 dungeon

	static bool bBWMode;				// experimental Bloodwych mode
	static bool bEOBMode;				// experimental Eye of the Beholder mode
	static bool bGameModeExtended;		// experimental

	static bool bRandomInitChampion;	// experimental Dungeon generation
	static bool bAllowFreeObjectMove;

	static bool bDM2V5Mode;

	static bool bAutoDefaultMaxLight;

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
	static bool bEnableDoubleStepMove;
	static bool bDisableFogEffect;
	static bool bWeakDoors;
	static bool bWalkOverPits;
	static bool bUseCustomSpells;	// reads spells from GDAT
	static bool bUnlimitedCharges;
	static bool bDebugInfiniteSpells;
	static bool bUseScrollIDType;
	static bool bNoTimer;			// allow to flush/clean active timers (or auto-cycling traps)

	static bool bDebugGFXNoWalls;
	static bool bDebugGFXNoInterWalls;
	static bool bDebugGFXNoDoors;
	static bool bDebugGFXNoCreatures;
	static bool bDebugNoCreatures;

	static bool bForceOrnateSound;

	static bool bDebugInfoMapInit;

	static float fDebugRainSpeed;

	static short int iTickSpeedFactor;	// replaces skwin.spfact
	static UINT iVideoScale;
	static float fVideoYScale;

	//--- Very specific for debugging
	static bool bDebugNoImageDecodingAssert;
	static bool bDebugBypassNullPointers;

public:
	SkCodeParam();

	//static void LogParams();
};

#endif // _SKCODE_PARAM_H_