//--- SkWinCoreEOB -------------------------------------
//SPX: Experimental code about Eye of the Beholder dungeon reading

#include <StdAfx.h>	// Required for MVC6 compilation

#define UseAltic 1

#include <SkParam.h>
#include <SkDebug.h>
#include <SkGlobal.h>
#include <KCORE.H>
//#include <SkWinCoreEOB.h>
#include <SkMIDI.h>
#include <SkParam.h>

//--- Common part with A.cpp
using namespace DMEncyclopaedia;
using namespace DM2Internal;
using namespace kkBitBlt;

#if defined(_USE_MFC80) || defined(_USE_MFC60)
#include "SkMFC.h"
#include "resource.h"
#endif // defined(_USE_MFC80) || defined(_USE_MFC60)


#ifdef _USE_SDL
#include "SkwinSDL.h"
#endif // _USE_SDL

#define RESET		0
#define BRIGHT 		1
#define DIM			2
#define UNDERLINE 	3
#define BLINK		4
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





#define MIN(a,b) ((a)>(b)?(b):(a))



/*
int tblCustomNbItemAllocationPerDB[16] = 
{
	256, 512, 256, 512, 
	75, 100, 60, 25, 
	12, 5, 200, 0, 
	0, 0, 60, 50
};
*/

//ObjectID* globalGroundObjects = NULL; // for all tiles


int SkWinCore::READ_DUNGEON_STRUCTURE_EOB(X16 isNewGame)
{
	DUNGEON_CREATE_BLANK();

	// Init start position
	glbPlayerPosX = 10;
	glbPlayerPosY = 15;
	glbPlayerDir = 0;
	glbPlayerMap = 0;

	READ_DUNGEON_STRUCTURE_EOB_MAPFILE(0);
	READ_DUNGEON_STRUCTURE_EOB_INFFILE(0);
	return 1;
}

typedef struct
{
	unsigned char	iCreatureIndex;
	unsigned char	iType;
	// Position (from Right bits to left)
	unsigned short	iPosX:5;	// 0-31
	unsigned short	iPosY:5;	// 0-31
	unsigned short	iPosFiller:6;

	unsigned char	iSubPosition;
	unsigned char	iDirection;
	unsigned char	iType2;
	unsigned char	iPictureModel;
	unsigned char	iPhase;	// Variation?
	unsigned char	iPause;
	unsigned short	iItemWeapon;
	unsigned short	iItemPossession;
} eob_CreatureDef;

int SkWinCore::READ_DUNGEON_STRUCTURE_EOB_INFFILE(int iMapIndex)
{
	char	sMAZFileNameString[64];
	Bit8u	iDataBuffer[32];
	Bit8u	xCreatureBuffer[32];

	Map_definition_info *xCurrentMap = NULL;

	sprintf(sMAZFileNameString, ".Z020LEVEL%d.INF", iMapIndex+1);
	glbDataFileHandle = OPEN_FILE(FORMAT_SKSTR((U8*)sMAZFileNameString, NULL));

	printf("Read INF file ...\n");
	//FILE_READ(glbDataFileHandle, 6, iDataBuffer);
	FILE_READ(glbDataFileHandle, 2, iDataBuffer); // 2 bytes
	FILE_READ(glbDataFileHandle, 12, iDataBuffer); // 12 bytes : "levelXX.maz."
	FILE_READ(glbDataFileHandle, 12, iDataBuffer); // 12 bytes : "brick"
	FILE_READ(glbDataFileHandle, 12, iDataBuffer); // 12 bytes : "brick"
	FILE_READ(glbDataFileHandle, 10, iDataBuffer); // 10 bytes
	FILE_READ(glbDataFileHandle, 13, iDataBuffer); // 13 bytes : "kobold"	monster type 1
	FILE_READ(glbDataFileHandle, 13, iDataBuffer); // 13 bytes : "leech"	monster type 2
	FILE_READ(glbDataFileHandle, 4, iDataBuffer); // 4 bytes

	int iCreatureIndex = 0;
	for (iCreatureIndex = 0; iCreatureIndex < 0x1E; iCreatureIndex++)
	//for (iCreatureIndex = 0; iCreatureIndex < 1; iCreatureIndex++)
	{
		int iHitPoints = 10;
		eob_CreatureDef*	xCreature;
		FILE_READ(glbDataFileHandle, 14, xCreatureBuffer);
		xCreature = (eob_CreatureDef*) &xCreatureBuffer;
		printf("[%04X] MON %02d / T=%d", 0,
			xCreatureBuffer[0],
			xCreatureBuffer[1]);
		for (int x = 2; x < 14; x ++)
		{
			printf(" %02X", xCreatureBuffer[x]);
		}
		printf("\n");

		if ( (iMapIndex*2+xCreature->iType) == 0) // kobold
			iHitPoints = 3 + rand()%3;
		if ( (iMapIndex*2+xCreature->iType) == 1) // leech
			iHitPoints = 7 + rand()%6;

		/*
		AddCreatureAt(
			xCreature->iPosX, xCreature->iPosY, m,
			m*2 + xCreature->iType,
			Dungeon::GetNullReference(), xCreature->iSubPosition, 1, iHitPoints, iHitPoints, iHitPoints, iHitPoints);
		*/	
		//DUNGEON_PUT_NEW_CREATURE(xCreature->iType, iMapIndex, xCreature->iPosX, xCreature->iPosY);
	}

	FILE_CLOSE(glbDataFileHandle);

	return 1;
}

