//--- SkWinCore-Theron'sQuest -------------------------------------
//SPX: This part of code contains:
// - Special code for reading original TQ's data

#include <StdAfx.h>	// Required for MVC6 compilation

#include <skver.h>

#include <skparam.h>
#include <skdebug.h>
#include <skglobal.h>
#include <skcore.h>
#include <skmidi.h>
#include <sklua.h>
#include <skcnsole.h>
#include <xsfxsdl.h>
#include <string.h>

#include <stdlib.h>

//==============================================================================

int SkWinCore::READ_DUNGEON_STRUCTURE_TQ(X16 isNewGame)
{
	int iReadSuccess = 0;

	int i = 0;
	int j = 0;
	int z = 0;
	int nCols = 0;
	int nRows = 0;
	int nMaps = 0;
	int iMapIndex = 0;
	int iMapDataOffset = 0;
	int iTotalMapDataSize = 0;
	X8 xBuffer[32]; memset(xBuffer, 0, 32);
	X8 xInfoBuffer[16*9]; // max 16 maps * 9 sets of data
	Map_definition_info* xCurrentMap;

	INIT_RANDOM();
	///--- Whatever file was opened (a dungeon.dat ?), we close it
	FILE_CLOSE(glbDataFileHandle);

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	glbDataFileHandle = OPEN_FILE(FORMAT_SKSTR((U8*)".Z020TQUS19.iso", NULL));
	if (1)
	{
		FILE_SEEK(glbDataFileHandle, 0x9B3C2);
		FILE_READ(glbDataFileHandle, 7, xBuffer);
		if (!strcmp((char*)xBuffer, (const char*)"GO AWAY"))
		{
			printf("This looks like a TQ file\n");
		}
		else
		{
			printf("This does not look like a TQ file\n");
			return 0;
		}
	}

	nMaps = 4;
	INIT_BLANK_DUNGEON(isNewGame, nMaps);
	// OK, let's start to read this from dungeon 1 @ 0x0002A0F1
	// read dimensions
	//getDungeon()->nLevels = maps;
	FILE_SEEK(glbDataFileHandle, 0x0002A0F1);

//	dunHeader->w0 = 0x0063; // DM seed
//	dunHeader->cbMapData = 0;
	dunHeader->nMaps = nMaps;
//	dunHeader->b5 = 0;
//	dunHeader->cwTextData = 0;
//	dunHeader->w8 = 0;
//	dunHeader->cwListSize = 0;

	FILE_READ(glbDataFileHandle, 1*nMaps, &xInfoBuffer[nMaps*0]);	// x dims of maps
	FILE_READ(glbDataFileHandle, 1*nMaps, &xInfoBuffer[nMaps*1]);	// y dims of maps
	FILE_READ(glbDataFileHandle, 1*nMaps, &xInfoBuffer[nMaps*2]);	// x offsets of maps
	FILE_READ(glbDataFileHandle, 1*nMaps, &xInfoBuffer[nMaps*3]);	// y offsets of maps
	FILE_READ(glbDataFileHandle, 1*nMaps, &xInfoBuffer[nMaps*4]);	// maps ID (altitude ?)
	FILE_READ(glbDataFileHandle, 1*nMaps, &xInfoBuffer[nMaps*5]);	// ?
	FILE_READ(glbDataFileHandle, 1*nMaps, &xInfoBuffer[nMaps*6]);	// ?
	FILE_READ(glbDataFileHandle, 1*nMaps, &xInfoBuffer[nMaps*7]);	// nb creatures per map
	FILE_READ(glbDataFileHandle, 1*nMaps, &xInfoBuffer[nMaps*8]);	// XP level modifier

	for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++)
	{
		int iLocalMapDataSize = 0;
		xCurrentMap = (Map_definition_info*) &dunMapsHeaders[iMapIndex];

		xCurrentMap->mapOffset = 0;
		xCurrentMap->unused1[0] = 0;
		xCurrentMap->unused1[1] = 0;
		xCurrentMap->unused1[2] = 0;
		xCurrentMap->unused1[3] = 0;

		xCurrentMap->xOffset = xInfoBuffer[nMaps*2+iMapIndex];
		xCurrentMap->yOffset = xInfoBuffer[nMaps*3+iMapIndex];
		xCurrentMap->level = xInfoBuffer[nMaps*4+iMapIndex];
		xCurrentMap->unused2 = 0;
		xCurrentMap->xDim = xInfoBuffer[nMaps*0+iMapIndex]-1;
		xCurrentMap->yDim = xInfoBuffer[nMaps*1+iMapIndex]-1;
		xCurrentMap->nWalls = 0;
		xCurrentMap->rWalls = 0;
		xCurrentMap->nFloors = 0;
		xCurrentMap->rFloors = 0;
		xCurrentMap->nOrnates = 0;
		xCurrentMap->nMonsters = 0; //xInfoBuffer[nMaps*7+iMapIndex];

		xCurrentMap->unknown1 = 0;
		xCurrentMap->depth = xInfoBuffer[nMaps*8+iMapIndex];
		xCurrentMap->unknown2 = 0;
		xCurrentMap->tileset = 0;	// default
		xCurrentMap->door1 = 2; // metal gate
		xCurrentMap->door2 = 0; // iron grate

		xCurrentMap->mapOffset = iMapDataOffset;

		iLocalMapDataSize = ( (xCurrentMap->xDim+1)  * (xCurrentMap->yDim+1) );
		//iLocalMapDataSize += ( xCurrentMap->nWalls + xCurrentMap->nFloors + xCurrentMap->nOrnates + xCurrentMap->nMonsters ); // for TQ, map data does not contain decorations lists
		iTotalMapDataSize += ( iLocalMapDataSize );

		iMapDataOffset += ( iLocalMapDataSize );
	}
	dunHeader->cbMapData = iTotalMapDataSize;
	dunMapData = ALLOC_MEMORY_RAM(dunHeader->cbMapData, afUseUpper, 0x400);

	if (SKLOAD_READ(dunHeader->nRecords, 2*16) == 0)	// 16 * items number (#items per category)
		return 0;

	///--- MAP DATA
	FILE_SEEK(glbDataFileHandle, 0x0002A2D7);
	SkD((DLV_DBG_GAME_LOAD, "Read Dungeon Map Data ...\n"));
	if (SKLOAD_READ(dunMapData, dunHeader->cbMapData) == 0)
		return 0;	
	if (_4976_3b5d != 0) {
		U16 iMapIndex = 0;
		U16 iColCount = 0;
		// SPX: 0x400 = 32*32 tiles / bp10 = #maps / _4976_4cb4 = #cols for all maps
		glbMapTileValue = reinterpret_cast<U8 ***>(ALLOC_MEMORY_RAM((_4976_4cb4 + nMaps) * sizeof(void *), afUseUpper, 0x400));
		//^2066:298A
		U8 ***pTile = &glbMapTileValue[nMaps];	// bp08
		//^2066:299C
		for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++) {
			// MARK SPX
			glbMapTileValue[iMapIndex] = reinterpret_cast<U8 **>(pTile);
			U8 *pTileFromMap = &dunMapData[dunMapsHeaders[iMapIndex].w0];	// w0 = map offset
			*pTile = reinterpret_cast<U8 **>(pTileFromMap);
			pTile++;
			for (iColCount = 1; dunMapsHeaders[iMapIndex].Column() -1 >= iColCount; iColCount++) {
				pTileFromMap += dunMapsHeaders[iMapIndex].Row();
				*pTile = reinterpret_cast<U8 **>(pTileFromMap);
				pTile++;
			}
		}
	}
	for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++)
	{
		xCurrentMap = (Map_definition_info*) &dunMapsHeaders[iMapIndex];
		xCurrentMap->nMonsters = xInfoBuffer[nMaps*7+iMapIndex];
	}
	///--- MAP DATA

	///--- GROUND REFERENCES
	dunHeader->cwListSize = 0xB7;
	FILE_SEEK(glbDataFileHandle, 0x0002A831);
	if (SKLOAD_READ(dunGroundStacks, dunHeader->cwListSize * 2) == 0)
		return 0;

	//--- READ ITEM/OBJECTS DATA (PART 1)
	int iCategoryDBIndex = 0;
	FILE_SEEK(glbDataFileHandle, 0x0002AD0F);
	SkD((DLV_DBG_GAME_LOAD, "Read Objects per Category ...\n"));
	for (iCategoryDBIndex = 0; iCategoryDBIndex < 4; iCategoryDBIndex++) {
		SkD((DLV_DBG_GAME_LOAD, "Category %02d of size %02d => %04d records ...\n", iCategoryDBIndex, glbItemSizePerDB[iCategoryDBIndex], dunHeader->nRecords[iCategoryDBIndex]));
		U16 nRecords = dunHeader->nRecords[iCategoryDBIndex];
		if (isNewGame != 0) {
			dunHeader->nRecords[iCategoryDBIndex] = min_value((iCategoryDBIndex == dbCloud) ? 0x300 : 0x400, tblDefaultNbItemAllocationPerDB[RCJ(16,iCategoryDBIndex)] + nRecords);
		}
		U16 iItemSize = glbItemSizePerDB[iCategoryDBIndex];
		if (_4976_3b5d != 0) {
			glbDBObjectData[iCategoryDBIndex] = ALLOC_MEMORY_RAM(dunHeader->nRecords[iCategoryDBIndex] * iItemSize, afUseUpper, 0x400);
		}
		U16 *pObjectData = (U16 *)glbDBObjectData[iCategoryDBIndex];
		if (SKLOAD_READ(glbDBObjectData[iCategoryDBIndex], iItemSize * nRecords) == 0)
			return 0;
		if (_4976_5bf2 == 0) {
			if (iCategoryDBIndex == DB_CATEGORY_CREATURE || iCategoryDBIndex >= DB_CATEGORY_MISSILE) {
				glbTimersMaximumCount += dunHeader->nRecords[iCategoryDBIndex];
			}
		}
		if (isNewGame != 0) {
			iItemSize >>= 1;
			pObjectData += nRecords * iItemSize;
			for (nRecords = tblDefaultNbItemAllocationPerDB[RCJ(16,iCategoryDBIndex)]; nRecords != 0; nRecords--) {
				*pObjectData = 0xFFFF;
				pObjectData += iItemSize;
			}
		}
	}

	FILE_SEEK(glbDataFileHandle, 0x0002B800);
	for (iCategoryDBIndex = 4; iCategoryDBIndex < 16; iCategoryDBIndex++) {
		SkD((DLV_DBG_GAME_LOAD, "Category %02d of size %02d => %04d records ...\n", iCategoryDBIndex, glbItemSizePerDB[iCategoryDBIndex], dunHeader->nRecords[iCategoryDBIndex]));
		U16 nRecords = dunHeader->nRecords[iCategoryDBIndex];
		//if (isNewGame != 0) {
		//	dunHeader->nRecords[iCategoryDBIndex] = min_value((iCategoryDBIndex == dbCloud) ? 0x300 : 0x400, tblDefaultNbItemAllocationPerDB[RCJ(16,iCategoryDBIndex)] + nRecords);
		//}
		U16 iItemSize = glbItemSizePerDB[iCategoryDBIndex];
		if (_4976_3b5d != 0) {
			glbDBObjectData[iCategoryDBIndex] = ALLOC_MEMORY_RAM(dunHeader->nRecords[iCategoryDBIndex] * iItemSize, afUseUpper, 0x400);
		}
		U16 *pObjectData = (U16 *)glbDBObjectData[iCategoryDBIndex];
		if (SKLOAD_READ(glbDBObjectData[iCategoryDBIndex], iItemSize * nRecords) == 0)
			return 0;
		if (_4976_5bf2 == 0) {
			if (iCategoryDBIndex == DB_CATEGORY_CREATURE || iCategoryDBIndex >= DB_CATEGORY_MISSILE) {
				glbTimersMaximumCount += dunHeader->nRecords[iCategoryDBIndex];
			}
		}
		/*if (isNewGame != 0) {
			iItemSize >>= 1;
			pObjectData += nRecords * iItemSize;
			for (nRecords = tblDefaultNbItemAllocationPerDB[RCJ(16,iCategoryDBIndex)]; nRecords != 0; nRecords--) {
				*pObjectData = 0xFFFF;
				pObjectData += iItemSize;
			}
		}*/
	}

// READ TEXT
	dunHeader->cwTextData = 0x13C;
	if (_4976_3b5d != 0) {
		dunTextData = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(dunHeader->cwTextData << 1, afUseUpper, 0x400));
	}
	if (SKLOAD_READ(dunTextData, dunHeader->cwTextData << 1) == 0)
		return 0;


//{0x0002A0F1, 0x0002A2D7, 0x0002A831, 0x0002AD0F, 0x0002B800, 0x0002C9D8},

	FILE_CLOSE(glbDataFileHandle);
	iReadSuccess = 1;

	// DEBUG, set starting pos @ 2,3
	//dunHeader->w8 = (2) + (3<<5);

	__LOAD_CREATURE_FROM_DUNGEON();
	INIT_DUNGEON_FINALIZE_ARRANGE(1);
	//LOG_FULL_DUNGEON_INFO();

	SkCodeParam::bDM1TQMode = true;

	return iReadSuccess;
}
