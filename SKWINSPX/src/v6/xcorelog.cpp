//--- SkWinCore-Log -------------------------------------
//SPX: This part of code contains:
// - Retrocompatibility with DM1
// - New code for expanding DM2

#if defined (SKDLL_EXPORTS) || defined (LIBSKWINDLL_EXPORTS)
#include <StdAfx.h>
#endif // LIBSKWINDLL_EXPORTS

#include <skcnsole.h>

#include <skver.h>
#include <skparam.h>
#include <skdebug.h>
#include <skglobal.h>
#include <skcore.h>
#include <skmidi.h>
#include <sklua.h>

//--- Common part with A.cpp
//using namespace DMEncyclopaedia;
//using namespace DM2Internal;
//using namespace kkBitBlt;

#include <string.h>


//#ifdef _USE_SDL
//#include <sksdl.h>
//#endif // _USE_SDL
//#if defined(__DJGPP__) || defined (__MINGW__)
//#include <skdos.h>
#include <stdlib.h> // rand note: putting stdlib here and not right after stdafx prevents a bunch of conflicts with min/max macros
//#endif // __DJGPP__

//#if defined (__MINGW__)
//#undef WIN32
//#endif



void CHANGE_CONSOLE_COLOR_FROM_DB_TYPE(int iDBType)
{
	switch (iDBType)
	{
		case DB_CATEGORY_DOOR:
			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK); break;
		case DB_CATEGORY_TELEPORTER:
			CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK); break;
		case DB_CATEGORY_TEXT:
			CHANGE_CONSOLE_COLOR(BRIGHT, WHITE, BLACK); break;
		case DB_CATEGORY_ACTUATOR:
			CHANGE_CONSOLE_COLOR(BRIGHT, GRAY, BLACK); break;
		case DB_CATEGORY_CREATURE:
			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_RED, BLACK); break;
		case DB_CATEGORY_WEAPON:
			CHANGE_CONSOLE_COLOR(BRIGHT, WHITE, BLACK); break;
		case DB_CATEGORY_CLOTHING:
			CHANGE_CONSOLE_COLOR(BRIGHT, RED, BLACK); break;
		case DB_CATEGORY_SCROLL:
			CHANGE_CONSOLE_COLOR(BRIGHT, WHITE, BLACK); break;
		case DB_CATEGORY_POTION:
			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_BLUE, BLACK); break;
		case DB_CATEGORY_CONTAINER:
			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_YELLOW, BLACK); break;
		case DB_CATEGORY_MISC_ITEM:
			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GREEN, BLACK); break;
		case DB_CATEGORY_MISSILE:
			CHANGE_CONSOLE_COLOR(BRIGHT, PURPLE, BLACK); break;
		case DB_CATEGORY_CLOUD:
			CHANGE_CONSOLE_COLOR(BRIGHT, AQUA, BLACK); break;
	}
}


void CHANGE_CONSOLE_COLOR_FROM_TILE(int iTileData)
{
	int iTileType = (iTileData & 0xF0)>>5;
	switch (iTileType)
	{
		case ttWall:
			CHANGE_CONSOLE_COLOR(BRIGHT, GRAY, BLACK); break;
		case ttFloor:
			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK); break;
		case ttPit:
			CHANGE_CONSOLE_COLOR(BRIGHT, RED, BLACK); break;
		case ttStairs:
			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_RED, BLACK); break;
		case ttDoor:
			CHANGE_CONSOLE_COLOR(BRIGHT, WHITE, BLACK); break;
		case ttTeleporter:
			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_BLUE, BLACK); break;
		case ttTrickWall:
			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_YELLOW, BLACK); break;
		case ttMapExit:
			CHANGE_CONSOLE_COLOR(BRIGHT, PURPLE, BLACK); break;
	}
}


void SkWinCore::LOG_HEXA(X8* pData, UINT iNbBytes)
{
	X8* pX = (X8*) pData;

	if (SkCodeParam::bRenderingEngineDOS)
		return;

	printf("PX: %16X | ", pX);
	for (UINT i = 0; i < iNbBytes; i++) {
		printf("%02X ", (X8) *pX);
		pX++;
	}
	printf("\n");
}


void SkWinCore::LOG_DUNGEON_INFO_GROUND_STACKS()
{
	UINT i = 0;
	U16 iMapIndex = 0;

	if (SkCodeParam::bRenderingEngineDOS)
		return;

	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	printf("\nGROUND STACKS DATA\n");
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("dunGroundStacks  : %08X\n", dunGroundStacks);
	printf("dunHeader->cwListSize  : %04d\n", dunHeader->cwListSize);
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	for (i = 0; i < dunHeader->cwListSize; i++)
	{
		U16 iGroundStack = 0;
		int iDBType = 0;
		if (i%32 == 0)
		{
			CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
			printf("x.%04X d.%04d ) ", i, i);
		}
		iGroundStack = dunGroundStacks[i];
		iDBType = ObjectID(iGroundStack).DBType();
		if (iGroundStack == OBJECT_END_MARKER || iGroundStack == OBJECT_NULL)
			CHANGE_CONSOLE_COLOR(BRIGHT, GRAY, BLACK);
		else
			CHANGE_CONSOLE_COLOR_FROM_DB_TYPE(iDBType);
		printf("%04X ", iGroundStack);
		if (i%32 == 31)
			printf("\n");
	}
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("\n");

	//--------------------------------------------------------------------------
	U16 nColumnCounter = 0;
	int nMaps = dunHeader->nMaps;
	for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++) 
	{
		nColumnCounter += dunMapsHeaders[iMapIndex].Column();
	}
	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	printf("\nOBJECTS PER COLUMN OFFSETS COUNTS\n");
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("dunMapTilesObjectIndexPerColumn  : %08X\n", dunMapTilesObjectIndexPerColumn);
	printf("nColumnCounter                   : %04d\n", nColumnCounter);
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	U16 iCountOffset = 0;
	U16 iPreviousOffset = 0;
	for (i = 0; i < nColumnCounter; i++)
	{
		if (i%32 == 0)
		{
			CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
			printf("x.%04X d.%04d ) ", i, i);
		}
		iCountOffset = dunMapTilesObjectIndexPerColumn[i];
		if (iCountOffset == iPreviousOffset)
			CHANGE_CONSOLE_COLOR(BRIGHT, GRAY, BLACK);
		else
			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
		printf("%04X ", iCountOffset);
		if (i%32 == 31)
			printf("\n");
		iPreviousOffset = iCountOffset;
	}
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("\n");

}


