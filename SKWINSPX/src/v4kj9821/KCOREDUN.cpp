//--- SkWinCore-Dun -------------------------------------
//SPX: This part of code contains:
// - Dungeon Basic Manupilation

#include <StdAfx.h>	// Required for MVC6 compilation

#include <SkVer.h>

#include <SkParam.h>
#include <SkDebug.h>
#include <SkGlobal.h>
#include <KCORE.H>
#include <SkMIDI.h>
#include <SkLUA.h>

#include <stdlib.h> // rand

//--- Common part with A.cpp
using namespace DMEncyclopaedia;
using namespace DM2Internal;
using namespace kkBitBlt;

#if defined(_USE_MFC80) || defined(_USE_MFC60)
#include <SkMFC.h>
#include <resource.h>
#endif // defined(_USE_MFC80) || defined(_USE_MFC60)


#ifdef _USE_SDL
#include <SkwinSDL.h>
#endif // _USE_SDL
#if defined(__DJGPP__) || defined (__MINGW__) || defined (__LINUX__)
#include <SkDOS.h>
#include <stdlib.h> // rand note: putting stdlib here and not right after stdafx prevents a bunch of conflicts with min/max macros
#endif // __DJGPP__


#include <KAITABLE.h>	// for dAITableGenuine (hard coded AI table)
//--- Common part with A.cpp

//==============================================================================
//------ For Extended Time ------ (not sure to be 100% compatible)
#include <sys/types.h>
#include <sys/timeb.h>
#include <time.h>

bool SkWinCore::INIT_RANDOM()
{
	struct timeb t;
	int iSeed = 0;
	unsigned long seconds;
	unsigned long millis;
	ftime(&t);
	millis = t.millitm;
	seconds = t.time;
	iSeed = ((millis*seconds)%16+1);
	srand(iSeed*((seconds+1)/4)*((millis+1)*3));
	return true;
}

//==============================================================================

U16 SkWinCore::DUNGEON_SET_DOOR_GFX(int iGfxID, int iMapIndex)
{
	U16 iGfxIndex = 0;
	U8* xGfxDataTab = NULL;
	U16 iLocalCurrentMapIndex = glbCurrentMapIndex;
	int iSearchIndex = 0;
	U8 iCurrentGfxID = -1;
	
	CHANGE_CURRENT_MAP_TO(iMapIndex);
	xGfxDataTab = &glbCurrentTileMap[glbCurrentMapWidth -1][glbCurrentMapHeight + dunMapLocalHeader->CreaturesTypes() + dunMapLocalHeader->WallGraphics() + dunMapLocalHeader->FloorGraphics()];

	iCurrentGfxID = *xGfxDataTab;
	while (iSearchIndex < 15 && iGfxID != iCurrentGfxID)
	{
		if (iCurrentGfxID == 0xFF)	// then it is not yet used
		{
			*xGfxDataTab = iGfxID;
			iCurrentGfxID = iGfxID;
			iGfxIndex = iSearchIndex;
			break;
		}
		iSearchIndex++;
		iCurrentGfxID = *(xGfxDataTab++);
	}

	CHANGE_CURRENT_MAP_TO(iLocalCurrentMapIndex);

	return iGfxIndex;
}

U16 SkWinCore::DUNGEON_SET_FLOOR_RANDOM_LIST_SIZE(int iRandomGfxListSize, int iMapIndex)
{
	U16 iListSize = 0;
	U16 iLocalCurrentMapIndex = glbCurrentMapIndex;
	Map_definition_info* xMapDef = NULL;

	CHANGE_CURRENT_MAP_TO(iMapIndex);
	xMapDef = (Map_definition_info*) dunMapLocalHeader;
	xMapDef->rFloors = iRandomGfxListSize;
	CHANGE_CURRENT_MAP_TO(iLocalCurrentMapIndex);

	return iListSize;
}

U16 SkWinCore::DUNGEON_SET_FLOOR_GFX(int iGfxID, int iMapIndex)
{
	U16 iGfxIndex = 0;
	U8* xGfxDataTab = NULL;
	U16 iLocalCurrentMapIndex = glbCurrentMapIndex;
	int iSearchIndex = 0;
	U8 iCurrentGfxID = -1;
	
	CHANGE_CURRENT_MAP_TO(iMapIndex);
	xGfxDataTab = &glbCurrentTileMap[glbCurrentMapWidth -1][glbCurrentMapHeight + dunMapLocalHeader->CreaturesTypes() + dunMapLocalHeader->WallGraphics()];

	iCurrentGfxID = *xGfxDataTab;
	while (iSearchIndex < 15 && iGfxID != iCurrentGfxID)
	{
		if (iCurrentGfxID == 0xFF)	// then it is not yet used
		{
			*xGfxDataTab = iGfxID;
			iCurrentGfxID = iGfxID;
			iGfxIndex = iSearchIndex;
			break;
		}
		iSearchIndex++;
		iCurrentGfxID = *(xGfxDataTab++);
	}

	CHANGE_CURRENT_MAP_TO(iLocalCurrentMapIndex);

	return iGfxIndex;
}

U16 SkWinCore::DUNGEON_SET_WALL_GFX(int iGfxID, int iMapIndex)
{
	U16 iGfxIndex = 0;
	U8* xGfxDataTab = NULL;
	U16 iLocalCurrentMapIndex = glbCurrentMapIndex;
	int iSearchIndex = 0;
	U8 iCurrentGfxID = -1;
	
	CHANGE_CURRENT_MAP_TO(iMapIndex);
	xGfxDataTab = &glbCurrentTileMap[glbCurrentMapWidth -1][glbCurrentMapHeight + dunMapLocalHeader->CreaturesTypes()];

	iCurrentGfxID = *xGfxDataTab;
	while (iSearchIndex < 15 && iGfxID != iCurrentGfxID)
	{
		if (iCurrentGfxID == 0xFF)	// then it is not yet used
		{
			*xGfxDataTab = iGfxID;
			iCurrentGfxID = iGfxID;
			iGfxIndex = iSearchIndex;
			break;
		}
		iSearchIndex++;
		iCurrentGfxID = *(xGfxDataTab++);
	}

	CHANGE_CURRENT_MAP_TO(iLocalCurrentMapIndex);

	return iGfxIndex;
}

//==============================================================================

void SkWinCore::DUNGEON_SET_TILESET(int iTilesetType, int iMapIndex)
{
	Map_definition_info *xCurrentMap = NULL;

	xCurrentMap = (Map_definition_info*) &dunMapsHeaders[iMapIndex];
	xCurrentMap->tileset = iTilesetType;

	// if ingame, force reload gfx
	if (glbTimersMaximumCount > 0 && iMapIndex == glbPlayerMap)
	{
		CHANGE_CURRENT_MAP_TO(glbPlayerMap);
		MOVE_RECORD_TO(OBJECT_NULL, glbPlayerPosX, glbPlayerPosY, -1, 0);
		LOAD_NEWMAP(iMapIndex);
		MOVE_RECORD_TO(OBJECT_NULL, -1, 0, glbPlayerPosX, glbPlayerPosY);
	}

}

//==============================================================================
extern ObjectID* globalGroundObjects;


ObjectID SkWinCore::DUNGEON_GET_TILE_RECORD(int iMapIndex, int iPosX, int iPosY)
{
	ObjectID oGroundObject = OBJECT_END_MARKER;
	int z = 0;
	z = iMapIndex*1024 + iPosY*32 + iPosX;

	oGroundObject = globalGroundObjects[z];
/*
	CHANGE_CURRENT_MAP_TO(iMapIndex);
	oGroundObject = GET_TILE_RECORD_LINK(iPosX, iPosY);
	CHANGE_CURRENT_MAP_TO(glbPlayerMap);
*/
	return oGroundObject;
}

Door* SkWinCore::DUNGEON_GET_DOOR_AT_POSITION(int iMapIndex, int iPosX, int iPosY)
{
	Door* xDoor = NULL;
	ObjectID oCurrentObject = OBJECT_END_MARKER;

	oCurrentObject = DUNGEON_GET_TILE_RECORD(iMapIndex, iPosX, iPosY);

	while (oCurrentObject != OBJECT_NULL && oCurrentObject != OBJECT_END_MARKER)
	{
		if (oCurrentObject.RealDBType() == dbDoor)
		{
			xDoor = GET_ADDRESS_OF_RECORD0(oCurrentObject);
			return xDoor;
		}
		oCurrentObject = GET_NEXT_RECORD_LINK(oCurrentObject);
	}
	return xDoor;
}



