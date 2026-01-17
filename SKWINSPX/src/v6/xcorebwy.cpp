//--- SkWinCoreBW -------------------------------------
//SPX: Experimental code about Bloodwych dungeon reading

#include <StdAfx.h>	// Required for MVC6 compilation

#include <skparam.h>
#include <skdebug.h>
#include <skglobal.h>
#include <skcore.h>
#include <skmidi.h>
#include <skcnsole.h>

#include <stdlib.h> // calloc
#include <string.h>

#define MIN(a,b) ((a)>(b)?(b):(a))

//------------------------------------------------------------------------------
// BLOODWYCH enum

enum {
	BW_SWITCHFUNC__REMOVE = 2,
	BW_SWITCHFUNC__TOGGLE_STONE_WALL = 4,
	BW_SWITCHFUNC__OPEN_VOIDLOCK_DOOR = 6,
	BW_SWITCHFUNC__ROTATE_BRICKWALL = 8,
	BW_SWITCHFUNC__TOGGLE_PILLAR = 10,
	BW_SWITCHFUNC__PLACE_PILLAR = 12,
	BW_SWITCHFUNC__ROTATE_WOODWALL = 14
} tSwitchFunction;


enum {
	BW_WALL_ALCOVE_SHELF = 0,
	BW_WALL_TAPESTRY = 1,
	BW_WALL_SWITCH_BUTTON = 2,
	BW_WALL_GEM_SLOT = 3
} tWallType;


union tUWallFunction {
    struct {
		unsigned char wtype:2;	// shelf, sign, switch, socket
		unsigned char state:1;	// 0 = off, 1 = on
		unsigned char id:5;	// switch id (32 values)
    } switchbutton;
    struct {
		unsigned char wtype:2;	// shelf, sign, switch, socket
		unsigned char id:6;	// id (64 values)
    } generic;
};



typedef struct {
//	unsigned short wtype:2;	// shelf, sign, switch, socket
//	unsigned short state:1;	// 0 = off, 1 = on
//	unsigned short id:5;	// switch id
	union tUWallFunction wallfunc;	// 8 low bits

	unsigned char ttype:4;		// tile type
	unsigned char facing:2;	// order : N/E/S/W
	unsigned char tt:1;
	unsigned char special:1;	// special wall = shelf, sign, switch, socket
} tBWTileWall;


// SPX: Switch/Trigger function
const char* BW_GetSwitchFunctionName(U8 iFuncNo)
{
	static char text[128] = "???";

	if (iFuncNo == 0) return "NONE";
	if (iFuncNo == 2) return "REMOVE PILLAR/WALL";
	if (iFuncNo == 4) return "TOGGLE STONE WALL";
	if (iFuncNo == 6) return "OPEN VOID-LOCK DOOR";
	if (iFuncNo == 8) return "ROTATE WALL";
	if (iFuncNo == 10) return "TOGGLE PILLAR";
	if (iFuncNo == 12) return "PLACE PILLAR";
	if (iFuncNo == 14) return "ROTATE WOOD WALL";

	return text;
};


// SPX: Special wall type
const char* BW_GetSpecialWallTypeName(U8 iWallType)
{
	static char text[128] = "???";

	if (iWallType == 0) return "SHELVES";
	if (iWallType == 1) return "TAPESTRY/MESSAGE";
	if (iWallType == 2) return "SWITCH/BUTTON";
	if (iWallType == 3) return "GEMSLOT";
	return text;
};


const char* BW_GetCreatureName(U8 ct)
{
	static Bit8u text[128] = {0};
	if (ct == 0)			return "BLODWYN";
	if (ct == 1)			return "MURLOCK";
	if (ct == 2)			return "ELEANOR";
	if (ct == 3)			return "ROSANNE";
	if (ct == 4)			return "ASTROTH";
	if (ct == 5)			return "ZOTHEN";
	if (ct == 6)			return "BALDRIC";
	if (ct == 7)			return "ELFRIC";
	if (ct == 8)			return "SIR EDWARD LION";
	if (ct == 9)			return "MEGRIM";
	if (ct == 0x0A)			return "SETHRA";
	if (ct == 0x0B)			return "MR. FLAY";
	if (ct == 0x0C)			return "ULRICH";
	if (ct == 0x0D)			return "ZASTAPH";
	if (ct == 0x0E)			return "HENGIST";
	if (ct == 0x0F)			return "THAI CHANG";

	if (ct == 0x10)			return "NINJA - GREY";
	if (ct == 0x11)			return "SKELETON - WHITE";
	if (ct == 0x12)			return "CHAINMAIL - GREY";
	if (ct == 0x13)			return "LIZARD - BLUE";
	if (ct == 0x14)			return "SPECTRE WARRIOR";
	if (ct == 0x15)			return "WARRIOR - BLUE/RED";
	if (ct == 0x16)			return "LIZARD - GREY/BLUE/RED";
	if (ct == 0x17)			return "DEMON - BLUE";
	if (ct == 0x18)			return "DEMON - GREEN";
	if (ct == 0x19)			return "DEMON - YELLOW";
	if (ct == 0x1A)			return "DEMON - BRONZE";
	if (ct == 0x1B)			return "DEMON - RED";
	if (ct == 0x1C)			return "NINJA - BLUE";
	if (ct == 0x1D)			return "NINJA - YELLOW";
	if (ct == 0x1E)			return "NINJA - TAN";
	if (ct == 0x1F)			return "WARRIORESS - RED";

	if (ct == 0x20)			return "WARRIORESS - GREY/CHAINMAIL";
	if (ct == 0x21)			return "WARRIORESS - GREY";
	if (ct == 0x22)			return "WARRIORESS - RED";
	if (ct == 0x23)			return "WARRIORESS - YELLOW";
	if (ct == 0x24)			return "WARRIORESS - YELLOW/PLATED";
	if (ct == 0x25)			return "LIZARD DEMON - BLUE";
	if (ct == 0x26)			return "LIZARD DEMON - RED";
	if (ct == 0x27)			return "ORC DEMON - GREEN";
	if (ct == 0x28)			return "ORC DEMON - ORANGE";
	if (ct == 0x29)			return "KNIGHT - GREY";
	if (ct == 0x2A)			return "KNIGHT - TAN";
	if (ct == 0x2B)			return "KNIGHT - YELLOW";
	if (ct == 0x2C)			return "KNIGHT - RED";
	if (ct == 0x2D)			return "KNIGHT - BLUE";
	if (ct == 0x2E)			return "KNIGHT - GREEN";
	if (ct == 0x2F)			return "ORC RED - GREEN";
	
	if (ct == 0x30)			return "LIZARD - GREEN/RED";
	if (ct == 0x31)			return "ORC - GREEN/BLUE";
	if (ct == 0x32)			return "WARRIOR - CHAINMAIL YELLOW/RED";
	if (ct == 0x33)			return "WARRIOR - CHAINMAIL BLUE/YELLOW";
	if (ct == 0x34)			return "WARRIOR - YELLOW/GREY/RED";
	if (ct == 0x35)			return "WARRIOR - GREY/RED/BLUE";
	if (ct == 0x36)			return "WARRIOR - RED/GREEN/GREY";
	if (ct == 0x37)			return "WARRIOR - RED/GREY/GREEN";
	if (ct == 0x38)			return "HOODED WARRIOR - GREY/RED/BLUE";
	if (ct == 0x39)			return "HOODED WARRIOR - BLUE";
	if (ct == 0x3A)			return "HOODED WARRIOR - BLUISH";
	if (ct == 0x3B)			return "HOODED WARRIOR - RED/YELLOW/GREY";
	if (ct == 0x3C)			return "CAPED WARRIOR - GREEN/GREY";
	if (ct == 0x3D)			return "CAPED WARRIOR - YELLOW/BLUE/RED";
	if (ct == 0x3E)			return "CAPED WARRIOR - GREY";
	if (ct == 0x3F)			return "CAPED WARRIOR - RED/BLUE/GREEN";

	if (ct == 0x40)			return "KING WARRIOR";
	if (ct == 0x41)			return "QUEEN WARRIORESS";
	if (ct == 0x42)			return "KNIGHT - BLACK";
	if (ct == 0x43)			return "WARRIOR CHAINMAIL - YELLOW/RED";
	if (ct == 0x44)			return "WARRIOR CHAINMAIL - YELLOW/GREEN";
	if (ct == 0x45)			return "WARRIOR CHAINMAIL - YELLOW/BLUE";
	if (ct == 0x46)			return "ORC CHAINMAIL - YELLOW/RED";
	if (ct == 0x47)			return "SKELETON - BLACK";
	if (ct == 0x48)			return "SKELETON - TAN";
	if (ct == 0x49)			return "SKELETON - GREEN";
	if (ct == 0x4A)			return "SKELETON - WHITE/GREEN";
	if (ct == 0x4B)			return "SKELETON - YELLOW";
	if (ct == 0x4C)			return "LIZARD CHAIMAIL - RED/YELLOW";
	if (ct == 0x4D)			return "ORC CHAINMAIL - YELLOW/GREEN";
	if (ct == 0x4E)			return "ORC - RED";
	if (ct == 0x4F)			return "LIZARD - RED";

	if (ct == 0x50)			return "GOLEM - GREY/GREEN";
	if (ct == 0x51)			return "GOLEM - RED";
	if (ct == 0x52)			return "GOLEM - YELLOW";
	if (ct == 0x53)			return "GOLEM - ORANGE";
	if (ct == 0x54)			return "GOLEM - YELLOW/GREEN 1";
	if (ct == 0x55)			return "GOLEM - YELLOW/GREEN 2";

	if (ct == 0x64)			return "FAT GUY 1";
	if (ct == 0x65)			return "FAT GUY 2";
	if (ct == 0x66)			return "BEHOLDER";
	if (ct == 0x67)			return "CRAB-HOLDER";
	if (ct == 0x68)			return "CRAB";
	if (ct == 0x69)			return "DRAGON";
	if (ct == 0x6A)			return "DRAGONLING";
	if (ct == 0x6B)			return "ZENDIK";

	if (text[0] != 0)
		return (LPCSTR)text;
	return "UNKNOWN ?";
}


int iNumTileStartOffset[8];
int iNbTilesPerMap[8];
int iGlobalMapsDimX[8];


int GetCoordinatesFromTileNumber(int iTileNumber, int* iCoordsMap, int* iCoordsX, int* iCoordsY)
{
	// depending on the tilenumber, must determine in which map it is
	int iMapWidth = -1;
	int iMapIndex = 0;
	int iMap = 0;
	int iTileX = 0;
	int iTileY = 0;
	
  	for (iMapIndex = 0; iMapIndex < 8; iMapIndex++)
  	{
  		int iTileMin = 0;
  		int iTileMax = 0;
  		iTileMin = iNumTileStartOffset[iMapIndex];
  		iTileMax = iNumTileStartOffset[iMapIndex] + iNbTilesPerMap[iMapIndex] - 1;
  		if (iTileMin <= iTileNumber && iTileNumber < iTileMax)
  		{
  			iMap = iMapIndex;
  			iMapWidth = iGlobalMapsDimX[iMapIndex];
			iTileNumber = iTileNumber - iTileMin;	// absolute tilenumber - offset tilenumber = tilenumber within current map
  			break;
  		}
  	}
  	
    iTileX = iTileNumber%iMapWidth;
    iTileY = iTileNumber/iMapWidth;
   	
   	*iCoordsX = iTileX;
   	*iCoordsY = iTileY;
   	*iCoordsMap = iMap;
		
	//printf("Transforming tile number %04X to coordinates (%d, %02d,%02d)\n", iTileNumber, *iCoordsMap, *iCoordsX, *iCoordsY);

	return 1;
}


int GetDBItemFromAbsoluteID(int iBWItemID)
{
	int iDBType = DB_CATEGORY_MISC_ITEM;

	if (iBWItemID >= 3 && iBWItemID <= 4)
		iDBType = DB_CATEGORY_WEAPON;
	if (iBWItemID >= 23 && iBWItemID <= 26)
		iDBType = DB_CATEGORY_POTION;
	if (iBWItemID >= 27 && iBWItemID <= 47)
		iDBType = DB_CATEGORY_CLOTHING;
	if (iBWItemID >= 48 && iBWItemID <= 63)
		iDBType = DB_CATEGORY_WEAPON;
	if (iBWItemID >= 87 && iBWItemID <= 94)
		iDBType = DB_CATEGORY_WEAPON;
	if (iBWItemID >= 104 && iBWItemID <= 109)
		iDBType = DB_CATEGORY_WEAPON;

	return iDBType;
}

/*
int tblCustomNbItemAllocationPerDB[16] = 
{
	256, 512, 256, 512, 
	75, 100, 60, 25, 
	12, 5, 200, 0, 
	0, 0, 60, 50
};
*/

extern ObjectID* globalGroundObjects; // for all tiles


U16* xInterWalls = NULL;


U16 SkWinCore::UNLOCK_INTERWALL_DOOR(U16 iMap, U16 iPosX, U16 iPosY, U16 iDir)
{
	int iUnlockStatus = 0;

	iUnlockStatus += UNLOCK_INTERWALL_DOOR_STATE(iMap, iPosX, iPosY, iDir);
	iUnlockStatus += UNLOCK_INTERWALL_DOOR_STATE(iMap, iPosX+glbXAxisDelta[iDir], iPosY+glbYAxisDelta[iDir], (iDir+2)%4);

	return iUnlockStatus;
}

