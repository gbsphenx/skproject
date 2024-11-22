#ifndef _SKGLOBAL_H_
#define _SKGLOBAL_H_

//------------------------------------------------------------------------------

#include <SkVer.h>

//------------------------------------------------------------------------------

#if defined(__MINGW__) || defined(__DJGPP__) || defined(__LINUX__)
#include <stdio.h> // for FILE
#endif

#include <DME.h>

using namespace DMEncyclopaedia;
using namespace DM2Internal;

//------------------------------------------------------------------------------


#if defined(_USE_MFC80) || defined(_USE_MFC60)
#include <SkMFC.h>
#include <resource.h>
#endif // defined(_USE_MFC80) || defined(_USE_MFC60)


#ifdef _USE_SDL
#include <SkSDL.h>
#endif // _USE_SDL

#if defined(_USE_MFC80) || defined(_USE_MFC60)
extern CSkWinMFC skwin;
#endif

#ifdef _USE_SDL
extern CSkWinSDL skwin;
#endif

#if defined(__DJGPP__) || defined(__MINGW__)
#include <SKDOS.H>
extern CSkWinDOS skwin;
#endif

#if defined(__LINUX__)
#include <SKSDL2.H>
extern CSkWinSDL2 skwin;
#endif


// In case of non WIN
#if defined(__MINGW__) || defined(__DJGPP__) || defined(__LINUX__)
#ifndef LPCSTR
#define LPCSTR const char*
#endif // LPCSTR

#ifndef CString
#define CString const char*
#endif // CString
#endif // __DJGPP__


//------------------------------------------------------------------------------
// CONSOLE COLORS / ATTRIBUTES

#define RESET		0
#define BRIGHT 		1
#define DIM			2
#define UNDERLINE 	3
#define CONSOLE_BLINK		4
#define REVERSE		7
#define HIDDEN		8

#if (defined (__MINGW__))
#define BLACK 			0
#define RED				1
#define GREEN			2
#define GOLD			3
#define YELLOW			3
#define BLUE			4		
#define PURPLE			5
#define AQUA			6
#define LIGHT_GRAY		7

#define GRAY			8
#define LIGHT_RED		9
#define LIGHT_GREEN		10
#define LIGHT_YELLOW	11
#define LIGHT_BLUE		12
#define LIGHT_PURPLE	13
#define CYAN			14
#define WHITE			15

#elif (defined (__DJGPP__))

#define BLACK 			0
#define BLUE			4		
#define GREEN			2
#define AQUA			6
#define RED				1
#define PURPLE			5
#define GOLD			3
	#define YELLOW			3
#define LIGHT_GRAY		7
#define GRAY			0
#define LIGHT_BLUE		4
#define LIGHT_GREEN		2
#define CYAN			6
#define LIGHT_RED		1
#define LIGHT_PURPLE	5
#define LIGHT_YELLOW	3
#define WHITE			7

//#elif (defined (__LINUX__) || (__CYGWIN__))
//#elif (defined (__LINUX__) || (__MINGW__))
#elif (defined (__LINUX__))

#define BLACK 			0
#define BLUE			4		
#define GREEN			2
#define AQUA			6
#define RED				1
#define PURPLE			5
#define GOLD			3
	#define YELLOW			3
#define LIGHT_GRAY		7
#define GRAY			0
#define LIGHT_BLUE		4
#define LIGHT_GREEN		2
#define CYAN			6
#define LIGHT_RED		1
#define LIGHT_PURPLE	5
#define LIGHT_YELLOW	3
#define WHITE			7

#elif (defined (WIN32))
//#elif (defined (__WINDOWS__))

#define BLACK 			0
#define BLUE			1		
#define GREEN			2
#define AQUA			3
#define RED				4
#define PURPLE			5
#define GOLD			6
	#define YELLOW			6
#define LIGHT_GRAY		7
#define GRAY			8
#define LIGHT_BLUE		9
#define LIGHT_GREEN		10
#define CYAN			11
#define LIGHT_RED		12
#define LIGHT_PURPLE	13
#define LIGHT_YELLOW	14
#define WHITE			15

#endif

//------------------------------------------------------------------------------
//	SPELLS and RUNES
//------------------------------------------------------------------------------

// SPX: no surprise, this is static, but can be increased with no difficulty
#define MAXSPELL_ORIGINAL	34
#define MAXSPELL_CUSTOM		255
// SPX: This MAXSPELL is used only in FIND_SPELL_BY_RUNES, so it is just for the table below.

#define SPELL_TYPE_POTION	1
#define SPELL_TYPE_MISSILE	2
#define SPELL_TYPE_GENERAL	3
#define SPELL_TYPE_SUMMON	4

extern SpellDefinition dSpellsTable[];			// Original table
extern SpellDefinition dSpellsTableCustom[];	// Custom table loaded from GDAT

#define MkssymVal(S2,S3,S4) ((S4) | ((S3) << 8) | ((S2) << 16))
// SPX: That bring to this table:
//	66: YA
//	67: VI
//	68: OH
//	69: FUL
//	6A: DES
//	6B:	ZO

//	6C:	VEN
//	6D: EW 
//	6E:	KATH
//	6F: IR
//	70: BRO
//	71:	GOR

//	72: KU
//	73:	ROS
//	74: DAIN
//	75:	NETA
//	76: RA
//	77: SAR

#define RUNE_LO		0x60
	#define RUNE_FIRST	RUNE_LO
#define RUNE_UM		0x61
#define RUNE_ON		0x62
#define RUNE_EE		0x63
#define RUNE_PAL	0x64
#define RUNE_MON	0x65

