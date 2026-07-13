//------------------------------------------------------------------------------
// SK-CHAMPION
//------------------------------------------------------------------------------

//#include <StdAfx.h>	// Required for MVC6 compilation

//#include <conio.h> // getch

#include <skcore.h>
#include <skglobal.h>
#include <skdebug.h>
#include <skcnsole.h>
#include <skparam.h>

#include <string.h>

#if defined(_USE_MFC80) || defined(_USE_MFC60)
//#include "SkWinMFC.h"
//#include "resource.h"
#endif // defined(_USE_MFC80) || defined(_USE_MFC60)


//^2C1D:2076
void SkWinCore::CALC_PLAYER_WEIGHT(U16 player)
{
	// updates player's weight. w253 doesn't contain weight of leader's hand possession.

	U16 iTotalWeight = 0;	// di
	i16 iInvSlot = 0;	// si

	//for (; si < 30; si++) {
	for (iInvSlot = 0; iInvSlot < C30_INVENTORY_MAX_SLOT; iInvSlot++) {
		iTotalWeight += QUERY_ITEM_WEIGHT(glbChampionSquad[player].inventory[iInvSlot]);
	}
	if (_4976_5336 - 1 == player && glbSelectedHandAction < 2) {
		if (IS_CONTAINER_CHEST(_4976_3de6[RCJ(4,player)][RCJ(2,glbSelectedHandAction)]) != 0) {
			for (U16 iContainerSlot = 0; iContainerSlot < 8; iContainerSlot++) {
				iTotalWeight += QUERY_ITEM_WEIGHT(glbCurrentContainerItems[iContainerSlot]);
			}
		}
	}
	glbChampionSquad[player].curWeight(iTotalWeight);
	glbChampionSquad[player].heroFlag |= CHAMPION_FLAG_1000;	// 0x1000
}