U16 SkWinCore::UNLOCK_INTERWALL_DOOR_STATE(X16 iMap, X16 iPosX, X16 iPosY, X8 iFace)
{
	int iSwitchStatus = 0;

	int z;
	int iInterwall = 0;
	int iInterwallTile = 0;
	int iTileType = 0;
	int iDoorClosed = 0;
	int iInterDoorStatus = 0;
	int iIsDoorFlag = 0;
	int iNewMask = 0xFFFF;
	int iNewDoorStatus = 0;
	
	if (iPosX < 0 || iPosX >= 32 || iPosY < 0 || iPosY >= 32 || iMap < 0 || iMap >= 8)
		return 0;
	z = iPosX + (iPosY*32) + (iMap*1024);
	
	if (xInterWalls == NULL)
		return 0;

	iInterwallTile = xInterWalls[z];
	iTileType = (iInterwallTile>>8) & 0x0F;
	iIsDoorFlag = (iInterwallTile>>(iFace*2))&0x02;
	if (iTileType == 2 && iIsDoorFlag)
	{ 
		int iLocked = (iInterwallTile & 0x1000);
		if (iLocked)
			xInterWalls[z] = (xInterWalls[z] & 0x0FFF);	// this might be wrong for tile having 0x4xxx

	}

	return iSwitchStatus;
}


U16 SkWinCore::TRY_SWITCH_INTERWALL_DOOR_STATUS(U16 iMap, U16 iPosX, U16 iPosY, U16 iDir)
{
	int iSwitchStatus = 0;

	iSwitchStatus += SWITCH_INTERWALL_DOOR_STATE(iMap, iPosX, iPosY, iDir);
	iSwitchStatus += SWITCH_INTERWALL_DOOR_STATE(iMap, iPosX+glbXAxisDelta[iDir], iPosY+glbYAxisDelta[iDir], (iDir+2)%4);

	return iSwitchStatus;
}


U16 SkWinCore::SWITCH_INTERWALL_DOOR_STATE(X16 iMap, X16 iPosX, X16 iPosY, X8 iFace)
{
	int iSwitchStatus = 0;

	int z;
	int iInterwall = 0;
	int iInterwallTile = 0;
	int iTileType = 0;
	int iDoorClosed = 0;
	int iInterDoorStatus = 0;
	int iIsDoorFlag = 0;
	int iNewMask = 0xFFFF;
	int iNewDoorStatus = 0;
	
	if (iPosX < 0 || iPosX >= 32 || iPosY < 0 || iPosY >= 32 || iMap < 0 || iMap >= 8)
		return 0;
	z = iPosX + (iPosY*32) + (iMap*1024);
	
	if (xInterWalls == NULL)
		return 0;

	iInterwallTile = xInterWalls[z];
	iTileType = (iInterwallTile>>8) & 0x0F;
	iIsDoorFlag = (iInterwallTile>>(iFace*2))&0x02;
	if (iTileType == 2 && iIsDoorFlag)
	{ 
		int iLocked = (iInterwallTile & 0x1000);
		if (iLocked)
			return 1;

		iInterDoorStatus = (iInterwallTile>>(iFace*2))&0x03;
		iDoorClosed = (iInterDoorStatus)&0x01;
		iDoorClosed = !iDoorClosed;
		iNewDoorStatus = 2 + iDoorClosed;

		switch (iFace)
		{
			case 0: iNewMask = 0xFFFC; break;
			case 1: iNewMask = 0xFFF3; break;
			case 2: iNewMask = 0xFFCF; break;
			case 3: iNewMask = 0xFF3F; break;
		}

		xInterWalls[z] = (xInterWalls[z] & iNewMask) + (iNewDoorStatus<<(iFace*2));

	}

	return iSwitchStatus;
}


U16 SkWinCore::GET_INTERWALL_VALUE(X16 iMap, X16 iPosX, X16 iPosY, X8 iFace)
{
	int z;
	int iInterwall = 0;
	int iInterwallTile = 0;
	int iTileType = 0;
	//int iInterwallType = 0;	// 1 = wall / 2 = door open / 3 = door closed
	if (iPosX < 0 || iPosX >= 32 || iPosY < 0 || iPosY >= 32 || iMap < 0 || iMap >= 8)
		return 0;
	z = iPosX + (iPosY*32) + (iMap*1024);
	
	if (xInterWalls == NULL)
		return 0;

	iInterwallTile = xInterWalls[z];
	iTileType = (iInterwallTile>>8) & 0x0F;
	if (iTileType == 2)
	{
		//iInterwallTile = iInterwallTile & 0x55;
		iInterwallTile = iInterwallTile & 0xFF;
		if (iFace == 0 && (iInterwallTile & 0x03))
			iInterwall = (iInterwallTile & 0x03);
		else if (iFace == 1 && (iInterwallTile & 0x0C))
			iInterwall = ((iInterwallTile & 0x0C)>>2);
		else if (iFace == 2 && (iInterwallTile & 0x30))
			iInterwall = ((iInterwallTile & 0x30)>>4);
		else if (iFace == 3 && (iInterwallTile & 0xC0))
			iInterwall = ((iInterwallTile & 0xC0)>>6);
	}
	return iInterwall;
}

// Required to check the correct value between two tiles, as the interwall value (if present) can be either on any of both tiles
U16 SkWinCore::GET_INTERWALL_VALUE_MERGED(U16 iInterwallValue1, U16 iInterwallValue2)
{
	int iInterwall = 0;
	if (iInterwallValue1 == iInterwallValue2)
		return iInterwallValue1;
	
	if ((iInterwallValue1 == 0 || iInterwallValue1 == 2) && (iInterwallValue2 == 0 || iInterwallValue2 == 2)) // no interwall or open
		return 0;
	
	if (iInterwallValue1 == 3 || iInterwallValue2 == 3) // closed but this is for test
		return 1;

	// else either wall or closed, consider it then as wall
	return 1;
}

char XTESTBUF[73][10][64];

#if defined (XDMX_BLOODWYCH_ENGINE)
Bit8u* SkWinCore::QUERY_BWDAT_TEXT(U8 iTextID, U8* sTextBuffer)
{
	*sTextBuffer = 0;
	char* pTextBuffer = (char*)sTextBuffer;
	int iRowLine = 3;

	for (iRowLine = 3; iRowLine < 10; iRowLine++) {
		strcat((char*)pTextBuffer, XTESTBUF[iTextID][iRowLine]);
		pTextBuffer += strlen(XTESTBUF[iTextID][iRowLine]);
		if (iRowLine < 8) {
			pTextBuffer[0] = vbLf;
			pTextBuffer++;
		}
	}
	//strcpy((char*)sTextBuffer, XTESTBUF[3]);
	//sprintf((char*)sTextBuffer, "%s%c%s", XTESTBUF[3], vbLf, XTESTBUF[4]);
	//strcpy((char*)sTextBuffer, XTESTBUF[4]);

	return sTextBuffer;
}
#endif // XDMX_BLOODWYCH_ENGINE

int SkWinCore::READ_DUNGEON_STRUCTURE_BW(X16 isNewGame)
{
	int iReadSuccess = 0;
	int iTotalNbTiles = 0;
	int iTotalMapDataSize = 0;
	int iMapIndex = 0;
	int nMaps = 8; // can be reduced the real number of non null maps
	int nBWMaps = 8; // static
	int iDBCategory = 0;
	int iMapDataOffset = 0;
	int iDataIndex = 0;
	int iTileIndex = 0;
	int iRefObjectIndex = 0;

	U16 iCreatureObjectCounter = 0;
	int iDoorObjectCounter = 0;
	int iSimpleActuatorCounter = 0;
	int iActuatorCounter = 0;
	int iMiscItemCounter = 0;
	int iWeaponCounter = 0;
	int iObjectItemCounter[16];

	int iFirstMapWithZeroDims = 8;

	int i = 0;
	int j = 0;
	int z = 0;
	int nCols = 0;
	int nRows = 0;

	bool bNoCreature = false;
	bool bNoDoor = false;
	bool bNoWalls = false;
	bool bNoWallDecorations = false;
	bool bNoWallSwitchs = false;
	bool bNoDoorLocked = false;
	bool bNoItems = false;
	bool bPutPillars = true;
	SkCodeParam::bDebugInfiniteSpells = true;

	int nMaxCreatures = 512;

	X8 xFloorPadData[4*32];

	//ObjectID* globalGroundObjects = NULL; // for all tiles // use a global one
	//U16* globalGroundObjects = NULL; // for all tiles
	Map_definition_info *xCurrentMap = NULL;
	U16* dunBWMapData = NULL;

	// 8 maps
	// 8 bytes for columns per map, then 8 bytes for rows per map
	Bit8u iMapDimX[8];
	Bit8u iMapDimY[8];

	Bit8u iDummy[32];

	INIT_RANDOM();

	//-- Adjust players colors for BW classes
	glbPaletteT16[COLOR_LIGHT_GREEN] = 0x7E;	// serpent
	glbPaletteT16[COLOR_YELLOW] = 0x8E;	// chaos
	glbPaletteT16[COLOR_RED] = 0xBF; //0x49; //0xAE;	// dragon
	glbPaletteT16[COLOR_BLUE] = 0x3A;	// moon

	///--- Whatever file was opened (a dungeon.dat ?), we close and open directly to the main BW file
	// Before reading the main dungeon data, we read triggers definitions.
	FILE_CLOSE(glbDataFileHandle);

	// 0x6CD2 - floor pad definitions
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	glbDataFileHandle = OPEN_FILE(FORMAT_SKSTR((U8*)".Z020BLOODWYCH", NULL));
	// Pad triggers, 4 bytes * 32 def
	if (1)
	{
		//X8 iFloorPadData[4];
		X8* iFloorPadData = xFloorPadData;
		U16 iDefIndex;
		int iNbFloorPadDefs = 32;
		FILE_SEEK(glbDataFileHandle, 0x06CD2);
		for (iDefIndex = 0; iDefIndex < 32; iDefIndex++)
		{
			FILE_READ(glbDataFileHandle, 4, iFloorPadData);
			printf("FPAD (%02d/%02X): %02X %02X %02X %02X\n", iDefIndex, iDefIndex, iFloorPadData[0], iFloorPadData[1], iFloorPadData[2], iFloorPadData[3]);
			iFloorPadData+=4;
		}
	}
//	FILE_CLOSE(glbDataFileHandle);

	//--- Texts referecences
	FILE_SEEK(glbDataFileHandle, 0x1A02A);
	int iNbTextBlocks = 73;
	int iTextBlockIndex = 0;

	{
		bool bEndBlock = false;
		X8 xTextBuffer[256]; memset(xTextBuffer, 0, 256);
		int iTextCursor = 0;
		X8* pTextBuffer = xTextBuffer;
		X8 cTextChar = 0;
		int iTextCommand = 0;
		int iRowCommand = 0;
		int iRowNumber = 0;

		for (iTextBlockIndex = 0; iTextBlockIndex < iNbTextBlocks; iTextBlockIndex++)
		{
			//printf("Text Block %02d ================\n", iTextBlockIndex);
			bEndBlock = false;
			FILE_READ(glbDataFileHandle, 1, &iTextCommand);
			FILE_READ(glbDataFileHandle, 1, &iRowCommand);
			FILE_READ(glbDataFileHandle, 1, &iRowNumber);
			//-- Read till next 0xFC or 0xFF

			while (!bEndBlock) {
				int iFillSpaces = 0;
				iFillSpaces = iRowCommand - 0x1E;	// 1E = starts at 0
				iTextCursor = 0;

				while (iFillSpaces > 0) {
					pTextBuffer[0] = ' ';
					pTextBuffer++;
					iTextCursor++;
					iFillSpaces--;
				}

				while (cTextChar != 0xFC && cTextChar != 0xFF && iTextCursor < 256) {
					FILE_READ(glbDataFileHandle, 1, pTextBuffer);
					cTextChar = pTextBuffer[0];
					if (cTextChar == 0xFC || cTextChar == 0xFF)
						pTextBuffer[0] = 0;	// or maybe \n
					pTextBuffer++;
					iTextCursor++;
				}
				//printf("T%d: %s\n", iRowNumber, xTextBuffer);
				strcpy((char*)(XTESTBUF[iTextBlockIndex][iRowNumber]), (char*)xTextBuffer);
				memset(xTextBuffer, 0, 256);
				pTextBuffer = xTextBuffer;
				iTextCursor = 0;

				if (cTextChar == 0xFF)
					bEndBlock = true;
				else {
					iTextCommand = cTextChar;
					FILE_READ(glbDataFileHandle, 1, &iRowCommand);
					FILE_READ(glbDataFileHandle, 1, &iRowNumber);
				}

				cTextChar = 0;
			}
			//	printf("=====================\n");
//				continue;
		}
	}

	//exit(1);

	//--- Open for main dungeon data
//	glbDataFileHandle = OPEN_FILE(FORMAT_SKSTR((U8*)".Z020BLOODWYCH", NULL));
	FILE_SEEK(glbDataFileHandle, 0xEBBC);


	printf("Read 8*2 bytes (map dimensions) ...\n");

	if (FILE_READ(glbDataFileHandle, nBWMaps, iMapDimX) == 0)
		return 0;
	if (FILE_READ(glbDataFileHandle, nBWMaps, iMapDimY) == 0)
		return 0;

	xInterWalls = (U16*) calloc(8*32*32,sizeof(U16));

	dunHeader = reinterpret_cast<File_header *>(ALLOC_MEMORY_RAM(44, 1, 1024));
	dunMapsHeaders = reinterpret_cast<Map_definitions *>(ALLOC_MEMORY_RAM(nMaps << 4, afUseUpper, 0x400));

	//globalGroundObjects = (U16*) calloc (nMaps*32*32, sizeof(U16));
//	globalGroundObjects = reinterpret_cast<U16 *> (ALLOC_MEMORY_RAM(nMaps*32*32*sizeof(U16), afUseUpper, 0x400));
	globalGroundObjects = reinterpret_cast<ObjectID *> (ALLOC_MEMORY_RAM(nMaps*32*32*sizeof(ObjectID), afUseUpper, 0x400));

	for (iDataIndex = 0; iDataIndex < 16; iDataIndex++)
		iObjectItemCounter[iDataIndex] = 0;

	nRows = 32;
	nCols = 32;
	for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++)
		for (j = 0; j < nRows; j++)
			for (i = 0; i < nCols; i++)
			{
				z = iMapIndex*1024 + j*32 + i;
				globalGroundObjects[z] = OBJECT_NULL;	// SPX: OBJECT_NULL and not END_MARKER ! for a ground tile, NULL must be used as default.
			}

	SkCodeParam::bAllowFreeObjectMove = true;	// allow handling any type of object (like actuator) and move them onto the dungeon
	SkCodeParam::bBWMode = true;
	SkCodeParam::bAutoDefaultMaxLight = true;

	dunHeader->w0 = 0x5742; // random seed, saying "BW"
	dunHeader->cbMapData = 0;
	dunHeader->nMaps = nBWMaps;
	dunHeader->b5 = 0;
	dunHeader->cwTextData = 0;
	dunHeader->w8 = 0;
	dunHeader->cwListSize = 0;
	for (iDBCategory = 0; iDBCategory < 16; iDBCategory++)
		dunHeader->nRecords[iDBCategory] = 0;

	int iGlobalMapOffsetCount = 0;
	for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++)
	{
		int iLocalMapDataSize = 0;
		xCurrentMap = (Map_definition_info*) &dunMapsHeaders[iMapIndex];

		xCurrentMap->mapOffset = 0;
		xCurrentMap->unused1[0] = 0;
		xCurrentMap->unused1[1] = 0;
		xCurrentMap->unused1[2] = 0;
		xCurrentMap->unused1[3] = 0;

		xCurrentMap->xOffset = 0;
		xCurrentMap->yOffset = 0;
		xCurrentMap->level = 7 - iMapIndex;
		xCurrentMap->unused2 = 0;
		xCurrentMap->xDim = iMapDimX[iMapIndex] - 1;
		xCurrentMap->yDim = iMapDimY[iMapIndex] - 1;
		if (iMapDimX[iMapIndex] == 0)
			xCurrentMap->xDim = 0;
		if (iMapDimY[iMapIndex] == 0)
			xCurrentMap->yDim = 0;
		if (iMapDimX[iMapIndex] == 0 && iMapDimY[iMapIndex] == 0)
			iFirstMapWithZeroDims = MIN(iMapIndex, iFirstMapWithZeroDims);	// that will tell to reduce the number of maps held by this dungeon
		xCurrentMap->nWalls = 0;
		xCurrentMap->rWalls = 0;
		xCurrentMap->nFloors = 0;
		xCurrentMap->rFloors = 0;
		xCurrentMap->nOrnates = 0;
		xCurrentMap->nMonsters = 0;

		//-- Note about ornate gfx : data is stored among map data in quite a static way. it is then simpler to allocate the max from the beginning and fill the data thereafter
		if (!bNoWallDecorations) {
			xCurrentMap->nWalls = 15;
		}
		xCurrentMap->nFloors = 15;
		xCurrentMap->nOrnates = 15;
		xCurrentMap->nMonsters = 15;
		

		xCurrentMap->unknown1 = 0;
		xCurrentMap->depth = 0;
		xCurrentMap->unknown2 = 0;
		xCurrentMap->tileset = 0;
		xCurrentMap->door1 = 2; // metal gate
		xCurrentMap->door2 = 0; // iron grate

		xCurrentMap->mapOffset = iMapDataOffset;

		iLocalMapDataSize = ( (xCurrentMap->xDim+1)  * (xCurrentMap->yDim+1) );
		iLocalMapDataSize += ( xCurrentMap->nWalls + xCurrentMap->nFloors + xCurrentMap->nOrnates + xCurrentMap->nMonsters );
		iTotalMapDataSize += ( iLocalMapDataSize );

		iMapDataOffset += ( iLocalMapDataSize );
//		iMapDataOffset += (iMapDimX[iMapIndex] * iMapDimY[iMapIndex] );

		// globals, for object positionning (tile number to tile coords)
		iNumTileStartOffset[iMapIndex] = iGlobalMapOffsetCount;
		iNbTilesPerMap[iMapIndex] = (iMapDimX[iMapIndex] * iMapDimY[iMapIndex]);
		iGlobalMapOffsetCount += iNbTilesPerMap[iMapIndex];
		iGlobalMapsDimX[iMapIndex] = xCurrentMap->xDim + 1;


	}
	printf("Map data would be = %05d (%04X)\n", iTotalMapDataSize, iTotalMapDataSize);
	dunHeader->cbMapData = iTotalMapDataSize;
	dunMapData = ALLOC_MEMORY_RAM(dunHeader->cbMapData, afUseUpper, 0x400);

	if (iFirstMapWithZeroDims < nBWMaps) // < 8
	{
		dunHeader->nMaps = iFirstMapWithZeroDims;
		nMaps = iFirstMapWithZeroDims;
	}

	// Init map data with invalid value -- 0xFF is used as non-used decoration for ornate lists
	for (iTileIndex = 0; iTileIndex < dunHeader->cbMapData; iTileIndex++)
		dunMapData[iTileIndex] = 0xFF;

	FILE_READ(glbDataFileHandle, 16, iDummy);
	FILE_READ(glbDataFileHandle, nBWMaps, iMapDimX);
	FILE_READ(glbDataFileHandle, nBWMaps, iMapDimY);
	for (iMapIndex = 0; iMapIndex < nBWMaps; iMapIndex++)
	{
		xCurrentMap = (Map_definition_info*) &dunMapsHeaders[iMapIndex];
		xCurrentMap->xOffset = iMapDimX[iMapIndex];
		xCurrentMap->yOffset = iMapDimY[iMapIndex];
	}

	FILE_READ(glbDataFileHandle, 8, iDummy);

	printf("Read map data (%d tiles) (%d bytes)\n", iTotalMapDataSize, iTotalMapDataSize*2);
	//dunBWMapData = (U16*) calloc(iTotalMapDataSize, sizeof(U16));
	dunBWMapData = reinterpret_cast<U16 *> (ALLOC_MEMORY_RAM(iTotalMapDataSize*sizeof(U16), afUseUpper, 0x400));
	
	// Due to ornate decorations list between maps data, data can't be read in one chunk.
	//FILE_READ(glbDataFileHandle, 2*iTotalMapDataSize, dunBWMapData);
	U16* pdunMapData = (U16*) dunBWMapData;
	for (iMapIndex = 0; iMapIndex < nBWMaps; iMapIndex++)
	{
		int iLocalMapDataSize = 0;
		xCurrentMap = (Map_definition_info*) &dunMapsHeaders[iMapIndex];
		iLocalMapDataSize = ( (xCurrentMap->xDim+1)  * (xCurrentMap->yDim+1) );
		FILE_READ(glbDataFileHandle, 2*iLocalMapDataSize, pdunMapData);
		pdunMapData += iLocalMapDataSize;
		pdunMapData += (( xCurrentMap->nWalls + xCurrentMap->nFloors + xCurrentMap->nOrnates + xCurrentMap->nMonsters ));
	}

	dunMapColumnsSumArray = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(nMaps << 1, afUseUpper, 0x400));