#define RUNE_YA		0x66
#define RUNE_VI		0x67
#define RUNE_OH		0x68
#define RUNE_FUL	0x69
#define RUNE_DES	0x6A
#define RUNE_ZO		0x6B

#define RUNE_VEN	0x6C
#define RUNE_EW		0x6D
#define RUNE_KATH	0x6E
#define RUNE_IR		0x6F
#define RUNE_BRO	0x70
#define RUNE_GOR	0x71

#define RUNE_KU		0x72
#define RUNE_ROS	0x73
#define RUNE_DAIN	0x74
#define RUNE_NETA	0x75
#define RUNE_RA		0x76
#define RUNE_SAR	0x77

//------------------------------------------------------------------------------
//	SPELL EFFECTS / MISSILE & CLOUD
//------------------------------------------------------------------------------

extern const ObjectID oFF80;
extern const ObjectID oFF81;
extern const ObjectID oFF82;
extern const ObjectID oFF83;
extern const ObjectID oFF84;
extern const ObjectID oFF86;
extern const ObjectID oFF87;
extern const ObjectID oFF89;
extern const ObjectID oFF8A;
extern const ObjectID oFF8D;
extern const ObjectID oFF8E;
extern const ObjectID oFFA8;
extern const ObjectID oFFB0;
extern const ObjectID oFFB1;
extern const ObjectID oFFE4;
extern const ObjectID oFFFE;
extern const ObjectID oFFFF;

// SPX BEGIN
#define OBJECT_EFFECT_FIREBALL			oFF80
//#define OBJECT_EFFECT_MYSTERIOUS		oFF81
#define OBJECT_EFFECT_POISON_BLOB		oFF81	// This spell in DM1 was for Poison Blob
#define OBJECT_EFFECT_LIGHTNING			oFF82
#define OBJECT_EFFECT_DISPELL			oFF83
#define OBJECT_EFFECT_ZO_SPELL			oFF84
//#define OBJECT_EFFECT_FUSE			oFF85	// Added, originally not in DM2
#define OBJECT_EFFECT_POISON_BOLT		oFF86
#define OBJECT_EFFECT_POISON_CLOUD		oFF87
#define OBJECT_EFFECT_PUSH				oFF89
#define OBJECT_EFFECT_PULL				oFF8A
#define OBJECT_EFFECT_ZO_2				oFF8D
#define OBJECT_EFFECT_REFLECTOR			oFF8E
#define OBJECT_EFFECT_CLOUD				oFFA8
#define OBJECT_EFFECT_THUNDER			oFFB0
#define OBJECT_EFFECT_ELECTRIC_SHOCK	oFFB1
#define OBJECT_EFFECT_CLOUD_REBIRTH		oFFE4	// Finally, this is special for Resurrection explosion from VI ALTAR

#if (DM2_EXTENDED_OBJECT_DEF == 0)
	#define OBJECT_END_MARKER				oFFFE
	#define OBJECT_NULL						oFFFF
	#define OID_T							U16
	#define OID_SIZE_BITSHIFT				1			// to get number of bytes, multiply 2 (shift << 1)

//#elif (DM2_EXTENDED_OBJECT_DEF == 1)
#else
	#define OBJECT_END_MARKER				0xFFFFFFFE
	#define OBJECT_NULL						0xFFFFFFFF
	#define OID_T							U32
	#define OID_SIZE_BITSHIFT				2			// to get number of bytes, multiply 4 (shift << 2)
#endif // DM2_EXTENDED_OBJECT_DEF


#define POTION_TYPE_VEN_BOMB			0x03
#define	POTION_TYPE_FUL_BOMB			0x13
// SPX END

//------------------------------------------------------------------------------
//	AI
//------------------------------------------------------------------------------

// SPX: there, I added a new table for holding correctly AI def.
#if DM2_EXTENDED_MODE == 0
	#define MAXAI 62
#else
	#define MAXAI 255
#endif

extern AIDefinition dAITable[];



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


#define CALL_IBMIO 
#define INDIRECT_CALL
#define LOADDS(X)
#define ENTER(X)

//------------------------------------------------------------------------------

// 4976:3B13  A7 3A 76 49 00 00 00 00 AB 3A 76 49 AF 3A 76 49  ..vI......vI..vI
// 4976:3B23  B7 3A 76 49 D7 3A 76 49 DB 3A 76 49 DF 3A 76 49  ..vI..vI..vI..vI
// 4976:3B33  E3 3A 76 49 E7 3A 76 49 F7 3A 76 49 00 00 00 00  ..vI..vI..vI....
// 4976:3B43  00 00 00 00 00 00 00 00 FF 3A 76 49 07 3B 76 49  ..........vI..vI

//{{SKSTRTBL
extern const void *_4976_3aa7;
extern const void *_4976_3aab;
extern const void *_4976_3aaf;
extern const void *_4976_3ab7;
extern const void *_4976_3ac7;
extern const void *_4976_3ad7;
extern const void *_4976_3adb;
extern const void *_4976_3adf;
extern const void *_4976_3ae3;
extern const void *_4976_3ae7;
extern const void *_4976_3aef;
extern const void *_4976_3af7;
extern const void *_4976_3afb;
extern const void *_4976_3aff;
extern const void *_4976_3b07;
extern const void *_4976_3b0b;

//#4976:3B13
extern const void *_4976_3b13[16];



