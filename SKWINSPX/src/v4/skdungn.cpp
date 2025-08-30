//------------------------------------------------------------------------------
// SK-DUNGEON
//------------------------------------------------------------------------------

//#include <StdAfx.h>	// Required for MVC6 compilation

//#include <stdlib.h>
//#include <conio.h> // getch


#include <skcore.h>
#include <skdebug.h>
#include <skglobal.h>
#include <skparam.h>

//#if defined(_USE_MFC80) || defined(_USE_MFC60)
//#include <SkMFC.h>
//#include <resource.h>
//#endif // defined(_USE_MFC80) || defined(_USE_MFC60)








//^0CEE:0AB5
// CSBWin //  TAG009a94 RN FindFirstObject(i32 x,i32 y)
ObjectID SkWinCore::GET_TILE_RECORD_LINK(i16 xx, i16 yy)
{
	//^0CEE:0AB5
	i16 index = (GET_OBJECT_INDEX_FROM_TILE(xx, yy)).w; // CSBWin LOCAL_2 = GetObjectListIndex(x, y);
	if (index == -1)
		return OBJECT_END_MARKER;
	if (index == -2)
		return OBJECT_NULL;

//	ATLASSERT(dunGroundStacks[index] != 0xFFFE && dunGroundStacks[index] != 0xFFFF);
	if (!SkCodeParam::bUsePowerDebug)
	ATLASSERT(dunGroundStacks[index] != OBJECT_END_MARKER && dunGroundStacks[index] != OBJECT_NULL);

	//^0CEE:0AD1
	return dunGroundStacks[index];
}


//^0CEE:0330
Bit16u SkWinCore::IS_TILE_PASSAGE(i16 xx, i16 yy)
{
	// Return 0 if block
	// Return 1 if passage

	ATLASSERT(U16(xx) < glbCurrentMapWidth && U16(yy) < glbCurrentMapHeight);

	//^0CEE:0330
	ATLASSERT(glbCurrentTileMap[xx]);
	Bit8u tileType = (glbCurrentTileMap[xx])[yy] >> 5;
	if (tileType == ttTeleporter) {
		//^0CEE:0357
		if (GET_ADDRESS_OF_TILE_RECORD(xx, yy)->castToTeleporter()->w4_0_0() != 0) {
			return 0;
		}
	}
	//^0CEE:0377
	if (tileType == ttWall)
		//^0CEE:0379
		return 0;
	//^0CEE:037B
	if (tileType != ttMapExit)
		//^0CEE:037E
		return 1;
	//^0CEE:0380
	return 0;
}



//^0CEE:0388
U8 SkWinCore::GET_TILE_VALUE(i16 xx, i16 yy)
{
	//^0CEE:0388
	//^0CEE:038E
	i16 si = xx;
	i16 di = yy;
	//^0CEE:0394
	int bp02 = (di >= 0 && di < glbCurrentMapHeight) ? 1 : 0;
	//^0CEE:03A8
	int bp04 = (si >= 0 && si < glbCurrentMapWidth) ? 1 : 0;

	//^0CEE:03BC
	if (bp04 != 0 && bp02 != 0) {
		//^0CEE:03C6
		return (glbCurrentTileMap[si])[di];
	}
	//^0CEE:03DA
	if (bp02 != 0) { // if y-pos (horz dir in map) is in range
		//^0CEE:03E0
		if (si == -1) {
			//^0CEE:03E5
			si = 0;
			bp04 = 4;
		}
		else {
			//^0CEE:03EE
			if (si != glbCurrentMapWidth)
				//^0CEE:03F4
				return 0xE0;
			//^0CEE:03F7
			si--;
			bp04 = 1;
		}
		//^0CEE:03FD
		if (IS_TILE_PASSAGE(si, di) != 0)
			//^0CEE:0409
			return U8(bp04);
		//^0CEE:040F
		if (di > 0) {
			//^0CEE:0413
			if (IS_TILE_PASSAGE(si, di -1) != 0)
				//^0CEE:0420
				return 0x00;
		}
		//^0CEE:0422
		if (di +1 >= glbCurrentMapHeight)
			//^0CEE:042B
			return 0xE0;
		//^0CEE:042E
		if (IS_TILE_PASSAGE(si, di +1) == 0)
			//^0CEE:043D
			return 0xE0;
		//^0CEE:0440
		return 0x00;
	}
	//^0CEE:0443
	else if (bp04 != 0) { // if x-pos (vert dir in map) is in range
		//^0CEE:0449
		if (di == -1) {
			//^0CEE:044E
			di = 0;
			bp02 = 2;
		}
		else {
			//^0CEE:0457
			if (di != glbCurrentMapHeight)
				//^0CEE:045D
				return 0xE0;
			//^0CEE:0460
			di--;
			bp02 = 8;
		}
		//^0CEE:0466
		if (IS_TILE_PASSAGE(si, di) != 0)
			//^0CEE:0472
            return U8(bp02);
		//^0CEE:0477
		if (si > 0 && IS_TILE_PASSAGE(si -1, di) != 0)
			//^0CEE:0488
			return 0x00;
		//^0CEE:048A
		if (si +1 >= glbCurrentMapWidth)
			//^0CEE:0491
			return 0xE0;
		//^0CEE:0493
		if (IS_TILE_PASSAGE(si +1, di) == 0)
			//^0CEE:04A0
			return 0xE0;
		//^0CEE:04A2
		return 0x00;
	}
	else {
		//^0CEE:04A4
		if (si == -1) {
			//^0CEE:04A9
			si = 0;
		}
		else {
			//^0CEE:04AD
			if (si != glbCurrentMapWidth)
				//^0CEE:04B1
				return 0xE0;
			//^0CEE:04B3
			si--;
		}
		//^0CEE:04B4
		if (di == -1 && IS_TILE_PASSAGE(si, 0) != 0)
			//^0CEE:04C4
			return 0x00;
		//^0CEE:04C6
		if (di != glbCurrentMapHeight)
			return 0xE0;
		//^0CEE:04CC
		if (IS_TILE_PASSAGE(si, di -1) == 0)
			return 0xE0;
		//^0CEE:04DB
		return 0x00;
	}
}


//^0CEE:073E
i16 SkWinCore::FIND_LADDER_AROUND(i16 xx, i16 yy, i16 isupper, Actuator **ref)
{
	// Find a ladder around your location.

	// returns dir(nesw) if found, otherwise -1.

	// isupper: +1 = finds ladder for upper
	// isupper: -1 = finds ladder for lower

	// ref: returns actuator if found as a ladder actuator?

	//^0CEE:073E
	ENTER(6);
	//^0CEE:0744
	for (i16 si = 0; si <= 3; si++) {
		//^0CEE:0749
		ObjectID bp06;
		bp06 = GET_TILE_RECORD_LINK(xx + glbXAxisDelta[si], yy + glbYAxisDelta[si]);
		//^0CEE:0768
		Bit16u di;
		for (; bp06 != OBJECT_END_MARKER && (di = bp06.DBType()) <= dbActuator; bp06 = GET_NEXT_RECORD_LINK(bp06)) {
			//^0CEE:076B
			if (di == dbActuator || di == dbText) {
				//^0CEE:0778
				GenericRecord *_bp04 = GET_ADDRESS_OF_RECORD(bp06);
				//^0CEE:0787
				if (di == dbActuator) {
					//^0CEE:078C
					Actuator *bp04 = _bp04->castToActuator();
					if (bp04->ActuatorType() == ACTUATOR_TYPE_SIMPLE_LADDER) { // SPX 0x1c -> was used only in Beta as simple ladder. Code still here
						//^0CEE:079E
						if (dunMapsHeaders[bp04->ActuatorData()].Level() - dunMapLocalHeader->Level() == isupper) {
							//^0CEE:07CE
							*ref = bp04;
							//^0CEE:07DE
							return si;
						}
					}
				}
				else {
					//^0CEE:07E3
					Text *bp04 = _bp04->castToText();
					if (true
						&& bp04->TextMode() == 1
						&& (bp04->SimpleTextExtUsage() == 6 || bp04->SimpleTextExtUsage() == 7)
					) {
						//^0CEE:0810
						Bit16u	isLadderUp = 0;
						isLadderUp = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp04->OrnateIndex(), dtWordValue, GDAT_WALL_ORNATE__IS_LADDER_UP);	// 0x11
						//^0CEE:0830
						if ((isupper == -1 && isLadderUp != 0) || (isupper == 1 && isLadderUp == 0)) {
							//^0CEE:0844
							*ref = NULL;
							//^0CEE:0852
							//^0CEE:07DE
							return si;
						}
					}
				}
			}
			//^0CEE:0854
		}
		//^0CEE:0879
	}
	//^0CEE:0882
	*ref = NULL;
	//^0CEE:0890
	return -1;
}


//^0CEE:315E
Bit16u SkWinCore::GET_WALL_ORNATE_ALCOVE_TYPE(Bit8u cls2)
{
	// return 0 if non-alcove.
	// return 1 if an alcove.
	// return 2 if a shop glass
	// return 3 if an active sleep device.

	//^0CEE:315E
	ENTER(0);
	//^0CEE:3161
	if (cls2 == 0xff) {
		//^0CEE:3167
		return 0;
	}
	//^0CEE:316B
	return QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, cls2, dtWordValue, GDAT_WALL_ORNATE__0A);	// 0x09 .. .. 0x0A
}

//^0CEE:17E7
// SPX: DM1 Reference: ReDMCSB F0169_DUNGEON_GetRandomOrnamentIndex
// SPX: _0cee_17e7 renamed GET_RANDOM_ORNAMENT_INDEX
Bit16u SkWinCore::GET_RANDOM_ORNAMENT_INDEX(Bit16u iValue1, Bit16u iValue2, Bit16u iModulo)
{
	// CSBwinSimilarity: TAG00a41c,Hash

	//^0CEE:17E7
	ENTER(0);
	//^0CEE:17EA
	//return ((((aa * 31417) >> 1) + (bb * 11) + dunHeader->w0) >> 2) % cc;
	return ((((iValue1 * 31417) >> 1) + (iValue2 * 11) + dunHeader->w0) >> 2) % iModulo;
}

//^0CEE:1815
// SPX: DM1 Reference F0170_DUNGEON_GetRandomOrnamentOrdinal
// SPX: _0cee_1815 renamed GET_RANDOM_ORNAMENT_ORDINAL
// (i16 xx, i16 yy, Bit16u ss, Bit16u tt, Bit16u uu)
Bit8u SkWinCore::GET_RANDOM_ORNAMENT_ORDINAL(i16 iRandomOrnamentAllowed, i16 yy, Bit16u ss, Bit16u tt, Bit16u uu)
{
	// CSBwinSimilarity: TAG00a44c,RandomWallDecoration

	//^0CEE:1815
	ENTER(0);
	//^0CEE:1819
	if (iRandomOrnamentAllowed != 0) {
		//^0CEE:181F
		// SPX: changed i16 to unsigned !
		Bit16u si = GET_RANDOM_ORNAMENT_INDEX( 
			(ss << 5) +tt +2000,
			(glbCurrentMapIndex << 6) + glbCurrentMapWidth + glbCurrentMapHeight +3000,
			uu
			);
		//printf("Get random %d / %d for wflag %d\n", si, yy, iRandomOrnamentAllowed);
		//^0CEE:184A
		if (si < yy) {
			//^0CEE:184F
			//printf("Found random wall ornate #%d in list = %d for wflag %d\n", si, _4976_4dc2[si], iRandomOrnamentAllowed);
			return _4976_4dc2[si]; // wall ornate gfx list
		}
	}
	//^0CEE:1855
    return 0xff;
}

//^0CEE:185A
// SPX: DM1 Reference F0171_DUNGEON_SetSquareAspectRandomWallOrnamentOrdinals
//P0309_pi_SquareAspect, P0310_B_LeftRandomWallOrnamentAllowed, P0311_B_FrontRandomWallOrnamentAllowed, P0312_B_RightRandomWallOrnamentAllowed, P0313_i_Direction, P0314_i_MapX, P0315_i_MapY, P0316_B_SquareIsFakeWall)
// _0cee_185a renamed SET_SQUARE_ASPECT_RANDOM_WALL_ORNAMENT_ORDINALS
void SkWinCore::SET_SQUARE_ASPECT_RANDOM_WALL_ORNAMENT_ORDINALS(ExtendedTileInfo *ref, Bit16u xx, Bit16u yy, Bit16u zz, Bit16u ww, Bit16u vv, i16 ss, i16 tt)
{
	// CSBwinSimilarity: TAG00a4a2

	Bit16u iPosX = ss;
	Bit16u iPosY = tt;

	//^0CEE:185A
	ENTER(0);
	//^0CEE:185F
	//i16 di = tt;
	//xx = yy = zz = ww = 1;
	//printf("Call SET_SQUARE_ASPECT_RANDOM_WALL_ORNAMENT_ORDINALS with %08x %02d,%02d,%02d,%02d %02d,%02d,%02d\n",
	//	ref, xx, yy, zz, ww, vv, ss, tt);
	//^0CEE:1862
	iPosY++;
	Bit16u si = dunMapLocalHeader->WallGraphicsRandomDecorations();
	//^0CEE:1872
	ref->tfoi[2] = (GET_RANDOM_ORNAMENT_ORDINAL(xx, si, iPosX, (++iPosY) * ((vv & 3) +1), 30));
	//^0CEE:1899
	ref->tfoi[3] = (GET_RANDOM_ORNAMENT_ORDINAL(yy, si, iPosX, iPosY * (((++vv) & 3) +1), 30));
	//^0CEE:18C2
	ref->tfoi[0] = (GET_RANDOM_ORNAMENT_ORDINAL(zz, si, iPosX, iPosY * (((++vv) & 3) +1), 30));
	//^0CEE:18EB
	ref->tfoi[1] = (GET_RANDOM_ORNAMENT_ORDINAL(ww, si, iPosX, (iPosY--) * (((++vv) & 3) +1), 30));
	// 1 = left wall
	// 2 = front wall
	// 3 = right wall
	// is order 2,3,0,1 ? instead of 0,1,2,3 ?

	//^0CEE:1915
	if (iPosX < 0 || iPosX >= glbCurrentMapWidth || iPosY < 0 || iPosY >= glbCurrentMapHeight) {
		//^0CEE:192E
		for (si = 3; si <= 6; si++) {
			//^0CEE:1933
			if (GET_WALL_ORNATE_ALCOVE_TYPE(Bit8u(ref->tfoi[RCJ(4,si - 3)])) != 0) {
				//^0CEE:194A
				ref->tfoi[RCJ(4,si -3)] = 0x00ff;
			}
			//^0CEE:1958
		}
	}
	//^0CEE:195E
	//printf("----------------------------------------\n");
	return;
}

