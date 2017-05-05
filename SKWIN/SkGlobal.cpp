// SPX: Put here all global variables temporary before better structure
//

#include <StdAfx.h>
#include <SkGlobal.h>

#include <SkWinDebug.h>

//using namespace DMEncyclopaedia;
using namespace DM2Internal;




#if defined(_USE_MFC80) || defined(_USE_MFC60)
CSkWinMFC skwin;
#endif
#ifdef _USE_SDL
CSkWinSDL skwin;
#endif




#include "sk37fc.h"
#include "sk3672.h"


// USE OF EXTENDED MODE
#if DM2_EXTENDED_MODE == 1
bool	bUseDM2ExtendedMode	= true;
float	fDebugRainSpeed = 30;
#else
bool	bUseDM2ExtendedMode = false;
float	fDebugRainSpeed = 1;
#endif
// END DM2_EXTENDED_MODE DEF
bool	bUseSuperMode = false;
bool	bUseIngameDebug = true;
//bool	bUseFixedMode = true;
bool	bUsePowerDebug = true;		// A new flag ON for solving some savegame load (attempt) and other test purpose
//=> SPX: put in SkCodeParam


//--> special table for localized text
/*static*/ U8 s_textLangSel[GDAT_CATEGORY_LIMIT][0xFF][0xFF]; // is one of 0x00,0x10,0x20,0x30

#if DM2_EXTENDED_MODE == 1
// special table for localized image, actually used for interface
	U8 s_imageLangSel[GDAT_CATEGORY_LIMIT][0xFF][0xFF];
#endif


// Name for the temporary file
//const char*	strSKSaveIOBin = "C:\\A\\SKSaveIO.bin";
// SPX: makes the temp file on current directory, to prevent crash when C:\A does not exist.
const char*	strSKSaveIOBin = "SKSaveIO.bin";

CVerifySKSaveIO s_testSKSave;






#if defined(_USE_MFC60) || defined(_USE_MFC80)

CVerifySKSaveIO::CVerifySKSaveIO(): f(NULL) 
{
}

void CVerifySKSaveIO::StartWrite(U32 off) {
#if _DEBUG
	if (f != NULL) {
		fclose(f);
	}
	f = fopen(strSKSaveIOBin, "wb");

	ATLVERIFY(1 == fwrite(&off, 4, 1, f));
#endif
}

void CVerifySKSaveIO::Write(const void *pvData, const void *pvMask, U16 cbData, U32 repeat) {
#if _DEBUG
	if (f == NULL)
		return;

	ATLASSERT(cbData < 1024);

	ATLVERIFY(1 == fwrite(&cbData, 2, 1, f));
	ATLVERIFY(1 == fwrite(&repeat, 4, 1, f));
	ATLVERIFY(1 == fwrite(pvMask, cbData, 1, f));
	ATLVERIFY(1 == fwrite(pvData, cbData * repeat, 1, f));
#endif
}

void CVerifySKSaveIO::StartRead(U32 off) {
#if _DEBUG
	if (f != NULL) {
		fclose(f);
	}
	f = fopen(strSKSaveIOBin, "rb");

	if (f == NULL)
		return;

	U32 offThen = 0;
	ATLVERIFY(1 == fread(&offThen, 4, 1, f));
	ATLASSERT(off == offThen);
#endif
}

void CVerifySKSaveIO::Read(const void *pvData, const void *pvMask, Bit16u buffSize, Bit32u repeat) {
#if _DEBUG
	if (f == NULL)
		return;

	U16 cbDataThen;
	ATLVERIFY(1 == fread(&cbDataThen, 2, 1, f));
	U32 repeatThen;
	ATLVERIFY(1 == fread(&repeatThen, 4, 1, f));

	ATLASSERT(buffSize == cbDataThen);
	ATLASSERT(repeat == repeatThen);

	char maskThen[1024];
	ATLVERIFY(1 == fread(maskThen, cbDataThen, 1, f));

	ATLASSERT(memcmp(pvMask, maskThen, cbDataThen) == 0);

	const U8 *pbData = reinterpret_cast<const U8 *>(pvData);

	for (U32 blk = 0; blk < repeat; blk++) {
		char buffTemp[1024];
		ATLVERIFY(1 == fread(buffTemp, buffSize, 1, f));
		for (int off = 0; off < buffSize; off++) {
			BYTE bMask = reinterpret_cast<const U8 *>(pvMask)[off];
			ATLASSERT((pbData[blk * buffSize + off] & bMask) == (bMask & buffTemp[off]));
		}
	}
#endif
}


#endif // defined(_USE_MFC60) || defined(_USE_MFC80)


namespace kkBitBlt 
{
	void Pic4::Apply(U16 mirror, U16 flip) {
		if (mirror) {
			ox += cx -1;
			dx = -1;
		}
		if (flip) {
			oy += cy -1;
			dy = -1;
		}
	}
	U8 Pic4::GetPure(U16 x, U16 y) {
		x = +ox +i16(x)*dx;
		y = +oy +i16(y)*dy;
		U8 c = buff[(x >> 1) +pitch*y];
		return ((x & 1) == 0) ? (c >> 4) : (c & 15);
	}
	U8 Pic4::Get(U16 x, U16 y) {
		return pal[GetPure(x, y)];
	}
	void Pic4::Put(U16 x, U16 y, U8 c) {
		x = +ox +x*dx;
		y = +oy +y*dy;
		U8 pix = buff[(x >> 1) +pitch*y];
		if ((x & 1) == 0) {
			pix &= 0x0f; pix |= c << 4;
		}
		else {
			pix &= 0xf0; pix |= c & 15;
		}
		buff[(x >> 1) +pitch*y] = pix;
	}

	U8 Pic8::GetPure(U16 x, U16 y) {
		return Get(x, y);
	}
	U8 Pic8::Get(U16 x, U16 y) {
		x = +ox +i16(x)*dx;
		y = +oy +i16(y)*dy;
		return buff[x +pitch*y];
	}
	void Pic8::Put(U16 x, U16 y, U8 c) {
		x = +ox +x*dx;
		y = +oy +y*dy;
		buff[x +pitch*y] = c;
	}
	void Pic8wPal::Apply(U16 mirror, U16 flip) {
		if (mirror) {
			ox += cx2 -1;
			dx = -1;
		}
		if (flip) {
			oy += cy -1;
			dy = -1;
		}
	}
	U8 Pic8wPal::GetPure(U16 x, U16 y) {
		return Get(x, y);
	}
	U8 Pic8wPal::Get(U16 x, U16 y) {
		i16 vx = +ox +i16(x)*dx;
		i16 vy = +oy +i16(y)*dy;
		return pal[buff[vx +cx*vy]];
	}
	void Pic8wPal::Put(U16 x, U16 y, U8 c) {
		x = +ox +x*dx;
		y = +oy +y*dy;
		buff[x +cx*y] = c;
	}


};







namespace DM2Internal {
	void Ax3::SetX(U16 val) {
		if (bUsePowerDebug) val = min(31, val);
		ATLASSERT(val <= 31 || val == 0xFFFFU);
		val &= 0x001f;
		w0 &= 0xffe0;
		w0 |= val;
	}
	void Ax3::SetY(U16 val) {
		if (bUsePowerDebug) val = min(31, val);
		ATLASSERT(val <= 31 || val == 0xFFFFU);
		val &= 0x001f;
		w0 &= 0xfc1f;
		w0 |= val << 5;
	}
	void Ax3::SetMap(U16 val) {
		ATLASSERT(val <= MAXMAPS);
		val &= 0x003f;
		w0 &= 0x03ff;
		w0 |= val << 10;
	}
};























int stdTickBalance = TICK_BALANCE;


// 4976:3B13  A7 3A 76 49 00 00 00 00 AB 3A 76 49 AF 3A 76 49  ..vI......vI..vI
// 4976:3B23  B7 3A 76 49 D7 3A 76 49 DB 3A 76 49 DF 3A 76 49  ..vI..vI..vI..vI
// 4976:3B33  E3 3A 76 49 E7 3A 76 49 F7 3A 76 49 00 00 00 00  ..vI..vI..vI....
// 4976:3B43  00 00 00 00 00 00 00 00 FF 3A 76 49 07 3B 76 49  ..........vI..vI

