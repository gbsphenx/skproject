#include <StdAfx.h>	// Required for MVC6 compilation
#include <conio.h>

#include <SkVer.h>

#include <KCORE.H>
#include <SkDebug.h>

#if defined(_USE_MFC80) || defined(_USE_MFC60)
#include <SkMFC.h>
#include <resource.h>
#endif // defined(_USE_MFC80) || defined(_USE_MFC60)

//^3A15:320C
// SPX: _3a15_320c renamed PROCESS_TIMER_0E
void SkWinCore::PROCESS_TIMER_0E(Timer *v0, Bit16u v1)
{
	//^3A15:320C
	ENTER(12);
	//^3A15:3211
	X32 bp0c = glbItemSizePerDB[v0->value];
	U8 *bp04;
	U8 *bp08;
	ObjectID si;
	COPY_MEMORY(
		bp08 = reinterpret_cast<U8 *>(GET_ADDRESS_OF_RECORD(si = ObjectID(0, v0->value, 0))),
		bp04 = ALLOC_MEMORY_RAM(bp0c, afDefault, 0x400),
		bp0c
		);
	//^3A15:326A
	SET_ITEMTYPE(si, v0->Value2());
	PROCESS_ITEM_BONUS(v0->actor, si, -1, v1);
	COPY_MEMORY(bp04, bp08, bp0c);
	DEALLOC_UPPER_MEMORY(bp0c);
	//^3A15:32B9
	return;
}




//^2066:2B6C
// SPX: _2066_2b6c renamed PROCEED_GLOBAL_EFFECT_TIMERS
void SkWinCore::PROCEED_GLOBAL_EFFECT_TIMERS()
{
	//^2066:2B6C
	Timer *bp04 = glbTimersTable;
	//^2066:2B7F
	ZERO_MEMORY(&glbGlobalSpellEffects, sizeof(glbGlobalSpellEffects));
	//^2066:2B8F
	for (Bit16u di=0; di < glbTimersActiveCount; bp04++, di++) {
		//^2066:2B94
		Bit16u si = bp04->actor;
		Bit16u bp08 = bp04->TimerType();
		//^2066:2BA8
		switch (bp08) {
			case ttyItemBonus:			// x0e
				{
					//^2066:2C60
					PROCESS_TIMER_0E(bp04, 3);
					break;
				}
			case ttyLight:		// x46
				{
					//^2066:2C1A
					i16 bp06 = bp04->value;
					//^2066:2C24
					if (bp06 == 0 || bp06 < -15 || bp06 > 15)
						break;
					//^2066:2C34
					if (bp06 < 0) {
						//^2066:2C3A
						glbGlobalSpellEffects.Light += tLightLevelItem[RCJ(16,-bp06)];
						break;
					}
					//^2066:2C4A
					glbGlobalSpellEffects.Light -= tLightLevelItem[RCJ(16,bp06)] << 1;
					break;
				}
			case ttyInvisibility:		// x47
				{
					//^2066:2C5A
					glbGlobalSpellEffects.Invisibility++;
					break;
				}
#if (XDM1_EXTENDED_SEETHRUWALLS == 1)
			case ttySeeThruWalls:		// x49
				{
					glbGlobalSpellEffects.SeeThruWalls++;
					break;
				}
#endif
			case ttyEnchantment:		// x48
				{
					//^2066:2BD1
					for (i16 bp06 = 0; bp06 < glbChampionsCount; bp06++) {
						//^2066:2BD8
						if ((si & (1 << bp06)) == 0)
							continue;
						//^2066:2BE4
						if (glbChampionSquad[bp06].curHP() == 0)
							continue;
						//^2066:2BF5
						glbChampionSquad[bp06].enchantmentPower += bp04->value;
						//^2066:2C0C
					}
					break;
				}
			case ttyPoison:	// x4B
				{
					//^2066:2BC1
					glbChampionSquad[si].PoisonValue++;
					break;
				}
		}
		//^2066:2C70
	}
	//^2066:2C7E
	for (Bit16u si=0; si < glbChampionsCount; si++) {
		//^2066:2C82
		CALC_PLAYER_WEIGHT(si);
		//^2066:2C89
	}
	//^2066:2C90
	return;
}




//^2066:1F37
Bit16u SkWinCore::_2066_1f37(ObjectID recordLink, Bit16u yy, Bit16u *zz)
{
	//^2066:1F37
	ObjectID di = recordLink;
	//^2066:1F40
	Bit16u si = 0;
	//^2066:1F42
	while ((di = GET_NEXT_RECORD_LINK(di)) != OBJECT_END_MARKER) {
		//^2066:1F44
		if (di.DBType() == dbActuator) {
			//^2066:1F51
			Actuator *bp04 = GET_ADDRESS_OF_ACTU(di);
			//^2066:1F5E
			if (bp04->ActuatorType() == ACTUATOR_FLOOR_TYPE__CROSS_SCENE) { // 0x27 -> Cross scene
				//^2066:1F6D
				si = 1;
				//^2066:1F70
				if (bp04->ActuatorData() == 0) {
					//^2066:1F7E
					bp04->ActuatorData(yy +1);
					//^2066:1F91
					(*zz)++;
				}
			}
		}
		//^2066:1F97
	}
	//^2066:1FA5
	return si;
}



//^2759:15D0
// SPX: _2759_15d0 replaced by PROCEED_LIGHT
// CSBWin: part of TAG01ca0c / CastSpell
void SkWinCore::PROCEED_LIGHT(U16 cmdNum, U16 yy)
{
	//^2759:15D0
	ENTER(12);
	//^2759:15D6
	i16 bp0c = 1;
	//^2759:15DB
	yy = BETWEEN_VALUE(32, yy +1, 256);
	//^2759:15F0
	i16 si = yy >> 3;
	//^2759:15F5
	si = max_value(8, si);
	//^2759:1601
	U16 di ;
	switch (cmdNum) {
		case CmDarkness: // 6
			//^2759:1615
			di = ((si -8) << 4) +16;
			//^2759:1622
			bp0c = -2;
			//^2759:1627
			//^2759:1649
			si >>= 1;
			si--;
			break;

		case CmLight: // 38
			//^2759:1629
			di = ((si -3) << 7) +2000;
			//^2759:1636
			si >>= 2;
			si++;
			//^2759:163A
			break;

		case CmLongLight: // 39	Long light
			//^2759:163C
			di = ((si -8) << 9) +10000;
			//^2759:1649
			si >>= 1;
			si--;

			break;
	}
	//^2759:164C
	Timer bp0a;
	bp0a.TimerType(ttyLight);
	//^2759:1650
	bp0a.value = (cmdNum == CmDarkness) ? si : -si;
	//^2759:1661
	bp0a.SetMap(glbPlayerMap);
	bp0a.SetTick(glbGameTick +di);
	//^2759:1683
	bp0a.actor = TIMER_ACTOR__00;
	//^2759:1687		
	QUEUE_TIMER(&bp0a);
	//^2759:1693
	//glbGlobalSpellEffects.Light = tLightLevelItem[RCJ(16,si)] * bp0c;
	// SPX: fix, to add new effect to current, still problem with DES IR SAR effect --> fixed using signed value in RECALC_LIGHT_LEVEL instead of unsigned.
	glbGlobalSpellEffects.Light += (tLightLevelItem[RCJ(16,si)] * bp0c);
	//^2759:16A3
	RECALC_LIGHT_LEVEL();
	//^2759:16A8
	return;
}



//^2759:1451
U16 SkWinCore::CONFUSE_CREATURE(U16 ww, U16 xx, U16 yy)
{
	//^2759:1451
	ENTER(8);
	//^2759:1457
	X16 si = 0;
	if (glbObjectID_4976_534c != OBJECT_NULL) {
		//^2759:1460
		Creature* creature = GET_ADDRESS_OF_RECORD4(glbObjectID_4976_534c);	//*bp04
		AIDefinition *bp08 = QUERY_CREATURE_AI_SPEC_FROM_TYPE(creature->CreatureType());
		U16 di;
		if (RAND16(ww) >= (di = bp08->w16_4_7()) && di != 15) {
			//^2759:14A6
			ATTACK_CREATURE(glbObjectID_4976_534c, xx, yy, 0x2005, 0x64, 0);
			//^2759:14BF
			si = 1;
		}
	}
	//^2759:14C4
	return si;
}



//^2C1D:0009
// SPX: _2c1d_0009 replaced by PROCEED_ENCHANTMENT_SELF
void SkWinCore::PROCEED_ENCHANTMENT_SELF(U16 mask, U16 yy, U16 zz, U16 tick) 
{
	//^2C1D:0009
	ENTER(18);
	//^2C1D:000F
	Timer bp12;
	X16 bp08 = 0;
	i16 bp06 = 0;
	//^2C1D:0019
	for (; bp06 < 4; bp06++) {
		//^2C1D:001C
		X16 di = 1 << bp06;
		if ((mask & di) != 0) {
			//^2C1D:002E
			if (glbChampionSquad[bp06].curHP() == 0) {
				//^2C1D:003F
				mask &= ~di;
			}
			if (glbChampionSquad[bp06].enchantmentAura != U8(yy) || glbChampionSquad[bp06].curHP() == 0) {
				glbChampionSquad[bp06].enchantmentPower = 0;
				i16 si = 0;
				Timer *bp04 = glbTimersTable;
				for (; si < glbTimersActiveCount; bp04++, si++) {
					//^2C1D:008B
					if (bp04->TimerType() == ttyEnchantment && (mask & bp04->actor) != 0) {
						if ((bp04->actor & (~mask)) == 0) {
							//^2C1D:00AF
							DELETE_TIMER(si);
						}
						else {
							//^2C1D:00B8
							bp04->actor = bp04->actor & (~U8(mask));
						}
					}
					//^2C1D:00CA
				}
			}
		}
		//^2C1D:00D5
		if (glbChampionSquad[bp06].enchantmentPower > 0x32)
			bp08 = 1;
		//^2C1D:00EB
	}
	//^2C1D:00F7
	if (bp08 != 0)
		zz >>= 2;
	//^2C1D:0101
	for (bp06 = 0; bp06 < glbChampionsCount; bp06++) {
		//^2C1D:0108
		if ((mask & (1 << bp06)) != 0) {
			glbChampionSquad[bp06].enchantmentAura = U8(yy);
			glbChampionSquad[bp06].enchantmentPower += zz;
		}
		//^2C1D:0137
	}
	//^2C1D:0143
	bp12.value = zz;
	bp12.TimerType(ttyEnchantment);
	bp12.actor = U8(mask);
	bp12.SetMap(U8(glbPlayerMap));
	bp12.SetTick(glbGameTick +tick);
	QUEUE_TIMER(&bp12);
	//^2C1D:0182
	return;
}


//^1C9A:1BAE
//SPX: somewhat called when in bat caves
U16 SkWinCore::_1c9a_1bae(i16 xx, i16 yy)
{
	//^1C9A:1BAE
	ENTER(0);
	//^1C9A:1BB1
	if (glbCreatureTimerGetX == xx && glbCreatureTimerGetY == yy)
		return 0;
	//^1C9A:1BC9
	return _19f0_00b8(xx, yy);
	//^1C9A:1BD6
}


//^3A15:3331
void SkWinCore::ACTIVATE_TICK_GENERATOR(Actuator *ref, ObjectID recordLink)
{
	//^3A15:3331
    //^3A15:3337
	Bit16u di = ref->ActuatorType();
	//^3A15:3343
	Bit16u si;
	switch (di) {
		case ACTUATOR_TYPE_TICK_GENERATOR: // 0x001e -> Activator, tick generator
			//^3A15:3358
			si = 1; break;
		case ACTUATOR_TYPE_X33_V008: // 0x0033 -> -
			//^3A15:335D
			si = 8; break;
		case ACTUATOR_TYPE_X34_V016: // 0x0034 -> -
			//^3A15:3362
			si = 16; break;
		case ACTUATOR_TYPE_X35_V032: // 0x0035 -> -
			//^3A15:3367
			si = 32; break;
		case ACTUATOR_TYPE_X36_V064: // 0x0036 -> -
			//^3A15:336C
			si = 64; break;
		case ACTUATOR_TYPE_X37_V128: // 0x0037 -> -
			//^3A15:3371
			si = 128; break;
	}
	//^3A15:3374
	if (ref->ActuatorData() != 0) {
		//^3A15:3385
		Timer bp0a;
		bp0a.SetMap(glbCurrentMapIndex);
		bp0a.SetTick(glbGameTick + (glbGameTick % (Bit32u(ref->ActuatorData()) * Bit32u(si))));
		bp0a.TimerType(ttyTickGenerator);
		bp0a.actor = TIMER_ACTOR__00;
		bp0a.value = recordLink;
		bp0a.Value2(Bit8u(si & 0xff));
		bp0a.ActionType(0);
		//^3A15:33ED
		QUEUE_TIMER(&bp0a);
	}
	//^3A15:3400
	return;
}

//^3A15:3511
void SkWinCore::PROCESS_ACTUATOR_TICK_GENERATOR() //#DS=4976?
{
	//^3A15:3511
	for (Bit16u di=0; dunHeader->nMaps > di; di++) {
		//^3A15:351C
		Bit16u bp14 = dunMapsHeaders[di].RawColumn();
		//^3A15:3534
		Bit16u bp16 = dunMapsHeaders[di].RawRow();
		//^3A15:354C
		Bit8u *bp04 = *glbMapTileValue[di];
		//^3A15:3567
		OID_T *bp0c = &dunGroundStacks[dunMapTilesObjectIndexPerColumn[dunMapColumnsSumArray[di]]];	 // Bit16u *bp0c
		//^3A15:3591
		for (Bit16u bp10=0; bp10 <= bp14; bp10++) {
			//^3A15:3599
			for (Bit16u bp12=0; bp12 <= bp16; bp12++) {
				//^3A15:35A1
				if ((*(bp04++) & 0x10) != 0) {
					//^3A15:35B0
					ObjectID bp0e = *bp0c;
					bp0c++;
					//^3A15:35BD
					while (bp0e.DBType() <= dbActuator) {
						//^3A15:35C0
						if (bp0e.DBType() == dbActuator) {
							//^3A15:35D1
							Actuator *bp08 = GET_ADDRESS_OF_RECORD(bp0e)->castToActuator();
							//^3A15:35E0
							Bit16u bp1c = bp08->ActuatorType();
							//^3A15:35ED
							switch (bp1c) {
								case ACTUATOR_TYPE_TICK_GENERATOR: // 0x001e -> 'Activator, tick generator'
								case ACTUATOR_TYPE_X33_V008: // 0x0033 -> ?
								case ACTUATOR_TYPE_X34_V016: // 0x0034 -> ?
								case ACTUATOR_TYPE_X35_V032: // 0x0035 -> ?
								case ACTUATOR_TYPE_X36_V064: // 0x0036 -> ?
								case ACTUATOR_TYPE_X37_V128: // 0x0037 -> ?
									{
										//^3A15:3606
										if (glbSpecialScreen != 0) {
											//^3A15:360D
											if (bp08->OnceOnlyActuator() != 0) {
												//^3A15:361E
												CHANGE_CURRENT_MAP_TO(di);
												//^3A15:3625
												ACTIVATE_TICK_GENERATOR(bp08, bp0e);
												//^3A15:3635
												break;
											}
											//^3A15:3638
											bp08->w4 &= 0xfffe;
											//^3A15:3640
											break;
										}
										//^3A15:3643
										Bit16u si;
										for (si=0; si < glbTimersMaximumCount; ) {
											//^3A15:3647
											Timer *bp1a = &glbTimersTable[si];
											si++;
											//^3A15:365F
											if (bp1a->TimerType() == ttyTickGenerator && bp1a->value == bp0e) {
												//^3A15:3672
												bp1a->SetMap(di);
												break;
											}
											//^3A15:36A1
										}
										//^3A15:36A7
										for (; si < glbTimersMaximumCount; si++) {
											//^3A15:36A9
											Timer *bp1a = &glbTimersTable[si];
											//^3A15:36C3
											if (bp1a->TimerType() == ttyTickGenerator) {
												//^3A15:36CA
												if (bp1a->value == bp0e) {
													//^3A15:36D3
													DELETE_TIMER(si);
												}
											}
											//^3A15:36D9
										}
										break;
									}
							}
						}
						//^3A15:36E0
						bp0e = GET_NEXT_RECORD_LINK(bp0e);
						//^3A15:36EC
					}
				}
				//^3A15:36FD
			}
			//^3A15:370B
		}
		//^3A15:3719
	}
	//^3A15:372B
	COMPACT_TIMERLIST();
	//^3A15:372F
	return;
}

