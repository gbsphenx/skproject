//------------------------------------------------------------------------------
// SKCODE CORE V4
//	Main structure for common global variables
//------------------------------------------------------------------------------

#ifndef _SKCODE_CORE_V4_H_
#define _SKCODE_CORE_V4_H_

#include <sktypes.h>
#include <sktypesx.h>
#include <dme.h>

// necessary for both V4/V5
using namespace DMEncyclopaedia;
using namespace DM2Internal;

//------------------------------------------------------------------------------
//..............................................................................

// http://dmweb.free.fr/community/documentation/file-formats/saved-game-files/
typedef struct
{
  // The following 128 bytes are the second thing
  // read from the game file.
  i32 Time;          //000; reversed
  i32 ranseed;       //004; reversed
  U16 ObjectInHand; //008; swapped
  i16 numcharacter;  //010; swapped
  i16 partyx;        //012; swapped
  i16 partyy;        //014; swapped
  i16 partyfacing;   //016; swapped
  i16 partyLevel;    //018; swapped
  i16 handChar;      //020; swapped Character index
  i16 MagicCaster;   //022; swapped
  i16 NumTimer;      //024; swapped
  i16 NextAvailTimer;//026; swapped
  i16 MaxTimers;     //028; swapped
  i16 ITEM16QueLen;  //030; swapped
  i32 Long12950;     //032; reversed
  i32 Long12954;     //036; A timestamp. reversed
  i16 Word11710;     //040; swapped
  i16 Word11712;     //042; swapped
  i16 Word11714;     //044; swapped
  i16 MaxITEM16;     //046; swapped
  i8  FILL180[80];   //048;
} tDM1SaveGameBlock3;

typedef struct  // 3-byte structure in character
{
  U8 ubMaximum;
  U8 ubCurrent;
  U8 ubMinimum;
} tATTRIBUTE;

typedef struct	// 6-byte structure in character
{
  i16 word0;
  i32 Long2;  // Experience in this skill
} tSKILL;

typedef struct  // character info???
{ 
  X8  name[8];  // 00
  X8  title[16];// 08 // size??
  i16 wordx24; // Not swapped because I don't see it used.
  U8 FILL26[28-26];
  U8 facing;  //28
  U8 position; //29
  U8 byte30;
  U8 byte31;
   i8 attackType; //32Signed so check for -1 works.
   i8 byte33;
   i8 incantation[4];//34;
        // [0] = power 96 through 111 --> 1 through 6
        // [1] =      102 through 107
        // [2] =      108 through 113
        // [3] =      114 through 119
  U8 FILL38[40-38];
  U8 facing3; //40
  U8 uByte41;
  U8 uByte42; //Poison count.  A timer for each one??
  U8 uByte43;
  i16 busyTimer;// 44 // Timer Index. Signed so -1 works.
  i16 timerIndex;//46;
  i16 word48; // ORed with 0x280
              // 0x8000 while selecting attack option
  i16 ouches;     //50;// mask of damaged body parts.
  i16 HP;         //52
  i16 maxHP;      //54
  i16 stamina;    //56;
  i16 maxStamina; //58;
  i16 mana;       //60;
  i16 maxMana;    //62;
  i16 word64;
  i16 food;       //66;
  i16 water;      //68;
  tATTRIBUTE Attributes[7]; // 70 (maximum, current, minimum)
                           // 70 = [0] Luck??
                           // 73 = [1] Strength
                           // 76 = [2] Dexterity
                           // 79 = [3] Wisdom
                           // 82 = [4] Vitality
                           // 85 = [5] AntiMagic
                           // 88 = [6] AntiFire
  i8  FILL91;
  tSKILL skills92[20]; // 92 //0 and 4-7  =Fighter
                            //1 and 8-11 =Ninja
                            //2 and 12-15=Priest
                            //3 and 16-19=Wizard
  X16  possessions[30]; //212
  U16 load;//272; In 10ths of KG
  U16 shieldStrength; //274;
  U8 FILL276[336-276];
  i8  portrait[1]; //336
  U8 FILL337[800-337];
} tCHARDESC;