void SkWinCore::LOG_FULL_DUNGEON_INFO()
{
	int i = 0;
	int j = 0;
	int iMapIndex = 0;

	if (SkCodeParam::bRenderingEngineDOS)
		return;

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_YELLOW, BLACK);

	printf("****************************************************\n");
	printf("TECHNICAL INFO\n");
	printf("****************************************************\n");

	printf("Byte-size for ObjectID = %d (%d-bits)\n", sizeof(OID_T), sizeof(OID_T)*8);
	printf("OID_T shift = %d\n", OID_SIZE_BITSHIFT);
	printf("OID NULL = %08X\n", OBJECT_NULL.w);
	printf("OID EOM  = %08X\n", OBJECT_END_MARKER.w);
	
	printf("****************************************************\n");
	printf("LOG FULL DUNGEON INFO\n");
	printf("****************************************************\n");

	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	printf("Header Information\n");
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	//File_header *dunHeader;		// file header (_4976_4d0e)
	printf("dunHeader->w0           Random Seed      : %04X [%c%c]\n", dunHeader->w0, ((char*)&dunHeader->w0)[0], ((char*)&dunHeader->w0)[1]);
	printf("dunHeader->cbMapData    Map Data Size    : %04X\n", dunHeader->cbMapData);
	printf("dunHeader->nMaps        Nb. Maps         : %02d\n", dunHeader->nMaps);
	printf("dunHeader->b5           Filler           : %02X\n", dunHeader->b5);
	printf("dunHeader->cwTextData   Text Data Size   : %04X\n", dunHeader->cwTextData);
	printf("dunHeader->w8           Starting Position: %04X\n", dunHeader->w8);
	printf("dunHeader->cwListSize   List Data Size   : %04X\n", dunHeader->cwListSize);
	for (i = 0; i < 16; i++)
	printf("dunHeader->nRecords[%02d] #Rec.DB (%02d) : %04X  (%04d)\n", i, i, dunHeader->nRecords[i], dunHeader->nRecords[i]);

	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	printf("\nGlobal Player Information\n");
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("glbPlayerPosX  : %02d\n", cd.pi.glbPlayerPosX);
	printf("glbPlayerPosY  : %02d\n", cd.pi.glbPlayerPosY);
	printf("glbPlayerDir   : %02d\n", cd.pi.glbPlayerDir);
	printf("glbPlayerMap   : %02d\n", cd.pi.glbPlayerMap);

	/*
		U16 StartPartyPosX() const { return (w8)&0x1f; }
		U16 StartPartyPosY() const { return (w8 >> 5)&0x1f; }
*/


	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	printf("\nTEXT DATA\n");
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("dunTextData  : %08X\n", dunTextData);
	printf("dunHeader->cwTextData  : %04d\n", dunHeader->cwTextData);


	//--------------------------------------------------------------------------
	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	printf("\nGROUND STACKS DATA\n");
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("dunGroundStacks  : %08X\n", dunGroundStacks);
	printf("dunHeader->cwListSize  : %04d\n", dunHeader->cwListSize);
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	for (i = 0; i < dunHeader->cwListSize; i++)
	{
		U16 iGroundStack = 0;
		int iDBType = 0;
		if (i%32 == 0)
		{
			CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
			printf("x.%04X d.%04d ) ", i, i);
		}
		iGroundStack = dunGroundStacks[i];
		iDBType = ObjectID(iGroundStack).DBType();
		if (iGroundStack == OBJECT_END_MARKER || iGroundStack == OBJECT_NULL)
			CHANGE_CONSOLE_COLOR(BRIGHT, GRAY, BLACK);
		else
			CHANGE_CONSOLE_COLOR_FROM_DB_TYPE(iDBType);
		printf("%04X ", iGroundStack);
		if (i%32 == 31)
			printf("\n");
	}
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("\n");

	//--------------------------------------------------------------------------
	Bit16u nColumnCounter = 0;
	int nMaps = dunHeader->nMaps;
	for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++) 
	{
		nColumnCounter += dunMapsHeaders[iMapIndex].Column();
	}
	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	printf("\nOBJECTS PER COLUMN OFFSETS COUNTS\n");
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("dunMapTilesObjectIndexPerColumn  : %08X\n", dunMapTilesObjectIndexPerColumn);
	printf("nColumnCounter                   : %04d\n", nColumnCounter);
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	U16 iCountOffset = 0;
	U16 iPreviousOffset = 0;
	for (i = 0; i < nColumnCounter; i++)
	{
		if (i%32 == 0)
		{
			CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
			printf("x.%04X d.%04d ) ", i, i);
		}
		iCountOffset = dunMapTilesObjectIndexPerColumn[i];
		if (iCountOffset == iPreviousOffset)
			CHANGE_CONSOLE_COLOR(BRIGHT, GRAY, BLACK);
		else
			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
		printf("%04X ", iCountOffset);
		if (i%32 == 31)
			printf("\n");
		iPreviousOffset = iCountOffset;
	}
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("\n");


	//--------------------------------------------------------------------------
	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	printf("\nDOORS DATA\n");

	int iDBCategory = DB_CATEGORY_DOOR;
	Bit16u nRecords = dunHeader->nRecords[iDBCategory];
	Bit16u iItemSize = glbItemSizePerDB[iDBCategory];
	U8* xObjectPointer = NULL;
	xObjectPointer = glbDBObjectData[iDBCategory];

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("dunHeader->nRecords[iDBCategory]  : %04d\n", nRecords);
	printf("glbItemSizePerDB[iDBCategory]     : %04d\n", iItemSize);
	printf("glbDBObjectData[iDBCategory]      : %08X\n", xObjectPointer);

	for (i = 0; i < nRecords; i++)
	{
		Door_Info* xDoor = (Door_Info*) xObjectPointer;
		ObjectID next;
		next = xDoor->next;
		CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
		printf("%04d) ", i);
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
		if (next == OBJECT_NULL)
			CHANGE_CONSOLE_COLOR(BRIGHT, GRAY, BLACK);
		printf("%02X%02X %02X%02X    ", xObjectPointer[0], xObjectPointer[1], xObjectPointer[2], xObjectPointer[3]);
		xObjectPointer += iItemSize;
		if (i%8 == 7)
			printf("\n");
	}
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("\n");


	//--------------------------------------------------------------------------
	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	printf("\nWEAPON DATA\n");

	iDBCategory = DB_CATEGORY_WEAPON;
	nRecords = dunHeader->nRecords[iDBCategory];
	iItemSize = glbItemSizePerDB[iDBCategory];
	xObjectPointer = NULL;
	xObjectPointer = glbDBObjectData[iDBCategory];

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("dunHeader->nRecords[iDBCategory]  : %04d\n", nRecords);
	printf("glbItemSizePerDB[iDBCategory]     : %04d\n", iItemSize);
	printf("glbDBObjectData[iDBCategory]      : %08X\n", xObjectPointer);

	for (i = 0; i < nRecords; i++)
	{
		Miscellaneous_item* xItem = (Miscellaneous_item*) xObjectPointer;
		ObjectID next;
		next = xItem->w0;
		CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
		printf("%04d) ", i);
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
		if (next == OBJECT_NULL)
			CHANGE_CONSOLE_COLOR(BRIGHT, GRAY, BLACK);
		printf("%02X%02X %02X%02X    ", xObjectPointer[0], xObjectPointer[1], xObjectPointer[2], xObjectPointer[3]);
		xObjectPointer += iItemSize;
		if (i%8 == 7)
			printf("\n");
	}
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("\n");

	//--------------------------------------------------------------------------
	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	printf("\nCLOTHING DATA\n");

	iDBCategory = DB_CATEGORY_CLOTHING;
	nRecords = dunHeader->nRecords[iDBCategory];
	iItemSize = glbItemSizePerDB[iDBCategory];
	xObjectPointer = NULL;
	xObjectPointer = glbDBObjectData[iDBCategory];

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("dunHeader->nRecords[iDBCategory]  : %04d\n", nRecords);
	printf("glbItemSizePerDB[iDBCategory]     : %04d\n", iItemSize);
	printf("glbDBObjectData[iDBCategory]      : %08X\n", xObjectPointer);

	for (i = 0; i < nRecords; i++)
	{
		Miscellaneous_item* xItem = (Miscellaneous_item*) xObjectPointer;
		ObjectID next;
		next = xItem->w0;
		CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
		printf("%04d) ", i);
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
		if (next == OBJECT_NULL)
			CHANGE_CONSOLE_COLOR(BRIGHT, GRAY, BLACK);
		printf("%02X%02X %02X%02X    ", xObjectPointer[0], xObjectPointer[1], xObjectPointer[2], xObjectPointer[3]);
		xObjectPointer += iItemSize;
		if (i%8 == 7)
			printf("\n");
	}
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("\n");


	//--------------------------------------------------------------------------
	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	printf("\nSCROLL DATA\n");

	iDBCategory = DB_CATEGORY_SCROLL;
	nRecords = dunHeader->nRecords[iDBCategory];
	iItemSize = glbItemSizePerDB[iDBCategory];
	xObjectPointer = NULL;
	xObjectPointer = glbDBObjectData[iDBCategory];

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("dunHeader->nRecords[iDBCategory]  : %04d\n", nRecords);
	printf("glbItemSizePerDB[iDBCategory]     : %04d\n", iItemSize);
	printf("glbDBObjectData[iDBCategory]      : %08X\n", xObjectPointer);

	for (i = 0; i < nRecords; i++)
	{
		Scroll* xItem = (Scroll*) xObjectPointer;
		ObjectID next;
		next = xItem->w0;
		CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
		printf("%04d) ", i);
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
		if (next == OBJECT_NULL)
			CHANGE_CONSOLE_COLOR(BRIGHT, GRAY, BLACK);
		printf("%02X%02X %02X%02X    ", xObjectPointer[0], xObjectPointer[1], xObjectPointer[2], xObjectPointer[3]);
		xObjectPointer += iItemSize;
		if (i%8 == 7)
			printf("\n");
	}
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("\n");


	//--------------------------------------------------------------------------
	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	printf("\nPOTION ITEM DATA\n");

	iDBCategory = DB_CATEGORY_POTION;
	nRecords = dunHeader->nRecords[iDBCategory];
	iItemSize = glbItemSizePerDB[iDBCategory];
	xObjectPointer = NULL;
	xObjectPointer = glbDBObjectData[iDBCategory];

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("dunHeader->nRecords[iDBCategory]  : %04d\n", nRecords);
	printf("glbItemSizePerDB[iDBCategory]     : %04d\n", iItemSize);
	printf("glbDBObjectData[iDBCategory]      : %08X\n", xObjectPointer);

	for (i = 0; i < nRecords; i++)
	{
		Potion* xItem = (Potion*) xObjectPointer;
		ObjectID next;
		next = xItem->w0;
		CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
		printf("%04d) ", i);
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
		if ((xObjectPointer[3] & 0x7F) == 0x13) // FUL BOMB
			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_RED, BLACK);
		if ((xObjectPointer[3] & 0x7F) == 0x03) // VEN BOMB
			CHANGE_CONSOLE_COLOR(BRIGHT, GREEN, BLACK);
		if (next == OBJECT_NULL)
			CHANGE_CONSOLE_COLOR(BRIGHT, GRAY, BLACK);
		printf("%02X%02X %02X%02X    ", xObjectPointer[0], xObjectPointer[1], xObjectPointer[2], xObjectPointer[3]);
		xObjectPointer += iItemSize;
		if (i%8 == 7)
			printf("\n");
	}
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("\n");


	//--------------------------------------------------------------------------
	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	printf("\nCONTAINER / MAGIC MAP DATA\n");

	iDBCategory = DB_CATEGORY_CONTAINER;
	nRecords = dunHeader->nRecords[iDBCategory];
	iItemSize = glbItemSizePerDB[iDBCategory];
	xObjectPointer = NULL;
	xObjectPointer = glbDBObjectData[iDBCategory];

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("dunHeader->nRecords[iDBCategory]  : %04d\n", nRecords);
	printf("glbItemSizePerDB[iDBCategory]     : %04d\n", iItemSize);
	printf("glbDBObjectData[iDBCategory]      : %08X\n", xObjectPointer);

	for (i = 0; i < nRecords; i++)
	{
		int iGlobalColor = LIGHT_GRAY;
		int c = 0;
		int iDBType = 0;
		Container* xItem = (Container*) xObjectPointer;
		ObjectID next;
		ObjectID possession;
		next = xItem->w0;
		possession = xItem->w2;

		CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
		printf("%04d) ", i);
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);

		iDBType = ObjectID(possession).DBType();

		if (next == OBJECT_NULL)
		{
			CHANGE_CONSOLE_COLOR(BRIGHT, GRAY, BLACK);
			iGlobalColor = GRAY;
		}
		for (c = 0; c < 8; c++)
		{
			if (c == 2 || c == 3)	// possession
				CHANGE_CONSOLE_COLOR_FROM_DB_TYPE(iDBType);
			if (c >= 4)
				CHANGE_CONSOLE_COLOR(BRIGHT, iGlobalColor, BLACK);
			printf("%02X", xObjectPointer[c]);
			if (c%2 == 1) printf(" ");
			
		}
		xObjectPointer += iItemSize;
		if (i%6 == 5)
			printf("\n");
	}
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("\n");

	//--------------------------------------------------------------------------
	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	printf("\nMISC ITEM DATA\n");

	iDBCategory = DB_CATEGORY_MISC_ITEM;
	nRecords = dunHeader->nRecords[iDBCategory];
	iItemSize = glbItemSizePerDB[iDBCategory];
	xObjectPointer = NULL;
	xObjectPointer = glbDBObjectData[iDBCategory];

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("dunHeader->nRecords[iDBCategory]  : %04d (%04X)\n", nRecords, nRecords);
	printf("glbItemSizePerDB[iDBCategory]     : %04d\n", iItemSize);
	printf("glbDBObjectData[iDBCategory]      : %08X\n", xObjectPointer);

	for (i = 0; i < nRecords; i++)
	{
		Miscellaneous_item* xItem = (Miscellaneous_item*) xObjectPointer;
		ObjectID next;
		next = xItem->w0;
		CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
		printf("%04X) ", i);
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
		if (next == OBJECT_NULL)
			CHANGE_CONSOLE_COLOR(BRIGHT, GRAY, BLACK);
		printf("%02X%02X %02X%02X    ", xObjectPointer[0], xObjectPointer[1], xObjectPointer[2], xObjectPointer[3]);
		xObjectPointer += iItemSize;
		if (i%8 == 7)
			printf("\n");
	}
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("\n");

	//--------------------------------------------------------------------------
	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	printf("\nCREATURE DATA\n");

	iDBCategory = DB_CATEGORY_CREATURE;
	nRecords = dunHeader->nRecords[iDBCategory];
	iItemSize = glbItemSizePerDB[iDBCategory];
	xObjectPointer = NULL;
	xObjectPointer = glbDBObjectData[iDBCategory];

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("dunHeader->nRecords[iDBCategory]  : %04d\n", nRecords);
	printf("glbItemSizePerDB[iDBCategory]     : %04d\n", iItemSize);
	printf("glbDBObjectData[iDBCategory]      : %08X\n", xObjectPointer);

	for (i = 0; i < nRecords; i++)
	{
		int c = 0;
		int iDBType = 0;
		int iGlobalColor = LIGHT_GRAY;
		Creature* xCreature = (Creature*) xObjectPointer;
		ObjectID next;
		ObjectID possession;
		next = xCreature->w0;
		possession = xCreature->possession;
		iDBType = ObjectID(possession).DBType();

			CHANGE_CONSOLE_COLOR_FROM_DB_TYPE(iDBType);

		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_RED, BLACK);
		printf("%04d) ", i);
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
		if (next == OBJECT_NULL)
		{
			CHANGE_CONSOLE_COLOR(BRIGHT, GRAY, BLACK);
			iGlobalColor = GRAY;
		}
		for (c = 0; c < 16; c++)
		{
			if (c == 2 || c == 3)	// possession
				CHANGE_CONSOLE_COLOR_FROM_DB_TYPE(iDBType);
			if (c >= 4)
				CHANGE_CONSOLE_COLOR(BRIGHT, iGlobalColor, BLACK);
			printf("%02X", xObjectPointer[c]);
			if (c%2 == 1) printf(" ");
			
		}
		printf("   ");
		xObjectPointer += iItemSize;
		if (i%4 == 3)
			printf("\n");
	}
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("\n");

	//--------------------------------------------------------------------------
	// TIMERS DATA
	int iTimerIndex = 0;
	int iSizeofTimersTable = glbTimersMaximumCount * 10;	// size of Timers table (nb timers * 10 (size of timer)
	int iSizeofNextEntriesTable = glbTimersMaximumCount << 1; // size of "Next entries" table (nb timers * 2 (size of index)
	Timer* xCurrentTimer = NULL;
	//glbTimersMaximumCount;

	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	printf("\nTIMER DATA\n");

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("glbTimersMaximumCount        : %04d\n", glbTimersMaximumCount);
	printf("glbTimersCount               : %04d\n", glbTimersCount);
	printf("iSizeofTimersTable (nb.Timers *10) : %04d\n", iSizeofTimersTable);
	printf("glbTimersTable                     : %08X\n", glbTimersTable);
	printf("iSizeofNextEntriesTable (nb. Timers *2) : %04d\n", iSizeofNextEntriesTable);
	printf("glbTimerNextEntries                     : %08X\n", glbTimerNextEntries);

	xCurrentTimer = (Timer*) glbTimersTable;
	xObjectPointer = (U8*) glbTimersTable;

	for (iTimerIndex = 0; iTimerIndex < glbTimersMaximumCount; iTimerIndex++)
	{
		int c = 0;
		Timer* xTimer = (Timer*) xCurrentTimer;

		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_PURPLE, BLACK);
		printf("%04d ", iTimerIndex);
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GREEN, BLACK);
		printf("[%08X]) ", xTimer);
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
		for (c = 0; c < 10; c++)
		{
			printf("%02X", xObjectPointer[c]);
			if (c%2 == 1) printf(" ");
		}
		printf("   ");

		if (iTimerIndex%4 == 3)
			printf("\n");

		xCurrentTimer++;
		xObjectPointer += 10;
	}
	printf("\n");

	xCurrentTimer = (Timer*) glbTimerNextEntries;
	xObjectPointer = (U8*) glbTimerNextEntries;

	for (iTimerIndex = 0; iTimerIndex < glbTimersMaximumCount; iTimerIndex++)
	{
		int c = 0;
		Timer* xTimer = (Timer*) xCurrentTimer;

		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_YELLOW, BLACK);
		printf("%04d) ", iTimerIndex);
