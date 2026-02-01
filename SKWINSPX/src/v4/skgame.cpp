//------------------------------------------------------------------------------
// SK-GAME
//------------------------------------------------------------------------------

//#include <StdAfx.h>	// Required for MVC6 compilation

#include <skver.h>

//#include <conio.h> // getch

#include <skcore.h>
#include <skdebug.h>
#include <skcnsole.h>
#include <sklua.h>
#include <skparam.h>

//#if defined(_USE_MFC80) || defined(_USE_MFC60)
//#include <SKMFC.h>
//#include <resource.h>
//#endif // defined(_USE_MFC80) || defined(_USE_MFC60)

#include <skglobal.h> // for skwin


void SkWinCore::LUA_CALL_SCRIPT(U8 iScriptEvent, i16 iPosMap, i16 iPosX, i16 iPosY, i16 iDir)
{
	char sScriptName[512];
	char sLocalScriptName[128];
	sprintf(sLocalScriptName, "dungeon.lua");
//	if (skwin.dung == _OPTION_DUNGEON_EOB_EOB1_)
//		sprintf(sLocalScriptName, "eobmap%02d.lua", iPosMap);		
//		sprintf(sLocalScriptName, "eobmap.lua");		
	sprintf(sScriptName, "%s/scripts/%s", skWinApp->GET_DATA_FOLDER_NAME(), sLocalScriptName);
	
	switch (iScriptEvent)
	{
		case _EXP_SCRIPT__GAME_INIT_:
			SkWinLua_RunFunctionFromScript(this, sScriptName, "init", 0, 0, 0, 0);
			break;
		case _EXP_SCRIPT__PLAYER_INIT_:
			SkWinLua_RunFunctionFromScript(this, sScriptName, "player_init", 0, 0, 0, 0);
			break;
		case _EXP_SCRIPT__PLAYER_ON_TILE_:
			SkWinLua_RunFunctionFromScript(this, sScriptName, "player_on_tile", iPosMap, iPosX, iPosY, iDir);
			break;
		case _EXP_SCRIPT__PLAYER_TURN_ON_TILE_:
			SkWinLua_RunFunctionFromScript(this, sScriptName, "player_turned_on_tile", iPosMap, iPosX, iPosY, iDir);
			break;
		case _EXP_SCRIPT__ANY_EVENT_:
			SkWinLua_RunFunctionFromScript(this, sScriptName, "event_any", iPosMap, iPosX, iPosY, iDir);
			break;
	};
	;
}


//^0CEE:29EC
U16 SkWinCore::ADD_ITEM_CHARGE(ObjectID recordLink, i16 delta)
{
	//^0CEE:29EC
	U16 si = 0;
	//^0CEE:29F4
	if (recordLink == OBJECT_NULL)
		return si;
	//^0CEE:29FD
	GenericRecord *bp04 = (GenericRecord *)GET_ADDRESS_OF_RECORD(recordLink);
	//^0CEE:2A0B
	U16 bp06 = recordLink.DBType();
	//^0CEE:2A17
	U16 di;
	switch (bp06) {
		case dbWeapon:
			//^0CEE:2A29
			si = reinterpret_cast<Weapon *>(bp04)->Charges();
			di = 0x000F;
			break;
		case dbCloth:
			//^0CEE:2A3D
			si = reinterpret_cast<Cloth *>(bp04)->Charges();
			di = 0x000F;
			break;
		case dbMiscellaneous_item:
			//^0CEE:2A48
			si = reinterpret_cast<Miscellaneous_item *>(bp04)->Compass();
			di = 0x03;
			break;
		default:
			//^0CEE:2AB6
			return si;
	}
	//^0CEE:2A5A
	si += delta;
	//^0CEE:2A5D
	si = BETWEEN_VALUE(0, si, di);
	//^0CEE:2A6B
	switch (bp06) {
		case dbWeapon:
			//^0CEE:2A7F
			reinterpret_cast<Weapon *>(bp04)->Charges(si);
			break;
		case dbCloth:
			//^0CEE:2A91
			reinterpret_cast<Cloth *>(bp04)->Charges(si);
			break;
		case dbMiscellaneous_item:
			//^0CEE:2AA2
			reinterpret_cast<Miscellaneous_item *>(bp04)->Compass(si);
			break;
		default:
			//^0CEE:2AB6
			return si;
	}
	//^0CEE:2AB6
	return si;
}

//^0CEE:26E5
U16 SkWinCore::IS_CONTAINER_MONEYBOX(ObjectID recordLink)
{
	//^0CEE:26E5
	if (recordLink.DBType() == dbContainer) {
		//^0CEE:26F9
		if (GET_ADDRESS_OF_RECORD9(recordLink)->ContainerType() == 0) {
			//^0CEE:2710
			if (QUERY_GDAT_ENTRY_DATA_INDEX(0x14, QUERY_CLS2_FROM_RECORD(recordLink), 0x0005, 0x0040) != (U16)-1) {
				// Exists possible item list? (e.g. "J26-28 J8 J56 J6<00>")
				return 1;
			}
		}
	}
	//^0CEE:272F
	return 0;
}

//^0CEE:25E5
U16 SkWinCore::QUERY_GDAT_DBSPEC_WORD_VALUE(ObjectID rl, Bit8u cls4)
{
	//^0CEE:25E5
	ObjectID si = rl;
	//^0CEE:25EC
	if (si == OBJECT_NULL)
		return 0;
	//^0CEE:25F5
	return QUERY_GDAT_ENTRY_DATA_INDEX(QUERY_CLS1_FROM_RECORD(si), QUERY_CLS2_FROM_RECORD(si), dtWordValue, cls4);
}

//^0CEE:2ACC
i32 SkWinCore::QUERY_ITEM_VALUE(ObjectID recordLink, Bit8u cls4)
{
	//^0CEE:2ACC
	ObjectID si = recordLink;
	if (si == OBJECT_NULL) {
		//^0CEE:2ADA
		return 0;
	}
	//^0CEE:2AE1
	i32 bp08 = QUERY_GDAT_DBSPEC_WORD_VALUE(si, cls4);
	if (cls4 == 1) { // 0x01 -> Weight in Kg (x10)
		//^0CEE:2AFA
		i32 bp0c = QUERY_GDAT_DBSPEC_WORD_VALUE(si, 0x34); // 0x34 -> Max charge count
		//^0CEE:2B0B
		if (bp0c > 0) {
			//^0CEE:2B15
			bp08 += ADD_ITEM_CHARGE(si, 0) * bp0c;
		}
	}
	//^0CEE:2B34
	if (cls4 == 2) { // 0x02 -> Money value
		//^0CEE:2B3D
		i32 bp0c = QUERY_GDAT_DBSPEC_WORD_VALUE(si, GDAT_ITEM_BONUS_MONEY_PER_CHARGE); // 0x35 -> Additional money value per charge??
		//^0CEE:2B4E
		if (bp0c > 0) {
			//^0CEE:2B58
			bp08 += ADD_ITEM_CHARGE(si, 0) * bp0c;
		}
		//^0CEE:2B77
		if (si.DBType() == dbPotion && bp08 > 1) {
			//^0CEE:2B92
			bp08 /= 2;
			//^0CEE:2BA2
			bp08 += (GET_ADDRESS_OF_RECORD(si)->castToPotion()->PotionPower() * bp08) / 255L;
		}
	}
	//^0CEE:2BD3
	if (si.DBType() == dbContainer) {
		//^0CEE:2BE3
		Container *bp04 = GET_ADDRESS_OF_RECORD(si)->castToContainer();
		//^0CEE:2BEF
		if (bp04->ContainerType() == 0) {
			//^0CEE:2C02
			U16 di = IS_CONTAINER_MONEYBOX(si);
			ObjectID si = bp04->GetContainedObject();
			i32 bp0c = 0;
			//^0CEE:2C1B
			for (; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
				//^0CEE:2C1D
				if (di != 0) {
					//^0CEE:2C21
					if (si.DBType() == dbMiscellaneous_item) {
						//^0CEE:2C2E
						bp0c += QUERY_GDAT_DBSPEC_WORD_VALUE(si, cls4) * (GET_ADDRESS_OF_RECORD(si)->castToMisc()->Charge() +1L);
						continue;
					}
				}
				//^0CEE:2C65
				bp08 += QUERY_ITEM_VALUE(si, cls4);
				//^0CEE:2C76
			}
			//^0CEE:2C83
			if (di != 0) {
				//^0CEE:2C87
				if (cls4 == 1) {
					//^0CEE:2C8D
					bp08 += (bp0c +4L) / 5L;
				}
				else {
					//^0CEE:2CAC
					bp08 += bp0c;
				}
			}
		}
	}
	//^0CEE:2CB8
	return bp08;
}

//^0CEE:2CC2
i32 SkWinCore::QUERY_ITEM_WEIGHT(ObjectID recordLink)
{
	//^0CEE:2CC2
	return QUERY_ITEM_VALUE(recordLink, 1);
}

//^0CEE:2734
U16 SkWinCore::IS_CONTAINER_CHEST(ObjectID recordLink)
{
	//^0CEE:2734
	ObjectID si = recordLink;
	//^0CEE:273B
	if (true
		&& si.DBType() == dbContainer
		&& IS_CONTAINER_MONEYBOX(si) == 0
		&& GET_ADDRESS_OF_RECORD9(si)->ContainerType() == 0
	) {
		//^0CEE:2769
		return 1;
	}
	//^0CEE:276E
	return 0;
}










//^0CEE:0002
void SkWinCore::CHANGE_CURRENT_MAP_TO(U16 new_map)
{
	// CSBWin:CSBCode.cpp/LoadLevel (TAG00a9d4)
	//SkD((DLV_DBG_INIT, "CHANGE_CURRENT_MAP_TO => %02d\n", new_map));

	i16 iLocalMap = new_map;	// si
	//if (glbCurrentMapIndex == iLocalMap)
	//	return;

	if (iLocalMap < 0)
		return;

	ATLASSERT(new_map < dunHeader->nMaps);

	glbCurrentMapIndex = iLocalMap;
	glbCurrentTileMap = glbMapTileValue[iLocalMap];
	dunMapLocalHeader = &dunMapsHeaders[iLocalMap];
	glbCurrentMapWidth = dunMapLocalHeader->Column();
	glbCurrentMapHeight = dunMapLocalHeader->Row();
	//SkD((DLV_DBG_INIT, "CHANGE_CURRENT_MAP_TO: pCurrentTileMap :%08X\n", glbCurrentTileMap));
	_4976_4c52 = &dunMapTilesObjectIndexPerColumn[dunMapColumnsSumArray[iLocalMap]];
	if (_4976_4c08 != 0) {
		if (iLocalMap == glbMap_4976_4c12) {
			glbSomePosX_4c2e = glbSomePosX_4976_4c04;
			glbSomePosY_4c30 = glbSomePosY_4976_4c1e; 
			glbMap_4c28 = glbMap_4976_4c12;
			_4976_4c2c = glbDir_4976_4c10;

			return;
		}
	}
	glbSomePosX_4c2e = cd.pi.glbPlayerPosX;
	glbSomePosY_4c30 = cd.pi.glbPlayerPosY;
	glbMap_4c28 = cd.pi.glbPlayerMap;
	_4976_4c2c = cd.pi.glbPlayerDir;

}



//^0CEE:0510
i16 SkWinCore::LOCATE_OTHER_LEVEL(U16 curmap, i16 zDelta, i16 *xx, i16 *yy, Bit8u **ss)
{
	//^0CEE:0510
	//^0CEE:0516
	Map_definitions *bp04 = &dunMapsHeaders[curmap];
	i16 bp0a = (*xx += bp04->MapOffsetX());
	i16 bp0c = (*yy += bp04->MapOffsetY());
	i16 bp16 = bp04->Level() + zDelta;


	// SPX: in BW mode, move +2 tiles in the current direction when using stairs
	if (SkCodeParam::bBWMode) {
		i16 iXtraDisX = 0;
		i16 iXtraDisY = 0;
		if (glbPlayerSpecialStairsDir != 0xFFFF) {
			ATLASSERT(glbPlayerSpecialStairsDir < 4);
			iXtraDisX = _4976_19b6[glbPlayerSpecialStairsDir]*2; // x
			iXtraDisY = (_4976_19b2[glbPlayerSpecialStairsDir])*(-2); // y
		}
		bp0a = (*xx += iXtraDisX);
		bp0c = (*yy += iXtraDisY);
	}
	
	//^0CEE:0560
	if (bp16 >= 0 && bp16 < MAXDEPTH) {
		//^0CEE:0572
		U16 bp14 = _4976_4cb0[bp16];
		//^0CEE:0581
		if (bp14 != 0xffff) {
			//^0CEE:0589
			Bit8u *bp08;
			if (ss != NULL && *ss != NULL) {
				//^0CEE:059D
				bp08 = *ss + 1;
			}
			else {
				//^0CEE:05AE
				bp08 = &_4976_4c72[bp14];
			}

			i16 bp0e;
			for (; (bp0e = ((i8)*bp08)) != -1; bp08++) {
				//^0CEE:05BA
				bp04 = &dunMapsHeaders[bp0e];
				//^0CEE:05D0
				i16 di = bp04->MapOffsetX();
				//^0CEE:05DB
				if (di -1 <= bp0a) {
					//^0CEE:05E4
					if (di + bp04->RawColumn() + 1 >= bp0a) {
						//^0CEE:05FB
						i16 si = bp04->MapOffsetY();
						//^0CEE:0603
						if (si -1 <= bp0c) {
							//^0CEE:060C
							if (si + bp04->RawRow() >= bp0c) {
								//^0CEE:0623
								di = bp0a - di;
								si = bp0c - si;
								//^0CEE:0631
								U16 bp12 = glbCurrentMapIndex;
								//^0CEE:0637
								CHANGE_CURRENT_MAP_TO(bp0e);
								//^0CEE:063F
								U16 bp10 = GET_TILE_VALUE(di, si) >> 5;
								//^0CEE:0651
								if (bp10 == ttTeleporter) {
									//^0CEE:0656
									if (GET_ADDRESS_OF_TILE_RECORD((Bit8u)di, (Bit8u)si)->castToTeleporter()->w4_0_0() != 0) {
										//^0CEE:066E
										bp10 = 7;
									}
								}
								//^0CEE:0673
								CHANGE_CURRENT_MAP_TO(bp12);
								//^0CEE:067B
								if (bp10 != ttMapExit) {
									//^0CEE:0681
									*xx = di;
									*yy = si;
									//^0CEE:068D
									if (ss != NULL) {
										//^0CEE:0695
										*ss = bp08;
									}
									//^0CEE:06A5
									return bp0e;
								}
							}
						}
					}
				}
				//^0CEE:06AA
			}
		}
	}
	//^0CEE:06BF
	if (ss != NULL) {
		//^0CEE:06C7
		*ss = 0;
	}
	//^0CEE:06D5
	// SPX: This case is bad. It will make the further SET_TILE_ATTRIBUTE_02 function crash
	return -1;
}



//^24A5:013D
void SkWinCore::RECALC_LIGHT_LEVEL()
{
	if (SkCodeParam::bUseIngameDebug)
	{
		U8 message[32];
		sprintf((char*)message, "PRECOMP LIGHT %d\n", glbPrecomputedLight);
		//DISPLAY_HINT_TEXT(COLOR_YELLOW, message);
	}
	//^24A5:013D
	ENTER(28);
	//^24A5:0143
	if (dunMapsHeaders[cd.pi.glbPlayerMap].Difficulty() == 0) {
		//^24A5:015D
		glbLightLevel = 1;
		if (SkCodeParam::bAutoDefaultMaxLight)
			glbLightLevel = 0;
	}
	else {
		//^24A5:0166
		U16 itemLightBonus[9];		// (bp1c) SPX: light bonus for holded item (1) + items in hand (2*4)
		ZERO_MEMORY(itemLightBonus, 9);
		//^24A5:0177
		U16 bonusIndex = 0;	// bp06
		//^24A5:017C
		// Give light bonus for item currently in hand (if any)
		if ((QUERY_GDAT_DBSPEC_WORD_VALUE(cd.pi.glbLeaderHandPossession.object, GDAT_ITEM_STATS_GEN_FLAGS) & ITEM_FLAG_PRODUCE_LIGHT) != 0) {
			//^24A5:018E
			itemLightBonus[bonusIndex] = ADD_ITEM_CHARGE(cd.pi.glbLeaderHandPossession.object, 0);
			bonusIndex++;
		}
		//^24A5:01AB
		U16 si;
		// Pass through all items in hands
		for (si=0; si < cd.pi.glbChampionsCount; si++) {
			//^24A5:01AF
			for (U16 bp04=0; bp04 <= 1; bp04++) {
				//^24A5:01B6
				U16 bp08;
				if ((QUERY_GDAT_DBSPEC_WORD_VALUE(bp08 = glbChampionSquad[si].Possess(bp04), GDAT_ITEM_STATS_GEN_FLAGS) & ITEM_FLAG_PRODUCE_LIGHT) != 0) {
					//^24A5:01DC
					itemLightBonus[bonusIndex] = ADD_ITEM_CHARGE(bp08, 0);
					//^24A5:01F5
					bonusIndex++;
				}
				//^24A5:01F8
			}
			//^24A5:0201
		}
		//^24A5:0208
		//for (si=0; bonusIndex -1 > si; si++) {
		// One pass of bubble sort, what for ?
		for (si=0; si < bonusIndex -1; si++) {
			//^24A5:020C
			if (itemLightBonus[si] > itemLightBonus[si +1]) {
				//^24A5:0226
				U16 bp04 = itemLightBonus[si];
				//^24A5:0235
				itemLightBonus[si] = itemLightBonus[si +1];
				//^24A5:024D
				itemLightBonus[si +1] = bp04;
			}
			//^24A5:025C
		}
		//^24A5:0265
		U16 bp02 = 6;
		//U16 di;
		i16 di = 0;	// SPX (2016-11-02) replacing unsigned by signed to hold negative light (with use of darkness) and hit minimum threshold
		// SPX: This is strange ... does that mean that each new value to add is decreased by its position on the light bonus table?
		for (di=0, si=0; si < bonusIndex; si++) {
			//^24A5:0270
			di += (tLightLevelItem[RCJ(16,itemLightBonus[si])] << bp02) >> 6;
			//^24A5:028B
			bp02 = max_value(0, bp02 -1);
			//^24A5:029C
		}
		//^24A5:02A2
		di += glbPrecomputedLight;	// SPX: is some sort of precalculed light level (depending on door and such?)
		di += glbGlobalSpellEffects.Light;
		//^24A5:02AA
		di = di + QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, dtWordValue, GDAT_GFXSET_AMBIANT_LIGHT);
		// SPX: GDAT_CATEGORY_GRAPHICSSET is 0x08
		//^24A5:02C2
		if (glbRainFlagSomething != 0) {
			//^24A5:02C9
			di += tLightLevelTable[RCJ(6,BETWEEN_VALUE(0, glbRainAmbientLightModifier + glbRainSomeLightMod, 5))];
		}
		if (SkCodeParam::bUseFixedMode && di < 0)
			di = 0;
		//^24A5:02EA
		// Light threshold is at 100 for the brighter light => lightlevel = 5.
		for (glbLightLevel=0; glbLightLevel <= 5; glbLightLevel++) {
			//^24A5:02F2
			if (tLightLevelTable[RCJ(6,glbLightLevel)] < di)
				break;
			//^24A5:02FF
		}
		//^24A5:030A
		// SPX: Get the highest default light (from wallset type)
		i16 bp0a = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, dtWordValue, GDAT_GFXSET_HIGHEST_LIGHT_LEVEL);
		if (SkCodeParam::bAutoDefaultMaxLight)
			bp0a = 0;
		//^24A5:031F
		if (glbLightLevel < bp0a) {
			//^24A5:0327
			glbLightLevel = bp0a;
		}
		//^24A5:032D
		if (glbRainFlagSomething != 0) {
			//^24A5:0334
			if (glbRecalcLightIndicator != 0) {
				//^24A5:033B
				glbLightLevel = 0;
			}
		}
	}
	//^24A5:0341
	//printf("Light level before modifier is %d\n", glbLightLevel);
	glbLightLevel -= (glbLightModifier > 12) ? 1 : 0;
	//printf("Light level before bound is %d\n", glbLightLevel);
	//^24A5:0353
	glbLightLevel = BETWEEN_VALUE(0, glbLightLevel, 5);	// SPX: minimum is 5 ? but it can be even darker with 6
	if (SkCodeParam::bFullLight)	// SPX: debug feature added to always get full light
		glbLightLevel = 0;
	//printf("Light level final is %d\n", glbLightLevel);
	//^24A5:0366
}



//^1031:0000
// SPX: returns 1 ?? that's all ?
// SPX: _1031_0000 renamed RETURN_1
U16 SkWinCore::RETURN_1(sk1891 *ref)
{
	ENTER(0);
	return 1;
}

//^1031:0008
// SPX: _1031_0008 renamed IS_GAME_ENDED
U16 SkWinCore::IS_GAME_ENDED(sk1891 *ref)
{
	ENTER(0);
	return (ref->b1 == cd.gg.glbGameHasEnded) ? 1 : 0;
}

//^1031:0023
// _1031_0023
U16 SkWinCore::_1031_0023_PFN12_02(sk1891 *ref)
{
	ENTER(0);
	return (ref->b1 == _4976_4bd2) ? 1 : 0;
}




