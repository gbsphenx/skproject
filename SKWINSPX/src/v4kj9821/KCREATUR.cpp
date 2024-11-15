#include <StdAfx.h>	// Required for MVC6 compilation

#include <SkVer.h>

#include <KCORE.H>
#include <SkDebug.h>

#if defined(_USE_MFC80) || defined(_USE_MFC60)
#include <SkMFC.h>
#include <resource.h>
#endif // defined(_USE_MFC80) || defined(_USE_MFC60)

#include <KAITABLE.h>	// for dAITableGenuine (hard coded AI table)

#if defined(__DJGPP__) || defined (__MINGW__) || defined (__LINUX__)
#define min(A,B) ((A < B) ? A : B)
#define max(A,B) ((A < B) ? B : A)
#endif

//^0CEE:2CFF
AIDefinition *SkWinCore::QUERY_CREATURE_AI_SPEC_FROM_TYPE(Bit8u creatureType)
{
	Bit16u res = QUERY_GDAT_CREATURE_WORD_VALUE(creatureType, CREATURE_STAT_AI);	// 0x05
	
	// SPX: Replaced _4976_03a2 with a real table dedicated to AI.
	if (SkCodeParam::bUseDM2ExtendedMode)
		return const_cast<AIDefinition *>(reinterpret_cast<const AIDefinition *>(&dAITable[res]));

	return const_cast<AIDefinition *>(reinterpret_cast<const AIDefinition *>(&_4976_03a2[res * 0x0024]));
	
}

//^0CEE:2D36
Bit16u SkWinCore::QUERY_GDAT_CREATURE_WORD_VALUE(Bit8u creatureType, Bit8u cls4)
{
	// If "glbSomeCreatureTable" is allocated, we get the value from it, else, we always query from GDAT
	//^0CEE:2D36
	if (glbSomeCreatureTable != NULL) {
		//^0CEE:2D46
		while (creatureType <= glbCreaturesMaxCount) {
			//^0CEE:2D4F
			Bit8u tblOffset; // bp05 offset in table = byte 0, 1 or 2 to get
			if (cls4 == CREATURE_STAT_00) { // 0
				tblOffset = 0x01;
			}
			else if (cls4 == CREATURE_STAT_01) { // 1
				tblOffset = 0x00;
			}
			else if (cls4 == CREATURE_STAT_AI) {	// 5 : AI
				tblOffset = 0x02;
			}
			else {
				break;
			}

			//^0CEE:2D74
			Bit8u *ptblCreatureByteInfo = glbSomeCreatureTable + creatureType * 3 + tblOffset; // bp04
			Bit16u iCreatureInfoValue = *ptblCreatureByteInfo; // bp08
			if (iCreatureInfoValue == 0xFF) { // table is initialized with 0xFF: if so, we want to overwrite by the value from GDAT
				//^0CEE:2DA5
				iCreatureInfoValue = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_CREATURES, creatureType, dtWordValue, cls4);

				*ptblCreatureByteInfo = (Bit8u)iCreatureInfoValue; // replace the 0xFF by the value found
			}
			//^0CEE:2DC5
			return iCreatureInfoValue;
		}
	}
	//^0CEE:2DCA
	return QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_CREATURES, creatureType, dtWordValue, cls4);
}



//^1C9A:02C3
// SPX: GET_CREATURE_INFO_DATA
sk1c9a02c3 *SkWinCore::GET_CREATURE_INFO_DATA(Creature *xCreature, AIDefinition *xAIDef)
{
	if (xAIDef->IsStaticObject() != 0) {
		return reinterpret_cast<sk1c9a02c3 *>(PTR_PADD(xCreature,+8));
	}
	return reinterpret_cast<sk1c9a02c3 *>(PTR_PADD(&glbTabCreaturesInfo[xCreature->b5_0_7()],+8));
}

//^4937:01A9
Bit16u SkWinCore::_4937_01a9(Bit16u xx, Bit16u *yy, CreatureAnimationFrame **rref)
{
	//^4937:01A9
	SkD((DLV_DBG_SED2, "DBG: _4937_01a9 %04X %04X \n", (Bitu)xx, (Bitu)*yy));
	Bit16u si = *yy;
	//^4937:01B7
	CreatureAnimationFrame *bp04;
	if (si == 0xffff) {
		//^4937:01BC
		si = 0;
	}
	else {
		//^4937:01C0
		Bit16u bp08 = tlbCreaturesAnimationSequences[xx + si].w2 & 0x000f;
		//^4937:01DA
		if (bp08 == 0)
			goto _0253;
		//^4937:01DE
		si += bp08;
	}
	//^4937:01E1
	bp04 = &tlbCreaturesAnimationSequences[xx + si];

	while (true) {
		//^4937:01FB
		if (bp04->w2_0_3() == 0)
			goto _0253;
		//^4937:0206
		Bit16u bp06 = bp04->w2_4_7();
		//^4937:0216
		if (bp06 == 0xf)
			break;
		//^4937:021B
		if ((RAND() & 0xf) <= bp06)
			break;
		//^4937:0228
		si++;
		bp04++;
	}
	//^4937:022F
	Bit16u di;
	if ((bp04->b4_0_2() + bp04->b4_3_4()) != 0) {
		//^4937:024E
		di = 1;
	}
	else {
		//^4937:0253
_0253:
		di = 0;
	}
	//^4937:0255
	*yy = si;
	//^4937:025D
	*rref = &tlbCreaturesAnimationSequences[xx + *yy];
	//^4937:0284
	return di;
}



//^0CEE:2FBD
Missile *SkWinCore::GET_MISSILE_REF_OF_MINION(ObjectID rlCreature, ObjectID rlContainer)
{
	// returns a invisible missile record. it is assoc with minion map's minion.
	// the missile record has (MAP,X,Y) position of recent open door.

	//^0CEE:2FBD
	ENTER(4);
	//^0CEE:2FC3
	ObjectID di = rlCreature;
	//^0CEE:2FC6
	if (di == OBJECT_NULL || di == OBJECT_END_MARKER)
		//^0CEE:2FD0
		//^0CEE:302D
		return NULL;
	//^0CEE:2FD2
	Creature *bp04 = GET_ADDRESS_OF_RECORDX4(di);
	//^0CEE:2FDE
	if (bp04->w0 == OBJECT_NULL)
		//^0CEE:2FE5
		//^0CEE:302D
		return NULL;
	//^0CEE:2FE7
	ObjectID si = bp04->GetPossessionObject();
	//^0CEE:2FEB
	while (si != OBJECT_END_MARKER) {
		//^0CEE:2FED
		if (si.DBType() == dbMissile) {
			//^0CEE:2FFA
			Missile *bp04 = GET_ADDRESS_OF_RECORDE(si);
			//^0CEE:3006
			if (rlContainer == OBJECT_NULL || bp04->GetMissileObject() == rlContainer) {
				//^0CEE:3018
				return bp04;
			}
		}
		//^0CEE:3020
		si = GET_NEXT_RECORD_LINK(si);
		//^0CEE:3028
	}
	//^0CEE:302D
	return NULL;
}


//^1C9A:103A
ObjectID SkWinCore::CREATE_MINION(U16 creatureType, U16 healthMultiplier_1to31_baseIs8, U16 creatureDir, U16 xx, U16 yy, U16 zz, ObjectID ww, i16 dir)
{
	//^1C9A:103A
	ENTER(20);
	//^1C9A:1040
	U16 bp08 = glbCurrentMapIndex;
	//^1C9A:1046
	CHANGE_CURRENT_MAP_TO(zz);
	//^1C9A:104F
	ObjectID si = OBJECT_NULL;
	//^1C9A:1052
	U16 bp0a = (ww != OBJECT_NULL) ? 1 : 0;
	//^1C9A:1062
	U16 bp0e = xx;
	//^1C9A:1068
	U16 bp10 = yy;
	//^1C9A:106E
	U16 bp14 = (dir < 0) ? 0 : 4;
	//^1C9A:107E
	i16 bp12 = (RAND01() != 0) ? 1 : -1;
	do {
		//^1C9A:1092
		U16 bp06 = GET_TILE_VALUE(bp0e, bp10);
		//^1C9A:10A4
		U16 bp0c = bp06 >> 5;
		//^1C9A:10AC
		if (true
			&& bp0c != ttWall 
			&& bp0c != ttStairs
			&& (bp0c != ttTrickWall || (bp06 & 5) != 0)
			&& (bp0c != ttDoor || (bp06 = bp06 & 7, bp06 == 0) || bp06 == 5)
			&& GET_CREATURE_AT(bp0e, bp10) == OBJECT_NULL
		) {
			//^1C9A:10EE
			si = ALLOC_NEW_CREATURE(
				((bp0a != 0) ? 0x8000 : 0)|creatureType,
				healthMultiplier_1to31_baseIs8,
				creatureDir,
				bp0e,
				bp10
				);
			//^1C9A:1117
			if (si != OBJECT_NULL)
				//^1C9A:111A
				break;
		}
		//^1C9A:111C
		if (bp14 != 0) {
			//^1C9A:1122
			bp0e = xx + glbXAxisDelta[dir];
			//^1C9A:1134
			bp10 = yy + glbYAxisDelta[dir];
			//^1C9A:1146
			dir = (dir +bp12) & 3;
		}
		//^1C9A:1151
	} while (bp14-- != 0);
	//^1C9A:115E
	if (bp0a != 0 && si != OBJECT_NULL) {
		//^1C9A:1169
		Creature *bp04 = GET_ADDRESS_OF_RECORD4(si);
		//^1C9A:1176
		ObjectID di = bp04->GetPossessionObject();
		//^1C9A:117D
		while (di != OBJECT_END_MARKER && di.DBType() != dbMissile)
			//^1C9A:117F
			di = GET_NEXT_RECORD_LINK(di);
		//^1C9A:119A
		ATLASSERT(di != OBJECT_END_MARKER);
		GET_ADDRESS_OF_RECORDE(di)->SetMissileObject(ww);
	}
	//^1C9A:11AC
	CHANGE_CURRENT_MAP_TO(bp08);
	//^1C9A:11B5
	return si;
}


//^1C9A:0CF7
void SkWinCore::QUEUE_THINK_CREATURE(U16 xx, U16 yy)
{
	// force call THINK_CREATURE by timer system.

	//^1C9A:0CF7
	ENTER(14);
	//^1C9A:0CFC
	ObjectID si = GET_CREATURE_AT(xx, yy);
	//^1C9A:0D0A
	Creature *bp04 = GET_ADDRESS_OF_RECORD4(si);
	//^1C9A:0D17
	if (glbTabCreaturesInfo[bp04->b5_0_7()].TimerIndex() != 0xffff) {
		//^1C9A:0D32
		RELEASE_CREATURE_TIMER(si);
	}
	//^1C9A:0D39
	Timer bp0e;
	bp0e.SetMap(glbCurrentMapIndex);
	bp0e.SetTick(glbGameTick +1);
	//^1C9A:0D5C
	bp0e.TimerType((bp04->iAnimSeq == 0xffff) ? tty21 : tty22);
	//^1C9A:0D6F
	bp0e.actor = bp04->CreatureType();
	//^1C9A:0D7C
	bp0e.XcoordB(U8(xx));
	bp0e.YcoordB(U8(yy));

#if UseAltic
	bp0e.Value2(0);
	bp0e.ActionType(0);
#endif
	//^1C9A:0D85
	glbTabCreaturesInfo[bp04->b5_0_7()].TimerIndex(QUEUE_TIMER(&bp0e));
	//^1C9A:0DAD
	return;
}

//^13E4:0360
// Set new creature command
void SkWinCore::_13e4_0360(ObjectID rlCreature, U16 xx, U16 yy, U8 ss, U16 tt)
{
	//^13E4:0360
	ENTER(6);
	//^13E4:0365
	ObjectID si = rlCreature;
	//^13E4:0368
	if (si == OBJECT_NULL) {
		//^13E4:036D
		si = GET_CREATURE_AT(xx, yy);
		//^13E4:037C
		if (si == OBJECT_NULL)
			//^13E4:037F
			return;
	}
	//^13E4:0381
	U8 bp01 = GET_ADDRESS_OF_RECORD4(si)->b5_0_7();
	//^13E4:0393
	if (bp01 == 0xff)
		//^13E4:0395
		return;
	//^13E4:0397
	CreatureInfoData *bp06 = &glbTabCreaturesInfo[bp01];
	//^13E4:03B1
	if (bp06->Command2 == ccmDestroy || bp06->Command == ccmDestroy)
		//^13E4:03C0
		return;
	//^13E4:03C2
	bp06->Command2 = ss;
	//^13E4:03CC
	if (tt == 0)
		//^13E4:03D0
		return;
	//^13E4:03D2
#if UseAltic
	if ((_4976_37a6[RCJ(86, max(bp06->Command, 0))] & 0x10) != 0) {
		//^13E4:03E0
		bp06->b33 = 1;
		//^13E4:03E8
		return;
	}
#else
	if ((_4976_37a6[RCJ(86,bp06->Command)] & 0x10) != 0) {	// bp06->b26 = command
		//^13E4:03E0
		bp06->b33 = 1;
		//^13E4:03E8
		return;
	}
#endif
	//^13E4:03EA
	RELEASE_CREATURE_TIMER(si);
	//^13E4:03F1
	QUEUE_THINK_CREATURE(xx, yy);
	//^13E4:03FE
	return;
}

//^1C9A:11BB
void SkWinCore::RELEASE_MINION(ObjectID rlCreature)
{
	//^1C9A:11BB
	ENTER(4);
	//^1C9A:11C0
	Missile *bp04 = GET_MISSILE_REF_OF_MINION(rlCreature, OBJECT_NULL);
	//^1C9A:11D2
	if (bp04 != NULL) {
		//^1C9A:11D6
		U16 si = glbCurrentMapIndex;
		//^1C9A:11DA
		CHANGE_CURRENT_MAP_TO(bp04->GetMap());
		//^1C9A:11EE
		_13e4_0360(rlCreature, bp04->GetX(), bp04->GetY(), 19, 1);
		//^1C9A:1213
		CHANGE_CURRENT_MAP_TO(si);
	}
	//^1C9A:121A
	return;
}


//^2066:0583
U16 SkWinCore::WRITE_MINION_ASSOC()
{
	// minion assoc: CREATURE MINION 
	//               -> Missile 
	//                  -> Minion map 
	//                    -> CREATURE MINION 
	//                      -> Missile 
	//                         -> Minion map 
	//                            -> ...

	// it is completely circulatory effect. minion assoc stores/recovers its connection.

	//^2066:0583
	Bit16u iLocalCount = glbMinionsAssocCount; // bp0a
	ObjectID *pMinionObjectID = glbMinionsObjectIDTable;	// bp04
	//^2066:059C
	while (iLocalCount-- != 0) {
		//^2066:059E
		ObjectID rCurrentMinionID = *pMinionObjectID;	// di
		pMinionObjectID++;
		//^2066:05A8
		GenericRecord *pRecord = reinterpret_cast<GenericRecord *>(GET_ADDRESS_OF_RECORD(rCurrentMinionID)); // bp08
		//^2066:05B5
		Bit16u ref; // si
		switch (rCurrentMinionID.DBType()) {
			case dbContainer:	// 9
				//^2066:05C9
				//si = _4976_5246[reinterpret_cast<Container *>(pRecord)->w2 & 0x03FF];
				ref = _4976_5246[reinterpret_cast<Container *>(pRecord)->GetContainedObject() & 0x03FF]; // _4976_5246 is allocated by number of creatures ?!
				// SPX: just replaced direct access to w2 by function to access it
				break;
			case dbMissile:		// 14
				//si = _4976_5240[reinterpret_cast<Missile   *>(pRecord)->w2 & 0x03FF];
				ref = _4976_5240[reinterpret_cast<Missile   *>(pRecord)->GetMissileObject() & 0x03FF];
				// SPX: just replaced direct access to w2 by function to access it
				break;
			default:
				//^2066:05C7
				continue;
		}
		//^2066:05EB
		//Bit16u ref = si; // bp0c
		//^2066:05F3
		Bit16u wmask = 0x03FF; // bp0e object id mask
		DEBUG_HELP_WRITER("Minion", &ref, 2, 1);
		if (SUPPRESS_WRITER(&ref, &wmask, 2, 1) != 0)
			return 1;

		//^2066:0613
	}
	//^2066:0620
	return 0;
}



//^19F0:1522
i16 SkWinCore::CREATURE_GO_THERE(X16 aa, i16 xx, i16 yy, i16 ss, i16 tt, i16 ww)
{
	// check if you can move from (xx,yy) to (ss,tt)

	// this is not for telling creature to "go there".

	// return 0 if cannot pass
	// return 1 if can pass. also glbCurrentThinkingCreatureData->w24 points next one step.

	//SkD((DLV_CAI, "CAI: CREATURE_GO_THERE(%04X,(%2d,%2d),(%2d,%2d),%2d) \n"
	//	, (Bitu)aa, (Bitu)xx, (Bitu)yy, (Bitu)ss, (Bitu)tt, (Bitu)(i16)ww
	//	));

	//^19F0:1522
	ENTER(50);
	//^19F0:1528
	i16 di = ss;
	X16 bp1a = 0;
	X16 bp16 = 0;
	X16 bp22 = 0;
	X16 bp24 = 0;
	X16 bp1c = aa & 0x80;
	i16 bp08;
	if (bp1c != 0)
		bp08 = ccmInv;
	//^19F0:154B
	X16 bp1e = aa & 0x40;
	X16 bp20 = aa & 0x20;
	aa &= 0xff1f;
	//^19F0:1562
	i16 bp0c;
	X16 bp18;
	U16 bp06;
	i16 bp14;
	X16 si;
	Actuator *bp2e = NULL; // defaulting to NULL
	if (aa != 2) {
		X16 bp28 = (aa == 3 || aa == 7) ? 1 : 0;
		if (bp28 == 0 && _4976_4eec == 0)
			goto _1df6;
		//^19F0:158F
		bp0c = ww;
		if (ww == 6) {
			//^19F0:159B
			bp18 = 0;
			bp0c = glbCurrentThinkingCreatureRec->b15 & 3;
			goto _1811;
		}
		//^19F0:15AE
		if (ww < 4)
			goto _17af;
		//^19F0:15B7
		bp18 = 0;
		if ((_4976_4eec & 0x2000) == 0)
			goto _1df6;
		//^19F0:15C7
		si = 0;
		bp06 = GET_TILE_VALUE(xx, yy);
//			Actuator *bp2e = NULL; // moved before any goto
		switch (bp06 >> 5) {
			case ttPit:
				//^19F0:15F2
				if (ww == 5 && (bp06 & 8) != 0 && (bp06 & 1) == 0) {
					//^19F0:1606
					si = 8;
					bp08 = ccm38;
					bp0c = glbCurrentThinkingCreatureRec->b15_0_1();
					break;
				}
				goto _161f;
			case ttFloor:
			case ttTeleporter:
			case ttTrickWall:
				//^19F0:161F
_161f:
				if ((bp06 & 2) == 0)
					break;
				//^19F0:1629
				bp0c = FIND_LADDER_AROUND(xx, yy, (ww == 4) ? -1 : 1, &bp2e);
				if (bp0c >= 0) {
					bp08 = (ww == 4) ? ccm39 : ccm3A;
					si = 0x100;
					break;
				}
				//^19F0:1668
				bp08 = ccm37;
				si = 0x10;
				bp0c = glbCurrentThinkingCreatureRec->b15_0_1();
				break;
			case ttStairs:
				//^19F0:1680
				if ((bp06 & 4) != 0) {
					if (ww != 4)
						break;
					//^19F0:168D
					bp08 = ccm35;
					goto _169f;
				}
				if (ww == 5) {
					bp08 = ccm36;
_169f:
					bp0c = (_0cee_06dc_GET_TILE_DIRECTION(xx, yy) +2) & 3;
					si = 0x100;
				}
				break;
			case ttDoor:
				break;
		}
		//^19F0:16B7
		if ((_4976_4eec & si) == 0)
			goto _1df6;
		bp14 = glbCurrentMapIndex;
		//^19F0:16C9
		i16 bp0e;
		if ((bp2e != NULL) && (bp08 == ccm39 || bp08 == ccm3A)) {
			//^19F0:16DA
			xx = bp2e->Xcoord();
			yy = bp2e->Ycoord();
			bp0e = bp2e->ActuatorData();
		}
		else {
			//^19F0:1706
			bp0e = LOCATE_OTHER_LEVEL(glbCurrentMapIndex, (ww == 4) ? -1 : 1, &xx, &yy, NULL);
			if (bp0e < 0)
				goto _1df6;
			if (bp08 == ccm37) {
				//^19F0:173F
				bp06 = glbMapTileValue[bp0e][xx][yy];
				if ((bp06 >> 5) != 2 || (bp06 & 8) == 0)
					goto _1df6;
			}
		}
		//^19F0:1778
		if (IS_CREATURE_ALLOWED_ON_LEVEL(glbCurrentThinkingCreatureID, bp0e) == 0)
			goto _1df6;
		//^19F0:178D
		bp16 = 1;
		_1c9a_0648(bp0e);
		di = xx;
		tt = yy;
		yy = xx = -1;
		//^19F0:17AD
		goto _1811;
		//^19F0:17AF
_17af:
		if (di == -1) {
			bp18 = 0;
			di = xx + glbXAxisDelta[ww];
			tt = yy + glbYAxisDelta[ww];
		}
		else {
			//^19F0:17D8
			bp18 = (xx == di && yy == tt) ? 1 : 0;
			if (bp18 == 0 && ww == -1) {
				//^19F0:17F9
				ww = bp0c = CALC_VECTOR_DIR(xx, yy, di, tt);
			}
		}
		//^19F0:1811
_1811:
		if (di < 0 || di >= glbCurrentMapWidth || tt < 0 || tt >= glbCurrentMapHeight)
			goto _1df6;
		//^19F0:1836
		if (bp28 != 0) {
			if (aa == 3) {
				//^19F0:1842
				bp1a = IS_TILE_WALL(di, tt);
			}
			//^19F0:184C
			else if (aa == 7) {
				bp1a = !_19f0_000a(di, tt);
			}
			//^19F0:1864
			if (bp1a == 0)
				goto _1df6;
			//^19F0:186D
			_19f0_045a(di, tt);
			goto _1df6;
		}
		//^19F0:187A
		_19f0_045a(di, tt);
		i16 bp2a;
		if (_4976_4ef8 == 0 || ww == 6 || (bp2a = _1c9a_1a48(1, _4976_4ef8)) == -1 || (bp2a & (1 << ((ww +2) & 3))) == 0) {
			//^19F0:18BC
			if ((aa == 4 || aa == 5) && (glbAIDef->ArmorClass == AI_DEF_ARMOR_MAX))
				//^19F0:18D3
				aa = 0;
			//^19F0:18D8
			bp06 = _4976_521c;
			switch (bp06 >> 5) {
				case ttWall: // 0
					//^19F0:18F4
					si = 1;
					break;
				case ttFloor: // 1
					//^19F0:18FA
					goto _1a94;
				case ttPit: // 2
					//^19F0:18FD
					if ((bp06 & 8) != 0) {
						if ((bp06 & 1) != 0) {
							si = 0x8024;
							break;
						}
						if (bp16 != 0) {
							si = 0x000c;
							break;
						}
						si = 0x800c;
						break;
					}
					si = 6;
					break;
				case ttStairs: // 3:
					//^19F0:1929
					si = 0x100;
					break;
				case ttDoor: // 4:
					{
					//^19F0:192F
					Door *door;	//*bp04
					bp06 &= 7;
					if (bp06 == 0 && aa != 4 && aa != 5)
						goto _1a94;
					if (bp06 == 5)
						goto _1a94;
					if (bp06 == 4)
						goto _19f9;
					//^19F0:195B
					if (_4976_521e == OBJECT_NULL) {
						//^19F0:1962
						_4976_521e = GET_TILE_RECORD_LINK(di, tt);
					}
					door = GET_ADDRESS_OF_RECORD0(_4976_521e);
					if (bp1e != 0) {
						//^19F0:1986
						if (door->DoorBit10() != 0) {
							if (door->DoorBit09() != 0)
								goto _1a94;
							si = 0x4200;
							break;
						}
					}
					//^19F0:19AD
					if (door->DoorBit10() != 0 && door->DoorBit09() == 0) {
						si = 0x200;
						break;
					}
					//^19F0:19D1
					if (((door->OpeningDir() != 0) ? glbAIDef->w0_6_7() : 1) < bp06) {
						//^19F0:19F9
_19f9:
						si = 0x4200;
						break;
					}
					//^19F0:19FF
					si = 2;
					break;
					}
				case ttTeleporter: // 5:
					{
					//^19F0:1A05
					if ((bp06 & 8) == 0) {
						if (GET_TELEPORTER_DETAIL(&_4976_5226, U8(di), U8(tt)) != 0) {
							//^19F0:1A20
							_4976_5225 = 1;
							if (IS_CREATURE_ALLOWED_ON_LEVEL(glbCurrentThinkingCreatureID, _4976_5226.b4) == 0)
								goto _1df6;
							si = 0x2000;
							break;
						}
						//^19F0:1A42
						si = 0x402;
						break;
					}
					//^19F0:1A47
					if (_4976_521e == OBJECT_NULL) {
						_4976_521e = GET_TILE_RECORD_LINK(di, tt);
					}
					bp06 = GET_ADDRESS_OF_RECORD1(_4976_521e)->Scope();
					if (bp06 == 1 || bp06 == 3) {
						//^19F0:1A83
						si = 0x400;
						break;
					}
					//^19F0:1A88
					si = 2;
					break;
					}
				case ttTrickWall: // 6:
					{
					//^19F0:1A8D
					if ((bp06 & 4) != 0) {
_1a94:
						si = 2;
						break;
					}
					//^19F0:1A99
					if ((bp06 & 1) != 0) {
						si = 0x80;
						break;
					}
					//^19F0:1AA5
					si = 0x40;
					break;
					}
				case 7:
					goto _1df6;
			}
			//^19F0:1AA8
			si &= _4976_4eec;
			if (si != 0) {
				//^19F0:1AB5
				if (aa != 1 || ABS16(di - glbCreaturePosX) + ABS16(tt - glbCreaturePosY) > 1) {
					//^19F0:1AE2
					X16 bp0a;
					bp0a = 0;
					if (glbCurrentMapIndex == glbCreatureMap && di == glbCreaturePosX && tt == glbCreaturePosY)
						bp0a = 0x800;
					//^19F0:1B04
					if (bp18 == 0) {
						ObjectID bp12;
						bp12 = _19f0_050f();
						if (bp12 != OBJECT_END_MARKER) {
							//^19F0:1B16
							if (bp1e == 0 || QUERY_CREATURE_AI_SPEC_FROM_RECORD(bp12)->w10 == 0) {
								//^19F0:1B30
								if (bp20 == 0 || bp16 != 0 || (bp22 = _19f0_1511(bp12)) == 0) {
									//^19F0:1B4B
                                    bp0a |= 0x1000;
								}
							}
						}
					}
					//^19F0:1B50
					if ((bp0a & _4976_4eec) == bp0a) {
						if (aa == 4 || aa == 5) {
							//^19F0:1B6E
							ObjectID bp12;
							for (bp12 = _19f0_04bf(); bp12 != OBJECT_END_MARKER; bp12 = GET_NEXT_RECORD_LINK(bp12)) {
								//^19F0:1B75
								X16 bp10 = 0;
								bp06 = bp12.DBType();
								if (bp06 == dbCloud) {
									//^19F0:1B8B
									U16 bp26 = GET_ADDRESS_OF_RECORDF(bp12)->CloudType();
									if (bp26 == 7) {
										//^19F0:1BA7
										bp10 = CALC_CLOUD_DAMAGE(bp12, glbCurrentThinkingCreatureID);
									}
									else {
										//^19F0:1BB7
										if (bp26 == 14)
											bp24 = 1;
									}
								}
								//^19F0:1BC4
								else  if (bp06 == dbMissile) {
									//^19F0:1BCA
									if (glbCreatureTimer.XcoordB() != di || glbCreatureTimer.YcoordB() != tt) {
										//^19F0:1BDD
										Missile *bp04 = GET_ADDRESS_OF_RECORDE(bp12);
										bp2a = (ww == 6) ? bp0c : ww;
										if (glbTimersTable[bp04->TimerIndex()].Direction() != bp2a) {
											//^19F0:1C1E
											bp10 = _075f_06bd(bp04, bp04->GetMissileObject());
										}
									}
								}
								//^19F0:1C36
								if (bp10 != 0)
									goto _1df6;
								//^19F0:1C3F
							}
							//^19F0:1C54
							if (aa == 5 && bp24 == 0 && _19f0_13aa(di, tt) != 0)
								goto _1df6;
						}
						//^19F0:1C71
						bp1a = 1;
						if (bp16 != 0 || si == 0x400) {
							//^19F0:1C85
							_2fcf_0434(glbCurrentThinkingCreatureID, xx, yy, di, tt, 0);
							if (glbCurrentMapIndex == _4976_581c && di == _4976_5826 && tt == _4976_5828)
								goto _1df6;
							//^19F0:1CB8
							bp0e = glbCurrentMapIndex;
							if (bp0e != _4976_581c) {
								if ((_4976_4eec & 0x2000) == 0 || IS_CREATURE_ALLOWED_ON_LEVEL(glbCurrentThinkingCreatureID, _4976_581c) == 0) {
									//^19F0:1CDF
									bp1a = 0;
									goto _1df6;
								}
								//^19F0:1CE7
								_1c9a_0648(_4976_581c);
							}
							//^19F0:1CF1
							bp1a = CREATURE_GO_THERE(aa | bp1e, _4976_5826, _4976_5828, _4976_5826, _4976_5828, 6);
							_1c9a_0648(bp0e);
							goto _1df6;
						}
						//^19F0:1D20
						if (bp18 == 0) {
							//^19F0:1D29
							if (si == 0x4000) {
								return _19f0_0d10(bp1c, xx, yy, di, tt, bp0c);
							}
							//^19F0:1D4A
							if ((si & 0x8000) != 0) {
								if (aa == 6) {
									//^19F0:1D59
									bp1a = 0;
								}
								else {
									//^19F0:1D60
									bp0a = _4976_4eec;
									_4976_4eec &= 0x7DB7;
									//^19F0:1D6C
									bp1a = CREATURE_GO_THERE(aa | bp1e, di, tt, di + glbXAxisDelta[bp0c], tt + glbYAxisDelta[bp0c], bp0c);
									_4976_4eec = bp0a;
								}
								//^19F0:1DA3
								if (bp1a != 0) {
									//^19F0:1DA9
									bp08 = ccmJump;
								}
								//^19F0:1DB0
								else if ((_4976_4eec & 8) != 0) {
									_4976_4eec &= 0x7FFF;
									bp1a = CREATURE_GO_THERE(aa | bp1e, xx, yy, di, tt, bp0c);
									if (bp1a != 0)
										bp08 = ccm38;
									_4976_4eec = bp0a;
								}
								//^19F0:1DEB
								if (bp1a == 0)
									_4976_5224 = 1;
							}
						}
					}
				}
			}
		}
	}
	//^19F0:1DF6
_1df6:
	if (bp1c != 0) {
		//^19F0:1DFF
		if (bp1a == 0) {
			//^19F0:1E05
			_4976_4ee5 = xactrNo;
		}
		//^19F0:1E0D
		else if (bp18 != 0) {
			//^19F0:1E13
			_4976_4ee5 = xactrYes;
		}
		//^19F0:1E1B
		else if (bp22 == 0 || _19f0_0891(0x82, xx, yy, di, tt, -1) == 0) {
			//^19F0:1E3E
			if ((_4976_4ef0 & 0x20) != 0) {
				//^19F0:1E46
				for (bp06 = 0; bp06 <= 3; bp06++) {
					//^19F0:1E4D
					if (bp06 != bp0c) {
						X16 bp30 = xx + glbXAxisDelta[bp06];
						X16 bp32 = yy + glbYAxisDelta[bp06];
						if ((GET_TILE_VALUE(bp30, bp32) >> 5) == ttDoor) {
							//^19F0:1E89
							if (_19f0_0d10(0x82, xx, yy, bp30, bp32, bp06) != 0)
								goto _1ff2;
						}
					}
					//^19F0:1EA9
				}
			}
			//^19F0:1EB2
			if (aa == 6) {
				bp0c += 2;
				bp0c &= 3;
				bp08 = ccm09;
			}
			//^19F0:1EC8
			else if (bp08 == i16(ccmInv)) {
				bp08 = (ABS16(xx -di) + ABS16(yy -tt) <= 1) ? ccm02 : ccm01;
			}
			//^19F0:1EFC
			bp06 = glbCurrentThinkingCreatureRec->b15_0_1();
			if (bp06 != bp0c) {
				//^19F0:1F0F
				if (((bp0c +2) & 3) == bp06 || bp08 != ccm02) {
					//^19F0:1F22
					if ((aa == 4 || aa == 5) && (tblAIStats01[_4976_4efa] & 0x100) == 0 && RAND02() != 0) {
						//^19F0:1F45
						bp08 = ccm09;
						goto _1f9a;
					}
					else {
						//^19F0:1F4C
						_19f0_0559(bp0c);
						goto _1ff2;
					}
				}
			}
			//^19F0:1F57
			if (bp06 != bp0c && bp08 != ccm09) {
				//^19F0:1F65
				bp08 = (((bp06 +1) & 3) == bp0c) ? ccm04 : ccm03;
				//bp08 = ccm1C;	// force for test
			}
			//^19F0:1F7C
			else if ((_4976_37a6[RCJ(86,_4976_4ee4)] & 2) != 0 && aa != 6 && bp16 == 0) {
				bp08 = ccmNeutral;
			}
			//^19F0:1F9A
_1f9a:
			glbCurrentThinkingCreatureData->w24.SetX(di);
			glbCurrentThinkingCreatureData->w24.SetY(tt);
			glbCurrentThinkingCreatureData->w24.SetMap(glbCurrentMapIndex);
			//^19F0:1FD1
			glbCurrentThinkingCreatureData->b27 = X8(ww);
			glbCurrentThinkingCreatureData->b29 = X8(bp0c);
			glbCurrentThinkingCreatureData->b32 = X8(aa);
			glbCurrentThinkingCreatureData->Command = X8(bp08);
			_4976_4ee5 = xactrAgain;
		}
	}
	//^19F0:1FF2
_1ff2:
	if (bp16 != 0)
		_1c9a_0648(bp14);
	//^19F0:2001
	//SkD((DLV_CAI, "CAI: CREATURE_GO_THERE %c \n"
	//	, (Bitu)((bp1a ? 'o' : 'x'))
	//	));
	return bp1a;
}


