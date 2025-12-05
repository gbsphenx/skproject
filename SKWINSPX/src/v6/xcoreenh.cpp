//--- SkWinCore-Enhanced -------------------------------------
//SPX: This part of code contains:
// - Enhanced code for expanding DM2
// - Some utility functions

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

//#if defined(__LINUX__)
//#include <SKSDL2.h>
//#include <stdlib.h>
//#elif defined(__DJGPP__) || defined(__MINGW__)
//#include <SKDOS.h>
//#include <stdlib.h> // rand note: putting stdlib here and not right after stdafx prevents a bunch of conflicts with min/max macros
//#endif // __DJGPP__
#include <stdlib.h>

//#include <kaitable.h>	// for dAITableGenuine (hard coded AI table)

//==============================================================================

X16
SkWinCore::EXTENDED_LOAD_SPELLS_DEFINITION(void)
{
	X16 iExtendedResult = 0;
	{
		U8 gdatentry = 0;
		U8 index = 0;
		U8 category = GDAT_CATEGORY_SPELL_DEF;
		for (index = 0; index < MAXSPELL_CUSTOM-1; index++)	// MAXSPELL_CUSTOM = 255, but the value 255 is kept for the default.
		{
			U8 spellname[0x80];
			gdatentry = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 0x01); // Test if RUNE01 is used
			if (gdatentry != 0)
			{
				U8 *rc = QUERY_GDAT_TEXT(category, index, 0x18, spellname);
				U8 rune2 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 0x02);
				U8 rune3 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 0x03);
				dSpellsTableCustom[index].symbols = (MkssymVal(gdatentry, rune2, rune3));
				dSpellsTableCustom[index].difficulty = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 0x04);
				dSpellsTableCustom[index].requiredSkill = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 0x05);
				U8 type = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 0x06);
				U8 result = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 0x07);
				dSpellsTableCustom[index].w6 = 0x0000 + (type & 0x0F) + ((result & 0x3F)<<4);

				dSpellsTableCustom[index].spellValue = result;

				SkD((DLV_TWEET, "Tweet: Loading spell %d (%02X) with %s (t:%s, d:%d, s:%s, v:%d)\n", index, index, spellname,
					getSpellTypeName(type), dSpellsTableCustom[index].difficulty, getSkillName(dSpellsTableCustom[index].requiredSkill),
					dSpellsTableCustom[index].SpellCastIndex()));
				
				iExtendedResult = 1;
				SkCodeParam::bUseCustomSpells = true;
			}
		}
#ifdef XDM2_EXTENDED_SPELLS_TAB
		//--- Note: Due to extended structure of SpellDefinition in extended mode, the original spell tables must be adapted to this value
		for (index = 0; index < MAXSPELL_ORIGINAL-1; index++)
		{
			dSpellsTable[index].spellValue = U8((dSpellsTable[index].w6 >> 4)&0x3F);
		}
#endif
		return iExtendedResult;
	}

	return iExtendedResult;
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



//==============================================================================

void SkWinCore::SET_NEXT_RECORD_LINK(ObjectID rlSourceObject, ObjectID rlObjectToAppend)
{
	GenericRecord *grObject = (GenericRecord *)GET_ADDRESS_OF_RECORD(rlSourceObject);
	grObject->w0 = rlObjectToAppend;
}