//==============================================================================

U8 SkWinCore::DUNGEON_GET_TILE_TYPE(int iMapIndex, int iPosX, int iPosY)
{
	U8 ttype = 0;

	if (iPosX < 0 || iPosY < 0 || iMapIndex < 0 || iPosX >= 32 || iPosY >= 32)
		return ttWall;
	
	ttype = (glbMapTileValue[iMapIndex][iPosX][iPosY] & 0xE0)>>5;
	return ttype;
}

int SkWinCore::DUNGEON_CHECK_IS_TILE_WALL(int iMapIndex, int iPosX, int iPosY)
{
	int bIsWall = 0;
	U8 ttype = 0;

	ttype = DUNGEON_GET_TILE_TYPE(iMapIndex, iPosX, iPosY);
	if (ttype == ttWall)
		bIsWall = 1;

	return bIsWall;
}

void SkWinCore::DUNGEON_SET_TILE(int iTileType, int iExtValue, int iMapIndex, int iPosX, int iPosY)
{
	U8 tile = 0;
	int iHasObject = 0;
	// Control dims

	if (iPosX < 0 || iPosY < 0 || iMapIndex < 0)
		return;

	tile = glbMapTileValue[iMapIndex][iPosX][iPosY];
	iHasObject = (tile & 0x10);

	glbMapTileValue[iMapIndex][iPosX][iPosY] = (iTileType<<5) + iHasObject + (iExtValue & 0x0F);

	if (iTileType == ttStairs || iTileType == ttDoor) // check walls around to tells if it is N/S or W/E
	{
		int iWallsAroundWestEast = 0;
		int iWallsAroundNorthSouth = 0;

		if (DUNGEON_CHECK_IS_TILE_WALL(iMapIndex, iPosX-1, iPosY) && DUNGEON_CHECK_IS_TILE_WALL(iMapIndex, iPosX+1, iPosY))
			iWallsAroundWestEast = 1;
		else if (DUNGEON_CHECK_IS_TILE_WALL(iMapIndex, iPosX, iPosY-1) && DUNGEON_CHECK_IS_TILE_WALL(iMapIndex, iPosX, iPosY+1))
			iWallsAroundNorthSouth = 1;

		if (iWallsAroundWestEast == 1) // then tile here will be N/S direction
		{
			DUNGEON_SET_TILE_FLAG(TILE_FLAG_IS_NORTH_SOUTH, iMapIndex, iPosX, iPosY);
		}
		else if (iWallsAroundNorthSouth == 1) // then tile here will be W/E direction
		{
			DUNGEON_SET_TILE_FLAG(TILE_FLAG_IS_WEST_EAST, iMapIndex, iPosX, iPosY);
		}
	}
}

void SkWinCore::DUNGEON_SET_TILE_FLAG_RAWVALUE(int iTileFlagValue, int iMapIndex, int iPosX, int iPosY)
{
	glbMapTileValue[iMapIndex][iPosX][iPosY] |= iTileFlagValue;
}

void SkWinCore::DUNGEON_SET_TILE_FLAG(int iTileFlag, int iMapIndex, int iPosX, int iPosY)
{
	U8 xTile = 0;
	U8 iTileType = 0;

	if (iPosX < 0 || iPosY < 0 || iMapIndex < 0)
		return;


	xTile = glbMapTileValue[iMapIndex][iPosX][iPosY];
	iTileType = (xTile & 0xE0)>>5;

	switch (iTileFlag)
	{
		case TILE_FLAG_HAS_OBJECT:
			glbMapTileValue[iMapIndex][iPosX][iPosY] |= 0x10;
			break;
	}

	// PIT : OH?I
	//	[8] Open (0 = closed) / [4] Hidden (invisible) / [2] ? / [1] Imaginary (false)
	
	// TELEPORTER : OH??
	//	[8] Open (0 = closed) / [4] Haze (visible) / [2] ? / [1] ?

	// STAIRS : DL??
	//	[8] Direction (0 = W/E) / [4] Lead (0 = down) / [2] ? / [1] ?

	// DOOR : DOSS
	//	[8] Direction (0 = W/E) / [4] Open (0 = closed) / [2] Door state / [1] Door state

	// pit
	if (iTileType == ttPit && iTileFlag == TILE_FLAG_IS_OPEN)
		glbMapTileValue[iMapIndex][iPosX][iPosY] |= 0x08;
	else if (iTileType == ttPit && iTileFlag == TILE_FLAG_IS_CLOSED)
		glbMapTileValue[iMapIndex][iPosX][iPosY] &= 0xF7;

	// door
	else if (iTileType == ttDoor && iTileFlag == TILE_FLAG_IS_CLOSED)
		glbMapTileValue[iMapIndex][iPosX][iPosY] |= 0x04;
	else if (iTileType == ttDoor && iTileFlag == TILE_FLAG_IS_NORTH_SOUTH)
		glbMapTileValue[iMapIndex][iPosX][iPosY] |= 0x08;
	else if (iTileType == ttDoor && iTileFlag == TILE_FLAG_IS_WEST_EAST)
		glbMapTileValue[iMapIndex][iPosX][iPosY] &= 0xF7;

	// stairs
	else if (iTileType == ttStairs && iTileFlag == TILE_FLAG_IS_NORTH_SOUTH)
		glbMapTileValue[iMapIndex][iPosX][iPosY] |= 0x08;
	else if (iTileType == ttStairs && iTileFlag == TILE_FLAG_IS_WEST_EAST)
		glbMapTileValue[iMapIndex][iPosX][iPosY] &= 0xF7;

	else if (iTileType == ttStairs && iTileFlag == TILE_FLAG_IS_LEADING_UP)
		glbMapTileValue[iMapIndex][iPosX][iPosY] |= 0x04;
	else if (iTileType == ttStairs && iTileFlag == TILE_FLAG_IS_LEADING_DOWN)
		glbMapTileValue[iMapIndex][iPosX][iPosY] &= 0xFB;

	// teleporter
	else if (iTileType == ttTeleporter && iTileFlag == TILE_FLAG_IS_OPEN)
		glbMapTileValue[iMapIndex][iPosX][iPosY] |= 0x08;
	else if (iTileType == ttTeleporter && iTileFlag == TILE_FLAG_IS_CLOSED)
		glbMapTileValue[iMapIndex][iPosX][iPosY] &= 0xF7;

	else if (iTileType == ttTeleporter && iTileFlag == TILE_FLAG_HAS_HAZE)
		glbMapTileValue[iMapIndex][iPosX][iPosY] |= 0x04;
	else if (iTileType == ttTeleporter && iTileFlag == TILE_FLAG_NO_HAZE)
		glbMapTileValue[iMapIndex][iPosX][iPosY] &= 0xFB;

}


//==============================================================================


Door* SkWinCore::GET_AVAILABLE_ADDRESS_OF_RECORD0(ObjectID* recordLink)
{ 
	Door* xDoor = NULL;
	static int iNextSearchID = 0;
	ObjectID searchRecordLink = OBJECT_NULL;
	bool bNotFoundYet = true;
	
	while (bNotFoundYet)
	{
		searchRecordLink = ObjectID(0, dbDoor, iNextSearchID);
		xDoor = GET_ADDRESS_OF_RECORD(searchRecordLink)->castToDoor();
		if (xDoor != NULL)
		{
			if (xDoor->next == OBJECT_NULL)
				bNotFoundYet = false;
			else
				iNextSearchID++;
		}
		else
			iNextSearchID++;
		if (iNextSearchID >= 1024)
		{
			bNotFoundYet = true;
			return NULL;
		}
	}
	iNextSearchID++;
	((ObjectID_Reference*)recordLink)->iID = ((ObjectID_Reference*)&searchRecordLink)->iID;
	((ObjectID_Reference*)recordLink)->iDBType = ((ObjectID_Reference*)&searchRecordLink)->iDBType;
	((ObjectID_Reference*)recordLink)->iDir = ((ObjectID_Reference*)&searchRecordLink)->iDir;
	return xDoor;
}


