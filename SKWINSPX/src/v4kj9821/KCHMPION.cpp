#include <StdAfx.h>	// Required for MVC6 compilation

//#include <conio.h> // getch

#include <KCORE.H>
#include <SkDebug.h>

#if defined(_USE_MFC80) || defined(_USE_MFC60)
//#include "SkWinMFC.h"
//#include "resource.h"
#endif // defined(_USE_MFC80) || defined(_USE_MFC60)


//^2C1D:2076
void SkWinCore::CALC_PLAYER_WEIGHT(Bit16u player)
{
	// updates player's weight. w253 doesn't contain weight of leader's hand possession.

	//^2C1D:2076
	Bit16u di=0;
	i16 si=0;
	//^2C1D:207F
	//for (; si < 30; si++) {
	for (si = 0; si < INVENTORY_MAX_SLOT; si++) {
		//^2C1D:2081
		di += QUERY_ITEM_WEIGHT(glbChampionSquad[player].inventory[si]);
		//^2C1D:209D
	}
	//^2C1D:20A3
	if (_4976_5336 -1 == player && glbSelectedHandAction < 2) {
		//^2C1D:20B3
		if (IS_CONTAINER_CHEST(_4976_3de6[RCJ(4,player)][RCJ(2,glbSelectedHandAction)]) != 0) {
			//^2C1D:20E3
			for (Bit16u si = 0; si < 8; si++) {
				//^2C1D:20D2
				di += QUERY_ITEM_WEIGHT(glbCurrentContainerItems[si]);
				//^2C1D:20E2
			}
		}
	}
	//^2C1D:20E8
	glbChampionSquad[player].curWeight(di);
	//^2C1D:20F6
	glbChampionSquad[player].heroFlag |= CHAMPION_FLAG_1000;	// 0x1000
}


//^2C1D:0300
// SPX: _2c1d_0300 replaced by BOOST_ATTRIBUTE, called when consuming potion or special item
void SkWinCore::BOOST_ATTRIBUTE(Champion *ref, Bit16u xx, Bit16u yy)
{
	//^2C1D:0300
	ENTER(4);
	//^2C1D:0306
	i16 di = yy;
	U8 *bp04 = ref->attributes[xx];
	i16 si = bp04[0] +di -bp04[1];
	//^2C1D:0333
	if (((si < 0) ? 1 : 0) == (di < 0) ? 1 : 0) {
		//^2C1D:034F
		si = ABS16(si);
		//^2C1D:0364
		while (si > 20) {
			//^2C1D:035A
			di -= di / 4;
			si -= 20;
		}
	}
	//^2C1D:0369
	bp04[0] = BETWEEN_VALUE(10, bp04[0] +di, 220);
	//^2C1D:0387
	return;
}

//^2C1D:03E7
// SPX: _2c1d_03e7 renamed PROCESS_ITEM_BONUS / Function called when engine looks through possessed items to give bonus
// SPX: Might check CSBWin AdjustStatsForItemCarried to handle CURSED state
void SkWinCore::PROCESS_ITEM_BONUS(i16 player, ObjectID recordLink, i16 inventorySlot, i16 x4)
{
	// SPX x4 = -1 when removed, 1 when added, 2 when consumed, 0 when done from SKRead
	//^2C1D:03E7
	ObjectID di = recordLink;
	i16 si = x4;
	i16 iBonusModifier = 0; // SPX: Added as a fix instead of using unsigned bp06
	//^2C1D:03F3
	if (player < 0)
		return;
	//^2C1D:03FC
	if (di == OBJECT_NULL)
		return;
	//^2C1D:0404
	if (inventorySlot < INVENTORY_MAX_SLOT) {	// (possess < 30)
		//^2C1D:040D
		Bit16u bp06 = QUERY_GDAT_DBSPEC_WORD_VALUE(di, GDAT_ITEM_STATS_GEN_FLAGS);
		//^2C1D:041A
		// SPX: 00/00/00 => flags, 0x0010 = item providing light, so must recompute light
		if (si != 0 && (bp06 & ITEM_FLAG_PRODUCE_LIGHT) != 0) {
			//^2C1D:0425
			RECALC_LIGHT_LEVEL();
		}
		//^2C1D:042A
		if ((bp06 & ITEM_FLAG_2000) == 0) {
			//^2C1D:0434
			Champion *champion = &glbChampionSquad[player]; //*bp04 
			//^2C1D:0445
			Bit16u bp0a = IS_ITEM_FIT_FOR_EQUIP(di, inventorySlot, 1);
			Bit16u bp0c = 0;
			//^2C1D:045B
			if (si != 0 && si != 3 && si != -2) {
				//^2C1D:0472
				// SPX: 0x14 MANA BONUS
				iBonusModifier = RETRIEVE_ITEM_BONUS(di, GDAT_ITEM_BONUS_MANA, bp0a, si);
				//^2C1D:0483
				if (iBonusModifier != 0) {
					//^2C1D:0487
					if (si == 1 || si == -1) {
						if (SkCodeParam::bUseFixedMode) // SPX: done to prevent weird cases of bonuses not taken into account correctly.
						{
							i16 iChampionManaMax = (i16) champion->manaMax;
							iChampionManaMax += iBonusModifier;
							if (iChampionManaMax < 0)
							{
								iChampionManaMax = 0;
								iBonusModifier = 0; // prevent bad stuff for curMP;
							}
							champion->manaMax = (Bit16u) iChampionManaMax;
						}
						else
							//^2C1D:0491
							champion->manaMax += iBonusModifier;
					}
					else {
						//^2C1D:049D
						if (si == 2) {
							//^2C1D:04A2
							champion->curMP(BETWEEN_VALUE(0, champion->curMP() + iBonusModifier, MAXMP_BONUS));
						}
					}
					//^2C1D:04C1
					champion->heroFlag |= CHAMPION_FLAG_0800;	// 0x0800
				}
				//^2C1D:04CA
				// SPX: Bonus 0x15 => 0x1B check for boosting attributes (x15 luck to x1B antifire)
				for (Bit16u attributeIndex = 0; attributeIndex < ATTRIBUTE_COUNT; attributeIndex++) {
					//^2C1D:04D1
					iBonusModifier = RETRIEVE_ITEM_BONUS(di, attributeIndex + GDAT_ITEM_BONUS_ATTRIBUTE_FIRST, bp0a, si);
					//^2C1D:04E6
					if (iBonusModifier != 0) {
						//^2C1D:04EA
						if (si == 1 || si == -1) {
							//^2C1D:04F4
							champion->attributesEnhanced[attributeIndex] = champion->attributesEnhanced[attributeIndex] + iBonusModifier;
						}
						else {
							//^2C1D:050D
							BOOST_ATTRIBUTE(champion, attributeIndex, iBonusModifier);
						}
						//^2C1D:0520
						champion->heroFlag |= CHAMPION_FLAG_3000;	// 0x3000
					}
					//^2C1D:0529
				}
			}
			//^2C1D:0532
			// SPX: Bonus 0x1E => 0x31 , 20 values = 4*5 skills
			for (Bit16u skillIndex = 0; skillIndex < SKILL_COUNT; skillIndex++) {
				//^2C1D:0539
				iBonusModifier = RETRIEVE_ITEM_BONUS(di, skillIndex + GDAT_ITEM_BONUS_SKILL_FIRST, bp0a, si);
				//^2C1D:054E
				if (bp06 != 0) {
					//^2C1D:0552
					champion->skillBonus[skillIndex] = champion->skillBonus[skillIndex] + iBonusModifier;
					//^2C1D:056B
					champion->heroFlag |= CHAMPION_FLAG_2000;	// 0x2000
					bp0c = 1;
				}
				//^2C1D:0579
			}
			//^2C1D:0582
			// SPX: Bonus 0x33 : SPEED ?
			iBonusModifier = RETRIEVE_ITEM_BONUS(di, GDAT_ITEM_BONUS_WALK_SPEED, bp0a, si);
			//^2C1D:0593
			if (iBonusModifier != 0) {
				//^2C1D:0597
				champion->walkSpeed = champion->walkSpeed + iBonusModifier;
				bp0c = 1;
			}
			//^2C1D:05AC
			// SPX: Bonus 0x32 : LIGHT
			if (si != 2 && si != -1 && si != 3) {
				//^2C1D:05BB
				iBonusModifier = RETRIEVE_ITEM_BONUS(di, GDAT_ITEM_BONUS_LIGHT, bp0a, si);
				//^2C1D:05CC
				if (bp06 != 0) {
					//^2C1D:05D0
					glbGlobalSpellEffects.Light += iBonusModifier;
					if (si != 0) {
						//^2C1D:05D8
						RECALC_LIGHT_LEVEL();
					}
				}
			}
			//^2C1D:05DD
			if (bp0c != 0 && si == 2) {
				//^2C1D:05E8
				Timer bp16;
				bp16.TimerType(ttyItemBonus);
				//^2C1D:05EC
				bp16.SetMap(glbPlayerMap);
				bp16.SetTick(QUERY_GDAT_DBSPEC_WORD_VALUE(di, 0x0013) + glbGameTick);
				//^2C1D:061C
				bp16.actor = (Bit8u)player;
				//^2C1D:0622
				bp16.value = di.DBType();
				//^2C1D:062D
				bp16.w8 = QUERY_CLS2_FROM_RECORD(di);
				//^2C1D:0637
				QUEUE_TIMER(&bp16);
			}
		}
	}
	//^2C1D:0643
	if (si != 0) {
		//^2C1D:0647
		CALC_PLAYER_WEIGHT(player);
	}
	//^2C1D:0650
	return;
}

//^2C1D:07A3
ObjectID SkWinCore::REMOVE_POSSESSION(Bit16u player, Bit16u possess)
{
	// CSBwinSimilarity: TAG015a66,RemovePossession

	//^2C1D:07A3
	ENTER(4);
	//^2C1D:07A9
	Bit16u si = possess;
	//^2C1D:07AC
	Champion *bp04 = &glbChampionSquad[player];
	//^2C1D:07BD
	ObjectID di;
	if (si >= INVENTORY_MAX_SLOT) {	// (si >= 30)
		//^2C1D:07C2
		di = glbCurrentContainerItems[si -INVENTORY_MAX_SLOT];
		//^2C1D:07CD
		glbCurrentContainerItems[si -INVENTORY_MAX_SLOT] = OBJECT_NULL;
	}
	else {
		//^2C1D:07DC
		di = bp04->Possess(si);
		//^2C1D:07EA
		bp04->Possess(si, OBJECT_NULL);
	}
	//^2C1D:07FA
	if (di == OBJECT_NULL) {
		//^2C1D:07FF
		return OBJECT_NULL;
	}
	//^2C1D:0804
	if (true
		&& glbChampionIndex -1 == player
		&& si <= 1
		&& si == glbSelectedHandAction
	) {
		//^2C1D:0818
		DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
	}
	//^2C1D:081D
	PROCESS_ITEM_BONUS(player, di, si, -1);
	//^2C1D:082B
	return di;
}

//^2C1D:0831
void SkWinCore::EQUIP_ITEM_TO_INVENTORY(Bit16u player, ObjectID rl, Bit16u inventorySlot)
{
	//^2C1D:0831
	ENTER(4);
	//^2C1D:0837
	ObjectID si = rl;
	Bit16u di = inventorySlot;
	//^2C1D:083D
	if (si != OBJECT_NULL) {
		//^2C1D:0842
		Champion *champion = &glbChampionSquad[player];
		//^2C1D:0853
		si.ClearDir();
		//^2C1D:0857
		if (di >= INVENTORY_MAX_SLOT) {	// (di >= 30)
			//^2C1D:085C
			glbCurrentContainerItems[di - INVENTORY_MAX_SLOT] = si;
		}
		else {
			//^2C1D:0869
			champion->Possess(di, si);
		}
		//^2C1D:0877
		PROCESS_ITEM_BONUS(player, si, di, 1);
	}
	//^2C1D:0885
	return;
}

//^2759:0E93
// TODO: related hand activation ?
Bit16u SkWinCore::_2759_0e93(Bit16u player, ObjectID rl, i16 hand)
{
	//^2759:0E93
	ENTER(2);
	//^2759:0E99
	X16 di = player;
	X16 bp02 = 0;
	if (rl != OBJECT_NULL) {
		//^2759:0EAA
		if (IS_ITEM_HAND_ACTIVABLE(di, rl, -1) != 0) {
			//^2759:0EBB
			U16 si;
			for (si = 0; si < _4976_53a4; si++) {
				//^2759:0EBF
				if (QUERY_CMDSTR_ENTRY(glbItemSelected[si].category, glbItemSelected[si].index, glbItemSelected[si].entry, 2) == hand) {
					//^2759:0EEB
					bp02 = 1;
					break;
				}
				//^2759:0EF2
			}
		}
		//^2759:0EF9
		di = glbChampionIndex;
		if (di != 0) {
			if (glbSelectedHandAction == 0 || glbSelectedHandAction == 1) {
				//^2759:0F10
				di--;
				IS_ITEM_HAND_ACTIVABLE(di, glbChampionSquad[di].Possess(glbSelectedHandAction), glbSelectedHandAction);
			}
		}
	}
	//^2759:0F32
	return bp02;
}

//^2759:0FB0
void SkWinCore::LOAD_PROJECTILE_TO_HAND(Bit16u player, i16 hand)
{
	//^2759:0FB0
	ENTER(14);
	//^2759:0FB6
	Champion *champion = &glbChampionSquad[player];	//*bp04
	//^2759:0FC7
    champion->handCooldown[hand] = 0;
	//^2759:0FD2
	if (champion->curHP() == 0 || hand >= 2)
		//^2759:0FE5
		return;
	//^2759:0FE8
	i16 bp0e = hand ^ 1;
	//^2759:0FF1
	i16 bp0a = hand;
	//^2759:0FF7
	Bit16u bp0c = champion->handCommand[hand];
	//^2759:1003
	champion->handCommand[hand] = 0xff;
	//^2759:100E
	champion->handDefenseClass[hand] = 0;
	//^2759:1019
	//if (bp0c == 32) {
	if (bp0c == CmLaunchProjectile) {
		//^2759:1022
		if (champion->Possess(bp0e) != OBJECT_NULL)
			//^2759:1034
			return;
		//^2759:1037
		Bit16u di = INVENTORY_SCABBARD_1;	// 12
		//^2759:103C
		if (IS_MISSILE_VALID_TO_LAUNCHER(player, bp0a, champion->Possess(di)) != 0) {
			//^2759:1059
_1059:
			ObjectID si = REMOVE_POSSESSION(player, di);

			//^2759:1066
			EQUIP_ITEM_TO_INVENTORY(player, si, bp0e);
			//^2759:1075
			return;
		}
		else {
			//^2759:1078
			ObjectID si = champion->Possess(INVENTORY_SCABBARD_1);	// 12
			//^2759:1082
			if (IS_CONTAINER_CHEST(si) != 0) {
				//^2759:108D
				ObjectID *bp08 = &GET_ADDRESS_OF_RECORD9(si)->w2;
				//^2759:109C
				si = *bp08;
				//^2759:10A2
				for (; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
					//^2759:10A4
					if (IS_MISSILE_VALID_TO_LAUNCHER(player, bp0a, si) != 0) {
						//^2759:10B6
						CUT_RECORD_FROM(si, bp08, -1, 0);
						//^2759:10C9
						//^2759:1066
						EQUIP_ITEM_TO_INVENTORY(player, si, bp0e);
						//^2759:1075
						return;
					}
					//^2759:10CB
				}
			}
			//^2759:10D9
			//for (i16 di = 7; di <= 9; di++) {
			for (i16 di = INVENTORY_SCABBARD_2; di <= INVENTORY_SCABBARD_4; di++) {
				//^2759:10DE
				if (IS_MISSILE_VALID_TO_LAUNCHER(player, bp0a, champion->Possess(di)) != 0)
					//^2759:10FD
					goto _1059;
				//^2759:1100
			}
			//^2759:1106
			return;
		}
	}
	//^2759:1109
	if (bp0c != 42)
		//^2759:110F
		return;
	//^2759:1112
	if (champion->Possess(bp0a) != OBJECT_NULL)
		//^2759:1124
		return;
	//^2759:1127
	Bit16u di = INVENTORY_SCABBARD_1;
	ObjectID si;
	if (_2759_0e93(player, champion->Possess(di), bp0c) != 0) {
		//^2759:1149
_1149:
		si = REMOVE_POSSESSION(player, di);
		//^2759:1156
_1156:
		EQUIP_ITEM_TO_INVENTORY(player, si, bp0a);
		//^2759:1075
		return;
	}
	//^2759:115C
	if (IS_CONTAINER_CHEST(si = champion->Possess(INVENTORY_SCABBARD_1)) != 0) {
		//^2759:1171
		ObjectID *bp08 = &GET_ADDRESS_OF_RECORD(si)->w0;
		//^2759:1180
		si = *bp08;

		//^2759:1186
		for (; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
			//^2759:1188
			if (_2759_0e93(player, si, bp0c) != 0) {
				//^2759:119A
				CUT_RECORD_FROM(si, bp08, -1, 0);
				//^2759:11AD
				goto _1156;
			}
			//^2759:11AF
		}
	}
	//^2759:11BD
//		for (di = 7; di <= 9; di++) {
	for (di = INVENTORY_SCABBARD_2; di <= INVENTORY_SCABBARD_4; di++) {
		//^2759:11C2
		if (_2759_0e93(player, champion->Possess(di), bp0c) != 0) {
			//^2759:11E1
			goto _1149;
		}
		//^2759:11E4
	}
	//^2759:11EA
	si = REMOVE_POSSESSION(player, 12);
	//^2759:11F8
	if (si != OBJECT_NULL)
		//^2759:11FD
		goto _1156;
	//^2759:1200
	return;
}