//		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GREEN, BLACK);
//		printf("[%08X]) ", xTimer);
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
		for (c = 0; c < 2; c++)
		{
			printf("%02X", xObjectPointer[c]);
			if (c%2 == 1) printf(" ");
		}
		printf("   ");

		if (iTimerIndex%8 == 7)
			printf("\n");

		xCurrentTimer++;
		xObjectPointer += 2;
	}
	printf("\n");

	//--------------------------------------------------------------------------
	// MAP DATA
	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	printf("\nMAPS DATA\n");

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("dunHeader->nMaps        : %04d\n", dunHeader->nMaps);
	printf("dunHeader->cbMapData    : %08X\n", dunHeader->cbMapData);
	printf("dunMapData              : %08X\n", dunMapData);


	Map_definition_info *xCurrentMap = NULL;

	for (iMapIndex = 0; iMapIndex < dunHeader->nMaps; iMapIndex++)
	{
		int nCols = 0;
		int nRows = 0;
		unsigned char xMapCharData = 0;
		char sHeadColLine[(32+3)*3]; memset(sHeadColLine, 0, (32+3)*3);
		char sHeadBuf[8];
		xCurrentMap = (Map_definition_info*) &dunMapsHeaders[iMapIndex];

		nCols = xCurrentMap->xDim+1;
		nRows = xCurrentMap->yDim+1;

		CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
		printf("\nMAP DATA # %02d\n", iMapIndex);
		printf("Dimensions : %02d x %02d\n", nCols, nRows);

		sprintf(sHeadColLine, "    ");
		for (i = 0; i < nCols; i++) {
			sprintf(sHeadBuf, "%02d ", i);
			strcat(sHeadColLine, sHeadBuf);
		}
		sprintf(sHeadBuf, "\n", i);
		strcat(sHeadColLine, sHeadBuf);

		CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
		printf(sHeadColLine);
		for (j = 0; j < nRows; j++)
		{
			CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
			printf("%02d) ", j);

			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
			for (i = 0; i < nCols; i++)
			{
				xMapCharData = glbMapTileValue[iMapIndex][i][j];
				CHANGE_CONSOLE_COLOR_FROM_TILE(xMapCharData);
				printf("%02X ", xMapCharData);
			}
			printf("\n");
		}
		printf("\n");

	}
	printf("\n");

	//--------------------------------------------------------------------------
	// MEMENT
	int iMemEntIndex = 0;
	int iSizeofMementTable = glbNumberOfMements * sizeof(mement);
	int iPointerSize = 4;
	mement** xMemEntPointer = NULL;
	mement* xMemEntData = NULL;

	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	printf("\nMEM ENT POINTERS DATA\n");

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("glbNumberOfMements      : %04d\n", glbNumberOfMements);
	printf("tblMementsPointers      : %08X\n", tblMementsPointers);

	xMemEntPointer = (mement**) tblMementsPointers;
	xObjectPointer = (U8*) tblMementsPointers;

	if (SkCodeParam::bCompilation64bits)
		iPointerSize = 8;

	//--- Addresses / Pointers
	for (iMemEntIndex = 0; iMemEntIndex < glbNumberOfMements; iMemEntIndex++)
	{
		int c = 0;

		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_PURPLE, BLACK);
		printf("%04d ", iMemEntIndex);
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GREEN, BLACK);
		if (SkCodeParam::bCompilation64bits)
			printf("[%016X]) ", xMemEntPointer);
		else
			printf("[%08X]) ", xMemEntPointer);
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
		//if (xObjectPointer[0] == 0 && xObjectPointer[1] == 0 && xObjectPointer[2] == 0 && xObjectPointer[3] == 0)
		if (*xObjectPointer == 0)
			CHANGE_CONSOLE_COLOR(BRIGHT, GRAY, BLACK);
		for (c = 0; c < iPointerSize; c++)
		{
			printf("%02X", xObjectPointer[c]);
			if (c%2 == 1) printf(" ");
		}

		printf("   ");

		if (SkCodeParam::bCompilation64bits) {
			if (iMemEntIndex%4 == 3)			printf("\n");
		}
		else {
			if (iMemEntIndex%6 == 5)			printf("\n");
		}

		xMemEntPointer++;
		xObjectPointer += sizeof(xObjectPointer);
	}
	printf("\n\n");

	//--- Addresses / Pointers
	xMemEntPointer = (mement**) tblMementsPointers;
	for (iMemEntIndex = 0; iMemEntIndex < glbNumberOfMements; iMemEntIndex++)
	{
		int c = 0;

		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_PURPLE, BLACK);
		printf("%04d ", iMemEntIndex);
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GREEN, BLACK);
		if (SkCodeParam::bCompilation64bits)
			printf("[%016X]) ", *xMemEntPointer); // address within pointer
		else
			printf("[%08X]) ", *xMemEntPointer); // address within pointer
		xMemEntData = (mement*) *xMemEntPointer;
		xObjectPointer = (U8*) xMemEntData;

		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
		if (xObjectPointer != NULL)
		{
			for (c = 0; c < 18; c++)
			{
				printf("%02X", xObjectPointer[c]);
				if (c%2 == 1) printf(" ");
			}
		}
		else
			for (c = 0; c < 18; c++)
			{
				printf("--");
				if (c%2 == 1) printf(" ");
			}

		printf("   ");

		if (SkCodeParam::bCompilation64bits) {
			if (iMemEntIndex%2 == 1)			printf("\n");
		}
		else {
			if (iMemEntIndex%3 == 2)			printf("\n");
		}

		xMemEntPointer++;
	}
	printf("\n");


	//--------------------------------------------------------------------------
	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	printf("\nRECTANGLES TABLE\n");

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("glbRectNoTable    : %08X\n", glbRectNoTable);

	{
		int iRectTableIndex = 0;
		RectTable* pRectTable = &glbRectNoTable;

		while (pRectTable != NULL) {
			printf("RECT %03d [%p] => [%p] %04d %04d . %03d %03d\n", iRectTableIndex, pRectTable, pRectTable->pNextTable, pRectTable->iRectNoMin, pRectTable->iRectNoMax, pRectTable->b8, pRectTable->b9);
			pRectTable = pRectTable->pNextTable;
			iRectTableIndex++;
		}
	}