/*static*/ const void *_4976_3aa7 = "\x00\x00\x00\x3E";
/*static*/ const void *_4976_3aab = "\x00\x00\x01\x00";
/*static*/ const void *_4976_3aaf = "\x00\x00\x00\x00\x05\x00";
/*static*/ const void *_4976_3ab7 = "\x00\x00\x00\x00\x00\x00\xFF\x3F\xFF\xFF\xFF\xFF\xFF\xFF\x80\x07";
/*static*/ const void *_4976_3ac7 = "\x00\x00\x00\x00\x00\x00\xFF\x3F\xFF\x07\xFF\xFF\x3F\x00\x00\x07";
/*static*/ const void *_4976_3ad7 = "\x00\x00\xFF\x3D";
/*static*/ const void *_4976_3adb = "\x00\x00\xFF\x1F";
/*static*/ const void *_4976_3adf = "\x00\x00\xFF\xFF";
/*static*/ const void *_4976_3ae3 = "\x00\x00\xFF\xFF";
/*static*/ const void *_4976_3ae7 = "\x00\x00\x00\x00\x00\xE0\x00\x04";
/*static*/ const void *_4976_3aef = "\x00\x00\x00\x00\x00\xE0\xFF\xFF";
/*static*/ const void *_4976_3af7 = "\x00\x00\xFF\xC0";
/*static*/ const void *_4976_3afb = "\x00\x00\x7F\xFF";
/*static*/ const void *_4976_3aff = "\x00\x00\x00\x00\xFF\xFF\xFF\x03";
/*static*/ const void *_4976_3b07 = "\x00\x00\xFF\xFF";
/*static*/ const void *_4976_3b0b = "\x00\x00\x00\x00\xFF\xFF\x01\x00";

//#4976:3B13
/*static*/const void *_4976_3b13[16] = {
	_4976_3aa7,
	NULL,
	_4976_3aab,
	_4976_3aaf,
	_4976_3ab7,
	_4976_3ad7,
	_4976_3adb,
	_4976_3adf,
	_4976_3ae3,
	_4976_3ae7,
	_4976_3af7,
	NULL,
	NULL,
	NULL,
	_4976_3aff,
	_4976_3b07,
};












//------------------------------------------------------------------------------
//	SK STR TBL
//------------------------------------------------------------------------------

//{{SKSTRTBL

const Bit8u * const _4976_395a = (const Bit8u *)
	"\xFF\xFF\xFF\x00\xFF\xFF\x00\x00\x07\x00\x1F\x00\x1F\x00\x03\x00"
	"\x3F\x00\x03\x00\xFF\x01\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x00\x07\x00"
	"\x07\x00\x03\x00\x01\x00\xFF\x00\x01\x00\x01\x03\xFF\xFF\xFF\x03"
	"\xFF\x00\x1F\x03\xFF\xFF\xFF";
const Bit8u * const _4976_3956 = (const Bit8u *)
	"\xFF\xFF";
const Bit8u * const _4976_3992 = (const Bit8u *)
	"\x7F\x7F\x7F\x7F\x7F\x7F\x7F\x00\x7F\x7F\x7F\x7F\x7F\x7F\x7F\x7F"
	"\x7F\x7F\x7F\x7F\x7F\x7F\x7F\x7F\x7F\x7F\x7F\x00\x03\x03\x07\x00"
	"\x3F\x3F\x7F\x7F\x7F\x7F\x00\x00\x03\xFF\xFF\xFF\xFF\x00\x00\x00"
	"\x00\x00\x12\x00\x3F\x00\xFF\x03\xFF\x03\xFF\x3F\xFF\x3F\xFF\x03"
	"\xFF\x03\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
	"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xE0\xFF\xFF"
	"\x1F\xE0\xFF\xFF\x1F\xE0\xFF\xFF\x1F\xE0\xFF\xFF\x1F\xE0\xFF\xFF"
	"\x1F\xE0\xFF\xFF\x1F\xE0\xFF\xFF\x1F\xE0\xFF\xFF\x1F\xE0\xFF\xFF"
	"\x1F\xE0\xFF\xFF\x1F\xE0\xFF\xFF\x1F\xE0\xFF\xFF\x1F\xE0\xFF\xFF"
	"\x1F\xE0\xFF\xFF\x1F\xE0\xFF\xFF\x1F\xE0\xFF\xFF\x1F\xE0\xFF\xFF"
	"\x1F\xE0\xFF\xFF\x1F\xE0\xFF\xFF\x1F\xE0\xFF\xFF\x1F\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x3F"
	"\x0F\x00\x00\x00\x00";
const Bit8u * const _4976_3a97 = (const Bit8u *)
	"\x00\x00\x00\xFF\xFF\x00";
const Bit8u * const _4976_3a9d = (const Bit8u *)
	"\xFF\xFF\xFF\x3F\x7F\xFF\xFF\xFF\xFF\xFF";

// SPX: _4976_3da4 renamed strZT_EndOfString
// SPX: _4976_3da5 renamed strZT_F
// SPX: _4976_3da7 renamed strZT_G
// SPX: _4976_3da9 renamed strZT_strData1
// SPX: _4976_3db4 renamed strZT_strData2
// SPX: _4976_3dbf renamed strZT_strData3
// SPX: _4976_3dca renamed strZT_strData4
// SPX: _4976_3dd0 renamed strZT_strData5

const Bit8u * const strZT_EndOfString = (const Bit8u *)
	"\x00";											// END OF STRING
const Bit8u * const strZT_F = (const Bit8u *)
	"\x46\x00";										// F (for French?)
const Bit8u * const strZT_G = (const Bit8u *)
	"\x47\x00";										// G (for German?)
const Bit8u * const strZT_strData1 = (const Bit8u *)
	"\x2E\x5A\x30\x30\x38\x44\x41\x54\x41\x5C\x00";	// ".Z008DATA\"
const Bit8u * const strZT_strData2 = (const Bit8u *)
	"\x2E\x5A\x30\x30\x38\x44\x41\x54\x41\x5C\x00";	// ".Z008DATA\"
const Bit8u * const strZT_strData3 = (const Bit8u *)
	"\x2E\x5A\x30\x30\x39\x44\x41\x54\x41\x5C\x00";	// ".Z009DATA\"
const Bit8u * const strZT_strData4 = (const Bit8u *)
	"\x2E\x5A\x30\x30\x39\x00";						// .Z009
const Bit8u * const strZT_strData5 = (const Bit8u *)
	"\x2E\x5A\x30\x30\x39\x44\x41\x54\x41\x5C\x00";	// ".Z009DATA\"

// SPX: _4976_3d84 renamed strZxxxTable
const Bit8u * const strZxxxTable[] = {
	strZT_EndOfString,	// 0	\0
	strZT_F,			// 1	F
	strZT_G,			// 2	G
	strZT_strData1,		// 3	".Z008DATA\"
	strZT_strData2,		// 4	".Z008DATA\"
	strZT_strData3,		// 5	".Z009DATA\"
	strZT_strData4,		// 6	".Z009"
	strZT_strData5,		// 7	".Z009DATA\"
};
// SPX: _4976_1a3e renamed strSKSave_dat
const Bit8u * const strSKSave_dat = (const Bit8u *)
	"\x2E\x5A\x30\x32\x32\x53\x4B\x53\x41\x56\x45\x2E\x5A\x30\x32\x33\x2E\x44\x41\x54\x00"; // .Z022SKSAVE.Z023.DAT
// SPX: _4976_1a53 renamed strSKSave_bak
const Bit8u * const strSKSave_bak = (const Bit8u *)
	"\x2E\x5A\x30\x32\x32\x53\x4B\x53\x41\x56\x45\x2E\x5A\x30\x32\x33\x2E\x42\x41\x4B\x00"; // .Z022SKSAVE.Z023.BAK
// SPX: _4976_19ee renamed strDungeon_ftl
const Bit8u * const strDungeon_ftl = (const Bit8u *)
	"\x2E\x5A\x30\x32\x30\x44\x55\x4E\x47\x45\x4F\x4E\x2E\x5A\x30\x32\x34\x2E\x46\x54\x4C"; // .Z020DUNGEON.Z024.FTL

// SPX: _4976_1a1a renamed strGraphics
const Bit8u * const strGraphics = (const Bit8u *)
	".Z020GRAPHICS.DAT";
	//"\x2E\x5A\x30\x32\x30\x47\x52\x41\x50\x48\x49\x43\x53\x2E\x44\x41\x54"; // .Z020GRAPHICS.DAT