//^2759:01FE
Bit16u SkWinCore::_2759_01fe(Bit16u player, ObjectID recordLink, Bit16u cmdNum)
{
	//^2759:01FE
    ENTER(8);
	//^2759:0204
	ObjectID di = recordLink;
	Bit16u si = cmdNum;
	//^2759:020A
	if (di == OBJECT_NULL) {
		//^2759:020F
		return 0;
	}
	//^2759:0214
	if (di.DBType() == dbContainer) {
		//^2759:0224
		Container *bp04 = GET_ADDRESS_OF_RECORD9(di);
		//^2759:0231
		if (bp04->ContainerType() == 1) {
			//^2759:0245
			Bit16u bp08 = bp04->b5_5_7();
			//^2759:0252
			if (false
				|| bp08 == 1 // 1 - SCOUT MAP?
				|| bp08 == 2 // 2 - MINION MAP?
			) {
				//^2759:025E
				if (GET_MISSILE_REF_OF_MINION(bp04->GetContainedObject(), di) != NULL) {
					//^2759:0271
					if (si == CmKillMinion)
						//^2759:0274
						//^2759:02CF
						return 1;
					//^2759:0276
					if (bp08 != 2)
						//^2759:027A
						//^2759:02CC
						//^2759:020F
						return 0;
					//^2759:027C
					Bit8u bp05 = GET_ADDRESS_OF_RECORD4(bp04->GetContainedObject())->CreatureType();
					//^2759:0294
					if (bp05 == 51) { // 51 - FETCH MINION
						//^2759:029A
						if (si != CmCallCarry)
							//^2759:029D
							//^2759:02CC
							//^2759:020F
							return 0;
						//^2759:029F
						//^2759:02CF
						return 1;
					}
					//^2759:02A1
					if (bp05 != 50) // 50 - CARRY MINION
						//^2759:02A5
						//^2759:02CC
						//^2759:020F
						return 0;
					//^2759:02A7
					if (si != CmCallFetch)
						//^2759:02AA
						//^2759:02CC
						//^2759:020F
						return 0;
					//^2759:02AC
					//^2759:02CF
					return 1;
				}
				//^2759:02AE
				if (si != CmCallScout && si != CmMark && (bp04->w6 == 0xffff || (si != CmCallCarry && si != CmCallFetch))) {
					//^2759:02CC
					//^2759:020F
					return 0;
				}
			}
			else {
				//^2759:02CC
				//^2759:020F
				return 0;
			}
		}
	}
	//^2759:02CF
	return 1;
}


//^2759:04C0
// SPX: _2759_04c0 renamed FIND_POUCH_OR_SCABBARD_POSSESSION_POS
i16 SkWinCore::FIND_POUCH_OR_SCABBARD_POSSESSION_POS(i16 player, i16 yy)	// yy = pouch or scabbard
{
	//^2759:04C0
	ENTER(0);
	//^2759:04C5
	i16 di = player;
	//^2759:04C8
	if (yy == 1) {	// SCABBARD
		//^2759:04CE
		if (glbChampionSquad[di].Possess(INVENTORY_SCABBARD_1) != OBJECT_NULL) {	// Possess(12)
			//^2759:04DE
			return INVENTORY_SCABBARD_1;	// 12
		}
		//^2759:04E3
		//for (i16 si = 7; si <= 9; si++) {
		for (i16 si = INVENTORY_SCABBARD_2; si <= INVENTORY_SCABBARD_4; si++) {
			//^2759:04E8
			if (glbChampionSquad[di].Possess(si) != OBJECT_NULL) {
				//^2759:04FE
				return si;
			}
			//^2759:0502
		}
	}
	//^2759:050A
	else if (yy == 0) {	// POUCH
		//^2759:0510
		if (glbChampionSquad[di].Possess(INVENTORY_POUCH_1) != OBJECT_NULL) {	// Possess(11)
			//^2759:0520
			return INVENTORY_POUCH_1;	// 11
		}
		//^2759:0525
		if (glbChampionSquad[di].Possess(INVENTORY_POUCH_2) != OBJECT_NULL) {	// Possess(6)
			//^2759:0535
			return INVENTORY_POUCH_2;	// 6
		}
	}
	//^2759:053A
	return -1;
}



//^2C1D:0A47
// SPX: 1 seems to be the minimal skill returned, which correspond to none ...
// Hence, NEOPHYTE would return 2 and ARCHMASTER 16
Bit16u SkWinCore::QUERY_PLAYER_SKILL_LV(i16 player, Bit16u skill, Bit16u yy)
{
	//^2C1D:0A47
	ENTER(8);
	//^2C1D:0A4D
	U16 di = skill;
	//^2C1D:0A50
	if (glbIsPlayerSleeping != 0) {
		//^2C1D:0A57
		return 1;
	}
	//^2C1D:0A5D
	Champion *champion = &glbChampionSquad[player]; //*bp04
	//^2C1D:0A6E
	U32 bp08 = champion->skills[di];	// Amount of current xp for that skill
	//^2C1D:0A86
	if (di > 3) {	// A subskill is requested
		//^2C1D:0A8B
		bp08 += (champion->skills[(di -4) >> 2]) * ((yy != 0) ? (i16(champion->skillBonus[(di -4) >> 2]) +1) : 1);
		//^2C1D:0AD2
		bp08 >>= 1;
	}
	//^2C1D:0AE2
	Bit16u si = 1;	// Minimum level is 1 and equal to NONE.
	//^2C1D:0AE5
	while (bp08 >= SKILL_EXPERIENCE_BASE_LEVEL) {	// (bp08 >= 512)
		//^2C1D:0AE7
		bp08 >>= 1; si++;
		//^2C1D:0AF8
	}
	//^2C1D:0B07
	if (yy != 0) {
		//^2C1D:0B0D
		si = max_value(1, champion->skillBonus[di] + si);
	}
	//^2C1D:0B26
	return si;
}

//^2759:02D6
// SPX: Returns 0 if current item is not hand activable => then it will have no icon in action hand.
// SPX: _2759_02d6 renamed IS_ITEM_HAND_ACTIVABLE
Bit16u SkWinCore::IS_ITEM_HAND_ACTIVABLE(Bit16u player, ObjectID recordLink, i16 yy)
{
	//^2759:02D6
	ENTER(14);
	//^2759:02DC
	ObjectID si = recordLink;
	Bit8u iCategory;	// bp01
	Bit8u iItemIndex;	// bp02
	if (recordLink == OBJECT_NULL) {
		//^2759:02E4
		iCategory = GDAT_CATEGORY_CHAMPIONS;	// 0x16
        iItemIndex = glbChampionSquad[player].HeroType();
	}
	else {
		//^2759:02F8
		if (IS_CONTAINER_MONEYBOX(si) != 0 || IS_CONTAINER_CHEST(si) != 0)
			//^2759:030E
			return 1;
		//^2759:0314
		iCategory = QUERY_CLS1_FROM_RECORD(si);
		iItemIndex = QUERY_CLS2_FROM_RECORD(si);
	}
	//^2759:0325
	//^2759:0328
	i16 di = 0;
	// SPX: scans through champion basic commands : 8 = PUNCH, 9 = KICK, 10 = POUCH, 11 = SCABBARD
	for (Bit8u iEntryNumber = 8; iEntryNumber < 12; iEntryNumber++) {	// Bit8u bp03
		//^2759:0331
		if (di >= 3)
			//^2759:0336
			break;
		//^2759:0339
		if (QUERY_GDAT_ENTRY_IF_LOADABLE(iCategory, iItemIndex, dtText, iEntryNumber) == 0)
			//^2759:0353
			continue;
		//^2759:0356
		Bit16u bp0a = QUERY_CMDSTR_ENTRY(iCategory, iItemIndex, iEntryNumber, CnCM);	// CnCM = Command
		//^2759:036E
		if (bp0a == 0)
			//^2759:0372
			continue;
		//^2759:0375
		Bit16u bp0e = QUERY_CMDSTR_ENTRY(iCategory, iItemIndex, iEntryNumber, CnWH);	// CnWH = where (pouch or scabbard?)
		//^2759:038D
		if (bp0e != 0 && bp0e -1 != yy)
			//^2759:0399
			continue;
		//^2759:039C
		if (si != OBJECT_NULL) {
			//^2759:03A1
			if (_2759_01fe(player, si, bp0a) == 0)
				//^2759:03B3
				continue;
			//^2759:03B6
			i16 bp0c = QUERY_CMDSTR_ENTRY(iCategory, iItemIndex, iEntryNumber, CnNC);	// NC = Number of charges consumed by action
			if (SkCodeParam::bUnlimitedCharges == true)
				bp0c = 0;
			//^2759:03CE
			if (bp0c == 18) {	// ?
				//^2759:03D4
				if (ADD_ITEM_CHARGE(si, 0) == 0) {
					//^2759:03E0
					goto _0425;
				}
				//^2759:03E2
				continue;
			}
			//^2759:03E5
			if (bp0c == 16 || bp0c == 17)
				//^2759:03F1
				bp0c = 1;
			//^2759:03F6
			if (bp0c == 0)
				//^2759:03FA
				goto _0425;
			//^2759:03FC
			if (ADD_ITEM_CHARGE(si, 0) >= bp0c)
				//^2759:0409
				goto _0425;
			//^2759:040B
			continue;
		}
		//^2759:040E
		if (bp0a == CmPouch) {	// (bp0a == 17)
			//^2759:0414
			if (FIND_POUCH_OR_SCABBARD_POSSESSION_POS(player, yy) < 0)
				//^2759:0423
				continue;
		}
		//^2759:0425
_0425:
		Bit16u bp08 = QUERY_CMDSTR_ENTRY(iCategory, iItemIndex, iEntryNumber, CnSK);
		//^2759:043D
		Bit16u bp06 = QUERY_CMDSTR_ENTRY(iCategory, iItemIndex, iEntryNumber, CnLV);
		//^2759:0455
		if (QUERY_PLAYER_SKILL_LV(player, bp08, 1) >= bp06) {
			//^2759:046A
			glbItemSelected[di].category = iCategory;
			//^2759:0476
			glbItemSelected[di].index = iItemIndex;
			//^2759:0482
			glbItemSelected[di].entry = iEntryNumber;
			//^2759:048E
			di++;
		}
		//^2759:048F
	}
	//^2759:049B
	_4976_53a4 = di;
	glbChampionItemInUse = si;
	//^2759:04A3
	if (IS_CONTAINER_MAP(si) != 0)
		//^2759:04AE
		//^2759:030E
		return 1;
	//^2759:04B1
	if (di > 0)
		//^2759:04B5
		return 1;
	//^2759:04BA
	return 0;
}



//^1031:007B
Bit16u SkWinCore::_1031_007b(sk1891 *ref)
{
	//^1031:007B
	ENTER(0);
	//^1031:007E
	return (glbChampionSquad[ref->b1].curHP() != 0) ? 1 : 0;
}

//^1031:009E
Bit16u SkWinCore::_1031_009e(sk1891 *ref)
{
	//^1031:009E
	ENTER(0);
	//^1031:00A1
	return (GET_PLAYER_AT_POSITION((ref->b1 + glbPlayerDir) & 3) >= 0) ? 1 : 0;
}


//^1031:012D
Bit16u SkWinCore::_1031_012d(sk1891 *ref)
{
	//^1031:012D
	ENTER(0);
	//^1031:0130
	return (glbChampionIndex != 0 && ref->b1 == _4976_53a4) ? 1 : 0;
}

//^1031:014F
Bit16u SkWinCore::_1031_014f(sk1891 *ref)
{
	//^1031:014F
	ENTER(0);
	//^1031:0152
	return (glbChampionIndex != 0 && (ref->b1 & (1 << glbChampionTable[glbChampionIndex].runesCount)) != 0) ? 1 : 0;
}

//^1031:0184
Bit16u SkWinCore::_1031_0184(sk1891 *ref)
{
	//^1031:0184
	ENTER(0);
	//^1031:0187
	return (glbChampionIndex != 0) 
		? (((glbMagicalMapFlags & 0x8000) != 0) 
			? ((ref->b1 == _4976_53a4) 
				? 1 
				: 0
			)
			: ((ref->b1 == 5)
				? 1
				: 0
			)
		)
		: 0;
}


//^2C1D:0EA2
U16 SkWinCore::GET_PLAYER_ABILITY(Champion *ref, U16 parm7, U16 getMax)
{
	//^2C1D:0EA2
	ENTER(2);
	//^2C1D:0EA8
	//SPX: Get attribute parm7, getMax = 0 : current / getMax = 1 : max
	i16 si = ref->attributes[parm7][getMax];
	//^2C1D:0EBD
	U16 di;
	U8 bp01;
	if (true
		&& (getMax == 0)
		&& (di = ref->enchantmentPower) != 0
		&& (bp01 = ref->enchantmentAura) >= ENCHANTMENT_AURA_FIRST
		&& (bp01) <= ENCHANTMENT_AURA_LAST
		&& (bp01 + i16(-2)) == parm7
	) {
		//^2C1D:0EED
		if (di > 100)
			//^2C1D:0EF2
			di = 100;
		//^2C1D:0EF5
		si += RAND16(((si * di) >> 7) +1) +4;
	}
	//^2C1D:0F09
	return BETWEEN_VALUE(10, si +(ref->attributesEnhanced[parm7]), 220);
}



//^2C1D:0654
void SkWinCore::PUT_ITEM_TO_PLAYER(Bit16u championIndex)
{
	// click player panel to store leader's holding item into your back pack.

	//^2C1D:0654
	ENTER(0);
	//^2C1D:0659
	if (glbLeaderHandPossession.object != OBJECT_NULL) {
		//^2C1D:0660
		if (glbChampionSquad[championIndex].curHP() != 0) {
			X16 si;
			//^2C1D:0671  
			for (si=INVENTORY_BACKPACK_1; si <= INVENTORY_BACKPACK_LAST; si++) {	// (si=13; si<30; si++)
				//^2C1D:0676
				if (glbChampionSquad[championIndex].Possess(si) == OBJECT_NULL)
					break;
				//^2C1D:068D  
			}
			//^2C1D:0693
			if (si != (INVENTORY_BACKPACK_LAST+1)) {	// (si != 30)
				//^2C1D:0698
				ObjectID di = REMOVE_OBJECT_FROM_HAND();
				//^2C1D:069F
				if (di != OBJECT_NULL) {
					//^2C1D:06A4
					EQUIP_ITEM_TO_INVENTORY(championIndex, di, si);
				}
			}
		}
	}
	//^2C1D:06B1
	return;
}


//^1031:16A0
// SPX: _1031_16a0 renamed SELECT_CHAMPION_LEADER
void SkWinCore::SELECT_CHAMPION_LEADER(Bit16u xx)
{
	//^1031:16A0
	ENTER(0);
	//^1031:16A4
	U16 si = xx;
	//^1031:16A7
	if (si == glbChampionLeader)
		//^1031:16A7
		return;
	//^1031:16AD
	if (si != 0xffff && glbChampionSquad[si].curHP() == 0)
		//^1031:16C0
		return;
	//^1031:16C2
	if (glbChampionLeader != -1) {
		//^1031:16C9
		glbChampionSquad[glbChampionLeader].heroFlag |= CHAMPION_FLAG_1400;	// 0x1400
	}
	//^1031:16D9
	glbChampionLeader = si;
	//^1031:16DD
	if (si == 0xffff)
		//^1031:16E0
		return;
	//^1031:16E2
	if (si +1 == glbNextChampionNumber)
		//^1031:16E9
		return;
	//^1031:16EB
	glbChampionSquad[si].heroFlag |= CHAMPION_FLAG_1400;	// 0x1400
	//^1031:16FA
	return;
}



//^2F3F:0000
void SkWinCore::ADD_ITEM_TO_PLAYER(U16 player, ObjectID rl)
{
	//^2F3F:0000
	ENTER(0);
	//^2F3F:0005
	for (U16 si = 0; si < 5; si++) {
		//^2F3F:000A
		U16 di = _4976_404d[si][0];
		//^2F3F:0017
		for (; _4976_404d[si][1] >= di; di++) {
			//^2F3F:0019
			if (true
				&& glbChampionSquad[player].Possess(di) == OBJECT_NULL
				&& IS_ITEM_FIT_FOR_EQUIP(rl, di, 0) != 0
				&& (false
					|| _4976_404d[si][2] == 0xffff
					|| rl.DBType() == _4976_404d[si][2]
				)
			) {
				//^2F3F:006C
				EQUIP_ITEM_TO_INVENTORY(player, rl, di);
				//^2F3F:007B
				return;
			}
			//^2F3F:007D
		}
		//^2F3F:008D
	}
	//^2F3F:0096
	return;
}

//^2F3F:04EA
// SPX: _2f3f_04ea renamed _2f3f_04ea_CHAMPION
void SkWinCore::_2f3f_04ea_CHAMPION(Bit16u xx, Bit16u yy, Bit16u dir, Bit16u zz, Bit16u ee)
{
	//^2F3F:04EA
	ENTER(142);
	//^2F3F:04F0
	U16 si = glbNextChampionNumber -1;
	//^2F3F:04F6
	Champion *bp04 = &glbChampionSquad[si];
	//^2F3F:0504
	U16 bp0c = glbCurrentMapIndex;
	//^2F3F:050A
	CHANGE_CURRENT_MAP_TO(zz);
	//^2F3F:0513
	ObjectID bp0e = REMOVE_OBJECT_FROM_HAND();
	//^2F3F:051B
	if (ee == UI_EVENTCODE_EXIT_CRYOCELL) { // 161
		//^2F3F:0525
		INTERFACE_CHAMPION(4);
		//^2F3F:052D
		FIRE_HIDE_MOUSE_CURSOR();
		//^2F3F:0532
        if (glbChampionsCount == 1)
			//^2F3F:0539
			SELECT_CHAMPION_LEADER(0xffff);
		//^2F3F:0541
		if (glbNextChampionNumber >= glbChampionsCount)
			//^2F3F:054A
			glbChampionsCount--;
		//^2F3F:054E
		glbNextChampionNumber = 0;
		//^2F3F:0554
		bp04->curHP(0);
		//^2F3F:055D
		glbChampionAlive[si] = 0;
		//^2F3F:0567
		_4976_3de2[si] = 0xff;
		_4976_3dde[RCJ(4,si)] = 0xff;
		//^2F3F:0571
		glbChampionHandCoolingDown[si][1] = 0;
		_4976_3de6[RCJ(4,si)][1] = 0xffff;
		//^2F3F:058A
		glbChampionHandCoolingDown[si][0] = 0;
		_4976_3de6[RCJ(4,si)][0] = 0xffff;
		//^2F3F:05A3
		glbChampionHandCoolingDown[si][2] = 0;
		//^2F3F:05B1
		FIRE_FILL_SCREEN_RECT(si +161, glbPaletteT16[COLOR_BLACK]);
		//^2F3F:05C8
		_38c8_0060();
		//^2F3F:05CD
		FIRE_SHOW_MOUSE_CURSOR();
	}
	else { // ee = UI_EVENTCODE_REVIVE_CHAMPION (160)
		//^2F3F:05D5
		if (_4976_404b == 0)
			//^2F3F:05DC
			DRAW_CRYOCELL_LEVER(1);
		//^2F3F:05E4
		if (bp0e != OBJECT_NULL)
			//^2F3F:05EA
			ADD_ITEM_TO_PLAYER(si, bp0e);
		//^2F3F:05F4
		glbNextChampionNumber = 0;
		//^2F3F:05FA
		xx += glbXAxisDelta[dir];
		//^2F3F:0606
		yy += glbYAxisDelta[dir];
		//^2F3F:0612
		for (U16 bp0a = 0; bp0a < 30; bp0a++) {
			//^2F3F:0619
			ObjectID di = bp04->Possess(bp0a);
			//^2F3F:062A
			if (di != OBJECT_NULL) {
				//^2F3F:062F
				CUT_RECORD_FROM(di, NULL, xx, yy);
			}
			//^2F3F:0642
		}
		//^2F3F:064B
		ObjectID di = GET_TILE_RECORD_LINK(xx, yy);
		// SPX: enter this infinite while only is there is something on the tile (normally that would be the case, calling champion from actuator), but if it isn't, just don't get in the loop.
		if (SkCodeParam::bUseFixedMode == true && (di != OBJECT_END_MARKER && di != OBJECT_NULL)) {
			while (true) {
				//^2F3F:0658
				if (di.DBType() == dbActuator) {
					//^2F3F:0667
					Actuator *bp08 = GET_ADDRESS_OF_ACTU(di);
					//^2F3F:0674
					if (bp08->ActuatorType() == ACTUATOR_TYPE_RESURECTOR)
					{
						//^2F3F:0683
						bp08->OnceOnlyActuator(0);
						//^2F3F:0688
						break;
					}
					else if (bp08->ActuatorType() == ACTUATOR_TYPE_CHAMPION_MIRROR) // SPX: handle DM1 Champion Mirror too
					{
						bp08->ActiveStatus(1); // change to "inactive", different from the "once only" status
						break;
					}
				}
				//^2F3F:068A
				di = GET_NEXT_RECORD_LINK(di);
				if (SkCodeParam::bUseFixedMode == true && (di == OBJECT_END_MARKER || di == OBJECT_NULL))
					break;
			}
		} // fixed mode
		//^2F3F:0693
		if (glbChampionsCount == 1) {
			//^2F3F:069A
			_4976_4b80 = glbGameTick;
		}
		//^2F3F:06A8
		if (_4976_404b == 0) {
			//^2F3F:06AF
			glbChampionBonesIndex = si;
			//^2F3F:06B3
			DISPLAY_HINT_NEW_LINE();
			//^2F3F:06B8
			U8 bp008e[128];
			// SPX: Text : AWAKENED
			DISPLAY_HINT_TEXT(glbChampionColor[si], QUERY_GDAT_TEXT(0x01, 0x00, 0x0e, bp008e));
			//^2F3F:06DD
			if (glbChampionInventory != 0) {
				//^2F3F:06E4
				INTERFACE_CHAMPION(4);
				//^2F3F:06EC
				_38c8_0060();
			}
		}
	}
	//^2F3F:06F1
	CHANGE_CURRENT_MAP_TO(bp0c);
	//^2F3F:06FA
	return;
}



