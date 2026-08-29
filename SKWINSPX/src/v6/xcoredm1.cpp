//--- SkWinCore-DM1 -------------------------------------
//SPX: This part of code contains:
// - Retrocompatibility with DM1

#if defined (SKDLL_EXPORTS) || defined (LIBSKWINDLL_EXPORTS)
#include <StdAfx.h>
#endif // LIBSKWINDLL_EXPORTS

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

//#if defined(_USE_MFC80) || defined(_USE_MFC60)
//#include <SKMFC.h>
//#include <resource.h>
//#endif // defined(_USE_MFC80) || defined(_USE_MFC60)


//#ifdef _USE_SDL
//#include <SKSDL.h>
//#endif // _USE_SDL
//#ifdef __DJGPP__
//#include <SKDOS.h>
//#include <stdlib.h> // rand note: putting stdlib here and not right after stdafx prevents a bunch of conflicts with min/max macros
//#endif // __DJGPP__


//#include <kaitable.h>	// for dAITableGenuine (hard coded AI table)

//==============================================================================



X16
SkWinCore::EXTENDED_LOAD_DM1_ITEM_CONVERSION_LIST(void)
{
	U16 iIndex = 0;
	U16 iBaseIndexGroup = 0;


	// COMPASS
	iBaseIndexGroup = 0;
	for (iIndex = 0; iIndex < 4; iIndex++)
	{
		glbDM1ItemConv[iBaseIndexGroup+iIndex].iItemDB = DB_CATEGORY_MISC_ITEM;
		glbDM1ItemConv[iBaseIndexGroup+iIndex].iItemID = 5;	// 5 = COMPASS
	}

	// TORCH
	iBaseIndexGroup = 4;
	for (iIndex = 0; iIndex < 4; iIndex++)
	{
		glbDM1ItemConv[iBaseIndexGroup+iIndex].iItemDB = DB_CATEGORY_WEAPON;
		glbDM1ItemConv[iBaseIndexGroup+iIndex].iItemID = 2;	// 2 = TORCH
	}

	// 2-STATE MISC
	iBaseIndexGroup = 8;
	for (iIndex = 0; iIndex < 6; iIndex++)
	{
		glbDM1ItemConv[iBaseIndexGroup+iIndex].iItemDB = DB_CATEGORY_MISC_ITEM;
	}
	glbDM1ItemConv[iBaseIndexGroup+0].iItemID = 1; // WATERSKIN
	glbDM1ItemConv[iBaseIndexGroup+1].iItemID = 1; // WATERSKIN
	glbDM1ItemConv[iBaseIndexGroup+2].iItemID = 2; // JEWEL SYMAL
	glbDM1ItemConv[iBaseIndexGroup+3].iItemID = 2; // JEWEL SYMAL
	glbDM1ItemConv[iBaseIndexGroup+4].iItemID = 3; // ILLUMULET
	glbDM1ItemConv[iBaseIndexGroup+5].iItemID = 3; // ILLUMULET

	// 2-STATE WEAPON
	iBaseIndexGroup = 14;
	for (iIndex = 0; iIndex < 16; iIndex++)
	{
		glbDM1ItemConv[iBaseIndexGroup+iIndex].iItemDB = DB_CATEGORY_WEAPON;
	}
	glbDM1ItemConv[iBaseIndexGroup+0].iItemID = 3; // FLAMITT
	glbDM1ItemConv[iBaseIndexGroup+1].iItemID = 3; // FLAMITT
	glbDM1ItemConv[iBaseIndexGroup+2].iItemID = 0; // EYE OF TIME
	glbDM1ItemConv[iBaseIndexGroup+3].iItemID = 0; // EYE OF TIME
	glbDM1ItemConv[iBaseIndexGroup+4].iItemID = 1; // STORMRING
	glbDM1ItemConv[iBaseIndexGroup+5].iItemID = 1; // STORMRING
	glbDM1ItemConv[iBaseIndexGroup+6].iItemID = 4; // STAFF OF CLAWS
	glbDM1ItemConv[iBaseIndexGroup+7].iItemID = 4; // STAFF OF CLAWS
	glbDM1ItemConv[iBaseIndexGroup+8].iItemID = 4; // STAFF OF CLAWS
	glbDM1ItemConv[iBaseIndexGroup+9].iItemID = 5; // BOLT BLADE
	glbDM1ItemConv[iBaseIndexGroup+10].iItemID = 5; // BOLT BLADE
	glbDM1ItemConv[iBaseIndexGroup+11].iItemID = 6; // FURY
	glbDM1ItemConv[iBaseIndexGroup+12].iItemID = 6; // FURY
	glbDM1ItemConv[iBaseIndexGroup+13].iItemID = 45; // THE FIRESTAFF
	glbDM1ItemConv[iBaseIndexGroup+14].iItemID = 7; // THE FIRESTAFF
	glbDM1ItemConv[iBaseIndexGroup+15].iItemID = 7; // THE FIRESTAFF

	// SCROLLS
	iBaseIndexGroup = 30;
	glbDM1ItemConv[iBaseIndexGroup+0].iItemDB = DB_CATEGORY_SCROLL;
	glbDM1ItemConv[iBaseIndexGroup+0].iItemID = 0; // SCROLL
	glbDM1ItemConv[iBaseIndexGroup+1].iItemDB = DB_CATEGORY_SCROLL;
	glbDM1ItemConv[iBaseIndexGroup+1].iItemID = 0; // SCROLL


	// WEAPONS
	iBaseIndexGroup = 32;
	for (iIndex = 0; iIndex < 35; iIndex++)
	{
		glbDM1ItemConv[iBaseIndexGroup+iIndex].iItemDB = DB_CATEGORY_WEAPON;
		glbDM1ItemConv[iBaseIndexGroup+iIndex].iItemID = 8 + iIndex;	// 8 = DAGGER
	}

	// CLOTHINGS
	iBaseIndexGroup = 67;
	for (iIndex = 0; iIndex < 53; iIndex++)
	{
		glbDM1ItemConv[iBaseIndexGroup+iIndex].iItemDB = DB_CATEGORY_CLOTHING;
	}
	glbDM1ItemConv[iBaseIndexGroup+0].iItemID = 5;
	glbDM1ItemConv[iBaseIndexGroup+1].iItemID = 7;
	glbDM1ItemConv[iBaseIndexGroup+2].iItemID = 9;
	glbDM1ItemConv[iBaseIndexGroup+3].iItemID = 10;
	glbDM1ItemConv[iBaseIndexGroup+4].iItemID = 13;
	glbDM1ItemConv[iBaseIndexGroup+5].iItemID = 16;
	glbDM1ItemConv[iBaseIndexGroup+6].iItemID = 20;
	glbDM1ItemConv[iBaseIndexGroup+7].iItemID = 21;
	glbDM1ItemConv[iBaseIndexGroup+8].iItemID = 32;
	glbDM1ItemConv[iBaseIndexGroup+9].iItemID = 34;
	glbDM1ItemConv[iBaseIndexGroup+10].iItemID = 39;
	glbDM1ItemConv[iBaseIndexGroup+11].iItemID = 44;
	glbDM1ItemConv[iBaseIndexGroup+12].iItemID = 49;
	glbDM1ItemConv[iBaseIndexGroup+13].iItemID = 0;
	glbDM1ItemConv[iBaseIndexGroup+14].iItemID = 1;
	glbDM1ItemConv[iBaseIndexGroup+15].iItemID = 2;
	glbDM1ItemConv[iBaseIndexGroup+16].iItemID = 6;
	glbDM1ItemConv[iBaseIndexGroup+17].iItemID = 8;
	glbDM1ItemConv[iBaseIndexGroup+18].iItemID = 11;
	glbDM1ItemConv[iBaseIndexGroup+19].iItemID = 12;
	glbDM1ItemConv[iBaseIndexGroup+20].iItemID = 14;
	glbDM1ItemConv[iBaseIndexGroup+21].iItemID = 17;
	glbDM1ItemConv[iBaseIndexGroup+22].iItemID = 19;
	glbDM1ItemConv[iBaseIndexGroup+23].iItemID = 22;
	glbDM1ItemConv[iBaseIndexGroup+24].iItemID = 33;
	glbDM1ItemConv[iBaseIndexGroup+25].iItemID = 35;
	glbDM1ItemConv[iBaseIndexGroup+26].iItemID = 40;
	glbDM1ItemConv[iBaseIndexGroup+27].iItemID = 45;
	glbDM1ItemConv[iBaseIndexGroup+28].iItemID = 50;
	glbDM1ItemConv[iBaseIndexGroup+29].iItemID = 25;
	glbDM1ItemConv[iBaseIndexGroup+30].iItemID = 26;
	glbDM1ItemConv[iBaseIndexGroup+31].iItemID = 27;
	glbDM1ItemConv[iBaseIndexGroup+32].iItemID = 36;
	glbDM1ItemConv[iBaseIndexGroup+33].iItemID = 38;
	glbDM1ItemConv[iBaseIndexGroup+34].iItemID = 43;
	glbDM1ItemConv[iBaseIndexGroup+35].iItemID = 48;
	glbDM1ItemConv[iBaseIndexGroup+36].iItemID = 23;
	glbDM1ItemConv[iBaseIndexGroup+37].iItemID = 24;
	glbDM1ItemConv[iBaseIndexGroup+38].iItemID = 28;
	glbDM1ItemConv[iBaseIndexGroup+39].iItemID = 29;
	glbDM1ItemConv[iBaseIndexGroup+40].iItemID = 31;
	glbDM1ItemConv[iBaseIndexGroup+41].iItemID = 30;
	glbDM1ItemConv[iBaseIndexGroup+42].iItemID = 42;
	glbDM1ItemConv[iBaseIndexGroup+43].iItemID = 47;
	glbDM1ItemConv[iBaseIndexGroup+44].iItemID = 52;
	glbDM1ItemConv[iBaseIndexGroup+45].iItemID = 3;
	glbDM1ItemConv[iBaseIndexGroup+46].iItemID = 18;
	glbDM1ItemConv[iBaseIndexGroup+47].iItemID = 4;
	glbDM1ItemConv[iBaseIndexGroup+48].iItemID = 37;
	glbDM1ItemConv[iBaseIndexGroup+49].iItemID = 41;
	glbDM1ItemConv[iBaseIndexGroup+50].iItemID = 46;
	glbDM1ItemConv[iBaseIndexGroup+51].iItemID = 51;
	glbDM1ItemConv[iBaseIndexGroup+52].iItemID = 15;

	// MISCS
	iBaseIndexGroup = 120;
	for (iIndex = 0; iIndex < 20; iIndex++)
	{
		glbDM1ItemConv[iBaseIndexGroup+iIndex].iItemDB = DB_CATEGORY_MISC_ITEM;
	}
	glbDM1ItemConv[iBaseIndexGroup+0].iItemID = 37;
	glbDM1ItemConv[iBaseIndexGroup+1].iItemID = 38;
	glbDM1ItemConv[iBaseIndexGroup+2].iItemID = 39;
	glbDM1ItemConv[iBaseIndexGroup+3].iItemID = 40;
	glbDM1ItemConv[iBaseIndexGroup+4].iItemID = 41;
	glbDM1ItemConv[iBaseIndexGroup+5].iItemID = 6;
	glbDM1ItemConv[iBaseIndexGroup+6].iItemID = 7;
	glbDM1ItemConv[iBaseIndexGroup+7].iItemID = 8;
	glbDM1ItemConv[iBaseIndexGroup+8].iItemID = 25;
	glbDM1ItemConv[iBaseIndexGroup+9].iItemID = 26;
	glbDM1ItemConv[iBaseIndexGroup+10].iItemID = 27;
	glbDM1ItemConv[iBaseIndexGroup+11].iItemID = 28;
	glbDM1ItemConv[iBaseIndexGroup+12].iItemID = 42;
	glbDM1ItemConv[iBaseIndexGroup+13].iItemID = 43;
	glbDM1ItemConv[iBaseIndexGroup+14].iItemID = 44;
	glbDM1ItemConv[iBaseIndexGroup+15].iItemDB = DB_CATEGORY_WEAPON;
	glbDM1ItemConv[iBaseIndexGroup+15].iItemID = 43;	// HORN OF FEAR
	glbDM1ItemConv[iBaseIndexGroup+16].iItemID = 45;
	glbDM1ItemConv[iBaseIndexGroup+17].iItemID = 46;
	glbDM1ItemConv[iBaseIndexGroup+18].iItemID = 47;
	glbDM1ItemConv[iBaseIndexGroup+19].iItemID = 48;


	// SPECIAL ITEMS
	glbDM1ItemConv[140].iItemDB = DB_CATEGORY_CLOTHING;
	glbDM1ItemConv[140].iItemID = 53;	// DEXHELM
	glbDM1ItemConv[141].iItemDB = DB_CATEGORY_CLOTHING;
	glbDM1ItemConv[141].iItemID = 54;	// FLAMEBAIN
	glbDM1ItemConv[142].iItemDB = DB_CATEGORY_CLOTHING;
	glbDM1ItemConv[142].iItemID = 55;	// POWERTOWERS
	glbDM1ItemConv[143].iItemDB = DB_CATEGORY_WEAPON;
	glbDM1ItemConv[143].iItemID = 44;	// SPEEDBOW
	glbDM1ItemConv[144].iItemDB = DB_CATEGORY_CONTAINER;
	glbDM1ItemConv[144].iItemID = 0;	// CHEST
	glbDM1ItemConv[145].iItemDB = DB_CATEGORY_CONTAINER;
	glbDM1ItemConv[145].iItemID = 0;	// CHEST
	glbDM1ItemConv[146].iItemDB = DB_CATEGORY_MISC_ITEM;
	glbDM1ItemConv[146].iItemID = 4;	// ASHES
	glbDM1ItemConv[147].iItemDB = DB_CATEGORY_MISC_ITEM;
	glbDM1ItemConv[147].iItemID = 52;	// BONES

	// POTIONS
	iBaseIndexGroup = 148;
	for (iIndex = 0; iIndex < 20; iIndex++)
	{
		glbDM1ItemConv[iBaseIndexGroup+iIndex].iItemDB = DB_CATEGORY_POTION;
		glbDM1ItemConv[iBaseIndexGroup+iIndex].iItemID = 0 + iIndex;	// 0 = MON POTION
	}

	// FOOD
	iBaseIndexGroup = 168;
	for (iIndex = 0; iIndex < 8; iIndex++)
	{
		glbDM1ItemConv[iBaseIndexGroup+iIndex].iItemDB = DB_CATEGORY_MISC_ITEM;
		glbDM1ItemConv[iBaseIndexGroup+iIndex].iItemID = 29 + iIndex;	// 29 = APPLE
	}

	// KEYS
	iBaseIndexGroup = 176;
	for (iIndex = 0; iIndex < 16; iIndex++)
	{
		glbDM1ItemConv[iBaseIndexGroup+iIndex].iItemDB = DB_CATEGORY_MISC_ITEM;
		glbDM1ItemConv[iBaseIndexGroup+iIndex].iItemID = 9 + iIndex;	// 9 = IRON KEY
	}
	
	// LAST ITEMS - SPECIAL
	glbDM1ItemConv[194].iItemDB = DB_CATEGORY_CLOTHING;
	glbDM1ItemConv[194].iItemID = 56;	// BOOTS OF SPEED
	glbDM1ItemConv[195].iItemDB = DB_CATEGORY_POTION;
	glbDM1ItemConv[195].iItemID = 20;	// EMPTY FLASK
	glbDM1ItemConv[196].iItemDB = DB_CATEGORY_CLOTHING;
	glbDM1ItemConv[196].iItemID = 57;	// BOOTS OF SPEED
	glbDM1ItemConv[197].iItemDB = DB_CATEGORY_MISC_ITEM;
	glbDM1ItemConv[197].iItemID = 51;	// ZOKATHRA SPELL
	glbDM1ItemConv[198].iItemDB = DB_CATEGORY_MISC_ITEM;
	glbDM1ItemConv[198].iItemID = 52;	// BONES

	return 0;
}