//^1C9A:0006
X16 SkWinCore::CREATURE_CAN_HANDLE_IT(ObjectID rlTarget, U16 flags)
{
	// return 0 if he can't.
	// return 1 if he can.

	// flags:

	// 0 to 63 means:                                       0F-4A is a WOLF.
	//   78 is ... (78&0x3F) -> 0xE, 0xE+0x10 -> 0x1E, See (0F-4A-05-1E-00-00) is "J34-36 J50 J58 J61 J8"
	//    5 is ... ( 5&0x3F) -> 0xE, 0x5+0x10 -> 0x15, See (0F-4A-05-15-00-00) is "J58 J61"

	//                       0F-22 is a MERCHANT
	//   16 merchant items. (0F-22-05-(20+3*Delta)-00-00)   Delta: 0=Weapshop, 1,2=Clothshop, 3=Miscitemshop, 4=MagicMerchant spec.
	//   17 merchant items. (0F-22-05-(21+3*Delta)-00-00)
	//   18 merchant items. (0F-22-05-(22+3*Delta)-00-00)
	//   40 ?

	//   41 ? moneybox?
	//   62 ? moneybox?
	//   63 means all items can be done?

	// 0x40: unknown. unused here.

	// 0x80: invert flag. if 1, exclude the items from specified item set.  
	//                    if 0, include the items.

	//^1C9A:0006
	ENTER(14);
	//^1C9A:000C
	i16 si = flags;
	U16 di = si & 0x80;
	U8 bp09;
	U16 bp0e;
	si &= 0x3f;
	if (si == -1 || si == -2)
		return 0;
	//^1C9A:002C
	if (si == 0x3f)
		goto _0238;
	//^1C9A:0034
	if (si == 0x3e) {
		//^1C9A:0039
		if (IS_CONTAINER_MONEYBOX(rlTarget) != 0) {
			if (GET_ADDRESS_OF_RECORD9(rlTarget)->b7_2_2() == 0) {
				return !di;
			}
		}
		return di;
	}
	//^1C9A:006E
	if (si == 0x29) {
		//^1C9A:0073
		if (IS_CONTAINER_MONEYBOX(rlTarget) != 0) {
			//^1C9A:0080
			if (GET_ADDRESS_OF_RECORD9(rlTarget)->b7_2_2() != 0) {
				return !di;
			}
			return di;
		}
		//^1C9A:00A2
		si = 7;
	}
	//^1C9A:00A5
	bp09 = glbCurrentThinkingCreatureRec->CreatureType();
	bp0e = rlTarget.DBType();
	if ((si >= 0x10 && si < 0x13) || si == 0x28) {
		//^1C9A:00CB
		if (si == 0x28) {
			//^1C9A:00D0
			if (IS_CONTAINER_MONEYBOX(rlTarget) != 0 && GET_ADDRESS_OF_RECORD9(rlTarget)->b7_2_2() == 0) {
				goto _0238;
			}
			//^1C9A:00FB
			si = 0x10;
		}
		//^1C9A:00FE
		si = glbCurrentThinkingCreatureRec->iAnimSeq * 3 +si;
	}
	//^1C9A:0111
	U8 *bp04;
	bp04 = _4976_5164[si];
	if (bp04 == 0) {
		//^1C9A:0128
		_4976_520c--;
		bp04 = _4976_5164[si] = _4976_520e;
		_4976_520e += 0x40;
		if (QUERY_CREATURES_ITEM_MASK(bp09, U8(si), bp04, (bp0e == dbCreature) ? 1 : 0) == 0)
			return 0;
	}
	//^1C9A:0173
	i16 bp0c;
	if (bp0e == dbCreature) {
		bp0c = GET_ADDRESS_OF_RECORD4(rlTarget)->CreatureType();
		goto _0219;
	}
	//^1C9A:0192
	Actuator *bp08;
	if (bp0e == dbActuator) {
		bp08 = GET_ADDRESS_OF_ACTU(rlTarget);
		switch (bp08->ActuatorType()) {
			case ACTUATOR_TYPE_X1B: // 0x1b -> '?'
				//^1C9A:01BD
				if (bp08->ActuatorData() == 0)
					return 0;
				goto _01d1;
			case ACTUATOR_TYPE_KEY_HOLE: // 0x1a -> 'Activator, key hole'
				//^1C9A:01D1
_01d1:
				bp0c = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, GET_WALL_DECORATION_OF_ACTUATOR(bp08), dtWordValue, GDAT_WALL_ORNATE__IS_ITEM_TRIGGERED);
				goto _0219;
			default:
				//^1C9A:01EF
				bp0c = bp08->ActuatorType();
				goto _0219;
		}
	}
	//^1C9A:01FE
	if (bp0e >= dbWeapon && bp0e <= dbMiscellaneous_item) {
		//^1C9A:020A
		bp0c = GET_DISTINCTIVE_ITEMTYPE(rlTarget);
		goto _0219;
	}
	//^1C9A:0216
	return 0;
	//^1C9A:0219
_0219:
	if ((bp04[bp0c >> 3] & (1 << (bp0c & 7))) != 0) {
		//^1C9A:0238
_0238:
		return !di;
	}
	//^1C9A:0241
	return di;
}



//^1C9A:19D4
void SkWinCore::_1c9a_19d4(ObjectID rl, i16 xx, i16 yy, U16 ww)
{
	//^1C9A:19D4
	ENTER(0);
	//^1C9A:19D9
	U16 si = ww;
	U16 di = si & 0x8000;
	si = si & 0x7fff;
	switch (si) {
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
		//^1C9A:19FB
		ATTACK_CREATURE(rl, xx, yy, (di != 0) ? ((si -6)|0x8000) : (si -6), 0x64, 0);
	}
	//^1C9A:1A24
	return;
}



//^12B4:0BF1
X16 SkWinCore::IS_CREATURE_MOVABLE_THERE(i16 xx, i16 yy, i16 dir, ObjectID *prlWhatsLying)
{
	// test if creature at (xx,yy) can move one step by dir.

	// return 0 if cannot
	// ... because no creature's lying there.
	// ... because block is there.
	// ... because he couldn't be alive there.

	// return 1 if can

	//^12B4:0BF1
	ENTER(12);
	//^12B4:0BF7
	i16 di = xx;
	i16 si = yy;
	ObjectID bp02 = GET_CREATURE_AT(di, si);
	if (bp02 == OBJECT_NULL || GET_CREATURE_WEIGHT(bp02) == 0xff)
		return 0;
	//^12B4:0C22
	TELE_inf bp0c;
	U8 bp03;
	if (GET_TELEPORTER_DETAIL(&bp0c, U8(di), U8(si)) != 0) {
		//^12B4:0C38
		if (((bp0c.b0 +2) & 3) == dir) {
			if (IS_CREATURE_ALLOWED_ON_LEVEL(bp02, bp0c.b4) == 0)
				return 0;
			//^12B4:0C61
			i16 bp06 = glbCurrentMapIndex;
			CHANGE_CURRENT_MAP_TO(bp0c.b4);
			bp03 = (IS_TILE_BLOCKED(GET_TILE_VALUE(bp0c.b2, bp0c.b3)) != 0 || (prlWhatsLying != NULL && GET_CREATURE_AT(bp0c.b2 + glbXAxisDelta[bp0c.b1], bp0c.b3 + glbYAxisDelta[bp0c.b1]) != OBJECT_NULL));
			//^12B4:0CD5
			CHANGE_CURRENT_MAP_TO(bp06);
			if (bp03 != 0) {
				return 0;
			}
		}
	}
	//^12B4:0CE7
	di += glbXAxisDelta[dir];
	si += glbYAxisDelta[dir];
	bp03 = GET_TILE_VALUE(di, si);
	if (IS_TILE_BLOCKED(bp03) != 0)
		return 0;
	if ((bp03 >> 5) == ttStairs)
		return 0;
	if (prlWhatsLying != NULL) {
		if (GET_CREATURE_AT(di, si) != OBJECT_NULL)
			return 0;
	}
	//^12B4:0D32
	if (GET_TELEPORTER_DETAIL(&bp0c, U8(di), U8(si)) != 0) {
		if (IS_CREATURE_ALLOWED_ON_LEVEL(bp02, bp0c.b4) == 0)
			return 0;
	}
	//^12B4:0D59
	if (prlWhatsLying != NULL) {
		*prlWhatsLying = bp02;
	}
	//^12B4:0D6A
	return 1;
}


//^2066:0569
void SkWinCore::ADD_MINION_ASSOC(ObjectID recordLink)
{
	//^2066:0569
	glbMinionsObjectIDTable[glbMinionsAssocCount] = recordLink;
	//^2066:057D
	glbMinionsAssocCount++;
}

//^2066:062B
Bit16u SkWinCore::RECOVER_MINION_ASSOC()
{
	//^2066:062B
	ENTER(12);
	//^2066:0631
	Bit16u bp0a = glbMinionsAssocCount;
	//^2066:0637
	ObjectID *bp04 = glbMinionsObjectIDTable;
	//^2066:0644
	for (; bp0a-- != 0; ) {
		//^2066:0647
		ObjectID di = *bp04; bp04++;
		//^2066:0651
		GenericRecord *bp08 = GET_ADDRESS_OF_RECORD(di);
		//^2066:065E
		switch (di.DBType()) {
			case dbContainer:
				{
					//^2066:0672
					Bit16u bp0e = 0x03ff;
					//^2066:0677
					Bit16u bp0c;
					if (SUPPRESS_READER(&bp0c, &bp0e, 2, 1, 1) != 0)
						//^2066:0692
						//^2066:06E1
						return 1;
					//^2066:0694
					Bit16u si = bp0c;
					bp08->castToContainer()->SetContainedObject(ObjectID(0, dbCreature, si));

					break;
				}
			case dbMissile:
				{
					//^2066:069F
					Bit16u bp0e = 0x03ff;
					//^2066:06A4
					Bit16u bp0c;
					if (SUPPRESS_READER(&bp0c, &bp0e, 2, 1, 1) != 0)
						//^2066:06BF
						//^2066:06E1
						return 1;
					//^2066:06C1
					Bit16u si = bp0c;
					bp08->castToMissile()->SetMissileObject(ObjectID(0, dbContainer, si));
				}
		}
		//^2066:06D0
	}
	//^2066:06DD
	return 0;
}



//^0CEE:2DE0
Bit16u SkWinCore::QUERY_CREATURE_AI_SPEC_FLAGS(ObjectID rl)
{
	return QUERY_CREATURE_AI_SPEC_FROM_RECORD(rl)->w0AIFlags;
}

//^0CEE:2D1B
AIDefinition* SkWinCore::QUERY_CREATURE_AI_SPEC_FROM_RECORD(ObjectID rlCreature)
{
	return QUERY_CREATURE_AI_SPEC_FROM_TYPE(GET_ADDRESS_OF_RECORD(rlCreature)->castToCreature()->CreatureType());
}


//^14CD:0802
// SPX: _14cd_0802 renamed CREATURE_THINK_FLUSH_POSITION
void SkWinCore::CREATURE_THINK_FLUSH_POSITION()
{
	//^14CD:0802
	ENTER(0);
	//^14CD:0805
	if (glbCurrentThinkingCreatureData == NULL && SkCodeParam::bDebugBypassNullPointers == true)
		return;
	glbCurrentThinkingCreatureData->x = xactrNeedReset;
	glbCurrentThinkingCreatureData->y = 0;
	//^14CD:0813
	return;
}
//^1C9A:0A48
// SPX: _1c9a_0a48 renamed CREATURE_SOMETHING_1c9a_0a48
X32 SkWinCore::CREATURE_SOMETHING_1c9a_0a48()
{
	// DIFF!

	//^1C9A:0A48
	ENTER(20);
	if (glbCurrentThinkingCreatureData == NULL && SkCodeParam::bDebugBypassNullPointers == true) // SPX DEBUG
		return 0;
	//^1C9A:0A4D
	i8 bp03 = glbAIDef->b9x; // bp03
	U16 bp06 = glbCurrentThinkingCreatureRec->CreatureType(); // bp06
	U16 bp0c = _4976_4ed6->iAnimSeq;
	U16 iCurrentFrameInfo = _4976_4ed6->iAnimInfo;	// bp0e
	CreatureAnimationFrame bp14; // bp14
	X16 bp02;
	if (glbCreatureAnimationFrame == NULL) {
		bp02 = (glbAIDef->IsStaticObject() != 0) ? glbCurrentThinkingCreatureRec->w12 : 0;
		GET_CREATURE_ANIMATION_FRAME(U8(bp06), glbCurrentThinkingCreatureData->Command, &bp0c, &iCurrentFrameInfo, &glbCreatureAnimationFrame, bp02);
		if (glbCreatureAnimationFrame == NULL) {
			ZERO_MEMORY(&bp14, 6);
			glbCreatureAnimationFrame = &bp14;
		}
	}
	//^1C9A:0AE6
	bp02 =glbCurrentThinkingCreatureData->b7;
	if ((glbCreatureAnimationFrame->b4 & 0x80) != 0) {
		//^1C9A:0B01
		i16 si = glbCurrentThinkingCreatureData->Command;
		if (si != ccm24 && si != ccm23 && si != ccm25) {
			//^1C9A:0B1B
			bp02 &= 0xc0;
			si = bp03 & 3;	// bits 1 & 2 => value between 0 and 3
			if (si != 0) {
				si = RAND16(si);
				if (RAND01() != 0) {
					si = (~si) & 7;	// b001 becomes b110 = 0x6
				}
				bp02 |= si;
			}
			//^1C9A:0B4B
			si = (bp03 >> 2) & 3;	// bits 3 & 4
			if (si != 0) {
				si = RAND16(si);
				if (RAND01() != 0) {
					si = (~si) & 7;
				}
				bp02 |= si << 3;
			}
		}
	}
	//^1C9A:0B80
	if ((glbCreatureAnimationFrame->b4 & 0x20) != 0) {
		bp02 |= 0x40;
	}
	else {
		bp02 &= 0xffbf;
	}
	//^1C9A:0B97
	if ((glbCreatureAnimationFrame->b4 & 0x40) != 0) {
		if (RAND01() != 0) {
			bp02 |= 0x40;
		}
		else {
			bp02 &= 0xffbf;
		}
	}
	//^1C9A:0BB7
	glbCurrentThinkingCreatureData->b7 = U8(bp02);
	_4976_4ed6->iAnimSeq = bp0c;
	_4976_4ed6->iAnimInfo = iCurrentFrameInfo;	// this is where creature animation change internal frame index
	//^1C9A:0BD3
	if (glbCreatureAnimationFrame->sound != SOUND_NONE) {	// != 0xFF
		if (SkCodeParam::bUsePowerDebug) { 
			U8 sCreatureName[32];
			QUERY_GDAT_TEXT(GDAT_CATEGORY_CREATURES, U8(bp06), 0x00, sCreatureName);
			SkD((DLV_DBG_SND_CRE, "SND: Creature sound > idx:%02X (%s) snd:%02X xx:%02X yy:%02X x:%02d y:%02d tick:%02d\n"
				, U8(bp06), sCreatureName, glbCreatureAnimationFrame->sound, 0x46, 0x80, glbCreatureTimer.XcoordB(), glbCreatureTimer.YcoordB(), 1));
		}
		// SPX: this correspond to create any useful sounds for a creature : walk, attack, growl ...
		QUEUE_NOISE_GEN1(GDAT_CATEGORY_CREATURES, U8(bp06), glbCreatureAnimationFrame->sound, 0x46, 0x80, glbCreatureTimer.XcoordB(), glbCreatureTimer.YcoordB(), 1);
	}
	//^1C9A:0C04
	X32 bp0a;
	i16 si;
	if (glbCurrentThinkingCreatureRec->w10_6_6() != 0) {
		bp0a = i16(min_value(1, glbCreatureAnimationFrame->b4 & 7)) + glbGameTick;
		goto _0cd1;
	}
	//^1C9A:0C2A
	SkD((DLV_DBG_SED2, "DBG: glbCreatureAnimationFrame %04X %04X %02X %02X \n", (Bitu)glbCreatureAnimationFrame->w0, (Bitu)glbCreatureAnimationFrame->w2, (Bitu)glbCreatureAnimationFrame->b4, (Bitu)glbCreatureAnimationFrame->sound));
	si = (glbCreatureAnimationFrame->b4 >> 3) & 3;
	if (si != 0) {
		si = RAND16(si);
	}
	//^1C9A:0C49
	si = (glbCreatureAnimationFrame->b4 & 7) +si;
	if (glbCurrentThinkingCreatureData->Command == ccmDestroy && glbGlobalSpellEffects.FreezeCounter != 0 && glbAIDef->w0_c_c() == 0) {
		//^1C9A:0C79
		si = 3 * si;
	}
	//^1C9A:0C84
	else if (glbIsPlayerSleeping != 0) {
		si <<= 1;
		if (glbCurrentMapIndex != glbPlayerMap && glbCurrentMapIndex != _4976_4c12)
			si <<= 1;
	}
	//^1C9A:0CA0
	else if ((glbCurrentThinkingCreatureRec->iAnimFrame & 8) != 0) {
		si = max_value(1, si >> 1);
	}
	//^1C9A:0CBC
	bp0a = si + glbGameTick;
_0cd1:
	if (&bp14 == glbCreatureAnimationFrame)
		//^1C9A:0CE2
		glbCreatureAnimationFrame = NULL;

	//^1C9A:0CEE
	return bp0a;
}
//^13E4:0329
void SkWinCore::UNPREPARE_LOCAL_CREATURE_VAR(U8 *ww)
{
	//^13E4:0329
	ENTER(0);
	//^13E4:032C
	if (ww == NULL) {
		_4976_5160 = 0;
		return;
	}
	//^13E4:033B
	//ATLASSERT(false);
	COPY_MEMORY(ww, &glbCurrentThinkingCreatureID, SIZE_LOCAL_CREATURE_VAR);
	DEALLOC_UPPER_MEMORY(SIZE_LOCAL_CREATURE_VAR);
	//^13E4:035E
	return;
}


//^1C9A:0E14
void SkWinCore::ALLOC_CAII_TO_CREATURE(ObjectID rl, i16 xx, i16 yy)
{
	//^1C9A:0E14
	ENTER(14);
	//^1C9A:0E1A
	X16 si = 0;
	Creature *bp08 = GET_ADDRESS_OF_RECORD4(rl);
	if (bp08->b5 != 0xff)
		return;
	//^1C9A:0E38
	si = bp08->b15_2_2();
	bp08->b15_2_2(1);
	CreatureInfoData *bp04;
	U16 bp0e;
	do {
		//^1C9A:0E4C
		bp04 = glbTabCreaturesInfo;
		for (bp0e = 0; bp0e < glbCreaturesCount; bp04++, bp0e++) {
			if (bp04->CreatureIndex() < 0)
				goto _0ea0;
		}
		//^1C9A:0E79
		for (U16 di = _4976_1a68; _4976_1a68 >= di; ) {
			if (RECYCLE_A_RECORD_FROM_THE_WORLD(dbCreature, 0xff) == OBJECT_NULL) {
				RAISE_SYSERR(SYSTEM_ERROR__CREATURE_ASSOC_FULL);
			}
		}
	} while (true);
	//^1C9A:0EA0
_0ea0:
	bp08->b15_2_2(U8(si));
	_4976_1a68++;
	ZERO_MEMORY(bp04, sizeof(CreatureInfoData));
	bp04->CreatureIndex(rl.DBIndex());
	bp08->b5 = U8(bp0e);
	bp04->TimerIndex(0xffff);
	bp04->Command = ccmInv;
	U8 *bp0c = PREPARE_LOCAL_CREATURE_VAR(rl, xx, yy, 0x22);
	CREATURE_THINK_FLUSH_POSITION();
	bp04->b6_ = U8(glbGameTick >> 2) -1;
	bp04->b4 = U8(glbGameTick) -0x7f;
	//^1C9A:0F2C
	bp04->w12.SetX(glbCreatureTimer.XcoordB());
	bp04->w12.SetY(glbCreatureTimer.YcoordB());
	bp04->w12.SetMap(glbCurrentMapIndex);
	_4976_514e.b1 = 0;
	bp04->b22 = -1;
	bp04->Command2 = ccmInv;
	bp04->b7 = 0;
	//^1C9A:0F78
	QUEUE_THINK_CREATURE(xx, yy);
	//^1C9A:0F84
	bp04->Command = (bp08->iAnimSeq == 0xFFFF) ? ccmSpawn : ccmNeutral;
	//^1C9A:0F9B
	if (glbAIDef->IsStaticObject() == 0) {
		glbCurrentThinkingCreatureRec->iAnimFrame |= 0x8000;
		glbCurrentThinkingCreatureRec->iAnimFrame &= 0xBFFF;
		CREATURE_SOMETHING_1c9a_0a48();
	}
	//^1C9A:0FBA
	UNPREPARE_LOCAL_CREATURE_VAR(bp0c);
	//^1C9A:0FC7
	return;
}


//^12B4:0D75
// SPX: Creature pushing back another creature or player?
void SkWinCore::_12b4_0d75(i16 xx, i16 yy, i16 ss, i16 tt)
{
	//^12B4:0D75
	ENTER(24);
	//^12B4:0D7B
	ObjectID bp12;
	i16 bp0a;
	i16 bp0c;
	X16 si;
	X16 bp06;
	X16 bp08;
	if (IS_CREATURE_MOVABLE_THERE(xx, yy, ss, &bp12) != 0) {
		//^12B4:0D97
		i16 bp10 = GET_CREATURE_WEIGHT(bp12);
		if (tt >= 0xfe || tt >= bp10 || RAND16(((bp10 -tt) >> 2) +1) == 0) {
			//^12B4:0DCA
			bp06 = xx + glbXAxisDelta[ss];
			bp08 = yy + glbYAxisDelta[ss];
			//^12B4:0DE8
			if (glbCurrentMapIndex == glbMap_4c28 && bp06 == glbSomePosX_4c2e && bp08 == glbSomePosY_4c30) {
				//^12B4:0E03
				_12b4_023f(xx, yy, &bp0a, &bp0c, glbPlayerDir, si = (ss +2) & 3);
			}
			else {
				//^12B4:0E2C
				_1c9a_0247(bp12);
				U8 bp13 = GET_ADDRESS_OF_RECORD4(bp12)->b5_0_7();
				if (bp13 != 0xff) {
					//^12B4:0E4D
					CreatureInfoData *bp18 = &glbTabCreaturesInfo[bp13];
					si = bp18->Command;
					if (si != -1 && si != ccmDestroy && (_4976_37a6[RCJ(86,si)] & 4) != 0) {
						RELEASE_CREATURE_TIMER(bp12);
						QUEUE_THINK_CREATURE(xx, yy);
					}
				}
				//^12B4:0E95
				MOVE_RECORD_TO(bp12, xx, yy, bp06, bp08);
				if (GET_CREATURE_AT(bp06, bp08) != OBJECT_NULL && (QUERY_CREATURE_AI_SPEC_FLAGS(bp12) & 1) == 0) {
					ATTACK_CREATURE(OBJECT_NULL, bp06, bp08, 0x4002, 0xa, 0);
				}
			}
		}
	}
	//^12B4:0EE3
	if (glbCurrentMapIndex != glbMap_4c28 || xx != glbSomePosX_4c2e || yy != glbSomePosY_4c30)
		return;
	//^12B4:0F07
	i16 di = 0;
	Champion *bp04 = glbChampionSquad;
	si = 0;
	i16 bp0e;
	for (bp0e = 0; bp0e < 4; bp04++, bp0e++) {
		//^12B4:0F13
		if (bp04->curHP() != 0  && RAND02() == 0) {
			//^12B4:0F2D
			// SPX: Creature has just bumped back the player
			QUEUE_NOISE_GEN2(GDAT_CATEGORY_CHAMPIONS, bp04->HeroType(), SOUND_CHAMPION_BUMP, 0xfe, xx, yy, 1, 0x64, 0xc8);
			di = (GET_PLAYER_WEIGHT(bp0e) / 10) +di;
			si++;
		}
		//^12B4:0F6A
	}
	//^12B4:0F78
	if (si == 0)
		return;
	di = di / si;
	di += 0x28;
	if (tt >= 0xfe || tt > di) {
		PERFORM_MOVE(((ss - glbPlayerDir) & 3) +3);
	}
	//^12B4:0FA5
	return;
}
//^32CB:0054
U16 SkWinCore::QUERY_CREATURE_5x5_POS(Creature *ref, U16 dir)
{
	// center is 12. most still object is at 5x5pos 12.

	//     0  1  2  3  4 ->X
	//   |--|--|--|--|--|
	// 0 | 0| 1| 2| 3| 4|
	//   |--|--|--|--|--|
	// 1 | 5| 6| 7| 8| 9|
	//   |--|--|--|--|--|
	// 2 |10|11|12|13|14|
	//   |--|--|--|--|--|
	// 3 |15|16|17|18|19|
	//   |--|--|--|--|--|
	// 4 |20|21|22|23|24|
	// V y

	//^32CB:0054
	ENTER(4);
	//^32CB:0058
	if (ref->b5_0_7() == 0xff)
		return 12;
	sk1c9a02c3* xAnimInfo = GET_CREATURE_INFO_DATA(ref, QUERY_CREATURE_AI_SPEC_FROM_TYPE(ref->CreatureType()));	// bp04
	i16 _5x5 = tblCreatureFrameInfo14[CREATURE_SEQUENCE_4937_000f(xAnimInfo->iAnimSeq, &xAnimInfo->iAnimInfo)][0];
	return ROTATE_5x5_POS(_5x5, dir);
}


//^1C9A:08BD
U16 SkWinCore::_1c9a_08bd(Creature *ref)
{
	//^1C9A:08BD
	ENTER(4);
	//^1C9A:08C3
	X16 si = 0;
	X16 di = ref->b5_0_7();
	if (di != 255) {
        CreatureInfoData *bp04 = &glbTabCreaturesInfo[di];
		if (bp04->Command == ccmJump) {
			switch (bp04->b31) {
				case 1:
				case 2:
					return 1;
			}
		}
		//^1C9A:0908
		return 0;
	}
	//^1C9A:090C
	return si;
}

