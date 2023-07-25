//--- SkWinCore2 -------------------------------------
//SPX: This part of code will hold more easily added code for
// - Retrocompatibility with DM1
// - New code for expanding DM2

#include <StdAfx.h>	// Required for MVC6 compilation

#include <SkCodeParam.h>
#include <SkWinDebug.h>
#include <SkGlobal.h>
#include <SkWinCore.h>
#include <SkWinMIDI.h>

//--- Common part with A.cpp
#define UseAltic 1

using namespace DMEncyclopaedia;
using namespace DM2Internal;
using namespace kkBitBlt;

#if defined(_USE_MFC80) || defined(_USE_MFC60)
#include "SkWinMFC.h"
#include "resource.h"
#endif // defined(_USE_MFC80) || defined(_USE_MFC60)


#ifdef _USE_SDL
#include "SkwinSDL.h"
#endif // _USE_SDL


#include <_4976_03a2.h>	// for dAITableGenuine (hard coded AI table)
//--- Common part with A.cpp


//==============================================================================
//--- NEW FUNCTION -------------------------------------
//SPX: NEW FUNCTION, TO BE USED FOR MISSILE CONTAINED WITHIN POTION
U16 SkWinCore::QUERY_GDAT_POTION_SPELL_TYPE_FROM_RECORD(ObjectID rl)
{
	ENTER(0);
	return QUERY_GDAT_DBSPEC_WORD_VALUE(rl, GDAT_ITEM_STATS_MISSILE_SPELL);
}

//--- NEW FUNCTION -------------------------------------
//SPX: NEW FUNCTION, TO BE USED FOR POTION TYPE
U16 SkWinCore::QUERY_GDAT_POTION_BEHAVIOUR_FROM_RECORD(ObjectID rl)
{
	ENTER(0);
	return QUERY_GDAT_DBSPEC_WORD_VALUE(rl, GDAT_POTION_STAT_BEHAVIOUR);
}

//--- NEW FUNCTION -------------------------------------
//SPX: NEW FUNCTION, WATER VALUE, SHOULD WORK LIKE FOOD VALUE
U16 SkWinCore::QUERY_GDAT_WATER_VALUE_FROM_RECORD(ObjectID rl)
{
	ENTER(0);
	return QUERY_GDAT_DBSPEC_WORD_VALUE(rl, GDAT_ITEM_STATS_WATER_VALUE);
}

//--- NEW FUNCTION -------------------------------------
//SPX: NEW FUNCTION, MIRROR VALUE FOR DOOR
U16 SkWinCore::QUERY_GDAT_DOOR_IS_MIRRORED(U8 doortype)
{
	return QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, doortype, dtWordValue, GDAT_DOOR_MIRRORED);
}



X16
SkWinCore::EXTENDED_LOAD_DM1_ITEM_CONVERSION_LIST(void)
{
	Bit16u iIndex = 0;
	Bit16u iBaseIndexGroup = 0;


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
		i16 index = GET_OBJECT_INDEX_FROM_TILE(iMapX, iMapY);
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

void SkWinCore::SET_NEXT_RECORD_LINK(ObjectID rlSourceObject, ObjectID rlObjectToAppend)
{
	GenericRecord *grObject = (GenericRecord *)GET_ADDRESS_OF_RECORD(rlSourceObject);
	grObject->w0 = rlObjectToAppend;
}

// SPX: Help function to get any Weapon to Misc item, generally from a wall
ObjectID SkWinCore::GET_WALL_TILE_ANY_TAKEABLE_ITEM_RECORD(U16 iMapX, U16 iMapY, U16 iDirection)
{
	ENTER(0);
	ObjectID xTakenObject = OBJECT_NULL;
	ObjectID xCurrentObject = GET_TILE_RECORD_LINK(iMapX, iMapY);
	while (xCurrentObject.Dir() != iDirection || xCurrentObject.DBType() < dbWeapon || xCurrentObject.DBType() >= dbMissile)
	{
		xCurrentObject = GET_NEXT_RECORD_LINK(xCurrentObject);
	}
	if (xCurrentObject.Dir() == iDirection && xCurrentObject.DBType() >= dbWeapon && xCurrentObject.DBType() <= dbMiscellaneous_item)
		xTakenObject = xCurrentObject;
	return xTakenObject;
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
	i16 index = GET_OBJECT_INDEX_FROM_TILE(iMapX, iMapY);
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

}

// This disable all hands and magic for some time; that allow some new type of attacks from creature
void SkWinCore::STUN_CHAMPION(U16 player, U16 stunvalue)
{
	Champion *champion = &glbChampionSquad[player];
	U16 cooldown = 0;
	U8 hand = 0;

	for (hand = 0; hand < 2; hand++)
	{
		cooldown = champion->handCooldown[hand];
		cooldown += stunvalue;
		if (cooldown >= 256)
			cooldown = 255;
		champion->handCooldown[hand] = cooldown;
	}

}

////////////////////////////////////////////////////////////////////////////////

// SPX: New function CURE_PLAGUE, similar to CURE_POISON
void SkWinCore::CURE_PLAGUE(U16 player)
{
	i16 di = player;
	if (di != -1) {
		U16 si = 0;
		Timer *bp04 = glbTimersTable;
		for (; si < glbTimersActiveCount; bp04++, si++) {
			if (bp04->TimerType() == ttyPlague) {
				if (bp04->actor == di) {
					DELETE_TIMER(si);
				}
			}
		}
		glbChampionSquad[di].PlagueValue = 0;
	}
	return;
}


// SPX: Custom, code added, similar to POISON
void SkWinCore::PROCESS_PLAGUE(i16 player, Bit16u counters)
{
	
	if (SkCodeParam::bUseIngameDebug)
	{
		U8 message[64];
		sprintf((char*)message, "PLAYER %d HAS %d PLAGUE COUNTERS.\n", player, counters);
		DISPLAY_HINT_TEXT(glbChampionColor[player], message);
	}
	X16 si = counters;
	if (player == -1)
		return;
	if (player +1 == glbNextChampionNumber)
		return;
	Champion *champion = &glbChampionSquad[player];
	WOUND_PLAYER(player, 1, 0, 0);
	ADJUST_STAMINA(player, max_value(1, si << 4));
	champion->curWater(champion->curWater() -100);
	if (champion->curWater() < WATER_MIN)
		champion->curWater(WATER_MIN);


	champion->heroFlag |= CHAMPION_FLAG_0800;	// 0x800
	champion->heroFlag |= CHAMPION_FLAG_2000;	// 0x2000
	si--;
	if (si == 0)
		return;
	champion->PlagueValue++;

	// Create a new timer
	Timer newtimer;
	newtimer.TimerType(ttyPlague);
	newtimer.actor = (U8)player;
	newtimer.SetMap(glbPlayerMap);
	newtimer.SetTick(glbGameTick +0x24);
	newtimer.value = si;
	QUEUE_TIMER(&newtimer);
	return;
}



////////////////////////////////////////////////////////////////////////////////

// SPX: New function to test more text ingame
void SkWinCore::TEST_TEXT()
{
	U8 str[200];
	// Get text from new category, then display this text
	Bit8u sBuf1[200];
	Bit8u sBuf2[200];
	Bit8u* sBuf3;

	sBuf3 = QUERY_GDAT_TEXT(0x1B, 0, 0, sBuf1);

	memset(str, 0, 200);
	printf("TEXT: <%s>\n", sBuf3);

	return;
}