//-------------------------------
/// 00 - Init start position
	cd.pi.glbPlayerPosX = 13;
	cd.pi.glbPlayerPosY = 28;
	cd.pi.glbPlayerDir = 0;
	cd.pi.glbPlayerMap = 3;

/// 00b - West Champions
	cd.pi.glbPlayerPosX = 3;
	cd.pi.glbPlayerPosY = 27;
	cd.pi.glbPlayerDir = 0;
	cd.pi.glbPlayerMap = 3;	
	
/// 01 - In the hall before keep
	cd.pi.glbPlayerPosX = 13;
	cd.pi.glbPlayerPosY = 17;
	cd.pi.glbPlayerDir = 0;
	cd.pi.glbPlayerMap = 3;

/// 02 - Before double wood doors
	cd.pi.glbPlayerPosX = 10;
	cd.pi.glbPlayerPosY = 6;
	cd.pi.glbPlayerDir = 3;
	cd.pi.glbPlayerMap = 3;

/// 02 - First Corridor Switch (grey)
	cd.pi.glbPlayerPosX = 1;
	cd.pi.glbPlayerPosY = 10;
	cd.pi.glbPlayerDir = 0;
	cd.pi.glbPlayerMap = 3;

/// 02b - Second Switch (red)
	cd.pi.glbPlayerPosX = 1;
	cd.pi.glbPlayerPosY = 8;
	cd.pi.glbPlayerDir = 0;
	cd.pi.glbPlayerMap = 3;

/// 03 - Toggle Switch (Iron Key)
	cd.pi.glbPlayerPosX = 8;
	cd.pi.glbPlayerPosY = 2;
	cd.pi.glbPlayerDir = 1;
	cd.pi.glbPlayerMap = 3;

/// 03b - Before Iron Lock
	cd.pi.glbPlayerPosX = 19;
	cd.pi.glbPlayerPosY = 1;
	cd.pi.glbPlayerDir = 1;
	cd.pi.glbPlayerMap = 3;

/// 04 - Floor pad before Big guy
	cd.pi.glbPlayerPosX = 24;
	cd.pi.glbPlayerPosY = 4;
	cd.pi.glbPlayerDir = 3;
	cd.pi.glbPlayerMap = 3;

/// 04b - In front of Giant Crab
	cd.pi.glbPlayerPosX = 30;
	cd.pi.glbPlayerPosY = 5;
	cd.pi.glbPlayerDir = 0;
	cd.pi.glbPlayerMap = 3;

/// 05 - Search for Dragon Key (Green Orc)
	cd.pi.glbPlayerPosX = 30;
	cd.pi.glbPlayerPosY = 23;
	cd.pi.glbPlayerDir = 2;
	cd.pi.glbPlayerMap = 3;

/// 05b - Dragon Key (Tan Orc)
	cd.pi.glbPlayerPosX = 25;
	cd.pi.glbPlayerPosY = 29;
	cd.pi.glbPlayerDir = 1;
	cd.pi.glbPlayerMap = 3;


/// 06 - Button to remove pillar
	cd.pi.glbPlayerPosX = 17;
	cd.pi.glbPlayerPosY = 21;
	cd.pi.glbPlayerDir = 3;
	cd.pi.glbPlayerMap = 3;


/// 07 - Before pit
	cd.pi.glbPlayerPosX = 18;
	cd.pi.glbPlayerPosY = 13;
	cd.pi.glbPlayerDir = 3;
	cd.pi.glbPlayerMap = 3;

//---------------------
/// 11 - Floor below
	cd.pi.glbPlayerPosX = 12;
	cd.pi.glbPlayerPosY = 5;
	cd.pi.glbPlayerDir = 2;
	cd.pi.glbPlayerMap = 2;

/// 12 - Button to remove central wall
	cd.pi.glbPlayerPosX = 13;
	cd.pi.glbPlayerPosY = 2;
	cd.pi.glbPlayerDir = 2;
	cd.pi.glbPlayerMap = 2;

//---------------------
/// 20 - Skeleton maze
	cd.pi.glbPlayerPosX = 1;
	cd.pi.glbPlayerPosY = 18;
	cd.pi.glbPlayerDir = 3;
	cd.pi.glbPlayerMap = 1;


/// 21 - Skeleton maze - before Door
	cd.pi.glbPlayerPosX = 19;
	cd.pi.glbPlayerPosY = 16;
	cd.pi.glbPlayerDir = 2;
	cd.pi.glbPlayerMap = 1;


/// 21b - Skeleton maze - Switch (East) to remove pillar before stairs
	cd.pi.glbPlayerPosX = 20;
	cd.pi.glbPlayerPosY = 18;
	cd.pi.glbPlayerDir = 0;
	cd.pi.glbPlayerMap = 1;

/// 22 - Skeleton maze - Stairs Down
	cd.pi.glbPlayerPosX = 17;
	cd.pi.glbPlayerPosY = 0;
	cd.pi.glbPlayerDir = 3;
	cd.pi.glbPlayerMap = 1;

/// 23 - Before the Crypt
	cd.pi.glbPlayerPosX = 0;
	cd.pi.glbPlayerPosY = 0;
	cd.pi.glbPlayerDir = 2;
	cd.pi.glbPlayerMap = 1;

/// 24 - Before the Crypt (after 1st pillar)
	cd.pi.glbPlayerPosX = 0;
	cd.pi.glbPlayerPosY = 3;
	cd.pi.glbPlayerDir = 2;
	cd.pi.glbPlayerMap = 1;


/// 25 - Before the Crypt (after 2nd pillar)
	cd.pi.glbPlayerPosX = 2;
	cd.pi.glbPlayerPosY = 1;
	cd.pi.glbPlayerDir = 2;
	cd.pi.glbPlayerMap = 1;

/// 30 - Chromatic stairs
	cd.pi.glbPlayerPosX = 18;
	cd.pi.glbPlayerPosY = 18;
	cd.pi.glbPlayerDir = 3;
	cd.pi.glbPlayerMap = 3;