//^0CEE:1A46
ObjectID SkWinCore::_0cee_1a46(ExtendedTileInfo *ref, ObjectID recordLink, i16 dir, i16 yy)
{
	//^0CEE:1A46
	ENTER(24);
	//^0CEE:1A4C
	Bit16u dbtype = -1; // bp08
	U8 iOppositeSide = 0; // SPX: added to protect opposite side Text value overwrite current facing actuator/mirror for portrait
	// SPX: TODO DM2_EXTENDED_DATABASE
	for (Bit16u bp16 = 0; recordLink != OBJECT_END_MARKER && (dbtype = recordLink.DBType()) <= dbActuator; recordLink = GET_NEXT_RECORD_LINK(recordLink)) {
		//^0CEE:1A54
		Bit16u bp12 = recordLink.Dir();
		iOppositeSide = (bp12 == dir);
		//^0CEE:1A5D
		if ((bp16 & (1 << bp12)) == 0) {
			//^0CEE:1A6D
			Bit16u bp06 = ((bp12 - dir) & 3) +3;
			//^0CEE:1A7C
			if (yy == 0 || bp06 == 5) {
				//^0CEE:1A8B
				Bit16u si = 0;
				Bit16u di = 0;
				//^0CEE:1A8F
				i32 bp0c = glbGameTick;
				//^0CEE:1A9C
				i32 bp10 = 0;
				//^0CEE:1AA6
				GenericRecord *_bp04 = GET_ADDRESS_OF_RECORD(recordLink);
				//^0CEE:1AB4
				Bit8u bp13;
				if (dbtype == dbText && (!SkCodeParam::bUseFixedMode || (SkCodeParam::bUseFixedMode && !iOppositeSide))) {
					Text *bp04 = _bp04->castToText();
					//^0CEE:1ABD
					switch (bp04->TextMode()) {
						default:
							//^0CEE:1AD2
							continue;
						case 0:
							//^0CEE:1AD5
_1ad5:
							ref->tfoi[RCJ(4,bp06 -3)] = 0;
							//^0CEE:1AE4
							//if (bp06 == 5 && bp04->TextVisibility()) {
							if ( (SkCodeParam::bDM1Mode || bp06 == 5) && bp04->TextVisibility()) { // SPX: allow DM1 text (when on side) to show in xvalue as RL, then we may display the gibberish side if visible
								//^0CEE:1AF8
								ref->xvalue = (recordLink);
							}
							else {
								//^0CEE:1B04
								ref->xvalue = (OBJECT_NULL);
							}
							//^0CEE:1B0D
							bp16 |= 1 << bp12;
							//^0CEE:1B18
							continue;

						case 1:
							//^0CEE:1B1B
							bp13 = Bit8u(bp04->OrnateIndex());
							//^0CEE:1B2D
							switch (bp04->SimpleTextExtUsage()) {
								case 14:
									{
										//^0CEE:1B49
										goto _1ad5;
									}
								case 5:
								case 13:
									{
										//^0CEE:1B4E
										if (bp04->TextVisibility() != 0) {
											//^0CEE:1B5F
											si = 1;
										}
										//^0CEE:1B62
										break;
									}
								case 2:
									{
										//^0CEE:1B65
										si = 1;
										//^0CEE:1B68
										break;
									}
								case 4:
								case 7:
								case 8:
									{
										//^0CEE:1B6B
										di = bp04->TextVisibility();
										//^0CEE:1B77
										if (di != 0) {
											//^0CEE:1B7E
											si = 1;
											//^0CEE:1B81
											bp16 |= 1 << bp12;
										}
										//^0CEE:1B8C
										break;
									}
								case 0:
								case 6:
									{
										//^0CEE:1B49
										break;
									}
								case 1:
								case 3:
								case 9:
								case 10:
								case 11:
								case 12:
									{
										//^0CEE:1B49
										continue;
									}
							}
					}
				}
				//^0CEE:1B8F
				// TODO DM2_EXTENDED_DATABASE
				else if (dbtype == dbActuator) {
					Actuator *bp04 = _bp04->castToActuator();
					//^0CEE:1B98
					bp13 = GET_WALL_DECORATION_OF_ACTUATOR(bp04);
					//^0CEE:1BA8
					if (bp13 != 0xff) {
						//^0CEE:1BAF
						Bit16u bp18 = bp04->ActuatorType();
						//^0CEE:1BBC
						switch (bp18) {
							case ACTUATOR_TYPE_SHOP_PANEL: // 0x3f: 'Shop panel'
								{
									//^0CEE:1BD5
									if (bp06 == 5) {
										//^0CEE:1BDB
										ref->xvalue = (recordLink);
									}
									else {
										//^0CEE:1BE8
										ref->xvalue = (OBJECT_NULL);
									}
									//^0CEE:1BF1
									break;
								}
							case ACTUATOR_TYPE_CHAMPION_MIRROR: // SPX: Add for DM1 retrocompatibility / 0x7F: Activator, champion mirror
								{
									if (bp04->ActiveStatus() == 0) {
										if (bp06 == 5) {
											ref->xvalue = (bp04->ActuatorData());
										}
										bp16 |= 1 << bp12;
										si = 1;
									}
									else
										ref->xvalue = 0xFF;	 /// SPX: this is special to display the "glass" image when there is no champion.
										// this has to be done else ref->xvalue remains at 0, and portrait displayed is number 0 (i.e. Elija)
									break;
								}
							case ACTUATOR_TYPE_RESURECTOR: // 0x7e: 'Activator, resuscitation'
								{
									//^0CEE:1BF4
									if (bp04->OnceOnlyActuator() != 0) {
										//^0CEE:1C08
										if (bp06 == 5) {
											//^0CEE:1C0E
											ref->xvalue = (bp04->ActuatorData());
										}
										//^0CEE:1C1F
										bp16 |= 1 << bp12;
										//^0CEE:1C2A
										si = 1;
									}
									//^0CEE:1C2D
									break;
								}
							case ACTUATOR_TYPE_2_STATE_WALL_SWITCH: // 0x17: 'Activator, 2 state wall switch'
								{
									//^0CEE:1C30
									di = bp04->OnceOnlyActuator();
									//^0CEE:1C3F
									if (di != 0) {
										//^0CEE:1C46
										si = 1;
									}
									//^0CEE:1C49
									break;
								}
							case ACTUATOR_TYPE_WALL_SWITCH: // 0x18: 'Activator, push button wall switch'
								{
									//^0CEE:1C4C
									di = bp04->ActiveStatus() ^ bp04->OnceOnlyActuator();
									//^0CEE:1C63
									//^0CEE:1D12
									break;
								}
							case ACTUATOR_TYPE_KEY_HOLE: // 0x1a: 'Activator, key hole'
								{
									//^0CEE:1C66
									di = bp04->OnceOnlyActuator();
									//^0CEE:1C75
									if (di != 0) {
										//^0CEE:1C7C
										si = 1;
										//^0CEE:1C7F
										bp16 |= 1 << bp12;
									}
									//^0CEE:1C8A
									break;
								}
							case ACTUATOR_TYPE_X1B: // 0x1b: '-'
								{
									//^0CEE:1C8D
									di = bp04->ActiveStatus();
									//^0CEE:1C99
									if (bp04->OnceOnlyActuator() == di) {
										//^0CEE:1CA7
										si = 1;
									}
									//^0CEE:1CAA
									break;
								}
							case ACTUATOR_TYPE_ORNATE_ANIMATOR_2: // 0x32: 'Activator, ornate animator'
								{
									//^0CEE:1CAC
									if (bp04->ActiveStatus() != 0) {
										//^0CEE:1CBA
										bp0c = bp04->ActuatorData();
										//^0CEE:1CCC
										si = 1;
									}
									//^0CEE:1CCF
									break;
								}
							case ACTUATOR_TYPE_ORNATE_ANIMATOR: // 0x2c: 'Continuous ornate animator'
								{
									//^0CEE:1CD1
									if (bp04->ActiveStatus() != 0) {
										//^0CEE:1CDF
										bp10 = bp04->ActuatorData() & 0xff;
										//^0CEE:1CF4
                                        si = 1;											
									}
									//^0CEE:1CF7
									break;
								}
							case ACTUATOR_TYPE_ORNATE_STEP_ANIMATOR: // 0x41: 'Ornate step animator'
								{
									//^0CEE:1CF9
									di = bp04->ActuatorData();
									//^0CEE:1D06
									//^0CEE:1D12
									break;
								}
							case ACTUATOR_TYPE_PUSH_BUTTON_WALL_SWITCH: // 0x46: 'Activator, seal-able push button wall switch'
								{
									//^0CEE:1D08
									di = bp04->ActiveStatus();

									break;
								}
						}
					}
				}
				else {
					//^0CEE:1B95
                    continue;
				}
				//^0CEE:1D14
				if (si != 0) {
					//^0CEE:1D18
					di = QUERY_ORNATE_ANIM_FRAME(GDAT_CATEGORY_WALL_GFX, bp13, bp0c, bp10);
				}
				//^0CEE:1D33
				ref->tfoi[bp06 -3] = (di << 10) | bp13;
			}
		}
		//^0CEE:1D4C
	}
	//^0CEE:1D71
	return recordLink;
}


//^0CEE:1DBE
// SPX: _0cee_1dbe renamed SUMMARIZE_STONE_ROOM
void SkWinCore::SUMMARIZE_STONE_ROOM(ExtendedTileInfo *ref, Bit16u dir, Bit16u xx, Bit16u yy)
{
	// ExtendedTileInfo *ref, Bit16u ww, Bit16u xx, Bit16u yy
	// CSBWin:Codea59a.cpp/SummarizeStoneRoom

	//^0CEE:1DBE
	ENTER(36);
	//^0CEE:1DC4
	i16 ldir = dir;
	i16 dbtype = -1;	// SPX: note, at first, "si" was holding "dir", then "dbtype". Safer to distinguish them.
#if defined (XDMX_EXTENDED_FEATURES)
	if (SkCodeParam::bBWMode == true) {
		int tblFaceDir[8] = {0, 1, 2, 3, 0, 1, 2, 3};
		for (int f = 0; f < 4; f++)
			ref->interwalls[f] = GET_INTERWALL_VALUE(glbCurrentMapIndex, xx, yy, tblFaceDir[f+ldir]);
	}
#endif // XDMX_EXTENDED_FEATURES
	//^0CEE:1DC7
	ref->tfoi[0] = (0x00FF);
	ref->tfoi[1] = (0x00FF);
	ref->tfoi[2] = (0x00FF);
	ref->tfoi[3] = (0x00FF);
	ref->xvalue = (0x0000);
	//^0CEE:1DE8
	Bit8u bp05 = GET_TILE_VALUE(xx, yy);	// CSBWin cellFlags = GetCellFlags(rslt->x, rslt->y);
	//^0CEE:1DF7
	ref->w2 = (bp05);
	//^0CEE:1E00
	ObjectID di = GET_TILE_RECORD_LINK(xx, yy);	// CSBWin LOCAL_12 = FindFirstObject(rslt->x, rslt->y);
	//^0CEE:1E0E
	ref->w0 = (bp05 >> 5);
	//^0CEE:1E1E
	Bit16u bp10;
	Bit16u bp14;
	Bit16u bp0e;
	Bit16u bp0a;
	Bit16u bp08;
	Bit16u bp0c;
	Bit8u bp15;
	i16 bp18;
	i16 bp1a;
	Bit16u bp12;
	Bit16u bp1c;
	Bit16u bp1e;
	switch (ref->w0) {
		case ttWall:	// 0
			{
				//^0CEE:1E2F
				switch (ldir) {
					case 0:
						//^0CEE:1E40
						bp0e = bp05 & 8;
						bp0a = bp05 & 4;
						bp08 = bp05 & 2;
						bp0c = bp05 & 1;
						//^0CEE:1E63
						break;
					case 1:
						//^0CEE:1E65
						bp0e = bp05 & 4;
						bp0a = bp05 & 2;
						bp08 = bp05 & 1;
						bp0c = bp05 & 8;
						//^0CEE:1E88
						break;
					case 2:
						//^0CEE:1E8A
						bp0e = bp05 & 2;
						bp0a = bp05 & 1;
						bp08 = bp05 & 8;
						bp0c = bp05 & 4;
						//^0CEE:1EAD
						break;
					case 3:
						//^0CEE:1EAF
						bp0e = bp05 & 1;
						bp0a = bp05 & 8;
						bp08 = bp05 & 4;
						bp0c = bp05 & 2;

						break;
				}
				//^0CEE:1ED7
				SET_SQUARE_ASPECT_RANDOM_WALL_ORNAMENT_ORDINALS(ref, bp0e, bp0a, bp08, bp0c, ldir, xx, yy); // CSBWin : TAG00a4a2
				//^0CEE:1EF7
				bp10 = 0;
				//^0CEE:1EFC
_1efc:
				di = _0cee_1a46(ref, di, ldir, 0);
				//^0CEE:1F0F
				if (bp10 != 0 && cd.pi.glbPlayerPosX != xx && cd.pi.glbPlayerPosY != yy) {
					//^0CEE:1F25
					ref->w4 = (0xfffe);
					//^0CEE:1F2E
					//^0CEE:2353
					return;
				}
				//^0CEE:1F31
				if (bp10 != 0)
					//^0CEE:1F37
					break;
				//^0CEE:1F3A
				for (bp14 = 0; bp14 < 4; bp14++) {
					//^0CEE:1F41
					bp18 = xx;
					bp1a = yy;
					//^0CEE:1F4D
					bp18 += glbXAxisDelta[(ldir + bp14) & 3];
					//^0CEE:1F5F
					bp1a += glbYAxisDelta[(ldir + bp14) & 3];
					//^0CEE:1F71
					if ((GET_TILE_VALUE(bp18, bp1a) >> 5) == ttDoor) {
						//^0CEE:1F86
						ref->tfoi[bp14] = 0x00FF;
					}
					//^0CEE:1F98
				}
				//^0CEE:1FA1
				break;
			}
		case ttPit:		// 2
			{
				//^0CEE:1FA4
				if ((bp05 & 8) != 0) {
					//^0CEE:1FAA
					ref->tfoi[0] = (bp05 & 4);
					//^0CEE:1FB8
					goto _2237;
				}
				//^0CEE:1FBB
				ref->w0 = (1);
				//^0CEE:1FC3
				goto _2000;
			}
		case ttTrickWall:	// 6
			{
				//^0CEE:1FC5
				if ((bp05 & 4) == 0) {
					//^0CEE:1FD0
					ref->w0 = (0);
					//^0CEE:1FD8
					bp0c = bp08 = bp0a = bp0e = (bp05 & 8);
					//^0CEE:1FEB
					bp10 = 1;
					//^0CEE:1FF0
					goto _1efc;
				}
				//^0CEE:1FF3
				bp05 = 1;
				//^0CEE:1FF8
				ref->w0 = (bp05);

				goto _2000;
			}
		case ttFloor:	// 1
			{
				//^0CEE:2000
_2000:
				bp14 = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, dunMapLocalHeader->MapGraphicsStyle(), dtWordValue, GDAT_GFXSET_ANIMATED_FLOOR);
				//^0CEE:2020
				if (bp14 != 0) {	// SPX: That value is only used for VOID and is 0x8028; 0x28 is the floor animated blue cloud
					//^0CEE:2027
					bp15 = Bit8u(bp14);
					//^0CEE:202D
					if ((bp14 & 0x8000) != 0) {
						//^0CEE:2034
						bp12 = QUERY_ORNATE_ANIM_FRAME(GDAT_CATEGORY_FLOOR_GFX, bp15, glbGameTick, 0);
						//^0CEE:204D
						ref->tfoi[2] = (((bp12 * 10) << 8) | bp15);
						//^0CEE:2063
					}
					else {
						//^0CEE:2066
						ref->tfoi[2] = (bp15);
					}
				}
				if (false) {
					//^0CEE:2075
_2075:
					bp1c = 0;
					bp12 = 0;
					Bit32u bp20 = glbGameTick;
					Bit32u bp24 = 0;
					//^0CEE:2096
					GenericRecord *_bp04 = GET_ADDRESS_OF_RECORD(di);
					Text *bp04 = _bp04->castToText();
					SimpleActuator* xSAct = (SimpleActuator*) bp04; // SPX: to debug more easily the Text conversion structure
					//^0CEE:20A2
					if (dbtype != dbText)
						//^0CEE:20A7
						goto _212a;

					//^0CEE:20AA
					if (bp04->TextMode() == 1) {
						//^0CEE:20BE
						bp15 = Bit8u(bp04->OrnateIndex());
						//^0CEE:20CD
						switch (bp04->SimpleTextExtUsage()) {
							case 0x11: // 0x11 - ?
								{
									//^0CEE:20EE
									bp12 = ((bp05 & 0x08) != 0) ? 1 : 0;
									//^0CEE:20FE
									goto _21f4;
								}
							case 0x02: // 0x02 - Animate image (e.g. Water basin / fireplace / lantern)
								{
									//^0CEE:2101
									bp1c = 1;
									//^0CEE:2106
									goto _21f4;
								}
							case 0x05: // 0x05 - ?
								{
									//^0CEE:2109
									if (bp04->TextVisibility() != 0) {
										//^0CEE:211A
										bp1c = 1;
									}
									//^0CEE:211F
									goto _21f4;
								}
							case 0x0f: // 0x0f - ?
								{
									//^0CEE:2122
									bp12 = 1;
									//^0CEE:2127
									goto _21f4;

									//^0CEE:212A
_212a:
									// TODO DM2_EXTENDED_DATABASE
									if (dbtype != dbActuator)
										//^0CEE:212F
										break;
									//^0CEE:2132
									Actuator *bp04 = _bp04->castToActuator();
									if (bp04->ActuatorType() == ACTUATOR_FLOOR_TYPE__SHOP) { // 0x30 - Shop exhibition
										//^0CEE:2141
										ref->tfoi[3] = (0);
									}
									//^0CEE:214A
									bp15 = GET_FLOOR_DECORATION_OF_ACTUATOR(bp04);
									//^0CEE:215A
									if (bp15 == 0xff)
										//^0CEE:215E
										break;
									//^0CEE:2161
									switch (bp04->ActuatorType()) {
										case ACTUATOR_FLOOR_TYPE__ORNATE_ANIMATOR: // 0x32 - Activator, ornate animator
											//^0CEE:217C
											if (bp04->ActiveStatus() == 0)
												//^0CEE:2188
												break;
											//^0CEE:218A
											bp20 = bp04->ActuatorData();
											//^0CEE:2197
											bp1e = 0;
											bp1c = 1;
											//^0CEE:21A1
											break;

										case ACTUATOR_FLOOR_TYPE__CROSS_SCENE: // 0x27 - Cross scene
											//^0CEE:21A3
											if (bp04->ActuatorData() -1 != glbCurrentMapIndex)
												//^0CEE:21B5
												goto _222f;
											//^0CEE:21B7
											if (bp04->ActiveStatus() != 0) {
												//^0CEE:21C5
												bp1c = 1;
											}
											//^0CEE:21CA
											break;

										case ACTUATOR_FLOOR_TYPE__INFINITE_ORNATE_ANIMATOR: // 0x2c - Continuous ornate animator
											//^0CEE:21CC
											if (bp04->ActiveStatus() != 0) {
												//^0CEE:21DA
												bp24 = bp04->ActuatorData() & 0xff;
												bp1c = 1;
											}

											break;
									}
									//^0CEE:217A
									goto _21f4;
								}
							case 0x00: // 0x00 - Still image (e.g. An alcove)
							case 0x10: // 0x10 - ?
							case 0x0a: // 0x0a - Bush
								{
									//^0CEE:21F4
_21f4:
									if (bp1c != 0) {
										//^0CEE:21FA
										bp12 = QUERY_ORNATE_ANIM_FRAME(GDAT_CATEGORY_FLOOR_GFX, bp15, bp20, bp24);
									}
									//^0CEE:2216
									ref->tfoi[2] = (((bp12 * 10) << 8) | bp15);

									break;
								}
							case 0x01:
							case 0x03:
							case 0x04:
							case 0x06:
							case 0x07:
							case 0x08:
							case 0x09:
							case 0x0b:
							case 0x0c:
							case 0x0d:
							case 0x0e:
								{
									break;
								}
						}
					}
					//^0CEE:222F
_222f:
					di = GET_NEXT_RECORD_LINK(di);
				}
				//^0CEE:2237
_2237:
				if (di == OBJECT_END_MARKER)
					//^0CEE:223C
					break;
				//^0CEE:223F
				dbtype = di.DBType();
				//^0CEE:2249
				if (dbtype <= dbActuator)
					//^0CEE:224E
					goto _2075;
				//^0CEE:2251
				break;
			}
		case ttTeleporter:	// 5
			{
				//^0CEE:2254
				if ((bp05 & 8) == 0 || (bp05 & 4) == 0) {
					//^0CEE:226A
					ref->w0 = (1);
					ref->w2 &= 0xfff7;
				}
				//^0CEE:2278
				goto _2237;
			}
		case ttStairs:	// 3
			{
				//^0CEE:227A
				ref->w0 = ((((bp05 & 8) >> 3) == (ldir & 1)) ? 0x12 : 0x13);
				//^0CEE:229E
				ref->tfoi[0] = (bp05 & 4);
				//^0CEE:22A9
				goto _233a;
			}
		case ttDoor:	// 4
			{
				//^0CEE:22AC
				ref->tfoi[0] = (bp05 & 7);
				//^0CEE:22BA
				ref->tfoi[1] = (GET_TILE_RECORD_LINK(xx, yy));
				//^0CEE:22CD
				if (((bp05 & 8) >> 3) == (ldir & 1)) {
					//^0CEE:22E3
					ref->w0 = (0x0010);	// Set the north/west direction of door frame
				}
				else {
					//^0CEE:22EA
					ref->w0 = (0x0011);
					//^0CEE:22F2
					ldir++;
					ldir &= 3;
					//^0CEE:22F7
					bp18 = xx + glbXAxisDelta[ldir];
					//^0CEE:2305
					bp1a = yy + glbYAxisDelta[ldir];
					//^0CEE:2313
					_0cee_1a46(
						ref,
						GET_TILE_RECORD_LINK(bp18, bp1a),
						ldir,
						1
						);
				}
				//^0CEE:2330
				goto _233a;

_233a:
				// TODO DM2_EXTENDED_DATABASE
				while (di != OBJECT_END_MARKER && di.DBType() <= dbActuator) {
					//^0CEE:2332
					di = GET_NEXT_RECORD_LINK(di);
					//^0CEE:233A
				}

				break;
			}
	}
	//^0CEE:234C
	ref->w4 = (di);
	//^0CEE:2353
	return;		
}