//^1C9A:0912
U16 SkWinCore::IS_CREATURE_FLOATING(ObjectID rl)
{
	//^1C9A:0912
	ENTER(4);
	//^1C9A:0916
	Creature *bp04 = GET_ADDRESS_OF_RECORD4(rl);
	if (QUERY_CREATURE_AI_SPEC_FROM_TYPE(bp04->CreatureType())->w10_2_2() == 0) {
		if (_1c9a_08bd(bp04) == 0)
			return 0;
	}
	//^1C9A:094F
	return 1;
}



//^1C9A:1374
// DROP_CREATURE_POSSESSION(ObjectID recordLink, i16 xx, i16 yy, Bit16u ss, i16 tt)
// dropMode = 0 => GEN + POSSESS / dropMode = 2 => NOTHING
void SkWinCore::DROP_CREATURE_POSSESSION(ObjectID recordLink, i16 xx, i16 yy, Bit16u dropMode, i16 tt)
{
	//^1C9A:1374
	//^1C9A:137A
	if (dropMode == CREATURE_NO_DROP)	// dropMode == 2
		return;
	//^1C9A:1383
	Creature *bp04 = GET_ADDRESS_OF_RECORDX4(recordLink);
	//^1C9A:1392
	ObjectID si;
	Bit16u di;
	if (dropMode == CREATURE_GENERATED_DROPS) {	// dropMode == 0
		//^1C9A:139B
		for (Bit16u bp0a = CREATURE_STAT_DROP_FIRST; bp0a <= CREATURE_STAT_DROP_LAST; bp0a++) { // for (Bit16u bp0a = 10; bp0a <= 20; bp0a++) {
			//^1C9A:13A3
			Bit16u bp06 = QUERY_GDAT_CREATURE_WORD_VALUE(bp04->CreatureType(), (Bit8u)bp0a);
			//^1C9A:13B9
			if (bp06 == 0)
				//^1C9A:13BF
				continue;

			// bp0c -> battle prize: base item count. 1 or 2 items.
			// bp0e -> battle prize: additional random item count. from 0 to 6 items.

            //^1C9A:13C2
			Bit16u bp0c = (bp06 & 15) + 1;
			//^1C9A:13C9
			Bit16u bp0e = (bp06 & 0x0070) >> 4;
			//^1C9A:13D5
			if (bp0e != 0) {
				//^1C9A:13D9
				bp0c = bp0c + RAND16(bp0e +1);
			}
			//^1C9A:13E9
			bp06 >>= 7;
			//^1C9A:13ED
			while (bp0c-- != 0) {
				//^1C9A:13F0
				si = ALLOC_NEW_DBITEM(bp06);
				//^1C9A:13FB
				if (si == OBJECT_NULL)
					//^1C9A:1400
					break;
				//^1C9A:1403
				if (xx == glbPlayerPosX && yy == glbPlayerPosY) {
					//^1C9A:1415
					di = (glbPlayerDir + RAND01()) & 3;
				}
				else {
					//^1C9A:1428
					di = RAND02();
				}
				//^1C9A:142F
				MOVE_RECORD_TO(ObjectID(si, di), -1, 0, xx, yy);
				//^1C9A:144F
				if (tt >= 0) {
					//^1C9A:1455
					QUEUE_NOISE_GEN2(
						QUERY_CLS1_FROM_RECORD(si),
						QUERY_CLS2_FROM_RECORD(si),
						SOUND_STD_KNOCK,
                        0xfe,
						xx,
						yy,
						tt,
						0x003a,
						0x0080
						);
				}
				//^1C9A:1481
			}
			//^1C9A:148E
		}
	}
	//^1C9A:149A
	si = bp04->GetPossessionObject();
	//^1C9A:14A3
	if (si == OBJECT_END_MARKER)
		//^1C9A:14A8
		return;

	ObjectID bp08;
	do {
		//^1C9A:14AB
		bp08 = GET_NEXT_RECORD_LINK(si);
		//^1C9A:14B5
		if ((QUERY_CREATURE_AI_SPEC_FLAGS(recordLink) & 0x0001) == 0) {
			//^1C9A:14C3
			if (xx == glbPlayerPosX && yy == glbPlayerPosY) {
				//^1C9A:14D5
				di = (glbPlayerDir + RAND01()) & 3;
			}
			else {
				//^1C9A:14E8
				di = RAND02();
			}
			//^1C9A:14EF
			si.Dir(di);
		}
		//^1C9A:14FE
		if (si.DBType() == dbMissile) {
			//^1C9A:150B
			DEALLOC_RECORD(si);
		}
		else {


			SkD((DLV_TWEET, "Tweet: %s (a#%03d, x:%d, y:%d, map:%d) have left %s by death! \n"
				, static_cast<LPCSTR>(getRecordNameOf(recordLink))
				, recordLink.DBIndex()
				, xx
				, yy
				, glbCurrentMapIndex
				, static_cast<LPCSTR>(getRecordNameOf(si))
				));			
			
			//^1C9A:1514
			MOVE_RECORD_TO(si, -1, 0, xx, yy);

			//^1C9A:1527
			if (tt >= 0) {
				//^1C9A:152D
				QUEUE_NOISE_GEN2(
					QUERY_CLS1_FROM_RECORD(si),
					QUERY_CLS2_FROM_RECORD(si),
					SOUND_STD_KNOCK,
					0xfe,
					xx,
					yy,
					tt,
					0x3a,
					0x80
					);
			}
		}
		//^1C9A:1559
		si = bp08;
		//^1C9A:155E
	} while (si != OBJECT_END_MARKER);

	//^1C9A:1566
	return;
}



//^1C9A:198E
//SPX: _1c9a_198e renamed APPLY_CREATURE_POISON_RESISTANCE
Bit16u SkWinCore::APPLY_CREATURE_POISON_RESISTANCE(ObjectID recordLink, Bit16u iPoisonDamage)
{
	//CSBWin:Code11f52.cpp/TAG00bbbe/DeterminePoisonDamage
	//^1C9A:198E
	//^1C9A:1992
	if (iPoisonDamage != 0) {
		//^1C9A:1998
		Bit16u iPoisonResistance = QUERY_CREATURE_AI_SPEC_FROM_RECORD(recordLink)->GetPoisonResistance(); // si
		//^1C9A:19B1
		if (iPoisonResistance == 15) { // 15 == immune
			//^1C9A:19B6
			return 0;
		}
		//^1C9A:19BA
		return ((iPoisonDamage + RAND02()) << 3) / (iPoisonResistance + 2);
	}
	//^1C9A:19B6
	return 0;
}


//^2FCF:013C
void SkWinCore::SET_MINION_RECENT_OPEN_DOOR_LOCATION(ObjectID recordLink, i16 xpos, i16 ypos, Bit16u curmap, Bit16u ww)
{
	//^2FCF:013C
	//^2FCF:0141
	ObjectID si = GET_ADDRESS_OF_RECORD(recordLink)->castToCreature()->possession;
	//^2FCF:0152
	for (; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
		//^2FCF:0154
		if (si.DBType() == dbMissile) {
			//^2FCF:0161
			Missile *bp04 = GET_ADDRESS_OF_RECORD(si)->castToMissile();
			//^2FCF:016E
			bp04->SetX(xpos);
			bp04->SetY(ypos);
			bp04->SetMap(curmap);
			bp04->b6_0_0(ww);
		}
		//^2FCF:01B4
	}
	//^2FCF:01C2
	return;
}

//^1C9A:0DB0
void SkWinCore::RELEASE_CREATURE_TIMER(ObjectID recordLink)
{
	//^1C9A:0DB0
	//^1C9A:0DB5
	if (recordLink.DBType() == dbCreature) {
		//^1C9A:0DC3
		Bit8u bp01 = GET_ADDRESS_OF_RECORD(recordLink)->castToCreature()->b5_0_7();
		//^1C9A:0DD7
		if (bp01 != 0xff) {
			//^1C9A:0DDD
			i16 si = glbTabCreaturesInfo[bp01].TimerIndex();
			//^1C9A:0DF0
			if (si >= 0) {
				//^1C9A:0DF4
				DELETE_TIMER(si);
				//^1C9A:0DFB
				glbTabCreaturesInfo[bp01].TimerIndex(0xffff);
			}
		}
	}
	//^1C9A:0E11
	return;
}

//^0CEE:2E1E
// SPX: _0cee_2e1e renamed GET_CREATURE_WEIGHT
Bit16u SkWinCore::GET_CREATURE_WEIGHT(ObjectID recordLink)
{
	//^0CEE:2E1E
	//^0CEE:2E21
	return QUERY_CREATURE_AI_SPEC_FROM_RECORD(recordLink)->Weight;
}

//^1C9A:0FCB
// TODO : release creature ?
void SkWinCore::_1c9a_0fcb(Bit16u xx)
{
	//^1C9A:0FCB
	//^1C9A:0FD1
	Bit16u di = xx;
	//^1C9A:0FD4
	if (di <= glbCreaturesCount) {
		//^1C9A:0FDA
		CreatureInfoData *bp04 = &glbTabCreaturesInfo[di];
		//^1C9A:1004
		ObjectID si(0, dbCreature, bp04->CreatureIndex());
		//^1C9A:100F
		Creature *bp08 = GET_ADDRESS_OF_RECORD(si)->castToCreature();
		//^1C9A:101C
		RELEASE_CREATURE_TIMER(si);
		//^1C9A:1022
		_4976_1a68--;
		//^1C9A:1026
		bp08->b5_0_7(0xff);
		bp04->CreatureIndex(-1);
	}
	//^1C9A:1036
}

//^1C9A:0397
ObjectID SkWinCore::GET_CREATURE_AT(i16 xpos, i16 ypos)
{
	// find a creature at (xpos,ypos).
	//
	// return OBJECT_NULL if no creature found.

	//^1C9A:0397
	//^1C9A:039B
	ObjectID si = GET_TILE_RECORD_LINK(xpos, ypos);
	//^1C9A:03A8
	for (; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
		//^1C9A:03AA
		if (si.DBType() == dbCreature)
			//^1C9A:03B7
			return si;
		//^1C9A:03BB
	}
	//^1C9A:03C9
	return OBJECT_NULL;
}


//^0CEE:2E53
Bit16u SkWinCore::IS_CREATURE_ALLOWED_ON_LEVEL(ObjectID rlCreature, Bit16u curmap)
{
	//^0CEE:2E53
	//^0CEE:2E59
	if ((QUERY_CREATURE_AI_SPEC_FLAGS(rlCreature) & 0x4000) != 0) {
		//^0CEE:2E66
		return 1;
	}
	//^0CEE:2E6C
	Bit8u bp09 = QUERY_CLS2_FROM_RECORD(rlCreature);
	//^0CEE:2E77
	Map_definitions *bp08 = &dunMapsHeaders[curmap];
	//^0CEE:2E8D
	Bit8u *bp04 = &glbMapTileValue[curmap][bp08->RawColumn()][bp08->RawRow() +1];
	//^0CEE:2ECE
	i16 si = bp08->CreaturesTypes();
	//^0CEE:2EDD
	for (; si > 0; --si) {
		//^0CEE:2EDF
		if (*(bp04++) == bp09)
			return 1;
		//^0CEE:2EF0
	}
	//^0CEE:2EF5
	return 0;
	//^0CEE:2EF7
}

//^1C9A:184C
// SPX: return 1 if creature dies from damage
X16 SkWinCore::WOUND_CREATURE(i16 damage)
{
	SkD((DLV_DBM, "DBM: WOUND_CREATURE(%3d) C:%p\n"
		, (Bitu)damage, glbCurrentThinkingCreatureRec));

	//^1C9A:184C
	ENTER(12);
	//^1C9A:1852
	Creature *xLocalCreature = glbCurrentThinkingCreatureRec; // bp04
	AIDefinition *xLocalAIDef = glbAIDef; // bp08
	X16 bDiesFromDamage = 0; // di

	if (glbCurrentMapIndex == glbPlayerMap)
	SkD((DLV_TWEET, "Tweet: %s (a#%03d, x:%d, y:%d, map:%d) took %d damage! (hp:%d ac:%d) \n"
		, static_cast<LPCSTR>(getRecordNameOf(glbCurrentThinkingCreatureID))
		, glbCurrentThinkingCreatureID.DBIndex()
		, glbCreatureTimer.XcoordB() // really? x
		, glbCreatureTimer.YcoordB() // really? y
		, glbCurrentMapIndex
		, damage
		, xLocalCreature->HP1()
		, xLocalAIDef->ArmorClass
		));

	// If the creature has max defense, then take no damage
	if (xLocalAIDef->ArmorClass == AI_DEF_ARMOR_MAX)
		return bDiesFromDamage;
	X16 si = 0;	// SPX : added default value to 0
	if (xLocalAIDef->IsStaticObject() == 0) {
		//^1C9A:1882
		X16 bp0a = QUERY_GDAT_CREATURE_WORD_VALUE(xLocalCreature->CreatureType(), 1);
		si = tblAIStats01[bp0a];
		if ((si & 4) == 0) {
			//^1C9A:18A5
			X16 iCurrentMap = glbCurrentMapIndex; // bp0c
			if (SkCodeParam::bUsePowerDebug) // SPX: issue here after gameload where some creature get their triggermap beyond the number of maps (then fail)
			{ // if that case happens here, let's default trigger map to current map, and let it go that way
				X16 iTriggerMap = xLocalCreature->TriggerMap();
				if (iTriggerMap > dunHeader->nMaps)
				{
					iTriggerMap = glbCurrentMapIndex;
					xLocalCreature->SetTriggerMap(iTriggerMap);
				}
			}
			// SPX: For DM1 compatibility, we do not invoke message on triggerX/Y which would be the location of creature generator; that will trigger back a creature to generate again on the same place.
			// Then, we just keep this code for DM2, but not for DM1.
			if (!SkCodeParam::bDM1Mode)
			{
				CHANGE_CURRENT_MAP_TO(xLocalCreature->TriggerMap());
				INVOKE_MESSAGE(xLocalCreature->TriggerX(), xLocalCreature->TriggerY(), 0, 1, glbGameTick +1);
			}
			CHANGE_CURRENT_MAP_TO(iCurrentMap);
		}
	}
	//^1C9A:18F9
	if (xLocalCreature->HP1() > U16(damage)) {
		xLocalCreature->HP1(xLocalCreature->HP1() - damage);
		return bDiesFromDamage;
	}
	// SPX : we get here if creature has not enough HP to survive damage done
	//^1C9A:190E
	xLocalCreature->HP1(1);
	if (xLocalAIDef->IsStaticObject() == 0) {
		if ((si & 0x800) != 0) {
			//^1C9A:1927
			if (_1c9a_17c7(glbCreatureTimer.XcoordB(), glbCreatureTimer.YcoordB(), glbSomeMap_4976_4ee7) == 0)
				return bDiesFromDamage;
		}
		//^1C9A:1944
		if ((si & 0x800) != 0)
			_4976_4dfe = 0x18;
		//^1C9A:1950
		_13e4_0360(glbCurrentThinkingCreatureID, glbCreatureTimer.XcoordB(), glbCreatureTimer.YcoordB(), ccmDestroy, 1); // 0x13
	}
	else {
		//^1C9A:196E
		bDiesFromDamage = 1;
		DELETE_CREATURE_RECORD(glbCreatureTimer.XcoordB(), glbCreatureTimer.YcoordB(), CREATURE_GENERATED_DROPS, 1);
	}
	//^1C9A:1988
	return bDiesFromDamage;
}


//^1C9A:37BE
// SPX: Retrieve current creature Walk Path ?
// SPX: _1c9a_37be renamed __SET_CURRENT_THINKING_CREATURE_WALK_PATH
void SkWinCore::__SET_CURRENT_THINKING_CREATURE_WALK_PATH()
{
	//^1C9A:37BE
	ENTER(2);
	//^1C9A:37C2
	glbMemWalkPath = NULL;
	if (glbCurrentThinkingCreatureData == NULL || _4976_514e.b0 == 0)
		return;
	U16 bp02;
	if (_3e74_5673(glbCurrentThinkingCreatureID.DBIndex()|0x28000000, &bp02, 0) == 0) {
		//^1C9A:3800
		_4976_514e.b1 = 0;
		_4976_514e.b0 = 0;
	}
	else {
		//^1C9A:380A
		glbMemWalkPath = reinterpret_cast<WalkPath *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp02));
	}
	//^1C9A:381A
	return;
}

//^1C9A:381C
// SPX: _1c9a_381c renamed CREATURE_THINK_381c
i16 SkWinCore::CREATURE_THINK_381c()
{
	//^1C9A:381C
	ENTER(4);
	//^1C9A:3821
	CreatureInfoData *xCreatureInfo = glbCurrentThinkingCreatureData; // bp04
	X16 si = 0;
	__SET_CURRENT_THINKING_CREATURE_WALK_PATH();
	if (_4976_514e.b0 != 0) {
		//^1C9A:383B
		si = _4976_514e.b1;
		if (si != 0) {
			xCreatureInfo->b27 = glbMemWalkPath[_4976_514e.b0 -si].getDir();
			return si;
		}
	}
	//^1C9A:3864
	if (glbSomeMap_4976_4ee7 == _4976_514e.w8.GetMap()) {
		if (glbCreatureTimer.XcoordB() == _4976_514e.w8.GetX()) {
			if (glbCreatureTimer.YcoordB() == _4976_514e.w8.GetY()) {
				_4976_514e.w8 = Ax3::Invalid;
			}
		}
	}
	//^1C9A:38A3
	return si;
}

//^1C9A:0732
ObjectID SkWinCore::CREATURE_CAN_HANDLE_ITEM_IN(i16 flags, ObjectID rlFindFrom, i8 dir)
{
	// find an item (which creature can handle for holding/selling/buying) from the rlFindFrom.

	// return OBJECT_END_MARKER if he can't.
	// return id if he can handle one of them.

	//^1C9A:0732
	ENTER(0);
	//^1C9A:0737
	ObjectID si;
	for (si = rlFindFrom; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
		//^1C9A:073C
		X16 di = si.DBType();
		if ((di > dbCreature && di < dbMissile) || di == dbContainer) {
			//^1C9A:0755
			if (dir == -1 || si.Dir() == dir) {
				//^1C9A:0769
				if (CREATURE_CAN_HANDLE_IT(si, flags) != 0)
					break;
			}
		}
		//^1C9A:0777
	}	
	//^1C9A:0785
	return si;
}

//^14CD:1316
// SPX: _14cd_1316 renamed CREATURE_THINK_1316
X16 SkWinCore::CREATURE_THINK_1316(U8 xx, X16 yy, U8 zz)
{
	//^14CD:1316
	ENTER(28);
	//^14CD:131C
	Creature *xCreature = glbCurrentThinkingCreatureRec;	// bp04
	CreatureInfoData *xCreatureInfo = glbCurrentThinkingCreatureData;	// bp08

	SkD((DLV_CAI, "CAI: (CREATURE_THINK_1316) a#%03d P(%3d,%3d,x,%d)\n"
		, (Bitu)glbCurrentThinkingCreatureData->CreatureIndex()
		, (Bitu)(U8)xx, (Bitu)(U8)yy, (Bitu)zz
		));

	if ((xx & 0x40) != 0) {
		if (xCreatureInfo->x == zz) {
			//^14CD:1348
			return 1;
		}
		//^14CD:134E
		xx &= 0xbf;
	}
	//^14CD:1356
	X16 bp0c = xx & 0x80;
	xx &= 0x7f;
	X16 bp0a = 0;
	i16 si;
	i16 bp10;
	ObjectID di;
	U8 *bp18;
	OID_T *bp1c; // U16 *bp1c
	X16 bp0e;
	i16 bp12;
	i16 bp14;
	switch (xx) {
		case 1://^1380
			//^14CD:1380
			if (glbSomeMap_4976_4ee7 != glbCreatureMap)
				break;
			if (CALC_VECTOR_DIR(glbCreaturePosX, glbCreaturePosY, glbCreatureTimer.XcoordB(), glbCreatureTimer.YcoordB()) != glbCreatureDir)
				break;
			goto _1470;
		case 2://^13B6
			//^14CD:13B6
			bp0a = (glbSomeMap_4976_4ee7 == glbCreatureMap && glbCreatureTimer.XcoordB() == glbCreaturePosX && glbCreatureTimer.YcoordB() == glbCreaturePosY) ? 1 : 0;
			break;
		case 3://^13E4
			//^14CD:13E4
_13e4:
			if (CREATURE_CAN_HANDLE_ITEM_IN(yy, xCreature->GetPossessionObject(), -1) == OBJECT_END_MARKER)
				break;
			goto _1470;
		case 4://^1403
			//^14CD:1403
			bp0a = _4976_4f03;
			break;
		case 5://^1409
		case 13://^1409
			//^14CD:1409
			if (xCreature->TriggerMap() != glbSomeMap_4976_4ee7 || xCreature->TriggerX() != glbCreatureTimer.XcoordB() || xCreature->TriggerY() != glbCreatureTimer.YcoordB())
				break;
			//^14CD:144E
			if (xx != 0xd)
				goto _1470;
			goto _13e4;

		case 16://^1456
			//^14CD:1456
			if (xCreature->TriggerMap() != glbSomeMap_4976_4ee7)
				break;
			goto _1470;

		case 0://^1470
			//^14CD:1470
_1470:
			bp0a = 1;
			break;
		case 6://^1478
			//^14CD:1478
			if ((xCreature->iAnimFrame & (1 << yy)) == 0)
				break;
			//^14CD:148C
			goto _1470;

		case 7://^148E
			//^14CD:148E
			if (glbSomeMap_4976_4ee7 != glbCreatureMap)
				break;
			goto _1470;

		case 8://^149E
			//^14CD:149E
			for (si = 0; si < 4; si++) {
				//^14CD:14A2
				bp10 = GET_PLAYER_AT_POSITION(si);
				if (bp10 == -1)
					continue;
				di = glbChampionSquad[bp10].Possess(1);
				if (di != OBJECT_NULL) {
					//^14CD:14C6
					if (CREATURE_CAN_HANDLE_IT(di, 0xb) != 0)
						goto _1470;
				}
				//^14CD:14D4
				di = glbChampionSquad[bp10].Possess(0);
				if (di != OBJECT_NULL) {
					//^14CD:14E9
					if (CREATURE_CAN_HANDLE_IT(di, 0xb) != 0)
						goto _1470;
				}
				//^14CD:14FA
			}
			//^14CD:1500
			break;
		case 9://^1503
			//^14CD:1503
			if (_2c1d_09d9() < yy)
				break;
			//^14CD:1510
			goto _1470;

		case 10://^1513
			//^14CD:1513
			if (glbSomeMap_4976_4ee7 != glbCreatureMap)
				break;
			//^14CD:1521
			if (glbSomeMap_4976_4ee7 == glbCreatureMap && glbCreatureTimer.XcoordB() == glbCreaturePosX && glbCreatureTimer.YcoordB() == glbCreaturePosY)
				break;
			//^14CD:1545
			if (ABS16(glbCreatureTimer.XcoordB() - glbCreaturePosX) + ABS16(glbCreatureTimer.YcoordB() - glbCreaturePosY) > 1)
				break;
			//^14CD:1571
			si = CALC_VECTOR_DIR(glbCreatureTimer.XcoordB(), glbCreatureTimer.YcoordB(), glbCreaturePosX, glbCreaturePosY);
			bp10 = GET_TILE_VALUE(glbCreaturePosX + glbXAxisDelta[si], glbCreaturePosY + glbYAxisDelta[si]);
			if ((bp10 >> 5) != ttPit)
				break;
			//^14CD:15BD
			if ((bp10 & 8) == 0 || (bp10 & 1) != 0)
				break;
			goto _1470;

		case 11://^15D4
			//^14CD:15D4
			bp10 = (xCreature->b15_0_1() +2)&3;
			if ((_4976_4ef0 & 0x20) == 0)
				break;
			//^14CD:15F1
			if ((GET_TILE_VALUE(glbCreatureTimer.XcoordB() + glbXAxisDelta[bp10], glbCreatureTimer.YcoordB() + glbYAxisDelta[bp10]) >> 5) != ttDoor)
				break;
			//^14CD:1622
			if (_19f0_0d10(1, glbCreatureTimer.XcoordB(), glbCreatureTimer.YcoordB(), -1, -1, bp10) == 0)
				break;
			goto _1470;
			break;
		case 12://^1649
			//^14CD:1649
			if (glbSomeMap_4976_4ee7 != glbCreatureMap)
				break;
			//^14CD:1657
			di = GET_CREATURE_AT(glbCreatureTimerGetX + glbXAxisDelta[xCreature->b15_0_1()], glbCreatureTimerGetY + glbYAxisDelta[xCreature->b15_0_1()]);
			if (di == OBJECT_NULL)
				break;
			//^14CD:1696
			if (QUERY_CREATURE_AI_SPEC_FROM_TYPE(GET_ADDRESS_OF_RECORD4(di)->CreatureType())->IsStaticObject() == 0)
				break;
			//^14CD:16BA
			for (si = 0; si < 4; si++) {
				//^14CD:16BF
				bp12 = glbCreaturePosX + glbXAxisDelta[si];
				bp14 = glbCreaturePosY + glbYAxisDelta[si];
				bp10 = GET_TILE_VALUE(bp12, bp14);
				if ((bp10 >> 5) != 0 || (bp10 & 0x10) == 0)
					continue;
				//^14CD:16FA
				for (di = GET_TILE_RECORD_LINK(bp12, bp14); di != OBJECT_END_MARKER; di = GET_NEXT_RECORD_LINK(di)) {
					//^14CD:1709
					bp10 = di.DBType();
					if (bp10 == dbActuator) {
						//^14CD:1719
						if (GET_ADDRESS_OF_ACTU(di)->ActuatorType() == ACTUATOR_TYPE_SHOP_PANEL) { // 0x3f: Shop panel
							goto _1470;
						}
					}
					//^14CD:1733
					if (bp10 > dbActuator)
						break;
					//^14CD:1739
				}
				//^14CD:1747
			}
			//^14CD:1750
			break;
		case 14://^1753
			//^14CD:1753
			if ((xCreature->hp1 * 100) / glbAIDef->BaseHP > i16(yy))
				break;
			//^14CD:1792
			goto _1470;
		case 15://^1795
			//^14CD:1795
			bp0e = si = 0;
			bp18 = glbCurrentTileMap[0];
			bp1c = &dunGroundStacks[_4976_4c52[0]];
			for (bp12 = 0; bp12 < glbCurrentMapWidth; bp12++) {
				//^14CD:17CD
				for (bp14 = 0; bp14 < glbCurrentMapHeight; bp14++) {
					//^14CD:17D4
					if ((*(bp18++) & 0x10) == 0)
						continue;
					di = *(bp1c++);
					do {
						//^14CD:17EA
						if (di.DBType() == dbCreature) {
							//^14CD:17F7
							bp10 = GET_ADDRESS_OF_RECORD4(di)->CreatureType();
							if (bp10 == yy) {
								//^14CD:1810
								si++;
							}
							// 0x34 -> 'GUARD MINION'; 0x31 -> 'ATTACK MINION'
							else if (bp10 == CREATURE_GOOD_GUARD_MINION || bp10 == CREATURE_GOOD_ATTACK_MINION) {
								//^14CD:181F
								bp0e++;
							}
							break;
						}
						//^14CD:1824
					} while ((di = GET_NEXT_RECORD_LINK(di)) != OBJECT_END_MARKER);
					//^14CD:1832
				}
				//^14CD:183E
			}
			//^14CD:184A
			if (min_value(4, bp0e +1) <= si)
				break;
			goto _1470;

		case 17://^185F
			//^14CD:185F
			_19f0_045a(glbCreatureTimerGetX, glbCreatureTimerGetY);
			if ((_4976_521c & 0x10) == 0)
				break;
			if (_1c9a_1b16(yy, xCreature->iAnimSeq) != 0)
				break;
			goto _1470;
	}
	//^14CD:1891
	return (bp0c != 0) ? !bp0a : bp0a;
}


//^14CD:0389
// SPX: _14cd_0389 renamed CREATURE_THINK_0389
X8 SkWinCore::CREATURE_THINK_0389()
{
	//^14CD:0389
	ENTER(10);
	//^14CD:038E
	CreatureInfoData *xCreatureInfo = glbCurrentThinkingCreatureData; // bp08
	X16 si = 0;
	i8 bp09;
	i8 bp0a;
	if (_4976_514e.b0 != 0 && _4976_514e.b1 != 0 && _4976_514e.b3 != 0xff) {
		//^14CD:03BB
		_4976_4fd8->b18(0);
		if (xCreatureInfo->x != -1) {
			//^14CD:03CA
			bp09 = xCreatureInfo->x;
			bp0a = xCreatureInfo->y;
			sk3672 *bp04 = _4976_3672[RCJ(56,bp09)];
			_14cd_0f0a(bp04[bp0a].b5(), bp04[bp0a].b6(), bp09, _4976_514e.pv10);
		}
		//^14CD:0429
		if (_4976_4fd8->b18() == 0) {
			si = 0;
		}
		else {
			si = _1c9a_38a8();
			if (si != 0) {
				xCreatureInfo->w24 = _4976_4fee[0].w2;
			}
		}
	}
	//^14CD:0449
	return (si != 0) ? bp09 : 0xff;
}