//^24A5:1798
// SPX: _24a5_1798 renamed INTERFACE_CHAMPION (not sure of what's inside)
void SkWinCore::INTERFACE_CHAMPION(Bit16u xx)
{
	//^24A5:1798
	ENTER(8);
	//^24A5:179E
	i16 di = xx;
	//^24A5:17A1
	U16 bp06 = 0;
	glbInventorySubpanel = 0;
	_4976_3d30 = OBJECT_NULL;
	//^24A5:17B2
	if (di < 4 && glbChampionSquad[di].curHP() == 0)
		//^24A5:17C7
		return;
	//^24A5:17CA
	if (_4976_4c3e != 0 || glbTryPushPullObject != 0 || _4976_4bfe != 0 || glbShowItemStats != 0)
		//^24A5:17EF
		return;
	//^24A5:17F2
	_4976_4e5c = 1;
	//^24A5:17F8
	U16 si = glbChampionInventory;
	//^24A5:17FC
	if (di +1 == si)
		//^24A5:1803
		di = 4;
	//^24A5:1806
	if (si == 0 || di == 4) {
		//^24A5:180F
		bp06 = 1;
		//^24A5:1814
		FIRE_HIDE_MOUSE_CURSOR();
	}
	//^24A5:1819
	if (si != 0) {
		//^24A5:1820
		glbChampionInventory = 0;
		//^24A5:1826
		Champion *bp04 = &glbChampionSquad[si -1];
		//^24A5:1837
		if (bp04->curHP() != 0 && glbNextChampionNumber == 0) {
			//^24A5:1848
			bp04->heroFlag |= CHAMPION_FLAG_4000;	// 0x4000
			//^24A5:184E
			REFRESH_PLAYER_STAT_DISP(si -1);
		}
		//^24A5:1858
		for (i16 bp08 = 0; bp08 < 4; bp08++) {
			//^24A5:185F
			glbChampionMajorSkillsLevel[si -1][bp08] -= _4976_52c0[bp08];
			//^24A5:1875
		}
		//^24A5:187E
		if (glbIsPlayerSleeping != 0) {
			//^24A5:1885
			if (bp06 != 0) {
				//^24A5:188E
				_443c_040e();
				FIRE_SHOW_MOUSE_CURSOR();
			}
			//^24A5:1898
			goto _1913;
		}
		//^24A5:189A
		if (di == 4) {
			//^24A5:189F
			if (_4976_3d59 != 0)
				//^24A5:18A6
				DRAW_ARROW_PANEL();
			//^24A5:18AB
			if (bp06 != 0)
				//^24A5:18B1
				FIRE_SHOW_MOUSE_CURSOR();
			//^24A5:18B6
			INIT_BACKBUFF();
			//^24A5:18BB
			goto _1913;
		}
	}
	//^24A5:18BD
	glbChampionInventory = di +1;
	//^24A5:18C3
	if (si == 0)
		//^24A5:18C7
		FIRE_FILL_HALFTONE_RECTI(9, 0);
	//^24A5:18D2
	ZERO_MEMORY(_4976_52c0, 4);
	//^24A5:18E2
	glbChampionSquad[di].heroFlag |= CHAMPION_FLAG_FC00;	// 0xFC00
	_4976_581a = 0;
	//^24A5:18F7
	REFRESH_PLAYER_STAT_DISP(di);
	//^24A5:18FE
	if (bp06 != 0)
		//^24A5:1904
		FIRE_SHOW_MOUSE_CURSOR();
	//^24A5:1909
	_1031_0541(8);
	//^24A5:1911
	return;
	//^24A5:1913
_1913:
	_1031_0667();
	//^24A5:1918
	return;
}


//^2759:21F5
i16 SkWinCore::FIND_HAND_WITH_EMPTY_FLASK(Champion *ref)
{
	// CSBwinSimilarity: TAG01c9b2,GetFlaskInHand

	// return -1 if you've not.
	// return 0 if you've at right hand.
	// return 1 if you've at left hand.

	//^2759:21F5
	ENTER(0);
	//^2759:21FA
	i16 si;
	for (si = 2; --si >= 0; ) {
		//^2759:21FF
		ObjectID di = ref->Possess(si);
		//^2759:220F
		if (true
			&& di.DBType() == dbPotion
			&& QUERY_CLS2_FROM_RECORD(di) == 0x14
		) {
			//^2759:2223
			break;
		}
		//^2759:2225
	}
	//^2759:2228
	return si;
}

//^2C1D:0B2C
// SPX: zz = base experience to gain
void SkWinCore::ADJUST_SKILLS(U16 player, U16 yy, U16 zz)
{
	// CSBwinSimilarity: TAG01605a,AdjustSkills
	// SPX: Should really compare with CSBWin LevelUp

	//^2C1D:0B2C
	ENTER(142);
	//^2C1D:0B32
	U16 di = yy;	// main skill
	U16 subSkill = di;
	U16 baseXP = zz;
	U16 doubleXP = 0;
	U16 halfBaseXP = 0;
	U16 halvedXP = zz;
	//^2C1D:0B35
	if (true
		&& di >= 4 
		&& di <= 11
		&& (glbGameTick -150 > _4976_0090)
	) {
		//^2C1D:0B5B
		zz >>= 1;
		halfBaseXP = 1; // SPX added for info
		halvedXP = zz;
	}
	//^2C1D:0B5E
	if (zz == 0)
		//^2C1D:0B64
		return;
	//^2C1D:0B67
	U16 bp06 = dunMapLocalHeader->Difficulty();	// map XP multiplicator
	U16 xpMapMultiplicator = bp06;	// SPX: added for reuse
	U16	afterMapXP = 0; // for info
	//^2C1D:0B78
	if (bp06 != 0) {
		//^2C1D:0B7C
		zz = zz * bp06;
	}
	afterMapXP = zz;
	//^2C1D:0B85
	Champion *champion = &glbChampionSquad[player]; //*bp04
	//^2C1D:0B96
	U16 si = (di >= 4) ? ((di -4) >>2) : di;
	U16 mainSkill = si;
	//^2C1D:0BA9
	bp06 = QUERY_PLAYER_SKILL_LV(player, si, 0);	// Current skill level
	//^2C1D:0BB9
	if (true
		&& di >= 4
		&& ((i32)glbGameTick -25 < (i32)_4976_0090)
	) {
		//^2C1D:0BD9
		zz <<= 1;
		doubleXP = 1; // SPX added for info
	}
	//^2C1D:0BDC
	champion->skills[di] += zz;
	//^2C1D:0BF2
	if (di >= 4) {
		//^2C1D:0BF7
		champion->skills[si] += zz;
	}
	
	if (SkCodeParam::bUsePowerDebug)
	{
		U8 message[256];
		U8 consolePlayerColorsTab[4] = { LIGHT_GREEN, LIGHT_YELLOW, LIGHT_RED, LIGHT_BLUE };
		U8 consolePlayerColor = 0;

		memset(message, 0, 256);
		sprintf((char*)message, "%s: +%d XP %s (XP=%d)!\n"
			, glbChampionTable[player+1].firstName, zz
			, getSkillName(di), champion->skills[di]);
		//sprintf((char*)message, "%s: Map XP Multi = %d; base XP gain = %d, Skill required = %d (main = %d), level of skill = %d | Halve = %d => %d | Map XP = %d\n",
		//	glbChampionTable[player+1].firstName, xpMapMultiplicator, baseXP, subSkill, mainSkill, bp06, halfBaseXP, halvedXP, afterMapXP, doubleXP);
#ifndef __SKWIN_PUBLIC_VERSION__        
		DISPLAY_HINT_TEXT(glbChampionColor[player], message);
#endif // __SKWIN_PUBLIC_VERSION__

		consolePlayerColor = consolePlayerColorsTab[player];
		sprintf((char*)message, "%7s: Base XP = %03d [%s][%s] Skill Level = %d || Halved? %d => %d XP || Map XP modified = %d => %d XP || Doubled? %d => %d XP (final) || Total result XP = %d (%s) - %d (%s)\n",
			glbChampionTable[player+1].firstName, baseXP, getSkillName(subSkill), getSkillName(mainSkill), bp06,
			halfBaseXP, halvedXP, xpMapMultiplicator, afterMapXP, doubleXP, zz,
			champion->skills[subSkill], getSkillName(subSkill), champion->skills[mainSkill], getSkillName(mainSkill));
		CHANGE_CONSOLE_COLOR(BRIGHT, consolePlayerColor, BLACK);
		SkD((DLV_XP, "%s", message));
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);

	}

	//^2C1D:0C0A
	zz = QUERY_PLAYER_SKILL_LV(player, si, 0);	// Compare with new skill level
	//^2C1D:0C1A
	if (zz <= bp06)
		//^2C1D:0C1F
		return;
	//^2C1D:0C22
	U16 bp0c = zz;
	//^2C1D:0C25
	U16 bp08 = RAND01();		// 0 - 1
	//^2C1D:0C2D
	U16 bp0a = RAND01() +1;		// 1 - 2
	//^2C1D:0C36
	bp06 = RAND01();			// 0 - 1
	//^2C1D:0C3E
	if (si != 2)
		//^2C1D:0C43
		bp06 &= zz;
	//^2C1D:0C49
	champion->maxVit(champion->maxVit() +U8(bp06));
	//^2C1D:0C57
	bp06 = champion->curStamina();
	//^2C1D:0C5E
	champion->maxAntiF(champion->maxAntiF() + (RAND01() & (~zz)));
	//^2C1D:0C77
	U8 bp0d;
	switch (si) {
		case 0:
			//^2C1D:0C88
			bp0d = 6;		// 0x06 -> "<01>' JUST GAINED A FIGHTER LEVEL.<00>"
			//^2C1D:0C8C
			bp06 /= 16;		// Stamina increase
			//^2C1D:0C90
			// SPX: This is strange
			//zz = bp06 * zz;
			zz *= 3;
			//^2C1D:0C9B
			champion->maxStr(champion->maxStr() +U8(bp0a));	// STR + { 1 - 2 }
			//^2C1D:0CA9
			champion->maxDex(champion->maxDex() +U8(bp08));	// DEX + { 0 - 1 }
			//^2C1D:0CB7
			break;

		case 1:
			//^2C1D:0CBA
			bp0d = 7;		// 0x07 -> "<01>' JUST GAINED A NINJA LEVEL.<00>"
			//^2C1D:0CBE
			bp06 = bp06 / 21;
			//^2C1D:0CCB
			zz <<= 1;
			//^2C1D:0CCE
			champion->maxStr(champion->maxStr() +U8(bp08));	// STR + { 0 - 1 }
			//^2C1D:0CDC
			champion->maxDex(champion->maxDex() +U8(bp0a));	// DEX + { 1 - 2 }
			//^2C1D:0CE3
			//^2C1D:0CB0
			//^2C1D:0CB7
			break;

		case 3:
			//^2C1D:0CE5
			bp0d = 9;		// 0x09 -> "<01>' JUST GAINED A WIZARD LEVEL.<00>"
			//^2C1D:0CE9
			bp06 >>= 5;
			//^2C1D:0CED
			champion->maxMP(zz + (zz >> 1) + champion->maxMP());
			//^2C1D:0D02
			champion->maxWiz(champion->maxWiz() +U8(bp0a));	// WIS + { 1 - 2 }
			//^2C1D:0D09
			goto _0d3a;

		case 2:
			//^2C1D:0D0B
			bp0d = 8;		// 0x08 -> "<01>' JUST GAINED A PRIEST LEVEL.<00>"
			//^2C1D:0D0F
			bp06 = bp06 / 25;
			//^2C1D:0D1C
			champion->maxMP(champion->maxMP() +zz);
			//^2C1D:0D2A
			zz += (zz +1) >> 1;
			//^2C1D:0D33
			champion->maxWiz(champion->maxWiz() +U8(bp08));	// WIS + { 0 - 1 }
			//^2C1D:0D41
// SPX: label _0d3a = end of check for wizard/priest
_0d3a:
			champion->maxMP(champion->maxMP() +min_value(RAND02(), bp0c -1));
			//^2C1D:0D5E
			// SPX: Removed hard coded 900
			if (champion->maxMP() > MAXMP)
				//^2C1D:0D63
				champion->maxMP(MAXMP);
			//^2C1D:0D69
			champion->maxAntiM(champion->maxAntiM() +RAND02());

			break;
	}
	//^2C1D:0D7B
	//bp04->maxHP(RAND16((zz >> 1) +1) +zz +bp04->maxHP());
	// SPX: From CSBWin11 LevelUp, it should be more like this:
	champion->maxHP(RAND16((zz / 2) +1) +zz +champion->maxHP());
	//^2C1D:0D98
	if (champion->maxHP() > MAXHP)
		//^2C1D:0D9F
		champion->maxHP(MAXHP);
	//^2C1D:0DA5
	champion->maxStamina(RAND16((bp06 / 2) +1) +bp06 +champion->maxStamina());
	//^2C1D:0DC2
	if (champion->maxStamina() > MAXSP)
		//^2C1D:0DC9
		champion->maxStamina((i16)MAXSP);
	//^2C1D:0DCF
	champion->heroFlag |= CHAMPION_FLAG_3800;	// 0x3800
	//^2C1D:0DD8
	glbChampionMajorSkillsLevel[player][si]++;
	//^2C1D:0DE2
	DISPLAY_HINT_NEW_LINE();
	//^2C1D:0DE7
	glbChampionBonesIndex = player;
	//^2C1D:0DED
	U8 bp008e[128];
	DISPLAY_HINT_TEXT(glbChampionColor[player], QUERY_GDAT_TEXT(GDAT_CATEGORY_INTERFACE_GENERAL, GDAT_INTERFACE_SUBCAT_BASE_DATA, bp0d, bp008e));
	//^2C1D:0E17
	return;
}



//^2C1D:1CF3
// SPX: _2c1d_1cf3 renamed SHOOT_CHAMPION_MISSILE
void SkWinCore::SHOOT_CHAMPION_MISSILE(Champion *ref, ObjectID rl, U16 yy, U16 zz, U16 ww)
{
	//^2C1D:1CF3
	ENTER(0);
	//^2C1D:1CF7
	U16 si = ref->playerDir();
	//^2C1D:1D02
	SHOOT_ITEM(
		rl,
		glbPlayerPosX,
		glbPlayerPosY,
		((((ref->playerPos() -si +1) & 2) >> 1) +si) & 3,
		si,
		min_value(yy, 255),
		min_value(zz, 255),
		min_value(ww, 255)
		);

	SkD((DLV_TWEET, "Tweet: You (x:%d, y:%d, map:%d) have thrown %s by cast! \n"
		, glbPlayerPosX, glbPlayerPosY, glbCurrentMapIndex, static_cast<LPCSTR>(getRecordNameOf(rl))
		));
	//^2C1D:1D57
	glbPlayerThrowCounter = 4;
	//^2C1D:1D5D
	_4976_4c0c = si;
	//^2C1D:1D61
	return;
}


//^2C1D:1D64
// SPX: _2c1d_1d64 renamed CAST_CHAMPION_MISSILE_SPELL
U16 SkWinCore::CAST_CHAMPION_MISSILE_SPELL(U16 xx, ObjectID rl, i16 zz, U16 ww)
{
	//^2C1D:1D64
	ENTER(4);
	//^2C1D:1D6A
	U16 di = zz;
	//^2C1D:1D6D
	Champion *champion = &glbChampionSquad[xx];	// *bp04
	//^2C1D:1D7E
	if (champion->curMP() < ww)
		//^2C1D:1D8A
		return 0;
	//^2C1D:1D8E
	champion->curMP(champion->curMP() -ww);
	//^2C1D:1D9C
	champion->heroFlag |= CHAMPION_FLAG_0800;	// 0x0800
	//^2C1D:1DA2
	U16 si = 10 - min_value(8, champion->maxStamina() >> 3);
	//^2C1D:1DBA
	if ((si << 2) > di) {
		//^2C1D:1DC3
		di += 3;
		//^2C1D:1DC6
		si--;
	}
	//^2C1D:1DC7
	SHOOT_CHAMPION_MISSILE(champion, rl, di, 90, si);
	//^2C1D:1DDB
	return 1;
}




	//^2C1D:0186
