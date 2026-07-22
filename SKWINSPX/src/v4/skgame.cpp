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


//## Extended function : call external lua script on event
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




//^0CEE:26E5
U16 SkWinCore::IS_CONTAINER_MONEYBOX(ObjectID recordLink)
{
	if (recordLink.DBType() == dbContainer) {
		if (GET_ADDRESS_OF_RECORD9(recordLink)->ContainerType() == 0) {
			if (QUERY_GDAT_ENTRY_DATA_INDEX(0x14, QUERY_CLS2_FROM_RECORD(recordLink), 0x0005, 0x0040) != (U16)-1) {
				// Exists possible item list? (e.g. "J26-28 J8 J56 J6<00>")
				return 1;
			}
		}
	}
	return 0;
}

//^0CEE:25E5
U16 SkWinCore::QUERY_GDAT_DBSPEC_WORD_VALUE(ObjectID rl, Bit8u cls4)
{
	ObjectID si = rl;
	if (si == OBJECT_NULL)
		return 0;
	return QUERY_GDAT_ENTRY_DATA_INDEX(QUERY_CLS1_FROM_RECORD(si), QUERY_CLS2_FROM_RECORD(si), dtWordValue, cls4);
}

//^0CEE:2ACC
i32 SkWinCore::QUERY_ITEM_VALUE(ObjectID recordLink, Bit8u cls4)
{
	ObjectID si = recordLink;
	if (si == OBJECT_NULL) {
		return 0;
	}
	i32 bp08 = QUERY_GDAT_DBSPEC_WORD_VALUE(si, cls4);
	if (cls4 == 1) { // 0x01 -> Weight in Kg (x10)
		i32 bp0c = QUERY_GDAT_DBSPEC_WORD_VALUE(si, 0x34); // 0x34 -> Max charge count
		if (bp0c > 0) {
			bp08 += ADD_ITEM_CHARGE(si, 0) * bp0c;
		}
	}
	if (cls4 == 2) { // 0x02 -> Money value
		i32 bp0c = QUERY_GDAT_DBSPEC_WORD_VALUE(si, GDAT_ITEM_BONUS_MONEY_PER_CHARGE); // 0x35 -> Additional money value per charge??
		if (bp0c > 0) {
			bp08 += ADD_ITEM_CHARGE(si, 0) * bp0c;
		}
		if (si.DBType() == dbPotion && bp08 > 1) {
			bp08 /= 2;
			bp08 += (GET_ADDRESS_OF_RECORD(si)->castToPotion()->PotionPower() * bp08) / 255L;
		}
	}
	if (si.DBType() == dbContainer) {
		Container *bp04 = GET_ADDRESS_OF_RECORD(si)->castToContainer();
		if (bp04->ContainerType() == 0) {
			U16 di = IS_CONTAINER_MONEYBOX(si);
			ObjectID si = bp04->GetContainedObject();
			i32 bp0c = 0;
			for (; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
				if (di != 0) {
					if (si.DBType() == dbMiscellaneous_item) {
						bp0c += QUERY_GDAT_DBSPEC_WORD_VALUE(si, cls4) * (GET_ADDRESS_OF_RECORD(si)->castToMisc()->Charge() +1L);
						continue;
					}
				}
				bp08 += QUERY_ITEM_VALUE(si, cls4);
			}
			if (di != 0) {
				if (cls4 == 1) {
					bp08 += (bp0c +4L) / 5L;
				}
				else {
					bp08 += bp0c;
				}
			}
		}
	}
	return bp08;
}

//^0CEE:2CC2
i32 SkWinCore::QUERY_ITEM_WEIGHT(ObjectID recordLink)
{
	return QUERY_ITEM_VALUE(recordLink, 1);
}

//^0CEE:2734
U16 SkWinCore::IS_CONTAINER_CHEST(ObjectID recordLink)
{
	ObjectID si = recordLink;
	if (true
		&& si.DBType() == dbContainer
		&& IS_CONTAINER_MONEYBOX(si) == 0
		&& GET_ADDRESS_OF_RECORD9(si)->ContainerType() == 0
	) {
		return 1;
	}
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
	glbIndexOfTilesWithObjects = &dunMapTilesObjectIndexPerColumn[dunMapColumnsSumArray[iLocalMap]];
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
	
	if (bp16 >= 0 && bp16 < MAXDEPTH) {
		U16 bp14 = _4976_4cb0[bp16];
		if (bp14 != 0xffff) {
			Bit8u *bp08;
			if (ss != NULL && *ss != NULL) {
				bp08 = *ss + 1;
			}
			else {
				bp08 = &_4976_4c72[bp14];
			}

			i16 bp0e;
			for (; (bp0e = ((i8)*bp08)) != -1; bp08++) {
				bp04 = &dunMapsHeaders[bp0e];
				i16 di = bp04->MapOffsetX();
				if (di -1 <= bp0a) {
					if (di + bp04->RawColumn() + 1 >= bp0a) {
						i16 si = bp04->MapOffsetY();
						if (si -1 <= bp0c) {
							if (si + bp04->RawRow() >= bp0c) {
								di = bp0a - di;
								si = bp0c - si;
								U16 bp12 = glbCurrentMapIndex;
								CHANGE_CURRENT_MAP_TO(bp0e);
								U16 bp10 = GET_TILE_VALUE(di, si) >> 5;
								if (bp10 == ttTeleporter) {
									if (GET_ADDRESS_OF_TILE_RECORD((Bit8u)di, (Bit8u)si)->castToTeleporter()->w4_0_0() != 0) {
										bp10 = 7;
									}
								}
								CHANGE_CURRENT_MAP_TO(bp12);
								if (bp10 != ttMapExit) {
									*xx = di;
									*yy = si;
									if (ss != NULL) {
										*ss = bp08;
									}
									return bp0e;
								}
							}
						}
					}
				}
			}
		}
	}
	if (ss != NULL) {
		*ss = 0;
	}
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
	ENTER(28);
	if (dunMapsHeaders[cd.pi.glbPlayerMap].Difficulty() == 0) {
		glbLightLevel = 1;
		if (SkCodeParam::bAutoDefaultMaxLight)
			glbLightLevel = 0;
	}
	else {
		U16 itemLightBonus[9];		// (bp1c) SPX: light bonus for holded item (1) + items in hand (2*4)
		ZERO_MEMORY(itemLightBonus, 9);
		U16 bonusIndex = 0;	// bp06
		// Give light bonus for item currently in hand (if any)
		if ((QUERY_GDAT_DBSPEC_WORD_VALUE(cd.pi.glbLeaderHandPossession.object, GDAT_ITEM_STATS_GEN_FLAGS) & ITEM_FLAG_PRODUCE_LIGHT) != 0) {
			itemLightBonus[bonusIndex] = ADD_ITEM_CHARGE(cd.pi.glbLeaderHandPossession.object, 0);
			bonusIndex++;
		}
		U16 si;
		// Pass through all items in hands
		for (si=0; si < cd.pi.glbChampionsCount; si++) {
			for (U16 bp04=0; bp04 <= 1; bp04++) {
				U16 bp08;
				if ((QUERY_GDAT_DBSPEC_WORD_VALUE(bp08 = tblChampionSquad[si].Possess(bp04), GDAT_ITEM_STATS_GEN_FLAGS) & ITEM_FLAG_PRODUCE_LIGHT) != 0) {
					itemLightBonus[bonusIndex] = ADD_ITEM_CHARGE(bp08, 0);
					bonusIndex++;
				}
			}
		}
		//for (si=0; bonusIndex -1 > si; si++) {
		// One pass of bubble sort, what for ?
		for (si=0; si < bonusIndex -1; si++) {
			if (itemLightBonus[si] > itemLightBonus[si +1]) {
				U16 bp04 = itemLightBonus[si];
				itemLightBonus[si] = itemLightBonus[si +1];
				itemLightBonus[si +1] = bp04;
			}
		}
		U16 bp02 = 6;
		i16 di = 0;	// SPX (2016-11-02) replacing unsigned by signed to hold negative light (with use of darkness) and hit minimum threshold
		// SPX: This is strange ... does that mean that each new value to add is decreased by its position on the light bonus table?
		for (di=0, si=0; si < bonusIndex; si++) {
			di += (tLightLevelItem[RCJ(16,itemLightBonus[si])] << bp02) >> 6;
			bp02 = max_value(0, bp02 -1);
		}
		di += glbPrecomputedLight;	// SPX: is some sort of precalculed light level (depending on door and such?)
		di += glbGlobalSpellEffects.Light;
		di = di + QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x08_GRAPHICSSET, glbMapGraphicsSet, dtWordValue, GDAT_GFXSET_x67_AMBIANT_LIGHT);
		if (glbRainFlagSomething != 0) {
			di += tLightLevelTable[RCJ(6,BETWEEN_VALUE(0, glbRainAmbientLightModifier + glbRainSomeLightMod, 5))];
		}
		if (SkCodeParam::bUseFixedMode && di < 0)
			di = 0;
		// Light threshold is at 100 for the brighter light => lightlevel = 5.
		for (glbLightLevel=0; glbLightLevel <= 5; glbLightLevel++) {
			if (tLightLevelTable[RCJ(6,glbLightLevel)] < di)
				break;
		}
		// SPX: Get the highest default light (from wallset type)
		i16 bp0a = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x08_GRAPHICSSET, glbMapGraphicsSet, dtWordValue, GDAT_GFXSET_x68_HIGHEST_LIGHT_LEVEL);
		if (SkCodeParam::bAutoDefaultMaxLight)
			bp0a = 0;
		if (glbLightLevel < bp0a) {
			glbLightLevel = bp0a;
		}
		if (glbRainFlagSomething != 0) {
			if (glbRecalcLightIndicator != 0) {
				glbLightLevel = 0;
			}
		}
	}
	glbLightLevel -= (glbLightModifier > 12) ? 1 : 0;
	glbLightLevel = BETWEEN_VALUE(0, glbLightLevel, 5);	// SPX: minimum is 5 ? but it can be even darker with 6
	if (SkCodeParam::bFullLight)	// SPX: debug feature added to always get full light
		glbLightLevel = 0;
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
	ENTER(6);
	if (xx == 0)
		return;
	cd.gg.glbRefreshViewport = 1;
	RESET_SQUAD_DIR();
	U16 si = GET_TILE_VALUE(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY);
	if (si >> 5 == ttStairs) {
		// _12b4_00af(si & 4); // original
		_12b4_00af(si & 4, si & 8);	// SPX TODO: modified => may use a define/param to control original behaviour vs custom
		return;
	}
	TELE_inf bp06;
	if (GET_TELEPORTER_DETAIL(&bp06, U8(cd.pi.glbPlayerPosX), U8(cd.pi.glbPlayerPosY)) != 0) {
		CHANGE_CURRENT_MAP_TO(cd.pi.glbPlayerMap);
		MOVE_RECORD_TO(OBJECT_NULL, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, -1, 0);
		LOAD_NEWMAP(bp06.b4);
		MOVE_RECORD_TO(OBJECT_NULL, -1, 0, bp06.b2, bp06.b3);
		ROTATE_SQUAD(bp06.b1);
		return;
	}
	PLACE_OR_REMOVE_OBJECT_IN_ROOM(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, OBJECT_NULL, 1, FCT_REMOVE_OFF, 0);
	ROTATE_SQUAD((cd.pi.glbPlayerDir + ((xx == 2) ? 1 : 3)) & 3);
	PLACE_OR_REMOVE_OBJECT_IN_ROOM(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, OBJECT_NULL, 1, FCT_PLACE_ON, 0);
	LUA_CALL_SCRIPT(_EXP_SCRIPT__PLAYER_TURN_ON_TILE_, cd.pi.glbPlayerMap, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 0);
	return;
}