// SPX: Help function to get any Weapon to Misc item, generally from a wall
ObjectID SkWinCore::GET_WALL_TILE_ANY_TAKEABLE_ITEM_RECORD(U16 iMapX, U16 iMapY, U16 iDirection)
{
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

// SPX: Help function to get number of items within wall (alcove) in order to get some alcove actuators to trigger when expected
U16 SkWinCore::GET_TILE_COUNT_TAKEABLE_ITEMS(U16 iMapX, U16 iMapY, U16 iDirection)
{
	U16 iNbItemCount = 0;
	ObjectID xTakenObject = OBJECT_NULL;
	ObjectID xCurrentObject = GET_TILE_RECORD_LINK(iMapX, iMapY);
	while (xCurrentObject != OBJECT_END_MARKER && xCurrentObject != OBJECT_NULL) {
		if (xCurrentObject.Dir() == iDirection && xCurrentObject.DBType() >= dbWeapon && xCurrentObject.DBType() < dbMissile)
			iNbItemCount ++;
		xCurrentObject = GET_NEXT_RECORD_LINK(xCurrentObject);
	}
	return iNbItemCount;
}

//==============================================================================

X16 SkWinCore::SELECT_CHAMPION_FROM_GDAT(U8 iChampionID)
{
	U16 iChampionNumber = 0;
	if (cd.pi.glbLeaderHandPossession.object != OBJECT_NULL || (iChampionNumber = cd.pi.glbChampionsCount) >= MAX_CHAMPIONS)
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
	cd.pi.glbNextChampionNumber = iChampionNumber + 1;
	cd.pi.glbChampionsCount++;
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

//==============================================================================



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

// Note: that method to find item by name would not work because of the language selection/filter
void SkWinCore::CREATE_NEW_ITEM_FOR_PLAYER(int iDBItem, const char* sItemName, int iChampionNumber)
{
	Bit8u* xItemGroundImage = NULL;
	Bit8u* xItemName = NULL;
	char sLocalItemName[128];
	bool bMatch = false;
	int iItemID = 1;
	int iGDATDB = GDAT_CATEGORY_MISCELLANEOUS;

	if (iDBItem == DB_CATEGORY_CLOTHING)
		iGDATDB = GDAT_CATEGORY_CLOTHES;
	else if (iDBItem == DB_CATEGORY_WEAPON)
		iGDATDB = GDAT_CATEGORY_WEAPONS;
	else if (iDBItem == DB_CATEGORY_MISC_ITEM)
		iGDATDB = GDAT_CATEGORY_MISCELLANEOUS;

	// Search from GDAT for this item
	for (iItemID = 0; iItemID < 0xFE; iItemID++) {
		xItemName = QUERY_GDAT_TEXT(iGDATDB, iItemID, 0x18, (U8*)sLocalItemName);	// text, item name
		xItemGroundImage = QUERY_GDAT_ENTRY_DATA_PTR(iGDATDB, iItemID, dtImage, 0x00);	// ground image
		if (xItemName != NULL && xItemGroundImage != NULL) {
			if (!strcmp(sItemName, sLocalItemName)) {
				bMatch = true;
				break;
			}
		}
	}
	if (bMatch == true)
		CREATE_NEW_ITEM_FOR_PLAYER(iDBItem, iItemID, iChampionNumber);
}



////////////////////////////////////////////////////////////////////////////////

// SPX: New function to test more text ingame
void SkWinCore::TEST_TEXT()
{
	U8 str[200];
	// Get text from new category, then display this text
	Bit8u sBuf1[200];
	//Bit8u sBuf2[200];
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
	CHANGE_CURRENT_MAP_TO(cd.pi.glbPlayerMap);
	MOVE_RECORD_TO(OBJECT_NULL, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, -1, 0);
	LOAD_NEWMAP(mapTo);
	iMoveRC = MOVE_RECORD_TO(OBJECT_NULL, -1, 0, xposTo, yposTo);
	//ROTATE_SQUAD(bp06.b1);

	//iMoveRC = MOVE_RECORD_TO(OBJECT_NULL, glbPlayerPosX, glbPlayerPosY, xposTo, yposTo);
	return iMoveRC;
}


void SkWinCore::ADD_PARTY_CHAMPION(int iChampionID)
{
	SELECT_CHAMPION_FROM_GDAT(iChampionID);
	REVIVE_CHAMPION(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, cd.pi.glbPlayerDir, cd.pi.glbPlayerMap, UI_EVENTCODE_REVIVE_CHAMPION);
	_4976_404b = 0;
	glbChampionSquad[0].playerDir(U8(cd.pi.glbPlayerDir));
	glbChampionSquad[0].playerPos(U8(cd.pi.glbPlayerDir));
	SET_PARTY_HERO_FLAG(0x4000);
	SELECT_CHAMPION_LEADER(0);
}


void SkWinCore::INIT_CHAMPIONS_CUSTOM_MODES()
{
	if (SkCodeParam::bBWMode == true || SkCodeParam::bEOBMode == true) {
		int iChampionSlot = 0;

		for (iChampionSlot = 0; iChampionSlot < 4; iChampionSlot++) {
			int iChampionID = rand()%16;

			// for BW, select champion among different factions
			if (SkCodeParam::bBWMode) {
				int iTabChampions[4][4] = { {0, 7, 10, 13},
											{1, 4, 11, 14},
											{2, 5,  8, 15},
											{3, 6,  9, 12},
											};
				iChampionID = rand()%4;
				iChampionID = iTabChampions[iChampionSlot][iChampionID];
			}


			printf("INIT_CHAMPIONS_CUSTOM_MODES FOR BW/EOB\n");
			SELECT_CHAMPION_FROM_GDAT(iChampionID);
			REVIVE_CHAMPION(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, cd.pi.glbPlayerDir, cd.pi.glbPlayerMap, UI_EVENTCODE_REVIVE_CHAMPION);
			_4976_404b = 0;
			glbChampionSquad[0].playerDir(U8(cd.pi.glbPlayerDir));
			glbChampionSquad[0].playerPos(U8(cd.pi.glbPlayerDir));
			SET_PARTY_HERO_FLAG(0x4000);
			SELECT_CHAMPION_LEADER(0);

			//CREATE_NEW_ITEM_AT_POSITION(DB_CATEGORY_MISC_ITEM, 5, glbPlayerMap, glbPlayerPosX+1, glbPlayerPosY, 0);
			CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_MISC_ITEM, 5+rand()%(22-5), iChampionSlot);
			CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_MISC_ITEM, 5+rand()%(22-5), iChampionSlot);
			CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_MISC_ITEM, 5+rand()%(22-5), iChampionSlot);
			CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_MISC_ITEM, 5+rand()%(22-5), iChampionSlot);
			CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_MISC_ITEM, 5+rand()%(22-5), iChampionSlot);
			CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_MISC_ITEM, 5+rand()%(22-5), iChampionSlot);

			CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_MISC_ITEM, "COMMON KEY", iChampionSlot);
			CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_MISC_ITEM, "COMMON KEY", iChampionSlot);
			CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_MISC_ITEM, "COMMON KEY", iChampionSlot);
			CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_MISC_ITEM, "GOLD COIN", iChampionSlot);
			CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_MISC_ITEM, "GOLD COIN", iChampionSlot);
			CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_MISC_ITEM, "GOLD COIN", iChampionSlot);
			
			//-- for BW, each champion has different items
			switch (iChampionID) {
				case 0: // Blodwyn
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_WEAPON, "STONE CLUB", iChampionSlot);
					break;
				case 1: // Murlock
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_WEAPON, "CHAOS STAFF", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "CRIMSON HOOD", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "CRIMSON DOUBLET", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "ELVEN HUKE", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "HALF-BOOTS", iChampionSlot);
					break;
				case 2:	// Eleanor
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_WEAPON, "SHORT SWORD", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "CRIMSON KIRTLE", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "CRIMSON SKIRT", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "HALF-BOOTS", iChampionSlot);
					break;
				case 3:	// Rosanne
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_WEAPON, "DAGGER", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "SILVERLIGHT KIRTLE", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "SILVERLIGHT SKIRT", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "HALF-BOOTS", iChampionSlot);
					break;
				case 4:	// Astroth
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_WEAPON, "LONG SWORD", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "HUNTER'S HUKE", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "HALF-BOOTS", iChampionSlot);
					break;
				case 5:	// Zothen
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_WEAPON, "DRAGON STAFF", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "YELLOW JERKIN", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "CRIMSON HUKE", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "HALF-BOOTS", iChampionSlot);
					break;
				case 6:	// Baldric
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_WEAPON, "SHORT SWORD", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "SILVERLIGHT CAP", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "SILVERLIGHT JERKIN", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "SILVERLIGHT HUKE", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "HALF-BOOTS", iChampionSlot);
					break;
				case 7:	// Elfric
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_WEAPON, "DAGGER", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "ELVEN CAP", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "ELVEN DOUBLET", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "ELVEN HUKE", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "HALF-BOOTS", iChampionSlot);
					break;
				case 8:	// Sir Lion
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_WEAPON, "BROAD SWORD", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "LION'S HELM", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "CRIMSON CLOAK", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "CRIMSON HUKE", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "HALF-BOOTS", iChampionSlot);
					break;
				case 9:	// Megrim
					// -- circlet
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_WEAPON, "MOON STAFF", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "MAGE CIRCLET", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "ELVEN KIRTLE", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "ELVEN SKIRT", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "ELVEN BOOTS", iChampionSlot);
					break;
				case 10:	// Sethra
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_WEAPON, "SHORT SWORD", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "YELLOW PANTS", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "HALF-BOOTS", iChampionSlot);
					break;
				case 11:	// Mr Flay
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_WEAPON, "DAGGER", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "HALF-BOOTS", iChampionSlot);
					break;
				case 12:	// Ulrich
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_WEAPON, "AXE", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "BLUE PANTS", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "HALF-BOOTS", iChampionSlot);
					break;
				case 13:	// Zastaph
					// -- hat
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_WEAPON, "SERPENT STAFF", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "SILVERLIGHT CAP", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "SILVERLIGHT JERKIN", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "YELLOW PANTS", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "HALF-BOOTS", iChampionSlot);
					break;
				case 14:	// Hengist
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_WEAPON, "SHORT SWORD", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "ELVEN DOUBLET", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "BLUE PANTS", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "HALF-BOOTS", iChampionSlot);
					break;
				case 15:	// Thai Chang
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_WEAPON, "DAGGER", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "NINJA'S MASK", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "CRIMSON DOUBLET", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "CRIMSON HUKE", iChampionSlot);
					CREATE_NEW_ITEM_FOR_PLAYER(DB_CATEGORY_CLOTHING, "HALF-BOOTS", iChampionSlot);
					break;
				default:
				break;
			}
		}


		return;
	}

	else if (SkCodeParam::bRandomInitChampion == true) 
	{
		LUA_CALL_SCRIPT(_EXP_SCRIPT__PLAYER_INIT_, cd.pi.glbPlayerMap, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 0);
		return;
	}
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

		sprintf(sBaseDir, "data_dm1_tq/music/%s", sIntModuleDirName);
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
		cd.pi.glbPlayerPosX = iArgX;
		cd.pi.glbPlayerPosY = iArgY;
		cd.pi.glbPlayerDir = iArgDir;
		cd.pi.glbPlayerMap = iArgMap;
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
		int iScanTargetMap = 0;
		int iScanTargetX = 0;
		int iScanTargetY = 0;
		//DCS_GenericCommand("create","actuator.floor.pad.party","tmap="..tmap..";tx="..tx..";ty="..ty,map,x,y,active,pad_gfx)
		iGfxMapID = DUNGEON_SET_FLOOR_GFX(iGfxID, iArgMap);
		xActuator = DUNGEON_PUT_FLOOR_ACTUATOR(iGfxID, iArgMap, iArgX, iArgY);
		if (xActuator != NULL)
		{
			sscanf(sSubCommand, "tmap=%d;tx=%d;ty=%d", &iScanTargetMap, &iScanTargetX, &iScanTargetY);
			iTargetMap = U16(iScanTargetMap);
			iTargetX = U16(iScanTargetX);
			iTargetY = U16(iScanTargetY);
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
	if (SkCodeParam::bUseAudioSDL == true)
		XAUDIO_SDL_PLAY_SOUND_FILE(sAudioFilename, volume);
	else {
//#if defined(_USE_MFC80) || defined(_USE_MFC60)
	skWinApp->skwin_SndPlayFile(sAudioFilename, volume);
//#endif
	;
	}
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

//	if (skwin.dung == _OPTION_DUNGEON_DM2_SK_BETA_ || skwin.dung == _OPTION_DUNGEON_DM2_SK_DEMO_ ||
//		skwin.dung == _OPTION_DUNGEON_DM2_SK_ || skwin.dung == _OPTION_DUNGEON_DM2_SK_EXT_ || 
//		skwin.dung == _OPTION_DUNGEON_DMX_)
		iNextRequestedMusic = tMusicMapsWAV[iMapNumber];
//	else
//		iNextRequestedMusic = iMapNumber;

//	if (skwin.dung == _OPTION_DUNGEON_DM1_DM_ && iMapNumber != 0)
//		iNextRequestedMusic = 1;



	if (iNextRequestedMusic == iCurrentWavMusic)
		return;
//#if defined(_USE_MFC80) || defined(_USE_MFC60)
	//PlaySound(NULL, NULL, SND_PURGE);
//	XAUDIO_SDL_PLAY_SOUND_FILE(NULL, 0);
//#endif
	/*
	switch(skwin.dung)
	{
		case _OPTION_DUNGEON_NO_SPECIFIC_:
		default:
			sprintf(sDataRootFolder, "DATA"); break;
	} // END of specific block
	*/
	sprintf(sDataRootFolder, "%s", skWinApp->GET_DATA_FOLDER_NAME());
	sprintf(sFullMusicPath, "./%s/music/%02x.wav", sDataRootFolder, iNextRequestedMusic);
	//PLAY_DIRECT_SOUND(sFullMusicPath, 50);
	//#if defined(_USE_MFC80) || defined(_USE_MFC60)
		iCurrentWavMusic = iNextRequestedMusic;
		skWinApp->skwin_SndPlayFile(sFullMusicPath, iVolume);
	//#endif
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

	if (SkCodeParam::bNoAudio == true || SkCodeParam::bNoMusic == true)
		return;

	//sprintf(sDataRootFolder, "%s", GET_DATA_FOLDER_NAME(skwin.dung));
	sprintf(sDataRootFolder, "%s", skWinApp->GET_DATA_FOLDER_NAME());
	sprintf(sFullMusicPath, "./%s/music/%02x.wav", sDataRootFolder, iNextRequestedMusic);
	//sprintf(sFullMusicPath, "./%s/music/%02x.hmp.mid", sDataRootFolder, iNextRequestedMusic);
	iCurrentWavMusic = iNextRequestedMusic;

	if (SkCodeParam::bUseAudioSDL) {
		XAUDIO_SDL_PLAY_SOUND_FILE(sFullMusicPath, iVolume);
		//XAUDIO_SDL_PLAY_MIDI_FILE(sFullMusicPath, iVolume);
	}
	else
	{
	////// XAUDIO MFC
#if defined(_USE_MFC80) || defined(_USE_MFC60)
	//PlaySound(NULL, NULL, SND_PURGE);
	XAUDIO_SDL_PLAY_SOUND_FILE(NULL, 0);
#endif
	//PLAY_DIRECT_SOUND(sFullMusicPath, 50);
	//#if defined(_USE_MFC80) || defined(_USE_MFC60)
		skWinApp->skwin_SndPlayFile(sFullMusicPath, iVolume);
	//#endif
/////// XAUDIO MFC
		;
	}


	return;
}


