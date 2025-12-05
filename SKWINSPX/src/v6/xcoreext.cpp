//--- SkWinCore2 -------------------------------------
//SPX: This part of code will hold more easily added code for
// - Retrocompatibility with DM1
// - New code for expanding DM2

#include <StdAfx.h>	// Required for MVC6 compilation

#include <skver.h>

#include <skparam.h>
#include <skdebug.h>
#include <skglobal.h>
#include <skcore.h>
#include <skmidi.h>
#include <sklua.h>

//==============================================================================

int SkWinCore::READ_DUNGEON_STRUCTURE_EXTENDED_GAME(X16 isNewGame, int iDungeonMode)
{
	if (isNewGame == 1 && SkCodeParam::bGameModeExtended)
		return DUNGEON_CREATE_BLANK();
	else if (isNewGame == 1 && SkCodeParam::bTQMode)
		return READ_DUNGEON_STRUCTURE_TQ(isNewGame);
#ifdef __SK_EXTENDED_SKWIN_V6__
	if (isNewGame == 1 && SkCodeParam::bBWMode)
		return READ_DUNGEON_STRUCTURE_BW(isNewGame);
	else if (isNewGame == 1 && SkCodeParam::bEOBMode) {
		//SkCodeParam::bEOBMode = true;
		return READ_DUNGEON_STRUCTURE_EOB(isNewGame);
	}
	//return INIT_BLANK_DUNGEON(isNewGame);
#endif // __SK_EXTENDED_SKWIN_V6__
	return 0;
}

//==============================================================================

// Specifically for Bloodwych locked doors
// The door will be unlocked and will open if the item in hand is corresponding to the misc item referenced on item-eater actuator over the door.
int SkWinCore::HANDLE_KEY_ON_DOOR_BUTTON(Door* xDoor, ObjectID rlHandObject, U16 iPosX, U16 iPosY)
{
	U8 iCanUnlock = 0;
	Door_Info* xDoorInfo = (Door_Info*) xDoor;
	ObjectID rlDoorActuator = OBJECT_NULL;
	Actuator* xActuator = NULL;
	U8 iUseKeyFromLeaderHand = 0;
	U8 iKeyFound = 0;
	printf("USING OBJECT ON DOOR BUTTON\n");

	if (xDoor == NULL)
		return 0;

	if (xDoorInfo->locked == 0)
		return 0;

	if (rlHandObject != OBJECT_NULL)
		iUseKeyFromLeaderHand = 1;

	//-- Check that object is a key
	if (rlHandObject.orif.db == DB_CATEGORY_MISC_ITEM) {
		iCanUnlock = 1;
	}

	//-- Check actuator over door to get the required key item info
	rlDoorActuator = xDoorInfo->next;
	if (rlDoorActuator == OBJECT_NULL || rlDoorActuator == OBJECT_END_MARKER)
		return 1;



	xActuator = GET_ADDRESS_OF_RECORD3(rlDoorActuator);
	iCanUnlock = 0;
	if (xActuator != NULL) {
		Miscellaneous_item* xMisc = NULL;
		if (xActuator->ActuatorType() == ACTUATOR_TYPE_DM1_ITEM_EATER) {
			int iRequestedMiscItemID = xActuator->ActuatorData();

			if (iUseKeyFromLeaderHand) {
				xMisc = GET_ADDRESS_OF_RECORDA(rlHandObject);
				if (xMisc->ItemType() == iRequestedMiscItemID)
					iCanUnlock = 1;
			}
			// Even if the player has no key in hand, it will be more convenient to check through all hands if any key may fit. If so, take it.
			else // try to get any possible key from all champions hands
			{
				iKeyFound = CONSUME_ANY_FROM_HAND(DB_CATEGORY_MISC_ITEM, iRequestedMiscItemID);
				if (iKeyFound)
					iCanUnlock = 1;
				//else {
				//	DISPLAY_HINT_TEXT(COLOR_LIGHT_GREEN, (const U8*) "NO KEY IN HAND\n");
				//	return 0;
				//}
			}
		}
	}

	if (iCanUnlock == 1) {
		xDoorInfo->locked = 0;
		// SPX: This plays the TICK sound when activating a door button
		QUEUE_NOISE_GEN1(GDAT_CATEGORY_MESSAGES, 0x00, SOUND_STD_ACTIVATION_MESSAGE, 0x8C, 0x80, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1);
		Timer bp22;
		INVOKE_MESSAGE(iPosX, iPosY, 0, 2, glbGameTick +1);
		bp22.SetMap(glbCurrentMapIndex);
		bp22.SetTick(glbGameTick +1);
		bp22.TimerType(ttyDoorButton);
		bp22.actor = TIMER_ACTOR__00;
		bp22.id6(GET_TILE_RECORD_LINK(iPosX, iPosY));
		QUEUE_TIMER(&bp22);
		xDoor->w2_b_b(1);
		cd.gg.glbRefreshViewport = 1;

		// Then consume key
		if (iUseKeyFromLeaderHand)
			DEALLOC_RECORD(REMOVE_OBJECT_FROM_HAND());
	}
	else if (iKeyFound) // there is a key, but not the expected one
		DISPLAY_HINT_TEXT(COLOR_LIGHT_GREEN, (const U8*) "IT DOES NOT FIT\n");
	// else, no key => don't display message here

	return 1;
}