//^29EE:0BCC
i16 SkWinCore::QUERY_DUNGEON_MAP_CHIP_PICT(Bit8u cls1, Bit8u cls2, Bit8u **ppBuff, Bit8u *localpal)
{
	//^29EE:0BCC
	ENTER(0);
	//^29EE:0BCF
	if (QUERY_GDAT_ENTRY_IF_LOADABLE(cls1, cls2, dtImage, GDAT_IMG_MAP_CHIP) != 0) {
		//^29EE:0BE8
		*ppBuff = QUERY_GDAT_IMAGE_ENTRY_BUFF(cls1, cls2, GDAT_IMG_MAP_CHIP);
		//^29EE:0C05
		COPY_MEMORY(
			QUERY_GDAT_IMAGE_LOCALPAL(cls1, cls2, GDAT_IMG_MAP_CHIP),
			localpal,
			16
			);
		//^29EE:0C2C
		return READ_I16(*ppBuff,-4) / glbMagicMapWidth;
	}
	//^29EE:0C3D
	return 0;
}


//^2FCF:2DA7
// SPX: _2fcf_2da7 renamed SEARCH_DUNGEON_FOR_SPECIAL_MARKER
// Called when falling from "the void" zone
// This looks to be also called by the X teleporter
// tt is 0 in general cases. only some random value for ss = 2; this is a number of "tries" to find a valid destination
i16 SkWinCore::SEARCH_DUNGEON_FOR_SPECIAL_MARKER(U16 ss, U16 tt, U16 uu, i16 *xx, i16 *yy)
{
	//^2FCF:2DA7
	ENTER(24);
	//^2FCF:2DAD
	U16 bp16 = 0;
	U16 bp18 = 0xffff;
	SkD((DLV_FSM, "Search for special markers: type %d (tries:%d, uu:%d)\n"
		,ss, tt, uu));
	//^2FCF:2DB7
	for (U16 si = 0; dunHeader->nMaps > si; si++) {
		//^2FCF:2DBC
		U16 bp12 = dunMapsHeaders[si].RawColumn();
		//^2FCF:2DD4
		U16 bp14 = dunMapsHeaders[si].RawRow();
		//^2FCF:2DEC
		U8 *bp04 = glbMapTileValue[si][0];	// pointer on tile
		//^2FCF:2E07
		OID_T *bp0c = &dunGroundStacks[dunMapTilesObjectIndexPerColumn[dunMapColumnsSumArray[si]]];	// U16 *bp0c
		//^2FCF:2E31
		for (U16 bp10 = 0; bp10 <= bp12; bp10++) {
			//^2FCF:2E39
			for (U16 di = 0; di <= bp14; di++) {
				//^2FCF:2E3E
				if ((*(bp04++) & 0x10) != 0) {	// is there an object on tile ? (0x10 flag)
					//^2FCF:2E4D
					ObjectID bp0e = *(bp0c++);
					do {
						//^2FCF:2E5A
						switch (ss) {	// SPX: but ... there is no distinctive case then ?
							case SDFSM_CMD_FIND_MAP_OFFVOID: // 2: 2nd pass void fall : stop on one special marker
							case SDFSM_CMD_COUNT_MARKERS: // 3: 1st pass void fall : count how many special markers
							case SDFSM_CMD_X_TELEPORTER:	// 4: x teleporter
							case SDFSM_CMD_X_ANCHOR:	// 5: anchor x teleporter
								//^2FCF:2E6E
								if (bp0e.DBType() == dbText) {
									//^2FCF:2E7F
									Text *bp08 = GET_ADDRESS_OF_RECORD2(bp0e);
									//^2FCF:2E8E
									if (bp08->TextMode() == 1) {
										//^2FCF:2EA2
										switch (bp08->SimpleTextExtUsage()) {
											case 11:	// SPX: This is a special marker for LAND POINT after falling from void
														// This correspond to the 0x5800 value in DMDC2
												//^2FCF:2EC4
												if ((bp08->TextIndex() & 0xff) != uu)	// ??
													//^2FCF:2ED7
													break;
												//^2FCF:2ED9
												// SPX: Increment the number of special markers found
												if (ss == SDFSM_CMD_COUNT_MARKERS) {
													//^2FCF:2EDF
													bp16++;
													//^2FCF:2EE2
													break;
												}
												//^2FCF:2EE4
												tt--;	// Decrements the number of tries
												//^2FCF:2EE7
												//if (tt != 0)	// If this is not yet 0, break = continue searching next marker
												if (tt <= 0)	// SPX: FIX, this is good when tt becomes 0 or less!
													//^2FCF:2EEC
													break;
												//^2FCF:2EEE
_2eee:	// logically when we get here, we have found the required marker, then, return the current map (si) where this marker is
												*xx = bp10;
												//^2FCF:2EF7
												*yy = di;
												//^2FCF:2EFD
												return si;

											case 16:
												//^2FCF:2F02
												if ((bp08->TextIndex() & 0xff) != uu)
													//^2FCF:2F15
													break;
												//^2FCF:2F17
												*xx = bp10;
												//^2FCF:2F20
												*yy = di;
												//^2FCF:2F26
												bp18 = si;
												//^2FCF:2F29
												break;

											case 15:
												//^2FCF:2F2B
												if (ss == SDFSM_CMD_X_TELEPORTER) {
													//^2FCF:2F31
													bp08->TextVisibility(0);
													//^2FCF:2F39
													break;
												}
												//^2FCF:2F3B
												if (ss != SDFSM_CMD_X_ANCHOR)
													//^2FCF:2F3F
													break;
												//^2FCF:2F41
												if (bp08->TextVisibility() == 0)
													//^2FCF:2F4D
													break;
												//^2FCF:2F4F
												goto _2eee;
										}
									}
								}

								break;
						}
						//^2FCF:2F51
						bp0e = GET_NEXT_RECORD_LINK(bp0e);
						//^2FCF:2F5D
					} while (bp0e != OBJECT_END_MARKER && bp0e.DBType() <= dbActuator);
				}
				//^2FCF:2F70
			}
			//^2FCF:2F79
		}
		//^2FCF:2F87
	}
	//^2FCF:2F99
	CHANGE_CURRENT_MAP_TO(cd.pi.glbPlayerMap);
	//^2FCF:2FA3
	if (ss == SDFSM_CMD_X_TELEPORTER) {	// 4 = cross teleporter
		//^2FCF:2FA9
		return bp18;	// must be the map returned
	}
	//^2FCF:2FAE
	if (ss == SDFSM_CMD_X_ANCHOR) {	// 5 = anchor teleporter (the one in the sun clan village)
		//^2FCF:2FB4
		return -1;
	}
	//^2FCF:2FB9
	return bp16;	// return the number of special markers found (should be only for ss = 3)
}




//^0CEE:317F
U16 SkWinCore::IS_WALL_ORNATE_ALCOVE_FROM_RECORD(ObjectID rl)
{
	// SPX: Add protection : if object is not a simple actuator or actuator, then exit!
	if (rl.RealDBType() != DB_CATEGORY_SIMPLE_ACTUATOR && rl.RealDBType() != DB_CATEGORY_ACTUATOR)
		return 0;
	//^0CEE:317F
	ENTER(0);
	//^0CEE:3182
	return (GET_WALL_ORNATE_ALCOVE_TYPE(QUERY_CLS2_FROM_RECORD(rl)) == WALL_ORNATE_OBJECT__ALCOVE) ? 1 : 0;
	//return (GET_WALL_ORNATE_ALCOVE_TYPE(QUERY_CLS2_FROM_RECORD(rl)) == 1) ? 1 : 0;
}

//^0CEE:0AE1
ObjectID SkWinCore::GET_WALL_TILE_ANYITEM_RECORD(U16 xx, U16 yy)
{
	//^0CEE:0AE1
	ENTER(0);
	//^0CEE:0AE5
	ObjectID si = GET_TILE_RECORD_LINK(xx, yy);
	//^0CEE:0AF1
	while (si.DBType() <= dbActuator) {
		//^0CEE:0AF3
		si = GET_NEXT_RECORD_LINK(si);
		//^0CEE:0AF9
	}
	//^0CEE:0B08
	return si;
}

//^0CEE:31D7
U16 SkWinCore::IS_WALL_ORNATE_SPRING(ObjectID rl)
{
	// SPX: Add protection : if object is not a simple actuator or actuator, then exit!
	// (case was Drumstick (misc id = 35) leading to fountain (wall id = 35)
	if (rl.RealDBType() != DB_CATEGORY_SIMPLE_ACTUATOR && rl.RealDBType() != DB_CATEGORY_ACTUATOR)
		return 0;
	//^0CEE:31D7
	ENTER(2);
	//^0CEE:31DB
	U8 bp01 = QUERY_CLS2_FROM_RECORD(rl);
	//^0CEE:31E6
	if (bp01 == 0xff)
		//^0CEE:31EA
		return 0;
	//^0CEE:31EE
	return QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp01, dtWordValue, GDAT_WALL_ORNATE__IS_WATER_SPRING);
}




//^2FCF:16B8
ObjectID SkWinCore::FIND_DISTINCTIVE_ITEM_ON_TILE(U16 xx, U16 yy, i16 dir, U16 disit)
{
	//^2FCF:16B8
	ENTER(0);
	//^2FCF:16BC
	for (ObjectID si = GET_WALL_TILE_ANYITEM_RECORD(xx, yy); si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
		//^2FCF:16CB
		if (GET_DISTINCTIVE_ITEMTYPE(si) == disit) {
			//^2FCF:16D7
			if (dir == -1 || si.Dir() == dir) {
				//^2FCF:16E7
				return si;
			}
		}
		//^2FCF:16EB
	}
	//^2FCF:16F9
	return OBJECT_NULL;
}

