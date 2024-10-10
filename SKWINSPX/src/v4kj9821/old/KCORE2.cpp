//--- SkWinCore2 -------------------------------------
//SPX: This part of code will hold more easily added code for
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
#ifdef __DJGPP__
#include <SkDOS.h>
#include <stdlib.h> // rand note: putting stdlib here and not right after stdafx prevents a bunch of conflicts with min/max macros
#endif // __DJGPP__


#include <KAITABLE.h>	// for dAITableGenuine (hard coded AI table)
//--- Common part with A.cpp


X16
SkWinCore::EXTENDED_LOAD_SPELLS_DEFINITION(void)
{
//#if DM2_EXTENDED_MODE == 1
	if (SkCodeParam::bUseDM2ExtendedMode)
	{
		U8 di = 0;
		U8 index = 0;
		U8 category = GDAT_CATEGORY_SPELL_DEF;
		for (index = 0; index < MAXSPELL_CUSTOM-1; index++)	// MAXSPELL_CUSTOM = 255, but the value 255 is kept for the default.
		{
			U8 spellname[0x80];
			di = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 0x01); // Test if RUNE01 is used
			if (di != 0)
			{
				U8 *rc = QUERY_GDAT_TEXT(category, index, 0x18, spellname);
				U8 rune2 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 0x02);
				U8 rune3 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 0x03);
				dSpellsTableCustom[index].dw0 = (MkssymVal(di, rune2, rune3));
				dSpellsTableCustom[index].difficulty = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 0x04);
				dSpellsTableCustom[index].requiredSkill = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 0x05);
				U8 type = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 0x06);
				U8 result = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 0x07);
				dSpellsTableCustom[index].w6 = 0x0000 + (type & 0x0F) + ((result & 0x3F)<<4);

				dSpellsTableCustom[index].spellValue = result;

				SkD((DLV_TWEET, "Tweet: Loading spell %d (%02X) with %s (t:%s, d:%d, s:%s, v:%d)\n", index, index, spellname,
					getSpellTypeName(type), dSpellsTableCustom[index].difficulty, getSkillName(dSpellsTableCustom[index].requiredSkill),
					dSpellsTableCustom[index].SpellCastIndex()));
				
			}
		}
		//--- Note: Due to extended structure of SpellDefinition in extended mode, the original spell tables must be adapted to this value
		for (index = 0; index < MAXSPELL_ORIGINAL-1; index++)
		{
			dSpellsTable[index].spellValue = U8((dSpellsTable[index].w6 >> 4)&0x3f);
		}
		return 1;
	}
//#endif
	return 0;
}



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
//	while (xCurrentObject.Dir() != iDirection || xCurrentObject.DBType() < dbWeapon || xCurrentObject.DBType() >= dbMissile)
	while ( (xCurrentObject.Dir() != iDirection || xCurrentObject.DBType() < dbWeapon || xCurrentObject.DBType() >= dbMissile) 
		&& (xCurrentObject != OBJECT_END_MARKER && xCurrentObject != OBJECT_NULL) )
	{
		xCurrentObject = GET_NEXT_RECORD_LINK(xCurrentObject);
	}
	if (SkCodeParam::bUsePowerDebug && (xCurrentObject == OBJECT_NULL || xCurrentObject == OBJECT_END_MARKER))
		return OBJECT_NULL;
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


X16 SkWinCore::SELECT_CHAMPION_FROM_GDAT(U8 iChampionID)
{
	U16 iChampionNumber = 0;
	if (glbLeaderHandPossession.object != OBJECT_NULL || (iChampionNumber = glbChampionsCount) >= MAX_CHAMPIONS)
		return 0;
	X16 iCurrentMap = glbCurrentMapIndex;
	ObjectID xObject;
	X16 iHeroType = iChampionID;
	int dir = 0;
	REVIVE_PLAYER(iHeroType, iChampionNumber, dir);
	if (iChampionNumber != 0) {
		DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
		UPDATE_RIGHT_PANEL(0);
	}
	glbNextChampionNumber = iChampionNumber + 1;
	glbChampionsCount++;
	if (iChampionNumber == 0)	// First champion, then select it as leader
		SELECT_CHAMPION_LEADER(0);
	if (_4976_404b == 0) {
		INTERFACE_CHAMPION(iChampionNumber);
		_1031_0541(7);
		_38c8_0002();
	}
	CHANGE_CURRENT_MAP_TO(iCurrentMap);
	CALC_PLAYER_WEIGHT(iChampionNumber);
	return iChampionNumber + 1;
}