////////////////////////////////////////////////////////////////////////////////


// This disables all hands and magic for some time; that allows some new type of attacks from creatures
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

#if (XDMX_FEATURE_PLAGUE == 1)
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
#endif // XDMX_FEATURE_PLAGUE

#if (XDMX_FEATURE_PLAGUE == 1)
// SPX: Custom, code added, based on PROCESS_POISON
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
	if (player +1 == cd.pi.glbNextChampionNumber)
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
	newtimer.SetMap(cd.pi.glbPlayerMap);
	newtimer.SetTick(glbGameTick +0x24);
	newtimer.value = si;
	QUEUE_TIMER(&newtimer);
	return;
}
#endif // XDMX_FEATURE_PLAGUE


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


void SkWinCore::DRAW_EXTENDED_INTERWALLS_DIRECT_FACE(i16 iDisplayCell, U8 iWallFace, U8 iInterwallType)
{
	i16 iDisplayViewportCell = iDisplayCell;

	U8 tblFrontInterwallFace[23]			= { 0x22, 0, 0,		0x25, 0xA6, 0xA7,	0x28, 0xA9, 0xAA, 0x2B, 0x2C,	0x2D, 0xAC, 0xAD, 0x30, 0x31,	0x32, 0, 0, 0, 0, 0, 0 };
	U8 tblFrontInterwallFaceDoorClosed[23]	= { 0x22, 0, 0,		0x93, 0x97, 0x98,	0x94, 0x99, 0x9A, 0x2B, 0x2C,	0x95, 0x9B, 0x9C, 0x30, 0x31,	0x96, 0, 0, 0, 0, 0, 0 };
	U8 tblFrontInterwallFaceDoorOpen[23]	= { 0x22, 0, 0,		0xB3, 0xB7, 0xB8,	0xB4, 0xB9, 0xBA, 0x2B, 0x2C,	0xB5, 0xBB, 0xBC, 0x30, 0x31,	0xB6, 0, 0, 0, 0, 0, 0 };
	//i8 tblSideLeftInterwallFace[23] = { 0, 0xA3, 0xA4,		 };

	if (SkCodeParam::bDebugGFXNoInterWalls)
		return;

	if (iDisplayCell < 0 || iDisplayCell > 23)
		return;

	U8 iMapGfx = glbMapGraphicsSet;	// bp02
	i16 iColorkey1 = glbSceneColorKey;	// bp06
	i8 iXDist = glbTabXAxisDistance[RCJ(23,iDisplayViewportCell)];	// bp07
	i16 iYDist = glbTabYAxisDistance[RCJ(23,iDisplayViewportCell)];	// bp0c
	U8 iWallGFXID = (iYDist > 1) ? GDAT_WALL_ORNATE__DATA_6F : GDAT_WALL_ORNATE__DATA_6E;	// SPX TODO: unknown GDAT values! never seen them in any GDAT
	X16 si = 0;	// si
	X16 iRectX = 0;	// di

	U16 bp0a = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, dtWordValue, iWallGFXID);
	// SPX: in case gfxset (custom dungeon) is >= 6, bypass these values.
	if (SkCodeParam::bUseFixedMode && glbMapGraphicsSet >= 0 && glbMapGraphicsSet < 6) {
		if (iYDist > 1) { // dist 2,3,4
			if (tblCellTilesRoom[4].xsrd.w0 == 0 && (iRectX = (bp0a >> 8)) == 0) {
				iRectX = _4976_463c_tblGfxSet[RCJ(6,glbMapGraphicsSet)];
			}
			if (tblCellTilesRoom[5].xsrd.w0 == 0) {
				si = bp0a & 0xff;
				if (si == 0) {
					si = _4976_4648_tblGfxSet[RCJ(6,glbMapGraphicsSet)];
				}
			}
		}
		else if (iYDist > 0) { // dist 1
			if (tblCellTilesRoom[0].xsrd.w0 == 0 && (iRectX = (bp0a >> 8)) == 0) {
				iRectX = _4976_4624_tblGfxSet[RCJ(6,glbMapGraphicsSet)];
			}
			if (tblCellTilesRoom[2].xsrd.w0 == 0 && (si = (bp0a & 0xff)) == 0) {
				si = _4976_4630_tblGfxSet[RCJ(6,glbMapGraphicsSet)];
			}
		}
	}
	if (iRectX != 0 || si != 0) {
		TRIM_BLIT_RECT(iRectX, 0, si, 0);
	}
	X16 bFlip = (iXDist <= 0) ? 0 : 1;	// (bp04) 0 = left / 1 = right
	if (iDisplayViewportCell >= 0x10) { // wall at D4
		if (iXDist == -2 || iXDist == 2) {
			bFlip = 0;	// left
		}
		bFlip ^= glbGeneralFlipGraphics;
		iWallGFXID = GDAT_GFXSET_WALL_D3_FRONT;	// 0x32 : D4 wall front
	}
	/*
	else if (glbGeneralFlipGraphics != 0) {
		iWallGFXID = tlbDoorSideFramesReorder[RCJ(23,iDisplayViewportCell)] - 80;
		if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_GRAPHICSSET, iMapGfx, dtImage, iWallGFXID) == 0) {
			if (iXDist == 0) {
				bFlip = 1;	// right
			}
			iWallGFXID = iWallGFXID + 0x72;
		}
		// -80 + 114 (x72) = 34 (x22), knowing that the first visible wall (D0L) is at x23.
		// So this is ordered to find wall GFX id from tlbDoorSideFramesReorder actually.
	}
	else 
		*/
	{
		//iWallGFXID = iDisplayViewportCell + GDAT_GFXSET_WALL_D0_START_MARKER;	// 0x23 is wall side D0
		if (iWallFace == 0 || iWallFace == 2)
			iWallGFXID = tblFrontInterwallFace[iDisplayViewportCell];
		else if (iWallFace == 3 || iWallFace == 1)
			//iWallGFXID = iDisplayViewportCell + GDAT_GFXSET_WALL_D0_START_MARKER;
			iWallGFXID = iDisplayViewportCell + 0x22;
	}
	//printf("Wall face: %02d T=%02d GFX=%02X\n", iWallFace, iInterwallType, iWallGFXID);
	if (iWallGFXID == 0)
		return;

	if (iInterwallType == 2) // open door
	{
		iWallGFXID = tblFrontInterwallFaceDoorOpen[iDisplayCell];
		if (iWallFace == 3 || iWallFace == 1)
			iWallGFXID = iDisplayViewportCell + 0xBD - 1;
	}
	else if (iInterwallType == 3) // closed door
	{
		iWallGFXID = tblFrontInterwallFaceDoorClosed[iDisplayCell];
		if (iWallFace == 3 || iWallFace == 1)
			iWallGFXID = iDisplayViewportCell + 0x9D - 1;
	}


	// Instead of displaying basic ground when image is not available, use 0x22 as blank image
	if (iWallGFXID == 0)
		iWallGFXID = 0x22; // blank image


	SkD((0, "Cell %02d @ R(%02d, %02d) (Display %02d) => WGFX-ID = %02X CK=%02X\n", 
		iDisplayViewportCell, iXDist, iYDist, iDisplayViewportCell, iWallGFXID, iColorkey1));
	//SkD((DLV_DBG_CELL, "Cell %02d (Display %02d) => WallGfxSet = %02d   WGFX = %02X\n", iDisplayViewportCell, iDisplayViewportCell, iMapGfx, bp01));
	QUERY_TEMP_PICST(bFlip, 0x40, 0x40, 0, 0, (cd.pi.glbIsPlayerMoving != 0) ? -iYDist : 0, iDisplayViewportCell + 0x2be, 0xffff, iColorkey1, -1, GDAT_CATEGORY_GRAPHICSSET, iMapGfx, iWallGFXID);
	DRAW_TEMP_PICST(); // draw wall

	if (iWallGFXID == 0xB3 || iWallGFXID == 0x93)
		MAKE_BUTTON_CLICKABLE(&glbTempPicture.rc36, 7, U8(iDisplayCell));	// 7 is a new value catched by CLICK_VWPT

	_098d_0c45();
	return;
}