// SPX: merge RotateActuatorList with caller parameters from PerformLocalAction (CSBWin reference)
// This is actually a free implementation. It should be tested in more cases.
X16
SkWinCore::DM1_ROTATE_ACTUATOR_LIST(X16 localActionType, i16 iMapX, i16 iMapY, i16 iMapLevel, U16 iSide)
{
	printf("DM1 Rotate Actuators on %d,%d dir=%d !\n", iMapX, iMapY, iSide);
	// TODO to be implemented
	ObjectID xFirstObject = GET_TILE_RECORD_LINK(iMapX, iMapY);
	ObjectID xCurrentObject = xFirstObject;
	ObjectID xThreeLastActuators[3];
	ObjectID xLastActNext = OBJECT_END_MARKER;
	U16 index = 0;
	U16 iActuatorDirection = 0; // to be checked with side to rotate
	U16 iObjectDBType = 0;

	DEBUG_HELP_DISPLAY_STACK(iMapX, iMapY, -1);

	xThreeLastActuators[0] = xThreeLastActuators[1] = xThreeLastActuators[2] = OBJECT_END_MARKER;

	for (index = 0; xCurrentObject != OBJECT_END_MARKER && index == 0; xCurrentObject = GET_NEXT_RECORD_LINK(xCurrentObject))
	{
		iActuatorDirection = xCurrentObject.Dir();
		iObjectDBType = xCurrentObject.DBType();
		if (iActuatorDirection != iSide || iObjectDBType != dbActuator)
			continue;
		// object is an actuator on the right side, we stack it
		xThreeLastActuators[0] = xThreeLastActuators[1];
		xThreeLastActuators[1] = xThreeLastActuators[2];
		xThreeLastActuators[2] = xCurrentObject;
		xLastActNext = GET_NEXT_RECORD_LINK(xCurrentObject);
	}

	// Perform the rotate
	// case 1: there is actually only one actuator, then do nothing
	if (xThreeLastActuators[0] == OBJECT_END_MARKER && xThreeLastActuators[1] == OBJECT_END_MARKER)
		return 0;

	// case 2: there are two actuators, rotate them
	if (xThreeLastActuators[0] == OBJECT_END_MARKER &&
			xThreeLastActuators[1] != OBJECT_END_MARKER	&& xThreeLastActuators[2] != OBJECT_END_MARKER)
	{
		// If ground actuator is [1], then it will be also switched.
		U16 xGroundItem = 0;
		U32 index = (GET_OBJECT_INDEX_FROM_TILE(iMapX, iMapY)).w;
		if (index != -1)
			xGroundItem = dunGroundStacks[index];

		// Act[2] goes under, and gets Act[1] as next
//		APPEND_RECORD_TO(xThreeLastActuators[1], &xThreeLastActuators[2], -1, 0);
		// Act[1] gets on top, then gets next as empty
//		APPEND_RECORD_TO(xLastActNext, &xThreeLastActuators[1], -1, 0);

		SET_NEXT_RECORD_LINK(xThreeLastActuators[2], xThreeLastActuators[1]);
		SET_NEXT_RECORD_LINK(xThreeLastActuators[1], xLastActNext);

		// replace ground reference by the actuator rotated under
		if (xGroundItem == xThreeLastActuators[1].w)
			dunGroundStacks[index] = xThreeLastActuators[2].w;

	}

	DEBUG_HELP_DISPLAY_STACK(iMapX, iMapY, -1);

	return 1;
}