Teleporter* SkWinCore::GET_AVAILABLE_ADDRESS_OF_RECORD1(ObjectID* recordLink)
{ 
	Teleporter* xTeleporter = NULL;
	static int iNextSearchID = 0;
	ObjectID searchRecordLink = OBJECT_NULL;
	bool bNotFoundYet = true;
	
	while (bNotFoundYet)
	{
		searchRecordLink = ObjectID(0, dbTeleporter, iNextSearchID);
		xTeleporter = GET_ADDRESS_OF_RECORD(searchRecordLink)->castToTeleporter();
		if (xTeleporter != NULL)
		{
			if (xTeleporter->w0 == OBJECT_NULL)
				bNotFoundYet = false;
			else
				iNextSearchID++;
		}
		else
			iNextSearchID++;
		if (iNextSearchID >= 1024)
		{
			bNotFoundYet = true;
			return NULL;
		}
	}
	iNextSearchID++;
	((ObjectID_Reference*)recordLink)->iID = ((ObjectID_Reference*)&searchRecordLink)->iID;
	((ObjectID_Reference*)recordLink)->iDBType = ((ObjectID_Reference*)&searchRecordLink)->iDBType;
	((ObjectID_Reference*)recordLink)->iDir = ((ObjectID_Reference*)&searchRecordLink)->iDir;
	return xTeleporter;
}

Text *SkWinCore::GET_AVAILABLE_ADDRESS_OF_RECORD2(ObjectID* recordLink)
{ 
	Text* xText = NULL;
	static int iNextSearchID = 0;
	ObjectID searchRecordLink = OBJECT_NULL;
	bool bNotFoundYet = true;
	
	while (bNotFoundYet)
	{
		searchRecordLink = ObjectID(0, dbText, iNextSearchID);
		xText = GET_ADDRESS_OF_RECORD(searchRecordLink)->castToText();
		if (xText != NULL)
		{
			if (xText->w0 == OBJECT_NULL)
				bNotFoundYet = false;
			else
				iNextSearchID++;
		}
		else
			iNextSearchID++;
		if (iNextSearchID >= 1024)
		{
			bNotFoundYet = true;
			return NULL;
		}
	}
	iNextSearchID++;
	((ObjectID_Reference*)recordLink)->iID = ((ObjectID_Reference*)&searchRecordLink)->iID;
	((ObjectID_Reference*)recordLink)->iDBType = ((ObjectID_Reference*)&searchRecordLink)->iDBType;
	((ObjectID_Reference*)recordLink)->iDir = ((ObjectID_Reference*)&searchRecordLink)->iDir;
	return xText;
}

SimpleActuator *SkWinCore::GET_AVAILABLE_ADDRESS_OF_RECORD2X(ObjectID* recordLink)
{ 
	SimpleActuator* xSimpleAct = NULL;
	static int iNextSearchID = 0;
	ObjectID searchRecordLink = OBJECT_NULL;
	bool bNotFoundYet = true;
	
	while (bNotFoundYet)
	{
		searchRecordLink = ObjectID(0, dbText, iNextSearchID);
		xSimpleAct = (SimpleActuator*) GET_ADDRESS_OF_RECORD(searchRecordLink)->castToText();
		if (xSimpleAct != NULL)
		{
			if (xSimpleAct->w0 == OBJECT_NULL)
				bNotFoundYet = false;
			else
				iNextSearchID++;
		}
		else
			iNextSearchID++;
		if (iNextSearchID >= 1024)
		{
			bNotFoundYet = true;
			return NULL;
		}
	}
	iNextSearchID++;
	((ObjectID_Reference*)recordLink)->iID = ((ObjectID_Reference*)&searchRecordLink)->iID;
	((ObjectID_Reference*)recordLink)->iDBType = ((ObjectID_Reference*)&searchRecordLink)->iDBType;
	((ObjectID_Reference*)recordLink)->iDir = ((ObjectID_Reference*)&searchRecordLink)->iDir;
	return xSimpleAct;
}

Actuator *SkWinCore::GET_AVAILABLE_ADDRESS_OF_RECORD3(ObjectID* recordLink)
{ 
	Actuator* xActuator = NULL;
	static int iNextSearchID = 0;
	ObjectID searchRecordLink = OBJECT_NULL;
	bool bNotFoundYet = true;
	
	while (bNotFoundYet)
	{
		searchRecordLink = ObjectID(0, dbActuator, iNextSearchID);
		xActuator = GET_ADDRESS_OF_RECORD(searchRecordLink)->castToActuator();
		if (xActuator != NULL)
		{
			if (xActuator->w0 == OBJECT_NULL)
				bNotFoundYet = false;
			else
				iNextSearchID++;
		}
		else
			iNextSearchID++;
		if (iNextSearchID >= 1024)
		{
			bNotFoundYet = true;
			return NULL;
		}
	}
	iNextSearchID++;
	((ObjectID_Reference*)recordLink)->iID = ((ObjectID_Reference*)&searchRecordLink)->iID;
	((ObjectID_Reference*)recordLink)->iDBType = ((ObjectID_Reference*)&searchRecordLink)->iDBType;
	((ObjectID_Reference*)recordLink)->iDir = ((ObjectID_Reference*)&searchRecordLink)->iDir;
	return xActuator;
}

Creature *SkWinCore::GET_AVAILABLE_ADDRESS_OF_RECORD4(ObjectID* recordLink)
{ 
	Creature* xCreature = NULL;
	static int iNextSearchID = 0;
	ObjectID searchRecordLink = OBJECT_NULL;
	bool bNotFoundYet = true;
	
	while (bNotFoundYet)
	{
		searchRecordLink = ObjectID(0, dbCreature, iNextSearchID);
		xCreature = GET_ADDRESS_OF_RECORD(searchRecordLink)->castToCreature();
		if (xCreature != NULL)
		{
			if (xCreature->w0 == OBJECT_NULL)
				bNotFoundYet = false;
			else
				iNextSearchID++;
		}
		else
			iNextSearchID++;
		if (iNextSearchID >= 1024)
		{
			bNotFoundYet = true;
			return NULL;
		}
	}
	iNextSearchID++;
	((ObjectID_Reference*)recordLink)->iID = ((ObjectID_Reference*)&searchRecordLink)->iID;
	((ObjectID_Reference*)recordLink)->iDBType = ((ObjectID_Reference*)&searchRecordLink)->iDBType;
	((ObjectID_Reference*)recordLink)->iDir = ((ObjectID_Reference*)&searchRecordLink)->iDir;
	return xCreature;
}

Weapon *SkWinCore::GET_AVAILABLE_ADDRESS_OF_RECORD5(ObjectID* recordLink)
{ 
	Weapon* xWeapon = NULL;
	static int iNextSearchID = 0;
	ObjectID searchRecordLink = OBJECT_NULL;
	bool bNotFoundYet = true;
	
	while (bNotFoundYet)
	{
		searchRecordLink = ObjectID(0, dbWeapon, iNextSearchID);
		//printf("WEAP: %d\n", iNextSearchID);
		xWeapon = GET_ADDRESS_OF_RECORD(searchRecordLink)->castToWeapon();
		if (xWeapon != NULL)
		{
			if (xWeapon->w0 == OBJECT_NULL)
				bNotFoundYet = false;
			else
				iNextSearchID++;
		}
		else
			iNextSearchID++;
		if (iNextSearchID >= 1024)
		{
			bNotFoundYet = true;
			return NULL;
		}
	}
	iNextSearchID++;
	((ObjectID_Reference*)recordLink)->iID = ((ObjectID_Reference*)&searchRecordLink)->iID;
	((ObjectID_Reference*)recordLink)->iDBType = ((ObjectID_Reference*)&searchRecordLink)->iDBType;
	((ObjectID_Reference*)recordLink)->iDir = ((ObjectID_Reference*)&searchRecordLink)->iDir;
	return xWeapon;
}