/*
	//glbRectNoTable
	Map_definition_info *xCurrentMap = NULL;

	for (iMapIndex = 0; iMapIndex < dunHeader->nMaps; iMapIndex++)
	{
		int nCols = 0;
		int nRows = 0;
		unsigned char xMapCharData = 0;
		xCurrentMap = (Map_definition_info*) &dunMapsHeaders[iMapIndex];

		nCols = xCurrentMap->xDim+1;
		nRows = xCurrentMap->yDim+1;

		CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
		printf("\nMAP DATA # %02d\n", iMapIndex);
		printf("Dimensions : %02d x %02d\n", nCols, nRows);

		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
		for (j = 0; j < nRows; j++)
		{
			CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
			printf("%02d) ", j);

			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
			for (i = 0; i < nCols; i++)
			{
				xMapCharData = glbMapTileValue[iMapIndex][i][j];
				CHANGE_CONSOLE_COLOR_FROM_TILE(xMapCharData);
				printf("%02X ", xMapCharData);
			}
			printf("\n");
		}
		printf("\n");

	}
	printf("\n");
	*/


	//--------------------------------------------------------------------------


	printf("\n");
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_YELLOW, BLACK);
	printf("****************************************************\n");
	printf("****************************************************\n");
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
}





void SkWinCore::DEBUG_HELP_WRITER(const char* sinfo, const void* xdata, unsigned int blocksize, unsigned int repeat)
{
	unsigned int icursor = 0;
	unsigned int block = 0;
	unsigned int bcursor = 0;
	unsigned char* pdata = (unsigned char*) xdata;
	printf("Savegame helper : <%s> (%d groups of size %d)\n", sinfo, repeat, blocksize);
	for (block = 0; block < repeat; block++)
	{
		printf("(G: %03d): ", block);
		icursor = block * blocksize;
		for (bcursor = 0; bcursor < blocksize; bcursor++)
		{
			printf("%02X.", pdata[icursor + bcursor]);
		}
		printf("\n");
	}
}