extern const Bit8u * const _4976_395a;
extern const Bit8u * const _4976_3956;
extern const Bit8u * const _4976_3992;
extern const Bit8u * const _4976_3a97;
extern const Bit8u * const _4976_3a9d;
extern const Bit8u * const strZT_EndOfString;
extern const Bit8u * const strZT_F;
extern const Bit8u * const strZT_G;
extern const Bit8u * const strZT_strData1;
extern const Bit8u * const strZT_strData2;
extern const Bit8u * const strZT_strData3;
extern const Bit8u * const strZT_strData4;
extern const Bit8u * const strZT_strData5;
extern const Bit8u * const strZxxxTable[];
extern const Bit8u * const strSKSave_dat;
extern const Bit8u * const strSKSave_bak;
extern const Bit8u * const strDungeon_ftl;
extern const Bit8u * const strGraphics;
extern const Bit8u * const strGraphics2;
extern const Bit8u * const _4976_3b68;
extern const Bit8u * const _4976_4853;
extern const Bit8u * const _4976_485a;
extern const Bit8u * const strPC286Required;
extern const Bit8u * const _04bf_067a;
extern const Bit8u * const strDungenB;
extern const Bit8u * const strDungeon;
extern const Bit8u * const strVersionNumber;
extern const Bit8u * const strSlash;
extern char strDirLetter[3];
extern char strDirLetter2[3];
//}}SKSTRTBL

//{{SKSTRREF
extern const Bit8u * const ptrDirLetter;
extern const Bit8u * const ptrDirLetter2;
extern const Bit8u * const ptrDungenB;
extern const Bit8u * const ptrSKSave_dat;
extern const Bit8u * const ptrSKSave_bak;
extern const Bit8u * const ptrDungeon_ftl;
extern const Bit8u * const ptrGraphics;
extern const Bit8u * const ptrGraphics2;
extern const Bit8u * const ptrDungeonFilename;
//}}SKSTRREF



//{{SKCONST

extern const Bit8u glbItemSizePerDB[16];
extern const Bit8u glbByte01[1];
extern const Bit8u glbGDAT2CatPerDB[16];
extern const Bit8u _4976_0c5e[0x2c];
extern const i16 glbXAxisDelta[4];
extern const i16 glbYAxisDelta[4];
extern const Bit16u glbActivationItemRangePerDB[16];
extern const Bit16u _4976_49c0[];
extern const char ItemCommandsTable[18][3];
extern const i8  _4976_3fc2[4];
extern const i16 _4976_3fb8[5];
extern const Bit8u _4976_3fa0[4];
extern const Bit8u _4976_3fa4[4];
extern const i8 _4976_3fa8[16];
extern const Bit8u * const strEndLine;


extern const Bit8u RuneManaPower[24];
extern const Bit8u RunePowerMultiplicator[30] ;
extern const char _4976_0362[32][2];
extern const char _4976_0262[32][8];
extern const i8 _4976_40e8[23][2];
extern const i8 glbTabXAxisDistance[23];	// (_4976_4116) x-axis distance (viewport)
extern const char EnvCM_CD[];
extern const char EnvCM_FW[];
extern const char EnvCM_CD2[];
extern const char EnvCM_FW2[];
extern const char EnvCM_MV[];
extern const char EnvCM_XLocation[];
extern const char EnvCM_YLocation[];
extern const char EnvCM_FD[];

extern const U8 _4976_4813[7];
extern const U8 tSkillLevelNameGDAT2Entry[15];
extern const U16 _4976_46f4_tblGfxSet[6];	// _4976_46f4
extern const U16 _4976_4700_tblGfxSet[6]; // _4976_4700
extern const U16 _4976_470c_tblGfxSet[6]; // _4976_470c
extern const U16 _4976_4718_tblGfxSet[6]; // _4976_4718
extern const Bit8u * const _4976_4760;
extern const U8 _4976_4a04[4];
extern const U16 tblAIStats01[42];			// _4976_3752 renamed tblAIStats01
extern const i8 glbTabYAxisDistance[23];	// (_4976_412d) y-axis distance (viewport)
extern const U8 tlbDisplayOrderLeft[25];
extern const U8 tlbDisplayOrderRight[25];
extern const U8 tlbDisplayOrderCenter[25];
extern const char * const _089c_00d8;
extern const U8 tlbPullPushArrow[6];	// _4976_435a
extern const U16 tlbPullPushArrowRectno[6];	// _4976_434e
extern const U8 tlbPullPushObjectMoveDirs[6];	// _4976_4360
extern const U8 tlbPullPushPlayerMoveDirs[6];	// _4976_4366
extern const i8 _4976_0154[32][2];

extern const i8 _4976_41d0[];
extern const i8 _4976_41d7[];
extern const U8 _4976_44bd[];
extern const U8 _4976_3d7d[];

//}}SKCONST













//{{SKVAL1

extern U16 tlbUIKeyInput[10];

// "Rr" ... 変数項の占有メモリ領域を確認し，RCJを付与済み
// "Rr`" ... "Rr"中，RCJを部分的に施した
// "Rp" ... 変数項の占有メモリは追跡しないことを決定し，RCJを付与しない

extern i8 _4976_0094[32]; //Rr
extern U8 _4976_00b4[8]; //Rr  // flags for 8 missiles. 2=can harm door
extern X8 _4976_00e3[5]; //Rr
extern U16 tRectnoOffsetsWallOrnates[16]; //Rr	(_4976_019a)

extern i8 _4976_19b2[4]; //Rr
extern i8 _4976_19b6[4]; //Rr

extern X16 _4976_38e8[4]; //Rr
extern U8 _4976_3932[28]; //Rr
//extern U8 _4976_3b6f[5]; //Rr
extern X16 _4976_3d5b[9]; //Rr
extern X16 _4976_3d6d[6]; //Rr
extern U8 _4976_3e1e[4]; //Rr
extern U8 _4976_3fc8[6]; //Rr

