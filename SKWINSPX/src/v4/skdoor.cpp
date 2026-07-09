//------------------------------------------------------------------------------
// SK-DOOR
//------------------------------------------------------------------------------

#include <skcore.h>
#include <skparam.h>

//^0CEE:32BA
//X16 SkWinCore::QUERY_DOOR_STRENGTH(X8 cls2)
U16 SkWinCore::QUERY_DOOR_STRENGTH(U8 iDoorType)
{
	ENTER(0);
	return QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x0E_DOORS, iDoorType, dtWordValue, GDAT_DOOR_x0F_STRENGTH);
}

//^0CEE:328C
// SPX: _0cee_328c renamed GET_DOOR_STAT_10
U16 SkWinCore::GET_DOOR_STAT_10(U8 iDoorType) // Bit8u cls2
{
	return QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x0E_DOORS, iDoorType, dtWordValue, GDAT_DOOR_x10); // 0x10
}

// _0cee_3275 renamed GET_DOOR_STAT_0D
U16 SkWinCore::GET_DOOR_STAT_0D(U8 iDoorType)
{
	ENTER(0);
	return QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x0E_DOORS, iDoorType, dtWordValue, GDAT_DOOR_x0D);
}

//^0CEE:322D
U8 SkWinCore::GET_GRAPHICS_FOR_DOOR(Door* xDoor)
{
	if (xDoor->DoorType() == 0) {
		if (dunMapLocalHeader->b2_7_7() == 0)
			return 0xFF;
		return dunMapLocalHeader->DoorType0();
	}
	else {
		if (dunMapLocalHeader->b3_0_0() == 0)
			return 0xFF;
		return dunMapLocalHeader->DoorType1();
	}
}

//^0CEE:32A3
i16 SkWinCore::QUERY_DOOR_DAMAGE_RESIST(U8 iDoorType)	// cls2
{
	ENTER(0);
	return QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x0E_DOORS, iDoorType, dtWordValue, GDAT_DOOR_x0E_DEFENSE);
}


//^075F:2205
U16 SkWinCore::ATTACK_DOOR(i16 x, i16 y, U16 iDamage, U16 isSpellAttack, U16 iDestroyDelay)
{
	ENTER(18);
	Door* xDoor = GET_ADDRESS_OF_TILE_RECORD(U8(x), U8(y))->castToDoor();	//*bp04
	// SPX: Disable door controls if "weak door" mode is activated. Controls within are original code
	if (!SkCodeParam::bWeakDoors) {
		if (isSpellAttack != 0 && xDoor->DestroyablebyFireball() == 0)
			return 0;
		if (isSpellAttack == 0 && xDoor->BashablebyChopping() == 0)
			return 0;
		if (QUERY_DOOR_DAMAGE_RESIST(GET_GRAPHICS_FOR_DOOR(xDoor)) <= iDamage)
			return 0;
	} // End of "weak door"	
	U8* iTile = &glbCurrentTileMap[x][y];	// bp08
	if ((*iTile & 7) != _DOOR_STATE_C04_CLOSED)	// ((*bp08 & 7) != 4) not closed
		return 0;	// won't destroy a door that is open
	if (iDestroyDelay != 0) {
		Timer xTimer;	// bp12
		xTimer.SetMap(glbCurrentMapIndex);
		xTimer.SetTick(glbGameTick + iDestroyDelay);
		xTimer.TimerType(C02_tty_DoorDestroy);
		xTimer.actor = TIMER_ACTOR__00;
		xTimer.XcoordB(U8(x));
		xTimer.YcoordB(U8(y));
		QUEUE_TIMER(&xTimer);
	}
	else {	// adjust tile with DESTROYED flag
		*iTile = (*iTile & 0xF8) | _DOOR_STATE_C05_DESTROYED; // (*bp08 = (*bp08 & 0xf8) | 5;) Destroyed or bashed
	}
	return 1;
}