//^2C1D:0300
// SPX: _2c1d_0300 replaced by BOOST_ATTRIBUTE, called when consuming potion or special item
void SkWinCore::BOOST_ATTRIBUTE(Champion *ref, U16 xx, U16 yy)
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
	if (inventorySlot < C30_INVENTORY_MAX_SLOT) {	// (possess < 30)
		//^2C1D:040D
		U16 bp06 = QUERY_GDAT_DBSPEC_WORD_VALUE(di, GDAT_ITEM_STATS_GEN_FLAGS);
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
			U16 bp0a = IS_ITEM_FIT_FOR_EQUIP(di, inventorySlot, 1);
			U16 bp0c = 0;
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
							champion->manaMax = (U16) iChampionManaMax;
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
				for (U16 attributeIndex = 0; attributeIndex < ATTRIBUTE_COUNT; attributeIndex++) {
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
			for (U16 skillIndex = 0; skillIndex < SKILL_COUNT; skillIndex++) {
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
				bp16.SetMap(cd.pi.glbPlayerMap);
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
ObjectID SkWinCore::REMOVE_POSSESSION(U16 iChampionIdx, U16 possess)
{
	// CSBwinSimilarity: TAG015a66,RemovePossession

	ENTER(4);
	U16 iInvSlot = possess;	// si
	Champion* xChampion = &glbChampionSquad[iChampionIdx];	// bp04
	ObjectID di;	// di
	if (iInvSlot >= C30_INVENTORY_MAX_SLOT) {	// (si >= 30)
		di = glbCurrentContainerItems[iInvSlot - C30_INVENTORY_MAX_SLOT];
		glbCurrentContainerItems[iInvSlot - C30_INVENTORY_MAX_SLOT] = OBJECT_NULL;
	}
	else {
		di = xChampion->Possess(iInvSlot);
		xChampion->Possess(iInvSlot, OBJECT_NULL);
	}
	if (di == OBJECT_NULL) {
		return OBJECT_NULL;
	}
	if (true
		&& cd.pi.glbChampionIndex -1 == iChampionIdx
		&& iInvSlot <= 1
		&& iInvSlot == glbSelectedHandAction
	) {
		DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
	}
	PROCESS_ITEM_BONUS(iChampionIdx, di, iInvSlot, -1);
	return di;
}

//^2C1D:0831
void SkWinCore::EQUIP_ITEM_TO_INVENTORY(U16 iChampionID, ObjectID rl, U16 inventorySlot)
{
	ENTER(4);
	ObjectID oObject = rl;	// si
	U16 iInvSlot = inventorySlot;	// di
	if (oObject != OBJECT_NULL) {
		Champion* xChampion = &glbChampionSquad[iChampionID];
		oObject.ClearDir();
		if (iInvSlot >= C30_INVENTORY_MAX_SLOT) {	// (di >= 30)
			glbCurrentContainerItems[iInvSlot - C30_INVENTORY_MAX_SLOT] = oObject;
		}
		else {
			xChampion->Possess(iInvSlot, oObject);
		}
		PROCESS_ITEM_BONUS(iChampionID, oObject, iInvSlot, 1);
	}
	return;
}

//^2759:0E93
// TODO: related hand activation ?
// SPX: _2759_0e93 renamed CHECK_SOME_HAND_READY
U16 SkWinCore::CHECK_SOME_HAND_READY(U16 iChampIdx, ObjectID rl, i16 hand)
{
	ENTER(2);
	X16 iLocalChampIdx = iChampIdx;	// di
	X16 bp02 = 0;
	if (rl != OBJECT_NULL) {
		if (IS_ITEM_HAND_ACTIVABLE(iLocalChampIdx, rl, -1) != 0) {
			U16 iCmdSlotIdx;	// si
			for (iCmdSlotIdx = 0; iCmdSlotIdx < glbCommandSlotsNum; iCmdSlotIdx++) {
				if (QUERY_CMDSTR_ENTRY(glbItemSelected[iCmdSlotIdx].category, glbItemSelected[iCmdSlotIdx].index, glbItemSelected[iCmdSlotIdx].entry, 2) == hand) {
					bp02 = 1;
					break;
				}
			}
		}
		iLocalChampIdx = cd.pi.glbChampionIndex;
		if (iLocalChampIdx != 0) {
			if (glbSelectedHandAction == 0 || glbSelectedHandAction == 1) {
				iLocalChampIdx--;
				IS_ITEM_HAND_ACTIVABLE(iLocalChampIdx, glbChampionSquad[iLocalChampIdx].Possess(glbSelectedHandAction), glbSelectedHandAction);
			}
		}
	}
	return bp02;
}

//^2759:0FB0
void SkWinCore::LOAD_PROJECTILE_TO_HAND(U16 iChampionIdx, i16 hand)
{
	ENTER(14);
	Champion* xChampion = &glbChampionSquad[iChampionIdx];	//*bp04
    xChampion->handCooldown[hand] = 0;
	if (xChampion->curHP() == 0 || hand >= 2)
		return;
	i16 iOtherHand = hand ^ 1;	// bp0e
	i16 bp0a = hand;	// bp0a
	U16 bp0c = xChampion->handCommand[hand];
	xChampion->handCommand[hand] = 0xFF;
	xChampion->handDefenseClass[hand] = 0;
	//if (bp0c == 32) {
	if (bp0c == CmLaunchProjectile) {
		if (xChampion->Possess(iOtherHand) != OBJECT_NULL)
			return;
		U16 di = C12_INVENTORY_SCABBARD_1;	// 12
		if (IS_MISSILE_VALID_TO_LAUNCHER(iChampionIdx, bp0a, xChampion->Possess(di)) != 0) {
_1059:
			ObjectID si = REMOVE_POSSESSION(iChampionIdx, di);

			EQUIP_ITEM_TO_INVENTORY(iChampionIdx, si, iOtherHand);
			return;
		}
		else {
			ObjectID si = xChampion->Possess(C12_INVENTORY_SCABBARD_1);	// 12
			if (IS_CONTAINER_CHEST(si) != 0) {
				ObjectID* bp08 = &GET_ADDRESS_OF_RECORD9(si)->w2;
				si = *bp08;
				for (; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
					if (IS_MISSILE_VALID_TO_LAUNCHER(iChampionIdx, bp0a, si) != 0) {
						CUT_RECORD_FROM(si, bp08, -1, 0);
						EQUIP_ITEM_TO_INVENTORY(iChampionIdx, si, iOtherHand);
						return;
					}
				}
			}
			//for (i16 di = 7; di <= 9; di++) {
			for (i16 di = C07_INVENTORY_SCABBARD_2; di <= C09_INVENTORY_SCABBARD_4; di++) {
				if (IS_MISSILE_VALID_TO_LAUNCHER(iChampionIdx, bp0a, xChampion->Possess(di)) != 0)
					goto _1059;
			}
			return;
		}
	}
	if (bp0c != 42)
		return;
	if (xChampion->Possess(bp0a) != OBJECT_NULL)
		return;
	U16 di = C12_INVENTORY_SCABBARD_1;
	ObjectID si;
	if (CHECK_SOME_HAND_READY(iChampionIdx, xChampion->Possess(di), bp0c) != 0) {
_1149:
		si = REMOVE_POSSESSION(iChampionIdx, di);
_1156:
		EQUIP_ITEM_TO_INVENTORY(iChampionIdx, si, bp0a);
		return;
	}
	if (IS_CONTAINER_CHEST(si = xChampion->Possess(C12_INVENTORY_SCABBARD_1)) != 0) {
		ObjectID *bp08 = &GET_ADDRESS_OF_RECORD(si)->w0;
		si = *bp08;

		for (; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
			if (CHECK_SOME_HAND_READY(iChampionIdx, si, bp0c) != 0) {
				CUT_RECORD_FROM(si, bp08, -1, 0);
				goto _1156;
			}
		}
	}
//		for (di = 7; di <= 9; di++) {
	for (di = C07_INVENTORY_SCABBARD_2; di <= C09_INVENTORY_SCABBARD_4; di++) {
		if (CHECK_SOME_HAND_READY(iChampionIdx, xChampion->Possess(di), bp0c) != 0) {
			goto _1149;
		}
	}
	si = REMOVE_POSSESSION(iChampionIdx, 12);
	if (si != OBJECT_NULL)
		goto _1156;
	return;
}



//^2759:01FE
U16 SkWinCore::_2759_01fe(U16 player, ObjectID recordLink, U16 cmdNum)
{
	//^2759:01FE
    ENTER(8);
	//^2759:0204
	ObjectID di = recordLink;
	U16 si = cmdNum;
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
			U16 bp08 = bp04->b5_5_7();
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
i16 SkWinCore::FIND_POUCH_OR_SCABBARD_POSSESSION_POS(i16 iChampionIdx, i16 iIsScabbardOrPouch)	// yy = pouch or scabbard
{
	ENTER(0);
	i16 iLChampIdx = iChampionIdx;	// di
	if (iIsScabbardOrPouch == 1) {	// SCABBARD
		if (glbChampionSquad[iLChampIdx].Possess(C12_INVENTORY_SCABBARD_1) != OBJECT_NULL) {	// Possess(12)
			return C12_INVENTORY_SCABBARD_1;	// 12
		}
		//for (i16 si = 7; si <= 9; si++) {
		for (i16 iInvSlot = C07_INVENTORY_SCABBARD_2; iInvSlot <= C09_INVENTORY_SCABBARD_4; iInvSlot++) {	// si
			if (glbChampionSquad[iLChampIdx].Possess(iInvSlot) != OBJECT_NULL) {
				return iInvSlot;
			}
		}
	}
	else if (iIsScabbardOrPouch == 0) {	// POUCH
		if (glbChampionSquad[iLChampIdx].Possess(C11_INVENTORY_POUCH_1) != OBJECT_NULL) {	// Possess(11)
			return C11_INVENTORY_POUCH_1;	// 11
		}
		if (glbChampionSquad[iLChampIdx].Possess(C06_INVENTORY_POUCH_2) != OBJECT_NULL) {	// Possess(6)
			return C06_INVENTORY_POUCH_2;	// 6
		}
	}
	return -1;
}



//^2C1D:0A47
// SPX: 1 seems to be the minimal skill returned, which correspond to none ...
// Hence, NEOPHYTE would return 2 and ARCHMASTER 16
U16 SkWinCore::QUERY_PLAYER_SKILL_LV(i16 player, U16 skill, U16 yy)
{
	ENTER(8);
	U16 iReqSkill = skill;	// di
	if (cd.pi.glbIsPlayerSleeping != 0) {
		return 1;
	}
	Champion* xChampion = &glbChampionSquad[player]; //*bp04
	U32 iXPAmount = xChampion->skills[iReqSkill];	// bp08 / Amount of current xp for that skill
	if (iReqSkill > 3) {	// A subskill is requested
		iXPAmount += (xChampion->skills[(iReqSkill - 4) >> 2]) * ((yy != 0) ? (i16(xChampion->skillBonus[(iReqSkill - 4) >> 2]) +1) : 1);
		iXPAmount >>= 1;
	}
	U16 iSkillLevel = 1;	// si / Minimum level is 1 and equal to NONE.
	while (iXPAmount >= SKILL_EXPERIENCE_BASE_LEVEL) {	// (bp08 >= 512)
		iXPAmount >>= 1; iSkillLevel++;
	}
	if (yy != 0) {
		iSkillLevel = max_value(1, xChampion->skillBonus[iReqSkill] + iSkillLevel);
	}
	return iSkillLevel;
}

//^2759:02D6
// SPX: Returns 0 if current item is not hand activable => then it will have no icon in action hand.
// SPX: _2759_02d6 renamed IS_ITEM_HAND_ACTIVABLE
U16 SkWinCore::IS_ITEM_HAND_ACTIVABLE(U16 player, ObjectID recordLink, i16 yy)
{
	//^2759:02D6
	ENTER(14);
	//^2759:02DC
	ObjectID si = recordLink;
	Bit8u iCategory;	// bp01
	Bit8u iItemIndex;	// bp02
	if (recordLink == OBJECT_NULL) {
		//^2759:02E4
		iCategory = GDAT_CATEGORY_x16_CHAMPIONS;	// 0x16
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
		U16 bp0a = QUERY_CMDSTR_ENTRY(iCategory, iItemIndex, iEntryNumber, CnCM_Command);	// CnCM_Command = Command
		//^2759:036E
		if (bp0a == 0)
			//^2759:0372
			continue;
		//^2759:0375
		U16 bp0e = QUERY_CMDSTR_ENTRY(iCategory, iItemIndex, iEntryNumber, CnWH_Where);	// CnWH_Where = where (pouch or scabbard?)
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
			i16 bp0c = QUERY_CMDSTR_ENTRY(iCategory, iItemIndex, iEntryNumber, CnNC_NumberCharge);	// NC = Number of charges consumed by action
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
		U16 bp08 = QUERY_CMDSTR_ENTRY(iCategory, iItemIndex, iEntryNumber, CnSK_Skill);
		//^2759:043D
		U16 bp06 = QUERY_CMDSTR_ENTRY(iCategory, iItemIndex, iEntryNumber, CnLV_Level);
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
	glbCommandSlotsNum = di;
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
// _1031_007b renamed _1031_007b
U16 SkWinCore::_1031_007b_PFN12_04(sk1891 *ref)
{
	ENTER(0);
	return (glbChampionSquad[ref->b1].curHP() != 0) ? 1 : 0;
}

//^1031:009E
// _1031_009e renamed _1031_009e_PFN12_05
U16 SkWinCore::_1031_009e_PFN12_05(sk1891 *ref)
{
	//^1031:009E
	ENTER(0);
	//^1031:00A1
	return (GET_PLAYER_AT_POSITION((ref->b1 + cd.pi.glbPlayerDir) & 3) >= 0) ? 1 : 0;
}


//^1031:012D
// _1031_012d renamed _1031_012d_PFN12_08
U16 SkWinCore::_1031_012d_PFN12_08(sk1891 *ref)
{
	ENTER(0);
	return (cd.pi.glbChampionIndex != 0 && ref->b1 == glbCommandSlotsNum) ? 1 : 0;
}

//^1031:014F
// _1031_014f renamed _1031_014f_PFN12_09
U16 SkWinCore::_1031_014f_PFN12_09(sk1891 *ref)
{
	ENTER(0);
	return (cd.pi.glbChampionIndex != 0 && (ref->b1 & (1 << glbChampionTable[cd.pi.glbChampionIndex].runesCount)) != 0) ? 1 : 0;
}

//^1031:0184
// _1031_0184 renamed _1031_0184_PFN12_10
U16 SkWinCore::_1031_0184_PFN12_10(sk1891 *ref)
{
	ENTER(0);
	return (cd.pi.glbChampionIndex != 0) 
		? (((glbMagicalMapFlags & 0x8000) != 0) 
			? ((ref->b1 == glbCommandSlotsNum) 
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
		&& (bp01 = ref->enchantmentAura) >= C03_ENCHANTMENT_AURA_FIRST
		&& (bp01) <= C06_ENCHANTMENT_AURA_LAST
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
void SkWinCore::PUT_ITEM_TO_PLAYER(U16 championIndex)
{
	// click player panel to store leader's holding item into your back pack.

	ENTER(0);
	if (cd.pi.glbLeaderHandPossession.object != OBJECT_NULL) {
		if (glbChampionSquad[championIndex].curHP() != 0) {
			X16 iInvSlotIdx;	// si
			for (iInvSlotIdx = C13_INVENTORY_BACKPACK_1; iInvSlotIdx <= C29_INVENTORY_BACKPACK_LAST; iInvSlotIdx++) {	// (si=13; si<30; si++)
				if (glbChampionSquad[championIndex].Possess(iInvSlotIdx) == OBJECT_NULL)
					break;
			}
			if (iInvSlotIdx != (C29_INVENTORY_BACKPACK_LAST+1)) {	// (si != 30)
				ObjectID di = REMOVE_OBJECT_FROM_HAND();	// di
				if (di != OBJECT_NULL) {
					EQUIP_ITEM_TO_INVENTORY(championIndex, di, iInvSlotIdx);
				}
			}
		}
	}
	return;
}


//^1031:16A0
// SPX: _1031_16a0 renamed SELECT_CHAMPION_LEADER
void SkWinCore::SELECT_CHAMPION_LEADER(U16 xx)
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
	if (si + 1 == cd.pi.glbNextChampionNumber)
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
// SPX: _2f3f_04ea renamed REVIVE_CHAMPION
void SkWinCore::REVIVE_CHAMPION(U16 xx, U16 yy, U16 dir, U16 zz, U16 iEventCode)
{
	ENTER(142);
	U16 iChampionNumber = cd.pi.glbNextChampionNumber - 1;	// si
	Champion* xChampion = &glbChampionSquad[iChampionNumber];	// bp04
	U16 iCurrentMapIndex = glbCurrentMapIndex;	// bp0c
	CHANGE_CURRENT_MAP_TO(zz);
	ObjectID tObjectHand = REMOVE_OBJECT_FROM_HAND();	// bp0e
	if (iEventCode == UI_EVENTCODE_xA1_EXIT_CRYOCELL) { // 161
		INTERFACE_CHAMPION(4);
		FIRE_HIDE_MOUSE_CURSOR();
        if (cd.pi.glbChampionsCount == 1)
			SELECT_CHAMPION_LEADER(0xffff);
		if (cd.pi.glbNextChampionNumber >= cd.pi.glbChampionsCount)
			cd.pi.glbChampionsCount--;
		cd.pi.glbNextChampionNumber = 0;
		xChampion->curHP(0);
		glbChampionAlive[iChampionNumber] = 0;
		_4976_3de2[iChampionNumber] = 0xFF;
		_4976_3dde[RCJ(4,iChampionNumber)] = 0xFF;
		glbChampionHandCoolingDown[iChampionNumber][1] = 0;
		_4976_3de6[RCJ(4,iChampionNumber)][1] = 0xFFFF;
		glbChampionHandCoolingDown[iChampionNumber][0] = 0;
		_4976_3de6[RCJ(4,iChampionNumber)][0] = 0xFFFF;
		glbChampionHandCoolingDown[iChampionNumber][2] = 0;
		FIRE_FILL_SCREEN_RECT(iChampionNumber + RECT_161_CHAMPION_SLAB_AREA, glbPaletteT16[COLOR_BLACK]);
		SLEEP_SCREEN_38c8_0060();
		FIRE_SHOW_MOUSE_CURSOR();
	}
	else { // iEventCode = UI_EVENTCODE_xA0_REVIVE_CHAMPION (160)
		if (glbChampionShowResurrect == 0)
			DRAW_CRYOCELL_LEVER(1);
		if (tObjectHand != OBJECT_NULL)
			ADD_ITEM_TO_PLAYER(iChampionNumber, tObjectHand);
		cd.pi.glbNextChampionNumber = 0;
		xx += glbXAxisDelta[dir];
		yy += glbYAxisDelta[dir];

		for (U16 iInventoryIndex = 0; iInventoryIndex < C30_INVENTORY_MAX_SLOT; iInventoryIndex++) {	// bp0a < 30
			ObjectID tObject = xChampion->Possess(iInventoryIndex);	// di
			if (tObject != OBJECT_NULL) {
				CUT_RECORD_FROM(tObject, NULL, xx, yy);
			}
		}
		ObjectID tObject = GET_TILE_RECORD_LINK(xx, yy);	// di
		// SPX: enter this infinite while only is there is something on the tile (normally that would be the case, calling champion from actuator), but if it isn't, just don't get in the loop.
		if (SkCodeParam::bUseFixedMode == true && (tObject != OBJECT_END_MARKER && tObject != OBJECT_NULL)) {
			while (true) {
				if (tObject.DBType() == dbActuator) {
					Actuator* xActuator = GET_ADDRESS_OF_ACTU(tObject);	// bp08
					if (xActuator->ActuatorType() == ACTUATOR_x7E_TYPE_RESURECTOR)	// DM2
					{
						xActuator->OnceOnlyActuator(0);
						break;
					}
					else if (xActuator->ActuatorType() == ACTUATOR_x7F_TYPE_CHAMPION_MIRROR) // SPX: handle DM1 Champion Mirror too
					{
						xActuator->ActiveStatus(1); // change to "inactive", different from the "once only" status
						break;
					}
				}
				tObject = GET_NEXT_RECORD_LINK(tObject);
				if (SkCodeParam::bUseFixedMode == true && (tObject == OBJECT_END_MARKER || tObject == OBJECT_NULL))
					break;
			}
		} // fixed mode
		if (cd.pi.glbChampionsCount == 1) {
			glbGameTick_4b80 = glbGameTick;
		}
		if (glbChampionShowResurrect == 0) {
			glbChampionBonesIndex = iChampionNumber;
			DISPLAY_HINT_NEW_LINE();
			U8 sTxtChampionAwakened[128];	// bp008e
			// SPX: Text 0x0E: AWAKENED / RESURRECTED
			DISPLAY_HINT_TEXT(glbChampionColor[iChampionNumber], QUERY_GDAT_TEXT(GDAT_CATEGORY_x01_INTERFACE_GENERAL, 0x00, 0x0E, sTxtChampionAwakened));
			if (glbChampionInventory != 0) {
				INTERFACE_CHAMPION(4);
				SLEEP_SCREEN_38c8_0060();
			}
		}
	}
	CHANGE_CURRENT_MAP_TO(iCurrentMapIndex);
	return;
}



//^24A5:1798
// SPX: _24a5_1798 renamed INTERFACE_CHAMPION (not sure of what's inside)
void SkWinCore::INTERFACE_CHAMPION(U16 iChampionIndex)
{
	ENTER(8);
	i16 iChampIdx = iChampionIndex;	// di
	U16 bp06 = 0;
	glbInventorySubpanel = 0;
	_4976_3d30 = OBJECT_NULL;
	if (iChampIdx < 4 && glbChampionSquad[iChampIdx].curHP() == 0)
		return;
	if (_4976_4c3e != 0 || glbTryPushPullObject != 0 || _4976_4bfe != 0 || glbShowItemStats != 0)
		return;
	cd.gg.glbRefreshViewport = 1;
	U16 si = glbChampionInventory;
	if (iChampIdx + 1 == si)
		iChampIdx = 4;
	if (si == 0 || iChampIdx == 4) {
		bp06 = 1;
		FIRE_HIDE_MOUSE_CURSOR();
	}
	if (si != 0) {
		glbChampionInventory = 0;
		Champion* xChampion = &glbChampionSquad[si -1];	// bp04
		if (xChampion->curHP() != 0 && cd.pi.glbNextChampionNumber == 0) {
			xChampion->heroFlag |= CHAMPION_FLAG_4000;	// 0x4000
			REFRESH_PLAYER_STAT_DISP(si -1);
		}
		for (i16 bp08 = 0; bp08 < 4; bp08++) {
			glbChampionMajorSkillsLevel[si -1][bp08] -= _4976_52c0[bp08];
		}
		if (cd.pi.glbIsPlayerSleeping != 0) {
			if (bp06 != 0) {
				_443c_040e_MOUSE();
				FIRE_SHOW_MOUSE_CURSOR();
			}
			goto _1913;
		}
		if (iChampIdx == 4) {
			if (_4976_3d59 != 0)
				DRAW_ARROW_PANEL();
			if (bp06 != 0)
				FIRE_SHOW_MOUSE_CURSOR();
			INIT_BACKBUFF();
			goto _1913;
		}
	}
	glbChampionInventory = iChampIdx + 1;
	if (si == 0)
		FIRE_FILL_HALFTONE_RECTI(9, 0);
	ZERO_MEMORY(_4976_52c0, 4);
	glbChampionSquad[iChampIdx].heroFlag |= CHAMPION_FLAG_FC00;	// 0xFC00
	_4976_581a = 0;
	REFRESH_PLAYER_STAT_DISP(iChampIdx);
	if (bp06 != 0)
		FIRE_SHOW_MOUSE_CURSOR();
	_1031_0541(8);
	return;
_1913:
	_1031_0667();
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
//#ifndef __SKWIN_PUBLIC_VERSION__        
//		DISPLAY_HINT_TEXT(glbChampionColor[player], message);
//#endif // __SKWIN_PUBLIC_VERSION__

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
	DISPLAY_HINT_TEXT(glbChampionColor[player], QUERY_GDAT_TEXT(GDAT_CATEGORY_x01_INTERFACE_GENERAL, GDAT_INTERFACE_SUBCAT_x00_BASE_DATA, bp0d, bp008e));
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
		cd.pi.glbPlayerPosX,
		cd.pi.glbPlayerPosY,
		((((ref->playerPos() -si +1) & 2) >> 1) +si) & 3,
		si,
		min_value(yy, 255),
		min_value(zz, 255),
		min_value(ww, 255)
		);

	SkD((DLV_TWEET, "Tweet: You (x:%d, y:%d, map:%d) have thrown %s by cast! \n"
		, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, glbCurrentMapIndex, static_cast<LPCSTR>(getRecordNameOf(rl))
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




//^075F:1F33
U16 SkWinCore::CALC_PLAYER_ATTACK_DAMAGE(Champion *ref, U16 player, ObjectID rlEnemy, U16 xx, U16 yy, U16 valPb, U16 valDM, U16 valSk, U16 valAt)
{
	//^075F:1F33
	ENTER(16);
	//^075F:1F39
	if (player >= cd.pi.glbChampionsCount)
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
U16 SkWinCore::WIELD_WEAPON(U16 iChampionIdx, U16 valPa, U16 xx, U16 yy, U16 valSk, U16 valAt)
{
	// returns 0 if no target, 1 if hits a creature.

	ENTER(4);
	Champion* xChampion = &glbChampionSquad[iChampionIdx];	//*bp04
	if (glbObjectID_4976_534c == OBJECT_NULL)
		return 0;
	if (GET_CREATURE_AT(xx, yy) == OBJECT_NULL)
		return 0;
	U16 iChampionPos = xChampion->playerPos();	// di
	U16 si;
	switch ((iChampionPos + 4 - xChampion->playerDir()) & 3) {
		case 2:
			si = 3;
			goto _152e;

		case 3:
			si = 1;
_152e:
			if (GET_PLAYER_AT_POSITION((iChampionPos + si) & 3) != -1) {
				glbChampionAttackDamage = ATTACK_FAILURE_NOT_FRONT; // -1
				glbSomeChampionPanelFlag = max_value(1, 8 - UPDATE_GLOB_VAR(C066_GLOB_BYTE_02_CHAMPION, 1, C03_GLOBAL_VAR_OP_ADD));
				return 0;
			}

			break;
	}
	if (valPa == 1) {
		if ((QUERY_CREATURE_AI_SPEC_FLAGS(glbObjectID_4976_534c) & 0x0020) == 0)
			return 0;
	}
	si = QUERY_CUR_CMDSTR_ENTRY(CnPB_Probability);
	U16	iWeaponDamage; // reused di
	iWeaponDamage = QUERY_CUR_CMDSTR_ENTRY(CnDM_Damage);
	if (QUERY_CUR_CMDSTR_ENTRY(CnHN_HarmNonMaterial) != 0)
		si |= 0x8000;	// Hurt non material
	glbChampionAttackDamage = CALC_PLAYER_ATTACK_DAMAGE(xChampion, iChampionIdx, glbObjectID_4976_534c, xx, yy, si, iWeaponDamage, valSk, valAt);
	return 1;
}


//^2C1D:0E57
i16 SkWinCore::STAMINA_ADJUSTED_ATTR(Champion* xChampion, i16 iValue)
{
	// CSBwinSimilarity: TAG0163c8,StaminaAdjustedAttribute

	ENTER(2);
	i16 iStaminaValue = iValue;	// si
	i16 iStamina = xChampion->curStamina();	// bp02
	U16 iMaxStaminaHalf = xChampion->maxStamina() >> 1;	// di
	if (iStamina < iMaxStaminaHalf) {
		iStaminaValue >>= 1;
		return (i32(iStaminaValue) * i32(iStamina)) / i32(iMaxStaminaHalf) + iStaminaValue;
	}
	return iStaminaValue;
}

//^2C1D:1DE2
//SPX: About shooting?
// _2c1d_1de2 renamed _2c1d_1de2_CHAMPION_SHOOT
U16 SkWinCore::_2c1d_1de2_CHAMPION_SHOOT(U16 xx, i16 yy, U16 zz)
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
		if (cd.pi.glbLeaderHandPossession.object == OBJECT_NULL)
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
		0xFE,
		cd.pi.glbPlayerPosX,
		cd.pi.glbPlayerPosY,
		1,
		0x6E,
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
	SHOOT_ITEM(si, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, (cd.pi.glbPlayerDir + zz) & 3, cd.pi.glbPlayerDir, bp0a, bp08, bp06);

	SkD((DLV_TWEET, "Tweet: You (x:%d, y:%d, map:%d) have thrown %s! \n"
		, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, glbCurrentMapIndex, static_cast<LPCSTR>(getRecordNameOf(si))
		));

	//^2C1D:1F9E
	glbPlayerThrowCounter = 4;
	//^2C1D:1FA4
	_4976_4c0c = cd.pi.glbPlayerDir;
	//^2C1D:1FAA
	return 1;
}


//^2C1D:1BB5
// SPX: _2c1d_1bb5 replaced by CURE_POISON
void SkWinCore::CURE_POISON(U16 iChampIdx)
{
	ENTER(4);
	i16 iLocalChampIdx = iChampIdx;	// di
	if (iLocalChampIdx != -1) {
		U16 iTimerIdx = 0;	// si
		Timer* xTimer = glbTimersTable; // bp04
		for (iTimerIdx = 0; iTimerIdx < glbTimersActiveCount; xTimer++, iTimerIdx++) {
			if (xTimer->TimerType() == C75_tty_PoisonChampion) {	// search for poison timer and delete it if it is for the same champion index
				if (xTimer->actor == iLocalChampIdx) {
					DELETE_TIMER(iTimerIdx);
				}
			}
		}
		glbChampionSquad[iLocalChampIdx].PoisonValue = 0;
	}
	return;
}



//^24A5:10FB
void SkWinCore::PLAYER_CONSUME_OBJECT(U16 player, ObjectID rlConsume, i16 possess)
{
	// SPX: Default values
	U16 PotionType = 0;
	U16 WaterValue = WATER_DEFAULT_ITEM_VALUE;	// 800


	ENTER(18);
	ObjectID tObjectConsumed = rlConsume;	// di
	U16 bp12 = 1;
	U16 iBodyFlagTestCounter = 10;	// bp0c	
	U16 iPowerDivisor = 8;	// bp0c
	if (tObjectConsumed != OBJECT_NULL) {
		bp12 = 0;
	}
	else if (cd.pi.glbLeaderHandPossession.object == OBJECT_NULL) {
		FIRE_MOUSE_SET_CAPTURE();
		_4976_4bfe = 1;
		if ((_4976_5dae.rc4.cy & 2) == 0) {
			FIRE_MOUSE_RELEASE_CAPTURE();
			_4976_4bfe = 0;
			return;
		}
		FIRE_HIDE_MOUSE_CURSOR();
		glbMouseVisibility = 1;
		return;
	}
	else {
		player = glbChampionInventory -1;
		tObjectConsumed = cd.pi.glbLeaderHandPossession.object;
		possess = -1;
	}
	if (cd.pi.glbNextChampionNumber != 0)
		return;

	Champion *champion = &glbChampionSquad[player];
	U16 iFoodValue = QUERY_GDAT_FOOD_VALUE_FROM_RECORD(tObjectConsumed);	// bp10
	U16 iMouthAnimCounter;	// bp0c
	U16 si;
	Potion *bp08;
	if (iFoodValue != 0) {
		if (bp12 != 0) {
			FIRE_HIDE_MOUSE_CURSOR();
			REMOVE_OBJECT_FROM_HAND();
			for (iMouthAnimCounter = 5; (--iMouthAnimCounter) != 0; ) {
				DRAW_STATIC_PIC(GDAT_CATEGORY_x07_INTERFACE_CHARSHEET, 0x00, ((iMouthAnimCounter & 1) != 0) ? 0x25 : 0x26, 545, -1);	// mouth icon
				CHANGE_VIEWPORT_TO_INVENTORY(0);
				SLEEP_SEVERAL_TIME(8);
			}
			FIRE_SHOW_MOUSE_CURSOR();
		}
		//SPX: Eating food item
		champion->curFood(min_value(champion->curFood() + iFoodValue, FOOD_MAX));
		PROCESS_ITEM_BONUS(player, tObjectConsumed, possess, 2);
		DEALLOC_RECORD(tObjectConsumed);
		if (possess != -1) {
			champion->Possess(possess, OBJECT_NULL);
		}
		goto _14a9;
	}
	U16 bp0a;
	if (IS_MISCITEM_DRINK_WATER(tObjectConsumed) != 0) {
		// SPX: This assume that any misc item gives 800 water per drink
		WaterValue = WATER_DEFAULT_ITEM_VALUE;

		// SPX: Custom Water value, override it if any
		if (SkCodeParam::bUseDM2ExtendedMode)
			WaterValue = QUERY_GDAT_WATER_VALUE_FROM_RECORD(tObjectConsumed);

		champion->curWater(min_value(champion->curWater() + WaterValue, WATER_MAX));
		if (possess != -1) {
			champion->Possess(possess, tObjectConsumed);
		}
		goto _14a9;
	}
	bp0a = tObjectConsumed.DBType();
	if (bp0a == dbPotion) {
		iPowerDivisor = 8;	// SPX, bp0c
		bp08 = GET_ADDRESS_OF_RECORD8(tObjectConsumed);
		si = bp08->PotionPower();
		// SPX: Degressive divisor => bp0c = 08 (lowest power) to 02 (highest power)
		iPowerDivisor = ((511 -si) / (((si +1) >> 3) +32)) >> 1;
		si = (si / 25) +8;
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
				champion->curStamina(champion->curStamina() +min_value(champion->maxStamina() -champion->curStamina(),champion->maxStamina() / iPowerDivisor));
				//^24A5:1354
				break;

			case 12:			// ”é–ò{YA}		Cast party shield
				//^24A5:1357
				si += (si >> 1);
				//^24A5:135D
				PROCEED_ENCHANTMENT_SELF(1 << player, C02_ENCHANTMENT_PARTY_SHIELD, si, si * si);
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
				champion->curHP(champion->curHP() + (champion->maxHP() / iPowerDivisor));
				//^24A5:13FB
				bp0e = champion->bodyFlag;
				//^24A5:1402
				if (bp0e == 0)
					//^24A5:1404
					break;
				//^24A5:1406
				iBodyFlagTestCounter = 10;	// bp0c
				do {
					for (si = 0; si < bp0a; si++) {
						champion->bodyFlag &= RAND();
					}
					bp0a = 1;
					if (champion->bodyFlag != bp0e)
						break;
					iBodyFlagTestCounter--;
				} while (iBodyFlagTestCounter != 0);
				break;

			case 15:			// WATER FLASK
				WaterValue = WATER_FLASK_VALUE;	// Default value 1600

				if (SkCodeParam::bUseDM2ExtendedMode)
					WaterValue = QUERY_GDAT_WATER_VALUE_FROM_RECORD(tObjectConsumed);
				
				//^24A5:143E
				champion->curWater(min_value(champion->curWater() +WaterValue, WATER_MAX));

				break;
		}
		//^24A5:145A
		if (possess == -1)
			//^24A5:1460
			REMOVE_OBJECT_FROM_HAND();
		//^24A5:1465
		DEALLOC_RECORD(tObjectConsumed);
		//^24A5:146C
		tObjectConsumed = ALLOC_NEW_RECORD(dbPotion);
		//^24A5:1476
		if (tObjectConsumed == OBJECT_NULL)
			//^24A5:147B
			return;
		//^24A5:147E
		SET_ITEMTYPE(tObjectConsumed, 0x14);
		//^24A5:1488
		if (possess == -1) {
			//^24A5:148E
			TAKE_OBJECT(tObjectConsumed, 0);
		}
		else {
			//^24A5:149A
			champion->Possess(possess, tObjectConsumed);
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
		QUEUE_NOISE_GEN2(GDAT_CATEGORY_x16_CHAMPIONS, champion->HeroType(), SOUND_CHAMPION_EAT_DRINK, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 0, 0x96, 0x80);
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
	ENTER(148);
	Champion* xChampion = &glbChampionSquad[player = cd.pi.glbChampionsCount];	//*bp04
	xChampion->HeroType(U8(heroType));
	xChampion->handCommand[1] = xChampion->handCommand[0] = 0xff;
	xChampion->timerIndex = TIMER_NONE;
	xChampion->playerDir(U8(dir));
	X16 iLocalIndex = 0;	// bp0e
	for (iLocalIndex = 0; GET_PLAYER_AT_POSITION((iLocalIndex + cd.pi.glbPlayerDir) & 3) != -1; iLocalIndex++);	// find available slot

	xChampion->playerPos(iLocalIndex + cd.pi.glbPlayerDir);
	xChampion->direction = U8(cd.pi.glbPlayerDir);
	for (iLocalIndex = 0; iLocalIndex < C30_INVENTORY_MAX_SLOT; iLocalIndex++)	// init inventory
		xChampion->Possess(iLocalIndex, OBJECT_NULL);

	U8 sChampionNameBuffer[0x80];	// bp0094
	// SPX: get HERO NAME
	U8* pChampionTextBuffer = QUERY_GDAT_TEXT(GDAT_CATEGORY_x16_CHAMPIONS, U8(heroType), 0x18, sChampionNameBuffer);	// bp0c
	X16 cNameCurChar = 0;	// bp10
	for (iLocalIndex = 0; (cNameCurChar = i8(*(pChampionTextBuffer++))) != 0x20 && cNameCurChar != 0 && iLocalIndex < 7; iLocalIndex++) {
		xChampion->firstName[iLocalIndex] = U8(cNameCurChar);
	}
	xChampion->firstName[iLocalIndex] = 0;
	iLocalIndex = 0;
	if (cNameCurChar != 0) {
		while (true) {
			cNameCurChar = *(pChampionTextBuffer++);
			if (cNameCurChar == 0)
				break;
			if ((xChampion->lastName[iLocalIndex++] = U8(cNameCurChar)) == 0x13)
				break;
		}
	}
	xChampion->lastName[iLocalIndex] = 0;
//DEBUG_DUMP_ULP();
	TSKHero* xChampionData = reinterpret_cast<TSKHero *>(QUERY_GDAT_ENTRY_DATA_PTR(GDAT_CATEGORY_x16_CHAMPIONS, U8(heroType), dt08, 0x00));	// bp08
#if DM2_EXTENDED_MODE == 1	// TODO To be replaced with fixedmode + checkmem
	if (xChampionData == NULL)
		RAISE_SYSERR(SYSTEM_ERROR__NO_PLAYER_DATA);
#endif
	xChampion->maxHP(xChampionData->maxHP());
	xChampion->curHP(xChampionData->maxHP());
	xChampion->maxStamina(xChampionData->maxStamina());
	xChampion->curStamina(xChampionData->maxStamina());
	xChampion->maxMP(xChampionData->maxMP());
	xChampion->curMP(xChampionData->maxMP());
	X16 iSkillAttributeIndex = 0; // SPX: added
	for (iSkillAttributeIndex = 0; iSkillAttributeIndex <= 6; iSkillAttributeIndex++) {
		xChampion->attributes[iSkillAttributeIndex][ATTRIBUTE_MAX] = xChampion->attributes[iSkillAttributeIndex][ATTRIBUTE_CURRENT] = U8(max_value(0x1E, xChampionData->herodata[iSkillAttributeIndex + 3]));
		xChampion->attributesEnhanced[iSkillAttributeIndex] = 0;
	}
	for (iSkillAttributeIndex = 4; iSkillAttributeIndex <= 19; iSkillAttributeIndex++) {	// sub-skills fighter / ninja / priest / wizard
		X16 iSkillValue = xChampionData->herodata[iSkillAttributeIndex + 10 - 4];	// bp10
		xChampion->skills[iSkillAttributeIndex] = (iSkillValue != 0) ? (U32(0x40) << iSkillValue) : 0;
	}
	for (iSkillAttributeIndex = 0; iSkillAttributeIndex <= 3; iSkillAttributeIndex++) {	// (bp0e) main-skills fighter / ninja / priest / wizard
		X32 iSkillExperience = 0;	// bp14
		X16 iBaseSkill = (iSkillAttributeIndex + 1) << 2;	// bp10
		U16 iSubSkillIndex;	// si
		for (iSubSkillIndex = 0; iSubSkillIndex < 4; iSubSkillIndex++) {
			iSkillExperience += xChampion->skills[iBaseSkill + iSubSkillIndex];
		}
		xChampion->skills[iSkillAttributeIndex] = iSkillExperience;
	}
	xChampion->curFood((RAND() & 255) + START_BASE_FOOD);
	xChampion->curWater((RAND() & 255) + START_BASE_WATER);

	// SPX: DM1 Compatibility code -- check TEXT at player position which should contain CHAMPION stats
	if (SkCodeParam::bDM1Mode) // maybe this could be extendable for DM2 anyway
	{
		ObjectID xObject = OBJECT_END_MARKER;
		U8 iPlayerPosX = cd.pi.glbPlayerPosX;
		U8 iPlayerPosY = cd.pi.glbPlayerPosY;
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
				memset(xChampion->firstName, 0, 8);
				strcpy((char*)xChampion->firstName, (char*)sBuffer);

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
				memset(xChampion->lastName, 0, 16);
				//strcpy((char*)champion->lastName, (char*)sBuffer);
				strncpy((char*)xChampion->lastName, (char*)sBuffer, 16);

				// Read gender
				sHeroPtr+=1;
				sGender=*sHeroPtr;

				// Read basic stats
				sHeroPtr+=2;
				iStatValue = DECODE_CHARACTER_VALUE(sHeroPtr, 4);
				xChampion->maxHP(iStatValue);
				xChampion->curHP(iStatValue);
				sHeroPtr+=4;
				iStatValue = DECODE_CHARACTER_VALUE(sHeroPtr, 4);
				xChampion->maxStamina(iStatValue);
				xChampion->curStamina(iStatValue);
				sHeroPtr+=4;
				iStatValue = DECODE_CHARACTER_VALUE(sHeroPtr, 4);
				xChampion->maxMP(iStatValue);
				xChampion->curMP(iStatValue);
				sHeroPtr+=4;

				sHeroPtr++; // new line
				// Read attributes stats
				for (iSkillIndex = 0; iSkillIndex < 7; iSkillIndex++)
				{
					iStatValue = DECODE_CHARACTER_VALUE(sHeroPtr, 2);
					xChampion->attributes[iSkillIndex][0] = iStatValue;
					xChampion->attributes[iSkillIndex][1] = xChampion->attributes[iSkillIndex][0];
					sHeroPtr+=2;
				}

				sHeroPtr++; // new line
				// Read sub-skills levels
				for (iSkillIndex = 4; iSkillIndex < 20; iSkillIndex++)
				{
					X32 iXP = 0;
					iStatValue = DECODE_CHARACTER_VALUE(sHeroPtr, 1);
					iXP = 125 << iStatValue;
					xChampion->skills[iSkillIndex] = iXP;
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
						iTotalMajorXP += xChampion->skills[iMainSkill + iSubSkill];
					}
					xChampion->skills[iSkillIndex] = iTotalMajorXP;
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

		xChampion->maxHP(statHP);
		xChampion->curHP(xChampion->maxHP());
		xChampion->maxStamina(statStamina);
		xChampion->curStamina(xChampion->maxStamina());
		xChampion->maxMP(statMP);
		xChampion->curMP(xChampion->maxMP());
		for (int i = 0; i < SKILL_COUNT; i++)
			xChampion->skills[i] = SKILL_EXPERIENCE_BASE_LEVEL<<(SkLvlMasterLo-2);
	} // End super/debug block

	return;
}

//^2F3F:0343
// SPX: _2f3f_0343 renamed SELECT_CHAMPION. Called when clicking on "mirror/cell" or at the beginning for
// automatic selection of Thoram
X16 SkWinCore::SELECT_CHAMPION(U16 iXPos, U16 iYPos, U16 iDir, U16 iMapNo)
{
	ENTER(10);
	U16 iChampionNumber = 0;	// di
	if (cd.pi.glbLeaderHandPossession.object != OBJECT_NULL || (iChampionNumber = cd.pi.glbChampionsCount) >= MAX_CHAMPIONS)
		return 0;
	X16 iCurrentMap = glbCurrentMapIndex;	// bp08
	CHANGE_CURRENT_MAP_TO(iMapNo);
	iXPos += glbXAxisDelta[iDir];
	iYPos += glbYAxisDelta[iDir];
	ObjectID oObject; // si
	X16 iHeroType = 0; // bp0a
	for (oObject = GET_TILE_RECORD_LINK(iXPos, iYPos); oObject != OBJECT_END_MARKER; oObject = GET_NEXT_RECORD_LINK(oObject)) {
		Actuator *refActuator; // bp04
		if (oObject.DBType() == dbActuator && (refActuator = GET_ADDRESS_OF_ACTU(oObject))->ActuatorType() == ACTUATOR_x7E_TYPE_RESURECTOR) { // 0x7E
			iHeroType = refActuator->ActuatorData();
			break;
		}
		// SPX: Add for 0x7F :Activator, champion mirror
		else if (oObject.DBType() == dbActuator && (refActuator = GET_ADDRESS_OF_ACTU(oObject))->ActuatorType() == ACTUATOR_x7F_TYPE_CHAMPION_MIRROR) { // 0x7F
			iHeroType = refActuator->ActuatorData();
			break;
		}
	}
	iXPos += glbXAxisDelta[(iDir + 2) & 3];
	iYPos += glbYAxisDelta[(iDir + 2) & 3];
	REVIVE_PLAYER(iHeroType, iChampionNumber, iDir);
	if (iChampionNumber != 0) {
		DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
		UPDATE_RIGHT_PANEL(0);
	}
	cd.pi.glbNextChampionNumber = iChampionNumber + 1;
	cd.pi.glbChampionsCount++;
	if (iChampionNumber == 0)	// First champion, then select it as leader
		SELECT_CHAMPION_LEADER(0);
	X16 iWallDir = (iDir + 2) & 3; // bp06
	iXPos += glbXAxisDelta[iDir];
	iYPos += glbYAxisDelta[iDir];
	// Take items from wall and add them to player
	for (oObject = GET_TILE_RECORD_LINK(iXPos, iYPos); oObject != OBJECT_END_MARKER; oObject = GET_NEXT_RECORD_LINK(oObject)) {
		if (oObject.DBType() > dbActuator && oObject.Dir() == iWallDir) {
			ADD_ITEM_TO_PLAYER(iChampionNumber, oObject);
		}
	}
	iXPos += glbXAxisDelta[(iDir + 2) & 3];
	iYPos += glbYAxisDelta[(iDir + 2) & 3];
	if (glbChampionShowResurrect == 0) {
		INTERFACE_CHAMPION(iChampionNumber);
		_1031_0541(7);
		_38c8_0002();
	}
	CHANGE_CURRENT_MAP_TO(iCurrentMap);
	CALC_PLAYER_WEIGHT(iChampionNumber);
	return iChampionNumber + 1;
}


//^2E62:0CD8
// SPX: _2e62_0cd8 renamed SET_PARTY_HERO_FLAG
void SkWinCore::SET_PARTY_HERO_FLAG(U16 flagvalue)
{
	//^2E62:0CD8
	ENTER(0);
	//^2E62:0CDC
	for (U16 si = 0; si < cd.pi.glbChampionsCount; si++) {
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
	if (cd.mo.glbSpecialScreen == _MENU_SCREEN__RESUME_GAME_SELECT) {
		//^2F3F:0799
		ObjectID di = cd.pi.glbLeaderHandPossession.object;
		//^2F3F:079E
		if (di == OBJECT_NULL) {
			//^2F3F:07A3
			_4976_57de = 0xff;
			//^2F3F:07A8
			CHANGE_CURSOR_HAND_ITEM();
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
		for (si = 0; si < cd.pi.glbChampionsCount; si++, champion++) {
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
	//printf("cd.pi.glbLeaderHandPossession\n"); getch();
	//^2F3F:0862
	cd.pi.glbLeaderHandPossession.object = OBJECT_NULL;
	_4976_57de = 0xff;
	//^2F3F:086D
	// SPX: DM2 will check a RESURECTOR at 0,0 (for TORHAM), while DM/TQ expects one at 1,0 for THERON.

	// DM2 Original code : search for champion at 0,0 facing south
	ObjectID tObject = OBJECT_NULL;	// di
	for (tObject = GET_TILE_RECORD_LINK(0, 0); tObject != OBJECT_END_MARKER; tObject = GET_NEXT_RECORD_LINK(tObject)) {
		if (tObject.DBType() == dbActuator) {
			Actuator* xActuator = GET_ADDRESS_OF_ACTU(tObject);	// bp08
			if (xActuator->ActuatorType() == ACTUATOR_x7E_TYPE_RESURECTOR) { // 0x007E
				glbChampionShowResurrect = 1;
				// SPX: Automatic selection of champion (Thoram)
				SELECT_CHAMPION(0, 1, DIR_NORTH, cd.pi.glbPlayerMap);	// player is imaginarily at 0,1 facing north
				REVIVE_CHAMPION(0, 1, DIR_NORTH, cd.pi.glbPlayerMap, UI_EVENTCODE_xA0_REVIVE_CHAMPION);
				glbChampionShowResurrect = 0;
				glbChampionSquad[0].playerDir(U8(cd.pi.glbPlayerDir));
				glbChampionSquad[0].playerPos(U8(cd.pi.glbPlayerDir));
				SET_PARTY_HERO_FLAG(0x4000);
				SELECT_CHAMPION_LEADER(0);
				return;
			}
		}
	}

	// SPX: Second loop to check for DM/TQ at 1,0 : search for champion at 1,0 facing west
	if (SkCodeParam::bDM1Mode == true || SkCodeParam::bDM1TQMode == true) {
		for (tObject = GET_TILE_RECORD_LINK(1, 0); tObject != OBJECT_END_MARKER; tObject = GET_NEXT_RECORD_LINK(tObject)) {
			if (tObject.DBType() == dbActuator) {
				Actuator* xActuator = GET_ADDRESS_OF_ACTU(tObject);	// bp08
				if (xActuator->ActuatorType() == ACTUATOR_x7F_TYPE_CHAMPION_MIRROR) { // 0x007F
					glbChampionShowResurrect = 1;
					SELECT_CHAMPION(0, 0, DIR_EAST, cd.pi.glbPlayerMap);	// player is really at 0,0 facing east
					REVIVE_CHAMPION(0, 0, DIR_EAST, cd.pi.glbPlayerMap, UI_EVENTCODE_xA0_REVIVE_CHAMPION);
					glbChampionShowResurrect = 0;
					glbChampionSquad[0].playerDir(U8(cd.pi.glbPlayerDir));
					glbChampionSquad[0].playerPos(U8(cd.pi.glbPlayerDir));
					SET_PARTY_HERO_FLAG(0x4000);
					SELECT_CHAMPION_LEADER(0);
					return;
				}
			}
		}
	}

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
X16 SkWinCore::GET_CHAMPION_SPECIAL_FORCE(U16 iChampionIdx)
{
	ENTER(0);
	U16 iLocalChampIdx = iChampionIdx;	// si
	if (glbChampionSquad[iLocalChampIdx].curHP() == 0)
		return 0;
	if (((GET_PLAYER_WEIGHT(iLocalChampIdx) / 10) + (glbChampionSquad[iLocalChampIdx].heroFlag & CHAMPION_FLAG_0010_MALE)) != 0)	// 0x10
		return 50;
	return 40;
}

//^2C1D:201D
// SPX: _2c1d_201d renamed GET_PARTY_SPECIAL_FORCE
U16 SkWinCore::GET_PARTY_SPECIAL_FORCE()
{
	ENTER(0);
	U16 iTotalForce = 0;	// di
	U16 iChampIdx = 0;	// si
	for (iChampIdx = 0; iChampIdx < cd.pi.glbChampionsCount; iChampIdx++) {
		iTotalForce += GET_CHAMPION_SPECIAL_FORCE(iChampIdx);
	}
	return iTotalForce;
}



//^2C1D:1B0F
// SPX: _2c1d_1b0f renamed PROCESS_POISON
void SkWinCore::PROCESS_POISON(i16 iChampIdx, U16 iPoisonValue) {
	
	if (SkCodeParam::bUseIngameDebug)
	{
		U8 message[64];
		sprintf((char*)message, "PLAYER %d HAS %d POISON COUNTERS.\n", iChampIdx, iPoisonValue);
		DISPLAY_HINT_TEXT(glbChampionColor[iChampIdx], message);
	}
	ENTER(14);
	X16 iLocalPoisonValue = iPoisonValue;	// si
	if (iChampIdx == -1)
		return;
	if (iChampIdx + 1 == cd.pi.glbNextChampionNumber)
		return;

	Champion* xChampion = &glbChampionSquad[iChampIdx];	// bp04
	WOUND_PLAYER(iChampIdx, max_value(1, iLocalPoisonValue >> 6), 0, 0);
	xChampion->heroFlag |= CHAMPION_FLAG_0800;	// 0x800
	xChampion->heroFlag |= CHAMPION_FLAG_2000;	// 0x2000
	iLocalPoisonValue--;
	if (iLocalPoisonValue == 0)
		return;
	
	xChampion->PoisonValue++;

	Timer xTimer;	// bp0e
	xTimer.TimerType(C75_tty_PoisonChampion);
	xTimer.actor = (U8)iChampIdx;
	xTimer.SetMap(cd.pi.glbPlayerMap);
	xTimer.SetTick(glbGameTick + 36);
	xTimer.value = iLocalPoisonValue;
	QUEUE_TIMER(&xTimer);

	return;
}


//^2C1D:135D
// SPX: _2c1d_135d renamed CHAMPION_COMPUTE_SOME_SHIELD
X16 SkWinCore::CHAMPION_COMPUTE_SOME_SHIELD(i16 play, U16 ww)
{
	ENTER(8);
	Champion* xChampion = &glbChampionSquad[play];	//*bp04
	U16 bp06 = ww & 0x8000;
	if (bp06 != 0)
		ww &= 0x7fff;
	X16 bp08 = 0;
	i16 si;
	U16 di;
	for (si = 0; si <= 1; si++) {
		// SPX: Checking armour value
		di = QUERY_GDAT_DBSPEC_WORD_VALUE(xChampion->Possess(si), GDAT_ITEM_STATS_ARMOR_CLASS);
		if ((di & 0x8000) != 0) {
			bp08 = (((_2c1d_132c(di, bp06) + COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(play, si, 7)) * _4976_3fc8[RCJ(6,ww)]) << ((si == ww) ? 4 : 5)) +bp08;
		}
	}
	si = RAND16((GET_PLAYER_ABILITY(xChampion, abVit, 0) >> 3) +1);
	if (bp06 != 0)
		si >>= 1;
	if (glbChampionSquad[play].enchantmentAura == C02_ENCHANTMENT_PARTY_SHIELD) {	// == 2
		si += glbChampionSquad[play].enchantmentPower;
	}
	si += xChampion->handDefenseClass[0] + xChampion->handDefenseClass[1] + bp08;
	if (ww > 1) {
		di = QUERY_GDAT_DBSPEC_WORD_VALUE(xChampion->Possess(ww), GDAT_ITEM_STATS_ARMOR_CLASS);
		si = si +_2c1d_132c(di, bp06);
	}
	if ((xChampion->bodyFlag & (1 << ww)) != 0) {
		si = si -(RAND02() +8);
	}
	if (cd.pi.glbIsPlayerSleeping != 0)
		si >>= 1;
	return BETWEEN_VALUE(0, si >> 1, 0x64);
}



//^2C1D:18AA
U16 SkWinCore::WOUND_PLAYER(i16 iCharIdx, i16 iSourceAttackDamage, U16 iMask, U16 iAttackType)
{
	// CSBwin: TAG017068: i32 DamageCharacter(i32 chIdx,i32 damage,i16 mask,i16 P4)
	// ReDMCSB: int16_t F0321_CHAMPION_AddPendingDamageAndWounds_GetDamage(
	ENTER(12);
	i16 iLocalAttackDamage = iSourceAttackDamage;	// si

	if (iCharIdx == -1)
		return 0;
	if (iCharIdx + 1 == cd.pi.glbNextChampionNumber)
		return 0;
	if (_4976_4c26 != 0)
		return 0;
	if (iLocalAttackDamage <= 0)
		return 0;

	Champion* xChampion = &glbChampionSquad[iCharIdx];
	if (xChampion->curHP() == 0)
		return 0;

	U16 iCanUseSharpDefense = iAttackType & MASK0x8000_USE_SHARP_DEFENSE;	// bp0a
	iAttackType &= 0x7fff;
	if (iAttackType != 0) {
		X16 iWoundCount = 0;	// bp08
		i16 iBodySlot = 0;	// bp06
		i16 iWisdomFactor = 0;	// bp06
		i16 iAdjustedAttack = 0;	// bp06
		U16 iDefense = 0;	// di
		for (iBodySlot = C00_INVENTORY_BODYPART_FIRST; iBodySlot <= C05_INVENTORY_BODYPART_LAST; iBodySlot++) {	// check body parts for wounds
			if (((1 << iBodySlot) & iMask) != 0) {
				iWoundCount++;
				iDefense += CHAMPION_COMPUTE_SOME_SHIELD(iCharIdx, ((iAttackType == C4_ATTACK_SHARP) ? MASK0x8000_USE_SHARP_DEFENSE : MASK0x0000_DO_NOT_USE_SHARP_DEFENSE) | iBodySlot);
			}
		}
		if (iWoundCount != 0) {
			iDefense = iDefense / iWoundCount;
		}
		X16 iDefenseReady = 0;	// bp0c
		iBodySlot = 0;
		for (iBodySlot = C00_INVENTORY_HAND_RIGHT; iBodySlot <= C01_INVENTORY_HAND_LAST; iBodySlot++) {
			if (xChampion->handCommand[iBodySlot] == 1) {
				iDefenseReady += xChampion->handDefenseClass[iBodySlot];
			}
		}
		if (iDefenseReady != 0 ) {
			if (QUERY_PLAYER_SKILL_LV(iCharIdx, SKILL_FIGHTER_PARRY, 1) +(iDefenseReady >> 3) > (RAND() & 15)) {
				if (iCanUseSharpDefense != 0 && (iLocalAttackDamage = iLocalAttackDamage - iDefenseReady) <= 0)
					return 0;
				iDefense += iDefenseReady >> 2;
			}
		}
		switch (iAttackType) {
		case C6_ATTACK_PSYCHIC://^19EC
			iWisdomFactor = 115 - GET_PLAYER_ABILITY(xChampion, 0, 0);
			if (iWisdomFactor <= 0)
				return 0;
			iLocalAttackDamage = GET_SCALED_PRODUCT(iLocalAttackDamage, 6, iWisdomFactor);
			goto _processNoScaledDefense;
		case C5_ATTACK_MAGIC://^1A20
			iLocalAttackDamage = USE_ABILITY_ATTRIBUTE(xChampion, abAntiMagic, iLocalAttackDamage);
			if (xChampion->enchantmentAura == C01_ENCHANTMENT_SPELL_SHIELD)	// == 1
				iLocalAttackDamage -= xChampion->enchantmentPower;
			goto _processNoScaledDefense;
		case C1_ATTACK_FIRE://^1A44
			iLocalAttackDamage = USE_ABILITY_ATTRIBUTE(xChampion, abAntiFire, iLocalAttackDamage);
			if (xChampion->enchantmentAura == C00_ENCHANTMENT_FIRE_SHIELD)	// == 0
				iLocalAttackDamage -= xChampion->enchantmentPower;
			break;
		case C2_ATTACK_SELF://^1A68
		case C8_ATTACK_UNKNOWN://^1A68
			iDefense >>= 1;
			iDefense += QUERY_PLAYER_SKILL_LV(iCharIdx, SKILL_NINJA_GLOBAL, 1);
			break;
		case C3_ATTACK_BLUNT://^1A7A
		case C4_ATTACK_SHARP://^1A7A
		case C7_ATTACK_LIGHTNING://^1A7A
			break;
		}

		if (iLocalAttackDamage <= 0)
			return 0;
		iLocalAttackDamage = GET_SCALED_PRODUCT(iLocalAttackDamage, 6, 130 - iDefense);	// scale = 6 => divide / 64
_processNoScaledDefense:	// _1a91:
		if (iLocalAttackDamage <= 0)
			return 0;
		iAdjustedAttack = USE_ABILITY_ATTRIBUTE(xChampion, abVit, (RAND() & 0x7f) +10);
		if (iAdjustedAttack < iLocalAttackDamage) {
			do {
				glbChampionsBodyFlags[iCharIdx] |= (1 << (RAND() & 7)) & iMask;
				iAdjustedAttack <<= 1;
				if (iAdjustedAttack >= iLocalAttackDamage)
					break;
			} while (iAdjustedAttack != 0);
		}
		if (cd.pi.glbIsPlayerSleeping != 0)
			RESUME_FROM_WAKE();
	}
	cd.pi.glbChampionsPendingDamage[iCharIdx] += iLocalAttackDamage;
	return iLocalAttackDamage;
}


//^2C1D:1C7C
void SkWinCore::ADJUST_STAMINA(U16 iCharIdx, i16 iStaminaDecrement)
{
	// CSBwinSimilarity: TAG01742a,AdjustStamina
	// ReDMCSB: F0325_CHAMPION_DecrementStamina?
	ENTER(4);
	X16 iLocalChampIndex = iCharIdx;
	if (iLocalChampIndex == 0xffff)
		return;
	Champion* xChampion = &glbChampionSquad[iLocalChampIndex];	//*bp04
	xChampion->curStamina(xChampion->curStamina() - iStaminaDecrement);
	i16 iChampStamina = xChampion->curStamina();	// si
	if (iChampStamina <= 0) {
		xChampion->curStamina(0);
		WOUND_PLAYER(iLocalChampIndex, (-iChampStamina) >> 1, 0, 0);
	}
	else if (xChampion->maxStamina() < iChampStamina) {
		xChampion->curStamina(xChampion->maxStamina());
	}
	if (ABS16(iStaminaDecrement) >= 10) {
		xChampion->heroFlag |= CHAMPION_FLAG_0800;	// 0x800	// SPX: flag 0x800 whereas DM/CSB has flag 0x200 + 0100. Is this different ?
		// pcA3->charFlags |= CHARFLAG_possession(0x200)| CHARFLAG_statsChanged(0x100);
	}
	return;
}
//^2C1D:203D
U16 SkWinCore::GET_PLAYER_WEIGHT(U16 iCharIdx)
{
	// Returns player's weight. Also include weight of leader's hand item.

	ENTER(0);
	X16 iChampWeight = 0; // si
	X16 iLocalChampIndex = iCharIdx;
	if (glbChampionSquad[iLocalChampIndex].curHP() == 0)
		return 0;
	iChampWeight = glbChampionSquad[iLocalChampIndex].curWeight();
	if (iLocalChampIndex == glbChampionLeader) {
		iChampWeight += glbLeaderItemWeight;
	}
	return iChampWeight;
}
//^2C1D:0FFC
U16 SkWinCore::MAX_LOAD(Champion* xChampion)
{
	// CSBwinSimilarity: TAG016508,MaxLoad(CHARDESC *pChar)

	ENTER(0);
	X16 iMaxLoad = (GET_PLAYER_ABILITY(xChampion, abStr, 0) << 3) + 100;	// si
	iMaxLoad = STAMINA_ADJUSTED_ATTR(xChampion, iMaxLoad);
	X16 iBodyFlag = xChampion->bodyFlag;	// di
	if (iBodyFlag != 0) {
		iMaxLoad -= iMaxLoad << (((iBodyFlag & CHAMPION_FLAG_0010_MALE) != 0) ? 2 : 3);
	}
	iMaxLoad += 9;
	iMaxLoad -= iMaxLoad % 10;
	return iMaxLoad;
}

//^2C1D:153F
void SkWinCore::_2c1d_153f(X16 xx)
{
	ENTER(6);
	Champion* xChampion = &glbChampionSquad[xx]; // bp04
	i16 iChampHP = xChampion->curHP();	// si
	U16 bp06 = 0;
	xChampion->curHP(0);
	if (xChampion->playerPos() != 0xFF) {
		if (GET_PLAYER_AT_POSITION(xChampion->playerPos()) == -1)
			goto _15a9;
	}
	for (; GET_PLAYER_AT_POSITION((bp06 + cd.pi.glbPlayerDir) & 3) != -1; bp06++) {
	}
	xChampion->playerPos((bp06 + cd.pi.glbPlayerDir) & 3);
_15a9:
	xChampion->playerDir(cd.pi.glbPlayerDir);
	xChampion->curHP(iChampHP);
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
		for (si = 0; si < C30_INVENTORY_MAX_SLOT; si++) {
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
		_443c_040e_MOUSE();
		//^2F3F:0786
		return;
//	}
//	else if (SkCodeParam::bUseFixedMode)	// 
//	{
//	// SPX: Code added from TAG014676 in CSBWin!!!
//		i16 player = xx;
//		U16 hpmax;
//		U16 hpnew;
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
void SkWinCore::DROP_PLAYER_ITEMS(U16 iChampionIdx)
{
	ENTER(2);
	X16 iQuarterPos = glbChampionSquad[iChampionIdx].playerPos();	// di
	X16 iInvSlot;	// si
	for (iInvSlot = 0; iInvSlot < C30_INVENTORY_MAX_SLOT; iInvSlot++) {
		ObjectID oChampItem = REMOVE_POSSESSION(iChampionIdx, tblDropChampionInventory[iInvSlot]);	// bp02
		if (oChampItem != OBJECT_NULL) {
			MOVE_RECORD_TO(ObjectID(oChampItem, iQuarterPos), -1, 0, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY);
		}
	}
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
		QUEUE_NOISE_GEN1(GDAT_CATEGORY_x16_CHAMPIONS, champion->HeroType(), SOUND_CHAMPION_SCREAM, 0x61, 0x80, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1);
	}
#endif

	if (cd.pi.glbChampionIndex -1 == di)
		//^2C1D:15DB
		DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
	//^2C1D:15E0
	champion->curHP(0);
	champion->herob44 = champion->handCooldown[C01_INVENTORY_HAND_LEFT] = champion->handCooldown[C00_INVENTORY_HAND_RIGHT] = 0;
	champion->heroFlag |= CHAMPION_FLAG_4000;	// 0x4000
	if (di +1 == glbChampionInventory) {
		//^2C1D:1606
		if (glbShowItemStats != 0) {
			//^2C1D:160D
			glbShowItemStats = 0;
			FIRE_MOUSE_RELEASE_CAPTURE();
			if (cd.pi.glbLeaderHandPossession.object != OBJECT_NULL)
				//^2C1D:161F
				DISPLAY_TAKEN_ITEM_NAME(cd.pi.glbLeaderHandPossession.object);
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
	ObjectID bp0c = ALLOC_NEW_RECORD(0x800A); // bp0C
	if (bp0c != OBJECT_NULL) {
		//^2C1D:16A0
		Miscellaneous_item *xItem = GET_ADDRESS_OF_RECORDA(bp0c); // bp08
		xItem->ItemType(GET_CHAMPION_BONES_ITEM_ID()); // SPX: changed hardcoded 0 to function to get proper Bones ID depending on DM2 or DM1 mode
		xItem->Important(1);
		xItem->Bone(di);
		MOVE_RECORD_TO(ObjectID(bp0c, bp0a), -1, 0, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY);
	}
	//^2C1D:16F1
	FIRE_HIDE_MOUSE_CURSOR();
	champion->RuneCnt(0);
	champion->GetRunes()[0] = 0;
	champion->playerDir(U8(cd.pi.glbPlayerDir));
	champion->herob41 = 0;
	if (((bp0a + 4 - cd.pi.glbPlayerDir) & 3) == _4976_5dbc)
		CHAMPION_SQUAD_RECOMPUTE_POSITION();
	if (champion->PoisonValue != 0)
		CURE_POISON(di);
#if (XDMX_FEATURE_PLAGUE == 1)
	if (champion->PlagueValue != 0)
		CURE_PLAGUE(di);
#endif
	//^2C1D:1739
	X16 si;
	for (si = 0; si < cd.pi.glbChampionsCount && glbChampionSquad[si].curHP() == 0; si++);
	if (si == cd.pi.glbChampionsCount) {
		cd.pi.glbPlayerDefeated = 1;
		_1031_098e();
	}
	else if (di == glbChampionLeader) {
		SELECT_CHAMPION_LEADER(si);
	}
	_1031_0667();
	FIRE_SHOW_MOUSE_CURSOR();
	return;
}

//^2C1D:1782
void SkWinCore::PROCESS_PLAYERS_DAMAGE()
{
	// CSBwinSimilarity: TAG016e54,DisplayCharacterDamage

	ENTER(16);
	if (SkCodeParam::bChampionImmune)
		return;

	Champion* champion = glbChampionSquad;
	for (U16 championIndex = 0; championIndex < cd.pi.glbChampionsCount; championIndex++, champion++) {
		//^2C1D:1798
		champion->bodyFlag = glbChampionsBodyFlags[championIndex];
		glbChampionsBodyFlags[championIndex] = 0;
		X16 di = cd.pi.glbChampionsPendingDamage[championIndex];
		if (di == 0)
			continue;
		//^2C1D:17C5
		cd.pi.glbChampionsPendingDamage[championIndex] = 0;
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
			bp10.SetMap(cd.pi.glbPlayerMap);
			bp10.SetTick(glbGameTick +5);
			bp10.actor = U8(championIndex);
			champion->timerIndex = QUEUE_TIMER(&bp10);
		}
		else {
			//^2C1D:1852
			glbTimersTable[si].SetMap(cd.pi.glbPlayerMap);
			glbTimersTable[si].SetTick(glbGameTick + 5);
			TIMER_3a15_05f7(si);
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
	if (cd.pi.glbChampionsCount == 0)
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
	for (; bp0e < cd.pi.glbChampionsCount; bp0e++, champion++) {
		//^2C1D:213D
		if (champion->curHP() == 0 || bp0e + 1 == cd.pi.glbNextChampionNumber)
			continue;
		if (champion->curMP() < champion->maxMP()) {
			//^2C1D:2164
			bp0a = QUERY_PLAYER_SKILL_LV(bp0e, SKILL_WIZARD_GLOBAL, 1) + QUERY_PLAYER_SKILL_LV(bp0e, SKILL_PRIEST_GLOBAL, 1);
			if (GET_PLAYER_ABILITY(champion, abWiz, 0) +bp0a > glbChampionTable[0].enchantmentPower) {
				//^2C1D:21A4
				X16 si = champion->maxMP() / 40;
				si++;
				if (cd.pi.glbIsPlayerSleeping != 0)
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
		bp0a = U16(glbGameTick) -U16(glbGameTick_4b80);
		if (bp0a > 0x50) {
			//^2C1D:2252
			di++;
			if (bp0a > 0xfa)
				di++;
		}
		//^2C1D:225B
		if (cd.pi.glbIsPlayerSleeping != 0)
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
			if (cd.pi.glbIsPlayerSleeping != 0)
				di <<= 1;
			//^2C1D:239A
			// SPX : This is the regeneration function
			champion->curHP(champion->curHP() +min_value(di, champion->maxHP() -champion->curHP()));
		}
		//^2C1D:23B5
		if ((glbGameTick & ((cd.pi.glbIsPlayerSleeping != 0) ? 0x3f : 0xff)) == 0) {
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
	if (cd.pi.glbLeaderHandPossession.object != OBJECT_NULL && (QUERY_GDAT_DBSPEC_WORD_VALUE(cd.pi.glbLeaderHandPossession.object, 0)&0x8000) != 0 && (bp01 = GET_ITEM_ICON_ANIM_FRAME(cd.pi.glbLeaderHandPossession.object, -1, 1)) != _4976_57de) {
		//^2E62:0DB8
		DRAW_ITEM_IN_HAND(&cd.pi.glbLeaderHandPossession);
		CHANGE_CURSOR_HAND_ITEM();
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
	X16 iLocalChampionCount = cd.pi.glbChampionsCount;
	if (cd.pi.glbNextChampionNumber != 0)
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
	for (U16 si = 0; si < cd.pi.glbChampionsCount; si++) {
		//^12B4:0126
		glbChampionSquad[si].playerDir(U8(cd.pi.glbPlayerDir));
	}
	//^12B4:013E
	return;
}

//^12B4:00AF
// SPX TODO related to rotate squad
//void SkWinCore::_12b4_00af(U16 xx)	// xx = lead / 0 = going down / 4 = going up
// SPX: in order to handle more custom behaviour, signature is changed
void SkWinCore::_12b4_00af(U8 iStairsLead, U8 iStairsDir)
{
	U16 xx = iStairsLead;	// SPX: new
	U16 iCurTileDir = 0;	// SPX: new

	ENTER(0);
	if (SkCodeParam::bBWMode) {
		iCurTileDir = _0cee_06dc_GET_TILE_DIRECTION(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY);
		glbPlayerSpecialStairsDir = (iCurTileDir + 2)%4;
	}

	MOVE_RECORD_TO(OBJECT_NULL, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, -1, 0);
	glbMapToLoad = LOCATE_OTHER_LEVEL(cd.pi.glbPlayerMap, (xx != 0) ? -1 : +1, &cd.pi.glbPlayerPosX, &cd.pi.glbPlayerPosY, NULL);
	glbPlayerSpecialStairsDir = 0xFFFF;	// Special BW
	CHANGE_CURRENT_MAP_TO(glbMapToLoad);
	ROTATE_SQUAD(_0cee_06dc_GET_TILE_DIRECTION(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY));
	CHANGE_CURRENT_MAP_TO(cd.pi.glbPlayerMap);

	return;
}



//^2C1D:09D9
U16 SkWinCore::_2c1d_09d9()
{
	//^2C1D:09D9
	ENTER(4);
	//^2C1D:09DF
	X32 bp04 = 0;
	U16 di;
	for (di = 0; di < cd.pi.glbChampionsCount; di++) {
		//^2C1D:09ED
		U16 si;
		for (si = 0; si < SKILL_MAJOR_COUNT; si++) {	// (si = 0; si <= 3; si++) 
			//^2C1D:09F1
			bp04 += glbChampionSquad[di].skills[si];
			//^2C1D:0A0F
		}
		//^2C1D:0A15
	}
	//^2C1D:0A1C
	X16 si;
	for (si = 1; bp04 >= 0x200; ) {
		//^2C1D:0A21
		bp04 = bp04 >> 1;
		si++;
		//^2C1D:0A32
	}
	//^2C1D:0A41
	return si;
}



//^2C1D:1FB1
// _2c1d_1fb1 renamed _2c1d_1fb1_CHAMPION_LEADER_SHOOT_DIR
U16 SkWinCore::_2c1d_1fb1_CHAMPION_LEADER_SHOOT_DIR(U16 dir)
{
	ENTER(0);
	if (glbChampionLeader == -1)
		return 0;
	return _2c1d_1de2_CHAMPION_SHOOT(glbChampionLeader, -1, dir);
}

//^121E:0351
// _121e_0351 renamed _121e_0351_THROW_LEFT_OR_RIGHT
U16 SkWinCore::_121e_0351_THROW_LEFT_OR_RIGHT(U16 xx, U16 yy)
{
	//^121E:0351
	ENTER(0);
	//^121E:0356
	U16 si = 0;
	//^121E:0358
	U16 di = (_4976_4dda == 17) ? 768 : 765; // rectangles ??
	//^121E:0367
	if (_098d_02a2(di, xx, yy) != 0) {
		//^121E:037A
		si = _2c1d_1fb1_CHAMPION_LEADER_SHOOT_DIR(0); // throw by left hand
	}
	//^121E:037E
	else if (_098d_02a2(di +1, xx, yy) != 0) {
		//^121E:0394
		si = _2c1d_1fb1_CHAMPION_LEADER_SHOOT_DIR(1); // throw by right hand
	}
	//^121E:039E
	if (si != 0)
		//^121E:03A2
		cd.gg.glbRefreshViewport = 1;
	//^121E:03A8
	return si;
}



//..............................................................................

//^2C1D:0F67
// SPX: _2c1d_0f67 renamed USE_LUCK_ATTRIBUTE
U16 SkWinCore::USE_LUCK_ATTRIBUTE(Champion* xChampion, U16 xx)
{
	ENTER(0);
	U16 iLuckVal = xx; // si

	if (RAND01() != 0 && RAND16(100) > iLuckVal) {
		return 1;
	}
	iLuckVal = (RAND16(GET_PLAYER_ABILITY(xChampion, ATTRIBUTE_LUCK, 0) << 1) > iLuckVal) ? 1 : 0;
	xChampion->attributes[ATTRIBUTE_LUCK][ATTRIBUTE_CURRENT] = 
		U8(BETWEEN_VALUE(10, xChampion->attributes[ATTRIBUTE_LUCK][ATTRIBUTE_CURRENT] + ((iLuckVal != 0) ? -2 : 2), min_value(220, GET_PLAYER_ABILITY(xChampion, abLuck, 1))));
	return iLuckVal;
}

//^2C1D:111E
// SPX: _2c1d_111e renamed USE_DEXTERITY_ATTRIBUTE
U16 SkWinCore::USE_DEXTERITY_ATTRIBUTE(U16 iHeroIndex)
{
	ENTER(4);
	Champion* xChampion = &glbChampionSquad[iHeroIndex];
	i16 iDexterityVal = (RAND() & 7) + GET_PLAYER_ABILITY(xChampion, abDex, 0); // si
	iDexterityVal -= (GET_PLAYER_WEIGHT(iHeroIndex) * i32(iDexterityVal >> 1)) / MAX_LOAD(xChampion);
	iDexterityVal = max_value(2, iDexterityVal);
	if (cd.pi.glbIsPlayerSleeping != 0)
		iDexterityVal >>= 1;
	return BETWEEN_VALUE((RAND() & 7) + 1, (iDexterityVal >> 1), 100 - (RAND() & 7));
}

//^2C1D:0F2A
// _2c1d_0f2a renamed USE_ABILITY_ATTRIBUTE
i16 SkWinCore::USE_ABILITY_ATTRIBUTE(Champion* xChampion, X16 iAbility, i16 tt)
{
	ENTER(0);
	i16 iAbilityVal = ATTRIBUTE_VALUE_THRESHOLD - GET_PLAYER_ABILITY(xChampion, iAbility, 0);	// 170 - Get_player_ab()
	return (iAbilityVal < 0x10) ? (tt >> 3) : (GET_SCALED_PRODUCT(tt, 7, iAbilityVal));
}


//..............................................................................