// SPX: _2c1d_0186 replaced by CALL_ENCHANTMENT_SELF
U16 SkWinCore::CALL_ENCHANTMENT_SELF(Champion *ref, i16 xx, U16 yy, U16 zz)
{
	//^2C1D:0186
	ENTER(0);
	//^2C1D:018B
	X16 di = yy;
	X16 si = 1;
	if (zz != 0) {
		//^2C1D:0197
		if (ref->curMP() == 0)
			return 0;
		//^2C1D:01A5
		if (ref->curMP() < 4) {
			//^2C1D:01AF
			di >>= 1;
			ref->curMP(0);
			si = 0;
		}
		else {
			//^2C1D:01BB
			ref->curMP(4);
		}
	}
	//^2C1D:01C3
	PROCEED_ENCHANTMENT_SELF(15, xx, di >> 5, di);
	//^2C1D:01D7
	return si;
}

//^2759:0F39
// SPX: _2759_0f39 renamed ADJUST_HAND_COOLDOWN
void SkWinCore::ADJUST_HAND_COOLDOWN(U16 player, U16 yy, U16 zz)
{
	//^2759:0F39
	ENTER(6);
	//^2759:0F3F
	U16 di = yy;
	U16 si = zz;
	//^2759:0F45
	Champion *champion = &glbChampionSquad[player];
	//^2759:0F56
	U16 bp06;
	(si == 0xffff) ? (bp06 = 3, si = 0) : (bp06 = 1);
	//^2759:0F69
	if (glbGlobalSpellEffects.AuraOfSpeed != 0)
		//^2759:0F70
		di >>= 2;
	//^2759:0F73
	di += 2;

	do {
		//^2759:0F75
		U16 cx = champion->handCooldown[si];
		//^2759:0F80
		if (cx < di) {
			//^2759:0F84
			cx = (cx >> 1) +di;
		}
		else {
			//^2759:0F8E
			cx += di >> 1;
		}
		//^2759:0F94
		if (cx > 255)
			//^2759:0F9A
			cx = 255;
		//^2759:0F9D
		champion->handCooldown[si] = U8(cx);
		//^2759:0FA6
		si++;
		//^2759:0FA7
	} while (--bp06 != 0);

	//^2759:0FAC
	return;
}

//^2759:222E
// SPX: _2759_222e replaced by CAST_SPELL_PLAYER
// CSBWin: partially TAG01ca0c
U16 SkWinCore::CAST_SPELL_PLAYER(U16 player, SpellDefinition *ref, U16 power)
{
	// CSBwinSimilarity: TAG01ca0c,CastSpell

	// 0 if succeeds
	// not 0 if fails

	//^2759:222E
	ENTER(30);
	//^2759:2234
	Champion *champion = &glbChampionSquad[player];	//*bp04
	//^2759:2245
	U16 bp0e = (ref->w6_a_f() * (power +18)) / 24;
	//^2759:2264
	U16 bp08 = ref->difficulty +power; // global difficulty of the spell
	//^2759:2273
	U16 bp0c = 0
		+((RAND() & 7) + (bp08 << 4))
		+((ref->difficulty * (power -1)) << 3)
		+(bp08 * bp08)
		;
	//^2759:22A6
	U16 bp06 = QUERY_PLAYER_SKILL_LV(player, ref->requiredSkill, 1);
	//^2759:22BD
	U16 di;
	i16 bp0a;
	for (bp0a = bp08 -bp06; (bp0a--) > 0; ) {
		//^2759:22C8
		if ((RAND() & 127) > min_value(GET_PLAYER_ABILITY(champion, abWiz, 0) +15, 115)) {
			//^2759:22F5
			ADJUST_SKILLS(player, ref->requiredSkill, bp0c << (bp08 -bp06));
			//^2759:2316
			di = 16;
			//^2759:2319
			goto _26fe; // go to end, spell failed ?
		}
		//^2759:231C
	}
	//^2759:2326
	U16 bp1a;
	//SPX: SPELL TYPE => 16 possible spell types!
	switch (ref->w6 & 15) {
		case SPELL_TYPE_POTION: // 1: transform flask to potion
			{
				//^2759:2341
				bp0a = FIND_HAND_WITH_EMPTY_FLASK(champion);
				//^2759:2350
				if (bp0a < 0) {
					//^2759:2354
					di = 48;		// SPX: for displaying graphics : needs flask in hand
					//^2759:2357
					goto _26fe;
				}
				//^2759:235A
				ObjectID si = REMOVE_POSSESSION(player, bp0a);
				//^2759:2369
				SET_ITEMTYPE(si, ref->SpellCastIndex());
				//^2759:237E
				GET_ADDRESS_OF_RECORD8(si)->PotionPower((RAND() & 15) + (power * 40));
				//^2759:23AC
				EQUIP_ITEM_TO_INVENTORY(player, si, bp0a);
				//^2759:23BB
				break;
			}
		case SPELL_TYPE_MISSILE: // 2: shoot spell
			{
				//^2759:23BE
				if (ref->SpellCastIndex() == 4) // zo spell ?
					//^2759:23D0
					bp06 <<= 1; // multiply by 2
				//^2759:23D3
				CAST_CHAMPION_MISSILE_SPELL(
					player,
					ObjectID::MissileRecord(ref->SpellCastIndex()),
					BETWEEN_VALUE(21, ((bp06 << 1) +4) * (power +2), 255),
					0
					);
				//^2759:2411
				break;
			}
		case SPELL_TYPE_SUMMON: // 4: summon creature
			{
				//^2759:2414
				// SPX: beware, hard test for U-Haul minion (0x35)
				if (ref->SpellCastIndex() == CREATURE_GOOD_U_HAUL_MINION) {
					//^2759:2426
					Creature *bp1e = reinterpret_cast<Creature *>(glbDBObjectData[dbCreature]);
					//^2759:2433
					for (bp08 = dunHeader->nRecords[dbCreature]; (bp08--) != 0; bp1e++) {
						//^2759:2440
						if (true
							&& bp1e->w0 != OBJECT_NULL 
							&& bp1e->CreatureType() == CREATURE_GOOD_U_HAUL_MINION
							&& (bp0a = bp1e->b5) != 0x00ff
							&& glbTabCreaturesInfo[bp0a].Command != ccmDestroy	// 0x13
						) {
							//^2759:2470
							glbTabCreaturesInfo[bp0a].Command2 = ccmDestroy;	// 0x13
							//^2759:2483
							goto _26cf;
						}
						//^2759:2486
					}
				}
				//^2759:2494
				ObjectID si = CREATE_MINION(
					ref->SpellCastIndex(),
					((RAND02() + (bp06 << 1)) * power) / 6,
					(glbPlayerDir +2) &3,
					glbPlayerPosX,
					glbPlayerPosY,
					glbPlayerMap,
					-1,
					glbPlayerDir
					);
				//^2759:24E0
				if (si == OBJECT_NULL)	// SPX: Summon failed? create a cloud
					//^2759:24E8
					CREATE_CLOUD(OBJECT_EFFECT_CLOUD, 0x6e, glbPlayerPosX, glbPlayerPosY, 255);	// oFFA8
				//^2759:24FF
				break;
			}
		case SPELL_TYPE_GENERAL: // 3: enchantments + general
			{
				//^2759:2502
				Timer bp18;
				bp18.actor = TIMER_ACTOR__00;
				//^2759:2506
				bp08 = (power +1) << 2; // (power+1)*4
				//^2759:2513
				ObjectID si;
				switch (ref->SpellCastIndex()) {
				case  0:		// Long Light (OH IR RA)
					//^2759:252D
					bp0a = CmLongLight;
					//^2759:2532
					goto _2540;

				case  1:		// Darkness (DES IR SAR)
					//^2759:2534
					bp0a = CmDarkness;
					//^2759:2539
					goto _2540;

				case  5:		// Light (FUL)
					//^2759:253B
					bp0a = CmLight;
					//^2759:2540
_2540:	// Do light
					PROCEED_LIGHT(bp0a, bp08 * 9);
					//^2759:2550
					//^2759:25AC
					//^2759:2696
					break;

				case  3:		// Invisibility (OH EW SAR)
					//^2759:2553
					bp18.TimerType(ttyInvisibility);
					//^2759:2557
					if (glbGlobalSpellEffects.Invisibility++ == 0 && glbChampionInventory != 0) {
						//^2759:256B
						glbChampionTable[glbChampionInventory].heroFlag |= CHAMPION_FLAG_4000;	// 0x4000
					}
					//^2759:257B
					bp08 <<= 3;
					//^2759:257F
					bp18.SetMap(glbPlayerMap);
					bp18.SetTick(glbGameTick +bp08);
					//^2759:25A2
					QUEUE_TIMER(&bp18);
					//^2759:25AC
					//^2759:2696
					break;

				// SPX: (DM1 : OH EW RA See thru walls, used TT_71)
				case  2:	// Spell shield
					//^2759:25AF
					bp0a = ENCHANTMENT_SPELL_SHIELD;	// type shield = 1
					//^2759:25B4
_25b4:
					bp08 = bp08 * bp08 + 100;
					//^2759:25C0
					goto _25e8;

				case  8:	// Fire shield
					//^2759:25C2
					bp0a = ENCHANTMENT_FIRE_SHIELD;		// type shield = 0
					//^2759:25C7
					goto _25b4;

				case  4:	// Party shield (YA IR)
					//^2759:25C9
					bp0a = ENCHANTMENT_PARTY_SHIELD;	// type shield = 2
					//^2759:25CE
					bp08 = bp08 * bp08;
					//^2759:25D4
					//^2759:25BD
					//^2759:25C0
					goto _25e8;

					// SPX: DM1: Was YA-BRO-ROS Foot prints
				case  6:	// Aura of Wisdom
					//^2759:25D6
					bp0a = ENCHANTMENT_AURA_WISDOM;		// type = 5
					//^2759:25DB
_25db:
					bp08 += 3;
					//^2759:25DF
					bp08 = bp08 * bp08;
					//^2759:25E8
_25e8:
					CALL_ENCHANTMENT_SELF(champion, bp0a, bp08, 0);
					//^2759:25FC
					//^2759:2664
					//^2759:2667
					break;
				// SPX DM1 : Was ZO KATH RA
				case  7:	// Aura of Dexterity
					//^2759:25FE
					bp0a = ENCHANTMENT_AURA_DEXTERITY;	// type = 4
					//^2759:2603
					goto _25db;

				case  9:	// Aura of Vitality
					//^2759:2605
					bp0a = ENCHANTMENT_AURA_VITALITY;	// type = 6
					//^2759:260A
					goto _25db;

				case 10:	// Aura of Strength
					//^2759:260C
					bp0a = ENCHANTMENT_AURA_STRENGTH;	// type = 3
					//^2759:2611
					goto _25db;

				case 11:	// Aura of Speed
					//^2759:2613
					glbGlobalSpellEffects.AuraOfSpeed = U8(min_value((bp08 << 3) + glbGlobalSpellEffects.AuraOfSpeed, 255));
					//^2759:262F
					break;

				case 14:	// Spell reflector : oFF8E
					//^2759:2632
					CREATE_CLOUD(
						OBJECT_EFFECT_REFLECTOR, 
						BETWEEN_VALUE(21, ((bp06 << 1) +4) * (power +2), 255),
						glbPlayerPosX,
						glbPlayerPosY,
						255
						);
					//^2759:2667
					break;

				case 15:	// Magical Marker => Item creator!
					//^2759:2669
					bp1a = QUERY_GDAT_ENTRY_DATA_INDEX(0x0d, 0x0f, dtWordValue, 0x42);
					//SPX query missile (0x0D) number 0x0F, read bin0B (dtWordValue) entry 0x42.
					//--> In the GDAT, it holds the value 013E (from item activation list) which targets the misc 0x3E = YaEw marker.
					//--> To change the generated value, the GDAT entry must be changed!

					//^2759:267C
					si = ALLOC_NEW_DBITEM(bp1a);
					//^2759:2687
					if (glbLeaderHandPossession.object == OBJECT_NULL) {
						//^2759:268E
						TAKE_OBJECT(si, 1);
						//^2759:2698
						break;
					}
					else {
						//^2759:269A
						bp08 = (RAND01() + glbPlayerDir) & 3;
						//^2759:26AC
						MOVE_RECORD_TO(
							ObjectID(si, bp08),
							-1,
							0,
							glbPlayerPosX,
							glbPlayerPosY
							);

						break;
					}
					// SPX: Two unused cases, would be nice to have Window spell back. Can we do this here?
					// In a more general way, the effects should not be hard coded ...
				/*case 12:	// NEW, used for restoring all party health
					if (SkCodeParam::bUseDM2ExtendedMode)
					{
						Champion*	ch;
						for (int championIndex = 0; championIndex < glbChampionsCount; championIndex++) {
							ch = &glbChampionSquad[championIndex];
							if (ch->curHP() == 0)
								continue;
							ch->curHP(ch->maxHP());
							ch->heroFlag |= CHAMPION_FLAG_0800;	// 0x0800
						}
						// This only does not refresh the HP bar.
					}
					break;
				case 13:	// NEW, used for restoring current champion health
					if (SkCodeParam::bUseDM2ExtendedMode)
					{
						champion->curHP(champion->maxHP());
						champion->heroFlag |= CHAMPION_FLAG_0800;	// 0x0800
						// This only does not refresh the HP bar.
					}
					break;*/
#if XDM1_EXTENDED_SEETHRUWALLS == 1
				case 12: // SPX: Reimplementation of OH EW RA See thru walls, use TT_71)
					bp18.TimerType(ttySeeThruWalls);
					glbGlobalSpellEffects.SeeThruWalls++;
					bp08 /= 2;
					// csbwin tag01cda4
					bp08 = bp08 * bp08;
					// csbwin tag01cdaa (not yet correctly done, see below)
					bp18.SetMap(glbPlayerMap);
					bp18.SetTick(glbGameTick + bp08);
					QUEUE_TIMER(&bp18);
					break;
					/*
tag01cdaa:
          D0L = d.Time;
          D6L &= 0xffff;
          D0L += D6L;
          D1L = d.partyLevel << 24;
          timer_20.timerTime = D1L | D0L;
          gameTimers.SetTimer(&timer_20);
		  */
#endif
				}

				break;
			}
	}
	//^2759:26CF
_26cf:
	di = 0;
	//^2759:26D1
	if (bp0e != 0) {
		//^2759:26D7
		ADJUST_SKILLS(player, ref->requiredSkill, bp0c);
		//^2759:26EF
		ADJUST_HAND_COOLDOWN(player, bp0e, 2);
	}
	//^2759:26FE
_26fe:
	if (di != 0) {
		//^2759:2702
		bp08 = ref->requiredSkill;
		//^2759:270E
		// SPX: if it is beyond the 4 global skills
		if (bp08 > 3) {
			//^2759:2713
			bp08 = (bp08 -4) >> 2;
			// SPX: Set to 1st detailed skill?
		}
		//^2759:271C
		di |= bp08;
	}
	//^2759:271F
	return di;
}



//^2C1D:0F67
// SPX: _2c1d_0f67 renamed USE_LUCK_ATTRIBUTE
U16 SkWinCore::USE_LUCK_ATTRIBUTE(Champion *ref, U16 xx)
{
	U8 message[256];
	U8 player = 0;
	//^2C1D:0F67
	ENTER(0);
	//^2C1D:0F6B
	U16 si = xx;
	
	//--- Debug / Display
	memset(message, 0, 256);
	U8 consolePlayerColorsTab[4] = { LIGHT_GREEN, LIGHT_YELLOW, LIGHT_RED, CYAN };
	U8 consolePlayerColor = 0;
	
//	sprintf((char*)message, "USE_LUCK_ATTRIBUTE(%7s, %02d)\n", ref->firstName, xx);
//	DISPLAY_HINT_TEXT(glbChampionColor[0], message);

	consolePlayerColor = consolePlayerColorsTab[player];
	sprintf((char*)message, "USE_LUCK_ATTRIBUTE(%7s, %02d)\n", ref->firstName, xx);
	CHANGE_CONSOLE_COLOR(BRIGHT, consolePlayerColor, BLACK);
	SkD((DLV_XP, "%s", message));
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);


	//^2C1D:0F6E
	if (RAND01() != 0 && RAND16(100) > si) {
		//^2C1D:0F83
		return 1;
	}
	//^2C1D:0F88
	si = (RAND16(GET_PLAYER_ABILITY(ref, ATTRIBUTE_LUCK, 0) << 1) > si) ? 1 : 0;
	//^2C1D:0FAF
	ref->attributes[ATTRIBUTE_LUCK][ATTRIBUTE_CURRENT] = U8(BETWEEN_VALUE(10, ref->attributes[ATTRIBUTE_LUCK][ATTRIBUTE_CURRENT] +((si != 0) ? -2 : 2), min_value(220, GET_PLAYER_ABILITY(ref, 0, 1))));
	//^2C1D:0FF9
	return si;
}