//^12B4:0141
// SPX: _12b4_0141 renamed PERFORM_TURN_SQUAD
void SkWinCore::PERFORM_TURN_SQUAD(U16 xx)
{
	//^12B4:0141
	ENTER(6);
	//^12B4:0146
	if (xx == 0)
		//^12B4:014C
		//^12B4:023C
		return;
	//^12B4:014F
	cd.gg.glbRefreshViewport = 1;
	//^12B4:0155
	RESET_SQUAD_DIR();
	//^12B4:0159
	U16 si = GET_TILE_VALUE(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY);
	//^12B4:016C
	if (si >> 5 == ttStairs) {
		//^12B4:0173
		// _12b4_00af(si & 4); // original
		_12b4_00af(si & 4, si & 8);
		//^12B4:017E
		//^12B4:023C
		return;
	}
	//^12B4:0181
	TELE_inf bp06;
	if (GET_TELEPORTER_DETAIL(&bp06, U8(cd.pi.glbPlayerPosX), U8(cd.pi.glbPlayerPosY)) != 0) {
		//^12B4:019A
		CHANGE_CURRENT_MAP_TO(cd.pi.glbPlayerMap);
		//^12B4:01A4
		MOVE_RECORD_TO(OBJECT_NULL, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, -1, 0);
		//^12B4:01BA
		LOAD_NEWMAP(bp06.b4);
		//^12B4:01C6
		MOVE_RECORD_TO(OBJECT_NULL, -1, 0, bp06.b2, bp06.b3);
		//^12B4:01E0
		ROTATE_SQUAD(bp06.b1);
		//^12B4:01EB
		//^12B4:023C
		return;
	}
	//^12B4:01ED
	PLACE_OR_REMOVE_OBJECT_IN_ROOM(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, OBJECT_NULL, 1, FCT_REMOVE_OFF, 0);
	//^12B4:0205
	ROTATE_SQUAD((cd.pi.glbPlayerDir + ((xx == 2) ? 1 : 3)) & 3);
	//^12B4:0224
	PLACE_OR_REMOVE_OBJECT_IN_ROOM(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, OBJECT_NULL, 1, FCT_PLACE_ON, 0);
	LUA_CALL_SCRIPT(_EXP_SCRIPT__PLAYER_TURN_ON_TILE_, cd.pi.glbPlayerMap, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 0);
	//^12B4:023C
	return;
}


//^2405:00E7
// NOTHING ?!
void SkWinCore::_2405_00e7()
{
	//^2405:00E7
	ENTER(0);
	//^2405:00EA
	return;
}



//^075F:000B
// SPX: TODO I wonder if aa is rather direction
void SkWinCore::SHOOT_ITEM(ObjectID rlItemThrown, U16 xx, U16 yy, U16 dir, U16 aa, U16 energyVal, U16 ene2Val, U16 dd)
{
	ATLASSERT(rlItemThrown.DBType() != dbDoor); // you cannot throw this kind of objects.
	ATLASSERT(rlItemThrown.DBType() != dbTeleporter); // you cannot throw this kind of objects.
	ATLASSERT(rlItemThrown.DBType() != dbText); // you cannot throw this kind of objects.
	ATLASSERT(rlItemThrown.DBType() != dbActuator); // you cannot throw this kind of objects.
	ATLASSERT(rlItemThrown.DBType() != dbCreature); // you cannot throw this kind of objects.
	ATLASSERT(rlItemThrown.DBType() != db11); // you cannot throw this kind of objects.
	ATLASSERT(rlItemThrown.DBType() != db12); // you cannot throw this kind of objects.
	ATLASSERT(rlItemThrown.DBType() != db13); // you cannot throw this kind of objects.
	ATLASSERT(rlItemThrown.IsMissile() || (!rlItemThrown.IsMissile() && rlItemThrown.DBType() != dbCloud)); // you cannot throw this kind of objects.

	//^075F:000B
	ENTER(14);
	//^075F:0011
	ObjectID di = rlItemThrown;
	//^075F:0014
	ObjectID si = ALLOC_NEW_RECORD(dbMissile);
	//^075F:001E
	if (si == OBJECT_NULL) {
		//^075F:0023
		if (di.DBType() != dbCloud) {
			//^075F:0033
			MOVE_RECORD_TO(ObjectID(di, dir), -1, 0, xx, yy);
		}
		//^075F:0054
		return;
	}
	//^075F:0057
	QUEUE_NOISE_GEN2(
		QUERY_CLS1_FROM_RECORD(di),
		QUERY_CLS2_FROM_RECORD(di),
		SOUND_STD_THROW,
		0xfe,
		xx,
		yy,
		1,
		0x6e,
		0x80
		);
	//^075F:0082
	si = ObjectID(si, dir);
	//^075F:0092
	Missile *missile = GET_ADDRESS_OF_RECORDE(si);	//*bp04
	//^075F:009F
	missile->SetMissileObject(di);
	//^075F:00A6
	missile->EnergyRemaining(U8(min_value(energyVal, 255)));
	//^075F:00BD
	missile->EnergyRemaining2(U8(ene2Val));
	//^075F:00C4
	APPEND_RECORD_TO(si, NULL, xx, yy);
	//^075F:00D7
	if (missile->GetMissileObject() == OBJECT_EFFECT_FIREBALL && glbCurrentMapIndex == cd.pi.glbPlayerMap)	// oFF80
		//^075F:00EA
		glbDoLightCheck = 1;
	//^075F:00F0
	Timer bp0e;
	bp0e.SetMap(glbCurrentMapIndex);
	bp0e.SetTick(glbGameTick +1);
	//^075F:0113
	bp0e.TimerType((_4976_4b7e != 0) ? tty1E : tty1D);
	//^075F:0124
	bp0e.actor = TIMER_ACTOR__00;
	//^075F:0128
	bp0e.value = si;
	//^075F:012B
	bp0e.w8_0_4(xx);
	//^075F:0138
	bp0e.w8_5_9(yy);
	//^075F:0149
	bp0e.w8_c_f(dd);
	//^075F:015B
	bp0e.w8_a_b(aa);
	//^075F:016B
	missile->TimerIndex(QUEUE_TIMER(&bp0e));
	//^075F:017E
	return;
}





//^48AE:0001
U16 SkWinCore::GET_GLOB_VAR(U16 var)
{
	// 0x00-0x3f: 1bit value
	// 0x40-0x7f: byte value
	// 0x80-0xbf: word value

	//^48AE:0001
	ENTER(0);
	//^48AE:0004
	if (var <= 0x3f) {
		//^48AE:000A
		if ((glbIngameGlobVarFlags[var >> 3] & (1 << (var & 7))) == 0)
			//^48AE:0023
			//^48AE:0054
			return 0;
		//^48AE:0025
		return 1;
	}
	//^48AE:002A
	if (var <= 0x7f) {
		//^48AE:0030
		return glbIngameGlobVarBytes[var -0x40];
	}
	//^48AE:003E
	if (var <= 0xbf) {
		//^48AE:0045
		return glbIngameGlobVarWords[var -0x80];
	}
	//^48AE:0054
	return 0;
}

//^48AE:0058
U16 SkWinCore::UPDATE_GLOB_VAR(U16 var, U16 newval, U16 op)
{
	// op == 0: var = 1
	// op == 1: var = 0
	// op == 2: var = !var
	// op == 3: var += newval
	// op == 4: var -= newval
	// op == 6: var = newval

	//^48AE:0058
	ENTER(0);
	//^48AE:005D
	U16 di = newval;
	//^48AE:0060
	U16 si = GET_GLOB_VAR(var);
	//^48AE:006A
	switch (op) {
		case 0:
			//^48AE:0079
			si = 1;
			//^48AE:007C
			break;

		case 1:
			//^48AE:007E
			//^48AE:0084
			si = 0;
			//^48AE:0086
			break;

		case 2:
			//^48AE:0080
			si = (si != 0) ? 0 : 1;
			//^48AE:008B
			break;

		case 3:
			//^48AE:008D
			si += di;
			//^48AE:008F
			break;

		case 4:
			//^48AE:0091
			si -= di;
			//^48AE:0093
			break;

		case 5:
			break;

		case 6:
			//^48AE:0095
			si = di;
			break;
	}
	//^48AE:0097
	// var x00 - x3F => FLAGS 0 to 8 (in fact var = #FLAG*8 ...)
	if (var <= 0x3f) {
		//^48AE:009D
		op = 1 << (var & 7);
		//^48AE:00AB
		var >>= 3;
		//^48AE:00AF
		if (si != 0) {
			//^48AE:00B3
			si = 1;
			//^48AE:00B6
			glbIngameGlobVarFlags[var] |= U8(op);
			//^48AE:00C0
			//^48AE:0106
			return si;
		}
		else {
			//^48AE:00C2
			glbIngameGlobVarFlags[var] &= ~U8(op);
			//^48AE:00CE
			//^48AE:0106
			return si;
		}
	}
	//^48AE:00D0
	// var x40 - x7F => BYTE 00 to 64
	if (var <= 0x7f) {
		//^48AE:00D6
		si = BETWEEN_VALUE(0, si, 255);
		//^48AE:00E6
		glbIngameGlobVarBytes[var -0x40] = U8(si);
		//^48AE:00F0
		//^48AE:0106
		return si;
	}
	//^48AE:00F2
	// var x80 - xbf => WORD 00 to 64
	if (var <= 0xbf) {
		//^48AE:00F9
		glbIngameGlobVarWords[var -0x80] = si;
	}
	//^48AE:0106
	return si;
}


//^29EE:0B4A
// SPX: _29ee_0b4a renamed PROCEED_SPELL_FAILURE
void SkWinCore::PROCEED_SPELL_FAILURE(U16 xx)
{
	//^29EE:0B4A
	ENTER(0);

	//^29EE:0B4F
	U16 di = 1;
	//^29EE:0B52
	U16 si;
	switch (xx & 0xfff0) {
		case 0x0010:
			//^29EE:0B69
			if ((xx & 15) == 3) {
				//^29EE:0B74
				glbChampionAttackDamage = ATTACK_FAILURE_WIZARD;	// -4
			}
			else {
				//^29EE:0B7C
				glbChampionAttackDamage = ATTACK_FAILURE_PRIEST;	// -5
			}
			//^29EE:0B82
			si = 0x45;
			//^29EE:0B85
			goto _0baa;

		case 0x0020:
			//^29EE:0B87
			glbChampionAttackDamage = ATTACK_MEANINGLESS_SPELL;	// -3
			//^29EE:0B8D
			si = 0x46;
			//^29EE:0B90
			goto _0baa;

		case 0x0030:	// need flask in hand 
			//^29EE:0B92
			//DRAW_TRANSPARENT_STATIC_PIC(0x01, 0x05, 0x0B, 0x5c, -1);
			DRAW_TRANSPARENT_STATIC_PIC(GDAT_CATEGORY_INTERFACE_GENERAL, GDAT_INTERFACE_CLASS_SPELLING, GDAT_IMG_NEED_FLASK, 0x5c, -1);
			//^29EE:0BA4
			si = 0x44;
			di = 0x03;
			//^29EE:0BAA
_0baa:
			glbSomeChampionPanelFlag = max_value(di, 8 - UPDATE_GLOB_VAR(si, 1, 3));

			break;
	}
	//^29EE:0BC8
	return;
}


//^2759:274D
U16 SkWinCore::TRY_CAST_SPELL()
{
	ENTER(10);
	U16 iChampionIndex = cd.pi.glbChampionIndex - 1;	// bp0a
	Champion* xChampion = &glbChampionSquad[iChampionIndex];	// bp04
	SpellDefinition* xSpellDef = FIND_SPELL_BY_RUNES(xChampion->GetRunes()); // bp08
	// SPX: then si = 32 means a "meaningless spell", not existing spell
	U16 si = (xSpellDef == NULL) 
		? 32
		: CAST_SPELL_PLAYER(iChampionIndex, xSpellDef, i16(xChampion->GetRunes()[0]) - 0x5F);	// (0x5F is first rune (0x60) - 1)
	// SPX: di = 0 : spell ok / di = 1 : spell failed
	U16 di = (si != 0) ? 1 : 0;
	if (di != 0)
		PROCEED_SPELL_FAILURE(si);
	si &= 0x00f0;
	if (si != 0x0030) {
		xChampion->GetRunes()[0] = 0;
		xChampion->runesCount = 0;
		DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
	}
	return di;
}

//^2759:289C
void SkWinCore::REMOVE_RUNE_FROM_TAIL()
{
	ENTER(4);
	Champion* xChampion = &glbChampionTable[cd.pi.glbChampionIndex];	//*bp04
	xChampion->RuneCnt(xChampion->RuneCnt() -1);
	xChampion->GetRunes()[xChampion->RuneCnt()] = 0;
	glbSomeChampionPanelFlag = 1;
	UPDATE_RIGHT_PANEL(0);
	_1031_0667();
	return;
}

//^2759:27DF
void SkWinCore::ADD_RUNE_TO_TAIL(U16 symbol_0to5)
{
	ENTER(6);
	Champion *champion = &glbChampionTable[cd.pi.glbChampionIndex];	//*bp04
	U16 iRuneCount = champion->RuneCnt();
	U16 iManaCost = RuneManaPower[iRuneCount * 6 +symbol_0to5];
	if (SkCodeParam::bDebugInfiniteSpells)
		iManaCost = 0;
	if (iRuneCount != 0) {	// if not on POWER rune (any other : INFLUENCE, FORM, ..)
		iManaCost = (RunePowerMultiplicator[i8(champion->GetRunes()[0]) -RUNE_FIRST] * iManaCost) >> 3;
	}
	U16 iUpdateStats = 0;
	if (champion->curMP() >= iManaCost) {
		champion->curMP(champion->curMP() - iManaCost);
		champion->heroFlag |= CHAMPION_FLAG_0800;	// 0x0800
		champion->GetRunes()[iRuneCount] = iRuneCount * 6 +symbol_0to5 +RUNE_FIRST; iRuneCount++;
		champion->RuneCnt(U8(iRuneCount));
		champion->GetRunes()[iRuneCount] = 0;
		iUpdateStats = 1;
	}
	glbSomeChampionPanelFlag = 1;
	UPDATE_RIGHT_PANEL(0);
	_1031_0667();
	if (iUpdateStats != 0) {
		REFRESH_PLAYER_STAT_DISP(cd.pi.glbChampionIndex -1);
	}
	return;
}

//^2C1D:11BF
//SPX: _2c1d_11bf renamed COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH
U16 SkWinCore::COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(U16 xx, U16 yy, i16 zz)
{
	//^2C1D:11BF
	ENTER(8);
	//^2C1D:11C5
	Champion *bp04 = &glbChampionSquad[xx];
	//^2C1D:11D6
	i16 si = (RAND() & 15) + GET_PLAYER_ABILITY(bp04, abStr, 0);
	//^2C1D:11F5
	ObjectID bp08 = bp04->Possess(yy);
	//^2C1D:1207
	U16 bp06 = QUERY_ITEM_WEIGHT(bp08);
	//^2C1D:1211
	U16 di = MAX_LOAD(bp04) >> 4;
	//^2C1D:1222
	si = (bp06 -12) +si;
	//^2C1D:122E
	if (bp06 > di) {
		//^2C1D:1233
		si = si - ((bp06 -di) >> 1);
		//^2C1D:123E
		di += (di -12) >> 1;
		//^2C1D:1249
		if (di < bp06) {
			//^2C1D:124E
			si = si -((bp06 -di) << 1);
		}
	}
	//^2C1D:1259
	if (zz >= 0) {
		//^2C1D:125F
		si = (QUERY_PLAYER_SKILL_LV(xx, zz, 1) << 1) +si;
		//^2C1D:1276
		di = 0;
		//^2C1D:1278
		switch (zz) {
			case  0:	// fighter 0	(global)
			case  4:	// fighter 1	swing
			case  5:	// fighter 2	thrust
			case  6:	// fighter 3	club
			case  7:	// fighter 4	parry
			case  9:	// ninja 2		fight
				//^2C1D:1287
				di = QUERY_GDAT_DBSPEC_WORD_VALUE(bp08, GDAT_ITEM_WEAPON_MELEE_STRENGTH);
				//^2C1D:1295
				break;

			case  1:	// ninja 0		(global)
			case 10:	// ninja 3		throw
			case 11:	// ninja 4		shoot
				//^2C1D:1297
				di = QUERY_GDAT_DBSPEC_WORD_VALUE(bp08, GDAT_ITEM_WEAPON_THROW_STRENGTH);
				//^2C1D:12A5
				if (di == 0)
					//^2C1D:12A7
					break;
				//^2C1D:12A9
				// SPX: x8000 = is a weapon that shoots items
				if ((QUERY_GDAT_DBSPEC_WORD_VALUE(bp08, GDAT_ITEM_WEAPON_PROJECTILE_FLAG) & 0x8000) != 0) {
					//^2C1D:12BA
					if (zz != 11)
						//^2C1D:12C0
						di = 0;
					//^2C1D:12C2
					break;
				}
				//^2C1D:12C4
				if (zz == 11)
					//^2C1D:12CA
					di = 0;

				break;

			case  2:	// priest 0		(global)
			case  3:	// wizard 0		(global)
			case  8:	// ninja 1		steal

				break;
		}
		//^2C1D:12CC
		si = si + di;
	}
	//^2C1D:12D2
	si = STAMINA_ADJUSTED_ATTR(bp04, si);
	//^2C1D:12E2
	if ((bp04->bodyFlag & ((yy == 0) ? 1 : 2)) != 0)
		//^2C1D:12FD
		si >>= 1;
	//^2C1D:12FF
	return BETWEEN_VALUE(0, si >> 1, 100);
}



//^2FCF:183C
void SkWinCore::TRANSFER_PLAYER(i16 xx, i16 yy, U16 zz, U16 dir)
{
	SkD((DLV_DBM, "DBM: TRANSFER_PLAYER(%2d,%2d,%2d,%d)\n"
		, (Bitu)xx, (Bitu)yy, (Bitu)zz, (Bitu)dir));

	//^2FCF:183C
	ENTER(0);
	//^2FCF:1841
	i16 di = yy;
	//^2FCF:1844
	U16 si = (zz != cd.pi.glbPlayerMap) ? 1 : 0;
	//^2FCF:1856
	if (si != 0)
		//^2FCF:185A
		CHANGE_CURRENT_MAP_TO(zz);
	//^2FCF:1863
	if (xx >= 0 && xx < glbCurrentMapWidth && di >= 0 && di < glbCurrentMapHeight) {
		//^2FCF:187C
		if (si != 0) {
			//^2FCF:1880
			CHANGE_CURRENT_MAP_TO(cd.pi.glbPlayerMap);
			//^2FCF:188A
			MOVE_RECORD_TO(OBJECT_NULL, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, -1, 0);
			//^2FCF:189F
			LOAD_NEWMAP(U8(zz));
			//^2FCF:18A8
			//^2FCF:18BE
			MOVE_RECORD_TO(OBJECT_NULL, -1, 0, xx, di);
		}
		else {
			//^2FCF:18B2
			MOVE_RECORD_TO(OBJECT_NULL, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, xx, di);
		}
		//^2FCF:18C7
		ROTATE_SQUAD(dir);
		//^2FCF:18CF
		return;
	}
	//^2FCF:18D1
	if (si != 0)
		//^2FCF:18D5
		CHANGE_CURRENT_MAP_TO(cd.pi.glbPlayerMap);
	//^2FCF:18DF
	return;
}



//^2FCF:18E3
U16 SkWinCore::ENGAGE_X_TELEPORTER()
{
	// activates the X mark floor teleporter at squad's position.

	// return 0 if failed.
	// return 1 if succeeded.

	//^2FCF:18E3
	ENTER(10);
	//^2FCF:18E9
	ObjectID si = GET_TILE_RECORD_LINK(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY);
	//^2FCF:18F8
	U16 bp06;
	for (; si != OBJECT_END_MARKER && (bp06 = si.DBType()) <= dbActuator; si = GET_NEXT_RECORD_LINK(si)) {
		//^2FCF:18FB
		if (bp06 == dbText) {
			//^2FCF:1904
			Text *bp04 = GET_ADDRESS_OF_RECORD2(si);
			//^2FCF:1911
			if (bp04->TextMode() == 1) {
				//^2FCF:1925
				U16 di;
				switch (bp04->SimpleTextExtUsage()) {
					case 16: // 16 -> Teleporter with flag
						//^2FCF:1942
						di = SDFSM_CMD_X_ANCHOR;	// 5
						//^2FCF:1945
						goto _194a;

					case 15: // 15 -> Teleporter
						//^2FCF:1947
						di = SDFSM_CMD_X_TELEPORTER;	// 4
						//^2FCF:194A
_194a:
						i16 bp08;
						i16 bp0a;
						// SPX: FIX added a variable for destination map
						i16 destMap;
						// SPX: There this function should change the destination map (to bp06)
//							if (SEARCH_DUNGEON_FOR_SPECIAL_MARKER(di, 0, bp04->TextIndex() & 0xff, &bp08, &bp0a) >= 0)
						destMap = SEARCH_DUNGEON_FOR_SPECIAL_MARKER(di, 0, bp04->TextIndex() & 0xff, &bp08, &bp0a);
						if (destMap >= 0)
						{
							//^2FCF:1977
							// SPX: There is BUG here, bp06 is the db type instead of required map
//								TRANSFER_PLAYER(bp08, bp0a, bp06, glbPlayerDir);
							TRANSFER_PLAYER(bp08, bp0a, destMap, cd.pi.glbPlayerDir);
							//^2FCF:198B
							// SPX: This is used by the special teleporter ground cross		
							QUEUE_NOISE_GEN2(GDAT_CATEGORY_FLOOR_GFX, bp04->TextIndex() & 0xff, SOUND_STD_ACTIVATION, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1, 0x8c, 0x80);
							//^2FCF:19BB
							if (di == SDFSM_CMD_X_TELEPORTER) {	// di == 4
								//^2FCF:19C0
								bp04->TextVisibility(1);
							}
							//^2FCF:19C8
							return 1;
						}

						break;
				}
			}
		}
		//^2FCF:19CD
	}
	//^2FCF:19EE
	return 0;
}

