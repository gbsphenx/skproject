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
		iTotalWeight += QUERY_ITEM_WEIGHT(tblChampionSquad[player].inventory[iInvSlot]);
	}
	if (_4976_5336 - 1 == player && glbSelectedHandAction < 2) {
		if (IS_CONTAINER_CHEST(_4976_3de6[RCJ(4,player)][RCJ(2,glbSelectedHandAction)]) != 0) {
			for (U16 iContainerSlot = 0; iContainerSlot < 8; iContainerSlot++) {
				iTotalWeight += QUERY_ITEM_WEIGHT(glbCurrentContainerItems[iContainerSlot]);
			}
		}
	}
	tblChampionSquad[player].curWeight(iTotalWeight);
	tblChampionSquad[player].heroFlag |= CHAMPION_FLAG_1000;	// 0x1000
}


//^2C1D:0300
// SPX: _2c1d_0300 replaced by BOOST_ATTRIBUTE, called when consuming potion or special item
void SkWinCore::BOOST_ATTRIBUTE(Champion *ref, U16 xx, U16 yy)
{
	ENTER(4);
	i16 di = yy;
	U8 *bp04 = ref->attributes[xx];
	i16 si = bp04[0] +di -bp04[1];
	if (((si < 0) ? 1 : 0) == (di < 0) ? 1 : 0) {
		si = ABS16(si);
		while (si > 20) {
			di -= di / 4;
			si -= 20;
		}
	}
	bp04[0] = BETWEEN_VALUE(10, bp04[0] +di, 220);
	return;
}

//^2C1D:03E7
// SPX: _2c1d_03e7 renamed PROCESS_ITEM_BONUS / Function called when engine looks through possessed items to give bonus
// SPX: Might check CSBWin AdjustStatsForItemCarried to handle CURSED state
void SkWinCore::PROCESS_ITEM_BONUS(i16 player, ObjectID recordLink, i16 inventorySlot, i16 x4)
{
	// SPX x4 = -1 when removed, 1 when added, 2 when consumed, 0 when done from SKRead
	ObjectID di = recordLink;
	i16 si = x4;
	i16 iBonusModifier = 0; // SPX: Added as a fix instead of using unsigned bp06
	if (player < 0)
		return;
	if (di == OBJECT_NULL)
		return;
	if (inventorySlot < C30_INVENTORY_MAX_SLOT) {	// (possess < 30)
		U16 bp06 = QUERY_GDAT_DBSPEC_WORD_VALUE(di, GDAT_ITEM_STATS_GEN_FLAGS);
		// SPX: 00/00/00 => flags, 0x0010 = item providing light, so must recompute light
		if (si != 0 && (bp06 & ITEM_FLAG_PRODUCE_LIGHT) != 0) {
			RECALC_LIGHT_LEVEL();
		}
		if ((bp06 & ITEM_FLAG_2000) == 0) {
			Champion *champion = &tblChampionSquad[player]; //*bp04 
			U16 bp0a = IS_ITEM_FIT_FOR_EQUIP(di, inventorySlot, 1);
			U16 bp0c = 0;
			if (si != 0 && si != 3 && si != -2) {
				// SPX: 0x14 MANA BONUS
				iBonusModifier = RETRIEVE_ITEM_BONUS(di, GDAT_ITEM_BONUS_MANA, bp0a, si);
				if (iBonusModifier != 0) {
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
							champion->manaMax += iBonusModifier;
					}
					else {
						if (si == 2) {
							champion->curMP(BETWEEN_VALUE(0, champion->curMP() + iBonusModifier, MAXMP_BONUS));
						}
					}
					champion->heroFlag |= CHAMPION_FLAG_0800;	// 0x0800
				}
				// SPX: Bonus 0x15 => 0x1B check for boosting attributes (x15 luck to x1B antifire)
				for (U16 attributeIndex = 0; attributeIndex < ATTRIBUTE_COUNT; attributeIndex++) {
					iBonusModifier = RETRIEVE_ITEM_BONUS(di, attributeIndex + GDAT_ITEM_BONUS_ATTRIBUTE_FIRST, bp0a, si);
					if (iBonusModifier != 0) {
						if (si == 1 || si == -1) {
							champion->attributesEnhanced[attributeIndex] = champion->attributesEnhanced[attributeIndex] + iBonusModifier;
						}
						else {
							BOOST_ATTRIBUTE(champion, attributeIndex, iBonusModifier);
						}
						champion->heroFlag |= CHAMPION_FLAG_3000;	// 0x3000
					}
				}
			}
			// SPX: Bonus 0x1E => 0x31 , 20 values = 4*5 skills
			for (U16 skillIndex = 0; skillIndex < SKILL_COUNT; skillIndex++) {
				iBonusModifier = RETRIEVE_ITEM_BONUS(di, skillIndex + GDAT_ITEM_BONUS_SKILL_FIRST, bp0a, si);
				if (bp06 != 0) {
					champion->skillBonus[skillIndex] = champion->skillBonus[skillIndex] + iBonusModifier;
					champion->heroFlag |= CHAMPION_FLAG_2000;	// 0x2000
					bp0c = 1;
				}
			}
			// SPX: Bonus 0x33 : SPEED ?
			iBonusModifier = RETRIEVE_ITEM_BONUS(di, GDAT_ITEM_BONUS_WALK_SPEED, bp0a, si);
			if (iBonusModifier != 0) {
				champion->walkSpeed = champion->walkSpeed + iBonusModifier;
				bp0c = 1;
			}
			// SPX: Bonus 0x32 : LIGHT
			if (si != 2 && si != -1 && si != 3) {
				iBonusModifier = RETRIEVE_ITEM_BONUS(di, GDAT_ITEM_BONUS_LIGHT, bp0a, si);
				if (bp06 != 0) {
					glbGlobalSpellEffects.Light += iBonusModifier;
					if (si != 0) {
						RECALC_LIGHT_LEVEL();
					}
				}
			}
			if (bp0c != 0 && si == 2) {
				Timer bp16;
				bp16.TimerType(ttyItemBonus);
				bp16.SetMap(cd.pi.glbPlayerMap);
				bp16.SetTick(QUERY_GDAT_DBSPEC_WORD_VALUE(di, 0x0013) + glbGameTick);
				bp16.actor = (Bit8u)player;
				bp16.value = di.DBType();
				bp16.w8 = QUERY_CLS2_FROM_RECORD(di);
				QUEUE_TIMER(&bp16);
			}
		}
	}
	if (si != 0) {
		CALC_PLAYER_WEIGHT(player);
	}
	return;
}

//^2C1D:07A3
ObjectID SkWinCore::REMOVE_POSSESSION(U16 iChampionIdx, U16 possess)
{
	// CSBwinSimilarity: TAG015a66,RemovePossession

	ENTER(4);
	U16 iInvSlot = possess;	// si
	Champion* xChampion = &tblChampionSquad[iChampionIdx];	// bp04
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
		Champion* xChampion = &tblChampionSquad[iChampionID];
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
				IS_ITEM_HAND_ACTIVABLE(iLocalChampIdx, tblChampionSquad[iLocalChampIdx].Possess(glbSelectedHandAction), glbSelectedHandAction);
			}
		}
	}
	return bp02;
}