//---------------------
/// 00 - Init start position
	cd.pi.glbPlayerPosX = 13;
	cd.pi.glbPlayerPosY = 28;
	cd.pi.glbPlayerDir = 0;
	cd.pi.glbPlayerMap = 3;




	Bit16u nColumnCounter = 0;

	for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++) 
	{
		int iDefaultTileset = 3; // 3 = keep

		dunMapColumnsSumArray[iMapIndex] = nColumnCounter;
		nColumnCounter += dunMapsHeaders[iMapIndex].Column();
	
		dunMapsHeaders[iMapIndex].bGfxFlags = MAPGFX_FLAG__PIT_UPPER_ROOF | MAPGFX_FLAG__PIT_LOWER_GROUND | MAPGFX_FLAG__STAIRS_GOING_UP | MAPGFX_FLAG__STAIRS_GOING_DOWN | MAPGFX_FLAG__TELEPORTER | MAPGFX_FLAG__DOOR_0 | MAPGFX_FLAG__DOOR_1;
		dunMapsHeaders[iMapIndex].w14 = (iDefaultTileset << 4) + (dunMapsHeaders[iMapIndex].w14 & 0xFF00); // tileset = 3 (keep)
	}
	_4976_4cb4 = nColumnCounter; // that one is super important: it defines the offset of columns when adding/remove object from columns item counts. If not set, any item add/remove will crash the game

	dunHeader->cwListSize = iTotalMapDataSize;	// I'm not sure of the correct size here.
	if (isNewGame != 0) {
		dunHeader->cwListSize += 300;
	}

	dunMapTilesObjectIndexPerColumn = reinterpret_cast<OID_T *>(ALLOC_MEMORY_RAM(nColumnCounter << OID_SIZE_BITSHIFT, afUseUpper, 0x400));
	dunGroundStacks = reinterpret_cast<OID_T *>(ALLOC_MEMORY_RAM(dunHeader->cwListSize << OID_SIZE_BITSHIFT, afUseUpper, 0x400));
	dunTextData = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(dunHeader->cwTextData << 1, afUseUpper, 0x400));

	for (int iGroundRefIndex = 0; iGroundRefIndex < dunHeader->cwListSize; iGroundRefIndex++)
		dunGroundStacks[iGroundRefIndex] = OBJECT_NULL;

	// init objects category
 	for (iDBCategory = 0; iDBCategory < 16; iDBCategory++)
	{
		Bit16u nRecords = dunHeader->nRecords[iDBCategory];
		printf("Category %02d of size %02d => %04d records ...\n", iDBCategory, glbItemSizePerDB[iDBCategory], nRecords);
		if (isNewGame != 0) 
		{
			nRecords = min_value((iDBCategory == dbCloud) ? 0x300 : 0x400, tblDefaultNbItemAllocationPerDB[RCJ(16,iDBCategory)] + nRecords);
			dunHeader->nRecords[iDBCategory] = nRecords;
		}
		Bit16u iItemSize = glbItemSizePerDB[iDBCategory];
		if (iDBCategory == DB_CATEGORY_DOOR)
			nRecords = dunHeader->nRecords[iDBCategory] = 128;	// Allocate 128 empty doors
		if (iDBCategory == DB_CATEGORY_TELEPORTER)
			nRecords = dunHeader->nRecords[iDBCategory] = 128;
		if (iDBCategory == DB_CATEGORY_SIMPLE_ACTUATOR)
			nRecords = dunHeader->nRecords[iDBCategory] = 512;	// Allocate 512 simple actuators
		if (iDBCategory == DB_CATEGORY_ACTUATOR)
			nRecords = dunHeader->nRecords[iDBCategory] = 512;	// Allocate 512 actuators
		if (iDBCategory == DB_CATEGORY_CREATURE)
			nRecords = dunHeader->nRecords[iDBCategory] = nMaxCreatures;	// Allocate XXX empty creatures, to get the pillars
		if (iDBCategory == DB_CATEGORY_WEAPON)
			nRecords = dunHeader->nRecords[iDBCategory] = 256;	// Allocate 256 weapons
		if (iDBCategory == DB_CATEGORY_CLOTHING)
			nRecords = dunHeader->nRecords[iDBCategory] = 256;
		if (iDBCategory == DB_CATEGORY_POTION)
			nRecords = dunHeader->nRecords[iDBCategory] = 128;
		if (iDBCategory == DB_CATEGORY_MISC_ITEM)
			nRecords = dunHeader->nRecords[iDBCategory] = 256;	// Allocate 256 misc items
		//if (_4976_3b5d != 0) 
		{
			glbDBObjectData[iDBCategory] = ALLOC_MEMORY_RAM(nRecords * iItemSize, afUseUpper, 0x400);
		}

		if (iDBCategory == DB_CATEGORY_DOOR)
		{
			U8* xObjectPointer = NULL;
			xObjectPointer = glbDBObjectData[iDBCategory];
			for (iRefObjectIndex = 0; iRefObjectIndex < nRecords; iRefObjectIndex++)
			{
				Door* xDoor = (Door*) xObjectPointer;
				xDoor->next = OBJECT_NULL;	// that tells this object is not used yet
				xDoor->w2 = 0;

				xObjectPointer += iItemSize;
			}
		}
		else if (iDBCategory == DB_CATEGORY_TELEPORTER)
		{
			U8* xObjectPointer = NULL;
			xObjectPointer = glbDBObjectData[iDBCategory];
			for (iRefObjectIndex = 0; iRefObjectIndex < nRecords; iRefObjectIndex++)
			{
				Teleporter* xTeleporter = (Teleporter*) xObjectPointer;
				xTeleporter->w0 = OBJECT_NULL;	// that tells this object is not used yet
				xTeleporter->w2 = 0;
				xTeleporter->w4 = 0;
				xObjectPointer += iItemSize;
			}
		}		
		else if (iDBCategory == DB_CATEGORY_SIMPLE_ACTUATOR)
		{
			U8* xObjectPointer = NULL;
			xObjectPointer = glbDBObjectData[iDBCategory];
			for (iRefObjectIndex = 0; iRefObjectIndex < nRecords; iRefObjectIndex++)
			{
				Text* xText = (Text*) xObjectPointer;
				xText->w0 = OBJECT_NULL;	// that tells this object is not used yet
				xText->w2 = 0;

				xObjectPointer += iItemSize;
			}
		}
		else if (iDBCategory == DB_CATEGORY_ACTUATOR)
		{
			U8* xObjectPointer = NULL;
			xObjectPointer = glbDBObjectData[iDBCategory];
			for (iRefObjectIndex = 0; iRefObjectIndex < nRecords; iRefObjectIndex++)
			{
				Actuator* xActuator = (Actuator*) xObjectPointer;
				xActuator->w0 = OBJECT_NULL;	// that tells this object is not used yet
				xActuator->w2 = 0;
				xActuator->w4 = 0;
				xActuator->w6 = 0;

				xObjectPointer += iItemSize;
			}
		}
		else if (iDBCategory == DB_CATEGORY_CREATURE)
		{
			U8* xObjectPointer = NULL;
			xObjectPointer = glbDBObjectData[iDBCategory];
			for (iRefObjectIndex = 0; iRefObjectIndex < nRecords; iRefObjectIndex++)
			{
				Creature* xCreature = (Creature*) xObjectPointer;
				xCreature->w0 = OBJECT_NULL;	// that tells this creature is not used
				xCreature->possession = OBJECT_END_MARKER;
				xCreature->b4 = 0;
				xCreature->iID = 0;
				xCreature->hp1 = 10;
				xCreature->iAnimSeq = 0;
				xCreature->iAnimFrame = 0;
				xCreature->w12 = 0;
				xCreature->b14 = 0;
				xCreature->b15 = 0;

				xObjectPointer += iItemSize;
			}
		}
		else if (iDBCategory == DB_CATEGORY_WEAPON)
		{
			U8* xObjectPointer = NULL;
			xObjectPointer = glbDBObjectData[iDBCategory];
			for (iRefObjectIndex = 0; iRefObjectIndex < nRecords; iRefObjectIndex++)
			{
				Weapon* xItem = (Weapon*) xObjectPointer;
				xItem->w0 = OBJECT_NULL;	// that tells this object is not used yet
				xItem->w2 = 0;

				xObjectPointer += iItemSize;
			}
		}
		else if (iDBCategory == DB_CATEGORY_CLOTHING)
		{
			U8* xObjectPointer = NULL;
			xObjectPointer = glbDBObjectData[iDBCategory];
			for (iRefObjectIndex = 0; iRefObjectIndex < nRecords; iRefObjectIndex++)
			{
				Cloth* xItem = (Cloth*) xObjectPointer;
				xItem->w0 = OBJECT_NULL;	// that tells this object is not used yet
				xItem->w2 = 0;

				xObjectPointer += iItemSize;
			}
		}
		else if (iDBCategory == DB_CATEGORY_SCROLL)
		{
			U8* xObjectPointer = NULL;
			xObjectPointer = glbDBObjectData[iDBCategory];
			for (iRefObjectIndex = 0; iRefObjectIndex < nRecords; iRefObjectIndex++)
			{
				Scroll* xItem = (Scroll*) xObjectPointer;
				xItem->w0 = OBJECT_NULL;	// that tells this object is not used yet
				xItem->w2 = 0;

				xObjectPointer += iItemSize;
			}
		}
		else if (iDBCategory == DB_CATEGORY_POTION)
		{
			U8* xObjectPointer = NULL;
			xObjectPointer = glbDBObjectData[iDBCategory];
			for (iRefObjectIndex = 0; iRefObjectIndex < nRecords; iRefObjectIndex++)
			{
				Potion* xItem = (Potion*) xObjectPointer;
				xItem->w0 = OBJECT_NULL;	// that tells this object is not used yet
				xItem->w2 = 0;

				xObjectPointer += iItemSize;
			}
		}
		else if (iDBCategory == DB_CATEGORY_CONTAINER)
		{
			U8* xObjectPointer = NULL;
			xObjectPointer = glbDBObjectData[iDBCategory];
			for (iRefObjectIndex = 0; iRefObjectIndex < nRecords; iRefObjectIndex++)
			{
				Container* xContainer = (Container*) xObjectPointer;
				xContainer->w0 = OBJECT_NULL;	// that tells this object is not used yet
				xContainer->w2 = OBJECT_NULL;
				xContainer->b4 = 0;
				xContainer->b5 = 0;
				xContainer->w6 = 0; // Unused
				xObjectPointer += iItemSize;
			}
		}
		else if (iDBCategory == DB_CATEGORY_MISC_ITEM)
		{
			U8* xObjectPointer = NULL;
			xObjectPointer = glbDBObjectData[iDBCategory];
			for (iRefObjectIndex = 0; iRefObjectIndex < nRecords; iRefObjectIndex++)
			{
				Miscellaneous_item* xItem = (Miscellaneous_item*) xObjectPointer;
				xItem->w0 = OBJECT_NULL;	// that tells this object is not used yet
				xItem->w2 = 0;

				xObjectPointer += iItemSize;
			}
		}
		else if (iDBCategory == DB_CATEGORY_MISSILE)
		{
			U8* xObjectPointer = NULL;
			xObjectPointer = glbDBObjectData[iDBCategory];
			for (iRefObjectIndex = 0; iRefObjectIndex < nRecords; iRefObjectIndex++)
			{
				Missile* xMissile = (Missile*) xObjectPointer;
				xMissile->w0 = OBJECT_NULL;	// that tells this object is not used yet
				xMissile->w2 = 0;
				xMissile->b4_ = 0;
				xMissile->b5_ = 0;
				xMissile->w6 = 0;
				xObjectPointer += iItemSize;
			}
		}
		else if (iDBCategory == DB_CATEGORY_CLOUD)
		{
			U8* xObjectPointer = NULL;
			xObjectPointer = glbDBObjectData[iDBCategory];
			for (iRefObjectIndex = 0; iRefObjectIndex < nRecords; iRefObjectIndex++)
			{
				Cloud* xCloud = (Cloud*) xObjectPointer;
				xCloud->w0 = OBJECT_NULL;	// that tells this object is not used yet
				xCloud->w2 = 0;

				xObjectPointer += iItemSize;
			}
		}

	}
	iReadSuccess = 1;

	// default init of dunGroundStacks
	for (iRefObjectIndex = 0 ; iRefObjectIndex < dunHeader->cwListSize; iRefObjectIndex++)
		dunGroundStacks[iRefObjectIndex] = OBJECT_NULL;



	glbMapTileValue = reinterpret_cast<U8 ***>(ALLOC_MEMORY_RAM((nColumnCounter + nMaps) * sizeof(void *), afUseUpper, 0x400));
	Bit8u ***bp08 = &glbMapTileValue[nMaps];

	int iNCol = 0;
	printf("Read Dungeon Map Data ...\n");
	//if (SKLOAD_READ(dunMapData, dunHeader->cbMapData) == 0)
	//	return 0;

	for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++) {
		glbMapTileValue[iMapIndex] = reinterpret_cast<U8 **>(bp08);
		Bit8u *bp0c = &dunMapData[dunMapsHeaders[iMapIndex].w0];
		*bp08 = reinterpret_cast<U8 **>(bp0c);
		bp08++;
		for (iNCol = 1; dunMapsHeaders[iMapIndex].Column() -1 >= iNCol; iNCol++) {
			bp0c += dunMapsHeaders[iMapIndex].Row();
			*bp08 = reinterpret_cast<U8 **>(bp0c);
			bp08++;
		}
		
	}

	ObjectID xNewObject = OBJECT_NULL;
	for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++) 
	{
		z = 0;
		int za = 0; // absolute z
		ObjectID xObject = OBJECT_END_MARKER;
		U16 iBWTileValue = 0;
		int iTileType = 0;
		int iTileFunction = 0;
		U8* xObjectPointer = NULL;
		int iFacing = 0;

		Creature* xCreatureList = NULL;
		Creature* xCreature = NULL;

		Door* xDoorList = NULL;
		Door_Info* xDoor = NULL;

		Text* xSimpleActuatorList = NULL;
		SimpleActuator* xSimpleActuator = NULL;

		Actuator* xActuatorList = NULL;
		Actuator* xActuator = NULL;
		Teleporter* xTeleporter = NULL;

		Miscellaneous_item* xMiscItemList = NULL;

		xCurrentMap = (Map_definition_info*) &dunMapsHeaders[iMapIndex];
		nCols = xCurrentMap->xDim + 1;
		nRows = xCurrentMap->yDim + 1;
		if (xCurrentMap->xDim == 0 && xCurrentMap->yDim == 0)
		{
			nCols = nRows = 0;
		}

		printf("MAP %d\n", iMapIndex);

		CHANGE_CURRENT_MAP_TO(iMapIndex);


		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);

		printf("COL) ");
		for (i = 0; i < nCols; i++)
			printf("%02d   ", i);
		printf("\n");

		for (j = 0; j < nRows; j++)
		{
			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
			printf("%02d) ", j);
			for (i = 0; i < nCols; i++)
			{
				glbMapTileValue[iMapIndex][i][j] = 0x20; // floor / SPX: this init is important else value is xFF and would consider there is an item on it (x10)

				z = i + j*nCols + xCurrentMap->mapOffset;
				za = i + j*32 + iMapIndex*1024;
				iBWTileValue = dunBWMapData[z];
				xInterWalls[za] = iBWTileValue;
				iTileType = (iBWTileValue>>8) & 0x0F;
				iTileFunction = iBWTileValue%256;
//				iTileFunction = (iBWTileValue>>8) & 0x0F;
//				iTileType = iBWTileValue%256;

				switch (iTileType)
				{
					case 0: CHANGE_CONSOLE_COLOR(BRIGHT, GRAY, BLACK); break;
					case 1: CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK); break;
					case 2: CHANGE_CONSOLE_COLOR(BRIGHT, RED, BLACK); break;
					case 3: CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GREEN, BLACK); break;
					case 4: CHANGE_CONSOLE_COLOR(BRIGHT, WHITE, GRAY); break;
					case 5: CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_RED, BLACK); break;
					case 6: CHANGE_CONSOLE_COLOR(BRIGHT, BLACK, LIGHT_GRAY); break;
				}

				printf("%04X.", iBWTileValue);

				//glbMapTileValue[iMapIndex][i][j] = 0x20; // floor
				DUNGEON_SET_TILE(ttFloor, 0, iMapIndex, i, j);

				if ( iTileType == 1 )	// Wall
				{
					tBWTileWall* tWallTile = (tBWTileWall*) &iBWTileValue;
					int iIsObjectOnWall = 0;
					int iObjectFace = 0;
					//glbMapTileValue[iMapIndex][i][j] = 0x00; // wall

					if (bNoWalls == true)
						continue;

					DUNGEON_SET_TILE(ttWall, 0, iMapIndex, i, j);

					iIsObjectOnWall = (iBWTileValue>>12) & 0x0F;
					iObjectFace = (iBWTileValue & 0x3000)>>12;
					// Objects on wall
					// 8100 : on north face
					// 9100 : on east face
					// A100 : on south face
					// B100 : on west face
					if (iIsObjectOnWall != 0)
					{
						// Create a simple actuator for alcoves and gfx
						
						if (iObjectItemCounter[DB_CATEGORY_SIMPLE_ACTUATOR] < dunHeader->nRecords[DB_CATEGORY_SIMPLE_ACTUATOR])
						//if (iObjectItemCounter[DB_CATEGORY_SIMPLE_ACTUATOR] < 0)
						{
							int iWallDecorationType = iTileFunction & 0x03;
							//int iColor = ((((i+j))%8)+8)%8; // rule for color based on tile position (tapestries)
							int iColor = ((i+j))%8;
							//glbMapTileValue[iMapIndex][i][j] = 0x10; // wall
							DUNGEON_SET_TILE(ttWall, 0, iMapIndex, i, j);

							xObjectPointer = glbDBObjectData[DB_CATEGORY_SIMPLE_ACTUATOR];
							xSimpleActuatorList = (Text*) xObjectPointer;
							xSimpleActuator = (SimpleActuator*) &xSimpleActuatorList[iObjectItemCounter[DB_CATEGORY_SIMPLE_ACTUATOR]];

							xSimpleActuator->w0 = OBJECT_END_MARKER;
							xSimpleActuator->activated = 0;
							xSimpleActuator->mode = 1;
							xSimpleActuator->graphism = 1; // alcove
							xSimpleActuator->extendedUsage = 0; // still image

							switch (iWallDecorationType)
							{
								case BW_WALL_ALCOVE_SHELF:		// 0
									xSimpleActuator->graphism = 1; // alcove
									break;
								case BW_WALL_TAPESTRY:		// 1
									if (tWallTile->wallfunc.generic.id == 0)	// generated color
										xSimpleActuator->graphism = 0x25+iColor; // tapestry
									else if (tWallTile->wallfunc.generic.id >= 5)	// message / sign
										xSimpleActuator->graphism = 0x27; // blue one
									else if (tWallTile->wallfunc.generic.id == 1)	// serpent
										xSimpleActuator->graphism = 0x21;
									else if (tWallTile->wallfunc.generic.id == 2)	// dragon
										xSimpleActuator->graphism = 0x22;
									else if (tWallTile->wallfunc.generic.id == 3)	// moon
										xSimpleActuator->graphism = 0x23;
									else if (tWallTile->wallfunc.generic.id == 4)	// chaos
										xSimpleActuator->graphism = 0x24;
									// if tWallTile->wallfunc.generic.id >= 5, then it is a text wall
									break;
								case BW_WALL_SWITCH_BUTTON:		// 2
									xSimpleActuator->graphism = 0x0F; // gem button
									break;
								case BW_WALL_GEM_SLOT:		// 3
									xSimpleActuator->graphism = 0x4B; // gem slot (borrowing fire shooter gfx)
									break;
							}

							///--- Manage here text wall
							if (iWallDecorationType == BW_WALL_TAPESTRY && tWallTile->wallfunc.generic.id >= 5) {
								int iTextWallID = tWallTile->wallfunc.generic.id - 5;
								xSimpleActuator->activated = 1;  // visible
								xSimpleActuator->mode = 1;
								xSimpleActuator->graphism = iTextWallID; // text ID to be displayed
								xSimpleActuator->extendedUsage = 14; // display GDAT text
							}



							iFacing = iObjectFace;

							xObject = ObjectID(iFacing, DB_CATEGORY_SIMPLE_ACTUATOR, iObjectItemCounter[DB_CATEGORY_SIMPLE_ACTUATOR]);
							ATLASSERT(xObject.DBType() == DB_CATEGORY_SIMPLE_ACTUATOR);
							z = iMapIndex*1024 + j*32 + i;
							
							if (iWallDecorationType != BW_WALL_SWITCH_BUTTON && !bNoWallDecorations) {
								DUNGEON_SET_TILE_FLAG(TILE_FLAG_HAS_OBJECT, iMapIndex, i, j);
								globalGroundObjects[z] = xObject;
								iSimpleActuatorCounter++;
								iObjectItemCounter[DB_CATEGORY_SIMPLE_ACTUATOR]++;
							}
							else
								printf("Switch\n");
							
							//printf("Adding Simple actuator #%03d @ (%d,%d,%d) as (%d,%d,G-%02d,%d)\n", iSimpleActuatorCounter, iMapIndex, i, j,
							//	xSimpleActuator->activated, xSimpleActuator->mode, xSimpleActuator->graphism, xSimpleActuator->extendedUsage);
						}
					}
				}


				if ( iTileType == 3 && iTileFunction == 1 && bPutPillars == true) // pillar
				{
					//glbMapTileValue[iMapIndex][i][j] = 0x20; // wall
					DUNGEON_SET_TILE(ttWall, 0, iMapIndex, i, j);
					
					//if (iTileFunction == 1 && iCreatureObjectCounter < nMaxCreatures && bNoCreature == 0 && iMapIndex == 3)
					//if (iTileFunction == 1 && iObjectItemCounter[DB_CATEGORY_CREATURE] < nMaxCreatures && bNoCreature == false && iMapIndex == 3)
					//if (iTileFunction == 1 && iObjectItemCounter[DB_CATEGORY_CREATURE] < nMaxCreatures && iMapIndex == 3)
					if (iTileFunction == 1 && iObjectItemCounter[DB_CATEGORY_CREATURE] < nMaxCreatures)
					{
						//glbMapTileValue[iMapIndex][i][j] = 0x30; // floor
						DUNGEON_SET_TILE(ttFloor, 0, iMapIndex, i, j);
						DUNGEON_SET_TILE_FLAG(TILE_FLAG_HAS_OBJECT, iMapIndex, i, j);

						xObjectPointer = glbDBObjectData[DB_CATEGORY_CREATURE];
						xCreatureList = (Creature*) xObjectPointer;
						xCreature = (Creature*) &xCreatureList[iObjectItemCounter[DB_CATEGORY_CREATURE]];
								
						xCreature->w0 = OBJECT_END_MARKER;
						xCreature->possession = OBJECT_END_MARKER;
						xCreature->b4 = 0x7F; // pillar
						xCreature->iID = 0;
						xCreature->hp1 = 255;
						xCreature->iAnimSeq = 0;
						xCreature->iAnimFrame = 0;
						xCreature->w12 = 0;
						xCreature->b14 = 0;
						xCreature->b15 = 0;

						xObject = ObjectID(0, DB_CATEGORY_CREATURE, iObjectItemCounter[DB_CATEGORY_CREATURE]);
						z = iMapIndex*1024 + j*32 + i;
						//globalGroundObjects[z] = xObject;
						DUNGEON_STACK_OBJECT(xObject, iMapIndex, i, j);

						//printf("Creature #%03d/%03d type PILLAR at position %d (%02d,%02d)\n", iObjectItemCounter[DB_CATEGORY_CREATURE], nMaxCreatures, iMapIndex, i, j);
						//iCreatureObjectCounter++;
						iObjectItemCounter[DB_CATEGORY_CREATURE]++;
					}
					
				}
				if (iTileType == 3 && iTileFunction == 0) // bed
				{
					//glbMapTileValue[iMapIndex][i][j] = 0x20; // floor
					DUNGEON_SET_TILE(ttFloor, 0, iMapIndex, i, j);
					
					//if (iTileFunction == 0 && iCreatureObjectCounter < nMaxCreatures && bNoCreature == 0 && iMapIndex == 3)
					if (iTileFunction == 0 && iObjectItemCounter[DB_CATEGORY_CREATURE] < nMaxCreatures && bNoCreature == false && iMapIndex == 3)
					{
						//glbMapTileValue[iMapIndex][i][j] = 0x30; // floor
						DUNGEON_SET_TILE_FLAG(TILE_FLAG_HAS_OBJECT, iMapIndex, i, j);

						xObjectPointer = glbDBObjectData[DB_CATEGORY_CREATURE];
						xCreatureList = (Creature*) xObjectPointer;
						xCreature = (Creature*) &xCreatureList[iObjectItemCounter[DB_CATEGORY_CREATURE]];
								
						xCreature->w0 = OBJECT_END_MARKER;
						xCreature->possession = OBJECT_END_MARKER;
						xCreature->b4 = 0x7E; // bed
						xCreature->iID = 0;
						xCreature->hp1 = 255;
						xCreature->iAnimSeq = 0;
						xCreature->iAnimFrame = 0;
						xCreature->w12 = 0;
						xCreature->b14 = 0;
						xCreature->b15 = 0;

						xObject = ObjectID(0, DB_CATEGORY_CREATURE, iObjectItemCounter[DB_CATEGORY_CREATURE]);
						z = iMapIndex*1024 + j*32 + i;
						globalGroundObjects[z] = xObject;

						//iCreatureObjectCounter++;
						iObjectItemCounter[DB_CATEGORY_CREATURE]++;
					}
				}
				

				if (iTileType == 4) // stairs
				{
					int iDMLeading = 0; // DM Down
					int iBWLeading = (iTileFunction & 0x01); // 1 = Down
					int iStairsLead = TILE_FLAG_IS_LEADING_UP;
					int iDMFacing = 0; // west/east
					int iBWFacing = (iTileFunction & 0x07)>>1;
					iDMFacing = (iBWFacing & 0x01) ? 1 : 0;	// 0 = N/S or 1 = E/W
					glbMapTileValue[iMapIndex][i][j] = 0x60;
					// 0x0400 : up, towards south
					// 0x0402 : up, towards west
					// 0x0404 : up, towards north
					// 0x0406 : up, towards east
					// 0x0001 : down
					iDMLeading = !iBWLeading;
					iStairsLead = (iBWLeading == 1) ? TILE_FLAG_IS_LEADING_DOWN : TILE_FLAG_IS_LEADING_UP;

					int iTileFlagStairsFacing = (iDMFacing) ? TILE_FLAG_IS_WEST_EAST : TILE_FLAG_IS_NORTH_SOUTH;
					//glbMapTileValue[iMapIndex][i][j] = 0x60 + (iDMLeading)<<1 + (iDMFacing)<<2;
					//glbMapTileValue[iMapIndex][i][j] = 0x60;
					DUNGEON_SET_TILE(ttStairs, 0, iMapIndex, i, j);
					DUNGEON_SET_TILE_FLAG(iStairsLead, iMapIndex, i, j);
					DUNGEON_SET_TILE_FLAG(iTileFlagStairsFacing, iMapIndex, i, j);
				}

				if (iTileType == 5) // doors
				{
					// 0x0501 metal door facing N/S
					// 0x0503 iron grate facing N/S
					// 0x0505 metal door facing W/E
					// 0x0507 iron grate facing W/E
					// 0x15Zx door locked to be unlocked by key "Z"
					// 0x050F locked, can't be unlocked by key, requires a special pad/trigger
					int iDoorType = 0;
					int iDoorFacing = 0;
					int iDoorSpecial = 0;
					int iDMDoorFacing = 0; // 0 is west/east
					int iDoorLocked = 0;

					//glbMapTileValue[iMapIndex][i][j] = 0x80; // door
					DUNGEON_SET_TILE(ttDoor, 0, iMapIndex, i, j);

					iDoorType = (iTileFunction & 0x02)>>1; // 0 = metal, 1 = iron grate
					iDoorFacing = (iTileFunction & 0x04)>>2; // 0 = N/S, 1 = W/E
					iDMDoorFacing = !iDoorFacing;
					iDoorLocked = (iBWTileValue & 0x1000)>>12;
					//printf("LOCKED = %d (from %04X)\n", iDoorLocked, iTileFunction);

					glbMapTileValue[iMapIndex][i][j] = 0x80 + (iDMDoorFacing<<3);
										
					//if (iDoorObjectCounter < 256 && bNoDoor == 0)
					if (iObjectItemCounter[DB_CATEGORY_DOOR] < dunHeader->nRecords[DB_CATEGORY_DOOR] && bNoDoor == 0)
					{
						int iDoorFunction = 0;
						iDoorFunction = (iBWTileValue>>8) & 0x07;

						glbMapTileValue[iMapIndex][i][j] = 0x90; // floor + default orientation west/east

						xObjectPointer = glbDBObjectData[DB_CATEGORY_DOOR];
						xDoorList = (Door*) xObjectPointer;
						xDoor = (Door_Info*) &xDoorList[iDoorObjectCounter];
								
						xDoor->next = OBJECT_END_MARKER;
						xDoor->button = 1;
						xDoor->type = iDoorType;
						xDoor->verticalOpening = 0; // metal door
						xDoor->locked = iDoorLocked;
						if (iDoorType == 1) // iron grate
							xDoor->verticalOpening = 1;

						if (bNoDoorLocked == true)
							xDoor->locked = false;
						/*
						if (iDoorFunction == 1 || iDoorFunction == 5)
							xDoor->type = 1; // metal gate
						if (iDoorFunction == 3 || iDoorFunction == 7) // north/south orientation
							glbMapTileValue[iMapIndex][i][j] = 0x98;
						*/
						glbMapTileValue[iMapIndex][i][j] = 0x90 + (iDMDoorFacing<<3); // has object + facing
						

						glbMapTileValue[iMapIndex][i][j] += 0x04; // the door is closed

						xObject = ObjectID(0, DB_CATEGORY_DOOR, iObjectItemCounter[DB_CATEGORY_DOOR]);
						z = iMapIndex*1024 + j*32 + i;
						globalGroundObjects[z] = xObject;

						iDoorObjectCounter++;
						iObjectItemCounter[DB_CATEGORY_DOOR]++;
					}

					// If the door is locked, put a WALL item eater on the door referencing the expected key
					// ACTUATOR_TYPE_DM1_ITEM_EATER
					if (iDoorLocked == 1 && bNoDoor == 0) {
						int iKeyID = (iTileFunction & 0xF0)>>4;
						int iMiscKeyItemID = 0x4F + iKeyID;

						xObjectPointer = glbDBObjectData[DB_CATEGORY_ACTUATOR];
						xActuatorList = (Actuator*) xObjectPointer;
						xActuator = (Actuator*) &xActuatorList[iActuatorCounter];						

						xActuator->w0 = OBJECT_NULL;
						xActuator->ActuatorType(ACTUATOR_TYPE_DM1_ITEM_EATER);
						xActuator->SetTarget(iMapIndex, i, j);
						xActuator->ActuatorData(iMiscKeyItemID);
						xActuator->GraphicNumber(0); // set no gfx else a permanent fireball will fly on the door

						xDoor->next = ObjectID(0, DB_CATEGORY_ACTUATOR, iActuatorCounter);

						//Actuator* xActuator;
						//printf("Expected KEY = %d\n", iKeyID);

						iActuatorCounter++;
						iObjectItemCounter[DB_CATEGORY_ACTUATOR]++;
					}
					
				}

				//--- Floors types
				// 0 = spell fizzle
				// 1 = pit
				// 2 = floor pad
				// 3 = invisible trigger
				if ( iTileType == 6 && iTileFunction == 1 )	// pit
				{
					//glbMapTileValue[iMapIndex][i][j] = 0x40; // pit
					DUNGEON_SET_TILE(ttPit, 0, iMapIndex, i, j);
					DUNGEON_SET_TILE_FLAG(TILE_FLAG_IS_OPEN, iMapIndex, i, j);
				}
				//else if ( iMapIndex == 3 && j == 4 && iTileType == 6 && (iTileFunction & 0x03) == 2 )	// floor pad
				//else if ( iTileType == 6 && (iTileFunction & 0x03) == 2 )	// floor pad
				else if ( iTileType == 6 )	// floor pad
				{
					int iIsPadInvisible = (iTileFunction & 0x01);
					int iIsFloorTrigger = (iTileFunction & 0x02)>>1;
					int iIsCeilingPit = (iTileFunction & 0x04)>>2;
					int iTriggerPadID = (iTileFunction & 0xF8)>>3; // 5 bits
					X8* pFloorPadData = NULL;
					int iTriggerFunctionType = 0;

					ObjectID rlActuator = OBJECT_END_MARKER;

					xObjectPointer = glbDBObjectData[DB_CATEGORY_ACTUATOR];
					xActuatorList = (Actuator*) xObjectPointer;
					//xActuator = (Actuator*) &xActuatorList[iActuatorCounter];						


					xObjectPointer = glbDBObjectData[DB_CATEGORY_SIMPLE_ACTUATOR];
					xSimpleActuatorList = (Text*) xObjectPointer;
					xSimpleActuator = (SimpleActuator*) &xSimpleActuatorList[iObjectItemCounter[DB_CATEGORY_SIMPLE_ACTUATOR]];

					xSimpleActuator->w0 = OBJECT_END_MARKER;
					xSimpleActuator->activated = 0;
					xSimpleActuator->mode = 1;
					xSimpleActuator->graphism = 0x0C; //floor pad
					xSimpleActuator->extendedUsage = 0; // still image





					//DUNGEON_PUT_FLOOR_DECORATION(0x0C, iMapIndex, i, j); // seems not to work properly

					rlActuator = ObjectID(0, DB_CATEGORY_SIMPLE_ACTUATOR, iObjectItemCounter[DB_CATEGORY_SIMPLE_ACTUATOR]);
					DUNGEON_SET_TILE_FLAG(TILE_FLAG_HAS_OBJECT, iMapIndex, i, j);
					z = iMapIndex*1024 + j*32 + i;
					globalGroundObjects[z] = rlActuator;
					iSimpleActuatorCounter++;
					iObjectItemCounter[DB_CATEGORY_SIMPLE_ACTUATOR]++;

				
					//if (!bNoFloorDecorations)
						DUNGEON_SET_FLOOR_GFX(0x0C, iMapIndex); // Set floor pad

					pFloorPadData = &xFloorPadData[iTriggerPadID*4];
					iTriggerFunctionType = pFloorPadData[0]>>1;
					
					switch(iTriggerFunctionType)
					{
					case 0:
						break;
					case 1: // spin 180
					case 2: // spin random
						{
							//DUNGEON_PUT_PARTY_SPIN_180
							//xTeleporter = DUNGEON_PUT_TELEPORTER(iMapIndex, i, j);
						}
						break;
					case 3: // open locked door
					case 11: // remove pillar
						{
							xActuator = DUNGEON_PUT_FLOOR_ACTUATOR(0x0C, iMapIndex, i, j);	// rlActuator created here
							xActuator->ActuatorType(ACTUATOR_FLOOR_TYPE__PARTY);
							xActuator->ActionType(ACTMSG_OPEN_SET);
							if (iTriggerFunctionType == 11) {
							//	xActuator->ActuatorType(ACTUATOR_FLOOR_TYPE__CREATURE_KILLER);
							}
							xActuator->Delay(0);
							xActuator->OnceOnlyActuator(1);
							xActuator->SoundEffect(1);
							xActuator->SetTarget(iMapIndex, 0, 0);
							xActuator->w0 = OBJECT_NULL;
							xActuator->GraphicNumber(1);

							//if (iTriggerFunctionType == 3 || iTriggerFunctionType == 11) // open closed door
							{
								pFloorPadData[2]; // Target X
								pFloorPadData[3]; // Target Y
								xActuator->SetTarget(iMapIndex, pFloorPadData[2], pFloorPadData[3]);
							}
							if (iTriggerFunctionType == 11) // remove pillar, add a creature killer on the pillar location
							{
								Actuator* xActuatorRemovePillar = NULL;
								xActuatorRemovePillar = DUNGEON_PUT_FLOOR_ACTUATOR(0x0C, iMapIndex, pFloorPadData[2], pFloorPadData[3]);
								xActuatorRemovePillar->ActuatorType(ACTUATOR_FLOOR_TYPE__CREATURE_KILLER);
								xActuatorRemovePillar->ActionType(ACTMSG_OPEN_SET);
								xActuatorRemovePillar->Delay(0);
								xActuatorRemovePillar->OnceOnlyActuator(1);
								xActuatorRemovePillar->SoundEffect(1);
								xActuatorRemovePillar->w0 = OBJECT_NULL;
								xActuatorRemovePillar->GraphicNumber(0); // no gfx
								xActuatorRemovePillar->SetTarget(iMapIndex, pFloorPadData[2], pFloorPadData[3]);	// self
								
								iActuatorCounter++;
								iObjectItemCounter[DB_CATEGORY_ACTUATOR]++;
							}
						}
						break;
					case 4: // vivify machine
					case 5:

						break;
					case 6: // blue gem
					case 7: // tan gem

						break;
					case 8: // trader doors
						break;
					case 9: // entrance tower pad aside
					case 10: // entrance tower pad middle
						break;
					default:
						break;
					}


					


					printf("FLOOR PAD: T%02d > @(M%02d|%02d,%02d) T(%02d,%02d)\n", iTriggerPadID, iMapIndex, i, j,
						pFloorPadData[2], pFloorPadData[3]);


					iActuatorCounter++;
					iObjectItemCounter[DB_CATEGORY_ACTUATOR]++;



				}

			}
			printf("\n");
		}
	}

	// Now, rebuild the ground stack / objects per column info
	//dunMapTilesObjectIndexPerColumn = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(nColumnCounter << 1, afUseUpper, 0x400));
	//dunGroundStacks = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(dunHeader->cwListSize << 1, afUseUpper, 0x400));

	{
//		int i = 0;
//		int j = 0;
//		int nCols = 0;
//		int nRows = 0;
		int iObjectCounts = 0;
		int iOffsetID = 0;
		int iTileValue = 0;
		int iGroundRefIndex = 0;

		for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++) 
		{
			xCurrentMap = (Map_definition_info*) &dunMapsHeaders[iMapIndex];
			nCols = xCurrentMap->xDim + 1;
			nRows = xCurrentMap->yDim + 1;

			for (i = 0; i < nCols; i++)
			{	
				dunMapTilesObjectIndexPerColumn[iOffsetID] = iObjectCounts;
				for (j = 0; j < nRows; j++)
				{
					z = iMapIndex*1024 + j*32 + i;
					iTileValue = glbMapTileValue[iMapIndex][i][j];
					if ( (iTileValue & 0x10) != 0)
					{
						dunGroundStacks[iGroundRefIndex] = globalGroundObjects[z];
						//printf("Copy %08X to %08x (size = %d)\n", &dunGroundStacks[iGroundRefIndex], &globalGroundObjects[z], sizeof(OID_T));
						//memcpy(&dunGroundStacks[iGroundRefIndex], &globalGroundObjects[z], sizeof(OID_T)*2);
						iObjectCounts++;
						iGroundRefIndex++;
					}
				}
				iOffsetID++;
			}
		}
	}

	// Depth of maps