//^2759:16AC
U16 SkWinCore::ENGAGE_COMMAND(U16 player, i16 cmdSlot)
{
	// CSBwinSimilarity: TAG01bf9a,_Attack

	// cmdSlot
	// is 0 if 1st command is selected on right command panel.
	// is 1 if 2nd command is selected on right command panel.
	// is 2 if 3rd command is selected on right command panel.

	//^2759:16AC
	ENTER(84);
	//^2759:16B2
	U16 di = player;
	//^2759:16B5
	U16 bp4a = 0;
	//^2759:16BA
	U16 bp48 = cmdSlot & 0x8000;
	//^2759:16C3
	cmdSlot &= 0x7fff;
	//^2759:16C8
	glbItemGDATCategory = glbItemSelected[cmdSlot].category;
	//^2759:16D5
	glbItemGDATIndex = glbItemSelected[cmdSlot].index;
	//^2759:16E2
	glbItemGDATEntry = glbItemSelected[cmdSlot].entry;
	//^2759:16EF
	U16 bp34 = glbSelectedHandAction;
	//^2759:16F5
	U16 bp3a = bp34 ^ 1;
	//^2759:16FB
	Champion *champion = &glbChampionSquad[di];	//*bp04
	//^2759:170B
	if (champion->curHP() == 0)
		//^2759:1715
		return 0;
	//^2759:171A
	U16 bp1c = QUERY_CUR_CMDSTR_ENTRY(CnCM_Command);		// Command
	//^2759:1724
	U8 cmdSound = U8(QUERY_CUR_CMDSTR_ENTRY(CnSD_Sound));		// bp36 Sound
	//^2759:172E
	U16 bp38 = 0;
	//^2759:1733
	champion->handCommand[bp34] = U8(bp1c);
	//^2759:1740
	Container *bp08 = NULL;
	//^2759:174A
	ObjectID bp2e = champion->Possess(bp34);
	//^2759:175C
	if (bp2e != OBJECT_NULL) {
		//^2759:1761
		bp08 = GET_ADDRESS_OF_RECORD9(bp2e);
	}
	//^2759:1770
	U16 bp22 = champion->playerDir();
	//^2759:177C
	U16 bp1e = cd.pi.glbPlayerPosX + glbXAxisDelta[bp22];
	//^2759:178A
	U16 bp20 = cd.pi.glbPlayerPosY + glbYAxisDelta[bp22];
	//^2759:1799
	glbObjectID_4976_534c = GET_CREATURE_AT(bp1e, bp20);
	//^2759:17A9
	U8 bp35;
	AIDefinition *bp1a;
	if (glbObjectID_4976_534c != OBJECT_NULL) {
		//^2759:17AE
		bp1a = QUERY_CREATURE_AI_SPEC_FROM_TYPE(bp35 = QUERY_CLS2_FROM_RECORD(glbObjectID_4976_534c));
	}
	U16 bp26 = QUERY_CUR_CMDSTR_ENTRY(CnBZ_Busy);	// bp26 Busy
	U16 bp2a = QUERY_CUR_CMDSTR_ENTRY(CnSK_Skill);	// bp2a Skill used
	U16 bp28 = QUERY_CUR_CMDSTR_ENTRY(CnTR_Tire) + RAND01();	// bp28 Fatigue
	i16 bp2c = QUERY_CUR_CMDSTR_ENTRY(CnEX_Experience);	// bp2c Experience
	U16 si = QUERY_CUR_CMDSTR_ENTRY(CnST_Strength);		// si Spell Strength
	i16 iAttackType = QUERY_CUR_CMDSTR_ENTRY(CnAT_AttackType);	// pb46 Attack type
	i16 iPouchOrScabbard = 0;// bp46 (SPX: added for clarity)
	champion->handDefenseClass[bp34] = U8(QUERY_CUR_CMDSTR_ENTRY(CnTA_TemporaryArmor));	// Armor modified
	U16 bp24 = GET_TILE_VALUE(bp1e, bp20);
	//^2759:182F
	U16 bp0e = 1;
	//^2759:1834
	ObjectID bp44;
	U16 bp42;
	i16 bp0c;
	U16 bp40;
	ObjectID bp32;
	U16 bp0a;
	ObjectID bp3e;
	U16 bp50;
	ObjectID bp30;
	U16 bp3c;
	switch (bp1c) {
		case CmConfuse: // 5
			//^2759:1848
			si = si + QUERY_PLAYER_SKILL_LV(di, SKILL_PRIEST_INFLUENCE, 1);
			//^2759:185B
			bp0e = CONFUSE_CREATURE(si, bp1e, bp20);
			//^2759:186C
			if (bp0e == 0)
				//^2759:1870
				bp2c >>= 2;
			//^2759:1874
			bp38 = 0;
			//^2759:1879
			break;

		case CmInvisibility: // 2
			//^2759:187C
			si = max_value(32, si);
			//^2759:1888
			bp50 = 0x47;
			//^2759:188C
			if (glbGlobalSpellEffects.Invisibility++ == 0 && glbChampionInventory != 0) {
				//^2759:18A0
				glbChampionTable[glbChampionInventory].heroFlag |= CHAMPION_FLAG_4000;	// 0x4000
			}
			//^2759:18B0
			Timer bp54;
			bp54.SetMap(cd.pi.glbPlayerMap);
			bp54.SetTick(glbGameTick +si);
			//^2759:18D8
			QUEUE_TIMER(&bp54);
			//^2759:18E4
			break;

		case CmLaunchMissile: // 3 magical missile
			//^2759:18E7
			bp44 = ObjectID::MissileRecord(QUERY_CUR_CMDSTR_ENTRY(CnPA_Parameter));
			//^2759:18F4
			//bp42 = 7 - min_value(6, QUERY_PLAYER_SKILL_LV(di, bp2a, 1));
			bp42 = 7 - min_value(SkLvlCraftsman, QUERY_PLAYER_SKILL_LV(di, bp2a, 1));
			//^2759:1914
			if (champion->curMP() < bp42) {
				//^2759:1920
				si = max_value(2, (champion->curMP() * si) / bp42);
				//^2759:1936
				bp42 = champion->curMP();
			}
			//^2759:1940
			bp0e = CAST_CHAMPION_MISSILE_SPELL(di, bp44, si, bp42);
			//^2759:1953
			if (bp0e == 0)
				//^2759:195A
				bp2c >>= 1;
			//^2759:195D
			break;

		case CmPhysicalDamage4: // 4
		case CmPhysicalDamage8: // 8
			//^2759:1960
			if ((bp24 >> 5) == ttDoor && (bp24 & 7) == 4) {
				//^2759:1973
				ATTACK_DOOR(
					bp1e,
					bp20,
					COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(di, bp34, bp2a), 
					0, 
					2
					);

				//^2759:1995
				bp26 = 6;
				//^2759:199A
				Door *bp12 = GET_ADDRESS_OF_TILE_RECORD(U8(bp1e), U8(bp20))->castToDoor();
				//^2759:19AD
				// SPX: Sound made by the door when hit?
				QUEUE_NOISE_GEN2(
					GDAT_CATEGORY_DOORS,
					GET_GRAPHICS_FOR_DOOR(bp12),
					SOUND_OBJECT_GETHIT,
					0xfe,
					bp1e,
					bp20,
					bp38 +1,
					0x3c,
					0xc8
					);
				//^2759:19DB
				break;
			}
			//^2759:19DE
			if (true
				&& glbObjectID_4976_534c != OBJECT_NULL 
				&& bp1a->IsStaticObject() != 0 
				&& bp1a->ArmorClass == AI_DEF_ARMOR_MAX 
				&& bp1a->Defense == 255
			) {
				//^2759:19FD
				bp2c >>= 1;
			}
			else {
				//^2759:1A02
				bp44 = QUERY_CUR_CMDSTR_ENTRY(CnPA_Parameter);		// Spell missile
				//^2759:1A0C
				bp0e = WIELD_WEAPON(di, bp44, bp1e, bp20, bp2a, iAttackType);
				//^2759:1A26
				if (bp0e == 0) {
					//^2759:1A2A
					bp2c >>= 1;
					//^2759:1A2D
					bp26 >>= 1;
					//^2759:1A30
					break;
				}
				//^2759:1A33
				if (glbObjectID_4976_534c == OBJECT_NULL)
					//^2759:1A3A
					break;
			}
			//^2759:1A3D
			QUEUE_NOISE_GEN1(GDAT_CATEGORY_CREATURES, bp35, SOUND_OBJECT_GETHIT, 0x3c, 0xc8, bp1e, bp20, bp38 +1);
			//^2759:1A5E
			break;

		case CmSpellReflection: // 7
			//^2759:1A61
			CREATE_CLOUD(OBJECT_EFFECT_REFLECTOR, max_value(2, si), cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 255);
			//^2759:1A81
			break;

		case CmAuraOfSpeed: // 9
			//^2759:1A84
			si = max_value(32, si);
			//^2759:1A90
			glbGlobalSpellEffects.AuraOfSpeed = U8(min_value(glbGlobalSpellEffects.AuraOfSpeed +si, 255));
			//^2759:1AA5
			break;

// Check CSBWin TAG01bf9a/_Attack
		case CmInvoke:	// DM1 retrocompatibility
			{
			ObjectID oMissileSpell = OBJECT_EFFECT_FIREBALL;
			i16 iEnergyRangePower = RAND()%128 + 100;
			  switch (RAND()%7)
			  {
				  case 0: oMissileSpell = OBJECT_EFFECT_POISON_BOLT; break;
				  case 1: oMissileSpell = OBJECT_EFFECT_POISON_CLOUD; break;
				  case 2: oMissileSpell = OBJECT_EFFECT_DISPELL; break;
				  case 3: oMissileSpell = OBJECT_EFFECT_POISON_BLOB; break;
				  case 4: oMissileSpell = OBJECT_EFFECT_LIGHTNING; break;
				  default:oMissileSpell = OBJECT_EFFECT_FIREBALL; break;
			  };
			bp42 = 7 - min_value(SkLvlCraftsman, QUERY_PLAYER_SKILL_LV(di, bp2a, 1));
			if (champion->curMP() < bp42) {
				si = max_value(2, (champion->curMP() * si) / bp42);
				bp42 = champion->curMP();
			}
			bp0e = CAST_CHAMPION_MISSILE_SPELL(di, oMissileSpell, iEnergyRangePower, bp42); // championid missile energy/range mana
			if (bp0e == 0)
				bp2c >>= 1;
			  break;
			}
		case CmLaunchProjectile: // 32
			//^2759:1AA8
			if (IS_MISSILE_VALID_TO_LAUNCHER(di, bp34, champion->Possess(bp3a)) == 0) {
				//^2759:1AC6
				if (bp48 == 0) {
					//^2759:1ACC
					glbSomeChampionPanelFlag = max_value(1, 8 - UPDATE_GLOB_VAR(67, 1, 3));
					//^2759:1AEC
					glbChampionAttackDamage = ATTACK_REQUIRES_HAND_ITEM;	// -2
					//^2759:1AF2
					glbWeaponShooterNum = QUERY_GDAT_DBSPEC_WORD_VALUE(bp2e, GDAT_ITEM_WEAPON_PROJECTILE_FLAG);	// 5
					//^2759:1B01
					glbWeaponMissileHand = bp3a;
					//^2759:1B07
					bp2c = 0;
				}
				//^2759:1B0C
				bp0e = 0;
				//^2759:1B11
				break;
			}
			//^2759:1B14
			bp30 = REMOVE_POSSESSION(di, bp3a);
			//^2759:1B22
			bp0a = QUERY_PLAYER_SKILL_LV(di, SKILL_NINJA_SHOOT, 1);
			//^2759:1B32
			// SPX: bp2e is the SHOOTER/LAUNCHER, bp30 is the PROJECTILE
			// param3 = energy (max 255) / param4 = energy2 / param5 = dd
			SHOOT_CHAMPION_MISSILE(
				champion,
				bp30,
				QUERY_GDAT_DBSPEC_WORD_VALUE(bp2e, GDAT_ITEM_WEAPON_ACCURACY) + QUERY_GDAT_DBSPEC_WORD_VALUE(bp30, GDAT_ITEM_WEAPON_THROW_STRENGTH) + bp0a,
				(bp0a << 1) + QUERY_GDAT_DBSPEC_WORD_VALUE(bp30, GDAT_ITEM_STATS_0x0C),	// 12
				QUERY_GDAT_DBSPEC_WORD_VALUE(bp30, GDAT_ITEM_STATS_0x0C)
				);
			//^2759:1B84
			bp4a = 1;
			//^2759:1B89
			break;

		case CmSpellshield: // 33
		case CmFireshield: // 34
		case CmShield: // 35
			//^2759:1B8C
			U16 enchantmentType;
//				bp0c = (bp1c == CmSpellshield) ? 1 : ((bp1c == CmFireshield) ? 0 : 2);
			enchantmentType = (bp1c == CmSpellshield) ? ENCHANTMENT_SPELL_SHIELD : ((bp1c == CmFireshield) ? ENCHANTMENT_FIRE_SHIELD : ENCHANTMENT_PARTY_SHIELD);
			//^2759:1BAB
			si = max_value(32, si) * 3;
			//^2759:1BBC
			if (CALL_ENCHANTMENT_SELF(champion, enchantmentType, si, 1) != 0)
				//^2759:1BD5
				break;
			//^2759:1BD8
			bp2c >>= 2;
			//^2759:1BDC
			bp26 >>= 1;
			//^2759:1BDF
			break;

		case CmConsume: // 16
			//^2759:1BE2
			PLAYER_CONSUME_OBJECT(di, bp2e, bp34);
			//^2759:1BF1
			break;

		case CmPouch: // 17
			//^2759:1BF4
			bp4a = 1;
			//^2759:1BF9
			iPouchOrScabbard = FIND_POUCH_OR_SCABBARD_POSSESSION_POS(di, bp34);
			if (iPouchOrScabbard < 0)
				break;
			bp2e = REMOVE_POSSESSION(di, iPouchOrScabbard);
			//^2759:1C1D
			EQUIP_ITEM_TO_INVENTORY(di, bp2e, bp34);
			//^2759:1C2C
			break;

		case CmHealing: // 36
			//^2759:1C2F
			bp0c = champion->maxHP() - champion->curHP();	// how much HP missing
			//^2759:1C3D
			if (bp0c <= 0)
				break;
			//^2759:1C44
			if (champion->curMP() == 0)
				//^2759:1C4B
				break;
			//^2759:1C4E
			bp0a = min_value(SkLvlMasterLo, QUERY_PLAYER_SKILL_LV(di, SKILL_PRIEST_HEAL, 1));	// skill, caped 10
			//^2759:1C68
			bp2c = 2;

			do {
				//^2759:1C6D
				bp0e = min_value(bp0c, bp0a);	// Minimum step between initial HP missing and skill level (max = 10)
				//^2759:1C7D
				champion->curHP(champion->curHP() +bp0e);
				//^2759:1C84
				bp2c += 2;
				//^2759:1C88
				champion->curMP(champion->curMP() -2);
				//^2759:1C8D
			} while (champion->curMP() > 0 && (bp0c -= champion->curMP()) != 0);

			//^2759:1C94
			if (champion->curMP() < 0)
				//^2759:1C9E
				champion->curMP(0);
			//^2759:1CA4
			champion->heroFlag |= CHAMPION_FLAG_0800;	// 0x0800 Refresh bar stat?
			//^2759:1CAD
			bp0e = 1;
			//^2759:1CB2
			break;

		case CmUseRope: // 10
			//^2759:1CB5
			bp1e = cd.pi.glbPlayerPosX + glbXAxisDelta[cd.pi.glbPlayerDir];
			//^2759:1CC5
			bp20 = cd.pi.glbPlayerPosY + glbYAxisDelta[cd.pi.glbPlayerDir];
			//^2759:1CD5
			bp40 = 1;
			//^2759:1CDA
			bp32 = GET_CREATURE_AT(bp1e, bp20);
			//^2759:1CEA
			if (bp32 != OBJECT_NULL && (QUERY_CREATURE_AI_SPEC_FLAGS(bp32) & 0x8000) == 0) {
				//^2759:1CFD
				bp40 = 0;
			}
			//^2759:1D02
			if ((GET_TILE_VALUE(bp1e, bp20) >> 5) == 2 && bp40 != 0) {
				//^2759:1D1E
				_4976_5824 = 1;
				//^2759:1D24
				MOVE_RECORD_TO(OBJECT_NULL, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, bp1e, bp20);
				//^2759:1D3C
				_4976_5824 = 0;
				//^2759:1D42
				break;
			}
			//^2759:1D45
			bp26 = 0;
			//^2759:1D4A
			break;

		case CmFreezeLife: // 11
			//^2759:1D4D
			glbGlobalSpellEffects.FreezeCounter = U8(min_value(EFFECT_FREEZE_MAX, glbGlobalSpellEffects.FreezeCounter +si));
			//^2759:1D62
			break;

		case CmAuraOfDex: // 12
			//^2759:1D65
			bp0c = 5;
			//^2759:1D6A
_1d6a:
			si = max_value(32, si) << 2;
			//^2759:1D79
			if (CALL_ENCHANTMENT_SELF(champion, bp0c, si, 0) != 0)
				//^2759:1D92
				break;
			//^2759:1D95
			bp2c >>= 2;
			bp26 >>= 1;
			//^2759:1D9C
			break;

		case CmAuraOfWiz: // 13
			//^2759:1D9F
			bp0c = 4;
			//^2759:1DA4
			goto _1d6a;

		case CmAuraOfVit: // 14
			//^2759:1DA6
			bp0c = 6;
			//^2759:1DAB
			goto _1d6a;

		case CmAuraOfStr: // 15
			//^2759:1DAD
			bp0c = 3;
			//^2759:1DB2
			goto _1d6a;

		case CmDarkness:	// 6
		case CmLight:		// 38
		case CmLongLight:	// 39
			//^2759:1DB4
			PROCEED_LIGHT(bp1c, si);
			//^2759:1DBE
			break;

		case CmThrow: // 42
			//^2759:1DC1
			bp0e = _2c1d_1de2_CHAMPION_SHOOT(
				di,
				bp34,
				(champion->playerPos() != ((cd.pi.glbPlayerDir +1) & 3))
					? (champion->playerPos() == ((cd.pi.glbPlayerDir +2) & 3))
						? 1
						: 0
					: 1
				);
			//^2759:1E02
			break;

		case CmMark: // 44
			//^2759:1E05
			bp0e = SET_DESTINATION_OF_MINION_MAP(bp2e, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, cd.pi.glbPlayerMap);
			//^2759:1E1F
			if (bp0e != 0)
				//^2759:1E23
				break;
			//^2759:1E26
			bp2c = 0;
			bp26 = 1;
			//^2759:1E30
			break;

		case CmCallScout: // 47
			//^2759:1E33
			bp35 = CREATURE_GOOD_SCOUT_MINION;		// SPX: 0x30 = Scout Minion
			//SPX: bp35 holds creature to generate
			//^2759:1E37
			goto _1ebe;

		case CmCallCarry: // 45
		case CmCallFetch: // 46
			//^2759:1E3A
			if (GET_MISSILE_REF_OF_MINION(bp08->GetContainedObject(), bp2e) != NULL) {
				GET_ADDRESS_OF_RECORD4(bp08->GetContainedObject())->CreatureType((bp1c == CmCallCarry) ? 0x32 : 0x33);
				_1c9a_0247_FREE_CACHE_FROM_CREATURE(bp08->GetContainedObject());
			}
			//^2759:1E7F
			else if (bp1c == CmCallFetch) {
				//^2759:1E85
				bp35 = CREATURE_GOOD_FETCH_MINION;	// SPX: Fetch Minion
				//^2759:1E89
				bp0a = cd.pi.glbPlayerDir;
				//^2759:1E8F
				bp1e = bp08->GetDestX();
				//^2759:1E9C
				bp20 = bp08->GetDestY();
				//^2759:1EA9
				bp22 = 0xffff;
				//^2759:1EAE
				bp0c = bp08->GetDestMap();
				//^2759:1EB8
				goto _1ed8;
			}
			else {
				//^2759:1EBA
				bp35 = CREATURE_GOOD_CARRY_MINION;	// SPX: Carry Minion
				//^2759:1EBE
_1ebe:
				bp1e = cd.pi.glbPlayerPosX;
				//^2759:1EC4
				bp20 = cd.pi.glbPlayerPosY;
				//^2759:1ECA
				bp0a = (cd.pi.glbPlayerDir +2) & 3;
				//^2759:1ED5
				bp0c = cd.pi.glbPlayerMap;
				//^2759:1EDB
_1ed8:
				bp3e = CREATE_MINION(
					bp35,
					QUERY_PLAYER_SKILL_LV(di, bp2a, 1),
					bp0a,
					bp1e,
					bp20,
					bp0c,
					bp2e,
					bp22
					);
				//^2759:1F0E
				bp0e = (bp3e != OBJECT_NULL) ? 1 : 0;
				//^2759:1F1E
				if (bp0e != 0) {
					//^2759:1F22
					if (bp1c == CmCallScout) {
						//^2759:1F28
						SET_DESTINATION_OF_MINION_MAP(bp2e, _4976_5826, _4976_5828, _4976_581c);
					}
					//^2759:1F3F
					bp08->SetContainedObject(bp3e);
					//^2759:1F49
					break;
				}
				//^2759:1F4C
				bp2c >>= 2;
				bp26 >>= 1;
				//^2759:1F53
				CREATE_CLOUD(OBJECT_EFFECT_CLOUD, 0x6e, bp1e, bp20, 255);
				//^2759:1F68
				break;

				//^2759:1F6B
//_1f6b:
_ReleaseMinion:
				RELEASE_MINION(bp08->GetContainedObject());
			}
			//^2759:1F77
			//^2759:1F78
			break;

		case CmKillMinion: // 48
			goto _ReleaseMinion;

		case CmGuardMinion: // 50
			//^2759:1F7B
			bp3c = CREATURE_GOOD_GUARD_MINION;	// Guard Minion
			//^2759:1F80
			goto _CreateMinion;

		case CmUHaulMinion: // 51
			//^2759:1F82
			bp3c = CREATURE_GOOD_U_HAUL_MINION;	// U-Haul Minion
			//^2759:1F87
			goto _CreateMinion;

		case CmAttackMinion: // 49
			//^2759:1F89
			bp3c = CREATURE_GOOD_ATTACK_MINION;	// Attack Minion
			//^2759:1F8E
//_1f8e:
_CreateMinion:
			if (CREATE_MINION(bp3c, si >> 3, (cd.pi.glbPlayerDir +2) & 3, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, cd.pi.glbPlayerMap, bp2e, cd.pi.glbPlayerDir) != OBJECT_NULL)
				//^2759:1FBE
				break;
			//^2759:1FC0
			CREATE_CLOUD(OBJECT_EFFECT_CLOUD, 0x6E, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 255);
			//^2759:1FD7
			break;

		case CmTeleport: // 54
			//^2759:1FD9
			if (ENGAGE_X_TELEPORTER() != 0)
				//^2759:1FE0
				break;
			//^2759:1FE2
			glbSomeChampionPanelFlag = max_value(1, 8 - UPDATE_GLOB_VAR(0x47, 1, 3));
			glbChampionAttackDamage = ATTACK_FAILURE_X_TELEPORT;

			break;

// SPX: All this set of unused commands. It makes some possibilities here.
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		//case 27:
		case 28:
		case 29:
		case 30:
		case 31:
		case 37:
		case 40:
		case 41:
		case 43:
		case 52:
		case 53:

			break;
	}
	//^2759:2008
	if (bp4a == 0) {
		//^2759:200E
		QUEUE_NOISE_GEN2(glbItemGDATCategory, glbItemGDATIndex, cmdSound, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, bp38, 0x73, 0xC8);
	}
	//^2759:2035
	if (champion->curHP() == 0 || bp48 != 0)
		//^2759:2048
		return bp0e;
	//^2759:204B
	if (bp26 != 0) {
		//^2759:2051
		ADJUST_HAND_COOLDOWN(di, bp26, bp34);
	}
	//^2759:205F
	if (bp28 != 0) {
		//^2759:2065
		ADJUST_STAMINA(di, bp28);
	}
	//^2759:2070
	if (bp2c != 0) {
		//^2759:2076
		ADJUST_SKILLS(di, bp2a, bp2c);
	}
	//^2759:2085
	if (bp0e != 0) {
		//^2759:208B
		sk536e *bp16 = &glbChampionEngagingHandCommand[di][bp34];
		//^2759:20A8
		bp16->w0 = QUERY_CUR_CMDSTR_ENTRY(CnRP_Repeat);
		//^2759:20B5
		if (bp16->w0 != 0) {
			//^2759:20B9
			bp16->b2 = 1;
			//^2759:20BE
			bp16->id4 = bp2e;
			bp16->b3 = i8(cmdSlot);
		}
	}
	//^2759:20CC
	return bp0e;
}