//^075F:1F33
U16 SkWinCore::CALC_PLAYER_ATTACK_DAMAGE(Champion *ref, U16 player, ObjectID rlEnemy, U16 xx, U16 yy, U16 valPb, U16 valDM, U16 valSk, U16 valAt)
{
	//^075F:1F33
	ENTER(16);
	//^075F:1F39
	if (player >= glbChampionsCount)
		//^075F:1F42
		return 0;
	//^075F:1F47
	if (ref->curHP() == 0)
		//^075F:1F51
		//^075F:1F42
		return 0;
	//^075F:1F53
	U16 bp0c = dunMapLocalHeader->Difficulty() << 1;
	//^075F:1F66
	Creature *bp08 = GET_ADDRESS_OF_RECORD4(rlEnemy);
	//^075F:1F75
	AIDefinition *bp04 = QUERY_CREATURE_AI_SPEC_FROM_TYPE(bp08->CreatureType());
	//^075F:1F89
	i16 si;
	i16 di;
	if (true
		&& bp04->Defense != 255
		&& ((bp04->w24 & 0x1000) == 0 || valAt == 1)
	) {
		//^075F:1FAA
		ObjectID bp10 = GET_CREATURE_AT(xx, yy);
		//^075F:1FBA
		if (bp10 == OBJECT_NULL || CREATURE_1c9a_0958(bp10) == 0) {
			//^075F:1FCF
			U16 bp0e = valPb & 0x8000;
			//^075F:1FD8
			valPb &= 0x7fff;
			//^075F:1FDD
			if (bp04->w0_5_5() == 0 || bp0e != 0) {
				//^075F:1FF0
				if (false
					|| USE_DEXTERITY_ATTRIBUTE(player) > (((RAND() & 31) + bp04->Defense +bp0c +(glbLightLevel << 1) -16) >> 1)
					|| RAND02() == 0
					|| USE_LUCK_ATTRIBUTE(ref, 75 -valPb) != 0
				) {
					//^075F:2047
					si = COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(player, glbSelectedHandAction, valSk);
					//^075F:205B
					if (si != 0) {
						//^075F:205F
						si = RAND16((si >> 1) +1) +si;
						//^075F:2071
						si = (i32(si) * valDM) >> 5;
						//^075F:208B
						U16 bp0a = bp04->ArmorClass +(RAND() & 31) +bp0c;
						//^075F:20A4
						di = si = (RAND() & 31) +si -bp0a;
						//^075F:20B5
						if (di > 1)
							//^075F:20B8
							goto _2112;
					}
					//^075F:20BA
					si = RAND02();
					//^075F:20C1
					if (si != 0) {
						//^075F:20C8
						si++;
						//^075F:20C9
						di = (RAND() & 15) +di;
						//^075F:20D7
						if (di > 0 || RAND01() != 0) {
							//^075F:20E6
							si = RAND02() +si;
							//^075F:20F1
							if (RAND02() == 0) {
								//^075F:20FA
								si += max_value(0, (RAND() & 15) +di);
							}
						}
						//^075F:2112
_2112:
						si >>= 1;
						//^075F:2114
						si = RAND16(si) +RAND02() +si;
						//^075F:2128
						si = RAND16(si) +si;
						//^075F:2135
						si >>= 2;
						//^075F:2138
						si = RAND02() +si +1;
						//^075F:2142
						if ((RAND() & 63) < QUERY_PLAYER_SKILL_LV(player, valSk, 1)) {
							//^075F:2160
							si += si +10;
						}
						//^075F:2167
						//SPX: Something to do with POISON
						di = QUERY_GDAT_DBSPEC_WORD_VALUE(ref->Possess(glbSelectedHandAction), GDAT_ITEM_STATS_POISONOUS);	// 13
						//^075F:2181
						if (di != 0 && (RAND() & 31) < si) {
							//^075F:2191
							// Add poison damage
							si = APPLY_CREATURE_POISON_RESISTANCE(rlEnemy, di) +si;
						}
						//^075F:21A2
						ADJUST_SKILLS(player, valSk, ((bp04->w22_8_b() * si) >> 4) +3);
						//^075F:21CA
						ADJUST_STAMINA(player, RAND02() +4);
						//^075F:21D2
						goto _21dd;
					}
				}
			}
		}
	}
	//^075F:21D4
	si = 0;
	//^075F:21D6
	ADJUST_STAMINA(player, RAND01() +2);
	//^075F:21E8
_21dd:
	ATTACK_CREATURE(rlEnemy, xx, yy, 0x6002, 0x5a, si);
	//^075F:21FF
	return si;
}

//^2759:14C8
U16 SkWinCore::WIELD_WEAPON(U16 player, U16 valPa, U16 xx, U16 yy, U16 valSk, U16 valAt)
{
	// returns 0 if no target, 1 if hits a creature.

	//^2759:14C8
	ENTER(4);
	//^2759:14CE
	Champion *champion = &glbChampionSquad[player];	//*bp04
	//^2759:14DF
	if (glbObjectID_4976_534c == OBJECT_NULL)
		//^2759:14E6
		//^2759:15CA
		return 0;
	//^2759:14E9
	if (GET_CREATURE_AT(xx, yy) == OBJECT_NULL)
		//^2759:14FB
		//^2759:15CA
		return 0;
	//^2759:14FE
	U16 di = champion->playerPos();
	//^2759:1509
	U16 si;
	switch ((di +4 -champion->playerDir()) & 3) {
		case 2:
			//^2759:1526
			si = 3;
			//^2759:1529
			goto _152e;

		case 3:
			//^2759:152B
			si = 1;
			//^2759:152E
_152e:
			if (GET_PLAYER_AT_POSITION((di + si) & 3) != -1) {
				//^2759:1541
				glbChampionAttackDamage = ATTACK_FAILURE_NOT_FRONT; // -1
				//^2759:1547
				glbSomeChampionPanelFlag = max_value(1, 8 - UPDATE_GLOB_VAR(66, 1, 3));
				//^2759:1567
				//^2759:15CA
				return 0;
			}

			break;
	}
	//^2759:1569
	if (valPa == 1) {
		//^2759:156F
		if ((QUERY_CREATURE_AI_SPEC_FLAGS(glbObjectID_4976_534c) & 0x0020) == 0)
			//^2759:157C
			//^2759:15CA
			return 0;
	}
	//^2759:157E
	si = QUERY_CUR_CMDSTR_ENTRY(CnPB);
	//^2759:1587
	di = QUERY_CUR_CMDSTR_ENTRY(CnDM);
	//^2759:1590
	if (QUERY_CUR_CMDSTR_ENTRY(CnHN) != 0)
		//^2759:159B
		si |= 0x8000;	// Hurt non material
	//^2759:159F
	glbChampionAttackDamage = CALC_PLAYER_ATTACK_DAMAGE(champion, player, glbObjectID_4976_534c, xx, yy, si, di, valSk, valAt);
	//^2759:15C5
	return 1;
}


//^2C1D:0E57
i16 SkWinCore::STAMINA_ADJUSTED_ATTR(Champion *ref, i16 quantity)
{
	// CSBwinSimilarity: TAG0163c8,StaminaAdjustedAttribute

	//^2C1D:0E57
	ENTER(2);
	//^2C1D:0E5D
	i16 si = quantity;
	//^2C1D:0E60
	i16 bp02 = ref->curStamina();
	//^2C1D:0E6A
	U16 di = ref->maxStamina() >> 1;
	//^2C1D:0E72
	if (bp02 < di) {
		//^2C1D:0E76
		si >>= 1;
		//^2C1D:0E78
		return (i32(si) * i32(bp02)) / i32(di) +si;
	}
	//^2C1D:0E9C
	return si;
}

//^2C1D:1DE2
//SPX: About shooting?
U16 SkWinCore::_2c1d_1de2(U16 xx, i16 yy, U16 zz)
{
	//^2C1D:1DE2
	ENTER(16);
	//^2C1D:1DE8
	U16 di = xx;
	//^2C1D:1DEB
	Champion *bp04 = &glbChampionSquad[di];
	//^2C1D:1DFB
	U16 bp0e = 0;
	//^2C1D:1E00
	ObjectID bp0c ;
	ObjectID si ;
	if (yy < 0) {
		//^2C1D:1E06
		if (glbLeaderHandPossession.object == OBJECT_NULL)
			//^2C1D:1E0D
			return 0;
		//^2C1D:1E12
        bp0c = bp04->Possess(1);
		//^2C1D:1E1D
		si = REMOVE_OBJECT_FROM_HAND();
		//^2C1D:1E23
		bp04->Possess(1, si);
		//^2C1D:1E2B
		yy = 1;
		//^2C1D:1E30
		bp0e = 1;
	}
	//^2C1D:1E35
	i16 bp0a = COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(di, yy, 10);
	//^2C1D:1E45
	if (bp0e != 0) {
		//^2C1D:1E4B
		bp04->Possess(1, bp0c);
	}
	else {
		//^2C1D:1E58
		si = REMOVE_POSSESSION(di, yy);
		//^2C1D:1E64
		if (si == OBJECT_NULL)
			//^2C1D:1E69
			//^2C1D:1E0D
			return 0;
	}
	//^2C1D:1E6B
	QUEUE_NOISE_GEN2(
		QUERY_CLS1_FROM_RECORD(si),
		QUERY_CLS2_FROM_RECORD(si),
		SOUND_STD_THROW,
		0xfe,
		glbPlayerPosX,
		glbPlayerPosY,
		1,
		0x6e,
		0x80);
	//^2C1D:1E98
	ADJUST_STAMINA(di, _2c1d_0e23(si));
	//^2C1D:1EA6
	ADJUST_HAND_COOLDOWN(di, 4, yy);
	//^2C1D:1EB4
	U16 bp08 = 8;
	U16 bp06 = 1;
	//^2C1D:1EBE
	U16 bp10 = QUERY_GDAT_DBSPEC_WORD_VALUE(si, 9);
	//^2C1D:1ECB
	if (bp10 != 0) {
		//^2C1D:1ECF
		bp08 = (bp10 >> 2) +bp08 +4;
		bp06 = bp10;
	}
	//^2C1D:1EE1
	ADJUST_SKILLS(di, 10, bp08);
	//^2C1D:1EEE
	bp0a += bp06;
	//^2C1D:1EF4
	bp06 = QUERY_PLAYER_SKILL_LV(di, SKILL_NINJA_THROW, 1);
	//^2C1D:1F03
	bp0a = RAND16((bp0a >> 2) +8) +bp06 +bp0a;
	//^2C1D:1F1C
	bp0a = min_value(bp0a, 255);
	//^2C1D:1F2C
	bp08 = BETWEEN_VALUE(40, (RAND() & 31) + (bp06 << 3), 200);
	//^2C1D:1F4D
	bp10 = QUERY_GDAT_DBSPEC_WORD_VALUE(si, 12);
	//^2C1D:1F5A
	bp06 = (bp10 != 0) ? bp06 : max_value(5, 11 -bp06);
	//^2C1D:1F70
	//^2C1D:1F73
	SHOOT_ITEM(si, glbPlayerPosX, glbPlayerPosY, (glbPlayerDir +zz) & 3, glbPlayerDir, bp0a, bp08, bp06);

	SkD((DLV_TWEET, "Tweet: You (x:%d, y:%d, map:%d) have thrown %s! \n"
		, glbPlayerPosX, glbPlayerPosY, glbCurrentMapIndex, static_cast<LPCSTR>(getRecordNameOf(si))
		));

	//^2C1D:1F9E
	glbPlayerThrowCounter = 4;
	//^2C1D:1FA4
	_4976_4c0c = glbPlayerDir;
	//^2C1D:1FAA
	return 1;
}


//^2C1D:1BB5
// SPX: _2c1d_1bb5 replaced by CURE_POISON
void SkWinCore::CURE_POISON(U16 player)
{
	//^2C1D:1BB5
	ENTER(4);
	//^2C1D:1BBB
	i16 di = player;
	//^2C1D:1BBE
	if (di != -1) {
		U16 si = 0;
		Timer *bp04 = glbTimersTable;
		for (; si < glbTimersActiveCount; bp04++, si++) {
			//^2C1D:1BD4
			if (bp04->TimerType() == ttyPoison) {
				//^2C1D:1BDE  
				if (bp04->actor == di) {
					//^2C1D:1BE8  
					DELETE_TIMER(si);
				}
			}
			//^2C1D:1BEF  
		}
		//^2C1D:1BFA  
		glbChampionSquad[di].PoisonValue = 0;
	}
	//^2C1D:1C08
	return;
}



//^24A5:10FB
void SkWinCore::PLAYER_CONSUME_OBJECT(U16 player, ObjectID rlConsume, i16 possess)
{
	// SPX: Default values
	U16 PotionType = 0;
	U16 WaterValue = WATER_DEFAULT_ITEM_VALUE;	// 800


	//^24A5:10FB
	ENTER(18);
	//^24A5:1101
	ObjectID di = rlConsume;
	//^24A5:1104
	U16 bp12 = 1;
	//^24A5:1109
	if (di != OBJECT_NULL) {
		//^24A5:110E
		bp12 = 0;
	}
	//^24A5:1115
	else if (glbLeaderHandPossession.object == OBJECT_NULL) {
		//^24A5:111C
		FIRE_MOUSE_SET_CAPTURE();
		//^24A5:1121
		_4976_4bfe = 1;
		//^24A5:1127
		//^24A5:113B
		if ((_4976_5dae.rc4.cy & 2) == 0) {
			//^24A5:1143
			FIRE_MOUSE_RELEASE_CAPTURE();
			//^24A5:1148
			_4976_4bfe = 0;
			//^24A5:114E
			return;
		}
		//^24A5:1151
		FIRE_HIDE_MOUSE_CURSOR();
		//^24A5:1156
		glbMouseVisibility = 1;
		//^24A5:115C
		return;
	}
	else {
		//^24A5:115F
		player = glbChampionInventory -1;
		//^24A5:1166
		di = glbLeaderHandPossession.object;
		//^24A5:116A
		possess = -1;
	}
	//^24A5:116F
	if (glbNextChampionNumber != 0)
		//^24A5:1176
		return;
	//^24A5:1179
	Champion *champion = &glbChampionSquad[player];
	//^24A5:118A
	U16 bp10 = QUERY_GDAT_FOOD_VALUE_FROM_RECORD(di);
	//^24A5:1194
	U16 bp0c;
	U16 si;
	Potion *bp08;
	if (bp10 != 0) {
		//^24A5:119B
		if (bp12 != 0) {
			//^24A5:11A1
			FIRE_HIDE_MOUSE_CURSOR();
			//^24A5:11A6
			REMOVE_OBJECT_FROM_HAND();
			//^24A5:11AB
			for (bp0c = 5; (--bp0c) != 0; ) {
				//^24A5:11B2
				DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, ((bp0c & 1) != 0) ? 0x25 : 0x26, 545, -1);	// mouth icon
				//^24A5:11D1
				CHANGE_VIEWPORT_TO_INVENTORY(0);
				//^24A5:11D9
				SLEEP_SEVERAL_TIME(8);
				//^24A5:11E1
			}
			//^24A5:11EB
			FIRE_SHOW_MOUSE_CURSOR();
		}
		//^24A5:11F0
		//SPX: Eating food item
		champion->curFood(min_value(champion->curFood() +bp10, FOOD_MAX));
		//^24A5:120C
		PROCESS_ITEM_BONUS(player, di, possess, 2);
		//^24A5:121D
		DEALLOC_RECORD(di);
		//^24A5:1224
		if (possess != -1) {
			//^24A5:122D
			champion->Possess(possess, OBJECT_NULL);
		}
		//^24A5:123E
		goto _14a9;
	}
	//^24A5:1241
	U16 bp0a;
	if (IS_MISCITEM_DRINK_WATER(di) != 0) {
		// SPX: This assume that any misc item gives 800 water per drink
		WaterValue = WATER_DEFAULT_ITEM_VALUE;

		// SPX: Custom Water value
		if (SkCodeParam::bUseDM2ExtendedMode)
			WaterValue = QUERY_GDAT_WATER_VALUE_FROM_RECORD(di);

		//^24A5:124B
		champion->curWater(min_value(champion->curWater() +WaterValue, WATER_MAX));
		//^24A5:1267
		if (possess != -1) {
			//^24A5:1270
			champion->Possess(possess, di);
		}
		//^24A5:127C
		goto _14a9;
	}
	//^24A5:127F
	bp0a = di.DBType();
	//^24A5:128A
	if (bp0a == dbPotion) {
		//^24A5:1292
		bp08 = GET_ADDRESS_OF_RECORD8(di);
		//^24A5:129F
		si = bp08->PotionPower();
		//^24A5:12AB
		// SPX: Degressive divisor => bp0c = 08 (lowest power) to 02 (highest power)
		bp0c = ((511 -si) / (((si +1) >> 3) +32)) >> 1;
		//^24A5:12C2
		si = (si / 25) +8;
		//^24A5:12D0
		U16 bp0e;
		switch (bp08->PotionType())
		{
			//// SPX, note: there is finally room to have potions that boost antimagic and antifire.
			/// We could think of the potion with FUL symbol to boost antifire.
			/// And the potion with ZO symbol would boost antimagic
			default:
				//^24A5:12E3
				return;

			case  6:			// ”é–ò{ROS}
				//^24A5:12ED
				BOOST_ATTRIBUTE(champion, ATTRIBUTE_DEXTERITY, si);	// 2 = DEX
				//^24A5:12FE
				break;

			case  7:			// ”é–ò{KU}
				//^24A5:1301
				//^24A5:1318
				BOOST_ATTRIBUTE(champion, ATTRIBUTE_STRENGTH, bp08->PotionPower() / 35 +5);	// 1 = STR
				//^24A5:12FE
				break;

			case  8:			// ”é–ò{DAIN}
				//^24A5:131A
				//^24A5:131D
				BOOST_ATTRIBUTE(champion, ATTRIBUTE_WISDOM, si);	// 3 = WIS
				//^24A5:12FE
				break;

			case  9:			// ”é–ò{NETA}
				//^24A5:131F
				//^24A5:1322
				BOOST_ATTRIBUTE(champion, ATTRIBUTE_VITALITY, si);	// 4 = DAIN
				//^24A5:12FE
				break;

			case 10:			// ‘Î{VEN}”é–ò
				//^24A5:1324
				// SPX: immediate cure? not like in PC DM2 where poisons have different strengths?
				CURE_POISON(player);
				//^24A5:132D
				break;

			case 11:			// ”é–ò{MON}	Regain stamina
				//^24A5:1330
				champion->curStamina(champion->curStamina() +min_value(champion->maxStamina() -champion->curStamina(),champion->maxStamina() / bp0c));
				//^24A5:1354
				break;

			case 12:			// ”é–ò{YA}		Cast party shield
				//^24A5:1357
				si += (si >> 1);
				//^24A5:135D
				PROCEED_ENCHANTMENT_SELF(1 << player, ENCHANTMENT_PARTY_SHIELD, si, si * si);
				//^24A5:1373
				break;

			case 13:			// ”é–ò{EE}		Regain mana
				//^24A5:1375
				bp0a = min_value(champion->curMP() +si +(si -8), MAXMP);
				//^24A5:1393
				if (champion->maxMP() < bp0a) {
					//^24A5:139F
					bp0a -= (bp0a -max_value(champion->curMP(), champion->maxMP())) >> 1;
				}
				//^24A5:13B8
				champion->curMP(bp0a);
				//^24A5:13C2
				break;

			case 14:			// ”é–ò{VI}		Regain health
				//^24A5:13C5
				bp0a = bp08->PotionPower() / 42;
				//^24A5:13E3
				champion->curHP(champion->curHP() + (champion->maxHP() / bp0c));
				//^24A5:13FB
				bp0e = champion->bodyFlag;
				//^24A5:1402
				if (bp0e == 0)
					//^24A5:1404
					break;
				//^24A5:1406
				bp0c = 10;
				do {
					//^24A5:140B
					for (si = 0; si < bp0a; si++) {
						//^24A5:140F
						champion->bodyFlag &= RAND();
						//^24A5:141B
					}
					//^24A5:1421
					bp0a = 1;
					//^24A5:1426
					if (champion->bodyFlag != bp0e)
						//^24A5:1430
						break;
					//^24A5:1432
					bp0c--;
					//^24A5:1435
				} while (bp0c != 0);
				//^24A5:143C
				break;

			case 15:			// WATER FLASK
				WaterValue = WATER_FLASK_VALUE;	// Default value 1600

				if (SkCodeParam::bUseDM2ExtendedMode)
					WaterValue = QUERY_GDAT_WATER_VALUE_FROM_RECORD(di);
				
				//^24A5:143E
				champion->curWater(min_value(champion->curWater() +WaterValue, WATER_MAX));

				break;
		}
		//^24A5:145A
		if (possess == -1)
			//^24A5:1460
			REMOVE_OBJECT_FROM_HAND();
		//^24A5:1465
		DEALLOC_RECORD(di);
		//^24A5:146C
		di = ALLOC_NEW_RECORD(dbPotion);
		//^24A5:1476
		if (di == OBJECT_NULL)
			//^24A5:147B
			return;
		//^24A5:147E
		SET_ITEMTYPE(di, 0x14);
		//^24A5:1488
		if (possess == -1) {
			//^24A5:148E
			TAKE_OBJECT(di, 0);
		}
		else {
			//^24A5:149A
			champion->Possess(possess, di);
		}
		//^24A5:14A9
_14a9:
		if (champion->curStamina() > champion->maxStamina()) {
			//^24A5:14B6
			champion->curStamina(champion->maxStamina());
		}
		//^24A5:14BE
		if (champion->curHP() > champion->maxHP()) {
			//^24A5:14CB
			champion->curHP(champion->maxHP());
		}
		//^24A5:14D3
		// SPX: Sound made when eating/drinking
		// 0x16 = Champion category, 0x83 = sound for eat/drink, 0xFE = default index
		// So there could be a sound for eat and one for drink
		QUEUE_NOISE_GEN2(GDAT_CATEGORY_CHAMPIONS, champion->HeroType(), SOUND_CHAMPION_EAT_DRINK, 0xfe, glbPlayerPosX, glbPlayerPosY, 0, 0x96, 0x80);
		//^24A5:14FC
		champion->heroFlag |= CHAMPION_FLAG_3800;	// 0x3800
		//^24A5:1505
		if (possess != -1) {
			//^24A5:150B
			champion->heroFlag |= CHAMPION_FLAG_4000;	// 0x4000
			//^24A5:1511
			CALC_PLAYER_WEIGHT(player);
		}
	}
	//^24A5:151A
	return;
}