// What cells to display for interwalls on sides and farther front faces
i8 tblNeighborCellFarther[23] = { 3,4,5,	6,7,8,		11,12,13,14,15,		16,17,18,19,20,		-1,-1,-1,-1,-1,-1,-1  };
i8 tblNeighborCellLeft[23] =    { 1,-1,0,	4,-1,3,		7,9,6,-1,8,			12,14,11,-1,13,		17,19,16,21,18,-1,20 };
i8 tblNeighborCellRight[23] =   { 2,0,-1,	5,3,-1,		8,6,10,7,-1,		13,11,15,12,-1,		18,16,20,17,22,19,-1 };

void SkWinCore::DRAW_EXTENDED_INTERWALLS(i16 iViewportCell)
{
	ENTER(12);
	i16 iDisplayViewportCell = iViewportCell;

	if (SkCodeParam::bDebugGFXNoInterWalls)
		return;

	U8 iMapGfx = glbMapGraphicsSet;	// bp02
	i16 iColorkey1 = glbSceneColorKey;	// bp06
	i8 iXDist = glbTabXAxisDistance[RCJ(23,iViewportCell)];	// bp07
	i16 iYDist = glbTabYAxisDistance[RCJ(23,iViewportCell)];	// bp0c
	U8 bp01 = (iYDist > 1) ? GDAT_WALL_ORNATE__DATA_6F : GDAT_WALL_ORNATE__DATA_6E;	// SPX TODO: unknown GDAT values! never seen them in any GDAT
	X16 si = 0;
	X16 di = 0;

	SkD((DLV_DBG_CELL, "Cell %02d @ (%02d,%02d)\n", iViewportCell, tblCellTilesRoom[iViewportCell].posx, tblCellTilesRoom[iViewportCell].posy));


	U16 iIsInterWall = 0;
	U16 iInterWall[4];
	U16 xWallInfo = 0;
	for (U8 face = 0; face < 4; face++) {
		xWallInfo = tblCellTilesRoom[iViewportCell].xsrd.interwalls[face];
		iInterWall[face] = xWallInfo;
		if (xWallInfo != 0)
			iIsInterWall = xWallInfo;
	}
	if (iIsInterWall == 0)
		return;

	//printf("C:%02d T=%02d\n", iViewportCell, iIsInterWall);
// NOTE: an left/right interwall will be displayed from different cells if it is on the left or right from the player
// And a right interwall for (-1,-1) would be displayed the same as left interwall for (0,-1)

	// if front interwall is on, the we can draw centered standard Dx walls (not sides) on that cell
	if (iInterWall[0]) // far wall on cell (0,-1)
		DRAW_EXTENDED_INTERWALLS_DIRECT_FACE(tblNeighborCellFarther[iViewportCell], 0, iInterWall[0]);
	if (iInterWall[1] && iXDist >= 0) // right wall on cell (+1,x)
		DRAW_EXTENDED_INTERWALLS_DIRECT_FACE(tblNeighborCellRight[iViewportCell], 3, iInterWall[1]);
	else if (iInterWall[1] && iXDist < 0) // right wall on cell (x,x)
		DRAW_EXTENDED_INTERWALLS_DIRECT_FACE(iViewportCell, 3, iInterWall[1]);
	if (iInterWall[3] && iXDist <= 0) // left wall on cell
		DRAW_EXTENDED_INTERWALLS_DIRECT_FACE(tblNeighborCellLeft[iViewportCell], 1, iInterWall[3]); // treat it as "east" wall
	else if (iInterWall[3] && iXDist > 0) // left wall on cell
		DRAW_EXTENDED_INTERWALLS_DIRECT_FACE(iViewportCell, 1, iInterWall[3]); // treat it as "east" wall
	if (iInterWall[2]) // front wall on same cell
		DRAW_EXTENDED_INTERWALLS_DIRECT_FACE(iViewportCell, 2, iInterWall[2]);
	return;
}