void SkWinCore::DEBUG_HELP_DISPLAY_STACK(i16 iMapX, i16 iMapY, i16 iMapLevel)
{
#if defined(_DEBUG) && !defined(__DJGPP__)
	U16 xGroundItem = 0;
	U32 index = (GET_OBJECT_INDEX_FROM_TILE(iMapX, iMapY)).w;
	ObjectID xFirstObject = GET_TILE_RECORD_LINK(iMapX, iMapY);
	ObjectID xCurrentObject = xFirstObject;
	U16 iObjectDirection = 0;
	U16 iObjectDBType = 0;

	if (index != -1)
		xGroundItem = dunGroundStacks[index];

	printf("--------------------------------------------\n");
	printf("Object stack at %02d,%02d\n", iMapX, iMapY);
	printf("Ground Item is %04X\n", xGroundItem);

	for (index = 0; xCurrentObject != OBJECT_END_MARKER; xCurrentObject = GET_NEXT_RECORD_LINK(xCurrentObject))
	{
		iObjectDirection = xCurrentObject.Dir();
		iObjectDBType = xCurrentObject.DBType();
		printf("%d) %04X (DB=%d/f=%d)\n", index, xCurrentObject.w, iObjectDBType, iObjectDirection);
		index++;
	}
	printf("--------------------------------------------\n");
#endif

}