//^2405:00E7
// SPX: _2405_00e7 renamed DO_NOTHING_FROM_REMOVE_HAND
// Check DM2-V5 ?
void SkWinCore::DO_NOTHING_FROM_REMOVE_HAND()
{
	ENTER(0);
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

	ENTER(14);
	ObjectID di = rlItemThrown;
	ObjectID si = ALLOC_NEW_RECORD(dbMissile);
	if (si == OBJECT_NULL) {
		if (di.DBType() != dbCloud) {
			MOVE_RECORD_TO(ObjectID(di, dir), -1, 0, xx, yy);
		}
		return;
	}
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
	si = ObjectID(si, dir);
	Missile *missile = GET_ADDRESS_OF_RECORDE(si);	//*bp04
	missile->SetMissileObject(di);
	missile->EnergyRemaining(U8(min_value(energyVal, 255)));
	missile->EnergyRemaining2(U8(ene2Val));
	APPEND_RECORD_TO(si, NULL, xx, yy);
	if (missile->GetMissileObject() == OBJECT_EFFECT_FIREBALL && glbCurrentMapIndex == cd.pi.glbPlayerMap)	// oFF80
		glbDoLightCheck = 1;
	Timer bp0e;
	bp0e.SetMap(glbCurrentMapIndex);
	bp0e.SetTick(glbGameTick +1);
	bp0e.TimerType((_4976_4b7e != 0) ? tty1E : tty1D);
	bp0e.actor = TIMER_ACTOR__00;
	bp0e.value = si;
	bp0e.w8_0_4(xx);
	bp0e.w8_5_9(yy);
	bp0e.w8_c_f(dd);
	bp0e.w8_a_b(aa);
	missile->TimerIndex(QUEUE_TIMER(&bp0e));
	return;
}





//^48AE:0001
U16 SkWinCore::GET_GLOB_VAR(U16 iGlobalVar)
{
	// 0x00-0x3f: 1bit value
	// 0x40-0x7f: byte value
	// 0x80-0xbf: word value

	ENTER(0);
	if (iGlobalVar <= 0x3F) {
		if ((glbIngameGlobVarFlags[iGlobalVar >> 3] & (1 << (iGlobalVar & 7))) == 0)
			return 0;
		return 1;
	}
	if (iGlobalVar <= 0x7F) {
		return glbIngameGlobVarBytes[iGlobalVar - 0x40];
	}
	if (iGlobalVar <= 0xBF) {
		return glbIngameGlobVarWords[iGlobalVar - 0x80];
	}
	return 0;
}

//^48AE:0058
U16 SkWinCore::UPDATE_GLOB_VAR(U16 iGlobalVar, U16 iOpValue, U16 iOperator)
{
	// op == 0: var = 1
	// op == 1: var = 0
	// op == 2: var = !var
	// op == 3: var += newval
	// op == 4: var -= newval
	// op == 6: var = newval

	ENTER(0);
	U16 iSetValue = iOpValue;	// di
	U16 iCurrentValue = GET_GLOB_VAR(iGlobalVar);	// si
	switch (iOperator) {
		case C00_GLOBAL_VAR_OP_FLAG_SET:
			iCurrentValue = 1;
			break;

		case C01_GLOBAL_VAR_OP_FLAG_CLEAR:
			iCurrentValue = 0;
			break;

		case C02_GLOBAL_VAR_OP_FLAG_TOGGLE:
			iCurrentValue = (iCurrentValue != 0) ? 0 : 1;
			break;

		case C03_GLOBAL_VAR_OP_ADD:
			iCurrentValue += iSetValue;
			break;

		case C04_GLOBAL_VAR_OP_SUBTRACT:
			iCurrentValue -= iSetValue;
			break;

		case C05_GLOBAL_VAR_OP_NONE:
			break;

		case C06_GLOBAL_VAR_OP_NEW_VALUE:
			iCurrentValue = iSetValue;
			break;
	}
	// var x00 - x3F => FLAGS 0 to 8 (in fact var = #FLAG*8 ...)
	if (iGlobalVar <= 0x3f) {
		iOperator = 1 << (iGlobalVar & 7);	// bit flag
		iGlobalVar >>= 3;
		if (iCurrentValue != 0) {
			iCurrentValue = 1;
			glbIngameGlobVarFlags[iGlobalVar] |= U8(iOperator);
			return iCurrentValue;
		}
		else {
			glbIngameGlobVarFlags[iGlobalVar] &= ~U8(iOperator);
			return iCurrentValue;
		}
	}
	// var x40 - x7F => BYTE 00 to 64
	if (iGlobalVar <= 0x7F) {
		iCurrentValue = BETWEEN_VALUE(0, iCurrentValue, 255);
		glbIngameGlobVarBytes[iGlobalVar - 0x40] = U8(iCurrentValue);
		return iCurrentValue;
	}
	// var x80 - xbf => WORD 00 to 64
	if (iGlobalVar <= 0xBF) {
		glbIngameGlobVarWords[iGlobalVar - 0x80] = iCurrentValue;
	}
	return iCurrentValue;
}


//^29EE:0B4A
// SPX: _29ee_0b4a renamed PROCEED_SPELL_FAILURE
void SkWinCore::PROCEED_SPELL_FAILURE(U16 xx)
{
	ENTER(0);

	U16 di = 1;
	U16 iGlobalVariable;	// si
	switch (xx & 0xfff0) {
		case 0x0010:
			if ((xx & 15) == 3) {
				glbChampionAttackDamage = ATTACK_FAILURE_WIZARD;	// -4
			}
			else {
				glbChampionAttackDamage = ATTACK_FAILURE_PRIEST;	// -5
			}
			iGlobalVariable = C069_GLOB_BYTE_05_CHAMPION_FAIL_SPELL;
			goto _0baa;

		case 0x0020:
			glbChampionAttackDamage = ATTACK_MEANINGLESS_SPELL;	// -3
			iGlobalVariable = C070_GLOB_BYTE_06_CHAMPION_UNKNOWN_SPELL;
			goto _0baa;

		case 0x0030:	// need flask in hand 
			//DRAW_TRANSPARENT_STATIC_PIC(0x01, 0x05, 0x0B, 0x5c, -1);
			DRAW_TRANSPARENT_STATIC_PIC(GDAT_CATEGORY_x01_INTERFACE_GENERAL, GDAT_INTERFACE_CLASS_SPELLING, GDAT_IMG_NEED_FLASK, 0x5C, -1);
			iGlobalVariable = C068_GLOB_BYTE_04_CHAMPION_NEED_FLASK;
			di = 0x03;
_0baa:
			glbSomeChampionPanelFlag = max_value(di, 8 - UPDATE_GLOB_VAR(iGlobalVariable, 1, C03_GLOBAL_VAR_OP_ADD));

			break;
	}
	return;
}


//^2759:274D
U16 SkWinCore::TRY_CAST_SPELL()
{
	ENTER(10);
	U16 iChampionIndex = cd.pi.glbChampionIndex - 1;	// bp0a
	Champion* xChampion = &tblChampionSquad[iChampionIndex];	// bp04
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
	ENTER(8);
	Champion *bp04 = &tblChampionSquad[xx];
	i16 si = (RAND() & 15) + GET_PLAYER_ABILITY(bp04, abStr, 0);
	ObjectID bp08 = bp04->Possess(yy);
	U16 bp06 = QUERY_ITEM_WEIGHT(bp08);
	U16 di = MAX_LOAD(bp04) >> 4;
	si = (bp06 -12) +si;
	if (bp06 > di) {
		si = si - ((bp06 -di) >> 1);
		di += (di -12) >> 1;
		if (di < bp06) {
			si = si -((bp06 -di) << 1);
		}
	}
	if (zz >= 0) {
		si = (QUERY_PLAYER_SKILL_LV(xx, zz, 1) << 1) +si;
		di = 0;
		switch (zz) {
			case  0:	// fighter 0	(global)
			case  4:	// fighter 1	swing
			case  5:	// fighter 2	thrust
			case  6:	// fighter 3	club
			case  7:	// fighter 4	parry
			case  9:	// ninja 2		fight
				di = QUERY_GDAT_DBSPEC_WORD_VALUE(bp08, GDAT_ITEM_WEAPON_MELEE_STRENGTH);
				break;

			case  1:	// ninja 0		(global)
			case 10:	// ninja 3		throw
			case 11:	// ninja 4		shoot
				di = QUERY_GDAT_DBSPEC_WORD_VALUE(bp08, GDAT_ITEM_WEAPON_THROW_STRENGTH);
				if (di == 0)
					break;
				// SPX: x8000 = is a weapon that shoots items
				if ((QUERY_GDAT_DBSPEC_WORD_VALUE(bp08, GDAT_ITEM_WEAPON_PROJECTILE_FLAG) & 0x8000) != 0) {
					if (zz != 11)
						di = 0;
					break;
				}
				if (zz == 11)
					di = 0;

				break;

			case  2:	// priest 0		(global)
			case  3:	// wizard 0		(global)
			case  8:	// ninja 1		steal

				break;
		}
		si = si + di;
	}
	si = STAMINA_ADJUSTED_ATTR(bp04, si);
	if ((bp04->bodyFlag & ((yy == 0) ? 1 : 2)) != 0)
		si >>= 1;
	return BETWEEN_VALUE(0, si >> 1, 100);
}



//^2FCF:183C
void SkWinCore::TRANSFER_PLAYER(i16 xx, i16 yy, U16 zz, U16 dir)
{
	SkD((DLV_DBM, "DBM: TRANSFER_PLAYER(%2d,%2d,%2d,%d)\n"
		, (Bitu)xx, (Bitu)yy, (Bitu)zz, (Bitu)dir));

	ENTER(0);
	i16 di = yy;
	U16 si = (zz != cd.pi.glbPlayerMap) ? 1 : 0;
	if (si != 0)
		CHANGE_CURRENT_MAP_TO(zz);
	if (xx >= 0 && xx < glbCurrentMapWidth && di >= 0 && di < glbCurrentMapHeight) {
		if (si != 0) {
			CHANGE_CURRENT_MAP_TO(cd.pi.glbPlayerMap);
			MOVE_RECORD_TO(OBJECT_NULL, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, -1, 0);
			LOAD_NEWMAP(U8(zz));
			MOVE_RECORD_TO(OBJECT_NULL, -1, 0, xx, di);
		}
		else {
			MOVE_RECORD_TO(OBJECT_NULL, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, xx, di);
		}
		ROTATE_SQUAD(dir);
		return;
	}
	if (si != 0)
		CHANGE_CURRENT_MAP_TO(cd.pi.glbPlayerMap);
	return;
}