//^3A15:0314
void SkWinCore::REARRANGE_TIMERLIST()
{
	// chain an unused timer entry to next unused timer entry.

	//^3A15:0314
	//^3A15:0319
	glbTimersActiveCount = 0;
	glbTimerIndexNextAvailable = 0xffff;
	//^3A15:0325
	Bit16u di;
	for (Bit16u si=0; si < glbTimersMaximumCount; si++) {
		//^3A15:0329
		if (glbTimersTable[si].TimerType() == tty00) {
			//^3A15:033D
			if (glbTimerIndexNextAvailable == TIMER_NONE) {	// 0xFFFF
				//^3A15:0344
				glbTimerIndexNextAvailable = si;
			}
			else {
				//^3A15:034A
				glbTimersTable[di].w0_0_f(si);
			}
			//^3A15:035A
			glbTimersTable[si].w0_0_f(0xffff);
			//^3A15:036C
			di = si;
		}
		else {
			//^3A15:0370
			glbTimersActiveCount = si+1;
		}
		//^3A15:0376
	}
	//^3A15:037D
	return;
}

//^3A15:008E
// SPX: _3a15_008e renamed __SORT_TIMERS
void SkWinCore::__SORT_TIMERS()
{
	//^3A15:008E
	//^3A15:0094
	Bit16u bp12 = glbTimersCount;
	//^3A15:009A
	if (bp12 != 0) {
		//^3A15:00A1
		for (Bit16u di = 0; di < bp12; di++) {
			//^3A15:00A5
			glbTimerNextEntries[di] = di;
			//^3A15:00B2
		}
		//^3A15:00B8
		if (bp12 != 1) {
			//^3A15:00C1
			for (i16 bp0e = (bp12 -2) >> 1; bp0e >= 0; bp0e--) {
				//^3A15:00CE
				Bit16u di = bp0e;
				Timer *bp04 = &glbTimersTable[glbTimerNextEntries[di]];
				while (true) {
					//^3A15:00F3
					Bit16u si = (di << 1) + 1;
					//^3A15:00FA
					if (si >= bp12)
						break;
					//^3A15:0102
					Timer *bp08 = &glbTimersTable[glbTimerNextEntries[si]];
					//^3A15:0124
					if (si+1 < bp12) {
						//^3A15:012C
						Timer *bp0c = &glbTimersTable[glbTimerNextEntries[si+1]];
						//^3A15:014F
						if (_3a15_0381(bp04, bp08) != 0) {
							//^3A15:0167
							if (_3a15_0381(bp04, bp0c) != 0)
								//^3A15:017D
								break;
							//^3A15:017F
							si++;
						}
						else {
							//^3A15:0182
							if (_3a15_0381(bp0c, bp08) != 0)
								//^3A15:019A
								si++;
						}
					}
					else {
						//^3A15:019D
						if (_3a15_0381(bp04, bp08) != 0)
							//^3A15:01B3
							break;
					}
					//^3A15:01B5
					// SPX: This is a swap typical in sort algo
					Bit16u bp10 = glbTimerNextEntries[di];
					//^3A15:01C5
					glbTimerNextEntries[di] = glbTimerNextEntries[si];
					//^3A15:01DF
					glbTimerNextEntries[si] = bp10;
					//^3A15:01EF
					di = si;
				}
				//^3A15:01F4
			}
		}
	}
	//^3A15:0200
	_4976_4762 = -1;
	//^3A15:0206
	REARRANGE_TIMERLIST();
	//^3A15:020B
	return;
}


//^3A15:0002
// _3a15_0002 renamed INIT_TIMERS
void SkWinCore::INIT_TIMERS()
{
	//^3A15:0002
	ENTER(4);
	//^3A15:0007
	glbTimersTable = reinterpret_cast<Timer *>(ALLOC_MEMORY_RAM(glbTimersMaximumCount * 10, afUseUpper, 0x400));
	glbTimerNextEntries = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(glbTimersMaximumCount << 1, afUseUpper, 0x400));
	if (glbSpecialScreen != 0) {
		Timer *bp04 = glbTimersTable;
		U16 si = 0;
		for (; si < glbTimersMaximumCount; bp04++) {
			//^3A15:005A
			bp04->TimerType(tty00);
			bp04->w0_0_f(++si);
			//^3A15:006A
		}
		//^3A15:0070
		glbTimersActiveCount = 0;
		bp04[-1].w0_0_f(0xffff);
		glbTimersCount = 0;
		glbTimerIndexNextAvailable = 0;
	}
	//^3A15:008B
	return;
}


//^2FCF:01C5
// _2fcf_01c5 renamed SET_TIMER_3C_OR_3D
void SkWinCore::SET_TIMER_3C_OR_3D(ObjectID recordLink, i16 xx, i16 yy, Bit16u curmap, Bit16u ss)
{
	//^2FCF:01C5
	ENTER(10);
	//^2FCF:01C9
	Timer xtimer; // bp0a
	xtimer.SetMap(curmap);
	xtimer.SetTick(glbGameTick +5);
	//^2FCF:01EC
	// SPX: though .. tty3D and tty3C seems to be handled exactly the same way!
	xtimer.TimerType((ss != 0) ? tty3D : tty3C);
	//^2FCF:01FB
	xtimer.actor = TIMER_ACTOR__00;
	//^2FCF:01FF
	xtimer.Xcoord(xx);
	xtimer.Ycoord(yy);
	xtimer.id8(recordLink);
	//^2FCF:0211
	SET_MINION_RECENT_OPEN_DOOR_LOCATION(recordLink, xx, yy, curmap, 1);
	//^2FCF:0226
	QUEUE_TIMER(&xtimer);
	//^2FCF:0232
	return;
}



//^075F:0182
void SkWinCore::_075f_0182(ObjectID rl, X16 xx, X16 yy)
{
	//^075F:0182
	ENTER(4);
	//^075F:0188
	ObjectID si = GET_TILE_RECORD_LINK(xx, yy);
	for (; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
		//^075F:0198
		if (si.DBType() == dbActuator) {
			Actuator *bp04 = GET_ADDRESS_OF_ACTU(si);
			if (bp04->ActuatorType() == ACTUATOR_FLOOR_TYPE__MISSILE_EXPLOSION) { // 0x26 -> Activator, missile explosion
				U16 di = bp04->RevertEffect()
					^ ((bp04->ActuatorData() == 0x1ff || bp04->ActuatorData() == rl.MissileType()) ? 1 : 0);
				if (bp04->ActionType() == 3) {
					INVOKE_ACTUATOR(bp04, (di != 0) ? 0 : 1, 0);
				}
				else if (di != 0) {
					INVOKE_ACTUATOR(bp04, bp04->ActionType(), 0);
				}
			}
		}
		//^075F:0240
	}
	//^075F:0251
	return;
}

//^075F:0255
void SkWinCore::CREATE_CLOUD(ObjectID rl, U16 ww, U16 xx, U16 yy, U16 ss)
{
	//^075F:0255
	ENTER(36);
	//^075F:025B
	i16 si = ww;
	ObjectID di = ALLOC_NEW_RECORD(dbCloud);
	if (di == OBJECT_NULL)
		return;
	//^075F:0270
	Cloud *bp04 = GET_ADDRESS_OF_RECORDF(di);
	X16 bp0e;
	X16 bp10;
	if (xx <= 255) {
		bp0e = xx;
		bp10 = yy;
	}
	else {
		bp0e = xx & 0xff;
		bp10 = yy & 0xff;
		xx >>= 8;
		xx--;
		yy >>= 8;
	}
	//^075F:02AF
	if (ss == 0xff) {
		bp04->b2_7_7(1);
	}
	else {
		bp04->b2_7_7(0);
		di.Dir(ss);
	}
	bp04->CloudType(U8(rl.MissileType()));
	bp04->b3_0_f(U8(si));
	X16 bp14;
	if (glbCurrentMapIndex == glbMap_4c28 && xx == glbSomePosX_4c2e && yy == glbSomePosY_4c30) {
		bp14 = 255;
	}
	else {
		bp14 = min_value(255, (si >> 1) +0x80);
	}
	//^075F:0332
	QUEUE_NOISE_GEN2(GDAT_CATEGORY_SPELL_MISSILES, bp04->CloudType(), SOUND_STD_EXPLOSION, 0xfe, xx, yy, 1, 0x6c, U8(bp14));
	APPEND_RECORD_TO(di, NULL, xx, yy);
	U16 bp12;
	if (rl == OBJECT_EFFECT_CLOUD_REBIRTH) {	// oFFE4
		bp12 = 5;
	}
	else if (rl == OBJECT_EFFECT_REFLECTOR) {	// oFF8E
		bp12 = si >> 1;
	}
	else {
		bp12 = 1;
	}
	//^075F:038E
	Timer bp24;
	bp24.SetMap(glbCurrentMapIndex);
	bp24.SetTick(bp12 +glbGameTick);
	bp24.TimerType(ttyCloud);
	bp24.actor = TIMER_ACTOR__00;
	bp24.w8 = di;
	bp24.XcoordB(U8(xx));
	bp24.YcoordB(U8(yy));
	QUEUE_TIMER(&bp24);
	//^075F:03DA
	//if (rl == oFF82 || rl == oFF80 || rl == oFFB0 || rl == oFFB1) {
	if (rl == OBJECT_EFFECT_LIGHTNING 
		|| rl == OBJECT_EFFECT_FIREBALL 
		|| rl == OBJECT_EFFECT_THUNDER 
		|| rl == OBJECT_EFFECT_ELECTRIC_SHOCK) {
		//^075F:03F5
		i16 bp18 = xx = bp0e;
		i16 bp1a = yy = bp10;
		X16 bp16 = 1;
		si = (si >> 1) +1;
		si += RAND16(si) +1;
		bp14 = si;
		//^075F:0420
		// SPX: TODO Did I misreplaced something? This does rl == FF80 || rl == FF80 .. ???
		if (rl == OBJECT_EFFECT_FIREBALL || rl == OBJECT_EFFECT_FIREBALL || (si >>= 1) != 0) {
			//^075F:0437
			if (glbCurrentMapIndex == glbMap_4c28 && xx == glbSomePosX_4c2e && yy == glbSomePosY_4c30) {
				ATTACK_PARTY(si, 0x3f, 1);
			}
			//^075F:045F
			di = GET_CREATURE_AT(xx, yy);
			if (di != OBJECT_NULL) {
				//^075F:0476
_0476:
				if (_1c9a_0958(di) == 0) {
					//^075F:0481
					Creature *bp0c = GET_ADDRESS_OF_RECORD4(di);
					AIDefinition *bp08 = QUERY_CREATURE_AI_SPEC_FROM_TYPE(bp0c->CreatureType());
					if (bp08->w24_c_c() == 0 || rl == OBJECT_EFFECT_FIREBALL) {
						//^075F:04B3
						bp12 = bp08->GetFireResistance();
						if (bp12 != 15) {
							//^075F:04C8
							if (bp08->w0_5_5() != 0)
								si >>= 1;
							si -= RAND16((bp12 << 1) +1);
							if (si > 0) {
								ATTACK_CREATURE(di, xx, yy, 0x200d, 0x64, si);
							}
						}
					}
				}
			}
			//^075F:04FC
			if (bp16 != 0) {
				//^075F:0502
				di = _1c9a_03cf(&bp18, &bp1a, ss);
				if (di != OBJECT_NULL && (bp18 != bp0e || bp1a != bp10)) {
					xx = bp18;
					yy = bp18;
					bp16 = 0;
					si = bp14 >> 1;
					goto _0476;
				}
			}
		}
		//^075F:0549
		if (glbCurrentMapIndex == glbPlayerMap) {
			glbDoLightCheck = 1;
		}
	}
	//^075F:0558
	_075f_0182(rl, bp0e, bp10);
}


//^3DF7:000A
// SPX: _3df7_000a renamed SET_TIMER_WEATHER
void SkWinCore::SET_TIMER_WEATHER(U32 tickDelta)
{
	//^3DF7:000A
	ENTER(10);

	if (SkCodeParam::bUseDM2ExtendedMode && fDebugRainSpeed != 1)
		tickDelta = (U32) ((float) tickDelta / (float) fDebugRainSpeed);

	SkD((DLV_DBG_RAIN, "Rain timer duration = %03d (will hit at %d)\n"
		, tickDelta
		, glbGameTick +tickDelta
		));


	//^3DF7:000E
	Timer bp0a;
	bp0a.SetMap(0);
	bp0a.SetTick(glbGameTick +tickDelta);
	bp0a.TimerType(ttyWeather);
	bp0a.actor = TIMER_ACTOR__00;
	QUEUE_TIMER(&bp0a);
	//^3DF7:0035
	return;
}



//^3A15:0447
// SPX: Count timers until timer number xx
// SPX: _3a15_0447 renamed GET_TIMER_NEW_INDEX
Bit16u SkWinCore::GET_TIMER_NEW_INDEX(Bit16u xx)
{
	//^3A15:0447
	ENTER(4);
	//^3A15:044C
	U16 *bp04 = glbTimerNextEntries;
	i16 si;
	for (si = 0; si < glbTimersActiveCount; si++) {
		//^3A15:045D
		if (*(bp04++) == xx)
			break;
	}
	if (si >= glbTimersActiveCount) {
		RAISE_SYSERR(SYSTEM_ERROR__TIMER_BEYOND_MAX);
	}
	//^3A15:0481
	return si;
}

//^3A15:0696
Bit16u SkWinCore::QUEUE_TIMER(Timer *ref)
{
	ATLASSERT(ref->TimerType() != 0xcc);

	//ATLASSERT(ref->GetMap() != 0 || ref->TimerType() != tty1E);
	//printf("Timers : AC:%d / C:%d / MX:%d\n", glbTimersActiveCount, glbTimersCount, glbTimersMaximumCount);

	//^3A15:0696
	if (ref->TimerType() == 0) {
		//^3A15:06A5
		return 0xffff;
	}
	//^3A15:06AB
	if (glbTimersCount == glbTimersMaximumCount) {
		//^3A15:06B4
		RAISE_SYSERR(SYSTEM_ERROR__TIMER_MAX_REACHED);
	}
	//^3A15:06BC
	Bit16u si = glbTimerIndexNextAvailable;
	//^3A15:06CD
	glbTimerIndexNextAvailable = glbTimersTable[si].w0_0_f();
	//^3A15:06D5
	glbTimersTable[si] = *ref;

	SkD((DLV_DBG_TIMER, "DBG: Timer#%03d(%6u,%2u,%02X,%02X,%04X,%04X) Added.\n"
		, (Bitu)si
		, (Bitu)ref->GetTick(), (Bitu)ref->GetMap(), (Bitu)ref->TimerType()
		, (Bitu)ref->actor, (Bitu)ref->value, (Bitu)ref->w8
		));

	//^3A15:06F3
	if (glbTimersActiveCount <= si) {
		//^3A15:
		glbTimersActiveCount = si +1;
	}
	//^3A15:06FF
	i16 di = _4976_4762;
	//^3A15:0704
	if (di < 0) {
		//^3A15:0708
		di = glbTimersCount;
	}
	//^3A15:070C
	_4976_4762 = -1;
	glbTimersCount++;
	//^3A15:0716
	glbTimerNextEntries[di] = si;
	//^3A15:0723
	_3a15_0486(di);
	//^3A15:0729
	return si;
}



//^3A15:061A
// SPX: _3a15_061a renamed DELETE_TIMER
void SkWinCore::DELETE_TIMER(Bit16u xx)
{
	//^3A15:061A
	//^3A15:061F
	Bit16u si = xx;
	//^3A15:0622
	if (_4976_4762 >= 0) {
		//^3A15:0629
		_3a15_0486(_4976_4762);
	}
	//^3A15:0632
	glbTimersTable[si].TimerType(tty00);
	//^3A15:0644
	glbTimersTable[si].w0_0_f(glbTimerIndexNextAvailable);
	//^3A15:0657
	glbTimerIndexNextAvailable = si;
	//^3A15:065B
	glbTimersCount--;
	//^3A15:065F
	Bit16u di = glbTimersCount;
	//^3A15:0664
	if (di != 0) {
		//^3A15:0668
		si = GET_TIMER_NEW_INDEX(si);
		//^3A15:0670
		if (si != di) {
			//^3A15:0674
			glbTimerNextEntries[_4976_4762 = si] = glbTimerNextEntries[di];
		}
	}
	//^3A15:0692
	return;
}