//..............................................................................

//using ObjectID = DMEncyclopaedia::ObjectID;
//typedef DMEncyclopaedia::ObjectID ObjectID;

//..............................................................................
//------------------------------------------------------------------------------

typedef U16 (*pfnOversee_t)(ObjectID *ref, void *pv);

// old legacy system variables
// cd.dos.
typedef struct {

	//U16	(SkWinCore::*pfnMouseEventReceiver)(U16 xx, U16 yy, i16 zz);	// (::*_04bf_179e)
	X16		glbMouseEventReceiverSet;	// (_04bf_03c6)
	X16		glbMouseHandlerSet;			// (_04bf_03d4)
	X16		glbKeyboardHook;			// (_04bf_02bc)

	U16		glbCPUClass;			// (_04bf_17aa)	CPU class

	U16		glbIBMTick;				// _01b0_14d8



} SKCoreLegacySystem;

// cd.si.
typedef struct {
	U8*	tblZStringsBuffer[2];	// (_4976_52ea)
	U16	glbZStringRoundRobinIndex;	// _4976_52f8

} SKCoreSysInternal;


// cd.mc.
typedef struct {

	U16	_4976_5c92_cache;		// (_4976_5c92) cnt used entries of _4976_5c7e?
	U16	_4976_5c90_cache; // (_4976_5c90)


} SKCodeMemCache;


//..............................................................................

// cd.gs.
typedef struct {


} SKCoreGameState;

//..............................................................................

// cd.gg.
typedef struct {

	U8	glbCreaturesMaxCount;	// (_4976_4c56) Nb of different types of creatures within GDAT
	U8*	glbSomeCreatureTable;	// (_4976_4c58) 3 values per creature type

	U16	glbRefreshViewport;		// _4976_4e5c (tells to refresh viewport ?)
	U16	glbGameHasEnded;		// (_4976_01ba)

} SKCoreGameGeneral;


//..............................................................................

// cd.mo.
typedef struct {

	i16	glbSpecialScreen;
	X8* glbImageCreditScreen;		// (_4976_52ae)
	X8* glbImageMenuScreen;			// (_4976_52b2)

} SKCoreMenuOption;

//..............................................................................

// cd.mk.
typedef struct {

	U16		glbMouseCurVis2;	// (_04bf_17a8) mouse cursor vis2?

	i16		glbMouseVisibility;		// (_4976_4860)

	U16		mice_btn;
	U16		mice_x;
	U16		mice_y;

	U16		glbMouseButtonState;		// (_04bf_17ee) mouse push state (latest); bit0=left, bit1=right
	U16		glbMouseXPos;				// (_04bf_03be) mouse x-pos
	U16		glbMouseYPos;				// (_04bf_03c0) mouse y-pos
	U16		glbMousePreviousXPos;		// (_04bf_03c2) prev xpos
	U16		glbMousePreviousYPos;		// (_04bf_03c4) prev ypos

	i16		glbSomeMouseX;			// (_04bf_185e)
	i16		glbSomeMouseY;			// (_04bf_1860)
	U16		glbSomeMouseButton;		// (_04bf_1936)


//--- These are special from external control outside SKWin (not original code)
	U16 glbExternalMouse;
	U16	glbExternalMouseX;
	U16	glbExternalMouseY;

} SKCoreMouseKeyboard;