// Check QUERY_CREATURES_ITEM_MASK
Bit16u SkWinCore::CREATE_ABSOLUTE_ACTIVATOR_ITEM_ID(int iDBType, int iItemID)
{
	Bit16u iAbsoluteItemID = 0xFFFF;
	switch (iDBType) {
		case dbWeapon:		// W - range 128
			iAbsoluteItemID = 0x0000 + iItemID; break;
		
		case dbCloth:		// A - range 128
			iAbsoluteItemID = 0x0080 + iItemID; break;

		case dbMiscellaneous_item:	// J - range 128
			iAbsoluteItemID = 0x0100 + iItemID; break;
		
		case dbPotion:		// P - range 96
			iAbsoluteItemID = 0x0180 + iItemID; break;
		
		case dbCreature:	// C - range 28
			iAbsoluteItemID = 0 + iItemID; break;
		case dbContainer:	// C - range 28
			iAbsoluteItemID = 0x01E0 + iItemID; break;
		
		case dbScroll:		// S - range 4
			iAbsoluteItemID = 0x01FC + iItemID; break;
	}
	return iAbsoluteItemID;
}








void SkWinCore::CREATE_NEW_ITEM_AT_POSITION(int iDBItem, int iItemType, int iMap, int iPosX, int iPosY, int iFace)
{
	X16 iCurrentMap = glbCurrentMapIndex;
	ObjectID xObject = 0;
	int iActivatorItemID = 0;

	iActivatorItemID = CREATE_ABSOLUTE_ACTIVATOR_ITEM_ID(iDBItem, iItemType);

	xObject = ALLOC_NEW_DBITEM(iActivatorItemID); // iDBItem | (iActivatorItemID & 0x8000)
	if (xObject != OBJECT_NULL && xObject != OBJECT_END_MARKER)
	{
		CHANGE_CURRENT_MAP_TO(iMap);
		MOVE_RECORD_TO(ObjectID(xObject, DIR_NORTH), -1, 0, iPosX, iPosY);
		CHANGE_CURRENT_MAP_TO(iCurrentMap);
	}
}

void SkWinCore::CREATE_NEW_ITEM_FOR_PLAYER(int iDBItem, int iItemType, int iChampionNumber)
{
	X16 iCurrentMap = glbCurrentMapIndex;
	ObjectID xObject = 0;
	int iActivatorItemID = 0;

	iActivatorItemID = CREATE_ABSOLUTE_ACTIVATOR_ITEM_ID(iDBItem, iItemType);

	xObject = ALLOC_NEW_DBITEM(iActivatorItemID); // iDBItem | (iActivatorItemID & 0x8000)
	if (xObject != OBJECT_NULL && xObject != OBJECT_END_MARKER)
	{
		ADD_ITEM_TO_PLAYER(iChampionNumber, xObject);
	}
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

// SPX: Bypass shelfmemory for direct GDAT TEXT, be careful that may not work in any other place
Bit8u *SkWinCore::DIRECT_QUERY_GDAT_ENTRY_DATA_BUFF(Bit8u cls1, Bit8u cls2, Bit8u cls3, Bit8u cls4)
{
	LOGX(("%40s: C%02d=I%02X=E%02X=T%03d to %08X", "QUERY_GDAT_ENTRY_DATA_BUFF from ", cls1, cls2, cls4, cls3 ));
	i16 si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, cls3, cls4);
	if (si == -1) {
		return NULL;
	}
	if (glbShelfMemoryTable[si].Absent()) {
		Bit16u bp02;
		return QUERY_GDAT_DYN_BUFF(si, &bp02, 0);
	}
	else {
		return REALIZE_GRAPHICS_DATA_MEMORY(glbShelfMemoryTable[si]);
	}
}

// SPX: Bypass some control to get TEXT for loading the dungeon name, be careful that may not work in any other place
Bit8u *SkWinCore::DIRECT_QUERY_GDAT_TEXT(Bit8u cls1, Bit8u cls2, Bit8u cls4, Bit8u *buff)
{
	*buff = 0;
	Bit16u di = 0;
	Bit16u si = 0;
	Bit8u *bp0e = DIRECT_QUERY_GDAT_ENTRY_DATA_BUFF(cls1, cls2, dtText, cls4);
	if (bp0e != NULL) {
		Bit8u *bp0a = ALLOC_MEMORY_RAM(
			di = si = QUERY_GDAT_ENTRY_DATA_LENGTH(cls1, cls2, dtText, cls4),
			0,
			1024
			);
		COPY_MEMORY(bp0e, bp0a, si);
		if (glbTextEntryEncoded != 0) {
			Bit8u *bp04 = bp0a;
			Bit8u bp05 = 0;
			while (di-- != 0) {
				*bp04 = (*bp04 ^ 0xFF) - bp05;
				bp05++;
				bp04++;
			}
		}
		FORMAT_SKSTR(bp0a, buff);
		DEALLOC_UPPER_MEMORY(si);
	}
	return buff;
}