//^3A15:0763
// SPX: _3a15_0763 renamed IS_TIMER_TO_PROCEED
X16 SkWinCore::IS_TIMER_TO_PROCEED()
{
	//^3A15:0763
	ENTER(0);
	//^3A15:0766
	if (_4976_4762 >= 0) {
		_3a15_0486(_4976_4762);
	}
	if (glbTimersCount != 0) {
		if (glbTimersTable[*glbTimerNextEntries].GetTick() <= glbGameTick)
			return 1;
	}
	//^3A15:07B0
	return 0;
}



//^3A15:072F
// SPX: _3a15_072f renamed GET_AND_DELETE_NEXT_TIMER
void SkWinCore::GET_AND_DELETE_NEXT_TIMER(Timer *ref)
{
	//^3A15:072F
	ENTER(0);
	//^3A15:0733
	X16 si;
	*ref = glbTimersTable[si = *glbTimerNextEntries];
	DELETE_TIMER(si);
	//^3A15:0760
	return;
}

//^1C9A:17C7
X16 SkWinCore::_1c9a_17c7(U8 xx, U8 yy, U8 zz)
{
	// SPX: I hit this one with undefined AI table
	//^1C9A:17C7
	ENTER(0);
	//^1C9A:17CC
	U16 di = xx;
	U16 si = yy;
	if (true
		&& glbCreatureMap == zz 
		&& glbIsPlayerSleeping == 0 
		&& glbChampionInventory == 0 
		&& ABS16(glbCreaturePosX -di) != ABS16(glbCreaturePosY -si)
		&& CALC_VECTOR_DIR(glbCreaturePosX, glbCreaturePosY, di, si) == glbCreatureDir
		&& ABS16(glbCreaturePosX -di) <= 2
		&& ABS16(glbCreaturePosY -si) <= 2
	) {
		//^1C9A:1841
		return 1;
	}
	//^1C9A:1846
	return 0;
	//^1C9A:1848
}



//^075F:18FD
void SkWinCore::PROCESS_CLOUD(Timer *ref)
{
	//^075F:18FD
	ENTER(18);
	//^075F:1903
	X16 di = ref->XcoordB();
	X16 si = ref->YcoordB();
	Cloud *bp04 = GET_ADDRESS_OF_RECORD(ref->id8())->castToCloud();
	if ((GET_TILE_VALUE(di, si) >> 5) == ttDoor) {
		//^075F:1938
		X16 bp06 = CALC_CLOUD_DAMAGE(ref->id8(), GET_TILE_RECORD_LINK(di, si));
		if (bp06 != 0) {
			//^075F:1956
			ATTACK_DOOR(di, si, bp06, 1, 0);
		}
	}
	//^075F:1967
	X16 bp06 = bp04->CloudType();
	if (bp06 != (missileTeleporter) && bp06 != (missileFireball) && bp06 != (missileLightning)) {
		//^075F:198E
		if (glbCurrentMapIndex == glbMap_4c28 && di == glbSomePosX_4c2e && si == glbSomePosY_4c30) {
			//^075F:19A3
			bp06 = CALC_CLOUD_DAMAGE(ref->id8(), OBJECT_NULL);
			if (bp06 != 0) {
				//^075F:19B9
				ATTACK_PARTY(bp06, 0, 0);
			}
		}
		//^075F:19C8
		ObjectID bp08 = GET_CREATURE_AT(di, si);
		if (bp08 != OBJECT_NULL) {
			//^075F:19D9
			if (_1c9a_0958(bp08) == 0) {
				//^075F:19E6
				bp06 = CALC_CLOUD_DAMAGE(ref->id8(), bp08);
				if (bp06 != 0) {
					//^075F:19FD
					ATTACK_CREATURE(bp08, di, si, 0x200d, 0x64, bp06);
				}
			}
		}
		//^075F:1A12
		Timer bp12;
		switch (bp04->CloudType()) {
		case 0x64: //^_1a2e
			//^075F:1A2E
			// SPX: Explosion spell sound
			QUEUE_NOISE_GEN2(GDAT_CATEGORY_SPELL_MISSILES, bp04->CloudType(), SOUND_STD_EXPLOSION, 0xfe, di, si, 1, 0x6c, 0xc8);
			bp04->CloudType(bp04->CloudType() +1);
			goto _1aae;
		case missileDustCloud: // 0x28: //^_1a6b
			//^075F:1A6B
			if (bp04->b3_0_f() <= 55)
				break;
			bp04->b3_0_f(bp04->b3_0_f() -40);
			goto _1aae;
		case missilePoisonCloud: // 7: //^_1a86
			//^075F:1A86
			if (bp04->b3_0_f() >= 6) {
				//^075F:1A95
				bp04->b3_0_f(bp04->b3_0_f() -3);
				//^075F:1AAE
_1aae:
				bp12 = *ref;
				bp12.SetTick(bp12.GetTick() +1);
				QUEUE_TIMER(&bp12);
				return;
			}
			break;
		}
	}
	//^075F:1AD7
	if (glbCurrentMapIndex == glbPlayerMap) {
		if (bp04->CloudType() == (missileLightning) || bp04->CloudType() == (missileFireball) || bp04->CloudType() == (missileThunderBolt)) {
			//^075F:1B06
			glbDoLightCheck = 1;
		}
	}
	//^075F:1B0C
    CUT_RECORD_FROM(ref->id8(), NULL, di, si);
	DEALLOC_RECORD(ref->id8());
	//^075F:1B2E
	return;
}
//^3A15:0D10
// SPX: _3a15_0d10 renamed PROCESS_TIMER_DESTROY_DOOR
void SkWinCore::PROCESS_TIMER_DESTROY_DOOR(Timer *ref)
{
	//^3A15:0D10
	ENTER(4);
	//^3A15:0D14
	U8 *bp04 = &glbCurrentTileMap[ref->Xcoord()][ref->Ycoord()];
	*bp04 = (*bp04 & 0xf8) | 0x05;
	if (glbCurrentMapIndex == glbPlayerMap)
		//^3A15:0D54
		glbDoLightCheck = 1;
	//^3A15:0D5A
	return;
}



//^3A15:14C0
void SkWinCore::ACTIVATE_RELAY1(Timer *ref, Actuator *pr4, X16 delayAsMult) 
{
	//^3A15:14C0
	ENTER(4);
	//^3A15:14C5
	if (pr4->OnceOnlyActuator() != 0) {
		//^3A15:14D6
		if (pr4->RevertEffect() != 0 || ref->ActionType() != 0) {
			//^3A15:14EE
			if (pr4->RevertEffect() == 0 || ref->ActionType() != 1)
				return;
		}
	}
	//^3A15:150F
	U32 bp04;
	if (delayAsMult != 0) {
		//^3A15:1515
		bp04 = glbGameTick +(U32(pr4->ActuatorData()) << pr4->Delay());
	}
	else {
		//^3A15:154A
		bp04 = pr4->ActuatorData() + glbGameTick +pr4->Delay();
	}
	//^3A15:157C
	INVOKE_MESSAGE(
		pr4->Xcoord(), pr4->Ycoord(), pr4->Direction(), 
		(pr4->OnceOnlyActuator() != 0) ? pr4->ActionType() : ref->ActionType(), bp04);
	//^3A15:15D5
	return;
}
//^3A15:13CB
void SkWinCore::ACTIVATE_RELAY2(Timer *ref, Actuator *pr4, X16 xx)
{
	//^3A15:13CB
	ENTER(0);
	//^3A15:13D0
	X16 di = xx;
	if (pr4->ActionType() == 3) {
		//^3A15:13E8
		if (pr4->RevertEffect() != 0) {
			//^3A15:13F6
			INVOKE_ACTUATOR(pr4, ref->ActionType(), 0);
			if (di == 0) {
				X16 si;
				//^3A15:1417
				switch (ref->ActionType()) {
				case 0: //^_1430
					//^3A15:1430
					si = 1;
					break;
				case 1: //^_1435
					//^3A15:1435
					si = 0;
					break;
				case 2: //^_1439
					//^3A15:1439
					si = 2;
					break;
				}
				//^3A15:143C
				INVOKE_ACTUATOR(pr4, si, di);
			}
			return;
		}
		//^3A15:144E
		INVOKE_ACTUATOR(pr4, ref->ActionType(), di);
		return;
	}
	//^3A15:1469
	if (pr4->RevertEffect() != 0 || ref->ActionType() != 0) {
		//^3A15:1484
		if (pr4->RevertEffect() == 0 || ref->ActionType() != 1)
			return;
	}
	//^3A15:149F
	INVOKE_ACTUATOR(pr4, pr4->ActionType(), di);
	//^3A15:14BC
	return;
}
//^3A15:0D5C
void SkWinCore::_3a15_0d5c(Timer *ref, Actuator *pr4) { // TODO: Unr
	Unr();
}
//^3A15:12B8
void SkWinCore::ACTIVATE_ORNATE_ANIMATOR(Timer *ref, Actuator *pr4, ObjectID rl, X16 isWall)
{
	//^3A15:12B8
	ENTER(12);
	//^3A15:12BE
	X16 di = ref->XcoordB();
	X16 si = ref->YcoordB();
	if (pr4->ActiveStatus() == 0) {
		//^3A15:12E2
		pr4->ActiveStatus(1);
		pr4->ActuatorData(0);
		Timer bp0a;
		bp0a.SetMap(glbCurrentMapIndex);
		bp0a.SetTick(glbGameTick +1);
		bp0a.TimerType(ttyOrnateAnimator);
		bp0a.actor = TIMER_ACTOR__00;
		bp0a.id6(rl);
		bp0a.w8 = isWall;
		QUEUE_TIMER(&bp0a);
		//^3A15:132F
		if (pr4->SoundEffect() != 0) {
			//^3A15:1340
			X8 bp0b;
			X8 bp0c;
			if (isWall != 0) {
				//^3A15:1346
				bp0b = GDAT_CATEGORY_WALL_GFX;	// 9
				bp0c = GET_WALL_DECORATION_OF_ACTUATOR(pr4);
			}
			else {
				//^3A15:1357
				bp0b = GDAT_CATEGORY_FLOOR_GFX;	// 10
				bp0c = GET_FLOOR_DECORATION_OF_ACTUATOR(pr4);
			}
			//^3A15:136B
			// SPX: Activation sound (0x88) from wall or floor actuator
			QUEUE_NOISE_GEN2(bp0b, bp0c, SOUND_STD_ACTIVATION, 0xfe, di, si, QUERY_GDAT_ENTRY_DATA_INDEX(bp0b, bp0c, dtWordValue, 0x88) +1,
				0x8c, 0x80
				);
		}
	}
	//^3A15:13A0
	if (pr4->OnceOnlyActuator() != 0) {
		//^3A15:13B1
		ACTIVATE_RELAY2(ref, pr4, 0);
	}
	//^3A15:13C7
	return;
}
//^3A15:10DC
void SkWinCore::ACTIVATE_CONTINUOUS_ORNATE_ANIMATOR(ObjectID rl, Timer *ref, Actuator *pr4, X16 isWall)
{
	//^3A15:10DC
	ENTER(12);
	//^3A15:10E2
	X16 bp02 = pr4->OnceOnlyActuator();
	pr4->OnceOnlyActuator(_3a15_1da8(ref->ActionType(), pr4->OnceOnlyActuator()));
	i16 di;
	if (pr4->OnceOnlyActuator() != bp02) {
		//^3A15:1132
		U16 si = GET_ORNATE_ANIM_LEN(pr4, isWall);
		if (pr4->OnceOnlyActuator() == 1) {
			//^3A15:1159
			if (pr4->ActiveStatus() == 0) {
				//^3A15:1167
				pr4->ActiveStatus(1);
				if (glbCurrentMapIndex == glbPlayerMap)
					glbDoLightCheck = 1;
				//^3A15:117B
				pr4->ActuatorData((pr4->ActuatorData() & 0x100) | (((si -(glbGameTick % si)) % si)&0xff));
				TRY_ORNATE_NOISE(pr4, rl, ref->XcoordB(), ref->YcoordB(), si, isWall);
			}
		}
		else {
			//^3A15:11ED
			di = (X16)(((pr4->ActuatorData() & 0xff) + glbGameTick ) % si);
			if (di == 0) {
				//^3A15:121A
				if (glbCurrentMapIndex == glbPlayerMap)
					glbDoLightCheck = 1;
				pr4->ActiveStatus(0);
			}
			else {
				//^3A15:1233
				di = si -di;
				Timer bp0c;
				bp0c.SetMap(glbCurrentMapIndex);
				bp0c.SetTick(glbGameTick +di);
				bp0c.TimerType(tty59);
				bp0c.actor = TIMER_ACTOR__00;
				bp0c.id8(rl);
				QUEUE_TIMER(&bp0c);
			}
		}
	}
	//^3A15:127A
	if (pr4->RevertEffect() != 0 && pr4->ActionType() == 3) {
		//^3A15:129A
		INVOKE_ACTUATOR(pr4, ref->ActionType(), 0);
	}
	//^3A15:12B4
	return;
}


