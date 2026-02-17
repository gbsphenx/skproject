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
U16 SkWinCore::ATTACK_DOOR(i16 x, i16 y, U16 damage, U16 isSpellAttack, U16 delay)
{
	ENTER(18);
	Door *door = GET_ADDRESS_OF_TILE_RECORD(U8(x), U8(y))->castToDoor();	//*bp04
	// SPX: Disable door controls if "weak door" mode is activated. Controls within are original code
	if (!SkCodeParam::bWeakDoors) {
		if (isSpellAttack != 0 && door->DestroyablebyFireball() == 0)
			return 0;
		if (isSpellAttack == 0 && door->BashablebyChopping() == 0)
			return 0;
		if (QUERY_DOOR_DAMAGE_RESIST(GET_GRAPHICS_FOR_DOOR(door)) <= damage)
			return 0;
	} // End of "weak door"	
	U8 *bp08 = &glbCurrentTileMap[x][y];
	if ((*bp08 & 7) != _DOOR_STATE_C04_CLOSED)	// ((*bp08 & 7) != 4) not closed
		return 0;
	if (delay != 0) {
		Timer bp12;
		bp12.SetMap(glbCurrentMapIndex);
		bp12.SetTick(glbGameTick +delay);
		bp12.TimerType(ttyDoorDestroy);
		bp12.actor = TIMER_ACTOR__00;
		bp12.XcoordB(U8(x));
		bp12.YcoordB(U8(y));
		QUEUE_TIMER(&bp12);
	}
	else {
		*bp08 = (*bp08 & 0xf8) | _DOOR_STATE_C05_DESTROYED; // (*bp08 = (*bp08 & 0xf8) | 5;) Destroyed or bashed
	}
	return 1;
}