//^2759:1204
// SPX: _2759_1204 renamed PROCEED_COMMAND_SLOT
U16 SkWinCore::PROCEED_COMMAND_SLOT(i16 cmdSlot)
{
	ENTER(6);
	SkD((DLV_TWEET, "Tweet: Command slot %d for object %s. \n"
		, cmdSlot, static_cast<LPCSTR>(getRecordNameOf(glbChampionItemInUse))
		));
	
	U16 iEngageResult = 0;	// bp06
	if (cd.pi.glbChampionIndex != 0) {
		U16 iChampionIndex = cd.pi.glbChampionIndex - 1;	// di
		Champion *champion = &glbChampionSquad[iChampionIndex];	//*bp04
		if (cmdSlot == -1) {
			glbMagicalMapFlags = 0;
		}
		else {
			champion->heroFlag |= CHAMPION_FLAG_0800;	// 0x0800
			iEngageResult = ENGAGE_COMMAND(iChampionIndex, cmdSlot);
			i16 iNbChargesToConsume = QUERY_CMDSTR_ENTRY(glbItemGDATCategory, glbItemGDATIndex, glbItemGDATEntry, CnNC_NumberCharge);	// si
			if (iNbChargesToConsume == C16_CHARGES_CONSUME_ALL_AT_ONCE) {	// 16
				iNbChargesToConsume = 15;	// an item has 15 charges at max, then consume all
			}
			if (iNbChargesToConsume != C17_CHARGES_REQUIRED_NO_DECREASE && iNbChargesToConsume != C18_CHARGES_AVAILABLE_WHEN_NONE) {	// !=17 && != 18
				ADD_ITEM_CHARGE(glbChampionItemInUse, -iNbChargesToConsume);
			}
			if (true
				&& ADD_ITEM_CHARGE(glbChampionItemInUse, 0) == 0
				&& (QUERY_GDAT_DBSPEC_WORD_VALUE(glbChampionItemInUse, 0x00) & 0x0800) != 0
			) {
				REMOVE_POSSESSION(iChampionIndex, glbSelectedHandAction);
				DEALLOC_RECORD(glbChampionItemInUse);
			}
		}
		if (glbMagicalMapFlags != 0) {
			glbMagicalMapFlags &= 0xFBFF;
		}
		else {
			DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
		}
		UPDATE_RIGHT_PANEL(0);
	}
	return iEngageResult;
}

//^2759:0589
// SPX: _2759_0589 renamed ACTIVATE_ACTION_HAND
void SkWinCore::ACTIVATE_ACTION_HAND(U16 iChampionNo, U16 iHandNo)
{
	ENTER(4);
	U16 iChampionIndex = iChampionNo;	// si / champion no
	U16 iHandIndex = iHandNo;	// di / hand
	Champion* xChampion = &glbChampionSquad[iChampionIndex];	//*bp04
	if (xChampion->curHP() == 0)
		return;
	if (IS_ITEM_HAND_ACTIVABLE(iChampionIndex, xChampion->Possess(iHandIndex), iHandIndex) == 0)
		return;
	glbSelectedHandAction = glbSelectedHand_2 = iHandIndex;
	cd.pi.glbChampionIndex = glbSomeChampionIndex = iChampionIndex + 1;
	glbSomeChampionPanelFlag = 1;
	glbMagicalMapFlags = 0;
	CHAMPION_SQUAD_RECOMPUTE_POSITION();
	UPDATE_RIGHT_PANEL(0);
	REFRESH_PLAYER_STAT_DISP(iChampionIndex);
	return;
}

//^2759:0602
//SPX: _2759_0602 renamed SET_SPELLING_CHAMPION
// Called when a champion is selected to spell runes
void SkWinCore::SET_SPELLING_CHAMPION(U16 iChampionNo)	// U16 xx
{
	ENTER(0);
	if (glbChampionSquad[iChampionNo].curHP() == 0)
		return;
	glbSelectedHandAction = glbSelectedHand_2 = 2;
	cd.pi.glbChampionIndex = glbSomeChampionIndex = iChampionNo + 1;
	glbSomeChampionPanelFlag = 1;
	glbMagicalMapFlags = 0;
	CHAMPION_SQUAD_RECOMPUTE_POSITION();
	UPDATE_RIGHT_PANEL(0);
	return;
}