//^2FCF:0C7D
X16 SkWinCore::IS_TILE_BLOCKED(U8 tile)
{
	// return 0 if you can enter
	// return 1 if you cannot enter

	//^2FCF:0C7D
	ENTER(0);
	//^2FCF:0C80
	U8 dl = tile;
	switch (dl >> 5) {
		case ttWall://^0C9A
		case ttMapExit://^0C9A
			return 1;
		case ttDoor://^0C9F
			dl = dl & 7;
			if (dl != 0 && dl != 1 && dl != 5)
				return 1;
			break;
		case ttTrickWall://^0CB5
			if ((dl & 4) == 0 && (dl & 1) == 0)
				return 1;
			break;
		case ttFloor://^0CCB
		case ttPit://^0CCB
		case ttStairs://^0CCB
		case ttTeleporter://^0CCB
			break;
	}
	return 0;
}




//^0CEE:0897
U8 SkWinCore::_0cee_0897(Teleporter **ref, i16 xx, i16 yy)
{
	//^0CEE:0897
	ENTER(0);
	//^0CEE:089B
	if ((GET_TILE_VALUE(xx, yy) >> 5) == ttTeleporter) {
		//^0CEE:08B0
		ObjectID si = GET_TILE_RECORD_LINK(xx, yy);
		*ref = GET_ADDRESS_OF_RECORD1(si);
		while ((si = GET_NEXT_RECORD_LINK(si)) != OBJECT_END_MARKER) {
			//^0CEE:08D2
			if (si.DBType() != dbActuator)
				continue;
			if (GET_ADDRESS_OF_ACTU(si)->ActuatorType() != ACTUATOR_FLOOR_TYPE__CROSS_SCENE) // 0x27: Cross scene
				continue;
			//^0CEE:08F6
			return (((*ref)->Rotation() +2)&3) +1;
			//^0CEE:090E
		}
	}
	//^0CEE:091C
	return 0;
}

//^0CEE:0921
U8 SkWinCore::GET_TELEPORTER_DETAIL(TELE_inf *ref, i16 xx, i16 yy)
{
	//^0CEE:0921
	ENTER(10);
	//^0CEE:0927
	X16 si = 0;
	Teleporter *bp06;
	U8 bp01 = _0cee_0897(&bp06, xx, yy);
	if (bp01 != 0) {
		//^0CEE:0945
		X16 di = glbCurrentMapIndex;
		CHANGE_CURRENT_MAP_TO(bp06->b5_0_7());
		Teleporter *bp0a;
		U8 bp02 = _0cee_0897(&bp0a, bp06->DestinationX(), bp06->DestinationY());
		if (bp02 != 0) {
			//^0CEE:0982
			si = 1;
			ref->b0 = bp01 -1;
			ref->b1 = bp02 -1;
			ref->b2 = bp06->DestinationX();
			ref->b3 = bp06->DestinationY();
			ref->b4 = bp06->b5_0_7();
		}
		//^0CEE:09CF
		CHANGE_CURRENT_MAP_TO(di);
	}
	//^0CEE:09D5
	return U8(si);
}


//^0CEE:0A43
// CSBWin TAG009a1e i16 GetObjectListIndex(i32 mapX,i32 mapY)
ObjectID SkWinCore::GET_OBJECT_INDEX_FROM_TILE(i16 xx, i16 yy)
{
	//^0CEE:0A43
	ENTER(4);
	//^0CEE:0A49
	i16 cx = xx;
	i16 di = yy;
	U8 *bp04 = NULL;

	// SPX: First we do coords check before getting the pointer and prevent bad mem pointer
	if (cx < 0 || cx >= glbCurrentMapWidth || di < 0 || di >= glbCurrentMapHeight)
		//^0CEE:0A8B
		return OBJECT_NULL;

	//^0CEE:0A67
	// SPX: Here, some crash because glbCurrentTileMap[cx] can be corrupted for some unknown reason ...
	// In case the pointer is corrupt, just return null ...
	bp04 = glbCurrentTileMap[cx];
	if (SkCodeParam::bUsePowerDebug && !CheckSafePointer(bp04))
		return OBJECT_NULL;
	if ((bp04[di] & 0x10) == 0) // tile has not object flag on it
		return OBJECT_NULL;
	//^0CEE:0A90
	U16 si = _4976_4c52[cx];
	while (--di >= 0) {
		//^0CEE:0A9F
		if ((*(bp04++) & 0x10) != 0)
			//^0CEE:0AAB
			si++;
	}
	//^0CEE:0AAF
	return si;
}

//^0CEE:0CB2
void SkWinCore::CUT_RECORD_FROM(ObjectID recordLink, ObjectID *recordLinkLookFor, i16 xposLookFor, i16 yposLookFor)
{
	// Cut the record from the target.
	// The record is specified by recordLink.
	// The target is one of recordLinkLookFor or a pair of xposLookFor&yposLookFor.

	SkD((DLV_DBM, "DBM: CUT_RECORD_FROM(%04X,%p(%04X),%2d,%2d)\n"
		, (Bitu)recordLink.w
		, recordLinkLookFor
		, (recordLinkLookFor != NULL) ? (Bitu)recordLinkLookFor->w : 0
		, (Bitu)xposLookFor
		, (Bitu)yposLookFor));

	ATLASSERT(false
		|| (xposLookFor == -1 && yposLookFor == 0) // open the chest?
		|| ((U16)xposLookFor < (U16)glbCurrentMapWidth && (U16)yposLookFor < (U16)glbCurrentMapHeight)
		);

	//^0CEE:0CB2
	ObjectID si = recordLink;
	//^0CEE:0CBB
	if (si == OBJECT_END_MARKER || si == OBJECT_NULL)
		return;
	//^0CEE:0CCB
	si.ClearDir();
	GenericRecord *bp08 = (GenericRecord *)GET_ADDRESS_OF_RECORD(si);
	//^0CEE:0CDB
	ObjectID *bp0c;
	if (xposLookFor >= 0) {
		//^0CEE:0CE4
		Bit16u bp0e = GET_OBJECT_INDEX_FROM_TILE(xposLookFor, yposLookFor);
		OID_T *bp04 = &dunGroundStacks[bp0e]; // Bit16u *bp04
		//^0CEE:0D05
		if (bp08->w0 == OBJECT_END_MARKER && ObjectID(*bp04).GetAsNorth() == si) {
			//^0CEE:0D21
			glbCurrentTileMap[xposLookFor][yposLookFor] &= 0xef;
			//^0CEE:0D4D
			Bit16u di = dunHeader->cwListSize -1;
			MOVE_MEMORY(
				bp04 +1,
				bp04,
				(di -bp0e) << 1
				);
			//^0CEE:0D77
			dunGroundStacks[di] = 0xffff;
			//^0CEE:0D86
			bp04 = &_4976_4c52[xposLookFor +1];
			//^0CEE:0D9D
			di = _4976_4cb4 - (dunMapColumnsSumArray[glbCurrentMapIndex] + xposLookFor) -1;
			//^0CEE:0DB7
			while (di-- != 0) {
				//^0CEE:0DB9
				(*bp04)--;
				bp04++;
				//^0CEE:0DC3
			}
			//^0CEE:0DCA
			return;
		}
		//^0CEE:0DCC
		if (ObjectID(*bp04).GetAsNorth() == si) {
			//^0CEE:0DD9
			Bit16u ax = bp08->w0;
			//^0CEE:0DE2
			//^0CEE:0E35
			*bp04 = ax;
			bp08->w0 = OBJECT_END_MARKER;
			return;
		}
		//^0CEE:0DE4
		//^0CEE:0DEA
		bp0c = reinterpret_cast<ObjectID *>(bp04);

		//ATLASSERT(*bp04 != 0xFFFE && *bp04 != 0xFFFF);
		ATLASSERT(*bp04 != OBJECT_END_MARKER && *bp04 != OBJECT_NULL);
	}
	else {
		//^0CEE:0DF2
		//^0CEE:0DEA
		bp0c = recordLinkLookFor;
	}
	//^0CEE:0E0B
	while (*bp0c != OBJECT_END_MARKER && bp0c->GetAsNorth() != si) {
		bp0c = &GET_ADDRESS_OF_RECORD(*bp0c)->w0;
	}
	//^0CEE:0E1E
	if (*bp0c != OBJECT_END_MARKER) {
		//^0CEE:0E27
		*bp0c = GET_NEXT_RECORD_LINK(*bp0c);
	}
	//^0CEE:0E38
	bp08->w0 = OBJECT_END_MARKER;
}


	//^0CEE:2614
Bit16u SkWinCore::GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(Bit16u actuatorData)
{
	//^0CEE:2614
	//^0CEE:2618
	Bit16u si = actuatorData & 0x01ff;
	//^0CEE:261F
	switch (si >> 7) {
		case 0:
			//^0CEE:2630
			return dbWeapon;
		case 1:
			//^0CEE:2635
			return dbCloth;
		case 2:
			//^0CEE:263A
			return dbMiscellaneous_item;
		case 3:
			//^0CEE:263F
			if (si > 0x01fc)
				break;
			//^0CEE:2645
			if (si == 0x01fc) {
				//^0CEE:264B
				return dbScroll;
			}
			//^0CEE:2650
			if (si >= 0x01e0) {
				//^0CEE:2656
				return dbContainer;
			}
			//^0CEE:265B
			if (si >= 0x01b0) {
				//^0CEE:2661
				return dbCreature;
			}
			//^0CEE:2666
			return dbPotion;
	}
	//^0CEE:266B
	return 0xffff;
}

//^0CEE:2679
Bit8u SkWinCore::GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(Bit16u actuatorData)
{
	// returns a value ranged from 0 to 127, as an item type.

	//^0CEE:2679
	//^0CEE:267C
	Bit16u dx = (actuatorData & 0x01ff);
	switch (dx >> 7) {
		case 1:
			//^0CEE:2699
			dx -= 0x0080;
			break;
		case 2:
			//^0CEE:269F
			dx -= 0x0100;
			break;
		case 3:
			//^0CEE:26A5
			if (dx >= 0x01fc) {
				//^0CEE:26AB
				dx = 0;
				break;
			}
			//^0CEE:26AF
			if (dx >= 0x01e0) {
				//^0CEE:26B5
				dx -= 0x01e0;
				break;
			}
			//^0CEE:26BB
			if (dx >= 0x01b0) {
				//^0CEE:26C1
				dx -= 0x01b0;
				break;
			}
			//^0CEE:26C7
			dx -= 0x0180;
	}
	//^0CEE:26CB
	return (Bit8u)dx;
}



//^0CEE:06DC
// SPX TODO related to direction depending on tile (like changing stairs ?)
// _0cee_06dc renamed _0cee_06dc_GET_TILE_DIRECTION
Bit16u SkWinCore::_0cee_06dc_GET_TILE_DIRECTION(i16 xx, i16 yy)
{
	//^0CEE:06DC
	ENTER(4);
	//^0CEE:06E2
	X16 di = xx;
	X16 si = yy;
	X16 bp02 = !(GET_TILE_VALUE(di, si) & 8);
	if (bp02 != 0) {	// west/east
		di += glbXAxisDelta[1];
		si += glbYAxisDelta[1];
	}
	else {	// north/south
		di += glbXAxisDelta[0];
		si += glbYAxisDelta[0];
	}
	//^0CEE:0712
	X16 bp04 = GET_TILE_VALUE(di, si) >> 5;
	//return bp02 + (((bp04 == ttWall || bp04 == ttStairs) ? 1 : 0) << 1); // SPX: I don't think its correct here to consider stairs as same as wall against stairs !
	return bp02 + (((bp04 == ttWall) ? 1 : 0) << 1);
}

//^0CEE:319E
// _0cee_319e renamed _0cee_319e_ALCOVE_GET_GDAT_X13
X16 SkWinCore::_0cee_319e_ALCOVE_GET_GDAT_X13(ObjectID rl)
{
	//^0CEE:319E
	ENTER(2);
	//^0CEE:31A2
	if (IS_OBJECT_ALCOVE(rl) == 0)	// _0cee_317f
		return 0;	// not an alcove, return
	//^0CEE:31B2
	U8 bp01 = QUERY_CLS2_FROM_RECORD(rl);
	if (bp01 == 0xff)
		return 0;
	//^0CEE:31C3
	return QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp01, dtWordValue, GDAT_WALL_ORNATE__DATA_13);
}