//^2FCF:18E3
U16 SkWinCore::ENGAGE_X_TELEPORTER()
{
	// activates the X mark floor teleporter at squad's position.

	// return 0 if failed.
	// return 1 if succeeded.

	ENTER(10);
	ObjectID si = GET_TILE_RECORD_LINK(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY);
	U16 bp06;
	for (; si != OBJECT_END_MARKER && (bp06 = si.DBType()) <= dbActuator; si = GET_NEXT_RECORD_LINK(si)) {
		if (bp06 == dbText) {
			Text *bp04 = GET_ADDRESS_OF_RECORD2(si);
			if (bp04->TextMode() == 1) {
				U16 di;
				switch (bp04->SimpleTextExtUsage()) {
					case 16: // 16 -> Teleporter with flag
						di = SDFSM_CMD_X_ANCHOR;	// 5
						goto _194a;

					case 15: // 15 -> Teleporter
						di = SDFSM_CMD_X_TELEPORTER;	// 4
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
							// SPX: There is BUG here, bp06 is the db type instead of required map
//								TRANSFER_PLAYER(bp08, bp0a, bp06, glbPlayerDir);
							TRANSFER_PLAYER(bp08, bp0a, destMap, cd.pi.glbPlayerDir);
							// SPX: This is used by the special teleporter ground cross		
							QUEUE_NOISE_GEN2(GDAT_CATEGORY_x0A_FLOOR_GFX, bp04->TextIndex() & 0xff, SOUND_STD_ACTIVATION, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1, 0x8c, 0x80);
							if (di == SDFSM_CMD_X_TELEPORTER) {	// di == 4
								bp04->TextVisibility(1);
							}
							return 1;
						}

						break;
				}
			}
		}
	}
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

	ENTER(84);
	U16 di = player;
	U16 bp4a = 0;
	U16 bp48 = cmdSlot & 0x8000;
	cmdSlot &= 0x7fff;
	glbItemGDATCategory = glbItemSelected[cmdSlot].category;
	glbItemGDATIndex = glbItemSelected[cmdSlot].index;
	glbItemGDATEntry = glbItemSelected[cmdSlot].entry;
	U16 bp34 = glbSelectedHandAction;
	U16 bp3a = bp34 ^ 1;
	Champion *champion = &tblChampionSquad[di];	//*bp04
	if (champion->curHP() == 0)
		return 0;
	U16 bp1c = QUERY_CUR_CMDSTR_ENTRY(CnCM_Command);		// Command
	U8 cmdSound = U8(QUERY_CUR_CMDSTR_ENTRY(CnSD_Sound));		// bp36 Sound
	U16 bp38 = 0;
	champion->handCommand[bp34] = U8(bp1c);
	Container *bp08 = NULL;
	ObjectID bp2e = champion->Possess(bp34);
	if (bp2e != OBJECT_NULL) {
		bp08 = GET_ADDRESS_OF_RECORD9(bp2e);
	}
	U16 bp22 = champion->playerDir();
	U16 bp1e = cd.pi.glbPlayerPosX + glbXAxisDelta[bp22];
	U16 bp20 = cd.pi.glbPlayerPosY + glbYAxisDelta[bp22];
	glbObjectID_4976_534c = GET_CREATURE_AT(bp1e, bp20);
	U8 bp35;
	AIDefinition *bp1a;
	if (glbObjectID_4976_534c != OBJECT_NULL) {
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
	U16 bp0e = 1;
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
			si = si + QUERY_PLAYER_SKILL_LV(di, SKILL_PRIEST_INFLUENCE, 1);
			bp0e = CONFUSE_CREATURE(si, bp1e, bp20);
			if (bp0e == 0)
				bp2c >>= 2;
			bp38 = 0;
			break;

		case CmInvisibility: // 2
			si = max_value(32, si);
			bp50 = 0x47;
			if (glbGlobalSpellEffects.Invisibility++ == 0 && glbChampionInventory != 0) {
				glbChampionTable[glbChampionInventory].heroFlag |= CHAMPION_FLAG_4000;	// 0x4000
			}
			Timer bp54;
			bp54.SetMap(cd.pi.glbPlayerMap);
			bp54.SetTick(glbGameTick +si);
			QUEUE_TIMER(&bp54);
			break;

		case CmLaunchMissile: // 3 magical missile
			bp44 = ObjectID::MissileRecord(QUERY_CUR_CMDSTR_ENTRY(CnPA_Parameter));
			//bp42 = 7 - min_value(6, QUERY_PLAYER_SKILL_LV(di, bp2a, 1));
			bp42 = 7 - min_value(SkLvlCraftsman, QUERY_PLAYER_SKILL_LV(di, bp2a, 1));
			if (champion->curMP() < bp42) {
				si = max_value(2, (champion->curMP() * si) / bp42);
				bp42 = champion->curMP();
			}
			bp0e = CAST_CHAMPION_MISSILE_SPELL(di, bp44, si, bp42);
			if (bp0e == 0)
				bp2c >>= 1;
			break;

		case CmPhysicalDamage4: // 4
		case CmPhysicalDamage8: // 8
			if ((bp24 >> 5) == ttDoor && (bp24 & 7) == 4) {
				ATTACK_DOOR(
					bp1e,
					bp20,
					COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(di, bp34, bp2a), 
					0, 
					2
					);

				bp26 = 6;
				Door *bp12 = GET_ADDRESS_OF_TILE_RECORD(U8(bp1e), U8(bp20))->castToDoor();
				// SPX: Sound made by the door when hit?
				QUEUE_NOISE_GEN2(
					GDAT_CATEGORY_x0E_DOORS,
					GET_GRAPHICS_FOR_DOOR(bp12),
					SOUND_OBJECT_GETHIT,
					0xfe,
					bp1e,
					bp20,
					bp38 +1,
					0x3c,
					0xc8
					);
				break;
			}
			if (true
				&& glbObjectID_4976_534c != OBJECT_NULL 
				&& bp1a->IsStaticObject() != 0 
				&& bp1a->ArmorClass == AI_DEF_ARMOR_MAX 
				&& bp1a->Defense == 255
			) {
				bp2c >>= 1;
			}
			else {
				bp44 = QUERY_CUR_CMDSTR_ENTRY(CnPA_Parameter);		// Spell missile
				bp0e = WIELD_WEAPON(di, bp44, bp1e, bp20, bp2a, iAttackType);
				if (bp0e == 0) {
					bp2c >>= 1;
					bp26 >>= 1;
					break;
				}
				if (glbObjectID_4976_534c == OBJECT_NULL)
					break;
			}
			QUEUE_NOISE_GEN1(GDAT_CATEGORY_x0F_CREATURES, bp35, SOUND_OBJECT_GETHIT, 0x3c, 0xc8, bp1e, bp20, bp38 +1);
			break;

		case CmSpellReflection: // 7
			CREATE_CLOUD(OBJECT_EFFECT_REFLECTOR, max_value(2, si), cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 255);
			break;

		case CmAuraOfSpeed: // 9
			si = max_value(32, si);
			glbGlobalSpellEffects.AuraOfSpeed = U8(min_value(glbGlobalSpellEffects.AuraOfSpeed +si, 255));
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
			if (IS_MISSILE_VALID_TO_LAUNCHER(di, bp34, champion->Possess(bp3a)) == 0) {
				if (bp48 == 0) {
					glbSomeChampionPanelFlag = max_value(1, 8 - UPDATE_GLOB_VAR(C067_GLOB_BYTE_03_CHAMPION_NEED_AMMO, 1, C03_GLOBAL_VAR_OP_ADD));
					glbChampionAttackDamage = ATTACK_REQUIRES_HAND_ITEM;	// -2
					glbWeaponShooterNum = QUERY_GDAT_DBSPEC_WORD_VALUE(bp2e, GDAT_ITEM_WEAPON_PROJECTILE_FLAG);	// 5
					glbWeaponMissileHand = bp3a;
					bp2c = 0;
				}
				bp0e = 0;
				break;
			}
			bp30 = REMOVE_POSSESSION(di, bp3a);
			bp0a = QUERY_PLAYER_SKILL_LV(di, SKILL_NINJA_SHOOT, 1);
			// SPX: bp2e is the SHOOTER/LAUNCHER, bp30 is the PROJECTILE
			// param3 = energy (max 255) / param4 = energy2 / param5 = dd
			SHOOT_CHAMPION_MISSILE(
				champion,
				bp30,
				QUERY_GDAT_DBSPEC_WORD_VALUE(bp2e, GDAT_ITEM_WEAPON_ACCURACY) + QUERY_GDAT_DBSPEC_WORD_VALUE(bp30, GDAT_ITEM_WEAPON_THROW_STRENGTH) + bp0a,
				(bp0a << 1) + QUERY_GDAT_DBSPEC_WORD_VALUE(bp30, GDAT_ITEM_STATS_0x0C),	// 12
				QUERY_GDAT_DBSPEC_WORD_VALUE(bp30, GDAT_ITEM_STATS_0x0C)
				);
			bp4a = 1;
			break;

		case CmSpellshield: // 33
		case CmFireshield: // 34
		case CmShield: // 35
			U16 enchantmentType;
//				bp0c = (bp1c == CmSpellshield) ? 1 : ((bp1c == CmFireshield) ? 0 : 2);
			enchantmentType = (bp1c == CmSpellshield) ? C01_ENCHANTMENT_SPELL_SHIELD : ((bp1c == CmFireshield) ? C00_ENCHANTMENT_FIRE_SHIELD : C02_ENCHANTMENT_PARTY_SHIELD);
			si = max_value(32, si) * 3;
			if (CALL_ENCHANTMENT_SELF(champion, enchantmentType, si, 1) != 0)
				break;
			bp2c >>= 2;
			bp26 >>= 1;
			break;

		case CmConsume: // 16
			PLAYER_CONSUME_OBJECT(di, bp2e, bp34);
			break;

		case CmPouch: // 17
			bp4a = 1;
			iPouchOrScabbard = FIND_POUCH_OR_SCABBARD_POSSESSION_POS(di, bp34);
			if (iPouchOrScabbard < 0)
				break;
			bp2e = REMOVE_POSSESSION(di, iPouchOrScabbard);
			EQUIP_ITEM_TO_INVENTORY(di, bp2e, bp34);
			break;

		case CmHealing: // 36
			bp0c = champion->maxHP() - champion->curHP();	// how much HP missing
			if (bp0c <= 0)
				break;
			if (champion->curMP() == 0)
				break;
			bp0a = min_value(SkLvlMasterLo, QUERY_PLAYER_SKILL_LV(di, SKILL_PRIEST_HEAL, 1));	// skill, caped 10
			bp2c = 2;

			do {
				bp0e = min_value(bp0c, bp0a);	// Minimum step between initial HP missing and skill level (max = 10)
				champion->curHP(champion->curHP() +bp0e);
				bp2c += 2;
				champion->curMP(champion->curMP() -2);
			} while (champion->curMP() > 0 && (bp0c -= champion->curMP()) != 0);

			if (champion->curMP() < 0)
				champion->curMP(0);
			champion->heroFlag |= CHAMPION_FLAG_0800;	// 0x0800 Refresh bar stat?
			bp0e = 1;
			break;

		case CmUseRope: // 10
			bp1e = cd.pi.glbPlayerPosX + glbXAxisDelta[cd.pi.glbPlayerDir];
			bp20 = cd.pi.glbPlayerPosY + glbYAxisDelta[cd.pi.glbPlayerDir];
			bp40 = 1;
			bp32 = GET_CREATURE_AT(bp1e, bp20);
			if (bp32 != OBJECT_NULL && (QUERY_CREATURE_AI_SPEC_FLAGS(bp32) & 0x8000) == 0) {
				bp40 = 0;
			}
			if ((GET_TILE_VALUE(bp1e, bp20) >> 5) == 2 && bp40 != 0) {
				_4976_5824 = 1;
				MOVE_RECORD_TO(OBJECT_NULL, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, bp1e, bp20);
				_4976_5824 = 0;
				break;
			}
			bp26 = 0;
			break;

		case CmFreezeLife: // 11
			glbGlobalSpellEffects.FreezeCounter = U8(min_value(EFFECT_FREEZE_MAX, glbGlobalSpellEffects.FreezeCounter +si));
			break;

		case CmAuraOfDex: // 12
			bp0c = 5;
_1d6a:
			si = max_value(32, si) << 2;
			if (CALL_ENCHANTMENT_SELF(champion, bp0c, si, 0) != 0)
				break;
			bp2c >>= 2;
			bp26 >>= 1;
			break;

		case CmAuraOfWiz: // 13
			bp0c = 4;
			goto _1d6a;

		case CmAuraOfVit: // 14
			bp0c = 6;
			goto _1d6a;

		case CmAuraOfStr: // 15
			bp0c = 3;
			goto _1d6a;

		case CmDarkness:	// 6
		case CmLight:		// 38
		case CmLongLight:	// 39
			PROCEED_LIGHT(bp1c, si);
			break;

		case CmThrow: // 42
			bp0e = _2c1d_1de2_CHAMPION_SHOOT(
				di,
				bp34,
				(champion->playerPos() != ((cd.pi.glbPlayerDir +1) & 3))
					? (champion->playerPos() == ((cd.pi.glbPlayerDir +2) & 3))
						? 1
						: 0
					: 1
				);
			break;

		case CmMark: // 44
			bp0e = SET_DESTINATION_OF_MINION_MAP(bp2e, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, cd.pi.glbPlayerMap);
			if (bp0e != 0)
				break;
			bp2c = 0;
			bp26 = 1;
			break;

		case CmCallScout: // 47
			bp35 = CREATURE_GOOD_SCOUT_MINION;		// SPX: 0x30 = Scout Minion
			//SPX: bp35 holds creature to generate
			goto _1ebe;

		case CmCallCarry: // 45
		case CmCallFetch: // 46
			if (GET_MISSILE_REF_OF_MINION(bp08->GetContainedObject(), bp2e) != NULL) {
				GET_ADDRESS_OF_RECORD4(bp08->GetContainedObject())->CreatureType((bp1c == CmCallCarry) ? 0x32 : 0x33);
				_1c9a_0247_FREE_CACHE_FROM_CREATURE(bp08->GetContainedObject());
			}
			else if (bp1c == CmCallFetch) {
				bp35 = CREATURE_GOOD_FETCH_MINION;	// SPX: Fetch Minion
				bp0a = cd.pi.glbPlayerDir;
				bp1e = bp08->GetDestX();
				bp20 = bp08->GetDestY();
				bp22 = 0xffff;
				bp0c = bp08->GetDestMap();
				goto _1ed8;
			}
			else {
				bp35 = CREATURE_GOOD_CARRY_MINION;	// SPX: Carry Minion
_1ebe:
				bp1e = cd.pi.glbPlayerPosX;
				bp20 = cd.pi.glbPlayerPosY;
				bp0a = (cd.pi.glbPlayerDir +2) & 3;
				bp0c = cd.pi.glbPlayerMap;
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
				bp0e = (bp3e != OBJECT_NULL) ? 1 : 0;
				if (bp0e != 0) {
					if (bp1c == CmCallScout) {
						SET_DESTINATION_OF_MINION_MAP(bp2e, _4976_5826, _4976_5828, _4976_581c);
					}
					bp08->SetContainedObject(bp3e);
					break;
				}
				bp2c >>= 2;
				bp26 >>= 1;
				CREATE_CLOUD(OBJECT_EFFECT_CLOUD, 0x6e, bp1e, bp20, 255);
				break;

//_1f6b:
_ReleaseMinion:
				RELEASE_MINION(bp08->GetContainedObject());
			}
			break;

		case CmKillMinion: // 48
			goto _ReleaseMinion;

		case CmGuardMinion: // 50
			bp3c = CREATURE_GOOD_GUARD_MINION;	// Guard Minion
			goto _CreateMinion;

		case CmUHaulMinion: // 51
			bp3c = CREATURE_GOOD_U_HAUL_MINION;	// U-Haul Minion
			goto _CreateMinion;

		case CmAttackMinion: // 49
			bp3c = CREATURE_GOOD_ATTACK_MINION;	// Attack Minion
//_1f8e:
_CreateMinion:
			if (CREATE_MINION(bp3c, si >> 3, (cd.pi.glbPlayerDir +2) & 3, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, cd.pi.glbPlayerMap, bp2e, cd.pi.glbPlayerDir) != OBJECT_NULL)
				break;
			CREATE_CLOUD(OBJECT_EFFECT_CLOUD, 0x6E, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 255);
			break;

		case CmTeleport: // 54
			if (ENGAGE_X_TELEPORTER() != 0)
				break;
			glbSomeChampionPanelFlag = max_value(1, 8 - UPDATE_GLOB_VAR(C071_GLOB_BYTE_07_CHAMPION_NEED_TELEPORT_MARK, 1, C03_GLOBAL_VAR_OP_ADD));
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
	if (bp4a == 0) {
		QUEUE_NOISE_GEN2(glbItemGDATCategory, glbItemGDATIndex, cmdSound, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, bp38, 0x73, 0xC8);
	}
	if (champion->curHP() == 0 || bp48 != 0)
		return bp0e;
	if (bp26 != 0) {
		ADJUST_HAND_COOLDOWN(di, bp26, bp34);
	}
	if (bp28 != 0) {
		ADJUST_STAMINA(di, bp28);
	}
	if (bp2c != 0) {
		ADJUST_SKILLS(di, bp2a, bp2c);
	}
	if (bp0e != 0) {
		sk536e *bp16 = &glbChampionEngagingHandCommand[di][bp34];
		bp16->w0 = QUERY_CUR_CMDSTR_ENTRY(CnRP_Repeat);
		if (bp16->w0 != 0) {
			bp16->b2 = 1;
			bp16->id4 = bp2e;
			bp16->b3 = i8(cmdSlot);
		}
	}
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
		Champion *champion = &tblChampionSquad[iChampionIndex];	//*bp04
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
	Champion* xChampion = &tblChampionSquad[iChampionIndex];	//*bp04
	if (xChampion->curHP() == 0)
		return;
	if (IS_ITEM_HAND_ACTIVABLE(iChampionIndex, xChampion->Possess(iHandIndex), iHandIndex) == 0)
		return;
	glbSelectedHandAction = glbSelectedHand_2 = iHandIndex;
	cd.pi.glbChampionIndex = glbActivatedChampionIndex = iChampionIndex + 1;
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
	if (tblChampionSquad[iChampionNo].curHP() == 0)
		return;
	glbSelectedHandAction = glbSelectedHand_2 = C2_HAND_SELECTED_SPELL;
	cd.pi.glbChampionIndex = glbActivatedChampionIndex = iChampionNo + 1;
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

	ENTER(68);
	ObjectID si = rlWhatYouPlace;
	U16 bp26 = 0;
	U16 bp2a = 0;
	U16 bDelayedActuatorsRotation = 0; // SPX: DM1 retrocompatibility
	U16 iWallSideToRotate = -1; // SPX: DM1 retrocompatibility
	ObjectID bp34 = OBJECT_NULL;
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

	for (U16 bp28 = 0; bp0e != OBJECT_END_MARKER && bp28 == 0; bp0e = GET_NEXT_RECORD_LINK(bp0e)) {
		U16 bp10 = bp0e.Dir();
		if (bp10 != dir)
			continue;
		if (IS_WALL_ORNATE_ALCOVE_FROM_RECORD(bp0e) != 0) {
			//printf("Click on wall %04X (dir=%d): this is a wall alcove!\n", bp0e, bp10);
			// SPX: for DM1 compatibility, we check if this actuator is the top actuator to consider its effect as alcove
			if (SkCodeParam::bDM1Mode && bp0e != xTopActuators[bp10])
			{
				//printf("DM1Mode: wall alcove effect is rejected!\n");
				// BUT we want to check its actuator code, so that we have to jump further
				goto checkactuator;
			}
			if (si == OBJECT_NULL) {
				ObjectID bp32 = GET_WALL_TILE_ANYITEM_RECORD(xx, yy);
				ObjectID bp30 = OBJECT_NULL;
				for (; bp32 != OBJECT_END_MARKER; bp32 = GET_NEXT_RECORD_LINK(bp32)) {
					if (bp32.Dir() == bp10) {
						bp30 = bp32;
					}
				}
				if (bp30 == OBJECT_NULL)
					break;
				U16 bp2e = bp30.DBType();
				if (bp2e < dbWeapon || bp2e > dbMiscellaneous_item)
					break;
				MOVE_RECORD_TO(bp30, xx, yy, -1, 0);
				bp34 = bp30;
				break;
			}
			bp2a = 1;
			MOVE_RECORD_TO(ObjectID(si, bp10), -1, 0, xx, yy);
			
			// SPX: Actually, DM2 code seems not to like multiple wall actuators on the same side and breaks after triggering the first one, unlike DM1 which
			// sometimes needs several wall actuators on same side for mechanism. Then we don't want to break in case of DM1.
			if (SkCodeParam::bDM1Mode)
				goto checkactuator;
				//continue;
			break;
		}
		if (IS_WALL_ORNATE_SPRING(bp0e) != 0) {
			//printf("Click on wall %04X (dir=%d): this is a wall fountain!\n", bp0e, bp10);
			if (si == OBJECT_NULL) {
				if (glbChampionLeader == -1)
					break;
				for (i16 championIndex = 0; championIndex < MAX_CHAMPIONS; championIndex++) {
					if (tblChampionSquad[championIndex].curHP() != 0) {
						tblChampionSquad[championIndex].curWater(C2048_WATER_MAX);
					}
				}
				// SPX: Sound when drinking from wall
				QUEUE_NOISE_GEN2(GDAT_CATEGORY_x16_CHAMPIONS, tblChampionSquad[glbChampionLeader].HeroType(), SOUND_CHAMPION_EAT_DRINK, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 0, 0x96, 0x80);
				break;
			}
			if ((QUERY_GDAT_DBSPEC_WORD_VALUE(si, 0) & 1) != 0) {
				ADD_ITEM_CHARGE(si, C15_CHARGES_MAX_15);
				bp2a = 1;
				bp34 = si;
				break;
			}
			if (!SkCodeParam::bDM1Mode) {
			if (GET_DISTINCTIVE_ITEMTYPE(si) != 0x0194) // SPX: not sure ? creature fountain ? (or tree ?)
				break;
			SET_ITEMTYPE(si, C15_CHARGES_MAX_15);
			bp2a = 1;
			bp34 = si;
			break;
			}
		}
//SPX: added for easy jump from DM1 mode
checkactuator:
		U16 bp1c = bp0e.DBType();
		U16 bp16;
		if (bp1c == dbActuator) {
			Actuator *bp04 = GET_ADDRESS_OF_ACTU(bp0e);
			bp16 = bp04->ActuatorType();
			U16 bp18 = bp04->ActuatorData();
			U16 bp1a = bp04->ActionType();
			// SPX: more data to check
			U16 iStatus = bp04->ActiveStatus();
			U16 iToggler = bp04->ActuatorToggler();
			U16 iRevert = bp04->RevertEffect();
			U16 iOnce = bp04->OnceOnlyActuator();
			U16 iDisabled = bp04->IsDisabled();
			//printf("Clicking on wall Actuator %04X (dir=%d) ACT=%02d (1=%d DIS=%d) action=%04x (R=%d) data=%04x status=%d toggler=%d\n", bp0e, bp10, bp16, iOnce, iDisabled, bp1a, iRevert, bp18, iStatus, iToggler);
			//if (glbChampionLeader == -1 && bp16 != 0x7e)
			if (glbChampionLeader == -1 && (bp16 != ACTUATOR_x7E_TYPE_RESURECTOR && bp16 != ACTUATOR_x7F_TYPE_CHAMPION_MIRROR)) // SPX : Add 0x7F
				continue;
			U8 bp23 = GET_WALL_DECORATION_OF_ACTUATOR(bp04);
			if (bp16 == 0)
				continue;
			U16 di = 1;
			U16 bp44 = bp16;
			U16 bp2c;
			Door *bp14;
			U16 bp1e;
			U16 bp36;
			switch (bp44) {
				default:
					continue;


				case ACTUATOR_TYPE_SHOP_PANEL: // 0x3F -> 'Shop panel'
					if (si == OBJECT_NULL)
						bp04->ActiveStatus(0);
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
					di = (bp04->RevertEffect() != ((si == OBJECT_NULL) ? 1 : 0)) ? 1 : 0;
					}
					else { // DM1 alcove item = ACTUATOR_TYPE_DM1_ALCOVE_ITEM
						;
					}
					break;

				case ACTUATOR_TYPE_CHARGED_ITEM_WATCHER: // 0x15 -> 'Activator, charged item watcher'
					if (ADD_ITEM_CHARGE(si, 0) == 0)
						break;

					goto _1cb6;

				case ACTUATOR_TYPE_ITEM_WATCHER: // 0x03 -> 'Activator, item watcher
_1cb6:
					bp2c = (GET_DISTINCTIVE_ITEMTYPE(si) == bp18) ? 1 : 0;
					di = (bp04->RevertEffect() == bp2c) ? 1 : 0;
					if (bp2c == 0 || bp04->OnceOnlyActuator() == 0)
						break;
					DEALLOC_RECORD(REMOVE_OBJECT_FROM_HAND());
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
						QUEUE_NOISE_GEN2(GDAT_CATEGORY_x09_WALL_GFX, bp23, SOUND_STD_ACTIVATION, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1, 0x8C, 0x80);
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
								QUEUE_NOISE_GEN2(GDAT_CATEGORY_x09_WALL_GFX, bp23, SOUND_STD_ACTIVATION, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1, 0x8C, 0x80);
							}
							INVOKE_ACTUATOR(bp04, bp04->ActionType(), 0);
						}
					}
					continue; // because toggler will be moved, but still need to operate

				case ACTUATOR_TYPE_PUSH_BUTTON_WALL_SWITCH: // 0x46 -> 'Activator, seal-able push button wall switch'
					bp14 = GET_ADDRESS_OF_TILE_RECORD(bp04->Xcoord(), bp04->Ycoord())->castToDoor();
					if (bp14->DoorBit13C() == 0)
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
							QUEUE_NOISE_GEN2(GDAT_CATEGORY_x0F_CREATURES, 0x7F, SOUND_CREATURE_DEATH, 0xFE, bp04->Xcoord(), bp04->Ycoord(), 1, 0x8c, 0x80);
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
							QUEUE_NOISE_GEN2(GDAT_CATEGORY_x09_WALL_GFX, bp23, SOUND_STD_ACTIVATION, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1, 0x8C, 0x80);
						}
					}

					di = 0;
					//if (di != 0 || bp1a != 3) // SPX: if actuator effect is not 3 (step in open / step out close), then break
					//	break;
					bp26 = 1;
					if (bp04->SoundEffect() != 0) {
						QUEUE_NOISE_GEN2(GDAT_CATEGORY_x09_WALL_GFX, bp23, SOUND_STD_ACTIVATION, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1, 0x8C, 0x80);
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
_1d4d:
					if (si != OBJECT_NULL || bp04->ActiveStatus() != 0)
						continue;
					Timer xtimer;
					xtimer.SetMap(glbCurrentMapIndex);
					xtimer.SetTick(glbGameTick +bp18 +2);
					xtimer.TimerType(ttyWallButton);
					xtimer.actor = TIMER_ACTOR__00;
					xtimer.value = bp0e;
					QUEUE_TIMER(&xtimer);
						bp04->ActiveStatus(1);
					
					di = 0;
					if (di != 0 || bp1a != 3) // SPX: if actuator effect is not 3 (step in open / step out close), then break
						break;
					bp26 = 1;
					if (bp04->SoundEffect() != 0) {
						QUEUE_NOISE_GEN2(GDAT_CATEGORY_x09_WALL_GFX, bp23, SOUND_STD_ACTIVATION, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1, 0x8C, 0x80);
					}
					INVOKE_ACTUATOR(bp04, 0, 0);
					INVOKE_ACTUATOR(bp04, 1, bp18 +1);
					continue;

				case ACTUATOR_TYPE_2_STATE_WALL_SWITCH: // 0x17 -> 'Activator, 2 state wall switch'
					if (si != OBJECT_NULL)
						continue;
					bp04->OnceOnlyActuator(bp04->OnceOnlyActuator() ^ 1);
					di = !(bp04->OnceOnlyActuator() ^ bp04->RevertEffect());
					break;

				case ACTUATOR_x7E_TYPE_RESURECTOR: // 0x7E -> 'Activator, resuscitation'
					if (bp04->OnceOnlyActuator() == 0 || ((cd.pi.glbPlayerDir + 2) & 3) != dir)
						break;
					SELECT_CHAMPION(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, cd.pi.glbPlayerDir, cd.pi.glbPlayerMap);
					bp28 = 1;
					di = 0;
					break;

				// SPX: addition for DM1 retrocompatibility
				case ACTUATOR_x7F_TYPE_CHAMPION_MIRROR: // 0x7F -> DM1 'Activator, resuscitation'

					if (bp04->ActiveStatus() == 1 || ((cd.pi.glbPlayerDir +2) & 3) != dir) // for DM1, just take condition of direction
						break;
					SELECT_CHAMPION(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, cd.pi.glbPlayerDir, cd.pi.glbPlayerMap);
					bp28 = 1;
					di = 0;
					break;


				case ACTUATOR_TYPE_KEY_HOLE: // 0x1A -> 'Activator, key hole'
					bp1e = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x09_WALL_GFX, bp23, dtWordValue, GDAT_WALL_ORNATE__IS_ITEM_TRIGGERED);
					if (bp04->OnceOnlyActuator() != 0) {
						if (si != OBJECT_NULL)
							goto _22b1;
						si = FIND_DISTINCTIVE_ITEM_ON_TILE(xx, yy, bp10, bp1e);
						if (si == OBJECT_END_MARKER) {
							si = ALLOC_NEW_DBITEM(bp1e);
							if (si == OBJECT_NULL)
								continue;
							APPEND_RECORD_TO(si = ObjectID(si, bp10), NULL, xx, yy);
						}
						ADD_ITEM_CHARGE(si, 15);
						MOVE_RECORD_TO(si, xx, yy, -1, 0);
						bp34 = si;
					}
					else {
						if (si == OBJECT_NULL || GET_DISTINCTIVE_ITEMTYPE(si) != bp1e)
							continue;
						bp2a = 1;
						MOVE_RECORD_TO(ObjectID(si, bp10), -1, 0, xx, yy);
					}
					si = OBJECT_NULL;
					goto _22ab;

				case ACTUATOR_TYPE_X1B: // 0x1b -> ?
					if (bp18 == 0)
						continue;
					bp1e = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x09_WALL_GFX, bp23, dtWordValue, 0x0e);
					if (GET_DISTINCTIVE_ITEMTYPE(si) != bp1e)
						continue;
					bp2a = 1;
					si = OBJECT_NULL;
					if (--bp18 == 0) {
						di = 0;
						bp04->ActiveStatus(1);
					}
					bp04->ActuatorData(bp18);
					break;

				case ACTUATOR_TYPE_SIMPLE_LADDER: // 0x1c -> ? (comes from BETA)
					if (SkCodeParam::bUseFixedMode == false)	// SPX: this actuator type seems to be used only in BETA dungeon, as a ladder. Though with the original code here, it does not trigger.
					{
					if (si != OBJECT_NULL || bp04->OnceOnlyActuator() != 0)
						continue;
					}
					else	// SPX: I am not sure here, I keep the condition in another way so that the ladder triggers when player click on it.
						if (si == OBJECT_NULL && bp04->OnceOnlyActuator() == 0)
							continue;
					bp36 = (bp04->RevertEffect() != 0) ? bp04->ActionType() : ((bp04->ActionType() + cd.pi.glbPlayerDir) & 3);
					TRANSFER_PLAYER(bp04->Xcoord(), bp04->Ycoord(), bp18 & 0x3f, bp36);
					_1031_098e();

					break;
			}
			if (bp1a == 3) {
				bp1a = (di != 0) ? 1 : 0;
				di = 0;
			}
			if (di != 0)
				continue;
			bp26 = 1;
			if (bp04->SoundEffect() != 0) {
				QUEUE_NOISE_GEN2(GDAT_CATEGORY_x09_WALL_GFX, bp23, SOUND_STD_ACTIVATION, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1, 0x8c, 0x80);
			}
			INVOKE_ACTUATOR(bp04, bp1a, 0);
			continue;
		}
		U8 bp23;
		Text *bp0c;
		SimpleActuator *xSimpleActuator;
		if (rlUnk == OBJECT_NULL && bp1c == dbText) {
			bp0c = GET_ADDRESS_OF_RECORD2(bp0e);
			xSimpleActuator = (SimpleActuator*) bp0c;
			//printf("Current Simple Actuator GFX = %d (%02x)\n", xSimpleActuator->graphism, xSimpleActuator->graphism);
			if (bp0c->TextMode() != 1)
				continue;
			bp23 = bp0c->TextIndex() & 0xff;
			if (glbChampionLeader == -1)
				continue;
			i16 bp20;
			i16 bp22;
			i16 bp36;
			U16 bp1e;
			switch (bp16 = bp0c->SimpleTextExtUsage()) {
				case  4: // Holder (Torch holder)
				case  8: // Recharge (Blue gem crop)
					bp1e = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x09_WALL_GFX, bp23, dtWordValue, GDAT_WALL_ORNATE__SWITCH_ITEM);
					if (bp0c->TextVisibility() != 0) {
						if (si != OBJECT_NULL)
							goto _22b1;
						si = FIND_DISTINCTIVE_ITEM_ON_TILE(xx, yy, bp10, bp1e);
						if (si == OBJECT_NULL) {
							si = ALLOC_NEW_DBITEM(bp1e);
							if (si == OBJECT_NULL)
								continue;
							APPEND_RECORD_TO(si = ObjectID(si, bp10), NULL, xx, yy);
						}
						ADD_ITEM_CHARGE(si, 15);
						MOVE_RECORD_TO(si, xx, yy, -1, 0);

						SkD((DLV_TWEET, "Tweet: You (x:%d, y:%d, map:%d) have taken %s from wall \n"
							, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, glbCurrentMapIndex
							, static_cast<LPCSTR>(getRecordNameOf(si))
							));
						bp34 = si;
						si = OBJECT_NULL;
						if (bp16 != 8)
							goto _22ab;
						Timer bp40;
						bp40.SetMap(glbCurrentMapIndex);
						// SPX: 0x12 is the RESPAWN value, then trigger a timer to setup again the ornate (gem)
						bp40.SetTick(QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x09_WALL_GFX, bp23, dtWordValue, GDAT_WALL_ORNATE__RESPAWN_COOLDOWN) + glbGameTick +2);
						bp40.TimerType(ttySimpleActuTurnOn);
						bp40.actor = TIMER_ACTOR__00;
						bp40.value = bp0e;
						QUEUE_TIMER(&bp40);
					}
					else if (bp16 != 8) {	// Is not Recharge (8), then it is Torch Holder (4)
						if (si == OBJECT_NULL)
							continue;
						if (GET_DISTINCTIVE_ITEMTYPE(si) != bp1e)
							continue;
						bp2a = 1;
						MOVE_RECORD_TO(ObjectID(si, bp10), -1, 0, xx, yy);

						SkD((DLV_TWEET, "Tweet: You (x:%d, y:%d, map:%d) have placed %s at wall \n"
							, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, glbCurrentMapIndex
							, static_cast<LPCSTR>(getRecordNameOf(si))
							));

						si = OBJECT_NULL;
					}