//^2FCF:19F4
// For CSBWin, check TAG01a436 Mouse.HandleClickInViewport
void SkWinCore::MOVE_RECORD_AT_WALL(U16 xx, U16 yy, U16 dir, ObjectID rlUnk, ObjectID rlWhatYouPlace)
{
	// 1) call when you place an apple to the ornate wall (alcove, keyhole, shop panel)
	// # xx=wall-x, yy=wall-y, dir=wall-side, rlUnk=FFFF, rlWhatYouPlace=your-holding-item

	// 2) call if you pick a torch at wall
	// # xx=wall-x, yy=wall-y, dir=wall-side, rlUnk=FFFF, rlWhatYouPlace=FFFF

	// e.g. you place a torch at wall. _2fcf_19f4( 8, 4, 2,FFFF,1502)
	// e.g. you take a torch at wall.  _2fcf_19f4( 8, 4, 2,FFFF,FFFF)

	// SPX: this seems to behave differently from DM1. DM1 would check the last actuator to determine what is activable from user.
	// specially seen on alcoves under key hole / coin slot to protect items to be taken in any case.

	//^2FCF:19F4
	ENTER(68);
	//^2FCF:19FA
	ObjectID si = rlWhatYouPlace;
	U16 bp26 = 0;
	U16 bp2a = 0;
	U16 bDelayedActuatorsRotation = 0; // SPX: DM1 retrocompatibility
	U16 iWallSideToRotate = -1; // SPX: DM1 retrocompatibility
	ObjectID bp34 = OBJECT_NULL;
	//^2FCF:1A0C
	ObjectID bp0e = GET_TILE_RECORD_LINK(xx, yy);
	//printf("Click on wall : ---------------\n");

	// SPX: Custom script compatibility: call event on wall click
	LUA_CALL_SCRIPT(_EXP_SCRIPT__ANY_EVENT_, cd.pi.glbPlayerMap, xx, yy, dir);

	// SPX: for DM1 compatibility, we first get the "top" actuator on each side, so that we can compare then if it needs to be considered by interaction or not.
	ObjectID xTopActuators[4];	// for all side
	xTopActuators[0] = xTopActuators[1] = xTopActuators[2] = xTopActuators[3] = OBJECT_END_MARKER;
	if (SkCodeParam::bDM1Mode)
	{
		for (U16 i = 0; bp0e != OBJECT_END_MARKER && i == 0; bp0e = GET_NEXT_RECORD_LINK(bp0e))
		{
			U16 bp1c = bp0e.DBType();
			U16 bp10 = bp0e.Dir();
			//if (bp10 != dir || bp1c != dbActuator)
			//	continue;
			if (bp1c != dbActuator)
				continue;
			xTopActuators[bp10] = bp0e;
		}
	}
#ifndef __SKWIN_PUBLIC_VERSION__
	SkD((0, "Top objects on sides N/E/S/W : %04X %04X %04X %04X\n", 
		xTopActuators[0].w, xTopActuators[1].w, xTopActuators[2].w, xTopActuators[3].w));
#endif // __SKWIN_PUBLIC_VERSION__
	bp0e = GET_TILE_RECORD_LINK(xx, yy);

	//^2FCF:1A1C
	for (U16 bp28 = 0; bp0e != OBJECT_END_MARKER && bp28 == 0; bp0e = GET_NEXT_RECORD_LINK(bp0e)) {
		//^2FCF:1A24
		U16 bp10 = bp0e.Dir();
		//^2FCF:1A2D
		if (bp10 != dir)
			//^2FCF:1A32
			continue;
		//^2FCF:1A35
		if (IS_WALL_ORNATE_ALCOVE_FROM_RECORD(bp0e) != 0) {
			//printf("Click on wall %04X (dir=%d): this is a wall alcove!\n", bp0e, bp10);
			// SPX: for DM1 compatibility, we check if this actuator is the top actuator to consider its effect as alcove
			if (SkCodeParam::bDM1Mode && bp0e != xTopActuators[bp10])
			{
				//printf("DM1Mode: wall alcove effect is rejected!\n");
				// BUT we want to check its actuator code, so that we have to jump further
				goto checkactuator;
			}
			//^2FCF:1A45
			if (si == OBJECT_NULL) {
				//^2FCF:1A4A
				ObjectID bp32 = GET_WALL_TILE_ANYITEM_RECORD(xx, yy);
				//^2FCF:1A5A
				ObjectID bp30 = OBJECT_NULL;
				//^2FCF:1A5F
				for (; bp32 != OBJECT_END_MARKER; bp32 = GET_NEXT_RECORD_LINK(bp32)) {
					//^2FCF:1A61
					if (bp32.Dir() == bp10) {
						//^2FCF:1A6C
						bp30 = bp32;
					}
					//^2FCF:1A72
				}
				//^2FCF:1A84
				if (bp30 == OBJECT_NULL)
					//^2FCF:1A8A
					break;
				//^2FCF:1A8D
				U16 bp2e = bp30.DBType();
				//^2FCF:1A99
				if (bp2e < dbWeapon || bp2e > dbMiscellaneous_item)
					//^2FCF:1AA8
					break;
				//^2FCF:1AAB
				MOVE_RECORD_TO(bp30, xx, yy, -1, 0);
				//^2FCF:1ABF
				bp34 = bp30;
				//^2FCF:1AC5
				break;
			}
			//^2FCF:1AC8
			bp2a = 1;
			//^2FCF:1ACD
			MOVE_RECORD_TO(ObjectID(si, bp10), -1, 0, xx, yy);
			
			// SPX: Actually, DM2 code seems not to like multiple wall actuators on the same side and breaks after triggering the first one, unlike DM1 which
			// sometimes needs several wall actuators on same side for mechanism. Then we don't want to break in case of DM1.
			if (SkCodeParam::bDM1Mode)
				goto checkactuator;
				//continue;
			//^2FCF:1AED
			break;
		}
		//^2FCF:1AF0
		if (IS_WALL_ORNATE_SPRING(bp0e) != 0) {
			//printf("Click on wall %04X (dir=%d): this is a wall fountain!\n", bp0e, bp10);
			//^2FCF:1B00
			if (si == OBJECT_NULL) {
				//^2FCF:1B05
				if (glbChampionLeader == -1)
					//^2FCF:1B0C
					break;
				//^2FCF:1B0F
				for (i16 championIndex = 0; championIndex < MAX_CHAMPIONS; championIndex++) {
					//^2FCF:1B16
					if (glbChampionSquad[championIndex].curHP() != 0) {
						//^2FCF:1B27
						glbChampionSquad[championIndex].curWater(WATER_MAX);
					}
					//^2FCF:1B37
				}
				//^2FCF:1B40
				// SPX: Sound when drinking from wall
				QUEUE_NOISE_GEN2(GDAT_CATEGORY_CHAMPIONS, glbChampionSquad[glbChampionLeader].HeroType(), SOUND_CHAMPION_EAT_DRINK, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 0, 0x96, 0x80);
				//^2FCF:1B6F
				break;
			}
			//^2FCF:1B72
			if ((QUERY_GDAT_DBSPEC_WORD_VALUE(si, 0) & 1) != 0) {
				//^2FCF:1B81
				ADD_ITEM_CHARGE(si, 15);
				//^2FCF:1B8B
				bp2a = 1;
				bp34 = si;
				//^2FCF:1B93
				break;
			}
			if (!SkCodeParam::bDM1Mode) {
			//^2FCF:1B96
			if (GET_DISTINCTIVE_ITEMTYPE(si) != 0x0194) // SPX: not sure ? creature fountain ? (or tree ?)
				//^2FCF:1BA2
				break;
			//^2FCF:1BA5
			SET_ITEMTYPE(si, 15);
			//^2FCF:1BAF
			bp2a = 1;
			bp34 = si;
			//^2FCF:1BB7
			break;
			}
		}
//SPX: added for easy jump from DM1 mode
checkactuator:
		//^2FCF:1BBA
		U16 bp1c = bp0e.DBType();
		//^2FCF:1BC6
		U16 bp16;
		if (bp1c == dbActuator) {
			//^2FCF:1BCE
			Actuator *bp04 = GET_ADDRESS_OF_ACTU(bp0e);
			//^2FCF:1BDD
			bp16 = bp04->ActuatorType();
			//^2FCF:1BEA
			U16 bp18 = bp04->ActuatorData();
			//^2FCF:1BF7
			U16 bp1a = bp04->ActionType();
			// SPX: more data to check
			U16 iStatus = bp04->ActiveStatus();
			U16 iToggler = bp04->ActuatorToggler();
			U16 iRevert = bp04->RevertEffect();
			U16 iOnce = bp04->OnceOnlyActuator();
			U16 iDisabled = bp04->IsDisabled();
			//printf("Clicking on wall Actuator %04X (dir=%d) ACT=%02d (1=%d DIS=%d) action=%04x (R=%d) data=%04x status=%d toggler=%d\n", bp0e, bp10, bp16, iOnce, iDisabled, bp1a, iRevert, bp18, iStatus, iToggler);
			//^2FCF:1C04
			//if (glbChampionLeader == -1 && bp16 != 0x7e)
			if (glbChampionLeader == -1 && (bp16 != ACTUATOR_TYPE_RESURECTOR && bp16 != ACTUATOR_TYPE_CHAMPION_MIRROR)) // SPX : Add 0x7F
				//^2FCF:1C11
				continue;
			//^2FCF:1C14
			U8 bp23 = GET_WALL_DECORATION_OF_ACTUATOR(bp04);
			//^2FCF:1C24
			if (bp16 == 0)
				//^2FCF:1C2A
				continue;
			//^2FCF:1C2D
			U16 di = 1;
			//^2FCF:1C30
			U16 bp44 = bp16;
			//^2FCF:1C36
			U16 bp2c;
			Door *bp14;
			U16 bp1e;
			U16 bp36;
			switch (bp44) {
				default:
					//^2FCF:1C48
					continue;


				case ACTUATOR_TYPE_SHOP_PANEL: // 0x3F -> 'Shop panel'
					//^2FCF:1C4F
					if (si == OBJECT_NULL)
						//^2FCF:1C57
						bp04->ActiveStatus(0);
					//^2FCF:1C5F
					break;

				// SPX: removed native DM2 x01 case for activating back DM1 push button x01
					/*
				case ACTUATOR_TYPE_X01: // 0x01 -> ?
					//^2FCF:1C62
					di = 0;
					//^2FCF:1C64
					if (bp04->ActionType() != 3)
						//^2FCF:1C74
						//^2FCF:1C79
						continue;
					//^2FCF:1C76
					break;
					*/

				case ACTUATOR_TYPE_X02: // 0x02 -> ?
				//case ACTUATOR_TYPE_DM1_ALCOVE_ITEM:
					if (SkCodeParam::bDM1Mode == false) { // DM2 behavior
					//^2FCF:1C7C
					di = (bp04->RevertEffect() != ((si == OBJECT_NULL) ? 1 : 0)) ? 1 : 0;
					//^2FCF:1CA2
					//^2FCF:1E65
					//^2FCF:1E67
					}
					else { // DM1 alcove item = ACTUATOR_TYPE_DM1_ALCOVE_ITEM
						;
					}
					break;

				case ACTUATOR_TYPE_CHARGED_ITEM_WATCHER: // 0x15 -> 'Activator, charged item watcher'
					//^2FCF:1CA5
					if (ADD_ITEM_CHARGE(si, 0) == 0)
						//^2FCF:1CB3
						break;

					goto _1cb6;

				case ACTUATOR_TYPE_ITEM_WATCHER: // 0x03 -> 'Activator, item watcher
					//^2FCF:1CB6
_1cb6:
					bp2c = (GET_DISTINCTIVE_ITEMTYPE(si) == bp18) ? 1 : 0;
					//^2FCF:1CCC
					di = (bp04->RevertEffect() == bp2c) ? 1 : 0;
					//^2FCF:1CE7
					if (bp2c == 0 || bp04->OnceOnlyActuator() == 0)
						//^2FCF:1D01
						break;
					//^2FCF:1D04
					DEALLOC_RECORD(REMOVE_OBJECT_FROM_HAND());
					//^2FCF:1D10
					break;

				// SPX: addition for DM1 retrocompatibility
				case ACTUATOR_TYPE_DM1_ITEM_EATER: // 0x04 -> 'Activator, item eater
					//printf("ITEM EATER: expected = %d / in hand = %d\n", bp18, GET_DISTINCTIVE_ITEMTYPE(si));
					bp2c = (GET_DISTINCTIVE_ITEMTYPE(si) == bp18) ? 1 : 0;
					if (bp2c == 0) // not the expected item
						break;
					DEALLOC_RECORD(REMOVE_OBJECT_FROM_HAND());
					//DM1_ROTATE_ACTUATOR_LIST(2, xx, yy, -1, bp10);
					bDelayedActuatorsRotation = 1;
					iWallSideToRotate = bp10;
					//di = (bp04->RevertEffect() == bp2c) ? 1 : 0;
					di = 0; // allow actuator invoke further
					if (bp04->SoundEffect() != 0 || SkCodeParam::bDM1TQMode == 1) {	// SPX: in TQ, wall trigger do sound, no matter sound flag
						QUEUE_NOISE_GEN2(GDAT_CATEGORY_WALL_GFX, bp23, SOUND_STD_ACTIVATION, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1, 0x8C, 0x80);
					}
					
					if (bp44 == ACTUATOR_TYPE_DM1_ITEM_EATER && bp04->OnceOnlyActuator() == 1)
					{
						bp04->Disable();
						bp04->DisableToNoneType();
						bp16 = bp04->ActuatorType();
					}
					break;

				// SPX: addition for DM1 retrocompatibility
				case ACTUATOR_TYPE_DM1_WALL_TOGGLER: // 0x0D -> 'Wall toggler (Torch holder)
					{
						U16 iInvokeActuator = 0;
						//printf("WALL TOGGLER: expected = %d / in hand = %d\n", bp18, GET_DISTINCTIVE_ITEMTYPE(si));
						// if hand is empty, player will get the item over the actuator, if it is on top
						// else, if item in hand matches the expected item, it will go into the wall
						//if (xTopActuators[bp10] == bp0e && GET_DISTINCTIVE_ITEMTYPE(si) == 511) // empty hand
						if (GET_DISTINCTIVE_ITEMTYPE(si) == 511) // empty hand // UPDATE with Kid Dungeon : seems that the toggler is not necessarily on top at first
						{
							//DM1_ROTATE_ACTUATOR_LIST(2, xx, yy, -1, bp10);
							bDelayedActuatorsRotation = 1;
							iWallSideToRotate = bp10;
							// get item from wall (same direction, and takeable item)
							{
								ObjectID oAnyItem = GET_WALL_TILE_ANY_TAKEABLE_ITEM_RECORD(xx, yy, bp10);
								if (oAnyItem != OBJECT_NULL)
								{
									MOVE_RECORD_TO(oAnyItem, xx, yy, -1, 0); // remove it from wall
									bp34 = oAnyItem; // object to take thereafter
									iInvokeActuator = 1;
								}
							}
						}
						else if (GET_DISTINCTIVE_ITEMTYPE(si) == bp18)
						{
							//DM1_ROTATE_ACTUATOR_LIST(2, xx, yy, -1, bp10);
							bDelayedActuatorsRotation = 1;
							iWallSideToRotate = bp10;
							MOVE_RECORD_TO(ObjectID(si, bp10), -1, 0, xx, yy);
							REMOVE_OBJECT_FROM_HAND();
							//DEALLOC_RECORD(REMOVE_OBJECT_FROM_HAND());
							iInvokeActuator = 1;
						}
						if (iInvokeActuator)
						{
							if (bp04->SoundEffect() != 0 || SkCodeParam::bDM1TQMode == 1) {	// SPX: in TQ, wall trigger do sound, no matter sound flag
								QUEUE_NOISE_GEN2(GDAT_CATEGORY_WALL_GFX, bp23, SOUND_STD_ACTIVATION, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1, 0x8C, 0x80);
							}
							INVOKE_ACTUATOR(bp04, bp04->ActionType(), 0);
						}
					}
					continue; // because toggler will be moved, but still need to operate

				case ACTUATOR_TYPE_PUSH_BUTTON_WALL_SWITCH: // 0x46 -> 'Activator, seal-able push button wall switch'
					//^2FCF:1D13
					bp14 = GET_ADDRESS_OF_TILE_RECORD(bp04->Xcoord(), bp04->Ycoord())->castToDoor();
					//^2FCF:1D39
					if (bp14->DoorBit13C() == 0)
						//^2FCF:1D4A
						continue;

					goto _1d4d;

				case ACTUATOR_TYPE_CREATURE_KILLER:	// SPX: New wall actuator, use same principle as floor CREATURE KILLER; that allows to do the same from a wall switch
					{
						/*ACTIVATE_CREATURE_KILLER(2,//bp04->ActuatorData()&15, 
							0x01E1, //(bp04->ActuatorData()>>4)&0x1f, 
							xx, 
							yy, 
							bp04->Xcoord(),
							bp04->Ycoord(), 
							ACTUATOR_FLOOR_TYPE__CREATURE_KILLER, 
							0);
							*/
						ObjectID rlCreature = GET_CREATURE_AT(bp04->Xcoord(), bp04->Ycoord());
						if (rlCreature != OBJECT_NULL && rlCreature != OBJECT_END_MARKER) {
							DELETE_CREATURE_RECORD(bp04->Xcoord(), bp04->Ycoord(), 0, 1);
							QUEUE_NOISE_GEN2(GDAT_CATEGORY_CREATURES, 0x7F, SOUND_CREATURE_DEATH, 0xFE, bp04->Xcoord(), bp04->Ycoord(), 1, 0x8c, 0x80);
						}
						if (bp04->OnceOnlyActuator() == 1)
						{
							bp04->Disable();
							bp04->DisableToNoneType();
							bp16 = bp04->ActuatorType();
						}
					}
					continue;

				case ACTUATOR_TYPE_DM1_WALL_SWITCH: // SPX: Add DM1 retrocompatibility : 0x01
					if (si != OBJECT_NULL || bp04->ActiveStatus() != 0)
						//^2FCF:1D52
						continue;
					// NOTE: DM1 wall switch can't check "disabled" flag because it is the same for "rotate" which is used here. A disabled wall switch is actually changed to "NONE" actuator.
					Timer bp40;
					bp40.SetMap(glbCurrentMapIndex);
					bp40.SetTick(glbGameTick +bp18 +2);
					bp40.TimerType(ttyWallButton);
					bp40.actor = TIMER_ACTOR__00;
					bp40.value = bp0e;
					QUEUE_TIMER(&bp40);
					// SPX: DM1 compatibility for PUSH BUTTON with actuator toggler info (generally used for 2-image state lever, buttons ...)
					if (bp44 == ACTUATOR_TYPE_DM1_WALL_SWITCH && bp04->ActuatorToggler() == 1)
					{
						bDelayedActuatorsRotation = 1;
						iWallSideToRotate = bp10;
						if (bp04->SoundEffect() != 0 || SkCodeParam::bDM1TQMode == 1) {	// SPX: in TQ, wall trigger do sound, no matter sound flag
							QUEUE_NOISE_GEN2(GDAT_CATEGORY_WALL_GFX, bp23, SOUND_STD_ACTIVATION, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1, 0x8C, 0x80);
						}
					}

					di = 0;
					//if (di != 0 || bp1a != 3) // SPX: if actuator effect is not 3 (step in open / step out close), then break
					//	break;
					bp26 = 1;
					if (bp04->SoundEffect() != 0) {
						QUEUE_NOISE_GEN2(GDAT_CATEGORY_WALL_GFX, bp23, SOUND_STD_ACTIVATION, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1, 0x8C, 0x80);
					}
					// Note: if the push button is in ROTATE mode (which shares the INACTIVE bit), then do not trigger a target effect.
					if (bp04->ActuatorToggler() == 0)
					{
						INVOKE_ACTUATOR(bp04, bp04->ActionType(), 0);
					}
					if (bp44 == ACTUATOR_TYPE_DM1_WALL_SWITCH && bp04->OnceOnlyActuator() == 1)
					{
						bp04->Disable();
						bp04->DisableToNoneType();
						bp16 = bp04->ActuatorType();
					}
					continue;

				case ACTUATOR_TYPE_WALL_SWITCH: // 0x18 -> 'Activator, push button wall switch'
					//^2FCF:1D4D
_1d4d:
					if (si != OBJECT_NULL || bp04->ActiveStatus() != 0)
						//^2FCF:1D52
						continue;
					//^2FCF:1D66
					Timer xtimer;
					xtimer.SetMap(glbCurrentMapIndex);
					xtimer.SetTick(glbGameTick +bp18 +2);
					//^2FCF:1D8F
					xtimer.TimerType(ttyWallButton);
					xtimer.actor = TIMER_ACTOR__00;
					//^2FCF:1D97
					xtimer.value = bp0e;
					//^2FCF:1D9D
					QUEUE_TIMER(&xtimer);
					//^2FCF:1DA9
						bp04->ActiveStatus(1);
					
					//^2FCF:1DB1
					di = 0;
					//^2FCF:1DB3
					if (di != 0 || bp1a != 3) // SPX: if actuator effect is not 3 (step in open / step out close), then break
						//^2FCF:1DC0
						break;
					//^2FCF:1DC3
					bp26 = 1;
					//^2FCF:1DC8
					if (bp04->SoundEffect() != 0) {
						//^2FCF:1DD6
						QUEUE_NOISE_GEN2(GDAT_CATEGORY_WALL_GFX, bp23, SOUND_STD_ACTIVATION, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1, 0x8C, 0x80);
					}
					//^2FCF:1DFA
					INVOKE_ACTUATOR(bp04, 0, 0);
					//^2FCF:1E0B
					INVOKE_ACTUATOR(bp04, 1, bp18 +1);
					//^2FCF:1E1F
					continue;

				case ACTUATOR_TYPE_2_STATE_WALL_SWITCH: // 0x17 -> 'Activator, 2 state wall switch'
					//^2FCF:1E22
					if (si != OBJECT_NULL)
						//^2FCF:1E27
						continue;
					//^2FCF:1E2A
					bp04->OnceOnlyActuator(bp04->OnceOnlyActuator() ^ 1);
					//^2FCF:1E49
					di = !(bp04->OnceOnlyActuator() ^ bp04->RevertEffect());
					//^2FCF:1E65
					//^2FCF:1E67
					break;

				case ACTUATOR_TYPE_RESURECTOR: // 0x7E -> 'Activator, resuscitation'
					//^2FCF:1E6A
					if (bp04->OnceOnlyActuator() == 0 || ((cd.pi.glbPlayerDir + 2) & 3) != dir)
						//^2FCF:1E8B
						break;
					//^2FCF:1E8E
					SELECT_CHAMPION(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, cd.pi.glbPlayerDir, cd.pi.glbPlayerMap);
					//^2FCF:1EA6
					bp28 = 1;
					//^2FCF:1EAB
					di = 0;
					//^2FCF:1EAD
					break;

				// SPX: addition for DM1 retrocompatibility
				case ACTUATOR_TYPE_CHAMPION_MIRROR: // 0x7F -> DM1 'Activator, resuscitation'

					if (bp04->ActiveStatus() == 1 || ((cd.pi.glbPlayerDir +2) & 3) != dir) // for DM1, just take condition of direction
						break;
					SELECT_CHAMPION(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, cd.pi.glbPlayerDir, cd.pi.glbPlayerMap);
					bp28 = 1;
					di = 0;
					break;


				case ACTUATOR_TYPE_KEY_HOLE: // 0x1A -> 'Activator, key hole'
					//^2FCF:1EB0
					bp1e = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp23, dtWordValue, GDAT_WALL_ORNATE__IS_ITEM_TRIGGERED);
					//^2FCF:1EC5
					if (bp04->OnceOnlyActuator() != 0) {
						//^2FCF:1ED6
						if (si != OBJECT_NULL)
							//^2FCF:1EDB
							goto _22b1;
						//^2FCF:1EDE
						si = FIND_DISTINCTIVE_ITEM_ON_TILE(xx, yy, bp10, bp1e);
						//^2FCF:1EF3
						if (si == OBJECT_END_MARKER) {
							//^2FCF:1EF8
							si = ALLOC_NEW_DBITEM(bp1e);
							//^2FCF:1F03
							if (si == OBJECT_NULL)
								//^2FCF:1F08
								continue;
							//^2FCF:1F0B
							APPEND_RECORD_TO(si = ObjectID(si, bp10), NULL, xx, yy);
						}
						//^2FCF:1F30
						ADD_ITEM_CHARGE(si, 15);
						//^2FCF:1F3A
						MOVE_RECORD_TO(si, xx, yy, -1, 0);
						//^2FCF:1F4C
						bp34 = si;
					}
					else {
						//^2FCF:1F51
						if (si == OBJECT_NULL || GET_DISTINCTIVE_ITEMTYPE(si) != bp1e)
							//^2FCF:1F65
							continue;
						//^2FCF:1F68
						bp2a = 1;
						//^2FCF:1F6D
						MOVE_RECORD_TO(ObjectID(si, bp10), -1, 0, xx, yy);
					}
					//^2FCF:1F8D
					si = OBJECT_NULL;
					//^2FCF:1F90
					goto _22ab;

				case ACTUATOR_TYPE_X1B: // 0x1b -> ?
					//^2FCF:1F96
					if (bp18 == 0)
						//^2FCF:1F9C
						continue;
					//^2FCF:1F9F
					bp1e = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp23, dtWordValue, 0x0e);
					//^2FCF:1FB4
					if (GET_DISTINCTIVE_ITEMTYPE(si) != bp1e)
						//^2FCF:1FC0
						continue;
					//^2FCF:1FC3
					bp2a = 1;
					//^2FCF:1FC8
					si = OBJECT_NULL;
					//^2FCF:1FCB
					if (--bp18 == 0) {
						//^2FCF:1FD5
						di = 0;
						//^2FCF:1FD7
						bp04->ActiveStatus(1);
					}
					//^2FCF:1FDF
					bp04->ActuatorData(bp18);
					//^2FCF:1FF5
					break;

				case ACTUATOR_TYPE_SIMPLE_LADDER: // 0x1c -> ? (comes from BETA)
					if (SkCodeParam::bUseFixedMode == false)	// SPX: this actuator type seems to be used only in BETA dungeon, as a ladder. Though with the original code here, it does not trigger.
					{
					//^2FCF:1FF8
					if (si != OBJECT_NULL || bp04->OnceOnlyActuator() != 0)
						//^2FCF:2011
						continue;
					}
					else	// SPX: I am not sure here, I keep the condition in another way so that the ladder triggers when player click on it.
						if (si == OBJECT_NULL && bp04->OnceOnlyActuator() == 0)
							continue;
					//^2FCF:2014
					bp36 = (bp04->RevertEffect() != 0) ? bp04->ActionType() : ((bp04->ActionType() + cd.pi.glbPlayerDir) & 3);
					//^2FCF:204E
					TRANSFER_PLAYER(bp04->Xcoord(), bp04->Ycoord(), bp18 & 0x3f, bp36);
					//^2FCF:2078
					_1031_098e();

					break;
			}
			//^2FCF:207D
			if (bp1a == 3) {
				//^2FCF:2083
				bp1a = (di != 0) ? 1 : 0;
				//^2FCF:2091
				di = 0;
			}
			//^2FCF:2093
			if (di != 0)
				//^2FCF:2097
				continue;
			//^2FCF:209A
			bp26 = 1;
			//^2FCF:209F
			if (bp04->SoundEffect() != 0) {
				//^2FCF:20B0
				QUEUE_NOISE_GEN2(GDAT_CATEGORY_WALL_GFX, bp23, SOUND_STD_ACTIVATION, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1, 0x8c, 0x80);
			}
			//^2FCF:20D4
			INVOKE_ACTUATOR(bp04, bp1a, 0);
			//^2FCF:20E6
			continue;
		}
		//^2FCF:20E9
		U8 bp23;
		Text *bp0c;
		SimpleActuator *xSimpleActuator;
		if (rlUnk == OBJECT_NULL && bp1c == dbText) {
			//^2FCF:20FB
			bp0c = GET_ADDRESS_OF_RECORD2(bp0e);
			xSimpleActuator = (SimpleActuator*) bp0c;
			//printf("Current Simple Actuator GFX = %d (%02x)\n", xSimpleActuator->graphism, xSimpleActuator->graphism);
			//^2FCF:210A
			if (bp0c->TextMode() != 1)
				//^2FCF:211B
				continue;
			//^2FCF:211E
			bp23 = bp0c->TextIndex() & 0xff;
			//^2FCF:212D
			if (glbChampionLeader == -1)
				//^2FCF:2134
				continue;
			//^2FCF:2137
			i16 bp20;
			i16 bp22;
			i16 bp36;
			U16 bp1e;
			switch (bp16 = bp0c->SimpleTextExtUsage()) {
				case  4: // Holder (Torch holder)
				case  8: // Recharge (Blue gem crop)
					//^2FCF:2161
					bp1e = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp23, dtWordValue, GDAT_WALL_ORNATE__SWITCH_ITEM);
					//^2FCF:2176
					if (bp0c->TextVisibility() != 0) {
						//^2FCF:2187
						if (si != OBJECT_NULL)
							//^2FCF:218C
							goto _22b1;
						//^2FCF:218F
						si = FIND_DISTINCTIVE_ITEM_ON_TILE(xx, yy, bp10, bp1e);
						//^2FCF:21A4
						if (si == OBJECT_NULL) {
							//^2FCF:21A9
							si = ALLOC_NEW_DBITEM(bp1e);
							//^2FCF:21B4
							if (si == OBJECT_NULL)
								//^2FCF:21B9
								continue;
							//^2FCF:21BC
							APPEND_RECORD_TO(si = ObjectID(si, bp10), NULL, xx, yy);
						}
						//^2FCF:21E1
						ADD_ITEM_CHARGE(si, 15);
						//^2FCF:21EB
						MOVE_RECORD_TO(si, xx, yy, -1, 0);

						SkD((DLV_TWEET, "Tweet: You (x:%d, y:%d, map:%d) have taken %s from wall \n"
							, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, glbCurrentMapIndex
							, static_cast<LPCSTR>(getRecordNameOf(si))
							));
						//^2FCF:21FD
						bp34 = si;
						//^2FCF:2200
						si = OBJECT_NULL;
						//^2FCF:2203
						if (bp16 != 8)
							//^2FCF:2209
							goto _22ab;
						//^2FCF:220C
						Timer bp40;
						bp40.SetMap(glbCurrentMapIndex);
						// SPX: 0x12 is the RESPAWN value, then trigger a timer to setup again the ornate (gem)
						bp40.SetTick(QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp23, dtWordValue, GDAT_WALL_ORNATE__RESPAWN_COOLDOWN) + glbGameTick +2);
						//^2FCF:224A
						bp40.TimerType(ttySimpleActuTurnOn);
						bp40.actor = TIMER_ACTOR__00;
						//^2FCF:2252
						bp40.value = bp0e;
						//^2FCF:2258
						QUEUE_TIMER(&bp40);
					}
					//^2FCF:2266
					else if (bp16 != 8) {	// Is not Recharge (8), then it is Torch Holder (4)
						//^2FCF:226C
						if (si == OBJECT_NULL)
							//^2FCF:2271
							continue;
						//^2FCF:2274
						if (GET_DISTINCTIVE_ITEMTYPE(si) != bp1e)
							//^2FCF:2280
							continue;
						//^2FCF:2283
						bp2a = 1;
						//^2FCF:2288
						MOVE_RECORD_TO(ObjectID(si, bp10), -1, 0, xx, yy);

						SkD((DLV_TWEET, "Tweet: You (x:%d, y:%d, map:%d) have placed %s at wall \n"
							, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, glbCurrentMapIndex
							, static_cast<LPCSTR>(getRecordNameOf(si))
							));

						//^2FCF:22A8
						si = OBJECT_NULL;
					}
					//^2FCF:22AB
_22ab:
					glbDoLightCheck = 1;
					//^2FCF:22B1
_22b1:
					bp28 = 1;
					//^2FCF:22B6
					continue;

				case  7:	// ladder disabled
					//^2FCF:22B9
					if (bp0c->TextVisibility() != 0)
						//^2FCF:22C7
						continue;

					goto _22ca;

				case  6:	// ladder enabled
					//^2FCF:22CA
_22ca:
					if (si != OBJECT_NULL)
						//^2FCF:22CF
						continue;
					//^2FCF:22D2
					bp36 = (QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp23, dtWordValue, GDAT_WALL_ORNATE__IS_LADDER_UP) != 0) ? -1 : 1;
					//^2FCF:22F4
					bp20 = cd.pi.glbPlayerPosX;
					bp22 = cd.pi.glbPlayerPosY;
					//^2FCF:2300
					bp36 = LOCATE_OTHER_LEVEL(cd.pi.glbPlayerMap, bp36, &bp20, &bp22, NULL);
					//^2FCF:2320
					if (bp36 < 0)
						//^2FCF:2322
						continue;
					//^2FCF:2324
					TRANSFER_PLAYER(bp20, bp22, bp36, cd.pi.glbPlayerDir);
					//^2FCF:2338
					_1031_098e();
					//^2FCF:233D
					continue;

				case 13:	// 2 state ornate (such as window)
					//^2FCF:233F
					if (si != OBJECT_NULL)
						//^2FCF:2342
						continue;
					//^2FCF:2344
					bp0c->TextVisibility(bp0c->TextVisibility() ^ 1);

					break;

				case  5:
				case  9:
				case 10:
				case 11:
				case 12:
					continue;
			}
		}
		//^2FCF:235F
		// TODO DM2_EXTENDED_DATABASE
		if (bp1c >= dbActuator)
			//^2FCF:2363
			break;
		//^2FCF:2365
	}


	// SPX: DM1: if actuators need to be rotated, do it here
	if (bDelayedActuatorsRotation == 1)
		DM1_ROTATE_ACTUATOR_LIST(2, xx, yy, -1, iWallSideToRotate);

	
	//^2FCF:2380
	if (rlUnk == OBJECT_NULL) {
		//^2FCF:2386
		if (bp2a != 0)
			//^2FCF:238C
			REMOVE_OBJECT_FROM_HAND();
		//^2FCF:2391
		if (bp34 != OBJECT_NULL) {
			//^2FCF:2397
			TAKE_OBJECT(bp34, 1);
		}
		//^2FCF:23A3
		cd.gg.glbRefreshViewport = bp26;
		//^2FCF:23A9
		return;
	}
	//^2FCF:23AB
	GenericContainerRecord *bp08 = GET_ADDRESS_OF_GENERIC_CONTAINER_RECORD(rlUnk);
	//^2FCF:23BA
	if (bp2a == 0 && rlWhatYouPlace != OBJECT_NULL) {
		//^2FCF:23C6
		APPEND_RECORD_TO(rlWhatYouPlace, &bp08->possession, -1, 0);
	}
	//^2FCF:23DE
	if (bp34 != OBJECT_NULL) {
		//^2FCF:23E4
		APPEND_RECORD_TO(bp34, &bp08->possession, -1, 0);
	}

	//^2FCF:23FC
	return;
}

//^2066:0B96
U16 SkWinCore::STORE_EXTRA_DUNGEON_DATA()
{
	//^2066:0B96
	_4976_5258 = 0;
	if (_2066_0b44_WRITE_TIMERS() != 0)
		return 0;
	U16 bp10 = glbCurrentMapIndex;
	U16 currentMap = 0;
	//^2066:0BBB
	while (currentMap < dunHeader->nMaps) {
		CHANGE_CURRENT_MAP_TO(currentMap);
		Bit8u *bp04 = *glbCurrentTileMap;
		OID_T *bp08 = &dunGroundStacks[*_4976_4c52];	// U16 *bp08
		U16 xpos = 0; // word [bp-0C]
        //^2066:0BF4
		while (!(xpos >= glbCurrentMapWidth)) {
			//^2066:0BF7
			U16 ypos = 0; // di
			//^2066:0BF9
			while (!(ypos >= glbCurrentMapHeight)) {
				Bit8u tileValue = *bp04; // [bp-0D]
				//^2066:0BFC
				bp04++;
				U16 skipSaveRecord = 0; // word [bp-18]
				Bit8u bp0e;
				TELE_inf bp16;
				switch (tileValue >> 5) {
					case ttWall:
					case ttFloor:
					case ttStairs:
					case ttMapExit:
						//^2066:0C22
						bp0e = 0x00;
						break;
					case ttPit:
						//^2066:0C28
						bp0e = 0x08;
						break;
					case ttDoor:
						//^2066:0C2E
						bp0e = 0x07;
						break;
					case ttTeleporter:
						//^2066:0C34
						if (GET_TELEPORTER_DETAIL(&bp16, (Bit8u)xpos, (Bit8u)ypos) != 0) {
							//^2066:0C49
							bp0e = 0x00;
							if (bp16.target_z() >= currentMap)
								break;
							//^2066:0C56
							skipSaveRecord = 0x01;
							break;
						}
						//^2066:0C5D
						bp0e = 0x08;
						break;
					case ttTrickWall:
						//^2066:0C63
						bp0e = 0x04;
						break;
				}
				//^2066:0C67
				if (!(bp0e == 0)) {
					//^2066:0C6D
					if (SUPPRESS_WRITER(&tileValue, &bp0e, 1, 1) != 0)
						return 0;
				}
				//^2066:0C88
				U16 tileRecord; // [bp-0A]
				if ((tileValue & 0x10) != 0) {
					//^2066:0C8E
					tileRecord = *bp08;
					bp08++;
				}
				else {
					//^2066:0C9D
					tileRecord = 0xFFFE;
				}
				//^2066:0CA2
				if (skipSaveRecord == 0) {
					if (WRITE_RECORD_CHECKCODE(tileRecord, 1, 1) != 0)
						return 0;
				}
				//^2066:0CBA
				ypos++;
				//^2066:0CBB
			}
			//^2066:0CC4
			xpos++;
			//^2066:0CC7
		}
		currentMap++;
		//^2066:0CD4
	}
	//^2066:0CE5
	CHANGE_CURRENT_MAP_TO(bp10);
	return 1;
}