//^32CB:4185
// SPX: _32cb_4185 renamed _32cb_4185_TILE_ROOM
void SkWinCore::_32cb_4185_TILE_ROOM(i16 xx, i16 yy, i16 cellPos, i16 dir)
{
	//^32CB:4185
	ENTER(34);
	//^32CB:418B
	i16 bp06 = xx;
	i16 bp08 = yy;
	CALC_VECTOR_W_DIR(dir, _4976_40e8[cellPos][1], _4976_40e8[cellPos][0], &bp06, &bp08);
	tblCellTilesRoom[cellPos].posx = U8(bp06);
	tblCellTilesRoom[cellPos].posy = U8(bp08);
	//^32CB:41EB
	SUMMARIZE_STONE_ROOM(&tblCellTilesRoom[cellPos].xsrd, dir, bp06, bp08);
	if (cd.pi.glbIsPlayerMoving != 0 && cellPos == 0) {
#if defined (XDMX_EXTENDED_FEATURES)
		tblCellTilesRoom->xsrd.interwalls[0] = 0;
		tblCellTilesRoom->xsrd.interwalls[1] = 0;
		tblCellTilesRoom->xsrd.interwalls[2] = 0;
		tblCellTilesRoom->xsrd.interwalls[3] = 0;
#endif // XDMX_EXTENDED_FEATURES
		//^32CB:4224
		if (tblCellTilesRoom->xsrd.w0 == 0) {
			if (tblCellTilesRoom[3].xsrd.w0 == 5) {
				//^32CB:4239
				tblCellTilesRoom->xsrd.w0 = (5);
			}
			else {
				//^32CB:4241
				tblCellTilesRoom->xsrd.tfoi[0] = (0xff);
				tblCellTilesRoom->xsrd.tfoi[1] = (0xff);
				tblCellTilesRoom->xsrd.tfoi[2] = (0xff);
				tblCellTilesRoom->xsrd.tfoi[3] = (0xff);
				//tblCellTilesRoom->xsrd.w6[4] = (0);	// is that right ????
				tblCellTilesRoom->xsrd.xvalue = 0;	// SPX: replaced w6[4] with xvalue
				tblCellTilesRoom->xsrd.w4 = (0xfffe);
			}
			//^32CB:4269
			X16 si;
			for (si = 1; si <= 2; si++) {
				//^32CB:426F
				ExtendedTileInfo *bp04 = &tblCellTilesRoom[si].xsrd;
				bp04->w4 = (0xfffe);
				bp04->w2 = (tblCellTilesRoom[si +3].xsrd.w2 & 0xe0);
				X16 bp22;
				bp04->w0 = (bp22 = tblCellTilesRoom[si +3].xsrd.w0);
				switch (bp22) {
					case 0x00://^42E4
						//^32CB:42E4
						bp04->tfoi[3] = (255);
						bp04->tfoi[2] = (255);
						bp04->tfoi[1] = (255);
						bp04->tfoi[0] = (255);
						bp04->xvalue = (0xffff);
						break;
					case 0x02://^4302
						//^32CB:4302
						bp04->w0 = (1);
						bp04->w2 = (0);
						break;
					case 0x01://^4312
					case 0x05://^4312
					case 0x10://^4312
						//^32CB:4312
						bp04->tfoi[2] = (0xff);
						break;
				}
				//^32CB:431B
				(*_4976_5a7c)[si] = 0;
				(*_4976_5be2)[si] = 0;
				//^32CB:4347
			}
		}
		//^32CB:4352
		else if (tblCellTilesRoom->xsrd.w0 == 0x11) {
			//^32CB:435D
			tblCellTilesRoom->xsrd.w0 = (1);
			tblCellTilesRoom->xsrd.w2 = (0);
			tblCellTilesRoom->xsrd.tfoi[2] = (0xff);
		}
	}
	//^32CB:436F
	(*_4976_5a7c)[cellPos] = 0;
	(*_4976_5be2)[cellPos] = 0;
	if (cellPos < 0x10 && tblCellTilesRoom[cellPos].xsrd.w0 != 0) { // if it is not wall
		//^32CB:43B8
		U8 *bp1e = &_4976_5a84[_4976_44c5[RCJ(16,cellPos)][0]][_4976_44c5[RCJ(16,cellPos)][1]];
		i16 si;
		//^32CB:43E9
		for (si = 0; si < 5; bp1e += 17, si++) {
			//^32CB:43ED
			i16 bp20;
			//^32CB:43ED
			for (bp20 = 0; bp20 < 4; bp20++) {
				//^32CB:43F4
				bp1e[bp20] = U8(cellPos);
				//^32CB:4400
			}
			//^32CB:4409
		}
	}
	//^32CB:4413
	ObjectID bp18 = tblCellTilesRoom[cellPos].xsrd.id4();
	if (!SkCodeParam::bUsePowerDebug)
		ATLASSERT(bp18 != OBJECT_NULL);
	if (bp18 != OBJECT_END_MARKER) {
		//^32CB:4430
		U16 bp0c;
		U16 bp0e;
		_098d_000f(xx, yy, _4976_44c1[RCJ(4,dir)], &bp0c, &bp0e);
		do {
			//^32CB:4451
			i16 bp1a = bp18.DBType();
			if (bp1a == dbCreature) {
				//^32CB:4465
				i16 bp16;
				if (_4976_5aa2 != 0 && cellPos == 3) {
					//^32CB:4472
					bp16 = _4976_41d0[RCJ(7,glbTargetTypeMoveObject)];
					bp16 = ROTATE_5x5_POS(bp16, _4976_44bd[RCJ(4,dir)]);
				}
				else {
					//^32CB:4492
					bp16 = QUERY_OBJECT_5x5_POS(bp18, 0);
				}
				//^32CB:44A1
				U16 bp10;
				U16 bp12;
				_098d_000f(bp06, bp08, bp16, &bp10, &bp12);
				Creature *bp04 = GET_ADDRESS_OF_RECORD4(bp18);
				if (cellPos != 0) {
					//^32CB:44D1
					X16 bp14 = QUERY_CREATURE_AI_SPEC_FROM_TYPE(bp04->CreatureType())->b34;
					_32cb_4069(bp0c, bp0e, bp14, &bp10, &bp12);
				}
				//^32CB:4505
				i16 bp0a;
				i16 di;
				switch (dir) {
					case 0: //^_4518
						//^32CB:4518
						bp0a = bp10 -bp0c;
						di = bp0e -bp12;
						break;
					case 1: //^_4529
						bp0a = bp12 -bp0e;
						di = bp10 -bp0c;
						break;
					case 2: //^_453a
						//^32CB:453A
						bp0a = bp0c -bp10;
						di = bp12 -bp0e;
						break;
					default: //^_454b
						//^32CB:454B
						bp0a = bp0e -bp12;
						di = bp0c -bp10;
						break;
				}
				//^32CB:455C
				if (di < 0x11 && ABS16(bp0a) < 10) {
					while (di >= 0 && _4976_5aa4[bp0a +10][di] != 0xff) {
						//^32CB:4577
						di--;
					}
					//^32CB:4593
					if (di >= 0) {
						//^32CB:459A
						_4976_5aa4[bp0a +10][di] = U8(cellPos);
					}
				}
			}
			//^32CB:45B4
			else if (bp1a == dbMissile) {
				//^32CB:45BA
				(*_4976_5a7c)[cellPos] |= U32(1) << QUERY_OBJECT_5x5_POS(bp18, _4976_5aa0);
			}
			//^32CB:45E7
			else if (bp1a >= dbWeapon && bp1a <= dbMiscellaneous_item) {
				//^32CB:45F3
				// SPX: Added here w0 == 17 (door tile) so that item on door tile is always displayed, even if door is open
				// w6[0] represents open door state where 0 is fully open and 1 to 4 semi closed to closed status; so when w6[0] != 0 means does not take when door is open! (why?)
				if (tblCellTilesRoom[cellPos].xsrd.w0 == 1 || tblCellTilesRoom[cellPos].xsrd.w0 == 17 || tblCellTilesRoom[cellPos].xsrd.tfoi[0] != 0) {
//					if (_4976_5a80[cellPos].x2.w0 == 1 || _4976_5a80[cellPos].x2.w6[0] != 0) {
					//^32CB:461D
					(*_4976_5be2)[cellPos] |= U32(1) << QUERY_OBJECT_5x5_POS(bp18, _4976_5aa0);
				}
			}
			//^32CB:4654
		} while ((bp18 = GET_NEXT_RECORD_LINK(bp18)) != OBJECT_END_MARKER);
	}
	//^32CB:4669
	return;
}


//^0CEE:3202
// SPX: _0cee_3202 renamed IS_REBIRTH_ALTAR()
Bit16u SkWinCore::IS_REBIRTH_ALTAR(ObjectID rl)
{
	//^0CEE:3202
	ENTER(2);
	U8 isViAltar = 0;
	//^0CEE:3206
	U8 wallOrnateNo = QUERY_CLS2_FROM_RECORD(rl);	// bp01
	//^0CEE:3211
	if (wallOrnateNo == 0xff)
		//^0CEE:3215
		return 0;
	//^0CEE:3219
	// SPX: This value was present in no wall graphics since VI ALTAR has been dropped in DM2; however, it works!
	isViAltar = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, wallOrnateNo, dtWordValue, GDAT_WALL_ORNATE__IS_REBIRTH_ALTAR); //0x0c
	return isViAltar;
}


//^0CEE:317F
//SPX: _0cee_317f renamed IS_OBJECT_ALCOVE
X16 SkWinCore::IS_OBJECT_ALCOVE(ObjectID rl)
{
	//^0CEE:317F
	ENTER(0);
	//^0CEE:3182
	if (GET_WALL_ORNATE_ALCOVE_TYPE(QUERY_CLS2_FROM_RECORD(rl)) == WALL_ORNATE_OBJECT__ALCOVE) // == 1
		return 1;
	return 0;
}


//^0CEE:14D9
ObjectID SkWinCore::ALLOC_NEW_DBITEM(Bit16u itemspec)
{
	SkD((DLV_DBM, "DBM: ALLOC_NEW_DBITEM(%04X)\n"
		, (Bitu)itemspec));

	//^0CEE:14D9
	//^0CEE:14DE
	Bit16u di = itemspec;
	//^0CEE:14E1
	ObjectID si = ALLOC_NEW_RECORD(GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(di) | (di & 0x8000));
	//^0CEE:14F8
	if (si != OBJECT_NULL) {
		//^0CEE:14FD
		SET_ITEMTYPE(si, GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(di));
	}
	//^0CEE:150D
	return si;
}

//^0CEE:30A6
Bit8u SkWinCore::GET_WALL_DECORATION_OF_ACTUATOR(Actuator *ref)
{
	//^0CEE:30A6
	Bit16u si = ref->GraphicNumber();
	if (si != 0) {
		//^0CEE:30BE
		Bit8u *bp04 = &glbCurrentTileMap[glbCurrentMapWidth -1][glbCurrentMapHeight + dunMapLocalHeader->CreaturesTypes()];
		//^0CEE:30ED
		return bp04[si -1];
	}
	//^0CEE:30F6
	return -1;
}

//^0CEE:30FB
Bit8u SkWinCore::GET_FLOOR_DECORATION_OF_ACTUATOR(Actuator *ref)
{
	//^0CEE:30FB
	//^0CEE:3100
	Bit16u si = ref->GraphicNumber();
	//^0CEE:310F
	if (si != 0) {
		//^0CEE:3113
		Bit8u *bp04 = &glbCurrentTileMap[glbCurrentMapWidth -1][glbCurrentMapHeight + dunMapLocalHeader->CreaturesTypes() + dunMapLocalHeader->WallGraphics()];
		//^0CEE:3150
		return bp04[si -1];
	}
	else {
		//^0CEE:3159
		return 0xff;
	}
}