//^14CD:0684
i8 SkWinCore::SELECT_CREATURE_3672()
{
	//^14CD:0684
	ENTER(14);
	//^14CD:068A
	CreatureInfoData *xCreatureInfo = glbCurrentThinkingCreatureData; // bp08
	X16 si = (CREATURE_THINK_0389() != 0xff) ? 1 : 0;
	X8 bp0a = 0xff;
	if ((tblAIStats01[_4976_4efa] & 1) != 0 || si == 0 || RAND02() == 0) {
		//^14CD:06CA
		_4976_4fd8->b18(0);
		_4976_5162 = _14cd_062e();
		i8 bp09 = xCreatureInfo->x;
		i8 bp0d = xCreatureInfo->y;
		_14cd_0550(_4976_4efe->pv2, bp09, bp0d, si);
		if (si != 0)
			_14cd_0457();
		if (_4976_4fd8->b18() != 0) {
			//^14CD:0712
			X16 di = ((glbCurrentThinkingCreatureRec->iAnimFrame & 0x2048) != 0) ? _4976_5163 : 0;
			if (glbCurrentThinkingCreatureRec->w10_c_c() != 0)
				di |= 0x20;
			//^14CD:0739
			ATLASSERT(_4976_4fd8->b18() < 16);
			U16 bp0c = FIND_WALK_PATH(glbCreatureTimer.XcoordB(), glbCreatureTimer.YcoordB(), di, 0, _4976_4fd8->b18(), _4976_4fee, glbCreatureWalkPath);
			_4976_4fd8->b19(U8(bp0c));
			if (bp0c != 0xffff) {
				//^14CD:0769
				skxxx9 *bp04 = &_4976_4fee[RCJ(16,bp0c)];
				bp0a = bp04->b14;
				xCreatureInfo->w24 = bp04->w2;
				_14cd_0276(bp04);
			}
			//^14CD:079D
			else if (_4976_5162 != 0) {
				return -3;
			}
		}
	}
	//^14CD:07A8
	if (bp0a != 0xff || si == 0) {
		//^14CD:07B2
		if (bp0a == 0xff) {
			bp0a = 0x11;
			_4976_514e.b1 = 0;
			_4976_514e.b3 = 0xff;
		}
		else {
			//^14CD:07C8
			if (xCreatureInfo->x != -1) {
				//^14CD:07D2
				i8 bp09 = xCreatureInfo->x;
				i8 bp0d = xCreatureInfo->y;
				if (bp09 == bp0a) {
					//^14CD:07E8
					bp0a = 0xff;
				}
				else {
					//^14CD:07EE
					xCreatureInfo->x = xactrNeedReset;
					xCreatureInfo->y = 0;
				}
			}
		}
	}
	//^14CD:07FB
	return bp0a;
}


//^14CD:0815
i8 SkWinCore::DECIDE_NEXT_XACT()
{
	//^14CD:0815
	ENTER(8);
	//^14CD:0819
	i8 bp05 = glbCurrentThinkingCreatureData->x;
	i8 cl = glbCurrentThinkingCreatureData->y;
	sk3672 *bp04 = _4976_3672[RCJ(56,bp05)];
	i8 bp07;
	for (; (bp07 = bp04[cl].b0()) < 0; cl++) {
		SkD((DLV_CAI, "CAI: ( ) a#%03d 3672[%3d,%3d] (%3d,%2d,%2d,%3d,%3d,%2d,%2d)\n"
			, (Bitu)glbCurrentThinkingCreatureData->CreatureIndex(), (Bitu)bp05, (Bitu)cl
			, (Bits)(i8)bp04[cl].b0_[0], (Bits)(i8)bp04[cl].b0_[1], (Bits)(i8)bp04[cl].b0_[2], (Bits)(i8)bp04[cl].b0_[3]
			, (Bits)(i8)bp04[cl].b0_[4], (Bits)    bp04[cl].b0_[5], (Bits)    bp04[cl].b0_[6]
			));

		//^14CD:0843
		if (bp07 != -10)
			continue;
		//^14CD:0849
		i8 bp06 = bp04[cl].b1();
		if (bp06 < 0 || bp06 >= 2)
			continue;
		//^14CD:0867
		glbCurrentThinkingCreatureData[bp06].w14 = bp04[cl].b2();
		//^14CD:088B
	}

	SkD((DLV_CAI, "CAI: (!) a#%03d 3672[%3d,%3d] (%3d,%2d,%2d,%3d,%3d,%2d,%2d)\n"
		, (Bitu)glbCurrentThinkingCreatureData->CreatureIndex(), (Bitu)bp05, (Bitu)cl
		, (Bits)(i8)bp04[cl].b0_[0], (Bits)(i8)bp04[cl].b0_[1], (Bits)(i8)bp04[cl].b0_[2], (Bits)(i8)bp04[cl].b0_[3]
		, (Bits)(i8)bp04[cl].b0_[4], (Bits)    bp04[cl].b0_[5], (Bits)    bp04[cl].b0_[6]
		));

	//^14CD:08A4
	glbCurrentThinkingCreatureData->x = bp05;
	glbCurrentThinkingCreatureData->y = cl;
	bp07 = bp04[cl].b0();
	if (bp07 == 86)
		printf("break");
	_4976_4ee8 = bp04[cl].b3();
    _4976_4eea = bp04[cl].b4();		
	//^14CD:08F0
	return bp07;
}
//^14CD:226D
// SPX: _14cd_226d renamed PROCEED_XACT_56
X8 SkWinCore::PROCEED_XACT_56()
{
	//^14CD:226D
	ENTER(0);
	//^14CD:2270
	return (CREATURE_GO_THERE(128, glbCreatureTimerGetX, glbCreatureTimerGetY, -1, -1, glbCurrentThinkingCreatureRec->b15_0_1()) != 0)
		? xactrAgain 
		: xactrYes;
}
//^14CD:22A3
// SPX: _14cd_22a3 renamed PROCEED_XACT_57
void SkWinCore::PROCEED_XACT_57()
{
	//^14CD:22A3
	ENTER(0);
	//^14CD:22A8
	i16 di = (RAND01() != 0) ? 1 : -1;
	X16 si = glbCurrentThinkingCreatureRec->b15_0_1();
	if (CREATURE_GO_THERE(128, glbCreatureTimerGetX, glbCreatureTimerGetY, -1, -1, (si +di)&3) != 0)
		return;
	//^14CD:22ED
	if (CREATURE_GO_THERE(128, glbCreatureTimerGetX, glbCreatureTimerGetY, -1, -1, (si -di)&3) != 0)
		return;
	//^14CD:2319
	_19f0_0559((si +di)&3);
	//^14CD:2327
	return;
}
//^14CD:232B
// SPX : _14cd_232b renamed PROCEED_XACT_59_76
X8 SkWinCore::PROCEED_XACT_59_76()
{
	//^14CD:232B
	ENTER(0);
	//^14CD:232F
	X16 si = _4976_4ee8;
	if (si == 0xffff)
		si = _4976_5152;
	//^14CD:233D
	if (_4976_4eea != 0 && CREATURE_CAN_HANDLE_ITEM_IN(si, glbCurrentThinkingCreatureRec->possession, -1) != OBJECT_END_MARKER)
		return xactrYes;
	//^14CD:2361
	_19f0_2165(128, glbCreatureTimerGetX, glbCreatureTimerGetY, glbCurrentThinkingCreatureData->w24.GetX(), glbCurrentThinkingCreatureData->w24.GetY(), -1, si);
	//^14CD:2392
	return _4976_4ee5;
}




//^14CD:2C23
// SPX : _14cd_2c23 renamed PROCEED_XACT_62
X8 SkWinCore::PROCEED_XACT_62()
{
	//^14CD:2C23
	ENTER(16);
	//^14CD:2C29
	X16 bp0e = 0;
	X16 bp02 = _4976_4ef2;
	if ((bp02 & 0x77) == 0)
		//^14CD:2C39
		return xactrNo;
	//^14CD:2C3E
	if (_4976_4eea == 1 && CREATURE_CAN_HANDLE_ITEM_IN(16, glbCurrentThinkingCreatureRec->possession, -1) != OBJECT_END_MARKER)
		return xactrYes;
	//^14CD:2C62
	X16 bp04;
	X16 bp06;
	if (_4976_4ee8 != 0) {
		//^14CD:2C69
		bp04 = glbCreatureTimerGetX;
		bp06 = glbCreatureTimerGetY;
	}
	else {
		//^14CD:2C78
		bp04 = glbCurrentThinkingCreatureData->w24.GetX();
		bp06 = glbCurrentThinkingCreatureData->w24.GetY();
	}
	//^14CD:2C93
	ObjectID si = FIND_TILE_ACTUATOR(bp04, bp06, -1, 0x30);
	if (si == OBJECT_NULL)
		return xactrNo;
	//^14CD:2CAE
	Actuator *bp0a = GET_ADDRESS_OF_ACTU(si);
	X16 bp0c = bp0a->ActuatorData();
	if (_4976_4eea == 2) {
		//^14CD:2CD2
		ObjectID bp10 = GET_CREATURE_AT(glbCreatureTimerGetX + glbXAxisDelta[glbCurrentThinkingCreatureRec->b15_0_1()], glbCreatureTimerGetY + glbYAxisDelta[glbCurrentThinkingCreatureRec->b15_0_1()]);
		if (bp10 != OBJECT_NULL) {
			//^14CD:2D11
			if (_1c9a_06bd(bp10, bp0c, (glbCurrentThinkingCreatureRec->b15_0_1() +2)&3) == NULL)
				return xactrNo;
			return xactrYes;
		}
	}
	//^14CD:2D3B
	si = GET_WALL_TILE_ANYITEM_RECORD(bp04, bp06);
	//^14CD:2D48
	while (si != OBJECT_END_MARKER && si.DBType() < dbWeapon)
		//^14CD:2D4A
		si = GET_NEXT_RECORD_LINK(si);
	//^14CD:2D65
	if (si != OBJECT_END_MARKER) {
		//^14CD:2D6D
		if (GET_DISTINCTIVE_ITEMTYPE(si) == bp0c) {
			//^14CD:2D79
			bp0e = 1;
		}
		else {
			//^14CD:2D81
			ObjectID di = si;
			//^14CD:2D83
			while (si != OBJECT_END_MARKER && si.DBType() <= dbMiscellaneous_item && GET_DISTINCTIVE_ITEMTYPE(si) != bp0c)
				//^14CD:2D85
				si = GET_NEXT_RECORD_LINK(si);
			//^14CD:2DAC
			if (si != OBJECT_END_MARKER) {
				//^14CD:2DB1
				bp0e = 1;
				//^14CD:2DB6
				while (di != OBJECT_END_MARKER && di.DBType() <= dbMiscellaneous_item && GET_DISTINCTIVE_ITEMTYPE(di) != bp0c) {
					//^14CD:2DB8
					si = GET_NEXT_RECORD_LINK(di);
					CUT_RECORD_FROM(di, NULL, bp04, bp06);
					APPEND_RECORD_TO(di, NULL, bp04, bp06);
					di = si;
				}
			}
		}
	}
	//^14CD:2E07
	if (bp0e == 0) {
		//^14CD:2E0D
		if (glbCurrentThinkingCreatureData->w14-- <= 0) {
			//^14CD:2E1D
			glbCurrentThinkingCreatureData->w14 = 3;
			glbCurrentThinkingCreatureData->Command = ccm1E;
			return xactrNo;
		}
		else {
			//^14CD:2E2B
			glbCurrentThinkingCreatureData->Command = ccm1D;
			return xactrNo;
		}
		//^14CD:2E37
		return xactrNo;
	}
	//^14CD:2E3A
	_4976_4ef2 &= 0x77;
	_19f0_2165(128, glbCreatureTimerGetX, glbCreatureTimerGetY, bp04, bp06, -1, 0x10);
	_4976_4ef2 = bp02;
	//^14CD:2E67
	return _4976_4ee5;
}
//^14CD:25B8
// SPX: _14cd_25b8 renamed PROCEED_XACT_63
X8 SkWinCore::PROCEED_XACT_63()
{
	// find an item around the creature. 
	// merchant uses this.

	// sk3672.b3 _4976_4ee8 = item pool. check CREATURE_CAN_HANDLE_ITEM_IN
	// sk3672.b4 _4976_4ee9 = dir. -1 for any dir. 
	//                              0 for front?
	//               33             1 for right side?
	//  MERCHANT   0-**-2  YOU      2 for opposite side,?
	//             0-**-2           3 for left side?
	//               11  TABLE

	//^14CD:25B8
	ENTER(6);
	//^14CD:25BE
	X8 bp04 = xactrNo;
	X16 bp06 = _4976_4ee8;
	if (bp06 == 0xffff)
		return bp04;
	//^14CD:25D0
	X16 di = glbCreatureTimerGetX;
	X16 si = glbCreatureTimerGetY;
	X8 bp03 = X8(_4976_4eea);
	if (bp03 != 0xff) {
		//^14CD:25E8
		bp03 = (glbCurrentThinkingCreatureRec->b15_0_1() +bp03 +2)&3;
	}
	//^14CD:25FD
	di += glbXAxisDelta[glbCurrentThinkingCreatureRec->b15_0_1()];
	si += glbYAxisDelta[glbCurrentThinkingCreatureRec->b15_0_1()];
	ObjectID bp02 = GET_CREATURE_AT(di, si);
	if (bp02 != OBJECT_NULL && CREATURE_CAN_HANDLE_ITEM_IN(bp06, GET_ADDRESS_OF_RECORD4(bp02)->GetPossessionObject(), bp03) != OBJECT_END_MARKER)
		//^14CD:2657
		bp04 = xactrYes;
	//^14CD:265B
	return bp04;
}
//^14CD:240E
// SPX: _14cd_240e renamed PROCEED_XACT_64
X8 SkWinCore::PROCEED_XACT_64()
{
	//^14CD:240E
	ENTER(0);
	//^14CD:2413
	X16 di;
	if (glbCurrentThinkingCreatureRec->possession == OBJECT_END_MARKER || ((di = _4976_4ef2) & 8) == 0)
		//^14CD:2428
		return xactrNo;
	//^14CD:242C
	X16 si = _4976_4ee8;
	if (si == 0xffff)
		si = 0x3f;
	//^14CD:2439
	if (CREATURE_CAN_HANDLE_ITEM_IN(si, glbCurrentThinkingCreatureRec->possession, -1) == OBJECT_END_MARKER)
		return xactrNo;
	//^14CD:2454
	_4976_4ef2 &= 8;
	_19f0_2165(0x81, glbCreatureTimerGetX, glbCreatureTimerGetY, -1, -1, glbCurrentThinkingCreatureRec->b15_0_1(), si);
	_4976_4ef2 = di;
	//^14CD:2486
	return _4976_4ee5;
}
//^14CD:2E6E
// SPX: _14cd_2e6e renamed PROCEED_XACT_65
X8 SkWinCore::PROCEED_XACT_65()
{
	//^14CD:2E6E
	ENTER(6);
	//^14CD:2E74
	glbCurrentThinkingCreatureData->w12 = Ax3::Invalid;
	X16 si = glbCurrentThinkingCreatureRec->b15_0_1();
	X16 bp02 = glbCreatureTimerGetX + (glbXAxisDelta[si] << 1);
	X16 di = glbCreatureTimerGetY + (glbYAxisDelta[si] << 1);
	ObjectID bp04 = GET_CREATURE_AT(bp02, di);
	X8 bp05;
	if ((bp04 != OBJECT_NULL && (QUERY_CREATURE_AI_SPEC_FLAGS(bp04) & 1) == 0) || (glbCurrentMapIndex == glbCreatureMap && bp02 == glbCreaturePosX && di == glbCreaturePosY)) {
		//^14CD:2EED
		bp05 = xactrYes;
	}
	else {
		//^14CD:2EF3
		glbCurrentThinkingCreatureData->Command = ccm1D;
		bp05 = xactrAgain;
	}
	//^14CD:2F00
	return bp05;
}


//^14CD:2662
X16 SkWinCore::_14cd_2662(i8 dir)
{
	//^14CD:2662
	ENTER(6);
	//^14CD:2668
	ObjectID si = OBJECT_END_MARKER;
	X16 bp04 = glbCreatureTimerGetX;
	X16 bp06 = glbCreatureTimerGetY;
	if (dir != -1) {
		//^14CD:2681
		dir = (glbCurrentThinkingCreatureRec->b15_0_1() +dir +2)&3;
	}
	//^14CD:2696
	bp04 += glbXAxisDelta[glbCurrentThinkingCreatureRec->b15_0_1()];
	bp06 += glbYAxisDelta[glbCurrentThinkingCreatureRec->b15_0_1()];
	ObjectID bp02 = GET_CREATURE_AT(bp04, bp06);
	if (bp02 != OBJECT_NULL) {
		//^14CD:26D5
		for (si = GET_ADDRESS_OF_RECORD4(bp02)->GetPossessionObject(); si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
			//^14CD:26E8
			i16 di = si.DBType();
			if ((di <= dbCreature || di >= dbMissile) && di != dbContainer)
				continue;
			//^14CD:2701
			if (dir != -1 && si.Dir() != dir)
				continue;
			//^14CD:2715
			if (CREATURE_CAN_HANDLE_IT(si, 16) != 0)
				continue;
			//^14CD:2723
			if (CREATURE_CAN_HANDLE_IT(si, 7) == 0)
				break;
			//^14CD:2731
		}
	}
	//^14CD:273F
	return (si != OBJECT_END_MARKER) ? 1 : 0;
}

//^14CD:274F
// SPX: _14cd_274f renamed PROCEED_XACT_66
X8 SkWinCore::PROCEED_XACT_66()
{
	//^14CD:274F
	ENTER(2);
	//^14CD:2754
	X8 bp01;
	if (_14cd_2662(2) != 0) {
		//^14CD:275F
		bp01 = xactrAgain;
		if (glbCurrentThinkingCreatureData->w14-- <= 5) {
			//^14CD:2774
			glbCurrentThinkingCreatureData->w14 = 9;
			glbCurrentThinkingCreatureData->Command = ccm1F;
		}
		else {
			//^14CD:2782
			glbCurrentThinkingCreatureData->Command = ccm1D;
		}
	}
	else {
		//^14CD:278D
		X16 si = _4976_4ee8;
		_4976_4ee8 = 16;
		_4976_4eea = 2;
		if (PROCEED_XACT_63() == 0xfe || (si != 0 && (_4976_4ee8 = 7) != 0 && PROCEED_XACT_63() == 0xfe)) {
			//^14CD:27BD
			bp01 = xactrYes;
		}
		//^14CD:27C3
		else if (glbCurrentThinkingCreatureData->w14-- <= 0) {
			//^14CD:27D3
			glbCurrentThinkingCreatureData->w14 = 5;
			bp01 = xactrNo;
			glbCurrentThinkingCreatureData->Command = ccm1E;
		}
		else {
			//^14CD:27E4
			if (glbCurrentThinkingCreatureData->w14 > 5)
				glbCurrentThinkingCreatureData->w14 -= 5;
			//^14CD:27F4
			bp01 = xactrNo;
			glbCurrentThinkingCreatureData->Command = ccm1D;
		}
	}
	//^14CD:2801
	return bp01;
}
//^14CD:2807
// _14cd_2807 renamed _14cd_2807_CREATURE
U16 SkWinCore::_14cd_2807_CREATURE(ObjectID *ref, skxxxi *pv)
{
	//^14CD:2807
	ENTER(0);
	//^14CD:280A
	if (CREATURE_CAN_HANDLE_IT(*ref, pv->w2) != 0) {
		//^14CD:2822
		if (pv->w0 == -1)
			pv->w0 = 0;
		pv->w0 += _48ae_05ae_CREATURE(
			GET_DISTINCTIVE_ITEMTYPE(*ref), 
			glbCurrentThinkingCreatureRec->CreatureType(), 
			glbCurrentThinkingCreatureRec->iAnimSeq, pv->w4, pv->w6, 
			(pv->w8 != 0) ? ADD_ITEM_CHARGE(*ref, 0) : 0xffff);
	}
	//^14CD:2882
	return 0;
}

//^14CD:2886
// _14cd_2886 renamed _14cd_2886_CREATURE
i16 SkWinCore::_14cd_2886_CREATURE(ObjectID *ref, X16 xx, i8 dir, X16 ss, X16 tt, X16 ww)
{
	//^14CD:2886
	ENTER(14);
	//^14CD:288A
	skxxxi bp0e;
	bp0e.w0 = -1;
	bp0e.w2 = xx;
	bp0e.w4 = ss;
	bp0e.w6 = tt;
	bp0e.w8 = ww;
	ObjectID *bp04;
	OVERSEE_RECORD(ref, dir, &bp04, (pfnOversee_t)&SkWinCore::_14cd_2807_CREATURE, &bp0e, 0, 1);
	//^14CD:28CD
	return bp0e.w0;
}

//^14CD:28D2
// SPX: _14cd_28d2 renamed PROCEED_XACT_67
X8 SkWinCore::PROCEED_XACT_67()
{
	//^14CD:28D2
	ENTER(40);
	//^14CD:28D8
	CreatureInfoData *bp04 = glbCurrentThinkingCreatureData;
	X8 bp0f = xactrNo;
	X16 bp0c = glbCurrentThinkingCreatureRec->b15_0_1();
	if (_14cd_2662(2) != 0) {
		//^14CD:2902
		if (--bp04->w14 <= 6) {
			//^14CD:2912
			bp04->w14 = RAND02() +9;
			bp04->Command = ccm1F;
		}
		else {
			//^14CD:2929
			bp04->Command = ccm1D;
		}
		//^14CD:2934
	}
	else {
		//^14CD:2937
		ObjectID bp0a = GET_CREATURE_AT(glbCreatureTimerGetX + glbXAxisDelta[bp0c], glbCreatureTimerGetY + glbYAxisDelta[bp0c]);
		if (bp0a != OBJECT_NULL) {
			//^14CD:2967
			bp04->Command = ccm1D;
			ObjectID *bp08 = &GET_ADDRESS_OF_RECORD4(bp0a)->possession;
			i16 si = _14cd_2886_CREATURE(bp08, 16, U8(bp0c), 0, 0, 0);
			i16 bp0e = _14cd_2886_CREATURE(bp08, 7, U8(bp0c), 0, 0, 0);
			if (bp0e != -1) {
				//^14CD:29BD
				si = (si == -1) ? bp0e : (si +bp0e);
			}

			do {
				//^14CD:29CB
				if (si == -1) {
					//^14CD:29D0
					bp04->w16 = 0;
				}
				else {
					//^14CD:29DC
					i16 di = _14cd_2886_CREATURE(bp08, 0x10, (bp0c +2)&3, 1, 1, 0);
					U8 bp28[22];
					i16 bp12;
					X16 bp16 = di = _48ae_0767_MONEY_VALUE(di, 0x12, bp28, &bp12, 1);
					bp0e = bp04->w16;
					bp04->w16 = si;
					if (di > 16) {
						//^14CD:2A2A
						di = i16(di - (i32(di) * RAND16(0x10)) / 100);
					}
					//^14CD:2A50
					X16 bp14 = (100 * si) / di;
					if (si != bp0e && U16(si) < U16(di)) {
						//^14CD:2A7C
						if (bp0e != 0xffff && si <= bp0e) {
							//^14CD:2A87
							if (RAND02() == 0 || ((RAND()&7) +0x4c) < bp14) {
								//^14CD:2AA0
								bp04->w12 = Ax3::Frm(0);
								if ((((RAND16(max_value(1, 100 -bp14)) < 5) ? 1 : 0) ^ (!RAND02())) != 0) {
									//^14CD:2ADC
									bp04->Command = ccm20;
								}
								else {
									//^14CD:2AE6
									bp04->Command = ccm1B;
								}
							}
						}
						else {
							//^14CD:2AF0
							bp04->Command = ccm1D;
						}
					}
					//^14CD:2AFB
					else if (U16(si) >= U16(di)) {
						//^14CD:2AFF
						bp04->w14 = min_value(si, bp16);
						bp04->Command = ccm1C;
						bp0f = xactrYes;
						//^14CD:2B1A
						break;
					}
					//^14CD:2B1C
					else if (bp04->w14-- <= 0) {
						if (true
							//^14CD:2B2B
							&& i16(bp14) > 0x4c
							//^14CD:2B31
							&& ((((RAND16(max_value(1, 100 -bp14)) < 5U) ? 1 : 0) ^ (!RAND02())) != 0)
						) {
							//^14CD:2B64
							bp04->Command = ccm20;
							//^14CD:2B6C
							break;
						}
						else {
							//^14CD:2B6E
							bp04->Command = ccm1B;
							//^14CD:2B76
							break;
						}
					}
					else {
						//^14CD:2B78
						if (bp04->w14 > 6) {
							//^14CD:2B82
							bp04->w14 -= 4;
						}
						//^14CD:2B87
						bp04->Command = ccm1D;
					}
					//^14CD:2B8F
					bp0f = xactrAgain;
				}
			} while (false);
		}
	}
	//^14CD:2B93
	return bp0f;
}
//^14CD:2F07
// SPX: _14cd_2f07 renamed PROCEED_XACT_68
X8 SkWinCore::PROCEED_XACT_68()
{
	//^14CD:2F07
	ENTER(34);
	//^14CD:2F0D
	X8 bp0d = xactrNo;
	X16 bp06 = glbCurrentThinkingCreatureRec->b15_0_1();
	if (_14cd_2662((_4976_4ee8 +2)&3) != 0) {
		//^14CD:2F30
		bp0d = xactrNo;
		glbCurrentThinkingCreatureData->Command = ccm1F;
	}
	else {
		//^14CD:2F40
		ObjectID bp0c = GET_CREATURE_AT(glbCreatureTimerGetX + glbXAxisDelta[bp06], glbCreatureTimerGetY +glbYAxisDelta[bp06]);
		if (bp0c != OBJECT_NULL) {
			//^14CD:2F70
			ObjectID *bp04 = &GET_ADDRESS_OF_RECORD4(bp0c)->possession;
			bp06 = (bp06 + _4976_4ee8)&3;
			i16 di = _14cd_2886_CREATURE(bp04, 0x10, (bp06 +2)&3, 1, 1, 0);
			U8 bp22[18];
			i16 bp10;
			di = _48ae_0767_MONEY_VALUE(di, 0x12, bp22, &bp10, 1);
			i16 bp08 = _14cd_2886_CREATURE(bp04, 7, (bp06 +2)&3, 0, 0, 0);
			if (bp08 != -1)
				//^14CD:2FEC
				di += bp08;
			else
				//^14CD:2FF1
				bp08 = 0;
			//^14CD:2FF6
			i16 si = _14cd_2886_CREATURE(bp04, 0x10, i8(bp06), 0, 0, 0);
			i16 bp0a = _14cd_2886_CREATURE(bp04, 7, i8(bp06), 0, 0, 0);
			if (si == -1)
				si = 0;
			//^14CD:3034
			if (bp0a != -1)
				si += bp0a;
			//^14CD:303D
			if (U16(glbCurrentThinkingCreatureData->w14 +bp08) > U16(si)) {
				glbCurrentThinkingCreatureData->Command = ccm1B;
			}
			else {
				//^14CD:3053
				if (glbCurrentThinkingCreatureData->w16 != si) {
					//^14CD:305D
					glbCurrentThinkingCreatureData->Command = ccm1C;
					glbCurrentThinkingCreatureData->w16 = si;
				}
				//^14CD:3066
				glbCurrentThinkingCreatureData->w12 = Ax3::Frm(max_value(0, si -di));
				bp0d = xactrYes;
			}
		}
	}
	//^14CD:3080
	return bp0d;
}
//^14CD:2398
// SPX: _14cd_2398 renamed PROCEED_XACT_69
void SkWinCore::PROCEED_XACT_69()
{
	//^14CD:2398
	ENTER(4);
	//^14CD:239D
	CreatureInfoData *bp04 = glbCurrentThinkingCreatureData;
	X16 si = glbCurrentThinkingCreatureRec->b15_0_1();
	bp04->w24.SetX((glbCreatureTimerGetX + glbXAxisDelta[si])&31);
	bp04->w24.SetY((glbCreatureTimerGetY + glbYAxisDelta[si])&31);
	bp04->b29 = U8(_4976_4ee8);
	bp04->Command = (bp04->b29 == 1) ? ccm16 : ccm15;
	//^14CD:240B
	return;
}


//^1C9A:078B
void SkWinCore::_1c9a_078b(ObjectID *ref, X16 xx, i8 dir)
{
	//^1C9A:078B
	ENTER(12);
	//^1C9A:0791
	ObjectID bp02 = *ref;
	ObjectID bp04;
	for (ObjectID si = bp02; (si = bp02) != OBJECT_END_MARKER; ) {
		//^1C9A:079D
		bp02 = GET_NEXT_RECORD_LINK(si);
		i16 bp06 = si.DBType();
		if ((bp06 <= dbCreature || bp06 >= dbMissile) && bp06 != dbContainer)
			continue;
		//^1C9A:07C7
		if (dir != -1 && si.Dir() != U8(dir))
			continue;
		//^1C9A:07DE
		X16 bp0c = CREATURE_CAN_HANDLE_IT(si, xx);
		if (bp06 == dbContainer) {
			//^1C9A:07F4
			if (IS_CONTAINER_MONEYBOX(si) != 0 && bp0c == 0)
				continue;
			//^1C9A:0808
			Container *bp0a = GET_ADDRESS_OF_RECORD9(si);
			_1c9a_078b(&bp0a->w2, xx, -1);
			if (bp0c == 0)
				continue;
			//^1C9A:0831
			bp04 = bp0a->GetContainedObject();
			ObjectID di;
			//^1C9A:083B
			while ((di = bp04) != OBJECT_END_MARKER) {
				//^1C9A:083D
				bp04 = GET_NEXT_RECORD_LINK(di);
				CUT_RECORD_FROM(di, &bp0a->w2, -1, 0);
				di.Dir(U8(dir));
				APPEND_RECORD_TO(di, ref, -1, 0);
				//^1C9A:0882
			}
		}
		//^1C9A:088C
		if (bp0c != 0) {
			CUT_RECORD_FROM(si, ref, -1, 0);
			DEALLOC_RECORD(si);
		}
		//^1C9A:08AC
	}
	//^1C9A:08B9
	return;
}