// SPX: _4976_1a2c renamed strGraphics2
const Bit8u * const strGraphics2 = (const Bit8u *)
	"\x2E\x5A\x30\x32\x36\x47\x52\x41\x50\x48\x49\x43\x32\x2E\x44\x41\x54"; // .Z026GRAPHIC2.DAT
const Bit8u * const _4976_3b68 = (const Bit8u *)
	"\x47\x41\x4D\x45\x20\x20\x00";                                    // GAME  .
const Bit8u * const _4976_4853 = (const Bit8u *)
	"\x0A\x44\x4D\x3A\x20\x56\x00";                                    // .DM. V.
const Bit8u * const _4976_485a = (const Bit8u *)
	"\x2D\x2D\x2D\x20\x00";                                            // ... .
// SPX: _04bf_0661 renamed strPC286Required
const Bit8u * const strPC286Required = (const Bit8u *)
	"\x32\x38\x36\x20\x6F\x72\x20\x68\x69\x67\x68\x65\x72\x20\x72\x65" // 286 or higher re
	"\x71\x75\x69\x72\x65\x64\x0A\x0D\x00";                            // quired...
const Bit8u * const _04bf_067a = (const Bit8u *)
	"\x20\x00";                                                        //  .
// SPX: _4976_1a04 renamed strDungenB
const Bit8u * const strDungenB = (const Bit8u *)
	"\x2E\x5A\x30\x32\x30\x44\x55\x4E\x47\x45\x4E\x42\x2E\x5A\x30\x32" // .Z020DUNGENB.Z02
	"\x34\x2E\x44\x41\x54\x00";                                        // 4.DAT.
// SPX: _4976_19d8 renamed strDungeon
#if (DM2_EXTENDED_JSON == 1)
const Bit8u * const strDungeon = (const Bit8u *) ".Z020dungeon.Z024.json";
#elif (DM2_EXTENDED_MAP == 1)
const Bit8u * const strDungeon = (const Bit8u *) ".Z020DUNGEON.Z024_XMAP.DAT";
#else
const Bit8u * const strDungeon = (const Bit8u *)
	"\x2E\x5A\x30\x32\x30\x44\x55\x4E\x47\x45\x4F\x4E\x2E\x5A\x30\x32" // .Z020DUNGEON.Z02
	"\x34\x2E\x44\x41\x54\x00";                                        // 4.DAT.
#endif // DM2_EXTENDED_MODE


// SPX: _4976_01e0 renamed strVersionNumber
#if (DM2_EXTENDED_MODE == 1)
const Bit8u * const strVersionNumber = (const Bit8u *) "V1.2";
#else
const Bit8u * const strVersionNumber = (const Bit8u *)
	"\x56\x31\x2E\x30\x00";                                            // V1.0.
#endif // DM2_EXTENDED_MODE

// SPX: _4976_4024 renamed strSlash
const Bit8u * const strSlash = (const Bit8u *)"/";

// SPX: _4976_4988 renamed strDirLetter
char strDirLetter[3] = {"A:"};
// SPX: _4976_498b renamed strDirLetter2
char strDirLetter2[3] = {"A:"};

//}}SKSTRTBL

//------------------------------------------------------------------------------
//	SK STR TBL
//------------------------------------------------------------------------------

//{{SKSTRREF
const Bit8u * const ptrDirLetter = reinterpret_cast<Bit8u *>(strDirLetter);	// SPX: _4976_4996 renamed ptrDirLetter
const Bit8u * const ptrDirLetter2 = reinterpret_cast<Bit8u *>(strDirLetter2);		// SPX: _4976_499a renamed ptrDirLetter2
const Bit8u * const ptrDungenB = strDungenB;							// SPX: _4976_19c4 renamed ptrDungenB
const Bit8u * const ptrSKSave_dat = strSKSave_dat;						// SPX: _4976_19d0 renamed ptrSKSave_dat
const Bit8u * const ptrSKSave_bak = strSKSave_bak;						// SPX: _4976_19d4 renamed ptrSKSave_bak
const Bit8u * const ptrDungeon_ftl = strDungeon_ftl;					// SPX: _4976_19c0 renamed ptrDungeon_ftl
const Bit8u * const ptrGraphics = strGraphics;							// SPX: _4976_19c8 renamed ptrGraphics
const Bit8u * const ptrGraphics2 = strGraphics2;						// SPX: _4976_19cc renamed ptrGraphics2
const Bit8u * const ptrDungeonFilename = strDungeon;					// SPX: _4976_19bc renamed ptrDungeonFilename
//}}SKSTRREF





//{{SKCONST

// SPX: _4976_0242 renamed glbItemSizePerDB
#if (DM2_EXTENDED_JSON == 1)
const Bit8u glbItemSizePerDB[16] = { // dbSize
		sizeof(Door),		sizeof(Teleporter),	sizeof(Text),		sizeof(Actuator),
		sizeof(Creature),	sizeof(Weapon),		sizeof(Cloth),		sizeof(Scroll),
		sizeof(Potion),		sizeof(Container),	sizeof(Miscellaneous_item),	DB_SIZE_UNUSED,
		DB_SIZE_UNUSED,		DB_SIZE_UNUSED,		sizeof(Missile),	sizeof(Cloud),
};
#elif (DM2_EXTENDED_DATABASE == 0)
const Bit8u glbItemSizePerDB[16] = { // dbSize
		DB_SIZE_DOOR,		DB_SIZE_TELEPORTER,	DB_SIZE_TEXT,		DB_SIZE_ACTUATOR,
		DB_SIZE_CREATURE,	DB_SIZE_WEAPON,		DB_SIZE_CLOTHING,	DB_SIZE_SCROLL,
		DB_SIZE_POTION,		DB_SIZE_CONTAINER,	DB_SIZE_MISC_ITEM,	DB_SIZE_UNUSED,
		DB_SIZE_UNUSED,		DB_SIZE_UNUSED,		DB_SIZE_MISSILE,	DB_SIZE_CLOUD,
};
#else if (DM2_EXTENDED_DATABASE == 1) // make use of db11,12,13 with actuators
const Bit8u glbItemSizePerDB[16] = {
		DB_SIZE_DOOR,		DB_SIZE_TELEPORTER,	DB_SIZE_TEXT,		DB_SIZE_ACTUATOR,
		DB_SIZE_CREATURE,	DB_SIZE_WEAPON,		DB_SIZE_CLOTHING,	DB_SIZE_SCROLL,
		DB_SIZE_POTION,		DB_SIZE_CONTAINER,	DB_SIZE_MISC_ITEM,	DB_SIZE_ACTUATOR,
		DB_SIZE_ACTUATOR,	DB_SIZE_ACTUATOR,	DB_SIZE_MISSILE,	DB_SIZE_CLOUD,
};
#endif // DM2_EXTENDED_DATABASE

// SPX: _4976_3b54 renamed glbByte01
const Bit8u glbByte01[1] = {0x01};

// SPX: _4976_0caa renamed glbGDAT2CatPerDB
// this seems to refer to the GDAT2 category for each database item type.
const Bit8u glbGDAT2CatPerDB[16] = {
	GDAT_CATEGORY_DOORS, // 0x0E door
	GDAT_CATEGORY_TELEPORTERS, // 0x18 tele
	0xFF, // 0xFF text
	0xFF, // 0xFF actu
	GDAT_CATEGORY_CREATURES, // 0x0F mons
	GDAT_CATEGORY_WEAPONS, // 0x10 weap
	GDAT_CATEGORY_CLOTHES, // 0x11 clot
	GDAT_CATEGORY_SCROLLS, // 0x12 scro
	GDAT_CATEGORY_POTIONS, // 0x13 poti
	GDAT_CATEGORY_CONTAINERS, // 0x14 ches
	GDAT_CATEGORY_MISCELLANEOUS, // 0x15 misc
	0xFF, // 0xFF 11
	0xFF, // 0xFF 12
	0xFF, // 0xFF 13
	0xFF, // 0xFF missile
	GDAT_CATEGORY_SPELL_MISSILES, // 0x0D cloud
};

// SPX: 44 values of ... ?
const Bit8u _4976_0c5e[0x2c] = {
//  0 1 2 3 4 5 6 7 8 9 A B C D E F
	0,1,1,0,1,0,0,1,0,1,0,0,0,0,0,0,	// 00
	0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,	// 10
	0,0,0,0,0,0,0,0,0,1,1,1,			// 20
};

