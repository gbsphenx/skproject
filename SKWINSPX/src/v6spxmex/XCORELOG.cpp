//--- SkWinCore-Log -------------------------------------
//SPX: This part of code contains:
// - Retrocompatibility with DM1
// - New code for expanding DM2

#include <StdAfx.h>	// Required for MVC6 compilation

#include <SkVer.h>

#include <SkParam.h>
#include <SkDebug.h>
#include <SkGlobal.h>
#include <KCORE.H>
#include <SkMIDI.h>
#include <SkLUA.h>

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
#if defined(__DJGPP__) || defined (__MINGW__)
#include <SkDOS.h>
#include <stdlib.h> // rand note: putting stdlib here and not right after stdafx prevents a bunch of conflicts with min/max macros
#endif // __DJGPP__

#if defined (__MINGW__)
#undef WIN32
#endif

void CHANGE_CONSOLE_COLOR(int attr, int fg, int bg)
{
	/* Command is the control command to the terminal */
#if (defined __WINDOWS__ || defined WIN32)
	HANDLE hconsole = GetStdHandle (STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute (hconsole, fg | bg << 4); 
#elif (__MINGW__)
	{
		char command[48];
		char sBold[8];
		bool bUseBold = false;
		memset(command, 0, 48);
		memset(sBold, 0, 8);

		if (fg >= 8)
		{
			bUseBold = true;
			fg = (fg%8);
			sprintf(sBold, "\x1B[1m");	// bold -- or light colors
		}
		if (bg >= 8)
			bg = (bg%8);

		//-- Bit of color magic :/
		if (bg == 1)
			bg = 4;
		else if (bg == 4)
			bg = 1;

		sprintf(command, "%s\x1B[3%dm\x1B[4%dm", sBold, fg, bg);
		if (attr == RESET)
			sprintf(command, "\x1B[0m");
		printf("%s", command);
	}
#elif (defined __LINUX__  || __CYGWIN__)
	{
		char command[13];
		//--- This is C magic!
		sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
		printf("%s", command);
	}
#endif
}

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


void SkWinCore::LOG_FULL_DUNGEON_INFO()
{
	int i = 0;
	int j = 0;
	int iMapIndex = 0;

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
	printf("glbPlayerPosX  : %02d\n", glbPlayerPosX);
	printf("glbPlayerPosY  : %02d\n", glbPlayerPosY);
	printf("glbPlayerDir   : %02d\n", glbPlayerDir);
	printf("glbPlayerMap   : %02d\n", glbPlayerMap);

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

	//--------------------------------------------------------------------------
	// MEMENT
	int iMemEntIndex = 0;
	int iSizeofMementTable = glbNumberOfMements * 18;
	mement** xMemEntPointer = NULL;
	mement* xMemEntData = NULL;

	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	printf("\nMEM ENT POINTERS DATA\n");

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("glbNumberOfMements      : %04d\n", glbNumberOfMements);
	printf("tlbMementsPointers      : %08X\n", tlbMementsPointers);

	xMemEntPointer = (mement**) tlbMementsPointers;
	xObjectPointer = (U8*) tlbMementsPointers;

	//--- Addresses / Pointers
	for (iMemEntIndex = 0; iMemEntIndex < glbNumberOfMements; iMemEntIndex++)
	{
		int c = 0;

		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_PURPLE, BLACK);
		printf("%04d ", iMemEntIndex);
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GREEN, BLACK);
		printf("[%08X]) ", xMemEntPointer);
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
		if (xObjectPointer[0] == 0 && xObjectPointer[1] == 0 && xObjectPointer[2] == 0 && xObjectPointer[3] == 0)
			CHANGE_CONSOLE_COLOR(BRIGHT, GRAY, BLACK);
		for (c = 0; c < 4; c++)
		{
			printf("%02X", xObjectPointer[c]);
			if (c%2 == 1) printf(" ");
		}

		printf("   ");

		if (iMemEntIndex%6 == 5)
			printf("\n");

		xMemEntPointer++;
		xObjectPointer += 4;
	}
	printf("\n\n");

	//--- Addresses / Pointers
	xMemEntPointer = (mement**) tlbMementsPointers;
	for (iMemEntIndex = 0; iMemEntIndex < glbNumberOfMements; iMemEntIndex++)
	{
		int c = 0;

		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_PURPLE, BLACK);
		printf("%04d ", iMemEntIndex);
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GREEN, BLACK);
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

		if (iMemEntIndex%3 == 2)
			printf("\n");

		xMemEntPointer++;
	}
	printf("\n");


	//--------------------------------------------------------------------------
	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	printf("\nRECTANGLES TABLE\n");

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	printf("glbRectNoTable    : %08X\n", glbRectNoTable);

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
	U16 xGroundItem = 0;
	U32 index = (GET_OBJECT_INDEX_FROM_TILE(iMapX, iMapY)).w;
	ObjectID xFirstObject = GET_TILE_RECORD_LINK(iMapX, iMapY);
	ObjectID xCurrentObject = xFirstObject;
	U16 iObjectDirection = 0;
	U16 iObjectDBType = 0;

	if (index != -1)
		xGroundItem = dunGroundStacks[index];
#ifndef __DJGPP__
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
#endif // __DJGPP__

}