//^2FCF:2444
// SPX: _2fcf_2444	renamed	PLACE_OR_REMOVE_OBJECT_IN_ROOM			(CSBWin:Code11f52.cpp/PlaceOrRemoveObjectInRoom)
void SkWinCore::PLACE_OR_REMOVE_OBJECT_IN_ROOM(i16 xpos, i16 ypos, ObjectID recordLink, Bit16u ss, Bit16u place, Bit16u uu)
{
	// CSBWin:Code11f52.cpp/PlaceOrRemoveObjectInRoom (TAG013380)
	// PlaceOrRemoveObjectInRoom(
    //                   i32 mapX,
    //                   i32 mapY,
    //                   RN object,          // RNnul refers to the party
    //                   bool unchangedRoom, // object is in same room as before
    //                   bool place,         // place object rather than remove
    //                   MMRECORD *pmmr)

	// you placed an item at floor.		_2fcf_2444(  3,  5,2801,1,1,0) @ 22
	// you took an item from floor.		_2fcf_2444(  3,  5,2801,1,0,0) @ 22
	//									_2fcf_2444(  3,  5,6009,1,0,0) @ 22

	// you have insert a key to hole	_2fcf_2444(  4,  5,E979,0,1,0) @ 22
	// you have no one at leader's hand	_2fcf_2444(  3,  5,FFFF,1,0,0) @ 22

	// you are moving		_2fcf_2444(  3,  3,FFFF,0,0,0) @ 22
	// you have moved		_2fcf_2444(  3,  2,FFFF,0,1,0) @ 22

	// creature is moving	_2fcf_2444(  8,  2,1055,0,0,0) @ 16
	// creature has moved	_2fcf_2444(  8,  3,1055,0,1,0) @ 16

	// creature has removed	_2fcf_2444(  1,  6,10EB,0,0,0) @ 22
	//						_2fcf_2444(  3,  5,10EB,1,0,0) @ 22

	// creature has dropped	_2fcf_2444(  3,  3,6009,0,1,0) @ 22

	//CSBWin:/PlaceOrRemoveObjectInRoom (TAG013380)

	//^2FCF:2444
	//^2FCF:244A
	ObjectID *bp1c = 0;
	//^2FCF:2454
	Bit16u iObjectDBType; // bp0e
	Bit16u di;
	/*SkD((DLV_TWEET, "Tweet: Invoked _2fcf_2444 (x:%d, y:%d, object:%s) (s:%d, t:%d, y:%d) \n"
		, xpos, ypos, static_cast<LPCSTR>(getRecordNameOf(recordLink)), ss, tt, uu
		));*/
	if (recordLink != OBJECT_NULL) {
		//^2FCF:245A
		iObjectDBType = recordLink.DBType();
		//^2FCF:2466
		di = GET_DISTINCTIVE_ITEMTYPE(recordLink);
		//^2FCF:2471
		ObjectID bp2c = GET_CREATURE_AT(xpos, ypos);
		//^2FCF:2481
		if (bp2c != OBJECT_NULL) {
			//^2FCF:2486
			Creature *bp08 = GET_ADDRESS_OF_RECORD4(bp2c);
			//^2FCF:2495
			AIDefinition *bp0c = QUERY_CREATURE_AI_SPEC_FROM_TYPE(bp08->CreatureType());
			//^2FCF:24A9
			if (uu != 0 && (place == FCT_REMOVE_OFF || (bp0c->IsStaticObject() != 0 && bp0c->w30_11_11() != 0))) {
				//^2FCF:24C7
				bp1c = &bp08->possession;
			}
		}
	}
	else {
		//^2FCF:24D7
		iObjectDBType = 0xFFFF;
		di = 0xffff;
	}
	//^2FCF:24DF
	if (place == FCT_REMOVE_OFF && iObjectDBType != 0xffff) {
		//^2FCF:24EB
		if (bp1c == NULL) {
			//^2FCF:24F3
			CUT_RECORD_FROM(recordLink, NULL, xpos, ypos);
		}
		else {
			//^2FCF:24FF
			CUT_RECORD_FROM(recordLink, bp1c, -1, 0);
		}
	}
	//^2FCF:2514
	Bit16u bp2a = glbCurrentTileMap[xpos][ypos];
	//^2FCF:252E
	Bit16u bp1e; // item direction
	if ((bp2a >> 5) == ttWall) {
		//^2FCF:2535
		bp1e = recordLink.Dir();
	}
	else {
		//^2FCF:2540
		bp1e = 0xffff;
	}
	//^2FCF:2545
	Bit16u bp26 = 0;
	Bit16u bp24 = 0;
	Bit16u bp22 = 0;
	Bit16u bp20 = 0;
	//^2FCF:2553
	ObjectID si = GET_TILE_RECORD_LINK(xpos, ypos);
	//^2FCF:2562
	if (si == OBJECT_NULL) {
		//^2FCF:256B
		for (; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
			//^2FCF:256E
			Bit16u bp12 = si.DBType();
			//^2FCF:2579
			if (bp12 == dbCreature && IS_OBJECT_FLOATING(si) == 0) {
				//^2FCF:2588
				if ((QUERY_CREATURE_AI_SPEC_FLAGS(si) & 0x0001) != 0 && _1c9a_06bd(si, di, 255) != NULL) {
					//^2FCF:25A5
					bp24 = 1;
				}
				else {
					//^2FCF:25AA
					bp22 = 1;
				}
			}
			//^2FCF:25B2
			if (bp12 == dbText && iObjectDBType == 0xffff && place == FCT_PLACE_ON && ss == 0) { // if (bp12 == dbText && bp0e == 0xffff && place != FCT_REMOVE_OFF && ss == 0) {
				//^2FCF:25CA
				Text *bp18 = GET_ADDRESS_OF_RECORD2(si);
				//^2FCF:25D7
				if (bp18->TextMode() != 0 || bp18->TextVisibility() == 0)
					//^2FCF:25F0
					continue;
				//^2FCF:25F2
				Bit8u bp0106[200];
				QUERY_MESSAGE_TEXT(bp0106, si, 1);
				//^2FCF:2603
				DISPLAY_HINT_TEXT(COLOR_WHITE, bp0106);
				//^2FCF:2613
				continue;
			}
			//^2FCF:2615
			if (bp12 > 4 && bp12 < 14) {
				//^2FCF:2621
				bp20 = 1;
				//^2FCF:2626
				bp24 |= (GET_DISTINCTIVE_ITEMTYPE(si) == di) ? 1 : 0;
				//^2FCF:263B
				bp26 |= (GET_DISTINCTIVE_ITEMTYPE(si) != di) ? 1 : 0;
			}
			//^2FCF:2650
		}
	}
	else {
		for (; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
			//^2FCF:2663
			if (si.Dir() == bp1e && si.DBType() > dbCreature) {
				//^2FCF:267A
				bp20 = 1;
				//^2FCF:267F
				bp24 |= (GET_DISTINCTIVE_ITEMTYPE(si) == di) ? 1 : 0;
				//^2FCF:2694
				bp26 |= (GET_DISTINCTIVE_ITEMTYPE(si) != di) ? 1 : 0;
			}
			//^2FCF:26B2
		}
	}
	//^2FCF:26B7
	if (place == FCT_PLACE_ON && iObjectDBType != 0xffff) { // if (place != FCT_REMOVE_OFF && bp0e != 0xffff) {
		//^2FCF:26C3
		if (bp1c == NULL) {
			//^2FCF:26CB
			APPEND_RECORD_TO(recordLink, NULL, xpos, ypos);
		}
		else {
			//^2FCF:26D7
			APPEND_RECORD_TO(recordLink, bp1c, -1, 0);
		}
	}
	//^2FCF:26EC
	for (si = GET_TILE_RECORD_LINK(xpos, ypos); si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
		//^2FCF:26FC
		if (bp1e != 0xffff	// SPX: FIX: changed to "!=" instead of "==" => bp1e = direction on wall (and 0xffff on ground...) so it must not be 0xffff
			&& IS_REBIRTH_ALTAR(si) != 0
			&& QUERY_CLS1_FROM_RECORD(recordLink) == GDAT_CATEGORY_MISCELLANEOUS
			&& QUERY_CLS2_FROM_RECORD(recordLink) == GET_CHAMPION_BONES_ITEM_ID()) {	// Was Misc 0 = someone's bones in DM2 / SPX: I changed to function to get the proper Bones ID depending on DM2 or DM1 mode
			//^2FCF:2730
			i16 bp2e = ADD_ITEM_CHARGE(recordLink, 0);
			//^2FCF:273F
			if (bp2e < 0 || bp2e >= cd.pi.glbChampionsCount)
				//^2FCF:274E
				break;
			//^2FCF:2751
			Timer bp3c;
			bp3c.SetMap(glbCurrentMapIndex);
			bp3c.SetTick(glbGameTick + 1);
			//^2FCF:2774
			bp3c.TimerType(ttyResurrect);
			bp3c.actor = (Bit8u)bp2e;
			bp3c.Xcoord((Bit8u)xpos);
			bp3c.Ycoord((Bit8u)ypos);
			bp3c.Value2(bp1e);
			bp3c.ActionType(2);
			//^2FCF:2794
			QUEUE_TIMER(&bp3c);
			//^2FCF:27A0
			break;
		}
		//^2FCF:27A3
		Bit16u bp12 = si.DBType();
		//^2FCF:27AE
		if (bp12 == dbActuator) {
			//^2FCF:27B6
			Actuator *xActuator = GET_ADDRESS_OF_ACTU(si);	// Actuator *bp04
			U8 iActType = xActuator->ActuatorType();
			//^2FCF:27C3
			if (iActType == 0)
				//^2FCF:27D1
				continue;
			//^2FCF:27D4
			Bit16u bp14 = xActuator->ActuatorData();
			Bit16u bp10 = place;
			//^2FCF:27EA
			//Bit16u bp3e;
			if (bp1e == 0xffff) {
				//^2FCF:27F3
				switch (iActType) {
					default:
						//^2FCF:2801
						continue;
					case ACTUATOR_FLOOR_TYPE__EVERYTHING: // 0x01: Activator, trap floor
						if (iObjectDBType >= DB_CATEGORY_MISSILE && iObjectDBType != 0xFFFF) // SPX: added this to prevent DM1 floor pad to trap missiles (0xFFFF is for player)
							continue;
						//^2FCF:280B
						if (ss != 0)
							//^2FCF:2811
							continue;
						//^2FCF:2814
						if (bp20 != 0)
							//^2FCF:281A
							continue;
						//^2FCF:281D
						if (bp22 == 0)
							//^2FCF:2823
							goto _29a8;
						//^2FCF:2826
						continue;
					case 2: // 0x02: -
						//^2FCF:2829
						if (iObjectDBType > DB_CATEGORY_CREATURE)	// (iObjectDBType > 4) weapons to cloud
							//^2FCF:282F
							continue;
						//^2FCF:2832
						if (ss != 0)
							//^2FCF:2838
							continue;
						//^2FCF:283B
						if (bp22 == 0)
							//^2FCF:2841
							goto _29a8;
						//^2FCF:2844
						continue;
					case ACTUATOR_FLOOR_TYPE__PARTY: // 0x03: Activator, trap floor
						//^2FCF:2847
						if (iObjectDBType != 0xffff)
							//^2FCF:284D
							continue;
						//^2FCF:2850
						if (cd.pi.glbChampionsCount == 0)
							//^2FCF:2857
							continue;
						//^2FCF:285A
						if (bp14 == 0) {
							//^2FCF:2860
							if (ss == 0)
								//^2FCF:2866
								goto _29a8;
							//^2FCF:2869
							continue;
						}
						//^2FCF:286C
						if (place == FCT_REMOVE_OFF) {
							//^2FCF:2872
							bp10 = 0;
							//^2FCF:2877
							goto _29a8;
						}
						//^2FCF:287A
						bp10 = (cd.pi.glbPlayerDir +1 == bp14) ? 1 : 0;
						//^2FCF:288D
						goto _29a8;
					case ACTUATOR_FLOOR_TYPE__ITEM: // 0x04: Activator, trap floor
						//^2FCF:2890
						if (bp14 != di)
							//^2FCF:2895
							continue;
						//^2FCF:2898
						if (bp24 == 0)
							//^2FCF:289E
							goto _29a8;
						//^2FCF:28A1
						continue;
					case ACTUATOR_FLOOR_TYPE__X5: // 0x05: -
					case ACTUATOR_FLOOR_TYPE__DM1_CREATURE_GENERATOR: // 0x06: -
						//^2FCF:2D72
						continue;
					case ACTUATOR_FLOOR_TYPE__CREATURE: // 0x07: Activator, trap floor
						if (!SkCodeParam::bUseFixedMode)	// SPX: the original converted code below is strange, as it never triggers something.
						{
							//^2FCF:28A4
							if (iObjectDBType > 4) // weapon to cloud
								//^2FCF:28AA
								continue;
							//^2FCF:28AD
							if (iObjectDBType == 0xffff)
								//^2FCF:28B3
								continue;
							//^2FCF:28B6
							if (bp22 == 0)
								//^2FCF:28BC
								continue;
						}
						else // I reimplement from my understanding and specially for DM1 compatibility
						{
							if (iObjectDBType == DB_CATEGORY_CREATURE)
							{
								Bit8u iActuatorTriggers = 0;
								Bit16u iActuatorEffectType = xActuator->ActionType();

								if (place == FCT_PLACE_ON &&
									(iActuatorEffectType == ACTEFFECT_STEP_ON__OPEN_SET ||
									iActuatorEffectType == ACTEFFECT_STEP_ON__CLOSE_CLEAR ||
									iActuatorEffectType == ACTEFFECT_STEP_ON__TOGGLE ||
									iActuatorEffectType == ACTEFFECT_STEP_CONSTANT__CLOSE))
									iActuatorTriggers = 1;
								else if (place == FCT_REMOVE_OFF &&
									(iActuatorEffectType == ACTEFFECT_STEP_CLOSE__OPEN_SET ||
									iActuatorEffectType == ACTEFFECT_STEP_CLOSE__CLOSE_CLEAR ||
									iActuatorEffectType == ACTEFFECT_STEP_CLOSE__TOGGLE ||
									iActuatorEffectType == ACTEFFECT_STEP_CONSTANT__OPEN))
									iActuatorTriggers = 1;

								if (iActuatorTriggers == 1)
								{
									if (xActuator->SoundEffect() != 0) {
										QUEUE_NOISE_GEN2(
											(bp1e == 0xffff) ? GDAT_CATEGORY_FLOOR_GFX : GDAT_CATEGORY_WALL_GFX,
											(bp1e == 0xffff) ? GET_FLOOR_DECORATION_OF_ACTUATOR(xActuator) : GET_WALL_DECORATION_OF_ACTUATOR(xActuator),
											SOUND_STD_ACTIVATION, 0xfe, xpos, ypos, 0x01, 0x8c, 0x80);
									}
									INVOKE_ACTUATOR(xActuator, iActuatorEffectType, 0);
								}
							}
						}
						//^2FCF:28BF
						continue;
					case ACTUATOR_FLOOR_TYPE__ITEM_POSSESSION: // 0x08: Activator, trap floor
						//^2FCF:28C2
						if (iObjectDBType != 0xffff)
							//^2FCF:28C8
							continue;
						//^2FCF:28CB
						bp10 = _2fcf_16ff(bp14);
						//^2FCF:28D6
						goto _29a8;
				}
			}
			//^2FCF:28D9
			if (si.Dir() != bp1e)
				//^2FCF:28E3
				continue;
			//^2FCF:28E6
			//bp3e = bp04->ActuatorType();
			//^2FCF:28F3
			switch (iActType) {
				case ACTUATOR_TYPE_KEY_HOLE: // &H1A -> Activator, key hole
					{
						//^2FCF:2952
						if (bp24 != 0)
							//^2FCF:2958
							continue;
						//^2FCF:295B
						if (xActuator->OnceOnlyActuator() == place)
							//^2FCF:296D
							continue;
						//^2FCF:2970
						if (QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, GET_WALL_DECORATION_OF_ACTUATOR(xActuator), dtWordValue, 0x0e) != di)
							//^2FCF:2990
							continue;
						//^2FCF:2993
						xActuator->OnceOnlyActuator(place);
						//^2FCF:29A8
_29a8:
						bp10 = bp10 ^ xActuator->RevertEffect();
						//^2FCF:29BD
						Bit16u bp28 = xActuator->ActionType();
						//^2FCF:29CA
						if (bp28 == 3) {
							//^2FCF:29CF
							bp28 = (bp10 != 0) ? 0 : 1;
						}
						else {
							//^2FCF:29E1
							if (bp10 == 0)
								//^2FCF:29E7
								continue;
						}
						//^2FCF:29EA
						if (xActuator->SoundEffect() != 0) {
							//^2FCF:29FB
							QUEUE_NOISE_GEN2(
								(bp1e == 0xffff) ? GDAT_CATEGORY_FLOOR_GFX : GDAT_CATEGORY_WALL_GFX,
//									(bp1e == 0xffff) ? 0x0a : 0x09,
								(bp1e == 0xffff)
									? GET_FLOOR_DECORATION_OF_ACTUATOR(xActuator)
									: GET_WALL_DECORATION_OF_ACTUATOR(xActuator),
								SOUND_STD_ACTIVATION,
								0xfe,
								xpos,
								ypos,
								0x01,
								0x8c,
								0x80
								);
						}
						//^2FCF:2A47
						INVOKE_ACTUATOR(xActuator, bp28, 0);
						//^2FCF:2A59
						continue;
					}
				case ACTUATOR_TYPE_X29: // &H29 -> -
					{
						//^2FCF:290C
						if (bp20 == 0)
							//^2FCF:2912
							goto _29a8;
						//^2FCF:2915
						continue;
					}
				case ACTUATOR_TYPE_ALCOVE_ITEM_TRAP: // &H2A -> Activator, alcove item trap
					{
						//^2FCF:2918
						if (bp24 != 0)
							//^2FCF:291E
							continue;
						//^2FCF:2921
						if (xActuator->ActuatorData() == di)
							//^2FCF:2930
							goto _29a8;
						//^2FCF:2932
						continue;
					}
				case ACTUATOR_TYPE_X2B: // &H2B -> -
					{
						//^2FCF:2935
						if (bp26 != 0)
							//^2FCF:293B
							continue;
						//^2FCF:293E
						if (xActuator->ActuatorData() != di)
							//^2FCF:294D
							goto _29a8;
						//^2FCF:294F
						continue;
					}
				case ACTUATOR_TYPE_DM1_WALL_SWITCH: // SPX DM1 retrocompatibility, triggered when an item comes into an alcove or is removed from alcove
					{
						if (bp24 != 0)
							continue;
						// The actuator should be an alcove -- this may need to be checked
						Bit16u iRevertEffect = bp10 ^ xActuator->RevertEffect();
						Bit16u iActionType = xActuator->ActionType();
						if ((iActionType & 3) == ACTEFFECT_STEP_CONSTANT__OPEN) {
							iActionType = (iRevertEffect != 0) ? ACTMSG_OPEN_SET : ACTMSG_CLOSE_CLEAR;
						}
						else {
							if (iRevertEffect == 0)
								continue;
						}
						/// The alcove actuator will trigger either if first item is put into, or last item is removed from.
						U16 iNbItems = 0;
						// the item being put into or removed from alcove is already counted from the tile itself
						iNbItems = GET_TILE_COUNT_TAKEABLE_ITEMS(xpos, ypos, bp1e);
						if ( (place == FCT_REMOVE_OFF && iNbItems == 0) 
							|| (place == FCT_PLACE_ON && iNbItems == 1) ) // then triggers
							;
						else
							continue;

						if (xActuator->SoundEffect() != 0) {
							QUEUE_NOISE_GEN2(
								(bp1e == 0xffff) ? GDAT_CATEGORY_FLOOR_GFX : GDAT_CATEGORY_WALL_GFX,
								(bp1e == 0xffff)
									? GET_FLOOR_DECORATION_OF_ACTUATOR(xActuator)
									: GET_WALL_DECORATION_OF_ACTUATOR(xActuator),
								SOUND_STD_ACTIVATION,
								0xFE,
								xpos,
								ypos,
								0x01,
								0x8C,
								0x80
								);
						}
						INVOKE_ACTUATOR(xActuator, iActionType, 0);
						continue;
					}
				case ACTUATOR_TYPE_DM1_ALCOVE_ITEM:	// SPX DM1 retrocompatibility, triggered when expected item is moved into or from alcove
					{
						if (bp24 != 0)
							continue;
						// The actuator should be an alcove -- this may need to be checked
						Bit16u iRevertEffect = bp10 ^ xActuator->RevertEffect();
						Bit16u iActionType = xActuator->ActionType();
						if ((iActionType & 3) == ACTEFFECT_STEP_CONSTANT__OPEN) {
							iActionType = (iRevertEffect != 0) ? ACTMSG_OPEN_SET : ACTMSG_CLOSE_CLEAR;
						}
						else {
							if (iRevertEffect == 0)
								continue;
						}
						/// Check that item is the expected one
						U16 iItemMatches = 0;
						U16 iNbItems = 0;
						U16 iExpectedItemID = xActuator->ActuatorData();
						// the item being put into or removed from alcove is already counted from the tile itself
						//iNbItems = GET_TILE_COUNT_TAKEABLE_ITEMS(xpos, ypos, bp1e);
						iItemMatches = (GET_DISTINCTIVE_ITEMTYPE(recordLink) == iExpectedItemID) ? 1 : 0;
						
						if (iItemMatches == 0)
							continue;

						// TODO : would check for other items of the same ID !!!
						iNbItems = GET_TILE_COUNT_TAKEABLE_ITEMS(xpos, ypos, bp1e);

						if ( (place == FCT_REMOVE_OFF && iNbItems == 0) 
							|| (place == FCT_PLACE_ON && iNbItems == 1) ) // then triggers
							;
						else
							continue;

						if (xActuator->SoundEffect() != 0) {
							QUEUE_NOISE_GEN2(
								(bp1e == 0xffff) ? GDAT_CATEGORY_FLOOR_GFX : GDAT_CATEGORY_WALL_GFX,
								(bp1e == 0xffff)
									? GET_FLOOR_DECORATION_OF_ACTUATOR(xActuator)
									: GET_WALL_DECORATION_OF_ACTUATOR(xActuator),
								SOUND_STD_ACTIVATION,
								0xFE,
								xpos,
								ypos,
								0x01,
								0x8C,
								0x80
								);
						}
						INVOKE_ACTUATOR(xActuator, iActionType, 0);
						continue;
					}
			}
			//^2FCF:2905
			continue;
		}
		//^2FCF:2A5C
		if (bp12 == dbText) {
			//^2FCF:2A65
			Text *bp18 = GET_ADDRESS_OF_RECORD2(si);
			//^2FCF:2A72
			if (bp18->TextMode() == 1) { }
			//SPX: test DM1
			if (SkCodeParam::bDM1Mode)
			{
				Bit8u bp0106[200];
				QUERY_MESSAGE_TEXT(bp0106, si, 1);
				DISPLAY_HINT_TEXT(COLOR_YELLOW, bp0106);
			}
			//^2FCF:2A81
			if (bp1e == 0xffff) {
				//^2FCF:2A8A
				// SPX: That part is called when an animated wall (even simple actuator) loops animation
				if (bp18->SimpleTextExtUsage() != 9) { // 0x09 -> ?
					//^2FCF:2AA2
					if (bp18->SimpleTextExtUsage() == 10) { // 0x0a -> Marsh
						//^2FCF:2AA7
						//^2FCF:2B5B
						if (iObjectDBType != 0xffff)
							//^2FCF:2B61
							continue;
						//^2FCF:2B64
						if (place == FCT_PLACE_ON) // if (place != FCT_REMOVE_OFF)
							//^2FCF:2B6A
							continue;
						//^2FCF:2B6D
						if (cd.pi.glbChampionsCount == 0)
							//^2FCF:2B74
							continue;
						//^2FCF:2B77
						Bit16u bp32 = 0;
						Bit16u bp30 = 0;
						//^2FCF:2B81
						for (; bp30 < cd.pi.glbChampionsCount; bp30++) {
							//^2FCF:2B83
							if (glbChampionSquad[bp30].curHP() != 0) {
								//^2FCF:2B94
								bp32 = bp32 + GET_PLAYER_WEIGHT(bp30) / MAX_LOAD(&glbChampionSquad[bp30]);
							}
							//^2FCF:2BC3
						}
						//^2FCF:2BCF
						bp32 = min_value(90, (bp32 * 10) + ((bp18->TextVisibility() != 0) ? 50 : 25));
						//^2FCF:2BFE
						// SPX: When the party is bogged down
						if (ss == 0 && RAND16(100) < bp32) {
							//^2FCF:2C17
							Timer bp3c;
							bp3c.SetTick(glbGameTick);
							bp3c.SetMap(glbCurrentMapIndex);
							//^2FCF:2C34
							bp3c.TimerType(tty5D);
							bp3c.actor = TIMER_ACTOR__00;
							bp3c.b6_0_4((Bit8u)xpos);
							bp3c.w6_5_9(ypos);
							bp3c.Value2((Bit8u)cd.pi.glbPlayerMap);
							bp3c.w6_a_b(cd.pi.glbPlayerDir);
							//^2FCF:2C70
							QUEUE_TIMER(&bp3c);
							//^2FCF:2C7C
							bp30 = RAND02();
							//^2FCF:2C84
							if (glbChampionSquad[bp30].curHP() == 0) {
								//^2FCF:2C92
								bp30 = glbChampionLeader;
							}
							//^2FCF:2C98
							QUEUE_NOISE_GEN2(
								GDAT_CATEGORY_CHAMPIONS,
								glbChampionSquad[bp30].HeroType(),
								SOUND_CHAMPION_GETHIT,
								0xfe,
								cd.pi.glbPlayerPosX,
								cd.pi.glbPlayerPosY,
								0x01,
								0x69,
								0xc8
								);
							//^2FCF:2CC6
							continue;
						}
						//^2FCF:2CC9
						QUEUE_NOISE_GEN1(
							GDAT_CATEGORY_FLOOR_GFX,
							((Bit8u)bp18->TextIndex()) & 0xff,
							SOUND_STD_ACTIVATION,
							0x8c,
							0x80,
							cd.pi.glbPlayerPosX,
							cd.pi.glbPlayerPosY,
							0x01
							);
						//^2FCF:2CF6
						continue;
					}
					//^2FCF:2AAA
					continue;
				}
				//^2FCF:2AAD
				if (iObjectDBType != 0xffff) {
					//^2FCF:2AB3
					continue;
				}
				//^2FCF:2AB6
				if (ss != 0) {
					//^2FCF:2ABC
					continue;
				}
				//^2FCF:2ABF
				if (cd.pi.glbChampionsCount == 0) {
					//^2FCF:2AC6
					continue;
				}
				//^2FCF:2AC9
				if (RAND16(100) >= (bp18->TextIndex() & 0x00ff)) {
					//^2FCF:2AE7
					continue;
				}
				//^2FCF:2AEA
				Bit16u bp10 = (bp18->TextVisibility() != place) ? 1 : 0;
				//^2FCF:2B00
				INVOKE_MESSAGE(
					xpos,
					ypos,
					0,
					(bp10 != 0) ? 0 : 1,
					glbGameTick +1
					);
				//^2FCF:2B2C
				INVOKE_MESSAGE(
					xpos,
					ypos,
					0,
					(bp10 != 0) ? 1 : 0,
					glbGameTick +5
					);
				//^2FCF:2B58
				continue;
			}
			//^2FCF:2CF8
			if (si.Dir() != bp1e)
				//^2FCF:2D00
				continue;
			//^2FCF:2D02
			if (bp18->SimpleTextExtUsage() != 4 && bp18->SimpleTextExtUsage() != 8)
				//^2FCF:2D1F
				continue;
			//^2FCF:2D21
			if (bp24 != 0)
				//^2FCF:2D25
				continue;
			//^2FCF:2D27
			if (bp18->TextVisibility() == place)
				//^2FCF:2D34
				continue;
			//^2FCF:2D36
			//if (QUERY_GDAT_ENTRY_DATA_INDEX(0x09, ((Bit8u)bp18->TextIndex()) & 0xff, dtWordValue, 0x0e) != di)
			if (QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, ((Bit8u)bp18->TextIndex()) & 0xff, dtWordValue, GDAT_WALL_ORNATE__IS_ITEM_TRIGGERED) != di)
				//^2FCF:2D56
				continue;
			//^2FCF:2D58
			bp18->TextVisibility(place);
			//^2FCF:2D6A
			continue;
		}
		//^2FCF:2D6C
		if (bp12 >= dbCreature)
			//^2FCF:2D70
			break;
		//^2FCF:2D72
	}

	//^2FCF:2D83
	return;
}