//^3A15:1EA8
void SkWinCore::ACTIVATE_ITEM_TELEPORT(Timer *ref, Actuator *pr4, ObjectID rl, X16 isFloor, X16 recycler, ObjectID *prl, X16 capture, X16 onlyFirstItem)
{
	// (capture != 0) -> capture from actuator's target, then put to actuator's location. (capture)
	// (capture == 0) -> capture from actuator's location, then put to actuator's target. (teleport)

	//^3A15:1EA8
	ENTER(30);
	//^3A15:1EAE
	X16 bp16;
	X16 di;
	X16 bp1c;
	X16 bp18;
	X16 bp1a;
	X16 bp1e;
	if (capture != 0) {
		//^3A15:1EB4
		bp16 = pr4->Xcoord();
		di = pr4->Ycoord();
		bp1c = pr4->Direction();
		bp18 = ref->XcoordB();
		bp1a = ref->YcoordB();
		bp1e = ref->Value2();
	}
	else {
		//^3A15:1EFA
		bp16 = ref->XcoordB();
		di = ref->YcoordB();
		bp1c = ref->Value2();
		bp18 = pr4->Xcoord();
		bp1a = pr4->Ycoord();
		bp1e = pr4->Direction();
	}
	//^3A15:1F41
	if (pr4->RevertEffect() != 0 || ref->ActionType() != 0) {
		//^3A15:1F5C
		if (pr4->RevertEffect() == 0 || ref->ActionType() != 1)
			return;
	}
	//^3A15:1F7D
	if (rl == OBJECT_NULL) {
		//^3A15:1FA5
		rl = GET_TILE_RECORD_LINK(bp16, di);
		if ((GET_TILE_VALUE(bp16, di) >> 5) != ttWall)
			isFloor = 1;
	}
	//^3A15:1FAA
	ObjectID si = rl;
	i16 bp02 = 0;
	//^3A15:1FB2
	for (; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
		//^3A15:1FB4
		i16 bp04 = si.DBType();
		if (bp04 >= dbWeapon && bp04 <= dbMiscellaneous_item) {
			//^3A15:1FCA
			if (isFloor != 0 || si.Dir() == bp1c) {
				//^3A15:1FDA
				bp02++;
			}
		}
		//^3A15:1FDD
	}
	//^3A15:1FEB
	si = rl;
	U8 *bp0a;
	if (recycler != 0) {
		//^3A15:1FF7
		bp0a = ALLOC_MEMORY_RAM(0x40, afDefault, 0x400);
		QUERY_CREATURES_ITEM_MASK(U8(pr4->ActuatorData()), U8((pr4->ShopItemPoolNo() * 3) +0x10), bp0a, 0);
	}
	//^3A15:203F
	while (si != OBJECT_END_MARKER) {
		//^3A15:2042
		i16 bp04 = si.DBType();
		ObjectID bp10 = GET_NEXT_RECORD_LINK(si);
		if (bp02 > 0 && bp04 >= dbWeapon && bp04 <= dbMiscellaneous_item) {
			//^3A15:2072
			if (isFloor != 0 || si.Dir() == bp1c) {
				//^3A15:2085
				bp02--;
				X16 bp06 = GET_DISTINCTIVE_ITEMTYPE(si);
				X16 bp0c;
				if (recycler != 0) {
					//^3A15:2098
					bp0c = ((bp0a[bp06 >> 3] & (1 << (bp06 & 7))) != 0) ? 1 : 0;
				}
				else {
					//^3A15:20BB
					if (pr4->ActuatorData() == bp06) {
						//^3A15:20CD
						if (pr4->OnceOnlyActuator() == 0) {
							//^3A15:20DB
							if (QUERY_CLS1_FROM_RECORD(si) == GDAT_CATEGORY_MISCELLANEOUS && QUERY_CLS2_FROM_RECORD(si) == 0) {
								//^3A15:20F1
								ObjectID bp0e;
								bp0e = si;
								si = bp10;
								CUT_RECORD_FROM(bp0e, prl, (prl != NULL) ? -1 : bp16, di);
								// SPX: There I wonder why the DEALLOC comes first ? ... I inverted the order in fixed mode
								if (!SkCodeParam::bUseFixedMode) {
									DEALLOC_RECORD(bp0e);
									BRING_CHAMPION_TO_LIFE(ADD_ITEM_CHARGE(bp0e, 0));
								}
								else if (SkCodeParam::bUseFixedMode)
								{
									BRING_CHAMPION_TO_LIFE(ADD_ITEM_CHARGE(bp0e, 0));
									DEALLOC_RECORD(bp0e);
								}
								continue;
							}
						}
						//^3A15:2139
						bp0c = 1;
					}
					else {
						//^3A15:2140
						if (pr4->ActuatorData() != 0x1ff) {
							//^3A15:2155
							bp0c = 0;
						}
						else {
							//^3A15:2152
							bp0c = 1;
						}
					}
				}
				//^3A15:215A
				if (bp0c != 0) {
					//^3A15:2163
					MOVE_ITEM_TO(si, pr4, bp16, di, bp18, bp1a, bp1e, (prl != NULL) ? 1 : 0, pr4->OnceOnlyActuator());
					//^3A15:219C
					if (onlyFirstItem != 0) {
						break;
					}
				}
			}
		}
		else {
			//^3A15:21A4
			if (recycler == 0 && bp04 == dbCreature) {
				//^3A15:21B0
				Creature *bp14 = GET_ADDRESS_OF_RECORD4(si);
				ACTIVATE_ITEM_TELEPORT(ref, pr4, bp14->possession, isFloor, recycler, &bp14->possession, capture, onlyFirstItem);
			}
		}
		//^3A15:21EC
		si = bp10;
		//^3A15:21EF
	}
	//^3A15:21F7
	if (recycler != 0) {
		DEALLOC_UPPER_MEMORY(0x40);
	}
	//^3A15:2208
	return;
}

//^3A15:15D8
// SPX: _3a15_15d8 renamed ACTIVATE_INVERSE_FLAG
void SkWinCore::ACTIVATE_INVERSE_FLAG(Timer *ref, Actuator *pr4) 
{
	//^3A15:15D8
	ENTER(0);
	//^3A15:15DB
	UPDATE_GLOB_VAR(pr4->ActuatorData(), 1, ((pr4->RevertEffect() != 0) ? 3 : 0) +ref->ActionType());
	INVOKE_ACTUATOR(pr4, (pr4->OnceOnlyActuator() != 0) ? pr4->ActionType() : ref->ActionType(), 0);
	//^3A15:164E
	return;
}
//^3A15:1650
// SPX: _3a15_1650 renamed ACTIVATE_TEST_FLAG. That one was activated just after the Dragon Door.
// This checks the flag value then triggers destination trigger.
void SkWinCore::ACTIVATE_TEST_FLAG(Timer *ref, Actuator *pr4)
{
	//^3A15:1650
	ENTER(0);
	//^3A15:1655
	U16 si = ref->b9_0_0();
	X16 di = (GET_GLOB_VAR(pr4->ActuatorData()) != 0) ? 1 : 0;
	//^3A15:1681
	if (false
		|| ((pr4->RevertEffect() != di) && (si == 0 || si == 2))
		|| ((pr4->RevertEffect() == di) && (si == 1))
	) {
		//^3A15:16B1
		INVOKE_ACTUATOR(pr4, (pr4->OnceOnlyActuator() != 0) ? pr4->ActionType() : ref->ActionType(), 0);
	}
	//^3A15:16E8
	return;
}

//^3A15:1B7F
void SkWinCore::ACTIVATE_SHOOTER(Actuator *pr4, Timer *ref)
{
	//^3A15:1B7F
	ENTER(20);
	//^3A15:1B85
	X16 bp06 = ref->XcoordB();
	X16 bp08 = ref->YcoordB();
	X16 bp02 = ref->Value2();
	X16 bp0e = (bp02 +2)&3;
	X16 bp04 = pr4->ActuatorType();
	X16 bp10 = pr4->ActuatorData();
	X16 bp12 = pr4->w6_4_f()&255;
	X16 bp14 = pr4->w6_4_f() >> 8;
	//^3A15:1BE5
	X16 bp0a = (bp04 == 7 || bp04 == 8 || bp04 == 14) ? 1 : 0;
	ObjectID si = ObjectID::MissileRecord(bp10);
	ObjectID di = si;
	//^3A15:1C0B
	if (false
		|| bp04 == 14 // 0x0e: Item shooter
		|| bp04 == 15 // 0x0f: -
	) {
		//^3A15:1C1A
		for (di = GET_TILE_RECORD_LINK(bp06, bp08); di != OBJECT_END_MARKER; di = GET_NEXT_RECORD_LINK(di)) {
			//^3A15:1C29
			X16 bp0c = di.Dir();
			//^3A15:1C33
			if (di.DBType() <= dbActuator)
				continue;
			//^3A15:1C3E
			if (bp0c == bp02 || ((bp02 +1)&3) == bp0c)
				break;
			//^3A15:1C52
		}
		//^3A15:1C60
		if (di == OBJECT_END_MARKER)
			return;
		//^3A15:1C68
		CUT_RECORD_FROM(di, NULL, bp06, bp08);
		if (bp0a == 0) {
			//^3A15:1C84
			for (si = GET_TILE_RECORD_LINK(bp06, bp08); si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
				//^3A15:1C93
				X16 bp0c = si.Dir();
				if (si.DBType() <= dbActuator)
					continue;
				if (bp0c == bp02 || ((bp02 +1)&3) == bp0c)
					break;
				//^3A15:1CBC
			}
			//^3A15:1CCA
			if (si == OBJECT_END_MARKER)
				goto _1d1b;
			//^3A15:1CD1
			CUT_RECORD_FROM(si, NULL, bp06, bp08);
		}
	}
	//^3A15:1CE6
	else if (true
		 && bp04 != 8  // 0x08: Trap shooter
		 && bp04 != 10 // 0x0a: Trap shooter 2
	) {
		//^3A15:1CF2
		di = ALLOC_NEW_DBITEM(bp10);
		if (di == OBJECT_NULL)
			return;
		//^3A15:1D05
		if (bp0a == 0) {
			//^3A15:1D0B
			si = ALLOC_NEW_DBITEM(bp10);
			if (si == OBJECT_NULL) {
				//^3A15:1D1B
_1d1b:
				bp0a = 1;
			}
		}
	}
	//^3A15:1D20
	if (bp0a != 0) {
		//^3A15:1D26
		bp0e = (bp0e +RAND01())&3;
	}
	//^3A15:1D37
	bp06 += glbXAxisDelta[bp02];
	bp08 += glbYAxisDelta[bp02];
	_4976_4b7e = 1;
	SHOOT_ITEM(di, bp06, bp08, bp0e, bp02, (U8)bp12, 100, (U8)bp14);
	if (bp0a == 0) {
		//^3A15:1D7A
		SHOOT_ITEM(si, bp06, bp08, (bp0e +1)&3, bp02, (U8)bp12, 100, (U8)bp14);
	}
	//^3A15:1D9E
	_4976_4b7e = 0;
	//^3A15:1DA4
	return;
}