//	if (_4976_3b5d != 0) 
	{
		_4976_4cb0 = ALLOC_MEMORY_RAM(MAXDEPTH, afUseUpper, 0x400);
		_4976_4c72 = ALLOC_MEMORY_RAM(MAXDEPTH + MAXMAPS, afUseUpper, 0x400);
	}
	Bit8u *bp18 = _4976_4c72;
	Bit8u *bp1c = _4976_4cb0;
	Bit16u bp14 = 0;
	*bp1c = 0;
	bp1c++;
	printf("Arrange Depth of Maps ...\n");
	for (Bit16u bp12 = 0; bp12 < MAXDEPTH; bp1c++, bp12++) {
		for (Bit16u bp1e = 0; bp1e < nMaps; bp1e++) {
			if (dunMapsHeaders[bp1e].Level() == bp12) {
				*bp18 = (Bit8u)bp1e;
				bp18++;
				bp14++;
			}
		}
		*bp18 = 0xff;
		bp18++;
		bp14++;
		*bp1c = (Bit8u)bp14;
	}

	// Go to the objects data file
	FILE_CLOSE(glbDataFileHandle);
	//glbDataFileHandle = OPEN_FILE(FORMAT_SKSTR(sDungeonFilename, NULL));
	//.Z020DUNGEON.Z024.DAT.