//  SPX: Taken from CSBWin : Character.cpp : TAG0139a2 / DecodeCharacterValue
U16 DECODE_CHARACTER_VALUE(U8* buf,i16 num, bool allowTruncation = false)
{
  U16 result = 0;
  do
  {
    result <<= 4;
    if ( (*buf < 'A') || (*buf > 'P') ) 
    {
      if (allowTruncation && (*buf == 0)) return result;
      else
        return -1;
    };
    result += *(buf++) - 'A';
    num--;
  } while (num!=0);
  return result;

}

//^2F3F:009A
void SkWinCore::REVIVE_PLAYER(X16 heroType, X16 player, X16 dir)
{
	//CSBWin similarities : Character.cpp:TAG0139be/AddCharacter
	//^2F3F:009A
	ENTER(148);
	//^2F3F:00A0
	Champion *champion = &glbChampionSquad[player = glbChampionsCount];	//*bp04
	champion->HeroType(U8(heroType));
	champion->handCommand[1] = champion->handCommand[0] = 0xff;
	champion->timerIndex = TIMER_NONE;
	champion->playerDir(U8(dir));
	X16 bp0e;
	for (bp0e = 0; GET_PLAYER_AT_POSITION((bp0e + glbPlayerDir) & 3) != -1; bp0e++);
	//^2F3F:00F6
	champion->playerPos(bp0e + glbPlayerDir);
	champion->direction = U8(glbPlayerDir);
	for (bp0e = 0; bp0e < INVENTORY_MAX_SLOT; bp0e++)
		champion->Possess(bp0e, OBJECT_NULL);
	//^2F3F:012E
	U8 bp0094[0x80];
	// SPX: get HERO NAME
	U8 *bp0c = QUERY_GDAT_TEXT(GDAT_CATEGORY_CHAMPIONS, U8(heroType), 0x18, bp0094);
	X16 bp10;
	for (bp0e = 0; (bp10 = i8(*(bp0c++))) != 0x20 && bp10 != 0 && bp0e < 7; bp0e++) {
		//^2F3F:0151
		champion->firstName[bp0e] = U8(bp10);
		//^2F3F:015D
	}
	//^2F3F:017E
	champion->firstName[bp0e] = 0;
	bp0e = 0;
	if (bp10 != 0) {
		//^2F3F:0193
		while (true) {
			bp10 = *(bp0c++);
			if (bp10 == 0)
				break;
			if ((champion->lastName[bp0e++] = U8(bp10)) == 0x13)
				break;
		}
	}
	//^2F3F:01BC
	champion->lastName[bp0e] = 0;
//DEBUG_DUMP_ULP();
	skhero *bp08 = reinterpret_cast<skhero *>(QUERY_GDAT_ENTRY_DATA_PTR(GDAT_CATEGORY_CHAMPIONS, U8(heroType), dt08, 0x00));
#if DM2_EXTENDED_MODE == 1	// TODO To be replaced with fixedmode + checkmem
	if (bp08 == NULL)
		RAISE_SYSERR(SYSTEM_ERROR__NO_PLAYER_DATA);
#endif
	champion->maxHP(bp08->maxHP());
	champion->curHP(bp08->maxHP());
	champion->maxStamina(bp08->maxStamina());
	champion->curStamina(bp08->maxStamina());
	champion->maxMP(bp08->maxMP());
	champion->curMP(bp08->maxMP());
	for (bp0e = 0; bp0e <= 6; bp0e++) {
		//^2F3F:0220
		champion->attributes[bp0e][ATTRIBUTE_MAX] = champion->attributes[bp0e][ATTRIBUTE_CURRENT] = U8(max_value(0x1e, bp08->herodata[bp0e +3]));
		champion->attributesEnhanced[bp0e] = 0;
		//^2F3F:0260
	}
	//^2F3F:0269
	for (bp0e = 4; bp0e <= 19; bp0e++) {
		//^2F3F:0270
		bp10 = bp08->herodata[bp0e +10 -4];
		champion->skills[bp0e] = (bp10 != 0) ? (U32(0x40) << bp10) : 0;
		//^2F3F:02B0
	}
	//^2F3F:02B9
	for (bp0e = 0; bp0e <= 3; bp0e++) {
		//^2F3F:02C0
		X32 bp14 = 0;
		bp10 = (bp0e +1) << 2;
		U16 si;
		for (si = 0; si < 4; si++) {
			bp14 += champion->skills[bp10 +si];
		}
		//^2F3F:02F9
		champion->skills[bp0e] = bp14;
		//^2F3F:0312
	}
	//^2F3F:031B
	champion->curFood((RAND() & 255) + START_BASE_FOOD);
	champion->curWater((RAND() & 255) + START_BASE_WATER);
	//^2F3F:033F

	// SPX: DM1 Compatibility code -- check TEXT at player position which should contain CHAMPION stats
	if (SkCodeParam::bDM1Mode) // maybe this could be extendable for DM2 anyway
	{
		ObjectID xObject = OBJECT_END_MARKER;
		U8 iPlayerPosX = glbPlayerPosX;
		U8 iPlayerPosY = glbPlayerPosY;
		char sHeroInfo[] =
			"BOGUS\012ILLEGAL HERO\012\012M\012AABBAABBAABB\012ABABABABABABAB\012CCCCCCCCCCCCCCCC";

		for (xObject = GET_TILE_RECORD_LINK(iPlayerPosX, iPlayerPosY); xObject != OBJECT_END_MARKER; xObject = GET_NEXT_RECORD_LINK(xObject)) {
			if (xObject.DBType() == dbText)
				break;
		}
		if (xObject != OBJECT_END_MARKER) {
			U8* sHeroPtr = NULL;
			U8 sBuffer[32];
			int iStrLimit = 7;
			int iWriteIndex = 0;
			int iSkillIndex = 0;
			memset(sBuffer, 0, 32);
			QUERY_MESSAGE_TEXT((U8*)sHeroInfo, xObject, 0x8000);
			if (sHeroInfo[0] != 0)
			{
				U8 sGender = '?';
				U16 iStatValue = 0;
				sHeroPtr = (U8*) &sHeroInfo;
				// Read first name
				while (*sHeroPtr != 10 && iStrLimit >= 0)
				{
					sBuffer[iWriteIndex] = *sHeroPtr;
					sHeroPtr++;
					iStrLimit--;
					iWriteIndex++;
				}
				memset(champion->firstName, 0, 8);
				strcpy((char*)champion->firstName, (char*)sBuffer);

				// Read last name
				sHeroPtr++;
				memset(sBuffer, 0, 32);
				iStrLimit = 16;
				iWriteIndex = 0;
				while (*sHeroPtr != 10 && iStrLimit >= 0)
				{
					sBuffer[iWriteIndex] = *sHeroPtr;
					sHeroPtr++;
					iStrLimit--;
					iWriteIndex++;
				}
				iStrLimit=16;
				sHeroPtr++;
				while (*sHeroPtr != 10 && iStrLimit >= 0)
				{
					sBuffer[iWriteIndex] = *sHeroPtr;
					sHeroPtr++;
					iStrLimit--;
					iWriteIndex++;
				}
				memset(champion->lastName, 0, 16);
				//strcpy((char*)champion->lastName, (char*)sBuffer);
				strncpy((char*)champion->lastName, (char*)sBuffer, 16);

				// Read gender
				sHeroPtr+=1;
				sGender=*sHeroPtr;

				// Read basic stats
				sHeroPtr+=2;
				iStatValue = DECODE_CHARACTER_VALUE(sHeroPtr, 4);
				champion->maxHP(iStatValue);
				champion->curHP(iStatValue);
				sHeroPtr+=4;
				iStatValue = DECODE_CHARACTER_VALUE(sHeroPtr, 4);
				champion->maxStamina(iStatValue);
				champion->curStamina(iStatValue);
				sHeroPtr+=4;
				iStatValue = DECODE_CHARACTER_VALUE(sHeroPtr, 4);
				champion->maxMP(iStatValue);
				champion->curMP(iStatValue);
				sHeroPtr+=4;

				sHeroPtr++; // new line
				// Read attributes stats
				for (iSkillIndex = 0; iSkillIndex < 7; iSkillIndex++)
				{
					iStatValue = DECODE_CHARACTER_VALUE(sHeroPtr, 2);
					champion->attributes[iSkillIndex][0] = iStatValue;
					champion->attributes[iSkillIndex][1] = champion->attributes[iSkillIndex][0];
					sHeroPtr+=2;
				}

				sHeroPtr++; // new line
				// Read sub-skills levels
				for (iSkillIndex = 4; iSkillIndex < 20; iSkillIndex++)
				{
					X32 iXP = 0;
					iStatValue = DECODE_CHARACTER_VALUE(sHeroPtr, 1);
					iXP = 125 << iStatValue;
					champion->skills[iSkillIndex] = iXP;
					sHeroPtr++;
				}
				//-- Main skills level
				for (iSkillIndex = 0; iSkillIndex < 4; iSkillIndex++)
				{
					X32 iTotalMajorXP = 0;
					U16 iMainSkill = 0;
					iMainSkill = (iSkillIndex + 1) << 2;
					U16 iSubSkill;
					for (iSubSkill = 0; iSubSkill < 4; iSubSkill++) {
						iTotalMajorXP += champion->skills[iMainSkill + iSubSkill];
					}
					champion->skills[iSkillIndex] = iTotalMajorXP;
				}


			}
		}
	}
		

	// SPX: Like a debugging character, make it strong from the beginning!
	if (SkCodeParam::bUseSuperMode)
	{
		U16	statHP		= 800;
		U16	statStamina = 8500;
		U16	statMP		= 650;

		if (SkCodeParam::bUseDM2ExtendedMode)
		{
			statHP		= 2500;
			statStamina = 28000;
			statMP		= 1700;
			//PROCESS_POISON(player, 10);
			//PROCESS_PLAGUE(player, 10);
		}

		champion->maxHP(statHP);
		champion->curHP(champion->maxHP());
		champion->maxStamina(statStamina);
		champion->curStamina(champion->maxStamina());
		champion->maxMP(statMP);
		champion->curMP(champion->maxMP());
		for (int i = 0; i < SKILL_COUNT; i++)
			champion->skills[i] = SKILL_EXPERIENCE_BASE_LEVEL<<(SkLvlMasterLo-2);
	} // End super/debug block

	return;
}

//^2F3F:0343
// SPX: _2f3f_0343 renamed SELECT_CHAMPION. Called when clicking on "mirror/cell" or at the beginning for
// automatic selection of Thoram
X16 SkWinCore::SELECT_CHAMPION(U16 xx, U16 yy, U16 dir, U16 mm)
{
	//^2F3F:0343
	ENTER(10);
	//^2F3F:0349
	U16 iChampionNumber = 0;	// di
	if (glbLeaderHandPossession.object != OBJECT_NULL || (iChampionNumber = glbChampionsCount) >= MAX_CHAMPIONS)
		//^2F3F:035A
		return 0;
	//^2F3F:035F
	X16 iCurrentMap = glbCurrentMapIndex;	// bp08
	CHANGE_CURRENT_MAP_TO(mm);
	xx += glbXAxisDelta[dir];
	yy += glbYAxisDelta[dir];
	//^2F3F:0386
	ObjectID xObject; // si
	X16 iHeroType = 0; // bp0a
	for (xObject = GET_TILE_RECORD_LINK(xx, yy); xObject != OBJECT_END_MARKER; xObject = GET_NEXT_RECORD_LINK(xObject)) {
		//^2F3F:0395
		Actuator *refActuator; // bp04
		if (xObject.DBType() == dbActuator && (refActuator = GET_ADDRESS_OF_ACTU(xObject))->ActuatorType() == ACTUATOR_TYPE_RESURECTOR) { // 0x7E
			//^2F3F:03BE
			iHeroType = refActuator->ActuatorData();
			break;
		}
		// SPX: Add for 0x7F :Activator, champion mirror
		else if (xObject.DBType() == dbActuator && (refActuator = GET_ADDRESS_OF_ACTU(xObject))->ActuatorType() == ACTUATOR_TYPE_CHAMPION_MIRROR) { // 0x7F
			//^2F3F:03BE
			iHeroType = refActuator->ActuatorData();
			break;
		}
		//^2F3F:03CD
	}
	//^2F3F:03DB
	xx += glbXAxisDelta[(dir +2) & 3];
	yy += glbYAxisDelta[(dir +2) & 3];
	REVIVE_PLAYER(iHeroType, iChampionNumber, dir);
	if (iChampionNumber != 0) {
		//^2F3F:0412
		DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
		UPDATE_RIGHT_PANEL(0);
	}
	//^2F3F:041F
	glbNextChampionNumber = iChampionNumber + 1;
	glbChampionsCount++;
	if (iChampionNumber == 0)	// First champion, then select it as leader
		//^2F3F:042D
		SELECT_CHAMPION_LEADER(0);
	//^2F3F:0435
	X16 iWallDir = (dir +2) & 3; // bp06
	xx += glbXAxisDelta[dir];
	yy += glbYAxisDelta[dir];
	// Take items from wall and add them to player
	for (xObject = GET_TILE_RECORD_LINK(xx, yy); xObject != OBJECT_END_MARKER; xObject = GET_NEXT_RECORD_LINK(xObject)) {
		//^2F3F:0467
		if (xObject.DBType() > dbActuator && xObject.Dir() == iWallDir) {
			//^2F3F:047E
			ADD_ITEM_TO_PLAYER(iChampionNumber, xObject);
		}
		//^2F3F:0486
	}
	//^2F3F:0494
	xx += glbXAxisDelta[(dir +2) & 3];
	yy += glbYAxisDelta[(dir +2) & 3];
	if (_4976_404b == 0) {
		//^2F3F:04BF
		INTERFACE_CHAMPION(iChampionNumber);
		_1031_0541(7);
		_38c8_0002();
	}
	//^2F3F:04D3
	CHANGE_CURRENT_MAP_TO(iCurrentMap);
	CALC_PLAYER_WEIGHT(iChampionNumber);
	//^2F3F:04E3
	return iChampionNumber + 1;
}


//^2E62:0CD8
// SPX: _2e62_0cd8 renamed SET_PARTY_HERO_FLAG
void SkWinCore::SET_PARTY_HERO_FLAG(U16 flagvalue)
{
	//^2E62:0CD8
	ENTER(0);
	//^2E62:0CDC
	for (U16 si = 0; si < glbChampionsCount; si++) {
		//^2E62:0CE0
		glbChampionSquad[si].heroFlag |= flagvalue;
	}
	//^2E62:0CF7
	return;
}