void SkWinCore::DRAW_EXTENDED_FLOOR_INTERWALLS(i16 iViewportCell)	// i16 xx
{
	i16 iLocalViewportCell = iViewportCell;	//i16 si = xx;
	X8 iMapGraphicsSet = glbMapGraphicsSet;
	U16 iMapSceneColorKey = glbSceneColorKey;
	
	if (!SkCodeParam::bBWMode)
		return;

	glbMapGraphicsSet = 2;	// special gfxset only for redwood interwalls
	glbSceneColorKey = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, dtWordValue, GDAT_GFXSET_SCENE_COLORKEY); // colorkey
	DRAW_EXTENDED_INTERWALLS(iLocalViewportCell);
	glbMapGraphicsSet = iMapGraphicsSet;
	glbSceneColorKey = iMapSceneColorKey;
	return;
}







////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

i16 SkWinCore::CONSUME_ANY_FROM_HAND(i16 iDBItem, i16 iItemID)
{
	int iFoundAndConsumed = 0;
	i16 iPlayer = 0;
	ObjectID rlItem = OBJECT_NULL;
	Miscellaneous_item* xMisc = NULL;
	i16 iHand = 0;

	for (iPlayer = 0; iPlayer < 4; iPlayer++) {
		for (iHand = 0; iHand < 2; iHand++) {
			rlItem = glbChampionSquad[iPlayer].Possess(INVENTORY_HAND_RIGHT + iHand);
			if (rlItem != OBJECT_NULL) {
				if (rlItem.orif.db == iDBItem) {
					if (iDBItem == DB_CATEGORY_MISC_ITEM) {
						xMisc = GET_ADDRESS_OF_RECORDA(rlItem);
						if (xMisc->ItemType() == iItemID) {
							iFoundAndConsumed = 1;
							glbChampionSquad[iPlayer].Possess(INVENTORY_HAND_RIGHT + iHand, OBJECT_NULL);
							DEALLOC_RECORD(rlItem);
							return iFoundAndConsumed;
						}
					}
				}
			}
		}
			
	}

	return iFoundAndConsumed;
}

//