_22ab:
					glbDoLightCheck = 1;
_22b1:
					bp28 = 1;
					continue;

				case  7:	// ladder disabled
					if (bp0c->TextVisibility() != 0)
						continue;

					goto _22ca;

				case  6:	// ladder enabled
_22ca:
					if (si != OBJECT_NULL)
						continue;
					bp36 = (QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x09_WALL_GFX, bp23, dtWordValue, GDAT_WALL_ORNATE__IS_LADDER_UP) != 0) ? -1 : 1;
					bp20 = cd.pi.glbPlayerPosX;
					bp22 = cd.pi.glbPlayerPosY;
					bp36 = LOCATE_OTHER_LEVEL(cd.pi.glbPlayerMap, bp36, &bp20, &bp22, NULL);
					if (bp36 < 0)
						continue;
					TRANSFER_PLAYER(bp20, bp22, bp36, cd.pi.glbPlayerDir);
					_1031_098e();
					continue;

				case 13:	// 2 state ornate (such as window)
					if (si != OBJECT_NULL)
						continue;
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
		// TODO DM2_EXTENDED_DATABASE
		if (bp1c >= dbActuator)
			break;
	}


	// SPX: DM1: if actuators need to be rotated, do it here
	if (bDelayedActuatorsRotation == 1)
		DM1_ROTATE_ACTUATOR_LIST(2, xx, yy, -1, iWallSideToRotate);

	
	if (rlUnk == OBJECT_NULL) {
		if (bp2a != 0)
			REMOVE_OBJECT_FROM_HAND();
		if (bp34 != OBJECT_NULL) {
			TAKE_OBJECT(bp34, 1);
		}
		cd.gg.glbRefreshViewport = bp26;
		return;
	}
	GenericContainerRecord *bp08 = GET_ADDRESS_OF_GENERIC_CONTAINER_RECORD(rlUnk);
	if (bp2a == 0 && rlWhatYouPlace != OBJECT_NULL) {
		APPEND_RECORD_TO(rlWhatYouPlace, &bp08->possession, -1, 0);
	}
	if (bp34 != OBJECT_NULL) {
		APPEND_RECORD_TO(bp34, &bp08->possession, -1, 0);
	}

	return;
}

