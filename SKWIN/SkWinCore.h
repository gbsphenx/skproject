//------------------------------------------------------------------------------

#ifndef _SKWINCORE_H_
#define _SKWINCORE_H_

//------------------------------------------------------------------------------

#include <SkVersionControl.h>

//------------------------------------------------------------------------------

#include <DME.h>
#include <fileset.h>
#include <SkWinMIDI.h>

//..............................................................................

using namespace DMEncyclopaedia;
using namespace DM2Internal;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

/*
#include "_4976_03a2.h"

#if defined(_USE_MFC80) || defined(_USE_MFC60)
#include "SkWinMFC.h"
#include "resource.h"
#endif // defined(_USE_MFC80) || defined(_USE_MFC60)
*/
//------------------------------------------------------------------------------

class SkWinCore 
{

public:
#if defined(_USE_MFC80) || defined(_USE_MFC60) || defined(_USE_SDL)
	// 
	std::stack<U32> vecLowerAlloc, vecUpperAlloc;
	// 
	std::stack<ObjectID> vecDelayDeallocRecords;
#endif // defined(_USE_MFC80) || defined(_USE_MFC60) || defined(_USE_SDL)

protected:

	// 
	typedef U16 (SkWinCore::*pfnOversee_t)(ObjectID *ref, void *pv);

// SPX: _4976_53b4 renamed glbChampionSquad
#define glbChampionSquad (&glbChampionTable[1])

	SkWinMIDI* skmidi;

#if DM2_EXTENDED_MODE == 1
	U8	glbXAmbientSoundActivated;
#endif

	//{{SKSAVE_INSORTED_VARS
	File_header fileHeader;
	void	*__vram;		// =0xA0000000
	Bit8u	*pbVram;		// =0xA0000000
	fileSet	fset;
	void (SkWinCore::*intrvect[256])();
	void (SkWinCore::*_int33_mouse_callback)();
	Bit16u	(SkWinCore::*_4976_0cba[12])(sk1891 *ref);
	void	(SkWinCore::*_4976_38a4[17])(U8 xx, U8 yy, sk1bf9 *ss);
	U16		mice_btn, mice_x, mice_y;
	U32	tickThen;
	//}}SKSAVE_INSORTED_VARS

	//{{SKSAVE_DESORTED_VARS
	U8 _04bf_00dc[16];
	U8 _00eb_0023[16];		
	U8 _04bf_0a50[16][3];
	void (SkWinCore::*_089c_025c)();	// int 0feh
	U8 *_089c_0348;
	U8 *_089c_0354;
	SRECT _089c_00d0; // 0,0,320,200
	X16 _089c_0352;
	void (SkWinCore::*_089c_0340)();	// int 0feh
	void (SkWinCore::*_089c_0254)();	// int 0ffh
	U8 _089c_0090[64];		// palette
	U8 _089c_034c[100];
	X32 _089c_02c0[100];
	sk0260 _089c_025a[100];
	i32	_089c_0344;

	X16	_01b0_3378;
	X16	_4976_532c;
	MouseState _4976_4e02[11];
	sk0e80 _04bf_0e80[4];
	U8	_4976_5e72[18];
	void	(SkWinCore::*_01b0_0453)();
	void	(SkWinCore::*_01b0_092d)();
	void	(SkWinCore::*_01b0_13be)();

	U16		_01b0_0933;
	X16		_01b0_13c2;
	U16 _01b0_13c6;
	Bit16u	_01b0_13c8;
	U16 _01b0_13ca;
	X16	glbSoundFreq_13cc;		// (_01b0_13cc)
	Bit16u	glbSoundFreq_13ce;	// (_01b0_13ce)
	Bit8u	*sndSoundToPlayBuffer;	// sndbuff: buff
	Bit16u	sndLockSoundBuffer;
	U8 _01b0_13d8[256];
	U16		_01b0_14d8;
	X16	_01b0_14d9;
	X16		_01b0_14db;
	Bit16u	_01b0_14dd;		// sndbuff: current writing cursor
	Bit16u	sndSoundToPlayBufferSize;		// sndbuff: buff size (=0x35f4)

	Bit16u	glbFreqComparator;		// (_01b0_4896)

	Bit8u	glbUpdatePalette;		// (_04bf_00fc)
	Bit16u	glbMouseCursorVisible;		// (_04bf_00fd) mouse cursor vis1: 0=invisible, 1=visible
	X16		glbZMode;		// (_04bf_027e)
	i16		_04bf_0280;
	Bit16u	glbDMode;		// (_04bf_0282)
	Bit16u	_04bf_0284;		// timer running
	X16		glbPType;		// (_04bf_0286)
	Bit16u	glbSoundBlasterBasePort;		// (_04bf_0288) sblaster base port #
	Bit16u	glbSoundCardType;		// (_04bf_028a) sndcard type: sblaster=6
	Bit16u	sndSoundInBuffer;		// (_04bf_028c) =1
	Bit16u	sndSoundBufferSize;		// (_04bf_028e)
	Bit8u	*sndSoundBuffer;	// (_04bf_0290) sound buffer
	X16	sysSomeSemaphore;
	i16		_04bf_0296;
	X16		_04bf_0298;
	X16		_04bf_029a;
	X16	_04bf_02bc;
	Bit16u	glbMouseXPos;		// (_04bf_03be) mouse x-pos
	Bit16u	glbMouseYPos;		// (_04bf_03c0) mouse y-pos
	Bit16u	glbMousePreviousXPos;		// (_04bf_03c2) prev xpos
	Bit16u	glbMousePreviousYPos;		// (_04bf_03c4) prev ypos
	Bit16u	_04bf_03c6;
	i16	_04bf_03c8;
	i16	_04bf_03ca;
	i16	_04bf_03cc;
	i16	_04bf_03ce;
	X16	_04bf_03d0;
	X16 _04bf_03d2;
	X16		_04bf_03d4;
	X16	_04bf_04f1;
	X16	_04bf_04f3;
	X16	glbDecreasing120;			// (_04bf_050d) SPX: Used only one time, as a constant = 120
	X16	glbCounterZero_0517;		// (_04bf_0517) value = 0 and it does not change ..
	i32	glbSomeCounter_0519;		// (_04bf_0519)
	U8		_04bf_060b[80];
	Bit16u	_04bf_079c;			// internalBlit colorkey
	Bit8u	_04bf_079e[0x0242];	// size=?. blit dest buffer
	X8		_04bf_09de;
	SRECT	_04bf_09e0;
	SRECT	_04bf_09e8;
	Bit8u	*_04bf_0cf0;	// internalBlit src
	Bit8u	_04bf_0cf4[320];
	Bit8u	*_04bf_0e34;	// internalBlit dst (=vram?)
	Bit16u	_04bf_0e38;
	X8	_04bf_0e4a;
	X16	_04bf_0e4c;
	X16		_04bf_0e4e;
	X16		_04bf_0e50;
	U16		sysMousePositionCaptured;		// (_04bf_0e52) 1=mouse position is captured
	X16	_04bf_0e64;
	Bit16u	_04bf_0e7a;		// mouse event lock: 1=locked, 0=unlocked
	__int16	_04bf_0e7c;		// mouse push state (delta)
	X16 _04bf_0e7e;

	X16	_04bf_1798;
	i8	_04bf_179a;
	__int16	_04bf_179c;
	U16		(SkWinCore::*_04bf_179e)(U16 xx, U16 yy, i16 zz);
	__int16	_04bf_17a2;
	__int16	_04bf_17a4;
	Bit16u	sndSoundPlaybackFrequency;		// (_04bf_17a6) playback speed?
	Bit16u	_04bf_17a8;		// mouse cursor vis2?
	U16		_04bf_17aa;
	sk04bf17ac	_04bf_17ac[10];	// mouse input queue
	Bit16u	_04bf_17e8;
	Bit16u	glbMouseButtonState;		// (_04bf_17ee) mouse push state (latest); bit0=left, bit1=right
	Bit16u	_04bf_1850;
	SRECT	_04bf_1852;
	i16		_04bf_185a;
	i16 _04bf_185c;
	__int16	_04bf_185e;
	__int16 _04bf_1860;
	X16 _04bf_1862;
	X16		_04bf_1864;		// key read index (ringed buffer)
	i16		_04bf_1886;		// key read avail count
	X16	_04bf_18a4;
	X16	_04bf_18a6;
	X16 _04bf_18a8;
	__int16	_04bf_18aa;
	Bit16u	_04bf_18ac;
	void (SkWinCore::*_04bf_18ae)();
	Bit16u	_04bf_18b2;		// mouse availability 0=n/a, 1=avail
	Bit16u	_04bf_1934;		// mouse push state (previous)
	Bit16u	_04bf_1936;
	Bit16u	_04bf_1938;

	X16		_069a_04a0;		// ___first
	X16		_069a_04a2;		// ___last

	U8		glbBlitPalette16[16];	// (_44c8_077d) palette for blit

	X8		*_4976_0083;	// _heapbase
	U8		*_4976_0087;	// _brklvl
	U8		*_4976_008b;	// _heaptop
	Bit32u	_4976_0090;		// SPX: Has something to do with GAME TICK
	i16		_4976_00e8;		// screen to viewport x (=0)
	i16		_4976_00ea;		// screen to viewport y (=40)
	X16		_4976_00ec;		// =9576 // avail heap size while game running
	Bit16u	glbScreenWidth;		// screen width =320
	Bit16u	glbScreenHeight;		// screen height =200
	Bit16u	_4976_00f4;		// SPX: Something to do with mempool
	Bit16u	_4976_00f6;		// off-screen viewport width? (=224)
	Bit16u	_4976_00f8;		// off-screen viewport height? (=136)
	i16		_4976_00fa;
	i16		_4976_00fc;
	SRECT	_4976_00fe;
	Bit16u	_4976_0106;
	Bit16u	_4976_0108;
	Bit16u	_4976_010e;
	Bit16u	_4976_0110;
	i16		_4976_0112;
	i16		_4976_0114;
	i16		_4976_0116;
	Bit16u	_4976_0118;
	Bit16u	_4976_011a;
	Bit16u	_4976_011e;
	__int16	_4976_0120;
	Bit16u	_4976_0124;		// us's text cy?
	Bit16u	_4976_0126;		// us's text cx?
	i16		glbPanelStatsYDelta;		// (_4976_0128) = 7 (default) SPX: space between skill/attribute text in stats panel
	U16		_4976_012a;
	U16		_4976_012c;
	U16		_4976_012e;
	Bit16u	_4976_0130;
	__int16	_4976_0132;
	Bit16u	_4976_0134;
	Bit16u	_4976_0136;
	Bit16u	_4976_013a;		// jp's text cy
	U16		_4976_013e;		// cx? =320
	U16		_4976_0140;
	i16		_4976_0142;
	Bit16u	glbMagicMapWidth;		// (_4976_0144) width of map chip (=7)
	Bit16u	glbMagicMapHeight;		// (_4976_0146) height of map chip (=7)
	i16		glbMagicMapInterlineX;		// =1 pixels between two tiles
	Bit16u	glbMagicMapInterlineY;		// =1
	RectTable	glbRectNoTable;	// (_4976_0194) starting mem for rectno info
	Bit16u	_4976_0198;		// current ring buffer index
	Bit16u	glbGameHasEnded;		// (_4976_01ba)
	Bit16u	_4976_022c;		// 1 if modal dialog working
	X16		glbTickStepReached;		// (_4976_022e) 1 if system interval timer reached
	Bit16u	glbMapToLoad;		// (_4976_0230) non 0xffff if you need load map. change the player's current map.
	Bit8u	_4976_04ee;
	i16		glbCurrentMapIndex;		// (_4976_0c5a) current map number (is not a player's map index)
	U16		glbTargetTypeMoveObject;		// candidate target to move table // 0=Fwd, 1=FwdR, 2=BackR, 3=Back, 4=BackL, 5=FwdL, 6=Noop

	__int16	_4976_19a5;
	Bit16u	_4976_19a7;
	Bit32u	_4976_19a9;
	Bit16u	_4976_19ad;
	Bit16u	_4976_19af;
	ObjectID	glbTableToMove;	// (_4976_19ba) moving table. set oFFFF if no table.
	Bit16u	_4976_1a68;			// SPX: may it be some count of thinking creatures ? used for table _4976_4ebe

	U16		glbPreviousPrecompLight;	// (_4976_38f0)
	U16		glbPreviousLightModifier;	// (_4976_38f2)
	X16		_4976_3930;
	i16		glbDataFileHandle;	// filehandle for sksaveX.dat (SPX: _4976_394e renamed glbDataFileHandle)
	Bit16u	_4976_3950;
	Bit16u	_4976_3952;
	Bit16u	glbMinionsAssocCount;	// (_4976_3954) count of minion assoc
	Bit16u	_4976_3b55;
	Bit16u	_4976_3b57;
	Bit16u	_4976_3b59;
	Bit16u	_4976_3b5b;
	__int16	_4976_3b5d;
	X16		_4976_3d2c;
	U16		glbInventorySubpanel;	// (_4976_3d2e)
	ObjectID	_4976_3d30;
	ObjectID	glbObjectForContainer;		// (_4976_3d32) object for container?
	Bit16u	_4976_3d59;
	ItemStatBarPos	glbItemStatBarPos;	// (_4976_3d79)
	U8		_4976_3d7f; // size=?
	Bit16u	glbLangageLetterIndex;	// (_4976_3d82)	holds index for langage letter?
	Bit16u	glbRightPanelType;	// (_4976_3ddc) right pane replacement: 3=magic map 4=money-box 5=container
	Bit8u	glbSquadInterfaceMapGfxSet;	// (_4976_3df6) graphics set only used for drawing player squad interface
	Bit16u	_4976_3df7;
	Bit16u	_4976_3f68;
	Bit16u	glbPreviousRightPanelType;	// (_4976_3f6a)
	sk3f6c	_4976_3f6c;		// screen buffer of right part chest
	i16		glbChampionLeader;		// (_4976_3fc6) selected player? (leader)
	U8		glbChampionColor[MAX_CHAMPIONS];	// (_4976_3fec) player health bar color
	sk3f6c	_4976_3ff0;