extern i8 _4976_406c[4][4]; //Rp
extern i8 _4976_407c[4][4]; //Rp
extern i8 _4976_408c[21][4]; //Rr
extern i8 tlbDoorSideFramesReorder[23]; //Rr
extern i8 _4976_415b[23]; //Rr
extern i8 _4976_4172[23]; //Rr
extern U8 tlbDistanceStretch[5]; //Rr (_4976_4189) // distance to creature scale factor
extern U8 _4976_418e[6][4]; //Rr`
extern U8 _4976_41a9[7]; //Rr
extern U8 _4976_41b0[16][2]; //Rr
extern i8 _4976_41de[8]; //Rr
extern U8 _4976_41e6[7]; //Rr
extern U8 _4976_41ed[17]; //Rr
extern U8 tlbRainScene[5][4]; //Rr	// _4976_41fe
extern i8 _4976_421b[6]; //Rr
extern i8 _4976_4221[5]; //Rr
extern i8 _4976_4226[5]; //Rr
extern i8 _4976_422b[5]; //Rr
extern X8 _4976_4230[23]; //Rr
extern X8 _4976_4247[23]; //Rr
extern X16 _4976_425e[9]; //Rr
extern X8 tlbGraphicsPitRoof[9]; //Rr
extern U8 _4976_4279[9]; //Rr
extern X16 _4976_4282[16]; //Rr
extern U8 tlbGraphicsFlip[16]; //Rr
extern X8 tlbGraphicsPitHidden[16]; //Rr
extern X8 tlbGraphicsPitNormal[16]; //Rr
extern X16 _4976_42d2[16]; //Rr
extern X8 _4976_42f2[23][4]; //Rr
extern i8 _4976_436c[4][5]; //Rr
extern U8 _4976_4380[4][2]; //Rr
extern i8 _4976_4388[4][4]; //Rr
extern i8 _4976_4398[2]; //Rr
extern X8 _4976_43e5[16]; //Rr
extern i8 _4976_43f5[16][2]; //Rr
extern i8 _4976_4415[25][2]; //Rr
extern U8 _4976_4447[3][4]; //Rr
extern U8 _4976_4453[16]; //Rr
extern i8 _4976_4463[23]; //Rr
extern i8 _4976_447a[4][8][2]; //Rp
extern i8 _4976_44ba[3]; //Rr
extern i8 _4976_44c1[4]; //Rr
extern U8 _4976_44c5[16][2]; //Rr
extern i8 tlbRectnoDoorButton[14]; //Rr _4976_44e5
extern X8 tlbGraphicsDoorSideFrames[14][2]; //Rr
extern X8 tlbGraphicsDoorRoofSlits[14]; //Rr
extern X16 tlbRectnoDoorRoofSlits[14]; //Rr
extern i8 tlbInvertedDoorDistance[5]; //Rr
extern X16 tlbRectnoDoorPosition[16]; //Rr
extern U8 _4976_455e[16]; //Rr
extern X8 tlbGraphicsStairsFront1[16][2]; //Rr
extern X8 tlbGraphicsStairsFront2[16][2]; //Rr
extern X16 _4976_45ae[16][2]; //Rr
extern X8 tlbGraphicsStairsSide[9][2]; //Rr
extern X16 _4976_4600[9][2]; //Rr
extern X16 _4976_4624_tblGfxSet[6]; //Rr
extern X16 _4976_4630_tblGfxSet[6]; //Rr
extern X16 _4976_463c_tblGfxSet[6]; //Rr
extern X16 _4976_4648_tblGfxSet[6]; //Rr
extern i8 _4976_4654[23]; //Rr
extern U8 _4976_466b[20]; //Rr
extern X32 _4976_4684[4]; //Rr
extern X32 _4976_4694[8]; //Rr
extern X32 _4976_46b4[8]; //Rr
extern X32 _4976_46d4[8]; //Rr
extern i8 _4976_4764[24]; //Rr
extern i8 tlbRainRelated[128]; //Rr	(_4976_477c)
extern X16 _4976_481a[15]; //Rr
extern U8 _4976_483f[5]; //Rr
extern i8 _4976_484f[4]; //Rr
extern U8 _4976_5268[40]; //Rp
extern U8 _4976_4844[7]; //Rp
extern sk0d9e _4976_0d9e[257]; //Rp
extern sk13a4 _4976_13a4[116]; //Rp
extern U8 _4976_169c[81]; //Rp
extern sk1891 _4976_1574[74]; //Rp
extern sk1891 _4976_1891[10]; //Rp
extern U32 blk1;
extern sk16ed _4976_16ed[60]; //Rp