// SPX: _4976_0232 renamed glbXAxisDelta / _4976_023a renamed glbYAxisDelta
/*static*/ const __int16 glbXAxisDelta[4] = { 0, 1, 0,-1}; // x-axis delta
/*static*/ const __int16 glbYAxisDelta[4] = {-1, 0, 1, 0}; // y-axis delta

// SPX: Items activator/activation list / _4976_0c8a renamed glbActivationItemRangePerDB
/*static*/ const Bit16u glbActivationItemRangePerDB[16] = {
	0x81FF, // door
	0x81FF, // tele
	0x81FF, // text
	0x81FF, // actu
	0x01B0, // mons // 0x1B0 to 0x1DF	C (48)
	0x0000, // weap // 0x000 to 0x07F	W (128)
	0x0080, // clot // 0x080 to 0x0FF	A (128)
	0x81FC, // scro // 0x1FC			S (1)
	0x0180, // poti // 0x180 to 0x1AF	P (48)
	0x01E0, // ches // 0x1E0 to 0x1FB	C (28)
	0x0100, // misc // 0x100 to 0x17F	J (128)
	0x81FF, // 11
	0x81FF, // 12
	0x81FF, // 13
	0x81FF, // missile
	0x81FF, // cloud
};

// SPX: some kind of symmetric table (8*2 values) that could be used for determining left-right volume from distant point
/*static*/ const Bit16u _4976_49c0[] = {
	0x25A0,
	0x11F0,
	0x0B00,
	0x0730,
	0x0490,
	0x0290,
	0x00D0,
	0x0000,
};

// The description is from Suule's post at http://www.dungeon-master.com/forum/viewtopic.php?t=24695
// Nice work, Suule!

// SPX: _4976_3f32 renamed ItemCommandsTable
const char ItemCommandsTable[18][3] = {
	"SK", //SK - Skill number used. Skills from 0-3 are 'normal' visible skills (Fighter, Ninja, Priest, Wizard) 
	"LV", //LV - minimal skill level for the command to appear 
	"CM", //CM - Command : It seems to be 'how to handle an attack'. 
	"BZ", //BZ - Stamina loss (SPX: I think this is BLITZ, amount of time to regain from action)
	"TR", //TR - Fatigue ( the ammount of time before reusal of the weapon) 
	"ST", //ST - Spell Strength (SPX)
	"PA", // Magical projectile associated.	// 6th
	"TA", //TA - Defence modifier 
	"NC", //NC - Number of Charges (SPX) How many charges are consummed by the action
	"EX", //EX - Experience gained 
	"PB", //PB - Hit Probality 
	"DM", //DM - Damage done 
	"MS",	// SPX: this one seems to be never used?
	"SD", //SD - Sound Entry (used for "shooting" items such as bow and staves)
	"RP", //RP - Repeat action
	"HN", //HN - Hurt Non material
	"AT",	// SPX: Something about fire and hit probability?
	"WH", //WH - Where? Pouch or Scabbard
};

/*static*/ const __int8  _4976_3fc2[4] = { 0, 3, 2, 1};
/*static*/ const __int16 _4976_3fb8[5] = {-2,-3, 0,-1,-2};

/*static*/ const Bit8u _4976_3fa0[4] = {0, 0, 3, 3};
/*static*/ const Bit8u _4976_3fa4[4] = {0, 1, 3, 2};
/*static*/ const i8 _4976_3fa8[16] = {
	0, 0, 2, 2, 
	0, 2, 2, 0,
	0, 0,-2,-2,
	0,-2,-2, 0,
};

// SPX: _4976_475a renamed strEndLine
const Bit8u * const strEndLine = (const Bit8u *)
	"\n";

//4976:3E00  01 02 03 04 05 06 02 03 04 05 06 07 04 05 06 07
//4976:3E10  07 09 02 02 03 04 06 07 08 0C 10 14 18 1C 02 03

//SPX: _4976_3e00 renamed RuneManaPower / Rune power multiplicator table
// Spotted at 0x765F0 in Skull.exe
/*static*/ const Bit8u RuneManaPower[24] = {
//	LO UM ON EE PAL MON
	1, 2, 3, 4, 5, 6,
	2, 3, 4, 5, 6, 7,
	4, 5, 6, 7, 7, 9,
	2, 2, 3, 4, 6, 7,
};

// This table is used for mana multiplicator per power level, divided by 8
// Then the power multiplicator are rather : 1, 1.5, 2, 2.5, 3, 3.5
// _4976_3e18 renamed RunePowerMultiplicator
/*static*/ const Bit8u RunePowerMultiplicator[30] = {
	8,12,16,20,24,28,
};

// SPX: What it is? Could be letters (24) and numbers (8) but then miss 8 and 9 ? ..

// 4976:0362  61 00 62 00 63 00 64 00 65 00 66 00 67 00 68 00
// 4976:0372  69 00 6A 00 6B 00 6C 00 6D 00 6E 00 6F 00 70 00
// 4976:0382  71 00 72 00 73 00 74 00 75 00 76 00 77 00 78 00
// 4976:0392  30 00 31 00 32 00 33 00 34 00 35 00 36 00 37 00

/*static*/ const char _4976_0362[32][2] = {
	"\x61",
	"\x62",
	"\x63",
	"\x64",
	"\x65",
	"\x66",
	"\x67",
	"\x68",
	"\x69",
	"\x6a",
	"\x6b",
	"\x6c",
	"\x6d",
	"\x6e",
	"\x6f",
	"\x70",
	"\x71",
	"\x72",
	"\x73",
	"\x74",
	"\x75",
	"\x76",
	"\x77",
	"\x78",
	"\x30",
	"\x31",
	"\x32",
	"\x33",
	"\x34",
	"\x35",
	"\x36",
	"\x37",
};

// 4976:0262  78 00 00 00 00 00 00 00 79 00 00 00 00 00 00 00  x.......y.......
// 4976:0272  54 48 45 20 00 00 00 00 59 4F 55 20 00 00 00 00  THE ....YOU ....
// 4976:0282  7A 00 00 00 00 00 00 00 7B 00 00 00 00 00 00 00  z.......{.......
// 4976:0292  7C 00 00 00 00 00 00 00 7D 00 00 00 00 00 00 00  |.......}.......
// 4976:02A2  7E 00 00 00 00 00 00 00 7F 00 00 00 00 00 00 00  ~..............
// 4976:02B2  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
// 4976:02C2  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
// 4976:02D2  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................

/*static*/ const char _4976_0262[32][8] = {
	"\x78",
	"\x79",
	"THE",
	"YOU",
	"\x7a",
	"\x7b",
	"\x7c",
	"\x7d",
	"\x7e",
	"\x7f",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
};

// 4976:40E8  00 00 FF 00 01 00 00 01 FF 01 01 01 00 02 FF 02  ................
// 4976:40F8  01 02 FE 02 02 02 00 03 FF 03 01 03 FE 03 02 03  ................
// 4976:4108  00 04 FF 04 01 04 FE 04 02 04 FD 04 03 04 00 FF  ................

//    -3 -2 -1  0 +1 +2 +3
//   |--|--|--|--|--|--|--|
// 4 |21|19|17|16|18|20|22|
//   |--|--|--|--|--|--|--|
// 3 |  |14|12|11|13|15|  |
//   |--|--|--|--|--|--|--|
// 2 |  | 9| 7| 6| 8|10|  |
//   |--|--|--|--|--|--|--|
// 1 |  |  | 4| 3| 5|  |  |
//   |--|--|--|--|--|--|--|
// 0 |  |  | 1| 0| 2|  |  |
//   |--|--|--|--|--|--|--|

/*static*/ const i8 _4976_40e8[23][2] = {
	{ 0, 0},{-1, 0},{ 1, 0},{ 0, 1},{-1, 1},{ 1, 1},{ 0, 2},{-1, 2},
	{ 1, 2},{-2, 2},{ 2, 2},{ 0, 3},{-1, 3},{ 1, 3},{-2, 3},{ 2, 3},
	{ 0, 4},{-1, 4},{ 1, 4},{-2, 4},{ 2, 4},{-3, 4},{ 3, 4},
};