//^14CD:3087
// SPX: _14cd_3087 renamed PROCEED_XACT_71
X8 SkWinCore::PROCEED_XACT_71()
{
	//^14CD:3087
	ENTER(4);
	//^14CD:308C
	ObjectID *bp04 = &glbCurrentThinkingCreatureRec->possession;
	X16 si = _4976_4eea;
	if (si != 0xfffe && (si != 0xffff || ((si = _4976_5154) != 0xffff)) && *bp04 != OBJECT_END_MARKER) {
		//^14CD:30BD
		_1c9a_078b(bp04, si, -1);
	}
	//^14CD:30CF
	if (*bp04 == OBJECT_END_MARKER)
		//^14CD:30D8
		return xactrNo;
	//^14CD:30DC
	si = _4976_4ee8;
	if (si == 0xffff && (si = _4976_5152) == 0xffff)
		//^14CD:30F0
		return xactrYes;
	//^14CD:30F4
	if (CREATURE_CAN_HANDLE_ITEM_IN(si, *bp04, -1) == OBJECT_END_MARKER)
		return xactrNo;
	//^14CD:310D
	_19f0_2165(0x81, glbCreatureTimerGetX, glbCreatureTimerGetY, glbCreatureTimerGetX, glbCreatureTimerGetY, -1, si);
	//^14CD:3133
	return _4976_4ee5;
}
//^14CD:3139
// SPX: _14cd_3139 renamed PROCEED_XACT_73
X8 SkWinCore::PROCEED_XACT_73()
{
	//^14CD:3139
	ENTER(16);
	//^14CD:313F
	Creature *bp08 = glbCurrentThinkingCreatureRec;
	X16 si = _4976_4eea;
	X16 bp10;
	X16 bp0a;
	X16 bp0c;
	X16 di;
	X8 bp0d;
	sk1bf9 *bp04;
	switch (si) {
		case 0://^3162
		case 1://^3162
		case 2://^3162
		case 16://^3162
		case 17://^3162
		case 18://^3162
			//^14CD:3162
			bp10 = si&16;
			si &= 15;
			bp0a = 1 << _4976_4ee8;
			bp0c = ((bp0a & bp08->iAnimFrame) == bp0a) ? 1 : 0;
			di = bp08->iAnimFrame;
			if (si == 0) {
				//^14CD:319B
				bp08->iAnimFrame &= ~bp0a;
			}
			//^14CD:31A6
			else if (si == 1) {
				//^14CD:31AB
				bp08->iAnimFrame |= bp0a;
			}
			//^14CD:31B5
			if (bp10 == 0 && bp08->iAnimFrame != di)
				//^14CD:31C4
				glbCurrentThinkingCreatureData->Command = ccm33;
			//^14CD:31CD
			if (bp0c != 0)
				return xactrYes;
			//^14CD:31D6
			return 0xfd;
		case 3://^31DB
		case 4://^31DB
			//^14CD:31DB
			bp0d = (si == 3) ? 0x13 : 0x14;
			di = bp08->iAnimFrame;
			bp04 = _4976_514e.pv10;
			if (bp04 == NULL)
				break;
			do {
				//^14CD:3203
				if (bp04->b12 == bp0d) {
					//^14CD:320F
					if (bp04->w4 == 0) {
						//^14CD:3216
						bp08->iAnimFrame |= 1 << bp04->b6_0_f();
					}
					//^14CD:3228
					else if (bp04->w4 == 1) {
						//^14CD:3232
						bp08->iAnimFrame &= ~(1 << bp04->b6_0_f());
					}
				}
				//^14CD:3244
			} while ((bp04++)->b13 != 0);
			//^14CD:3252
			if (bp08->iAnimFrame != di)
				//^14CD:325B
				glbCurrentThinkingCreatureData->Command = ccm33;
			//^14CD:3264
			if (bp08->iAnimFrame != di)
				//^14CD:326D
				return xactrYes;
			//^14CD:3272
			return 0xfd;
		case 5://^3278
		case 6://^3278
		case 7://^3278
		case 8://^3278
		case 9://^3278
		case 10://^3278
		case 11://^3278
		case 12://^3278
		case 13://^3278
		case 14://^3278
		case 15://^3278
			break;
	}
	//^14CD:3278
	return xactrNo;
}
//^14CD:32A4
// SPX: _14cd_32a4 renamed PROCEED_XACT_74
i8 SkWinCore::PROCEED_XACT_74()
{
	//^14CD:32A4
	ENTER(6);
	//^14CD:32AA
	CreatureInfoData *bp04 = glbCurrentThinkingCreatureData;
	X16 si = glbAIDef->w22_c_f();
	X16 bp06 = (si != 0 && (RAND()&15) < ((glbCurrentThinkingCreatureRec->w10_d_d() != 0) ? (si >> 2) : si)) ? 1 : 0;
	//^14CD:32F5
	if (CREATURE_THINK_381c() == 0) {
		//^14CD:3301
		if (bp04->w24.GetX() == glbCreatureTimerGetX && bp04->w24.GetY() == glbCreatureTimerGetY && bp04->w24.GetMap() == glbCurrentMapIndex)
			//^14CD:3339
			return xactrYes;
		//^14CD:333E
		si = CALC_VECTOR_DIR(glbCreatureTimerGetX, glbCreatureTimerGetY, bp04->w24.GetX(), bp04->w24.GetY());
		if (glbCurrentThinkingCreatureRec->b15_0_1() == si)
			return xactrYes;
		//^14CD:337B
		if (bp06 != 0 && RAND01() != 0) {
			//^14CD:338A
_338a:
			bp04->Command = ccmNeutral;
			//^14CD:3392
			return xactrAgain;
		}
		//^14CD:3396
		_19f0_0559(si);
	}
	else {
		//^14CD:339F
		if (bp06 != 0)
			goto _338a;
		X16 di = 0x80;
		if (glbCurrentThinkingCreatureRec->w10_c_c() != 0)
			di |= 0x20;
		//^14CD:33B8
		CREATURE_GO_THERE(di, glbCreatureTimerGetX, glbCreatureTimerGetY, -1, -1, bp04->b27);
	}
	//^14CD:33DA
	return _4976_4ee5;
}
//^14CD:2162
// SPX: _14cd_2162 renamed PROCEED_XACT_75
X8 SkWinCore::PROCEED_XACT_75()
{
	//^14CD:2162
	ENTER(10);
	//^14CD:2168
	CreatureInfoData *bp04 = glbCurrentThinkingCreatureData;
	sk4f04 *bp08 = _14cd_10d2(_4976_514e.pv10, _4976_5150);
	X8 bp09 = _4976_5151;
	X16 si;
	if (bp09 == 8) {
		//^14CD:2199
		si = 2;
	}
	//^14CD:219E
	else if (bp09 == 9) {
		//^14CD:21A4
		si = 3;
	}
	else {
		//^14CD:21A9
		si = 0;
	}
	//^14CD:21AB
	bp04->ItemToThrow = U8(_4976_5152);
	X16 di = glbAIAttacksSpells;
	if ((di & 8) != 0 && (bp08->b5 > 0 || bp08->b6 != 0))
		//^14CD:21D0
		glbAIAttacksSpells &= 0xfff7;
	//^14CD:21D6
	glbAIAttacksSpells &= _4976_5154;
	_19f0_0891(si |0x80, glbCreatureTimerGetX, glbCreatureTimerGetY, bp04->w24.GetX(), bp04->w24.GetY(), 0xffff);
	glbAIAttacksSpells = di;
	//^14CD:2213
	return _4976_4ee5;
}
//^14CD:33E1
// SPX: _14cd_33e1 renamed PROCEED_XACT_77
X8 SkWinCore::PROCEED_XACT_77()
{
	//^14CD:33E1
	ENTER(4);
	//^14CD:33E5
	_4976_4fd8->b18(0);
	sk1bf9 *bp04 = _4976_514e.pv10;
	if (bp04 != NULL) {
		do {
			//^14CD:33FB
			if (bp04->b12 == 0x11 && bp04->w4 != 0xffff) {
				//^14CD:340C
				_14cd_0f3c(0x14, bp04, bp04, 0x11, 0, Ax3::Invalid, (U8)0xffff, 0);
			}
			//^14CD:342B
		} while ((bp04++)->b13 != 0);
		//^14CD:3439
		if (_4976_4fd8->b18() != 0) {
			//^14CD:3440
			ATLASSERT(_4976_4fd8->b18() < 16);
			_4976_4fd8->b19(U8(FIND_WALK_PATH(glbCreatureTimerGetX, glbCreatureTimerGetY, 3, 0, _4976_4fd8->b18(), _4976_4fee, glbCreatureWalkPath)));
			if (_4976_4fd8->b19() != 0xff)
				//^14CD:346C
				return xactrYes;
		}
	}
	//^14CD:3470
	return xactrNo;
}
//^14CD:3474
// SPX: _14cd_3474 renamed PROCEED_XACT_78
X8 SkWinCore::PROCEED_XACT_78()
{
	//^14CD:3474
	ENTER(0);
	//^14CD:3478
	_4976_4ee5 = xactrNo;
	if (glbCurrentMapIndex == glbCreatureMap) {
		//^14CD:3486
		X16 si = CALC_VECTOR_DIR(glbCreatureTimerGetX, glbCreatureTimerGetY, glbCreaturePosX, glbCreaturePosY);
		if ((_0cee_04e5(si, 1, 0, glbCreatureTimerGetX, glbCreatureTimerGetY) >> 5) != 0) {
			//^14CD:34C6
			_19f0_0559(si);
		}
	}
	//^14CD:34CD
	return _4976_4ee5;
}
//^14CD:221A
// SPX: _14cd_221a renamed PROCEED_XACT_79
void SkWinCore::PROCEED_XACT_79()
{
	//^14CD:221A
	ENTER(8);
	//^14CD:221E
	CreatureInfoData *bp04 = glbCurrentThinkingCreatureData;
	X16 bp06 = RAND01();
	X16 bp08 = RAND02();
	bp04->ItemToThrow = i8(0x82);
	bp04->Command = (bp06 != 0) ? ccmCastSpell2 : ccmCastSpell1;
	bp04->b27 = U8(bp08);
	bp04->b28 = U8(bp06)&3;
	bp04->b32 = 0;
	//^14CD:226B
	return;
}
//^14CD:34D3
// SPX: _14cd_34d3 renamed PROCEED_XACT_80
X8 SkWinCore::PROCEED_XACT_80()
{
	//^14CD:34D3
	ENTER(2);
	//^14CD:34D9
	X16 di = _4976_4ee8;
	X16 bp02 = (di == 0) ? 0 : 6;
	X16 si = _4976_4eec;
	_4976_4eec |= 0x1800;
	CREATURE_GO_THERE(bp02|0x80, glbCreatureTimerGetX, glbCreatureTimerGetY, -1, -1, (di + glbCurrentThinkingCreatureRec->b15_0_1()) & 3);
	_4976_4eec = si;
	//^14CD:352E
	return _4976_4ee5;
}

//^14CD:3535
// SPX: _14cd_3535 renamed PROCEED_XACT_81
X8 SkWinCore::PROCEED_XACT_81()
{
	//^14CD:3535
	ENTER(4);
	//^14CD:3539
	CreatureInfoData *bp04 = glbCurrentThinkingCreatureData;
	_19f0_2813(_4976_5154|0x80, glbCreatureTimerGetX, glbCreatureTimerGetY, bp04->w24.GetX(), bp04->w24.GetY(), -1, _4976_5152);
	return _4976_4ee5;
	//^14CD:3580
}

//^14CD:3582
X16 SkWinCore::EXCHANGE_MONEYBOX(X16 onlyTest, ObjectID rlWallet) 
{
	// return 0 if no exchange is useful
	// return 1 if any exchange is useful

	//^14CD:3582
	ENTER(46);
	//^14CD:3588
	X16 bp06 = 0;
	i16 moneyQuantityPerType[MONEY_ITEM_MAX];
	COUNT_BY_COIN_TYPES(rlWallet, moneyQuantityPerType);
	i16 moneyAmount = 0;
	i16 si = 0;
	for (; si < glbCountMoneyItems; si++) {
		//^14CD:35A5
		moneyAmount += glbMoneyItemsValueTable[si] * moneyQuantityPerType[si];
		//^14CD:35BD
	}
	//^14CD:35C4
	i16 bp2e[MONEY_ITEM_MAX];
	ZERO_MEMORY(bp2e, sizeof(bp2e));
	si = glbCountMoneyItems -1;
	i16 bp02;
	ObjectID bp04;
	for (; si >= 0 && moneyAmount > 0; si--) {
		//^14CD:35EC
		while ((bp02 = glbMoneyItemsValueTable[si]) <= moneyAmount) {
			//^14CD:35DD
			bp2e[si]++;
			moneyAmount -= bp02;
			//^14CD:35EC
		}
		//^14CD:35FB
		bp06 |= (moneyQuantityPerType[si] != bp2e[si]) ? 1 : 0;
		if (bp06 != 0 && onlyTest == 1)
			return 1;
	}
	//^14CD:3633
	if (bp06 == 0 || onlyTest == 1) {
		//^14CD:363F
		return bp06;
	}
	//^14CD:3644
	for (si = 0; si < glbCountMoneyItems; si++) {
		//^14CD:3648
		while ((bp04 = TAKE_COIN_FROM_WALLET(rlWallet, si)) != OBJECT_NULL) {
			//^14CD:3648
			DEALLOC_RECORD(bp04);
			//^14CD:3651
		}
		//^14CD:3665
	}
	//^14CD:366B
	for (si = 0; si < glbCountMoneyItems; si++) {
		//^14CD:366F
		while (bp2e[si]-- != 0) {
			//^14CD:366F
			bp04 = ALLOC_NEW_DBITEM(glbMoneyItemsIDTable[si]);
			//^14CD:3680
			if (bp04 != OBJECT_NULL) {
				//^14CD:3685
				ADD_COIN_TO_WALLET(rlWallet, bp04);
			}
			//^14CD:3692
		}
		//^14CD:36A6
	}
	//^14CD:36AC
	return 1;
}

//^0CEE:2821
X16 SkWinCore::ADD_COIN_TO_WALLET(ObjectID rlMoneyBox, ObjectID rlCoin)
{
	//^0CEE:2821
	ENTER(12);
	//^0CEE:2827
	ObjectID di = rlCoin;
	if (IS_CONTAINER_MONEYBOX(rlMoneyBox) == 0)
		//^0CEE:2836
		return 0;
	if (IS_MISCITEM_CURRENCY(di) == 0)
		return 0;
	//^0CEE:2847
	Miscellaneous_item *bp08 = GET_ADDRESS_OF_RECORDA(di);
	Container *bp0c = GET_ADDRESS_OF_RECORD9(rlMoneyBox);
	ObjectID si = bp0c->GetContainedObject();
	for (; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
		//^0CEE:286B
		if (si.DBType() == dbMiscellaneous_item) {
			//^0CEE:2878
			Miscellaneous_item *bp04 = GET_ADDRESS_OF_RECORDA(si);
			if (bp04->ItemType() != bp08->ItemType())
				continue;
			if (bp04->Charge() >= 0x3f)
				continue;
			bp04->Charge(bp04->Charge() +1);
			QUEUE_DEALLOC_RECORD(di); // this causes many assertion prompts!
			break;
		}
		//^0CEE:28C9
		return 0;
		//^0CEE:28CC
	}
	//^0CEE:28D9
	if (si != OBJECT_END_MARKER)
		return 1;
	//^0CEE:28DE
	APPEND_RECORD_TO(di, &bp0c->w2, -1, 0);
	//^0CEE:28FB
	return 1;
}
//^14CD:36B3
// SPX: _14cd_36b3 renamed PROCEED_XACT_82
X8 SkWinCore::PROCEED_XACT_82()
{
	//^14CD:36B3
	ENTER(36);
	//^14CD:36B9
	X8 bp09 = xactrNo;
	ObjectID bp12 = OBJECT_NULL;
	X16 bp0e = glbCurrentThinkingCreatureRec->b15_0_1();
	ObjectID bp0c = GET_CREATURE_AT(glbCreatureTimerGetX + glbXAxisDelta[bp0e], glbCreatureTimerGetY + glbYAxisDelta[bp0e]);
	ObjectID si;
	//^14CD:36F5
	if (bp0c != OBJECT_NULL) {
		//^14CD:3700
		glbCurrentThinkingCreatureData->Command = ccm1D;
		Creature *bp04 = GET_ADDRESS_OF_RECORD4(bp0c);
		//^14CD:3718
		if (_4976_4ee8 == 0) {
			//^14CD:3722
			ObjectID bp12 = CREATURE_CAN_HANDLE_ITEM_IN(0x3e, GET_ADDRESS_OF_RECORD4(bp0c)->GetPossessionObject(), (glbCurrentThinkingCreatureRec->b15_0_1() +2)&3);
			if (bp12 == OBJECT_END_MARKER)
				bp12 = OBJECT_NULL;
			//^14CD:375A
			i16 di = _14cd_2886_CREATURE(&bp04->possession, 0x10, (bp0e +2)&3, 0, 1, 1);
			if ((di != 0 && di == -1) || bp12 == -1) {
				//^14CD:378E
				glbCurrentThinkingCreatureData->w16 = max_value(0, di);
				bp09 = xactrYes;
			}
		}
		else {
			//^14CD:37A7
			i16 *bp08;
			if (_4976_4ee8 == 1) {
				//^14CD:37AE
				bp08 = &glbCurrentThinkingCreatureData->w16;
			}
			else if (_4976_4ee8 == 2) {
				//^14CD:37C1
				bp08 = &glbCurrentThinkingCreatureData->w14;
			}
			else {
				//^14CD:37CD
				bp08 = reinterpret_cast<i16 *>(&glbCurrentThinkingCreatureData->w12);
			}
			//^14CD:37DD
			for (si = glbCurrentThinkingCreatureRec->possession; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
				//^14CD:37E7
				if (IS_CONTAINER_MONEYBOX(si) != 0) {
					//^14CD:37F2
					bp12 = si;
					EXCHANGE_MONEYBOX(0, bp12);
				}
				//^14CD:3800
			}
			//^14CD:380E
			_4976_4ee8 = 0x47;
			if (*bp08 <= 0) {
				//^14CD:381D
				bp09 = PROCEED_XACT_64();
				if (bp09 != xactrAgain)
					//^14CD:382B
					bp09 = xactrYes;
			}
			else {
				//^14CD:3832
				i16 bp10;
				U8 bp24[18];
				_48ae_0767_MONEY_VALUE(*bp08, 0x12, bp24, &bp10, 0);
				*bp08 = -1;
				if (bp10 <= 0) {
					//^14CD:385C
					bp09 = xactrYes;
				}
				else {
					//^14CD:3862
					i16 di = bp10 -1;
					do {
						//^14CD:3868
						si = ALLOC_NEW_DBITEM(glbMoneyItemsIDTable[bp24[di]]);
						if (si == OBJECT_NULL)
							break;
						//^14CD:3882
						if (bp12 != OBJECT_NULL) {
							//^14CD:3888
							ADD_COIN_TO_WALLET(bp12, si);
						}
						else {
							//^14CD:3895
							APPEND_RECORD_TO(si, &glbCurrentThinkingCreatureRec->possession, -1, 0);
						}
						//^14CD:38AC
					} while (di-- != 0);
					//^14CD:38B3
					PROCEED_XACT_64();
					bp09 = xactrAgain;
				}
			}
		}
	}
	//^14CD:38BB
	return bp09;
}

//^14CD:38C2
// SPX: _14cd_38c2 renamed PROCEED_XACT_83
i8 SkWinCore::PROCEED_XACT_83()
{
	//^14CD:38C2
	ENTER(2);
	//^14CD:38C6
	i8 bp01 = xactrYes;
	if (glbCurrentThinkingCreatureRec->w10_7_7() == 0 && _4976_4ee8 == 0) {
		//^14CD:38DD
		bp01 = xactrNo;
	}
	else {
		//^14CD:38E3
		glbCurrentThinkingCreatureData->Command = BETWEEN_VALUE(0, _4976_4ee8, 2) +0x23;
		//^14CD:38FD
		if (glbCurrentThinkingCreatureRec->w10_7_7() != 0 && _4976_4ee8 == 1) {
			//^14CD:3910
			bp01 = xactrAgain;
		}
	}
	//^14CD:3914
	return bp01;
	//^14CD:3917
}

//^14CD:3919
// SPX: somewhat used to discard consumed or unimportant item?
// SPX: _14cd_3919 renamed PROCEED_XACT_84
X8 SkWinCore::PROCEED_XACT_84()
{
	//^14CD:3919
	ENTER(6);
	//^14CD:391F
	X8 bp05 = xactrNo;
	ObjectID si = glbCurrentThinkingCreatureRec->GetPossessionObject();
	if (si == OBJECT_END_MARKER)
		return bp05;
	//^14CD:3933
	GenericRecord *_bp04 = GET_ADDRESS_OF_RECORD(si);
	X16 di;
	switch (si.DBType()) {
	case dbCloth://^3958
		{
			//^14CD:3958
			Cloth *bp04 = _bp04->castToCloth();
			di = !bp04->Important();
			break;
		}
	case dbWeapon://^396E
		{
			//^14CD:396E
			Weapon *bp04 = _bp04->castToWeapon();
			di = !bp04->Important();
			break;
		}
	case dbMiscellaneous_item://^3970
		{
			//^14CD:3970
			Miscellaneous_item *bp04 = _bp04->castToMisc();
			di = (bp04->Important() != 0 || QUERY_GDAT_FOOD_VALUE_FROM_RECORD(si) != 0) ? 1 : 0;
			break;
		}
	case dbPotion://^3995
		{
			//^14CD:3995
			Potion *bp04 = _bp04->castToPotion();
			di = !bp04->VisiblePower();
			break;
		}
	case dbScroll://^399E
	case dbContainer://^399E
	default:
		{
			//^14CD:399E
			di = 0;
			break;
		}
	}
	//^14CD:39A0
	if (di != 0) {
		//^14CD:39A4
		CUT_RECORD_FROM(si, &glbCurrentThinkingCreatureRec->possession, -1, 0);
		DEALLOC_RECORD(si);
		glbCurrentThinkingCreatureData->Command = ccmNeutral;
		bp05 = xactrYes;
	}
	else {
		//^14CD:39D1
		_19f0_2165(0x81, glbCreatureTimerGetX, glbCreatureTimerGetY, glbCreatureTimerGetX, glbCreatureTimerGetY, -1, 6);
	}
	//^14CD:39F8
	return bp05;
}
//^14CD:3A0B
// SPX: _14cd_3a0b renamed PROCEED_XACT_85
i8 SkWinCore::PROCEED_XACT_85()
{
	//^14CD:3A0B
	ENTER(6);
	//^14CD:3A11
	ObjectID si = GET_TILE_RECORD_LINK(glbCreatureTimerGetX, glbCreatureTimerGetY);
	U16 di = 0; // defaulting to 0
	for (; si != OBJECT_END_MARKER && (di = si.DBType()) <= dbActuator; si = GET_NEXT_RECORD_LINK(si)) {
		//^14CD:3A26
		if (di == dbText) {
			//^14CD:3A2B
			Text *bp04 = GET_ADDRESS_OF_RECORD2(si);
			if (bp04->TextMode() == 1) {
				U16 bp06 = bp04->TextIndex();
				if (bp04->SimpleTextExtUsage() == 1) {
					//^14CD:3A61
					glbCurrentThinkingCreatureData->ItemToThrow = i8(bp06 & 0xff);
					glbCurrentThinkingCreatureData->Command = ccmTransform;
					return xactrYes;
				}
			}
		}
		//^14CD:3A77
	}
	//^14CD:3A94
	_13e4_0360(glbCurrentThinkingCreatureID, glbCreatureTimerGetX, glbCreatureTimerGetY, 0x13, 1);
	glbCurrentThinkingCreatureData->Command = ccm33;
	return xactrNo;
	//^14CD:3ABB
}
//^14CD:3ABF
// SPX: _14cd_3abf renamed PROCEED_XACT_86
X8 SkWinCore::PROCEED_XACT_86() 
{
	//^14CD:3ABF
	ENTER(4);
	//^14CD:3AC3
	CreatureInfoData *bp04 = glbCurrentThinkingCreatureData;
	bp04->b32 = _4976_514e.b4();
	bp04->ItemToThrow = _4976_514e.b6();
	bp04->Command = U8(_4976_4ee8) + ccmExplode;
	//^14CD:3AEA
	return xactrYes;
}
//^14CD:3AEE
// SPX: _14cd_3aee renamed PROCEED_XACT_72_87_88
void SkWinCore::PROCEED_XACT_72_87_88()
{
	//^14CD:3AEE
	ENTER(0);
	//^14CD:3AF1
	U8 dl = U8(_4976_4ee8);
	if (dl == U8(ccmInv))
		dl = U8(_4976_5152);
	//^14CD:3AFF
	glbCurrentThinkingCreatureData->Command = dl;
	//^14CD:3B09
	return;
}
//^14CD:3B0B
// SPX: Hit this one in front of Rocky
// SPX: _14cd_3b0b renamed PROCEED_XACT_89
X8 SkWinCore::PROCEED_XACT_89() 
{
	//^14CD:3B0B
	ENTER(0);
	//^14CD:3B0F
	X16 si = _4976_5154;
	//^14CD:3B13
	//_19f0_0d10(si |0x80u, glbCreatureTimerGetX, glbCreatureTimerGetY, glbCurrentThinkingCreatureData->w24.X, glbCurrentThinkingCreatureData->w24.Y, -1);
	_19f0_0d10(si |0x80u, glbCreatureTimerGetX, glbCreatureTimerGetY, glbCurrentThinkingCreatureData->w24.GetX(), glbCurrentThinkingCreatureData->w24.GetY(), -1); // without MSCV Property
	//^14CD:3B46
	return _4976_4ee5;
	//^14CD:3B49
}
//^14CD:3B4C
// SPX: _14cd_3b4c renamed PROCEED_XACT_90
X8 SkWinCore::PROCEED_XACT_90()
{
	//^14CD:3B4C
	ENTER(2);
	//^14CD:3B50
	X8 bp01;
	return bp01 = ((RAND16(0x64) < _4976_4ee8) ? xactrYes : xactrNo);
}
//^14CD:3B69
// SPX: _14cd_3b69 renamed PROCEED_XACT_91
X8 SkWinCore::PROCEED_XACT_91()
{
	//^14CD:3B69
	ENTER(0);
	//^14CD:3B6C
	return (false
		|| CREATURE_CAN_HANDLE_ITEM_IN(_4976_4ee8, glbCurrentThinkingCreatureRec->GetPossessionObject(), -1) != OBJECT_END_MARKER
		|| CREATURE_CAN_HANDLE_ITEM_IN(_4976_4eea, glbCurrentThinkingCreatureRec->GetPossessionObject(), -1) != OBJECT_END_MARKER
		) ? xactrYes : xactrNo;
}
//^14CD:248D
// SPX: _14cd_248d renamed PROCEED_XACT_70
X8 SkWinCore::PROCEED_XACT_70()
{
	//^14CD:248D
	ENTER(8);
	//^14CD:2493
	CreatureInfoData *bp04 = glbCurrentThinkingCreatureData;
	X8 bp05 = xactrNo;
	X16 bp08 = _4976_4ee8;
	if (bp08 == 0xffff)
		bp08 = 0x3f;
	//^14CD:24B4
	bp04->w24.SetX(glbCreatureTimer.b6_0_4());
	bp04->w24.SetY(glbCreatureTimer.b7_0_4());
	bp04->b28 = (glbCurrentThinkingCreatureRec->b15_0_1() +2)&3;
	bp04->ItemToThrow = bp08;
	bp04->w24.SetX(bp04->w24.GetX() + glbXAxisDelta[glbCurrentThinkingCreatureRec->b15_0_1()]);
	bp04->w24.SetY(bp04->w24.GetY() + glbYAxisDelta[glbCurrentThinkingCreatureRec->b15_0_1()]);
	ObjectID si = GET_CREATURE_AT(bp04->w24.GetX(), bp04->w24.GetY());
	if (si != OBJECT_NULL) {
		//^14CD:2578
		if (CREATURE_CAN_HANDLE_ITEM_IN(bp08, GET_ADDRESS_OF_RECORD4(si)->possession, bp04->b28) == OBJECT_END_MARKER) {
			bp05 = xactrYes;
		}
		else {
			bp04->Command = ccmTakeMerchandise;
			bp05 = xactrAgain;
		}
	}
	//^14CD:25B1
	return bp05;
}