//^24A5:036A
// SPX: This one in the end compute light depending on champion position
// SPX: This func is called for each tile in the nearby area from the champion
// SPX: _24a5_036a renamed ADD_BACKGROUND_LIGHT_FROM_TILE
void SkWinCore::ADD_BACKGROUND_LIGHT_FROM_TILE(i16 aa, U16 bb, i16 xx, i16 yy, U16 ww)
{
	//^24A5:036A
	ENTER(46);
	//^24A5:0370
	X16 di = 0;
	X16 bp12 = 0;
	X16 bp14 = 0;
	ExtendedTileInfo bp2e;
	//printf("ADD_BACKGROUND_LIGHT_FROM_TILE: aa=%d bb=%d xx=%d yy=%d ww=%d\n", aa, bb, xx, yy, ww);
	SUMMARIZE_STONE_ROOM(&bp2e,bb,xx,yy);	// SPX: light is precomputed within this function
	i16 bp16;
	X8 bp0b;
	U16 si;
	U16 bp10;
	U16 bp1e;
	ObjectID bp0a;
	U16 bp0e;
	switch (bp2e.w0) {
		case 0://^03A4
			//^24A5:03A4
			if ((ww & 4) == 0)
				break;
			bp16 = bp2e.tfoi[2];
			bp0b = X8(bp16 & 0xff);
			if (bp0b == 0xff)
				break;
			si = QUERY_GDAT_ENTRY_DATA_INDEX(9, bp0b, dtWordValue, 0xf8);	// SPX: A light value (wall actuator)?
			if (si == 0)
				break;
			bp10 = ((si & 0x8000) != 0) ? 1 : 0;
			si &= 0x7fff;
			if (QUERY_GDAT_ENTRY_DATA_INDEX(9, bp0b, dtWordValue, 0x63) != 0) {
				//^24A5:0408
				bp1e = BETWEEN_VALUE(0, glbRainAmbientLightModifier + glbRainSomeLightMod, 5);
				bp12 = (tLightLevelTable[RCJ(6,bp1e)] * si) / 0x64 +bp12;
				break;
			}
			//^24A5:043F
			if (bp10 != 0 && (bp16 >> 8) == 0)
				break;
			//^24A5:0452
			di += si;
			break;
		case 1://^0457
		case 2://^0457
		case 5://^0457
		case 16://^0457
		case 17://^0457
		case 18://^0457
		case 19://^0457
			//^24A5:0457
			if ((ww & 1) == 0)
				break;
			if (bp2e.w0 != 1 && bp2e.w0 != 2 && bp2e.w0 != 5)
				goto _0519;
			//^24A5:0476
			bp16 = bp2e.tfoi[2];
			bp0b = X8(bp16);
			if (bp0b == 0xff)
				goto _0519;
			si = QUERY_GDAT_ENTRY_DATA_INDEX(10, bp0b, dtWordValue, 0xf8);
			if (si == 0)
				break;
			bp10 = ((si & 0x8000) != 0) ? 1 : 0;
			si &= 0x7fff;
			//^24A5:04BA
			TELE_inf bp1c;
			if (GET_TELEPORTER_DETAIL(&bp1c, U8(xx), U8(yy)) != 0) {
				//^24A5:04D1
				bp1e = BETWEEN_VALUE(0, glbRainAmbientLightModifier + glbRainSomeLightMod, 5);
				bp12 = (tLightLevelTable[RCJ(6,bp1e)] * si) / 0x64 +bp12;
				goto _0519;
			}
			else {
				//^24A5:0507
				if (bp10 == 0 || (bp16 >> 8) != 0)
					di += si;
				//^24A5:0519
_0519:
				bp0a = bp2e.w4;
			}
			for (; bp0a != OBJECT_END_MARKER; bp0a = GET_NEXT_RECORD_LINK(bp0a)) {
				//^24A5:051F
				bp0e = bp0a.DBType();
				if (bp0e == dbCloud) {
					//^24A5:0530
					Cloud *bp04 = GET_ADDRESS_OF_RECORDF(bp0a);
					if (bp04->CloudType() != 2 && bp04->CloudType() != 0 && bp04->CloudType() != 0x30)
						continue;
					//^24A5:0565
					bp14 = (bp04->b3_0_f() >> 1) +bp14;
					continue;
				}
				//^24A5:057B
				if (bp0e == dbMissile) {
					Missile *bp08 = GET_ADDRESS_OF_RECORDE(bp0a);
					if (bp08->GetMissileObject() == OBJECT_EFFECT_FIREBALL) {	// oFF80
						bp14 += (i16(bp08->EnergyRemaining()) >> 1) - _4976_3d6d[3];
					}
				}
				//^24A5:05A9
			}
			//^24A5:05BE
			if ((ww & 2) != 0) {
				bp0a = GET_CREATURE_AT(xx, yy);
				if (bp0a != OBJECT_NULL) {
					si = QUERY_GDAT_DBSPEC_WORD_VALUE(bp0a, 0xf8);
					di = si & 0x7fff;
				}
			}
			break;
		case 3://^05EE
		case 4://^05EE
		case 6://^05EE
		case 7://^05EE
		case 8://^05EE
		case 9://^05EE
		case 10://^05EE
		case 11://^05EE
		case 12://^05EE
		case 13://^05EE
		case 14://^05EE
		case 15://^05EE
			break;
	}
	//^24A5:05EE
	// SPX: aa is distance between player and the current processed tile
	if (aa <= 8 && bp12 != 0) {
		bp12 = max_value(3, bp12 - _4976_3d5b[RCJ(9,aa)]);
	}
	else {
		bp12 = 0;
	}
	//^24A5:061A
	if (aa > 5)
		aa = 5;
	//^24A5:0625
	if (di != 0) {
		//^24A5:0629
		di = max_value(2, di - _4976_3d6d[RCJ(6,aa)]);
	}
	//^24A5:0640
	//printf("Calculting light: %d + %d + %d => %d\n", glbPrecomputedLight, di, bp12, glbPrecomputedLight + di + bp12);
	glbPrecomputedLight += di + bp12;

	//^24A5:0649
	if (bp14 != 0) {
		//^24A5:064F
		bp14 = max_value(2, bp14 - _4976_3d6d[RCJ(6,aa)]);
	}
	//^24A5:0668
	glbLightModifier += bp14;
	//^24A5:066F
	return;
}


//^1C9A:02F6
// SPX: _1c9a_02f6 renamed CHECK_RECOMPUTE_LIGHT
void SkWinCore::CHECK_RECOMPUTE_LIGHT(i16 xx, i16 yy)
{
	//^1C9A:02F6
	ENTER(34);
	//^1C9A:02FB
	U16 recomputeLight = 0;	// si
	//^1C9A:02FD
	// SPX: Comparison of these 0x6D (GDAT_GFXSET_AMBIANT_DARKNESS) values ..
	// VOID => 0
	// THICKET => 3
	// CAVE => 8
	// KEEP => 8
	// ROOF => 3
	// MISTY => 3
	// DEBUG => 8
	i16 bp0c = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, dtWordValue, GDAT_GFXSET_AMBIANT_DARKNESS);
	//^1C9A:0312
	if (bp0c == 0) {
		//^1C9A:0318
		glbDoLightCheck = 0;
		//^1C9A:031E
		glbPrecomputedLight = 0;
		glbPreviousPrecompLight = 0;
		glbLightModifier = 0;
		glbPreviousLightModifier = 0;
		//^1C9A:032E
		return;
	}
	//^1C9A:0330
	if (bp0c > 8) {	// Thresholding at 8
		//^1C9A:0336
		bp0c = 8;
	}
	//^1C9A:033B
	skxxx9 bp22;
	bp22.b0 = i8(bp0c);
	bp22.b1 = 0;
	bp22.b7 = 0x17;
	//^1C9A:0349
	WalkPath bp0a[5]; // size=?
	FIND_WALK_PATH(xx, yy, 7, 0, 1, &bp22, bp0a);
	//^1C9A:0367
	if (glbPrecomputedLight != glbPreviousPrecompLight) {
		//^1C9A:0370
		glbPreviousPrecompLight = glbPrecomputedLight;
		recomputeLight = 1;
	}
	//^1C9A:0376
	if (glbLightModifier != glbPreviousLightModifier) {
		//^1C9A:037F
		glbPreviousLightModifier = glbLightModifier;
		recomputeLight = 1;
	}
	//^1C9A:0385
	if (recomputeLight != 0) {
		//^1C9A:0389
		RECALC_LIGHT_LEVEL();
	}
	//^1C9A:038E
	glbDoLightCheck = 0;
	//^1C9A:0394
	return;
}

//^38C8:03AD
// SPX: _38C8_03AD renamed __INIT_GAME_38c8_03ad
void SkWinCore::__INIT_GAME_38c8_03ad()
{
	SkD((SkCodeParam::bEngineNoDisplay, "__INIT_GAME_38c8_03ad\n"));
	ENTER(0);
	glbShowItemStats = 0;
	_4976_4bfe = 0;
	_4976_4c3e = 0;
	glbTryPushPullObject = 0;
	cd.pi.glbIsPlayerSleeping = 0;
	_4976_5bec = 0;
	ZERO_MEMORY(glbChampionMajorSkillsLevel, sizeof(glbChampionMajorSkillsLevel));
	ZERO_MEMORY(glbChampionEngagingHandCommand, sizeof(glbChampionEngagingHandCommand));
	cd.pi.glbChampionIndex = 0;
	_4976_5dbc = 0;
	RESET_CAII();
	_1031_0541(5);
	LOAD_NEWMAP(U8(cd.pi.glbPlayerMap));
	_4976_4bd8 = 0;
	if (cd.mo.glbSpecialScreen == _MENU_SCREEN__RESUME_GAME_SELECT) {
		FIRE_FILL_SCREEN_RECT(2, 0);
		_0aaf_0067(_0aaf_02f8_DIALOG_BOX(6, 0));
		U16 iGDatFlag = glbGDatOpenCloseFlag;
		if (iGDatFlag != 0) {
			glbGDatOpenCloseFlag = 1;
			GRAPHICS_DATA_CLOSE();
		}
		_2066_03e0(0);
		if (iGDatFlag != 0) {
			GRAPHICS_DATA_OPEN();
			glbGDatOpenCloseFlag = iGDatFlag;
		}
		WAIT_SCREEN_REFRESH();
	}
	else {
		WAIT_SCREEN_REFRESH();
		cd.gg.glbGameHasEnded = 0;
	}
	FIRE_FILL_SCREEN_RECT(2, 0);
	DRAW_ARROW_PANEL();
	printf("38c8_03ad:INIT_CHAMPIONS\n");
	SEARCH_STARTER_CHAMPION();
	_4976_4c02 = 1;
	CHECK_RECOMPUTE_LIGHT(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY);
	return;
}

//^101B:0001
//SPX: _101b_0001 renamed END_GAME
void SkWinCore::END_GAME(U16 xx)
{
	ENTER(2);
	FIRE_HIDE_MOUSE_CURSOR();
	//^101B:000A
	if (xx != 0 && _4976_4c26 == 0) {
		//^101B:0017
		U8 bp01 = (cd.pi.glbChampionsCount > 0) ? glbChampionSquad[0].HeroType() : 0xFE;
		//^101B:002A
		// SPX: Sound when dying
		QUEUE_NOISE_GEN2(GDAT_CATEGORY_CHAMPIONS, bp01, SOUND_CHAMPION_SCREAM, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 0, 255, 255);
		//^101B:004E
		SLEEP_SEVERAL_TIME(240);
	}
	//^101B:0057
	_2066_03e0(0);
	//^101B:005F
	cd.gg.glbGameHasEnded = 1;
#if DM2_EXTENDED_MODE == 1
	glbXAmbientSoundActivated = 0;	// reinit that variable to get ambient sound again with a restart
#endif
	//^101B:0065
	if (xx != 0) {
		//^101B:006B
		FIRE_FILL_SCREEN_RECT(2, 0);
		//^101B:0076
		if (_4976_5bf6 != 0) {
			//^101B:007D
			_4976_5bf2 = (_0aaf_0067(_0aaf_02f8_DIALOG_BOX(16, 0)) == 0) ? 1 : 0;
			//^101B:009D
			if (_4976_5bf2 != 0) {
				//^101B:00A1
				cd.mo.glbSpecialScreen = _MENU_SCREEN__RESUME_GAME_SELECT;
				//^101B:00A7
				if (GAME_LOAD() != -1) {
					//^101B:00B1
					__INIT_GAME_38c8_03ad();
					//^101B:00B6
					_4976_5bf2 = 0;
					//^101B:00BC
					FIRE_SHOW_MOUSE_CURSOR();
					//^101B:00C1
					_1031_098e();
					//^101B:00C6
					return;
				}
			}
		}
	}
	//^101B:00C8
	FIRE_FILL_SCREEN_RECT(2, 0);
	//^101B:00D3
	FIRE_SELECT_PALETTE_SET(0);
	//^101B:00DB
	//DRAW_TRANSPARENT_STATIC_PIC(0x06, 0x00, 0x01, 2, 0xffff);
	DRAW_TRANSPARENT_STATIC_PIC(GDAT_CATEGORY_CREDITS, 0x00, GDAT_IMG_CREDITS_TOMBSTONE, 2, 0xffff);
	//^101B:00ED
	FIRE_SELECT_PALETTE_SET(1);
	//^101B:00F5
	_1031_0541(1);
	//^101B:00FD
	while (MAIN_LOOP(), glbUIEventCode != UI_EVENTCODE_QUIT_CREDITS); // 239
	//^101B:010A
	SK_PREPARE_EXIT();
	//^101B:010F
	return;
}