// 4976:4116  00 FF 01 00 FF 01 00 FF 01 FE 02 00 FF 01 FE 02  ................
// 4976:4126  00 FF 01 FE 02 FD 03 00 00 00 01 01 01 02 02 02  ................

/*static*/ const i8 glbTabXAxisDistance[23] = { // (_4976_4116) x-axis distance
	 0,-1, 1, 0,-1, 1, 0,-1, 1,-2, 2, 0,-1, 1,-2, 2,
	 0,-1, 1,-2, 2,-3, 3,
};

// SPX: These are specific commands used in weather/environment definition (category 0x17)
/*static*/ const char EnvCM_CD[] = {"cd"};	// ?
/*static*/ const char EnvCM_FW[] = {"fw"};
/*static*/ const char EnvCM_CD2[] = {"cd"};
/*static*/ const char EnvCM_FW2[] = {"fw"};
/*static*/ const char EnvCM_MV[] = {"mv"};
/*static*/ const char EnvCM_XLocation[] = {"xl"};	// X Location (center) of main image (Skullkeep distant sight)
/*static*/ const char EnvCM_YLocation[] = {"yl"};	// Y Location (center) of main image (Skullkeep distant sight)
/*static*/ const char EnvCM_FD[] = {"fd"};
/*
static const char _4976_4800[] = {"cd"};
static const char _4976_4803[] = {"fw"};
static const char _4976_4730[] = {"cd"};
static const char _4976_4733[] = {"fw"};
static const char _4976_4724[] = {"mv"};
static const char _4976_4727[] = {"xl"};
static const char _4976_472a[] = {"yl"};
static const char _4976_472d[] = {"fd"};
*/
// Here are some examples found for thicket environment:
// cd6000xl48yl32mv1fd16
// cd6001fw8
// cd6002
// cd6004fw32
// cd6005fw8

const U8 _4976_4813[7] = {'T','I','D','S','P','F','G'};

// SPX: Entry number for skill level names (in GDAT2 Cat 0x07/0x00)
// This begins with 0x04 because entries 0x00 to 0x03 are for FIGHTER, ... , WIZARD
// _4976_3d4a renamed SkillLevelNameGDAT2Entry
const U8 tSkillLevelNameGDAT2Entry[15] = {
	0x04, // NEOPHYTE <01>1<00>
	0x05, // NOVICE <01>1<00>
	0x06, // APPRENTICE <01>1<00>
	0x07, // JOURNEYMAN <01>1<00>
	0x08, // CRAFTSMAN <01>1<00>
	0x09, // ARTISAN <01>1<00>
	0x0a, // ADEPT <01>1<00>
	0x0b, // EXPERT <01>1<00>
	0x0c, // {LO} MASTER <01>1<00>
	0x0d, // {UM} MASTER <01>1<00>
	0x0e, // {ON} MASTER <01>1<00>
	0x0f, // {EE} MASTER <01>1<00>
	0x10, // {PAL} MASTER <01>1<00>
	0x11, // {MON} MASTER <01>1<00>
	0x12, // ARCHMASTER <01>1<00>
};

const U16 _4976_46f4[6] = {  0,105,126,126,  0,105};
const U16 _4976_4700[6] = {  0,110,118,118, 38,110};
const U16 _4976_470c[6] = {  0,104,116,116,  0,104};
const U16 _4976_4718[6] = {  0, 82, 90, 90, 37, 82};

const Bit8u * const _4976_4760 = (const Bit8u *)" ";

const U8 _4976_4a04[4] = { 6, 8,18,16}; // n,e,s,w	0.0110/0.1000//1.1000/1.0110

const U16 _4976_3752[42] = { // by wordval 0F-xx-01
0                                                                   , //  0
0                                                                   , //  1 ^ROCKY
0                                         |0x20                     , //  2 ^TIGER STRIPED WORM ^SPECTRE ^MUMMY ^SKELETON ^GHOST
0                         |0x100|0x80                               , //  3 ^TOWER BAT ^CAVERN BAT
0                               |0x80|0x40|0x20     |0x8            , //  4 ^VORTEX
0                                         |0x20                     , //  5 ^GLOP
0                                                                   , //  6 ^GIGGLER
//------------------------------------------------------------------
0                               |0x80               |0x8|0x4        , //  7 ^SCOUT MINION
0                               |0x80                   |0x4        , //  8 ^ATTACK MINION
0                               |0x80               |0x8|0x4        , //  9 ^CARRY MINION
0                               |0x80               |0x8|0x4        , // 10 ^FETCH MINION
0                               |0x80     |0x20         |0x4        , // 11 ^GUARD MINION
0                               |0x80                   |0x4        , // 12 ^U-HAUL MINION
//------------------------------------------------------------------
0                                                               |0x1, // 13 ^MERCHANT ^BAD MERCHANT ^MAGIC MERCHANT
0                                                                   , // 14
0                         |0x100                                    , // 15 ^THORN DEMON
0                                                                   , // 16
0                                                           |0x2    , // 17 ^MERCHANT GUARD
0                               |0x80|0x40|0x20     |0x8            , // 18 ^SPIKED WALL
0                                                                   , // 19 ^TREE GORGON
0                               |0x80                               , // 20 ^SARDES MINION ^EVIL SCOUT MINION
0                                                                   , // 21
0                               |0x80|0x40|0x20     |0x8            , // 22 ^VOID DOOR
0                                                                   , // 23 ^THICKET THEIF ^AXEMAN THIEF
0                                                                   , // 24 ^AXEMAN
0                               |0x80     |0x20                     , // 25 ^EVIL GUARD MINION
0                                         |0x20                     , // 26 ^FLAME ORB
0                                                           |0x2    , // 27
0                                         |0x20                     , // 28 ^DIGGER WORM
//------------------------------------------------------------------
0|0x1000                  |0x100                                    , // 29 ^WOLF
0                                                               |0x1, // 30 ^DRU TAN
0                                    |0x40|0x20                     , // 31 ^PIT GHOST
0                                                               |0x1, // 32 ^DOOR GHOST
//------------------------------------------------------------------
0       |0x800      |0x200                     |0x10|0x8    |0x2|0x1, // 33 ^DRAGOTH
0                   |0x200                                      |0x1, // 34 ^DARK VEXIRK
0                   |0x200                                  |0x2    , // 35 ^VEXIRK KING
0                   |0x200                                          , // 36 ^ARCHER-GUARD
//------------------------------------------------------------------
0                               |0x80                               , // 37 ^EVIL ATTACK MINION
0                               |0x80                               , // 38
0                               |0x80|0x40|0x20     |0x8            , // 39 ^CAVE IN
0             |0x400                                |0x8    |0x2    , // 40 ^CHEST
0                               |0x80                   |0x4        , // 41 ^DRAGOTH ATTACK MINION
};

const i8 glbTabYAxisDistance[23] = { // (_4976_412d) summary position tp viewport distance (0 to 4)
	0,0,0,1,1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,4,4,4,4,
};
const U8 tlbDisplayOrderLeft[25] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24}; // (_4976_439a) left side
const U8 tlbDisplayOrderRight[25] = { 4, 3, 2, 1, 0, 9, 8, 7, 6, 5,14,13,12,11,10,19,18,17,16,15,24,23,22,21,20}; // (_4976_43b3) right side
const U8 tlbDisplayOrderCenter[25] = { 0, 4, 1, 3, 2, 5, 9, 6, 8, 7,10,14,11,13,12,15,19,16,18,17,20,24,21,23,22}; // (_4976_43cc) center

const char * const _089c_00d8 = 
	"\x61\x6E\x69\x6D\x2E\x64\x61\x74\x00";                            // anim.dat.

const U8 _4976_435a[6] = {0x12, 0x10, 0x14, 0x16, 0x18, 0x0E}; // 6arrow to cls4
const U16 _4976_434e[6] = {0x2A, 0x29, 0x2B, 0x2C, 0x2D, 0x28}; // 6arrow to rectno
const U8 _4976_4360[6] = {0, 1, 2, 2, 2, 3}; // 6arrow to moving dir for table.
const U8 _4976_4366[6] = {0, 0, 1, 2, 3, 0}; // 6arrow to moving dir for player.

// 4976:0154  FE 0A/F8 03/07 FD/FA 0A/08 F9/F8 0B/02 FE/08 F5  ................
// 4976:0164  00 00/04 08/F8 F6/06 07/02 02/FA 09/FD F5/02 0B  ................