Cloth *SkWinCore::GET_AVAILABLE_ADDRESS_OF_RECORD6(ObjectID* recordLink)
{ 
	Cloth* xClothing = NULL;
	static int iNextSearchID = 0;
	ObjectID searchRecordLink = OBJECT_NULL;
	bool bNotFoundYet = true;
	
	while (bNotFoundYet)
	{
		searchRecordLink = ObjectID(0, dbCloth, iNextSearchID);
		xClothing = GET_ADDRESS_OF_RECORD(searchRecordLink)->castToCloth();
		if (xClothing != NULL)
		{
			if (xClothing->w0 == OBJECT_NULL)
				bNotFoundYet = false;
			else
				iNextSearchID++;
		}
		else
			iNextSearchID++;
		if (iNextSearchID >= 1024)
		{
			bNotFoundYet = true;
			return NULL;
		}
	}
	iNextSearchID++;
	((ObjectID_Reference*)recordLink)->iID = ((ObjectID_Reference*)&searchRecordLink)->iID;
	((ObjectID_Reference*)recordLink)->iDBType = ((ObjectID_Reference*)&searchRecordLink)->iDBType;
	((ObjectID_Reference*)recordLink)->iDir = ((ObjectID_Reference*)&searchRecordLink)->iDir;
	return xClothing;
}

Scroll *SkWinCore::GET_AVAILABLE_ADDRESS_OF_RECORD7(ObjectID* recordLink)
{ 
	Scroll* xScroll = NULL;
	static int iNextSearchID = 0;
	ObjectID searchRecordLink = OBJECT_NULL;
	bool bNotFoundYet = true;
	
	while (bNotFoundYet)
	{
		searchRecordLink = ObjectID(0, dbScroll, iNextSearchID);
		xScroll = GET_ADDRESS_OF_RECORD(searchRecordLink)->castToScroll();
		if (xScroll != NULL)
		{
			if (xScroll->w0 == OBJECT_NULL)
				bNotFoundYet = false;
			else
				iNextSearchID++;
		}
		else
			iNextSearchID++;
		if (iNextSearchID >= 1024)
		{
			bNotFoundYet = true;
			return NULL;
		}
	}
	iNextSearchID++;
	((ObjectID_Reference*)recordLink)->iID = ((ObjectID_Reference*)&searchRecordLink)->iID;
	((ObjectID_Reference*)recordLink)->iDBType = ((ObjectID_Reference*)&searchRecordLink)->iDBType;
	((ObjectID_Reference*)recordLink)->iDir = ((ObjectID_Reference*)&searchRecordLink)->iDir;
	return xScroll;
}

Potion *SkWinCore::GET_AVAILABLE_ADDRESS_OF_RECORD8(ObjectID* recordLink)
{ 
	Potion* xPotion = NULL;
	static int iNextSearchID = 0;
	ObjectID searchRecordLink = OBJECT_NULL;
	bool bNotFoundYet = true;
	
	while (bNotFoundYet)
	{
		searchRecordLink = ObjectID(0, dbPotion, iNextSearchID);
		xPotion = GET_ADDRESS_OF_RECORD(searchRecordLink)->castToPotion();
		if (xPotion != NULL)
		{
			if (xPotion->w0 == OBJECT_NULL)
				bNotFoundYet = false;
			else
				iNextSearchID++;
		}
		else
			iNextSearchID++;
		if (iNextSearchID >= 1024)
		{
			bNotFoundYet = true;
			return NULL;
		}
	}
	iNextSearchID++;
	((ObjectID_Reference*)recordLink)->iID = ((ObjectID_Reference*)&searchRecordLink)->iID;
	((ObjectID_Reference*)recordLink)->iDBType = ((ObjectID_Reference*)&searchRecordLink)->iDBType;
	((ObjectID_Reference*)recordLink)->iDir = ((ObjectID_Reference*)&searchRecordLink)->iDir;
	return xPotion;
}

Container *SkWinCore::GET_AVAILABLE_ADDRESS_OF_RECORD9(ObjectID* recordLink)
{ 
	Container* xContainer = NULL;
	static int iNextSearchID = 0;
	ObjectID searchRecordLink = OBJECT_NULL;
	bool bNotFoundYet = true;
	
	while (bNotFoundYet)
	{
		searchRecordLink = ObjectID(0, dbContainer, iNextSearchID);
		xContainer = GET_ADDRESS_OF_RECORD(searchRecordLink)->castToContainer();
		if (xContainer != NULL)
		{
			if (xContainer->w0 == OBJECT_NULL)
				bNotFoundYet = false;
			else
				iNextSearchID++;
		}
		else
			iNextSearchID++;
		if (iNextSearchID >= 1024)
		{
			bNotFoundYet = true;
			return NULL;
		}
	}
	iNextSearchID++;
	((ObjectID_Reference*)recordLink)->iID = ((ObjectID_Reference*)&searchRecordLink)->iID;
	((ObjectID_Reference*)recordLink)->iDBType = ((ObjectID_Reference*)&searchRecordLink)->iDBType;
	((ObjectID_Reference*)recordLink)->iDir = ((ObjectID_Reference*)&searchRecordLink)->iDir;
	return xContainer;
}

Miscellaneous_item *SkWinCore::GET_AVAILABLE_ADDRESS_OF_RECORDA(ObjectID* recordLink)
{ 
	Miscellaneous_item* xMisc = NULL;
	static int iNextSearchID = 0;
	ObjectID searchRecordLink = OBJECT_NULL;
	bool bNotFoundYet = true;
	
	while (bNotFoundYet)
	{
		searchRecordLink = ObjectID(0, dbMiscellaneous_item, iNextSearchID);
		//printf("MISC: %d\n", iNextSearchID);
		xMisc = GET_ADDRESS_OF_RECORD(searchRecordLink)->castToMisc();
		if (xMisc != NULL)
		{
			if (xMisc->w0 == OBJECT_NULL)
				bNotFoundYet = false;
			else
				iNextSearchID++;
		}
		else
			iNextSearchID++;
		if (iNextSearchID >= 1024)
		{
			bNotFoundYet = true;
			return NULL;
		}
	}
	iNextSearchID++;
	((ObjectID_Reference*)recordLink)->iID = ((ObjectID_Reference*)&searchRecordLink)->iID;
	((ObjectID_Reference*)recordLink)->iDBType = ((ObjectID_Reference*)&searchRecordLink)->iDBType;
	((ObjectID_Reference*)recordLink)->iDir = ((ObjectID_Reference*)&searchRecordLink)->iDir;
	return xMisc;
}



//==============================================================================

void SkWinCore::DUNGEON_PUT_NEW_CREATURE(int iCreatureType, int iMapIndex, int iPosX, int iPosY)
{
	ObjectID oNewCreature = OBJECT_NULL;
//	ObjectID_Reference oNewCreatureRef;
	U16 iHealthMultiplier = 7;
	U16 iDirection = rand()%4;
	U16 iLocalCurrentMapIndex = glbCurrentMapIndex;
	Creature* xNewCreature = NULL;
	int z = 0;


	//-- This method works if timers are already allocated = game was initialized (else MOVE_RECORD fails to ERROR 45)
	if (glbTimersCount >= glbTimersMaximumCount || glbTimersMaximumCount == 0)
	{
		z = iMapIndex*1024 + iPosY*32 + iPosX;
		//oNewCreature = DUNGEON_GET_AVAILABLE_CREATURE(iCreatureType, iHealthMultiplier, iDirection, iPosX, iPosY);
		oNewCreature = ObjectID(0, dbCreature, 0);
		xNewCreature = GET_AVAILABLE_ADDRESS_OF_RECORD4(&oNewCreature);
		if (xNewCreature != NULL)
		{
			xNewCreature->w0 = OBJECT_END_MARKER;
			xNewCreature->possession = OBJECT_END_MARKER;

			xNewCreature->b4 = iCreatureType;
			xNewCreature->b5 = 0;	// pos
			xNewCreature->hp1 = 100;
			xNewCreature->iAnimSeq = 0;
			xNewCreature->iAnimFrame = 0;
			xNewCreature->w12 = 0;
			xNewCreature->b14 = 0;
			xNewCreature->b15 = 0;


			//globalGroundObjects[z] = oNewCreature;
			DUNGEON_STACK_OBJECT(oNewCreature, iMapIndex, iPosX, iPosY);
			//DUNGEON_SET_TILE_FLAG(TILE_FLAG_HAS_OBJECT, iMapIndex, iPosX, iPosY);
		}
	}
	else	// game is "started" / timers are active
	{
		CHANGE_CURRENT_MAP_TO(iMapIndex);
		oNewCreature = ALLOC_NEW_CREATURE(iCreatureType, iHealthMultiplier, iDirection, iPosX, iPosY);
		MOVE_RECORD_TO(ObjectID(oNewCreature, 0), -1, 0, iPosX, iPosY);
		CHANGE_CURRENT_MAP_TO(iLocalCurrentMapIndex);
	}
}