	U16		_4976_404b;
	X16		_4976_4212;
	U16		glbMistyMap;		// (_4976_4214) is misty fogged map
	U16		glbSceneColorKey;		// (_4976_4216) scene colorkey
	Bit8u	glbMapGraphicsSet;		// (_4976_4218) current dungeon graphics set
	ObjectID	_4976_421a;
	X16		_4976_467f;
	X16		_4976_4681;
	X8		_4976_4683;
	__int32	_4976_4742;
	Bit16u	_4976_4746;
	X16		_4976_4748;
	Bit16u	_4976_474a;
	Bit16u	_4976_474c;
	X16		_4976_474e;
	Bit16u	_4976_4750;
	U16		_4976_475c;
	Bit8u	_4976_475e;
	__int16	_4976_4762;
	X16		_4976_47fc;
	X16		glbThunderJustCast;	// (_4976_47fe)
	X8		_4976_4806;
	Bit16u	_4976_4807;
	Bit16u	_4976_4809;
	i16		_4976_480b;		// graphics.dat signature
	Bit16u	_4976_480d;		// position at buffer unit
	X32		_4976_480f; // 90 D0 03 00 -> 00030D90
	X8		_4976_4838;
	Bit16u	_4976_4839;		// previously loaded raw data index
	Bit32u	_4976_483b;		// previously loaded raw data file pos
	Bit16u	_4976_484b;		// incremental serial for use with memory alloc counter?
	U16		_4976_484d;
	__int16	glbMouseVisibility;		// (_4976_4860)
	SRECT	_4976_4942;
	SRECT	_4976_494a;
	X16		_4976_4952;
	U16		_4976_4954[4];	// may be SRECT?
	SRECT	_4976_495c;
	Bit8u	*_4976_4964;	// NULL (to blit to screen)
	U16		_4976_4968;				// ems avail more than 16 pages, if 1
	Bit16u	_4976_499e;
	Bit16u	_4976_49a0;
	Bit32u	glbRandomSeed;		// (_4976_49ba) random seed
	Bit16u	_4976_49be;
	i16		_4976_49d0;
	i16		_4976_49d2;
	X16		glbMiscItemsLoaded;	// (_4976_4a02) 1=loaded 0=not loaded
	Bit16u	_4976_4b7a;
	Bit16u	glbPoisonAttackDamage;		// (_4976_4b7c) poisonous attack value
	U16		_4976_4b7e;
	Bit32u	_4976_4b80;
	sk4b84	_4976_4b84[4];	// size=4? temporary ring rect buffer
	Bit16u	_4976_4ba4;		// 1=rect is present
	Bit16u	_4976_4ba6;		// index of next avail temporary rects
	SRECT	_4976_4ba8[4];	// temporary 4 rects
	Bit16u	_4976_4bc8;
	SRECT	_4976_4bca;
	Bit16u	_4976_4bd2;
	Bit8u	_4976_4bd4;		// =0x01
	Bit8u	_4976_4bd5;		// =0x1a
	Bit8u	_4976_4bd6;		// =0xfe
	Bit16u	_4976_4bd8;		// dialogue visibility: 1=dialog mode
	U8		*glbPaletteT16;	// (_4976_4a02) probably, a palette set (16 bytes).
	sk4bde	*_4976_4bde;
	sk4be2	*_4976_4be2;
	Bit16u	_4976_4be6;
	Bit16u	glbIsPlayerSleeping; // (_4976_4be8) 1 if sleep?
	Bit16u	glbIsPlayerMoving; // (_4976_4bea) 0 if you stay, not 0 if stepping forward/backward
	Bit32u	glbGameTick;	// (_4976_4bec) current game tick
	Bit16u	glbTickSpeed;	// (_4976_4bf0) 0 is the fastest (no wait between game tick), 15 is slow
	Bit16u	glbPlayerLastMove;		// (_4976_4bf2) Last effective move (see PERFORM_MOVE)
	Bit16u	glbPlayerMap;	// (_4976_4bf4) current player map #
	Bit16u	glbPlayerDir;	// (_4976_4bf6) current player dir (0,1,2,3=N,E,S,W)
	Bit16u	glbRecalcLightIndicator;	// (_4976_4bf8)
	i16		glbPlayerPosX;	// (_4976_4bfa) current player x-axis (column)
	Bit16u	glbNextChampionNumber;		// (_4976_4bfc) Related to champions count ?
	Bit16u	_4976_4bfe;
	Bit16u	_4976_4c00;
	Bit16u	_4976_4c02;
	Bit16u	_4976_4c04;
	i16		glbPlayerPosY;	// (_4976_4c06) current player y-axis (row)
	Bit16u	_4976_4c08;
	U16		_4976_4c0a;
	Bit16u	_4976_4c0c;
	X16		glbIntermediateTickCounter;	// (_4976_4c0e)
	Bit16u	_4976_4c10;
	Bit16u	_4976_4c12;
	Bit16u	glbPlayerThrowCounter;	// (_4976_4c14) Spx: I don't get the real usage of this .. seems quite useless ...
	Bit8u	*_4976_4c16;	// off-screen viewport? (224x136) =0xE0008006
	Bit32u	_4976_4c1a;		// game tick?
	Bit16u	_4976_4c1e;
	Bit32u	glbAbsoluteTickCounter;			// (_4976_4c20)
	__int16	glbSpecialScreen;		// (_4976_4c24) Spx: some sort of flag telling if we are in MAIN screen, CREDITS or INGAME ?
	Bit16u	_4976_4c26;
	Bit16u	glbMap_4c28;	// (_4976_4c28) a map?
	Bit16u	glbPlayerDefeated;	// (_4976_4c2a) all player are defeated
	Bit16u	_4976_4c2c;
	Bit16u	glbSomePosX_4c2e;	// (_4976_4c2e) a x?
	Bit16u	glbSomePosY_4c30;	// (_4976_4c30) a y?
	i16		_4976_4c32;
	i16		_4976_4c34;
	Bit16u	glbLightLevel; // light level2? // SPX: _4976_4c36 renamed glbLightLevel
	Bit16u	glbShowItemStats;		// (_4976_4c38)
	U16		glbShowMousePointer;	// (_4976_4c3a) ShowCursor16572
	Bit16u	glbChampionsCount;				// (_4976_4c3c) player count
	Bit16u	_4976_4c3e;
	U16		_4976_4c40;	// dir?
	U8		_4976_4c42[16];
	Bit16u	*_4976_4c52;			// [LOCAL] Index of tiles with objects on them (per column)
	Bit8u	glbCreaturesMaxCount;				// (_4976_4c56) creatures cnt in GRAPHICS.dat
	U8		*glbSomeCreatureTable;			// (_4976_4c58) 3 values per creature type
	Bit8u	*_4976_4c5c;			// default dungeon graphics set (outside map) per level (6=ground floor)
	Bit8u	glbMapDoorOrnatesList[16];			// locallevel door deco. graphics?
	U8		*_4976_4c72;			// array of map indexes. grouped by zLevel. termed by 0xff
	i16		glbCountMoneyItems;		// (_4976_4c76) count of coin types (=6)
	Bit8u	**glbCurrentTileMap;			// (_4976_4c78) tileval = glbCurrentTileMap[column][row]
	U16		*dunMapTilesObjectIndexPerColumn;	// (_4976_4c7c) [GLOBAL] Index of tiles with objects on them (per column)
	i16		glbMoneyItemsValueTable[MONEY_ITEM_MAX];	// (_4976_4c80)
	U16		glbMoneyItemsIDTable[MONEY_ITEM_MAX];	// (_4976_4c94) coin types (by distinctive item types)
	U8		***glbMapTileValue;	// (_4976_4ca8) tileval = glbMapTileValue[mapnum][column][row]; map-number-to-map-data-column-pointer
	Bit8u	*glbMapCreaturesTypeNumber;			// (_4976_4cac) creature types on level
	U8		*_4976_4cb0;			// map[zLevel] to _4976_4c72 index
	Bit16u	_4976_4cb4;				// word count of (Index of tiles with objects on them (per column))
	U8		glbMapDoorType[2];			// (_4976_4cb6) doortype0,1
	Bit16u	glbDoLightCheck;				// (_4976_4cb8) 1 if something happens at player's map (e.g. creature barks?)
	Bit8u	_4976_4cba[16];			// locallevel floor graphics?
	U8	*glbDBObjectData[16];		// (_4976_4cca)
	Map_definitions *dunMapLocalHeader;	// [LOCAL] map def
	File_header *dunHeader;		// file header (_4976_4d0e)
	U16		*dunTextData;	// Text Data (_4976_4d12)
	U16		*dunGroundStacks;	// (_4976_4d16) List of object IDs of first objects on tiles
	sk4d1a	_4976_4d1a[13];		// pick up able items
	U8		*dunMapData;		// (_4976_4db6) Map data
	Map_definitions	*dunMapsHeaders;	// (_4976_4dba) [GLOBAL] map def
	U16		_4976_4dbe;
	U16		_4976_4dc0;
	Bit8u	_4976_4dc2[16];	// locallevel wall graphics?
	i16		glbCurrentMapWidth;				// (_4976_4dd2) current map width
	i16		glbCurrentMapHeight;				// (_4976_4dd4) current map height
	U16		*dunMapColumnsSumArray;	// (_4976_4dd6) array-of-sum-of-columns-per-map
	U16		_4976_4dda;
	Bit16u	_4976_4ddc;
	U16		_4976_4dde;		// pick up able item count
	jmp_buf	_4976_4de0;
	i16		_4976_4df4;
	Bit8u	_4976_4df6[2];
	Bit16u	_4976_4df8;		// rectno?
	i16		glbUIEventCode;		// (_4976_4dfa) SPX: sort of timer for credits screen?
	Bit16u	_4976_4dfc;
	Bit16u	_4976_4dfe;
	__int16	_4976_4e00;
	Bit16u	_4976_4e44;		// rectno?
	Bit16u	_4976_4e46;
	Bit16u	_4976_4e48;
	Bit16u	_4976_4e4a;
	Bit16u	_4976_4e4c;
	MousePosition	glbMousePosition; // (_4976_4e4e)
	Bit16u	_4976_4e5c;
	Bit16u	_4976_4e5e;	// (ring 0 to 2) last index of click event
	i16		_4976_4e60;	// (ring 0 to 2) processed index of click event
	i16		_4976_4e62;
	Bit16u	_4976_4e64;
	Bit16u	_4976_4e66;
	Bit16u	_4976_4e68;
	Bit16u	_4976_4e6a;
	MousePosition	_4976_4e6c[3];
	Bit8u	*_4976_4e96;
	Bit16u	_4976_4e9a;
	Bit16u	_4976_4e9c;
	SRECT	_4976_4e9e;
	Bit32u	_4976_4ea2;
	__int16	_4976_4ea6;
	Bit16u	_4976_4ea8;
	U16		glbMap_4976_4eaa;	// map#?
	U16		_4976_4eac; // map#?
	U16		glbPosX_4976_4eae;	// x?
	U16		glbPosY_4976_4eb0;	// y?
	U16		_4976_4eb2;
	Bit16u	_4976_4eb4;
	Bit16u	_4976_4eb6;
	U16		glbTargetPosXTest;	// (_4976_4eb8) x?
	U16		glbTargetPosYTest; // (_4976_4eba) y?
	Bit16u	_4976_4ebc;
	CreatureInfoData	*glbTabCreaturesInfo;	// (_4976_4ebe) creatures in game info
	Bit16u	glbCreaturesCount;		// (_4976_4ec2) creature cnt

#define SIZE_LOCAL_CREATURE_VAR (reinterpret_cast<const U8 *>(&glbCreatureTempTargetX) - reinterpret_cast<const U8 *>(&glbCurrentThinkingCreatureID))
	// save@PREPARE_LOCAL_CREATURE_VAR ->
	ObjectID	glbCurrentThinkingCreatureID;	// (_4976_4ec4) current thinking creature (id)
	Creature	*glbCurrentThinkingCreatureRec;	// (_4976_4ec6) current thinking creature (record)
	AIDefinition	*glbAIDef;	// (_4976_4eca)
	CreatureInfoData	*glbCurrentThinkingCreatureData;	// (_4976_4ece)
	CreatureAnimationFrame	*glbCreatureAnimationFrame;		// (_4976_4ed2)
	sk1c9a02c3 *_4976_4ed6;
	Timer	glbCreatureTimer;		// (_4976_4eda) might be a timer for creature?
	i8		_4976_4ee4;
	X8		_4976_4ee5;		// selector of next xact?
	X8		_4976_4ee6;		// creature move block?
	U8		glbSomeMap_4976_4ee7;		// _4976_4ee7 some map?
	X16		_4976_4ee8;	// sk3672.b3()
	X16		_4976_4eea;	// sk3672.b4()
	U16		_4976_4eec;
	U16		glbAIAttacksSpells;		// (_4976_4eee) global Attack and Spells types for current creature?
	X16		_4976_4ef0;
	X16		_4976_4ef2;
	X16		_4976_4ef4;
	X16		_4976_4ef6;
	X16		_4976_4ef8;
	X16		_4976_4efa;		// temp creature's word val (0F-xx-01)
	X16		_4976_4efc;		// 2nd param of _4976_37fc
	const sk4efe	*_4976_4efe;
	X8		_4976_4f02;		// 1=need zeromem _4976_4f04
	i8		_4976_4f03;
	sk4f04	_4976_4f04[4];	// 128 bytes
	WalkPath	glbCreatureWalkPath[42];	// (_4976_4f84)
	skxxx9	_4976_4fd8[1];
	skxxx9	_4976_4fee[16];
	sk514e	_4976_514e;
	WalkPath	*glbMemWalkPath;	// (_4976_515c)
	X8		_4976_5160;
	X8		_4976_5161;
	X8		_4976_5162;
	X8		_4976_5163;
	U8		*_4976_5164[42];
	//...
	X16		_4976_520c;  // temp creature's buffer rest count (64 bytes per 1 count)
	U8		*_4976_520e; // temp creature's buffer (256 bytes)
	// <- save@PREPARE_LOCAL_CREATURE_VAR