// SPX: This gets the standard CHAMPIONS BONES ID expected for either regular DM2 or regular DM1 compatible mode
Bit8u SkWinCore::GET_CHAMPION_BONES_ITEM_ID()
{
	if (SkCodeParam::bDM1Mode == true)
		return 5;
	// else default is 0 for DM2
	return 0;
}

// SPX: For displaying wall side gibberish text or not, take an object ID and test if text is visible or not
Bit8u SkWinCore::IS_OBJECT_VISIBLE_TEXT(ObjectID rlObject)
{
	if (rlObject == OBJECT_NULL)
		return 0;
	if (rlObject.DBType() == dbText)
	{
		Text *xText;
		xText = GET_ADDRESS_OF_RECORD2(rlObject);
		return xText->TextVisibility();
	}
	return 0;
}



// SPX: Move Player to specific position (like move or teleport)
U16 SkWinCore::FORCE_PLAYER_PARTY_TO(i16 xposTo, i16 yposTo, i16 mapTo)
{
	U16 iMoveRC = 0;
	// force change map
	CHANGE_CURRENT_MAP_TO(glbPlayerMap);
	MOVE_RECORD_TO(OBJECT_NULL, glbPlayerPosX, glbPlayerPosY, -1, 0);
	LOAD_NEWMAP(mapTo);
	iMoveRC = MOVE_RECORD_TO(OBJECT_NULL, -1, 0, xposTo, yposTo);
	//ROTATE_SQUAD(bp06.b1);

	//iMoveRC = MOVE_RECORD_TO(OBJECT_NULL, glbPlayerPosX, glbPlayerPosY, xposTo, yposTo);
	return iMoveRC;
}


void SkWinCore::ADD_PARTY_CHAMPION(int iChampionID)
{
	SELECT_CHAMPION_FROM_GDAT(iChampionID);
	_2f3f_04ea_CHAMPION(glbPlayerPosX, glbPlayerPosY, glbPlayerDir, glbPlayerMap, UI_EVENTCODE_REVIVE_CHAMPION);
	_4976_404b = 0;
	glbChampionSquad[0].playerDir(U8(glbPlayerDir));
	glbChampionSquad[0].playerPos(U8(glbPlayerDir));
	SET_PARTY_HERO_FLAG(0x4000);
	SELECT_CHAMPION_LEADER(0);
}


void SkWinCore::INIT_CHAMPIONS_CUSTOM_MODES()
{
	if (SkCodeParam::bBWMode == true) {
		SELECT_CHAMPION_FROM_GDAT(rand()%16);
		_2f3f_04ea_CHAMPION(glbPlayerPosX, glbPlayerPosY, glbPlayerDir, glbPlayerMap, UI_EVENTCODE_REVIVE_CHAMPION);
		_4976_404b = 0;
		glbChampionSquad[0].playerDir(U8(glbPlayerDir));
		glbChampionSquad[0].playerPos(U8(glbPlayerDir));
		SET_PARTY_HERO_FLAG(0x4000);
		SELECT_CHAMPION_LEADER(0);

		//CREATE_NEW_ITEM_AT_POSITION(DB_CATEGORY_MISC_ITEM, 5, glbPlayerMap, glbPlayerPosX+1, glbPlayerPosY, 0);
		CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_MISC_ITEM, 5+rand()%(22-5), 0);
		CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_MISC_ITEM, 5+rand()%(22-5), 0);
		CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_MISC_ITEM, 5+rand()%(22-5), 0);
		CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_MISC_ITEM, 5+rand()%(22-5), 0);
		CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_MISC_ITEM, 5+rand()%(22-5), 0);
		CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_MISC_ITEM, 5+rand()%(22-5), 0);

		return;
	}

	else if (SkCodeParam::bRandomInitChampion == true) 
	{
		LUA_CALL_SCRIPT(_EXP_SCRIPT__PLAYER_INIT_, glbPlayerMap, glbPlayerPosX, glbPlayerPosY, 0);
		/*
		if (skwin.dung == _OPTION_DUNGEON_DMX_)
			SkWinLua_RunFunctionFromScript(this, "data_dmx/script/dungeon.lua", "player_init", glbPlayerMap, glbPlayerPosX, glbPlayerPosY, 0);
		else if (skwin.dung == _OPTION_DUNGEON_EOB_EOB1_)
			SkWinLua_RunFunctionFromScript(this, "data_eob_eob1/script/dungeon.lua", "player_init", glbPlayerMap, glbPlayerPosX, glbPlayerPosY, 0);
*/
		return;
	}
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////









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
	printf("OID NULL = %08X\n", OBJECT_NULL);
	printf("OID EOM  = %08X\n", OBJECT_END_MARKER);
	
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






