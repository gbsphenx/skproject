#include <skparam.h>
//#include <stdio.h>

#ifdef __LINUX__
bool SkCodeParam::bCompilation64bits = true;
#else
bool SkCodeParam::bCompilation64bits = false;
#endif


bool SkCodeParam::bRenderingEngineSDL = false;
#ifdef __DJGPP__
	bool SkCodeParam::bRenderingEngineDOS = true;
#else
	bool SkCodeParam::bRenderingEngineDOS = false;
#endif // __DJGPP__

bool SkCodeParam::bRenderingEngineMFC = false;

bool SkCodeParam::bEngineNoDisplay = false;

UINT SkCodeParam::bOptionNewGame = 0;
int SkCodeParam::bOptionResumeSaveGame = -1;

bool SkCodeParam::bUseBigEnd = false;	// Default is little-end

bool SkCodeParam::bNoAudio = false;
bool SkCodeParam::bNoMusic = false;
bool SkCodeParam::bChampionImmune = true;

bool SkCodeParam::bUseAudioSDL = false;

bool SkCodeParam::bDM1Mode = false;
bool SkCodeParam::bGFXFixModeDM1 = false;	// adjust gfx positions for DM1 mode
bool SkCodeParam::bDM1ReadSavegame = false;
bool SkCodeParam::bDM1TQMode = false;	// Try to get the more TQ compatibility mode, specially in dungeon, many visible actuators do sound whereas the sound flag is not set.

bool SkCodeParam::bDM2V5Mode = false;	// activate when detecting PC-DOS V5 graphics.dat

bool SkCodeParam::bAutoDefaultMaxLight = false;	// turn this to true for DM1-like dungeon (or rather not DM2) instead to get a value from GDAT.
// If not set, the default "full" light will always be one step below normal light

bool SkCodeParam::bTQMode = false;
bool SkCodeParam::bBWMode = false;
bool SkCodeParam::bEOBMode = false;
bool SkCodeParam::bGameModeExtended = false;


bool SkCodeParam::bRandomInitChampion = false;
bool SkCodeParam::bAllowFreeObjectMove = false;	// activated with custom scripted dungeon, to allow any object to be put into the dungeon from commands

bool SkCodeParam::bUseDM2ExtendedMode = true;
bool SkCodeParam::bUsePowerDebug = true;
bool SkCodeParam::bUseFixedMode = true;
bool SkCodeParam::bUseIngameDebug = false;
bool SkCodeParam::bDebugInfoMapInit = false;

bool SkCodeParam::bUseSuperInfoEye = false;
bool SkCodeParam::bShowLuck = false;

bool SkCodeParam::bEnableDoubleStepMove = true;
bool SkCodeParam::bDisableFogEffect = false;
bool SkCodeParam::bWeakDoors = true;
bool SkCodeParam::bWalkOverPits = false;
bool SkCodeParam::bUseCustomSpells = false;	// Start FALSE. This is automatically put to TRUE if graphics.dat contains custom spell definitions
bool SkCodeParam::bUnlimitedCharges = false;
bool SkCodeParam::bNoTimer = false;

bool SkCodeParam::bDebugInfiniteSpells = false;
bool SkCodeParam::bDebugGFXNoWalls = false;			// don't display solid walls
bool SkCodeParam::bDebugGFXNoInterWalls = false;	// don't display interwalls
bool SkCodeParam::bDebugGFXNoDoors = false;		// don't display doors
bool SkCodeParam::bDebugGFXNoCreatures = false;	// don't display creature
bool SkCodeParam::bDebugNoCreatures = false;		// drop all creatures -- don't generate creatures
bool SkCodeParam::bDebugTileset = false;


bool SkCodeParam::bMIDIMusicEnabled = true;
bool SkCodeParam::bWAVMusicEnabled = false;

bool SkCodeParam::bUseScrollIDType = false;

bool SkCodeParam::bForceOrnateSound = true;

bool SkCodeParam::bFullLight = false;
bool SkCodeParam::bForceSaveGameReadOK = true;

float SkCodeParam::fDebugRainSpeed = 1;
short int SkCodeParam::iTickSpeedFactor = 3;

UINT SkCodeParam::iVideoScale = 3;
float SkCodeParam::fVideoYScale = 1.20f;

bool SkCodeParam::bUseExtendedSound = false;

#if (DM2_EXTENDED_MODE == 0)
bool SkCodeParam::bUseVaryingPlaybackFrequency = false;
bool SkCodeParam::bUseMultilanguageExtended = false;
bool SkCodeParam::bUsePlayerWalkSound = false;
#elif (DM2_EXTENDED_MODE == 1)
bool SkCodeParam::bUseVaryingPlaybackFrequency = false;
bool SkCodeParam::bUseMultilanguageExtended = true;
bool SkCodeParam::bUsePlayerWalkSound = true;
#endif // DM2_DEBUG_SUPER_MODE



#if (DM2_DEBUG_SUPER_MODE == 0)
bool SkCodeParam::bUseSuperMode = false;
#elif (DM2_DEBUG_SUPER_MODE == 1)
bool SkCodeParam::bUseSuperMode = true;
#endif // DM2_DEBUG_SUPER_MODE


bool SkCodeParam::bDebugNoImageDecodingAssert = true;
bool SkCodeParam::bDebugBypassNullPointers = true;	// DEBUG PURPOSE, try running further even with some NULL pointers.
// Normally it would crash it if it NULL. Meaning there is something wrong before that makes that pointer NULL.