	X16		glbCreatureTempTargetX; // (_4976_5212) temp creature's target x
	X16		glbCreatureTempTargetY; // (_4976_5214) temp creature's target y
	X16		glbCreatureSomeX; // (_4976_5216) x?
	X16		glbCreatureSomeY; // (_4976_5218) y?
	X16		glbCreatureSomeZMap; // (_4976_521a) z?
	X16		_4976_521c; // tilevalue
	ObjectID	_4976_521e;	// first record on tile (glbCreatureSomeX, glbCreatureSomeY)
	ObjectID	_4976_5220;	// first creature record or xxx.
	ObjectID	_4976_5222;	// creature record
	X8		_4976_5224;
	U8		_4976_5225;	// teleporter existence for find walk path.
	TELE_inf	_4976_5226;
	X8		_4976_522c;
	X8		_4976_522d;
	X8		_4976_522e[4];
	U16		glbCreatureMap;		// (_4976_5232) creature map
	U16		glbCreatureDir;		// (_4976_5234) creature dir?
	U16		glbCreaturePosX;		// (_4976_5236) creature x
	U16		glbCreaturePosY;		// (_4976_5238) creature y
	U16		_4976_523a;
	U32		_4976_523c;
	U16		*_4976_5240;	// certain table for containers
	Bit16u	_4976_5244;
	U16		*_4976_5246;	// certain table for creatures
	Bit32u	_4976_524a;				// DUNGEON.dat file position while gameload
	Bit8u	_4976_524e;				// by suppress writer (current bit position)
	sksave_header_asc	*_4976_5250;	// 10 pieces of sksave header while gameload
	Bit8u	_4976_5254;				// by suppress writer (queued nibble)
	Bit32u	_4976_5258;				// total amount of processed bits by suppression writer
	Bit16u	_4976_525c;
	ObjectID	*glbMinionsObjectIDTable;	// (_4976_525e) minion assoc
	sksave_header_asc	_4976_5266;
	U8		_4976_5290[30];	// maybe status text.
	U8		*glbImageCreditScreen;		// (_4976_52ae)
	Champion	glbChampionTable[MAX_CHAMPIONS+1];	// (_4976_52af) SPX: oddly, the champion base starts at 1 ??
	U8		*glbImageMenuScreen;			// (_4976_52b2)
	U8		*_4976_52b6;
	U8		*_4976_52ba;
	Bit16u	glbChampionInventory;			// (_4976_52be) player index in inventory (1,2,3,4)
	U8		_4976_52c0[4];		// champion related
	ObjectID	glbCurrentContainerItems[CONTAINER_MAX_SLOT];	// (_4976_52c4[8]) recordlinks for chest.
	Bit16u	glbPrecomputedLight;		// (_4976_52d4) Precomputed light
	Bit16u	glbLightModifier;		// (_4976_52d6) 
	i16		_4976_52d8;		// drawtext xpos
	i16		_4976_52da;		// drawtext ypos
	Bit16u	glbItemWeightDg;		// weight   x part of ???.x
	Bit16u	glbTextEntryEncoded;	// (_4976_52de)
	Bit16u	_4976_52e0;
	Bit8u	_4976_52e2;
	Bit16u	_4976_52e4;		// 0=fighter, 1=ninja, 2=priest, 3=wizard
	Bit16u	glbChampionBonesIndex;		// (_4976_52e6) who's bone? (0 to 3)
	Bit16u	glbItemWeightKg;		// (_4976_52e8) weight xxx part of xxx.?
	Bit8u	*_4976_52ea[2];
	Bit16u	_4976_52f4;
	Bit16u	_4976_52f6;
	Bit16u	_4976_52f8;
	Bit16u	_4976_52fa;
	SkLoadEnt	*_4976_52fc;
	X16		_4976_5300;		// count of used _4976_52fc entries
	Bit8u	glbStrBufferActionName[20]; // (_4976_5302[20])
	Bit16u	_4976_5316;
	__int16	_4976_5318; // a map?
	Bit16u	_4976_531a;
	Bit16u	_4976_531c;
	__int16	glbSomeChampionIndex;	// SPX: somewhat a champion index (leader ??)
	__int16	_4976_5320;	// a x2?
	__int16	_4976_5322; // a y2?
	Bit16u	glbSelectedHand_2;	// (_4976_5324)
	Bit16u	_4976_5326;
	__int16	_4976_5328; // a x3?
	__int16	_4976_532a; // a y3?
	Bit16u	glbWeaponShooterNum;	// (_4976_532e) Word value 0x05 of launcher
	Bit8u	glbChampionHandCoolingDown[MAX_CHAMPIONS][3];	// _4976_5330[4][3]
	Bit16u	_4976_5336;
	__int16	_4976_533c;	// a map?
	__int16	_4976_533e;	// a map?
	Bit16u	glbChampionAttackDamage; // (_4976_5340) damage to target creature?
	Bit16u	glbChampionAlive[MAX_CHAMPIONS];	// (_4976_5342[4])
	Bit16u	glbWeaponMissileHand;	// (_4976_534a)
	ObjectID	glbObjectID_4976_534c;	// (_4976_534c)
	U8		glbItemGDATEntry;	// (_4976_534e) cmd cls4?
	U8		glbItemGDATCategory;	// (_4976_534f) cmd cls1?
	U8		glbItemGDATIndex;	// (_4976_5350) cmd cls2?
	Bit16u	glbSomeChampionPanelFlag;	// (_4976_5352)
	__int16	glbSelectedHandAction; // (_4976_5354) try to open item in hand: 0=left-hand, 1=right-hand
	Bit16u	glbMagicalMapFlags; // (_4976_5356) flags: 1=show-attacker, 2=show-items, 4=show-spells, 8=fix-position, 32=minion raising, etc
	sk5358	glbItemSelected[3];	// (_4976_5358)
	__int8	_4976_5364;
	__int8	_4976_5365;
	__int16	glbChampionIndex;	// (_4976_5366) SPX: Used as a champion cursor in the MAX_CHAMPIONS+1 champions table.
	ObjectID	glbChampionItemInUse;	// (_4976_5368)
	__int16	_4976_536a;	// a x1?
	__int16	_4976_536c;	// a y1?
	sk536e	glbChampionEngagingHandCommand[MAX_CHAMPIONS][2];	// (_4976_536e[4][2])	related to engaging commands (weapons ?)
	__int8	_4976_539e;
	__int8	_4976_539f;
	Bit16u	_4976_53a0;
	Bit16u	_4976_53a2;
	Bit16u	_4976_53a4;
	SRECT	_4976_53a6;
	Bit8u	glbHoldedContainerType;	// (_4976_53ae) type of showing chest
	LeaderPossession	glbLeaderHandPossession;	// (_4976_57c8) player handy posession
	U8		_4976_57de;
	U16		glbLeaderItemFlags;	// (_4976_57e0) item flags (wordvalue 00)
	U16		glbLeaderItemWeight;	// (_4976_57e2) weight of holding item by leader
	GlobalEffects	glbGlobalSpellEffects; // (_4976_57e4) structure holding freeze counter?
	X16		glbChampionsPendingDamage[MAX_CHAMPIONS];	// (_4976_57ea) amount of damage for players
	U16		_4976_57f2;
	X16		glbChampionsBodyFlags[MAX_CHAMPIONS];	// (_4976_57f4) SPX: flags for damage ??
	U8		glbStrBufferNumber[5];	// (_4976_57fc) temp str to store damage number
	U16		glbLastChampionInventory;		// (_4976_5802) older player index in inventory (1,2,3,4)
	U16		_4976_5804;
	U16		_4976_5806;
	U16		_4976_5808;
	U8		glbChampionMajorSkillsLevel[MAX_CHAMPIONS][SKILL_MAJOR_COUNT];	// _4976_580a[4][4]
	Bit16u	_4976_581a;		// inventory button pushing flag: 1=name-plate, 2=x icon, 4=zzz icon, 8=save icon
	Bit16u	_4976_581c;		// new-map?
	Bit16u	_4976_581e;		// new-dir?
	Bit16u	_4976_5820;
	Bit16u	_4976_5822;
	Bit16u	_4976_5824;
	__int16	_4976_5826;		// new-x?
	__int16	_4976_5828;		// new-y?
	U8		_4976_582a[16];	// palette
	X8		_4976_592a;
	U8		_4976_592b[13];
	SRECT	_4976_5938;
	ExtendedPicture	glbTempPicture;		// (_4976_5940) temp picst
//	X16		_4976_5944;
//	X16		_4976_5940.w18;
//	X16		_4976_5954;
//	i16		_4976_5958;
//	X16		_4976_595a;
//	X16		_4976_595c;
//	X16		_4976_595e;
//	X16		_4976_5940.w32;
//	X16		_4976_5940.w34;
//	SRECT	_4976_5964;
//	U8		*_4976_5940.pb44;
//	X16		_4976_5970;
//	X16		_4976_5972;
//	X16		_4976_5974;
//	X16		_4976_5976;
//	X16		_4976_5978;
	U16		glbSceneFlags;	// (_4976_5a7a) flags defined in word 0x65 of graphicsset category
	U32		(*_4976_5a7c)[23];
	sk5a80	*_4976_5a80;
	U8		(*_4976_5a84)[17]; // (cx,cy)=(17,21). 17*21+1¨358 bytes
	i16		_4976_5a88;	// light level?
	U32		_4976_5a8a;
	Creature	*_4976_5a8e;
	U16		glbGeneralFlipGraphics;		// flips graphics depending on player step
	i16		_4976_5a94;		// clicked x?
	i16		_4976_5a96;		// clicked y?
	U8		(*_4976_5a98)[14]; // 01-00-07-0A-00-00
	i16		_4976_5a9c;
	i16		_4976_5a9e;
	U16		_4976_5aa0;		// base dir
	U16		_4976_5aa2;		// 1=doing table operation
	U8		(*_4976_5aa4)[17]; // (cx,cy)=(17,21). 17*21+1¨358 bytes
	ExtendedPicture	_4976_5aa8;
	U32		(*_4976_5be2)[23];
	U16		_4976_5be6;
	X16		_4976_5be8;
	Bit16u	_4976_5bea;
	Bit16u	_4976_5bec;
	U8		_4976_5bee[4]; // [0] 1 if MUCH memory avail.
	__int16	_4976_5bf2;
	X16		_4976_5bf4;
	Bit16u	_4976_5bf6;
	Bit8u	_4976_5bf8[2];	// cnt kanji entries in each cls2
	skxxxf	*_4976_5bfa[2];	// skxxxf *
	U32		_4976_5c02[1];
	U16		_4976_5c06;
	U8		*_4976_5c08;
	U16		_4976_5c0c;		// index for _4976_5c02
	Bit8u	*_4976_5c0e;	// font-bitmap at 01-00-07-00-00-00
	U16		_4976_5c12;
	Bit16u	glbTimersActiveCount;				// (_4976_5c14) active timer count
	Bit16u	glbTimersMaximumCount;				// (_4976_5c16) maximum timer count
	Bit16u	glbTimerIndexNextAvailable;				// (_4976_5c18) next avail timer index
	Bit16u	*glbTimerNextEntries;			// (_4976_5c1a) next timer entries to be proceeded
	Bit16u	glbTimersCount;				// (_4976_5c1e) timer count?
	Timer	*glbTimersTable;	// (_4976_5c20) timer entities?
	Bit32u	_4976_5c24;
	X16		_4976_5c28;
	DistantEnvironment	_4976_5c2a[5];
	U8		glbRainHasThunderImage;	// (_4976_5c5c)
	Bit8u	glbRainFlagSomething;	// (_4976_5c5d)
	Bit8u	glbRainStrength;	// (_4976_5c5e) rain strength: 0=no rainfall, 1to15=lv1, 16to63=lv2, 64to127=lv3, 128to255=lv4
	Bit8u	glbRainRelated3;	// (_4976_5c5f)
	Bit8u	glbRainLevelForSky;		// (_4976_5c60)
	Bit8u	glbRainLevelForGround;	// (_4976_5c61)
	Bit8u	glbRainRelated2;		// (_4976_5c62)
	Bit8u	glbRainAmbientLightModifier;	// (_4976_5c63) 1 when darker, 0 else
	Bit8u	glbRainMultiplicator;				// (_4976_5c64) Rain/Thunder related
	i16		glbRainStormController; // (_4976_5c66) Rain and storm strength
	Bit16u	glbRainSomeLightMod;
	Bit8u	glbRainDirection;		// (_4976_5c6a) rain direction
	U8		glbRainHasRainFall;		// (_4976_5c6b)
	U8		glbRainHasCloudSky;		// (_4976_5c6c)
	Bit32u	glbRainSpecialNextTick;	// (_4976_5c6e)
	X16		glbRainThunderRandom;	// (_4976_5c72)
	U8		glbRainHasWetGround;	// (_4976_5c74)
	U16		glbRainSceneType;	// (_4976_5c76) WordValue:66
	Bit16u	_4976_5c78;		// next index of free entry mement
	X16		_4976_5c7a;
	U16		_4976_5c7c;
	Bit16u	*_4976_5c7e;	// ici-to-cacheindex. to supply quick sort for searching hash value from _4976_5c86
	U16		*_4976_5c82;	// raw data index to mementi. (usually 3491 word values). 0xffff for unused mark.
	Bit32u	*_4976_5c86;	// cacheindex-to-hashval:  0xFFFFxxxx for pict?  0x2000xxxx for creature thing?  (13:dbidx,7:horzScale,5:vertScale) for stretched pict?
	U16		_4976_5c8a;
	mement	*_4976_5c8c;	// mement#3
	Bit16u	_4976_5c90;
	Bit16u	_4976_5c92;		// cnt used entries of _4976_5c7e?
	__int16	_4976_5c94;		// filehandle of GRAPH1.DAT or GRAPHICS.DAT
	__int16	_4976_5c96;		// filehandle of GRAPH2.DAT if exists.
	shelf_memory *_4976_5c98;
	Bit16u	_4976_5c9c;		// 1 if run on floppy, 0 if run on harddisk. (to know which is used GRAPH1.DAT or GRAPHICS.DAT)
	Bit16u	_4976_5c9e;
	X32		_4976_5ca0; // 98 B7 00 00 -> 0000B798
	mement	**tlbMementsPointers;	// (_4976_5ca4) mementi-to-mement
	Bit16u	_4976_5ca8;
	Bit32u	_4976_5caa;		// copy of GRAPHICS.DAT absolute file position of 2nd raw data
	X16		_4976_5cae;
	Bit16u	_4976_5cb0;
	mement	*_4976_5cb2;	// for complex alloc mem. most lower address of memory pool(#x1) in FIRE.exe?
	GDATEntries	glbGDatEntries;		// (sk5cb6 _4976_5cb6) entries for loaded gdat
	U8		*_4976_5ce2;	// for complex alloc mem. most upper address of memory pool(#x1) in FIRE.exe
	mement	*_4976_5ce6;	// mement init pool
	Bit32u	_4976_5cea;		// size of GRAPH1.DAT or GRAPHICS.DAT
	U16		_4976_5cee;
	U8		*_4976_5cf0;
	Bit8u	*_4976_5cf4;	// for allocmem. most upper address of free memory pool(#1) in FIRE.exe
	i32		_4976_5cf8;		// for complex alloc mem. avail size of memory pool(#x1)?
	sk5cfc_root	_4976_5cfc;
	sk5d00	*_4976_5d00;	// EMS mapped memory location. always point to E000:0000. also first mement located at first EMS page
	U8		*_4976_5d04;	// temp buffer. size=1024
	Bit16u	*_4976_5d08;	// cacheindex-to-mementi. use with tlbMementsPointers[_4976_5d08[xxx]]
	sk5d0c	*_4976_5d0c;	// image chain table
	Bit16u	_4976_5d10;
	sk5d12	_4976_5d12;
	shelf_memory _4976_5d20;	// top of EMS
	Bit16u	_4976_5d24;	// =128?
	i32		_4976_5d26; // 9E AA 01 00 -> 0001AA9E
	Bit32u	_4976_5d2a;		// tick?
	X16		glbGDatNumberOfData; // (_4976_5d2e) 3,491 entries in graphics data file
	i32		glbFreeRAMMemPool;		// (_4976_5d30) for allocmem. avail size of free memory pool(#1). 325732 bytes avail when dosbox runs
	Bit16u	_4976_5d34;
	__int16	_4976_5d36;		// index to (_4976_5d08|_4976_5c7e). for recycle indexer?
	U16		*_4976_5d38;	// at first it points [1st rawdata], at second it points [1st CGDEntry]
	X16		glbGDatNumberOfRawEntries;		// (_4976_5d3c) w2 of 1st raw (cnt of raw entries)
	X16		_4976_5d3e;		// size of CGDEntry
	X16		_4976_5d40;		// w4 of 2nd raw
	i16		_4976_5d42[7];
	U8		_4976_5d50[7];
	U16		_4976_5d58;
	tiamat	_4976_5d5a;	// for allocmem. most upper address of largest memory pool (#2) in ibmio?
	mement	*_4976_5d5e;	// mement#5
	i32		_4976_5d62; // C6 6C D0 00 -> 00D06CC6
	U32		_4976_5d66;
	Bit32u	_4976_5d6a;		// GRAPHICS.DAT absolute file position of 2nd raw data
	Bit16u	_4976_5d6e;		// poolflag of EMS memory pool
	mement	*_4976_5d70;	// mement#4
	Bit16u	glbNumberOfMements;		// (_4976_5d74) cnt tlbMementsPointers
	Bit16u	_4976_5d76;		// complex memory pool availability: 0=disabled, 1=enabled
	Bit16u	_4976_5d78;		// image chain map (entry cnt)
	Bit32u	_4976_5d7a;		// size of 1st direntry rawdata
	sk5d12	_4976_5d7e;
	__int32	glbFreeEMSMemPool;		// for allocmem. avail size of memory pool (#2). 540 byes avail when dosbox runs
	mement	*_4976_5d90;	// mement#2
	mement	*_4976_5d94;	// mement#1
	Bit16u	_4976_5d98;	// x?
	Bit16u	_4976_5d9a;	// y?
	Bit16u	_4976_5d9c;
	Bit16u	_4976_5d9e;
	i16		_4976_5da0;	// 2nd screen's mousex?
	i16		_4976_5da2;	// 2nd screen's mousey?
	sk0cea	*_4976_5da4;
	Bit16u	_4976_5da8;	// x?
	Bit16u	_4976_5daa;	// y?
	U16		_4976_5dac;	// 1=holding item
	skxxx7	_4976_5dae;
	Bit16u	_4976_5dba;
	Bit16u	_4976_5dbc;		// 0=no drag, 1,2,3,4=dragging player icon
	U8		*_4976_5dbe;	// palette?
	X16		_4976_5dc2;
	Bit8u	*_4976_5e64;	// blit2mem src
	Bit16u	_4976_5e68;
	Bit8u	*_4976_5e6a;	// blit2mem dst
	U8		*_4976_5e6e;	// blit2mem underlay
	X32		_4976_5e88;		// ptr int 0xff
	X32		_4976_5e8c;		// ptr int 0xff
	X32		_4976_5e90;		// ptr int 0xfe
	Bit8u	*_4976_5e94;
	Bit16u	_4976_5e96;
	X32		_4976_5e98;		// ptr int 0xfe
	U32		_4976_5e9c;
	Bit8u	*_4976_5ea0;	// EMS mapped memory location (E000:0000)
	Bit16u	_4976_5ea4;		// EMS handle
	U32		_4976_5ea6;		// EMS alloced size
	i16		glbTickStepValue;	// (_4976_5eaa)
	shelf_memory _4976_5eac;		// ems last page? for back VRAM?
	Bit16u	_4976_5eb0;
	U16		_4976_5eb2;		// curdrive? 1=A, 2=B, ...
	X16		_4976_5eb4;
	X16		_4976_5eb6;
	Bit16u	_4976_5eb8;
	X16		glbDriveNumber;		// drive number
	X16		_4976_5ebc;
	sk5f0a	*_4976_5efa;
	SoundStructure	*_4976_5efe;
	SoundStructure	*_4976_5f02;
	sk5f06	*_4976_5f06;	// size=?
	sk5f0a	*_4976_5f0a;	// size=?
	SoundEntryInfo	*glbSoundList;	// size=8 (_4976_5f0e)
	Bit8u	glbIngameGlobVarBytes[64];	// game byte val	// (_4976_5f12)
	Bit8u	glbIngameGlobVarFlags[8];	// game flags		// (_4976_5f52)
	Bit16u	glbIngameGlobVarWords[64];	// game word val	// (_4976_5f5a)
	CreatureCommandAnimation	*tlbCreaturesActionsGroupSets; // [0..471] // (_4976_5fda) Item 01-00-00 part 2 actions group sets
	CreatureAnimationFrame	*tlbCreaturesAnimationSequences; // [0..517] // (_4976_5fde)	Item 01-00-00 part 1 animations sequences
	U16		*tlbCreaturesActionsGroupOffsets; // [0..41]  // (_4976_5fe2) Item 01-00-00 part 3 groups beginning offsets
	//}}SKSAVE_DESORTED_VARS

public:
	SkWinCore();

// SPX: New procedures here
	X16	EXTENDED_LOAD_SPELLS_DEFINITION(void);
	X16 EXTENDED_LOAD_AI_DEFINITION(void);

	const char *getXActrName(int x);
	CString getSpellTypeName(U8 spelltype);
	CString getSkillName(U8 skill);
	CString getStatBonusName(U8 bonus);
	CString getUIEventName(U8 event);	// to be rewritten
	CString getAIName(U8 ai);
	CString getRecordNameOf(ObjectID recordLink);

	void printDistMap(int mapno, DistMapTile const (* const *bp1a)[1][32]);

protected:

	X16 IS_NEGATIVE(i16 xx);
	X16 ANIM_FILE_OPEN(const char *filename);
	U32 ANIM_GET_FILE_SIZE(X16 fh);
	U8 *ANIM_farmalloc(U32 size);
	U16 ANIM_READ_HUGE_FILE(X16 fh, U32 readSize, U8 *buff);
	void ANIM_FILE_CLOSE(X16 fh);

	U8 _069a_03fc(char *xx);	// ToDo Unr()

	char *ANIM_STRCPY(char *xx, const char *yy);

	void _0759_0126();
	void _0759_06c2();
	void _0759_06db();
	X16 _0759_072c();
	void _0759_071b();
	X16 _01b0_1ed2();	// Has Soundcard?
	void ANIM_farfree(U8 *buff);
	U32 ANIM_farcoreleft();

	X32 _0759_07f2(U8 *xx);		// Todo Unr()
	void _0759_0855();		// Todo Unr()
	U8 *_0759_0869(U32 xx);	// Todo Unr()

	void _0759_06b5();
	void _0759_065f();

	void _0759_0792();	// Todo Unr()
	void _0759_0739(U8 *xx, U16 yy, U16 zz);		// Todo Unr()


	void ANIM_DECODE_IMG1(U8 *xx, U8 *yy);
	void _0759_06a1(U8 ps);
	void _00eb_04bc(skxxxj *xx, U16 yy);
	void _0759_0688(skxxxj *xx, U16 yy);
	X16 ANIM_TOUPPER(X16 xx);
	void ANIM_FILL_SEQ_4BPP(U16 offDst, i16 clr, U16 cnt);
	void ANIM_SETPIXEL_SEQ_4BPP(U16 offDst, i8 clr);
	void ANIM_BLIT_TO_MEMORY_ROW_4TO4BPP(U16 offSrc, U16 offDst, U16 width);
	void IBMIO_LOAD_4TO8BPP_PAL(const U8 *pal);
	void IBMIO_BLIT_ROW_4TO8BPP(U16 offSrc, U16 offDst, U16 size);
	void _00eb_070c(U8 *buffSrc, U16 yy, U16 zz, U16 ww);
	void _0759_0310(U16 xx, U16 yy);
	void _0759_02c6(U16 xx, U16 yy, U16 zz);
	int _0759_08e7(i16 argc, char **argv, char **env);	// About anim ?

	int ANIM_BOOTSTRAP_SWOOSH();
	int ANIM_BOOTSTRAP_TITLE();

	U16 QUERY_CREATURES_ITEM_MASK(U8 cls2, U8 cls4, U8 itemflags[64], U16 isCreature);
	void ZERO_MEMORY(void *buff, U32 size);
	void PROCESS_TIMER_0E(Timer *v0, Bit16u v1);
	Bit16u ADD_ITEM_CHARGE(ObjectID recordLink, __int16 delta);
	Bit16u IS_CONTAINER_MONEYBOX(ObjectID recordLink);
	Bit16u QUERY_GDAT_DBSPEC_WORD_VALUE(ObjectID rl, Bit8u cls4);
	__int32 QUERY_ITEM_VALUE(ObjectID recordLink, Bit8u cls4);
	__int32 QUERY_ITEM_WEIGHT(ObjectID recordLink);
	Bit16u IS_CONTAINER_CHEST(ObjectID recordLink);
	void CALC_PLAYER_WEIGHT(Bit16u player);
	void PROCEED_GLOBAL_EFFECT_TIMERS();
	Bit16u _2066_1f37(ObjectID recordLink, Bit16u yy, Bit16u *zz);
	void APPEND_RECORD_TO(ObjectID recordLink_whatYouAppend, ObjectID *recordLink_newParent, __int16 xpos_newParent, __int16 ypos_newParent);

	ObjectID _2066_1ec9(ObjectID link1Dst, ObjectID link2Src);

	Creature *GET_ADDRESS_OF_RECORDX4(ObjectID recordLink);
	Door *GET_ADDRESS_OF_RECORD0(ObjectID recordLink);
	Teleporter *GET_ADDRESS_OF_RECORD1(ObjectID recordLink);
	Text *GET_ADDRESS_OF_RECORD2(ObjectID recordLink);
	Actuator *GET_ADDRESS_OF_RECORD3(ObjectID recordLink);
	Creature *GET_ADDRESS_OF_RECORD4(ObjectID recordLink);
	Weapon *GET_ADDRESS_OF_RECORD5(ObjectID recordLink);
	Cloth *GET_ADDRESS_OF_RECORD6(ObjectID recordLink);
	Scroll *GET_ADDRESS_OF_RECORD7(ObjectID recordLink);
	Potion *GET_ADDRESS_OF_RECORD8(ObjectID recordLink);
	Container *GET_ADDRESS_OF_RECORD9(ObjectID recordLink);
	Miscellaneous_item *GET_ADDRESS_OF_RECORDA(ObjectID recordLink);
	void *GET_ADDRESS_OF_RECORDB(ObjectID recordLink);
	void *GET_ADDRESS_OF_RECORDC(ObjectID recordLink);
	void *GET_ADDRESS_OF_RECORDD(ObjectID recordLink);
	Missile *GET_ADDRESS_OF_RECORDE(ObjectID recordLink);
	Cloud *GET_ADDRESS_OF_RECORDF(ObjectID recordLink);
	GenericContainerRecord *GET_ADDRESS_OF_GENERIC_CONTAINER_RECORD(ObjectID recordLink);