//^2759:0FB0
void SkWinCore::LOAD_PROJECTILE_TO_HAND(U16 iChampionIdx, i16 hand)
{
	ENTER(14);
	Champion* xChampion = &tblChampionSquad[iChampionIdx];	//*bp04
    xChampion->handCooldown[hand] = 0;
	if (xChampion->curHP() == 0 || hand >= 2)
		return;
	i16 iOtherHand = hand ^ 1;	// bp0e
	i16 bp0a = hand;	// bp0a
	U16 bp0c = xChampion->handCommand[hand];
	xChampion->handCommand[hand] = 0xFF;
	xChampion->handDefenseClass[hand] = 0;
	if (bp0c == CmLaunchProjectile) {	// 32
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
// SPX: _2759_01fe renamed COMMAND_ON_MAGIC_MAP
U16 SkWinCore::COMMAND_ON_MAGIC_MAP(U16 player, ObjectID recordLink, U16 cmdNum)
{
    ENTER(8);
	ObjectID di = recordLink;
	U16 si = cmdNum;
	if (di == OBJECT_NULL) {
		return 0;
	}
	if (di.DBType() == dbContainer) {
		Container *bp04 = GET_ADDRESS_OF_RECORD9(di);
		if (bp04->ContainerType() == 1) {
			U16 bp08 = bp04->b5_5_7();
			if (false
				|| bp08 == 1 // 1 - SCOUT MAP?
				|| bp08 == 2 // 2 - MINION MAP?
			) {
				if (GET_MISSILE_REF_OF_MINION(bp04->GetContainedObject(), di) != NULL) {
					if (si == CmKillMinion)
						return 1;
					if (bp08 != 2)
						return 0;
					Bit8u bp05 = GET_ADDRESS_OF_RECORD4(bp04->GetContainedObject())->CreatureType();
					if (bp05 == 51) { // 51 - FETCH MINION
						if (si != CmCallCarry)
							return 0;
						return 1;
					}
					if (bp05 != 50) // 50 - CARRY MINION
						return 0;
					if (si != CmCallFetch)
						return 0;
					return 1;
				}
				if (si != CmCallScout && si != CmMark && (bp04->w6 == 0xffff || (si != CmCallCarry && si != CmCallFetch))) {
					return 0;
				}
			}
			else {
				return 0;
			}
		}
	}
	return 1;
}


//^2759:04C0
// SPX: _2759_04c0 renamed FIND_POUCH_OR_SCABBARD_POSSESSION_POS
i16 SkWinCore::FIND_POUCH_OR_SCABBARD_POSSESSION_POS(i16 iChampionIdx, i16 iIsScabbardOrPouch)	// yy = pouch or scabbard
{
	ENTER(0);
	i16 iLChampIdx = iChampionIdx;	// di
	if (iIsScabbardOrPouch == 1) {	// SCABBARD
		if (tblChampionSquad[iLChampIdx].Possess(C12_INVENTORY_SCABBARD_1) != OBJECT_NULL) {	// Possess(12)
			return C12_INVENTORY_SCABBARD_1;	// 12
		}
		//for (i16 si = 7; si <= 9; si++) {
		for (i16 iInvSlot = C07_INVENTORY_SCABBARD_2; iInvSlot <= C09_INVENTORY_SCABBARD_4; iInvSlot++) {	// si
			if (tblChampionSquad[iLChampIdx].Possess(iInvSlot) != OBJECT_NULL) {
				return iInvSlot;
			}
		}
	}
	else if (iIsScabbardOrPouch == 0) {	// POUCH
		if (tblChampionSquad[iLChampIdx].Possess(C11_INVENTORY_POUCH_1) != OBJECT_NULL) {	// Possess(11)
			return C11_INVENTORY_POUCH_1;	// 11
		}
		if (tblChampionSquad[iLChampIdx].Possess(C06_INVENTORY_POUCH_2) != OBJECT_NULL) {	// Possess(6)
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
	Champion* xChampion = &tblChampionSquad[player]; //*bp04
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
	ENTER(14);
	ObjectID si = recordLink;
	Bit8u iCategory;	// bp01
	Bit8u iItemIndex;	// bp02
	if (recordLink == OBJECT_NULL) {
		iCategory = GDAT_CATEGORY_x16_CHAMPIONS;	// 0x16
        iItemIndex = tblChampionSquad[player].HeroType();
	}
	else {
		if (IS_CONTAINER_MONEYBOX(si) != 0 || IS_CONTAINER_CHEST(si) != 0)
			return 1;
		iCategory = QUERY_CLS1_FROM_RECORD(si);
		iItemIndex = QUERY_CLS2_FROM_RECORD(si);
	}
	i16 di = 0;
	// SPX: scans through champion basic commands : 8 = PUNCH, 9 = KICK, 10 = POUCH, 11 = SCABBARD
	for (Bit8u iEntryNumber = 8; iEntryNumber < 12; iEntryNumber++) {	// Bit8u bp03
		if (di >= 3)
			break;
		if (QUERY_GDAT_ENTRY_IF_LOADABLE(iCategory, iItemIndex, dtText, iEntryNumber) == 0)
			continue;
		U16 bp0a = QUERY_CMDSTR_ENTRY(iCategory, iItemIndex, iEntryNumber, CnCM_Command);	// CnCM_Command = Command
		if (bp0a == 0)
			continue;
		U16 bp0e = QUERY_CMDSTR_ENTRY(iCategory, iItemIndex, iEntryNumber, CnWH_Where);	// CnWH_Where = where (pouch or scabbard?)
		if (bp0e != 0 && bp0e -1 != yy)
			continue;
		if (si != OBJECT_NULL) {
			if (COMMAND_ON_MAGIC_MAP(player, si, bp0a) == 0)
				continue;
			i16 bp0c = QUERY_CMDSTR_ENTRY(iCategory, iItemIndex, iEntryNumber, CnNC_NumberCharge);	// NC = Number of charges consumed by action
			if (SkCodeParam::bUnlimitedCharges == true)
				bp0c = 0;
			if (bp0c == 18) {	// ?
				if (ADD_ITEM_CHARGE(si, 0) == 0) {
					goto _0425;
				}
				continue;
			}
			if (bp0c == 16 || bp0c == 17)
				bp0c = 1;
			if (bp0c == 0)
				goto _0425;
			if (ADD_ITEM_CHARGE(si, 0) >= bp0c)
				goto _0425;
			continue;
		}
		if (bp0a == CmPouch) {	// (bp0a == 17)
			if (FIND_POUCH_OR_SCABBARD_POSSESSION_POS(player, yy) < 0)
				continue;
		}
_0425:
		U16 bp08 = QUERY_CMDSTR_ENTRY(iCategory, iItemIndex, iEntryNumber, CnSK_Skill);
		U16 bp06 = QUERY_CMDSTR_ENTRY(iCategory, iItemIndex, iEntryNumber, CnLV_Level);
		if (QUERY_PLAYER_SKILL_LV(player, bp08, 1) >= bp06) {
			glbItemSelected[di].category = iCategory;
			glbItemSelected[di].index = iItemIndex;
			glbItemSelected[di].entry = iEntryNumber;
			di++;
		}
	}
	glbCommandSlotsNum = di;
	glbChampionItemInUse = si;
	if (IS_CONTAINER_MAP(si) != 0)
		return 1;
	if (di > 0)
		return 1;
	return 0;
}



//^1031:007B
// _1031_007b renamed _1031_007b
U16 SkWinCore::_1031_007b_PFN12_04(sk1891 *ref)
{
	ENTER(0);
	return (tblChampionSquad[ref->b1].curHP() != 0) ? 1 : 0;
}

//^1031:009E
// _1031_009e renamed _1031_009e_PFN12_05
U16 SkWinCore::_1031_009e_PFN12_05(sk1891 *ref)
{
	ENTER(0);
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
	ENTER(2);
	//SPX: Get attribute parm7, getMax = 0 : current / getMax = 1 : max
	i16 si = ref->attributes[parm7][getMax];
	U16 di;
	U8 bp01;
	if (true
		&& (getMax == 0)
		&& (di = ref->enchantmentPower) != 0
		&& (bp01 = ref->enchantmentAura) >= C03_ENCHANTMENT_AURA_FIRST
		&& (bp01) <= C06_ENCHANTMENT_AURA_LAST
		&& (bp01 + i16(-2)) == parm7
	) {
		if (di > 100)
			di = 100;
		si += RAND16(((si * di) >> 7) +1) +4;
	}
	return BETWEEN_VALUE(10, si +(ref->attributesEnhanced[parm7]), 220);
}



//^2C1D:0654
void SkWinCore::PUT_ITEM_TO_PLAYER(U16 championIndex)
{
	// click player panel to store leader's holding item into your back pack.

	ENTER(0);
	if (cd.pi.glbLeaderHandPossession.object != OBJECT_NULL) {
		if (tblChampionSquad[championIndex].curHP() != 0) {
			X16 iInvSlotIdx;	// si
			for (iInvSlotIdx = C13_INVENTORY_BACKPACK_1; iInvSlotIdx <= C29_INVENTORY_BACKPACK_LAST; iInvSlotIdx++) {	// (si=13; si<30; si++)
				if (tblChampionSquad[championIndex].Possess(iInvSlotIdx) == OBJECT_NULL)
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
	ENTER(0);
	U16 si = xx;
	if (si == glbChampionLeader)
		return;
	if (si != 0xffff && tblChampionSquad[si].curHP() == 0)
		return;
	if (glbChampionLeader != -1) {
		tblChampionSquad[glbChampionLeader].heroFlag |= CHAMPION_FLAG_1400;	// 0x1400
	}
	glbChampionLeader = si;
	if (si == 0xffff)
		return;
	if (si + 1 == cd.pi.glbNextChampionNumber)
		return;
	tblChampionSquad[si].heroFlag |= CHAMPION_FLAG_1400;	// 0x1400
	return;
}



//^2F3F:0000
void SkWinCore::ADD_ITEM_TO_PLAYER(U16 player, ObjectID rl)
{
	ENTER(0);
	for (U16 si = 0; si < 5; si++) {
		U16 di = _4976_404d[si][0];
		for (; _4976_404d[si][1] >= di; di++) {
			if (true
				&& tblChampionSquad[player].Possess(di) == OBJECT_NULL
				&& IS_ITEM_FIT_FOR_EQUIP(rl, di, 0) != 0
				&& (false
					|| _4976_404d[si][2] == 0xffff
					|| rl.DBType() == _4976_404d[si][2]
				)
			) {
				EQUIP_ITEM_TO_INVENTORY(player, rl, di);
				return;
			}
		}
	}
	return;
}

//^2F3F:04EA
// SPX: _2f3f_04ea renamed REVIVE_CHAMPION
void SkWinCore::REVIVE_CHAMPION(U16 xx, U16 yy, U16 dir, U16 zz, U16 iEventCode)
{
	ENTER(142);
	U16 iChampionNumber = cd.pi.glbNextChampionNumber - 1;	// si
	Champion* xChampion = &tblChampionSquad[iChampionNumber];	// bp04
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
		FIRE_FILL_SCREEN_RECT(iChampionNumber + RECT_161_CHAMPION_SLAB_AREA_1, glbPaletteT16[C00_COLOR_BLACK]);
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
	if (iChampIdx < 4 && tblChampionSquad[iChampIdx].curHP() == 0)
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
		Champion* xChampion = &tblChampionSquad[si -1];	// bp04
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
	tblChampionSquad[iChampIdx].heroFlag |= CHAMPION_FLAG_FC00;	// 0xFC00
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

	ENTER(0);
	i16 si;
	for (si = 2; --si >= 0; ) {
		ObjectID di = ref->Possess(si);
		if (true
			&& di.DBType() == dbPotion
			&& QUERY_CLS2_FROM_RECORD(di) == 0x14	// potion 0x14 is empty flash
		) {
			break;
		}
	}
	return si;
}

//^2C1D:0B2C
// SPX: zz = base experience to gain
void SkWinCore::ADJUST_SKILLS(U16 player, U16 yy, U16 zz)
{
	// CSBwinSimilarity: TAG01605a,AdjustSkills
	// SPX: Should really compare with CSBWin LevelUp

	ENTER(142);
	U16 di = yy;	// main skill
	U16 subSkill = di;
	U16 baseXP = zz;
	U16 doubleXP = 0;
	U16 halfBaseXP = 0;
	U16 halvedXP = zz;
	if (true
		&& di >= 4 
		&& di <= 11
		&& (glbGameTick -150 > _4976_0090)
	) {
		zz >>= 1;
		halfBaseXP = 1; // SPX added for info
		halvedXP = zz;
	}
	if (zz == 0)
		return;
	U16 bp06 = dunMapLocalHeader->Difficulty();	// map XP multiplicator
	U16 xpMapMultiplicator = bp06;	// SPX: added for reuse
	U16	afterMapXP = 0; // for info
	if (bp06 != 0) {
		zz = zz * bp06;
	}
	afterMapXP = zz;
	Champion *champion = &tblChampionSquad[player]; //*bp04
	U16 si = (di >= 4) ? ((di -4) >>2) : di;
	U16 mainSkill = si;
	bp06 = QUERY_PLAYER_SKILL_LV(player, si, 0);	// Current skill level
	if (true
		&& di >= 4
		&& ((i32)glbGameTick -25 < (i32)_4976_0090)
	) {
		zz <<= 1;
		doubleXP = 1; // SPX added for info
	}
	champion->skills[di] += zz;
	if (di >= 4) {
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

	zz = QUERY_PLAYER_SKILL_LV(player, si, 0);	// Compare with new skill level
	if (zz <= bp06)
		return;
	U16 bp0c = zz;
	U16 bp08 = RAND01();		// 0 - 1
	U16 bp0a = RAND01() +1;		// 1 - 2
	bp06 = RAND01();			// 0 - 1
	if (si != 2)
		bp06 &= zz;
	champion->maxVit(champion->maxVit() +U8(bp06));
	bp06 = champion->curStamina();
	champion->maxAntiF(champion->maxAntiF() + (RAND01() & (~zz)));
	U8 bp0d;
	switch (si) {
		case 0:
			bp0d = 6;		// 0x06 -> "<01>' JUST GAINED A FIGHTER LEVEL.<00>"
			bp06 /= 16;		// Stamina increase
			// SPX: This is strange
			//zz = bp06 * zz;
			zz *= 3;
			champion->maxStr(champion->maxStr() +U8(bp0a));	// STR + { 1 - 2 }
			champion->maxDex(champion->maxDex() +U8(bp08));	// DEX + { 0 - 1 }
			break;

		case 1:
			bp0d = 7;		// 0x07 -> "<01>' JUST GAINED A NINJA LEVEL.<00>"
			bp06 = bp06 / 21;
			zz <<= 1;
			champion->maxStr(champion->maxStr() +U8(bp08));	// STR + { 0 - 1 }
			champion->maxDex(champion->maxDex() +U8(bp0a));	// DEX + { 1 - 2 }
			break;

		case 3:
			bp0d = 9;		// 0x09 -> "<01>' JUST GAINED A WIZARD LEVEL.<00>"
			bp06 >>= 5;
			champion->maxMP(zz + (zz >> 1) + champion->maxMP());
			champion->maxWiz(champion->maxWiz() +U8(bp0a));	// WIS + { 1 - 2 }
			goto _0d3a;

		case 2:
			bp0d = 8;		// 0x08 -> "<01>' JUST GAINED A PRIEST LEVEL.<00>"
			bp06 = bp06 / 25;
			champion->maxMP(champion->maxMP() +zz);
			zz += (zz +1) >> 1;
			champion->maxWiz(champion->maxWiz() +U8(bp08));	// WIS + { 0 - 1 }
// SPX: label _0d3a = end of check for wizard/priest
_0d3a:
			champion->maxMP(champion->maxMP() +min_value(RAND02(), bp0c -1));
			// SPX: Removed hard coded 900
			if (champion->maxMP() > MAXMP)
				champion->maxMP(MAXMP);
			champion->maxAntiM(champion->maxAntiM() +RAND02());

			break;
	}
	//bp04->maxHP(RAND16((zz >> 1) +1) +zz +bp04->maxHP());
	// SPX: From CSBWin11 LevelUp, it should be more like this:
	champion->maxHP(RAND16((zz / 2) +1) +zz +champion->maxHP());
	if (champion->maxHP() > MAXHP)
		champion->maxHP(MAXHP);
	champion->maxStamina(RAND16((bp06 / 2) +1) +bp06 +champion->maxStamina());
	if (champion->maxStamina() > MAXSP)
		champion->maxStamina((i16)MAXSP);
	champion->heroFlag |= CHAMPION_FLAG_3800;	// 0x3800
	glbChampionMajorSkillsLevel[player][si]++;
	DISPLAY_HINT_NEW_LINE();
	glbChampionBonesIndex = player;
	U8 bp008e[128];
	DISPLAY_HINT_TEXT(glbChampionColor[player], QUERY_GDAT_TEXT(GDAT_CATEGORY_x01_INTERFACE_GENERAL, GDAT_INTERFACE_SUBCAT_x00_BASE_DATA, bp0d, bp008e));
	return;
}



//^2C1D:1CF3
// SPX: _2c1d_1cf3 renamed SHOOT_CHAMPION_MISSILE
void SkWinCore::SHOOT_CHAMPION_MISSILE(Champion *ref, ObjectID rl, U16 yy, U16 zz, U16 ww)
{
	ENTER(0);
	U16 si = ref->playerDir();
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
	glbPlayerThrowCounter = 4;
	_4976_4c0c = si;
	return;
}


//^2C1D:1D64
// SPX: _2c1d_1d64 renamed CAST_CHAMPION_MISSILE_SPELL
U16 SkWinCore::CAST_CHAMPION_MISSILE_SPELL(U16 xx, ObjectID rl, i16 zz, U16 ww)
{
	ENTER(4);
	U16 di = zz;
	Champion *champion = &tblChampionSquad[xx];	// *bp04
	if (champion->curMP() < ww)
		return 0;
	champion->curMP(champion->curMP() -ww);
	champion->heroFlag |= CHAMPION_FLAG_0800;	// 0x0800
	U16 si = 10 - min_value(8, champion->maxStamina() >> 3);
	if ((si << 2) > di) {
		di += 3;
		si--;
	}
	SHOOT_CHAMPION_MISSILE(champion, rl, di, 90, si);
	return 1;
}




	//^2C1D:0186
// SPX: _2c1d_0186 replaced by CALL_ENCHANTMENT_SELF
U16 SkWinCore::CALL_ENCHANTMENT_SELF(Champion *ref, i16 xx, U16 yy, U16 zz)
{
	ENTER(0);
	X16 di = yy;
	X16 si = 1;
	if (zz != 0) {
		if (ref->curMP() == 0)
			return 0;
		if (ref->curMP() < 4) {
			di >>= 1;
			ref->curMP(0);
			si = 0;
		}
		else {
			ref->curMP(4);
		}
	}
	PROCEED_ENCHANTMENT_SELF(15, xx, di >> 5, di);
	return si;
}

//^2759:0F39
// SPX: _2759_0f39 renamed ADJUST_HAND_COOLDOWN
void SkWinCore::ADJUST_HAND_COOLDOWN(U16 player, U16 yy, U16 zz)
{
	ENTER(6);
	U16 di = yy;
	U16 si = zz;
	Champion *champion = &tblChampionSquad[player];
	U16 bp06;
	(si == 0xffff) ? (bp06 = 3, si = 0) : (bp06 = 1);
	if (glbGlobalSpellEffects.AuraOfSpeed != 0)
		di >>= 2;
	di += 2;

	do {
		U16 cx = champion->handCooldown[si];
		if (cx < di) {
			cx = (cx >> 1) +di;
		}
		else {
			cx += di >> 1;
		}
		if (cx > 255)
			cx = 255;
		champion->handCooldown[si] = U8(cx);
		si++;
	} while (--bp06 != 0);

	return;
}




//^075F:1F33
U16 SkWinCore::CALC_PLAYER_ATTACK_DAMAGE(Champion *ref, U16 player, ObjectID rlEnemy, U16 xx, U16 yy, U16 valPb, U16 valDM, U16 valSk, U16 valAt)
{
	ENTER(16);
	if (player >= cd.pi.glbChampionsCount)
		return 0;
	if (ref->curHP() == 0)
		return 0;
	U16 bp0c = dunMapLocalHeader->Difficulty() << 1;
	Creature *bp08 = GET_ADDRESS_OF_RECORD4(rlEnemy);
	AIDefinition *bp04 = QUERY_CREATURE_AI_SPEC_FROM_TYPE(bp08->CreatureType());
	i16 si;
	i16 di;
	if (true
		&& bp04->Defense != 255
		&& ((bp04->w24 & 0x1000) == 0 || valAt == 1)
	) {
		ObjectID bp10 = GET_CREATURE_AT(xx, yy);
		if (bp10 == OBJECT_NULL || CREATURE_1c9a_0958(bp10) == 0) {
			U16 bp0e = valPb & 0x8000;
			valPb &= 0x7fff;
			if (bp04->w0_5_5() == 0 || bp0e != 0) {
				if (false
					|| USE_DEXTERITY_ATTRIBUTE(player) > (((RAND() & 31) + bp04->Defense +bp0c +(glbLightLevel << 1) -16) >> 1)
					|| RAND02() == 0
					|| USE_LUCK_ATTRIBUTE(ref, 75 -valPb) != 0
				) {
					si = COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(player, glbSelectedHandAction, valSk);
					if (si != 0) {
						si = RAND16((si >> 1) +1) +si;
						si = (i32(si) * valDM) >> 5;
						U16 bp0a = bp04->ArmorClass +(RAND() & 31) +bp0c;
						di = si = (RAND() & 31) +si -bp0a;
						if (di > 1)
							goto _2112;
					}
					si = RAND02();
					if (si != 0) {
						si++;
						di = (RAND() & 15) +di;
						if (di > 0 || RAND01() != 0) {
							si = RAND02() +si;
							if (RAND02() == 0) {
								si += max_value(0, (RAND() & 15) +di);
							}
						}
_2112:
						si >>= 1;
						si = RAND16(si) +RAND02() +si;
						si = RAND16(si) +si;
						si >>= 2;
						si = RAND02() +si +1;
						if ((RAND() & 63) < QUERY_PLAYER_SKILL_LV(player, valSk, 1)) {
							si += si +10;
						}
						//SPX: Something to do with POISON
						di = QUERY_GDAT_DBSPEC_WORD_VALUE(ref->Possess(glbSelectedHandAction), GDAT_ITEM_STATS_POISONOUS);	// 13
						if (di != 0 && (RAND() & 31) < si) {
							// Add poison damage
							si = APPLY_CREATURE_POISON_RESISTANCE(rlEnemy, di) +si;
						}
						ADJUST_SKILLS(player, valSk, ((bp04->w22_8_b() * si) >> 4) +3);
						ADJUST_STAMINA(player, RAND02() +4);
						goto _21dd;
					}
				}
			}
		}
	}
	si = 0;
	ADJUST_STAMINA(player, RAND01() +2);
_21dd:
	ATTACK_CREATURE(rlEnemy, xx, yy, 0x6002, 0x5a, si);
	return si;
}

//^2759:14C8
U16 SkWinCore::WIELD_WEAPON(U16 iChampionIdx, U16 valPa, U16 xx, U16 yy, U16 valSk, U16 valAt)
{
	// returns 0 if no target, 1 if hits a creature.

	ENTER(4);
	Champion* xChampion = &tblChampionSquad[iChampionIdx];	//*bp04
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
	ENTER(16);
	U16 di = xx;
	Champion *bp04 = &tblChampionSquad[di];
	U16 bp0e = 0;
	ObjectID bp0c ;
	ObjectID si ;
	if (yy < 0) {
		if (cd.pi.glbLeaderHandPossession.object == OBJECT_NULL)
			return 0;
        bp0c = bp04->Possess(1);
		si = REMOVE_OBJECT_FROM_HAND();
		bp04->Possess(1, si);
		yy = 1;
		bp0e = 1;
	}
	i16 bp0a = COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(di, yy, 10);
	if (bp0e != 0) {
		bp04->Possess(1, bp0c);
	}
	else {
		si = REMOVE_POSSESSION(di, yy);
		if (si == OBJECT_NULL)
			return 0;
	}
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
	ADJUST_STAMINA(di, SOMETHING_ITEM_WEIGHT_2c1d_0e23(si));
	ADJUST_HAND_COOLDOWN(di, 4, yy);
	U16 bp08 = 8;
	U16 bp06 = 1;
	U16 bp10 = QUERY_GDAT_DBSPEC_WORD_VALUE(si, 9);
	if (bp10 != 0) {
		bp08 = (bp10 >> 2) +bp08 +4;
		bp06 = bp10;
	}
	ADJUST_SKILLS(di, 10, bp08);
	bp0a += bp06;
	bp06 = QUERY_PLAYER_SKILL_LV(di, SKILL_NINJA_THROW, 1);
	bp0a = RAND16((bp0a >> 2) +8) +bp06 +bp0a;
	bp0a = min_value(bp0a, 255);
	bp08 = BETWEEN_VALUE(40, (RAND() & 31) + (bp06 << 3), 200);
	bp10 = QUERY_GDAT_DBSPEC_WORD_VALUE(si, 12);
	bp06 = (bp10 != 0) ? bp06 : max_value(5, 11 -bp06);
	SHOOT_ITEM(si, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, (cd.pi.glbPlayerDir + zz) & 3, cd.pi.glbPlayerDir, bp0a, bp08, bp06);

	SkD((DLV_TWEET, "Tweet: You (x:%d, y:%d, map:%d) have thrown %s! \n"
		, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, glbCurrentMapIndex, static_cast<LPCSTR>(getRecordNameOf(si))
		));

	glbPlayerThrowCounter = 4;
	_4976_4c0c = cd.pi.glbPlayerDir;
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
		tblChampionSquad[iLocalChampIdx].PoisonValue = 0;
	}
	return;
}



//^24A5:10FB
void SkWinCore::PLAYER_CONSUME_OBJECT(U16 player, ObjectID rlConsume, i16 possess)
{
	// SPX: Default values
	U16 PotionType = 0;
	U16 WaterValue = C0800_WATER_DEFAULT_ITEM_VALUE;	// 800


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

	Champion *champion = &tblChampionSquad[player];
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
		champion->curFood(min_value(champion->curFood() + iFoodValue, C2048_FOOD_MAX));
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
		WaterValue = C0800_WATER_DEFAULT_ITEM_VALUE;

		// SPX: Custom Water value, override it if any
		if (SkCodeParam::bUseDM2ExtendedMode)
			WaterValue = QUERY_GDAT_WATER_VALUE_FROM_RECORD(tObjectConsumed);

		champion->curWater(min_value(champion->curWater() + WaterValue, C2048_WATER_MAX));
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
				return;

			case  6:			// ”é–ò{ROS}
				BOOST_ATTRIBUTE(champion, ATTRIBUTE_DEXTERITY, si);	// 2 = DEX
				break;

			case  7:			// ”é–ò{KU}
				BOOST_ATTRIBUTE(champion, ATTRIBUTE_STRENGTH, bp08->PotionPower() / 35 +5);	// 1 = STR
				break;

			case  8:			// ”é–ò{DAIN}
				BOOST_ATTRIBUTE(champion, ATTRIBUTE_WISDOM, si);	// 3 = WIS
				break;

			case  9:			// ”é–ò{NETA}
				BOOST_ATTRIBUTE(champion, ATTRIBUTE_VITALITY, si);	// 4 = DAIN
				break;

			case 10:			// ‘Î{VEN}”é–ò
				// SPX: immediate cure? not like in PC DM2 where poisons have different strengths?
				CURE_POISON(player);
				break;

			case 11:			// ”é–ò{MON}	Regain stamina
				champion->curStamina(champion->curStamina() +min_value(champion->maxStamina() -champion->curStamina(),champion->maxStamina() / iPowerDivisor));
				break;

			case 12:			// ”é–ò{YA}		Cast party shield
				si += (si >> 1);
				PROCEED_ENCHANTMENT_SELF(1 << player, C02_ENCHANTMENT_PARTY_SHIELD, si, si * si);
				break;

			case 13:			// ”é–ò{EE}		Regain mana
				bp0a = min_value(champion->curMP() +si +(si -8), MAXMP);
				if (champion->maxMP() < bp0a) {
					bp0a -= (bp0a -max_value(champion->curMP(), champion->maxMP())) >> 1;
				}
				champion->curMP(bp0a);
				break;

			case 14:			// ”é–ò{VI}		Regain health
				bp0a = bp08->PotionPower() / 42;
				champion->curHP(champion->curHP() + (champion->maxHP() / iPowerDivisor));
				bp0e = champion->bodyFlag;
				if (bp0e == 0)
					break;
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
				WaterValue = C1600_WATER_FLASK_VALUE;	// Default value 1600

				if (SkCodeParam::bUseDM2ExtendedMode)
					WaterValue = QUERY_GDAT_WATER_VALUE_FROM_RECORD(tObjectConsumed);
				
				champion->curWater(min_value(champion->curWater() +WaterValue, C2048_WATER_MAX));

				break;
		}
		if (possess == -1)
			REMOVE_OBJECT_FROM_HAND();
		DEALLOC_RECORD(tObjectConsumed);
		tObjectConsumed = ALLOC_NEW_RECORD(dbPotion);
		if (tObjectConsumed == OBJECT_NULL)
			return;
		SET_ITEMTYPE(tObjectConsumed, C20_POTION_EMPTY_FLASK);	// 0x14 = EMPTY FLASK
		if (possess == -1) {
			TAKE_OBJECT(tObjectConsumed, 0);
		}
		else {
			champion->Possess(possess, tObjectConsumed);
		}
_14a9:
		if (champion->curStamina() > champion->maxStamina()) {
			champion->curStamina(champion->maxStamina());
		}
		if (champion->curHP() > champion->maxHP()) {
			champion->curHP(champion->maxHP());
		}
		// SPX: Sound made when eating/drinking
		// 0x16 = Champion category, 0x83 = sound for eat/drink, 0xFE = default index
		// So there could be a sound for eat and one for drink
		QUEUE_NOISE_GEN2(GDAT_CATEGORY_x16_CHAMPIONS, champion->HeroType(), SOUND_CHAMPION_EAT_DRINK, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 0, 0x96, 0x80);
		champion->heroFlag |= CHAMPION_FLAG_3800;	// 0x3800
		if (possess != -1) {
			champion->heroFlag |= CHAMPION_FLAG_4000;	// 0x4000
			CALC_PLAYER_WEIGHT(player);
		}
	}
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
	Champion* xChampion = &tblChampionSquad[player = cd.pi.glbChampionsCount];	//*bp04
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
	xChampion->curFood((RAND() & 255) + C1500_START_BASE_FOOD);
	xChampion->curWater((RAND() & 255) + C1500_START_BASE_WATER);

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
	ENTER(0);
	for (U16 si = 0; si < cd.pi.glbChampionsCount; si++) {
		tblChampionSquad[si].heroFlag |= flagvalue;
	}
	return;
}

//^2F3F:0789
// SPX: _2f3f_0789 renamed SEARCH_STARTER_CHAMPION
void SkWinCore::SEARCH_STARTER_CHAMPION() // _2f3f_0789
{
	ENTER(8);
	if (cd.mo.glbSpecialScreen == _MENU_SCREEN__RESUME_GAME_SELECT) {
		ObjectID di = cd.pi.glbLeaderHandPossession.object;
		if (di == OBJECT_NULL) {
			_4976_57de = 0xff;
			CHANGE_CURSOR_HAND_ITEM();
		}
		else {
			i16 si = glbChampionLeader;
			glbChampionLeader = -1;
			TAKE_OBJECT(di, 1);
			glbChampionLeader = si;
		}
		Champion *champion = tblChampionSquad;	//*bp04
		U16 si;
		for (si = 0; si < cd.pi.glbChampionsCount; si++, champion++) {
			champion->heroFlag &= 1023;	// 0x3FF
			WRITE_UI8(_4976_3de2,+si,0xff);
			glbChampionAlive[si] = 0;
			_4976_3dde[RCJ(4,si)] = 0xff;
			glbChampionHandCoolingDown[si][1] = 0;
			_4976_3de6[RCJ(4,si)][1] = 0xffff;
			glbChampionHandCoolingDown[si][0] = 0;
			_4976_3de6[RCJ(4,si)][0] = 0xffff;
			glbChampionHandCoolingDown[si][2] = 0;
		}
		SET_PARTY_HERO_FLAG(0x4000);
		si = glbChampionLeader;
		if (si == 0xffff)
			return;
		glbChampionLeader = -1;
		SELECT_CHAMPION_LEADER(si);
		return;
	}
	//printf("cd.pi.glbLeaderHandPossession\n"); getch();
	cd.pi.glbLeaderHandPossession.object = OBJECT_NULL;
	_4976_57de = 0xff;
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
				tblChampionSquad[0].playerDir(U8(cd.pi.glbPlayerDir));
				tblChampionSquad[0].playerPos(U8(cd.pi.glbPlayerDir));
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
					tblChampionSquad[0].playerDir(U8(cd.pi.glbPlayerDir));
					tblChampionSquad[0].playerPos(U8(cd.pi.glbPlayerDir));
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
	if (_4976_5dbc != 0) {
		CHANGE_PLAYER_POS(_4976_5dba | 0x8000);
	}
}


//^2C1D:1FD1
// SPX: _2c1d_1fd1 renamed GET_CHAMPION_SPECIAL_FORCE
X16 SkWinCore::GET_CHAMPION_SPECIAL_FORCE(U16 iChampionIdx)
{
	ENTER(0);
	U16 iLocalChampIdx = iChampionIdx;	// si
	if (tblChampionSquad[iLocalChampIdx].curHP() == 0)
		return 0;
	if (((GET_PLAYER_WEIGHT(iLocalChampIdx) / 10) + (tblChampionSquad[iLocalChampIdx].heroFlag & CHAMPION_FLAG_0010_MALE)) != 0)	// 0x10
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

	Champion* xChampion = &tblChampionSquad[iChampIdx];	// bp04
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
	Champion* xChampion = &tblChampionSquad[play];	//*bp04
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
	if (tblChampionSquad[play].enchantmentAura == C02_ENCHANTMENT_PARTY_SHIELD) {	// == 2
		si += tblChampionSquad[play].enchantmentPower;
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

	Champion* xChampion = &tblChampionSquad[iCharIdx];
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
	Champion* xChampion = &tblChampionSquad[iLocalChampIndex];	//*bp04
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
	if (tblChampionSquad[iLocalChampIndex].curHP() == 0)
		return 0;
	iChampWeight = tblChampionSquad[iLocalChampIndex].curWeight();
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
	Champion* xChampion = &tblChampionSquad[xx]; // bp04
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
		ENTER(4);
		Champion *bp04 = &tblChampionSquad[xx];
		_2c1d_153f(xx);
		bp04->curWeight(0);
		U16 si;
		for (si = 0; si < C30_INVENTORY_MAX_SLOT; si++) {
			bp04->Possess(si, oFFFF);
		}
		si = bp04->maxHP();
		bp04->maxHP(max_value(25, si - (si >> 6) - 1));
		bp04->curHP(bp04->maxHP() >> 1);
		bp04->heroFlag |= 0x4000;
		bp04->enchantmentAura = 0;
		bp04->enchantmentPower = 0;
		_1031_0667();
		_443c_040e_MOUSE();
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
//		champion = &tblChampionSquad[player];
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
	X16 iQuarterPos = tblChampionSquad[iChampionIdx].playerPos();	// di
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

	ENTER(12);
	X16 di;
	Champion *champion = &tblChampionSquad[di = player];

#if (DM2_EXTENDED_MODE == 1)
	if (SkCodeParam::bUseExtendedSound == true) {	// SPX: Play the champion own scream sound if dead
		printf("champion dead: %d\n", champion->HeroType());
		QUEUE_NOISE_GEN1(GDAT_CATEGORY_x16_CHAMPIONS, champion->HeroType(), SOUND_CHAMPION_SCREAM, 0x61, 0x80, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1);
	}
#endif

	if (cd.pi.glbChampionIndex -1 == di)
		DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
	champion->curHP(0);
	champion->herob44 = champion->handCooldown[C01_INVENTORY_HAND_LEFT] = champion->handCooldown[C00_INVENTORY_HAND_RIGHT] = 0;
	champion->heroFlag |= CHAMPION_FLAG_4000;	// 0x4000
	if (di +1 == glbChampionInventory) {
		if (glbShowItemStats != 0) {
			glbShowItemStats = 0;
			FIRE_MOUSE_RELEASE_CAPTURE();
			if (cd.pi.glbLeaderHandPossession.object != OBJECT_NULL)
				DISPLAY_TAKEN_ITEM_NAME(cd.pi.glbLeaderHandPossession.object);
			glbMouseVisibility = 1;
		}
		if (_4976_4bfe != 0) {
			_4976_4bfe = 0;
			FIRE_MOUSE_RELEASE_CAPTURE();
			glbMouseVisibility = 1;
			FIRE_SHOW_MOUSE_CURSOR();
		}
		INTERFACE_CHAMPION(4);
	}
	if (di == glbChampionLeader && _4976_4c3e != 0) {
		_4976_4c3e = 0;
		FIRE_MOUSE_RELEASE_CAPTURE();
		glbMouseVisibility = 1;
		FIRE_SHOW_MOUSE_CURSOR();
	}
	DROP_PLAYER_ITEMS(di);
	X16 bp0a = champion->playerPos();
	ObjectID bp0c = ALLOC_NEW_RECORD(0x800A); // bp0C
	if (bp0c != OBJECT_NULL) {
		Miscellaneous_item *xItem = GET_ADDRESS_OF_RECORDA(bp0c); // bp08
		xItem->ItemType(GET_CHAMPION_BONES_ITEM_ID()); // SPX: changed hardcoded 0 to function to get proper Bones ID depending on DM2 or DM1 mode
		xItem->Important(1);
		xItem->Bone(di);
		MOVE_RECORD_TO(ObjectID(bp0c, bp0a), -1, 0, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY);
	}
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
	X16 si;
	for (si = 0; si < cd.pi.glbChampionsCount && tblChampionSquad[si].curHP() == 0; si++);
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

	Champion* champion = tblChampionSquad;
	for (U16 championIndex = 0; championIndex < cd.pi.glbChampionsCount; championIndex++, champion++) {
		champion->bodyFlag = glbChampionsBodyFlags[championIndex];
		glbChampionsBodyFlags[championIndex] = 0;
		X16 di = cd.pi.glbChampionsPendingDamage[championIndex];
		if (di == 0)
			continue;
		cd.pi.glbChampionsPendingDamage[championIndex] = 0;
		i16 si = champion->curHP();
		if (si == 0)
			continue;
		si = si -di;
		if (si <= 0) {
			CHAMPION_DEFEATED(championIndex);
			continue;
		}
		champion->curHP(si);
		champion->damageSuffered = di;
		champion->heroFlag |= CHAMPION_FLAG_0800;	// 0x800
		si = champion->timerIndex;
		//if (si == -1) {	// SPX: or == 0xFFFF ....
		if (champion->timerIndex == TIMER_NONE) {
			Timer bp10;
			bp10.TimerType(tty0C);
			bp10.SetMap(cd.pi.glbPlayerMap);
			bp10.SetTick(glbGameTick +5);
			bp10.actor = U8(championIndex);
			champion->timerIndex = QUEUE_TIMER(&bp10);
		}
		else {
			glbTimersTable[si].SetMap(cd.pi.glbPlayerMap);
			glbTimersTable[si].SetTick(glbGameTick + 5);
			TIMER_3a15_05f7(si);
		}
	}
	return;
}
//^2C1D:210A
// SPX: _2c1d_210a renamed UPDATE_CHAMPIONS_STATS
void SkWinCore::UPDATE_CHAMPIONS_STATS()
{
	ENTER(14);
	if (cd.pi.glbChampionsCount == 0)
		return;
	glbChampionTable[0].enchantmentPower += 0x38;
	if (glbChampionTable[0].enchantmentPower > 0x80U)
		glbChampionTable[0].enchantmentPower -= 0x80;
	U16 bp0e = 0;	
	Champion *champion = tblChampionSquad;
	X16 bp0c;
	U16 bp0a;
	for (; bp0e < cd.pi.glbChampionsCount; bp0e++, champion++) {
		if (champion->curHP() == 0 || bp0e + 1 == cd.pi.glbNextChampionNumber)
			continue;
		if (champion->curMP() < champion->maxMP()) {
			bp0a = QUERY_PLAYER_SKILL_LV(bp0e, SKILL_WIZARD_GLOBAL, 1) + QUERY_PLAYER_SKILL_LV(bp0e, SKILL_PRIEST_GLOBAL, 1);
			if (GET_PLAYER_ABILITY(champion, abWiz, 0) +bp0a > glbChampionTable[0].enchantmentPower) {
				X16 si = champion->maxMP() / 40;
				si++;
				if (cd.pi.glbIsPlayerSleeping != 0)
					si <<= 1;
				ADJUST_STAMINA(bp0e, si * max_value(7, 0x10 -bp0a));
				champion->curMP(champion->curMP() +min_value(si, champion->maxMP() -champion->curMP()));
			}
		}
		// SPX: Decrease of current mana if it exceed maximum mana
		else if (champion->curMP() > champion->maxMP()) {
			champion->curMP(champion->curMP() -1);
		}
		bp0c = 4;
		i16 si = champion->maxStamina();
		for (; champion->curStamina() < (si >>= 1); bp0c += 2);
		si = 0;
		X16 di = BETWEEN_VALUE(1, (champion->maxStamina() >> 8) -1, 6);
		bp0a = U16(glbGameTick) -U16(glbGameTick_4b80);
		if (bp0a > 0x50) {
			di++;
			if (bp0a > 0xfa)
				di++;
		}
		if (cd.pi.glbIsPlayerSleeping != 0)
			di <<= 1;
		do {
			bp0a = (bp0c <= 4) ? 1 : 0;
			if (champion->curFood() < M0512_FOOD_WARNING) {
				if (bp0a != 0) {
					si += di;
					champion->curFood(champion->curFood() -2);
				}
			}
			else {
				if (champion->curFood() >= 0)
					si -= di;
				champion->curFood(champion->curFood() -((bp0a != 0) ? 2 : (bp0c >> 1)));
			}
			if (champion->curWater() < M0512_WATER_WARNING) {
				if (bp0a != 0) {
					si += di;
					champion->curWater(champion->curWater() -1);
				}
			}
			else {
				if (champion->curWater() >= 0)
					si -= di;
				champion->curWater(champion->curWater() -((bp0a != 0) ? 1 : (bp0c >> 2)));
			}
		} while (--bp0c != 0 && champion->curStamina() < champion->maxStamina());

		ADJUST_STAMINA(bp0e, si);
		if (champion->curFood() < M1024_FOOD_MIN)
			champion->curFood(M1024_FOOD_MIN);
		if (champion->curWater() < M1024_WATER_MIN)
			champion->curWater(M1024_WATER_MIN);
		if (champion->curHP() < champion->maxHP() && champion->curStamina() >= (champion->maxStamina() >> 2) && GET_PLAYER_ABILITY(champion, abVit, 0) > glbChampionTable[0].enchantmentPower) {
			di = (champion->maxHP() >> 7) +1;
			if (cd.pi.glbIsPlayerSleeping != 0)
				di <<= 1;
			// SPX : This is the regeneration function
			champion->curHP(champion->curHP() +min_value(di, champion->maxHP() -champion->curHP()));
		}
		if ((glbGameTick & ((cd.pi.glbIsPlayerSleeping != 0) ? 0x3f : 0xff)) == 0) {
			for (si = ATTRIBUTE_FIRST; si <= ATTRIBUTE_LAST; si++) {	// (si = 0; si <= 6; si++)
				U8 *attributes = champion->attributes[si];	// *bp08
                U16 attrMax = attributes[ATTRIBUTE_MAX];	// bp0a
				if (attributes[ATTRIBUTE_CURRENT] < attrMax) {
					attributes[ATTRIBUTE_CURRENT]++;
				}
				else if (attributes[ATTRIBUTE_CURRENT] > attrMax) {
					attributes[ATTRIBUTE_CURRENT] -= attributes[ATTRIBUTE_CURRENT] / attrMax;
				}
			}
		}
		champion->heroFlag |= CHAMPION_FLAG_0800;	// 0x800
		if (bp0e +1 == glbChampionInventory)
			champion->heroFlag |= CHAMPION_FLAG_3000;	// 0x3000
	}
	return;
}

//^2E62:0D82
// SPX: _2e62_0d82 renamed GLOBAL_UPDATE_UNKNOW1
void SkWinCore::GLOBAL_UPDATE_UNKNOW1()
{
	ENTER(2);
	U8 bp01;
	if (cd.pi.glbLeaderHandPossession.object != OBJECT_NULL && (QUERY_GDAT_DBSPEC_WORD_VALUE(cd.pi.glbLeaderHandPossession.object, 0)&0x8000) != 0 && (bp01 = GET_ITEM_ICON_ANIM_FRAME(cd.pi.glbLeaderHandPossession.object, -1, 1)) != _4976_57de) {
		DRAW_ITEM_IN_HAND(&cd.pi.glbLeaderHandPossession);
		CHANGE_CURSOR_HAND_ITEM();
		_4976_57de = bp01;
	}
	return;
}


//^24A5:069B
// SPX: _24a5_069b renamed BURN_PLAYER_LIGHTING_ITEMS
void SkWinCore::BURN_PLAYER_LIGHTING_ITEMS()
{
	ENTER(8);
	X16 bRecomputeLight = 0;
	X16 iLocalChampionCount = cd.pi.glbChampionsCount;
	if (cd.pi.glbNextChampionNumber != 0)
		--iLocalChampionCount;
	Champion *xChampion = tblChampionSquad;	//*bp04
	for (; iLocalChampionCount-- != 0; xChampion++) {
		X16 iInventorySlot = 2;
		for (; iInventorySlot-- != 0; ) { // go from inventory position 2 to 0 (hands)
			ObjectID oItem = OBJECT_NULL; // SPX: add default init
			if ((QUERY_GDAT_DBSPEC_WORD_VALUE(oItem = xChampion->Possess(iInventorySlot), 0) & ITEM_FLAG_PRODUCE_LIGHT) == 0) // if item does not have 0x10 flag, then it does not produces light.
				continue;
			if (ADD_ITEM_CHARGE(oItem, 0) == 0)	// get current charge, if = 0, then do nothing
				continue;
			if (ADD_ITEM_CHARGE(oItem, -1) == 0) // decrease one charge
				SET_ITEM_IMPORTANCE(oItem, 0);
			bRecomputeLight = 1;	// Item charge has been changed
		}
	}
	if (bRecomputeLight != 0)
		RECALC_LIGHT_LEVEL();
	return;
}



//^12B4:011E
void SkWinCore::RESET_SQUAD_DIR()
{
	ENTER(0);
	for (U16 si = 0; si < cd.pi.glbChampionsCount; si++) {
		tblChampionSquad[si].playerDir(U8(cd.pi.glbPlayerDir));
	}
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
	ENTER(4);
	X32 bp04 = 0;
	U16 di;
	for (di = 0; di < cd.pi.glbChampionsCount; di++) {
		U16 si;
		for (si = 0; si < SKILL_MAJOR_COUNT; si++) {	// (si = 0; si <= 3; si++) 
			bp04 += tblChampionSquad[di].skills[si];
		}
	}
	X16 si;
	for (si = 1; bp04 >= 0x200; ) {
		bp04 = bp04 >> 1;
		si++;
	}
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
	ENTER(0);
	U16 si = 0;
	U16 di = (_4976_4dda == 17) ? 768 : 765; // rectangles ??
	if (_098d_02a2(di, xx, yy) != 0) {
		si = _2c1d_1fb1_CHAMPION_LEADER_SHOOT_DIR(0); // throw by left hand
	}
	else if (_098d_02a2(di +1, xx, yy) != 0) {
		si = _2c1d_1fb1_CHAMPION_LEADER_SHOOT_DIR(1); // throw by right hand
	}
	if (si != 0)
		cd.gg.glbRefreshViewport = 1;
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
	Champion* xChampion = &tblChampionSquad[iHeroIndex];
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