//^12B4:0300
// SPX: _12b4_0300 renamed PERFORM_MOVE
U16 SkWinCore::PERFORM_MOVE(X16 xx)
{
	// Moves:
	//	xx = 3	forward
	//	xx = 4	move right
	//	xx = 5	backward
	//	xx = 6	move left
	ENTER(38);
	ObjectID bp14;
	U16 iCurrentTile = GET_TILE_VALUE(glbSomePosX_4c2e, glbSomePosY_4c30); // bp06
	X16 iIsStairs = ((iCurrentTile >> 5) == ttStairs) ? 1 : 0; // bp08
	i16 bp26 = 1;
	Champion *xChampion = glbChampionSquad;
	int iFinalInterwallValue = 0;	// SPX: new
	U16 iChampionIndex = 0;	/// si
	SkD((DLV_MOVE, "------------------------------------------------\n"));
	for (iChampionIndex = 0; iChampionIndex < cd.pi.glbChampionsCount; xChampion++, iChampionIndex++) {
		if (xChampion->curHP() != 0) {
			bp26 = max_value(bp26, CALC_PLAYER_WALK_DELAY(iChampionIndex));
			SkD((DLV_MOVE, "Move: champion %d sets walk delay to %d\n", iChampionIndex, bp26));
		}
	}
	U16 iDestTile = 0;	// bp0a
	i16 bp0e = 0;	// bp0e
	i16 playerDestPosX = 0; // bp1a
	i16 playerDestPosY = 0; // bp1c
	X16 di = 0;
	SkD((DLV_MOVE, "Move: delay:%d, glbIsPlayerMoving:%d, move:%d, DSM:%d, _4976_4c08:%d, iIsStairs:%d\n"
		, bp26
		, cd.pi.glbIsPlayerMoving
		, xx
		, SkCodeParam::bEnableDoubleStepMove
		, _4976_4c08
		, iIsStairs));

#if UseAltic
	if (bp26 > 1 && cd.pi.glbIsPlayerMoving == 0 &&
			((xx == 3 && SkCodeParam::bEnableDoubleStepMove) 
			|| ((xx == 5 && SkCodeParam::bEnableDoubleStepMove) && _4976_4c08 == 0 && iIsStairs == 0)
			|| glbTableToMove != OBJECT_NULL)
		) {
#else // isn't the same ??
	if (bp26 > 1 && cd.pi.glbIsPlayerMoving == 0 && (xx == 3 || (xx == 5 && _4976_4c08 == 0 && iIsStairs == 0) || glbTableToMove != OBJECT_NULL)) {
#endif
		cd.pi.glbIsPlayerMoving = bp26 >> 1;
		_4976_4c32 = cd.pi.glbPlayerPosX;
		_4976_4c34 = cd.pi.glbPlayerPosY;
		_4976_4c40 = cd.pi.glbPlayerDir;
		cd.pi.glbPlayerLastMove = xx;
		if (xx != 3) {
			CALC_VECTOR_W_DIR(cd.pi.glbPlayerDir, -1, 0, &_4976_4c32, &_4976_4c34);
		}
		if (SkCodeParam::bUsePlayerWalkSound)	// Use ROCKY walk sound
			//QUEUE_NOISE_GEN1(GDAT_CATEGORY_CREATURES, 0x0C, 0x00, 0x46, 0x80, glbPlayerPosX, glbPlayerPosY, 1);
			QUEUE_NOISE_GEN1(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, SOUND_CHAMPION_FOOTSTEP, 0x46, 0x80, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1);
		goto _0768;
	}
	U16 bp10;
	if (glbTableToMove != OBJECT_NULL) {
		if (true
			&& cd.pi.glbPlayerDir == _4976_4c40 && cd.pi.glbPlayerMap == glbMap_4976_4eaa && cd.pi.glbPlayerPosX == glbPosX_4976_4eae && cd.pi.glbPlayerPosY == glbPosY_4976_4eb0
			&& GET_CREATURE_AT(glbTargetPosXTest, glbTargetPosYTest) == glbTableToMove
		) {
			glbTableToMove = OBJECT_NULL;
			bp10 = cd.pi.glbPlayerMap;
			if (xx == 3) {
				_12b4_0d75_CREATURE(glbTargetPosXTest, glbTargetPosYTest, _4976_4eac, 0xfe);
			}
			if (PERFORM_MOVE(xx) != 0) {
				PERFORM_TURN_SQUAD(_4976_4eb2);
				if (xx != 3) {
					CHANGE_CURRENT_MAP_TO(bp10);
					_12b4_0d75_CREATURE(glbTargetPosXTest, glbTargetPosYTest, _4976_4eac, 0xfe);
				}
			}
			CHANGE_CURRENT_MAP_TO(cd.pi.glbPlayerMap);
		}
		glbTableToMove = OBJECT_NULL;
		if (glbTryPushPullObject == 0)
			glbTargetTypeMoveObject = 6;
		return 1;
	}
	bp10 = glbCurrentMapIndex;
	di = xx - 3;
	if (_4976_4c08 != 0 && di == 2) {
		CHANGE_CURRENT_MAP_TO(glbMap_4976_4c12);
	}
	playerDestPosX = glbSomePosX_4c2e;
	playerDestPosY = glbSomePosY_4c30;
	bp0e = _4976_4c2c;
	CALC_VECTOR_W_DIR(bp0e, _4976_19b2[RCJ(4,di)], _4976_19b6[RCJ(4,di)], &playerDestPosX, &playerDestPosY);
	iDestTile = GET_TILE_VALUE(playerDestPosX, playerDestPosY);	// bp0a
	cd.gg.glbRefreshViewport = 1;
	xChampion = glbChampionSquad;
	for (iChampionIndex = 0; iChampionIndex < cd.pi.glbChampionsCount; xChampion++, iChampionIndex++) {
		if (xChampion->curHP() != 0) {
			ADJUST_STAMINA(iChampionIndex, ((GET_PLAYER_WEIGHT(iChampionIndex) * 3) / MAX_LOAD(xChampion)) +1);
		}
	}
	RESET_SQUAD_DIR();
	X16 bp12;
	X16 bp24;
	i16 bp16;
	i16 bp18;
	U16 bp0c;
#if defined (XDMX_EXTENDED_FEATURES)
	//_4976_4c2c // move dir, not the player facing
	if (SkCodeParam::bBWMode == true) {
		int tblFaceDir[8] = {0, 1, 2, 3, 0, 1, 2, 3};
		int iInterfaceCellFrom = (di + cd.pi.glbPlayerDir)%4;
		int iInterfaceCellDest = (di + cd.pi.glbPlayerDir + 2)%4;
		U16 iInterwallFrom = 0;
		U16 iInterwallDest = 0;

		iInterwallFrom = GET_INTERWALL_VALUE(glbCurrentMapIndex, glbSomePosX_4c2e, glbSomePosY_4c30, iInterfaceCellFrom);
		iInterwallDest = GET_INTERWALL_VALUE(glbCurrentMapIndex, playerDestPosX, playerDestPosY, iInterfaceCellDest);
		iFinalInterwallValue = GET_INTERWALL_VALUE_MERGED(iInterwallFrom, iInterwallDest);
		printf("INTERWALL: D/T/F = %02d > %02d = %02d\n", iInterwallFrom, iInterwallDest, iFinalInterwallValue);
	}
#endif // XDMX_EXTENDED_FEATURES
	//switch (_12b4_0881_CHECK_MOVE_BETWEEN_TILES(di, iCurrentTile, iDestTile, playerDestPosX, playerDestPosY, &bp14)) {	// original
	switch (CHECK_MOVE_BETWEEN_TILES_AND_INTERWALLS(di, iFinalInterwallValue, iCurrentTile, iDestTile, playerDestPosX, playerDestPosY, &bp14)) {
	case C2_CHECKMOVE_TILE_STAIRS://^059A	// new tile is stairs
		MOVE_RECORD_TO(OBJECT_NULL, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, -1, 0);
		cd.pi.glbPlayerPosX = playerDestPosX;
		cd.pi.glbPlayerPosY = playerDestPosY;
		iCurrentTile = iDestTile;
		goto _05c2;
	case C1_CHECKMOVE_OLD_TILE_STAIRS://^05C2	// old tile is stairs
_05c2:
		//_12b4_00af(iCurrentTile & 4); // original
		_12b4_00af(iCurrentTile & 4, iCurrentTile & 8);
		break;
	case 4://^05D0	// blocked by some creature
		if (cd.pi.glbChampionsCount == 0)
			break;
		bp12 = QUERY_CREATURE_AI_SPEC_FLAGS(bp14) & 1;
		if (bp12 != 0 || GET_CREATURE_WEIGHT(bp14) == 0xFF) {
			_12b4_023f_WOUND_RUNNING_INTO_CREATURE(playerDestPosX, playerDestPosY, &bp16, &bp18, bp0e, di);
		}
		bp24 = ((bp0e +xx) -3) & 3;
		if (IS_CREATURE_MOVABLE_THERE(playerDestPosX, playerDestPosY, bp24, &bp14) != 0 && _12b4_099e(bp14) != 0) {
			_12b4_0d75_CREATURE(playerDestPosX, playerDestPosY, bp24, 0xfe);
			goto _0685;
		}
		if (SkCodeParam::bDebugNoCreatures) {
			ATTACK_CREATURE(OBJECT_NULL, playerDestPosX, playerDestPosY, 0x4005, 5, 9999);
			CREATURE_SET_NEW_COMMAND(bp14, playerDestPosX, playerDestPosY, ccm13_Destroy, 1);
		}
		if (bp12 != 0)
			break;	
		ATTACK_CREATURE(OBJECT_NULL, playerDestPosX, playerDestPosY, 0x4005, 5, 0);
		break;
	case C6_CHECKMOVE_TILE_PASSABLE://^0685	// any other case
_0685:
		if (_4976_4c08 != 0 && di == 2) {
			CHANGE_CURRENT_MAP_TO(bp10);
			playerDestPosX = glbSomePosX_4c2e;
			playerDestPosY = glbSomePosY_4c30;
			CALC_VECTOR_W_DIR(bp0e = _4976_4c2c, -1, 0, &playerDestPosX, &playerDestPosY);
		}
		if (iIsStairs != 0) {
			MOVE_RECORD_TO(OBJECT_NULL, -1, 0, playerDestPosX, playerDestPosY);
			goto _0768;
		}
		TELE_inf bp22;
		if (GET_TELEPORTER_DETAIL(&bp22, U8(playerDestPosX), U8(playerDestPosY)) != 0) {
			if (((bp22.b1 +2) & 3) != bp0e) {
				//^12B4:06FC
				CHANGE_CURRENT_MAP_TO(cd.pi.glbPlayerMap);
				MOVE_RECORD_TO(OBJECT_NULL, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, -1, 0);
				LOAD_NEWMAP(bp22.b4);
				MOVE_RECORD_TO(OBJECT_NULL, -1, 0, bp22.b2, bp22.b3);
				ROTATE_SQUAD(bp22.b1);
				goto _0768;
			}
		}
		MOVE_RECORD_TO(OBJECT_NULL, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, playerDestPosX, playerDestPosY);
_0768:
		_4976_4c00 = bp26;
		glbPlayerThrowCounter = 0;
		// SPX addition for external script when player arrives on tile
		LUA_CALL_SCRIPT(_EXP_SCRIPT__PLAYER_ON_TILE_, cd.pi.glbPlayerMap, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 0);
		// SPX re-implementation of music trigger from CD.DAT when player arrives on certain tile
		if (SkCodeParam::bDM2CDMusic && !SkCodeParam::bDM2V5Mode)
			CHECK_TRIGGER_MUSIC_FROM_PLAYER_POSITION(cd.pi.glbPlayerMap, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY);
	
		return 1;
	case C3_CHECKMOVE_TILE_BLOCKED://^0779	// tile is blocked
		if (cd.pi.glbChampionsCount == 0)
			break;
		_12b4_023f_WOUND_RUNNING_INTO_CREATURE(playerDestPosX, playerDestPosY, &bp16, &bp18, bp0e, di);
		if ((iDestTile >> 5) != ttDoor || (iDestTile & 7) != 4)
			break;
		bp0c = 0;
		if (bp16 != 0xFFFF) {
			xChampion = &glbChampionSquad[bp16];
			bp0c += STAMINA_ADJUSTED_ATTR(xChampion, GET_PLAYER_ABILITY(xChampion, abStr, 0) + (RAND() & 15));
		}
		if (bp18 != bp16 && bp18 != 0xFFFF) {
			xChampion = &glbChampionSquad[bp18];
			bp0c += STAMINA_ADJUSTED_ATTR(xChampion, GET_PLAYER_ABILITY(xChampion, abStr, 0) + (RAND() & 15));
		}
		ATTACK_DOOR(playerDestPosX, playerDestPosY, bp0c, 0, 0);
		break;
	case C5_CHECKMOVE_STOP://^0864
		break;
	}

	_1031_098e();
	cd.gg.glbRefreshViewport = 0;
	return 0;
}


//^1031:1E6A
// SPX: _1031_1e6a renamed MAIN_LOOP
void SkWinCore::MAIN_LOOP() //#DS=4976
{
	ENTER(0);
	if (glbEndCounter != 0)
		glbEndCounter--;
	_4976_4e48 = 0;

	do {
		MessageLoop(true); // get mouse change

		IBMIO_USER_INPUT_CHECK();
		if (glbUIClickEventIndex == glbUIClickEventLast)
			break;

		SkD((DLV_DBG_UI, "Check input %08X = %08X\n", glbUIClickEventIndex, glbUIClickEventLast));
		U16 iUIClickNext = glbUIClickEventIndex++; // si
		if (glbUIClickEventIndex > 2)
			glbUIClickEventIndex = 0;
		if (HANDLE_UI_EVENT(&tlbUIClickEvents[iUIClickNext]) != 0) {
			if (_4976_4e62 != 0)
				_4976_4e62--;
		}
		else {
			glbUIClickEventIndex = iUIClickNext;
		}
	} while (cd.gg.glbRefreshViewport == 0);
	return;
}

//^476D:04ED
U16 SkWinCore::_476d_04ed_DOES_NOTHING(U16 xx)
{
	ENTER(0);
	return 0;
}



//^2066:2CA8
U16 SkWinCore::LOAD_NEW_DUNGEON() 
{
	//^2066:2CA8
	ENTER(6);
	//^2066:2CAD
	X8 bp05 = 0;
	U8 *bp04 = ALLOC_MEMORY_RAM(0x400, afDefault, 0x80);

	Bit8u* sDungeonFilename = NULL;

	if (false) {
		//^2066:2CCB

		//^2066:2D0C
	}
	DEALLOC_UPPER_MEMORY(0x400);
	if (_4976_5bea == 0 || (glbDataFileHandle = OPEN_FILE(FORMAT_SKSTR(ptrDungenB, NULL))) < 0)
	{
		//U8 sGDATDungeonName[0x80];
		// Default is the DUNGEON.DAT filename
		sDungeonFilename = (Bit8u*) ptrDungeonFilename;

		if (skWinApp->sCustomDungeonDatFilename != NULL)
			sDungeonFilename = (Bit8u*) skWinApp->sCustomDungeonDatFilename;
		//--- SPX: Get name from GDAT itself
		//DIRECT_QUERY_GDAT_TEXT(0x03, 0x10, 6, sGDATDungeonName);
		//if (sGDATDungeonName[0] != 0)
		//	sDungeonFilename = sGDATDungeonName;

		glbDataFileHandle = OPEN_FILE(FORMAT_SKSTR(sDungeonFilename, NULL));
		//printf("Open file = %s\n ", sDungeonFilename);
		//printf("glbHandle = %d\n", glbDataFileHandle);
		//getch();
	}
	if (glbDataFileHandle < 0) 
	{
		//^2066:2D6C
		RAISE_SYSERR(SYSTEM_ERROR__MISSING_DUNGEON);
	}
	//^2066:2D74
	_4976_5bf6 = 0;
	cd.pi.glbChampionsCount = 0;
	cd.pi.glbLeaderHandPossession.object = OBJECT_NULL;
	_4976_524a = 0;
	return READ_DUNGEON_STRUCTURE(1);
}

//^2066:2CA8
// SPX: original LOAD_NEW_DUNGEON
U16 SkWinCore::ORIGINAL__LOAD_NEW_DUNGEON() 
{
	//^2066:2CA8
	ENTER(6);
	//^2066:2CAD
	X8 bp05 = 0;
	U8 *bp04 = ALLOC_MEMORY_RAM(0x400, afDefault, 0x80);
	if (false) {
		//^2066:2CCB

		//^2066:2D0C
	}
	DEALLOC_UPPER_MEMORY(0x400);
	if (_4976_5bea == 0 || (glbDataFileHandle = OPEN_FILE(FORMAT_SKSTR(ptrDungenB, NULL))) < 0) {
		//^2066:2D45
		glbDataFileHandle = OPEN_FILE(FORMAT_SKSTR(ptrDungeonFilename, NULL));
	}
	if (glbDataFileHandle < 0) {
		//^2066:2D6C
		RAISE_SYSERR(SYSTEM_ERROR__MISSING_DUNGEON);
	}
	//^2066:2D74
	_4976_5bf6 = 0;
	cd.pi.glbChampionsCount = 0;
	cd.pi.glbLeaderHandPossession.object = OBJECT_NULL;
	_4976_524a = 0;
	return READ_DUNGEON_STRUCTURE(1);
}

//^2066:32BB
i16 SkWinCore::SELECT_LOAD_GAME()
{
	//^2066:32BB
	ENTER(14);
	//^2066:32C1
	if (_4976_5bf2 != 0)
		return glbSKSaveNum;
	__OPEN_DIALOG_PANEL(0x80, 5);
	_2066_398a(-1);
	_0aaf_002f();
	FIRE_FADE_SCREEN(0);
	U16 bp06 = 0;
	i16 si = -1;
	_4976_4dfc = 255;
	do {
		MessageLoop(true); // game load

		while (IS_THERE_KEY_INPUT_2() != 0) {
			_1031_0d36_KEYBOARD(0x20, SPECIAL_UI_KEY_TRANSFORMATION());
		}
		//^2066:3317
		MAIN_LOOP();
		SRECT bp0e;
		i16 bp02;
		i16 bp04;
		U16 di;
		switch (_4976_4dfc) {
			case 1://^_3330
				//^2066:3330
				si = -1;
				goto _3333;
			case 2://^_3333
				//^2066:3333
_3333:
				bp06 = 1;
				break;
			case 3://^_333a
				//^2066:333A
				QUERY_EXPANDED_RECT(_4976_4df8, &bp0e);
				QUERY_TOPLEFT_OF_RECT(_4976_4e44, &bp02, &bp04);
				di = _4976_4e68;
				di -= bp0e.y +bp04;
				si = min_value(di / glbPanelStatsYDelta, 10);
				//^2066:3380
				if (_4976_5250[si].w40 == 0xDEAD && _4976_5250[si].w38 == 0xBEEF)
					si = -1;
				_2066_398a(si);
				_0aaf_002f();
			default://^_33aa
				break;
		}
		//^2066:33AA
		_4976_4dfc = 0xffff;
		//^2066:33B0
	} while (bp06 == 0);
	//^2066:33B9
	_2066_37f2();
	return si;
}


//^2066:1FAB
void SkWinCore::ARRANGE_DUNGEON()
{
	//^2066:1FAB
	//^2066:1FB1
	SkD((DLV_DBG_GAME_LOAD, "ARRANGE_DUNGEON\n"));
	for (U16 iMapIndex = 0; iMapIndex < dunHeader->nMaps; iMapIndex++) {
		//^2066:1FB6

		U16 bp2e = dunMapsHeaders[iMapIndex].RawColumn();
		U16 bp30 = dunMapsHeaders[iMapIndex].RawRow();
		//^2066:1FE6
		X8 *bp04 = *glbMapTileValue[iMapIndex];
		SkD((DLV_DBG_GAME_LOAD, "MAP %d/%d (C=%d, R=%d) TMAP=%08x\n", iMapIndex, dunHeader->nMaps, bp2e, bp30, bp04));
		//^2066:2001
		OID_T *bp1c = &dunGroundStacks[dunMapTilesObjectIndexPerColumn[dunMapColumnsSumArray[iMapIndex]]]; // U16 *bp1c
		//^2066:202B
		for (U16 bp2a=0; bp2a <= bp2e; bp2a++) {
			//^2066:2033
			for (U16 bp2c=0; bp2c <= bp30; bp2c++) {
				//^2066:203B
				X8 bp35 = *(bp04++);
				//^2066:2047
				if ((bp35 & 0x10) != 0) {
					//^2066:204E
					ObjectID di = *bp1c;
					bp1c++;
					//^2066:2058
					do {
						switch (di.DBType()) {
							case dbTeleporter:
								{
									//^2066:22D0
									Teleporter *bp0c = (Teleporter *)GET_ADDRESS_OF_RECORD(di);
									//^2066:22DD
									CHANGE_CURRENT_MAP_TO(bp0c->DestinationMap());
									//^2066:22EE
									ObjectID bp26 = GET_TILE_RECORD_LINK(bp0c->DestinationX(), bp0c->DestinationY());
									//^2066:230E
									if (bp26 == OBJECT_END_MARKER)
										break;
									//^2066:2316
									if (bp26.DBType() != dbTeleporter)
										break;
									//^2066:2327
									Teleporter *bp10 = GET_ADDRESS_OF_RECORD1(bp26);
									//^2066:2336
									if (bp10->DestinationX() != bp2a)
										break;
									//^2066:2348
									if (bp10->DestinationY() != bp2c)
										break;
									//^2066:2357
									if (bp10->DestinationMap() != iMapIndex)
										break;
									//^2066:2362
									U16 bp3c = 0;
									//^2066:2367
									U16 bp32 = _2066_1f37(bp26, bp0c->DestinationMap(), &bp3c);
									//^2066:2384
									bp32 |= _2066_1f37(di, iMapIndex, &bp3c);
									//^2066:2395
									if (bp3c == 2) {
										//^2066:239B
										bp10->w0 = bp0c->w0 = _2066_1ec9(GET_NEXT_RECORD_LINK(di), GET_NEXT_RECORD_LINK(bp26));
										break;
									}
									//^2066:23C1
									if (bp3c == 0 && bp32 != 0)
										goto _23de;
									break;
								}
							case dbText:
								{
									//^2066:2071
									Text *bp20 = GET_ADDRESS_OF_RECORD(di)->castToText();
									//^2066:207E
									if (bp20->TextMode() != 1)
										break;
									//^2066:2092
									switch (bp20->SimpleTextExtUsage()) {
										case 0x06: // Ladder enabled
										case 0x07: // Ladder disabled
											{
												//^2066:20B4
												U16 bp34 = di.Dir();
												//^2066:20BC
												SET_TILE_ATTRIBUTE_02(bp2a + glbXAxisDelta[bp34], bp2c + glbYAxisDelta[bp34], iMapIndex);
												break;
											}
										case 0x08: // Recharge
											{
												//^2066:20E1
												bp20->TextVisibility(1);
												break;
											}
									}
									break;
								}
							case dbActuator:
								{
									//^2066:20EB
									Actuator *bp08 = GET_ADDRESS_OF_RECORD(di)->castToActuator();
									//^2066:20F8
									U16 bp3e = bp08->ActuatorType();
									//^2066:2105
									switch (bp3e) {
										case ACTUATOR_TYPE_SIMPLE_LADDER: // 0x1C unused in retail ver.
										case ACTUATOR_TYPE_SHOP_PANEL: // 0x3F Shop panel
											//^2066:213E
											bp08->ActiveStatus(0);
											bp08->OnceOnlyActuator(1);
											break;
										case ACTUATOR_FLOOR_TYPE__CROSS_SCENE: // 0x27 Cross scene
										case ACTUATOR_FLOOR_TYPE__INFINITE_ORNATE_ANIMATOR: // 0x2C Continuous ornate animator
											//^2066:211E
											if (bp08->OnceOnlyActuator() != 1)
												break;
											//^2066:2133
											bp08->ActiveStatus(1);
											break;
										case ACTUATOR_TYPE_PUSH_BUTTON_WALL_SWITCH: // 0x46 Activator, seal-able push button wall switch
											//^2066:214E
											CHANGE_CURRENT_MAP_TO(iMapIndex);
											//^2066:2155
											if ((GET_TILE_VALUE(bp08->Xcoord(), bp08->Ycoord()) >> 5) != ttDoor)
												break;
											//^2066:2181
											//^2066:2192
											GET_ADDRESS_OF_TILE_RECORD(bp08->Xcoord(), bp08->Ycoord())->castToDoor()->DoorBit13B(bp08->OnceOnlyActuator() & 1);
											break;
									}
									break;
								}
							case dbCreature:
								{
									//^2066:21C3
									Creature *bp14 = GET_ADDRESS_OF_RECORD(di)->castToCreature();
									//^2066:21D0
									AIDefinition *bp18 = QUERY_CREATURE_AI_SPEC_FROM_TYPE(bp14->CreatureType());
									//^2066:21E4
									// SPX in non DM2 mode, creature HP is not defined by AIDef
									if (!SkCodeParam::bDM1Mode && !SkCodeParam::bBWMode)
										bp14->HP1(bp18->BaseHP);	// Otherwise, set HP from AIDef -- DM2 behaviour
									//^2066:21F2
									if (bp18->IsStaticObject() == 0) {
										//^2066:21FC
										bp14->HP3(0);
										//^2066:2205
										bp14->SetTriggerXPos(bp2a);
										//^2066:2214
										bp14->SetTriggerYPos(bp2c);
										//^2066:2227
										bp14->SetTriggerMap(iMapIndex);
										break;
									}
									//^2066:223B
									bp14->iAnimSeq = bp14->iAnimFrame = 0;
									//bp14->iAnimSeq = 0x01EB;	// SPX add	0x01EB = spawn animation sequence
									//bp14->iAnimFrame = 0x8001;	// SPX add => 1 frame
									if (SkCodeParam::bDebugNoCreatures) {
										bp14->HP1(0);
										bp14->iAnimSeq = 0x00D3;	// SPX: not sure about this
									}
									//^2066:2248
									CREATURE_SET_ANIM_FRAME(di);
									//^2066:224F
									if (bp14->b14_7_7() != 0)
										break;
									//^2066:2263
									U16 bp28 = bp14->possession;
									//^2066:226A
									if (bp28 == 0xfffe)
										break;
									//^2066:2272
									bp14->possession = (bp28 & 0x3fff) | (RAND02() << 14);
									//^2066:228A
									while (bp28 != 0xfffe) {
										//^2066:228C
										GenericRecord *bp24 = GET_ADDRESS_OF_RECORD(bp28);
										//^2066:229B
										if (bp24->w0 != 0xfffe) {
											//^2066:22A4
											bp24->w0.Dir(RAND02());
										}
										//^2066:22BB
										bp28 = GET_NEXT_RECORD_LINK(bp28);
										//^2066:22C7
									}
									break;
								}
						}
						//^2066:23CD
						di = GET_NEXT_RECORD_LINK(di);
					} while(di != OBJECT_END_MARKER);
				}
				//^2066:23DE
_23de:
				if ((bp35 >> 5) == ttPit) {
					//^2066:23EA
					// SPX: The item 0x6A is 1 only for VOID. It does not exist for other tilesets.
					// Beware! if this is not set to a map expecting it, it will crash the map loading!
					if (QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, dunMapsHeaders[iMapIndex].MapGraphicsStyle(), dtWordValue, GDAT_GFXSET_VOID_RANDOM_FALL) == 0) {	// 0x6A
						//^2066:2412
						i16 xx = bp2a; //i16 bp38 = bp2a;
						i16 yy = bp2c; //i16 bp3a = bp2c;
						U16 iLocatedMap = LOCATE_OTHER_LEVEL(iMapIndex, 1, &xx, &yy, NULL); // bp34 = ...
						//SPX: locate_other_level can return -1 (65535) and it will crash the next function
						if (SkCodeParam::bUseFixedMode && iLocatedMap != 65535) // Use fixed mode to prevent crash
							//^2066:243A
							//SET_TILE_ATTRIBUTE_02(bp38, bp3a, bp34);
							SET_TILE_ATTRIBUTE_02(xx, yy, iLocatedMap);
					}
				}
				//^2066:244A
				SkD((DLV_DBG_GAME_LOAD, "MAP %d/%d (CI=%d, RI=%d) TMAP=%08x\n", iMapIndex, dunHeader->nMaps, bp2a, bp2c, bp04));
			}
			//^2066:2458
			SkD((DLV_DBG_GAME_LOAD, "MAP %d/%d (CI=%d)\n", iMapIndex, dunHeader->nMaps, bp2a));
		}
		//^2066:2466
	}
	//exit(1);
}

//^2C1D:0250
i16 SkWinCore::GET_PLAYER_AT_POSITION(U16 position)
{
	Champion* xChampion = glbChampionSquad; // bp04
	for (U16 iChampionIndex = 0; iChampionIndex < cd.pi.glbChampionsCount; iChampionIndex++, xChampion++) {
		if (xChampion->playerPos() == position && xChampion->curHP() != 0) {
			return iChampionIndex;
		}
	}
	return -1;
}