	Actuator *GET_ADDRESS_OF_ACTU(ObjectID recordLink);
	ObjectID GET_NEXT_RECORD_LINK(ObjectID rl);
	GenericRecord *GET_ADDRESS_OF_DETACHED_RECORD(ObjectID rl);
	GenericRecord *GET_ADDRESS_OF_RECORD(ObjectID rl);
	void CHANGE_CURRENT_MAP_TO(Bit16u new_map);
	ObjectID GET_TILE_RECORD_LINK(i16 xx, i16 yy);
	GenericRecord *GET_ADDRESS_OF_TILE_RECORD(i16 xx, i16 yy);
	Bit16u IS_TILE_PASSAGE(i16 xx, i16 yy);
	U8 GET_TILE_VALUE(i16 xx, i16 yy);
	AIDefinition *QUERY_CREATURE_AI_SPEC_FROM_TYPE(Bit8u creatureType);
	Bit16u QUERY_GDAT_CREATURE_WORD_VALUE(Bit8u creatureType, Bit8u cls4);
	//void _2066_1ea3(Bit16u xx, Bit16u yy, Bit16u zz);		// An interesting one about changing bits on tile (void/pit)
	void SET_TILE_ATTRIBUTE_02(Bit16u xx, Bit16u yy, Bit16u map); // _2066_1ea3
	sk1c9a02c3 *_1c9a_02c3(Creature *xx, AIDefinition *yy);
	Bit16u _4937_01a9(Bit16u xx, Bit16u *yy, CreatureAnimationFrame **rref);
	void GRAPHICS_DATA_OPEN();
	i16 LOCATE_OTHER_LEVEL(Bit16u curmap, i16 zDelta, i16 *xx, i16 *yy, Bit8u **ss);
	SRECT *ALLOC_TEMP_RECT(__int16 x, __int16 y, __int16 cx, __int16 cy);
	SRECT *ALLOC_TEMP_ORIGIN_RECT(Bit16u width, Bit16u height);
	Bit16u FIND_ICI_FROM_CACHE_HASH(Bit32u cacheHash, Bit16u *ici);
	void RECYCLE_MEMENTI(Bit16u mementi, Bit16u yy);
	void TEST_MEMENT(mement *bp04);
	mement *_3e74_48c9(Bit16u mementi);
	void FREE_CACHE_INDEX(Bit16u cacheIndex);
	void _3e74_4549(Bit16u xx);
	void _3e74_0c8c(mement *ref);
	void _3e74_0d32(mement *ref);
	virtual bool ValidateMements(bool display);
	void FREE_INDEXED_MEMENT(Bit16u index);
	Bit16u _3e74_4471();
	Bit16u INSERT_CACHE_HASH_AT(Bit32u cacheHash, Bit16u ici);
	void _3e74_44ad();
	Bit16u QUERY_MEMENTI_FROM(Bit16u xx);
	Bit16u ADD_CACHE_HASH(Bit32u cacheHash, Bit16u *piYaCacheIndex);
	U8 *QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(Bit16u cacheIndex);
	void _3e74_583a(Bit16u xx);
	Bit16u FIND_FREE_MEMENTI();
	void GUARANTEE_FREE_CPXHEAP_SIZE(__int32 buffSize);
	void _3e74_2b30();
	mement *ALLOC_LOWER_CPXHEAP(__int32 buffSize);
	Bit8u *ALLOC_CPXHEAP_MEM(Bit16u index, Bit32u buffSize);
	void _3e74_585a(Bit16u xx, Bit16u yy);
	void FILL_ENTIRE_PICT(Bit8u *buff, Bit16u fill);
	void FIRE_HIDE_MOUSE_CURSOR();
	void _01b0_0adb();
	void IBMIO_SET_CURSOR_PATTERN(	__int16 index, 	Bit8u *buffSrc, 	Bit8u zz, 	Bit8u ss, 	Bit16u srcWidth, 	Bit16u srcHeight,	Bit16u srcBits, 	Bit8u *localPal, 	Bit16u colorkey);
	void _01b0_0c70(Bit16u xx);
	void _01b0_0ca4(Bit16u *xx, Bit16u yy);
	void _443c_040e();
	Bit8u *ALLOC_NEW_PICT(Bit16u index, Bit16u width, Bit16u height, Bit16u bpp);
	Bit32u GET_TEMP_CACHE_HASH();
	Bit16u ALLOC_TEMP_CACHE_INDEX();
	void _0b36_0d67(sk3f6c *ref, SRECT *rc);
	sk3f6c *_0b36_0c52(sk3f6c *ref, Bit16u rectno, Bit16u ww);
	SRECT *OFFSET_RECT(const sk3f6c *refOrg, const SRECT *prcSrc, SRECT *prcOut);
	void FILL_RECT_SUMMARY(sk3f6c *ref, SRECT *rc, Bit8u fill);
	void _29ee_00a3(Bit16u xx);
	U16 IS_MISSILE_VALID_TO_LAUNCHER(Bit16u player, __int16 hand, ObjectID rlWhatYouLaunch);
	__int16 BETWEEN_VALUE(__int16 minv, __int16 newv, __int16 maxv);
	void RECALC_LIGHT_LEVEL();
	Bit16u IS_ITEM_FIT_FOR_EQUIP(ObjectID recordLink, i16 inventorySlot, Bit16u onlyBodyPart);
	Bit16u RETRIEVE_ITEM_BONUS(ObjectID x1, Bit8u x2, Bit16u x3, Bit16u x4);
	void BOOST_ATTRIBUTE(Champion *ref, Bit16u xx, Bit16u yy);
	void PROCESS_ITEM_BONUS(__int16 player, ObjectID recordLink, i16 inventorySlot, __int16 x4);	// Is that working?
	ObjectID REMOVE_POSSESSION(Bit16u player, Bit16u possess);
	void EQUIP_ITEM_TO_INVENTORY(Bit16u player, ObjectID rl, Bit16u inventorySlot);
	Bit16u _2759_0e93(Bit16u player, ObjectID rl, i16 hand);
	void LOAD_PROJECTILE_TO_HAND(Bit16u player, __int16 hand);
	Bit8u *QUERY_GDAT_ENTRY_DATA_PTR(Bit8u cls1, Bit8u cls2, Bit8u cls3, Bit8u cls4);
	void FREE_PICT6(SkImage *ref);
	__int16 QUERY_GDAT_PICT_OFFSET(Bit8u cls1, Bit8u cls2, Bit8u cls4);
	ExtendedPicture *QUERY_GDAT_SUMMARY_IMAGE(ExtendedPicture *ref, Bit8u cls1, Bit8u cls2, Bit8u cls4);
	void DRAW_DEF_PICT(ExtendedPicture *ref);
	Bit8u *QUERY_PICT_BITS(Picture *ref);
	void _0b36_11c0(ExtendedPicture *xx, sk3f6c *yy, Bit16u ss, __int16 colorkey2);
	i16 CALC_STRETCHED_SIZE(__int16 val, __int16 fact64);
	Bit8u *_0b36_00c3(__int16 xx, Picture *ref);
	Bit8u *QUERY_PICST_IMAGE(Picture *ref);
	void ALLOC_IMAGE_MEMENT(Bit8u cls1, Bit8u cls2, Bit8u cls4);
	void ALLOC_PICT_MEMENT(Picture *ref);
	Bit32u CALC_PICT_ENT_HASH(ExtendedPicture *ref);
	void FREE_IMAGE_MEMENT(Bit8u cls1, Bit8u cls2, Bit8u cls4);
	void FREE_PICT_MEMENT(Picture *ref);
	void _44c8_2307(X16 xx, X16 yy, X16 zz, X16 ww);
	void _44c8_2351(Bit8u *xx, Bit8u *yy, Bit16u ss, Bit16u tt, Bit16u uu, Bit16u vv);
	void FIRE_BLIT_TO_MEMORY_ROW_4TO4BPP(Bit16u offSrc, Bit16u offDst, Bit16u width);
	void _44c8_20e5(Bit16u srcOff, Bit16u dstOff, Bit16u srcWidth, Bit16u dstWidth);
	void _44c8_2143(Bit16u xx, Bit16u yy, Bit16u ss, Bit16u tt);
	void FIRE_STRETCH_BLIT_TO_MEMORY_4TO4BPP(Bit8u *src, Bit8u *dst, Bit16u srcWidth, Bit16u srcHeight, Bit16u dstWidth, Bit16u dstHeight, U8 *aa);
	ExtendedPicture *QUERY_PICST_IT(ExtendedPicture *ref);
	void DRAW_SIMPLE_STR(sk3f6c *ref, Bit16u rectno, Bit16u clr1, Bit16u clr2, Bit8u *str);
	void DRAW_ICON_PICT_BUFF(const Bit8u *buff, 	sk3f6c *tt, 	SRECT *rc, 	__int16 srcx, 	__int16 srcy, 	__int16 colorkey, 	__int16 flipmirror, 	Bit8u *localpal);
	void DRAW_ICON_PICT_ENTRY(Bit8u cls1, Bit8u cls2, Bit8u cls4, sk3f6c *ss, Bit16u rectno, __int16 colorkey);
	void SHOW_ATTACK_RESULT(__int16 yourValue);
	void PUT_OBJECT_INTO_CONTAINER();
	void DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
	Bit16u GET_MAX_CHARGE(ObjectID recordLink);
	Bit8u _2405_014a(ObjectID recordLink, __int16 xx, __int16 yy);
	Bit8u *TRANSLATE_PALETTE(Bit8u *localpal, Bit8u cls1, Bit8u cls2, Bit8u cls4, __int16 palentcnt);
	Bit8u *DRAW_ITEM_ON_WOOD_PANEL(Bit16u player, Bit16u possessionIndex, Picture *ref);
	void FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP(Bit16u offSrc, Bit16u offDst, Bit16u width, __int16 colorkey);
	void _44c8_1aca(Bit8u *buff, SRECT *rc, Bit16u xx, Bit16u yy);
	void _0b36_105b(sk3f6c *ref, SRECT *rc, Bit16u xx);
	void DRAW_HAND_ACTION_ICONS(Bit16u playerIndex, Bit16u possessionIndex, Bit16u leftOrRight);
	void DRAW_SQUAD_SPELL_AND_LEADER_ICON(Bit16u player, Bit16u yy);
	void __CHECK_ROOM_FOR_CONTAINER(ObjectID rl, Container *ref);
	void DRAW_MONEYBOX(ObjectID rl);
	void DRAW_CONTAINER_PANEL(ObjectID rl, Bit16u xx);
	void DRAW_SQUAD_POS_INTERFACE(); // _29ee_0396
	void DRAW_STRONG_TEXT(Bit8u *buff, Bit16u ww, Bit16u cx, Bit16u xx, Bit16u yy, Bit16u clr1, Bit16u fill, Bit8u *str);
	void DRAW_NAME_STR(sk3f6c *ref, Bit16u rectno, Bit16u clr1, Bit16u fill, Bit8u *str);
	void DRAW_PLAYER_NAME_AT_CMDSLOT();
	i16 SK_STRLEN(const Bit8u *ref);
	void DRAW_SPELL_TO_BE_CAST(Bit16u xx);
	void DRAW_SPELL_PANEL();
	Bit8u *SK_STRSTR(const Bit8u *xx, const Bit8u *yy);
	Bit16u QUERY_CMDSTR_TEXT(const Bit8u *cmdStr, const Bit8u *name);
	Bit16u QUERY_CMDSTR_ENTRY(Bit8u cls1, Bit8u cls2, Bit8u cls4, Bit16u cmdNum);
	Missile *GET_MISSILE_REF_OF_MINION(ObjectID rlCreature, ObjectID rlContainer);
	Bit16u _2759_01fe(Bit16u player, ObjectID recordLink, Bit16u cmdNum);
	__int16 FIND_POUCH_OR_SCABBARD_POSSESSION_POS(__int16 player, __int16 yy);	// yy = pouch or scabbard;
	Bit16u QUERY_PLAYER_SKILL_LV(__int16 player, Bit16u skill, Bit16u yy);
	Bit16u IS_ITEM_HAND_ACTIVABLE(Bit16u player, ObjectID recordLink, __int16 yy);
	void FIRE_QUEUE_MOUSE_EVENT(Bit16u xx, Bit16u yy, Bit16u button);
	void _1031_0b7e();
	void _1031_098e();
	Bit8u *_1031_023b(sk1891 *xx);
	void _443c_087c();
	void _443c_0889();
	void _443c_00a9(sk0cea *ref, i16 x, i16 cx, i16 y, i16 cy);
	void _443c_06b4(sk0cea *ref);
	void _443c_07d5(sk0cea *ref);
	void _1031_0541(Bit16u xx);
	void _1031_0667();
	Bit8u *QUERY_CMDSTR_NAME(Bit8u cls1, Bit8u cls2, Bit8u cls4);
	void DRAW_CMD_SLOT(Bit16u cmdSlot, Bit8u ww);
	void _29ee_0b2b();
	void _29ee_1d03(Bit16u xx);
	__int16 FIND_LADDER_AROUND(__int16 xx, __int16 yy, __int16 isupper, Actuator **ref);
	Bit16u _19f0_124b(__int16 *xx, __int16 *yy, Bit16u ww, __int16 ss, Bit16u tt);
	void _29ee_18eb(Bit16u xx, Bit16u yy, Bit16u zz);
	void CALC_VECTOR_W_DIR(__int16 dir, __int16 xx, __int16 yy, __int16 *ss, __int16 *tt);
	Bit16u IS_WALL_ORNATE_ALCOVE(Bit8u cls2);
	Bit16u _0cee_17e7(Bit16u aa, Bit16u bb, Bit16u cc);
	Bit8u _0cee_1815(i16 xx, i16 yy, Bit16u ss, Bit16u tt, Bit16u uu);
	void _0cee_185a(skxxx6 *ref, Bit16u xx, Bit16u yy, Bit16u zz, Bit16u ww, Bit16u vv, i16 ss, i16 tt);
	ObjectID _0cee_1a46(skxxx6 *ref, ObjectID recordLink, i16 xx, i16 yy);
	Bit16u QUERY_ORNATE_ANIM_FRAME(Bit8u cls1, Bit8u cls2, Bit32u tick, Bit32u delta);
	void SUMMARIZE_STONE_ROOM(skxxx6 *ref, Bit16u ww, Bit16u xx, Bit16u yy);
	__int16 QUERY_DUNGEON_MAP_CHIP_PICT(Bit8u cls1, Bit8u cls2, Bit8u **ppBuff, Bit8u *localpal);
	void DRAW_CHIP_OF_MAGIC_MAP(const Bit8u *buff, Bit16u aa, Bit16u xx, Bit16u yy, Bit16u flipMirror, Bit8u *localpal);
	__int16 _4937_000f(Bit16u xx, Bit16u *yy);
	Bit16u _0cee_2df4(ObjectID recordLink);
	Bit16u _48ae_011a(ObjectID recordLink);
	void DRAW_MAP_CHIP(ObjectID recordLink, __int16 ss, __int16 tt, __int16 ww, Bit16u xx, Bit16u yy, Bit8u cc, Bit16u flags);
	void _29ee_1946(ObjectID recordLink, __int16 xx, __int16 yy, __int16 zz, __int16 dir, __int16 ss, __int16 tt, Bit16u flags);
	void DRAW_MAJIC_MAP(ObjectID recordLink);
	void DRAW_PLAYER_ATTACK_DIR();
	void FREE_TEMP_CACHE_INDEX(Bit16u cacheIndex);
	void _0b36_0cbe(sk3f6c *ref, Bit16u yy);
	Bit16u IS_CONTAINER_MAP(ObjectID recordLink);
	void UPDATE_RIGHT_PANEL(Bit16u xx);
	void _1031_04f5();
	void HIGHLIGHT_ARROW_PANEL(Bit16u cls4, Bit16u rectno, Bit16u bright);
	void _12b4_0092();
	void IBMIO_FILL_HALFTONE_RECT(SRECT *rc);
	void FIRE_FILL_HALFTONE_RECTV(SRECT *rc, Bit16u aa);
	void FIRE_FILL_HALFTONE_RECTI(Bit16u rectno, Bit16u aa);
	void IBMIO_MOUSE_RELEASE_CAPTURE();
	void FIRE_MOUSE_RELEASE_CAPTURE();
	void _1031_050c();
	void _1031_0675(Bit16u xx);
	Bit16u RETURN_1(sk1891 *ref);
	Bit16u IS_GAME_ENDED(sk1891 *ref);
	Bit16u _1031_0023(sk1891 *ref);
	Bit16u _1031_003e(sk1891 *ref);
	Bit16u _1031_007b(sk1891 *ref);
	Bit16u _1031_009e(sk1891 *ref);
	Bit16u _1031_00c5(sk1891 *ref);
	Bit16u _1031_00f3(sk1891 *ref);
	Bit16u _1031_012d(sk1891 *ref);
	Bit16u _1031_014f(sk1891 *ref);
	Bit16u _1031_0184(sk1891 *ref);
	Bit16u _1031_01ba(sk1891 *ref);
	void _1031_027e(sk1891 *ref);
	Bit16u PT_IN_RECT(SRECT *rc, i16 xx, i16 yy);
	SRECT *_1031_01d5(Bit16u rectno, SRECT *rc);
	Bit16u _1031_0a88(sk0d9e *ref, Bit16u xx, Bit16u yy, Bit16u ww);
	sk0d9e *_1031_024c(sk1891 *ref);
	Bit16u _1031_030a(sk1891 *ref, Bit16u xx, Bit16u yy, Bit16u zz);
	Bit16u _1031_0c58(Bit16u xx, sk0d9e *ref);
	Bit16u _1031_03f2(sk1891 *ref, Bit16u xx);
	void ADJUST_UI_EVENT(MousePosition *ref);
	Bit8u *TRANSMIT_UI_EVENT(MousePosition *xx);
	void _1031_10c8(sk3f6c *ref, SRECT *rc, Bit16u cx, Bit16u cy);
	void _0b36_129a(sk3f6c *ref, i16 xx, i16 yy, Bit8u clr1, Bit8u clr2, Bit8u *str);
	void DRAW_PICST(ExtendedPicture *ref);
	void DRAW_STATIC_PIC(Bit8u cls1, Bit8u cls2, Bit8u cls4, Bit16u rectno, i16 colorkey);
	void DRAW_CHARSHEET_OPTION_ICON(Bit8u cls4, Bit16u rectno, Bit16u zz);
	void DRAW_PLAYER_3STAT_PANE(Bit16u player, Bit16u xx);
	void DRAW_CHAMPION_PICTURE(Bit16u player); // _2e62_061d
	void DRAW_PLAYER_3STAT_HEALTH_BAR(U16 player);
	void DRAW_PLAYER_DAMAGE(Bit16u player);
	void DRAW_CUR_MAX_HMS(U16 rectno, i16 curVal, i16 maxVal);
	void DRAW_PLAYER_3STAT_TEXT(Champion *ref);
	void _2405_00ec(U16 rectno, SRECT *rc);
	void _2405_011f(U16 rectno, SRECT *rc);
	void DRAW_EYE_MOUTH_COLORED_RECTANGLE(Bit8u cls4, Bit16u rectno);
	void DRAW_CRYOCELL_LEVER(Bit16u leverIsOn);
	void DRAW_POWER_STAT_BAR(i16 curval, U16 rectno, U16 color, i16 vv, U16 ww);
	i16 QUERY_FOOD_WATER_BAR_COLOR(U8 cls4, i16 def_color);
	i16 QUERY_3STAT_BAR_COLOR(U8 cls4, i16 def_color);
	void DRAW_FOOD_WATER_POISON_PANEL();
	void DRAW_GUIDED_STR(const U8 *ref);
	void _24a5_0732(i16 xx, i16 yy, U8 *str);
	void DRAW_SCROLL_TEXT(ObjectID rl);
	U8 *QUERY_GDAT_ITEM_NAME(U8 cls1, U8 cls2);
	U8 *GET_ITEM_NAME(ObjectID recordLink);
	void DRAW_ITEM_ICON(ObjectID recordLink, i16 xx, U16 yy, U16 zz, U16 ww);