//^2066:0B96
U16 SkWinCore::STORE_EXTRA_DUNGEON_DATA()
{
	_4976_5258 = 0;
	if (_2066_0b44_WRITE_TIMERS() != 0)
		return 0;
	U16 bp10 = glbCurrentMapIndex;
	U16 currentMap = 0;
	while (currentMap < dunHeader->nMaps) {
		CHANGE_CURRENT_MAP_TO(currentMap);
		Bit8u *bp04 = *glbCurrentTileMap;
		OID_T *bp08 = &dunGroundStacks[*glbIndexOfTilesWithObjects];	// U16 *bp08
		U16 xpos = 0; // word [bp-0C]
		while (!(xpos >= glbCurrentMapWidth)) {
			U16 ypos = 0; // di
			while (!(ypos >= glbCurrentMapHeight)) {
				Bit8u tileValue = *bp04; // [bp-0D]
				bp04++;
				U16 skipSaveRecord = 0; // word [bp-18]
				Bit8u bp0e;
				TELE_inf bp16;
				switch (tileValue >> 5) {
					case ttWall:
					case ttFloor:
					case ttStairs:
					case ttMapExit:
						bp0e = 0x00;
						break;
					case ttPit:
						bp0e = 0x08;
						break;
					case ttDoor:
						bp0e = 0x07;
						break;
					case ttTeleporter:
						if (GET_TELEPORTER_DETAIL(&bp16, (Bit8u)xpos, (Bit8u)ypos) != 0) {
							bp0e = 0x00;
							if (bp16.target_z() >= currentMap)
								break;
							skipSaveRecord = 0x01;
							break;
						}
						bp0e = 0x08;
						break;
					case ttTrickWall:
						bp0e = 0x04;
						break;
				}
				if (!(bp0e == 0)) {
					if (SUPPRESS_WRITER(&tileValue, &bp0e, 1, 1) != 0)
						return 0;
				}
				U16 tileRecord; // [bp-0A]
				if ((tileValue & 0x10) != 0) {
					tileRecord = *bp08;
					bp08++;
				}
				else {
					tileRecord = 0xFFFE;
				}
				if (skipSaveRecord == 0) {
					if (WRITE_RECORD_CHECKCODE(tileRecord, 1, 1) != 0)
						return 0;
				}
				ypos++;
			}
			xpos++;
		}
		currentMap++;
	}
	CHANGE_CURRENT_MAP_TO(bp10);
	return 1;
}