//==============================================================================

void SkWinCore::INIT_CHAMPION(Champion *xChampion)
{
	U8 iInventoryIndex = 0;
	U8 iAttributeIndex = 0;
	i16 iMaxHP = 100;
	i16 iMaxStamina = 1000;
	i16 iMaxMana = 100;
	i16 iXP = 1;
	if (xChampion == NULL)
		return;

	xChampion->HeroType(1);
	xChampion->handCommand[1] = xChampion->handCommand[0] = 0xFF;
	xChampion->timerIndex = TIMER_NONE;
	xChampion->playerDir(0);

	for (iInventoryIndex = 0; iInventoryIndex < INVENTORY_MAX_SLOT; iInventoryIndex++)
		xChampion->Possess(iInventoryIndex, OBJECT_NULL);

	sprintf((char*)xChampion->firstName, "%s", "Champ");
	sprintf((char*)xChampion->lastName, "%s", "Hero");

	xChampion->maxHP(iMaxHP);
	xChampion->curHP(iMaxHP);
	xChampion->maxStamina(iMaxStamina);
	xChampion->curStamina(iMaxStamina);
	xChampion->maxMP(iMaxMana);
	xChampion->curMP(iMaxMana);
	for (iAttributeIndex = 0; iAttributeIndex <= 6; iAttributeIndex++) {
		xChampion->attributes[iAttributeIndex][ATTRIBUTE_MAX] = xChampion->attributes[iAttributeIndex][ATTRIBUTE_CURRENT] = U8(max_value(30, 100));
		xChampion->attributesEnhanced[iAttributeIndex] = 0;
	}
	for (iAttributeIndex = 4; iAttributeIndex <= 19; iAttributeIndex++) {
		iXP = 3;
		xChampion->skills[iAttributeIndex] = (iXP != 0) ? (U32(0x40) << iXP) : 0;
	}
	//^2F3F:02B9
	for (iAttributeIndex = 0; iAttributeIndex <= 3; iAttributeIndex++) {
		X32 bp14 = 0;
		iXP = (iAttributeIndex +1) << 2;
		U16 si;
		for (si = 0; si < 4; si++) {
			bp14 += xChampion->skills[iXP +si];
		}
		xChampion->skills[iAttributeIndex] = bp14;
	}
	xChampion->curFood((RAND() & 255) + START_BASE_FOOD);
	xChampion->curWater((RAND() & 255) + START_BASE_WATER);

}


//==============================================================================