//^1C9A:156A
void SkWinCore::DELETE_CREATURE_RECORD(i16 xpos, i16 ypos, Bit16u dropMode, Bit16u tt)	// ss = dropMode
{
	SkD((DLV_DBM, "DBM: DELETE_CREATURE_RECORD(%2d,%2d,%04X,%04X)\n"
		, (Bitu)xpos, (Bitu)ypos, (Bitu)dropMode, (Bitu)tt));

	//^1C9A:156A
	//^1C9A:1570
	ObjectID xCreatureReference = GET_CREATURE_AT(xpos, ypos);	// ObjectID si
	//^1C9A:157E
	if (xCreatureReference == OBJECT_NULL)
		//^1C9A:1583
		return;
	//^1C9A:1586
	Creature *xCreature = GET_ADDRESS_OF_RECORD4(xCreatureReference);	// Creature *bp04
	//^1C9A:1593
	AIDefinition *xAIStats = QUERY_CREATURE_AI_SPEC_FROM_TYPE(xCreature->CreatureType());	// AIDefinition *bp08
	//^1C9A:15A7
	if (xAIStats->IsStaticObject() == 0) {
		//^1C9A:15B4
		Bit16u iAIStatIndex = QUERY_GDAT_CREATURE_WORD_VALUE(xCreature->CreatureType(), CREATURE_STAT_AI_SEQ_01);	// bp10	// 0x01
		//^1C9A:15C8
		Bit16u di = tblAIStats01[iAIStatIndex];
		//^1C9A:15D0
		if ((di & 0x0004) == 0) {
			//^1C9A:15D6
			Creature *xCreature2 = xCreature;	// Creature *bp0c
			//^1C9A:15E5
			Bit16u iCurrentMapIndex = glbCurrentMapIndex;	// Bit16u bp0e
			if (!SkCodeParam::bDM1Mode)
			{
				//^1C9A:15EB
				CHANGE_CURRENT_MAP_TO(xCreature2->TriggerMap());
				//^1C9A:15FF
				INVOKE_MESSAGE(xCreature2->TriggerX(), xCreature2->TriggerY(), 0, 0, glbGameTick +1);
			}
			//^1C9A:162E
			CHANGE_CURRENT_MAP_TO(iCurrentMapIndex);
		}
	}
	//^1C9A:1637
	MOVE_RECORD_TO(xCreatureReference, xpos, ypos, -4, 0);
	//^1C9A:164A
	DROP_CREATURE_POSSESSION(xCreatureReference, xpos, ypos, dropMode, tt);
	//^1C9A:165E
	_1c9a_0247_FREE_CACHE_FROM_CREATURE(xCreatureReference);
	//^1C9A:1664
	DEALLOC_RECORD(xCreatureReference);
	//^1C9A:166B
	return;
}