U16 SkWinCore::EXT_PROCEED_DCS_GENERIC_COMMAND(const char* sCommandName, const char* sObjectScope, const char* sSubCommand, int iArg1, int iArg2, int iArg3, int iArg4, int iArg5)
{
	U16 iReturnCode = 0;
	int i = 0;
	int j = 0;
	int iMapIndex = 0;

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_YELLOW, BLACK);
	//printf("****************************************************\n");
	//printf("****************************************************\n");
	//PLAY_DIRECT_SOUND("data_dm1_tq/sound/hint.wav", 100);
	//PLAY_DIRECT_SOUND("data_dm1_tq/sound/TheDenOfTheDragon.mp3", 100);
	//PLAY_DIRECT_SOUND("data_dm1_tq/music/TQ-AmbianceDropsShort.wav", 100);
	
/*
	//==== BEGIN FLAGS
	if (!strcmp(sCommandName, "get") && !strcmp(sObjectScope, "flag"))
	{
		//iReturnValue = __testFlag;
		iReturnValue = _DCSEngine_SearchFlagValue(sSubCommand);
	}
	if (!strcmp(sCommandName, "set") && !strcmp(sObjectScope, "flag"))
	{
		//__testFlag = iSetValue;
		_DCSEngine_SetFlagValue(sSubCommand, iSetValue);
	}
	//==== END FLAGS
	*/
	int iTileType = 0;
	int iArgMap = iArg1;
	int iArgX = iArg2;
	int iArgY = iArg3;
	int iArgDir = iArg4;
	int iArgTile = iArg5;
	int iArgTileset = iArg5;
	int iArgItemID = iArg5;

	int iCreatureType = iArg4;
	int iCreatureHealth = iArg5;

	int iGfxID = iArg5;
	

	//==== BEGIN MUSIC
	if (!strcmp(sCommandName, "play") && !strcmp(sObjectScope, "music"))
	{
		int iLoopMode = iArg1;
		int iSoundID = iArg4;
		int iVolume = iArg5;
		
		char sBaseDir[128];
		char sFullMusicPath[256];
		char sIntModuleDirName[32];

		if (iLoopMode > 0)
			iLoopMode = 1;	// loop music
		else
			iLoopMode = 0;	// no loop

		sprintf(sBaseDir, "data_dm1_tq/music/", sIntModuleDirName);
		sprintf(sFullMusicPath, "%s%s", sBaseDir, sSubCommand);	// sSubCommand contains music filename

		PLAY_DIRECT_SOUND(sFullMusicPath, iVolume/3);
	}
	else if (!strcmp(sCommandName, "stop") && !strcmp(sObjectScope, "music"))
	{
		;//resete music channels
	}
	else if (!strcmp(sCommandName, "start") && !strcmp(sObjectScope, "music"))
	{
		;//obsolete
	}
	else if (!strcmp(sCommandName, "set") && !strcmp(sObjectScope, "music"))
	{
		;
		
	}
	//==== END MUSIC

	//==== TILE
	if (!strcmp(sCommandName, "set") && !strcmp(sObjectScope, "tile"))
	{
		DUNGEON_SET_TILE(iArgTile, 0,  iArgMap, iArgX, iArgY);
	}
	else if (!strcmp(sCommandName, "set") && !strcmp(sObjectScope, "tile.flags"))
	{
		int iArgTileFlags = iArg5;
		//DUNGEON_SET_TILE(iArgTile, 0,  iArgMap, iArgX, iArgY);
		DUNGEON_SET_TILE_FLAG_RAWVALUE(iArgTileFlags, iArgMap, iArgX, iArgY);
	}
	else if (!strcmp(sCommandName, "put") && !strcmp(sObjectScope, "door"))
	{
		DUNGEON_SET_TILE(ttDoor, 0,  iArgMap, iArgX, iArgY);
		DUNGEON_PUT_NEW_DOOR(0, iArgMap, iArgX, iArgY);

	}
	else if (!strcmp(sCommandName, "create") && !strcmp(sObjectScope, "teleporter"))
	{
		int iActive = iArg4;
		DUNGEON_SET_TILE(ttTeleporter, 0,  iArgMap, iArgX, iArgY);
		DUNGEON_PUT_NEW_TELEPORTER(iArgMap, iArgX, iArgY);
		DUNGEON_SET_TILE_FLAG(TILE_FLAG_HAS_HAZE, iArgMap, iArgX, iArgY);
	}
	else if (!strcmp(sCommandName, "open") && !strcmp(sObjectScope, "pit"))
	{
		iTileType = DUNGEON_GET_TILE_TYPE(iArgMap, iArgX, iArgY);
		if (iTileType == ttPit)
			DUNGEON_SET_TILE_FLAG(TILE_FLAG_IS_OPEN, iArgMap, iArgX, iArgY);
	}
	else if (!strcmp(sCommandName, "close") && !strcmp(sObjectScope, "pit"))
	{
		iTileType = DUNGEON_GET_TILE_TYPE(iArgMap, iArgX, iArgY);
		if (iTileType == ttPit)
			DUNGEON_SET_TILE_FLAG(TILE_FLAG_IS_CLOSED, iArgMap, iArgX, iArgY);
	}
	else if (!strcmp(sCommandName, "set") && !strcmp(sObjectScope, "stairs") && !strcmp(sSubCommand, "up"))
	{
		iTileType = DUNGEON_GET_TILE_TYPE(iArgMap, iArgX, iArgY);
		if (iTileType == ttStairs)
			DUNGEON_SET_TILE_FLAG(TILE_FLAG_IS_LEADING_UP, iArgMap, iArgX, iArgY);
	}
	else if (!strcmp(sCommandName, "set") && !strcmp(sObjectScope, "stairs") && !strcmp(sSubCommand, "down"))
	{
		iTileType = DUNGEON_GET_TILE_TYPE(iArgMap, iArgX, iArgY);
		if (iTileType == ttStairs)
			DUNGEON_SET_TILE_FLAG(TILE_FLAG_IS_LEADING_DOWN, iArgMap, iArgX, iArgY);
	}
	//==== END TILE

	//==== DOOR
	else if (!strcmp(sCommandName, "set") && !strcmp(sObjectScope, "door.type"))
	{
		int iDoorID = iArg5;	// absolute ID, to be compared against map's door type 1 and 2.
		Door_Info* xDoor = NULL;
		xDoor = (Door_Info*) DUNGEON_GET_DOOR_AT_POSITION(iArgMap, iArgX, iArgY);
		if (xDoor != NULL)
		{
			xDoor->type = 1;
		}

	}
	else if (!strcmp(sCommandName, "set") && !strcmp(sObjectScope, "door.opening"))
	{
		int iDoorID = iArg5;	// absolute ID, to be compared against map's door type 1 and 2.
		Door_Info* xDoor = NULL;
		xDoor = (Door_Info*) DUNGEON_GET_DOOR_AT_POSITION(iArgMap, iArgX, iArgY);
		if (xDoor != NULL)
		{
			if (!strcmp(sSubCommand, "vertical"))
				xDoor->verticalOpening = 1;
			else if (!strcmp(sSubCommand, "horizontal"))
				xDoor->verticalOpening = 0;
		}

	}
	else if (!strcmp(sCommandName, "set") && !strcmp(sObjectScope, "door.decoration"))
	{
		int iDoorDecoration = iArg5;	// absolute gfx
		int iGfxMapID = 0;
		Door_Info* xDoor = NULL;
		xDoor = (Door_Info*) DUNGEON_GET_DOOR_AT_POSITION(iArgMap, iArgX, iArgY);
		if (xDoor != NULL)
		{
			iGfxMapID = DUNGEON_SET_DOOR_GFX(iGfxID, iArgMap);
			xDoor->ornate = iGfxMapID+1;
		}
	}

	else if (!strcmp(sCommandName, "toggle") && !strcmp(sObjectScope, "door"))
	{
		Door_Info* xDoor = NULL;
		xDoor = (Door_Info*) DUNGEON_GET_DOOR_AT_POSITION(iArgMap, iArgX, iArgY);
		if (xDoor != NULL)
		{
			INVOKE_MESSAGE(iArgX, iArgY, 0, 2, glbGameTick + 1);
		}
	}
	else if (!strcmp(sCommandName, "open") && !strcmp(sObjectScope, "door"))
	{
		Door_Info* xDoor = NULL;
		xDoor = (Door_Info*) DUNGEON_GET_DOOR_AT_POSITION(iArgMap, iArgX, iArgY);
		if (xDoor != NULL)
		{
			INVOKE_MESSAGE(iArgX, iArgY, 0, 2, glbGameTick + 1);
		}
	}
	else if (!strcmp(sCommandName, "close") && !strcmp(sObjectScope, "door"))
	{
		Door_Info* xDoor = NULL;
		xDoor = (Door_Info*) DUNGEON_GET_DOOR_AT_POSITION(iArgMap, iArgX, iArgY);
		if (xDoor != NULL)
		{
			INVOKE_MESSAGE(iArgX, iArgY, 0, 2, glbGameTick + 1);
		}
	}
	//==== END DOOR


	//==== TILESET
	if (!strcmp(sCommandName, "set") && !strcmp(sObjectScope, "tileset"))
	{
		DUNGEON_SET_TILESET(iArgTileset, iArgMap);
	}
	//==== END TILESET

	//==== ITEMS
	if (!strcmp(sCommandName, "create") && !strcmp(sObjectScope, "weapon"))
	{
		DUNGEON_PUT_NEW_WEAPON(iArgItemID, iArgMap, iArgX, iArgY);
	}
	else if (!strcmp(sCommandName, "create") && !strcmp(sObjectScope, "clothing"))
	{
		DUNGEON_PUT_NEW_CLOTHING(iArgItemID, iArgMap, iArgX, iArgY);
	}
	else if (!strcmp(sCommandName, "create") && !strcmp(sObjectScope, "misc"))
	{
		DUNGEON_PUT_NEW_MISC_ITEM(iArgItemID, iArgMap, iArgX, iArgY);
	}
	else if (!strcmp(sCommandName, "create") && !strcmp(sObjectScope, "potion"))
	{
		DUNGEON_PUT_NEW_POTION(iArgItemID, iArgMap, iArgX, iArgY);
	}
	else if (!strcmp(sCommandName, "create") && !strcmp(sObjectScope, "container"))
	{
		DUNGEON_PUT_NEW_CONTAINER(iArgItemID, iArgMap, iArgX, iArgY);
	}
	//==== END ITEMS

	//==== CREATURE
	if (!strcmp(sCommandName, "create") && !strcmp(sObjectScope, "creature"))
	{
		//iCreatureHealth;
		DUNGEON_PUT_NEW_CREATURE(iCreatureType, iArgMap, iArgX, iArgY);
	}
	//==== END CREATURE

	//==== CREATURE DATA (AI TABLE)
	if (!strcmp(sCommandName, "modify") && !strcmp(sObjectScope, "creature.data"))
	{
		char* sAttribute = (char*) sSubCommand;
		int iCreatureType = iArg4;
		int iAttributeValue = iArg5;
		if (!strcmp(sAttribute, "hp"))
			dAITable[iCreatureType].BaseHP = iAttributeValue;
		else if (!strcmp(sAttribute, "ac"))
			dAITable[iCreatureType].ArmorClass = iAttributeValue;
		else if (!strcmp(sAttribute, "def"))
			dAITable[iCreatureType].Defense = iAttributeValue;
		
	}
	//==== END CREATURE DATA (AI TABLE)

	//==== PLAYER
	if (!strcmp(sCommandName, "set") && !strcmp(sObjectScope, "player.position"))	// ingame position, like teleport
	{
		//DUNGEON_PUT_FLOOR_DECORATION(iGfxID, iArgMap, iArgX, iArgY);
		//MOVE_RECORD_TO(OBJECT_NULL, -1, 0, glbPlayerPosX, glbPlayerPosY);
		FORCE_PLAYER_PARTY_TO(iArgX, iArgY, iArgMap);
	}
	else if (!strcmp(sCommandName, "set") && !strcmp(sObjectScope, "player.start")) // starting position
	{
		glbPlayerPosX = iArgX;
		glbPlayerPosY = iArgY;
		glbPlayerDir = iArgDir;
		glbPlayerMap = iArgMap;
	}
	else if (!strcmp(sCommandName, "create") && !strcmp(sObjectScope, "player.item")) // create item for player
	{
		int iChampionID = iArg3;
		int iDBCategory = iArg1;
		int iItemID = iArg2;
		CREATE_NEW_ITEM_FOR_PLAYER(iDBCategory, iItemID, iChampionID);
	}
	else if (!strcmp(sCommandName, "add") && !strcmp(sObjectScope, "champion"))
	{
		int iChampionID = iArg4;
		ADD_PARTY_CHAMPION(iChampionID);
	}
	//==== END PLAYER


	//==== GFX OBJECTS
	if (!strcmp(sCommandName, "create") && !strcmp(sObjectScope, "floor.decoration"))
	{
		DUNGEON_PUT_FLOOR_DECORATION(iGfxID, iArgMap, iArgX, iArgY);
	}
	else if (!strcmp(sCommandName, "set") && !strcmp(sObjectScope, "floor.decoration"))
	{
		DUNGEON_SET_FLOOR_GFX(iGfxID, iArgMap);
	}
	else if (!strcmp(sCommandName, "set") && !strcmp(sObjectScope, "floor.random.size"))
	{
		int iRandomGfxListSize = iArg5;
		DUNGEON_SET_FLOOR_RANDOM_LIST_SIZE(iRandomGfxListSize, iArgMap);
	}
	//==== END GFX OBJECTS


	//==== ACTUATORS
	if (!strcmp(sCommandName, "create") && !strcmp(sObjectScope, "actuator.floor.pad.party"))
	{
		Actuator* xActuator = NULL;
		U16 iGfxMapID = 0;
		U16 iTargetMap = iArgMap;
		U16 iTargetX = iArgX;
		U16 iTargetY = iArgY;
		//DCS_GenericCommand("create","actuator.floor.pad.party","tmap="..tmap..";tx="..tx..";ty="..ty,map,x,y,active,pad_gfx)
		iGfxMapID = DUNGEON_SET_FLOOR_GFX(iGfxID, iArgMap);
		xActuator = DUNGEON_PUT_FLOOR_ACTUATOR(iGfxID, iArgMap, iArgX, iArgY);
		if (xActuator != NULL)
		{
			sscanf(sSubCommand, "tmap=%d;tx=%d;ty=%d", &iTargetMap, &iTargetX, &iTargetY);
			xActuator->OnceOnlyActuator(0);
			xActuator->SetTarget(iTargetMap, iTargetX, iTargetY);
			xActuator->GraphicNumber(iGfxMapID);
			xActuator->SoundEffect(1);
			xActuator->ActuatorType(ACTUATOR_FLOOR_TYPE__PARTY);
			xActuator->ActionType(ACTEFFECT_STEP_ON__OPEN_SET);
		}
	}
	else if (!strcmp(sCommandName, "create") && !strcmp(sObjectScope, "actuator.wall.champion"))
	{
		Actuator* xActuator = NULL;
		U16 iGfxMapID = 0;
		U16 iTargetMap = iArgMap;
		U16 iTargetX = iArgX;
		U16 iTargetY = iArgY;
		U16 iChampionID = iArg5;
		
		iGfxMapID = DUNGEON_SET_WALL_GFX(0x2B, iArgMap); // champion mirror
		xActuator = DUNGEON_PUT_WALL_ACTUATOR(iGfxID, iArgMap, iArgX, iArgY, iArgDir);
		if (xActuator != NULL)
		{
			xActuator->OnceOnlyActuator(1);
			xActuator->SetTarget(iArgMap, iArgX, iArgY);
			xActuator->GraphicNumber(iGfxMapID);
			xActuator->SoundEffect(1);
			xActuator->ActuatorType(ACTUATOR_TYPE_RESURECTOR);
			xActuator->ActionType(ACTEFFECT_STEP_ON__OPEN_SET);
			xActuator->ActuatorData(iChampionID);
		}
	}
	//==== END ACTUATORS


	//==== MESSAGES
	if (!strcmp(sCommandName, "display") && !strcmp(sObjectScope, "message"))
	{
		char* sMessage = (char*) sSubCommand;
		if (sMessage != NULL)
			DISPLAY_HINT_TEXT(COLOR_YELLOW, (U8*)sMessage);
	}
	//==== END MESSAGES

	//==== DEBUG MESSAGES
	if (!strcmp(sCommandName, "print") && !strcmp(sObjectScope, "debug"))
	{
		char* sMessage = (char*) sSubCommand;
		if (sMessage != NULL)
		{
			DISPLAY_HINT_TEXT(COLOR_YELLOW, (U8*)sMessage);
			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_YELLOW, BLACK);
			printf("%s\n", sMessage);
			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);		}
	}
	//==== END DEBUG MESSAGES


	return iReturnCode;
}