//^14CD:1FF3
i8 SkWinCore::PROCEED_XACT(i8 xact)
{
	SkD((DLV_CAI, "CAI: S-- a#%03d PROCEED_XACT_%d\n"
		, (Bitu)glbCurrentThinkingCreatureData->CreatureIndex(), (Bitu)xact, (Bitu)xact
		));
	//^14CD:1FF3
	ENTER(2);
	//^14CD:1FF7
	i8 bp01 = -2;
	//printf("XACT = %d\n", xact);
	switch (xact) {
	case 56://^2013
		//^14CD:2013
		bp01 = PROCEED_XACT_56();
		break;
	case 57://^201B
		//^14CD:201B
		PROCEED_XACT_57();
		break;
	case 58://^2023	// XACT 58 tells creature to destroy itself ?
		//^14CD:2023
		glbCurrentThinkingCreatureData->Command = ccmDestroy;
		break;
	case 76://^202F
		//^14CD:202F
		_4976_4ee8 = 0xffff;
		_4976_4eea = 0;
		bp01 = PROCEED_XACT_59_76();
		break;
	case 59://^203B
		//^14CD:203B
		bp01 = PROCEED_XACT_59_76();
		break;
	case 60://^2043 // still. do nothing
		//^14CD:2043
		glbCurrentThinkingCreatureData->Command = ccmNeutral;
		break;
	case 62://^204F
		//^14CD:204F
		bp01 = PROCEED_XACT_62();
		break;
	case 63://^2057
		//^14CD:2057
		bp01 = PROCEED_XACT_63();
		break;
	case 64://^205F
		//^14CD:205F
		bp01 = PROCEED_XACT_64();
		break;
	case 65://^2067
		//^14CD:2067
		bp01 = PROCEED_XACT_65();
		break;
	case 66://^206F
		//^14CD:206F
		bp01 = PROCEED_XACT_66();
		break;
	case 67://^2077
		//^14CD:2077
		bp01 = PROCEED_XACT_67();
		break;
	case 68://^207F
		//^14CD:207F
		bp01 = PROCEED_XACT_68();
		break;
	case 69://^2087
		//^14CD:2087
		PROCEED_XACT_69();
		break;
	case 70://^208F
		//^14CD:208F
		bp01 = PROCEED_XACT_70();
		break;
	case 71://^2096
		//^14CD:2096
		bp01 = PROCEED_XACT_71();
		break;
	case 73://^209D
		//^14CD:209D
		bp01 = PROCEED_XACT_73();
		break;
	case 74://^20A4
		//^14CD:20A4
		bp01 = PROCEED_XACT_74();
		break;
	case 75://^20AB
		//^14CD:20AB
		bp01 = PROCEED_XACT_75();
		break;
	case 77://^20B2
		//^14CD:20B2
		bp01 = PROCEED_XACT_77();
		break;
	case 78://^20B9
		//^14CD:20B9
		bp01 = PROCEED_XACT_78();
		break;
	case 79://^20C0
		//^14CD:20C0
		PROCEED_XACT_79();
		break;
	case 80://^20C7
		//^14CD:20C7
		bp01 = PROCEED_XACT_80();
		break;
	case 81://^20CE
		//^14CD:20CE
		bp01 = PROCEED_XACT_81();
		break;
	case 82://^20D5
		//^14CD:20D5
		bp01 = PROCEED_XACT_82();
		break;
	case 83://^20DC
		//^14CD:20DC
		bp01 = PROCEED_XACT_83();
		break;
	case 84://^20E3
		//^14CD:20E3
		bp01 = PROCEED_XACT_84();
		break;
	case 85://^20EA
		//^14CD:20EA
		bp01 = PROCEED_XACT_85();
		break;
	case 86://^20F1
		//^14CD:20F1
		bp01 = PROCEED_XACT_86();
		break;
	case 72://^20F8
	case 87://^20F8
	case 88://^20F8
		//^14CD:20F8
		PROCEED_XACT_72_87_88();
		break;
	case 89://^20FF
		//^14CD:20FF
		bp01 = PROCEED_XACT_89();
		break;
	case 90://^2106
		//^14CD:2106
		bp01 = PROCEED_XACT_90();
		break;
	case 91://^210D
		//^14CD:210D
		bp01 = PROCEED_XACT_91();
		break;
	case 61://^2115
		break;
	}
	//^14CD:2115
	SkD((DLV_CAI, "CAI: --S a#%03d r %d   %s \n"
		, (Bitu)glbCurrentThinkingCreatureData->CreatureIndex(), (Bits)bp01, getXActrName((Bits)bp01)
		));

	return bp01;
}
//^14CD:08F5
// SPX: _14cd_08f5 renamed CREATURE_THINK_08F5
X16 SkWinCore::CREATURE_THINK_08F5(i8 xx)
{
	//^14CD:08F5
	ENTER(10);
	//^14CD:08FA
	i8 cl = xx;
	CreatureInfoData *xCreatureInfo = glbCurrentThinkingCreatureData;	 // bp08
	i8 bp09 = -1;
	X16 si = 0;
	bp09 = xCreatureInfo->x;
	i8 bp0a = xCreatureInfo->y;
	sk3672 *bp04 = _4976_3672[RCJ(56,bp09)];
	cl = (cl == xactrYes)
		? bp04[bp0a].b1()
		: bp04[bp0a].b2();
	if (cl == xactrYes || cl == xactrNo) {
		//^14CD:096F
		xCreatureInfo->x = xactrNeedReset;
		xCreatureInfo->y = 0;
		return si = 1;
	}
	//^14CD:0981
	if (cl <= xactrNext1 && cl >= xactrNext2) {
		//^14CD:098B
		if (cl != xactrRetry) {
			if (cl == xactrNext2) {
				//^14CD:0995
				bp0a += 2;
			}
			//^14CD:099F
			else if (cl == xactrNext1) {
				//^14CD:09A4
				bp0a += +1;
			}
			else {
				//^14CD:09A8
				bp0a += -1;
			}
			//^14CD:09B2
			si = 1;
		}
	}
	else {
		//^14CD:09B7
		si = (bp0a != cl) ? 1 : 0;
		bp0a = cl;
	}
	//^14CD:09CC
	xCreatureInfo->x = bp09;
	xCreatureInfo->y = bp0a;
	//^14CD:09DD
	return si;
}

//^14CD:09E2
// SPX: _14cd_09e2 renamed CREATURE_THINK_09E2
void SkWinCore::CREATURE_THINK_09E2()
{
	//^14CD:09E2
	ENTER(26);
	//^14CD:09E8
	CreatureInfoData *xCreatureInfo = glbCurrentThinkingCreatureData;	// bp08
	Creature *xCreature = glbCurrentThinkingCreatureRec;	// bp0c
	SELECT_CREATURE_37FC();
	X16 bp14 = tblAIStats01[_4976_4efa];
	if ((bp14 & 0x40) != 0) {
		_4976_5163 = 0;
	}
	else if ((bp14 & 0x20) != 0) {
		_4976_5163 = 4;
	}
	else {
		_4976_5163 = 5;
	}
	//^14CD:0A2F
	if (_4976_4efe->pv2 == _4976_1d6c) {
		//^14CD:0A43
	_0a43:
		xCreatureInfo->Command = ccmNeutral;
		return;
	}
	//^14CD:0A4E
	if (_4976_4efe->pv2 == _4976_1d65) {
		i8 bp0e = RAND() & 7;
		if (bp0e > 3) {
			//^14CD:0A76
			goto _0a43;
		}
		//^14CD:0A78
		if (bp0e != 0) {
			//^14CD:0A81
			xCreatureInfo->b27 = xCreature->b15_0_1();
			xCreatureInfo->w24.SetX(glbCreatureTimer.XcoordB() + glbXAxisDelta[xCreature->b15_0_1()]);
			xCreatureInfo->w24.SetY(glbCreatureTimer.YcoordB() + glbYAxisDelta[xCreature->b15_0_1()]);
			U8 bp11 = GET_TILE_VALUE(xCreatureInfo->w24.GetX(), xCreatureInfo->w24.GetY()) >> 5;
			TELE_inf bp1a;
			if (bp11 == 0 || bp11 == 7 || (bp11 == 5 && GET_TELEPORTER_DETAIL(&bp1a, U8(xCreatureInfo->w24.GetX()), U8(xCreatureInfo->w24.GetY())) != 0 && IS_CREATURE_ALLOWED_ON_LEVEL(glbCurrentThinkingCreatureID, bp1a.b4) == 0)) {
				//^14CD:0B4E
				goto _0a43;
			}
			//^14CD:0B51
			xCreatureInfo->Command = ccm02;
			return;
		}
		//^14CD:0B5C
		_19f0_0559(U16(glbGameTick) & 3);
		return;
	}
	//^14CD:0B6C
	_13e4_01a3();
	_4976_520e = ALLOC_MEMORY_RAM(0x100, afDefault, 0x400);
	X16 si = !RAND02();
	i8 bp0d;
	if ((bp14 & 0x40) == 0) {
		//^14CD:0BA0
		if (CREATURE_GO_THERE(_4976_5163, glbCreatureTimer.XcoordB(), glbCreatureTimer.YcoordB(), glbCreatureTimer.XcoordB(), glbCreatureTimer.YcoordB(), -1) == 0) {
			//^14CD:0BCF
			if (false
				|| (_4976_5163 == 5 && _19f0_13aa(glbCreatureTimer.XcoordB(), glbCreatureTimer.YcoordB()) != 0) 
				|| _4976_4f03 == 0
				|| _19f0_0891(0, glbCreatureTimer.XcoordB(), glbCreatureTimer.YcoordB(), -1, -1, xCreature->b15_0_1()) == 0
				|| RAND16((glbAIDef->GetPoisonResistance() >> 2) +1) <= 1
			) {
				//^14CD:0C3E
				xCreature->w10_d_d(1);
				do {
					//^14CD:0C47
					if ((bp14 & 0x10) != 0) {
						//^14CD:0C4E
						if (((xCreature->w10_3_3() != 0 || si != 0) ? RAND01() : !RAND02()) != 0) {
							//^14CD:0C72
							if (_19f0_13aa(glbCreatureTimer.XcoordB(), glbCreatureTimer.YcoordB()) != 0) {
								xCreatureInfo->x = xactrNeedReset;
								xCreatureInfo->y = 0;
								xCreatureInfo->Command = ccm55;
								goto _0eee;
							}
						}
					}
					//^14CD:0C9E
					X16 di = 4;
					U8 bp10 = (CREATURE_THINK_381c() > 0) 
						? xCreatureInfo->b27 
						: ((RAND01() != 0) 
							? ((xCreature->b15_0_1() +2) & 3) 
							: RAND02()
						);
					//^14CD:0CD4
					i8 bp0e = (RAND01() != 0) ? 1 : -1;
					for (; di-- != 0; bp10 = (bp10 +bp0e) & 3) {
						//^14CD:0CE8
						if (CREATURE_GO_THERE(((si != 0) ? 0 : _4976_5163)|0x80, glbCreatureTimer.XcoordB(), glbCreatureTimer.YcoordB(), -1,  -1, bp10) != 0) {
							//^14CD:0D1A
							xCreatureInfo->x = xactrNeedReset;
							xCreatureInfo->y = 0;
							goto _0eee;
						}
						//^14CD:0D2A
					}
					//^14CD:0D3C
				} while ((si++) == 0 && RAND01() != 0);
			}
		}
	}
	//^14CD:0D4F
	xCreatureInfo->Command = ccmInv;
	bp0d = SELECT_CREATURE_3672();
	X8 bp11;
	X8 bp12;
	i16 di;
	sk3672 *bp04;
	if (bp0d == -3) {
		if ((_4976_5162 & 0x80) == 0) {
			//^14CD:0D6F
			xCreatureInfo->Command = ccmNeutral;
		}
		else {
			//^14CD:0D7A
		    if (CREATURE_THINK_08F5(-3) != 0)
				_4976_514e.b1 = 0;
			//^14CD:0D8A
			xCreatureInfo->Command = ccm33;
		}
	}
	else {
		//^14CD:0D95
		i8 bp0e;
		if (bp0d >= 0 && bp0d < MAX3672) {
			//^14CD:0DA1
			SkD((DLV_CAI, "CAI: ( ) a#%03d 3672[xxx] <- %d\n"
				, (Bitu)glbCurrentThinkingCreatureData->CreatureIndex(), bp0d
				));

			xCreatureInfo->x = bp0d; // new 3672[xxx]
			bp0e = 0;
			xCreatureInfo->y = 0;
		}
		else {
			ATLASSERT(xCreatureInfo->x >= 0);
			//^14CD:0DB6
			bp0d = xCreatureInfo->x;
			bp0e = xCreatureInfo->y;
		}
		//^14CD:0DC7
		bp04 = _4976_3672[RCJ(MAX3672,bp0d)];
		bp11 = bp04[bp0e].b5() & 0x1f;
		bp12 = bp04[bp0e].b6();
		di = 0;
		si = 0;
		do {
			//^14CD:0E0E
			bp0d = xCreatureInfo->x;
			bp0e = xCreatureInfo->y;
			sk3672 *bp04 = _4976_3672[RCJ(56,bp0d)];
			if (si != 0) {
				X8 bp10 = bp04[bp0e].b5() & 0x1f;
				if ((bp10 != bp11 || bp11 == 1) && (bp10 != 0) & (bp11 != 1 || bp10 != 1 || bp04[bp0e].b6() != bp12)) {
					//^14CD:0E85
					xCreatureInfo->Command = ccm33;
					break;
				}
			}
			//^14CD:0E8F
			i8 bp0f = DECIDE_NEXT_XACT();
			bp0d = PROCEED_XACT(bp0f);
			//^14CD:0EA0
			if (bp0d != xactrAgain) {
				si |= CREATURE_THINK_08F5(bp0d);
			}
			//^14CD:0EAC
			if (si != 0)
				_4976_514e.b1 = 0;
			if (di++ > 0x20)
				xCreatureInfo->Command = ccmNeutral;
			//^14CD:0EC5
			if (xCreatureInfo->Command == i8(ccmInv) && bp0d != xactrAgain && xCreatureInfo->x == xactrNeedReset)
				xCreatureInfo->Command = ccmNeutral;
			//^14CD:0EE1
		} while (xCreatureInfo->Command == i8(ccmInv));
	}
	//^14CD:0EEE
_0eee:
	_4976_520e = NULL;
	DEALLOC_UPPER_MEMORY(0x100);
	//^14CD:0F06
	return;
}
//^13E4:01A3
void SkWinCore::_13e4_01a3()
{
	//^13E4:01A3
	ENTER(14);
	//^13E4:01A9
	if (_4976_5161 != 0)
		return;
	//^13E4:01B3
	_4976_5161 = 1;
	AIDefinition *bp04 = glbAIDef;
	Creature *bp08 = glbCurrentThinkingCreatureRec;
	if (_4976_4efa == 0xffff) {
		//^13E4:01D9
		_4976_4efa = QUERY_GDAT_CREATURE_WORD_VALUE(bp08->CreatureType(), 1);
	}
	//^13E4:01ED
	_4976_4eec = bp04->w10;
	glbAIAttacksSpells = bp04->AttacksSpells;
	_4976_4ef0 = bp04->w16;
	_4976_4ef2 = bp04->w12;
	_4976_4ef4 = bp04->w18;
	if (_1c9a_08bd(bp08) != 0)
		_4976_4eec &= 0x7fff;
	//^13E4:022A
	_4976_4f02 = 1;
	ZERO_MEMORY(_4976_5164, sizeof(_4976_5164));
	_4976_520e = NULL;
	_4976_520c = 4;
	_4976_5162 = 0;
	_4976_4ef8 = QUERY_GDAT_CREATURE_WORD_VALUE(bp08->CreatureType(), 7);
	i16 si = (i8)((U8)glbGameTick - glbCurrentThinkingCreatureData->b4);
	if (si < 0)
		si += 0x100;
	//^13E4:0281
    X16 di = (15 -bp04->w22_0_3()) << 1;
	//^13E4:0294
	_4976_4f03 = (RAND16(di +1) + (((0x0147 | 0x0CD5) != 0) ? (di >> 2) : 0) <= (U16)si) ? 1 : 0;
	U16 bp0e;
	//^13E4:02C1
	if (_3e74_5673(glbCurrentThinkingCreatureID.DBIndex()|0x20000000, &bp0e, 0) == 0) {
		//^13E4:02E3
		ZERO_MEMORY(&_4976_514e, sizeof(sk514e));
		_4976_5151 = 0xff;
		_4976_5156 = Ax3::Invalid;
	}
	else {
		//^13E4:0300
		sk514e *bp0c = reinterpret_cast<sk514e *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp0e));
		COPY_MEMORY(bp0c, &_4976_514e, sizeof(sk514e));
	}
	//^13E4:0325
	return;
}


//^1887:00F8
U16 SkWinCore::CREATURE_WALK_NOW()
{
	// return 0 if walk is possible or have done
	// return 1 if keep to stay

	//^1887:00F8
	ENTER(4);
	//^1887:00FD
	CreatureInfoData *bp04 = glbCurrentThinkingCreatureData;
	if (glbAIDef->PushWhenMoving() != 0) {
		//^1887:0115
		bp04->Command = ccmPushBack;
		bp04->b28 = 4;
		CREATURE_ATTACKS_PARTY();
	}
	//^1887:0127
	if (CREATURE_GO_THERE(bp04->b32|0x80, glbCreatureTimerGetX, glbCreatureTimerGetY, glbCreatureTempTargetX, glbCreatureTempTargetY, bp04->b27) != 0) {
		//^1887:015C
		//SPX: is b9 a special for pit/tele ghost?
		//X16 si = glbAIDef->b9 & 0x40;
		X16 si = glbAIDef->CanActivatePitTeleport();
		if (si != 0) {
			//^1887:016E
			OPERATE_PIT_TELE_TILE(glbCreatureTimerGetX, glbCreatureTimerGetY, 1);
		}
		//^1887:0183
		if (MOVE_RECORD_TO(glbCurrentThinkingCreatureID, glbCreatureTimerGetX, glbCreatureTimerGetY, bp04->w24.GetX(), bp04->w24.GetY()) != 0) {
			_4976_4ee6 = 1;
			return 0;
		}
		//^1887:01BC
		glbCreatureTimer.XcoordB((U8)_4976_5826);
		glbCreatureTimer.YcoordB((U8)_4976_5828);
		glbSomeMap_4976_4ee7 = (U8)_4976_581c;
		_1c9a_0648(glbSomeMap_4976_4ee7);
		if (si != 0) {
			//^1887:01DB
			OPERATE_PIT_TELE_TILE(glbCreatureTimerGetX, glbCreatureTimerGetY, 0);
		}
		//^1887:01F0
		if (_4976_514e.b1 > 0)
			//^1887:01F7
			_4976_514e.b1--;
		//^1887:01FB
		return 0;
	}
	//^1887:01FF
	return 1;
}
//^1887:0205
X16 SkWinCore::CREATURE_CCM03()
{
	//^1887:0205
	ENTER(0);
	//^1887:0209
	X16 si;
	switch (glbCurrentThinkingCreatureData->b31) {
		case 0: //^_021d
			//^1887:021D
			CREATURE_CCM06();
			si = 0;
			break;
		case 1: //^_0226
			//^1887:0226
			si = CREATURE_WALK_NOW();
	}
	//^1887:022C
	glbCurrentThinkingCreatureData->b31++;
	//^1887:0234
	return si;
}
//^1887:0239
// SPX: Creature jumps
X16 SkWinCore::CREATURE_JUMPS()
{
	//^1887:0239
	ENTER(6);
	//^1887:023E
	CreatureInfoData *bp04 = glbCurrentThinkingCreatureData;
	X16 bp06;
	X16 si;
	switch (bp04->b31) {
		case 0: //^_0267
			//^1887:0267
			bp04->b31 = 1;
			bp06 = _4976_514e.b1;
			si = CREATURE_WALK_NOW();
			_4976_514e.b1 = U8(bp06);
			bp04->w24.SetX(bp04->w24.GetX() + glbXAxisDelta[bp04->b27]);
			bp04->w24.SetY(bp04->w24.GetY() + glbYAxisDelta[bp04->b27]);
			break;
		case 1: //^_02d9
			//^1887:02D9
			si = CREATURE_WALK_NOW();
			if (si != 0) {
				bp04->b31 = 2;
				break;
			}
			bp04->b31 = 3;
			break;
		case 2: //^_02f7
			//^1887:02F7
			si = 0;
			bp04->b31 = 3;
			// SPX: creature has to be moved on another tile?
			if (MOVE_RECORD_TO(glbCurrentThinkingCreatureID, glbCreatureTimerGetX, glbCreatureTimerGetY, glbCreatureTimerGetX, glbCreatureTimerGetY) != 0)
				_4976_4ee6 = 1;
			//^1887:032E
			glbCreatureTimer.XcoordB(U8(_4976_5826));
			glbCreatureTimer.YcoordB(U8(_4976_5828));
			glbSomeMap_4976_4ee7 = U8(_4976_581c);
			_1c9a_0648(glbSomeMap_4976_4ee7);
			break;
	}
	//^1887:0349
	return si;
}
//^1887:05AA
void SkWinCore::CREATURE_CCM06()
{
	//^1887:05AA
	ENTER(0);
	//^1887:05AE
	i16 si = glbCurrentThinkingCreatureData->b29;
	if (((glbCurrentThinkingCreatureRec->b15_0_1() -si)&3) == 2) {
		si = ((RAND()&2) +si +1)&3;
	}
	//^1887:05DE
	glbCurrentThinkingCreatureRec->b15_0_1(si&3);
	//^1887:05F0
	return;
}
//^075F:1C74
i16 SkWinCore::CREATURE_ATTACKS_PLAYER(Creature *ref, U16 player)
{
	//^075F:1C74
	ENTER(16);
	//^075F:1C7A
	Champion *champion;	//*bp04
    if (player >= glbChampionsCount || (champion = &glbChampionSquad[player])->curHP() == 0)
		return 0;
	//^075F:1CA3
	i16 si = 0;
	X16 bp0e = dunMapLocalHeader->Difficulty() << 1;
	AIDefinition *AIdef = QUERY_CREATURE_AI_SPEC_FROM_TYPE(ref->CreatureType());	//*bp08
	X16 bp10 = AIdef->b28;
	ADJUST_SKILLS(player, SKILL_FIGHTER_PARRY, AIdef->w22_8_b());	// 7
	X16 di;
	if (glbGlobalSpellEffects.Invisibility != 0 && AIdef->w0_a_a() == 0) {
		//^075F:1D01
		di = 0x10;
	}
	else if (AIdef->w0_b_b() != 0) {
		//^075F:1D10
		di = 0;
	}
	else {
		//^075F:1D14
		di = glbLightLevel << 1;
	}
	//^075F:1D1B
	X16 bp0c = AIdef->Defense;
	if (bp10 == 9) {
		//^075F:1D2D
		bp0c = min_value(255, bp0c << 1);
	}
	else if (bp10 == 8) {
		//^075F:1D45
		bp0c = 255;
	}
	//^075F:1D4A
	if (false
		|| glbIsPlayerSleeping != 0
		|| bp0c == 255
		|| ((USE_DEXTERITY_ATTRIBUTE(player) < U16((RAND()&0x1f) +bp0c +bp0e +di -0x10) || RAND02() == 0) && USE_LUCK_ATTRIBUTE(champion, 0x3c) == 0)
	) {
		//^075F:1D9D
		di = RAND();
		U16 bp0a;
		if ((di & 0x70) != 0 && (bp0c = AIdef->w26) != 0) {
			//^075F:1DB7
			di &= 15;
			if (di == 0)
				di = 1;
			//^075F:1DC4
			for (bp0a = 0; bp0a < 3 && (bp0c&15) < di; ) {
				//^075F:1DCB
				bp0a++;
				bp0c >>= 4;
				//^075F:1DD2
			}
			//^075F:1DE2
			bp0a = _4976_00e3[RCJ(5, 1 +bp0a)];
		}
		else {
			//^075F:1DED
			bp0a = (di&1) +1;
		}
		//^075F:1DF6
		si = AIdef->AttackStrength +min_value(AIdef->AttackStrength, (RAND()&15) +bp0e);
		if (bp10 != 8) {
			//^075F:1E26
			si = si -(QUERY_PLAYER_SKILL_LV(player, SKILL_FIGHTER_PARRY, 1) << 1);
			if (si <= 1) {
				//^075F:1E44
				if (RAND01() != 0) {
					//^075F:1E4D
					si = 0;
					goto _1f21;
				}
				else {
					//^075F:1E52
					si = RAND02() +2;
				}
			}
		}
		//^075F:1E5B
		si >>= 1;
		si = RAND16(si) +RAND02() +si;
		si = si +RAND16(si);
		si >>= 2;
		si = RAND02() +si +1;
		if (RAND01() != 0) {
			//^075F:1E94
			si = si -(RAND16((si >> 1) +1) -1);
		}
		//^075F:1EA7
		si = WOUND_PLAYER(player, si, bp0a, bp10);
		if (si != 0) {
			//^075F:1EBF
			// SPX: Champion "oof" sound when hit
			// => it would be possible to have several oof sounds using a random
			QUEUE_NOISE_GEN2(GDAT_CATEGORY_CHAMPIONS, champion->HeroType(), SOUND_CHAMPION_GETHIT, 0xfe, glbPlayerPosX, glbPlayerPosY, 2, 0x69, 0xc8);
			di = AIdef->PoisonDamage;
			if (di != 0 && RAND01() != 0 && (di = _2c1d_0f2a(champion, abVit, di)) > 0) {
				//^075F:1F16
				PROCESS_POISON(player, di);
			}

#if __SK_EXTENDED_SKWIN_V6__
			{
				U16 iPlagueDamage = 0;
				// Get plague damage from creature if any
				//iPlagueDamage = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_CREATURES, ref->CreatureType(), dtWordValueExt, GDAT_AI_PLAGUE_DAMAGE);
				iPlagueDamage = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_CREATURES, ref->CreatureType(), dtWordValue, GDAT_AI_PLAGUE_DAMAGE);
				if (iPlagueDamage > 0 && iPlagueDamage < 255)
					PROCESS_PLAGUE(player, iPlagueDamage);
			
			}
#endif
		}
	}
	//^075F:1F21
_1f21:
	// SPX: damage(?) is done to champions so that they are forced to wake
	if (glbIsPlayerSleeping != 0)
		RESUME_FROM_WAKE();
	//^075F:1F2D
	return si;
}
//^1C9A:166F
// SPX: _1c9a_166f renamed CREATURE_ATTACKS_CREATURE
i16 SkWinCore::CREATURE_ATTACKS_CREATURE(X16 xx, X16 yy)
{
	//^1C9A:166F
	ENTER(14);
	//^1C9A:1675
	i16 si = 0;
	ObjectID di = GET_CREATURE_AT(xx, yy);
	if (di == OBJECT_NULL)
		return -1;
	//^1C9A:1690
	Creature *bp04 = GET_ADDRESS_OF_RECORD4(di);
	AIDefinition *bp0c = glbAIDef;
	AIDefinition *bp08 = QUERY_CREATURE_AI_SPEC_FROM_TYPE(bp04->CreatureType());
	//SPX: creature/object is undestructible, no attack can work
	if (bp08->Defense == 0xff)
		return 0;
	//^1C9A:16CB
	if (i16((RAND()&0x1f) +bp0c->Defense) < i16((RAND()&0x1f) +bp08->Defense) && RAND02() != 0)
		return si;
	//^1C9A:1703
	X16 bp0e = ((RAND()&0x1f) +bp08->ArmorClass) >> 3;
	si = bp0c->AttackStrength +min_value(bp0c->AttackStrength, RAND()&15) -bp0e;
	if (si <= 1) {
		//^1C9A:174D
		if (RAND01() != 0)
			return 0;
		//^1C9A:175A
		si = RAND02() +2;
	}
	//^1C9A:1763
	si = RAND16(si) +RAND02();
	si = si +RAND16(si);
	si >>= 2;
	si = RAND02() +si +1;
	if (RAND01() != 0) {
		//^1C9A:179A
		si = si -RAND16((si >> 2)+1);
	}
	//^1C9A:17AD
	ATTACK_CREATURE(di, xx, yy, 2, 0x3c, si);
	//^1C9A:17C1
	return si;
}
//^1887:05F3
X16 SkWinCore::CREATURE_ATTACKS_PARTY()
{
	//^1887:05F3
	ENTER(38);
	//^1887:05F9
	CreatureInfoData *bp08 = glbCurrentThinkingCreatureData;
	AIDefinition *bp0c = glbAIDef;
	X16 bp10 = glbCreatureTempTargetX;
	X16 bp12 = glbCreatureTempTargetY;
	X16 bp14 = glbCurrentThinkingCreatureRec->b15_0_1();
	if (bp08->b32 == 6) {
		//^1887:0637
		bp14 += 2;
		bp14 &= 3;
	}
	//^1887:0640
	X16 bp1e = (glbCreatureTimerGetX == bp10 && glbCreatureTimerGetY == bp12) ? 1 : 0;
	if (bp1e == 0) {
		//^1887:0664
		if (ABS16(glbCreatureTimerGetX -bp10) +ABS16(glbCreatureTimerGetY -bp12) > 1)
			return 1;
		//^1887:068E
		if (CALC_VECTOR_DIR(glbCreatureTimerGetX, glbCreatureTimerGetY, bp10, bp12) != bp14 && bp0c->w0_2_2() == 0)
			return 1;
	}
	//^1887:06BA
	if (glbCurrentMapIndex == glbCreatureMap && bp10 == glbCreaturePosX && bp12 == glbCreaturePosY) {
		//^1887:06DE
		_4976_0090 = glbGameTick;
		X16 bp22 = 0;
		X16 bp20 = bp0c->w0_4_4();
		i16 di = 0;
		i16 bp1c = 0;
		i8 bp26[4];
		for (; bp1c < glbChampionsCount; bp1c++) {
			//^1887:0706
			if (glbChampionSquad[bp1c].curHP() != 0) {
				bp26[di++] = U8(bp1c);
			}
			//^1887:071E
		}
		//^1887:072A
		if (di == 0)
			return 1;
		//^1887:0731
		X16 bp18;
		if (bp0c->w0_3_3() != 0) {
			bp18 = (bp20 != 0) ? (RAND16(di) +1) : glbChampionsCount;
		}
		else {
			//^1887:0756
			bp18 = (bp0c->b9_5_5() != 0) ? 2 : 1;
		}
		i16 si;
		//^1887:076C
		if (bp08->b28 == 4) {
			//^1887:0776
			si = _2c1d_028c(glbCreatureTimerGetX, glbCreatureTimerGetY, 0xff);
			if (si != -1) {
				//^1887:0794
				bp08->b28 = glbChampionSquad[si].playerPos();
			}
		}
		//^1887:07A6
		X16 bp16 = bp08->b28;
		bp18 = min_value(bp18, glbChampionsCount);
		//^1887:07C3
		// SPX: bp18 is either
		for (; bp18-- != 0; ) {
			//^1887:07C6
			if (bp20 != 0) {
				//^1887:07CC
				si = RAND16(di);
				if (bp26[si] < 0) {
					for (bp1c = 0; bp1c < di; bp1c++) {
						//^1887:07E2
						if (++si >= di)
							si = 0;
						if (bp26[si] >= 0)
							break;
						//^1887:07F1
					}
				}
				//^1887:07F9
				bp1c = si;
				si = bp26[bp1c];
				bp26[bp1c] = -1;
			}
			else {
				//^1887:0819
				si = GET_PLAYER_AT_POSITION(bp16);
				X16 bp1a = CALC_VECTOR_DIR(bp10, bp12, glbCreatureTimerGetX, glbCreatureTimerGetY);
				(((bp16 +bp1a) & 1) != 0) ? --bp16 : ++bp16;
				bp16 &= 3;
				if (si == -1) {
					//^1887:085E
					si = _2c1d_028c(glbCreatureTimerGetX, glbCreatureTimerGetY, 0xff);
				}
				//^1887:0877
				if (si == -1)
					continue;
			}
			//^1887:087C
			bp22 = 1;
			i16 bp0e = CREATURE_ATTACKS_PLAYER(glbCurrentThinkingCreatureRec, si) +1;
			Champion *champion = &glbChampionSquad[si];
			if (champion->herob41 < bp0e) {
				//^1887:08B4
				champion->herob41 = U8(bp0e);
				champion->direction = (CALC_VECTOR_DIR(glbCreatureTimerGetX, glbCreatureTimerGetY, glbCreaturePosX, glbCreaturePosY) +2)&3;
			}
			//^1887:08E2
		}
		//^1887:08EF
		if (bp22 == 0)
			return 1;
	}
	else {
		//^1887:08F8
		i16 bp0e;
		if ((GET_TILE_VALUE(bp10, bp12) >> 5) == ttDoor) {
			//^1887:090E
			bp0e = RAND16(bp0c->AttackStrength + (bp0c->AttackStrength >> 1));
			if (ATTACK_DOOR(bp10, bp12, bp0e, 0, 0) != 0)
				return 1;
		}
		//^1887:0946
		if (bp1e != 0)
			return 1;
		bp0e = CREATURE_ATTACKS_CREATURE(bp10, bp12);
		if (bp0e < 0)
			return 1;
	}
	//^1887:0960
	if (bp08->Command == ccmPushBack) {
		//^1887:096A
		if ((GET_CREATURE_WEIGHT(glbCurrentThinkingCreatureID) > 100 && (RAND() & 15) != 0) || RAND02() != 0) {
			//^1887:098C
			_12b4_0d75(bp10, bp12, bp14, 0xfe);
		}
	}
	//^1887:09A0
	return 0;
}
//^1887:09AB
// CREATURE_ccmCastSpell1
X16 SkWinCore::CREATURE_CAST_SPELL()
{
	//^1887:09AB
	ENTER(12);
	//^1887:09AF
	CreatureInfoData *bp04 = glbCurrentThinkingCreatureData;
	AIDefinition *AIdef = glbAIDef;	//*bp08
	if (bp04->b32 <= 1) {
		_4976_0090 = glbGameTick;
	}
	//^1887:09E1
	if (glbCurrentThinkingCreatureRec->b15_0_1() == bp04->b27 || AIdef->w0_2_2() != 0) {
		//^1887:0A07
		//i16 bp0a = (AIdef->AttackStrength >> 2) +1;
		i16 bp0a = (AIdef->GetShootStrength()) +1;
		bp0a = bp0a +RAND16(bp0a);
		bp0a = bp0a +RAND16(bp0a);
		switch (bp04->ItemToThrow) {
			case -121://^0A52
			case -119://^0A52
			case -118://^0A52
				//^1887:0A52
				bp0a <<= 3;
				break;
			case -122://^0A58
				//^1887:0A58
				bp0a <<= 2;
				break;
			case -120://^0A5C
				break;
		}
		//^1887:0A5C
		bp0a = BETWEEN_VALUE(4, bp0a, 255);
		X16 bp0c;
		if (bp0a <= 8) {
			//^1887:0A75
			bp0c = 1;
		}
		else if (bp0a <= 0x10) {
			//^1887:0A82
			bp0c = 2;
		}
		else if (bp0a <= 0x20) {
			//^1887:0A8F
			bp0c = RAND01() +3;
		}
		else {
			//^1887:0A99
			bp0c = RAND02() +7;
		}
		//^1887:0AA4
		SHOOT_ITEM(ObjectID::Raw(bp04->ItemToThrow), glbCreatureTimerGetX, glbCreatureTimerGetY, bp04->b28, bp04->b27, bp0a, AIdef->Defense, bp0c);

		SkD((DLV_TWEET, "Tweet: %s (a#%3d, x:%d, y:%d, map:%d) has thrown %s by cast! \n"
			, static_cast<LPCSTR>(getRecordNameOf(glbCurrentThinkingCreatureID))
			, glbCurrentThinkingCreatureID.DBIndex()
			, glbPlayerPosX
			, glbPlayerPosY
			, glbCurrentMapIndex
			, static_cast<LPCSTR>(getRecordNameOf(ObjectID::Raw(bp04->ItemToThrow)))
			));

		if (AIdef->b9_4_4() != 0 && WOUND_CREATURE((bp0a >> 1) +RAND01()) != 0) {
			//^1887:0AFF
			return _4976_4ee6 = 1;
		}
		//^1887:0B08
		return 0;
	}
	//^1887:0B0C
	return 1;
}