//^2FCF:0DD5
Bit16u SkWinCore::MOVE_RECORD_TO(ObjectID rlWhatYouMove, i16 xposFrom, i16 yposFrom, i16 xposTo, i16 yposTo) //#DS=4976?
{
	SkD((DLV_DBM, "DBM: MOVE_RECORD_TO(%04X,%2d,%2d,%2d,%2d)\n"
		, (Bitu)rlWhatYouMove.w, (Bitu)xposFrom, (Bitu)yposFrom, (Bitu)xposTo, (Bitu)yposTo));

	// CSBwinSimilarity: TAG011f52,MoveObject

	// in CSBwin, similar to TAG011f52,MoveObject
	// place battle prize?

	// place an object to tile. or take an object from tile. the target can be floor/wall/alcove/creature/missile.
	// you can move creatures/missiles too.
	//
	// rlWhatYouMove: an object what you wanna move. this also includes new tile position.
	//
	// a) player [takes] an object at [creature]
	//    xposFrom=xx, yposFrom=yy, xposTo=[-1], yposTo=[-1]
	// b) player [takes] an object at [alcove/floor]
	//    xposFrom=xx, yposFrom=yy, xposTo=[-1], yposTo=[ 0]
	//
	// c) player [places] an object to [creature]
	//    xposFrom=[-1], yposFrom=[-1], xposTo=xx, yposTo=yy
	// d) player [places] an object to [alcove/floor]
    //    xposFrom=[-1], yposFrom=[ 0], xposTo=xx, yposTo=yy
	//
	// e) if player surely moves (not called if run to wall)
	//    xposFrom=[old xpos], yposFrom=[old ypos], xposTo=[new xpos], yposTo=[new ypos], rlWhatYouMove=0xFFFF
	//
	// f) if an item in missile drops
	//    xposFrom=[-2], yposFrom=[ 0], xposTo=[xpos], yposTo=[ypos], rlWhatYouMove=[an flying item such as gold coin]
	// g) if an item injects to creature
	//    xposFrom=[xpos], yposFrom=[ypos], xposTo=[-2], yposTo=[ 0], rlWhatYouMove=[an flying item such as gold coin]
	//
	// h) to generate creature,
	//    xposFrom=[-4], yposFrom=[ 0], xposTo=[xpos], yposTo=[ypos], rlWhatYouMove=[creature]
	// i) to kill creature,
	//    xposFrom=[xpos], yposFrom=[ypos], xposTo=[-4], yposTo=[ 0], rlWhatYouMove=[creature]

	// return 0 if move success
	// return non 0 if move failed

	if (SkCodeParam::bAllowFreeObjectMove == false)
	{
		if (SkCodeParam::bUsePowerDebug == true) {
			if (rlWhatYouMove.DBType() == dbDoor) return 1;
			if (rlWhatYouMove.DBType() == dbTeleporter) return 1;
			if (rlWhatYouMove.DBType() == dbText) return 1;
			if (rlWhatYouMove.DBType() == dbActuator) return 1;

		}
		ATLASSERT(rlWhatYouMove.DBType() != dbDoor); // you would not move this kind of object
		ATLASSERT(rlWhatYouMove.DBType() != dbTeleporter); // you would not move this kind of object
		ATLASSERT(rlWhatYouMove.DBType() != dbText); // you would not move this kind of object
		ATLASSERT(rlWhatYouMove.DBType() != dbActuator); // you would not move this kind of object
	}

	//^2FCF:0DD5
	//^2FCF:0DDB
	ObjectID si = rlWhatYouMove;
	_4976_5822 = 0;
	Bit16u bp10 = 0;
	Bit16u bp12 = 0;
	Bit16u bp08 = glbCurrentMapIndex; // current mapno
	Bit16u bp0c = cd.pi.glbPlayerDir; // current player dir
	Bit16u bp1a = 0;
	//^2FCF:0DFA
	Bit16u bp0a;
	Bit16u bp0e;
	GenericRecord *bp04 = NULL;
	ObjectID bp1c;
	if (si == OBJECT_NULL) {
		//^2FCF:0DFF
		glbDoLightCheck = 1;
		bp0a = 0xffff;
		//^2FCF:0E0A
		if (xposFrom >= 0) {
			//^2FCF:0E10
			_2fcf_0234(xposFrom, yposFrom, xposTo, yposTo);
		}
	}
	else {
		//^2FCF:0E25
		bp0a = si.DBType();
		//^2FCF:0E30
		if (bp0a == dbCreature) {
			//^2FCF:0E35
			bp04 = GET_ADDRESS_OF_RECORD(si);
		}
		//^2FCF:0E42
		bp0e = si.Dir();
	}
	//^2FCF:0E4A
	Bit16u bp1e = 0; // defaulting to 0
	Bit16u di = 0; // defaulting to 0
	if (xposTo >= 0) {
		//^2FCF:0E53
		bp10 = _2fcf_0434(si, xposFrom, yposFrom, xposTo, yposTo, 1); // SPX: check if going into pit or teleporter ?
		//^2FCF:0E6C
		if (si != OBJECT_NULL) {
			//^2FCF:0E71
			if (QUERY_GDAT_DBSPEC_WORD_VALUE(si, 0xf8) != 0) {
				//^2FCF:0E80
				if (bp08 == cd.pi.glbPlayerMap || _4976_581c == cd.pi.glbPlayerMap) {
					//^2FCF:0E92
					glbDoLightCheck = 1;
				}
			}
			//^2FCF:0E98
			si.Dir(_4976_5820);
		}
		//^2FCF:0EA8
		xposTo = _4976_5826;
		yposTo = _4976_5828;
		bp1e = _4976_581e;
		di = _4976_581c;
		//^2FCF:0EBF
		CHANGE_CURRENT_MAP_TO(di);
		//^2FCF:0EC6
		// SPX: Moved the 2nd condition checking if creature is allowed on target level into a variable.
		// This allows to break the limitation of "creatures list" for any map and also allows any creature to be summoned from custom spells
		// The original logic was " if (bp0a == dbCreature && IS_CREATURE_ALLOWED_ON_LEVEL(si, di) == 0) "
		U8 bCreatureAllowedOnLevel = 1;
		if (bp0a == dbCreature)
		{
			if (!SkCodeParam::bUseDM2ExtendedMode)
				bCreatureAllowedOnLevel = IS_CREATURE_ALLOWED_ON_LEVEL(si, di);

			if (bCreatureAllowedOnLevel == 0) {
				//^2FCF:0ED9
				DROP_CREATURE_POSSESSION(si, xposTo, yposTo, CREATURE_GENERATED_DROPS, 1);	// (si, xposTo, yposTo, 0, 1)
				//^2FCF:0EEC
				CHANGE_CURRENT_MAP_TO(bp08);
				//^2FCF:0EF5
				if (xposFrom >= 0) {
					//^2FCF:0EFB
					DELETE_CREATURE_RECORD(xposFrom, yposFrom, CREATURE_NO_DROP, 0xffff);	// (xposFrom, yposFrom, 2, 0xffff)
				}
				//^2FCF:0F0D
				return 1;
			}
		}
		//^2FCF:0F13
		bp12 = (di == bp08 && xposTo == xposFrom && yposTo == yposFrom) ? 1 : 0;
		//^2FCF:0F32
		if (bp12 != 0) {
			//^2FCF:0F38
			if (si == 0xffff) {
				//^2FCF:0F3D
				if (cd.pi.glbPlayerDir == bp0c) {
					//^2FCF:0F45
					goto _1562;
				}
			}
			else {
				//^2FCF:0F48
				if (_4976_5820 == bp0e && bp0a != dbMissile) {
					//^2FCF:0F56
					goto _1562;
				}
			}
		}
		else {
			//^2FCF:0F59
			if (di != bp08) {
				//^2FCF:0F5E
				if (si == 0xffff && cd.pi.glbPlayerMap != bp08) {
					//^2FCF:0F6B
					LOAD_LOCALLEVEL_GRAPHICS_TABLE(bp08);
				}
				else {
					//^2FCF:0F75
					CHANGE_CURRENT_MAP_TO(bp08);
				}
			}
		}
	}
	//^2FCF:0F7E
	if (xposFrom >= 0) {
		//^2FCF:0F87
		if (si == OBJECT_NULL) {
			//^2FCF:0F8C
			PLACE_OR_REMOVE_OBJECT_IN_ROOM(xposFrom, yposFrom, -1, bp12, FCT_REMOVE_OFF, 0); // SPX: remove player from source position
		}
		else {
			//^2FCF:0FA6
			if (bp10 != 0) {
				//^2FCF:0FAC
				CUT_RECORD_FROM(si, NULL, xposFrom, yposFrom);
			}
			else {
				//^2FCF:0FC1
				PLACE_OR_REMOVE_OBJECT_IN_ROOM(
					xposFrom, 
					yposFrom, 
					si, 
					(glbCurrentMapIndex == glbMap_4c28 && glbCurrentMapIndex == glbSomePosX_4c2e && glbCurrentMapIndex == glbSomePosY_4c30) ? 1 : 0,
					FCT_REMOVE_OFF,
					(xposTo < 0 && yposTo < 0) ? 1 : 0 
					);
			}
			//^2FCF:1009
			if (bp0a == dbCreature && xposTo < -1) {
				//^2FCF:1015
				Bit16u bp06 = bp04->castToCreature()->InternalID();
				//^2FCF:1021
				if (bp06 != 255) {
					//^2FCF:1026
					_1c9a_0fcb(bp06);
				}
			}
		}
	}
	//^2FCF:102F
	i16 bp16, bp18;
	if (xposTo < 0) {
		//^2FCF:1035
		goto _1562;
	}
	else {
		//^2FCF:1038
		if (si == OBJECT_NULL) {
			//^2FCF:1040
			CHANGE_CURRENT_MAP_TO(di);
			//^2FCF:1047
			_2fcf_0b8b(xposTo, yposTo, di); // SPX: check teleporter information on tile
			//^2FCF:1055
			si = GET_CREATURE_AT(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY);
			//^2FCF:1066
			if (si != OBJECT_NULL) {
				//^2FCF:106E
				Bit16u bp20 = QUERY_CREATURE_AI_SPEC_FLAGS(si);
				//^2FCF:1078
				if ((bp20 & CREATURE_AI_FLAG_8000) == 0) {
					//^2FCF:1080
					if ((bp20 & CREATURE_AI_FLAG_0001) != 0 && GET_CREATURE_WEIGHT(si) == 255) {
						//^2FCF:1093
						if (TRY_PUSH_OBJECT_TO(0xffff, xposTo, yposTo, &bp16, &bp18) != 0) {
							//^2FCF:10B3
							LOAD_LOCALLEVEL_GRAPHICS_TABLE(di);
							//^2FCF:10BA
							glbMapToLoad = di;
							//^2FCF:10BE
							MOVE_RECORD_TO(0xffff, -1, 0, bp16, bp18);
							//^2FCF:10D1
							LOAD_LOCALLEVEL_GRAPHICS_TABLE(bp08);
							//^2FCF:10DA
							if (glbMapToLoad == bp08) {
								//^2FCF:10E5
								glbMapToLoad = 0xffff;
								//^2FCF:10EB
								goto _1183;
							}
							else {
								//^2FCF:10E2
								goto _1562;
							}
						}
						else {
							//^2FCF:10B0
							goto _1151;
						}
					}
					//^2FCF:10EE
					if (TRY_PUSH_OBJECT_TO(si, xposTo, yposTo, &bp16, &bp18) != 0) {
						//^2FCF:110A
						bp1a = GET_PARTY_SPECIAL_FORCE() >> 2;
						//^2FCF:1115
						bp1a += RAND16((bp1a >> 1) +1);
						//^2FCF:1127
						ATTACK_CREATURE(si, xposTo, yposTo, 0x2000, 0x0000, bp1a);
						//^2FCF:113E
						MOVE_RECORD_TO(si, xposTo, yposTo, bp16, bp18);
						//^2FCF:114F
						goto _1183;
					}
					//^2FCF:1108
					goto _1151;
				}
			}
			//^2FCF:106B
			goto _1183;
		}
		//^2FCF:103D
		goto _11a3;
	}

	//^2FCF:1151
_1151:
	RELEASE_CREATURE_TIMER(si);
	//^2FCF:1158
	if (MOVE_RECORD_TO(si, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, -3, 0) == 0) {
		//^2FCF:1170
		SET_TIMER_3C_OR_3D(si, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, di, 0);
	}

	//^2FCF:1183
_1183:
	if (di == bp08) {
		//^2FCF:1188
		//^2FCF:1199
		//^2FCF:155A
		PLACE_OR_REMOVE_OBJECT_IN_ROOM(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 0xffff, bp12, FCT_PLACE_ON, 0);
	}
	else {
		//^2FCF:119C
		glbMapToLoad = di;
		//^2FCF:11A0
	}
	goto _1562;

	//^2FCF:11A3
_11a3:
	if (bp0a == dbCreature) {
		//^2FCF:11AC
		CHANGE_CURRENT_MAP_TO(di);
		//^2FCF:11B3
		Bit16u bp06 = bp04->castToCreature()->InternalID();
		//^2FCF:11BF
		if (di == glbMap_4c28 && xposTo == glbSomePosX_4c2e && yposTo == glbSomePosY_4c30) {
			//^2FCF:11E0
			if ((QUERY_CREATURE_AI_SPEC_FLAGS(si) & 0x8000) == 0) {
				//^2FCF:11EC
				if (TRY_PUSH_OBJECT_TO(0xffff, xposTo, yposTo, &bp16, &bp18) != 0) {
					//^2FCF:120C
					bp1a = (GET_CREATURE_WEIGHT(si) >> 3) + RAND16(10);
					//^2FCF:1225
					ATTACK_PARTY(bp1a, 4, 2);
					//^2FCF:1234
					// SPX: Bumping sound
					QUEUE_NOISE_GEN2(GDAT_CATEGORY_CHAMPIONS, 0xfe, SOUND_CHAMPION_BUMP, 0xfe, xposTo, yposTo, 1, 100, 200);
					//^2FCF:1254
					MOVE_RECORD_TO(0xffff, xposTo, yposTo, bp16, bp18);
				}
				else {
					//^2FCF:1209
					goto _13a5;
				}
			}
		}
		//^2FCF:1269
		bp1c = GET_CREATURE_AT(xposTo, yposTo);
		//^2FCF:1279
		Bit16u bp22;
		if (bp1c != OBJECT_NULL) {
			//^2FCF:1281
			bp22 = GET_CREATURE_WEIGHT(si);
			//^2FCF:128B
			Bit16u bp20 = QUERY_CREATURE_AI_SPEC_FLAGS(bp1c);
			//^2FCF:1297
			if (QUERY_CREATURE_AI_SPEC_FLAGS(si) != 0 && bp22 == 255) {
				//^2FCF:12AA
				if ((bp20 & 0x0001) != 0) {
					//^2FCF:12B1
					if (GET_CREATURE_WEIGHT(bp1c) == 255) {
						//^2FCF:12BF
						goto _13a5;
					}
				}
				//^2FCF:12C2
				if (TRY_PUSH_OBJECT_TO(bp1c, xposTo, yposTo, &bp16, &bp18) == 0) {
					//^2FCF:12E0
					RELEASE_CREATURE_TIMER(si);
					//^2FCF:12E7
					SET_TIMER_3C_OR_3D(si, xposTo, yposTo, di, 0);
					//^2FCF:12F5
					goto _1562;
				}
				else {
					//^2FCF:12DE
					goto _1359;
				}
			}
			//^2FCF:12F8
			if ((bp20 & 1) != 0 && GET_CREATURE_WEIGHT(bp1c) == 255) {
				//^2FCF:130D
				if (TRY_PUSH_OBJECT_TO(si, xposTo, yposTo, &bp16, &bp18) == 0)
					//^2FCF:1327
					goto _13a5;
				//^2FCF:1329
				MOVE_RECORD_TO(si, -1, 0, bp16, bp18);
				//^2FCF:1338
				goto _1562;
			}
			else {
				//^2FCF:133B
				if (TRY_PUSH_OBJECT_TO(bp1c, xposTo, yposTo, &bp16, &bp18) == 0)
					//^2FCF:1357
					goto _13a5;

				goto _1359;
			}
		}
		else {
			//^2FCF:127E
			goto _13ce;
		}

		//^2FCF:1359
_1359:
		{
			bp1a = bp22 >> 2;
			//^2FCF:1362
			bp1a += RAND16((bp1a >> 1) +1);
			//^2FCF:1374
			ATTACK_CREATURE(bp1c, xposTo, yposTo, 0x2000, 0, bp1a);
			//^2FCF:138D
			MOVE_RECORD_TO(bp1c, xposTo, yposTo, bp16, bp18);
			//^2FCF:13A3
			goto _13ce;
		}

		//^2FCF:13A5
_13a5:
		{
			if (bp06 != 255) {
				//^2FCF:13AC
				_1c9a_0fcb(bp06);
			}
			//^2FCF:13B5
			SET_TIMER_3C_OR_3D(si, xposTo, yposTo, di, _4976_5822);
			//^2FCF:13CB
			return 2;
		}

		//^2FCF:13CE
_13ce:
		{
			if (bp10 != 0) {
				//^2FCF:13D4
				APPEND_RECORD_TO(si, NULL, xposTo, yposTo);
			}
			else {
				//^2FCF:13E9
				PLACE_OR_REMOVE_OBJECT_IN_ROOM(xposTo, yposTo, si, 0, FCT_PLACE_ON, 0);
			}
			//^2FCF:13FE
			SET_MINION_RECENT_OPEN_DOOR_LOCATION(si, xposTo, yposTo, di, 0);
			//^2FCF:140F
			if (bp06 == 255) {
				//^2FCF:1416
				if ((QUERY_CREATURE_AI_SPEC_FLAGS(si) & 1) == 0) {
					//^2FCF:1425
					ALLOC_CAII_TO_CREATURE(si, xposTo, yposTo);
					//^2FCF:1434
				}
			}
			else {
				//^2FCF:1437
				bp06 = glbTabCreaturesInfo[bp06].TimerIndex();
				//^2FCF:144C
				if (bp06 != 0xffff) {
					//^2FCF:1454
					glbTimersTable[bp06].Xcoord((Bit8u)xposTo);
					//^2FCF:1466
					glbTimersTable[bp06].Ycoord((Bit8u)yposTo);
					//^2FCF:147B
					glbTimersTable[bp06].SetMap(di);
					//^2FCF:14C3
				}
			}
			goto _1562;
		}
	}
	else {
		//^2FCF:14C6
		CHANGE_CURRENT_MAP_TO(di);
		//^2FCF:14CD
		if (bp0a == dbCloud) {
			//^2FCF:14D3
			APPEND_RECORD_TO(si, NULL, xposTo, yposTo);
			//^2FCF:14E6
		}
		else {
			//^2FCF:14E8
			Bit16u bp14 = (xposFrom < 0 && yposFrom < 0) ? 1 : 0;
			//^2FCF:14FE
			if (bp14 == 0 && di != bp08) {
				//^2FCF:1507
				bp1c = GET_CREATURE_AT(xposTo, yposTo);
				//^2FCF:1517
				if (bp1c != OBJECT_NULL) {
					//^2FCF:151C
					bp14 = QUERY_CREATURE_AI_SPEC_FLAGS(bp1c) & 1;
				}
			}
			//^2FCF:152B
			PLACE_OR_REMOVE_OBJECT_IN_ROOM(
				xposTo, 
				yposTo, 
				si, 
				(glbCurrentMapIndex == glbMap_4c28 && xposTo == glbSomePosX_4c2e && yposTo == glbSomePosY_4c30) ? 1 : 0,
				FCT_PLACE_ON,
				bp14
				);
		}
	}
	//^2FCF:1562
_1562:
	_4976_581e = bp1e;
	_4976_5826 = xposTo;
	_4976_5828 = yposTo;
	_4976_581c = di;
	//^2FCF:1578
	CHANGE_CURRENT_MAP_TO(bp08);
	//^2FCF:1581
	return 0;
	//^2FCF:1583
}



//^1C9A:3C30
// SPX: _1c9a_3c30 Something related to loading creatures ?
void SkWinCore::__LOAD_CREATURE_FROM_DUNGEON()
{
	ENTER(4);
	glbCreaturesCount = 0;
	//printf("glbDBObjectData: %16X\n", glbDBObjectData[dbCreature]);
	Creature *pCreature = reinterpret_cast<Creature *>(static_cast<X8 *>(glbDBObjectData[dbCreature]));

	U16 iCreatureIndex = dunHeader->nRecords[DB_CATEGORY_CREATURE];
	for (; iCreatureIndex-- != 0; pCreature++) {
		//LOG_HEXA((X8*)pCreature, 16);
		//printf("LCFD: %d/%d [%16X] (%02d) POSS:%02X | %02X\n",iCreatureIndex, glbCreaturesCount, pCreature, sizeof(pCreature), (U16) pCreature->w0, (X8) pCreature->GetPossessionObject()); 
		if (pCreature->w0 != OBJECT_NULL
			//&& QUERY_CREATURE_AI_SPEC_FROM_TYPE(pCreature->CreatureType())->IsStaticObject() == 0
			)
			glbCreaturesCount++;
	}
	glbCreaturesCount = min_value(glbCreaturesCount + 0x4B, dunHeader->nRecords[DB_CATEGORY_CREATURE]);
	//printf("glbCreaturesCount * size(CreatureInfoData): %d * %d = %d\n", glbCreaturesCount, sizeof(CreatureInfoData), glbCreaturesCount * sizeof(CreatureInfoData));
	glbTabCreaturesInfo = reinterpret_cast<CreatureInfoData *>(ALLOC_MEMORY_RAM(glbCreaturesCount * sizeof(CreatureInfoData), afUseUpper, 0x400));
	return;
}



//^14CD:2B9A
ObjectID SkWinCore::FIND_TILE_ACTUATOR(X16 xx, X16 yy, i8 dir, X16 actuatorType)
{
	//^14CD:2B9A
	ENTER(6);
	//^14CD:2BA0
	ObjectID si = GET_TILE_RECORD_LINK(xx, yy);
	X16 di = 0;
	for (; si != OBJECT_END_MARKER; si  = GET_NEXT_RECORD_LINK(si)) {
		//^14CD:2BB3
		i16 bp06 = si.DBType();
		if (bp06 > dbActuator)
			break;
		//^14CD:2BC3
		if (bp06 != dbActuator || dir != -1) {
			//^14CD:2BCF
			if (si.Dir() != U8(dir))
				continue;
		}
		//^14CD:2BDD
		Actuator *bp04 = GET_ADDRESS_OF_ACTU(si);
		di = (bp04->ActuatorType() == actuatorType) ? 1 : 0;
		if (di != 0)
			break;
		//^14CD:2C06
	}
	//^14CD:2C14
	return (di != 0) ? si : OBJECT_NULL;
}


