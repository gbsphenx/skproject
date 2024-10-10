//--- SkWinCoreBW -------------------------------------
//SPX: Experimental code about Bloodwych dungeon reading

#include <StdAfx.h>	// Required for MVC6 compilation

#include <SkVer.h>

#include <SkParam.h>
#include <SkDebug.h>
#include <SkGlobal.h>
#include <KCORE.H>
//#include <SkWinCoreBW.h>
#include <SkMIDI.h>
#include <SkParam.h>

//--- Common part with A.cpp
using namespace DMEncyclopaedia;
using namespace DM2Internal;
using namespace kkBitBlt;

#if defined(_USE_MFC80) || defined(_USE_MFC60)
#include <SkMFC.h>
#include <resource.h>
#endif // defined(_USE_MFC80) || defined(_USE_MFC60)


#ifdef _USE_SDL
#include <SkSDL.h>
#endif // _USE_SDL
#ifdef __DJGPP__
#include <SkDOS.h>
#include <stdlib.h> // rand note: putting stdlib here and not right after stdafx prevents a bunch of conflicts with min/max macros
#endif // __DJGPP__

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
		
	printf("Transforming tile number %04X to coordinates (%d, %02d,%02d)\n", iTileNumber, *iCoordsMap, *iCoordsX, *iCoordsY);

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


	ObjectID* globalGroundObjects = NULL; // for all tiles
	//U16* globalGroundObjects = NULL; // for all tiles
	Map_definition_info *xCurrentMap = NULL;
	U16* dunBWMapData = NULL;

	// 8 maps
	// 8 bytes for columns per map, then 8 bytes for rows per map
	Bit8u iMapDimX[8];
	Bit8u iMapDimY[8];

	Bit8u iDummy[32];

	INIT_RANDOM();

	printf("Read 8*2 bytes (map dimensions) ...\n");

	if (FILE_READ(glbDataFileHandle, nBWMaps, iMapDimX) == 0)
		return 0;
	if (FILE_READ(glbDataFileHandle, nBWMaps, iMapDimY) == 0)
		return 0;

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


	SkCodeParam::bBWMode = true;

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
		xCurrentMap->door1 = 2; // metal gate
		xCurrentMap->door2 = 0; // iron grate

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

	if (iFirstMapWithZeroDims < nBWMaps) // < 8
	{
		dunHeader->nMaps = iFirstMapWithZeroDims;
		nMaps = iFirstMapWithZeroDims;
	}

	// Init map data with floors
	for (iTileIndex = 0; iTileIndex < dunHeader->cbMapData; iTileIndex++)
		dunMapData[iTileIndex] = 0x20;

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
	FILE_READ(glbDataFileHandle, 2*iTotalMapDataSize, dunBWMapData);

	dunMapColumnsSumArray = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(nMaps << 1, afUseUpper, 0x400));

	// Init start position
	glbPlayerPosX = 8;
	glbPlayerPosY = 11;
	glbPlayerDir = 0;
	glbPlayerMap = 3;

	Bit16u nColumnCounter = 0;

	for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++) 
	{
		dunMapColumnsSumArray[iMapIndex] = nColumnCounter;
		nColumnCounter += dunMapsHeaders[iMapIndex].Column();
	
		dunMapsHeaders[iMapIndex].bGfxFlags = MAPGFX_FLAG__PIT_UPPER_ROOF | MAPGFX_FLAG__PIT_LOWER_GROUND | MAPGFX_FLAG__STAIRS_GOING_UP | MAPGFX_FLAG__STAIRS_GOING_DOWN | MAPGFX_FLAG__TELEPORTER | MAPGFX_FLAG__DOOR_0 | MAPGFX_FLAG__DOOR_1;
		dunMapsHeaders[iMapIndex].w14 = (3 << 4) + (dunMapsHeaders[iMapIndex].w14 & 0xFF00); // tileset = 3 (keep)
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
			nRecords = dunHeader->nRecords[iDBCategory] = nMaxCreatures;	// Allocate XXX empty creatures, to get the pillars
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
				z = i + j*nCols + xCurrentMap->mapOffset;
				iBWTileValue = dunBWMapData[z];
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

				if ( iTileType == 1 )
				{
					int iIsObjectOnWall = 0;
					int iObjectFace = 0;
					//glbMapTileValue[iMapIndex][i][j] = 0x00; // wall

					iIsObjectOnWall = (iBWTileValue>>12) & 0x0F;
					iObjectFace = (iBWTileValue & 0x3000)>>12;
					// Objects on wall
					// 0100 : on north face
					// 1100 : on east face
					// 2100 : on south face
					// 3100 : on west face
					if (iIsObjectOnWall != 0)
					{
						// Create a simple actuator for alcoves and gfx
						
						if (iObjectItemCounter[DB_CATEGORY_SIMPLE_ACTUATOR] < 256)
						//if (iObjectItemCounter[DB_CATEGORY_SIMPLE_ACTUATOR] < 0)
						{
							int iWallDecorationType = iTileFunction & 0x03;
							int iColor = ((((i+j))%8)+8)%8; // rule for color based on tile position (tapestries)
							//glbMapTileValue[iMapIndex][i][j] = 0x10; // wall
							DUNGEON_SET_TILE(ttWall, 0, iMapIndex, i, j);
							DUNGEON_SET_TILE_FLAG(TILE_FLAG_HAS_OBJECT, iMapIndex, i, j);

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
								case 0: 
									xSimpleActuator->graphism = 1; // alcove
									break;
								case 1: 
									xSimpleActuator->graphism = 0x25+iColor; // tapestry
									break;
								case 2: 
									xSimpleActuator->graphism = 0x0F; // gem button
									break;
								case 3: 
									xSimpleActuator->graphism = 0x4B; // gem slot (borrowing fire shooter gfx)
									break;
							}

							iFacing = iObjectFace;

							xObject = ObjectID(iFacing, DB_CATEGORY_SIMPLE_ACTUATOR, iObjectItemCounter[DB_CATEGORY_SIMPLE_ACTUATOR]);
							ATLASSERT(xObject.DBType() == DB_CATEGORY_SIMPLE_ACTUATOR);
							z = iMapIndex*1024 + j*32 + i;
							globalGroundObjects[z] = xObject;
							
							//printf("Adding Simple actuator #%03d @ (%d,%d,%d) as (%d,%d,G-%02d,%d)\n", iSimpleActuatorCounter, iMapIndex, i, j,
							//	xSimpleActuator->activated, xSimpleActuator->mode, xSimpleActuator->graphism, xSimpleActuator->extendedUsage);
							//iSimpleActuatorCounter++;
							iObjectItemCounter[DB_CATEGORY_SIMPLE_ACTUATOR]++;

							// Test
							/*
							if (iWallDecorationType == 0) // alcove
							{
								// add item into alcove
								if (iMiscItemCounter < 256)
								{
									U8* xItemDataPointer = NULL;
									Miscellaneous_item* xItem;
									xItemDataPointer = glbDBObjectData[DB_CATEGORY_MISC_ITEM];
									xMiscItemList = (Miscellaneous_item*) xItemDataPointer;
									xItem = (Miscellaneous_item*) &xMiscItemList[iMiscItemCounter];

									xItem->w0 = OBJECT_END_MARKER;
									xItem->w2 = 0;
									xItem->ItemType(5+ (rand()%(22-5))); // apple
									xObject = ObjectID(iFacing, DB_CATEGORY_MISC_ITEM, iMiscItemCounter);
									iMiscItemCounter++;

									xSimpleActuator->w0 = xObject;
								}
							}
							*/
						}
					}
				}


				if ( iTileType == 3 && iTileFunction == 1 ) // pillar
				{
					//glbMapTileValue[iMapIndex][i][j] = 0x20; // wall
					DUNGEON_SET_TILE(ttWall, 0, iMapIndex, i, j);
					
					//if (iTileFunction == 1 && iCreatureObjectCounter < nMaxCreatures && bNoCreature == 0 && iMapIndex == 3)
					if (iTileFunction == 1 && iObjectItemCounter[DB_CATEGORY_CREATURE] < nMaxCreatures && bNoCreature == 0 && iMapIndex == 3)
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
						xCreature->b5 = 0;
						xCreature->hp1 = 255;
						xCreature->w8 = 0;
						xCreature->w10 = 0;
						xCreature->w12 = 0;
						xCreature->b14 = 0;
						xCreature->b15 = 0;

						xObject = ObjectID(0, DB_CATEGORY_CREATURE, iObjectItemCounter[DB_CATEGORY_CREATURE]);
						z = iMapIndex*1024 + j*32 + i;
						globalGroundObjects[z] = xObject;

						printf("Creature #%03d/%03d type PILLAR at position %d (%02d,%02d)\n", iObjectItemCounter[DB_CATEGORY_CREATURE], nMaxCreatures, iMapIndex, i, j);
						//iCreatureObjectCounter++;
						iObjectItemCounter[DB_CATEGORY_CREATURE]++;
					}
					
				}
				if (iTileType == 3 && iTileFunction == 0) // bed
				{
					//glbMapTileValue[iMapIndex][i][j] = 0x20; // floor
					DUNGEON_SET_TILE(ttFloor, 0, iMapIndex, i, j);
					
					//if (iTileFunction == 0 && iCreatureObjectCounter < nMaxCreatures && bNoCreature == 0 && iMapIndex == 3)
					if (iTileFunction == 0 && iObjectItemCounter[DB_CATEGORY_CREATURE] < nMaxCreatures && bNoCreature == 0 && iMapIndex == 3)
					{
						//glbMapTileValue[iMapIndex][i][j] = 0x30; // floor
						DUNGEON_SET_TILE_FLAG(TILE_FLAG_HAS_OBJECT, iMapIndex, i, j);

						xObjectPointer = glbDBObjectData[DB_CATEGORY_CREATURE];
						xCreatureList = (Creature*) xObjectPointer;
						xCreature = (Creature*) &xCreatureList[iObjectItemCounter[DB_CATEGORY_CREATURE]];
								
						xCreature->w0 = OBJECT_END_MARKER;
						xCreature->possession = OBJECT_END_MARKER;
						xCreature->b4 = 0x7F;
						xCreature->b5 = 0;
						xCreature->hp1 = 255;
						xCreature->w8 = 0;
						xCreature->w10 = 0;
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
				
/*
				if (iTileType == 4) // stairs
				{
					int iDMLeading = 0; // DM Down
					int iBWLeading = (iTileFunction & 0x01); // 1 = Down
					int iDMFacing = 0; // west/east
					int iBWFacing = (iTileFunction & 0x06);
					glbMapTileValue[iMapIndex][i][j] = 0x60;
					// 0x0400 : up, towards south
					// 0x0402 : up, towards west
					// 0x0404 : up, towards north
					// 0x0406 : up, towards east
					// 0x04x1 : down
					iDMLeading = !iBWLeading;
					switch (iBWFacing)
					{
						case 0:
						case 4:
							iDMFacing = 1; break; // north / south
						case 2:
						case 6:
							iDMFacing = 0; break; // west / east
					};
					glbMapTileValue[iMapIndex][i][j] = 0x60 + (iDMLeading)<<1 + (iDMFacing)<<2;
				}
*/
				if (iTileType == 5) // doors
				{
					// 0x0501 metal door facing N/S
					// 0x0503 iron grate facing N/S
					// 0x0505 metal door facing W/E
					// 0x0507 iron grate facing W/E
					// 0x15Zx door closed to be unlocked by key "Z"
					// 0x050F locked, can't be unlocked by key, requires a special pad/trigger
					int iDoorType = 0;
					int iDoorFacing = 0;
					int iDoorSpecial = 0;
					int iDMDoorFacing = 0; // 0 is west/east

					//glbMapTileValue[iMapIndex][i][j] = 0x80; // door
					DUNGEON_SET_TILE(ttDoor, 0, iMapIndex, i, j);

					iDoorType = (iTileFunction & 0x02)>>1; // 0 = metal, 1 = iron grate
					iDoorFacing = (iTileFunction & 0x04)>>2; // 0 = N/S, 1 = W/E
					iDMDoorFacing = !iDoorFacing;

					glbMapTileValue[iMapIndex][i][j] = 0x80 + (iDMDoorFacing<<3);
										
					//if (iDoorObjectCounter < 256 && bNoDoor == 0)
					if (iObjectItemCounter[DB_CATEGORY_DOOR] < 256 && bNoDoor == 0)
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
						if (iDoorType == 1) // iron grate
							xDoor->verticalOpening = 1;

						/*
						if (iDoorFunction == 1 || iDoorFunction == 5)
							xDoor->type = 1; // metal gate
						if (iDoorFunction == 3 || iDoorFunction == 7) // north/south orientation
							glbMapTileValue[iMapIndex][i][j] = 0x98;
						*/
						glbMapTileValue[iMapIndex][i][j] = 0x90 + (iDMDoorFacing<<3);
						

						glbMapTileValue[iMapIndex][i][j] += 0x04; // the door is closed

						xObject = ObjectID(0, DB_CATEGORY_DOOR, iObjectItemCounter[DB_CATEGORY_DOOR]);
						z = iMapIndex*1024 + j*32 + i;
						globalGroundObjects[z] = xObject;

						iDoorObjectCounter++;
						iObjectItemCounter[DB_CATEGORY_DOOR]++;
					}
					
				}

				if ( iTileType == 6 && iTileFunction == 1 )	// pit
				{
					//glbMapTileValue[iMapIndex][i][j] = 0x40; // pit
					DUNGEON_SET_TILE(ttPit, 0, iMapIndex, i, j);
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
	//glbDataFileHandle = OPEN_FILE(FORMAT_SKSTR(sDungeonFilename, NULL));
	//.Z020DUNGEON.Z024.DAT.
//	glbDataFileHandle = OPEN_FILE(FORMAT_SKSTR((U8*)".Z020MOD0.OB.", NULL));
	glbDataFileHandle = OPEN_FILE(FORMAT_SKSTR((U8*)".Z020MOD0.CR.", NULL));
	if (1)
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
			ObjectID xObject = OBJECT_END_MARKER;
			ObjectID_Reference* xObjectReference = NULL;
			U8* xCreatureData = &xCreatureRawData[16*iCreatureIndex];

			iPositionX = xCreatureData[0];
			iPositionY = xCreatureData[1];
			iQuarter = xCreatureData[2];
			//iSpeed = xCreatureData[3];
			iMapIndex = xCreatureData[4];
			//iCurrentState = xCreatureData[5];
			//iLevelNColor = xCreatureData[6];
			iHitPoints = xCreatureData[8] + (xCreatureData[9]*256);
			//iDirection = xCreatureData[10];
			iCreatureType = xCreatureData[11];

			printf("Creature #%03d/%03d type %02d (%02X) at map %d (%02d,%02d)\n",
				iObjectItemCounter[DB_CATEGORY_CREATURE], nMaxCreatures, iCreatureType, iCreatureType, iMapIndex, iPositionX, iPositionY);
			//if (iCreatureType >= 100 && iCreatureObjectCounter < 253 && bNoCreature == 0)
			if (iObjectItemCounter[DB_CATEGORY_CREATURE] < 253 && bNoCreature == 0)
			{
				if (iPositionX == 255 || iPositionX > 32)	// that might be a removed creature
					continue;
				//if (iCreatureType > 16)
				//	continue;

				//glbMapTileValue[iMapIndex][iPositionX][iPositionY] = 0x30; // floor
				DUNGEON_SET_TILE(ttFloor, 0, iMapIndex, i, j);
				DUNGEON_SET_TILE_FLAG(TILE_FLAG_HAS_OBJECT, iMapIndex, i, j);

				xObjectPointer = glbDBObjectData[DB_CATEGORY_CREATURE];
				xCreatureList = (Creature*) xObjectPointer;
				xCreature = (Creature*) &xCreatureList[iObjectItemCounter[DB_CATEGORY_CREATURE]];
						
				xCreature->w0 = OBJECT_END_MARKER;
				xCreature->possession = OBJECT_END_MARKER;
				xCreature->b4 = iCreatureType;	// type
				xCreature->b4 = 100;	// type
				xCreature->b5 = 0;
				xCreature->hp1 = iHitPoints;
				xCreature->w8 = 0;
				xCreature->w10 = 0;
				xCreature->w12 = 0;
				xCreature->b14 = 0;
				xCreature->b15 = 0;

				xObject = ObjectID(0, DB_CATEGORY_CREATURE, iObjectItemCounter[DB_CATEGORY_CREATURE]);
				xObjectReference = (ObjectID_Reference*) &xObject;
				z = iMapIndex*1024 + iPositionY*32 + iPositionX;
				globalGroundObjects[z] = xObject;
				printf("Ground Record added : %04x (%02d|%04d d=%d)\n", xObject, xObjectReference->iDBType, xObjectReference->iID, xObjectReference->iDir);

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
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	glbDataFileHandle = OPEN_FILE(FORMAT_SKSTR((U8*)".Z020MOD0.OB.", NULL));
	if (0)
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

					printf("\n");

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

					printf("OB#%03d TN=%04X NB=%d - ", iNbItems, iTileNumber, iItemLoops);
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
								printf("ID=%02d (%02X) Qty=%d - ", iItemID, iItemID, iQuantity);
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
												printf("Item (%02x|%02x) added to ground\n", iDBType, iItemID);
												

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

												printf("Item (%02x|%02x) stacked on previous item\n", iDBType, iItemID);

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