//^2F3F:0789
// SPX: _2f3f_0789 renamed SEARCH_STARTER_CHAMPION
void SkWinCore::SEARCH_STARTER_CHAMPION() // _2f3f_0789
{
	//printf("SEARCH_STARTER_CHAMPION enter\n"); getch();
	//^2F3F:0789
	ENTER(8);
	//^2F3F:078F
	if (glbSpecialScreen == 0) {
		//^2F3F:0799
		ObjectID di = glbLeaderHandPossession.object;
		//^2F3F:079E
		if (di == OBJECT_NULL) {
			//^2F3F:07A3
			_4976_57de = 0xff;
			//^2F3F:07A8
			_443c_0434();
		}
		else {
			i16 si = glbChampionLeader;
			//^2F3F:07B3
			glbChampionLeader = -1;
			//^2F3F:07B9
			TAKE_OBJECT(di, 1);
			//^2F3F:07C3
			glbChampionLeader = si;
		}
		//^2F3F:07C7
		Champion *champion = glbChampionSquad;	//*bp04
		//^2F3F:07CF
		U16 si;
		for (si = 0; si < glbChampionsCount; si++, champion++) {
			//^2F3F:07D36
			champion->heroFlag &= 1023;	// 0x3FF
			//^2F3F:07DC
			WRITE_UI8(_4976_3de2,+si,0xff);
			//^2F3F:07E1
			glbChampionAlive[si] = 0;
			//^2F3F:07EB
			_4976_3dde[RCJ(4,si)] = 0xff;
			//^2F3F:07F0
			glbChampionHandCoolingDown[si][1] = 0;
			//^2F3F:07FE
			_4976_3de6[RCJ(4,si)][1] = 0xffff;
			//^2F3F:0809
			glbChampionHandCoolingDown[si][0] = 0;
			//^2F3F:0817
			_4976_3de6[RCJ(4,si)][0] = 0xffff;
			//^2F3F:0822
			glbChampionHandCoolingDown[si][2] = 0;
			//^2F3F:0830
		}
		//^2F3F:083C
		SET_PARTY_HERO_FLAG(0x4000);
		//^2F3F:0845
		si = glbChampionLeader;
		//^2F3F:084A
		if (si == 0xffff)
			//^2F3F:084F
			return;
		//^2F3F:0852
		glbChampionLeader = -1;
		//^2F3F:0858
		SELECT_CHAMPION_LEADER(si);
		//^2F3F:085F
		return;
	}
	//printf("glbLeaderHandPossession\n"); getch();
	//^2F3F:0862
	glbLeaderHandPossession.object = OBJECT_NULL;
	_4976_57de = 0xff;
	//^2F3F:086D
	// SPX: DM2 will check a RESURECTOR at 0,0 (for TORHAM), while DM/TQ expects one at 1,0 for THERON.

	// DM2 Original code
	//printf("Search Torham\n"); getch();
	ObjectID di = OBJECT_NULL;
	for (di = GET_TILE_RECORD_LINK(0, 0); di != OBJECT_END_MARKER; di = GET_NEXT_RECORD_LINK(di)) {
		//^2F3F:087B
		if (di.DBType() == dbActuator) {
			//^2F3F:0888
			Actuator *bp08 = GET_ADDRESS_OF_ACTU(di);
			//^2F3F:0895
			if (bp08->ActuatorType() == ACTUATOR_TYPE_RESURECTOR) { // 0x007E
				//^2F3F:08A4
				_4976_404b = 1;
				//^2F3F:08AA
				// SPX: Automatic selection of champion (Thoram)
				SELECT_CHAMPION(0, 1, DIR_NORTH, glbPlayerMap);	// player is imaginarily at 0,1 facing north
				//^2F3F:08BB
				_2f3f_04ea_CHAMPION(0, 1, DIR_NORTH, glbPlayerMap, UI_EVENTCODE_REVIVE_CHAMPION);
				//^2F3F:08CF
				_4976_404b = 0;
				//^2F3F:08D5
				glbChampionSquad[0].playerDir(U8(glbPlayerDir));
				//^2F3F:08DB
				glbChampionSquad[0].playerPos(U8(glbPlayerDir));
				//^2F3F:08E1
				SET_PARTY_HERO_FLAG(0x4000);
				//^2F3F:08EA
				SELECT_CHAMPION_LEADER(0);
				//^2F3F:08F2
				return;
			}
		}
		//^2F3F:08F4
	}
	//^2F3F:0905

	//printf("Search DM / TQ\n"); getch();
	// SPX: Second loop to check for DM/TQ at 1,0
	if (SkCodeParam::bDM1Mode == true || SkCodeParam::bDM1TQMode == true) {
		for (di = GET_TILE_RECORD_LINK(1, 0); di != OBJECT_END_MARKER; di = GET_NEXT_RECORD_LINK(di)) {
			if (di.DBType() == dbActuator) {
				Actuator *bp08 = GET_ADDRESS_OF_ACTU(di);
				if (bp08->ActuatorType() == ACTUATOR_TYPE_CHAMPION_MIRROR) { // 0x007F
					_4976_404b = 1;
					SELECT_CHAMPION(0, 0, DIR_EAST, glbPlayerMap);	// player is really at 0,0 facing east
					_2f3f_04ea_CHAMPION(0, 0, DIR_EAST, glbPlayerMap, UI_EVENTCODE_REVIVE_CHAMPION);
					_4976_404b = 0;
					glbChampionSquad[0].playerDir(U8(glbPlayerDir));
					glbChampionSquad[0].playerPos(U8(glbPlayerDir));
					SET_PARTY_HERO_FLAG(0x4000);
					SELECT_CHAMPION_LEADER(0);
					return;
				}
			}
		}
	}
	//printf("SEARCH_STARTER_CHAMPION_CUSTOM_MODES\n"); getch();
	INIT_CHAMPIONS_CUSTOM_MODES();	// SPX added for special custom mode init/start

	return;
}


//^443C:0662
// SPX: _443c_0662 renamed CHAMPION_SQUAD_RECOMPUTE_POSITION
void SkWinCore::CHAMPION_SQUAD_RECOMPUTE_POSITION()
{
	//^443C:0662
	if (_4976_5dbc != 0) {
		//^443C:066C
		CHANGE_PLAYER_POS(_4976_5dba | 0x8000);
	}
	//^443C:0678
}


//^2C1D:1FD1
// SPX: _2c1d_1fd1 renamed GET_CHAMPION_SPECIAL_FORCE
X16 SkWinCore::GET_CHAMPION_SPECIAL_FORCE(U16 player)
{
	//^2C1D:1FD1
	ENTER(0);
	//^2C1D:1FD5
	U16 si = player;
	if (glbChampionSquad[si].curHP() == 0)
		return 0;
	//^2C1D:1FEC
	if (((GET_PLAYER_WEIGHT(si) / 10) + (glbChampionSquad[si].heroFlag & CHAMPION_FLAG_0010)) != 0)	// 0x10
		//^2C1D:2012
		return 50;
	//^2C1D:2017
	return 40;
	//^2C1D:201A
}

//^2C1D:201D
// SPX: _2c1d_201d renamed GET_PARTY_SPECIAL_FORCE
Bit16u SkWinCore::GET_PARTY_SPECIAL_FORCE()
{
	//^2C1D:201D
	ENTER(0);
	//^2C1D:2022
	U16 di = 0;
	U16 si = 0;
	for (; si < glbChampionsCount; si++) {
		di += GET_CHAMPION_SPECIAL_FORCE(si);
	}
	//^2C1D:2037
	return di;
	//^2C1D:2039
}



//^2C1D:1B0F
// SPX: _2c1d_1b0f renamed PROCESS_POISON
void SkWinCore::PROCESS_POISON(i16 player, Bit16u yy) {
	
	if (SkCodeParam::bUseIngameDebug)
	{
		U8 message[64];
		sprintf((char*)message, "PLAYER %d HAS %d POISON COUNTERS.\n", player, yy);
		DISPLAY_HINT_TEXT(glbChampionColor[player], message);
	}
	//^2C1D:1B0F
	ENTER(14);
	//^2C1D:1B14
	X16 si = yy;
	if (player == -1)
		//^2C1D:1B1D
		return;
	//^2C1D:1B20
	if (player +1 == glbNextChampionNumber)
		//^2C1D:1B2A  
		return;
	//^2C1D:1B2D
	/*sk53b4*/Champion *bp04 = &glbChampionSquad[player];
	//^2C1D:1B3E
	WOUND_PLAYER(player, max_value(1, si >> 6), 0, 0);
	//^2C1D:1B5F
	bp04->heroFlag |= CHAMPION_FLAG_0800;	// 0x800
	bp04->heroFlag |= CHAMPION_FLAG_2000;	// 0x2000
	//^2C1D:1B6B
	si--;
	//^2C1D:1B6C
	if (si == 0)
		//^2C1D:1B70
		return;
	//^2C1D:1B72
	bp04->PoisonValue++;
	//^2C1D:1B76
	Timer bp0e;
	bp0e.TimerType(ttyPoison);
	bp0e.actor = (U8)player;
	bp0e.SetMap(glbPlayerMap);
	bp0e.SetTick(glbGameTick +0x24);
	bp0e.value = si;
	//^2C1D:1BA6  
	QUEUE_TIMER(&bp0e);
	//^2C1D:1BB2
	return;
}


//^2C1D:135D
X16 SkWinCore::_2c1d_135d(i16 play, U16 ww)
{
	//^2C1D:135D
	ENTER(8);
	//^2C1D:1363
	Champion *champion = &glbChampionSquad[play];	//*bp04
	U16 bp06 = ww & 0x8000;
	if (bp06 != 0)
		ww &= 0x7fff;
	X16 bp08 = 0;
	//^2C1D:138B
	i16 si;
	U16 di;
	for (si = 0; si <= 1; si++) {
		//^2C1D:138F
		// SPX: Checking armour value
		di = QUERY_GDAT_DBSPEC_WORD_VALUE(champion->Possess(si), GDAT_ITEM_STATS_ARMOR_CLASS);
		if ((di & 0x8000) != 0) {
			bp08 = (((_2c1d_132c(di, bp06) + COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(play, si, 7)) * _4976_3fc8[RCJ(6,ww)]) << ((si == ww) ? 4 : 5)) +bp08;
		}
		//^2C1D:13EF
	}
	//^2C1D:13F5
	si = RAND16((GET_PLAYER_ABILITY(champion, abVit, 0) >> 3) +1);
	if (bp06 != 0)
		si >>= 1;
	if (glbChampionSquad[play].enchantmentAura == ENCHANTMENT_PARTY_SHIELD) {	// == 2
		si += glbChampionSquad[play].enchantmentPower;
	}
	//^2C1D:143A
	si += champion->handDefenseClass[0] +champion->handDefenseClass[1] +bp08;
	if (ww > 1) {
		//^2C1D:1456
		di = QUERY_GDAT_DBSPEC_WORD_VALUE(champion->Possess(ww), GDAT_ITEM_STATS_ARMOR_CLASS);
		si = si +_2c1d_132c(di, bp06);
	}
	//^2C1D:147D
	if ((champion->bodyFlag & (1 << ww)) != 0) {
		si = si -(RAND02() +8);
	}
	//^2C1D:149C
	if (glbIsPlayerSleeping != 0)
		si >>= 1;
	return BETWEEN_VALUE(0, si >> 1, 0x64);
}


//^2C1D:0F2A
i16 SkWinCore::_2c1d_0f2a(Champion *ref, X16 ability, i16 tt)
{
	//^2C1D:0F2A
	ENTER(0);
	//^2C1D:0F2E
	i16 si = ATTRIBUTE_VALUE_THRESHOLD - GET_PLAYER_ABILITY(ref, ability, 0);	// 170 - Get_player_ab()
	//^2C1D:0F47
	return (si < 0x10) ? (tt >> 3) : (_0cd5_0176(tt, 7, si));
}

//^2C1D:18AA
Bit16u SkWinCore::WOUND_PLAYER(i16 play, i16 quantity, Bit16u ss, Bit16u tt)
{
	// CSBwinSimilarity: TAG017068,DamageCharacter

	//^2C1D:18AA
	ENTER(12);
	//^2C1D:18B0
	i16 si = quantity;
	if (play == -1)
		return 0;
	if (play +1 == glbNextChampionNumber)
		return 0;
	if (_4976_4c26 != 0)
		return 0;
	if (si <= 0)
		return 0;
	//^2C1D:18DA
	Champion *champion = &glbChampionSquad[play];
	//^2C1D:18EB
	if (champion->curHP() == 0)
		return 0;
	//^2C1D:18F8
	U16 bp0a = tt & 0x8000;
	tt &= 0x7fff;
	if (tt != 0) {
		//^2C1D:190F
		X16 bp08 = 0;
		i16 bp06 = 0;
		U16 di = 0;
		//^2C1D:191B
		for (; bp06 <= 5; bp06++) {
			//^2C1D:191D
			if (((1 << bp06) & ss) != 0) {
				bp08++;
				di += _2c1d_135d(play, ((tt == 4) ? 0x8000 : 0) | bp06);
			}
			//^2C1D:194B
		}
		//^2C1D:1954
		if (bp08 != 0) {
			//^2C1D:195A
			di = di / bp08;
		}
		//^2C1D:1963
		X16 bp0c = 0;
		bp06 = 0;
		//^2C1D:196D
		for (; bp06 <= 1; bp06++) {
			//^2C1D:196F
			if (champion->handCommand[bp06] == 1) {
				bp0c += champion->handDefenseClass[bp06];
			}
			//^2C1D:198A
		}
		//^2C1D:1993
		if (bp0c != 0 ) {
			//^2C1D:1999
			if (QUERY_PLAYER_SKILL_LV(play, SKILL_FIGHTER_PARRY, 1) +(bp0c >> 3) > (RAND() & 15)) {
				//^2C1D:19BD
				if (bp0a != 0 && (si = si -bp0c) <= 0)
					return 0;
				//^2C1D:19D1
				di += bp0c >> 2;
			}
		}
		//^2C1D:19D9
		switch (tt) {
		case 6://^19EC
			//^2C1D:19EC
			bp06 = 0x73 - GET_PLAYER_ABILITY(champion, 0, 0);
			if (bp06 <= 0)
				return 0;
			si = _0cd5_0176(si, 6, bp06);
			goto _1a91;
		case 5://^1A20
			//^2C1D:1A20
			si = _2c1d_0f2a(champion, abAntiMagic, si);
			if (champion->enchantmentAura == ENCHANTMENT_SPELL_SHIELD)	// == 1
				si -= champion->enchantmentPower;
			goto _1a91;
		case 1://^1A44
			//^2C1D:1A44
			si = _2c1d_0f2a(champion, abAntiFire, si);
			if (champion->enchantmentAura == ENCHANTMENT_FIRE_SHIELD)	// == 0
				si -= champion->enchantmentPower;
			break;
		case 2://^1A68
		case 8://^1A68
			//^2C1D:1A68
			di >>= 1;
			di += QUERY_PLAYER_SKILL_LV(play, SKILL_NINJA_GLOBAL, 1);
			break;
		case 3://^1A7A
		case 4://^1A7A
		case 7://^1A7A
			break;
		}
		//^2C1D:1A7A
		if (si <= 0)
			return 0;
		si = _0cd5_0176(si, 6, 0x82 -di);
		//^2C1D:1A91
_1a91:
		if (si <= 0)
			return 0;
		bp06 = _2c1d_0f2a(champion, abVit, (RAND() & 0x7f) +10);
		if (bp06 < si) {
			do {
				//^2C1D:1AB7
				glbChampionsBodyFlags[play] |= (1 << (RAND() & 7)) & ss;
				bp06 <<= 1;
				if (bp06 >= si)
					break;
			} while (bp06 != 0);
		}
		//^2C1D:1AE1
		if (glbIsPlayerSleeping != 0)
			RESUME_FROM_WAKE();
	}
	//^2C1D:1AEC
	glbChampionsPendingDamage[play] += si;
	return si;
}


//^2C1D:1C7C
void SkWinCore::ADJUST_STAMINA(Bit16u player, i16 drain)
{
	// CSBwinSimilarity: TAG01742a,AdjustStamina

	//^2C1D:1C7C
	ENTER(4);
	//^2C1D:1C82
	X16 di = player;
	if (di == 0xffff)
		return;
	//^2C1D:1C8A
	Champion *champion = &glbChampionSquad[di];	//*bp04
	champion->curStamina(champion->curStamina() -drain);
	i16 si = champion->curStamina();
	if (si <= 0) {
		//^2C1D:1CAE
		champion->curStamina(0);
		WOUND_PLAYER(di, (-si) >> 1, 0, 0);
	}
	//^2C1D:1CC7
	else if (champion->maxStamina() < si) {
		champion->curStamina(champion->maxStamina());
	}
	//^2C1D:1CD8
	if (ABS16(drain) >= 10) {
		//^2C1D:1CE6
		champion->heroFlag |= CHAMPION_FLAG_0800;	// 0x800
	}
	//^2C1D:1CEF
	return;
}
//^2C1D:203D
Bit16u SkWinCore::GET_PLAYER_WEIGHT(Bit16u player)
{
	// Returns player's weight. Also include weight of leader's hand item.

	//^2C1D:203D
	ENTER(0);
	//^2C1D:2042
	X16 di = player;
	if (glbChampionSquad[di].curHP() == 0)
		return 0;
	X16 si = glbChampionSquad[di].curWeight();
	if (di == glbChampionLeader) {
		//^2C1D:206C
		si += glbLeaderItemWeight;
	}
	return si;
}
//^2C1D:0FFC
Bit16u SkWinCore::MAX_LOAD(Champion *ref)
{
	// CSBwinSimilarity: TAG016508,MaxLoad

	//^2C1D:0FFC
	ENTER(0);
	//^2C1D:1001
	X16 si = (GET_PLAYER_ABILITY(ref, abStr, 0) << 3) +100;
	si = STAMINA_ADJUSTED_ATTR(ref, si);
	X16 di = ref->bodyFlag;
	if (di != 0) {
		si -= si << (((di & 0x10) != 0) ? 2 : 3);
	}
	//^2C1D:104B
	si += 9;
	si -= si % 10;
	return si;
}

//^2C1D:153F
void SkWinCore::_2c1d_153f(X16 xx)
{
	//^2C1D:153F
	ENTER(6);
	//^2C1D:1544
	Champion *bp04 = &glbChampionSquad[xx];
	i16 si = bp04->curHP();
	U16 bp06 = 0;
	bp04->curHP(0);
	//^2C1D:1562 
	if (bp04->playerPos() != 0xFF) {
		//^2C1D:1569 
		if (GET_PLAYER_AT_POSITION(bp04->playerPos()) == -1)
			goto _15a9;
	}
	//^2C1D:157A
	for (; GET_PLAYER_AT_POSITION((bp06 + glbPlayerDir) & 3) != -1; bp06++) {
		//^2C1D:1584  
	}
	//^2C1D:1599
	bp04->playerPos((bp06 + glbPlayerDir) & 3);
_15a9:
	bp04->playerDir(glbPlayerDir);
	bp04->curHP(si);
	//^2C1D:15B7
	return;
}