void SkWinCore::PLAY_DIRECT_SOUND(const char* sAudioFilename, int volume)
{
#if defined(_USE_MFC80) || defined(_USE_MFC60)
	skwin.SndPlayFile(sAudioFilename, volume);
#endif
	;
}

// static table of musics mapped to maps -- poor implementation and not dynamic yet
int tMusicMapsWAV[64] =
{
	0x02, 0x11, 0x0e, 0x1b,		0x04, 0x0c, 0x0c, 0x12,
	0x0f, 0x0d, 0x0c, 0x0c,		0x10, 0x06, 0x15, 0x0e,
	
	0x11, 0x11, 0x11, 0x11,		0x03, 0x08, 0x11, 0x0e,
	0x02, 0x17, 0x16, 0x14,		0x11, 0x00, 0x02, 0x02,
	
	0x02, 0x09, 0x02, 0x03,		0x0e, 0x10, 0x1c, 0x16,
	0x13, 0x09, 0x16, 0x03,		0x11, 0x02, 0xFF, 0xFF,

	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF
};

void SkWinCore::CHANGE_MUSIC_ID(int iMapNumber, int iMusicID)
{
	tMusicMapsWAV[iMapNumber] = iMusicID;
}

int iCurrentWavMusic = -1;

void SkWinCore::REQUEST_PLAY_MUSIC_FROM_MAP(int iMapNumber)
{
	int iNextRequestedMusic = 0;
	char sDataRootFolder[64];
	char sFullMusicPath[256];
	int iVolume = 50;
	sprintf(sDataRootFolder, "DATA");	// default

	if (skwin.dung == _OPTION_DUNGEON_DM2_SK_BETA_ || skwin.dung == _OPTION_DUNGEON_DM2_SK_DEMO_ ||
		skwin.dung == _OPTION_DUNGEON_DM2_SK_ || skwin.dung == _OPTION_DUNGEON_DM2_SK_EXT_ || 
		skwin.dung == _OPTION_DUNGEON_DMX_)
		iNextRequestedMusic = tMusicMapsWAV[iMapNumber];
	else
		iNextRequestedMusic = iMapNumber;

	if (skwin.dung == _OPTION_DUNGEON_DM1_DM_ && iMapNumber != 0)
		iNextRequestedMusic = 1;



	if (iNextRequestedMusic == iCurrentWavMusic)
		return;

	/*
	switch(skwin.dung)
	{
		case _OPTION_DUNGEON_NO_SPECIFIC_:
		default:
			sprintf(sDataRootFolder, "DATA"); break;
	} // END of specific block
	*/
	sprintf(sDataRootFolder, "%s", GET_DATA_FOLDER_NAME(skwin.dung));
	sprintf(sFullMusicPath, "./%s/music/%02x.wav", sDataRootFolder, iNextRequestedMusic);
	//PLAY_DIRECT_SOUND(sFullMusicPath, 50);
	#if defined(_USE_MFC80) || defined(_USE_MFC60)
		iCurrentWavMusic = iNextRequestedMusic;
		skwin.SndPlayFile(sFullMusicPath, iVolume);
	#endif
	return;
}