void SkWinCore::DEBUG_SHOW_RECT_INFO(SRECT xRectZone)
{
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_YELLOW, BLACK);
	printf("RECTZONE INFO: pos = %d,%d size = %d,%d\n", xRectZone.x, xRectZone.y, xRectZone.cx, xRectZone.cy);
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
}

void SkWinCore::DEBUG_SHOW_MULTIRECT_INFO(sk3f6c xMultiRects)
{
	CHANGE_CONSOLE_COLOR(BRIGHT, WHITE, BLACK);
	printf("MULTIRECTZONE INFO: cache main %d -- cache %d\n", xMultiRects.iCacheIndex0, xMultiRects.iCacheIndex);
	DEBUG_SHOW_RECT_INFO(xMultiRects.rc2);
	for (int i = 0; i < 5; i++)
		DEBUG_SHOW_RECT_INFO(xMultiRects.w12[i]);
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
}

void SkWinCore::DEBUG_SHOW_ICON_PICT_BUFF(
	const U8 *buff, 
	sk3f6c *tt, 
	SRECT *rc, 
	i16 srcx, 
	i16 srcy, 
	i16 colorkey, 
	i16 flipmirror, 
	U8 *localpal)
{
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_RED, BLACK);
	printf("ICON PICT BUFF: %p SIZE = (%d,%d) @%d\n", buff, READ_IMGBUFF_WIDTH(buff), READ_IMGBUFF_HEIGHT(buff), READ_IMGBUFF_BPP(buff));
	printf(">> Local Palette: %p ");
	if (localpal != NULL) {
		for (int iPalIdx = 0; iPalIdx < 16; iPalIdx++)
			printf("%02X ", localpal[iPalIdx]);
	}
	printf("\n");
	DEBUG_SHOW_RECT_INFO(*rc);
	DEBUG_SHOW_MULTIRECT_INFO(*tt);
	

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_RED, BLACK);
	printf("ICON PICT BUFF - END\n");
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
}