// cd.sc.
typedef struct {

	U16		glbSoundBlasterBasePort;		// (_04bf_0288) sblaster base port #
	U16		glbSoundCardType;		// (_04bf_028a) sndcard type: sblaster=6
	U16		sndSoundInBuffer;		// (_04bf_028c) =1
	U16		sndSoundBufferSize;		// (_04bf_028e)
	X8		*sndSoundBuffer;	// (_04bf_0290) sound buffer
	U16		sndSoundPlaybackFrequency;		// (_04bf_17a6) playback speed

	U16		sndLockSoundBuffer;

	U16		glbMemRAMAudioStuff;	// (_4976_5c7c)

	X16		_04bf_04f1;
	U8		_4976_04ee;			// (_4976_04ee)
	X16		_01b0_14d9;			// _01b0_14d9

	tCDMusicMap		*glbTabCDMusicTriggers;	// Contains CD.DAT file data

	X16		glbSndDataLoaded;			// (_4976_5cae)
	U16		glbGDatSoundRawDatCount;		// (_4976_5d58)

	SoundStructure		*_4976_5efe;
	SoundStructure		*_4976_5f02;
	SoundShortInfo		*tblSoundShortInfo;	// (_4976_5f06) size=?
	SoundShelfChained	*tblSoundShelf;	// (_4976_5f0a) size=nb gdat snd loaded
	SoundEntryInfo		*glbSoundList;	// size=8 (_4976_5f0e)


} SKCoreSoundCard;

// cd.pi.
typedef struct {

	U16	glbIsPlayerSleeping; // (_4976_4be8) 1 if sleep?
	U16	glbIsPlayerMoving; // (_4976_4bea) 0 if you stay, not 0 if stepping forward/backward
	U16	glbPlayerLastMove;		// (_4976_4bf2) Last effective move (see PERFORM_MOVE)
	U16	glbPlayerMap;	// (_4976_4bf4) current player map #
	U16	glbPlayerDir;	// (_4976_4bf6) current player dir (0,1,2,3=N,E,S,W)
	U16	glbPlayerSpecialStairsDir;	// SPX: added this for special behaviour going stairs for BW
	i16	glbPlayerPosX;	// (_4976_4bfa) current player x-axis (column)
	i16 glbPlayerPosY;	// (_4976_4c06) current player y-axis (row)
	U16	glbNextChampionNumber;		// (_4976_4bfc) Related to champions count ?
	U16	glbChampionsCount;				// (_4976_4c3c) player count
	U16	glbChampionsPendingDamage[MAX_CHAMPIONS];	// (_4976_57ea) amount of damage for players
	i16	glbChampionIndex;	// (_4976_5366) SPX: Used as a champion cursor in the MAX_CHAMPIONS+1 champions table.
	U16	glbPlayerDefeated;	// (_4976_4c2a) all player are defeated
	LeaderPossession	glbLeaderHandPossession;	// (_4976_57c8) player handy posession

} SKCodePlayerInfo;

// SPX: _4976_53b4 renamed tblChampionSquad
#define tblChampionSquad (&glbChampionTable[1])

//..............................................................................

// cd.dm1.
typedef struct {

// DM1 support variables
	bool	bDM1PortraitsActivated[4];
	X8		xDM1PortraitsData[4][(29*32)+6];	// a DM2 image contains 6 bytes of info before image itself

//--- Savegame variables
	X8*		pDM1SavegameBlock[10];	// store the different blocks to decrypt when reading a DM1 savegame
	U16		iDM1SVGDecryptKey2;
	U16		iDM1SVGBlock2Checksum;
	U16		iDM1SVGDecryptKey3;
	U16		iDM1SVGDecryptKey4ExtCreatures;
	U16		iDM1SVGDecryptKey5Champions;
	U16		iDM1SVGDecryptKey6Timers;
	U16		iDM1SVGDecryptKey7TimerQueue;
	U16		iDM1SVGNbExtCreatures;

//
} SKCoreDM1Variables;


//..............................................................................
//------------------------------------------------------------------------------

typedef struct {

	SKCoreLegacySystem		dos;

	SKCodeMemCache			mc;
	SKCoreSysInternal		si;

	SKCoreDM1Variables		dm1;

	SKCoreMenuOption		mo;

	SKCoreSoundCard			sc;
	SKCoreMouseKeyboard		mk;

	SKCoreGameGeneral		gg;
	SKCoreGameState			gs;
	SKCodePlayerInfo		pi;

} SKCoreData;

//------------------------------------------------------------------------------

#endif // _SKCODE_CORE_V4_H_