extern U16	_04bf_05f9[18]; //Rr
extern X16	_04bf_02ac[8]; //Rr
extern X16	_04bf_029c[8]; //Rr
extern U8	_04bf_067f[256]; //Rp
extern U16	_04bf_0e6a[8]; //Rp
extern U16	_04bf_0e54[8]; //Rp
extern sk01b2	_04bf_01b2[4]; //Rr
extern U8 _04bf_03d6[220]; //Rr
extern U8 tlbUIEventTable[236]; //Rr
extern U8	tblDefaultNbItemAllocationPerDB[16]; //Rr // _4976_0252
extern U8	_4976_3b5f[9]; //Rp
extern X8	_4976_4736[12]; //Rp
extern U16	_4976_404d[5][3]; //Rp
extern U16	_4976_49e8[13]; //Rr // player item holding mask
extern U8	_4976_4862[96]; //Rp
extern U8	_4976_48c2[128]; //Rp
extern U8	_4976_00bc[8][4]; //Rp
extern sk3b74	_4976_3b74[55]; //Rp
extern sk0cea	_4976_0ce0[19]; //Rp
extern U16		_4976_49d4[5][2]; //Rp
extern U8		_4976_3fce[30]; //Rp
extern U8		tlbCreatureCommandsFlags[86]; //Rr
extern U8	_04bf_02be[128]; //Rp // rawkey to ascii
extern U8	_04bf_033e[128]; //Rp // rawkey+shift to ascii
extern U8	_4976_00dc[8]; //Rr
extern U16	_4976_014c[4]; //Rr
extern U16	_4976_01bc[2][8]; //Rr
extern i16	_4976_3924[4]; //Rr
extern U8	_4976_392c[4]; //Rr
extern i8	tLightLevelItem[16]; //Rr
extern i8	tLightLevelTable[6]; //Rr // (_4976_3d44) Light level correspondance
extern U8	_4976_3dde[4]; //Rr // each player dir?
extern U16	_4976_3de2[6]; //Rp // players in squad position list?
extern U16	_4976_3de6[4][2]; //Rr
extern U8	_4976_3df9[7]; //Rr
extern U8	_4976_4980[4]; //Rp
extern U8	_4976_4984[4]; //Rp
extern sk38f4	_4976_38f4[12]; //Rr
extern U8	_089c_00e3[256]; //Rp
//}}SKVAL1

extern const Bit8u * const _4976_498e;
extern const Bit8u * const _4976_4992;
extern const Bit8u * const _4976_4756;




//{{SKSAVE_OUTSORTED_VARS
extern U8	cram[MAX_CRAM];
#ifdef _SK_DYNAMIC_CEMS
extern U8* cems;
#else
extern U8	cems[MAX_CEMS];
#endif // _SK_DYNAMIC_CEMS

extern U8	vram[MAX_VRAM];

extern U8 	glbPaletteRGB[256][3];	// (_04bf_09f0) RGB palette
//}}SKSAVE_OUTSORTED_VARS

//------------------------------------------------------------------------------

#define tableMask _4976_3b13

//------------------------------------------------------------------------------

// lodsb
/*inline*/ U8 lodsb(const U8 *&dssi);
/*inline*/ U8 lodsb(U8 *&dssi);
// stosb
/*inline*/ void stosb(U8 *&esdi, U8 al);
// lodsw 
/*inline*/ U16 lodsw(const U8 *&dssi);
/*inline*/ U16 lodsw(U8 *&dssi);
// stosw
/*inline*/ void stosw(U8 *&esdi, U16 ax);
// movsb
/*inline*/ void movsb(U8 *&esdi, const U8 *&dssi);
/*inline*/ void movsb(U8 *&esdi, U8 *&dssi);
// movsw
/*inline*/ void movsw(U8 *&esdi, const U8 *&dssi);
/*inline*/ void movsw(U8 *&esdi, U8 *&dssi);
/*inline*/ unsigned RCJ(unsigned cx, unsigned x);

// dynamic range check by hand work
#define RCX(CX,X) RCJ(CX,X)



//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------

#define TEMP_BUFF_SIZE (1024)

#define DELETE_FILE FILE_DELETE
#define WRITE_FILE FILE_WRITE
#define CREATE_FILE FILE_CREATE
#define READ_FILE FILE_READ
#define CLOSE_FILE FILE_CLOSE
#define OPEN_FILE FILE_OPEN
#define SEEK_FILE FILE_SEEK

#define dbSize glbItemSizePerDB
#define glbCreatureTimerGetX glbCreatureTimer.XcoordB()	// (_4976_4ee0)
#define glbCreatureTimerGetY glbCreatureTimer.YcoordB()	// (_4976_4ee1)
#define _4976_5150 _4976_514e.b2
#define _4976_5151 _4976_514e.b3
#define _4976_5152 _4976_514e.w4
#define _4976_5154 _4976_514e.w6
#define _4976_5156 _4976_514e.w8



tileTypeIndex UtilConvertU16ToTile(i16 u16tileType);
bool	CheckSafePointer(void* p);
#define MVALID(_p_)	CheckSafePointer((void*)(_p_))








#define PTR_PADD(PTR,DELTA) &((U8 *)(PTR))[+(DELTA)]
#define PTR_PSBP(PTR1,PTR2) (((const U8 *)(PTR1)) - ((const U8 *)(PTR2)))

#define PTR_PADA(PTR,DELTA) *(U8 **)&(PTR) += (DELTA);
#define PTR_PSBA(PTR,DELTA) *(U8 **)&(PTR) -= (DELTA);

#define READ_UI8(PTR,DELTA) *((Bit8u *)(PTR_PADD(PTR,DELTA)))
#define READ_UI16(PTR,DELTA) *((Bit16u *)(PTR_PADD(PTR,DELTA)))
#define READ_UI32(PTR,DELTA) *((Bit32u *)(PTR_PADD(PTR,DELTA)))
#define READ_I8(PTR,DELTA) *((i8 *)(PTR_PADD(PTR,DELTA)))
#define READ_I16(PTR,DELTA) *((i16 *)(PTR_PADD(PTR,DELTA)))
#define READ_I32(PTR,DELTA) *((i32 *)(PTR_PADD(PTR,DELTA)))

#define WRITE_UI8(PTR,DELTA,VAL) *((Bit8u *)(PTR_PADD(PTR,DELTA))) = (VAL);
#define WRITE_UI16(PTR,DELTA,VAL) *((Bit16u *)(PTR_PADD(PTR,DELTA))) = (VAL);
#define WRITE_UI32(PTR,DELTA,VAL) *((Bit32u *)(PTR_PADD(PTR,DELTA))) = (VAL);
#define WRITE_I8(PTR,DELTA,VAL) *((i8 *)(PTR_PADD(PTR,DELTA))) = (VAL);
#define WRITE_I16(PTR,DELTA,VAL) *((i16 *)(PTR_PADD(PTR,DELTA))) = (VAL);
#define WRITE_I32(PTR,DELTA,VAL) *((i32 *)(PTR_PADD(PTR,DELTA))) = (VAL);