void SkWinCore::DUNGEON_PUT_NEW_DOOR(int iDoorType, int iMapIndex, int iPosX, int iPosY)
{
	ObjectID oNewDoor = OBJECT_NULL;
	//Door* xDoor = NULL;
	Door_Info* xDoor = NULL;
	int z = 0;

	//if (glbTimersCount >= glbTimersMaximumCount || glbTimersMaximumCount == 0)
	{
		z = iMapIndex*1024 + iPosY*32 + iPosX;
		
		oNewDoor = ObjectID(0, dbDoor, 0);
		xDoor = (Door_Info*) GET_AVAILABLE_ADDRESS_OF_RECORD0(&oNewDoor);
		if (xDoor != NULL)
		{
			xDoor->next = OBJECT_END_MARKER;
			xDoor->button = 1;
			xDoor->type = iDoorType;
			xDoor->verticalOpening = 0; // metal door
			xDoor->destroyable = 0;
			xDoor->bashable = 0;
			xDoor->ornate = 0;
			xDoor->unused = 0;
			if (iDoorType == 1) // iron grate
				xDoor->verticalOpening = 1;
		}

		//-- Put reference on ground/stack
		if (xDoor != NULL)
		{
			DUNGEON_STACK_OBJECT(oNewDoor, iMapIndex, iPosX, iPosY);
		}
	}

	DUNGEON_SET_TILE_FLAG(TILE_FLAG_IS_CLOSED, iMapIndex, iPosX, iPosY);	// make the door closed
}


void SkWinCore::DUNGEON_PUT_NEW_TELEPORTER(int iMapIndex, int iPosX, int iPosY)
{
	ObjectID oNewTeleporter = OBJECT_NULL;
	//Door* xDoor = NULL;
	Teleporter* xTeleporter = NULL;
	int z = 0;

	if (glbTimersCount >= glbTimersMaximumCount || glbTimersMaximumCount == 0)
	{
		z = iMapIndex*1024 + iPosY*32 + iPosX;
		
		oNewTeleporter = ObjectID(0, dbTeleporter, 0);
		xTeleporter = (Teleporter*) GET_AVAILABLE_ADDRESS_OF_RECORD1(&oNewTeleporter);
		if (xTeleporter != NULL)
		{
			xTeleporter->w0 = OBJECT_END_MARKER;
			xTeleporter->DestinationMap(iMapIndex);
			xTeleporter->DestinationX(iPosX);
			xTeleporter->DestinationY(iPosY);
			xTeleporter->Scope(0);
			xTeleporter->Sound(1);
			xTeleporter->Rotation(0);
			xTeleporter->RotationType(0);
		}

		//-- Put reference on ground/stack
		if (xTeleporter != NULL)
		{
			DUNGEON_STACK_OBJECT(oNewTeleporter, iMapIndex, iPosX, iPosY);
			DUNGEON_SET_TILE_FLAG(TILE_FLAG_IS_OPEN, iMapIndex, iPosX, iPosY);
		}
	}

}


//==============================================================================

void SkWinCore::DUNGEON_STACK_OBJECT(ObjectID oNewObject, int iMapIndex, int iPosX, int iPosY)
{
	ObjectID xGroundObject = OBJECT_NULL;
	ObjectID xCurrentObject = OBJECT_NULL;
	ObjectID xNextObject = OBJECT_NULL;
	int z = 0;
	z = iMapIndex*1024 + iPosY*32 + iPosX;

	xGroundObject = globalGroundObjects[z];

	if (xGroundObject == OBJECT_NULL || xGroundObject == OBJECT_END_MARKER)
	{
		globalGroundObjects[z] = oNewObject;
		DUNGEON_SET_TILE_FLAG(TILE_FLAG_HAS_OBJECT, iMapIndex, iPosX, iPosY);
	}
	else
	{
		xCurrentObject = xGroundObject;
		xNextObject = xCurrentObject;
		// need to stack on objects
		while (xNextObject != OBJECT_END_MARKER && xNextObject != OBJECT_NULL)
		{
			xCurrentObject = xNextObject;
			xNextObject = GET_NEXT_RECORD_LINK(xCurrentObject);
		}
		if (xNextObject == OBJECT_END_MARKER)
			GET_ADDRESS_OF_RECORD(xCurrentObject)->w0 = oNewObject;
	}
}

//==============================================================================

void SkWinCore::DUNGEON_PUT_NEW_ITEM_INGAME(int iAbsoluteItemID, int iMapIndex, int iPosX, int iPosY)
{
	ObjectID oNewItem = OBJECT_NULL;
	U16 iTileQuarter = 0;
	Weapon* xWeapon = NULL;
	int z = 0;
	
	U16 iLocalCurrentMapIndex = glbCurrentMapIndex;

	//-- This method works if timers are already allocated = game was initialized (else MOVE_RECORD fails to ERROR 45)
	if (glbTimersCount >= glbTimersMaximumCount || glbTimersMaximumCount == 0)
	{
		return;
	}
	else	// game is "started" / timers are active
	{
		CHANGE_CURRENT_MAP_TO(iMapIndex);

		oNewItem = ALLOC_NEW_DBITEM(iAbsoluteItemID);
		if (oNewItem == OBJECT_NULL)
			return;
		
		if (iPosX == glbPlayerPosX && iPosY == glbPlayerPosY)
			iTileQuarter = (glbPlayerDir + RAND01()) & 3;
		else
			iTileQuarter = RAND02();
		
		MOVE_RECORD_TO(ObjectID(oNewItem, iTileQuarter), -1, 0, iPosX, iPosY);
		
		CHANGE_CURRENT_MAP_TO(iLocalCurrentMapIndex);
	}

}

void SkWinCore::DUNGEON_PUT_NEW_ITEM(int iCategory, int iItemID, int iMapIndex, int iPosX, int iPosY)
{
	ObjectID oNewItem = OBJECT_NULL;
	U16 iTileQuarter = 0;
	Weapon* xWeapon = NULL;
	Cloth* xClothing = NULL;
	Scroll* xScroll = NULL;
	Potion* xPotion = NULL;
	Container* xContainer = NULL;
	Miscellaneous_item* xMisc = NULL;
	void* xItem = NULL;
	int z = 0;
	
	U16 iLocalCurrentMapIndex = glbCurrentMapIndex;

	//-- This method works if timers are already allocated = game was initialized (else MOVE_RECORD fails to ERROR 45)
	if (glbTimersCount >= glbTimersMaximumCount || glbTimersMaximumCount == 0)
	{
		z = iMapIndex*1024 + iPosY*32 + iPosX;
		
		oNewItem = ObjectID(0, dbWeapon, 0);
		//-- Get new data pointer
		if (iCategory == dbWeapon)
		{
			xWeapon = GET_AVAILABLE_ADDRESS_OF_RECORD5(&oNewItem);
			xItem = (void*) xWeapon;
		}
		else if (iCategory == dbCloth)
		{
			xClothing = GET_AVAILABLE_ADDRESS_OF_RECORD6(&oNewItem);
			xItem = (void*) xClothing;
		}
		else if (iCategory == dbScroll)
		{
			xScroll = GET_AVAILABLE_ADDRESS_OF_RECORD7(&oNewItem);
			xItem = (void*) xScroll;
		}
		else if (iCategory == dbPotion)
		{
			xPotion = GET_AVAILABLE_ADDRESS_OF_RECORD8(&oNewItem);
			xItem = (void*) xPotion;
		}
		else if (iCategory == dbContainer)
		{
			xContainer = GET_AVAILABLE_ADDRESS_OF_RECORD9(&oNewItem);
			xItem = (void*) xContainer;
		}
		else if (iCategory == dbMiscellaneous_item)
		{
			xMisc = GET_AVAILABLE_ADDRESS_OF_RECORDA(&oNewItem);
			xItem = (void*) xMisc;
		}
		
		//-- Init item data
		if (xWeapon != NULL)
		{
			xWeapon->w0 = OBJECT_END_MARKER;
			xWeapon->ItemType(iItemID);
			xWeapon->Charges(0);
			xWeapon->w2 |= 0x80;	// important bit
		}
		else if (xClothing != NULL)
		{
			xClothing->w0 = OBJECT_END_MARKER;
			xClothing->ItemType(iItemID);
			xClothing->w2 |= 0x80;	// important bit
		}
		else if (xScroll != NULL)
		{
			xScroll->w0 = OBJECT_END_MARKER;
			//xScroll->ItemType(iItemID);
		}
		else if (xPotion != NULL)
		{
			xPotion->w0 = OBJECT_END_MARKER;
			xPotion->PotionType(iItemID);
			xPotion->VisiblePower(0);
			xPotion->PotionPower(32);
		}
		else if (xContainer != NULL)
		{
			xContainer->w0 = OBJECT_END_MARKER;
			xContainer->w2 = OBJECT_END_MARKER;	// empty
			xContainer->ContainerType(iItemID);
		}
		else if (xMisc != NULL)
		{
			xMisc->w0 = OBJECT_END_MARKER;
			xMisc->w2 = 0;
			xMisc->ItemType(iItemID);
			xMisc->Charge(0);
			xMisc->w2 |= 0x80;	// important bit
		}

		//-- Put reference on ground/stack
		if (xItem != NULL)
		{
			//globalGroundObjects[z] = oNewItem;
			DUNGEON_STACK_OBJECT(oNewItem, iMapIndex, iPosX, iPosY);
			//DUNGEON_SET_TILE_FLAG(TILE_FLAG_HAS_OBJECT, iMapIndex, iPosX, iPosY);
		}
	}
	else	// game is "started" / timers are active
	{
		return;
	}

}