// 4976:0174  00 F5|FB F9|FD 03|08 0B|00 FB|06 F7|F9 03|05 01  ................
// 4976:0184  04 F9|F8 FD|08 03|FD FE|00 07|FD FC|FC 00|02 F5  ................
const i8 _4976_0154[32][2] = {
	{ -2, 10},
	{ -8,  3},
	{  7, -3},
	{-10, 10},
	{  8, -7},
	{ -8, 11},
	{  2, -2},
	{  8,-11},

	{  0,  0},
	{  4,  8},
	{ -8,-10},
	{  6,  7},
	{  2,  2},
	{ -6,  9},
	{ -3,-11},
	{  2, 11},

	{  0,-11},
	{ -5, -7},
	{ -3,  3},
	{  8, 11},
	{  0, -5},
	{  6, -9},
	{ -7,  3},
	{  5,  1},

	{  4, -7},
	{ -8, -3},
	{  8,  3},
	{ -3, -2},
	{  0,  7},
	{ -3, -4},
	{ -4,  0},
	{  2,-11},
};

const i8 _4976_41d0[] = {0x02, 0x0E, 0x16, 0x16, 0x16, 0x0A, 0x0C}; // table operation 5x5 pos
const i8 _4976_41d7[] = {0x34, 0x40, 0x4E, 0x4E, 0x4E, 0x40, 0x40}; // creature scale while table operation (64=1.0)

const U8 _4976_44bd[] = {0, 3, 2, 1};

const U8 _4976_3d7d[] = {12, 0}; // reset drawtext position

//}}SKCONST




























//SPX: after the symbols:
// Byte 00-03: Runes
// Byte 04: spell difficulty
// Byte 05: specific skill used
// Byte 06-07: Spell type, result and some bits
// SPX: _4976_3e22 replaced by dSpellsTable
//const sk3e22 _4976_3e22[] = {
#if DM2_EXTENDED_MODE == 0
/*const*/ SpellDefinition dSpellsTable[] =
{
	{MkssymVal(s2OH  ,s3IR  ,s4RA  ),0x04,0x11,0x2C03 },	// Long Light
	{MkssymVal(s2DES ,s3IR  ,s4SAR ),0x01,0x0F,0x1813 },	// Darkness
	{MkssymVal(s2YA  ,s3IR  ,s4DAIN),0x04,0x0F,0x3823 },	// Spell Shield
	{MkssymVal(s2OH  ,s3EW  ,s4SAR ),0x03,0x11,0x5833 },	// Invisibility
	{MkssymVal(s2YA  ,s3IR  ,0     ),0x02,0x0F,0x3C43 },	// Magical Shield
	{MkssymVal(s2FUL ,0     ,0     ),0x01,0x10,0x1C53 },	// Light
	{MkssymVal(s2OH  ,s3EW  ,s4DAIN),0x02,0x02,0x3863 },	// Aura of Wisdom
	{MkssymVal(s2OH  ,s3EW  ,s4ROS ),0x02,0x02,0x3873 },	// Aura of Dexterity
	{MkssymVal(s2FUL ,s3BRO ,s4NETA),0x04,0x0F,0x3883 },	// Fire Shield
	{MkssymVal(s2OH  ,s3EW  ,s4NETA),0x02,0x02,0x3893 },	// Aura of Vitality
	{MkssymVal(s2OH  ,s3EW  ,s4KU  ),0x02,0x02,0x38A3 },	// Aura of Strength
	{MkssymVal(s2OH  ,s3IR  ,s4ROS ),0x04,0x03,0x78B3 },	// Aura of Speed
	{MkssymVal(s2ZO  ,s3BRO ,s4ROS ),0x03,0x02,0x78E3 },	// Spell Reflector
	{MkssymVal(s2YA  ,s3EW  ,0     ),0x00,0x03,0x04F3 },	// Magical Marker (15/3E)

	{MkssymVal(s2OH  ,s3VEN ,0     ),0x03,0x13,0x4072 },	// Poison Cloud (0D/07)
	{MkssymVal(s2OH  ,s3KATH,s4RA  ),0x04,0x11,0x3C22 },	// Lightning (0D/02)
	{MkssymVal(s2FUL ,s3IR  ,0     ),0x03,0x10,0x5402 },	// Fireball (0D/00)

	{MkssymVal(s2FUL ,s3BRO ,s4KU  ),0x04,0x0D,0x1C71 },	// NP : STR Potion (13/07)

	{MkssymVal(s2DES ,s3EW  ,0     ),0x01,0x12,0x2832 },	// Antimater (0D/03)
	{MkssymVal(s2DES ,s3VEN ,0     ),0x01,0x13,0x2062 },	// Poison Bolt (0D/06)
	{MkssymVal(s2ZO  ,0     ,0     ),0x01,0x11,0x1C42 },	// Open/Close door (0D/04)

	{MkssymVal(s2YA  ,s3BRO ,0     ),0x02,0x0F,0x30C1 },	// NP : Shield Potion (13/0C)
	{MkssymVal(s2YA  ,0     ,0     ),0x02,0x0D,0x1CB1 },	// NP : Stamina Potion (13/0B)
	{MkssymVal(s2YA  ,s3BRO ,s4DAIN),0x04,0x0D,0x1C81 },	// NP : Wisdom Potion (13/08)
	{MkssymVal(s2YA  ,s3BRO ,s4NETA),0x04,0x0D,0x1C91 },	// NP : Vitality Potion (13/09)
	{MkssymVal(s2VI  ,0     ,0     ),0x01,0x0D,0x40E1 },	// NP : Health Potion (13/0E)
	{MkssymVal(s2VI  ,s3BRO ,0     ),0x01,0x0D,0x34A1 },	// NP : Anti Venin (13/0A)
	{MkssymVal(s2OH  ,s3BRO ,s4ROS ),0x04,0x0D,0x1C61 },	// NP : Dexterity Potion (13/06)
	{MkssymVal(s2ZO  ,s3BRO ,s4RA  ),0x03,0x02,0x80D1 },	// NP : Mana Potion (13/0D)

	{MkssymVal(s2ZO  ,s3EW  ,s4KU  ),0x06,0x03,0x7B14 },	// Attack Minion (0F/31)
	{MkssymVal(s2ZO  ,s3EW  ,s4NETA),0x04,0x0F,0x3F44 },	// Guard Minion (0F/34)
	{MkssymVal(s2ZO  ,s3EW  ,s4ROS ),0x05,0x02,0x3354 },	// U-Haul Minion (0F/35)

	{MkssymVal(s2OH  ,s3KATH,s4KU  ),0x02,0x03,0x5892 },	// Push (0D/09)
	{MkssymVal(s2OH  ,s3KATH,s4ROS ),0x02,0x03,0x58A2 },	// Pull (0D/0A)
};
#elif DM2_EXTENDED_MODE == 1
SpellDefinition dSpellsTable[MAXSPELL];
#endif // DM2_EXTENDED_MODE
// SPX: the last 2 bits tells the type of spell (maybe there are on 4 bits?)
// 01: potion, requires a flask
// 02: missile spell
// 03: general spell
// 04: summon creature

//#define SPELL_TYPE_POTION	1
//#define SPELL_TYPE_MISSILE	2
//#define SPELL_TYPE_GENERAL	3
//#define SPELL_TYPE_SUMMON	4

// In order (for example, PULL = 58A2)
// B1:2 
// B1:2 ?
// B1:2 ?
//	Some bits for ? are for the cooldown. which ones, though?
// B1:2 spell produced (high bits) => 6 bits = 64 possible values (0x3F mask)
// B2:4 spell produced (low bits)
// B2:4	type (only 2 bits used)

// SPX: Making spells definition within GDAT2: (search category 0x2D (0x0D is for spell))
// 00 = Power symbol (not to be used)
// 01 = Symbol 1 (Influence)
// 02 = Symbol 2 (Form)
// 03 = Symbol 3 (Alignment)
// 04 = Difficulty
// 05 = Skill used
// 06 = Spell type
// 07 = Spell result
// 08 = Cooldown

// SPX: For rememberence, the non DM2 spells are (DM1 / NEXUS):
// YA BRO ROS: Magic Footprints
// YA VEN SAR: Petrify
// VI BRO NETA: Restore Health
// VI BRO RA: Restore Health for Party
// OH EW RA: See Through Walls (Window)
// ZO KATH RA: ZoKathRa Spell