//	glbDataFileHandle = OPEN_FILE(FORMAT_SKSTR((U8*)".Z020MOD0.OB.", NULL));
	glbDataFileHandle = OPEN_FILE(FORMAT_SKSTR((U8*)".Z020MOD0.CR.", NULL));
	if (!bNoCreature)
	{
		U16 nCreatures = 0;
		U8* xCreatureRawData = NULL;
		int iCreatureIndex = 0;

		U8* xObjectPointer = NULL;
//		int iFacing = 0;

		Creature* xCreatureList = NULL;
		Creature* xCreature = NULL;

		// Read creatures
		if (FILE_READ(glbDataFileHandle, 2, &nCreatures) == 0)
			return 0;
		printf("Nb. Creatures = %d\n", nCreatures);

		//xCreatureRawData = ALLOC_MEMORY_RAM(16 * nCreatures, afUseUpper, 0x400);
		xCreatureRawData = (U8*) calloc (nCreatures, sizeof(U8)*16);
		if (SKLOAD_READ(xCreatureRawData, 16 * nCreatures) == 0)
			return 0;

		for (iCreatureIndex = 0; iCreatureIndex < nCreatures; iCreatureIndex++)
		//for (iCreatureIndex = 0; iCreatureIndex < 0; iCreatureIndex++)
		{
			int iQuarter = 0;
			int iHitPoints = 1;
			int iCreatureType = 100;
			int iPositionX = 0;
			int iPositionY = 0;
			int iDirection = 0;
			int iSpeed = 0;
			int iCurrentState = 0;
			int iLevelNColor = 0;
			ObjectID xObject = OBJECT_END_MARKER;
			ObjectID_Reference* xObjectReference = NULL;
			U8* xCreatureData = &xCreatureRawData[16*iCreatureIndex];

			iPositionX = xCreatureData[0];
			iPositionY = xCreatureData[1];
			iQuarter = xCreatureData[2];
			iSpeed = xCreatureData[3];
			iMapIndex = xCreatureData[4];
			iCurrentState = xCreatureData[5];
			iLevelNColor = xCreatureData[6];
			iHitPoints = xCreatureData[8] + (xCreatureData[9]*256);
			iDirection = xCreatureData[10];
			iCreatureType = xCreatureData[11];

			//printf("Creature #%03d/%03d type %02d (x%02X) at map %d (%02d,%02d)\n",
			//	iObjectItemCounter[DB_CATEGORY_CREATURE], nMaxCreatures, iCreatureType, iCreatureType, iMapIndex, iPositionX, iPositionY);
			//if (iCreatureType >= 100 && iCreatureObjectCounter < 253 && bNoCreature == 0)

//			if (iPositionX != 0 || iPositionY != 14 || iMapIndex != 4)
//				continue;

			if (iObjectItemCounter[DB_CATEGORY_CREATURE] < nMaxCreatures && bNoCreature == false)
			{
				if (iPositionX == 255 || iPositionX > 32)	// that might be a removed creature
					continue;
				//if (iCreatureType > 16)
				//	continue;

				//glbMapTileValue[iMapIndex][iPositionX][iPositionY] = 0x30; // floor
				DUNGEON_SET_TILE(ttFloor, 0, iMapIndex, iPositionX, iPositionY);
				DUNGEON_SET_TILE_FLAG(TILE_FLAG_HAS_OBJECT, iMapIndex, iPositionX, iPositionY);

				xObjectPointer = glbDBObjectData[DB_CATEGORY_CREATURE];
				xCreatureList = (Creature*) xObjectPointer;
				xCreature = (Creature*) &xCreatureList[iObjectItemCounter[DB_CATEGORY_CREATURE]];
						
				xCreature->w0 = OBJECT_END_MARKER;
				xCreature->possession = OBJECT_END_MARKER;
				xCreature->b4 = iCreatureType;	// type
				//xCreature->b4 = 0x10;	// type
				//xCreature->b4 = 0x68;	// crab
				//xCreature->b4 = 0x64;	// fat guy
				xCreature->iID = 0;
				xCreature->hp1 = iHitPoints;
				xCreature->hp1 = 1;
				xCreature->iAnimSeq = 0;
				xCreature->iAnimFrame = 0;
				xCreature->w12 = 0;
				xCreature->b14 = 0;
				xCreature->b15 = 0;

				printf("Put creature : type %02X @(%02d|%02d,%02d|q%02X|f%d) HP=%04d SPD=%02X ST=%03d LV=%02X [%s]\n", iCreatureType, iMapIndex, iPositionX, iPositionY, iQuarter, iDirection, 
					iHitPoints, iSpeed, iCurrentState, iLevelNColor, BW_GetCreatureName(iCreatureType));
				xObject = ObjectID(0, DB_CATEGORY_CREATURE, iObjectItemCounter[DB_CATEGORY_CREATURE]);
				xObjectReference = (ObjectID_Reference*) &xObject;
				z = iMapIndex*1024 + iPositionY*32 + iPositionX;
				globalGroundObjects[z] = xObject;
				//printf("Ground Record added : %04x (%02d|%04d d=%d)\n", xObject, xObjectReference->iDBType, xObjectReference->iID, xObjectReference->iDir);

				//iCreatureObjectCounter++;
				iObjectItemCounter[DB_CATEGORY_CREATURE]++;
			}
		}

	}
	// AGAIN
	{

		int iObjectCounts = 0;
		int iOffsetID = 0;
		int iTileValue = 0;
		int iGroundRefIndex = 0;

		for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++) 
		{
			xCurrentMap = (Map_definition_info*) &dunMapsHeaders[iMapIndex];
			nCols = xCurrentMap->xDim + 1;
			nRows = xCurrentMap->yDim + 1;

			for (i = 0; i < nCols; i++)
			{	
				dunMapTilesObjectIndexPerColumn[iOffsetID] = iObjectCounts;
				for (j = 0; j < nRows; j++)
				{
					z = iMapIndex*1024 + j*32 + i;
					iTileValue = glbMapTileValue[iMapIndex][i][j];
					if ( (iTileValue & 0x10) != 0)
					{
						dunGroundStacks[iGroundRefIndex] = globalGroundObjects[z];
						iObjectCounts++;
						iGroundRefIndex++;
					}
				}
				iOffsetID++;
			}
		}
	}

	FILE_CLOSE(glbDataFileHandle);

	// Items/Objects
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	glbDataFileHandle = OPEN_FILE(FORMAT_SKSTR((U8*)".Z020MOD0.OB.", NULL));
	if (bNoItems == false)
	{
		int READ_OBJECT_STEP__TILENUMBER = 1;
		int READ_OBJECT_STEP__NBITEMS = 2;
		int READ_OBJECT_STEP__ITEMID = 3;
		int READ_OBJECT_STEP__QUANTITY = 4;
		int iNbItems = 0;
		int iNbMaxItemsToAdd = 1024;

		{
			int iByteValue = 0;
			int iCursor = 0;
			int iQuantity = 0;
			int iItemID = 0;
			int iItemLoops = 0;
			int iStep = READ_OBJECT_STEP__TILENUMBER;
			int iItemCursor = 0;
			int iTileNumber = 0;
			int iAlcove = 0;
			//t_MapCoords tItemCoords;

			int iCoordsMap = 0;
			int iCoordsX = 0;
			int iCoordsY = 0;
			
			FILE_READ(glbDataFileHandle, 1, &iByteValue);
			FILE_READ(glbDataFileHandle, 1, &iByteValue);
			
			// read loop, through steps
			//return false;
			//while (iCursor < 1023)
			while (iCursor < 1023 && iNbItems < iNbMaxItemsToAdd)
			{
				//printf("Cursor = %d\n", iCursor);
				if (iStep == READ_OBJECT_STEP__TILENUMBER)
				{
					int iOriginalTileNumber = 0;

					//printf("\n");

					FILE_READ(glbDataFileHandle, 1, &iByteValue);
					iTileNumber = (iByteValue*256);
					FILE_READ(glbDataFileHandle, 1, &iByteValue);
					iTileNumber += iByteValue;
					iOriginalTileNumber = iTileNumber;
					iTileNumber = (iTileNumber & 0x0FFE)>>1;
					GetCoordinatesFromTileNumber(iTileNumber, &iCoordsMap, &iCoordsX, &iCoordsY);
					iAlcove = (iOriginalTileNumber & 0xC000)>>14;
					iCursor += 2;
					iStep = READ_OBJECT_STEP__NBITEMS;
				}
				else if (iStep == READ_OBJECT_STEP__NBITEMS)
				{
					FILE_READ(glbDataFileHandle, 1, &iByteValue);
					iItemLoops = iByteValue + 1;
					iCursor++;
					iStep = READ_OBJECT_STEP__ITEMID;

					//printf("OB#%03d TN=%04X NB=%d - ", iNbItems, iTileNumber, iItemLoops);
				}
				else if (iStep == READ_OBJECT_STEP__ITEMID)
				{
					FILE_READ(glbDataFileHandle, 1, &iByteValue);
					iItemID = iByteValue;
					iCursor++;
					iStep = READ_OBJECT_STEP__QUANTITY;
				}
				else if (iStep == READ_OBJECT_STEP__QUANTITY)
				{
					FILE_READ(glbDataFileHandle, 1, &iByteValue);
					iQuantity = iByteValue;
					iCursor++;
					iStep = READ_OBJECT_STEP__TILENUMBER;
					if (iItemLoops > 1)
					{
						iItemLoops--;
						iStep = READ_OBJECT_STEP__ITEMID;				
					}
					if (iItemID != 0 && iNbItems < iNbMaxItemsToAdd)
					{
				//			bw_ItemInfo* xItem = NULL;
				//			xItem = &xTabItems[iItemCursor];
				//printf("get item = %d\n", iCursor);

				//			if (MVALID(xItem))
							{
				//printf("assign item = %d (%x)\n", iCursor, xItem);
				/*
								xItem->iTileNumber = iTileNumber;
								xItem->iMap = tItemCoords.iMap;
								xItem->iPosX = tItemCoords.iPosX;
								xItem->iPosY = tItemCoords.iPosY;
								xItem->iObjectIDType = iItemID;
								xItem->iQuantity = iQuantity;
								xItem->iAlcove = iAlcove;
								*/
								
				//printf("assigned ok\n");
								//printf("ID=%02d (%02X) Qty=%d - ", iItemID, iItemID, iQuantity);
								iNbItems++;
								iItemCursor++;

								/// ADD ITEM ON DM DATA
								{
									ObjectID_Reference* rStackObject = NULL;
									ObjectID_Reference* rNextObject = NULL;
									ObjectID_Reference* rNewObject = NULL;

									int z = 0;
									int iDBType = 0;
									int iFacing = 0;
									ObjectID xObject = OBJECT_NULL;
									ObjectID xGroundStackObject = OBJECT_NULL;
									z = iCoordsMap*1024 + iCoordsY*32 + iCoordsX;
									xGroundStackObject = globalGroundObjects[z];
									rStackObject = (ObjectID_Reference*) &xGroundStackObject; // debug purpose
									
									iDBType = DB_CATEGORY_WEAPON;
									iDBType = GetDBItemFromAbsoluteID(iItemID);
									{
										// add item into alcove
										//if (iWeaponCounter < 256)
										if (iObjectItemCounter[iDBType] < dunHeader->nRecords[iDBType]) 
										{
											U8* xItemDataPointer = NULL;
											Weapon* xWeaponList = NULL;
											Weapon* xItem;
											Potion* xPotion;
											xItemDataPointer = glbDBObjectData[iDBType];
											xWeaponList = (Weapon*) xItemDataPointer;
											xItem = (Weapon*) &xWeaponList[iObjectItemCounter[iDBType]];
											xPotion= (Potion*) &xWeaponList[iObjectItemCounter[iDBType]];

											xItem->w0 = OBJECT_END_MARKER;
											xItem->w2 = 0;
											xItem->ItemType(iItemID);
											if (iDBType == DB_CATEGORY_POTION)
											{
												xPotion->w0 = OBJECT_END_MARKER;
												xPotion->w2 = 0;
												xPotion->PotionType(iItemID);
												xPotion->PotionPower(120);
												xPotion->VisiblePower(1);
											}
											else if (iDBType == DB_CATEGORY_MISC_ITEM)
											{
												Miscellaneous_item* xMisc = NULL;
												xMisc = (Miscellaneous_item*) &xWeaponList[iObjectItemCounter[iDBType]];
												xMisc->ItemType(iItemID);
											}
											xObject = ObjectID(iFacing, iDBType, iObjectItemCounter[iDBType]);
											rNewObject = (ObjectID_Reference*) &xObject; // debug purpose

											rStackObject = (ObjectID_Reference*) &xGroundStackObject; // debug purpose

											// If ground is none, then add the item on this tile)
											if (xGroundStackObject == OBJECT_END_MARKER || xGroundStackObject == OBJECT_NULL)
											{
												xGroundStackObject = xObject;
												//glbMapTileValue[iCoordsMap][iCoordsX][iCoordsY] |= 0x10;
												DUNGEON_SET_TILE_FLAG(TILE_FLAG_HAS_OBJECT, iCoordsMap, iCoordsX, iCoordsY);
												globalGroundObjects[z] = xObject;
												//printf("Item (%02x|%02x) added to ground\n", iDBType, iItemID);
												

											}
											// else, append this new item to the last on the linked list
											else if (1)
											{
												GenericRecord* xStackItem = NULL;
												ObjectID xNextItem = OBJECT_NULL;

												xNextItem = GET_ADDRESS_OF_RECORD(xGroundStackObject)->w0;
												rStackObject = (ObjectID_Reference*) &xGroundStackObject; // debug purpose
												rNextObject = (ObjectID_Reference*) &xNextItem; // debug purpose

												while (xGroundStackObject != OBJECT_END_MARKER && xGroundStackObject != OBJECT_NULL &&
													xNextItem != OBJECT_END_MARKER && xNextItem != OBJECT_NULL)
												{
													xGroundStackObject = GET_NEXT_RECORD_LINK(xGroundStackObject);
													xNextItem = GET_ADDRESS_OF_RECORD(xGroundStackObject)->w0;
													rStackObject = (ObjectID_Reference*) &xGroundStackObject; // debug purpose
													rNextObject = (ObjectID_Reference*) &xNextItem; // debug purpose
												}
												
												iFacing = xGroundStackObject.Dir();
												xObject = ObjectID(iFacing, iDBType, iObjectItemCounter[iDBType]);
												rNewObject = (ObjectID_Reference*) &xObject; // debug purpose

												GET_ADDRESS_OF_RECORD(xGroundStackObject)->w0 = xObject;

												//printf("Item (%02x|%02x) stacked on previous item\n", iDBType, iItemID);

												//glbMapTileValue[iCoordsMap][iCoordsX][iCoordsY] |= 0x10;
												DUNGEON_SET_TILE_FLAG(TILE_FLAG_HAS_OBJECT, iCoordsMap, iCoordsX, iCoordsY);
											}

											//iWeaponCounter++;
											iObjectItemCounter[iDBType]++;

										}

									}
								} // END ADD
							}
					}
				}
				else
				{
					FILE_READ(glbDataFileHandle, 1, &iByteValue);
					iCursor++;
				}
			}
			
		}
		
		printf("\nNumber of items = %d\n", iNbItems);
	}
	FILE_CLOSE(glbDataFileHandle);
	// Items/Objects

	// AGAIN
	{

		int iObjectCounts = 0;
		int iOffsetID = 0;
		int iTileValue = 0;
		int iGroundRefIndex = 0;

		for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++) 
		{
			xCurrentMap = (Map_definition_info*) &dunMapsHeaders[iMapIndex];
			nCols = xCurrentMap->xDim + 1;
			nRows = xCurrentMap->yDim + 1;

			for (i = 0; i < nCols; i++)
			{	
				dunMapTilesObjectIndexPerColumn[iOffsetID] = iObjectCounts;
				for (j = 0; j < nRows; j++)
				{
					z = iMapIndex*1024 + j*32 + i;
					iTileValue = glbMapTileValue[iMapIndex][i][j];
					if ( (iTileValue & 0x10) != 0)
					{
						dunGroundStacks[iGroundRefIndex] = globalGroundObjects[z];
						iObjectCounts++;
						iGroundRefIndex++;
					}
				}
				iOffsetID++;
			}
		}
	}
	


	//--- Read "buttons" information
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	glbDataFileHandle = OPEN_FILE(FORMAT_SKSTR((U8*)".Z020MOD0.BT.", NULL));
	// Buttons  & Triggers file contains 5*64 trigger definitions, 1 per "dungeon" (keep + 4 towers)
	if (1)
	{
		int iTabButtons[128][4];
		int c = 0;
		U8 iButtonData[4];
		{
			int iButtonIndex = 0;
			for (iButtonIndex = 0; iButtonIndex < 64; iButtonIndex++)	// 64 for just "keep"
			{
				FILE_READ(glbDataFileHandle, 4, &iButtonData);
				for (c = 0; c < 4; c++)
					iTabButtons[iButtonIndex][c] = iButtonData[c];

				SkD((DLV_DBG_GAME_LOAD, "Trigger %02X > %02d %02d %02d %02d -- %s at (%02d,%02d)\n",
					iButtonIndex, iTabButtons[iButtonIndex][0], iTabButtons[iButtonIndex][1],
					iTabButtons[iButtonIndex][2], iTabButtons[iButtonIndex][3], 
					
					BW_GetSwitchFunctionName(iTabButtons[iButtonIndex][0]),
					iTabButtons[iButtonIndex][2], iTabButtons[iButtonIndex][3]
					));

			}
		}
		//--- Then we go again through the maps to adjust buttons/switchs/triggers
		Actuator* xActuatorList = NULL;
		Actuator* xActuator = NULL;
		for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++) 
		{
			z = 0;
			ObjectID xObject = OBJECT_END_MARKER;
			U16 iBWTileValue = 0;
			int iTileType = 0;
			int iTileFunction = 0;
			U8* xObjectPointer = NULL;
			int iFacing = 0;

			Actuator* xActuatorList = NULL;
			Actuator* xActuator = NULL;

			xCurrentMap = (Map_definition_info*) &dunMapsHeaders[iMapIndex];
			nCols = xCurrentMap->xDim + 1;
			nRows = xCurrentMap->yDim + 1;
			if (xCurrentMap->xDim == 0 && xCurrentMap->yDim == 0)
			{
				nCols = nRows = 0;
			}
			CHANGE_CURRENT_MAP_TO(iMapIndex);
			for (j = 0; j < nRows; j++)
			{
				for (i = 0; i < nCols; i++)
				{
					z = i + j*nCols + xCurrentMap->mapOffset;
					iBWTileValue = dunBWMapData[z];
					iTileType = (iBWTileValue>>8) & 0x0F;
					if ( iTileType == 1 )	// Wall
					{
						tBWTileWall* tWallTile = (tBWTileWall*) &iBWTileValue;
						int iIsObjectOnWall = 0;
						int iObjectFace = 0;

						CHANGE_CONSOLE_COLOR(BRIGHT, (tWallTile->wallfunc.generic.wtype+2+8), BLACK);
						//printf("SWALL: (%d|%02d,%02d) T=%2d (%s) [ID=%02X]\n", iMapIndex, i, j, tWallTile->wallfunc.generic.wtype, 
						//	BW_GetSpecialWallTypeName(tWallTile->wallfunc.generic.wtype), tWallTile->wallfunc.generic.id);

						if (tWallTile->special == 1 && tWallTile->wallfunc.generic.wtype == 2) // button
						{
							int iBWTargetTileValue = 0;
							int iTargetTileType = 0;
							int iTriggerID = tWallTile->wallfunc.switchbutton.id;
							int iActionType = iTabButtons[iTriggerID][0];
							int iTargetPosX = iTabButtons[iTriggerID][2];
							int iTargetPosY = iTabButtons[iTriggerID][3];
							
							//-- Get target tile
							iBWTargetTileValue = dunBWMapData[iTargetPosX + iTargetPosY*nCols + xCurrentMap->mapOffset];
							iTargetTileType = (iBWTargetTileValue>>8) & 0x0F;

							printf(" > Target(%d|%02d,%02d) A=%2d (%s)\n", iMapIndex, iTargetPosX, iTargetPosY, iActionType, BW_GetSwitchFunctionName(iActionType));

							U8 iButtonColor = 0;
							U8 iButtonGFX = 0x10;
							U16 iGfxLocalIndex = 0;

							iButtonColor = (i + j)%8;
							iButtonGFX = 0x10 + iButtonColor;	
							
							if (iActionType == BW_SWITCHFUNC__REMOVE)
							{


								if (iTargetTileType == 1) // wall, then it should be replaced with a false wall
								{
									ObjectID rlActuator = OBJECT_END_MARKER;
									DUNGEON_SET_TILE(ttTrickWall, 0, iMapIndex, iTargetPosX, iTargetPosY);
									//DUNGEON_SET_TILE(ttFloor, 0, iMapIndex, iTargetPosX, iTargetPosY);
									
									if (!bNoWallDecorations)
										iGfxLocalIndex = DUNGEON_SET_WALL_GFX(iButtonGFX, iMapIndex); // Set green button gfx

									xActuator = DUNGEON_PUT_WALL_ACTUATOR(iButtonGFX, iMapIndex, i, j, tWallTile->facing);
									// Create an actuator switch that removes this wall
									//xObjectPointer = glbDBObjectData[DB_CATEGORY_ACTUATOR];
									//xActuatorList = (Actuator*) xObjectPointer;
									//xActuator = (Actuator*) &xActuatorList[iActuatorCounter];						

									xActuator->w0 = OBJECT_NULL;
									xActuator->ActuatorType(ACTUATOR_TYPE_DM1_WALL_SWITCH);
									xActuator->SetTarget(iMapIndex, iTargetPosX, iTargetPosY);
									xActuator->ActionType(ACTMSG_OPEN_SET);
									xActuator->Delay(0);
									xActuator->GraphicNumber(iGfxLocalIndex);
									xActuator->SoundEffect(1);
									
									globalGroundObjects[z] = xObject;
									//rlActuator = ObjectID(tWallTile->facing, DB_CATEGORY_ACTUATOR, iActuatorCounter);
									//DUNGEON_STACK_OBJECT(rlActuator, iMapIndex, i, j);

									iActuatorCounter++;
									iObjectItemCounter[DB_CATEGORY_ACTUATOR]++;
								}
								if (iTargetTileType == 3) // pillar, probably
								{
									ObjectID rlActuator = OBJECT_END_MARKER;
									
									if (!bNoWallDecorations)
										iGfxLocalIndex = DUNGEON_SET_WALL_GFX(iButtonGFX, iMapIndex); // Set button gfx

									xActuator = DUNGEON_PUT_WALL_ACTUATOR(iButtonGFX, iMapIndex, i, j, tWallTile->facing);
									xActuator->w0 = OBJECT_NULL;
									xActuator->ActuatorType(ACTUATOR_TYPE_CREATURE_KILLER);
									xActuator->SetTarget(iMapIndex, iTargetPosX, iTargetPosY);
									xActuator->ActionType(ACTMSG_OPEN_SET);
									xActuator->Delay(0);
									xActuator->OnceOnlyActuator(1);
									xActuator->GraphicNumber(iGfxLocalIndex);
									xActuator->SoundEffect(1);
									
									globalGroundObjects[z] = xObject;

									iActuatorCounter++;
									iObjectItemCounter[DB_CATEGORY_ACTUATOR]++;
								}
							}
							else if (iActionType == BW_SWITCHFUNC__TOGGLE_STONE_WALL) {
								if (iTargetTileType == 1) // wall, then it should be replaced with a false wall
								{
									ObjectID rlActuator = OBJECT_END_MARKER;
									DUNGEON_SET_TILE(ttTrickWall, 0, iMapIndex, iTargetPosX, iTargetPosY);
									
									if (!bNoWallDecorations)
										iGfxLocalIndex = DUNGEON_SET_WALL_GFX(iButtonGFX, iMapIndex); // Set green button gfx

									xActuator = DUNGEON_PUT_WALL_ACTUATOR(iButtonGFX, iMapIndex, i, j, tWallTile->facing);
									xActuator->w0 = OBJECT_NULL;
									xActuator->ActuatorType(ACTUATOR_TYPE_DM1_WALL_SWITCH);
									xActuator->SetTarget(iMapIndex, iTargetPosX, iTargetPosY);
									xActuator->ActionType(ACTMSG_TOGGLE);
									xActuator->Delay(0);
									xActuator->GraphicNumber(iGfxLocalIndex);
									xActuator->SoundEffect(1);
									
									globalGroundObjects[z] = xObject;

									iActuatorCounter++;
									iObjectItemCounter[DB_CATEGORY_ACTUATOR]++;
								}
							}
						}
					}
				}
			}
		}
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);

	}
	FILE_CLOSE(glbDataFileHandle);


	// AGAIN
	{

		int iObjectCounts = 0;
		int iOffsetID = 0;
		int iTileValue = 0;
		int iGroundRefIndex = 0;

		for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++) 
		{
			xCurrentMap = (Map_definition_info*) &dunMapsHeaders[iMapIndex];
			nCols = xCurrentMap->xDim + 1;
			nRows = xCurrentMap->yDim + 1;

			for (i = 0; i < nCols; i++)
			{	
				dunMapTilesObjectIndexPerColumn[iOffsetID] = iObjectCounts;
				for (j = 0; j < nRows; j++)
				{
					z = iMapIndex*1024 + j*32 + i;
					iTileValue = glbMapTileValue[iMapIndex][i][j];
					if ( (iTileValue & 0x10) != 0 && globalGroundObjects[z] != OBJECT_NULL)
					{
						dunGroundStacks[iGroundRefIndex] = globalGroundObjects[z];
						iObjectCounts++;
						iGroundRefIndex++;
					}
				}
				iOffsetID++;
			}
		}
	}

	glbTimersMaximumCount = MAX_TIMER_NUMBER;	// = 50
	INIT_TIMERS();

	for (iDataIndex = 0; iDataIndex < 16; iDataIndex++)
		printf("Object Count DB %02d = %04d [%s]\n", iDataIndex, iObjectItemCounter[iDataIndex], getDatabaseCategoryTypeName(iDataIndex));

	printf("Arrange Dungeon ...\n");
	if (isNewGame != 0) {
		ARRANGE_DUNGEON();
	}
	if (_4976_3b5d != 0) {
		DECIDE_DEFAULT_DUNGEON_MAP_CHIP_SET();
	}
	_4976_3b5d = 0;
	CHANGE_CURRENT_MAP_TO(cd.pi.glbPlayerMap);

	SkCodeParam::bBWMode = true;
	SkCodeParam::bDM1Mode = true; // for graphical adjustments
	SkCodeParam::bGFXFixModeDM1 = true;
	SkCodeParam::bAllowFreeObjectMove = false;
	SkCodeParam::bEnableDoubleStepMove = false;

	return iReadSuccess;
}