int SkWinCore::READ_DUNGEON_STRUCTURE_EOB_MAPFILE(int iMapIndex)
{
	U16* dunEOBMapData = NULL;
	Bit8u iMapDimX;
	Bit8u iMapDimY;
	Bit8u iNbFacesPerTile = 4;
	Bit8u iDataBuffer[32];
	char	sMAZFileNameString[64];

	Map_definition_info *xCurrentMap = NULL;

	sprintf(sMAZFileNameString, ".Z020LEVEL%d.MAZ", iMapIndex+1);
	glbDataFileHandle = OPEN_FILE(FORMAT_SKSTR((U8*)sMAZFileNameString, NULL));

	printf("Read MAZ dimensions ...\n");
	FILE_READ(glbDataFileHandle, 6, iDataBuffer);

	iMapDimX = iDataBuffer[0] + iDataBuffer[1]*256;
	iMapDimY = iDataBuffer[2] + iDataBuffer[3]*256;
	iNbFacesPerTile = iDataBuffer[4];

	printf("Read map data (%d tiles) (%d bytes)\n", iMapDimX*iMapDimX, iMapDimX*iMapDimY*iNbFacesPerTile);
	dunEOBMapData = reinterpret_cast<U16 *> (ALLOC_MEMORY_RAM((iMapDimX*iMapDimY*iNbFacesPerTile)*sizeof(U16), afUseUpper, 0x400));

	FILE_READ(glbDataFileHandle, iMapDimX*iMapDimY*iNbFacesPerTile, dunEOBMapData);
	FILE_CLOSE(glbDataFileHandle);

	//--- Convert tile faces values into DM tiles
	
	{
		int z = 0;
		int i = 0;
		int j = 0;
		int nCols = 0;
		int nRows = 0;
		
		ObjectID xObject = OBJECT_END_MARKER;
		U16 iEOBTileValue = 0;
		int iTileType = 0;
		int iTileFunction = 0;
		U8* xObjectPointer = NULL;
		int iFacing = 0;

		xCurrentMap = (Map_definition_info*) &dunMapsHeaders[iMapIndex];
		nCols = xCurrentMap->xDim + 1;
		nRows = xCurrentMap->yDim + 1;
		if (xCurrentMap->xDim == 0 && xCurrentMap->yDim == 0)
		{
			nCols = nRows = 0;
		}
		xCurrentMap->door1 = 0; // sewer grate
		xCurrentMap->door2 = 0; // sewer grate

		printf("MAP %d\n", iMapIndex);

		CHANGE_CURRENT_MAP_TO(iMapIndex);
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);

		printf("COL) ");
		for (i = 0; i < nCols; i++)
			printf("%02d   ", i);
		printf("\n");
	
		int iHalfWord = iNbFacesPerTile / 2;
		int iFaceValues[9];	// generally 4 faces! but one LOL map weirdly contains 9 values per tile

		int EOB_TILE_DOOR = 0x03;	// Metal Sewer door
		int EOB_TILE_DOOR_NOBUTTON = 0x08;	// Metal Sewer door

		for (j = 0; j < nRows; j++)
		{
			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
			printf("%02d) ", j);
			for (i = 0; i < nCols; i++)
			{
				z = (i*iHalfWord + j*nCols*iHalfWord);
				iEOBTileValue = dunEOBMapData[z];
				iFaceValues[0] = (dunEOBMapData[z] & 0x00FF);
				iFaceValues[1] = (dunEOBMapData[z] & 0xFF00)>>8;
				iFaceValues[2] = (dunEOBMapData[z+1] & 0x00FF);
				iFaceValues[3] = (dunEOBMapData[z+1] & 0xFF00)>>8;
				iTileType = 0;
				iTileFunction = 0;
				if (iEOBTileValue == 0)
					iTileType = 0;
				else iTileType = 1;

				switch (iTileType)
				{
					case 1: CHANGE_CONSOLE_COLOR(BRIGHT, GRAY, BLACK); break;
					case 0: CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK); break;
				}

				printf("%04X.", iEOBTileValue);

				//glbMapTileValue[iMapIndex][i][j] = 0x20; // floor
				DUNGEON_SET_TILE(ttFloor, 0, iMapIndex, i, j);
				if (iEOBTileValue != 0)
					DUNGEON_SET_TILE(ttWall, 0, iMapIndex, i, j);

				//--- stairs down / ladder down
				if ( (iFaceValues[0] == 24 || iFaceValues[2] == 24) ||
					(iFaceValues[1] == 24 || iFaceValues[3] == 24) )
				{
					DUNGEON_SET_TILE(ttStairs, 0, iMapIndex, i, j);
					DUNGEON_SET_TILE_FLAG(TILE_FLAG_IS_LEADING_DOWN, iMapIndex, i, j);
				}

				//--- Door detection
				int iPossibleDoorFaceValues[] = {3, 8, 12, 13, 18, 30, 31};
				bool bThereIsDoor = false;
				bool bRatherNorthSouth = false;
				bool bRatherWestEast = false;
				int iDoorDirFlag = TILE_FLAG_IS_NORTH_SOUTH;
				int iCheckFace = 0;
				int iCheckPossibleDoor = 0;
				for (iCheckFace = 0; iCheckFace < 4; iCheckFace++)
				{
					for (iCheckPossibleDoor = 0; iCheckPossibleDoor < 7; iCheckPossibleDoor++)
					{
						if (iFaceValues[iCheckFace] == iPossibleDoorFaceValues[iCheckPossibleDoor])
						{
							bThereIsDoor = true;
							if (iCheckFace == 0 || iCheckFace == 2)
							{
								bRatherNorthSouth = true;
								iDoorDirFlag = TILE_FLAG_IS_NORTH_SOUTH;
							}
							else if (iCheckFace == 1 || iCheckFace == 3)
							{
								bRatherWestEast = true;
								iDoorDirFlag = TILE_FLAG_IS_WEST_EAST;
							}
						}
					}
				}
				

				if (bThereIsDoor == true)
				{
					//Door* xDoor = NULL;
					Door_Info* xDoor = NULL;
					DUNGEON_SET_TILE(ttDoor, 0, iMapIndex, i, j);
					DUNGEON_PUT_NEW_DOOR(0, iMapIndex, i, j);
					DUNGEON_SET_TILE_FLAG(iDoorDirFlag, iMapIndex, i, j);
					xDoor = (Door_Info*) DUNGEON_GET_DOOR_AT_POSITION(iMapIndex, i, j);
					if (MVALID(xDoor))
					{
						xDoor->ornate = 0;
						//xDoor->SetButtonState(1);
						xDoor->button = 1;
					}
				}

				else if ( iFaceValues[1] == 0x1C ) // pad
				{
					DUNGEON_SET_TILE(ttFloor, 0, iMapIndex, i, j);
					DUNGEON_PUT_FLOOR_DECORATION(1, iMapIndex, i, j); // square pad
				}
				else if ( iFaceValues[1] == 0x19 ) // floor
				{
					DUNGEON_SET_TILE(ttFloor, 0, iMapIndex, i, j);
				}
				
				//--- Test all wall faces for decorations / buttons
				for (int iFaceIndex = 0; iFaceIndex < 4; iFaceIndex++)
				{
					if (iFaceValues[iFaceIndex] == 0x45) // rock wall
					{
						//DUNGEON_PUT_WALL_DECORATION(0x38, iMapIndex, i, j, iFaceIndex); // amalgam
						// Turn the tile to FLOOR then add a ROCK WALL OBJECT (similar to an indestructible CAVE-IN)
						DUNGEON_SET_TILE(ttFloor, 0, iMapIndex, i, j);
						DUNGEON_PUT_NEW_CREATURE(0x20, iMapIndex, i, j);
					}

					else if (iFaceValues[iFaceIndex] == 43) // round grate
						DUNGEON_PUT_WALL_DECORATION(0x22, iMapIndex, i, j, iFaceIndex); // grate
					else if (iFaceValues[iFaceIndex] == 44) // arched grate with eyes
						DUNGEON_PUT_WALL_DECORATION(0x22, iMapIndex, i, j, iFaceIndex); // grate
					else if (iFaceValues[iFaceIndex] == 62) // arched grate
						DUNGEON_PUT_WALL_DECORATION(0x22, iMapIndex, i, j, iFaceIndex); // grate

					
					else if (iFaceValues[iFaceIndex] == 0x37) // lever (up)
						DUNGEON_PUT_WALL_DECORATION(0x2C, iMapIndex, i, j, iFaceIndex);

					/*
					else if (iFaceValues[iFaceIndex] == 29) // alcove
						DUNGEON_PUT_WALL_DECORATION(0x01, iMapIndex, i, j, iFaceIndex);


					else if (iFaceValues[iFaceIndex] == 63) // pipe outlet
						DUNGEON_PUT_WALL_DECORATION(0x0C, iMapIndex, i, j, iFaceIndex); // slime outlet


					else if (iFaceValues[iFaceIndex] == 0x27) // big brick button
						DUNGEON_PUT_WALL_DECORATION(0x31, iMapIndex, i, j, iFaceIndex);
					else if (iFaceValues[iFaceIndex] == 0x32) // tiny button
						DUNGEON_PUT_WALL_DECORATION(0x0E, iMapIndex, i, j, iFaceIndex);
					else if (iFaceValues[iFaceIndex] == 0x3C) // brick button
						DUNGEON_PUT_WALL_DECORATION(0x07, iMapIndex, i, j, iFaceIndex);

					else if (iFaceValues[iFaceIndex] == 0x41) // rune entrance
						DUNGEON_PUT_WALL_DECORATION(0x41, iMapIndex, i, j, iFaceIndex);
					else if (iFaceValues[iFaceIndex] == 0x18) // ladder down
						;
					else if (iFaceValues[iFaceIndex] == 0x3A) // side door frame, handled by door tile
						;
					*/
				}

			}
			printf("\n");
			// -- Second row , only for raw display
			printf("%02d) ", j);
			for (i = 0; i < nCols; i++)
			{
				z = (i*iHalfWord + j*nCols*iHalfWord) + 1;
				iEOBTileValue = dunEOBMapData[z];
				iTileType = 0;
				iTileFunction = 0;
				if (iEOBTileValue == 0)
					iTileType = 0;
				else iTileType = 1;

				switch (iTileType)
				{
					case 1: CHANGE_CONSOLE_COLOR(BRIGHT, GRAY, BLACK); break;
					case 0: CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK); break;
				}

				printf("%04X.", iEOBTileValue);
			}
			printf("\n");

		
		} // j loop
	}

	return 1;
}