//^24A5:036A
// SPX: This one in the end compute light depending on champion position
// SPX: This func is called for each tile in the nearby area from the champion
// SPX: _24a5_036a renamed ADD_BACKGROUND_LIGHT_FROM_TILE
void SkWinCore::ADD_BACKGROUND_LIGHT_FROM_TILE(i16 aa, U16 bb, i16 xx, i16 yy, U16 ww)
{
	ENTER(46);
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
				bp1e = BETWEEN_VALUE(0, glbRainAmbientLightModifier + glbRainSomeLightMod, 5);
				bp12 = (tLightLevelTable[RCJ(6,bp1e)] * si) / 0x64 +bp12;
				break;
			}
			if (bp10 != 0 && (bp16 >> 8) == 0)
				break;
			di += si;
			break;
		case 1://^0457
		case 2://^0457
		case 5://^0457
		case 16://^0457
		case 17://^0457
		case 18://^0457
		case 19://^0457
			if ((ww & 1) == 0)
				break;
			if (bp2e.w0 != 1 && bp2e.w0 != 2 && bp2e.w0 != 5)
				goto _0519;
			bp16 = bp2e.tfoi[2];
			bp0b = X8(bp16);
			if (bp0b == 0xff)
				goto _0519;
			si = QUERY_GDAT_ENTRY_DATA_INDEX(10, bp0b, dtWordValue, 0xf8);
			if (si == 0)
				break;
			bp10 = ((si & 0x8000) != 0) ? 1 : 0;
			si &= 0x7fff;
			TELE_inf bp1c;
			if (GET_TELEPORTER_DETAIL(&bp1c, U8(xx), U8(yy)) != 0) {
				bp1e = BETWEEN_VALUE(0, glbRainAmbientLightModifier + glbRainSomeLightMod, 5);
				bp12 = (tLightLevelTable[RCJ(6,bp1e)] * si) / 0x64 +bp12;
				goto _0519;
			}
			else {
				if (bp10 == 0 || (bp16 >> 8) != 0)
					di += si;
_0519:
				bp0a = bp2e.w4;
			}
			for (; bp0a != OBJECT_END_MARKER; bp0a = GET_NEXT_RECORD_LINK(bp0a)) {
				bp0e = bp0a.DBType();
				if (bp0e == dbCloud) {
					Cloud *bp04 = GET_ADDRESS_OF_RECORDF(bp0a);
					if (bp04->CloudType() != 2 && bp04->CloudType() != 0 && bp04->CloudType() != 0x30)
						continue;
					bp14 = (bp04->b3_0_f() >> 1) +bp14;
					continue;
				}
				if (bp0e == dbMissile) {
					Missile *bp08 = GET_ADDRESS_OF_RECORDE(bp0a);
					if (bp08->GetMissileObject() == OBJECT_EFFECT_FIREBALL) {	// oFF80
						bp14 += (i16(bp08->EnergyRemaining()) >> 1) - _4976_3d6d[3];
					}
				}
			}
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
	// SPX: aa is distance between player and the current processed tile
	if (aa <= 8 && bp12 != 0) {
		bp12 = max_value(3, bp12 - _4976_3d5b[RCJ(9,aa)]);
	}
	else {
		bp12 = 0;
	}
	if (aa > 5)
		aa = 5;
	if (di != 0) {
		di = max_value(2, di - _4976_3d6d[RCJ(6,aa)]);
	}
	//printf("Calculting light: %d + %d + %d => %d\n", glbPrecomputedLight, di, bp12, glbPrecomputedLight + di + bp12);
	glbPrecomputedLight += di + bp12;

	if (bp14 != 0) {
		bp14 = max_value(2, bp14 - _4976_3d6d[RCJ(6,aa)]);
	}
	glbLightModifier += bp14;
	return;
}


//^1C9A:02F6
// SPX: _1c9a_02f6 renamed CHECK_RECOMPUTE_LIGHT
void SkWinCore::CHECK_RECOMPUTE_LIGHT(i16 xx, i16 yy)
{
	ENTER(34);
	U16 recomputeLight = 0;	// si
	// SPX: Comparison of these 0x6D (GDAT_GFXSET_AMBIANT_DARKNESS) values ..
	// VOID => 0
	// THICKET => 3
	// CAVE => 8
	// KEEP => 8
	// ROOF => 3
	// MISTY => 3
	// DEBUG => 8
	i16 bp0c = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x08_GRAPHICSSET, glbMapGraphicsSet, dtWordValue, GDAT_GFXSET_x6D_AMBIANT_DARKNESS);
	if (bp0c == 0) {
		glbDoLightCheck = 0;
		glbPrecomputedLight = 0;
		glbPreviousPrecompLight = 0;
		glbLightModifier = 0;
		glbPreviousLightModifier = 0;
		return;
	}
	if (bp0c > 8) {	// Thresholding at 8
		bp0c = 8;
	}
	skxxx9 bp22;
	bp22.b0 = i8(bp0c);
	bp22.b1 = 0;
	bp22.b7 = 0x17;
	WalkPath bp0a[5]; // size=?
	FIND_WALK_PATH(xx, yy, 7, 0, 1, &bp22, bp0a);
	if (glbPrecomputedLight != glbPreviousPrecompLight) {
		glbPreviousPrecompLight = glbPrecomputedLight;
		recomputeLight = 1;
	}
	if (glbLightModifier != glbPreviousLightModifier) {
		glbPreviousLightModifier = glbLightModifier;
		recomputeLight = 1;
	}
	if (recomputeLight != 0) {
		RECALC_LIGHT_LEVEL();
	}
	glbDoLightCheck = 0;
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
		DIALOG_BOX_0aaf_0067(_0aaf_02f8_DIALOG_BOX(DIALOGBOX_x06_GAME_LOADED, 0));
		U16 iGDatFlag = glbGDatOpenCloseFlag;
		if (iGDatFlag != 0) {
			glbGDatOpenCloseFlag = 1;
			GRAPHICS_DATA_CLOSE();
		}
		DIALOG_BOX_2066_03e0(0);
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
	if (xx != 0 && _4976_4c26 == 0) {
		U8 bp01 = (cd.pi.glbChampionsCount > 0) ? tblChampionSquad[0].HeroType() : 0xFE;
		// SPX: Sound when dying
		QUEUE_NOISE_GEN2(GDAT_CATEGORY_x16_CHAMPIONS, bp01, SOUND_CHAMPION_SCREAM, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 0, 255, 255);
		SLEEP_SEVERAL_TIME(240);
	}
	DIALOG_BOX_2066_03e0(0);
	cd.gg.glbGameHasEnded = 1;
#if DM2_EXTENDED_MODE == 1
	glbXAmbientSoundActivated = 0;	// reinit that variable to get ambient sound again with a restart