void SkWinCore::REQUEST_PLAY_MUSIC(int iMusicNumber)
{
	int iNextRequestedMusic = 0;
	char sDataRootFolder[64];
	char sFullMusicPath[256];
	int iVolume = 50;
	sprintf(sDataRootFolder, "DATA");	// default
	iNextRequestedMusic = iMusicNumber;

	if (iNextRequestedMusic == iCurrentWavMusic)
		return;
/*
	switch(skwin.dung)
	{
		case _OPTION_DUNGEON_DM1_KID_:
			sprintf(sDataRootFolder, "DATA_DM1_KID"); break;
		case _OPTION_DUNGEON_DM1_DM_:
			sprintf(sDataRootFolder, "DATA_DM1_DM"); break;
		case _OPTION_DUNGEON_DM1_CSB_:
			sprintf(sDataRootFolder, "DATA_DM1_CSB"); break;
		case _OPTION_DUNGEON_DM1_TQ_:
			sprintf(sDataRootFolder, "DATA_DM1_TQ"); break;
		case _OPTION_DUNGEON_DM2_DM_:
			sprintf(sDataRootFolder, "DATA_DM2_DM"); break;
		case _OPTION_DUNGEON_DM2_CSB_:
			sprintf(sDataRootFolder, "DATA_DM2_CSB"); break;
		case _OPTION_DUNGEON_DM2_TQ_:
			sprintf(sDataRootFolder, "DATA_DM2_TQ"); break;
		case _OPTION_DUNGEON_DM2_SK_BETA_:
			sprintf(sDataRootFolder, "DATA_DM2_BETA"); break;
		case _OPTION_DUNGEON_DM2_SK_DEMO_:
			sprintf(sDataRootFolder, "DATA_DM2_DEMO"); break;
		case _OPTION_DUNGEON_DM2_SK_:
			sprintf(sDataRootFolder, "DATA_DM2_SK"); break;
		case _OPTION_DUNGEON_DM2_SK_EXT_:
			sprintf(sDataRootFolder, "DATA_DM2_TORC"); break;
		case _OPTION_DUNGEON_DMX_:
			sprintf(sDataRootFolder, "DATA_DMX"); break;

		case _OPTION_DUNGEON_NO_SPECIFIC_:
		default:
			sprintf(sDataRootFolder, "DATA"); break;
	} // END of specific block
	*/
	sprintf(sDataRootFolder, "%s", GET_DATA_FOLDER_NAME(skwin.dung));
	sprintf(sFullMusicPath, "./%s/music/%02x.wav", sDataRootFolder, iNextRequestedMusic);
	//PLAY_DIRECT_SOUND(sFullMusicPath, 50);
	#if defined(_USE_MFC80) || defined(_USE_MFC60)
		iCurrentWavMusic = iNextRequestedMusic;
		skwin.SndPlayFile(sFullMusicPath, iVolume);
	#endif
	return;
}