#if (XDMX_BLOODWYCH_ENGINE == 1)
U8 SkWinCore::BW_GET_DOOR_BUTTON_CLASS(Door_Info* xDoorInfo)
{
	U8 iButtonClass = 0; // default
	ObjectID rlDoorActuator = OBJECT_NULL;
	Actuator* xActuator = NULL;

	if (xDoorInfo->locked) {
		iButtonClass = 1;
		//-- In order to know which button to display, get the actuator on top of door to retrieve the misc item ID (the key ID) which is expected to unlock the door
		rlDoorActuator = xDoorInfo->next;
		if (rlDoorActuator == OBJECT_NULL || rlDoorActuator == OBJECT_END_MARKER)
			return iButtonClass;

		xActuator = GET_ADDRESS_OF_RECORD3(rlDoorActuator);
		if (xActuator != NULL) {
			Miscellaneous_item* xMisc = NULL;
			if (xActuator->ActuatorType() == ACTUATOR_TYPE_DM1_ITEM_EATER) {
				int iRequestedMiscItemID = xActuator->ActuatorData();
				if (iRequestedMiscItemID >= 0x4F && iRequestedMiscItemID <= 0x56) {
					iButtonClass = (iRequestedMiscItemID - 0x4F) + 1;
				}
			}
		}
	}

	return iButtonClass;
}
#endif // XDMX_BLOODWYCH_ENGINE