int SkWinCore::READ_DUNGEON_STRUCTURE_EOB__OLD(X16 isNewGame)
{
	int iReadSuccess = 0;
	int iTotalNbTiles = 0;
	int iTotalMapDataSize = 0;
	int iMapIndex = 0;
	int nMaps = 1; // can be reduced the real number of non null maps
	int nEOBMaps = 1;
	int iDBCategory = 0;
	int iMapDataOffset = 0;
	int iDataIndex = 0;
	int iTileIndex = 0;
	int iRefObjectIndex = 0;

	U16 iCreatureObjectCounter = 0;
	int iDoorObjectCounter = 0;
	int iSimpleActuatorCounter = 0;
	int iMiscItemCounter = 0;
	int iWeaponCounter = 0;
	int iObjectItemCounter[16];

	int iFirstMapWithZeroDims = 8;

	int i = 0;
	int j = 0;
	int z = 0;
	int nCols = 0;
	int nRows = 0;

	bool bNoCreature = 0;
	bool bNoDoor = 0;

	int nMaxCreatures = 512;


int iNumTileStartOffset[8];
int iNbTilesPerMap[8];
int iGlobalMapsDimX[8];





	ObjectID* globalGroundObjects = NULL; // for all tiles
	//U16* globalGroundObjects = NULL; // for all tiles
	Map_definition_info *xCurrentMap = NULL;
	U16* dunEOBMapData = NULL;

	// 8 maps
	// 8 bytes for columns per map, then 8 bytes for rows per map
	Bit8u iMapDimX[8];
	Bit8u iMapDimY[8];
	Bit8u iNbFacesPerTile = 4;

	Bit8u iDummy[32];
	Bit8u iDataBuffer[32];

	INIT_RANDOM();

	FILE_CLOSE(glbDataFileHandle);	// Close whatever has been opened yet (dungeon.dat by default)
	glbDataFileHandle = OPEN_FILE(FORMAT_SKSTR((U8*)".Z020LEVEL1.MAZ", NULL));

	printf("Read MAZ dimensions ...\n");
	FILE_READ(glbDataFileHandle, 6, iDataBuffer);

	iMapDimX[0] = iDataBuffer[0] + iDataBuffer[1]*256;
	iMapDimY[0] = iDataBuffer[2] + iDataBuffer[3]*256;
	iNbFacesPerTile = iDataBuffer[4];
	nEOBMaps = 1;


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
				globalGroundObjects[z] = OBJECT_END_MARKER;
			}


	SkCodeParam::bEOBMode = true;

	dunHeader->w0 = 0x4245; // random seed, saying "EB"
	dunHeader->cbMapData = 0;
	dunHeader->nMaps = nEOBMaps;
	dunHeader->b5 = 0;
	dunHeader->cwTextData = 0;
	dunHeader->w8 = 0;
	dunHeader->cwListSize = 0;
	for (iDBCategory = 0; iDBCategory < 16; iDBCategory++)
		dunHeader->nRecords[iDBCategory] = 0;

	int iGlobalMapOffsetCount = 0;
	for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++)
	{
		iTotalMapDataSize += (iMapDimX[iMapIndex] * iMapDimY[iMapIndex]);
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

		xCurrentMap->unknown1 = 0;
		xCurrentMap->depth = 0;
		xCurrentMap->unknown2 = 0;
		xCurrentMap->tileset = 3;
		xCurrentMap->door1 = 0; // sewer grate
		xCurrentMap->door2 = 0; // sewer grate
		//xCurrentMap->door2 = 2; // metal gate

		xCurrentMap->mapOffset = iMapDataOffset;
		iMapDataOffset += (iMapDimX[iMapIndex] * iMapDimY[iMapIndex] );

		// globals, for object positionning (tile number to tile coords)
		iNumTileStartOffset[iMapIndex] = iGlobalMapOffsetCount;
		iNbTilesPerMap[iMapIndex] = (iMapDimX[iMapIndex] * iMapDimY[iMapIndex]);
		iGlobalMapOffsetCount += iNbTilesPerMap[iMapIndex];
		iGlobalMapsDimX[iMapIndex] = xCurrentMap->xDim + 1;


	}
	printf("Map data would be = %05d (%04X)\n", iTotalMapDataSize, iTotalMapDataSize);
	dunHeader->cbMapData = iTotalMapDataSize;
	dunMapData = ALLOC_MEMORY_RAM(dunHeader->cbMapData, afUseUpper, 0x400);


	printf("Read map data (%d tiles) (%d bytes)\n", iMapDimX[0]*iMapDimX[2], iMapDimX[0]*iMapDimX[2]*iNbFacesPerTile);
	dunEOBMapData = reinterpret_cast<U16 *> (ALLOC_MEMORY_RAM((iMapDimX[0]*iMapDimX[2]*iNbFacesPerTile)*sizeof(U16), afUseUpper, 0x400));
	FILE_READ(glbDataFileHandle, iMapDimX[0]*iMapDimX[2]*iNbFacesPerTile, dunEOBMapData);


	// Init map data with floors
	for (iTileIndex = 0; iTileIndex < dunHeader->cbMapData; iTileIndex++)
		dunMapData[iTileIndex] = 0x20;

	for (iMapIndex = 0; iMapIndex < nEOBMaps; iMapIndex++)
	{
		xCurrentMap = (Map_definition_info*) &dunMapsHeaders[iMapIndex];
		xCurrentMap->xOffset = iMapDimX[iMapIndex];
		xCurrentMap->yOffset = iMapDimY[iMapIndex];
	}

	FILE_READ(glbDataFileHandle, 8, iDummy);


	dunMapColumnsSumArray = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(nMaps << 1, afUseUpper, 0x400));

	// Init start position
	glbPlayerPosX = 10;
	glbPlayerPosY = 15;
	glbPlayerDir = 0;
	glbPlayerMap = 0;

	Bit16u nColumnCounter = 0;

	for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++) 
	{
		dunMapColumnsSumArray[iMapIndex] = nColumnCounter;
		nColumnCounter += dunMapsHeaders[iMapIndex].Column();
	
		dunMapsHeaders[iMapIndex].bGfxFlags = MAPGFX_FLAG__PIT_UPPER_ROOF | MAPGFX_FLAG__PIT_LOWER_GROUND | MAPGFX_FLAG__STAIRS_GOING_UP | MAPGFX_FLAG__STAIRS_GOING_DOWN | MAPGFX_FLAG__TELEPORTER | MAPGFX_FLAG__DOOR_0 | MAPGFX_FLAG__DOOR_1;
		dunMapsHeaders[iMapIndex].w14 = (0 << 4) + (dunMapsHeaders[iMapIndex].w14 & 0xFF00); // tileset = 3 (keep)
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
		if (iDBCategory == DB_CATEGORY_SIMPLE_ACTUATOR)
			nRecords = dunHeader->nRecords[iDBCategory] = 512;	// Allocate 512 simple actuators
		if (iDBCategory == DB_CATEGORY_CREATURE)
			nRecords = dunHeader->nRecords[iDBCategory] = nMaxCreatures;	// Allocate XXX empty creatures
		if (iDBCategory == DB_CATEGORY_DOOR)
			nRecords = dunHeader->nRecords[iDBCategory] = 128;	// Allocate 128 empty doors
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
				xCreature->b5 = 0;
				xCreature->hp1 = 10;
				xCreature->w8 = 0;
				xCreature->w10 = 0;
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
		ObjectID xObject = OBJECT_END_MARKER;
		U16 iEOBTileValue = 0;
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
	
		int iHalfWord = iNbFacesPerTile / 2;
		int iFaceValues[9];	// generally 4 faces! but one LOL map weirdly contains 9 values per tile

		int EOB_TILE_DOOR = 0x03;	// Metal Sewer door
		int EOB_TILE_DOOR_NOBUTTON = 0x08;	// Metal Sewer door

		for (j = 0; j < nRows; j++)
		{
			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
			printf("%02d) ", j);
			for (i = 0; i < nCols; i++)
			{
				z = (i*iHalfWord + j*nCols*iHalfWord);
				iEOBTileValue = dunEOBMapData[z];
				iFaceValues[0] = (dunEOBMapData[z] & 0xFF00)>>8;
				iFaceValues[1] = (dunEOBMapData[z] & 0x00FF);
				iFaceValues[2] = (dunEOBMapData[z+1] & 0xFF00)>>8;
				iFaceValues[3] = (dunEOBMapData[z+1] & 0x00FF);
				iTileType = 0;
				iTileFunction = 0;
				if (iEOBTileValue == 0)
					iTileType = 0;
				else iTileType = 1;

				switch (iTileType)
				{
					case 1: CHANGE_CONSOLE_COLOR(BRIGHT, GRAY, BLACK); break;
					case 0: CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK); break;
				}

				printf("%04X.", iEOBTileValue);

				//glbMapTileValue[iMapIndex][i][j] = 0x20; // floor
				DUNGEON_SET_TILE(ttFloor, 0, iMapIndex, i, j);
				if (iEOBTileValue != 0)
					DUNGEON_SET_TILE(ttWall, 0, iMapIndex, i, j);

				//--- 03 door with button
				if (iFaceValues[0] == EOB_TILE_DOOR_NOBUTTON || iFaceValues[2] == EOB_TILE_DOOR_NOBUTTON)
				{
					//dTile.tTile.Type = DM_TILE_DOOR;
					//dTile.tDoor.facing = DOOR_FACING__NORTH_SOUTH;
					//bDoorButton = true;
					DUNGEON_SET_TILE(ttDoor, 0, iMapIndex, i, j);
					DUNGEON_PUT_NEW_DOOR(0, iMapIndex, i, j);
					DUNGEON_SET_TILE_FLAG(TILE_FLAG_IS_NORTH_SOUTH, iMapIndex, i, j);
				}
				else if (iFaceValues[1] == EOB_TILE_DOOR_NOBUTTON || iFaceValues[3] == EOB_TILE_DOOR_NOBUTTON)
				{
					//dTile.tTile.Type = DM_TILE_DOOR;
					//dTile.tDoor.facing = DOOR_FACING__WEST_EAST;
					//bDoorButton = true;
					DUNGEON_SET_TILE(ttDoor, 0, iMapIndex, i, j);
					DUNGEON_PUT_NEW_DOOR(0, iMapIndex, i, j);
					DUNGEON_SET_TILE_FLAG(TILE_FLAG_IS_WEST_EAST, iMapIndex, i, j);
				}

			}
			printf("\n");
			// -- Second row , only for raw display
			printf("%02d) ", j);
			for (i = 0; i < nCols; i++)
			{
				z = (i*iHalfWord + j*nCols*iHalfWord) + 1;
				iEOBTileValue = dunEOBMapData[z];
				iTileType = 0;
				iTileFunction = 0;
				if (iEOBTileValue == 0)
					iTileType = 0;
				else iTileType = 1;

				switch (iTileType)
				{
					case 1: CHANGE_CONSOLE_COLOR(BRIGHT, GRAY, BLACK); break;
					case 0: CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK); break;
				}

				printf("%04X.", iEOBTileValue);
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
	glbTimersMaximumCount = MAX_TIMER_NUMBER;	// = 50
	INIT_TIMERS();

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

	for (iDataIndex = 0; iDataIndex < 16; iDataIndex++)
		printf("Object Count DB %02d = %04d\n", iDataIndex, iObjectItemCounter[iDataIndex]);

	printf("Arrange Dungeon ...\n");
	if (isNewGame != 0) {
		ARRANGE_DUNGEON();
	}
	if (_4976_3b5d != 0) {
		DECIDE_DEFAULT_DUNGEON_MAP_CHIP_SET();
	}
	_4976_3b5d = 0;
	CHANGE_CURRENT_MAP_TO(glbPlayerMap);

	SkCodeParam::bBWMode = true;

	return iReadSuccess;
}