// DM1:
// Gold coin = 127, DM2 = 264
// The DM1 activation list is different and static
U16 SkWinCore::GET_DM1_DISTINCTIVE_ITEMTYPE(ObjectID recordLink)
{
	X16 iSearchIndex = 0;
	// get the object type of record in the form of uniquely identified number.
	// e.g. it always returns 2 if you get any type of weapon Torch record.
	// returns 511 if record is no meaningful to distinct.

	if (recordLink != OBJECT_NULL)
	{
		Bit8u iItemID = QUERY_CLS2_FROM_RECORD(recordLink);	// item type index
		U16 iItemDB = recordLink.DBType();
		//if (di == DB_CATEGORY_MISC_ITEM)
		//	return 119 + bp01;
		// Search through the table of conversion item to get the DM1 item value
		for (iSearchIndex = 0; iSearchIndex < 200; iSearchIndex++)
		{
			if (iItemDB == glbDM1ItemConv[iSearchIndex].iItemDB &&
				iItemID == glbDM1ItemConv[iSearchIndex].iItemID)
				return iSearchIndex;
			// SPX: TODO : special state for multi-state items (compass, torches, charged/discharged items)
		}
	}
	return 511;
}

// SPX: Decryption algorithm from Christophe Fontanel :
// http://dmweb.free.fr/community/documentation/file-formats/saved-game-files/
U16 SkWinCore::DM1_DECYPHER_SAVEGAME_BLOCK(U16* pDataBlock, U16 iCypherKey, U16 iBlockSize)
{
	U16 iTempValue = iCypherKey;
	U16 iComputedChecksum = iCypherKey;

	for (int i = 0; i < iBlockSize; i++)
	{
		iComputedChecksum = (iComputedChecksum + pDataBlock[i]) & 0xFFFF;
		pDataBlock[i] = pDataBlock[i] ^ iTempValue;
		iComputedChecksum = (iComputedChecksum + pDataBlock[i]) & 0xFFFF;
		iTempValue = (iTempValue + iBlockSize - i) & 0xFFFF;
	}
	return iComputedChecksum;
}