#define outportb(PORT,VAL)


//--- Some Debug/Test flags
extern bool	bUseDM2ExtendedMode;
extern float	fDebugRainSpeed;
extern bool	bUseSuperMode;
extern bool	bUseIngameDebug;
//extern bool	bUseFixedMode;
extern bool	bUsePowerDebug;


#if DM2_EXTENDED_BUFFER == 1
	#define BUFF_SIZE_MAX	1024000
#else
	#define BUFF_SIZE_MAX	60000	// Original value = 60000
#endif


#define WATER_MAX		2048
#define FOOD_MAX		2048
#define WATER_MIN		-1024
#define FOOD_MIN		-1024
#define WATER_WARNING	-512
#define FOOD_WARNING	-512

#define START_BASE_FOOD		1500
#define START_BASE_WATER	1500

#define WATER_DEFAULT_ITEM_VALUE	800
#define WATER_FLASK_VALUE			1600


#if DM2_EXTENDED_MODE == 1
	#define MAXHP 9999		// Max HP		= 9999
	#define MAXMP 9999		// Max Mana		= 9999
	#define MAXMP_BONUS	9999
	#define MAXSP 99999		// Max Stamina	= 9999
#else	// Original max values
	#define MAXHP 999
	#define MAXMP 900
	#define MAXMP_BONUS	999
	#define MAXSP 9999
#endif

#if (DM2_EXTENDED_MAP == 1)
#define MAXMAPS (256U)
#else
#define MAXMAPS (64U)
#endif

#define MAXDEPTH (63U) // eg. 6=ground floor, 7=sub-ground, 10=void area.
#define MAXDEPTH1 ((MAXDEPTH)+1U)

#define MAX3672 (56)

#define MIN_EYE_DIST 0

// SPX DEFINES put here
#include <XDEFINES.H>

#if DM2_EXTENDED_MODE == 1
#define GDAT_CATEGORY_LIMIT				0xF0
#else
#define GDAT_CATEGORY_LIMIT				0x1D	// Original code allow up to 0x1D (last being 0x1C = japanese fonts)
#endif


// The TICK_BALANCE value is anyway replaced by the user's speed selection.
#ifdef _USE_SDL
# define TICK_BALANCE	6
# define TICK_TIME_STAY	15U
#elif DM2_EXTENDED_MODE == 1
# define TICK_BALANCE	12			// 3
# define TICK_TIME_STAY	2U			// 1U
#else
# define TICK_BALANCE	12			// Rev10: Increased from 6
# define TICK_TIME_STAY	2U			// Rev10: Reduced from 3U
#endif
extern int stdTickBalance;




#define vbCr 13
#define vbLf 10



extern const char*	strSKSaveIOBin;





class CVerifySKSaveIO 
{
protected:
	FILE *f;

public:
	CVerifySKSaveIO();
	void StartWrite(U32 off);
	void Write(const void *pvData, const void *pvMask, U16 cbData, U32 repeat);
	void StartRead(U32 off);
	void Read(const void *pvData, const void *pvMask, Bit16u buffSize, Bit32u repeat);
};

extern CVerifySKSaveIO s_testSKSave;









namespace kkBitBlt 
{
	class Pic4 {
	public:
		U8 *buff;
		const U8 *pal;
		i16 ox, oy;
		U16 pitch, cx, cy;
		i16 dx, dy;

		void Apply(U16 mirror, U16 flip);
		U8 GetPure(U16 x, U16 y);
		U8 Get(U16 x, U16 y);
		void Put(U16 x, U16 y, U8 c);
	};
	class Pic8 {
	public:
		U8 *buff;
		U16 ox, oy, pitch, cy;
		i16 dx, dy;

		U8 GetPure(U16 x, U16 y);
		U8 Get(U16 x, U16 y);
		void Put(U16 x, U16 y, U8 c);
	};
	class Pic8wPal {
	public:
		U8 *buff;
		const U8 *pal;
		U16 cx, cy, cx2, cy2;
		i16 ox, oy, dx, dy;

		void Apply(U16 mirror, U16 flip);
		U8 GetPure(U16 x, U16 y);
		U8 Get(U16 x, U16 y);
		void Put(U16 x, U16 y, U8 c);
	};
	template<typename SP, typename DP> void BlitSvc(SP &sp, DP &dp, U16 cx, U16 cy, i16 ck) {
		for (U16 y=0; y<cy; y++) {
			for (U16 x=0; x<cx; x++) {
				if (sp.GetPure(x, y) != ck)
					dp.Put(x, y, sp.Get(x, y));
			}
		}
	}
};



extern U8 s_textLangSel[GDAT_CATEGORY_LIMIT][0xFF][0xFF];

#if DM2_EXTENDED_MODE == 1
	extern U8 s_imageLangSel[GDAT_CATEGORY_LIMIT][0xFF][0xFF];
#endif

#define DOSIMPL() ATLASSERT(false)

#define RUp2(X) (((X) +1)&0xFFFE)