//^1887:0B1B
// SPX: CREATURE_CCM0A
X16 SkWinCore::CREATURE_STEAL_FROM_CHAMPION()
{
	//^1887:0B1B
	ENTER(10);
	//^1887:0B21
	i16 bp08;

	if (false
		|| glbCurrentMapIndex != glbCreatureMap 
		|| glbCreatureTempTargetX != glbCreaturePosX 
		|| glbCreatureTempTargetY != glbCreaturePosY
		|| (bp08 = GET_PLAYER_AT_POSITION(glbCurrentThinkingCreatureData->b28)) == -1
	) {
		//^1887:0B55
		return 1;
	}
	//^1887:0B5B
	Champion *champion = &glbChampionSquad[bp08];
	//^1887:0B6C
	i16 di = 0; // defaulting to 0
	if (true
		&& USE_LUCK_ATTRIBUTE(champion, 100 - USE_DEXTERITY_ATTRIBUTE(bp08)) != 0
		&& (di = i16(RAND())) >= 0
	) {
		//^1887:0B98
		//^1887:0C6F
		return 0;
	}
	//^1887:0B9B
	X16 bp06 = 0;
	ObjectID si = champion->Possess(0);
	ObjectID bp0a = champion->Possess(1);
	if ((((di & 3) != 0) ? ((QUERY_ITEM_MONEY_VALUE(si) < QUERY_ITEM_MONEY_VALUE(bp0a)) ? 1 : 0) : (di & 4)) != 0) {
		//^1887:0BDE
		bp06 = 1;
		si = bp0a;
	}
	//^1887:0BE6
	if (si != OBJECT_NULL) {
		//^1887:0BEE
		if (CREATURE_CAN_HANDLE_IT(si, glbCurrentThinkingCreatureData->ItemToThrow) == 0) {
			//^1887:0C04
			si = REMOVE_POSSESSION(bp08, bp06);
			APPEND_RECORD_TO(si, &glbCurrentThinkingCreatureRec->possession, -1, 0);

			SkD((DLV_CCM,"CCM 0x0A: creature %s stole item %s from champion %d!\n"
			, static_cast<LPCSTR>(getRecordNameOf(glbCurrentThinkingCreatureID))
			, static_cast<LPCSTR>(getRecordNameOf(si))
			, bp08
			));

			//^1887:0C2A
			if (glbIsPlayerSleeping != 0) {
				//^1887:0C31
				glbIsPlayerSleeping = 0;
				di = (((RAND() & 15) + QUERY_PLAYER_SKILL_LV(bp08, 1, 1)) > 7) ? 1 : 0;
				glbIsPlayerSleeping = 1;
				if (di != 0) {
					//^1887:0C6A
					RESUME_FROM_WAKE();
				}
			}
		}
	}
	//^1887:0C6F
	return 0;
	//^1887:0C71
}
//^1887:0C75
X16 SkWinCore::CREATURE_CCM0B()
{
	//^1887:0C75
	ENTER(0);
	//^1887:0C78
	if (true
		&& _19f0_0d10(glbCurrentThinkingCreatureData->b32 | 0x80, glbCreatureTimerGetX, glbCreatureTimerGetY, glbCreatureTempTargetX, glbCreatureTempTargetY, glbCurrentThinkingCreatureRec->b15_0_1()) != 0
		&& glbCurrentThinkingCreatureData->Command == ccm0B
	) {
		//^1887:0CBC
		// SPX: Invoke msg X, Y, DIR, ActionType, TICK
		INVOKE_MESSAGE(glbCreatureTempTargetX, glbCreatureTempTargetY, 0, 2, glbGameTick);
		return 0;
	}
	//^1887:0CDC
	return 1;
}
//^1887:0CE1
X16 SkWinCore::CREATURE_CCM0C()
{
	//^1887:0CE1
	ENTER(2);
	//^1887:0CE6
	X16 si = 0;
	X8 bp01;
	switch (glbCurrentThinkingCreatureData->b31) {
		case 0: //^_0cfc
			//^1887:0CFC
			bp01 = glbCurrentThinkingCreatureRec->b15_0_1();
			CREATURE_CCM06();
			glbCurrentThinkingCreatureData->b29 = bp01;
			break;
		case 1: //^_0d1b
			//^1887:0D1B
			CREATURE_CCM06();
			si = CREATURE_TAKES_ITEM();
			break;
	}
	//^1887:0D26
	glbCurrentThinkingCreatureData->b31++;
	//^1887:0D2E
	return si;
}
//^1887:0E19
X16 SkWinCore::CREATURE_TAKES_ITEM()
{
	//^1887:0E19
	ENTER(0);
	//^1887:0E1E
	X16 di = 0;
	ObjectID si;
	for (si = GET_WALL_TILE_ANYITEM_RECORD(glbCreatureTempTargetX, glbCreatureTempTargetY); si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
		//^1887:0E32
		if (si.DBType() == dbCreature)
			continue;
		//^1887:0E3F
		if (glbCurrentThinkingCreatureData->b28 != 0xff && si.Dir() != glbCurrentThinkingCreatureData->b28)
			continue;
		//^1887:0E59
		if (CREATURE_CAN_HANDLE_IT(si, glbCurrentThinkingCreatureData->ItemToThrow) == 0)
			continue;
		//^1887:0E6F
		SkD((DLV_TWEET, "Tweet: %s (a#%03d, x:%d, y:%d, map:%d) takes %s! \n"
			, static_cast<LPCSTR>(getRecordNameOf(glbCurrentThinkingCreatureID))
			, glbCurrentThinkingCreatureID.DBIndex()
			, glbCreatureTempTargetX
			, glbCreatureTempTargetY
			, glbCurrentMapIndex
			, static_cast<LPCSTR>(getRecordNameOf(si))
			));
		MOVE_RECORD_TO(si, glbCreatureTempTargetX, glbCreatureTempTargetY, -1, 0);
		APPEND_RECORD_TO(si, &glbCurrentThinkingCreatureRec->possession, -1, 0);
		di++;
		if (glbCurrentThinkingCreatureData->b30_6_6() == 0)
			break;
		//^1887:0EAA
	}
	//^1887:0EBB
	return (di == 0) ? 1 : 0;
}
//^1887:0ECA
X16 SkWinCore::CREATURE_SHOOT_ITEM()
{
	//^1887:0ECA
	ENTER(0);
	//^1887:0ECF
	ObjectID di = CREATURE_CAN_HANDLE_ITEM_IN(glbCurrentThinkingCreatureData->ItemToThrow, glbCurrentThinkingCreatureRec->possession, -1);
	if (di == OBJECT_END_MARKER)
		return 1;
	//^1887:0EF9
	CUT_RECORD_FROM(di, &glbCurrentThinkingCreatureRec->possession, -1, 0);
	X16 si = glbAIDef->GetShootStrength() +1;
	si = si +RAND16(si);
	si = si +RAND16(si);
	SHOOT_ITEM(di, glbCreatureTimerGetX, glbCreatureTimerGetY, glbCurrentThinkingCreatureData->b28, glbCurrentThinkingCreatureData->b27, BETWEEN_VALUE(0x14, si, 255), glbAIDef->Defense, 8);
	//^1887:0F7A
	return 0;
}
//^1887:0D33
X16 SkWinCore::CREATURE_PUTS_DOWN_ITEM()
{
	//^1887:0D33
	ENTER(0);
	//^1887:0D38
	X16 di = 0;
	ObjectID si;
	for (; (si = CREATURE_CAN_HANDLE_ITEM_IN(glbCurrentThinkingCreatureData->ItemToThrow, glbCurrentThinkingCreatureRec->possession, -1)) != OBJECT_END_MARKER; ) {
		//^1887:0D3D
		di++;
		CUT_RECORD_FROM(si, &glbCurrentThinkingCreatureRec->possession, -1, 0);
		si.Dir(glbCurrentThinkingCreatureData->b28);
		MOVE_RECORD_TO(si, -1, 0, glbCreatureTempTargetX, glbCreatureTempTargetY);
		SkD((DLV_TWEET, "Tweet: %s (a#%03d, x:%d, y:%d, map:%d) put %s! \n"
			, static_cast<LPCSTR>(getRecordNameOf(glbCurrentThinkingCreatureID))
			, glbCurrentThinkingCreatureID.DBIndex()
			, glbCreatureTempTargetX
			, glbCreatureTempTargetY
			, glbCurrentMapIndex
			, static_cast<LPCSTR>(getRecordNameOf(si))
			));
		if (di == 1) {
			//^1887:0D86
			QUEUE_NOISE_GEN2(QUERY_CLS1_FROM_RECORD(si), QUERY_CLS2_FROM_RECORD(si), 
				SOUND_STD_KNOCK, 0xfe, glbCreatureTempTargetX, glbCreatureTempTargetY, 1, 0x3a, 0x80);
		}
		//^1887:0DB3
		if (glbCurrentThinkingCreatureData->b30_6_6() == 0)
			break;
		//^1887:0DC1
	}
	//^1887:0DE8
	return (di == 0) ? 1 : 0;
}
//^1887:0DF7
void SkWinCore::CREATURE_KILL_ON_TIMER_POSITION()	// ccmDestroy
{
	//^1887:0DF7
	ENTER(0);
	//^1887:0DFA
	DELETE_CREATURE_RECORD(glbCreatureTimerGetX, glbCreatureTimerGetY, CREATURE_GENERATED_DROPS, 1);
	_4976_4ee6 = 1;
	//^1887:0E17
	return;
}
//^1887:0F80
X16 SkWinCore::CREATURE_ROTATES_TARGET_CREATURE()
{
	//^1887:0F80
	ENTER(0);
	//^1887:0F84
	ObjectID si = GET_CREATURE_AT(glbCreatureTempTargetX, glbCreatureTempTargetY);
	if (si != OBJECT_NULL) {
		//^1887:0F9A
		ROTATE_CREATURE(si, 0, glbCurrentThinkingCreatureData->b29);
		return 0;
	}
	//^1887:0FB3
	return 1;
}
//^1887:0FB9
X16 SkWinCore::PLACE_MERCHANDISE()
{
	// 0 if not placed.
	// 1 if placed.

	//^1887:0FB9
	ENTER(8);
	//^1887:0FBF
	X16 di = 0;
	X16 bp08 = glbCreatureTempTargetY;
	X16 bp06 = glbCreatureTempTargetX;
	ObjectID bp02 = GET_CREATURE_AT(bp06, bp08);
	if (bp02 != OBJECT_NULL) {
		//^1887:0FE1
		if ((QUERY_CREATURE_AI_SPEC_FLAGS(bp02) & 1) != 0) {
			//^1887:0FEF
			ObjectID si;
			for (; (si = CREATURE_CAN_HANDLE_ITEM_IN(glbCurrentThinkingCreatureData->ItemToThrow, glbCurrentThinkingCreatureRec->GetPossessionObject(), -1)) != OBJECT_END_MARKER; ) {
				//^1887:0FF5
				di++;
				//^1887:0FF6
				CUT_RECORD_FROM(si, &glbCurrentThinkingCreatureRec->possession, -1, 0);
				X8 bp03 = (glbCurrentThinkingCreatureData->b28 == 0xff) ? RAND02() : glbCurrentThinkingCreatureData->b28;
				si.Dir(bp03);
				if (IS_CONTAINER_MONEYBOX(si) != 0) {
					//^1887:1044
					GET_ADDRESS_OF_RECORD9(si)->b7_2_2(1);
				}
				//^1887:1054
				MOVE_RECORD_TO(si, -1, -1, bp06, bp08);
				if (di == 1) {
					//^1887:106C
					QUEUE_NOISE_GEN2(QUERY_CLS1_FROM_RECORD(si), QUERY_CLS2_FROM_RECORD(si),
						SOUND_STD_KNOCK, 0xfe, bp06, bp08, 1, 0x3a, 0x80);
				}
				//^1887:1097
				if (glbCurrentThinkingCreatureData->b30_6_6() == 0)
					break;
				//^1887:10A5
			}
		}
	}
	//^1887:10CC
	return (di == 0) ? 1 : 0;
}
//^1887:10DB
X16 SkWinCore::TAKE_MERCHANDISE()
{
	//^1887:10DB
	ENTER(2);
	//^1887:10E1
	X16 di = 0;
	ObjectID bp02 = GET_CREATURE_AT(glbCreatureTempTargetX, glbCreatureTempTargetY);
	if (bp02 != OBJECT_NULL) {
		//^1887:10FD
		if ((QUERY_CREATURE_AI_SPEC_FLAGS(bp02)&1) != 0) {
			//^1887:1109
			ObjectID si;
			for (; (si = CREATURE_CAN_HANDLE_ITEM_IN(glbCurrentThinkingCreatureData->ItemToThrow, GET_ADDRESS_OF_RECORD4(bp02)->GetPossessionObject(), glbCurrentThinkingCreatureData->b28)) != OBJECT_END_MARKER; ) {
				//^1887:110D
				di++;
				MOVE_RECORD_TO(si, glbCreatureTempTargetX, glbCreatureTempTargetY, -1, -1);
				si = si.GetAsNorth();
				APPEND_RECORD_TO(si, &glbCurrentThinkingCreatureRec->possession, -1, 0);
				if (glbCurrentThinkingCreatureData->b30_6_6() == 0)
					break;
				//^1887:114F
			}
		}
	}
	//^1887:1182
	return (di == 0) ? 1 : 0;
}
//^1887:1191
X16 SkWinCore::CREATURE_ACTIVATES_WALL()
{
	//^1887:1191
	ENTER(6);
	//^1887:1197
	CreatureInfoData *bp04 = glbCurrentThinkingCreatureData;
	i16 di = bp04->ItemToThrow;
	i16 bp06;
	if (_19f0_2813((bp06 = bp04->b32)|0x80, glbCreatureTimerGetX, glbCreatureTimerGetY, glbCreatureTempTargetX, glbCreatureTempTargetY, glbCurrentThinkingCreatureRec->b15_0_1(), di) == 0)
		//^1887:11E8
		//^1887:1266
		return 1;
	ObjectID si;
	//^1887:11EA
	if (bp06 == 2 || di == -1) {
		//^1887:11F5
		si = OBJECT_NULL;
	}
	else {
		//^1887:11FA
		si = CREATURE_CAN_HANDLE_ITEM_IN(bp04->ItemToThrow, glbCurrentThinkingCreatureRec->possession, (i8)-1);
		//^1887:1218
		if (si == OBJECT_END_MARKER)
			//^1887:121B
			//^1887:1266
			return 1;
		//^1887:121D
		CUT_RECORD_FROM(si, &glbCurrentThinkingCreatureRec->possession, -1, 0);
	}
	//^1887:1234
	MOVE_RECORD_AT_WALL(bp04->w24.GetX(), bp04->w24.GetY(), (bp04->b29 +2)&3, glbCurrentThinkingCreatureID, si);
	//^1887:1262
	return 0;
	//^1887:1269
}
//^1887:034E
X16 SkWinCore::CREATURE_USES_LADDER_HOLE()
{
	//^1887:034E
	ENTER(18);
	//^1887:0354
	CreatureInfoData *bp04 = glbCurrentThinkingCreatureData;
	//^1887:0361
	if (CREATURE_GO_THERE(bp04->b32|0x80, glbCreatureTimerGetX, glbCreatureTimerGetY, glbCreatureTempTargetX, glbCreatureTempTargetY, bp04->b27) == 0)
		//^1887:0393
		//^1887:05A3
		return 1;
	//^1887:0396
	if ((_4976_37a6[RCJ(86,bp04->Command)] & 4) == 0)
		//^1887:03A7
		//^1887:05A3
		return 1;
	//^1887:03AA
	X16 si = glbCurrentThinkingCreatureRec->b15_0_1();
	glbCreatureTempTargetX = bp04->w24.GetX();
	glbCreatureTempTargetY = bp04->w24.GetY();
	//^1887:03D1
	if (bp04->Command == ccm39 || bp04->Command == ccm3A) {
		//^1887:03E2
		Actuator *bp0e;
		i16 di;
		si = FIND_LADDER_AROUND(glbCreatureTimerGetX, glbCreatureTimerGetY, di = (bp04->Command == ccm39) ? -1 : 1, reinterpret_cast<Actuator **>(&bp0e));
		//^1887:0412  
		if (bp0e != NULL) {
			//^1887:041A
			if (bp0e->RevertEffect() != 0) {
				//^1887:042B
				si = bp0e->ActionType();
			}
			else {
				//^1887:0439
				si = (si +bp0e->ActionType()) & 3;
			}
		}
		else {
			//^1887:0452
			i16 bp08;
			i16 bp10, bp12;
			i16 bp0a = LOCATE_OTHER_LEVEL(bp08 = glbCurrentMapIndex, di, &bp10, &bp12, NULL);
			//^1887:0483
			CHANGE_CURRENT_MAP_TO(bp0a);
			//^1887:048C
			si = FIND_LADDER_AROUND(bp10, bp12, -di, reinterpret_cast<Actuator **>(&bp0e));
		}
	}
	//^1887:04AF
	X16 bp06 = glbAIDef->CanActivatePitTeleport(); // actually, 0x40 or 0
	if (bp06 != 0) {
		//^1887:04C2
		OPERATE_PIT_TELE_TILE(glbCreatureTimerGetX, glbCreatureTimerGetY, 1);
	}
	//^1887:04D7
	MOVE_RECORD_TO(glbCurrentThinkingCreatureID, glbCreatureTimerGetX, glbCreatureTimerGetY, -1, 0);
	//^1887:04F3
	_1c9a_0648(bp04->w24.GetMap());
	//^1887:0507
	if (bp04->Command == ccm35 || bp04->Command == ccm36) {
		//^1887:0518
		si = _0cee_06dc_GET_TILE_DIRECTION(glbCreatureTempTargetX, glbCreatureTempTargetY);
	}
	//^1887:0529
	glbCurrentThinkingCreatureRec->b15_0_1(si);
	//^1887:053B
	if (MOVE_RECORD_TO(glbCurrentThinkingCreatureID, -1, 0, glbCreatureTempTargetX, glbCreatureTempTargetY) != 0) {
		//^1887:0557
		_4976_4ee6 = 1;
		//^1887:059F
		return 0;
	}
	//^1887:055E
	glbCreatureTimer.XcoordB((U8)_4976_5826);//glbCreatureTimerGetX
	glbCreatureTimer.YcoordB((U8)_4976_5828);//glbCreatureTimerGetY
	_1c9a_0648(glbSomeMap_4976_4ee7 = (U8)_4976_581c);
	//^1887:0579
	if (bp06 != 0) {
		//^1887:057F
		OPERATE_PIT_TELE_TILE(glbCreatureTimerGetX, glbCreatureTimerGetY, 0);
	}
	//^1887:0594
	if (_4976_514e.b1 > 0)
		//^1887:059B
		_4976_514e.b1--;
	//^1887:059F
	return 0;
	//^1887:05A6
}

//^1887:126D
// CREATURE_CCM3B renamed CREATURE_TRANSFORM
void SkWinCore::CREATURE_TRANSFORM()
{
	//^1887:126D
	ENTER(8);
	//^1887:1273
	CreatureInfoData *bp04 = glbCurrentThinkingCreatureData;
	U16 di = glbCreatureTimerGetX;
	U16 si = glbCreatureTimerGetY;
	i8 bp07 = bp04->b31;
	if (bp04->Command == ccmTransform) {
		//^1887:12A6
		X16 bp06 = RAND02() +1;
		for (; bp06-- != 0; ) {
			//^1887:12B1
			U8 bp08 = RAND16(5);
			if (bp08 == 4)
				bp08 = 255;
			CREATE_CLOUD(OBJECT_EFFECT_DISPELL, RAND16(156) +100, di, si, bp08);	// oFF83
			//^1887:12E3
		}
		//^1887:12ED
		if (bp07 >= 1) {
			//^1887:12F3
			CREATE_CLOUD(OBJECT_EFFECT_CLOUD, bp07 *85, di, si, 255);	// oFFA8
		}
		//^1887:130C
		if (bp07 < 3) {
			QUEUE_NOISE_GEN1(GDAT_CATEGORY_MESSAGES, 0, SOUND_MINION_TRANSFORMS, 0x6C, 0xC8, di, si, 1);
		}
		else {
			//^1887:1321
			bp04->Command = ccmTransform2;
			bp04->b31 = 0;
			glbCurrentThinkingCreatureRec->HP3(0);
			glbCurrentThinkingCreatureRec->CreatureType(bp04->ItemToThrow);
			glbAIDef = QUERY_CREATURE_AI_SPEC_FROM_TYPE(bp04->ItemToThrow);
			//^1887:1355
			bp06 = glbAIDef->BaseHP;
			glbCurrentThinkingCreatureRec->HP1(bp06 + RAND16((bp06 >> 3) +1));
			GET_CREATURE_ANIMATION_FRAME(glbCurrentThinkingCreatureRec->CreatureType(), bp04->Command, &_4976_4ed6->iAnimSeq, &_4976_4ed6->iAnimInfo, &glbCreatureAnimationFrame, 0);
			if (IS_CREATURE_ALLOWED_ON_LEVEL(glbCurrentThinkingCreatureID, glbSomeMap_4976_4ee7) == 0) {
				//^1887:13C1
				DELETE_CREATURE_RECORD(di, si, CREATURE_GENERATED_DROPS, 1);
				_4976_4ee6 = 1;
			}
			//^1887:13D4
			QUEUE_NOISE_GEN1(GDAT_CATEGORY_MESSAGES, 0, SOUND_STD_EXPLOSION, 0x6C, 0xC8, di, si, 1);
		}
	}
	else {
		//^1887:13EE
		CREATE_CLOUD(OBJECT_EFFECT_CLOUD, 255, di, si, 255);	// oFFA8
		QUEUE_NOISE_GEN1(GDAT_CATEGORY_MESSAGES, 0, SOUND_STD_EXPLOSION, 0x6C, 0xC8, di, si, 1);
	}
	//^1887:1418
	return;
}

//^1887:141C
// SPX: Event from Dragoth or Flame Orb
X16 SkWinCore::CREATURE_EXPLODE_OR_SUMMON()
{
	//^1887:141C
	ENTER(2);
	//^1887:1421
	i8 bp01 = glbCurrentThinkingCreatureData->b32;
	if (bp01 == 0) {	// SPX: This happens for the FIRE ORB which explode like a fireball
		//^1887:1430
		X16 si = glbAIDef->GetShootStrength() +1;
		si = RAND16(si) +si;
		si += RAND16(si);
		CREATE_CLOUD(OBJECT_EFFECT_FIREBALL, BETWEEN_VALUE(0x14, si, 255), // 20 to 255
			glbCreatureTimerGetX, glbCreatureTimerGetY, 255);
		
		// SPX: I removed this condition because in case of FLAME ORB, it does not throw anything, but wants to kill itself after one explosion.
		//if (glbCurrentThinkingCreatureData->ItemToThrow != 0)
			//^1887:148E
			CREATURE_KILL_ON_TIMER_POSITION();
	}
	//^1887:1493
	else if (bp01 == 1) {
		//^1887:1499
		// SPX: When dragoth calls a minion. But b30 (ItemToThrow) was badly init to 1 instead of 76 [Dragoth Attack Minion]
		glbCurrentThinkingCreatureData->ItemToThrow = CREATURE_DRAGOTH_ATTACK_MINION;	// 0x4C Dragoth Minion
		if (CREATE_MINION(glbCurrentThinkingCreatureData->ItemToThrow, 7, RAND02(), glbCreatureTimerGetX, glbCreatureTimerGetY, 
			glbSomeMap_4976_4ee7, OBJECT_NULL, glbCurrentThinkingCreatureRec->b15_0_1()) == OBJECT_NULL
		) {
			return 1;
		}
	}
	//^1887:14DD
	return 0;
}