//^2FCF:0CDF
// SPX: _2fcf_0cdf renamed TRY_PUSH_OBJECT_TO
U16 SkWinCore::TRY_PUSH_OBJECT_TO(ObjectID rl, i16 xpos, i16 ypos, i16 *xx, i16 *yy)
{
	//^2FCF:0CDF
	ENTER(12);
	//^2FCF:0CE5
	X16 si = RAND02();
	X16 bp0a;
	i8 *bp04 = (rl == OBJECT_NULL) ? (bp0a = cd.pi.glbPlayerDir, _4976_406c[si]) : (bp0a = 0, _4976_407c[si]);
	i16 di;
	for (di = 0; di <= 3; di++) {
		//^2FCF:0D1A
		si = (bp04[di] +bp0a) &3;
		X16 bp06 = xpos + glbXAxisDelta[si];
		X16 bp08 = ypos + glbYAxisDelta[si];
		si = GET_TILE_VALUE(bp06, bp08);
		if (IS_TILE_BLOCKED(U8(si)) == 0 && (si = (si >> 5)) != ttStairs) {
			if (rl.DBType() != dbCreature || (si != ttTeleporter && si != ttPit)) {
				//^2FCF:0D86
				ObjectID bp0c = GET_CREATURE_AT(bp06, bp08);
				if (bp0c == OBJECT_NULL || (rl == OBJECT_NULL && (QUERY_CREATURE_AI_SPEC_FLAGS(bp0c) & CREATURE_AI_FLAG_8000) != 0)) {
					//^2FCF:0DAF
					*xx = bp06;
					*yy = bp08;
					return 1;
				}
			}
		}
		//^2FCF:0DC6
	}
	//^2FCF:0DCF
	return 0;
}

//^2C1D:1C0C
X16 SkWinCore::ATTACK_PARTY(U16 quantity, U16 yy, U16 zz)
{
	//^2C1D:1C0C
	ENTER(4);
	//^2C1D:1C12
	X16 di = quantity;
	if (quantity == 0)
		return 0;
	//^2C1D:1C1D
	X16 si = 0;
	X16 bp04 = (di >> 3) +1;
	di -= bp04;
	bp04 <<= 1;
	U16 bp02;
	for (bp02 = 0; bp02 < cd.pi.glbChampionsCount; bp02++) {
		//^2C1D:1C34
		if (WOUND_PLAYER(bp02, max_value(1, di + RAND16(bp04)), yy, zz) != 0) {
			//^2C1D:1C60
			si |= 1 << bp02;
		}
		//^2C1D:1C6A
	}
	//^2C1D:1C76
	return si;
}




//^13E4:0401
void SkWinCore::ATTACK_CREATURE(ObjectID rl, i16 xx, i16 yy, U16 ss, i16 tt, U16 quantity)
{
	// quantity=0 to warn the creature. e.g. hazard notice of incoming missile.
	printf("ATTACK CREATURE @ %d,%d with %d\n", xx, yy, quantity);
	//^13E4:0401
	ENTER(20);
	//^13E4:0407
	U16 di = 0;
	if (rl == OBJECT_NULL) {
		rl = GET_CREATURE_AT(xx, yy);
		if (rl == OBJECT_NULL)
			return;
	}
	//^13E4:0427
	U16 bp12 = ss & 0x4000;
	if (bp12 != 0) {
		ss &= 0xbfff;
		if (RAND01() != 0)
			bp12 = 0;
	}
	//^13E4:0447
	U16 bp14 = ss & 0x2000;
	if (bp14 != 0)
		ss &= 0xdfff;
	//^13E4:0459
	Creature* xCreature = GET_ADDRESS_OF_RECORD4(rl); // bp04
	AIDefinition* xAIDef = QUERY_CREATURE_AI_SPEC_FROM_TYPE(xCreature->CreatureType()); // bp0c
	U16 bp10 = xAIDef->IsStaticObject();
	if (xCreature->InternalID() == 0xFF) {
		if (bp10 == 0)
			return;
		ALLOC_CAII_TO_CREATURE(rl, xx, yy);
	}
	//^13E4:04AC
	CreatureInfoData* xCreatureData = &glbTabCreaturesInfo[xCreature->InternalID()];	// bp08
	xCreatureData->w20 += quantity;
	quantity = xCreatureData->w20;
	//^13E4:04DB
	U16 si;	// si , new command
	U16 bp0e;
	if (bp10 == 0 && tt > 0) {
		//^13E4:04ED
		if ((xCreature->iAnimFrame & 4) == 0) {
			if (quantity <= 0x1E && (quantity <= 4 || RAND02() != 0) && (((quantity * U32(0x64)) / xAIDef->BaseHP) > 15)) {
				//^13E4:0537
				xCreature->iAnimFrame |= 4;
				bp12 = 1;
			}
		}
		//^13E4:0545
		if (bp12 != 0) {
			//^13E4:054E
			if ((tblAIStats01[QUERY_GDAT_CREATURE_WORD_VALUE(xCreature->CreatureType(), 1)] & 0x80) == 0 && RAND01() != 0) {
				//^13E4:057A
				si = CALC_VECTOR_DIR(xx, yy, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY);
				if ((xCreature->iAnimFrame & 8) != 0 && RAND02() != 0)
					goto _05bd;
				if ((xCreature->b15 & 3) != si && RAND02() == 0) {
					//^13E4:05BD
_05bd:
					si = (si +2) & 3;
				}
				//^13E4:05C6
				if (xCreature->b15_0_1() == ((si +2) & 3)) {
					si = (RAND01() != 0) ? 7 : 6;
				}
				//^13E4:05F1
				else if (xCreature->b15_0_1() != si) {
					si = (xCreature->b15_0_1() == ((si -1) & 3)) ? 7 : 6;
				}
				//^13E4:061A
				else if (RAND02() == 0) {
					si = (RAND01() != 0) ? 6 : 7;
				}
				else {
					//^13E4:0638
					si = 0xffff;
				}
				//^13E4:063B
				if (si != 0xffff) {
					CREATURE_SET_NEW_COMMAND(rl, xx, yy, U8(si), 0);
				}
			}
		}
		//^13E4:0653
		di = (RAND16(0x64) < tt) ? 1 : 0;
		if (di != 0) {
			//^13E4:066D
			bp0e = ss & 0x8000;
			ss = 1 << (ss & 0xff);
			if (bp0e != 0) {
				xCreature->iAnimFrame &= ~ss;
			}
			else {
				xCreature->iAnimFrame |= ss;
			}
		}
	}
	//^13E4:069F
	if (bp10 == 0 && bp14 != 0 && tt == 0) {
		//^13E4:06B1
		di = 1;
	}
	//^13E4:06B1
	else if (di != 0 && ((bp0e == 0 && ((ss & 0x40) != 0)) || bp14 != 0) && (tlbCreatureCommandsFlags[RCJ(MAX_CREATURE_COMMANDS,xCreatureData->Command)] & 0x10) == 0) {
		//^13E4:06DE
		di = 1;
	}
	//^13E4:06E3
	else {
		di = 0;
	}
	if (SkCodeParam::bDebugNoCreatures) {
		xCreatureData->Command = ccm13_Destroy;
		xCreature->hp1 = 0;
		QUEUE_THINK_CREATURE(xx, yy);
	}
	if (xCreatureData->Command != ccm13_Destroy && (di != 0 || xCreature->hp1 <= quantity)) {
		RELEASE_CREATURE_TIMER(rl);
		QUEUE_THINK_CREATURE(xx, yy);
	}
	return;
}

//^13E4:0004
U8 *SkWinCore::PREPARE_LOCAL_CREATURE_VAR(ObjectID rl, i16 xx, i16 yy, U16 timerType)
{
	SkD((DLV_DBG_CAI, "DBM: PREPARE_LOCAL_CREATURE_VAR(rl=%04X, z=%d, x=%d, y=%d, TiTy=%d)\n"
		, (Bitu)rl.w, (Bitu)glbCurrentMapIndex, (Bitu)xx, (Bitu)yy, (Bitu)timerType));

	//^13E4:0004
	ENTER(14);
	//^13E4:0008
	U8 *bp04 = NULL;
	i16 bp0e = glbCurrentMapIndex;
	glbCurrentMapIndex = -1;
	_1c9a_0648(bp0e);
	glbCreatureSomeX = 0xffff;
	if (_4976_5160 != 0) {
		// reach here when DRAGOTH creates DRAGOTH ATTACK MINION.

		bp04 = ALLOC_MEMORY_RAM(SIZE_LOCAL_CREATURE_VAR, afDefault, 1024); // 0x34e in DM2
		// TODO: fixme
		//ATLASSERT(false);
		COPY_MEMORY(&glbCurrentThinkingCreatureID, bp04, SIZE_LOCAL_CREATURE_VAR);
	}
	_4976_5160 = 1;
	_4976_5161 = 0;
	glbCurrentThinkingCreatureID = rl;
	//^13E4:0073
	Creature *xCreature = glbCurrentThinkingCreatureRec = GET_ADDRESS_OF_RECORD4(rl); // bp08
	CreatureInfoData *xCreatureInfo = (xCreature->InternalID() != 0xFF) ? &glbTabCreaturesInfo[xCreature->InternalID()] : NULL; // bp0c
#ifndef __DJGPP__
	if (xCreatureInfo != NULL)	// SPX : NULL should not happen
		SkD((DLV_DBG_CAI, "DBG: CreatureIndex:%d, CreatureType:%02X, 4eceIndex:%u, XX=%02X, YY=%02X\n"
			, (Bitu)rl.DBIndex()
			, (Bitu)xCreature->CreatureType()
			, (Bitu)xCreature->InternalID()
			, (Bitu)(U8)xCreatureInfo->x
			, (Bitu)(U8)xCreatureInfo->y
		));
	else
		SkD((DLV_DBG_CAI, "DBG: CreatureIndex:%d, CreatureType:%02X, 4eceIndex:%u, XX=NULL, YY=NULL\n"
			, (Bitu)rl.DBIndex()
			, (Bitu)xCreature->CreatureType()
			, (Bitu)xCreature->InternalID()
		));
#endif // __DJGPP__
	//^13E4:00BA
	glbCurrentThinkingCreatureData = xCreatureInfo;
	glbAIDef = QUERY_CREATURE_AI_SPEC_FROM_TYPE(xCreature->CreatureType());
	//^13E4:00DC
	glbCreatureAnimSeqInfo = GET_CREATURE_INFO_DATA(xCreature, glbAIDef);
	glbSomeMap_4976_4ee7 = U8(bp0e);
	glbCreatureTimer.SetMap(bp0e);
	glbCreatureTimer.SetTick(glbGameTick);
	//^13E4:0118
	glbCreatureTimer.actor = xCreature->CreatureType();
	glbCreatureTimer.TimerType(U8(timerType));
	glbCreatureTimer.XcoordB(U8(xx));
	glbCreatureTimer.YcoordB(U8(yy));
	glbCreatureAnimationFrame = NULL;
	glbCreatureActionProceeded = 0;
	glbCreatureAIStatIndex = 0xffff;
	//^13E4:014B
	if (xCreatureInfo == NULL && SkCodeParam::bDebugBypassNullPointers == false) {
		if (timerType == tty22) {
			glbCreatureCommand_4976_4ee8 = 0;
			_4976_4eea = 0;
			glbCreatureCommandThinking = (xCreatureInfo->Command == -1) ? 0 : xCreatureInfo->Command;
			if (xCreatureInfo != NULL) {
				//^13E4:017D
				ZERO_MEMORY(&xCreatureInfo->w24, 10);
				xCreatureInfo->Command = ccmFF_Idle;
			}
		}
	} // SPX: Debug bypass
	CHANGE_CONSOLE_COLOR(BRIGHT, (glbCurrentThinkingCreatureID.DBIndex()%6+2)+8, BLACK);
	if (MVALID(glbCurrentThinkingCreatureData))
	SkD((DLV_CCM, "Current creature CCM = %02x (%s) / %02X (%s)\n", glbCurrentThinkingCreatureData->Command, getCreatureCommandName(glbCurrentThinkingCreatureData->Command),
		glbCurrentThinkingCreatureData->Command2, getCreatureCommandName(glbCurrentThinkingCreatureData->Command2)));
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	//^13E4:019B
	return bp04;
}



//^075F:084D
U16 SkWinCore::ATTACK_WALL(i16 xTo, i16 yTo, i16 xFrm, i16 yFrm, U16 dirTo, ObjectID rlThrown)
{
	//^075F:084D
	ENTER(16);
	//^075F:0853
	ObjectID si = rlThrown;
	U16 bp0c = 0;
	dirTo += 2;
	dirTo &= 3;
	ObjectID di = GET_TILE_RECORD_LINK(xTo, yTo);
	Missile *bp08 = GET_ADDRESS_OF_RECORDE(si);
	for (; di != OBJECT_END_MARKER; di = GET_NEXT_RECORD_LINK(di)) {
		//^075F:0883
		if (di.Dir() != dirTo)
			continue;
		//^075F:0890
		if (true
			&& bp08->GetMissileObject().DBType() != dbCloud 
			&& bp0c == 0 
			&& _0cee_319e_ALCOVE_GET_GDAT_X13(di) != 0 
			&& RAND02() == 0
		) {
			//^075F:08BC
			CUT_RECORD_FROM(si, NULL, xFrm, yFrm);
			si.Dir(di.Dir());
			DELETE_MISSILE_RECORD(si, NULL, xTo, yTo);
			bp0c = 1;
			continue;
		}
		//^075F:08FB
		if (di.DBType() != dbActuator)
			continue;
		ObjectID bp0a = bp08->GetMissileObject();
		if (bp0a.DBType() == dbCloud)
			continue;
		//^075F:0923
		Actuator *bp04 = GET_ADDRESS_OF_ACTU(di);
		U16 bp10 = bp04->ActuatorData();
		U16 bp0e;
		switch (bp04->ActuatorType()) {
			case ACTUATOR_TYPE_FLYING_ITEM_CATCHER: // 0x22 -> Activator, knock sensor (flying item)
				//^075F:0957
				bp0e = (bp10 == 0x1ff || GET_DISTINCTIVE_ITEMTYPE(bp0a) == bp10) ? 1 : 0;
				if ((bp04->RevertEffect() ^ bp0e) == 0)
					continue;
				if (bp04->ActionType() == 3) {
					//^075F:099E
					INVOKE_ACTUATOR(bp04, ((bp04->RevertEffect() ^ bp0e) != 0) ? 0 : 1, 0);
				}
				else {
					//^075F:09BC
					INVOKE_ACTUATOR(bp04, bp04->ActionType(), 0);
				}
				//^075F:09DA
				if (bp0c != 0)
					continue;
				//^075F:09E3
				if (bp04->OnceOnlyActuator() == 0)
					continue;
				//^075F:09F7
				CUT_RECORD_FROM(si, NULL, xFrm, yFrm);
				si.Dir(dirTo);
				DELETE_MISSILE_RECORD(si, NULL, xTo, yTo);
				bp0c = 1;
				continue;
			case ACTUATOR_TYPE_FLYING_ITEM_TELEPORTER: // 0x23 -> Item teleporter (flying item)
				//^075F:0A34
				if (bp0c != 0)
					continue;
				if (bp10 != 0x1ff && GET_DISTINCTIVE_ITEMTYPE(bp0a) != bp10)
					continue;
				CUT_RECORD_FROM(si, NULL, xFrm, yFrm);
				si.Dir(
					((GET_TILE_VALUE(bp04->Xcoord(), bp04->Ycoord()) >> 5) == ttWall)
					? bp04->Direction()
					: bp04->ActionType()
					);
				//^075F:0AB7
				DELETE_MISSILE_RECORD(si, NULL, bp04->Xcoord(), bp04->Ycoord());
				bp0c = 1;
				break;
			default: // ^_0ae1
				break;
		}
		//^075F:0AE1
	}
	//^075F:0AF2
	return bp0c;
}


//^0CEE:32A3
i16 SkWinCore::QUERY_DOOR_DAMAGE_RESIST(U8 cls2)
{
	//^0CEE:32A3
	ENTER(0);
	//^0CEE:32A6
	return QUERY_GDAT_ENTRY_DATA_INDEX(0x0e, cls2, dtWordValue, 0x0e);
	//^0CEE:32B8
}

//^075F:2205
U16 SkWinCore::ATTACK_DOOR(i16 x, i16 y, U16 damage, U16 isSpellAttack, U16 delay)
{
	//^075F:2205
	ENTER(18);
	//^075F:2209
	Door *door = GET_ADDRESS_OF_TILE_RECORD(U8(x), U8(y))->castToDoor();	//*bp04
	// SPX: Disable door controls if "weak door" mode is activated. Controls within are original code
	if (!SkCodeParam::bWeakDoors) {
		if (isSpellAttack != 0 && door->DestroyablebyFireball() == 0)
			return 0;
		//^075F:2233
		if (isSpellAttack == 0 && door->BashablebyChopping() == 0)
			return 0;
		//^075F:224A
		if (QUERY_DOOR_DAMAGE_RESIST(GET_GRAPHICS_FOR_DOOR(door)) <= damage)
			return 0;
	} // End of "weak door"	
	//^075F:2266
	U8 *bp08 = &glbCurrentTileMap[x][y];
	if ((*bp08 & 7) != _DOOR_STATE__CLOSED_)	// ((*bp08 & 7) != 4) not closed
		return 0;
	//^075F:228E
	if (delay != 0) {
		//^075F:2294
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
		//^075F:22D9
		*bp08 = (*bp08 & 0xf8) | _DOOR_STATE__DESTROYED_; // (*bp08 = (*bp08 & 0xf8) | 5;) Destroyed or bashed
	}
	return 1;
}



//^0CEE:0E44
void SkWinCore::SET_ITEM_IMPORTANCE(ObjectID rlItem, X16 isImportant) 
{
	// for potion: important -> power visibility.
	// for creature: ?

	//^0CEE:0E44
	ENTER(4);
	//^0CEE:0E49
	X16 si = isImportant;
	GenericRecord *bp04 = GET_ADDRESS_OF_RECORD(rlItem);
	//^0CEE:0E5A
	switch (rlItem.DBType()) {
	case dbCreature: // 4://^0E73
		//^0CEE:0E73
		bp04->castToCreature()->b15_2_2(si & 1);
		break;
	case dbCloth: // 6://^0E89
		//^0CEE:0E89
		goto _0e8d;
	case dbWeapon: // 5://^0E8B
		//^0CEE:0E8B
		goto _0e8d;
	case dbMiscellaneous_item: // 10://^0E8D
		//^0CEE:0E8D
_0e8d:
		bp04->castToMisc()->Important(si & 1);
		break;
	case dbPotion: // 8://^0EA3
		//^0CEE:0EA3
		bp04->castToPotion()->VisiblePower(si & 1);
		break;
	case dbScroll: // 7://^0EB7
	case dbContainer: // 9://^0EB7
		break;
	}
	//^0CEE:0EB7
	return;
}


//^3A15:1DCA
void SkWinCore::MOVE_ITEM_TO(ObjectID rl, Actuator *pr4, i16 xposFrm, i16 yposFrm, i16 xposTo, i16 yposTo, X16 dir, X16 takeFrmCreature, X16 putToCreature) 
{
	ENTER(0);
	ObjectID si = rl;
	if (QUERY_CLS1_FROM_RECORD(si) != 0x15 || QUERY_CLS2_FROM_RECORD(si) != 0) {
		i16 di = 0;	// di	// SPX init this 0
		switch (pr4->ActionType()) {
			case 0: //^_1e05
				di = GET_MAX_CHARGE(si);
				break;
			case 1: //^_1e0d
				di = 0;
				break;
			case 2: //^_1e11
				di = RAND16(GET_MAX_CHARGE(si) + 1);
				break;
			default: //^_1e22 // di is still uninit. BUG?
				break;
		}
		ADD_ITEM_CHARGE(si, di);
	}
	if ((GET_TILE_VALUE(xposTo, yposTo) >> 5) != ttWall) {
		dir = RAND16(4);
	}
	if (xposFrm >= 0 && yposFrm >= 0) {
		MOVE_RECORD_TO(si, xposFrm, yposFrm, -1, (takeFrmCreature != 0) ? -1 : 0);
	}
	MOVE_RECORD_TO(ObjectID(si, dir), -1, (putToCreature != 0) ? -1 : 0, xposTo, yposTo);
	return;
}

// SPX: new function (or reimplementation)
void SkWinCore::CHECK_TRIGGER_MUSIC_FROM_PLAYER_POSITION(U16 iMapPosition, U16 iPositionX, U16 iPositionY)
{
	U16 iSelectedTrigger = -1;
	U8 iSelectedMusicID = -1;
	U16 iTriggerDefIndex = 0;
	//U16 iCurrentMapIndex = glbCurrentMapIndex;
	//CHANGE_CURRENT_MAP_TO(iMapPosition);
	Map_definitions* xCurrentMapDef = NULL;
	U16 iMapAltitude = 0;
	U16 iAbsolutePositionX = 0;
	U16 iAbsolutePositionY = 0;
	xCurrentMapDef = &dunMapsHeaders[iMapPosition];
	iAbsolutePositionX = xCurrentMapDef->MapOffsetX() + iPositionX;
	iAbsolutePositionY = xCurrentMapDef->MapOffsetY() + iPositionY;
	iMapAltitude = (U16) xCurrentMapDef->Level();
	//printf("position = (%d | %d, %d) => AG = (%d | %d, %d)\n", iMapPosition, iPositionX, iPositionY,
	//	iMapAltitude, iAbsolutePositionX, iAbsolutePositionY);
	
	if (SkCodeParam::bDM2CDMusic && CheckSafePointer(cd.sc.glbTabCDMusicTriggers)) {
		// Check if location is a music trigger point
		for (iTriggerDefIndex = 0; iTriggerDefIndex < 10; iTriggerDefIndex++) {
			if (cd.sc.glbTabCDMusicTriggers[iTriggerDefIndex].iZTriggerPosition == iMapAltitude
				&& cd.sc.glbTabCDMusicTriggers[iTriggerDefIndex].iXTriggerPosition == iAbsolutePositionX
				&& cd.sc.glbTabCDMusicTriggers[iTriggerDefIndex].iYTriggerPosition == iAbsolutePositionY) {
				iSelectedMusicID = cd.sc.glbTabCDMusicTriggers[iTriggerDefIndex].iCDMusicID;
				iSelectedTrigger = iTriggerDefIndex;
				break;
			}
		}
	}

	if (iSelectedTrigger != 0xFFFF)
		REQUEST_PLAY_MUSIC(iSelectedMusicID, 25);
	
	//CHANGE_CURRENT_MAP_TO(iCurrentMapIndex);
}