//^3A15:220C
void SkWinCore::ACTUATE_WALL_MECHA(Timer *ref)
{
	// activate wall mechanics (like text/actuators)

	//^3A15:220C
	ENTER(54);
	//^3A15:2212
	ObjectID si;
	for (si = GET_TILE_RECORD_LINK(ref->XcoordB(), ref->YcoordB()); si != OBJECT_END_MARKER && si.DBType() <= dbActuator; si = GET_NEXT_RECORD_LINK(si)) {
		//^3A15:222D
		X16 bp14 = si.DBType();
		if (bp14 == dbText) {
			//^3A15:2240
			if (si.Dir() == ref->Value2()) {
				//^3A15:2255
				Text *bp08 = GET_ADDRESS_OF_RECORD2(si);
				if (bp08->TextMode() != 0 && bp08->TextMode() != 1)
					continue;
				//^3A15:2283
				X16 bp10 = bp08->SimpleTextExtUsage();
				if (!SkCodeParam::bDM1Mode && (bp10 != 5 && bp10 != 7)) // DM1 Compatibility mode or DM2 Ext condition
					continue;
				//^3A15:22A6
				// bp08->TextVisibility((ref->ActionType() == 2 [TOGGLE] ) ? !bp08->TextVisibility() : ((ref->ActionType() == 0 [OPEN] ) ? 1 [VISIBLE] : 0 [NOT VISIBLE]));
				bp08->TextVisibility((ref->ActionType() == ACTMSG_TOGGLE) ? !bp08->TextVisibility() : ((ref->ActionType() == ACTMSG_OPEN_SET) ? 1 : 0));
				if (bp10 != 7)
					continue;
				//^3A15:22EA
				i16 bp2e = (QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp08->OrnateIndex(), dtWordValue, GDAT_WALL_ORNATE__IS_LADDER_UP) != 0) ? -1 : 1;
				i16 bp2a = ref->XcoordB();
				i16 bp2c = ref->YcoordB();
				bp2e = LOCATE_OTHER_LEVEL(glbPlayerMap, bp2e, &bp2a, &bp2c, NULL);
				if (bp2e < 0)
					continue;
				//^3A15:2354
				X16 bp16 = glbCurrentMapIndex;
				CHANGE_CURRENT_MAP_TO(bp2e);
				//^3A15:2363
				ObjectID di;
				for (di = GET_TILE_RECORD_LINK(bp2a, bp2c); di != OBJECT_END_MARKER; di = GET_NEXT_RECORD_LINK(di)) {
					//^3A15:2372
					if (di.DBType() != dbText || di.Dir() != ref->Value2())
						continue;
					//^3A15:2391
					Text *bp32 = GET_ADDRESS_OF_RECORD2(di);
					if (bp32->TextMode() != 1 || bp32->SimpleTextExtUsage() != 7)
						continue;
					//^3A15:23C4
					bp32->TextVisibility(bp08->TextVisibility());
					//^3A15:23DD
				}
				//^3A15:23EB
				CHANGE_CURRENT_MAP_TO(bp16);
				continue;
			}
		}
		//^3A15:23F7
		if (bp14 == dbActuator) {
			Actuator *bp04 = GET_ADDRESS_OF_ACTU(si);
			X16 bp10 = bp04->ActuatorType();
			X16 bp0a = bp04->ActuatorData();
			Timer bp20;
			X16 bp2a;
			X16 bp2c;
			X16 bp24;
			X16 bp26;
			ObjectID bp28;
			X16 bp0c;
			X16 bp0e;
			X16 bp22;
			X16 bp12;
			ObjectID di;
			Door *door;	//*bp36

			// SPX: in case of DM1 TRIGGER (BIT FIELDS), the actual position of the actuator is not relevant since the effect direction/facing is used to change the bit field.
			// Then, we check if the target actuator is a TRIGGER, and let activation thereafter
			//^3A15:2400
			if (bp10 != ACTUATOR_TYPE_DM1_BITFIELDS_TRIGGER && bp10 != ACTUATOR_TYPE_DM1_COUNTER 
				&& (si.Dir() != ref->Value2()))
				continue;
					
			switch (bp10) {
			case ACTUATOR_TYPE_WORK_TIMER://^2451 // 0x31 -> 'Activator, work timer'
				//^3A15:2451
				if (bp04->ActiveStatus() != 0)
					break;
				//^3A15:2462
				bp20.SetMap(glbCurrentMapIndex);
				bp20.SetTick(glbGameTick +bp04->ActuatorData());
				bp20.TimerType(tty5B);
				bp20.actor = TIMER_ACTOR__00;
				bp20.id6(si);
				QUEUE_TIMER(&bp20);
				//^3A15:24A9
				bp04->ActiveStatus(1);
				if (bp04->RevertEffect() != 0 || ref->ActionType() != 0) {
					//^3A15:24C9
					if (bp04->RevertEffect() == 0)
						break;
					if (ref->ActionType() != 1)
						break;
				}
				//^3A15:24EA
				INVOKE_ACTUATOR(bp04, (bp04->OnceOnlyActuator() != 0) ? bp04->ActionType() : ref->ActionType(), 0);
				break;
			case ACTUATOR_TYPE_CREATURE_GENERATOR://^2524 // 0x2e -> 'Creature generator'
				//^3A15:2524
				if (ref->ActionType() != 0)
					break;
				//^3A15:2531
				bp2a = bp04->Xcoord();
				bp2c = bp04->Ycoord();
				bp24 = 7;
				bp26 = (bp04->OnceOnlyActuator() == 0) ? RAND02() : (bp04->ActionType());
				bp28 = ALLOC_NEW_CREATURE(bp04->ActuatorData(), bp24, bp26, bp2a, bp2c);
				if (bp04->RevertEffect() != 0 && bp28 != OBJECT_NULL) {
					//^3A15:25B4
					GET_ADDRESS_OF_RECORD(bp28)->castToCreature()->w8 = bp04->Delay();
				}
				//^3A15:25D3
				if (bp04->SoundEffect() == 0)
					break;
				//^3A15:25E7
				QUEUE_NOISE_GEN1(GDAT_CATEGORY_MESSAGES, 0, SOUND_STD_TELEPORT_MESSAGE, 0x61, 0x80, bp2a, bp2c, 1);
				break;
			case ACTUATOR_TYPE_DM1_BITFIELDS_TRIGGER: // SPX: retrocompatibility, DM1 BitFields Trigger
				{
					U8 iBitFields = bp04->ActuatorData()>>4;
					U8 iOldBitFields = iBitFields;
					U8 iChangeableBit = 0;
					U8 iEffectType = bp04->ActionType();
					U8 iActionType = ACTMSG_OPEN_SET; // default
					U8 iBitOperation = ref->ActionType();

					U8 iTargetBitfield = bp04->BitfieldTarget();
					U8 iCurrentBitfield = bp04->BitfieldCurrent();
//					printf("[%08X] (%02d,%02d) TARGET BITFIELD = %02X (%02X)  CUR. BITFIELD = %X with EFFECT %d FACING %d\n", bp04, ref->XcoordB(), ref->YcoordB(), 
//						iTargetBitfield, bp04->ActuatorData(), iCurrentBitfield, ref->ActionType(), ref->Value2());

					iChangeableBit = 1<<ref->Value2(); // power of 2 of the target face/dir
					iBitFields = iBitFields ^ iChangeableBit;

					if (iBitOperation == ACTMSG_OPEN_SET)
						iCurrentBitfield = iCurrentBitfield | iChangeableBit; // add bit
					else if (iBitOperation == ACTMSG_TOGGLE)
						iCurrentBitfield = iCurrentBitfield ^ iChangeableBit;	// toggle bit
					else // clear bit ACTMSG_CLOSE_CLEAR
						iCurrentBitfield = iCurrentBitfield & ~(iChangeableBit);	// clear bit

					//bp04->ActuatorData((iBitFields<<4) + (bp04->ActuatorData()%16));
					bp04->ActuatorData((iTargetBitfield<<4) + (iCurrentBitfield));

//					printf("[%08X] (%02d,%02d) TARGET BITFIELD = %02X (%02X)  CUR. BITFIELD = %X from BITCHANGE %X\n", bp04, ref->XcoordB(), ref->YcoordB(),
//						iTargetBitfield, bp04->ActuatorData(), iCurrentBitfield, iBitFields, iChangeableBit);
					//if (iOldBitFields != iBitFields && iBitFields == 0) // trigger actuator
					if (iCurrentBitfield == iTargetBitfield) // trigger actuator
					{
						if (iEffectType == ACTEFFECT_STEP_CONSTANT__OPEN || iEffectType == ACTEFFECT_STEP_ON__OPEN_SET)
							iActionType = ACTMSG_OPEN_SET;
						else if (iEffectType == ACTEFFECT_STEP_ON__CLOSE_CLEAR)
							iActionType = ACTMSG_CLOSE_CLEAR;
						else if (iEffectType == ACTEFFECT_STEP_ON__TOGGLE)
							iActionType = ACTMSG_TOGGLE;

						INVOKE_ACTUATOR(bp04, iActionType, 0);
					}
					else if (iOldBitFields == 0 && iBitFields != 0)
					{
						if (iEffectType == ACTEFFECT_STEP_CONSTANT__OPEN || iEffectType == ACTEFFECT_STEP_CLOSE__OPEN_SET)
							iActionType = ACTMSG_OPEN_SET;
						else if (iEffectType == ACTEFFECT_STEP_CLOSE__CLOSE_CLEAR)
							iActionType = ACTMSG_CLOSE_CLEAR;
						else if (iEffectType == ACTEFFECT_STEP_CLOSE__TOGGLE)
							iActionType = ACTMSG_TOGGLE;

						INVOKE_ACTUATOR(bp04, iActionType, 0);
					}
					break;
				}
			case ACTUATOR_TYPE_DM1_COUNTER: // SPX: retrocompatibility, DM1 counter
				// DM1 counter does not work the same as DM2 counter
				printf("COUNTER VALUE = %d with EFFECT %d\n", bp04->ActuatorData(), ref->ActionType());
				bp0c = (bp04->ActuatorData() == 0 || (bp04->ActuatorData() & 256) != 0) ? 1 : 0; // not sure what this is
				if (ref->ActionType() == ACTMSG_CLOSE_CLEAR) { // close
					bp04->ActuatorData(bp04->ActuatorData()  - 1);
				}
				else if (ref->ActionType() == ACTMSG_OPEN_SET) { // open
					if (bp04->OnceOnlyActuator() == 0 || bp04->ActuatorData() != 0) {
						bp04->ActuatorData(bp04->ActuatorData() + 1);
					}
				}
				printf("COUNTER VALUE NEW = %d\n", bp04->ActuatorData());
				// then test if the counter equals 0, if so, trigger the counter actuator action
				//bp0e = (bp04->ActuatorData() == 0 || (bp04->ActuatorData() & 256) != 0) ? 1 : 0;
				bp0e = (bp04->ActuatorData() == 0) ? 1 : 0; // if counter is zero, then we can continue to invoke actuator
				if (bp0e == 0)
					break;
				INVOKE_ACTUATOR(bp04, bp04->ActionType(), 0);
				break;
			case ACTUATOR_TYPE_COUNTER://^2606 // 0x1d -> 'Activator, counter'
				//^3A15:2606
				bp0c = (bp04->ActuatorData() == 0 || (bp04->ActuatorData() & 256) != 0) ? 1 : 0;
				//^3A15:2630
				if (ref->ActionType() == 1) { // close
					//^3A15:263A
					bp04->ActuatorData(bp04->ActuatorData() +1);
				}
				else if (ref->ActionType() == 0) { // open
					//^3A15:2654
					if (bp04->OnceOnlyActuator() == 0 || bp04->ActuatorData() != 0) {
						//^3A15:2673
						//^3A15:2681
						bp04->ActuatorData(bp04->ActuatorData() -1);
					}
				}
				//^3A15:2694
				bp0e = (bp04->ActuatorData() == 0 || (bp04->ActuatorData() & 256) != 0) ? 1 : 0;
				if (bp0e == bp0c)
					break;
				//^3A15:26C6
				if (bp04->ActionType() == 3) { // step in : open / step out : close
					//^3A15:26D8
					INVOKE_ACTUATOR(bp04, (bp04->RevertEffect() == bp0e) ? 1 : 0, 0);
				}
				else if (bp0e != 0){
					//^3A15:2707
					INVOKE_ACTUATOR(bp04, bp04->ActionType(), 0);
				}
				break;
			case ACTUATOR_TYPE_FINITE_ACTIVATOR_RELAY://^2728 // 0x2d -> 'Activator, finite activator relay'
				//^3A15:2728
				bp22 = 0;
				if (bp04->ActuatorData() > 0 && bp04->ActuatorData() <= 400) {
					//^3A15:274D
					bp04->ActuatorData(bp04->ActuatorData() -1);
					INVOKE_ACTUATOR(bp04, ref->ActionType(), 0);
					break;
				}
				//^3A15:2776
				if (bp04->ActuatorData() > 400 && bp04->ActuatorData() < 500) {
					//^3A15:279D
					bp22 = (RAND16(0x64) < bp04->ActuatorData() -400) ? 1 : 0;
					if (bp04->ActionType() == 3) {
						//^3A15:27D7
						INVOKE_ACTUATOR(bp04, (bp22 != 0) ? 0 : 1, 0);
						break;
					}
					//^3A15:27EA
					if (bp22 != 0) {
						//^3A15:27EA
						INVOKE_ACTUATOR(bp04, ref->ActionType(), 0);
					}
				}
				break;
			case ACTUATOR_TYPE_TICK_GENERATOR://^2810 // 0x1e -> 'Activator, tick generator'
			case ACTUATOR_TYPE_X33_V008://^2810 // 0x33 -> '-'
			case ACTUATOR_TYPE_X34_V016://^2810 // 0x34 -> '-'
			case ACTUATOR_TYPE_X35_V032://^2810 // 0x35 -> '-'
			case ACTUATOR_TYPE_X36_V064://^2810 // 0x36 -> '-'
			case ACTUATOR_TYPE_X37_V128://^2810 // 0x37 -> '-'
				//^3A15:2810
				bp04->OnceOnlyActuator(_3a15_1da8(ref->ActionType(), bp04->OnceOnlyActuator()));
				if (bp04->ActiveStatus() != 0 || bp04->OnceOnlyActuator() == 0)
					break;
				//^3A15:285F
				ACTIVATE_TICK_GENERATOR(bp04, si);
				break;
			case ACTUATOR_TYPE_RELAY_1://^2871 // 0x20 -> 'Activator, relay 1'
			case ACTUATOR_TYPE_RELAY_3://^2871 // 0x45 -> 'Activator, relay 3'
				//^3A15:2871
				ACTIVATE_RELAY1(ref, bp04, (bp10 == 0x45) ? 1 : 0);
				break;
			case ACTUATOR_TYPE_RELAY_2://^2891 // 0x3d -> 'Activator, relay 2'
				//^3A15:2891
				ACTIVATE_RELAY2(ref, bp04, bp04->ActuatorData());
				break;
			case ACTUATOR_TYPE_SOME_SHOOTER://^28B5 // 0x07 -> 'Activator, trap floor'
			case ACTUATOR_TYPE_MISSILE_SHOOTER://^28B5 // 0x08 -> 'Trap shooter' or 'Activator, trap floor'
			case ACTUATOR_TYPE_WEAPON_SHOOTER://^28B5 // 0x09 -> '-'
			case ACTUATOR_TYPE_MISSILE_SHOOTER_2://^28B5 // 0x0a -> 'Trap shooter 2'
			case ACTUATOR_TYPE_ITEM_SHOOTER://^28B5 // 0x0e -> 'Item shooter'
			case ACTUATOR_TYPE_ITEM_SHOOTER_X2://^28B5 // 0x0f -> '-'
				//^3A15:28B5
				ACTIVATE_SHOOTER(bp04, ref);
				break;
			case ACTUATOR_TYPE_CROSS_MAP://^28C8 // 0x16 -> 'Activator, cross map'
				//^3A15:28C8
				bp20.SetMap(bp0a & 63);
				bp20.SetTick(glbGameTick);
				bp20.actor = ref->actor;
				bp20.XcoordB(bp04->Xcoord());
				bp20.YcoordB(bp04->Ycoord());
				bp12 = glbMapTileValue[bp0a & 63][bp20.XcoordB()][bp20.YcoordB()] >> 5;
				bp20.Value2((bp12 == 0) ? ((bp0a >> 6)&3) : 0);
				bp20.TimerType(tty04);
				bp20.ActionType(ref->ActionType());
				QUEUE_TIMER(&bp20);
				break;
			case ACTUATOR_TYPE_THE_END://^2976 // 0x12 -> 'The END'
				//^3A15:2976
				FIRE_SELECT_PALETTE_SET(0);
				_4976_4c26 = 1;
				SK_PREPARE_EXIT();
				break;
			case ACTUATOR_TYPE_ORNATE_ANIMATOR_2://^298C // 0x32 -> 'Activator, ornate animator'
				//^3A15:298C
				ACTIVATE_ORNATE_ANIMATOR(ref, bp04, si, 1);
				break;
			case ACTUATOR_TYPE_ORNATE_STEP_ANIMATOR://^29A1 // 0x41 -> 'Ornate step animator'
				//^3A15:29A1
				(ref->ActionType() == 0) ? bp0a++ : bp0a--;
				//^3A15:29B3
				bp04->ActuatorData((bp0a % GET_ORNATE_ANIM_LEN(bp04, 1)) & 511);
				break;
			case ACTUATOR_TYPE_ORNATE_ANIMATOR://^29E2 // 0x2c -> 'Continuous ornate animator'
				//^3A15:29E2
				ACTIVATE_CONTINUOUS_ORNATE_ANIMATOR(si, ref, bp04, 1);
				break;
			case ACTUATOR_TYPE_SWITCH_SIGN_FOR_CREATURE://^29FB // 0x26 -> 'Switch sign for creature' or 'Activator, missile explosion'
				//^3A15:29FB
				bp04->OnceOnlyActuator(_3a15_1da8(ref->ActionType(), bp04->OnceOnlyActuator()));
				break;
			case ACTUATOR_TYPE_PLACED_ITEM_TELEPORTER://^2A2E // 0x3b -> 'Item teleporter (placed item)'
				//^3A15:2A2E
				ACTIVATE_ITEM_TELEPORT(ref, bp04, si, 0, 0, NULL, 0, 0);
				break;
			case ACTUATOR_TYPE_ITEM_CAPTURE://^2A3D // 0x47 -> 'Item capture'
				//^3A15:2A3D
				ACTIVATE_ITEM_TELEPORT(ref, bp04, OBJECT_NULL, 0, 0, NULL, 1, 0);
				break;
			case ACTUATOR_TYPE_ITEM_RECYCLER://^2A41 // 0x40 -> 'Item recycler'
				//^3A15:2A41
				ACTIVATE_ITEM_TELEPORT(ref, bp04, si, 0, 1, NULL, 0, 0);
				break;
			case ACTUATOR_TYPE_ITEM_TELEPORT_UNKNOWN://^2A4D // 0x48 -> '-'
				//^3A15:2A4D
				ACTIVATE_ITEM_TELEPORT(ref, bp04, si, 0, 0, NULL, 0, 1);
				break;
			case ACTUATOR_FLOOR_TYPE__ITEM_CAPTURE_FROM_CREATURE://^2A51 // 0x49 -> 'Item capture from creature'
				//^3A15:2A51
				ACTIVATE_ITEM_TELEPORT(ref, bp04, OBJECT_NULL, 0, 0, NULL, 1, 1);
				break;
			case ACTUATOR_TYPE_ITEM_GENERATOR://^2A75 // 0x3c -> 'Item generator'
				//^3A15:2A75
				if (bp04->RevertEffect() != 0 || ref->ActionType() != 0) {
					//^3A15:2A90
					if (bp04->RevertEffect() == 0 || ref->ActionType() != 0)
						break;
				}
				//^3A15:2AB1
				di = ALLOC_NEW_RECORD(GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(bp04->ActuatorData()));
				if (di == OBJECT_NULL)
					break;
				//^3A15:2AD6
				SET_ITEMTYPE(di, GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(bp04->ActuatorData()));
				if (bp04->ActionType() == 3) {
					//^3A15:2B05
					SET_ITEM_IMPORTANCE(di, 1);
				}
				//^3A15:2B0F
				MOVE_ITEM_TO(di, bp04, -1, -1, bp04->Xcoord(), bp04->Ycoord(), bp04->Direction(), 0, bp04->OnceOnlyActuator());
				break;
			case ACTUATOR_FLOOR_TYPE__CREATURE_DIRECTION://^2B55 // 0x42 -> 'Change creature facing direction'
				//^3A15:2B55
				_3a15_0d5c(ref, bp04);
				break;
			case ACTUATOR_TYPE_INVERSE_FLAG://^2B67 // 0x43 -> 'Activator, inverse flag'
				//^3A15:2B67
				ACTIVATE_INVERSE_FLAG(ref, bp04);
				break;
			case ACTUATOR_TYPE_TEST_FLAG://^2B79 // 0x44 -> 'Activator, test flag'
				//^3A15:2B79
				ACTIVATE_TEST_FLAG(ref, bp04);
				break;
			case ACTUATOR_TYPE_PUSH_BUTTON_WALL_SWITCH://^2B8E // 0x46 -> 'Activator, seal-able push button wall switch'
				//^3A15:2B8E
				// SPX: these actions work for door!
				door = GET_ADDRESS_OF_TILE_RECORD(bp04->Xcoord(), bp04->Ycoord())->castToDoor();
				switch (ref->ActionType()) {
				case 0: //^_2bcd
					//^3A15:2BCD
                    door->DoorBit13B(1);
					break;
				case 1: //^_2bd7
					//^3A15:2BD7
					door->DoorBit13B(0);
					break;
				case 2: //^_2be1
					//^3A15:2BE1
					door->DoorBit13B(door->DoorBit13() ^ 1);
					break;
				}
				break;
			case 11://^2C00
			case 12://^2C00
			case 13://^2C00
			case 16://^2C00
			case 17://^2C00
			case 19://^2C00
			case 20://^2C00
			case 21://^2C00
			case 23://^2C00
			case 24://^2C00
			case 25://^2C00
			case 26://^2C00
			case 27://^2C00
			case 28://^2C00
			case 31://^2C00
			case 33://^2C00
			case 34://^2C00
			case 35://^2C00
			case 36://^2C00
			case 37://^2C00
			case 39://^2C00
			case 40://^2C00
			case 41://^2C00
			case 42://^2C00
			case 43://^2C00
			case 47://^2C00
			case 48://^2C00
			case 56://^2C00
			case 57://^2C00
			case 58://^2C00
			case 62://^2C00
			case 63://^2C00
				break;
			}
		}
		//^3A15:2C00
	}
	//^3A15:2C1E
	return;
}