void SkWinCore::DUNGEON_PUT_NEW_WEAPON(int iWeaponType, int iMapIndex, int iPosX, int iPosY)
{
	if (glbTimersCount >= glbTimersMaximumCount || glbTimersMaximumCount == 0)
		DUNGEON_PUT_NEW_ITEM( dbWeapon, iWeaponType, iMapIndex, iPosX, iPosY);
	else
		DUNGEON_PUT_NEW_ITEM_INGAME( CREATE_ABSOLUTE_ACTIVATOR_ITEM_ID(dbWeapon, iWeaponType), iMapIndex, iPosX, iPosY);
}

void SkWinCore::DUNGEON_PUT_NEW_CLOTHING(int iClothingType, int iMapIndex, int iPosX, int iPosY)
{
	if (glbTimersCount >= glbTimersMaximumCount || glbTimersMaximumCount == 0)
		DUNGEON_PUT_NEW_ITEM( dbCloth, iClothingType, iMapIndex, iPosX, iPosY);
	else
		DUNGEON_PUT_NEW_ITEM_INGAME( CREATE_ABSOLUTE_ACTIVATOR_ITEM_ID(dbCloth, iClothingType), iMapIndex, iPosX, iPosY);
}

void SkWinCore::DUNGEON_PUT_NEW_MISC_ITEM(int iMiscType, int iMapIndex, int iPosX, int iPosY)
{
	if (glbTimersCount >= glbTimersMaximumCount || glbTimersMaximumCount == 0)
		DUNGEON_PUT_NEW_ITEM( dbMiscellaneous_item, iMiscType, iMapIndex, iPosX, iPosY);
	else
		DUNGEON_PUT_NEW_ITEM_INGAME( CREATE_ABSOLUTE_ACTIVATOR_ITEM_ID(dbMiscellaneous_item, iMiscType), iMapIndex, iPosX, iPosY);
}

void SkWinCore::DUNGEON_PUT_NEW_POTION(int iPotionType, int iMapIndex, int iPosX, int iPosY)
{
	if (glbTimersCount >= glbTimersMaximumCount || glbTimersMaximumCount == 0)
		DUNGEON_PUT_NEW_ITEM( dbPotion, iPotionType, iMapIndex, iPosX, iPosY);
	else
		DUNGEON_PUT_NEW_ITEM_INGAME( CREATE_ABSOLUTE_ACTIVATOR_ITEM_ID(dbPotion, iPotionType), iMapIndex, iPosX, iPosY);
}

void SkWinCore::DUNGEON_PUT_NEW_CONTAINER(int iContainerType, int iMapIndex, int iPosX, int iPosY)
{
	if (glbTimersCount >= glbTimersMaximumCount || glbTimersMaximumCount == 0)
		DUNGEON_PUT_NEW_ITEM( dbContainer, iContainerType, iMapIndex, iPosX, iPosY);
	else
		DUNGEON_PUT_NEW_ITEM_INGAME( CREATE_ABSOLUTE_ACTIVATOR_ITEM_ID(dbContainer, iContainerType), iMapIndex, iPosX, iPosY);
}

void SkWinCore::DUNGEON_PUT_NEW_SCROLL(int iScrollType, int iMessageID, int iMapIndex, int iPosX, int iPosY)
{
	if (glbTimersCount >= glbTimersMaximumCount || glbTimersMaximumCount == 0)
		DUNGEON_PUT_NEW_ITEM( dbScroll, iScrollType, iMapIndex, iPosX, iPosY);
	else
		DUNGEON_PUT_NEW_ITEM_INGAME( CREATE_ABSOLUTE_ACTIVATOR_ITEM_ID(dbScroll, iScrollType), iMapIndex, iPosX, iPosY);
}


//==============================================================================

void SkWinCore::DUNGEON_PUT_FLOOR_DECORATION(int iGfxType, int iMapIndex, int iPosX, int iPosY)
{
	ObjectID oNewObject = OBJECT_NULL;
	SimpleActuator* xSimpAct = NULL;
	U16 iTileQuarter = 0;
	int z = 0;

	U16 iLocalCurrentMapIndex = glbCurrentMapIndex;


	//-- This method works if timers are already allocated = game was initialized (else MOVE_RECORD fails to ERROR 45)
	if (glbTimersCount >= glbTimersMaximumCount || glbTimersMaximumCount == 0)
	{
		z = iMapIndex*1024 + iPosY*32 + iPosX;
		
		oNewObject = ObjectID(0, dbText, 0);
		xSimpAct = GET_AVAILABLE_ADDRESS_OF_RECORD2X(&oNewObject);
		if (xSimpAct != NULL)
		{
			xSimpAct->w0 = OBJECT_END_MARKER;

			xSimpAct->activated = 0;
			xSimpAct->mode = 1; // active decoration
			xSimpAct->graphism = iGfxType;
			xSimpAct->extendedUsage = 0; // 0 = still image

			DUNGEON_STACK_OBJECT(oNewObject, iMapIndex, iPosX, iPosY);
		}
	}
	else // Game started
	{
		CHANGE_CURRENT_MAP_TO(iMapIndex);

		oNewObject = ALLOC_NEW_RECORD(DB_CATEGORY_SIMPLE_ACTUATOR);
		xSimpAct = GET_ADDRESS_OF_RECORD2X(oNewObject);
		if (xSimpAct != NULL)
		{
			xSimpAct->activated = 0;
			xSimpAct->mode = 1; // active decoration
			xSimpAct->graphism = iGfxType;
			xSimpAct->extendedUsage = 0; // 0 = still image
			MOVE_RECORD_TO(ObjectID(oNewObject, iTileQuarter), -1, 0, iPosX, iPosY);
		}
		
		CHANGE_CURRENT_MAP_TO(iLocalCurrentMapIndex);
	}
}


Actuator* SkWinCore::DUNGEON_PUT_FLOOR_ACTUATOR(int iGfxType, int iMapIndex, int iPosX, int iPosY)
{
	ObjectID oNewObject = OBJECT_NULL;
	Actuator* xAct = NULL;
	U16 iTileQuarter = 0;
	int z = 0;

	U16 iLocalCurrentMapIndex = glbCurrentMapIndex;


	//-- This method works if timers are already allocated = game was initialized (else MOVE_RECORD fails to ERROR 45)
	if (glbTimersCount >= glbTimersMaximumCount || glbTimersMaximumCount == 0)
	{
		z = iMapIndex*1024 + iPosY*32 + iPosX;
		
		oNewObject = ObjectID(0, dbActuator, 0);
		xAct = GET_AVAILABLE_ADDRESS_OF_RECORD3(&oNewObject);
		if (xAct != NULL)
		{
			xAct->w0 = OBJECT_END_MARKER;

			xAct->w2 = xAct->w4 = xAct->w6 = 0;

			DUNGEON_STACK_OBJECT(oNewObject, iMapIndex, iPosX, iPosY);
		}
	}
	else // Game started
	{
		CHANGE_CURRENT_MAP_TO(iMapIndex);

		oNewObject = ALLOC_NEW_RECORD(DB_CATEGORY_ACTUATOR);
		xAct = GET_ADDRESS_OF_RECORD3(oNewObject);
		if (xAct != NULL)
		{
			xAct->w2 = xAct->w4 = xAct->w6 = 0;

			MOVE_RECORD_TO(ObjectID(oNewObject, iTileQuarter), -1, 0, iPosX, iPosY);
		}
		
		CHANGE_CURRENT_MAP_TO(iLocalCurrentMapIndex);
	}
	return xAct;
}