#endif
	if (xx != 0) {
		FIRE_FILL_SCREEN_RECT(2, 0);
		if (_4976_5bf6 != 0) {
			_4976_5bf2 = (DIALOG_BOX_0aaf_0067(_0aaf_02f8_DIALOG_BOX(DIALOGBOX_x10_RESTART_GAME, 0)) == 0) ? 1 : 0;
			if (_4976_5bf2 != 0) {
				cd.mo.glbSpecialScreen = _MENU_SCREEN__RESUME_GAME_SELECT;
				if (GAME_LOAD() != -1) {
					__INIT_GAME_38c8_03ad();
					_4976_5bf2 = 0;
					FIRE_SHOW_MOUSE_CURSOR();
					_1031_098e();
					return;
				}
			}
		}
	}
	FIRE_FILL_SCREEN_RECT(2, 0);
	FIRE_SELECT_PALETTE_SET(0);
	//DRAW_TRANSPARENT_STATIC_PIC(0x06, 0x00, 0x01, 2, 0xffff);
	DRAW_TRANSPARENT_STATIC_PIC(GDAT_CATEGORY_x06_CREDITS, 0x00, GDAT_IMG_CREDITS_TOMBSTONE, 2, 0xffff);
	FIRE_SELECT_PALETTE_SET(1);
	_1031_0541(1);
	while (MAIN_LOOP(), glbUIEventCode != UI_EVENTCODE_xEF_QUIT_CREDITS); // 239
	SK_PREPARE_EXIT();
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
	Champion *xChampion = tblChampionSquad;
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
			//QUEUE_NOISE_GEN1(GDAT_CATEGORY_x0F_CREATURES, 0x0C, 0x00, 0x46, 0x80, glbPlayerPosX, glbPlayerPosY, 1);
			QUEUE_NOISE_GEN1(GDAT_CATEGORY_x08_GRAPHICSSET, glbMapGraphicsSet, SOUND_CHAMPION_FOOTSTEP, 0x46, 0x80, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1);
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
	xChampion = tblChampionSquad;
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
			xChampion = &tblChampionSquad[bp16];
			bp0c += STAMINA_ADJUSTED_ATTR(xChampion, GET_PLAYER_ABILITY(xChampion, abStr, 0) + (RAND() & 15));
		}
		if (bp18 != bp16 && bp18 != 0xFFFF) {
			xChampion = &tblChampionSquad[bp18];
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
	ENTER(6);
	X8 bp05 = 0;
	U8 *bp04 = ALLOC_MEMORY_RAM(0x400, afDefault, 0x80);

	Bit8u* sDungeonFilename = NULL;

	if (false) {
		//^2066:2CCB
		/// ???
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
		/// ???
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
	ENTER(14);
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
		MAIN_LOOP();
		SRECT bp0e;
		i16 bp02;
		i16 bp04;
		U16 di;
		switch (_4976_4dfc) {
			case 1://^_3330
				si = -1;
				goto _3333;
			case 2://^_3333
_3333:
				bp06 = 1;
				break;
			case 3://^_333a
				QUERY_EXPANDED_RECT(_4976_4df8, &bp0e);
				QUERY_TOPLEFT_OF_RECT(_4976_4e44, &bp02, &bp04);
				di = _4976_4e68;
				di -= bp0e.y +bp04;
				si = min_value(di / glbPanelStatsYDelta, 10);
				if (_4976_5250[si].w40 == 0xDEAD && _4976_5250[si].w38 == 0xBEEF)
					si = -1;
				_2066_398a(si);
				_0aaf_002f();
			default://^_33aa
				break;
		}
		_4976_4dfc = 0xffff;
	} while (bp06 == 0);
	DIALOG_2066_37f2();
	return si;
}


//^2066:1FAB
void SkWinCore::ARRANGE_DUNGEON()
{
	SkD((DLV_DBG_GAME_LOAD, "ARRANGE_DUNGEON\n"));
	for (U16 iMapIndex = 0; iMapIndex < dunHeader->nMaps; iMapIndex++) {

		U16 bp2e = dunMapsHeaders[iMapIndex].RawColumn();
		U16 bp30 = dunMapsHeaders[iMapIndex].RawRow();
		X8 *bp04 = *glbMapTileValue[iMapIndex];
		SkD((DLV_DBG_GAME_LOAD, "MAP %d/%d (C=%d, R=%d) TMAP=%08x\n", iMapIndex, dunHeader->nMaps, bp2e, bp30, bp04));
		OID_T *bp1c = &dunGroundStacks[dunMapTilesObjectIndexPerColumn[dunMapColumnsSumArray[iMapIndex]]]; // U16 *bp1c
		for (U16 bp2a=0; bp2a <= bp2e; bp2a++) {
			for (U16 bp2c=0; bp2c <= bp30; bp2c++) {
				X8 bp35 = *(bp04++);
				if ((bp35 & 0x10) != 0) {
					ObjectID di = *bp1c;
					bp1c++;
					do {
						switch (di.DBType()) {
							case dbTeleporter:
								{
									Teleporter *bp0c = (Teleporter *)GET_ADDRESS_OF_RECORD(di);
									CHANGE_CURRENT_MAP_TO(bp0c->DestinationMap());
									ObjectID bp26 = GET_TILE_RECORD_LINK(bp0c->DestinationX(), bp0c->DestinationY());
									if (bp26 == OBJECT_END_MARKER)
										break;
									if (bp26.DBType() != dbTeleporter)
										break;
									Teleporter *bp10 = GET_ADDRESS_OF_RECORD1(bp26);
									if (bp10->DestinationX() != bp2a)
										break;
									if (bp10->DestinationY() != bp2c)
										break;
									if (bp10->DestinationMap() != iMapIndex)
										break;
									U16 bp3c = 0;
									U16 bp32 = _2066_1f37(bp26, bp0c->DestinationMap(), &bp3c);
									bp32 |= _2066_1f37(di, iMapIndex, &bp3c);
									if (bp3c == 2) {
										bp10->w0 = bp0c->w0 = _2066_1ec9(GET_NEXT_RECORD_LINK(di), GET_NEXT_RECORD_LINK(bp26));
										break;
									}
									if (bp3c == 0 && bp32 != 0)
										goto _23de;
									break;
								}
							case dbText:
								{
									Text *bp20 = GET_ADDRESS_OF_RECORD(di)->castToText();
									if (bp20->TextMode() != 1)
										break;
									switch (bp20->SimpleTextExtUsage()) {
										case 0x06: // Ladder enabled
										case 0x07: // Ladder disabled
											{
												U16 bp34 = di.Dir();
												SET_TILE_ATTRIBUTE_02(bp2a + glbXAxisDelta[bp34], bp2c + glbYAxisDelta[bp34], iMapIndex);
												break;
											}
										case 0x08: // Recharge
											{
												bp20->TextVisibility(1);
												break;
											}
									}
									break;
								}
							case dbActuator:
								{
									Actuator *bp08 = GET_ADDRESS_OF_RECORD(di)->castToActuator();
									U16 bp3e = bp08->ActuatorType();
									switch (bp3e) {
										case ACTUATOR_TYPE_SIMPLE_LADDER: // 0x1C unused in retail ver.
										case ACTUATOR_TYPE_SHOP_PANEL: // 0x3F Shop panel
											bp08->ActiveStatus(0);
											bp08->OnceOnlyActuator(1);
											break;
										case ACTUATOR_FLOOR_TYPE__CROSS_SCENE: // 0x27 Cross scene
										case ACTUATOR_FLOOR_TYPE__INFINITE_ORNATE_ANIMATOR: // 0x2C Continuous ornate animator
											if (bp08->OnceOnlyActuator() != 1)
												break;
											bp08->ActiveStatus(1);
											break;
										case ACTUATOR_TYPE_PUSH_BUTTON_WALL_SWITCH: // 0x46 Activator, seal-able push button wall switch
											CHANGE_CURRENT_MAP_TO(iMapIndex);
											if ((GET_TILE_VALUE(bp08->Xcoord(), bp08->Ycoord()) >> 5) != ttDoor)
												break;
											GET_ADDRESS_OF_TILE_RECORD(bp08->Xcoord(), bp08->Ycoord())->castToDoor()->DoorBit13B(bp08->OnceOnlyActuator() & 1);
											break;
									}
									break;
								}
							case dbCreature:
								{
									Creature *bp14 = GET_ADDRESS_OF_RECORD(di)->castToCreature();
									AIDefinition *bp18 = QUERY_CREATURE_AI_SPEC_FROM_TYPE(bp14->CreatureType());
									// SPX in non DM2 mode, creature HP is not defined by AIDef
									if (!SkCodeParam::bDM1Mode && !SkCodeParam::bBWMode)
										bp14->HP1(bp18->BaseHP);	// Otherwise, set HP from AIDef -- DM2 behaviour
									if (bp18->IsStaticObject() == 0) {
										bp14->HP3(0);
										bp14->SetTriggerXPos(bp2a);
										bp14->SetTriggerYPos(bp2c);
										bp14->SetTriggerMap(iMapIndex);
										break;
									}
									bp14->iAnimSeq = bp14->iAnimFrame = 0;
									//bp14->iAnimSeq = 0x01EB;	// SPX add	0x01EB = spawn animation sequence
									//bp14->iAnimFrame = 0x8001;	// SPX add => 1 frame
									if (SkCodeParam::bDebugNoCreatures) {
										bp14->HP1(0);
										bp14->iAnimSeq = 0x00D3;	// SPX: not sure about this
									}
									CREATURE_SET_ANIM_FRAME(di);
									if (bp14->b14_7_7() != 0)
										break;
									U16 bp28 = bp14->possession;
									if (bp28 == 0xfffe)
										break;
									bp14->possession = (bp28 & 0x3fff) | (RAND02() << 14);
									while (bp28 != 0xfffe) {
										GenericRecord *bp24 = GET_ADDRESS_OF_RECORD(bp28);
										if (bp24->w0 != 0xfffe) {
											bp24->w0.Dir(RAND02());
										}
										bp28 = GET_NEXT_RECORD_LINK(bp28);
									}
									break;
								}
						}
						di = GET_NEXT_RECORD_LINK(di);
					} while(di != OBJECT_END_MARKER);
				}
_23de:
				if ((bp35 >> 5) == ttPit) {
					// SPX: The item 0x6A is 1 only for VOID. It does not exist for other tilesets.
					// Beware! if this is not set to a map expecting it, it will crash the map loading!
					if (QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x08_GRAPHICSSET, dunMapsHeaders[iMapIndex].MapGraphicsStyle(), dtWordValue, GDAT_GFXSET_x6A_VOID_RANDOM_FALL) == 0) {	// 0x6A
						i16 xx = bp2a; //i16 bp38 = bp2a;
						i16 yy = bp2c; //i16 bp3a = bp2c;
						U16 iLocatedMap = LOCATE_OTHER_LEVEL(iMapIndex, 1, &xx, &yy, NULL); // bp34 = ...
						//SPX: locate_other_level can return -1 (65535) and it will crash the next function
						if (SkCodeParam::bUseFixedMode && iLocatedMap != 65535) // Use fixed mode to prevent crash
							//SET_TILE_ATTRIBUTE_02(bp38, bp3a, bp34);
							SET_TILE_ATTRIBUTE_02(xx, yy, iLocatedMap);
					}
				}
				SkD((DLV_DBG_GAME_LOAD, "MAP %d/%d (CI=%d, RI=%d) TMAP=%08x\n", iMapIndex, dunHeader->nMaps, bp2a, bp2c, bp04));
			}
			SkD((DLV_DBG_GAME_LOAD, "MAP %d/%d (CI=%d)\n", iMapIndex, dunHeader->nMaps, bp2a));
		}
	}
}