//^3A15:16EC
void SkWinCore::ACTUATE_FLOOR_MECHA(Timer *ref)
{
	// activate floor mechanics

	//^3A15:16EC
	ENTER(220);
	//^3A15:16F2
	X16 di;
	X16 bp0a;
	ObjectID si;
	for (si = GET_TILE_RECORD_LINK(bp0a = ref->XcoordB(), di = ref->YcoordB()); si != OBJECT_END_MARKER && si.DBType() <= dbActuator; si = GET_NEXT_RECORD_LINK(si)) {
		//^3A15:1712
		X16 bp0e = si.DBType();
		if (bp0e == dbText) {
			//^3A15:1725
			Text *bp08 = GET_ADDRESS_OF_RECORD2(si);
			if (bp08->TextMode() != 0) {
				if ((bp08->TextMode() != 1 || bp08->SimpleTextExtUsage() != 5) && (bp08->TextMode() != 2 || bp08->w2_c_f() != 2))
					continue;
			}
			//^3A15:1791
			X16 bp0c = bp08->TextVisibility();
			bp08->TextVisibility((ref->ActionType() == 2) ? !bp0c : ((ref->ActionType() == 0) ? 1 : 0));
			if (bp08->TextMode() != 0 || bp0c != 0 || bp08->TextVisibility() == 0 || glbCurrentMapIndex != glbMap_4c28 || bp0a != glbSomePosX_4c2e || di != glbSomePosY_4c30)
				continue;
			//^3A15:181A
			U8 bp00dc[206];
			QUERY_MESSAGE_TEXT(bp00dc, si, 1);
			DISPLAY_HINT_TEXT(COLOR_WHITE, bp00dc);
			continue;
		}
		//^3A15:183E
		if (bp0e == dbActuator) {
			//^3A15:1847
			Actuator *bp04 = GET_ADDRESS_OF_ACTU(si);
			X16 bp10;
			X16 bp14;
			bp10 = bp14 = bp04->ActuatorType();
			X16 bp12;
			switch (bp14) {
			case ACTUATOR_FLOOR_TYPE__CROSS_SCENE://^187D // 0x27 -> 'Cross scene'
				//^3A15:187D
				bp04->b4_0_0(U8(_3a15_1da8(ref->ActionType(), bp04->b4_0_0())));
				break;
			case ACTUATOR_FLOOR_TYPE__INFINITE_ORNATE_ANIMATOR://^18AB // 0x2c -> 'Continuous ornate animator'
				//^3A15:18AB
				ACTIVATE_CONTINUOUS_ORNATE_ANIMATOR(si, ref, bp04, 0);
				break;
			case ACTUATOR_FLOOR_TYPE__RELAY_1://^18C1 // 0x20 -> 'Activator, relay 1'
			case ACTUATOR_FLOOR_TYPE__RELAY_3://^18C1 // 0x45 -> 'Activator, relay 3'
				//^3A15:18C1
				ACTIVATE_RELAY1(ref, bp04, (bp10 == 0x45) ? 1 : 0);
				break;
			case ACTUATOR_FLOOR_TYPE__RELAY_2://^18E1 // 0x3D -> 'Activator, relay 2'
				//^3A15:18E1
				ACTIVATE_RELAY2(ref, bp04, bp04->ActuatorData());
				break;
			case ACTUATOR_FLOOR_TYPE__CREATURE_KILLER://^1905 // 0x0B -> 'Creature killer'
			case ACTUATOR_FLOOR_TYPE__CREATURE_AI_STATE://^1905 // 0x28 -> '?'
				//^3A15:1905
				ACTIVATE_CREATURE_KILLER(bp04->ActuatorData()&15, 
					(bp04->ActuatorData()>>4)&0x1f, 
					bp0a, 
					di, 
					bp04->Xcoord(),
					bp04->Ycoord(), 
					bp10, 
					ref->ActionType());
				break;

			// SPX : DM1 retrocompatibility : Creature generator
			case ACTUATOR_FLOOR_TYPE__DM1_CREATURE_GENERATOR:
				{
					int iLocationX = 0;
					int iLocationY = 0;
					U16 iDirection = 0;
					U16 iHealthMultiplier = 7;
					ObjectID oNewCreature = OBJECT_NULL;
					//if (ref->ActionType() != 0)
					//	break;
//					iLocationX = bp04->Xcoord();
//					iLocationY = bp04->Ycoord();
					// location of creature to generate does not come from actuator data, but from current actuator location
					iLocationX = bp0a;
					iLocationY = di;
					iDirection = (bp04->OnceOnlyActuator() == 0) ? RAND02() : (bp04->ActionType());
					oNewCreature = ALLOC_NEW_CREATURE(bp04->ActuatorData(), iHealthMultiplier, iDirection, iLocationX, iLocationY);
					if (bp04->RevertEffect() != 0 && oNewCreature != OBJECT_NULL) {
						//^3A15:25B4
						GET_ADDRESS_OF_RECORD(oNewCreature)->castToCreature()->w8 = bp04->Delay();
					}
					//^3A15:25D3
					if (bp04->SoundEffect() == 0)
						break;
					//^3A15:25E7
					QUEUE_NOISE_GEN1(GDAT_CATEGORY_MESSAGES, 0, SOUND_STD_TELEPORT_MESSAGE, 0x61, 0x80, iLocationX, iLocationY, 1);
					break;
				}
			case ACTUATOR_FLOOR_TYPE__PARTY_TELEPORTER://^1958 // 0x2E -> '-'
				//^3A15:1958
				bp12 = glbCurrentMapIndex;
				if (bp12 != glbPlayerMap) {
					//^3A15:1964
					CHANGE_CURRENT_MAP_TO(glbPlayerMap);
					MOVE_RECORD_TO(OBJECT_NULL, glbPlayerPosX, glbPlayerPosY, -1, 0);
					LOAD_NEWMAP(U8(bp12));
					if (bp04->OnceOnlyActuator() != 0) {
						//^3A15:199E
						MOVE_RECORD_TO(OBJECT_NULL, -1, 0, bp04->Xcoord(), bp04->Ycoord());
					}
					else {
						//^3A15:19BA
						MOVE_RECORD_TO(OBJECT_NULL, -1, 0, bp0a, di);
					}
				}
				//^3A15:19CE
				else if (bp04->OnceOnlyActuator() != 0) {
					//^3A15:19DF
					MOVE_RECORD_TO(OBJECT_NULL, glbPlayerPosX, glbPlayerPosY, bp04->Xcoord(), bp04->Ycoord());
				}
				else {
					//^3A15:19F7
					MOVE_RECORD_TO(OBJECT_NULL, glbPlayerPosX, glbPlayerPosY, bp0a, di);
				}
				//^3A15:1A0D
				if (bp04->RevertEffect() != 0) {
					//^3A15:1A1E
					ROTATE_SQUAD(bp04->ActionType());
				}
				else {
					//^3A15:1A32
					ROTATE_SQUAD((bp04->ActionType() +glbPlayerDir) & 3);
				}
				break;
			case ACTUATOR_FLOOR_TYPE__ORNATE_ANIMATOR://^1A53 // 0x32 -> 'Activator, ornate animator'
				//^3A15:1A55
				ACTIVATE_ORNATE_ANIMATOR(ref, bp04, si, 0);
				break;
			case ACTUATOR_FLOOR_TYPE__CREATURE_ANIMATOR://^1A6C // 0x3a -> 'Creature animator'
				//^3A15:1A6C
				ANIMATE_CREATURE(bp0a, di, (ref->ActionType() == 0) ? 1 : 0);
				break;
			case ACTUATOR_FLOOR_TYPE__ITEM_TELEPORTER://^1A8D // 0x3b -> 'Item teleporter (placed item)'
				//^3A15:1A8D
				ACTIVATE_ITEM_TELEPORT(ref, bp04, si, 1, 0, NULL, 0, 0);
				break;
			case ACTUATOR_TYPE_ITEM_CAPTURE://^1A9C // 0x47 -> 'Item capture'
				//^3A15:1A9C
				ACTIVATE_ITEM_TELEPORT(ref, bp04, OBJECT_NULL, 0, 0, NULL, 1, 0);
				break;
			case ACTUATOR_FLOOR_TYPE__ITEM_RECYCLER://^1AA0 // 0x40 -> 'Item recycler'
				//^3A15:1AA0
				ACTIVATE_ITEM_TELEPORT(ref, bp04, si, 1, 1, NULL, 0, 0);
				break;
			case ACTUATOR_TYPE_ITEM_TELEPORT_UNKNOWN://^1AAC // 0x48 -> '-'
				//^3A15:1AAC
				ACTIVATE_ITEM_TELEPORT(ref, bp04, si, 0, 0, NULL, 0, 1);
				break;
			case ACTUATOR_FLOOR_TYPE__ITEM_CAPTURE_FROM_CREATURE://^1ABA // 0x49 -> 'Item capture from creature'
				//^3A15:1ABA
				ACTIVATE_ITEM_TELEPORT(ref, bp04, OBJECT_NULL, 0, 0, NULL, 1, 1);
				break;
			case ACTUATOR_FLOOR_TYPE__CREATURE_DIRECTION://^1ADE // 0x42 -> 'Change creature facing direction'
				//^3A15:1ADE
				_3a15_0d5c(ref, bp04);
				break;
			case ACTUATOR_TYPE_INVERSE_FLAG://^1AF0 // 0x43 -> 'Activator, inverse flag'
				//^3A15:1AF0
				ACTIVATE_INVERSE_FLAG(ref, bp04);
				break;
			case ACTUATOR_TYPE_TEST_FLAG://^1B02 // 0x44 -> 'Activator, test flag'
				//^3A15:1B02
				ACTIVATE_TEST_FLAG(ref, bp04);
				break;
			}
		}
		//^3A15:1B15
	}
	//^3A15:1B33
	return;
}
//^3A15:2F61
void SkWinCore::ACTUATE_PITFALL(Timer *ref) 
{
	//^3A15:2F61
	ENTER(6);
	//^3A15:2F67
	X16 bp06 = ref->XcoordB();
	X16 di;
	U8 *bp04 = &glbCurrentTileMap[bp06][di = ref->YcoordB()];
	X16 si = ref->ActionType();
	if (si == 2) {
		//^3A15:2FA6
		si = ((bp04[0] & 8) != 0) ? 1 : 0;
	}
	//^3A15:2FB9
	if (si == 0) {
		//^3A15:2FBD
		*bp04 |= 8;
		ADVANCE_TILES_TIME(bp06, di);
	}
	else {
		//^3A15:2FD0
		*bp04 &= 0xf7;
	}
	//^3A15:2FDB
	ACTUATE_FLOOR_MECHA(ref);
	//^3A15:2FE7
	return;
}


//^3A15:1DA8
// SPX: interesting ... If xx = 0 => 1, if xx = 2
// Called from DOOR to change opening direction
// xx = new action direction, yy = current door direction
X16 SkWinCore::_3a15_1da8(X8 iNewDirection, X8 iCurrentDirection)
{
	// SPX: Here is the original code from conversion, but it misses case 1 for door to switch opening direction when asked. Careful to check with other actuators actions.
	// SPX: to be checked: for door it was rater outside this function.
	/*
	ENTER(0);
	switch (xx) {
	case 0: //^_1db9
		return 1;
	case 2: //^_1dc2
		return yy ^1;
	}
	return 0;
	*/
	switch (iNewDirection) {
	case ACTMSG_OPEN_SET:
		return 1;
	case ACTMSG_TOGGLE:
		return iCurrentDirection ^ 1;
	}
	return 0;
}

//^3A15:0ACD
// CSBWin Timer.cpp:TAG0105ba/ProcessTT_DOOR
// This one is called when a door is activated by an actuator/button. It does not process an ongoing door step timer
// The other function to do that is STEP_DOOR
void SkWinCore::ACTUATE_DOOR(Timer *ref)
{
	//^3A15:0ACD
	ENTER(4);
	//^3A15:0AD3
	X16 iOpenCloseState = glbCurrentTileMap[ref->XcoordB()][ref->YcoordB()] & 7; // state of door : 0 is open, 4 is fully closed, 2 is mid-opened
	if (iOpenCloseState == _DOOR_STATE__DESTROYED_) // 5, destroyed, can't operate
		return;
	//^3A15:0B0B
	Door *door = GET_ADDRESS_OF_TILE_RECORD(ref->XcoordB(), ref->YcoordB())->castToDoor();	//*bp04
	if (door->DoorBit10() != 0) { // door is currently moving
		//^3A15:0B3A
		//door->DoorBit10(U8(_3a15_1da8(ref->ActionType(), door->DoorBit09())));
		door->DoorBit09(U8(_3a15_1da8(ref->ActionType(), door->DoorBit09()))); // SPX using Bit09 instead of Bit10 seems correct to change the door direction
		if (door->DoorBit10() != 0)
			return;
		//^3A15:0B6F
		door->DoorBit12(0);
		return;
	}
	//^3A15:0B77
	X16 iNewDoorTimer = 0; // will issue a timer again if 1
	if (iOpenCloseState == 0) {	// totally open
		//^3A15:0B7D
		if (ref->ActionType() == ACTMSG_CLOSE_CLEAR || ref->ActionType() == ACTMSG_TOGGLE) {	// 1 or 2
			//^3A15:0B8E
			door->DoorBit09(DOORACTION_CLOSING); // 0 => closing
			iNewDoorTimer = 1;
		}
	}
	//^3A15:0B9B
	else if (iOpenCloseState == 4) { // totally closed
		//^3A15:0BA0
		if (ref->ActionType() == ACTMSG_OPEN_SET || ref->ActionType() == ACTMSG_TOGGLE) { // 0 or 2
			door->DoorBit09(DOORACTION_OPENING); // 1 => opening
			iNewDoorTimer = 1;
		}
	}
	else {
		//^3A15:0BBE
		door->DoorBit09((ref->ActionType() == ACTMSG_OPEN_SET) ? DOORACTION_OPENING : DOORACTION_CLOSING);	// ? 1 : 0
		iNewDoorTimer = 1;
	}
	//^3A15:0BE3
	door->DoorBit10(U8(iNewDoorTimer));
	if (door->DoorBit10() == 0)
		return;
	//^3A15:0BFD
	if (door->DoorBit09() == DOORACTION_CLOSING)	// 0
		door->DoorBit12(0);
	//^3A15:0C0F
	ref->TimerType(ttyDoorStep);
	QUEUE_TIMER(ref);
	//^3A15:0C23
	return;
}