void SkWinCore::DEBUG_SHOW_MEMENT(mement* pMement, bool bPrintOK)
{
	if (!CheckSafePointer(pMement)) {
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_YELLOW, BLACK);
		printf("INVALID POINTER! %p\n", pMement);
		return;
	}
	if (bPrintOK)
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GREEN, BLACK);
	else
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_RED, BLACK);
	if (pMement->iNegBuffSize != *(((U32*)pMement)-1))
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_RED, BLACK);

	printf("MEMENT %p (IMG: (%3d,%3d)@%d bpp | cache %5d | w %04X-%04X-%04X | negsize = %6d [-4]=> %6d || p1 %p - p2 %p\n", pMement,
		pMement->_w14, pMement->_w16, pMement->_w12, pMement->iMemEntCacheIndex, pMement->_w4, pMement->_w6, pMement->_w8, 
		pMement->iNegBuffSize, *(((U32*)pMement)-1), pMement->xMementRef1, pMement->xMementRef2);
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
}

void SkWinCore::DEBUG_SHOW_GDAT_ENTRIES_MEM()
{
	static char tsType[16][16] = { "0", "Image", "Sound", "Music", "Rect", "Text", "Raw6", "Raw7", "Raw8", "PalIRGB", "10", "Word", "PictOff", "PalIdx", "14", "15" };
	U16 iEntryIndex = 0;
	U16 iRawDatIndex = 0;
	U8 iCls1Category = 0;
	static int iDebugTabColor[16];
	static int iDebugTabData[16] = {1, 1, 1, 1,		1, 1, 1, 1,		1, 1, 0, 0,		0, 1, 1, 1};
	static char sDatType[2][5] = { "....", "DATA" };
	static int iDebugTabPresentShelf[2] = {GRAY, LIGHT_GREEN};
	static char sPresent[2][8] = { "absent", "present" };
	U8 iDataType = 0;
	X8 *pDataMem = NULL;
	SpxGDatEntryShelfMement *pDebugEntryData = tblDebugGdatEntryShelfMement;
	SpxGDatShelf *pDebugDataShelf = tblDebugGdatShelf;

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GREEN, BLACK);
	printf("DEBUG_SHOW_GDAT_ENTRIES_MEM\n");
	// 1st pass, go through the ENT1 item to link Cls1-2-4 to a RawDatID
	printf("GDAT: #Data     = %d\n", glbGDatNumberOfData);
	printf("GDAT: #Entries  = %d\n", glbGDatNumberOfRawEntries);
	printf("GDAT: EntrySize = %d\n", glbGDatEntrySize);
	printf("Debug Entry Table = %p\n", tblDebugGdatEntryShelfMement);
	
	iDebugTabColor[0] = LIGHT_GRAY;
	iDebugTabColor[fmtImage] = LIGHT_GREEN;
	iDebugTabColor[fmtSound] = AQUA;
	iDebugTabColor[fmtHMP] = BLUE;

	iDebugTabColor[fmtRect] = LIGHT_YELLOW;
	iDebugTabColor[fmtText] = LIGHT_RED;
	iDebugTabColor[fmt06] = LIGHT_PURPLE;
	iDebugTabColor[fmt07] = LIGHT_PURPLE;

	iDebugTabColor[fmt08] = LIGHT_PURPLE;
	iDebugTabColor[fmtPalIRGB] = LIGHT_YELLOW;
	iDebugTabColor[10] = CYAN;	// word
	iDebugTabColor[fmtWordVal] = CYAN;

	iDebugTabColor[fmtPicOff] = GREEN;
	iDebugTabColor[fmtPalIndex] = LIGHT_YELLOW;
	iDebugTabColor[fmt0E] = WHITE;
	iDebugTabColor[15] = LIGHT_GRAY;

	// go through the shelf mem table
	for (iRawDatIndex = 0; iRawDatIndex < glbGDatNumberOfData; iRawDatIndex++) {
		tblDebugGdatShelf[iRawDatIndex].iGDatRawId = iRawDatIndex;
		tblDebugGdatShelf[iRawDatIndex].xShelfMem = glbShelfMemoryTable[iRawDatIndex];
		tblDebugGdatShelf[iRawDatIndex].pDataMem = NULL;
	}
	// go through the mement cache table
	for (iRawDatIndex = 0; iRawDatIndex < glbGDatNumberOfData; iRawDatIndex++) {
		mement* pMemEnt = NULL;
		U16 iMemEntIdx = tblRawDataToMement[iRawDatIndex];
		//printf("MEMENTIDX %05d = %04X\n", iRawDatIndex, iMemEntIdx);
		tblDebugGdatShelf[iRawDatIndex].iMemEntIdx = iMemEntIdx;	// get the MemEntry Index for that RawData index
		if (iMemEntIdx != 0xFFFF) {
			pMemEnt = tblMementsPointers[iMemEntIdx];	// out of glbNumberOfMements, get the real mement pointer
		}
		tblDebugGdatShelf[iRawDatIndex].pMemEntry = pMemEnt;
	}

	for (iEntryIndex = 0; iEntryIndex < glbGDatNumberOfRawEntries; iEntryIndex++, pDebugEntryData++) {
		bool bNoLf = false;
		iRawDatIndex = pDebugEntryData->sEntryData.data;
		iDataType = pDebugEntryData->sEntryData.cls3;
		//iCls1Category = QUERY_GDAT_ENTRY_VALUE(iEntryIndex, EPcls1);
		//iRawDatIdx = QUERY_GDAT_ENTRY_VALUE(iEntryIndex, EPdata);
		CHANGE_CONSOLE_COLOR(BRIGHT, iDebugTabColor[iDataType], BLACK);
		printf("#Entry %5d / %5d = [%02X-%02X-%02X] [%2d](%8s) [%4s]=%5d (%04X) ", iEntryIndex, glbGDatNumberOfRawEntries, 
			pDebugEntryData->sEntryData.cls1, pDebugEntryData->sEntryData.cls2, pDebugEntryData->sEntryData.cls4,
			iDataType, tsType[iDataType], sDatType[iDebugTabData[iDataType]], pDebugEntryData->sEntryData.data, pDebugEntryData->sEntryData.data);
		if (iDebugTabData[iDataType]) {
			bool bPresent = false;
			CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
			printf("Shelf=%08X", tblDebugGdatShelf[iRawDatIndex].xShelfMem.val);
			bPresent = tblDebugGdatShelf[iRawDatIndex].xShelfMem.Present();
			CHANGE_CONSOLE_COLOR(BRIGHT, iDebugTabPresentShelf[(int)bPresent], BLACK);
			printf(" <%7s> ", sPresent[(int)bPresent]);
			if (iDataType == fmtImage && bPresent)
					pDataMem = tblDebugGdatShelf[iRawDatIndex].pDataMem = (X8*) REALIZE_GRAPHICS_DATA_MEMORY(tblDebugGdatShelf[iRawDatIndex].xShelfMem);
			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GREEN, BLACK);
			if (pDataMem == 0)
				CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
			if (bPresent)
				printf("MEM:%p ", pDataMem);
			// Get extra info if image and pmem are ok
			if (iDataType == fmtImage && bPresent && pDataMem != 0) {
				U32 iGDatItemSize = QUERY_GDAT_RAW_DATA_LENGTH(iRawDatIndex);
				U32 iMemItemSize = READ_UI16(pDataMem, -2);	// should be the same, since it is what QUERY_GDAT_RAW_DATA_LENGTH does.
				CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_YELLOW, BLACK);
				printf("DSIZE = L:%05d M:%05d ", iGDatItemSize, iMemItemSize);	/// "L"ogical & direct read from "M"emory

				// mement
				CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
				if (tblDebugGdatShelf[iRawDatIndex].iMemEntIdx == 0xFFFF)
					CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
				printf("ME# %04X (%p) ", tblDebugGdatShelf[iRawDatIndex].iMemEntIdx, tblDebugGdatShelf[iRawDatIndex].pMemEntry); 
				if (tblDebugGdatShelf[iRawDatIndex].iMemEntIdx != 0xFFFF) {
					CHANGE_CONSOLE_COLOR(BRIGHT, WHITE, BLACK);
					printf("--------\n>>");
					DEBUG_SHOW_MEMENT(tblDebugGdatShelf[iRawDatIndex].pMemEntry, true);
//					bNoLf = true;
				}
			}
		}
//		if (!bNoLf)
			printf("\n");
	}

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
}
