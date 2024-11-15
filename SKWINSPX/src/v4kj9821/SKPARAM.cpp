#include <StdAfx.h>
#include <SkParam.h>

bool SkCodeParam::bUseBigEnd = false;	// Default is little-end

bool SkCodeParam::bNoAudio = true;
bool SkCodeParam::bChampionImmune = true;

bool SkCodeParam::bDM1Mode = false;
bool SkCodeParam::bDM1ReadSavegame = false;
bool SkCodeParam::bDM1TQMode = false;	// Try to get the more TQ compatibility mode, specially in dungeon, many visible actuators do sound whereas the sound flag is not set.

bool SkCodeParam::bDM2V5Mode = false;	// activate when detecting PC-DOS V5 graphics.dat

bool SkCodeParam::bBWMode = false;
bool SkCodeParam::bEOBMode = false;
bool SkCodeParam::bRandomInitChampion = false;
bool SkCodeParam::bAllowFreeObjectMove = false;	// activated with custom scripted dungeon, to allow any object to be put into the dungeon from commands

bool SkCodeParam::bUseDM2ExtendedMode = true;
bool SkCodeParam::bUsePowerDebug = true;
bool SkCodeParam::bUseFixedMode = true;
bool SkCodeParam::bUseIngameDebug = false;
bool SkCodeParam::bDebugInfoMapInit = false;

bool SkCodeParam::bUseSuperInfoEye = false;
bool SkCodeParam::bShowLuck = false;

bool SkCodeParam::bDisableFogEffect = false;
bool SkCodeParam::bWeakDoors = false;
bool SkCodeParam::bWalkOverPits = false;
bool SkCodeParam::bUseCustomSpells = false;	// Start FALSE. This is automatically put to TRUE if graphics.dat contains custom spell definitions
bool SkCodeParam::bUnlimitedCharges = false;
bool SkCodeParam::bInfiniteSpells = false;
bool SkCodeParam::bNoTimer = false;

bool SkCodeParam::bMIDIMusicEnabled = true;
bool SkCodeParam::bWAVMusicEnabled = false;

bool SkCodeParam::bUseScrollIDType = false;

bool SkCodeParam::bForceOrnateSound = true;

bool SkCodeParam::bFullLight = false;
bool SkCodeParam::bForceSaveGameReadOK = true;


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