//^3A15:07B4
void SkWinCore::STEP_DOOR(Timer *ref)
{
	SkD((DLV_DBG_DOOR, "DOOR: STEP_DOOR(TIMER=%04X,T=%d,Actor=%d,V=%2d,W8=%2d\n", (Bitu)ref->dw00, (Bitu)ref->ttype, (Bitu)ref->actor, (Bitu)ref->value, (Bitu)ref->w8));

		Bit32u dw00;	// @0
		Bit8u ttype;		// @4	b04
		Bit8u actor;	// @5	// b5 player index or creature type or other ...
		Bit16u value;		// @6	// w6 => position x & y	// SPX: don't it take object ID ? or spell power ? it can also be an object! (door) ..
		Bit16u w8;		// @8

	//^3A15:07B4
	ENTER(26);
	//^3A15:07BA
	X16 iNewDoorTimer = 0;	// bp18
	X16 bp1a = 0;	// bp1a
	X16 di = ref->XcoordB();
	X16 si = ref->YcoordB();
	U8 *xTileValue = &glbCurrentTileMap[di][si];	// bp04
	X16 iOpenCloseState = *xTileValue & 7; // bp0a
	if (iOpenCloseState == _DOOR_STATE__DESTROYED_) // 5, destroyed, can't operate
		return;
	//^3A15:0807
	if (glbCurrentMapIndex == glbPlayerMap)
		glbDoLightCheck = 1;
	//^3A15:0816
	Door *bp08 = GET_ADDRESS_OF_TILE_RECORD(U8(di), U8(si))->castToDoor();
	if (bp08->DoorBit10() == 0)
		return;
	//^3A15:0839
	ref->SetTick(ref->GetTick() +1);
	if (bp08->DoorBit09() == 0) {
		//^3A15:0858
		if (iOpenCloseState == 4) { // totally closed
			bp08->DoorBit10(0);
			return;
		}
		//^3A15:0861
		X16 bp0e = bp08->b2_5_5();
		X16 bp14 = QUERY_DOOR_STRENGTH(GET_GRAPHICS_FOR_DOOR(bp08));
		if (glbCurrentMapIndex == glbMap_4c28 && di == glbSomePosX_4c2e && si == glbSomePosY_4c30 && iOpenCloseState != 0) {
			//^3A15:08AF
			bp1a = 1;
			if (glbChampionsCount > 0) {
				//^3A15:08BB
				*xTileValue &= 0xF8;
				X16 bp12 = ATTACK_PARTY(bp14, ((8 |bp0e) != 0) ? 4 : 3, 2);
				if (bp12 != 0) {
					//^3A15:08ED
					i16 bp16 = 0;
					for (; bp16 < 4; bp16++) {
						//^3A15:08F4
						if ((bp12 & (1 << bp16)) == 0)
							continue;
						//^3A15:0901
						// SPX: Bump sound when door closing on champions
						QUEUE_NOISE_GEN2(GDAT_CATEGORY_CHAMPIONS, glbChampionSquad[bp16].HeroType(), SOUND_CHAMPION_BUMP, 0xfe, di, si, 1, 0x64, 0xc8);
						//^3A15:0929
					}
				}
			}
		}
		//^3A15:0932
		ObjectID bp0c = GET_CREATURE_AT(di, si);
		if (bp0c != OBJECT_NULL) {
			//^3A15:0946
			X16 bp10 = QUERY_CREATURE_AI_SPEC_FLAGS(bp0c);
			if ((bp10 & 0x20) == 0) {
				//^3A15:095A
				if (((bp0e != 0) ? ((bp10 >> 6)&3) : 1) <= iOpenCloseState) {
					//^3A15:0973
					ATTACK_CREATURE(bp0c, di, si, 0x2006, 0x64, (QUERY_CREATURE_AI_SPEC_FROM_RECORD(bp0c)->w24_c_c() != 0) ? 0 : bp14);
					iOpenCloseState = (iOpenCloseState == 0) ? 0 : (iOpenCloseState -1);
					*xTileValue = *xTileValue & 0xF8 | iOpenCloseState; // write new state on tile
					QUEUE_NOISE_GEN2(GDAT_CATEGORY_CREATURES, QUERY_CLS2_FROM_RECORD(bp0c), SOUND_OBJECT_GETHIT, 0xfe,
						di, si, 1, 0x46, 0x80);
					QUEUE_NOISE_GEN2(GDAT_CATEGORY_MISCELLANEOUS, 0xFE, SOUND_STD_KNOCK, 0xFE, di, si, 1, 0x46, 0x80);
					bp1a = 1;
				}
			}
		}
		//^3A15:0A05
		if (bp1a != 0) {
			//^3A15:0A0B
			ref->SetTick(ref->GetTick() +1);
			iNewDoorTimer = 1;
		}
	}
	else {
		//^3A15:0A1E
		if (iOpenCloseState == 0) {
			bp08->DoorBit10(0);
			return;
		}
	}
	//^3A15:0A27
	if (iNewDoorTimer == 0) { // has arrived either at totally closed or totally opened state
		//^3A15:0A2D
		iOpenCloseState += (bp08->DoorBit09() != 0) ? -1 : 1;
		*xTileValue = (*xTileValue & 0xF8) | U8(iOpenCloseState); // write new state on tile
		// SPX: Door step sound
		QUEUE_NOISE_GEN2(GDAT_CATEGORY_DOORS, GET_GRAPHICS_FOR_DOOR(bp08), SOUND_DOOR_STEP, 0xfe, di, si, 1, 0x5f, 0x80);
		if (SkCodeParam::bUseDM2ExtendedMode && iOpenCloseState == 4) // Special sound for door totally closed (exists in V5)
			QUEUE_NOISE_GEN2(GDAT_CATEGORY_DOORS, GET_GRAPHICS_FOR_DOOR(bp08), SOUND_DOOR_CLOSE, 0xfe, di, si, 1, 0x5f, 0x80);
		else if (SkCodeParam::bUseDM2ExtendedMode && iOpenCloseState == 0) // Special sound for totally door opened
			QUEUE_NOISE_GEN2(GDAT_CATEGORY_DOORS, GET_GRAPHICS_FOR_DOOR(bp08), SOUND_DOOR_OPENED, 0xfe, di, si, 1, 0x5f, 0x80);

		if (bp08->DoorBit09() != 0) {
			if (iOpenCloseState != 0)
				iNewDoorTimer = 1;
		}
		else if (iOpenCloseState != 4) {
			iNewDoorTimer = 1;
		}
		//printf("step : iOpenCloseState = %d\n", iOpenCloseState);
	}
	//^3A15:0AA5
	if (iNewDoorTimer != 0) {
		bp08->DoorBit12(1);
		QUEUE_TIMER(ref);
		return;
	}
	//^3A15:0AC1
	bp08->DoorBit10(0);
	//^3A15:0AC9
	return;
}



//^3A15:2EB5
void SkWinCore::ACTUATE_TELEPORTER(Timer *ref)
{
	//^3A15:2EB5
	ENTER(10);
	//^3A15:2EBB
	X16 bp06 = ref->XcoordB();
	X16 di;
	U8 *bp04 = &glbCurrentTileMap[bp06][di = ref->YcoordB()];
	Teleporter *bp0a = GET_ADDRESS_OF_TILE_RECORD(U8(bp06), U8(di))->castToTeleporter();
	if (bp0a->b4_1_2() != 3) {
		X16 si = ref->ActionType();
		if (si == 2) {
			si = ((*bp04 & 8) != 0) ? 1 : 0;
		}
		//^3A15:2F2F
		if (si == 0) {
			//^3A15:2F33
			*bp04 |= 8;
			ADVANCE_TILES_TIME(bp06, di);
		}
		else {
			//^3A15:2F46
			*bp04 &= 0xf7;
		}
	}
	//^3A15:2F51
	ACTUATE_FLOOR_MECHA(ref);
	//^3A15:2F5D
	return;
}
//^3A15:0C27
void SkWinCore::ACTUATE_TRICKWALL(Timer *ref)
{
	//^3A15:0C27
	ENTER(10);
	//^3A15:0C2D
	X16 si = 0;
	X16 bp06 = ref->XcoordB();
	X16 bp08;
	U8 *bp04 = &glbCurrentTileMap[bp06][bp08 = ref->YcoordB()];
	X16 di = ref->ActionType();
	if (di == 2) {
		di = ((*bp04 & 4) != 0) ? 1 : 0;
	}
	//^3A15:0C82
	if (di == 1) {
		if (glbCurrentMapIndex == glbMap_4c28 && bp06 == glbSomePosX_4c2e && bp08 == glbSomePosY_4c30)
			si = 1;
		//^3A15:0CA5
		ObjectID bp0a = GET_CREATURE_AT(bp06, bp08);
		if (bp0a != OBJECT_NULL && (QUERY_CREATURE_AI_SPEC_FLAGS(bp0a) & 0x20) == 0)
			si = 1;
		//^3A15:0CCB
		if (si != 0) {
			//^3A15:0CCF
			ref->SetTick(ref->GetTick() +1);
			QUEUE_TIMER(ref);
		}
		else {
			//^3A15:0CE9
			*bp04 &= 0xfb;
		}
	}
	else {
		//^3A15:0CF6
		*bp04 |= 4;
	}
	//^3A15:0CFD
	if (glbCurrentMapIndex == glbPlayerMap)
		glbDoLightCheck = 1;
	//^3A15:0D0C
	return;
}
//^3A15:2FEB
// SPX: _3a15_2feb renamed PROCESS_TIMER_3D
void SkWinCore::PROCESS_TIMER_3D(Timer *ref)
{
	//^3A15:2FEB
	ENTER(0);
	//^3A15:2FF0
	U16 x = ref->XcoordB();
	U16 y = ref->YcoordB();
	//^3A15:3003
	if (MOVE_RECORD_TO(ref->id8(), -3, 0, x, y) == 0) {
		//^3A15:3019
		if (ref->TimerType() == tty3D) {
			//^3A15:3023
			QUEUE_NOISE_GEN1(GDAT_CATEGORY_MESSAGES, 0, SOUND_STD_TELEPORT_MESSAGE, 0x61, 0x80, x, y, 1); // teleport noise
		}
	}
	//^3A15:303B
	return;
}


//^482B:04E9
// SPX: _482b_04e9 renamed PROCESS_SOUND
void SkWinCore::PROCESS_SOUND(U8 xx)
{
	//^482B:04E9
	ENTER(0);
	//^482B:04ED
	X16 si = xx;
	if (glbSoundList[si].b5 == glbMap_4c28 || glbSoundList[si].b5 == _4976_4c12) {
		//^482B:0523
		QUEUE_NOISE_GEN1(
			glbSoundList[si].category,
			glbSoundList[si].index,
			glbSoundList[si].entry,
			U8(glbSoundList[si].w8),
			U8(glbSoundList[si].w10),
			glbSoundList[si].b6,
			glbSoundList[si].b7,
			1
			);
	}
	//^482B:05AA
	glbSoundList[si].w0 = 0;
	//^482B:05BC
	return;
}
//^3A15:303F
// SPX: _3a15_303f renamed PROCESS_TIMER_0C
void SkWinCore::PROCESS_TIMER_0C(U8 player)
{
	//^3A15:303F
	ENTER(4);
	//^3A15:3043
	Champion *champion = &glbChampionSquad[player];
	champion->timerIndex = TIMER_NONE;
	if (champion->curHP() != 0)
		champion->heroFlag |= CHAMPION_FLAG_0800;	// 0x800
	//^3A15:306A
	return;
}
//^3A15:316C
// SPX: _3a15_316c renamed PROCESS_TIMER_LIGHT
// CSBWin: Timer.cpp TAG0114fa ProcessLightLevelTimer
void SkWinCore::PROCESS_TIMER_LIGHT(Timer *ref)
{
	//^3A15:316C
	ENTER(12);
	//^3A15:3172
	i16 si = ref->value;	// si
	if (si == 0)
		return;
	//^3A15:3182
	X16 isNegative = (si < 0) ? 1 : 0; // bp02
	if (isNegative) //if (bp02 != 0)
		si = -si;
	//^3A15:319A
	i16 di;	// di
	
	si = tLightLevelItem[RCJ(16,si)] - tLightLevelItem[RCJ(16,di = si -1)];
	//di = si - 1;
	//si = tLightLevelItem[RCJ(16,si)] - tLightLevelItem[RCJ(16,di)];
	

	if (isNegative) // if (bp02 != 0) 
	{
		//^3A15:31B7
		si = -si;
		di = -di;
	}
	else {
		//^3A15:31C5
		si <<= 1;
	}
	//^3A15:31C7
	glbGlobalSpellEffects.Light += si;
	if (di == 0)
		return;
	//^3A15:31CF
	Timer newTimer; // bp0c
	newTimer.TimerType(ttyLight);
	newTimer.value = di;
	newTimer.SetMap(glbPlayerMap);
	newTimer.SetTick(glbGameTick +8);
	newTimer.actor = TIMER_ACTOR__00;
	QUEUE_TIMER(&newTimer);
	//^3A15:3208
	return;
}
//^3A15:306C
// SPX: _3a15_306c renamed PROCESS_TIMER_RESURRECTION
// Check CSBWin::ProcessTT_ViAltar
void SkWinCore::PROCESS_TIMER_RESURRECTION(Timer *ref)
{
	//^3A15:306C
	ENTER(8);
	//^3A15:3072
	X16 bonesX = ref->XcoordB();	// bp02
	X16 bonesY = ref->YcoordB();	// bp04
	X16 bonesPos = ref->Value2();	// bp06
	X16 championNo = ref->actor;		// bp08
	//X16 di = ref->b9_0_0();
	X16 VIState = ref->ActionType(); // di / SPX: I think this is action rather than b9_0_0 which is only 1 bit, and Action is set when the timer is created ...
	ObjectID si;
	switch (VIState) {
		case 2:
			//^3A15:30B5
			CREATE_CLOUD(OBJECT_EFFECT_CLOUD_REBIRTH, 0, bonesX, bonesY, bonesPos);	// oFFE4
			ref->SetTick(ref->GetTick() +5);
			//^3A15:30D6
			VIState--;
			//ref->b9_0_0(U8(di));
			ref->ActionType(U8(VIState)); // SPX: fix
			QUEUE_TIMER(ref);
			break;
		case 1:
			//^3A15:30EE
			for (si = GET_TILE_RECORD_LINK(bonesX, bonesY); si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
				//^3A15:30FD
				if (true
					&& si.Dir() == bonesPos 
					&& QUERY_CLS1_FROM_RECORD(si) == GDAT_CATEGORY_MISCELLANEOUS
					&& QUERY_CLS2_FROM_RECORD(si) == GET_CHAMPION_BONES_ITEM_ID()	// bones item (SPX: was hard coded 0, for DM1 compatibility, I changed using a function to get the correct item ID)
					&& ADD_ITEM_CHARGE(si, 0) == championNo
				) {
					//^3A15:312C
					CUT_RECORD_FROM(si, NULL, bonesX, bonesY);
					DEALLOC_RECORD(si);
					ref->SetTick(ref->GetTick() +1);
					//^3A15:314D
					//^3A15:30D6
					VIState--;
//						ref->b9_0_0(U8(di));
					ref->ActionType(U8(VIState)); // SPX: fix
					QUEUE_TIMER(ref);
					break;
				}
				//^3A15:314F
			}
			//^3A15:315D
			break;
		case 0:
			//^3A15:315F
			BRING_CHAMPION_TO_LIFE(championNo);
			break;
		default:
			//^3A15:30B2
			break;
	}
	//^3A15:3168
	return;
}
//^3A15:32BC
void SkWinCore::CONTINUE_ORNATE_ANIMATOR(Timer *ref)
{
	//^3A15:32BC
	ENTER(4);
	//^3A15:32C1
	ref->SetTick(ref->GetTick() +1);
	Actuator *bp04 = GET_ADDRESS_OF_ACTU(ref->id6());
	X16 si = GET_ORNATE_ANIM_LEN(bp04, ref->w8);
	bp04->ActuatorData(bp04->ActuatorData() +1);
	if ((bp04->ActuatorData() % si) == 0) {
		bp04->ActiveStatus(0);
	}
	else {
		//^3A15:3322
		QUEUE_TIMER(ref);
	}
	//^3A15:332E
	return;
}
//^3A15:341C
void SkWinCore::CONTINUE_TICK_GENERATOR(Timer *ref)
{
	//^3A15:341C
	ENTER(4);
	//^3A15:3422
	Actuator *bp04 = GET_ADDRESS_OF_ACTU(ref->id6());
	X16 si;
	if (bp04->ActionType() == 3) {
		//^3A15:3447
		ref->b9_0_0(ref->b9_0_0() ^ 1);
		si = ref->b9_0_0() | bp04->OnceOnlyActuator();
		INVOKE_ACTUATOR(bp04, (ref->b9_0_0() != 0) ? 0 : 1, 0);
	}
	//^3A15:348C
	else if ((si = bp04->OnceOnlyActuator()) != 0) {
		//^3A15:349F
		INVOKE_ACTUATOR(bp04, bp04->ActionType(), 0);
	}
	//^3A15:34BA
	if (si != 0) {
		//^3A15:34BE
		ref->SetTick(ref->GetTick() + ref->Value2() * bp04->ActuatorData());
		QUEUE_TIMER(ref);
	}
	else {
		//^3A15:3505
		bp04->b4_0_0(0);
	}
	//^3A15:350D
	return;
}
//^3A15:374B
// SPX: _3a15_374b renamed PROCESS_TIMER_RELEASE_DOOR_BUTTON
void SkWinCore::PROCESS_TIMER_RELEASE_DOOR_BUTTON(Timer *ref) 
{
	//^3A15:374B
	ENTER(0);
	//^3A15:374E
	GET_ADDRESS_OF_RECORD0(ref->id6())->SetButtonState(0);
	//^3A15:3764
	return;
}
//^3A15:3766
// SPX: _3a15_3766 renamed PROCESS_TIMER_59 
// I hit this one in starting cave, after reviving some champions
void SkWinCore::PROCESS_TIMER_59(Timer *ref) 
{
	//^3A15:3766
	ENTER(4);
	//^3A15:376A
	Actuator *bp04 = GET_ADDRESS_OF_ACTU(ref->id8());
	if (bp04->OnceOnlyActuator() == 0) {
		//^3A15:378E
		if (glbCurrentMapIndex == glbPlayerMap) {
			//^3A15:3797
			glbDoLightCheck = 1;
		}
		//^3A15:379D
		bp04->ActiveStatus(0);
	}
	//^3A15:37A5
	return;
}