//SPX: spotted at 0x76612 in Skull.exe
//4976:3E22  76 6F 68#00 04 11 03 2C|77 6F 6A#00 01 0F 13 18
//4976:3E32  74 6F 66#00 04 0F 23 38|77 6D 68#00 03 11 33 58
//4976:3E42  00 6F 66#00 02 0F 43 3C|00 00 69#00 01 10 53 1C
//4976:3E52  74 6D 68#00 02 02 63 38|73 6D 68#00 02 02 73 38
//4976:3E62  75 70 69#00 04 0F 83 38|75 6D 68#00 02 02 93 38
//4976:3E72  72 6D 68#00 02 02 A3 38|73 6F 68#00 04 03 B3 78
//4976:3E82  73 70 6B#00 03 02 E3 78|00 6D 66#00 00 03 F3 04
//4976:3E92  00 6C 68#00 03 13 72 40|76 6E 68#00 04 11 22 3C
//4976:3EA2  00 6F 69#00 03 10 02 54|72 70 69#00 04 0D 71 1C
//4976:3EB2  00 6D 6A#00 01 12 32 28|00 6C 6A#00 01 13 62 20
//4976:3EC2  00 00 6B#00 01 11 42 1C|00 70 66#00 02 0F C1 30
//4976:3ED2  00 00 66#00 02 0D B1 1C|74 70 66#00 04 0D 81 1C
//4976:3EE2  75 70 66#00 04 0D 91 1C|00 00 67#00 01 0D E1 40
//4976:3EF2  00 70 67#00 01 0D A1 34|73 70 68#00 04 0D 61 1C
//4976:3F02  76 70 6B#00 03 02 D1 80|72 6D 6B#00 06 03 14 7B
//4976:3F12  75 6D 6B#00 04 0F 44 3F|73 6D 6B#00 05 02 54 33
//4976:3F22  72 6E 68#00 02 03 92 58|73 6E 68#00 02 03 A2 58



const ObjectID oFF80 = ObjectID::Raw(0xFF80); // fireball
const ObjectID oFF81 = ObjectID::Raw(0xFF81); // unused in DM2, but would be the slot for poison blob (CSB)
const ObjectID oFF82 = ObjectID::Raw(0xFF82); // lightning bolt
const ObjectID oFF83 = ObjectID::Raw(0xFF83); // harm unmaterialed
const ObjectID oFF84 = ObjectID::Raw(0xFF84); // open door
const ObjectID oFF86 = ObjectID::Raw(0xFF86); // poison bolt
const ObjectID oFF87 = ObjectID::Raw(0xFF87); // poison cloud
const ObjectID oFF89 = ObjectID::Raw(0xFF89); // push
const ObjectID oFF8A = ObjectID::Raw(0xFF8A); // pull
// 0xFF8B was Thunder in beta
// 0xFF8C was Electric Shock in beta
const ObjectID oFF8D = ObjectID::Raw(0xFF8D); // ? ?
const ObjectID oFF8E = ObjectID::Raw(0xFF8E); // reflect spell
// SPX, the 0x0F (oFF8F) spell is called from YAEW spell which creates an item.
// The item created is found within the spell 0F in GDAT2.
const ObjectID oFFA8 = ObjectID::Raw(0xFFA8); // dust cloud
const ObjectID oFFB0 = ObjectID::Raw(0xFFB0); // thunder bolt
const ObjectID oFFB1 = ObjectID::Raw(0xFFB1); // electric shock
const ObjectID oFFE4 = ObjectID::Raw(0xFFE4); // rebirth effect
const ObjectID oFFFE = ObjectID::Raw(0xFFFE); // end of the record link
const ObjectID oFFFF = ObjectID::Raw(0xFFFF); // mark as free record


AIDefinition dAITable[MAXAI];


//{{SKVAL1
U16 _04bf_188e[10];

// "Rr" ... 変数項の占有メモリ領域を確認し，RCJを付与済み
// "Rr`" ... "Rr"中，RCJを部分的に施した
// "Rp" ... 変数項の占有メモリは追跡しないことを決定し，RCJを付与しない

i8 _4976_0094[32]; //Rr
U8 _4976_00b4[8]; //Rr  // flags for 8 missiles. 2=can harm door
X8 _4976_00e3[5]; //Rr
U16 tRectnoOffsetsWallOrnates[16]; //Rr (_4976_019a) // recto offsets tables for wall ornates

i8 _4976_19b2[4]; //Rr
i8 _4976_19b6[4]; //Rr

X16 _4976_38e8[4]; //Rr
U8 _4976_3932[28]; //Rr
U8 _4976_3b6f[5]; //Rr
X16 _4976_3d5b[9]; //Rr
X16 _4976_3d6d[6]; //Rr
U8 _4976_3e1e[4]; //Rr
U8 _4976_3fc8[6]; //Rr

i8 _4976_406c[4][4]; //Rp
i8 _4976_407c[4][4]; //Rp
i8 _4976_408c[21][4]; //Rr
i8 tlbDoorSideFramesReorder[23]; //Rr
i8 _4976_415b[23]; //Rr
i8 _4976_4172[23]; //Rr
U8 tlbDistanceStretch[5]; //Rr (_4976_4189) // distance to creature scale factor
U8 _4976_418e[6][4]; //Rr`
U8 _4976_41a9[7]; //Rr
U8 _4976_41b0[16][2]; //Rr
i8 _4976_41de[8]; //Rr
U8 _4976_41e6[7]; //Rr
U8 _4976_41ed[17]; //Rr
U8 tlbRainScene[5][4]; //Rr	// (_4976_41fe)
i8 _4976_421b[6]; //Rr
i8 _4976_4221[5]; //Rr
i8 _4976_4226[5]; //Rr
i8 _4976_422b[5]; //Rr
X8 _4976_4230[23]; //Rr
X8 _4976_4247[23]; //Rr
X16 _4976_425e[9]; //Rr
X8 tlbGraphicsPitRoof[9]; //Rr
U8 _4976_4279[9]; //Rr
X16 _4976_4282[16]; //Rr
U8 tlbGraphicsFlip[16]; //Rr
X8 tlbGraphicsPitHidden[16]; //Rr
X8 tlbGraphicsPitNormal[16]; //Rr
X16 _4976_42d2[16]; //Rr
X8 _4976_42f2[23][4]; //Rr
i8 _4976_436c[4][5]; //Rr
U8 _4976_4380[4][2]; //Rr
i8 _4976_4388[4][4]; //Rr
i8 _4976_4398[2]; //Rr
X8 _4976_43e5[16]; //Rr
i8 _4976_43f5[16][2]; //Rr
i8 _4976_4415[25][2]; //Rr
U8 _4976_4447[3][4]; //Rr
U8 _4976_4453[16]; //Rr
i8 _4976_4463[23]; //Rr
i8 _4976_447a[4][8][2]; //Rp
i8 _4976_44ba[3]; //Rr
i8 _4976_44c1[4]; //Rr
U8 _4976_44c5[16][2]; //Rr
i8 tlbRectnoDoorButton[14]; //Rr _4976_44e5
X8 tlbGraphicsDoorSideFrames[14][2]; //Rr
X8 tlbGraphicsDoorRoofSlits[14]; //Rr
X16 tlbRectnoDoorRoofSlits[14]; //Rr
i8 tlbInvertedDoorDistance[5]; //Rr
X16 tlbRectnoDoorPosition[16]; //Rr
U8 _4976_455e[16]; //Rr
X8 tlbGraphicsStairsFront1[16][2]; //Rr
X8 tlbGraphicsStairsFront2[16][2]; //Rr
X16 _4976_45ae[16][2]; //Rr
X8 tlbGraphicsStairsSide[9][2]; //Rr
X16 _4976_4600[9][2]; //Rr
X16 _4976_4624[6]; //Rr
X16 _4976_4630[6]; //Rr
X16 _4976_463c[6]; //Rr
X16 _4976_4648[6]; //Rr
i8 _4976_4654[23]; //Rr
U8 _4976_466b[20]; //Rr
X32 _4976_4684[4]; //Rr
X32 _4976_4694[8]; //Rr
X32 _4976_46b4[8]; //Rr
X32 _4976_46d4[8]; //Rr
i8 _4976_4764[24]; //Rr
i8 tlbRainRelated[128]; //Rr	(_4976_477c)
X16 _4976_481a[15]; //Rr
U8 _4976_483f[5]; //Rr
i8 _4976_484f[4]; //Rr
U8 _4976_5268[40]; //Rp
U8 _4976_4844[7]; //Rp
sk0d9e _4976_0d9e[257]; //Rp
sk13a4 _4976_13a4[116]; //Rp
U8 _4976_169c[81]; //Rp
sk1891 _4976_1574[74]; //Rp
sk1891 _4976_1891[10]; //Rp
U32 blk1 = 0xfaceface;
sk16ed _4976_16ed[60]; //Rp