//{{SK3672_DEF
extern sk3672 _4976_1a6a[];
extern sk3672 _4976_1d7f[];
extern sk3672 _4976_213f[];
extern sk3672 _4976_2356[];
extern sk3672 _4976_23a1[];
extern sk3672 _4976_2401[];
extern sk3672 _4976_2565[];
extern sk3672 _4976_2caa[];
extern sk3672 _4976_30d4[];
extern sk3672 _4976_362e[];
#define _4976_1a78 &_4976_1a6a[2]
#define _4976_1a86 &_4976_1a6a[4]
#define _4976_1a94 &_4976_1a6a[6]
#define _4976_1aa2 &_4976_1a6a[8]
#define _4976_1ab0 &_4976_1a6a[10]
#define _4976_1ab7 &_4976_1a6a[11]
#define _4976_1abe &_4976_1a6a[12]
#define _4976_1ac5 &_4976_1a6a[13]
#define _4976_1acc &_4976_1a6a[14]
#define _4976_1ada &_4976_1a6a[16]
#define _4976_1ae8 &_4976_1a6a[18]
#define _4976_1aef &_4976_1a6a[19]
#define _4976_1af6 &_4976_1a6a[20]
#define _4976_1afd &_4976_1a6a[21]
#define _4976_1b04 &_4976_1a6a[22]
#define _4976_1b0b &_4976_1a6a[23]
#define _4976_1b12 &_4976_1a6a[24]
#define _4976_1b27 &_4976_1a6a[27]
#define _4976_1b3c &_4976_1a6a[30]
#define _4976_1b43 &_4976_1a6a[31]
#define _4976_1b4a &_4976_1a6a[32]
#define _4976_1b51 &_4976_1a6a[33]
#define _4976_1b58 &_4976_1a6a[34]
#define _4976_1b5f &_4976_1a6a[35]
#define _4976_1b66 &_4976_1a6a[36]
#define _4976_1b6d &_4976_1a6a[37]
#define _4976_1b74 &_4976_1a6a[38]
#define _4976_1b82 &_4976_1a6a[40]
#define _4976_1b89 &_4976_1a6a[41]
#define _4976_1b90 &_4976_1a6a[42]
#define _4976_1b97 &_4976_1a6a[43]
#define _4976_1bac &_4976_1a6a[46]
#define _4976_1bb3 &_4976_1a6a[47]
#define _4976_1bc1 &_4976_1a6a[49]
#define _4976_256c &_4976_2565[1]
#define _4976_2573 &_4976_2565[2]
#define _4976_257a &_4976_2565[3]
#define _4976_2581 &_4976_2565[4]
#define _4976_258f &_4976_2565[6]
#define _4976_25b2 &_4976_2565[11]
#define _4976_261b &_4976_2565[26]
#define _4976_2622 &_4976_2565[27]
#define _4976_2629 &_4976_2565[28]
#define _4976_275d &_4976_2565[72]
extern sk3672 *_4976_3672[56];
//}}SK3672_DEF