//^3A15:37A7
void SkWinCore::CONTINUE_ORNATE_NOISE(Timer *ref) 
{
	//^3A15:37A7
	ENTER(6);
	//^3A15:37AC
	Actuator *pActuator = GET_ADDRESS_OF_ACTU(ref->id8());	// bp04
	if (pActuator->ActiveStatus() != 0 && ref->GetMap() == glbPlayerMap) {
		//^3A15:37EA
		X16 si = ((GET_TILE_VALUE(ref->XcoordB(), ref->YcoordB()) >> 5) == ttWall) ? 1 : 0;
		U8 iCategory = 0;	// bp05
		U8 iItemNumber = 0;	// bp06
		if (si != 0) {
			//^3A15:3818
			iCategory = GDAT_CATEGORY_WALL_GFX;	// 09
			iItemNumber = GET_WALL_DECORATION_OF_ACTUATOR(pActuator);
		}
		else {
			//^3A15:3829
			iCategory = GDAT_CATEGORY_FLOOR_GFX;	// 10
			iItemNumber = GET_FLOOR_DECORATION_OF_ACTUATOR(pActuator);
		}
		//^3A15:383D
		ref->SetTick(ref->GetTick() + GET_ORNATE_ANIM_LEN(pActuator, si));
		QUEUE_TIMER(ref);
		// SPX: Wall or floor activation sound
		QUEUE_NOISE_GEN2(iCategory, iItemNumber, SOUND_STD_ACTIVATION, 0xfe, ref->XcoordB(), ref->YcoordB(), 1, 0x8c, 0x80);
		
		
		// SPX special block to try amplifier/fireball activation over lava crater 
		if (SkCodeParam::bUseFixedMode)
		{
			// Is the ornate a lava crater ?
			if (iCategory == GDAT_CATEGORY_FLOOR_GFX &&
				iItemNumber == 0x08) // lava crater
			{
				X16 iSpellPower = 128;
				X16 iPosX = ref->Xcoord();
				X16 iPosY = ref->Ycoord();
				// Get the creature info on top of the crater if any
				ObjectID xObject = GET_CREATURE_AT(iPosX, iPosY);
				Creature *xCreature = GET_ADDRESS_OF_RECORD4(xObject);
				if (xObject != OBJECT_NULL)
				{
					// Note: PREPARE/UNPREPARE can't be used for static variables
					//U8 *bp04 = PREPARE_LOCAL_CREATURE_VAR(xCreature,iPosX,iPosY,0x22);
					//UNPREPARE_LOCAL_CREATURE_VAR(bp04);
					X16 iMissileSpell = i16(OBJECT_EFFECT_FIREBALL);	// May take this effect from AI spell flags
					X16 iSpellPower = 128;
					U16 iDirection = xCreature->b15_0_1();
					iPosX += glbXAxisDelta[iDirection];
					iPosY += glbYAxisDelta[iDirection];
					SHOOT_ITEM(iMissileSpell, 
						iPosX, iPosY, 
						0, iDirection, iSpellPower, iSpellPower, 0);
				}
			} // End block / Amplifier
			if (iCategory == GDAT_CATEGORY_FLOOR_GFX &&
				iItemNumber == 0x0D) // poison rift
			{
				X16 iSpellPower = 128;
				X16 iPosX = ref->Xcoord();
				X16 iPosY = ref->Ycoord();

				CREATE_CLOUD(OBJECT_EFFECT_POISON_CLOUD, iSpellPower, iPosX, iPosY, 255);
			} // end poison rift

		}
		// SPX End block / Amplifier / Poison Rift
	}
	else {
		//^3A15:3893
		pActuator->ActuatorData(pActuator->ActuatorData() & 255);
	}
	//^3A15:38B3
	return;
}

#if DM2_EXTENDED_MODE == 1
void SkWinCore::PROCESS_TIMER_AMBIENT_SOUND(Timer *ref)
{
	ENTER(0);
	// Play sound for rain ! 
	SkD((1, "Process Timer Ambient Sound >> \n"));
	

	U16 rainLevel = 0;
	U16 iSoundID = 0x80;
	if (glbRainStrength >= RAIN_THRESHOLD_LEVEL_3) {
		rainLevel = 4;
	}
	else if (glbRainStrength >= RAIN_THRESHOLD_LEVEL_2) {
		rainLevel = 3;
	}
	else if (glbRainStrength >= RAIN_THRESHOLD_LEVEL_1) {
		rainLevel = 2;
	}
	else if (glbRainStrength > RAIN_THRESHOLD_LEVEL_0 && glbRainStrength < RAIN_THRESHOLD_LEVEL_1) {
		rainLevel = 1;
	}
	iSoundID = 0x70 + rainLevel;

	
	/*if (rainLevel == 0)	// If there is no rain, we get another ambient sound, like random wind
	{
		int iRandomValue = rand()%5;
		iSoundID = 0x80 + iRandomValue;
	}*/

	//static int iRainSoundCount = 0;
	SkD((1, "Tick %d / Playing Rain sound %02x ! (rain level = %d, rain strength = %d) | Map: %d / GfxSet: %d\n",
		glbGameTick, iSoundID, rainLevel, glbRainStrength, glbPlayerMap, glbMapGraphicsSet));
	
	// Generate sound
	if (SkCodeParam::bUseExtendedSound)
		QUEUE_NOISE_GEN1(GDAT_CATEGORY_ENVIRONMENT, glbMapGraphicsSet, iSoundID, 0x96, 0x80, glbPlayerPosX, glbPlayerPosY, 0);
	//iRainSoundCount = (iRainSoundCount+1)%20;
	
	// Issue a new timer
	U8 iDeltaTick = 32;
// SPX: TODO -> see how to handle not directly from "skwin"
#if defined(_USE_MFC80) || defined(_USE_MFC60)
	switch (skwin.spfact)
	{
		case 0: iDeltaTick = 32; break;
		case 1: iDeltaTick = 16; break;
		case 2: iDeltaTick = 8; break;
		case 3: iDeltaTick = 6; break;
		case 4: iDeltaTick = 4; break;
		case 5: iDeltaTick = 3; break;
	}
#endif
	Timer xTimerRef;
	xTimerRef.SetMap(glbPlayerMap);
	xTimerRef.SetTick(glbGameTick + iDeltaTick);
	xTimerRef.TimerType(ttyAmbientSound);
	xTimerRef.actor = TIMER_ACTOR__00;
	xTimerRef.value = 0;
	QUEUE_TIMER(&xTimerRef);

	return;
}
#endif

//^3A15:3A9E
void SkWinCore::PROCEED_TIMERS()
{
#if DLV_DBC
	{
		for (Bitu x=0; x<glbCreaturesCount; x++) {
			sk4ebe &a = glbTabCreaturesInfo[x];
			if (a.w0 != -1) {
				SkD((DLV_DBC, 
					"#%03u. " "%04X,"   "%04X,"   "__%02X," "%02X," "%02X," "__%02X," "%02X,%02X,%02X,%02X," "%04X,"   "%04X,"   "%04X,"   "\n"
					"--    "  "__%02X," "__%02X," "%04X,"   "%02X," "%02X," "%04X,"   "%02X,%02X,%02X,%02X," "__%02X," "__%02X," "__%02X," "%02X" "\n"
					, (Bitu)x
					, (Bitu)a.w0, (Bitu)a.w2, (Bitu)a.b4, (Bitu)a.b5_, (Bitu)a.b6_, (Bitu)a.b7, (Bitu)a.b8_, (Bitu)a.b9_, (Bitu)a.b10_, (Bitu)a.b11_
					, (Bitu)a.w12.w0, (Bitu)a.w14, (Bitu)a.w16

					, (Bitu)(Bit8u)a.b18, (Bitu)(Bit8u)a.b19, (Bitu)a.w20, (Bitu)a.b22, (Bitu)a.b23, (Bitu)a.w24.w0, (Bitu)a.b26, (Bitu)a.b27, (Bitu)a.b28, (Bitu)a.b29
					, (Bitu)a.b30, (Bitu)a.b31, (Bitu)a.b32, (Bitu)a.b33
					));
				Creature &db = reinterpret_cast<Creature *>(glbDBObjectData[dbCreature])[+a.w0];
				SkD((DLV_DBC,
					"-- db " "%04X,%04X,%02X,%02X,{%04X,%04X,%04X,%04X},%02X,%02X \n"
					"\n"
					, (Bitu)db.w0, (Bitu)db.w2, (Bitu)db.b4, (Bitu)db.b5, (Bitu)db.w6, (Bitu)db.w8, (Bitu)db.w10, (Bitu)db.w12, (Bitu)db.b14, (Bitu)db.b15
					));
			}
		}
	}
#endif

#if 0
	{
		printf("-- %6u \n", glbGameTick);
		for (int x = 0; x < glbTimersCount; x++) {
			Timer &r = glbTimersTable[glbTimerNextEntries[x]];
			printf("%2d %6u %2u %02X %02X %04X %04X \n", x, r.GetTick(), r.GetMap(), r.TimerType(), r.b5, r.w6, r.w8);
		}
	}
#endif

//printf("PROCEED_TIMERS\n"); getch();
	//^3A15:3A9E
	ENTER(16);
	//^3A15:3AA4
	Timer timer;	//bp10
	Timer *bp04 = &timer;
	U16 bp06;
	for (; IS_TIMER_TO_PROCEED() != 0; ) {
		//printf("PROCEED_TIMERS: Timer to proceed %08X!\n", bp04); getch();
		//^3A15:3AB0
		//LOGX(("3A15:3AB0 %u %u %u %d", (U32)_4976_4e46, (U32)glbAbsoluteTickCounter, (U32)_4976_19a9, (i16)_4976_4e62, (U16)_4976_4e48, (U16)_4976_4e64));
		IBMIO_USER_INPUT_CHECK();
		GET_AND_DELETE_NEXT_TIMER(bp04);
		CHANGE_CURRENT_MAP_TO(timer.GetMap());
		X16 si = timer.TimerType();
		X16 di = si;
#if UseAltic
		SkD((DLV_DBG_TIMER, "DBG: Timer#xx (%6u,%2u,%02X,%02X,%04X,%04X)\n"
			, (Bitu)bp04->GetTick(), (Bitu)bp04->GetMap(), (Bitu)bp04->TimerType()
			, (Bitu)bp04->actor, (Bitu)bp04->value, (Bitu)bp04->w8
			));
#endif
		//^3A15:3ADE
		//printf("PROCEED_TIMERS: Timer type = %d\n", di);// getch();
		switch (di) {
			case tty21://^3AF4
			case tty22://^3AF4 // TiTy22: 'think creature'
				//^3A15:3AF4
				THINK_CREATURE(timer.XcoordB(), timer.YcoordB(), si);
				break;
			case tty1D://^3B09
			case tty1E://^3B09
				STEP_MISSILE(bp04);
				break;
			case ttyDoorStep://^3B17
				//^3A15:3B17
				STEP_DOOR(bp04);
				break;

			case ttyCloud://^3B24
				//^3A15:3B24
				PROCESS_CLOUD(bp04);
				break;

			case ttyDoorDestroy://^3B32
				//^3A15:3B32
				PROCESS_TIMER_DESTROY_DOOR(bp04);
				break;

			case tty04://^3B3F // TiTy04: 'launch actuator'
				//^3A15:3B3F
				switch (glbCurrentTileMap[timer.XcoordB()][timer.YcoordB()] >> 5) {
					case ttWall://^3B72
						//^3A15:3B72
						ACTUATE_WALL_MECHA(bp04);
						break;
					case ttFloor://^3B7F
						//^3A15:3B7F
						ACTUATE_FLOOR_MECHA(bp04);
						break;
					case ttPit://^3B8C
						//^3A15:3B8C
						ACTUATE_PITFALL(bp04);
						break;
					case ttDoor://^3B99
						//^3A15:3B99
						ACTUATE_DOOR(bp04);
						break;
					case ttTeleporter://^3BA6
						//^3A15:3BA6
						ACTUATE_TELEPORTER(bp04);
						break;
					case ttTrickWall://^3BB3
						//^3A15:3BB3
						ACTUATE_TRICKWALL(bp04);
						break;
					case ttStairs://^3D93
						goto _3d93;
				}
				break;
			case tty3C://^3BC0
			case tty3D://^3BC0
				//^3A15:3BC0
				PROCESS_TIMER_3D(bp04);
				break;
			case ttySound://^3BCD
				//^3A15:3BCD
				PROCESS_SOUND(timer.XcoordB());
				break;
			case tty0C://^3BD8
				//^3A15:3BD8
				PROCESS_TIMER_0C(timer.actor);
				break;
			case ttyLight://^3BE5
				//^3A15:3BE5
				CHANGE_CURRENT_MAP_TO(glbPlayerMap);
				PROCESS_TIMER_LIGHT(bp04);
				RECALC_LIGHT_LEVEL();
				break;
			case ttyInvisibility: //^3C03	(0x47 / 71)		// SPX: decrease invisibility value
				//^3A15:3C03
				glbGlobalSpellEffects.Invisibility = glbGlobalSpellEffects.Invisibility -1;
				if (glbGlobalSpellEffects.Invisibility != 0 || glbChampionInventory == 0)
					break;
				//^3A15:3C1E
				glbChampionTable[glbChampionInventory].heroFlag |= CHAMPION_FLAG_4000;	// 0x4000
				break;
			case ttyEnchantment://^3C31		(0x48 / 72)
				//^3A15:3C31
				for (bp06 = 0; bp06 < glbChampionsCount; bp06++) {
					//^3A15:3C38
					if (((1 << bp06) & timer.actor) != 0 && glbChampionSquad[bp06].curHP() != 0) {
						//^3A15:3C5A
						glbChampionSquad[bp06].enchantmentPower -= bp04->value; 
					}
					//^3A15:3C71
					if (glbChampionSquad[bp06].enchantmentPower < 0) {
						//^3A15:3C82
						glbChampionSquad[bp06].enchantmentPower = 0;
					}
					//^3A15:3C92
				}
				//^3A15:3C9E
				break;
#if (XDM1_EXTENDED_SEETHRUWALLS == 1)
			case ttySeeThruWalls:	// (0x49 / 73)		// SPX: decrease see_thru_walls value
				glbGlobalSpellEffects.SeeThruWalls = glbGlobalSpellEffects.SeeThruWalls -1;
				break;
#endif
			case ttyPoison://^3CA1	(0x4B / 75)
				//^3A15:3CA1
				glbChampionSquad[si = timer.actor].PoisonValue--;
				PROCESS_POISON(si, timer.value);
				break;
#if __SK_EXTENDED_SKWIN_V6__
			case ttyPlague:
				glbChampionSquad[si = timer.actor].PlagueValue--;
				PROCESS_PLAGUE(si, timer.value);
				break;
#endif
			case ttyResurrect://^3CBE
				//^3A15:3CBE
				PROCESS_TIMER_RESURRECTION(bp04);
			case ttyItemBonus://^3CCA
				//^3A15:3CCA
				PROCESS_TIMER_0E(bp04, 0xfffe);
				break;
			case ttyWeather://^3CDC
				//^3A15:3CDC
				UPDATE_WEATHER(1);
				break;
			case ttyOrnateAnimator://^3CE7
				//^3A15:3CE7
				CONTINUE_ORNATE_ANIMATOR(bp04);
				break;
			case ttyTickGenerator://^3CF3
				//^3A15:3CF3
				CONTINUE_TICK_GENERATOR(bp04);
				break;
			case ttyDoorButton://^3CFF
				//^3A15:3CFF
				PROCESS_TIMER_RELEASE_DOOR_BUTTON(bp04);
				break;
			case tty59://^3D0B
				//^3A15:3D0B
				PROCESS_TIMER_59(bp04);
				break;
			case tty5A://^3D17
				//^3A15:3D17
				CONTINUE_ORNATE_NOISE(bp04);
				break;
			case ttyWallButton://^3D25
			case tty5B://^3D25
				//^3A15:3D25
				// when door opens/closes by actuator?
				GET_ADDRESS_OF_ACTU(bp04->id6())->ActiveStatus(0);
				break;
			case ttySimpleActuTurnOn://^3D3D
				//^3A15:3D3D
				// when acquire mineral?
				GET_ADDRESS_OF_RECORD2(bp04->id6())->TextVisibility(1);
				break;
			case tty5D://^3D55
				//^3A15:3D55
				if (timer.Value2() == glbPlayerMap) {
					//^3A15:3D60
					MOVE_RECORD_TO(OBJECT_NULL, glbPlayerPosX, glbPlayerPosY, timer.b6_0_4(), timer.w6_5_9());
					ROTATE_SQUAD(timer.w6_a_b());
				}
				break;
#if (DM2_EXTENDED_MODE == 1)

			case ttyAmbientSound:
				{
					PROCESS_TIMER_AMBIENT_SOUND(bp04);
				}
				break;
#endif
		}
		//^3A15:3D93
_3d93:
		;
	}
	//printf("PROCEED_TIMERS: Change map then returns.\n"); getch();
	//^3A15:3D9E
	CHANGE_CURRENT_MAP_TO(glbPlayerMap);
	//^3A15:3DA8
	return;
}