i16 SkWinCore::DM1_GAME_LOAD()
{
	do {
		U16 iTimerIndex;
		U8 iBlockIndex;

		U8 iChampionIndex = 0;
		U8 glbDummyData[10000];
		// there are 0x24C0 bytes of encoded data
		SkD((DLV_DBG_GAME_LOAD, "Read as a DM1 savegame ...\n"));

		// Init savegame blocks
		for (iBlockIndex = 0; iBlockIndex < 10; iBlockIndex++) {
			cd.dm1.pDM1SavegameBlock[iBlockIndex] = NULL;
		}

		// Init champions
		for (iChampionIndex = 0; iChampionIndex < 4; iChampionIndex++) {
			Champion *pChampion = &tblChampionSquad[iChampionIndex];
			INIT_CHAMPION(pChampion);
		}

		// 42 bytes already been read
		if (FILE_READ(glbDataFileHandle, 0x24C0-42, glbDummyData) == 0)
			break;

		// 0x24C0 (9408 bytes) of data, what to put into it ? 
		// An Amiga save looks to have 0x2924 = 10532 bytes
		// CSBWin tells there are 1 block of 512, then a 2nd block of data,
		// then block of items, heros, timers, timers queued
		// some global variables and that may be all ? ...
		// CSB hero would be around 284 without portraits and fillers
		// 284 * 4 = 1136
		// +50 fill * 4 = +200
		// timers should be around 10 or 12, maybe having 50 at first ? so 50 * 10 = 500, not much

		// Go back to start and read 1st block
		U16 *pDataBlock = NULL;
		FILE_SEEK(glbDataFileHandle, 0);
		cd.dm1.pDM1SavegameBlock[0] = (X8*) calloc(128, sizeof(X16));
		pDataBlock = (U16*)cd.dm1.pDM1SavegameBlock[0];
		FILE_READ(glbDataFileHandle, 128*sizeof(X16), pDataBlock);
		cd.dm1.iDM1SVGDecryptKey2 = pDataBlock[10];
		cd.dm1.iDM1SVGBlock2Checksum = pDataBlock[10+1+116];
		free(pDataBlock);

		// Block 2
		U16 iBlockChecksum = -1;
		cd.dm1.pDM1SavegameBlock[1] = (X8*) calloc(128, sizeof(X16));
		pDataBlock = (U16*)cd.dm1.pDM1SavegameBlock[1];
		FILE_READ(glbDataFileHandle, 128*sizeof(X16), pDataBlock);
		iBlockChecksum = DM1_DECYPHER_SAVEGAME_BLOCK(pDataBlock, cd.dm1.iDM1SVGDecryptKey2, 128);
		cd.dm1.iDM1SVGDecryptKey3 = pDataBlock[27];
		cd.dm1.iDM1SVGDecryptKey4ExtCreatures = pDataBlock[28];
		cd.dm1.iDM1SVGDecryptKey5Champions = pDataBlock[29];
		cd.dm1.iDM1SVGDecryptKey6Timers = pDataBlock[30];
		cd.dm1.iDM1SVGDecryptKey7TimerQueue = pDataBlock[31];
		free(pDataBlock);

		// Block 3 - Global game data
		tDM1SaveGameBlock3* pSGBlock3 = NULL;
		cd.dm1.pDM1SavegameBlock[2] = (X8*) calloc(64, sizeof(X16));
		pDataBlock = (U16*)cd.dm1.pDM1SavegameBlock[2];
		FILE_READ(glbDataFileHandle, 64*sizeof(X16), pDataBlock);
		iBlockChecksum = DM1_DECYPHER_SAVEGAME_BLOCK(pDataBlock, cd.dm1.iDM1SVGDecryptKey3, 64);
		pSGBlock3 = (tDM1SaveGameBlock3*) pDataBlock;
		cd.dm1.iDM1SVGNbExtCreatures = pSGBlock3->MaxITEM16;
		cd.pi.glbChampionsCount = pSGBlock3->numcharacter;

		// Block 4 - Extended Creature Data
		cd.dm1.pDM1SavegameBlock[3] = (X8*) calloc(cd.dm1.iDM1SVGNbExtCreatures*16, sizeof(X8));
		pDataBlock = (U16*)cd.dm1.pDM1SavegameBlock[3];
		FILE_READ(glbDataFileHandle, cd.dm1.iDM1SVGNbExtCreatures*16*sizeof(X8), pDataBlock);
		iBlockChecksum = DM1_DECYPHER_SAVEGAME_BLOCK(pDataBlock, cd.dm1.iDM1SVGDecryptKey4ExtCreatures, cd.dm1.iDM1SVGNbExtCreatures*8);
		free(pDataBlock);		

		// Block 5 - Champion Data
		cd.dm1.pDM1SavegameBlock[4] = (X8*) calloc(1404, sizeof(X8));
		pDataBlock = (U16*)cd.dm1.pDM1SavegameBlock[4];
		FILE_READ(glbDataFileHandle, 1404, pDataBlock);
		iBlockChecksum = DM1_DECYPHER_SAVEGAME_BLOCK(pDataBlock, cd.dm1.iDM1SVGDecryptKey5Champions, 702);
		tCHARDESC *pBlock4Champion = (tCHARDESC*) pDataBlock;
		Champion *pChampion = &glbChampionTable[1];
		for (iChampionIndex = 0; iChampionIndex < cd.pi.glbChampionsCount; iChampionIndex++) {
			U8 iSkillIndex = 0;
			U16 iInventorySlot = 0;
			strcpy((char*)pChampion->firstName, (char*)pBlock4Champion->name);
			strcpy((char*)pChampion->lastName, (char*)pBlock4Champion->title);
			pChampion->maxHP(pBlock4Champion->maxHP);
			pChampion->curHP(pBlock4Champion->HP);
			pChampion->maxStamina(pBlock4Champion->maxStamina);
			pChampion->curStamina(pBlock4Champion->stamina);
			pChampion->maxMP(pBlock4Champion->maxMana);
			pChampion->curMP(pBlock4Champion->mana);
			pChampion->curFood(pBlock4Champion->food);
			pChampion->curWater(pBlock4Champion->water);
			

//			pChampion->maxLuck(pBlock4Champion->Attributes[0].ubMinimum);
			pChampion->maxLuck(pBlock4Champion->Attributes[0].ubMaximum);
			pChampion->curLuck(pBlock4Champion->Attributes[0].ubCurrent);

//			pChampion->maxStr(pBlock4Champion->Attributes[1].ubMinimum);
			pChampion->maxStr(pBlock4Champion->Attributes[1].ubMaximum);
			pChampion->curStr(pBlock4Champion->Attributes[1].ubCurrent);

//			pChampion->maxDex(pBlock4Champion->Attributes[2].ubMinimum);
			pChampion->maxDex(pBlock4Champion->Attributes[2].ubMaximum);
			pChampion->curDex(pBlock4Champion->Attributes[2].ubCurrent);

//			pChampion->maxWiz(pBlock4Champion->Attributes[3].ubMinimum);
			pChampion->maxWiz(pBlock4Champion->Attributes[3].ubMaximum);
			pChampion->curWiz(pBlock4Champion->Attributes[3].ubCurrent);

//			pChampion->maxVit(pBlock4Champion->Attributes[4].ubMinimum);
			pChampion->maxVit(pBlock4Champion->Attributes[4].ubMaximum);
			pChampion->curVit(pBlock4Champion->Attributes[4].ubCurrent);

//			pChampion->maxAntiM(pBlock4Champion->Attributes[5].ubMinimum);
			pChampion->maxAntiM(pBlock4Champion->Attributes[5].ubMaximum);
			pChampion->curAntiM(pBlock4Champion->Attributes[5].ubCurrent);

//			pChampion->maxAntiF(pBlock4Champion->Attributes[6].ubMinimum);
			pChampion->maxAntiF(pBlock4Champion->Attributes[6].ubMaximum);
			pChampion->curAntiF(pBlock4Champion->Attributes[6].ubCurrent);
			

			for (iSkillIndex = 0; iSkillIndex < 20; iSkillIndex++) {
				pChampion->skills[iSkillIndex] = pBlock4Champion->skills92[iSkillIndex].Long2;
			}
			pChampion->weight = pBlock4Champion->load;
			
			// inventory
			for (iInventorySlot = 0; iInventorySlot < 30; iInventorySlot++) {
				pChampion->Possess(iInventorySlot, pBlock4Champion->possessions[iInventorySlot]);
			}

			pBlock4Champion++;
			pChampion++;
		}
		pBlock4Champion = (tCHARDESC*) pDataBlock;
	

		FILE_SEEK(glbDataFileHandle, 0x24C0);
		// then comes portraits : 4 * 29*16 bytes
		SkD((DLV_DBG_GAME_LOAD, "Read Portraits ...\n"));
		// copy portrait data at +6 because we need some meta-info before
		if (FILE_READ(glbDataFileHandle, (29*16), glbDummyData) == 0)
			break;
		memcpy(cd.dm1.xDM1PortraitsData[0]+6, glbDummyData, 29*16);

		if (FILE_READ(glbDataFileHandle, (29*16), glbDummyData) == 0)
			break;
		memcpy(cd.dm1.xDM1PortraitsData[1]+6, glbDummyData, 29*16);
		if (FILE_READ(glbDataFileHandle, (29*16), glbDummyData) == 0)
			break;
		memcpy(cd.dm1.xDM1PortraitsData[2]+6, glbDummyData, 29*16);
		if (FILE_READ(glbDataFileHandle, (29*16), glbDummyData) == 0)
			break;
		memcpy(cd.dm1.xDM1PortraitsData[3]+6, glbDummyData, 29*16);
		//if (FILE_READ(glbDataFileHandle, 8, bp26) == 0)

		// adjust image data with meta-info
		for (iChampionIndex = 0; iChampionIndex < 4; iChampionIndex++)
		{
			U16 *pPortraitImage = (U16*) (cd.dm1.xDM1PortraitsData[iChampionIndex]+6);
			cd.dm1.bDM1PortraitsActivated[iChampionIndex] = false;
			//*((U16*)(&cd.dm1.xDM1PortraitsData[iChampionIndex])) = IMG_4_BPP; // 4bpp
			//*((U16*)(&cd.dm1.xDM1PortraitsData[iChampionIndex])+1) = 32; // x pitch
			//*((U16*)(&cd.dm1.xDM1PortraitsData[iChampionIndex])+2) = 29; // y pitch
			WRITE_IMGBUFF_WIDTH(pPortraitImage,32);
			WRITE_IMGBUFF_HEIGHT(pPortraitImage,29);
			WRITE_IMGBUFF_BPP(pPortraitImage,IMG_4_BPP);
		}
		// Note: if there are only X champions (with X < 4), then the X+1 portrait would be a zero array.
		cd.pi.glbChampionsCount = 4;	// player cnt
		cd.dm1.bDM1PortraitsActivated[0] = true;
		for (iChampionIndex = 1; iChampionIndex < 4; iChampionIndex++)
		{
			// test array
			if (IS_ZERO_ARRAY(cd.dm1.xDM1PortraitsData[iChampionIndex]+6, 29*16) != 0) {
				cd.pi.glbChampionsCount = iChampionIndex;
				break;
			}
			// little hack, portrait would have grey in corners (i.e. nibble 11), so I may use this to check if it looks like a portrait
			if (*(X8*)(cd.dm1.xDM1PortraitsData[iChampionIndex]+6) == 0x11) {
				cd.pi.glbChampionsCount = iChampionIndex+1;
				cd.dm1.bDM1PortraitsActivated[iChampionIndex] = true;
			}
			else
				cd.dm1.bDM1PortraitsActivated[iChampionIndex] = false;
		}

		glbDungeonDatFileCursor = (0x24C0 + (4*29*16)); // file cursor to start dungeon structure


		// the regular dungeon structure starts at 0x2C00
		SkD((DLV_DBG_GAME_LOAD, "Read Dungeon Structure DM1 Mode ...\n"));
		SkCodeParam::bDM1ReadSavegame = true;
		if (READ_DUNGEON_STRUCTURE(0) == 0)
		{
			break;
		}



		_4976_5bf2 = 1; // load is complete
		// Copy from DM2 read
		SkD((DLV_DBG_GAME_LOAD, "Suppress Init ...\n"));
		cd.mo.glbSpecialScreen = _MENU_SCREEN__RESUME_GAME_SELECT;
		SUPPRESS_INIT();
		SkD((DLV_DBG_GAME_LOAD, "Start Read File Handle %02d ...\n", glbDataFileHandle));
		SkD((DLV_DBG_GAME_LOAD, "Read Global Variables ...\n"));

		cd.pi.glbLeaderHandPossession.object = OBJECT_NULL;

		glbGameTick = _4976_4c1a = 1;	// game tick
		glbRandomSeed = 63;	// random seed
		//cd.pi.glbChampionsCount = 4;	// player cnt

		cd.pi.glbPlayerPosX = pSGBlock3->partyx;		// column
		cd.pi.glbPlayerPosY = pSGBlock3->partyy;		// row
		cd.pi.glbPlayerDir = pSGBlock3->partyfacing;	// dir
		cd.pi.glbPlayerMap = pSGBlock3->partyLevel;		// map#


		glbChampionLeader = 0;	// leader
		glbTimersCount = 0;	// wTimersCount
		_4976_0090 = 0;
		glbGameTick_4b80 = 0;
		_4976_4c00 = 0;
		glbPlayerThrowCounter = 0; //wPlayerThrowCounter
		_4976_4c0c = 0;
		glbRainFlagSomething = 0;
		glbRainAmbientLightModifier = 0;
		glbRainDirection = 0;	// rain dir.
		glbRainStrength = 0;	// rain str.
		glbRainLevelForSky = 0;
		glbRainLevelForGround = 0;
		glbRainMultiplicator = 0;
		glbRainStormController = 0;
		glbRainRelated3 = 0;
		glbRainRelated2 = 0;
		glbRainSpecialNextTick = 0;

		free((X8*)pSGBlock3);

	/*
		skload_table_60 t1;
		if (SUPPRESS_READER(&t1, _4976_395a, 56, 1, 1) != 0)
			break;
	*/
		iTimerIndex = glbTimersCount;

		SkD((DLV_DBG_GAME_LOAD, "Read Timers ...\n"));
		while (iTimerIndex < glbTimersMaximumCount) {
			glbTimersTable[iTimerIndex].TimerType(tty00);
			iTimerIndex++;
		}
		__SORT_TIMERS();
		if (READ_SKSAVE_DUNGEON() != 0)	// this resets champion inventory
			break;

		// sets inventory from savegame at this point
		pChampion = &glbChampionTable[1];
		pDataBlock = (X16*)pBlock4Champion;
		for (iChampionIndex = 0; iChampionIndex < cd.pi.glbChampionsCount; iChampionIndex++) {
			U16 iInventorySlot = 0;
			// inventory
			for (iInventorySlot = 0; iInventorySlot < 30; iInventorySlot++) {
				pChampion->Possess(iInventorySlot, pBlock4Champion->possessions[iInventorySlot]);
			}
			pBlock4Champion++;
			pChampion++;
		}
		free(pDataBlock);
		// inventory block

		PROCEED_GLOBAL_EFFECT_TIMERS();
		_3a15_020f();
		_4976_5bf2 = 1;
		FILE_CLOSE(glbDataFileHandle);
		
		PROCESS_ACTUATOR_TICK_GENERATOR();
		glbGDatFloppyFlag = 0;
		if (cd.mo.glbSpecialScreen != _MENU_SCREEN__RESUME_GAME_SELECT) {
			//if (bp04 != 0) {
				WAIT_SCREEN_REFRESH();
			//}
			DIALOG_BOX_2066_03e0(0);
		}
		else {
			_4976_5bf6 = 1;
//			if (bp08 != 0) {
				FILE_RENAME(FORMAT_SKSTR(ptrSKSave_bak, NULL), FORMAT_SKSTR(ptrSKSave_dat, NULL));
//			}
			DIALOG_BOX_2066_03e0(1);
		}
		_0aaf_02f8_DIALOG_BOX(DIALOGBOX_x0E_LOADING_GAME, 0x0000);
		_4976_4bd8 = 0x0001;
		cd.pi.glbPlayerDefeated = 0x0000;
		TELEPORTER_2fcf_0b8b(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, cd.pi.glbPlayerMap);
		glbDoLightCheck = 0x0001;
		//LOG_FULL_DUNGEON_INFO();
		return 1;
	} while (false);


	return 0;
}

//==============================================================================