//^2C1D:0250
i16 SkWinCore::GET_PLAYER_AT_POSITION(U16 position)
{
	Champion* xChampion = tblChampionSquad; // bp04
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
	ENTER(12);
	X16 si = RAND02();
	X16 bp0a;
	i8 *bp04 = (rl == OBJECT_NULL) ? (bp0a = cd.pi.glbPlayerDir, _4976_406c[si]) : (bp0a = 0, _4976_407c[si]);
	i16 di;
	for (di = 0; di <= 3; di++) {
		si = (bp04[di] +bp0a) &3;
		X16 bp06 = xpos + glbXAxisDelta[si];
		X16 bp08 = ypos + glbYAxisDelta[si];
		si = GET_TILE_VALUE(bp06, bp08);
		if (IS_TILE_BLOCKED(U8(si)) == 0 && (si = (si >> 5)) != ttStairs) {
			if (rl.DBType() != dbCreature || (si != ttTeleporter && si != ttPit)) {
				ObjectID bp0c = GET_CREATURE_AT(bp06, bp08);
				if (bp0c == OBJECT_NULL || (rl == OBJECT_NULL && (QUERY_CREATURE_AI_SPEC_FLAGS(bp0c) & CREATURE_AI_FLAG_8000) != 0)) {
					*xx = bp06;
					*yy = bp08;
					return 1;
				}
			}
		}
	}
	return 0;
}

//^2C1D:1C0C
X16 SkWinCore::ATTACK_PARTY(U16 quantity, U16 yy, U16 zz)
{
	ENTER(4);
	X16 di = quantity;
	if (quantity == 0)
		return 0;
	X16 si = 0;
	X16 bp04 = (di >> 3) +1;
	di -= bp04;
	bp04 <<= 1;
	U16 bp02;
	for (bp02 = 0; bp02 < cd.pi.glbChampionsCount; bp02++) {
		if (WOUND_PLAYER(bp02, max_value(1, di + RAND16(bp04)), yy, zz) != 0) {
			si |= 1 << bp02;
		}
	}
	return si;
}




//^13E4:0401
void SkWinCore::ATTACK_CREATURE(ObjectID rl, i16 xx, i16 yy, U16 ss, i16 tt, U16 quantity)
{
	// quantity=0 to warn the creature. e.g. hazard notice of incoming missile.
	printf("ATTACK CREATURE @ %d,%d with %d\n", xx, yy, quantity);
	ENTER(20);
	U16 di = 0;
	if (rl == OBJECT_NULL) {
		rl = GET_CREATURE_AT(xx, yy);
		if (rl == OBJECT_NULL)
			return;
	}
	U16 bp12 = ss & 0x4000;
	if (bp12 != 0) {
		ss &= 0xbfff;
		if (RAND01() != 0)
			bp12 = 0;
	}
	U16 bp14 = ss & 0x2000;
	if (bp14 != 0)
		ss &= 0xdfff;
	Creature* xCreature = GET_ADDRESS_OF_RECORD4(rl); // bp04
	AIDefinition* xAIDef = QUERY_CREATURE_AI_SPEC_FROM_TYPE(xCreature->CreatureType()); // bp0c
	U16 bp10 = xAIDef->IsStaticObject();
	if (xCreature->InternalID() == 0xFF) {
		if (bp10 == 0)
			return;
		ALLOC_CAII_TO_CREATURE(rl, xx, yy);
	}
	CreatureInfoData* xCreatureData = &glbTabCreaturesInfo[xCreature->InternalID()];	// bp08
	xCreatureData->w20 += quantity;
	quantity = xCreatureData->w20;
	U16 si;	// si , new command
	U16 bp0e;
	if (bp10 == 0 && tt > 0) {
		if ((xCreature->iAnimFrame & 4) == 0) {
			if (quantity <= 0x1E && (quantity <= 4 || RAND02() != 0) && (((quantity * U32(0x64)) / xAIDef->BaseHP) > 15)) {
				xCreature->iAnimFrame |= 4;
				bp12 = 1;
			}
		}
		if (bp12 != 0) {
			if ((tblAIStats01[QUERY_GDAT_CREATURE_WORD_VALUE(xCreature->CreatureType(), 1)] & 0x80) == 0 && RAND01() != 0) {
				si = CALC_VECTOR_DIR(xx, yy, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY);
				if ((xCreature->iAnimFrame & 8) != 0 && RAND02() != 0)
					goto _05bd;
				if ((xCreature->b15 & 3) != si && RAND02() == 0) {
_05bd:
					si = (si +2) & 3;
				}
				if (xCreature->b15_0_1() == ((si +2) & 3)) {
					si = (RAND01() != 0) ? 7 : 6;
				}
				else if (xCreature->b15_0_1() != si) {
					si = (xCreature->b15_0_1() == ((si -1) & 3)) ? 7 : 6;
				}
				else if (RAND02() == 0) {
					si = (RAND01() != 0) ? 6 : 7;
				}
				else {
					si = 0xffff;
				}
				if (si != 0xffff) {
					CREATURE_SET_NEW_COMMAND(rl, xx, yy, U8(si), 0);
				}
			}
		}
		di = (RAND16(0x64) < tt) ? 1 : 0;
		if (di != 0) {
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
	if (bp10 == 0 && bp14 != 0 && tt == 0) {
		di = 1;
	}
	else if (di != 0 && ((bp0e == 0 && ((ss & 0x40) != 0)) || bp14 != 0) && (tlbCreatureCommandsFlags[RCJ(MAX_CREATURE_COMMANDS,xCreatureData->Command)] & 0x10) == 0) {
		di = 1;
	}
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

	ENTER(14);
	U8* xLocalThinkCreatureData = NULL;	// bp04
	i16 bp0e = glbCurrentMapIndex;
	glbCurrentMapIndex = -1;
	CHANGE_GLOBAL_CREATURE_POS(bp0e);
	glbCreatureSomeX = 0xffff;
	if (_4976_5160 != 0) {
		// reach here when DRAGOTH creates DRAGOTH ATTACK MINION.

		xLocalThinkCreatureData = ALLOC_MEMORY_RAM(SIZE_LOCAL_CREATURE_VAR, afDefault, 1024); // 0x34e in DM2
		// TODO: fixme
		//ATLASSERT(false);
		COPY_MEMORY(&glbCurrentThinkingCreatureID, xLocalThinkCreatureData, SIZE_LOCAL_CREATURE_VAR);
	}
	_4976_5160 = 1;
	_4976_5161 = 0;
	glbCurrentThinkingCreatureID = rl;
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
	glbCurrentThinkingCreatureData = xCreatureInfo;
	glbAIDef = QUERY_CREATURE_AI_SPEC_FROM_TYPE(xCreature->CreatureType());
	glbCreatureAnimSeqInfo = GET_CREATURE_INFO_DATA(xCreature, glbAIDef);
	glbSomeMap_4976_4ee7 = U8(bp0e);
	glbCreatureTimer.SetMap(bp0e);
	glbCreatureTimer.SetTick(glbGameTick);
	glbCreatureTimer.actor = xCreature->CreatureType();
	glbCreatureTimer.TimerType(U8(timerType));
	glbCreatureTimer.XcoordB(U8(xx));
	glbCreatureTimer.YcoordB(U8(yy));
	glbCreatureAnimationFrame = NULL;
	glbCreatureActionProceeded = 0;
	glbCreatureAIStatIndex = 0xffff;
	if (xCreatureInfo == NULL && SkCodeParam::bDebugBypassNullPointers == false) {
		if (timerType == tty22) {
			glbCreatureCommand_4976_4ee8 = 0;
			_4976_4eea = 0;
			glbCreatureCommandThinking = (xCreatureInfo->Command == -1) ? 0 : xCreatureInfo->Command;
			if (xCreatureInfo != NULL) {
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
	return xLocalThinkCreatureData;
}



//^075F:084D
U16 SkWinCore::ATTACK_WALL(i16 xTo, i16 yTo, i16 xFrm, i16 yFrm, U16 dirTo, ObjectID rlThrown)
{
	ENTER(16);
	ObjectID si = rlThrown;
	U16 bp0c = 0;
	dirTo += 2;
	dirTo &= 3;
	ObjectID di = GET_TILE_RECORD_LINK(xTo, yTo);
	Missile *bp08 = GET_ADDRESS_OF_RECORDE(si);
	for (; di != OBJECT_END_MARKER; di = GET_NEXT_RECORD_LINK(di)) {
		if (di.Dir() != dirTo)
			continue;
		if (true
			&& bp08->GetMissileObject().DBType() != dbCloud 
			&& bp0c == 0 
			&& _0cee_319e_ALCOVE_GET_GDAT_X13(di) != 0 
			&& RAND02() == 0
		) {
			CUT_RECORD_FROM(si, NULL, xFrm, yFrm);
			si.Dir(di.Dir());
			DELETE_MISSILE_RECORD(si, NULL, xTo, yTo);
			bp0c = 1;
			continue;
		}
		if (di.DBType() != dbActuator)
			continue;
		ObjectID bp0a = bp08->GetMissileObject();
		if (bp0a.DBType() == dbCloud)
			continue;
		Actuator *bp04 = GET_ADDRESS_OF_ACTU(di);
		U16 bp10 = bp04->ActuatorData();
		U16 bp0e;
		switch (bp04->ActuatorType()) {
			case ACTUATOR_TYPE_FLYING_ITEM_CATCHER: // 0x22 -> Activator, knock sensor (flying item)
				bp0e = (bp10 == 0x1ff || GET_DISTINCTIVE_ITEMTYPE(bp0a) == bp10) ? 1 : 0;
				if ((bp04->RevertEffect() ^ bp0e) == 0)
					continue;
				if (bp04->ActionType() == 3) {
					INVOKE_ACTUATOR(bp04, ((bp04->RevertEffect() ^ bp0e) != 0) ? 0 : 1, 0);
				}
				else {
					INVOKE_ACTUATOR(bp04, bp04->ActionType(), 0);
				}
				if (bp0c != 0)
					continue;
				if (bp04->OnceOnlyActuator() == 0)
					continue;
				CUT_RECORD_FROM(si, NULL, xFrm, yFrm);
				si.Dir(dirTo);
				DELETE_MISSILE_RECORD(si, NULL, xTo, yTo);
				bp0c = 1;
				continue;
			case ACTUATOR_TYPE_FLYING_ITEM_TELEPORTER: // 0x23 -> Item teleporter (flying item)
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
				DELETE_MISSILE_RECORD(si, NULL, bp04->Xcoord(), bp04->Ycoord());
				bp0c = 1;
				break;
			default: // ^_0ae1
				break;
		}
	}
	return bp0c;
}






//^0CEE:0E44
void SkWinCore::SET_ITEM_IMPORTANCE(ObjectID rlItem, X16 isImportant) 
{
	// for potion: important -> power visibility.
	// for creature: ?

	ENTER(4);
	X16 si = isImportant;
	GenericRecord *bp04 = GET_ADDRESS_OF_RECORD(rlItem);
	switch (rlItem.DBType()) {
	case dbCreature: // 4://^0E73
		bp04->castToCreature()->b15_2_2(si & 1);
		break;
	case dbCloth: // 6://^0E89
		goto _0e8d;
	case dbWeapon: // 5://^0E8B
		goto _0e8d;
	case dbMiscellaneous_item: // 10://^0E8D
_0e8d:
		bp04->castToMisc()->Important(si & 1);
		break;
	case dbPotion: // 8://^0EA3
		bp04->castToPotion()->VisiblePower(si & 1);
		break;
	case dbScroll: // 7://^0EB7
	case dbContainer: // 9://^0EB7
		break;
	}
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
				di = GET_ITEM_MAX_CHARGE(si);
				break;
			case 1: //^_1e0d
				di = 0;
				break;
			case 2: //^_1e11
				di = RAND16(GET_ITEM_MAX_CHARGE(si) + 1);
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