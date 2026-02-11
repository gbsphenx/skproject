//------------------------------------------------------------------------------
// SK-DOOR
//------------------------------------------------------------------------------

#include <skcore.h>

//^0CEE:32BA
//X16 SkWinCore::QUERY_DOOR_STRENGTH(X8 cls2)
U16 SkWinCore::QUERY_DOOR_STRENGTH(U8 iDoorType)
{
	ENTER(0);
	return QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x0E_DOORS, iDoorType, dtWordValue, GDAT_DOOR_STRENGTH);
}

//^0CEE:328C
// SPX: _0cee_328c renamed GET_DOOR_STAT_10
U16 SkWinCore::GET_DOOR_STAT_10(U8 iDoorType) // Bit8u cls2
{
	// TODO what is 10 ??
	return QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x0E_DOORS, iDoorType, dtWordValue, GDAT_DOOR_X10); // 0x10
}

// _0cee_3275 renamed GET_DOOR_STAT_0D
U16 SkWinCore::GET_DOOR_STAT_0D(U8 iDoorType)
{
	ENTER(0);
	return QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x0E_DOORS, iDoorType, dtWordValue, GDAT_DOOR_X0D);
}

//^0CEE:322D
U8 SkWinCore::GET_GRAPHICS_FOR_DOOR(Door *ref)
{
	if (ref->DoorType() == 0) {
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