void SkWinCore::DUNGEON_PUT_WALL_DECORATION(int iGfxType, int iMapIndex, int iPosX, int iPosY, int iFacing)
{
	ObjectID oNewObject = OBJECT_NULL;
	SimpleActuator* xSimpAct = NULL;
	U16 iTileQuarter = iFacing;
	int z = 0;

	U16 iLocalCurrentMapIndex = glbCurrentMapIndex;


	//-- This method works if timers are already allocated = game was initialized (else MOVE_RECORD fails to ERROR 45)
	if (glbTimersCount >= glbTimersMaximumCount || glbTimersMaximumCount == 0)
	{
		z = iMapIndex*1024 + iPosY*32 + iPosX;
		
		oNewObject = ObjectID(0, dbText, iFacing);
		xSimpAct = GET_AVAILABLE_ADDRESS_OF_RECORD2X(&oNewObject);
		if (xSimpAct != NULL)
		{
			xSimpAct->w0 = OBJECT_END_MARKER;

			xSimpAct->activated = 0;
			xSimpAct->mode = 1; // active decoration
			xSimpAct->graphism = iGfxType;
			xSimpAct->extendedUsage = 0; // 0 = still image

			DUNGEON_STACK_OBJECT(oNewObject, iMapIndex, iPosX, iPosY);
		}
	}
	else // Game started
	{
		CHANGE_CURRENT_MAP_TO(iMapIndex);

		oNewObject = ALLOC_NEW_RECORD(DB_CATEGORY_SIMPLE_ACTUATOR);
		xSimpAct = GET_ADDRESS_OF_RECORD2X(oNewObject);
		if (xSimpAct != NULL)
		{
			xSimpAct->activated = 0;
			xSimpAct->mode = 1; // active decoration
			xSimpAct->graphism = iGfxType;
			xSimpAct->extendedUsage = 0; // 0 = still image
			MOVE_RECORD_TO(ObjectID(oNewObject, iTileQuarter), -1, 0, iPosX, iPosY);
		}
		
		CHANGE_CURRENT_MAP_TO(iLocalCurrentMapIndex);
	}
}

Actuator* SkWinCore::DUNGEON_PUT_WALL_ACTUATOR(int iGfxType, int iMapIndex, int iPosX, int iPosY, int iDir)
{
	ObjectID oNewObject = OBJECT_NULL;
	Actuator* xAct = NULL;
	U16 iTileQuarter = 0;
	int z = 0;

	U16 iLocalCurrentMapIndex = glbCurrentMapIndex;


	//-- This method works if timers are already allocated = game was initialized (else MOVE_RECORD fails to ERROR 45)
	if (glbTimersCount >= glbTimersMaximumCount || glbTimersMaximumCount == 0)
	{
		z = iMapIndex*1024 + iPosY*32 + iPosX;
		
		oNewObject = ObjectID(iDir, dbActuator, 0);
		xAct = GET_AVAILABLE_ADDRESS_OF_RECORD3(&oNewObject);
		if (xAct != NULL)
		{
			xAct->w0 = OBJECT_END_MARKER;

			xAct->w2 = xAct->w4 = xAct->w6 = 0;
			oNewObject.Dir(iDir);
			DUNGEON_STACK_OBJECT(oNewObject, iMapIndex, iPosX, iPosY);
		}
	}
	else // Game started
	{
		CHANGE_CURRENT_MAP_TO(iMapIndex);

		oNewObject = ALLOC_NEW_RECORD(DB_CATEGORY_ACTUATOR);
		xAct = GET_ADDRESS_OF_RECORD3(oNewObject);
		if (xAct != NULL)
		{
			xAct->w2 = xAct->w4 = xAct->w6 = 0;
			oNewObject.Dir(iDir);
			MOVE_RECORD_TO(ObjectID(oNewObject, iTileQuarter), -1, 0, iPosX, iPosY);
		}
		
		CHANGE_CURRENT_MAP_TO(iLocalCurrentMapIndex);
	}
	return xAct;
}

//==============================================================================

ObjectID* globalGroundObjects = NULL; // for all tiles

int tblCustomNbItemAllocationPerDB[16] = 
{
	256, 512, 256, 512, 
	75, 100, 60, 25, 
	12, 5, 200, 0, 
	0, 0, 60, 50
};