//^1887:150C
U16 SkWinCore::PROCEED_CCM()
{
	//^1887:150C
	ENTER(0);
	//^1887:1510
	X16 si = 0;
	_4976_520e = ALLOC_MEMORY_RAM(0x100, afDefault, 1024);
	glbCreatureTempTargetX = glbCurrentThinkingCreatureData->w24.GetX();
	glbCreatureTempTargetY = glbCurrentThinkingCreatureData->w24.GetY();

	//^1887:1546
	SkD((DLV_CAI, "CAI: T-- a#%03d ccm<%02X> (%2d,%2d) \n"
		, (Bitu)glbCurrentThinkingCreatureData->CreatureIndex(), (Bitu)glbCurrentThinkingCreatureData->Command, (Bitu)glbCreatureTempTargetX, (Bitu)glbCreatureTempTargetY 
		));

	switch (glbCurrentThinkingCreatureData->Command) {
	case ccm01://^155D
	case ccm02://^155D // track objective located at (glbCreatureTempTargetX,glbCreatureTempTargetY)
	case ccm09://^155D
		//^1887:155D
		si = CREATURE_WALK_NOW();
		break;
	case ccm03://^1563 // track you?
	case ccm04://^1563
		//^1887:1563
		si = CREATURE_CCM03();
		break;
	case ccmJump://^1569
		//^1887:1569
		si = CREATURE_JUMPS();
		break;
	case ccm06://^156F
	case ccm07://^156F
		//^1887:156F
		CREATURE_CCM06();
		break;
	case ccmMeleeAttack://^1575 // attack you?
	case ccmPushBack://^1575
		//^1887:1575
		si = CREATURE_ATTACKS_PARTY();
		break;
	case ccmCastSpell1://^157B
	case ccmCastSpell2://^157B // throw you something. poison ball?
		//^1887:157B
		si = CREATURE_CAST_SPELL();
		break;
	case ccmSteal://^1581
		//^1887:1581
		si = CREATURE_STEAL_FROM_CHAMPION();
		break;
	case ccm0B://^1587
		//^1887:1587
		si = CREATURE_CCM0B();
		break;
	case ccm0C://^158D
	case ccm0D://^158D
		//^1887:158D
		si = CREATURE_CCM0C();
		break;
	case ccm1A://^1593
	case ccm2B://^1593
	case ccm2C://^1593
		//^1887:1593
		si = CREATURE_TAKES_ITEM();
		break;
	case ccmShootItem1://^1599
	case ccmShootItem2://^1599
		//^1887:1599
		si = CREATURE_SHOOT_ITEM();
		break;
	case ccm19://^159F
	case ccm29://^159F
	case ccm2A://^159F
	case ccm2D://^159F
	case ccm2E://^159F
		//^1887:159F
		si = CREATURE_PUTS_DOWN_ITEM();
		break;
	case ccmDestroy://^15A5
		//^1887:15A5
		CREATURE_KILL_ON_TIMER_POSITION();
		break;
	case ccm15://^15AB
	case ccm16://^15AB
		//^1887:15AB
		si = CREATURE_ROTATES_TARGET_CREATURE();
		break;
	case ccmPlaceMerchandise://^15B1
		//^1887:15B1
		si = PLACE_MERCHANDISE();
		break;
	case ccmTakeMerchandise://^15B7
		//^1887:15B7
		si = TAKE_MERCHANDISE();
		break;
	case ccm2F://^15BD
	case ccm30://^15BD
	case ccm31://^15BD
		//^1887:15BD
		si = CREATURE_ACTIVATES_WALL();
		break;
	case ccm35://^15C3
	case ccm36://^15C3
	case ccm37://^15C3
	case ccm38://^15C3
	case ccm39://^15C3
	case ccm3A://^15C3
		//^1887:15C3
		si = CREATURE_USES_LADDER_HOLE();
		break;
	case ccmTransform://^15C9	// ccm3B
	case ccmTransform2://^15C9
		//^1887:15C9
		CREATURE_TRANSFORM();
		break;
	case ccmExplode://^15CF
	case ccm3E://^15CF
	case ccm3F://^15CF
	case ccm40://^15CF
		//^1887:15CF
		CREATURE_EXPLODE_OR_SUMMON();
		break;	// SPX: added break here, safer.
	case ccm10://^15D5
	case ccmSpawn://^15D5
	case ccm12://^15D5
	case ccm14://^15D5
	case ccm1B://^15D5
	case ccm1C://^15D5
	case ccm1D://^15D5
	case ccm1E://^15D5
	case ccm1F://^15D5
	case ccm20://^15D5
	case ccm21://^15D5
	case ccm22://^15D5
	case ccm23://^15D5
	case ccm24://^15D5
	case ccm25://^15D5
	case ccm32://^15D5
	case ccm33://^15D5
	case ccm34://^15D5
        break;
	}
	//^1887:15D5
	SkD((DLV_CAI, "CAI: --T a#%03d r %d \n"
		, (Bitu)glbCurrentThinkingCreatureData->CreatureIndex(), (Bits)si
		));

	if (si != 0) {
		_4976_514e.b1 = 0;
	}
	else {
		//^1887:15E0
		if ((_4976_37a6[RCJ(86,glbCurrentThinkingCreatureData->Command)] & 3) != 0) {
			//^1887:15F2
			glbCurrentThinkingCreatureData->b4 = U8(glbGameTick);
		}
	}
	//^1887:15FD
	_4976_520e = NULL;
	DEALLOC_UPPER_MEMORY(0x100);
	//^1887:1615
	return si;
}
//^4937:028A
X16 SkWinCore::CREATURE_4937_028a(U16 xx, U16 *yy, CreatureAnimationFrame **ref)
{
	//^4937:028A
	ENTER(0);
	//^4937:028F
	X16 cx = xx;
	X16 di;
	if (*yy == 0xffff) {
		//^4937:029B
		*yy = 0;
		goto _02d0;
	}
	//^4937:02A2
	di = tlbCreaturesAnimationSequences[cx +*yy].w2_a_f();
	X16 si;
	if (di == 0) {
		//^4937:02C5
		si = 2;
	}
	else {
		//^4937:02CA
		*yy += di;
		//^4937:02D0
_02d0:
		if (tlbCreaturesAnimationSequences[cx +*yy].w2_0_3() == 0) {
			//^4937:02EB
			si = 0;
		}
		else {
			//^4937:02EB
			si = 1;
		}
	}
	//^4937:02F2
	*ref = &tlbCreaturesAnimationSequences[cx +*yy];
	//^4937:031A
	return si;
}

//^13E4:0982
// SPX: _13e4_0982 renamed CREATURE_THINK_0982
void SkWinCore::CREATURE_THINK_0982()
{
	//^13E4:0982
	ENTER(16);
	//^13E4:0988
	CreatureInfoData *xCreatureInfo = glbCurrentThinkingCreatureData;	// *bp04
	// SPX DEBUG
	if (xCreatureInfo->ItemToThrow != 0)
	{
		//if(SkCodeParam::bUseIngameDebug) printf("BP04 B30 = %04X\n", bp04->ItemToThrow);
		SkD((DLV_TWEET, "Tweet: Item %d flying (x:%d, y:%d, map:%d)\n"
			, xCreatureInfo->ItemToThrow, 0, 0, 0 
			));
		
	}
	sk1c9a02c3 *bp08;
	if (glbGlobalSpellEffects.FreezeCounter != 0 && glbAIDef->w0_c_c() == 0 && xCreatureInfo->Command != ccmDestroy && xCreatureInfo->Command2 != ccmDestroy) {
		//^13E4:09B8
		glbCreatureTimer.dw00 += 4;
		goto _0c47;
	}
	//^13E4:09C5
	bp08 = _4976_4ed6;
	X16 bp0a;
	X16 di;
	if (glbCreatureTimer.TimerType() == tty22) {
        //^13E4:09DC
		bp0a = 1;
		//^13E4:09E1
		//LOGX(("13E4:09E1 ccm%02X", (U8)xCreatureInfo->Command2));
		xCreatureInfo->Command = xCreatureInfo->Command2;
		if (xCreatureInfo->Command != i8(ccmInv)) {
			//^13E4:09F0
			xCreatureInfo->Command2 = ccmInv;
			if (_14cd_062e() != 0 && xCreatureInfo->Command != ccmDestroy) {
				//^13E4:0A08
				xCreatureInfo->Command = ccmInv;
				goto _0a6a;
			}
			//^13E4:0A0F
			if ((_4976_37a6[RCJ(86,xCreatureInfo->Command)] & 4) != 0) {
				//^13E4:0A20
				_4976_514e.b1 = 0;
				_4976_514e.b3 = 0xff;
			}
			//^13E4:0A2A
			switch (xCreatureInfo->Command) {
				case ccm06: //^_0a3e
					//^13E4:0A3E
					xCreatureInfo->b29 = (glbCurrentThinkingCreatureRec->b15_0_1() -1) & 3;
					break;
				case ccm07: //^_0a4d
					//^13E4:0A4D
					xCreatureInfo->b29 = (glbCurrentThinkingCreatureRec->b15_0_1() +1) & 3;
					break;
			}
		}
		else {
			//^13E4:0A65
			CREATURE_THINK_09E2();
		}
		//^13E4:0A6A
_0a6a:
		X16 si;
		if (xCreatureInfo->Command == ccmDestroy) {
			//^13E4:0A74
			xCreatureInfo->w14 = CREATURE_SEQUENCE_4937_000f(bp08->iAnimSeq, &bp08->iAnimInfo);
			if ((tblAIStats01[QUERY_GDAT_CREATURE_WORD_VALUE(glbCurrentThinkingCreatureRec->CreatureType(), 1)] & 8) == 0) {
				X16 iCloudPower = 0;
				//^13E4:0AB0
				switch (glbAIDef->b35) {
					case 1: //^_0aca
						//^13E4:0ACA
						iCloudPower = 0xBE; // si = 0xbe;
						break;
					case 2: //^_0acf
						//^13E4:0ACF
						iCloudPower = 0xFF; // si = 0xff;
						break;
					default://^_0ac5
						//^13E4:0AC5
						iCloudPower = 0x6E; // si = 0x6e;
						break;
				}
				//^13E4:0AD2
				//CREATE_CLOUD(OBJECT_EFFECT_CLOUD, si, glbCreatureTimer.XcoordB(), glbCreatureTimer.YcoordB(), 0xff);	// oFFA8
				CREATE_CLOUD(OBJECT_EFFECT_CLOUD, iCloudPower, glbCreatureTimer.XcoordB(), glbCreatureTimer.YcoordB(), 0xff);	// oFFA8
			}
		}
		//^13E4:0AEC
		if ((si = xCreatureInfo->Command) >= ccm32 && si <= ccm34) {
			//^13E4:0B00
			glbCreatureTimer.SetMap(glbSomeMap_4976_4ee7);
			glbCreatureTimer.SetTick(glbGameTick +(si -50));
			goto _0c47;
		}
		//^13E4:0B2B
		if (si == ccm55) { // 0x55
			//si = (RAND01() != 0) ? 0x27 : 0x28;
			si = (RAND01() != 0) ? ccmCastSpell1 : ccmCastSpell2;
		}
		//^13E4:0B43
		di = GET_CREATURE_ANIMATION_FRAME(glbCurrentThinkingCreatureRec->CreatureType(), si, &bp08->iAnimSeq, &bp08->iAnimInfo, &glbCreatureAnimationFrame, 0);
	}
	else {
		//^13E4:0B6E
		bp0a = 0;
		di = _4937_01a9(bp08->iAnimSeq, &bp08->iAnimInfo, &glbCreatureAnimationFrame);
	}
	//^13E4:0B90
_0b90:
	if (bp0a != 0 || xCreatureInfo->b33 == 0 || glbCreatureAnimationFrame->w2_9_9() == 0) {
		if (glbCreatureAnimationFrame->w2_8_8() != 0) {
			//^13E4:0BB8
			_13e4_01a3();
			xCreatureInfo->b33 |= PROCEED_CCM();
		}
	}
	//^13E4:0BCE
	if (xCreatureInfo->b33 != 0 && glbCreatureAnimationFrame->w2_9_9() != 0) {
		//^13E4:0BE4
		di = CREATURE_4937_028a(bp08->iAnimSeq, &bp08->iAnimInfo, &glbCreatureAnimationFrame);
		if (di != 2)
			goto _0b90;
	}
	//^13E4:0C06
	if (di == 1)
		glbCreatureTimer.TimerType(tty21);
	else
		glbCreatureTimer.TimerType(tty22);
	//^13E4:0C17
	if (_4976_4ee6 != 0)
		return;
	//^13E4:0C21
	glbCreatureTimer.SetTick(CREATURE_SOMETHING_1c9a_0a48());
	glbCreatureTimer.SetMap(glbSomeMap_4976_4ee7);
	//^13E4:0C47
_0c47:
	if (_4976_5161 != 0 && (_4976_514e.b0 != 0 || _4976_514e.b3 != 0xff)) {
		//^13E4:0C5C
		U16 bp10;
		U8 *bp0e;
		if (_3e74_5673(0x20000000 | glbCurrentThinkingCreatureID.DBIndex(), &bp10, 1) == 0) {
			//^13E4:0C7E
			bp0e = ALLOC_CPXHEAP_MEM(bp10, sizeof(sk514e)); // sizeof(sk514e) == 14
			_3e74_585a(bp10, 1);
		}
		else {
			//^13E4:0CA1
			bp0e = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp10));
		}
		//^13E4:0CB0
		COPY_MEMORY(&_4976_514e, bp0e, sizeof(sk514e)); // sizeof(sk514e) == 14
	}
	//^13E4:0CC6
	if (glbCurrentThinkingCreatureData->TimerIndex() != 0xffff) {
		RELEASE_CREATURE_TIMER(glbCurrentThinkingCreatureID);
	}
	//^13E4:0CDB
#if UseAltic
	glbCreatureTimer.Value2(0);
	glbCreatureTimer.ActionType(0);
#endif
	glbCurrentThinkingCreatureData->TimerIndex(QUEUE_TIMER(&glbCreatureTimer));
	//^13E4:0CEE
	return;
}
//^13E4:071B
void SkWinCore::CREATURE_13e4_071b()
{
	//^13E4:071B
	ENTER(10);
	//^13E4:0721
	sk1c9a02c3* xAnimInfo = _4976_4ed6; // bp08
	if ((xAnimInfo->iAnimInfo & 0xe03f) == 0x8001) // 1 possible frame and static object ?
		return;
	//^13E4:0740
	CreatureAnimationFrame* bp04 = &tlbCreaturesAnimationSequences[xAnimInfo->iAnimSeq];
	//^13E4:075B
	i16 si = 1;
	for (; (bp04->w2_0_3() != 0); si++, bp04++);
	//^13E4:0770
	X16 di = (xAnimInfo->iAnimInfo & 0xFC0);
	X16 bp0a = (X16)((glbGameTick +di) % si);
	if (bp0a == 0) {
		//^13E4:079D
		xAnimInfo->iAnimInfo = 0x8001|di;
		return;
	}
	//^13E4:07A7
	xAnimInfo->iAnimInfo = si |di |0xc000;
	RELEASE_CREATURE_TIMER(glbCurrentThinkingCreatureID);
	glbCreatureTimer.SetMap(glbSomeMap_4976_4ee7);
    glbCreatureTimer.SetTick(glbGameTick +si -bp0a);
	glbCurrentThinkingCreatureData->TimerIndex(QUEUE_TIMER(&glbCreatureTimer));
	//^13E4:0802
	return;
}
//^13E4:0806
void SkWinCore::CREATURE_13e4_0806()
{
	//^13E4:0806
	ENTER(10);
	//^13E4:080C
	sk1c9a02c3* xAnimInfo = _4976_4ed6;	// bp08
	if ((xAnimInfo->iAnimInfo & 0xE000) == 0x8000 && (xAnimInfo->w2_0_5() > 1))
		return;
	//^13E4:0837
	CreatureAnimationFrame *bp04 = &tlbCreaturesAnimationSequences[xAnimInfo->iAnimSeq];
	i16 si = 1;
	for (; bp04->w2_0_3() != 0; si++, bp04++);
	//^13E4:0867
	X16 di = xAnimInfo->iAnimInfo & 0x0FC0;
	X16 bp0a = (X16)((glbGameTick +di) % si);
	if (bp0a == 0) {
		//^13E4:0894
		xAnimInfo->iAnimInfo = si | di | 0x8000;
		return;
	}
	//^13E4:08A0
	xAnimInfo->iAnimInfo = si | di | 0xA000;
	RELEASE_CREATURE_TIMER(glbCurrentThinkingCreatureID);
	glbCreatureTimer.SetMap(glbSomeMap_4976_4ee7);
	glbCreatureTimer.SetTick(glbGameTick + si - bp0a);
	glbCurrentThinkingCreatureData->TimerIndex(QUEUE_TIMER(&glbCreatureTimer));
	//^13E4:08FB
	return;
}

//^13E4:0CF2
void SkWinCore::THINK_CREATURE(X8 xx, X8 yy, X16 timerType)
{
	SkD((DLV_DBG_CAI, "DBM: THINK_CREATURE(x=%d ,y=%d ,TiTy=%d ,z=%d)\n"
		, (Bitu)xx, (Bitu)yy, (Bitu)timerType, (Bitu)glbCurrentMapIndex));

	//^13E4:0CF2
	ENTER(18);
	//^13E4:0CF8
	ObjectID bp10 = GET_CREATURE_AT(xx, yy);
	if (bp10 == OBJECT_NULL)
		return;
	U8 *bp04 = PREPARE_LOCAL_CREATURE_VAR(bp10, xx, yy, timerType);
	CreatureInfoData *bp08 = glbCurrentThinkingCreatureData;
	AIDefinition* currentAI = glbAIDef;	// bp0c

	// SPX: protection
	if (bp08 == NULL)
		return;

	bp08->TimerIndex(0xffff);
	if (glbCurrentThinkingCreatureRec->hp1 == 0) {
		//^13E4:0D57
		bp08->w20 = 1;
		glbCurrentThinkingCreatureRec->hp1 = 1;
	}
	//^13E4:0D69
	i16 bp0e = bp08->w20;
	bp08->w20 = 0;
	i16 di = currentAI->b3;
	if (di != 0) {
		//^13E4:0D8A
		if (di < 0)
			di = -di;
		i16 si = U8(glbGameTick >> 2);
		if (si < bp08->b6_)
			si += 0x100;
		//^13E4:0DB5
		si -= bp08->b6_;
		i16 bp12 = si / di;
		if (bp12 > 0) {
			//^13E4:0DCC
			if (currentAI->b3 < 0) {
				bp0e += bp12;
			}
			else if (glbCurrentThinkingCreatureRec->hp1 < currentAI->BaseHP) {
				glbCurrentThinkingCreatureRec->hp1 += bp12;
			}
			//^13E4:0DF7
			bp08->b6_ = U8(glbGameTick >> 2) - U8(si % di);
		}
	}
	//^13E4:0E15
	if (bp0e > 0) {
		//^13E4:0E1B
		if (currentAI->IsStaticObject() == 0)
			glbCurrentThinkingCreatureRec->w10_f_f(0);
		//^13E4:0E2F
		if (WOUND_CREATURE(bp0e) != 0)
			goto _0e89;
	}
	//^13E4:0E3C
	if (currentAI->IsStaticObject() == 0) {
		CREATURE_THINK_0982();
	}
	else {
		//^13E4:0E4C
		if (_4976_4ed6->w2_e_e() != 0) {
			CREATURE_13e4_071b();
		}
		//^13E4:0E5E
		else if (_4976_4ed6->w2_d_d() != 0) {
			CREATURE_13e4_0806();
		}
		//^13E4:0E6E
		if (bp08->TimerIndex() == 0xffff) {
			//^13E4:0E78
			_1c9a_0fcb(glbCurrentThinkingCreatureRec->b5);
		}
	}
	//^13E4:0E89
_0e89:
	UNPREPARE_LOCAL_CREATURE_VAR(bp04);
	//^13E4:0E95
	return;
}

//^1C9A:09B9
X16 SkWinCore::_1c9a_09b9(ObjectID rlCreature, X16 xx)
{
	//^1C9A:09B9
	ENTER(0);
	//^1C9A:09BC
	return (GET_ADDRESS_OF_RECORD4(rlCreature)->iAnimSeq == xx) ? 1 : 0;
}

//^19F0:13AA
X16 SkWinCore::_19f0_13aa(i16 xx, i16 yy)
{
	//^19F0:13AA
	ENTER(10);
	//^19F0:13B0
	i16 bp08 = 0;
	do {
		//^19F0:13B5
		if ((tblAIStats01[_4976_4efa] & 0x400) != 0 || ((glbCurrentThinkingCreatureRec->iAnimFrame & 0x80) == 0 
			&& (glbAIDef->w0_2_2() != 0 || glbCreatureTimer.XcoordB() != xx || glbCreatureTimer.YcoordB() != yy || ((glbCurrentThinkingCreatureRec->b15_0_1() +2) & 3) != bp08) || (RAND() & 7) == 0)) {
			//^19F0:1410
			i16 di = xx;
			i16 si = yy;
			for (i16 bp0a = 0; bp0a++ < 3; ) {
				//^19F0:141E
				di += glbXAxisDelta[bp08];
				si += glbYAxisDelta[bp08];
				//^19F0:1430
				if (di < 0 || di >= glbCurrentMapWidth || si < 0 || si >= glbCurrentMapHeight)
					break;
				//^19F0:1450
				for (ObjectID bp06 = GET_WALL_TILE_ANYITEM_RECORD(di, si); bp06 != OBJECT_END_MARKER; bp06 = GET_NEXT_RECORD_LINK(bp06)) {
					//^19F0:145B
					if (bp06.DBType() == dbMissile) {
						Missile *bp04 = GET_ADDRESS_OF_RECORDE(bp06);
						if (glbTimersTable[bp04->TimerIndex()].Direction() == ((bp08 +2) & 3) && _075f_06bd(bp04, bp04->GetMissileObject()) != 0) {
							//^19F0:14BA
							return 1;
						}
					}
					//^19F0:14BF
				}
				//^19F0:14D1
				if (glbCreatureTimer.XcoordB() != di || glbCreatureTimer.YcoordB() != si) {
					//^19F0:14E3
					if (_19f0_00b8(di, si) != 0)
						break;
				}
				//^19F0:14EF
			}
		}
		//^19F0:14FD
	} while (++bp08 < 4);
	//^19F0:150B
	return 0;
}

//^3A15:0DDC
void SkWinCore::ACTIVATE_CREATURE_KILLER(X16 argLo, X16 argHi, X16 srcx, X16 srcy, X16 tarx, X16 tary, X16 actuatorType, X16 actionType) 
{
	// ACTIVATE_CREATURE_KILLER:
	// - kill the void door.
	// - 

	//^3A15:0DDC
	ENTER(4);
	//^3A15:0DE2
	X16 si = ABS16(srcx -tarx);
	X16 di = ABS16(srcy -tary);
	tarx = srcx -si;
	tary = srcy -di;
	si = (si << 1) +1;
	X16 bp04 = si;
	di = (di << 1) +1;
	for (; (di--) != 0; ) {
		//^3A15:0E24
		for (si = bp04; (si--) != 0; ) {
			//^3A15:0E2A
			if (tarx +si < 0 || tarx +si >= glbCurrentMapWidth || tary +di < 0 || tary +di >= glbCurrentMapHeight)
				continue;
			//^3A15:0E5A
			ObjectID bp02 = GET_CREATURE_AT(tarx +si, tary +di);
			if (bp02 == OBJECT_NULL)
				continue;
			//^3A15:0E75
			if (_1c9a_09b9(bp02, argHi) == 0)
				continue;
			//^3A15:0E86
			if (actuatorType == 0xb) { // 0x0B -> 'Creature killer'
				//^3A15:0E8C
				switch (argLo) {
					case 0:
					case 1:
						continue;
					case 2:
						//^3A15:0E9F
						_13e4_0360(bp02, tarx +si, tary +di, 0x13, 1);
						continue;
					default:
						return;
				}
			}
			//^3A15:0EBC
			if (actuatorType == 0x28) {
				//^3A15:0EC2
				ATTACK_CREATURE(bp02, tarx +si, tary +di, (actionType != 0) ? (argLo |0x8000) : argLo, 0x64, 0);
			}
			//^3A15:0EEF
		}
		//^3A15:0EF9
	}
	//^3A15:0F03
	return;
}
//^13E4:08FF
// SPX: _13e4_08ff renamed ANIMATE_CREATURE
void SkWinCore::ANIMATE_CREATURE(X16 xx, X16 yy, X16 ww)
{
	//^13E4:08FF
	ENTER(4);
	//^13E4:0905
	X16 di = yy;
	ObjectID si = GET_CREATURE_AT(xx, di);
	if (si == OBJECT_NULL)
		return;
	//^13E4:091A
	if ((QUERY_CREATURE_AI_SPEC_FLAGS(si)&1) != 0 && GET_ADDRESS_OF_RECORD4(si)->b5_0_7() == 0xff) {
		//^13E4:0938
		ALLOC_CAII_TO_CREATURE(si, xx, di);
	}
	//^13E4:0945
	U8 *bp04 = PREPARE_LOCAL_CREATURE_VAR(si, xx, di, 0x21);
	if (glbCurrentThinkingCreatureData != NULL) {
		if (ww == 0) {
			//^13E4:0968
			CREATURE_13e4_071b();
		}
		else {
			//^13E4:096E
			CREATURE_13e4_0806();
		}
		//^13E4:0972
		UNPREPARE_LOCAL_CREATURE_VAR(bp04);
	}
	//^13E4:097E
	return;
}




//^1C9A:121D
ObjectID SkWinCore::ALLOC_NEW_CREATURE(U16 creaturetype, U16 healthMultiplier_1to31_baseIs8, U16 dir, U16 xx, U16 yy)
{
	// arrange for a new creature record.

	// (creature += 0x8000) if you wanna create minion map's minion creature. then new missile record is attached to the minion creature.

	//^1C9A:121D
	ENTER(12);
	//^1C9A:1223
	U16 si = healthMultiplier_1to31_baseIs8;
	//^1C9A:1226
	U16 bp0c = creaturetype & 0x8000;
	//^1C9A:122F
	ObjectID bp0a;
	if (bp0c != 0) {
		//^1C9A:1233
		creaturetype &= 0x7fff;
		//^1C9A:1238
		bp0a = ALLOC_NEW_RECORD(dbMissile);
		//^1C9A:1243
		if (bp0a == OBJECT_NULL) {
			//^1C9A:1248
			return OBJECT_NULL;
		}
	}
	//^1C9A:124E
	ObjectID di = ALLOC_NEW_RECORD(dbCreature);
	//^1C9A:1258
	if (di == OBJECT_NULL || _4976_1a68 >= glbCreaturesCount) {
		//^1C9A:1266
		if (bp0c != 0)
			//^1C9A:126C
			DEALLOC_RECORD(bp0a);
		//^1C9A:1275
		//^1C9A:1248
		return OBJECT_NULL;
	}
	//^1C9A:1277
	Creature *xCreature = GET_ADDRESS_OF_RECORD4(di);	//*bp04
	xCreature->CreatureType(U8(creaturetype));
	//^1C9A:128E
	AIDefinition *bp08 = QUERY_CREATURE_AI_SPEC_FROM_TYPE(U8(creaturetype));
	//^1C9A:129B
	xCreature->SetPossessionObject(OBJECT_END_MARKER);
	xCreature->b15 = 0xFB;
	xCreature->b15_0_1(dir);
	xCreature->b5_0_7(0xFF);
	if (bp0c != 0) {
		//^1C9A:12C3
		APPEND_RECORD_TO(bp0a, &xCreature->possession, -1, 0);
	}
	//^1C9A:12DB
	si = min_value(si, 31);
	//^1C9A:12E8
	// SPX: bp08->w4 = Hit Points
	si = (si * bp08->BaseHP) >> 3;
	//^1C9A:12F4
	xCreature->HP1(RAND16((si >> 3) +1) +si);
	xCreature->HP3(0);
	xCreature->SetTriggerXPos(xx);
	xCreature->SetTriggerYPos(yy);
	xCreature->SetTriggerMap(glbCurrentMapIndex);
	xCreature->iAnimSeq = 0xFFFF;
	if (MOVE_RECORD_TO(di, -4, 0, xx, yy) == 1) {
		return OBJECT_NULL;
	}
	xCreature->iAnimSeq = 0;
	//printf("Creature T=%02d Anim=%02X,%02X\n", creaturetype, xCreature->iAnimSeq, xCreature->iAnimFrame); 
	return di;
}


//^0CEE:2E35
// SPX: _0cee_2e35 renamed CREATURE_GET_COLORKEY
i16 SkWinCore::CREATURE_GET_COLORKEY(U8 cls2)
{
	ENTER(0);
	U16 si = QUERY_GDAT_CREATURE_WORD_VALUE(cls2, GDAT_IMG_COLORKEY_1);	// 0x04
	if (si == 0)
		return 4;
	return si;
}


//^121E:0222
U16 SkWinCore::CREATURE_121e_0222(U16 xx, U16 yy, U16 ww)
{
	ENTER(12);
	Creature* xCreature = NULL;	// bp0c

	if (glbChampionLeader == -1)
		return 0;

	U16 bp06 = (glbPlayerDir +ww) & 3;
	ObjectID di = GET_CREATURE_AT(xx, yy);
	if (di != OBJECT_NULL) {
		AIDefinition* xAIDef = QUERY_CREATURE_AI_SPEC_FROM_RECORD(di);	// bp04
		xCreature = GET_ADDRESS_OF_RECORD4(di);
		if (true
			&& xAIDef->IsStaticObject() != 0
			&& (false
				|| (ww >= 4 && ww <= 7 && (xAIDef->wc30 & 0x0800) == 0)
				|| (ww < 4 && (xAIDef->wc30 & 0x2000) == 0)
			)
		) {
			return 0;
		}
	}
	const ObjectID rlCreature = di;
	di = REMOVE_OBJECT_FROM_HAND();
	//^121E:02B0
	if (ww >= 4 && ww <= 7) {
		//^121E:02BC
		U8 bp07 = (ww -4 + glbPlayerDir) & 3;
		//^121E:02CA
		if (xCreature != NULL) {
			//^121E:02D2
			for (ObjectID si = xCreature->GetPossessionObject(); si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
				//^121E:02DB
				if (si.Dir() == bp07 && IS_CONTAINER_MONEYBOX(si) != 0) {
					//^121E:02F4
					GET_ADDRESS_OF_RECORD9(si)->b7_2_2(0);
				}
				//^121E:0304
			}
		}
		//^121E:0312
		MOVE_RECORD_TO(ObjectID(di, bp07), -1, -1, xx, yy);

		SkD((DLV_TWEET, "Tweet: You have put %s on %s (a#%03d, x:%d, y:%d) \n"
			, static_cast<LPCSTR>(getRecordNameOf(di))
			, static_cast<LPCSTR>(getRecordNameOf(rlCreature))
			, rlCreature.DBIndex()
			, xx
			, yy
			));
	}
	else{
		//^121E:0323
		MOVE_RECORD_TO(ObjectID(di, bp06), -1, 0, xx, yy);

		SkD((DLV_TWEET, "Tweet: You have put %s! \n"
			, static_cast<LPCSTR>(getRecordNameOf(di))
			));
	}
	//^121E:0344
	_4976_4e5c = 1;
	//^121E:034A
	return 1;
}

//^4937:005C
// TODO: creature animation related ?
Bit16u SkWinCore::CREATURE_4937_005c(Bit16u iAnimSeq, Bit16u* piSeqInfo) // xx *yy
{
	// piSeqInfo XS.. .... ..FF FFFF
	// S : Static => frame 0 will always be returned
	// FF FFFF : Number of frames in the sequence

	Bit16u iFrame = 0;
	if ((*piSeqInfo & 0x4000) != 0) {
		iFrame = 0;
	}
	else if ((*piSeqInfo & 0x8000) != 0) {
		Bit16u di;
		if ((*piSeqInfo & 0x1000) != 0) {
			*piSeqInfo &= 0xE03F;
			di = 0;
		}
		else {
			di = (*piSeqInfo & 0x0FC0) >> 6;
		}
		//^4937:0097
		// SPX: I case of wrong data, avoid 0 division (because of next %) and consider anim to have at least 1 frame
		if (SkCodeParam::bUseFixedMode)
		{
			if ( (*piSeqInfo & 0x003F) == 0)
				*piSeqInfo &= 1;
		}
		// Depending on game tick, select frame number within the sequence, flag x3F holding the number of possible frames.
		iFrame = (glbGameTick + di) % (*piSeqInfo & 0x003F);
	}
	else {
		iFrame = *piSeqInfo & 0x003F;
	}
	//printf("Creature Anim => Tick= %04d Frame= %02X => Final Seq = %04X (search into tlbCreaturesAnimationSequences)\n", glbGameTick, iFrame, (iAnimSeq + iFrame));
	return iAnimSeq + iFrame;
}

//^4937:0036
// TODO: creature animation related, get some sequence
// 4937_0036 renamed CREATURE_4937_0036
CreatureAnimationFrame* SkWinCore::CREATURE_4937_0036(Bit16u iAnimSeq, Bit16u* piAnimInfo)
{
	return &tlbCreaturesAnimationSequences[CREATURE_4937_005c(iAnimSeq, piAnimInfo)];
}