//^2F3F:06FE
// SPX: This should correspond to CSBWin [TAG014676] BringCharacterToLife(ui16 chIdx)
// _2f3f_06fe renamed BRING_CHAMPION_TO_LIFE
void SkWinCore::BRING_CHAMPION_TO_LIFE(X16 xx) { // TODO: Unr
//	if (!SkCodeParam::bUseFixedMode) {
		//^2F3F:06FE
		ENTER(4);
		//^2F3F:0703
		Champion *bp04 = &glbChampionSquad[xx];
		_2c1d_153f(xx);
		bp04->curWeight(0);
		//^2F3F:0727
		U16 si;
		for (si = 0; si < INVENTORY_MAX_SLOT; si++) {
			//^2F3F:072B
			bp04->Possess(si, oFFFF);
			//^2F3F:073B
		}
		//^2F3F:0741
		si = bp04->maxHP();
		bp04->maxHP(max_value(25, si - (si >> 6) - 1));
		bp04->curHP(bp04->maxHP() >> 1);
		//^2F3F:0769
		bp04->heroFlag |= 0x4000;
		bp04->enchantmentAura = 0;
		bp04->enchantmentPower = 0;
		_1031_0667();
		_443c_040e();
		//^2F3F:0786
		return;
//	}
//	else if (SkCodeParam::bUseFixedMode)	// 
//	{
//	// SPX: Code added from TAG014676 in CSBWin!!!
//		i16 player = xx;
//		Bit16u hpmax;
//		Bit16u hpnew;
//	  //dReg D0, D7;
//	  //CHARDESC *pcA3;
//		Champion *champion;
//	  //pcA3 = &d.CH16482[chIdx];
//		champion = &glbChampionSquad[player];
//	  //D0L = CharacterAtPosition(pcA3->charPosition);
//	  //if (D0L != -1)
//	  //{ //Find a position to put the character.
//		//for (D7W=0; CharacterAtPosition(D7W)!=-1; D7W++) {};
//	//
//	//
//		//pcA3->charPosition = D7UB;
//	  //};
//	  //D7W = pcA3->MaxHP();
//		hpmax = champion->maxHP();
//	  //D0W = sw(D7W - D7W/64 - 1);
//		hpnew = (hpmax - hpmax/64 - 1);
//	  //pcA3->MaxHP(Larger(25, D0W));
//		champion->maxHP(max_value(25, hpnew));
//	  //pcA3->HP(pcA3->MaxHP() / 2);
//		champion->curHP(champion->maxHP() / 2);
//	  //DrawSpellControls(d.MagicCaster);
//	  //pcA3->facing = (ui8)d.partyFacing;
//		champion->direction = 0;
//	  //pcA3->charFlags |= CHARFLAG_weaponAttack | CHARFLAG_portraitChanged | CHARFLAG_positionChanged;//0x9400;
//		champion->heroFlag |= CHAMPION_FLAG_8000 | CHAMPION_FLAG_1000 | CHAMPION_FLAG_0400 ;	// 0x8000 | 0x1000 | 0x0400 
//	  //DrawCharacterState(chIdx);
//		REFRESH_PLAYER_STAT_DISP(player);
//
//		// Non CSBWin code, is this useful?
//		CHAMPION_SQUAD_RECOMPUTE_POSITION();
//		UPDATE_RIGHT_PANEL(1);
//	}
}


//^2C1D:14DE
void SkWinCore::DROP_PLAYER_ITEMS(U16 player)
{
	//^2C1D:14DE
	ENTER(2);
	//^2C1D:14E4
	X16 di = glbChampionSquad[player].playerPos();
	X16 si;
	for (si = 0; si < INVENTORY_MAX_SLOT; si++) {
		//^2C1D:14FA
		ObjectID bp02 = REMOVE_POSSESSION(player, _4976_3fce[si]);
		if (bp02 != OBJECT_NULL) {
			//^2C1D:1512
			MOVE_RECORD_TO(ObjectID(bp02, di), -1, 0, glbPlayerPosX, glbPlayerPosY);
		}
		//^2C1D:1535
	}
	//^2C1D:153B
	return;
}

//^2C1D:15BA
// SPX: PLAYER_DEFEATED renamed CHAMPION_DEFEATED
void SkWinCore::CHAMPION_DEFEATED(X16 player)
{
	// CSBwinSimilarity: TAG016c5a,KillCharacter

	//^2C1D:15BA
	ENTER(12);
	//^2C1D:15C0
	X16 di;
	Champion *champion = &glbChampionSquad[di = player];

#if (DM2_EXTENDED_MODE == 1)
	if (SkCodeParam::bUseExtendedSound == true) {	// SPX: Play the champion own scream sound if dead
		printf("champion dead: %d\n", champion->HeroType());
		QUEUE_NOISE_GEN1(GDAT_CATEGORY_CHAMPIONS, champion->HeroType(), SOUND_CHAMPION_SCREAM, 0x61, 0x80, glbPlayerPosX, glbPlayerPosY, 1);
	}
#endif

	if (glbChampionIndex -1 == di)
		//^2C1D:15DB
		DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
	//^2C1D:15E0
	champion->curHP(0);
	champion->herob44 = champion->handCooldown[INVENTORY_HAND_LEFT] = champion->handCooldown[INVENTORY_HAND_RIGHT] = 0;
	champion->heroFlag |= CHAMPION_FLAG_4000;	// 0x4000
	if (di +1 == glbChampionInventory) {
		//^2C1D:1606
		if (glbShowItemStats != 0) {
			//^2C1D:160D
			glbShowItemStats = 0;
			FIRE_MOUSE_RELEASE_CAPTURE();
			if (glbLeaderHandPossession.object != OBJECT_NULL)
				//^2C1D:161F
				DISPLAY_TAKEN_ITEM_NAME(glbLeaderHandPossession.object);
			//^2C1D:1629
			glbMouseVisibility = 1;
		}
		//^2C1D:1634
		if (_4976_4bfe != 0) {
			//^2C1D:163B
			_4976_4bfe = 0;
			FIRE_MOUSE_RELEASE_CAPTURE();
			glbMouseVisibility = 1;
			FIRE_SHOW_MOUSE_CURSOR();
		}
		//^2C1D:1651
		INTERFACE_CHAMPION(4);
	}
	//^2C1D:1659
	if (di == glbChampionLeader && _4976_4c3e != 0) {
		//^2C1D:1666
		_4976_4c3e = 0;
		FIRE_MOUSE_RELEASE_CAPTURE();
		glbMouseVisibility = 1;
		FIRE_SHOW_MOUSE_CURSOR();
	}
	//^2C1D:167C
	DROP_PLAYER_ITEMS(di);
	X16 bp0a = champion->playerPos();
	ObjectID bp0c = ALLOC_NEW_RECORD(0x800a);
	if (bp0c != OBJECT_NULL) {
		//^2C1D:16A0
		Miscellaneous_item *bp08 = GET_ADDRESS_OF_RECORDA(bp0c);
		bp08->ItemType(GET_CHAMPION_BONES_ITEM_ID()); // SPX: changed hardcoded 0 to function to get proper Bones ID depending on DM2 or DM1 mode
		bp08->Important(1);
		bp08->Bone(di);
		MOVE_RECORD_TO(ObjectID(bp0c, bp0a), -1, 0, glbPlayerPosX, glbPlayerPosY);
	}
	//^2C1D:16F1
	FIRE_HIDE_MOUSE_CURSOR();
	champion->RuneCnt(0);
	champion->GetRunes()[0] = 0;
	champion->playerDir(U8(glbPlayerDir));
	champion->herob41 = 0;
	if (((bp0a +4 - glbPlayerDir) & 3) == _4976_5dbc)
		//^2C1D:1723
		CHAMPION_SQUAD_RECOMPUTE_POSITION();
	//^2C1D:1728
	if (champion->PoisonValue != 0)
		//^2C1D:1732
		CURE_POISON(di);
#if __SK_EXTENDED_SKWIN_V6__
	if (champion->PlagueValue != 0)
		CURE_PLAGUE(di);
#endif
	//^2C1D:1739
	X16 si;
	for (si = 0; si < glbChampionsCount && glbChampionSquad[si].curHP() == 0; si++);
	//^2C1D:1754
	if (si == glbChampionsCount) {
		//^2C1D:175A
		glbPlayerDefeated = 1;
		_1031_098e();
	}
	//^2C1D:1767
	else if (di == glbChampionLeader) {
		//^2C1D:176D
		SELECT_CHAMPION_LEADER(si);
	}
	//^2C1D:1774
	_1031_0667();
	FIRE_SHOW_MOUSE_CURSOR();
	//^2C1D:177E
	return;
}

//^2C1D:1782
void SkWinCore::PROCESS_PLAYERS_DAMAGE()
{
	// CSBwinSimilarity: TAG016e54,DisplayCharacterDamage

	//^2C1D:1782
	ENTER(16);
	//^2C1D:1788
	if (SkCodeParam::bChampionImmune)
		return;

	Champion* champion = glbChampionSquad;
	for (U16 championIndex = 0; championIndex < glbChampionsCount; championIndex++, champion++) {
		//^2C1D:1798
		champion->bodyFlag = glbChampionsBodyFlags[championIndex];
		glbChampionsBodyFlags[championIndex] = 0;
		X16 di = glbChampionsPendingDamage[championIndex];
		if (di == 0)
			continue;
		//^2C1D:17C5
		glbChampionsPendingDamage[championIndex] = 0;
		i16 si = champion->curHP();
		if (si == 0)
			continue;
		//^2C1D:17E0
		si = si -di;
		if (si <= 0) {
			CHAMPION_DEFEATED(championIndex);
			continue;
		}
		//^2C1D:17F4
		champion->curHP(si);
		champion->damageSuffered = di;
		champion->heroFlag |= CHAMPION_FLAG_0800;	// 0x800
		si = champion->timerIndex;
		//if (si == -1) {	// SPX: or == 0xFFFF ....
		if (champion->timerIndex == TIMER_NONE) {
			//^2C1D:1810
			Timer bp10;
			bp10.TimerType(tty0C);
			bp10.SetMap(glbPlayerMap);
			bp10.SetTick(glbGameTick +5);
			bp10.actor = U8(championIndex);
			champion->timerIndex = QUEUE_TIMER(&bp10);
		}
		else {
			//^2C1D:1852
			glbTimersTable[si].SetMap(glbPlayerMap);
			glbTimersTable[si].SetTick(glbGameTick +5);
			_3a15_05f7(si);
		}
		//^2C1D:1892
	}
	//^2C1D:18A6
	return;
}
//^2C1D:210A
// SPX: _2c1d_210a renamed UPDATE_CHAMPIONS_STATS
void SkWinCore::UPDATE_CHAMPIONS_STATS()
{
	//^2C1D:210A
	ENTER(14);
	//^2C1D:2110
	if (glbChampionsCount == 0)
		return;
	//^2C1D:211A
	glbChampionTable[0].enchantmentPower += 0x38;
	if (glbChampionTable[0].enchantmentPower > 0x80U)
		glbChampionTable[0].enchantmentPower -= 0x80;
	//^2C1D:212D
	U16 bp0e = 0;	
	Champion *champion = glbChampionSquad;
	X16 bp0c;
	U16 bp0a;
	for (; bp0e < glbChampionsCount; bp0e++, champion++) {
		//^2C1D:213D
		if (champion->curHP() == 0 || bp0e +1 == glbNextChampionNumber)
			continue;
		if (champion->curMP() < champion->maxMP()) {
			//^2C1D:2164
			bp0a = QUERY_PLAYER_SKILL_LV(bp0e, SKILL_WIZARD_GLOBAL, 1) + QUERY_PLAYER_SKILL_LV(bp0e, SKILL_PRIEST_GLOBAL, 1);
			if (GET_PLAYER_ABILITY(champion, abWiz, 0) +bp0a > glbChampionTable[0].enchantmentPower) {
				//^2C1D:21A4
				X16 si = champion->maxMP() / 40;
				si++;
				if (glbIsPlayerSleeping != 0)
					si <<= 1;
				//^2C1D:21BD
				ADJUST_STAMINA(bp0e, si * max_value(7, 0x10 -bp0a));
				champion->curMP(champion->curMP() +min_value(si, champion->maxMP() -champion->curMP()));
			}
		}
		//^2C1D:21FA
		// SPX: Decrease of current mana if it exceed maximum mana
		else if (champion->curMP() > champion->maxMP()) {
			champion->curMP(champion->curMP() -1);
		}
		//^2C1D:220B
		bp0c = 4;
		i16 si = champion->maxStamina();
		for (; champion->curStamina() < (si >>= 1); bp0c += 2);
		//^2C1D:222A
		si = 0;
		X16 di = BETWEEN_VALUE(1, (champion->maxStamina() >> 8) -1, 6);
		bp0a = U16(glbGameTick) -U16(_4976_4b80);
		if (bp0a > 0x50) {
			//^2C1D:2252
			di++;
			if (bp0a > 0xfa)
				di++;
		}
		//^2C1D:225B
		if (glbIsPlayerSleeping != 0)
			di <<= 1;
		//^2C1D:2264
		do {
			bp0a = (bp0c <= 4) ? 1 : 0;
			if (champion->curFood() < FOOD_WARNING) {
				//^2C1D:227F
				if (bp0a != 0) {
					si += di;
					champion->curFood(champion->curFood() -2);
				}
			}
			else {
				//^2C1D:228E
				if (champion->curFood() >= 0)
					si -= di;
				//^2C1D:229A
				champion->curFood(champion->curFood() -((bp0a != 0) ? 2 : (bp0c >> 1)));
			}
			//^2C1D:22BC
			if (champion->curWater() < WATER_WARNING) {
				if (bp0a != 0) {
					si += di;
					champion->curWater(champion->curWater() -1);
				}
			}
			else {
				//^2C1D:22D5
				if (champion->curWater() >= 0)
					si -= di;
				//^2C1D:22E1
				champion->curWater(champion->curWater() -((bp0a != 0) ? 1 : (bp0c >> 2)));
			}
			//^2C1D:2304
		} while (--bp0c != 0 && champion->curStamina() < champion->maxStamina());

		//^2C1D:2320
		ADJUST_STAMINA(bp0e, si);
		if (champion->curFood() < FOOD_MIN)
			champion->curFood(FOOD_MIN);
		if (champion->curWater() < WATER_MIN)
			champion->curWater(WATER_MIN);
		//^2C1D:234C
		if (champion->curHP() < champion->maxHP() && champion->curStamina() >= (champion->maxStamina() >> 2) && GET_PLAYER_ABILITY(champion, abVit, 0) > glbChampionTable[0].enchantmentPower) {
			//^2C1D:2384
			di = (champion->maxHP() >> 7) +1;
			if (glbIsPlayerSleeping != 0)
				di <<= 1;
			//^2C1D:239A
			// SPX : This is the regeneration function
			champion->curHP(champion->curHP() +min_value(di, champion->maxHP() -champion->curHP()));
		}
		//^2C1D:23B5
		if ((glbGameTick & ((glbIsPlayerSleeping != 0) ? 0x3f : 0xff)) == 0) {
			//^2C1D:23CA
			for (si = ATTRIBUTE_FIRST; si <= ATTRIBUTE_LAST; si++) {	// (si = 0; si <= 6; si++)
				//^2C1D:23CE
				U8 *attributes = champion->attributes[si];	// *bp08
                U16 attrMax = attributes[ATTRIBUTE_MAX];	// bp0a
				if (attributes[ATTRIBUTE_CURRENT] < attrMax) {
					attributes[ATTRIBUTE_CURRENT]++;
				}
				else if (attributes[ATTRIBUTE_CURRENT] > attrMax) {
					attributes[ATTRIBUTE_CURRENT] -= attributes[ATTRIBUTE_CURRENT] / attrMax;
				}
				//^2C1D:241D
			}
		}
		//^2C1D:2423
		champion->heroFlag |= CHAMPION_FLAG_0800;	// 0x800
		if (bp0e +1 == glbChampionInventory)
			champion->heroFlag |= CHAMPION_FLAG_3000;	// 0x3000
		//^2C1D:243C
	}
	//^2C1D:2450
	return;
}

//^2E62:0D82
// SPX: _2e62_0d82 renamed GLOBAL_UPDATE_UNKNOW1
void SkWinCore::GLOBAL_UPDATE_UNKNOW1()
{
	//^2E62:0D82
	ENTER(2);
	//^2E62:0D86
	U8 bp01;
	if (glbLeaderHandPossession.object != OBJECT_NULL && (QUERY_GDAT_DBSPEC_WORD_VALUE(glbLeaderHandPossession.object, 0)&0x8000) != 0 && (bp01 = GET_ITEM_ICON_ANIM_FRAME(glbLeaderHandPossession.object, -1, 1)) != _4976_57de) {
		//^2E62:0DB8
		DRAW_ITEM_IN_HAND(&glbLeaderHandPossession);
		_443c_0434();
		_4976_57de = bp01;
	}
	//^2E62:0DCE
	return;
}


//^24A5:069B
// SPX: _24a5_069b renamed BURN_PLAYER_LIGHTING_ITEMS
void SkWinCore::BURN_PLAYER_LIGHTING_ITEMS()
{
	//^24A5:069B
	ENTER(8);
	//^24A5:06A1
	X16 bRecomputeLight = 0;
	X16 iLocalChampionCount = glbChampionsCount;
	if (glbNextChampionNumber != 0)
		--iLocalChampionCount;
	Champion *xChampion = glbChampionSquad;	//*bp04
	for (; iLocalChampionCount-- != 0; xChampion++) {
		//^24A5:06BC
		X16 iInventorySlot = 2;
		for (; iInventorySlot-- != 0; ) { // go from inventory position 2 to 0 (hands)
			//^24A5:06C3
			ObjectID oItem = OBJECT_NULL; // SPX: add default init
			if ((QUERY_GDAT_DBSPEC_WORD_VALUE(oItem = xChampion->Possess(iInventorySlot), 0) & ITEM_FLAG_PRODUCE_LIGHT) == 0) // if item does not have 0x10 flag, then it does not produces light.
				continue;
			if (ADD_ITEM_CHARGE(oItem, 0) == 0)	// get current charge, if = 0, then do nothing
				continue;
			if (ADD_ITEM_CHARGE(oItem, -1) == 0) // decrease one charge
				SET_ITEM_IMPORTANCE(oItem, 0);
			//^24A5:0709
			bRecomputeLight = 1;	// Item charge has been changed
			//^24A5:070E
		}
		//^24A5:0718
	}
	//^24A5:0724
	if (bRecomputeLight != 0)
		RECALC_LIGHT_LEVEL();
	//^24A5:072E
	return;
}



//^12B4:011E
void SkWinCore::RESET_SQUAD_DIR()
{
	//^12B4:011E
	ENTER(0);
	//^12B4:0122
	for (U16 si = 0; si < glbChampionsCount; si++) {
		//^12B4:0126
		glbChampionSquad[si].playerDir(U8(glbPlayerDir));
	}
	//^12B4:013E
	return;
}

//^12B4:00AF
// SPX TODO related to rotate squad
void SkWinCore::_12b4_00af(U16 xx)
{
	//^12B4:00AF
	ENTER(0);
	//^12B4:00B2
	MOVE_RECORD_TO(OBJECT_NULL, glbPlayerPosX, glbPlayerPosY, -1, 0);
	glbMapToLoad = LOCATE_OTHER_LEVEL(glbPlayerMap, (xx != 0) ? -1 : +1, &glbPlayerPosX, &glbPlayerPosY, NULL);
	CHANGE_CURRENT_MAP_TO(glbMapToLoad);
	ROTATE_SQUAD(_0cee_06dc_GET_TILE_DIRECTION(glbPlayerPosX, glbPlayerPosY));
	CHANGE_CURRENT_MAP_TO(glbPlayerMap);
	//^12B4:011C
	return;
}