int SkWinCore::INIT_BLANK_DUNGEON(X16 isNewGame)
{
	int iInitSuccess = 1;
	int nMaps = 2;
	int nRows = 32;
	int nCols = 32;
	int iMapIndex = 0;
	int i = 0;
	int j = 0;
	int z = 0;
	int iDBCategory = 0;
	Map_definition_info* xCurrentMap = NULL;
	int iTotalMapDataSize = 0;
	int iMapDataOffset = 0;
	int iTileIndex = 0;
	int nColumnCounter = 0;

	// Init start position
	glbPlayerPosX = 0;
	glbPlayerPosY = 0;
	glbPlayerDir = 0;
	glbPlayerMap = 0;

	//U16* globalGroundObjects = NULL; // for all tiles
	//ObjectID* globalGroundObjects = NULL; // for all tiles 

	dunHeader = reinterpret_cast<File_header *>(ALLOC_MEMORY_RAM(44, 1, 1024));
	dunMapsHeaders = reinterpret_cast<Map_definitions *>(ALLOC_MEMORY_RAM(nMaps << 4, afUseUpper, 0x400));
	//globalGroundObjects = reinterpret_cast<U16 *> (ALLOC_MEMORY_RAM(nMaps*32*32*sizeof(U16), afUseUpper, 0x400));
	globalGroundObjects = reinterpret_cast<ObjectID *> (ALLOC_MEMORY_RAM(nMaps*32*32*sizeof(ObjectID), afUseUpper, 0x400));

	for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++)
		for (j = 0; j < nRows; j++)
			for (i = 0; i < nCols; i++)
			{
				z = iMapIndex*1024 + j*32 + i;
				globalGroundObjects[z] = OBJECT_END_MARKER;
			}

	// Init dungeon header and map headers
	dunHeader->w0 = 0x0063; // DM seed
	dunHeader->cbMapData = 0;
	dunHeader->nMaps = nMaps;
	dunHeader->b5 = 0;
	dunHeader->cwTextData = 0;
	dunHeader->w8 = 0;
	dunHeader->cwListSize = 0;
	for (iDBCategory = 0; iDBCategory < 16; iDBCategory++)
		dunHeader->nRecords[iDBCategory] = 0;

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
		xCurrentMap->level = iMapIndex;
		xCurrentMap->unused2 = 0;
		xCurrentMap->xDim = 32 - 1;
		xCurrentMap->yDim = 32 - 1;
		xCurrentMap->nWalls = 0;
		xCurrentMap->rWalls = 0;
		xCurrentMap->nFloors = 0;
		xCurrentMap->rFloors = 0;
		xCurrentMap->nOrnates = 0;
		xCurrentMap->nMonsters = 0;
		//-- Note about ornate gfx : data is stored among map data in quite a static way. it is then simpler to allocate the max from the beginning and fill the data thereafter
		xCurrentMap->nWalls = 15;
		xCurrentMap->nFloors = 15;
		xCurrentMap->nOrnates = 15;
		xCurrentMap->nMonsters = 15;	// this one is less important as creature data holds itself the creature id

		xCurrentMap->unknown1 = 0;
		xCurrentMap->depth = 0;
		xCurrentMap->unknown2 = 0;
		xCurrentMap->tileset = 3;
		xCurrentMap->door1 = 2; // metal gate
		xCurrentMap->door2 = 0; // iron grate

		xCurrentMap->mapOffset = iMapDataOffset;

		iLocalMapDataSize = ( (xCurrentMap->xDim+1)  * (xCurrentMap->yDim+1) );
		iLocalMapDataSize += ( xCurrentMap->nWalls + xCurrentMap->nFloors + xCurrentMap->nOrnates + xCurrentMap->nMonsters );
		iTotalMapDataSize += ( iLocalMapDataSize );

		iMapDataOffset += ( iLocalMapDataSize );

	}
	dunHeader->cbMapData = iTotalMapDataSize;
	dunMapData = ALLOC_MEMORY_RAM(dunHeader->cbMapData, afUseUpper, 0x400);

	// Init map data with floors
	for (iTileIndex = 0; iTileIndex < dunHeader->cbMapData; iTileIndex++)
		dunMapData[iTileIndex] = 0x20;

	U8* xMapData = dunMapData;
	for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++)
	{
		int iMapSize = 32*32;
		int iGfxSize = 15*4;
		int i = 0;
		xCurrentMap = (Map_definition_info*) &dunMapsHeaders[iMapIndex];
		iMapSize = (xCurrentMap->xDim+1) * (xCurrentMap->yDim+1);
		iGfxSize = (xCurrentMap->nWalls) + (xCurrentMap->nFloors) + (xCurrentMap->nOrnates) + (xCurrentMap->nMonsters);

		for (i = 0; i < iMapSize; i++)
		{
			*xMapData = 0x20;
			xMapData++;
		}

		for (i = 0; i < iGfxSize; i++)
		{
			*xMapData = 0xFF;
			xMapData++;
		}
	
	}
		



	// Columns count and update map header flags
	dunMapColumnsSumArray = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(nMaps << 1, afUseUpper, 0x400));

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
	dunGroundStacks = reinterpret_cast<OID_T*>(ALLOC_MEMORY_RAM(dunHeader->cwListSize << OID_SIZE_BITSHIFT, afUseUpper, 0x400));
	dunTextData = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(dunHeader->cwTextData << 1, afUseUpper, 0x400));

	for (int iGroundRefIndex = 0; iGroundRefIndex < dunHeader->cwListSize; iGroundRefIndex++)
		dunGroundStacks[iGroundRefIndex] = OBJECT_NULL;

	//--- Init items
 	for (iDBCategory = 0; iDBCategory < 16; iDBCategory++)
	{
		int c = 0;
		U8* xObjectPointer = NULL;
		int iRefObjectIndex = 0;
		Bit16u nRecords = dunHeader->nRecords[iDBCategory];
		if (isNewGame != 0) 
		{
			nRecords = min_value((iDBCategory == dbCloud) ? 0x300 : 0x400, tblCustomNbItemAllocationPerDB[RCJ(16,iDBCategory)] + nRecords);
			//nRecords = min_value((iDBCategory == dbCloud) ? 0x300 : 0x400, tblDefaultNbItemAllocationPerDB[RCJ(16,iDBCategory)] + nRecords);
			dunHeader->nRecords[iDBCategory] = nRecords;
		}
		Bit16u iItemSize = glbItemSizePerDB[iDBCategory];

		glbDBObjectData[iDBCategory] = ALLOC_MEMORY_RAM(nRecords * iItemSize, afUseUpper, 0x400);

		xObjectPointer = glbDBObjectData[iDBCategory];
		for (iRefObjectIndex = 0; iRefObjectIndex < nRecords; iRefObjectIndex++)
		{
			for (c = 0; c < iItemSize; c++)
				xObjectPointer[c] = 0xFF;

			xObjectPointer += iItemSize;
		}
	}

	// default init of dunGroundStacks
	int iRefObjectIndex = 0;
	for (iRefObjectIndex = 0 ; iRefObjectIndex < 0; iRefObjectIndex++)
		dunGroundStacks[iRefObjectIndex] = OBJECT_NULL;

	// Alloc map data
	glbMapTileValue = reinterpret_cast<U8 ***>(ALLOC_MEMORY_RAM((nColumnCounter + nMaps) * sizeof(void *), afUseUpper, 0x400));
	Bit8u ***bp08 = &glbMapTileValue[nMaps];

	for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++)
	{
		int iColIndex = 0;
		glbMapTileValue[iMapIndex] = reinterpret_cast<U8 **>(bp08);
		Bit8u *bp0c = &dunMapData[dunMapsHeaders[iMapIndex].w0];
		*bp08 = reinterpret_cast<U8 **>(bp0c);
		bp08++;
		for (iColIndex = 1; dunMapsHeaders[iMapIndex].Column() -1 >= iColIndex; iColIndex++)
		{
			bp0c += dunMapsHeaders[iMapIndex].Row();
			*bp08 = reinterpret_cast<U8 **>(bp0c);
			bp08++;
		}
		
	}

	// Rebuild objects stacks
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
/*
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


	printf("Arrange Dungeon ...\n");
	if (isNewGame != 0) {
		ARRANGE_DUNGEON();
	}
	if (_4976_3b5d != 0) {
		DECIDE_DEFAULT_DUNGEON_MAP_CHIP_SET();
	}
	_4976_3b5d = 0;
	CHANGE_CURRENT_MAP_TO(glbPlayerMap);

	SkCodeParam::bRandomInitChampion = true;
	*/
	

	return iInitSuccess;
}

int SkWinCore::INIT_REBUILD_DUNGEON_GROUND_STACKS()
{
	int iMapIndex = 0;
	int nMaps = 0;
	Map_definition_info* xCurrentMap = NULL;
	int nCols = 0;
	int nRows = 0;
	int i = 0;
	int j = 0;
	int z = 0;
	nMaps = dunHeader->nMaps;

	// Rebuild objects stacks
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
	return 0;
}

int SkWinCore::INIT_DUNGEON_FINALIZE_ARRANGE(X16 isNewGame)
{
	int iInitSuccess = 0;

	int nMaps = dunHeader->nMaps;


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


	printf("Arrange Dungeon ...\n");
	if (isNewGame != 0)
		ARRANGE_DUNGEON();
	
	if (_4976_3b5d != 0)
		DECIDE_DEFAULT_DUNGEON_MAP_CHIP_SET();
	
	_4976_3b5d = 0;
	CHANGE_CURRENT_MAP_TO(glbPlayerMap);

	SkCodeParam::bRandomInitChampion = true;

	return iInitSuccess;
}




//==============================================================================

int SkWinCore::DUNGEON_CREATE_BLANK()
{
	int iCreateDungeonSuccess = 0;
	

	INIT_RANDOM();
	SkCodeParam::bAllowFreeObjectMove = true;
	iCreateDungeonSuccess = INIT_BLANK_DUNGEON(1);
	//__LOAD_CREATURE_FROM_DUNGEON();
	
	SkWinLua_RunFunctionFromScript(this, "data_dmx/script/dungeon.lua", "generate", 0, 0, 0, 0);
	SkCodeParam::bDM1Mode = 1;

	for (int iMapIndex = 0; iMapIndex < 64; iMapIndex++)
		CHANGE_MUSIC_ID(iMapIndex, iMapIndex+1);

	//DUNGEON_SET_TILE(ttPit, 0,  0, 0, 0);
	//DUNGEON_SET_TILE_FLAG(TILE_FLAG_IS_OPEN, 0, 0, 0);
	//DUNGEON_PUT_NEW_CREATURE(5,  1, 2, 2);
	//DUNGEON_PUT_NEW_CLOTHING(rand()%50, 0, 2, 2);
	//DUNGEON_PUT_NEW_WEAPON(rand()%50, 0, 2, 2);
	//DUNGEON_PUT_NEW_MISC_ITEM(rand()%50, 0, 2, 2);
	//DUNGEON_PUT_NEW_POTION(rand()%20, 0, 2, 2);

//	DUNGEON_PUT_FLOOR_DECORATION(11, 0, 1, 1);
	//DUNGEON_PUT_NEW_WEAPON(rand()%50, 0, 1, 1);

//DUNGEON_PUT_FLOOR_DECORATION(0x0B, 0, 2, 1);
//DUNGEON_PUT_FLOOR_DECORATION(0x0C, 0, 1, 2);
//DUNGEON_PUT_FLOOR_DECORATION(0x18, 0, 2, 2);
//DUNGEON_PUT_FLOOR_DECORATION(0x23, 0, 3, 3);

	INIT_REBUILD_DUNGEON_GROUND_STACKS();

	__LOAD_CREATURE_FROM_DUNGEON();
	INIT_DUNGEON_FINALIZE_ARRANGE(1);		//

	//LOG_FULL_DUNGEON_INFO();
	
	return iCreateDungeonSuccess;

}