U16	_04bf_05f9[18]; //Rr
X16	_04bf_02ac[8]; //Rr
X16	_04bf_029c[8]; //Rr
U8	_04bf_067f[256]; //Rp
U16	_04bf_0e6a[8]; //Rp
U16	_04bf_0e54[8]; //Rp
sk01b2	_04bf_01b2[4]; //Rr
U8 _04bf_03d6[220]; //Rr
U8 _4976_18b9[236]; //Rr
U8	_4976_0252[16]; //Rr
U8	_4976_3b5f[9]; //Rp
X8	_4976_4736[12]; //Rp
U16	_4976_404d[5][3]; //Rp
U16	_4976_49e8[13]; //Rr // player item holding mask
U8	_4976_4862[96]; //Rp
U8	_4976_48c2[128]; //Rp
U8	_4976_00bc[8][4]; //Rp
sk3b74	_4976_3b74[55]; //Rp
sk0cea	_4976_0ce0[19]; //Rp
U16		_4976_49d4[5][2]; //Rp
U8		_4976_3fce[30]; //Rp
U8		_4976_37a6[86]; //Rr
U8	_04bf_02be[128]; //Rp // rawkey to ascii
U8	_04bf_033e[128]; //Rp // rawkey+shift to ascii
U8	_4976_00dc[8]; //Rr
U16	_4976_014c[4]; //Rr
U16	_4976_01bc[2][8]; //Rr
i16	_4976_3924[4]; //Rr
U8	_4976_392c[4]; //Rr
i8	tLightLevelItem[16]; //Rr
i8	tLightLevelTable[6]; //Rr // (_4976_3d44) Light level correspondance
U8	_4976_3dde[4]; //Rr // each player dir?
U16	_4976_3de2[6]; //Rp // players in squad position list?
U16	_4976_3de6[4][2]; //Rr
U8	_4976_3df9[7]; //Rr
U8	_4976_4980[4]; //Rp
U8	_4976_4984[4]; //Rp
sk38f4	_4976_38f4[12]; //Rr
U8	_089c_00e3[256]; //Rp
//}}SKVAL1

const Bit8u * const _4976_498e = _4976_4980;
const Bit8u * const _4976_4992 = _4976_4984;
const Bit8u * const _4976_4756 = _4976_4760;


















//{{SKSAVE_OUTSORTED_VARS
U8	cram[MAX_CRAM];
U8	cems[MAX_CEMS];
U8	vram[MAX_VRAM];

U8 	_04bf_09f0[256][3];	// palette?
//}}SKSAVE_OUTSORTED_VARS














// lodsb

/*inline*/ U8 lodsb(const U8 *&dssi) {
	return *(dssi++);
}
/*inline*/ U8 lodsb(U8 *&dssi) {
	return *(dssi++);
}

// stosb

/*inline*/ void stosb(U8 *&esdi, U8 al) {
	*(esdi++) = al;
}

// lodsw 

/*inline*/ U16 lodsw(const U8 *&dssi) {
	U16 r = *dssi; dssi++;
	r |= *dssi << 8; dssi++;
	return r;
}
/*inline*/ U16 lodsw(U8 *&dssi) {
	U16 r = *dssi; dssi++;
	r |= *dssi << 8; dssi++;
	return r;
}

// stosw

/*inline*/ void stosw(U8 *&esdi, U16 ax) {
	*esdi = U8(ax); esdi++;
	*esdi = U8(ax >> 8); esdi++;
}

// movsb

/*inline*/ void movsb(U8 *&esdi, const U8 *&dssi) {
	*esdi = *dssi; esdi++; dssi++;
}
/*inline*/ void movsb(U8 *&esdi, U8 *&dssi) {
	*esdi = *dssi; esdi++; dssi++;
}

// movsw

/*inline*/ void movsw(U8 *&esdi, const U8 *&dssi) {
	*esdi = *dssi; esdi++; dssi++;
	*esdi = *dssi; esdi++; dssi++;
}
/*inline*/ void movsw(U8 *&esdi, U8 *&dssi) {
	*esdi = *dssi; esdi++; dssi++;
	*esdi = *dssi; esdi++; dssi++;
}

/*inline*/ unsigned RCJ(unsigned cx, unsigned x) {
	ATLASSERT(x < cx);
	return x;
}







// SPX BEGIN -- SOME UTILS FUNCTIONS
// Sometime required for converting values into more suitable structure or type
// It also allows to spot places where types are not matching

// Needed when tile value is between -3 and 4
tileTypeIndex UtilConvertU16ToTile(__int16 u16tileType)
{
	tileTypeIndex	tileType;

	switch(u16tileType)
	{
		case ttWall:
		case ttFloor:
		case ttPit:
		case ttStairs:
		case ttDoor:
		case ttMapExit:
		case ttTrickWall:
		case ttTeleporter:
			tileType = (enum DMEncyclopaedia::tileTypeIndex) u16tileType;
			break;
		case -3:
			tileType = ttTeleporter;
			break;
		case -2:
			tileType = ttTrickWall;
			break;
		case -1:
			tileType = ttMapExit;
			break;
	}

	return tileType;
}

// For debug purpose, added this special pointer check
bool	CheckSafePointer(void* p)
{
	if (p == NULL ||
		p <  (void*)0x00010000 ||	// low pages
		p == (void*)0xCCCCCCCC ||	// bypass uninitialized memory
		p == (void*)0xCDCDCDCD ||	// bypass clean debug unallocated memory
		p == (void*)0xDDDDDDDD ||	// bypass dead debug deallocated memory
		p == (void*)0xE5000000 ||	// reserved video memory
		p == (void*)0xEEFEEEFE ||	//
		p == (void*)0xFDFDFDFD ||	// malloc border values
		p == (void*)0xFEEEFEEA ||	// 
		p == (void*)0xEEFEEEFE ||	// debug dallocated memory
		p == (void*)0xFFFFFFFF ||	// obviously something unset
		(p >= (void*)0x78000000 && p < (void*)0x7FFF0000) ||
		p >= (void*)0x7FFF0000)		// protected area + system space
	{
		if (p != NULL)
		{
			char strMemMessage[128];
			sprintf(strMemMessage, "bad pointer");
			if (p < (void*) 0x0001000)
				sprintf(strMemMessage, "low pages");
			else if (p == (void*) 0xCCCCCCCC)
				sprintf(strMemMessage, "bypass uninitialized memory");
			else if (p == (void*) 0xCDCDCDCD)
				sprintf(strMemMessage, "bypass clean debug unallocated memory");
			else if (p == (void*) 0xDDDDDDDD)
				sprintf(strMemMessage, "bypass dead debug deallocated memory");
			else if (p == (void*) 0xE5000000)
				sprintf(strMemMessage, "reserved video memory");
			else if (p == (void*) 0xEEFEEEFE)
				sprintf(strMemMessage, "???");
			else if (p == (void*) 0xFDFDFDFD)
				sprintf(strMemMessage, "malloc border values");
			else if (p == (void*) 0xFEEEFEEA)
				sprintf(strMemMessage, "???");
			else if (p == (void*) 0xEEFEEEFE)
				sprintf(strMemMessage, "debug dallocated memory");
			else if (p == (void*) 0xFFFFFFFF)
				sprintf(strMemMessage, "obviously something unset");
			else if (p >= (void*)0x78000000 && p < (void*)0x7FFF0000)
				sprintf(strMemMessage, "unallocated memory space");
			else if (p >= (void*) 0x7FFF0000)
				sprintf(strMemMessage, "protected area + system space");
			SkD((DLV_BUGHERE, "Bad pointer adress @%p (%s)\n", p, strMemMessage));
		}
		return false;
	}
	return true;
}

// SPX END