//{{SK37FC_DEF
extern sk1bf9 _4976_1bcf[];
extern sk1bf9 _4976_1c31[];
extern sk1bf9 _4976_1bf9[];
extern sk1bf9 _4976_2027[];
extern sk1bf9 _4976_1fe1[];
extern sk1bf9 _4976_1c07[];
extern sk1bf9 _4976_205f[];
extern sk1bf9 _4976_1cbd[];
extern sk1bf9 _4976_200b[];
extern sk1bf9 _4976_1fd3[];
extern sk1bf9 _4976_22ad[];
extern sk1bf9 _4976_1cd9[];
extern sk1bf9 _4976_1da9[];
extern sk1bf9 _4976_2cb1[];
extern sk1bf9 _4976_1e1f[];
extern sk1bf9 _4976_2ce8[];
extern sk1bf9 _4976_230b[];
extern sk1bf9 _4976_22e3[];
extern sk1bf9 _4976_1ca1[];
extern sk1bf9 _4976_2379[];
extern sk1bf9 _4976_23cb[];
extern sk1bf9 _4976_2326[];
extern sk1bf9 _4976_2424[];
extern sk1bf9 _4976_281a[];
extern sk1bf9 _4976_2994[];
extern sk1bf9 _4976_294e[];
extern sk1bf9 _4976_2860[];
extern sk1bf9 _4976_2924[];
extern sk1bf9 _4976_2978[];
extern sk1bf9 _4976_1ccb[];
extern sk1bf9 _4976_29cc[];
extern sk1bf9 _4976_2916[];
extern sk1bf9 _4976_28fa[];
extern sk1bf9 _4976_287c[];
extern sk1bf9 _4976_2836[];
extern sk1bf9 _4976_2844[];
extern sk1bf9 _4976_2828[];
extern sk1bf9 _4976_2177[];
extern sk1bf9 _4976_214d[];
extern sk1bf9 _4976_1e9c[];
extern sk1bf9 _4976_1eaa[];
extern sk1bf9 _4976_1ed4[];
extern sk1bf9 _4976_1beb[];
extern sk1bf9 _4976_2b1f[];
extern sk1bf9 _4976_2b65[];
extern sk1bf9 _4976_1c93[];
extern sk1bf9 _4976_2b3b[];
extern sk1bf9 _4976_2db9[];
extern sk1bf9 _4976_2d8f[];
extern sk1bf9 _4976_244c[];
extern sk1bf9 _4976_2e6e[];
extern sk1bf9 _4976_2e36[];
extern sk1bf9 _4976_2f38[];
extern sk1bf9 _4976_2527[];
extern sk1bf9 _4976_2fc6[];
extern sk1bf9 _4976_3003[];
extern sk1bf9 _4976_3011[];
extern sk1bf9 _4976_302d[];
extern sk1bf9 _4976_312f[];
extern sk1bf9 _4976_3167[];
extern sk1bf9 _4976_30f7[];
extern sk1bf9 _4976_3191[];
extern sk1bf9 _4976_333c[];
extern sk1bf9 _4976_33d9[];
extern sk1bf9 _4976_1bdd[];
extern sk1bf9 _4976_3422[];
extern sk1bf9 _4976_1c69[];
extern sk1bf9 _4976_3470[];
extern sk1bf9 _4976_3531[];
extern sk1bf9 _4976_223e[];
extern sk1bf9 _4976_35b9[];
extern sk1bf9 _4976_35e9[];
extern sk1bf9 _4976_3651[];
extern skxxxh _4976_1d65[];
extern skxxxh _4976_1ce7[];
extern skxxxh _4976_20eb[];
extern skxxxh _4976_1d03[];
extern skxxxh _4976_20ac[];
extern skxxxh _4976_207b[];
extern skxxxh _4976_22bb[];
extern skxxxh _4976_1d18[];
extern skxxxh _4976_1dd3[];
extern skxxxh _4976_2ccd[];
extern skxxxh _4976_1d57[];
extern skxxxh _4976_1e49[];
extern skxxxh _4976_2d35[];
extern skxxxh _4976_2d20[];
extern skxxxh _4976_2319[];
extern skxxxh _4976_1d49[];
extern skxxxh _4976_22f1[];
extern skxxxh _4976_2387[];
extern skxxxh _4976_23e7[];
extern skxxxh _4976_2342[];
extern skxxxh _4976_2432[];
extern skxxxh _4976_2ac1[];
extern skxxxh _4976_2a82[];
extern skxxxh _4976_1d6c[];
extern skxxxh _4976_2acf[];
extern skxxxh _4976_2aac[];
extern skxxxh _4976_2a51[];
extern skxxxh _4976_2a20[];
extern skxxxh _4976_21e7[];
extern skxxxh _4976_1f83[];
extern skxxxh _4976_1f0c[];
extern skxxxh _4976_1ef0[];
extern skxxxh _4976_1f2f[];
extern skxxxh _4976_1f6e[];
extern skxxxh _4976_1f52[];
extern skxxxh _4976_2c1b[];
extern skxxxh _4976_2c30[];
extern skxxxh _4976_2bdc[];
extern skxxxh _4976_2bb9[];
extern skxxxh _4976_2d7c[];
extern skxxxh _4976_2d75[];
extern skxxxh _4976_2dea[];
extern skxxxh _4976_2dd5[];
extern skxxxh _4976_2468[];
extern skxxxh _4976_2492[];
extern skxxxh _4976_2e29[];
extern skxxxh _4976_2ede[];
extern skxxxh _4976_2ea6[];
extern skxxxh _4976_2f7e[];
extern skxxxh _4976_2551[];
extern skxxxh _4976_2fe2[];
extern skxxxh _4976_306c[];
extern skxxxh _4976_3049[];
extern skxxxh _4976_307a[];
extern skxxxh _4976_3088[];
extern skxxxh _4976_3239[];
extern skxxxh _4976_3247[];
extern skxxxh _4976_31c9[];
extern skxxxh _4976_31ec[];
extern skxxxh _4976_32b7[];
extern skxxxh _4976_3278[];
extern skxxxh _4976_3374[];
extern skxxxh _4976_3397[];
extern skxxxh _4976_33e7[];
extern skxxxh _4976_3401[];
extern skxxxh _4976_1d34[];
extern skxxxh _4976_3430[];
extern skxxxh _4976_34c4[];
extern skxxxh _4976_34a8[];
extern skxxxh _4976_355b[];
extern skxxxh _4976_225a[];
extern skxxxh _4976_24d9[];
extern skxxxh _4976_35d5[];
extern skxxxh _4976_3621[];
extern skxxxh _4976_365f[];
extern sk4efe _4976_1d73[];
extern sk4efe _4976_2115[];
extern sk4efe _4976_22d7[];
extern sk4efe _4976_1def[];
extern sk4efe _4976_2ce2[];
extern sk4efe _4976_1e6c[];
extern sk4efe _4976_2d51[];
extern sk4efe _4976_2320[];
extern sk4efe _4976_22ff[];
extern sk4efe _4976_2395[];
extern sk4efe _4976_23f5[];
extern sk4efe _4976_2350[];
extern sk4efe _4976_2440[];
extern sk4efe _4976_2add[];
extern sk4efe _4976_2226[];
extern sk4efe _4976_1f91[];
extern sk4efe _4976_2c68[];
extern sk4efe _4976_2d83[];
extern sk4efe _4976_2dff[];
extern sk4efe _4976_24b5[];
extern sk4efe _4976_2e30[];
extern sk4efe _4976_2f08[];
extern sk4efe _4976_2fa8[];
extern sk4efe _4976_255f[];
extern sk4efe _4976_2ff7[];
extern sk4efe _4976_30a4[];
extern sk4efe _4976_32e8[];
extern sk4efe _4976_33c1[];
extern sk4efe _4976_33f5[];
extern sk4efe _4976_3416[];
extern sk4efe _4976_344c[];
extern sk4efe _4976_34f5[];
extern sk4efe _4976_3577[];
extern sk4efe _4976_227d[];
extern sk4efe _4976_2503[];
extern sk4efe _4976_35e3[];
extern sk4efe _4976_3628[];
extern sk4efe _4976_3666[];

#if XDM2_EXTENDED_AI_TABSIZE == 1
	#define CREATURE_AI_TAB_SIZE	64
#else
	#define CREATURE_AI_TAB_SIZE	42
#endif // DM2_EXTENDED_MODE

extern sk4efe* tblAIStatsRef0FXX01[CREATURE_AI_TAB_SIZE]; // tblAIStatsRef0FXX01
//}}SK37FC_DEF



#endif // _SKGLOBAL_H_