	void DRAW_CONTAINER_SURVEY(Container *ref);
	U16 IS_MISCITEM_CURRENCY(ObjectID rl);
	void COUNT_BY_COIN_TYPES(ObjectID rlMoneybox, i16 *piCount);
	i16 GET_ITEM_ORDER_IN_CONTAINER(ObjectID rl, i16 xx);
	U8 *SK_LTOA10(i32 value, U8 *str);
	void MONEY_BOX_SURVEY(ObjectID recordLink);
	U16 _2759_0155(ObjectID rl);
	void DRAW_ITEM_STATS_BAR(U16 rectno, i16 curVal, i16 maxVal, U8 chr, U16 color);
	U16 DRAW_ITEM_SURVEY(ObjectID recordLink, Bit16u xx);	// quite modified
	U16 _2e62_03b5(Bit16u player, Bit16u itemNo, Bit16u yy);
	void DRAW_LOCAL_TEXT(Bit16u rectno, Bit16u clr1, Bit16u clr2, Bit8u *str);
	U16 GET_PLAYER_ABILITY(Champion *ref, U16 parm7, U16 getMax);
	void FILL_STR(Bit8u *buff, Bit16u count, Bit8u value, Bit16u delta);
	U8 *FMT_NUM(U16 value, U16 clean, U16 keta);
	U8 *SK_STRCPY(U8 *strTo, const U8 *strFrom);
	U8 *SK_STRCAT(U8 *strTo, const U8 *strFrom);

	void DRAW_SKILL_PANEL();
	void REFRESH_PLAYER_STAT_DISP(i16 player);
	void _1031_111e(Bit16u xx);
	void IBMIO_USER_INPUT_CHECK();
	void RESET_SQUAD_DIR();
	void _12b4_00af(U16 xx);
	U16 GET_ORNATE_ANIM_LEN(Actuator *ref, U16 isWall);
	void TRY_ORNATE_NOISE(Actuator *ref, ObjectID rl, U16 xx, U16 yy, U16 animLen, U16 isWall);
	void _3a15_38b6(U16 xx);
	void FILL_CAII_CUR_MAP();
	void LOAD_NEWMAP(U8 newmap);
	void PERFORM_TURN_SQUAD(Bit16u xx);
	void _2405_00e7();
	ObjectID REMOVE_OBJECT_FROM_HAND();
	void DRAW_ITEM_IN_HAND(LeaderPossession *ref);
	void _00eb_069a(SRECT *prc, i16 yy);
	void _44c8_1c99();
	void _3929_07e1(U16 xx, U16 yy);
	void _3929_0826();
	void _3929_0914();
	void PRINT_SYSERR_TEXT(Bit16u dstx, Bit16u dsty, Bit16u clr1, Bit16u clr2, Bit8u *str);
	void _3929_0929(U16 clr1, U8 *str);
	void DISPLAY_HINT_TEXT(Bit16u color, const U8 *str);
	void DISPLAY_HINT_NEW_LINE();
	void DISPLAY_TAKEN_ITEM_NAME(ObjectID rl);
	void TAKE_OBJECT(ObjectID rl, U16 xx);
	void _2e62_0cfa(U16 xx);
	void _443c_0004(U8 *buffSrc, U8 *buffDst, U8 *localpal);
	void _443c_0434();
	void CLICK_ITEM_SLOT(Bit16u xx);
	void PUT_ITEM_TO_PLAYER(Bit16u championIndex);
	ObjectID TAKE_COIN_FROM_WALLET(ObjectID rl, i16 xx);
	void CLICK_MONEYBOX(Bit16u xx);
	void SELECT_CHAMPION_LEADER(Bit16u xx);
	void ADD_ITEM_TO_PLAYER(U16 player, ObjectID rl);
	void _2f3f_04ea(Bit16u xx, Bit16u yy, Bit16u dir, Bit16u zz, Bit16u ee);
	void INTERFACE_CHAMPION(Bit16u xx); // _24a5_1798
	void INIT_BACKBUFF();
	SpellDefinition *FIND_SPELL_BY_RUNES(U8 *runes);
	ObjectID ALLOC_NEW_CREATURE(U16 creaturetype, U16 healthMultiplier_1to31_baseIs8, U16 dir, U16 xx, U16 yy);
	ObjectID CREATE_MINION(U16 creatureType, U16 healthMultiplier_1to31_baseIs8, U16 creatureDir, U16 xx, U16 yy, U16 zz, ObjectID ww, i16 dir);
	i16 FIND_HAND_WITH_EMPTY_FLASK(Champion *ref);
	void ADJUST_SKILLS(U16 player, U16 yy, U16 zz);
	void SHOOT_ITEM(ObjectID rlItemThrown, U16 xx, U16 yy, U16 dir, U16 aa, U16 energyVal, U16 ene2Val, U16 dd);
	void SHOOT_CHAMPION_MISSILE(Champion *ref, ObjectID rl, U16 yy, U16 zz, U16 ww);
	U16 CAST_CHAMPION_MISSILE_SPELL(U16 xx, ObjectID rl, i16 zz, U16 ww);
	void PROCEED_LIGHT(U16 cmdNum, U16 yy);
	U16 CALL_ENCHANTMENT_SELF(Champion *ref, i16 xx, U16 yy, U16 zz);
	void ADJUST_HAND_COOLDOWN(U16 player, U16 yy, U16 zz);
	U16 CAST_SPELL_PLAYER(U16 player, SpellDefinition *ref, U16 power);
	void DRAW_TRANSPARENT_STATIC_PIC(U8 cls1, U8 cls2, U8 cls4, U16 rectno, U16 colorkey);
	U16 GET_GLOB_VAR(U16 var);
	U16 UPDATE_GLOB_VAR(U16 var, U16 newval, U16 op);	// GLOBAL VAR
	void PROCEED_SPELL_FAILURE(U16 xx);
	Bit16u TRY_CAST_SPELL();
	void REMOVE_RUNE_FROM_TAIL();
	void ADD_RUNE_TO_TAIL(Bit16u symbol_0to5);
	U16 QUERY_CUR_CMDSTR_ENTRY(U16 cmdNum);
	U16 USE_DEXTERITY_ATTRIBUTE(U16 player);
	U16 USE_LUCK_ATTRIBUTE(Champion *ref, U16 xx);
	U16 CALC_PLAYER_ATTACK_DAMAGE(Champion *ref, U16 player, ObjectID rlEnemy, U16 xx, U16 yy, U16 valPb, U16 valDM, U16 valSk, U16 valAt);
	U16 WIELD_WEAPON(U16 player, U16 valPa, U16 xx, U16 yy, U16 valSk, U16 valAt);
	U16 CONFUSE_CREATURE(U16 ww, U16 xx, U16 yy);
	i16 STAMINA_ADJUSTED_ATTR(Champion *ref, i16 quantity);
	U16 COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(U16 xx, U16 yy, i16 zz);
	U16 _2c1d_0e23(U16 xx);
	U16 _2c1d_1de2(U16 xx, i16 yy, U16 zz);
	U16 SET_DESTINATION_OF_MINION_MAP(ObjectID rlContainer, i16 xx, i16 yy, U16 zz);
	void QUEUE_THINK_CREATURE(U16 xx, U16 yy);
	void _13e4_0360(ObjectID rlCreature, U16 xx, U16 yy, U8 ss, U16 tt);
	void RELEASE_MINION(ObjectID rlCreature);
	void TRANSFER_PLAYER(i16 xx, i16 yy, U16 zz, U16 dir);
	i16 SEARCH_DUNGEON_FOR_SPECIAL_MARKER(U16 ss, U16 tt, U16 uu, i16 *xx, i16 *yy);
	U16 ENGAGE_X_TELEPORTER();
	U16 ENGAGE_COMMAND(U16 player, i16 cmdSlot);
	Bit16u PROCEED_COMMAND_SLOT(i16 cmdSlot);
	void ACTIVATE_ACTION_HAND(Bit16u xx, Bit16u yy);
	void SET_SPELLING_CHAMPION(Bit16u iChampionNo);
	void IBMIO_MOUSE_SET_CAPTURE();
	void FIRE_MOUSE_SET_CAPTURE();
	void _443c_08ab(i16 *xx, i16 *yy, i16 *zz);

	//--- NEW FUNCTIONS -------------------------------------
	U16 QUERY_GDAT_POTION_SPELL_TYPE_FROM_RECORD(ObjectID rl);
	U16 QUERY_GDAT_POTION_BEHAVIOUR_FROM_RECORD(ObjectID rl);
	U16 QUERY_GDAT_WATER_VALUE_FROM_RECORD(ObjectID rl);
	U16 QUERY_GDAT_DOOR_IS_MIRRORED(U8 doortype);
#ifdef DM2_EXTENDED_MODE
	void PROCESS_TIMER_AMBIENT_SOUND(Timer *ref);
#endif

	ObjectID LOOKUP_DATAMAP_FOR(U16 dbType, U16 index);
	ObjectID FIND_ANY_RECORD_OF(U16 dbType);
	//--- NEW FUNCTIONS -------------------------------------


	U16 QUERY_GDAT_FOOD_VALUE_FROM_RECORD(ObjectID rl);
	U16 IS_MISCITEM_DRINK_WATER(ObjectID rlMiscItem);
	void CURE_POISON(U16 player);
	void PROCEED_ENCHANTMENT_SELF(U16 mask, U16 yy, U16 zz, U16 tick);
	void PLAYER_CONSUME_OBJECT(U16 player, ObjectID rlConsume, i16 possess);

	void CLICK_INVENTORY_EYE();
	void PUSH_PULL_RIGID_BODY(U16 arrowPos);
	U16 IS_WALL_ORNATE_ALCOVE_FROM_RECORD(ObjectID rl);
	ObjectID GET_WALL_TILE_ANYITEM_RECORD(U16 xx, U16 yy);
	U16 IS_WALL_ORNATE_SPRING(ObjectID rl);
	void REVIVE_PLAYER(X16 heroType, X16 player, X16 dir);
	X16 SELECT_CHAMPION(U16 xx, U16 yy, U16 dir, U16 zz);
	ObjectID FIND_DISTINCTIVE_ITEM_ON_TILE(U16 xx, U16 yy, i16 dir, U16 disit);
	void MOVE_RECORD_AT_WALL(U16 xx, U16 yy, U16 dir, ObjectID rlUnk, ObjectID rlWhatYouPlace);	// Big func
	U16 QUERY_RECTNO_FOR_WALL_ORNATE(i16 xx, U16 yy, U16 zz);	// _098d_0cd7 renamed QUERY_RECTNO_FOR_WALL_ORNATE
	void QUERY_TEMP_PICST(
	U16 mirrorflip, U16 horzStretch, U16 vertStretch, i16 offx, i16 offy, i16 uu, U16 rectno, U16 pp, 
	i16 colorkey1, i16 colorkey2, U8 cls1, U8 cls2, U8 cls4);

	void DRAW_TEMP_PICST();
	void MAKE_BUTTON_CLICKABLE(SRECT *prc, U8 xx, U8 yy);	// _32cb_0a4c
	void QUERY_GDAT_IMAGE_METRICS(U8 cls1, U8 cls2, U8 cls4, i16 *pcx, i16 *pcy);
	U8 SKCHR_TO_SCRIPTCHR(U8 xx);
	U8 *_0b36_037e(U8 *localpal, i8 xx, i16 colorkey1, i16 colorkey2, i16 palentcnt);
	void _32cb_0804(U8 *localpal, i16 cls4, U16 colorkey1, i16 colorkey2, U16 palentcnt);
	U16 QUERY_MULTILAYERS_PIC(ExtendedPicture *ref, U8 cls1, U8 cls2, U8 cls4, U16 horzRes, U16 vertRes, i16 zz, U16 mirrorflip, i16 colorkey1, U16 colorkey2);
	void QUERY_RAINFALL_PARAM(U8 *cls4, U16 *isMirrored);
	U8 *_32cb_0649(U8 cls1, U8 cls2, U8 cls4, i16 colorkey);
	void FIRE_UPDATE_BLIT_PALETTE(const U8 *localpal);
	void FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(U16 offSrc, U16 offDst, U16 size);
	U16 _44c8_1e1a(U8 *xx, U16 yy);
	void _44c8_1e43(U8 *src, U8 *dst, U8 *zz, SRECT *prc, U16 ss, U16 tt, U16 aa, U16 bb, U16 hazeWidth, U16 dstWidth, i16 colorkey, U8 *localpal);
	void _44c8_20a4(U8 *src, U8 *dst, U8 *zz, SRECT *prc, U16 ss, U16 tt, U16 oo, i16 pp, U8 *localpal);
	void _32cb_0c7d(ExtendedPicture *ref, U16 xx, U16 yy);
	Bit8u *QUERY_GDAT_ENTRY_DATA_BUFF(Bit8u cls1, Bit8u cls2, Bit8u cls3, Bit8u cls4);
	U8 QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(U16 actuatorData);
	ObjectID ALLOC_NEW_DBITEM_DEEPLY(U16 actuatorData);
	U16 IS_DISTINCTIVE_ITEM_ON_ACTUATOR(Actuator *ref, U16 disit, U16 onTile);
	void DRAW_TEXT_TO_BACKBUFF(i16 xx, i16 yy, U8 *str);
	U16 _48ae_05ae(i16 disit, U8 yy, U16 zz, U16 ss, U16 tt, i16 ww);
	U16 _48ae_0767(i16 xx, i16 yy, U8 *zz, i16 *vv, i16 ww);
	void _32cb_0f82(Actuator *ref, U8 cls4, i16 bb, i16 cellPos, U16 horzStretch, U16 vertStretch, U16 rectno, U16 gg, U16 colorkey1);
	void DRAW_ALCOVE_ITEMS(U16 xx);	// _32cb_3f0d
	i16 DRAW_WALL_ORNATE(i16 cellPos, i16 yy, i16 zz);	// _32cb_15b8
	i16 _0b36_1446(Picture *ref, i16 xx, i16 yy, U16 colorkey);
	U16 _32cb_00f1(U16 xx, U16 yy, i16 zz);
	U16 _32cb_0287(U16 xx, U16 yy, U16 zz);
	void CLICK_WALL(U16 xx); // _121e_0003
	U16 _32cb_35c1(i16 *xx, i16 *yy, i16 zz, i16 ww);
	i16 ROTATE_5x5_POS(i16 _5x5, U16 dir);
	i16 QUERY_OBJECT_5x5_POS(ObjectID rl, U16 reldir);
	i16 QUERY_CREATURE_BLIT_RECTI(U16 cellPos, i16 _5x5, U16 dir);
	void DRAW_ITEM(ObjectID rl, i16 xx, U16 yy, U16 zz, i16 vv, Creature *ref, U16 ww, U16 ss, U16 tt);
	ObjectID _32cb_03a6(U16 xx, U16 yy, U16 zz, U16 ww, U16 vv, ObjectID ss, U16 tt, U16 uu);
	void _121e_013a(U16 xx, U16 yy, U16 zz);
	U16 _121e_03ae(U16 aa, U16 bb, U16 xx, U16 yy, U16 cc, U16 dd, U16 ee);
	U16 _098d_02a2(U16 rectno, i16 xx, i16 yy);
	void PLAYER_TESTING_WALL(U16 ww, U16 xx, U16 yy);
	i16 _0cee_2e35(U8 cls2);
	void QUERY_CREATURE_PICST(U16 xx, i16 dist, Creature *vv, CreatureInfoData *ww, ObjectID rl);
	U16 _32cb_01b6(U16 xx, U16 yy, U16 ss, U16 tt, U16 *ww);
	U16 _121e_0222(U16 xx, U16 yy, U16 ww);
	U16 _0cee_2e09(ObjectID rl);
	U16 _2c1d_1fb1(U16 dir);
	U16 _121e_0351(U16 xx, U16 yy);
	void CLICK_VWPT(i16 xx, i16 yy);
	void CLICK_MAGICAL_MAP_AT(Bit16u ww, i16 xx, i16 yy);
	void CLICK_MAGICAL_MAP_RUNE(Bit16u rune);
	void _38c8_0060();
	void DRAW_WAKE_UP_TEXT();
	void RESUME_FROM_WAKE();
	void FILE_DELETE(const U8 *curf);
	U16 FILE_WRITE(i16 handle, U32 buffSize, const void *buff);
	i16 FILE_CREATE(const U8 *newf);
	Bit16u _2066_036e(void *buff, Bit16u buffSize);
	U16 SKSAVE_WRITE(void *buff, U16 buffSize);
	U8 SUPPRESS_WRITER(const void *pvData, const void *pvMask, U16 cbData, U32 repeat);
	Bit8u WRITE_1BIT(Bit8u nibble);
	Bit8u WRITE_RECORD_CHECKCODE(ObjectID recordLink, Bit8u writeDir, Bit8u writeFollowing);
	i16 _2066_0b44();
	U16 STORE_EXTRA_DUNGEON_DATA();
	U16 WRITE_MINION_ASSOC();
	U16 SUPPRESS_FLUSH();
	void _2066_046d(); // TODO: Unr

	void _2066_38d9();
	void __OPEN_DIALOG_PANEL(U8 cls2, U16 yy);
	void _2066_33c4(U8 *str, U16 yy);
	void _2066_398a(i16 xx);
	void _2066_3820(U8 *xx, U16 yy);

	void _3929_0b01(U16 xx, U16 yy); // TODO: Unr
	void _3929_0b20(U16 xx, U32 yy); // TODO: Unr

	void _3e74_5b7c(U16 xx);
	void _1031_0d36(U16 xx, U16 yy);
	U8 _01b0_054a(U16 xx);
	U8 _476d_05b6(U16 xx);
	void _2066_37f2();
	i16 _2066_33e7();
	void _1c9a_3bab();
	void FILL_ORPHAN_CAII();
	void RESET_CAII();
	void SET_PARTY_HERO_FLAG(U16 flagvalue);
	void _2f3f_0789();
	void FILL_U16(i16 *buff, X16 cnt, i16 val, i16 delta);
	void _19f0_045a(i16 xx, i16 yy);
	i16 _19f0_0207(i16 xx, i16 yy, i16 ss, i16 tt, U16 (SkWinCore::*hazardTest)(i16 xx, i16 yy));
	U16 _19f0_00b8(i16 xx, i16 yy);
	ObjectID _19f0_04bf();
	U16 _19f0_0559(X16 xx);
	void _075f_1bc2(U8 zz[4], U16 xx, U16 yy, i16 ss, i16 tt, X16 ww);
	i16 _2c1d_028c(i16 xx, i16 yy, X16 ww);
	U32 COMPUTE_POWER_4_WITHIN(U32 val, U16 cnt);
	U16 _1c9a_0598(U32 val);
	U16 _19f0_0891(i16 zz, X16 xx, X16 yy, i16 ss, X16 tt, X16 ww);
	X16 _19f0_0547(ObjectID rl, X16 xx);
	U16 _19f0_01d6(i16 xx, i16 yy);
	i16 _19f0_05e8(X16 aa, DistMapTile (*bb)[1][32], Ax3 *cc, i16 xx, i16 yy, i16 zz, X16 ww);
	i16 _1c9a_1b16(X16 xx, X16 yy);
	i16 _1c9a_1a48(X16 xx, X16 yy);
	X16 IS_TILE_WALL(i16 xx, i16 yy); // _19f0_0081
	X16 _19f0_1511(ObjectID rl);
	X16 CALC_CLOUD_DAMAGE(ObjectID rlCloud, ObjectID rlTarget);
	X16 _19f0_13aa(i16 xx, i16 yy);
	i16 CREATURE_GO_THERE(X16 aa, i16 xx, i16 yy, i16 ss, i16 tt, i16 ww);		// Big func
	ObjectID _19f0_050f();
	X16 CREATURE_CAN_HANDLE_IT(ObjectID rlTarget, U16 flags);	// Check it
	void _1c9a_19d4(ObjectID rl, i16 xx, i16 yy, U16 ww);
	void ADD_BACKGROUND_LIGHT_FROM_TILE(i16 aa, U16 bb, i16 xx, i16 yy, U16 ww);
	ObjectID _19f0_266c(ObjectID rl, U16 dir, U16 ww, U16 bb);
	X16 _19f0_2723(ObjectID rl, U16 ww, U16 bb, X16 cc);
	U16 _19f0_2813(U16 ww, i16 xx, i16 yy, i16 ss, i16 tt, i16 aa, U16 bb);
	U16 _19f0_0d10(U16 ww,i16 xx,i16 yy,i16 ss,i16 tt,i16 aa);
	X16 _0cee_3275(U8 xx);
	U16 _19f0_000a(i16 xx, i16 yy);
	U16 _1c9a_1bae(i16 xx, i16 yy);
	U16 FIND_WALK_PATH(i16 xx, i16 yy, U16 aa, U16 bb, i16 cc, skxxx9 *ss, WalkPath *wp);	// One very big func
	void _1c9a_0648(U16 mapno);
	void CHECK_RECOMPUTE_LIGHT(i16 xx, i16 yy);
	void __INIT_GAME_38c8_03ad();
	void END_GAME(U16 xx);
	void GAME_SAVE_MENU();

	void SHOW_CREDITS();
	U8 inportb(U16 port);
	void MessageLoop(bool fBalanceWait, bool fShortWait = false);

	i16 CALC_PLAYER_WALK_DELAY(U16 player);
	X16 IS_TILE_BLOCKED(U8 tile);
	X16 _12b4_0953(Creature *rec, U16 ww);
	X16 _12b4_0881(X16 aa, U16 oldTile, U16 newTile, i16 xx, i16 yy, ObjectID *rl);
	U16 _12b4_023f(i16 xx, i16 yy, i16 *ss, i16 *tt, i16 ww, X16 vv);
	X16 IS_CREATURE_MOVABLE_THERE(i16 xx, i16 yy, i16 dir, ObjectID *prlWhatsLying);
	X16 _12b4_099e(ObjectID rl);
	U16 PERFORM_MOVE(X16 xx);
	Bit16u HANDLE_UI_EVENT(MousePosition *ref);	// Important func
	void MAIN_LOOP(); // _1031_1e6a
	Bit16u _476d_04ed(Bit16u xx);
	sk0d9e *_1031_06b3(sk1891 *ref, X16 xx);
	void _1031_0781(Bit16u xx);
	void _4726_0383();
	Bit16u _01b0_051a();
	Bit16u _476d_05a9();

	Bit8u _476d_04e8(Bit16u xx); // TODO: Unr
	Bit16u _476d_04af(Bit16u xx); // TODO: Unr

	X16 _01b0_04e4();
	Bit16u _476d_050e();
	void _1031_06a5();
	Bit16u LOAD_NEW_DUNGEON();
	i16 SELECT_LOAD_GAME();
	void _2fcf_0b8b(Bit16u xx, Bit16u yy, Bit16u zz);
	void _443c_06af(sk0cea *ref);	// NOTHING
	void _1031_096a();
	Bit16u _482b_015c(Bit16u xx);
	void _01b0_18d3(Bit16u xx);

	void _47eb_02c3(sk5f0a *ref);
	void _47eb_00d9(sk5f0a *ref);
	void _482b_07c2(Bit16u xx);
	void EMS_FREE();
	void _farfree(void *ref);
	void __restorezero();	// ????
	void __declspec(noreturn) __terminate(Bit8u errorLevel);
	void SK_EXIT();
	void _sk_longjmp(jmp_buf xx, Bit16u yy);
	void SK_PREPARE_EXIT();
	void ACTIVATE_TICK_GENERATOR(Actuator *ref, ObjectID recordLink);
	void DRAW_DIALOGUE_PARTS_PICT(Bit8u *buffsrc, SRECT *rc, __int16 colorkey, Bit8u *localpal);
	void FREE_PICT_ENTRY(Bit8u *buff);
	void DRAW_VP_RC_STR(Bit16u rectno, Bit16u clr1, const U8 *str);
	Bit16u QUERY_MBCS_PRESENCE(const Bit8u *str);
	skxxxf *QUERY_CHAR_METRICS(U8 cls2, U8 yy, U8 chr, U8 *tableIdxOut);
	Bit16u QUERY_STR_METRICS(const Bit8u *buff, __int16 *xx, __int16 *yy);
	inline Bit16u QUERY_STR_METRICS(Bit8u *buff, Bit16u *xx, Bit16u *yy);	// INLINE
	Bit16u _3929_04e2(const Bit8u *strIn, Bit8u *strOut, Bit16u *textPos, __int16 maxWidth);
	const Bit8u *DRAW_MBCS_STR(const Bit8u *str,	Bit8u *dst, 	__int16 zz, 	Bit8u *ww, 	Bit16u clr1, 	Bit16u clr2, 	Bit16u *xx, 	Bit16u *yy, 	Bit16u dstpitch, 	Bit16u dstbpp);
	Bit8u *QUERY_FONT(i8 input, Bit8u fore, Bit8u back);
	void DRAW_STRING(Bit8u *dst, 	Bit16u ss, 	Bit16u dstpitch, 	Bit16u dstx, 	Bit16u dsty, 	Bit16u clr1, 	Bit16u clr2, 	const Bit8u *str, 	Bit16u dstbpp);
	void DRAW_VP_STR(Bit16u dstx, Bit16u dsty, Bit16u clr1, const Bit8u *str);
	void _3a15_020f();
	void COMPACT_TIMERLIST();
	U16 RAND16(U16 maxcnt);
	U16 RAND01();
	U16 RAND();
	U16 RAND02();
	Bit16u _RAND02();
	Bit16u CALC_SIZE_OF_COMPRESSED_RECT(Bit16u xx);
	void _098d_1208();
	void DEALLOC_LOWER_MEMORY(Bit32u size);
	Bit16u QUERY_GDAT_ENTRY_DATA_LENGTH(Bit8u cls1, Bit8u cls2, Bit8u cls3, Bit8u cls4);
	void LOAD_RECTS_AND_COMPRESS(Bit8u cls1, Bit8u cls2, Bit8u cls4);
	void LOAD_GDAT_ENTRY_DATA_TO(Bit8u cls1, Bit8u cls2, Bit8u cls3, Bit8u cls4, Bit8u *where);
	Bit8u *ALLOC_UPPER_MEMORY(Bit32u size);
	void COMPRESS_RECTS(__int16 *data, Bit32u size, RectTable *zz, Bit8u *(SkWinCore::*allocFunc)(Bit32u size));
	SRECT *QUERY_RECT(RectTable *entry, Bit16u rectno);
	SRECT *SET_SRECT(SRECT *prc, Bit16u x, Bit16u y, Bit16u cx, Bit16u cy);
	SRECT *UNION_RECT(SRECT *rc1, const SRECT *rc2, __int16 *offx, __int16 *offy);
	SRECT *QUERY_BLIT_RECT(Bit8u *buff, SRECT *rect, Bit16u rectno, __int16 *yourcx, __int16 *yourcy, __int16 ww);
	void DRAW_GAMELOAD_DIALOGUE_TO_SCREEN(Bit8u *buffsrc, Bit16u rectno, __int16 colorkey, Bit8u localpal[16]);
	void _01b0_0d39(__int16 *xx, __int16 *yy, __int16 *zz, Bit16u ww);
	void MOVE_MEMORY(const void *buffSrc, void *buffDst, Bit16u size);
	void MOVE_MEMORY_WITH_DELTA(Bit16u offSrc, Bit16u offDst, Bit16u size);
	void COPY_MEMORY_WITH_DELTA_AND_TRANSPARENT(Bit16u offSrc, Bit16u offDst, Bit16u size, Bit8u colorkey);
	void MOVE_MEMORY_STRETCH_13TO16(Bit16u offSrc, Bit16u offDst);
	void _00eb_0845(Bit8u *buff, SRECT *rc, Bit16u ww);
	SRECT *SCALE_RECT(Bit16u rectno, SRECT *rc, Bit16u horzResolution, Bit16u vertResolution);
	SRECT *QUERY_EXPANDED_RECT(Bit16u rectno, SRECT *rc);
	void CHANGE_VIEWPORT_TO_INVENTORY(Bit16u xx);
	void _0aaf_002f();
	Bit16u max_value(__int16 v1, __int16 v2);
	Bit16u min_value(__int16 v1, __int16 v2);
	void FIRE_WAIT_VSYNC();
	void WAIT_SCREEN_REFRESH();
	RawEntry *QUERY_GDAT_ENTRYPTR(Bit8u cls1, Bit16u cls2, Bit8u cls3, Bit16u cls4);

	Bit16u QUERY_GDAT_ENTRY_IF_LOADABLE(Bit8u cls1, Bit8u cls2, Bit8u cls3, Bit8u cls4);
	Bit16u CALC_IMAGE_BYTE_LENGTH(Bit8u *buff);
	void IBMIO_WAIT_VSYNC();
	void IBMIO_UPDATE_PALETTE_SET();
	void IBMIO_SELECT_PALETTE_SET(Bit8u number);
	void FIRE_SELECT_PALETTE_SET(Bit8u number);
	void IBMIO_FILL_SCREEN_LINE(Bit16u offDst, Bit16u fill, Bit16u size);
	void IBMIO_FILL_RECT_SCREEN(SRECT *rc, Bit16u fill);
	void FIRE_FILL_4BPP_PICT_LINE(Bit16u offDst, Bit16u fill, Bit16u size);
	void FIRE_FILL_RECT_4BPP_PICT(Bit8u *buff, SRECT *rc, Bit16u fill, Bit16u width);
	void FIRE_FILL_8BPP_PICT_LINE(Bit16u offDst, Bit16u fill, Bit16u size);
	void FIRE_FILL_RECT_8BPP_PICT(Bit8u *buff, SRECT *rc, Bit16u fill, Bit16u width);
	void FIRE_FILL_RECT_ANY(Bit8u *buff, SRECT *rc, Bit16u fill, Bit16u width, Bit16u bpp);
	void FIRE_FILL_SCREEN_RECT(Bit16u rectno, Bit8u fill);
	void FIRE_FADE_SCREEN(Bit16u fadeOutIfTrue);
	void FIRE_FILL_BACKBUFF_RECT(SRECT *rc, Bit8u fill);
	SRECT *SET_ORIGIN_RECT(SRECT *rc, __int16 width, __int16 height);
	SRECT *INFLATE_RECT(SRECT *rc, __int16 halfWidth, __int16 halfHeight);
	SRECT *CALC_CENTERED_RECT_IN_RECT(SRECT *rcNew, const SRECT *rcBBox, Bit16u newWidth, Bit16u newHeight);
	void QUERY_TOPLEFT_OF_RECT(Bit16u rectno, __int16 *xpos, __int16 *ypos);
	void SLEEP_SEVERAL_TIME(Bit16u count);
	void _0aaf_01db(Bit16u rectno, Bit16u yy);
	Bit8u _0aaf_02f8(Bit8u xx, Bit8u yy);
	void GRAPHICS_DATA_CLOSE();
	void PROCESS_ACTUATOR_TICK_GENERATOR();
	Bit16u _RAND();
	Bit16u GET_CREATURE_ANIMATION_FRAME(Bit8u ct, Bit16u command, Bit16u *pw08, Bit16u *pw0a, CreatureAnimationFrame **animframe, Bit16u vv);	

	void _1c9a_09db(ObjectID recordLink);
	void ARRANGE_DUNGEON();
	i16 FILE_OPEN(const U8 *filename);
	U16 FILE_READ(Bit16u handle, Bit32u buffSize, void *buff);
	void FILE_CLOSE(Bit16u handle);
	void FILE_RENAME(const U8 *curf, const U8 *newf);

	void IBMIO_BLIT_TO_SCREEN_xTO8BPP(const void *src,	SRECT *rc,	__int16 srcx,	__int16 srcy,	__int16 srcPitch,	__int16 colorkey,	const void *localPal,	__int16 srcBpp	);
	void FIRE_BLIT_TO_MEMORY_4TO4BPP(const void *src,	void *dst,	SRECT *rc,	__int16 srcx,	__int16 srcy,	__int16 srcPitch,	__int16 dstPitch,	__int16 colorkey,	__int16 mirrorFlip	);
	void FIRE_BLIT_TO_MEMORY_4TO8BPP(const void *src,	void *dst,	SRECT *rc,	__int16 srcx,	__int16 srcy,	__int16 srcPitch,	__int16 dstPitch,	__int16 colorkey,	__int16 mirrorFlip,	const U8 *localPal	);
	void _44c8_0b8d(U16 src, U16 dst, U16 pitch);

	void _44c8_0bc5(U16 src, U16 dst, U16 pitch, U8 ck); // TODO: Unr
	void _44c8_0bf8(U16 src, U16 dst, U16 pitch); // TODO: Unr
	void _44c8_0c3c(U16 src, U16 dst, U16 pitch, U8 ck); // TODO: Unr


	void FIRE_BLIT_TO_MEMORY_8TO8BPP(const void *src,	void *dst,	SRECT *rc,	__int16 srcx,	__int16 srcy,	__int16 srcPitch,	__int16 dstPitch,	__int16 colorkey,	__int16 mirrorFlip	);	
	void _44c8_0f29(const void *src,	void *dst,	SRECT *rc,	__int16 srcx,	__int16 srcy,	__int16 srcPitch,	__int16 dstPitch,	__int16 colorkey,	__int16 mirrorFlip,	const void *localPal	);		// TODO: Unr

	void FIRE_BLIT_PICTURE(const void *src,	void *dst,	SRECT *rc,	U16 srcx,	U16 srcy,	U16 srcPitch,	U16 dstPitch,	__int16 colorkey,	U16 mirrorFlip,	U16 srcBpp,	U16 dstBpp,	const U8 *localPal	);	
	void IBMIO_BLIT_TO_SCREEN_8TO8BPP(void *src, void *dst,	SRECT *prc,	__int16 srcx,	__int16 srcy,	__int16 srcpitch,	__int16 dstpitch,	__int16 colorkey);

	Bit16u _476d_030a(Bit16u xx); // TODO: Unr
	__int16 GET_PLAYER_AT_POSITION(Bit16u position);
	void EMS_MAP_BANK_TO_MEMORY(); // TODO: Unr
	Bit8u *EMS_MAP_BANK_TO_MEMORY(shelf_memory info, Bit16u physPage0to3); // TODO: Unr
	Bit8u *FIND_FREE_POOL(i32 size, Bit16u poolno);
	U8 *ALLOC_MEMORY_RAM(i32 size, Bit16u flags, Bit16u poolno);
	shelf_memory ALLOC_MEMORY_EMS(i32 size, Bit16u flags, Bit16u poolno);
	tiamat ALLOC_MEMORY_(i32 size, Bit16u flags, Bit16u poolno);
	Bit8u *ALLOC_PICT_BUFF(Bit16u srccx, Bit16u srccy, Bit16u flags, Bit16u bpp);
	void COPY_MEMORY(const void *buffSrc, void *buffDst, Bit32u buffSize);
	U16 QUERY_GDAT_ENTRY_DATA_INDEX(U8 cls1, U16 cls2, U8 cls3, U16 cls4);
	Bit8u *REALIZE_GRAPHICS_DATA_MEMORY(shelf_memory info);
	Bit16u QUERY_GDAT_RAW_DATA_LENGTH(Bit16u index);	// * check rewrite
	Bit16u IS_CLS1_CRITICAL_FOR_LOAD(Bit8u cls1);

	i32 QUERY_GDAT_RAW_DATA_FILE_POS(Bit16u index);

	U16 FILE_SEEK(Bit16u handle, Bit32u pos);
	U32 FILE_TELL(Bit16u handle);
	void _476d_05e3(U8 *str); // TODO: Unr
	void __declspec(noreturn) RAISE_SYSERR(Bit16u syserr);
	void GRAPHICS_DATA_READ(Bit32u location, Bit32u size, Bit8u *buff);
	void LOAD_GDAT_RAW_DATA(Bit16u index, shelf_memory ps);
	shelf_memory CONVERT_PHYS_TO_SHELF_FORM(Bit8u *buff);
	Bit8u *QUERY_GDAT_DYN_BUFF(Bit16u index, Bit16u *yy, Bit16u allocUpper);
	Bit8u *QUERY_GDAT_IMAGE_LOCALPAL(Bit8u cls1, Bit8u cls2, Bit8u cls4);
	U8 *_3e74_0245(X16 xx, X16 yy);
	X8 READ_IMG3_NIBBLE();
	U16 READ_IMG3_DURATION();
	void SPILL_IMG3_PIXELS(X16 offDst, X16 offSrc, X16 size);
	void TRANSPARENT_IMG3_PIXELS(X16 offSrcDst, X16 size);
	void _44c8_1202(X16 offDst, X8 pixel);
	void DECODE_IMG3_OVERLAY(U8 *blitUnderlay, IMG3 *blitSrc, U8 *blitDst);
	void DECODE_IMG9(IMG3 *xx, U8 *pOut);
	void DECODE_IMG3_UNDERLAY(IMG3 *xx, U8 *yy);
	void DECODE_IMG3_UNDERLAY_LOCAL(IMG3 *xx, U8 *yy);
	Bit8u *EXTRACT_GDAT_IMAGE(Bit16u index, i16 allocUpper);
	void DUMP_5CA4();
	Bit8u *QUERY_GDAT_IMAGE_ENTRY_BUFF(Bit8u cls1, Bit8u cls2, Bit8u cls4);
	void DRAW_DIALOGUE_PICT(Bit8u *srcImage, Bit8u *dstImage, SRECT *rect, Bit16u srcx, Bit16u srcy, Bit16u colorkey, Bit8u localpal[16]);
	Bit8u *QUERY_GDAT_SQUAD_ICON(Bit8u *dstImage, Bit8u colorno, Bit8u localpal[16]);
	void DEALLOC_UPPER_MEMORY(Bit32u size);
	void FREE_PICT_BUFF(Bit8u *buff);
	void LOCK_MOUSE_EVENT();
	X16 _443c_00f8(X16 xx, X16 yy);
	Bit16u FIRE_MOUSE_EVENT_RECEIVER(Bit16u xx, Bit16u yy, i16 button);
	void IBMIO_BLIT_MOUSE_CURSOR(Bit8u *buff, SRECT *rc, Bit16u srcx, Bit16u srcy, Bit16u srcpitch, __int16 colorkey);
	void _01b0_05ae();
	void IBMIO_HIDE_MOUSE_CURSOR();
	void _01b0_073d();
	void IBMIO_MOUSE_EVENT_RECEIVER(Bit16u cursorx, Bit16u cursory, Bit16u buttons);
	void UNLOCK_MOUSE_EVENT();
	void _01b0_0aa8();
	void FIRE_SHOW_MOUSE_CURSOR();
	void CHANGE_PLAYER_POS(Bit16u squadPos);
	void CHAMPION_SQUAD_RECOMPUTE_POSITION();
	void _38c8_0002();
	Bit8u *QUERY_GDAT_TEXT(Bit8u cls1, Bit8u cls2, Bit8u cls4, Bit8u *buff);
	Bit8u _0aaf_0067(Bit8u cls2);
	Bit16u _2066_03e0(Bit16u xx);
	void SUPPRESS_INIT();
	void REARRANGE_TIMERLIST();
	void __SORT_TIMERS();
	Bit8u *FORMAT_SKSTR(const Bit8u *format, Bit8u *output);	// Z00* replacement
	U16 _2066_033c(void *buff, int size);
	int SKLOAD_READ(void *buff, int size);
	void _3a15_0002();
	void DECIDE_DEFAULT_DUNGEON_MAP_CHIP_SET();
	int READ_DUNGEON_STRUCTURE(X16 isNewGame);
	void CHECK_TILE_RECORDS();
	int SUPPRESS_READER(void *_data, const void *_mask, Bit16u buffSize, Bit32u repeat, Bit16u fill);
	int READ_1BIT(Bit16u *pw);
	void ADD_MINION_ASSOC(ObjectID recordLink);
	int READ_RECORD_CHECKCODE(__int16 xpos, __int16 ypos, ObjectID *recordLinkPtr, Bit16u readDir, Bit16u readSub);
	U8 _0cee_0897(Teleporter **ref, i16 xx, i16 yy);
	U8 GET_TELEPORTER_DETAIL(TELE_inf *ref, i16 xx, i16 yy);
	Bit16u _2066_197c();
	Bit16u RECOVER_MINION_ASSOC();
	U16 READ_SKSAVE_DUNGEON();
	__int16 GAME_LOAD();	// GAME LOAD
	ObjectID GET_OBJECT_INDEX_FROM_TILE(i16 xx, i16 yy);
	void CUT_RECORD_FROM(ObjectID recordLink, ObjectID *recordLinkLookFor, __int16 xposLookFor, __int16 yposLookFor);
	Bit8u QUERY_CLS2_OF_TEXT_RECORD(ObjectID recordLink);
	Bit8u QUERY_CLS2_FROM_RECORD(ObjectID recordLink);
	Bit16u GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(Bit16u actuatorData);
	Bit8u GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(Bit16u actuatorData);
	Bit16u QUERY_CREATURE_AI_SPEC_FLAGS(ObjectID rl);
	AIDefinition *QUERY_CREATURE_AI_SPEC_FROM_RECORD(ObjectID rlCreature);
	Bit8u QUERY_CLS1_FROM_RECORD(ObjectID recordLink);
	tiamat _3e74_0756(Bit8u *xx, i32 size); // TODO: Unr
	Bit16u TRY_PUSH_OBJECT_TO(ObjectID rl, i16 xpos, i16 ypos, i16 *xx, i16 *yy);
	X16 GET_CHAMPION_SPECIAL_FORCE(U16 player);
	Bit16u GET_PARTY_SPECIAL_FORCE();
	void _2fcf_01c5(ObjectID recordLink, i16 xx, i16 yy, Bit16u curmap, Bit16u ss);
	X16 ATTACK_PARTY(Bit16u quantity, Bit16u yy, Bit16u zz);
	void PROCESS_POISON(i16 player, Bit16u yy);
	Bit16u _0cee_06dc(i16 xx, i16 yy);
	void ATTACK_CREATURE(ObjectID rl, i16 xx, i16 yy, Bit16u ss, i16 tt, Bit16u quantity);
	U8 *PREPARE_LOCAL_CREATURE_VAR(ObjectID rl, i16 xx, i16 yy, U16 timerType);
	void CREATURE_THINK_FLUSH_POSITION(); // _14cd_0802
	X32 CREATURE_SOMETHING_1c9a_0a48();
	void UNPREPARE_LOCAL_CREATURE_VAR(U8 *ww);
	void ALLOC_CAII_TO_CREATURE(ObjectID rl, i16 xx, i16 yy);
	X16 _0cee_319e(ObjectID rl);
	U16 ATTACK_WALL(i16 xTo, i16 yTo, i16 xFrm, i16 yFrm, U16 dirTo, ObjectID rlThrown);
	i16 QUERY_DOOR_DAMAGE_RESIST(U8 cls2);
	U16 ATTACK_DOOR(i16 x, i16 y, U16 damage, U16 isSpellAttack, U16 delay);
	X16 _2c1d_132c(i16 ss, U16 tt);
	X16 _2c1d_135d(i16 play, U16 ww);
	i16 _0cd5_0176(U16 val, U16 rshift, U16 multiplier);
	i16 _2c1d_0f2a(Champion *ref, X16 ability, i16 tt);

	Bit16u WOUND_PLAYER(i16 play, i16 quantity, Bit16u ss, Bit16u tt);
	void _075f_0182(ObjectID rl, X16 xx, X16 yy);
	void CREATE_CLOUD(ObjectID rl, U16 ww, U16 xx, U16 yy, U16 ss);
	void _12b4_0d75(i16 xx, i16 yy, i16 ss, i16 tt);
	U16 QUERY_CREATURE_5x5_POS(Creature *ref, U16 dir);
	void _098d_000f(i16 xx, i16 yy, U16 ww, U16 *x2, U16 *y2);
	void _29ee_000f();
	U16 _1c9a_08bd(Creature *ref);

	U16 IS_CREATURE_FLOATING(ObjectID rl);
	Bit16u IS_OBJECT_FLOATING(ObjectID rl);
	void MARK_DYN_LOAD(U32 aa);
	void MARK_DYN_EXCLUDE_RANGE(U32 ent4, U8 cls4);
	void _2676_006a(U32 aa);
	void _2676_00d0(U8 cls1, U8 cls2, U8 cls4);
	U16 IS_SCARD_BUSY();
	void _01b0_1983();
	void _47eb_02d3(U32 aa);
	void _482b_060e();
	X16 QUERY_NEXT_GDAT_ENTRY(SkEntIter *ref);
	SkEnt4 MAKE_ENT4(U8 cls1, U8 cls2, U8 cls3, U8 cls4);
	void _482b_0624(U8 cls1, U8 cls2, U8 cls4);
	U16 TRACK_UNDERLAY(U16 ww);
	X16 _3e74_00ed(sk5d12 *ref, shelf_memory ps);
	tiamat s2t(shelf_memory s);
	shelf_memory t2s(tiamat t);
	U8 *t2ptr(tiamat t);
	tiamat ptr2t(U8 *pb);
	void _3e74_3200(X16 ww); // TODO: Unr
	void MAP_EMS_TO_PTR(U32 xx, U32 yy, U8 **ss, U8 **tt);
	U8 *_4726_02ac();
	void _3e74_2f41(sk5d12 *ref);
	void _3e74_30bd(sk5d12 *ref);
	X32 _3e74_0017(sk5d12 *ref);

	i32 _3e74_0081(sk5d12 *ref); // TODO: Unr
	tiamat _3e74_32fb(sk5d12 *ss, U8 *tt, i32 ww); // TODO: Unr


	void DRAW_DIALOGUE_PROGRESS(X32 xx);
	shelf_memory _3e74_5133(U8 cls1, U8 cls2, U8 cls3, U8 cls4);
	void _47eb_00a4(sk5f0a *ref);
	void  _482b_0684();
	tiamat _3e74_32a2(sk5d12 *ref, i32 xx);
	void LOAD_DYN4(SkLoadEnt *ref, i16 aa);	// big func
	void LOAD_MISCITEM();
	void SET_TIMER_WEATHER(U32 tickDelta);
	void _3df7_0037(X16 ww);
	X16 RETRIEVE_ENVIRONMENT_CMD_CD_FW(DistantEnvironment *ref);
	X16 _098d_04c7(X16 rcno1, X16 rcno2, X16 scale, X16 *ss, X16 *tt);
	void UPDATE_WEATHER(U16 aa);
	void LOAD_LOCALLEVEL_DYN();	//*	interesting stuff in there
	void CHANCE_TABLE_OPERATION();
	void _32cb_4069(i16 xx, i16 yy, X16 ww, U16 *ss, U16 *tt);
	void _32cb_4185(i16 xx, i16 yy, i16 cellPos, i16 dir);
	void TRIM_BLIT_RECT(i16 xx, i16 yy, i16 ss, i16 tt);
	X16 SET_GRAPHICS_FLIP_FROM_POSITION(X16 aa, X16 ww, X16 xx, X16 yy);
	void DRAW_DUNGEON_GRAPHIC(U8 cls1, U8 cls2, U8 cls4, X16 rectno, i16 colorkey, U16 mirrorflip);
	void _098d_0c45();
	void ENVIRONMENT_DRAW_DISTANT_ELEMENT(DistantEnvironment *ref, X16 dir, X16 xx, X16 yy);
	i16 _32cb_54ce(X16 dir, i16 *xx, i16 *yy, X16 xl, X16 yl);
	X16 ENVIRONMENT_SET_DISTANT_ELEMENT(DistantEnvironment *ref, U8 *str, X16 dir, X16 xx, X16 yy);
	void ENVIRONMENT_DISPLAY_ELEMENTS(X16 dir, X16 xx, X16 yy);
	void _32cb_5c67();
	void _32cb_5a8f();
	U8 _48ae_01af(X16 xx, X16 yy);
	void MAKE_PUT_DOWN_ITEM_CLICKABLE_ZONE(X16 xx, ObjectID rl, i16 yy, X16 zz);
	void DRAW_PUT_DOWN_ITEM(ObjectID rl, i16 cellPos, i16 dir, Creature *ref);
	X16 DIR_FROM_5x5_POS(X16 _5x5);
	void _32cb_3e08(ObjectID rl, X16 xx, U32 yy, Creature *ref);
	void SUMMARY_DRAW_CREATURE(ObjectID rl, i16 cellPos, U32 ss);
	void _32cb_2cf3(U8 cls2, U16 scale64, U16 mirrorFlip, U16 rectno);
	void _32cb_2d8c(ObjectID rl, X16 yy, X32 aa);
	void _32cb_3edd(i16 xx);
	void DRAW_DEFAULT_DOOR_BUTTON(U8 cls1, U8 cls2, U8 cls4, i16 ww);
	void DRAW_DOOR_FRAMES(i16 xx, X16 yy);
	X16 _32cb_48d5(U16 xx, i16 yy);
	void DRAW_DOOR(i16 xx, X16 yy, X16 zz, X32 aa);
	void DRAW_DOOR_TILE(i16 xx);
	void DRAW_STAIRS_SIDE(i16 xx);
	void DRAW_STAIRS_FRONT(i16 xx);
	void DRAW_PIT_TILE(i16 xx);
	X16 DRAW_EXTERNAL_TILE(i16 xx);
	void DRAW_PIT_ROOF(i16 xx);
	void DRAW_FLYING_ITEM(ObjectID rl, i16 cellPos, X16 _5x5);
	void DRAW_STATIC_OBJECT(i16 xx, X32 ss, X16 ww);
	void DRAW_TELEPORTER_TILE(i16 xx, X16 cls1, X16 cls2);
	void DRAW_WALL(i16 xx);
	void DRAW_WALL_TILE(i16 xx);
	void DRAW_DUNGEON_TILES();
	void COPY_SKIMAGE_HEADER(const U8 *xx, U8 *yy);
	void APPLY_MIRROR_FLIP_TO(const U8 *blitSrc, U8 *blitDst, X16 mirrorFlip);
	void DRAW_MIRRORED_PIC(const U8 *blitSrc, U8 *blitDst);
	void DRAW_RAIN();
	void DRAW_PLAYER_TILE();
	void DISPLAY_VIEWPORT(Bit16u dir, i16 xx, i16 yy);
	void ADJUST_STAMINA(Bit16u player, i16 drain);
	Bit16u GET_PLAYER_WEIGHT(Bit16u player);
	Bit16u MAX_LOAD(Champion *ref);
	void _482b_05bf(Bit16u xx);
	X32 _3e74_5673(X32 cacheHash, U16 *piYaCacheIndex, X16 ifTryInsert);
	void _1c9a_0247(ObjectID rl);
	void QUERY_MESSAGE_TEXT(U8 *str, ObjectID rl, Bit16u ww);
	Bit16u IS_REBIRTH_ALTAR(ObjectID rl);
	X16 _2fcf_164e(ObjectID xx, ObjectID yy);
	Bit16u _2fcf_16ff(ObjectID rl);
	Bit16u GET_TIMER_NEW_INDEX(Bit16u xx);
	void BLEND_TO_SNDBUFF_TANDY(void *ref, Bit16u xx, Bit16u yy, Bit16u zz); // TODO: Unr
	void _01b0_0ec3(X16 xx); // TODO: Unr
	void BLEND_TO_SNDBUFF_GENERAL(U8 *buff, Bit16u buffSize, Bit16u volume, Bit16u caller, Bit16u ss, Bit16u tt);
	void BLEND_TO_SNDBUFF_SNDCARD_4(void *ref, Bit16u xx, Bit16u yy, Bit16u zz); // TODO: Unr
	Bit16u _47eb_02e0(SoundStructure *xx, SoundStructure *yy);
	Bit8u *_47eb_0048(shelf_memory info, Bit16u physPage0to3);
	X16 _0cee_317f(ObjectID rl);
	X16 _19f0_2024(ObjectID rl, i16 ss, i16 tt);
	i16 _19f0_2165(X16 aa, i16 xx, i16 yy, i16 ss, i16 tt, i16 vv, i16 ww);	
	ObjectID *OVERSEE_RECORD(ObjectID *ref, Bit8u dir, ObjectID **recordMatched, U16 (SkWinCore::*pfnFinder)(ObjectID *ref, void *pvUser), void *pvUser, Bit16u alsoCreaturePossessions, Bit16u alsoContainedObjects);
	int _3a15_0381(Timer *xx, Timer *yy);
	void _3a15_0486(Bit16u xx);
	Bit16u QUEUE_TIMER(Timer *ref);
	void _01b0_1c8d(Bit16u xx);
	Bit16u IBMIO_SBLASTER_BLEND_TO_SNDBUFF(Bit8u *buff, Bit16u buffSize, __int8 volume, Bit16u caller);
	Bit16u IBMIO_BLEND_TO_SNDBUFF(Bit8u *buff, Bit16u buffSize, Bit8u volume, Bit16u caller);
	void _47eb_0333(SoundStructure *ref);
	Bit16u PLAY_SOUND(Bit16u xx, SoundStructure *ref);
	Bit16u QUERY_SND_ENTRY_INDEX(Bit8u cls1, Bit8u cls2, Bit8u cls4);
	void QUEUE_NOISE_GEN1(Bit8u cls1, Bit8u cls2, Bit8u cls4, Bit8u xx, Bit8u yy, __int16 xpos, __int16 ypos, __int16 tickDelta);
	void QUEUE_NOISE_GEN2(Bit8u cls1, Bit8u cls2, Bit8u cls4, Bit8u cls2alt, __int16 xpos, __int16 ypos, Bit16u tickDelta, Bit8u ss, Bit8u tt);
	void DELETE_TIMER(Bit16u xx);
	void INVOKE_MESSAGE(__int16 xpos, __int16 ypos, Bit16u dir, Bit16u actionType, Bit32u tick);
	void SET_ITEMTYPE(ObjectID recordLink, Bit8u itemType);
	ObjectID ALLOC_NEW_DBITEM(Bit16u itemspec);
	Bit8u GET_WALL_DECORATION_OF_ACTUATOR(Actuator *ref);	
	Bit8u GET_FLOOR_DECORATION_OF_ACTUATOR(Actuator *ref);
	Bit16u GET_DISTINCTIVE_ITEMTYPE(ObjectID recordLink);
	void INVOKE_ACTUATOR(Actuator *ref, Bit16u actionType, __int16 delayPlus);
	U16 _1c9a_0694(ObjectID *ref, void *pv);	
	ObjectID *_1c9a_06bd(ObjectID recordLink, Bit16u ss, Bit16u dir);
	void _2fcf_2444(__int16 xpos, __int16 ypos, ObjectID recordLink, Bit16u ss, Bit16u tt, Bit16u uu);
	void DELETE_CREATURE_RECORD(__int16 xpos, __int16 ypos, Bit16u dropMode, Bit16u tt);
	void DROP_CREATURE_POSSESSION(ObjectID recordLink, __int16 xx, __int16 yy, Bit16u dropMode, __int16 tt);
	ObjectID ROTATE_RECORD_BY_TELEPORTER(Teleporter *ref, ObjectID recordLink);
	void ROTATE_CREATURE(ObjectID recordLink, Bit16u rotationType, Bit16u rotation);
	void ROTATE_SQUAD(Bit16u dir);
	Bit16u _2fcf_0434(ObjectID recordLink, __int16 xpos, __int16 ypos, __int16 xx, __int16 yy, Bit16u zz);	// big funct
	ObjectID _1c9a_03cf(__int16 *xx, __int16 *yy, Bit16u dir);
	__int16 ABS16(__int16 val);
	Bit16u CALC_VECTOR_DIR(__int16 x1, __int16 y1, __int16 x2, __int16 y2);
	Bit16u CALC_SQUARE_DISTANCE(__int16 x1, __int16 y1, __int16 x2, __int16 y2);
	Bit16u _4937_005c(Bit16u xx, Bit16u *yy);
	CreatureAnimationFrame *_4937_0036(Bit16u xx, Bit16u *yy);
	Bit16u APPLY_CREATURE_POISON_RESISTANCE(ObjectID recordLink, Bit16u xx);
	void PROCESS_QUEUED_DEALLOC_RECORD();
	void QUEUE_DEALLOC_RECORD(ObjectID recordLink);
	void DEALLOC_RECORD(ObjectID recordLink);
	Bit16u _075f_06bd(Missile *ref, ObjectID recordLink);
	Bit16u _1c9a_0958(ObjectID recordLink);
	Bit16u _RAND01();
	Bit16u GET_DOOR_STAT_0X10(Bit8u cls2);
	Bit8u GET_GRAPHICS_FOR_DOOR(Door *ref);
	Bit16u _075f_0af9(__int16 u16tileType, __int16 xpos, __int16 ypos, Bit16u dir, ObjectID rlMissile);
	void _2fcf_0234(__int16 xposFrom, __int16 yposFrom, __int16 xposTo, __int16 yposTo);
	void SET_MINION_RECENT_OPEN_DOOR_LOCATION(ObjectID recordLink, __int16 xpos, __int16 ypos, Bit16u curmap, Bit16u ww);
	void RELEASE_CREATURE_TIMER(ObjectID recordLink);
	Bit16u _RAND16(Bit16u maxcnt);
	Bit16u GET_CREATURE_WEIGHT(ObjectID recordLink);
	void _1c9a_0fcb(Bit16u xx);
	void LOAD_LOCALLEVEL_GRAPHICS_TABLE(Bit16u curmap);
	ObjectID GET_CREATURE_AT(__int16 xpos, __int16 ypos);
	Bit16u IS_CREATURE_ALLOWED_ON_LEVEL(ObjectID rlCreature, Bit16u curmap);

	Bit16u MOVE_RECORD_TO(ObjectID rlWhatYouMove, __int16 xposFrom, __int16 yposFrom, __int16 xposTo, __int16 yposTo);

	void _075f_056c(ObjectID rl);
	void DELETE_MISSILE_RECORD(ObjectID rlMissile, ObjectID *prlDropTo, __int16 xposDropTo, __int16 yposDropTo);
	ObjectID RECYCLE_A_RECORD_FROM_THE_WORLD(Bit16u itemdb, Bit8u itemtype);
	ObjectID ALLOC_NEW_RECORD(Bit16u db);
	X16 _069a_035b(X16 xx);
	X16 _sk_setjmp(jmp_buf xx);
	i16 _01b0_20ef();
	void (SkWinCore::*_crt_getvect(U16 interruptno))();
	//void *_crt_getvect(U16 interruptno)();
	void _00eb_0bc4();

	X16 _01b0_2b1b();	// RETURNS 0

	X16 _01b0_0e80(void (SkWinCore::*pfn)());

	void _4726_03b2();

	U32 _crt_farcoreleft(); // TODO: Unr
	U32 _crt_coreleft();  // TODO: Unr
	i16 _crt_setblock(U16 segx, U16 newsize);  // TODO: Unr
	U16 _crt_normalize(U8 *cp);  // TODO: Unr
	U8 *_crt_brk(U32 addr);  // TODO: Unr
	U8 *_crt_CreateHeap(U16 ax);  // TODO: Unr
	U8 *_crt_farmalloc(U32 size);  // TODO: Unr




	void INIT_FREE_POOL(sk5d00 *ref, i32 size, U16 poolflag);
	sk5d00 *_3e74_059e(X16 xx);
	U16 ADD_MEM_TO_FREE_POOL(sk5d00 *pool, i32 size, X16 poolflag);
	U32 EMS_ALLOC_POOL(U8 **buff);
	void _4726_02f7();
	X16 _01b0_1ffc(X16 xx);	// Returns 1 and assert false ...
	X16 _476d_02e0(X16 xx);
	void _01b0_20ca(i16 drvno, U8 *str);
	void _476d_04f4(i16 drvno, U8 *str);
	void _476d_018a();
	void _2636_03d4();
	U32 GET_FILE_POS_6(i16 handle);
	U32 GET_FILE_SIZE(i16 handle);
	U16 SWAPW(U16 xx);
	U32 QUERY_GDAT_ENTRY_VALUE(U16 entryIndex, U16 entryPos);
	void LOAD_GDAT_ENTRIES();
	X16 _3e74_2162(U16 xx);	// RETURNS 1!!
	X16 LANG_FILTER(U16 entryIndex);
	void BUILD_GDAT_ENTRY_DATA(GDATEntries *ref, X16 (SkWinCore::*pfnIfLoad)(U16 xx), U8 *zz);
	void LOAD_ENT1();
	void _3e74_24b8();
	void READ_GRAPHICS_STRUCTURE();
	X16 IS_SCARD_PRESENT();
	X16 _RELOAD_SOUND_BUFFER(U8 *buff, X16 buffSize, X16 playbackRate);
	void _47eb_0003();
	void _482b_0004();
	void LOAD_GDAT_INTERFACE_00_0A();
	void LOAD_GDAT_INTERFACE_00_02();
	void LOAD_GDAT_INTERFACE_00_00();
	void _38c8_00c8();
	void KANJI_FONT_LOAD(X8 cls2);

	void _470a_0003();	// NOTHING??
	void _3929_0e16();
	void _2405_0009();
	void _01b0_08b6(U16 (SkWinCore::*pfn)(U16 xx, U16 yy, i16 zz));
	void _443c_0380();	// IBMIO
	void _443c_067a(sk0cea *ref);
	void _1031_07d6();
	U8 _3e74_2439(X8 cls1, X8 cls4);
	void _2481_0002();
	void _3e74_0a77(U8 *ref);
	void SHOW_MENU_SCREEN();
	void __LOAD_CREATURE_FROM_DUNGEON();
	void _3e74_07b2();
	void _3e74_0820();
	X16 _38c8_0109(X8 **buff, X32 *xx, X16 *yy);
	void _38c8_0104();
	void INIT_CPXHEAP(sk5d12 *ref, tiamat poolBuff, U32 poolSize, U16 poolflag);
	X16 _38c8_0224(X16 xx, i32 yy);
	void _3e74_1330(X8 *xx);
	X16 _00eb_05c7(U8 (*pal)[4], X16 yy);
	void _44c8_1baf(U8 *pal);
	void _38c8_04aa();
	X16 IS_TIMER_TO_PROCEED();
	void GET_AND_DELETE_NEXT_TIMER(Timer *ref);
	X16 _1c9a_17c7(U8 xx, U8 yy, U8 zz);
	X16 WOUND_CREATURE(i16 damage);
	X8 _14cd_062e();
	i16 SELECT_CREATURE_4EFE(const sk4efe *ref);
	void SELECT_CREATURE_37FC();
	void __SET_CURRENT_THINKING_CREATURE_WALK_PATH();
	i16 CREATURE_THINK_381c(); // _1c9a_381c
	U8 *_3e74_5788(U16 xx, i32 yy);
	void _14cd_0276(skxxx9 *ref);
	void _14cd_0f0a(U8 func, U8 xx, U8 yy, sk1bf9 *ss);
	void _14cd_0f3c(i8 aa, sk1bf9 *ss, sk1bf9 *tt, X8 ww, i8 vv, Ax3 uu, U8 xx, U8 yy);
	void _14cd_18cc(U8 xx, U8 yy, sk1bf9 *ss);
	ObjectID CREATURE_CAN_HANDLE_ITEM_IN(i16 flags, ObjectID rlFindFrom, i8 dir);
	U16 _2c1d_09d9();
	X16 CREATURE_THINK_1316(U8 xx, X16 yy, U8 zz); // _14cd_1316
	void _14cd_18f2(i8 xx, U8 yy, sk1bf9 *ss, X8 ww, Ax3 vv);
	void _14cd_19a4(U8 xx, U8 yy, sk1bf9 *ss);
	X16 _14cd_102e(X16 ww, ObjectID rl, i8 dir, X16 alsoPossession, X16 alsoContainer);
	sk4f04 *_14cd_10d2(sk1bf9 *ss, i8 ww);
	void _14cd_19c2(U8 xx, U8 yy, sk1bf9 *ss, i8 vv, i8 ww);
	void _14cd_1a3c(U8 xx, U8 yy, sk1bf9 *ss);
	void _14cd_1a5a(U8 xx, U8 yy, sk1bf9 *ss);
	void _14cd_1a78(U8 xx, U8 yy, sk1bf9 *ss, X8 ww);

	void _14cd_1b74(U8 xx, U8 yy, sk1bf9 *ss);
	void _14cd_1b90(U8 xx, U8 yy, sk1bf9 *ss);
	void _14cd_1c27(U8 xx, U8 yy, sk1bf9 *ss);
	void _14cd_1bac(U8 xx, U8 yy, sk1bf9 *ss, X8 vv, X8 ww);
	void _14cd_1c45(U8 xx, U8 yy, sk1bf9 *ss);

	void _14cd_1c63(U8 xx, U8 yy, sk1bf9 *ss);
	void _14cd_1c8d(U8 xx, U8 yy, sk1bf9 *ss);
	void _14cd_1cec(U8 xx, U8 yy, sk1bf9 *ss);
	void _14cd_1d42(U8 xx, U8 yy, sk1bf9 *ss);
	void _14cd_1d6c(U8 xx, U8 yy, sk1bf9 *ss, i8 ww);

	void _14cd_1e36(U8 xx, U8 yy, sk1bf9 *ss);
	void _14cd_1e52(U8 xx, U8 yy, sk1bf9 *ss); // TODO: Unr
	U16 IS_MAP_INSIDE(U16 mapno);
	void _14cd_1e6e(U8 xx, U8 yy, sk1bf9 *ss);
	void _14cd_1eec(U8 xx, U8 yy, sk1bf9 *ss, X8 ww);
	void _14cd_1fa7(U8 xx, U8 yy, sk1bf9 *ss);
	void _14cd_1f8b(U8 xx, U8 yy, sk1bf9 *ss);
	X16 _1c9a_38a8();
	X8 CREATURE_THINK_0389(); // _14cd_0389
	void _14cd_0550(skxxxh *ref, i8 xx, i8 yy, X16 ww);
	void _14cd_0457();
	i8 SELECT_CREATURE_3672();
	i8 DECIDE_NEXT_XACT();
	X8 PROCEED_XACT_56();
	void PROCEED_XACT_57();
	X8 PROCEED_XACT_59_76();
	ObjectID FIND_TILE_ACTUATOR(X16 xx, X16 yy, i8 dir, X16 actuatorType);
	X8 PROCEED_XACT_62();
	X8 PROCEED_XACT_63();
	X8 PROCEED_XACT_64();
	X8 PROCEED_XACT_65();
	X16 _14cd_2662(i8 dir);
	X8 PROCEED_XACT_66();
	U16 _14cd_2807(ObjectID *ref, skxxxi *pv);
	i16 _14cd_2886(ObjectID *ref, X16 xx, i8 dir, X16 ss, X16 tt, X16 ww);
	X8 PROCEED_XACT_67();
	X8 PROCEED_XACT_68();
	void PROCEED_XACT_69();
	void _1c9a_078b(ObjectID *ref, X16 xx, i8 dir);
	X8 PROCEED_XACT_71();
	X8 PROCEED_XACT_73();
	i8 PROCEED_XACT_74();
	X8 PROCEED_XACT_75();
	X8 PROCEED_XACT_77();
	X8 PROCEED_XACT_78();
	void PROCEED_XACT_79();
	X8 PROCEED_XACT_80();
	X8 PROCEED_XACT_81();
	X16 EXCHANGE_MONEYBOX(X16 onlyTest, ObjectID rlWallet);
	X16 ADD_COIN_TO_WALLET(ObjectID rlMoneyBox, ObjectID rlCoin);
	X8 PROCEED_XACT_82();
	i8 PROCEED_XACT_83();
	X8 PROCEED_XACT_84();
	i8 PROCEED_XACT_85();
	X8 PROCEED_XACT_86();
	void PROCEED_XACT_72_87_88();
	X8 PROCEED_XACT_89();
	X8 PROCEED_XACT_90();
	X8 PROCEED_XACT_91();
	X8 PROCEED_XACT_70();
	i8 PROCEED_XACT(i8 xact);
	X16 CREATURE_THINK_08F5(i8 xx);	// _14cd_08f5
	void CREATURE_THINK_09E2(); // _14cd_09e2
	void _13e4_01a3();
	void ADVANCE_TILES_TIME(X16 xx, X16 yy);
	X16 OPERATE_PIT_TELE_TILE(X16 xx, X16 yy, X16 ww);
	U16 CREATURE_WALK_NOW();
	X16 CREATURE_CCM03();
	X16 CREATURE_JUMPS();
	void CREATURE_CCM06();
	i16 CREATURE_ATTACKS_PLAYER(Creature *ref, U16 player);
	i16 CREATURE_ATTACKS_CREATURE(X16 xx, X16 yy);
	X16 CREATURE_ATTACKS_PARTY();
	X16 CREATURE_CAST_SPELL();
	U16 QUERY_ITEM_MONEY_VALUE(ObjectID xx);
	X16 CREATURE_STEAL_FROM_CHAMPION();
	X16 CREATURE_CCM0B();
	X16 CREATURE_CCM0C();
	X16 CREATURE_TAKES_ITEM();
	X16 CREATURE_SHOOT_ITEM();
	X16 CREATURE_PUTS_DOWN_ITEM();
	void CREATURE_KILL_ON_TIMER_POSITION();
	X16 CREATURE_ROTATES_TARGET_CREATURE();
	X16 PLACE_MERCHANDISE();
	X16 TAKE_MERCHANDISE();
	X16 CREATURE_ACTIVATES_WALL();
	X16 CREATURE_USES_LADDER_HOLE();
	void CREATURE_TRANSFORM();
	X16 CREATURE_EXPLODE_OR_SUMMON();
	U16 PROCEED_CCM();
	X16 _4937_028a(U16 xx, U16 *yy, CreatureAnimationFrame **ref);
	void CREATURE_THINK_0982(); // _13e4_0982
	void _13e4_071b();
	void _13e4_0806();
	void THINK_CREATURE(X8 xx, X8 yy, X16 timerType);
	void STEP_MISSILE(Timer *ref);
	X16 QUERY_DOOR_STRENGTH(X8 cls2);
	void STEP_DOOR(Timer *ref);
	void PROCESS_CLOUD(Timer *ref);
	void PROCESS_TIMER_DESTROY_DOOR(Timer *ref);
	X16 _3a15_1da8(X8 xx, X8 yy);
	void ACTIVATE_RELAY1(Timer *ref, Actuator *pr4, X16 delayAsMult);
	void ACTIVATE_RELAY2(Timer *ref, Actuator *pr4, X16 xx);
	void _3a15_0d5c(Timer *ref, Actuator *pr4); // TODO: Unr
	void ACTIVATE_ORNATE_ANIMATOR(Timer *ref, Actuator *pr4, ObjectID rl, X16 isWall);
	void ACTIVATE_CONTINUOUS_ORNATE_ANIMATOR(ObjectID rl, Timer *ref, Actuator *pr4, X16 isWall);
	void _2c1d_153f(X16 xx);
	void BRING_CHAMPION_TO_LIFE(X16 xx);
	void ACTIVATE_ITEM_TELEPORT(Timer *ref, Actuator *pr4, ObjectID rl, X16 isFloor, X16 recycler, ObjectID *prl, X16 capture, X16 onlyFirstItem);
	void SET_ITEM_IMPORTANCE(ObjectID rlItem, X16 isImportant);
	void MOVE_ITEM_TO(ObjectID rl, Actuator *pr4, i16 xposFrm, i16 yposFrm, i16 xposTo, i16 yposTo, X16 dir, X16 takeFrmCreature, X16 putToCreature);
	void ACTIVATE_INVERSE_FLAG(Timer *ref, Actuator *pr4); 
	void ACTIVATE_TEST_FLAG(Timer *ref, Actuator *pr4);
	void ACTIVATE_SHOOTER(Actuator *pr4, Timer *ref);
	void ACTUATE_WALL_MECHA(Timer *ref);
	X16 _1c9a_09b9(ObjectID rlCreature, X16 xx);
	void ACTIVATE_CREATURE_KILLER(X16 argLo, X16 argHi, X16 srcx, X16 srcy, X16 tarx, X16 tary, X16 actuatorType, X16 actionType); 
	void ANIMATE_CREATURE(X16 xx, X16 yy, X16 ww);
	void ACTUATE_FLOOR_MECHA(Timer *ref);
	void ACTUATE_PITFALL(Timer *ref); 
	void ACTUATE_DOOR(Timer *ref);
	void ACTUATE_TELEPORTER(Timer *ref);
	void ACTUATE_TRICKWALL(Timer *ref);
	void PROCESS_TIMER_3D(Timer *ref);
	void PROCESS_SOUND(U8 xx);
	void PROCESS_TIMER_0C(U8 player);
	void PROCESS_TIMER_LIGHT(Timer *ref);
	void PROCESS_TIMER_RESURRECTION(Timer *ref);
	void CONTINUE_ORNATE_ANIMATOR(Timer *ref);
	void CONTINUE_TICK_GENERATOR(Timer *ref);
	void PROCESS_TIMER_RELEASE_DOOR_BUTTON(Timer *ref);
	void PROCESS_TIMER_59(Timer *ref);
	void CONTINUE_ORNATE_NOISE(Timer *ref);
	void PROCEED_TIMERS();
	void DROP_PLAYER_ITEMS(U16 player);
	void CHAMPION_DEFEATED(X16 player);
	void _3a15_05f7(X16 xx);
	void PROCESS_PLAYERS_DAMAGE();
	void UPDATE_CHAMPIONS_STATS();
	void GLOBAL_UPDATE_UNKNOW1();
	void BURN_PLAYER_LIGHTING_ITEMS();
	void _44c8_1dfc(SRECT *prc, U8 colorkey);
	void _3929_086f();
	X16 _0cee_04e5(X16 xx, X16 yy, X16 zz, i16 ss, i16 tt);
	void _2759_12e6();
	void GAME_LOOP();
	i16 FIRE_MAIN(i16 argc, char **argv, char **env);
public:
	int FIRE_BOOTSTRAP();
//protected:
	i16 SK88_TOUPPER(i16 c);
	U8 *SK88_STRCPY(U8 *strDestination, const U8 *strSource);
	U8 *SK88_STRCAT(U8 *strDestination, const U8 *strSource);
	i16 IBMIO_EXEC(const U8 *exe, const U8 *arg);
	void IBMIO_PRINT_ERROR(const U8 *str);
	void _0088_020b(X16 xx); // TODO: Unr
	U16 IBMIO_CHECK_CPU_ERA();

	void _sys_setvect(i16 interruptno, void (SkWinCore::*isr)());
	void (SkWinCore::*_sys_getvect(U16 interruptno))();
	void _crt88_setvect(i16 interruptno, void (SkWinCore::*isr)());

	U16 IBMIO_DETECT_SBLASTER();
	void IBMIO_INIT_VID();
	X16 IBMIO_INIT_MOUSE();
	void _04bf_0090(); // TODO: Unr
	void _01b0_08d8();
	void IBMIO_SET_MOUSE_HANDLER();
	void IBMIO_MOUSE_HANDLER();
	void INIT_KBOARD_HOOK();
	void IBMIO_KBOARD_HANDLER();
	void _01b0_00ce(X16 xx);
	void _01b0_00fc();
	void _01b0_18ae(); // TODO: Unr
	X16 _01b0_292b(); // TODO: Unr
	void _01b0_237f(); // TODO: Unr
	void _01b0_20ff();
	U16 _01b0_0e2c();
	X16 _01b0_0e27(X16 xx);
	void _01b0_0fa3();
	void _01b0_180c();
	void _INT08_HANDLER();
	void IBMIO_INIT_TIMER();
	void IBMIO_UNINIT_TIMER();
	void IBMIO_UNINIT_MOUSE();
	void IBMIO_UNINIT_KBOARD_HOOK();
	i16 IBMIO_UNINIT_VID();
	void zeroMem(void *pv, size_t cb);
	void _04bf_0102();
	i16	IBMIO_MAIN(i16 argc, char **argv, char **env);
	int IBMIO_BOOTSTRAP();
	void _sys_intr();

};

#endif // _SKWINCORE_H_

//------------------------------------------------------------------------------