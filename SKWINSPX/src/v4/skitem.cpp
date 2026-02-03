//------------------------------------------------------------------------------
// SK-ITEM
//------------------------------------------------------------------------------

//#include <StdAfx.h>	// Required for MVC6 compilation
#include <skcore.h>
#include <skglobal.h>
#include <skparam.h>

//#if defined(_USE_MFC80) || defined(_USE_MFC60)
//#include <SKMFC.h>
//#include <resource.h>
//#endif // defined(_USE_MFC80) || defined(_USE_MFC60)



//^48AE:01EC
//U16 IS_ITEM_FIT_FOR_EQUIP(ObjectID recordLink, i16 possess, U16 onlyBodyPart)
// Beware : recursive !
U16 SkWinCore::IS_ITEM_FIT_FOR_EQUIP(ObjectID recordLink, i16 inventorySlot, U16 onlyBodyPart)
{
	ENTER(0);
	i16 iInvSlot = inventorySlot;	// si
	U16 iFlagsValue = QUERY_GDAT_DBSPEC_WORD_VALUE(recordLink, GDAT_ITEM_STATS_EQUIP_FLAGS);	// di, 4
	if (onlyBodyPart != 0) {
		if (iInvSlot < INVENTORY_BACKPACK_1) {	// 0x0D = 13 = backpack first
			if (iInvSlot < 0) {
				return iFlagsValue & ITEM_EQUIP_FLAG_0400;	// 0x0400
			}
			return _4976_49e8_ItemHoldMask[RCJ(INVENTORY_BACKPACK_1,iInvSlot)] & iFlagsValue;	// _4976_49e8[RCJ(13,si)] & di
		}
		return 0;
	}
	if (iInvSlot < INVENTORY_BACKPACK_1) {	// 0x000D
		if (iInvSlot > INVENTORY_HAND_LAST) {	// 0x0001
			return iFlagsValue & _4976_49e8_ItemHoldMask[RCJ(INVENTORY_BACKPACK_1,iInvSlot)];	// _4976_49e8[RCJ(13,si)]
		}
	}
	//^_023e
	// That part would be for arrows within quiver which are moved to scabbard
	if (iInvSlot >= INVENTORY_OVER_1) {	// 0x001E
		if (iInvSlot < INVENTORY_MAX_OVER) {	// 00x26
			if ((iFlagsValue & ITEM_EQUIP_FLAG_CONTAINER_2) != 0) {	// 0x8000
				return 0;
			}
			if (IS_ITEM_FIT_FOR_EQUIP(_4976_3de2[(cd.pi.glbChampionIndex << 1) + (glbSelectedHandAction)], INVENTORY_SCABBARD_1, 0x0000) != 0) {	// 0x000c
				return iFlagsValue & ITEM_EQUIP_FLAG_SCABBARD;	// 0x0040
			}
		}
	}
	return 1;
}

//^2C1D:038B
// SPX: _2c1d_038b renamed RETRIEVE_ITEM_BONUS // changed U16 to Bit16 since return can be negative !
// i16 SkWinCore::RETRIEVE_ITEM_BONUS(ObjectID x1, Bit8u x2, U16 x3, U16 x4)
i16 SkWinCore::RETRIEVE_ITEM_BONUS(ObjectID x1, Bit8u x2, U16 x3, i16 iBonusDir)
{
	ENTER(2);
	//U16 si = x4; // iBonusDir
	i16 iBonusValue = QUERY_GDAT_DBSPEC_WORD_VALUE(x1, x2);
	if (iBonusValue == 0)
		return 0;
	if ((iBonusValue & 0x4000) != 0) {
		switch ((X16)iBonusDir) {	// iBonusDir == 0xFFFF or -1 means we want to get negative value of this bonus, certainly to remove it from champion
			case 0xFFFE: // may come from event/timer ?
			case 0x0002:
			case 0x0003:
				break;
			default:
				return 0;
		}
	}
	else {
		if (x3 == 0) {
			if ((iBonusValue & 0x8000) == 0)
				return 0;
		}
	}
	//iBonusValue = (Bit8u)iBonusValue;	// SPX is this to reduce to 255 max ? I remove this
	if (iBonusDir < 0) {
		iBonusValue = -iBonusValue;
	}
	return iBonusValue;
}



//^24A5:0930
// SPX: _24a5_0930 renamed PUT_OBJECT_INTO_CONTAINER
void SkWinCore::PUT_OBJECT_INTO_CONTAINER()
{
	//^24A5:0930
	ENTER(4);
	//^24A5:0936
	if (glbObjectForContainer == OBJECT_NULL)
		return;
	//^24A5:093D
	GenericContainerRecord *bp04 = GET_ADDRESS_OF_GENERIC_CONTAINER_RECORD(glbObjectForContainer);
	glbObjectForContainer = OBJECT_NULL;
	i16 si;
	for (si = 0; si < 8; si++) {
		//^24A5:0957
		ObjectID di = glbCurrentContainerItems[si];
		if (di != OBJECT_NULL) {
			glbCurrentContainerItems[si] = OBJECT_NULL;
			APPEND_RECORD_TO(di, &bp04->possession, -1, 0);
		}
		//^24A5:0986
	}
	//^24A5:098C
	return;
}


//^0CEE:29B9
U16 SkWinCore::GET_MAX_CHARGE(ObjectID recordLink)
{
	ENTER(0);
	if (recordLink != OBJECT_NULL) {
		switch (recordLink.DBType()) {
			case dbWeapon:
				return 15;
			case dbCloth:
				return 15;
			case dbMiscellaneous_item:
				return 3;
		}
	}
	return 0;
}


//^24A5:0990
// SPX: _24a5_0990 renamed __CHECK_ROOM_FOR_CONTAINER
void SkWinCore::__CHECK_ROOM_FOR_CONTAINER(ObjectID rl, Container *ref)
{
	//^24A5:0990
	ENTER(0);
	//^24A5:0995
	if (rl != OBJECT_NULL && rl != glbObjectForContainer) {
		//^24A5:09A4
		X16 di = 0;
		X16 si = 0;
		while (ref->GetContainedObject() != OBJECT_END_MARKER) {
			//^24A5:09AC
			if (++di > 8)
				break;
			//^24A5:09B4
			glbCurrentContainerItems[si++] = ref->GetContainedObject();
			CUT_RECORD_FROM(ref->GetContainedObject(), &ref->w2, -1, 0);
			//^24A5:09E0
		};
		//^24A5:09EA
		while (si < 8) {
			//^24A5:09EC
			glbCurrentContainerItems[si++] = OBJECT_NULL;
			//^24A5:09F7
		}
		//^24A5:09FC
		glbObjectForContainer = rl;
	}
	//^24A5:0A02
	return;
}


//^0CEE:2EFB
U16 SkWinCore::IS_CONTAINER_MAP(ObjectID recordLink)
{
	//^0CEE:2EFB
	if (recordLink.DBType() == dbContainer) {
		if (GET_ADDRESS_OF_RECORD9(recordLink)->ContainerType() == 1) {
			return 1;
		}
	}
	//^0CEE:2F2B
	return 0;
}




//^2405:0029
U8 *SkWinCore::QUERY_GDAT_ITEM_NAME(U8 cls1, U8 cls2)
{
	ENTER(34);
	U8 bp22[30];
	U8 *bp04 = QUERY_GDAT_TEXT(cls1, cls2, 0x18, bp22);
	return FORMAT_SKSTR(bp04, _4976_5290);
}

//^2405:005E
// _2405_005e renamed GET_ITEM_NAME
U8 *SkWinCore::GET_ITEM_NAME(ObjectID recordLink)
{
	//^2405:005E
	ENTER(6);
	//^2405:0064
	ObjectID di = recordLink;
	//^2405:0067
	U8 iCategory = QUERY_CLS1_FROM_RECORD(di);	// bp01
	//^2405:0071
	U8 iItemNumber = QUERY_CLS2_FROM_RECORD(di);	// bp02
	//^2405:007B
	glbChampionBonesIndex = 0xffff;
	//^2405:0081
	// SPX: If item is CHAMPION'S BONES : I changed to function to get proper ID depending on DM1 or DM2 mode
	if (iCategory == GDAT_CATEGORY_MISCELLANEOUS && iItemNumber == GET_CHAMPION_BONES_ITEM_ID()) { // (bp01 == 0x15 && bp02 == 0x00) ==> ITEM == BONES
		//^2405:008D
		Miscellaneous_item *bp06 = GET_ADDRESS_OF_RECORDA(di);
		//^2405:009A
		i16 si = bp06->Who();
		//^2405:00A9
		if (si >= 0 && si < cd.pi.glbChampionsCount) {
			//^2405:00B3
			glbChampionBonesIndex = si;
		}
	}
	//^2405:00B7
	return QUERY_GDAT_ITEM_NAME(iCategory, iItemNumber);
}


//^0CEE:2773
U16 SkWinCore::IS_MISCITEM_CURRENCY(ObjectID rl)
{
	//^0CEE:2773
	ENTER(0);
	//^0CEE:2776
	if (true
		&& rl.DBType() == dbMiscellaneous_item
		&& (QUERY_GDAT_DBSPEC_WORD_VALUE(rl, GDAT_ITEM_STATS_GEN_FLAGS) & ITEM_FLAG_CURRENCY) != 0		// flags_currency = 0x4000
	) {
		//^0CEE:2794
		return 1;
	}
	//^0CEE:2799
	return 0;
}



//^0CEE:279D
void SkWinCore::COUNT_BY_COIN_TYPES(ObjectID rlMoneybox, i16 *piCount)
{
	//^0CEE:279D
	ENTER(2);
	//^0CEE:27A3
	ZERO_MEMORY(piCount, MONEY_ITEM_MAX*2);	// SPX: original 20 = 10*2
	//^0CEE:27B5
	for (ObjectID si = GET_ADDRESS_OF_RECORD9(rlMoneybox)->GetContainedObject(); si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
		//^0CEE:27C7
		if (IS_MISCITEM_CURRENCY(si) == 0)
			//^0CEE:27CF
			continue;
		//^0CEE:27D1
		U16 bp02 = GET_DISTINCTIVE_ITEMTYPE(si);
		//^0CEE:27DA
		for (i16 di = 0; di < glbCountMoneyItems; di++) {
			//^0CEE:27DE
			if (glbMoneyItemsIDTable[di] == bp02) {
				//^0CEE:27EB
				piCount[di] += GET_ADDRESS_OF_RECORDA(si)->Charge() +1;
			}
			//^0CEE:2809
		}
		//^0CEE:2810
	}
	//^0CEE:281D
	return;
}




//^48AE:04B7
i16 SkWinCore::GET_ITEM_ORDER_IN_CONTAINER(ObjectID rl, i16 xx)
{
	// if rl is money-box:
	//  xx=0, return 5. (6th expensive coin BLUE-GEM)

	//^48AE:04B7
	ENTER(142);
	//^48AE:04BD
	U8 bp008e[128];
	U8 *bp0e = QUERY_GDAT_TEXT(GDAT_CATEGORY_CONTAINERS, QUERY_CLS2_FROM_RECORD(rl), 0x40, bp008e);
    //^48AE:04E3
	if (*bp0e == 0)
		//^48AE:04E9
		//^48AE:05A7
		return -1;
	//^48AE:04EC
	U16 bp02 = 0;
	U16 di = 0;
	U16 bp08 = 0;
	//^48AE:04F6
	i16 si = -1;
	i16 bp04 = -1;

	while (bp02 == 0) {
		//^48AE:04F9
		U8 bp09 = *(bp0e++);
		//^48AE:0505
		if (bp09 >= '0' && bp09 <= '9') {
			//^48AE:050F
			di = di * 10 +bp09 -'0';
			//^48AE:0522
			continue;
		}
		//^48AE:0524
		switch (bp09) {
			case 'J':
				{
					//^48AE:0539
					if (di != 0) {
						//^48AE:053D
						bp0e--;
						//^48AE:0540
						break;
					}
                    //^48AE:0542
					bp04 = 256;
					//^48AE:0547
					continue;
				}
			case '-':
				{
					//^48AE:0549
					si = di;
					di = 0;
					//^48AE:054D
					continue;
				}
			case 0x00:
				{
					//^48AE:054F
					bp02 = 1;

					break;
				}
		}
		//^48AE:0554
		if (si < 0)
			si = di;
		//^48AE:055A
		for (; si <= di; bp08++, si++) {
			//^48AE:055C
			if (bp08 == xx) {
				//^48AE:0564
				for (i16 bp06 = 0; bp06 < glbCountMoneyItems; bp06++) {
					//^48AE:056B
					if (glbMoneyItemsIDTable[bp06] == si +bp04) {
						//^48AE:057B
						return bp06;
					}
					//^48AE:0580
				}
			}
			//^48AE:058C
		}
		//^48AE:0594
		di = 0;
		//^48AE:0596
		si = bp04 = -1;
		//^48AE:059E
	}
	//^48AE:05A7
	return -1;
}



//^24A5:0A06
// SPX _24a5_0a06 renamed MONEY_BOX_SURVEY
// This function will crash if more items are flagged "currency" than possible max (default = 10) (or increase MONEY_ITEM_MAX)
void SkWinCore::MONEY_BOX_SURVEY(ObjectID recordLink)
{
	//^24A5:0A06
	ENTER(36);
	//^24A5:0A0C
	i32 totalAmount = 0;	//bp06
	//^24A5:0A16
	i16 moneyTable[MONEY_ITEM_MAX];	//bp24[10]
	COUNT_BY_COIN_TYPES(recordLink, moneyTable);
	//^24A5:0A26
	U16 bp02 = 618;
	//^24A5:0A2B
	U8 bp10[10];
	for (i16 si = 0; si < MONEY_ITEM_MAX; si++) {
		//^24A5:0A2F
		i16 di = GET_ITEM_ORDER_IN_CONTAINER(recordLink, si);
		//^24A5:0A3C
		if (di >= 0) {
			//^24A5:0A40
			totalAmount += moneyTable[di] * glbMoneyItemsValueTable[di];
			//^24A5:0A65
			DRAW_VP_RC_STR(
				bp02++,
				glbPaletteT16[COLOR_LIGHTER_GRAY],
				SK_LTOA10(moneyTable[di], bp10)
				);
		}
		//^24A5:0A9D
	}
	//^24A5:0AA3
	DRAW_VP_RC_STR(
		569,
		glbPaletteT16[COLOR_LIGHTER_GRAY],
		SK_LTOA10(totalAmount, bp10)
		);
	//^24A5:0ACE
	return;
}




//^0CEE:2902
ObjectID SkWinCore::TAKE_COIN_FROM_WALLET(ObjectID rl, i16 xx)
{
	//^0CEE:2902
	ENTER(10);
	//^0CEE:2908
	U16 bp0a = glbMoneyItemsIDTable[xx];
	Container *bp08 = GET_ADDRESS_OF_RECORD9(rl);
	ObjectID di = bp08->GetContainedObject();
	ObjectID si = OBJECT_NULL;
	//^0CEE:292C
	for (; di != OBJECT_END_MARKER; di = GET_NEXT_RECORD_LINK(di)) {
		//^0CEE:292E
		if (di.DBType() == dbMiscellaneous_item) {
			if (GET_DISTINCTIVE_ITEMTYPE(di) == bp0a)
				si = di;
			continue;
		}
		//^0CEE:294A
		return OBJECT_NULL;
		//^0CEE:294F
	}
	//^0CEE:295C
	if (si != OBJECT_NULL) {
		//^0CEE:2961
		Miscellaneous_item *bp04 = GET_ADDRESS_OF_RECORDA(si);
		if (bp04->Charge() == 0) {
			//^0CEE:297B
			CUT_RECORD_FROM(si, &bp08->w2, -1, 0);
		}
		else {
			//^0CEE:2992
			bp04->Charge(bp04->Charge() -1);
			si = ALLOC_NEW_DBITEM(bp0a);
		}
	}
	//^0CEE:29B3
	return si;
}


//^0CEE:2F2F
U16 SkWinCore::SET_DESTINATION_OF_MINION_MAP(ObjectID rlContainer, i16 xx, i16 yy, U16 zz)
{
	//^0CEE:2F2F
	ENTER(4);
	//^0CEE:2F35
	U16 di = glbCurrentMapIndex;
	//^0CEE:2F39
	CHANGE_CURRENT_MAP_TO(zz);
	//^0CEE:2F41
	U16 si = (xx >= 0 && xx < glbCurrentMapWidth && yy >= 0 && yy < glbCurrentMapHeight) ? 1 : 0;
	//^0CEE:2F66
	if (si != 0) {
		//^0CEE:2F6C
		Container *bp04 = GET_ADDRESS_OF_RECORD9(rlContainer);
		//^0CEE:2F7A
		bp04->SetDestX(xx);
		//^0CEE:2F8C
		bp04->SetDestY(yy);
		//^0CEE:2F9F
		bp04->SetDestMap(zz);
	}
	//^0CEE:2FB1
	CHANGE_CURRENT_MAP_TO(di);
	//^0CEE:2FB7
	return si;
}


//^0CEE:2ABC
U16 SkWinCore::QUERY_GDAT_FOOD_VALUE_FROM_RECORD(ObjectID rl)
{
	//^0CEE:2ABC
	ENTER(0);
	//^0CEE:2ABF
	return QUERY_GDAT_DBSPEC_WORD_VALUE(rl, 3);
}

//^24A5:10B3
U16 SkWinCore::IS_MISCITEM_DRINK_WATER(ObjectID rlMiscItem)
{
	//^24A5:10B3
	ENTER(0);
	//^24A5:10B7
	ObjectID si = rlMiscItem;
	//^24A5:10BA
	if (true
		&& (QUERY_GDAT_DBSPEC_WORD_VALUE(si, 0) & 1) != 0
		&& ADD_ITEM_CHARGE(si, 0) != 0
	) {
		//^24A5:10D7
		ADD_ITEM_CHARGE(si, -1);
		//^24A5:10E1
		if (si == cd.pi.glbLeaderHandPossession.object) {
			//^24A5:10E7
			TAKE_OBJECT(si, 0);
		}
		//^24A5:10F1
		return 1;
	}
	//^24A5:10F6
	return 0;
}


//^2FCF:0109
U16 SkWinCore::IS_OBJECT_FLOATING(ObjectID rl)
{
	// return 0 if object can fall down.
	// return 1 if object cannot be fall down.

	//^2FCF:0109
	ENTER(0);
	//^2FCF:010D
	U16 si = rl.DBType();
	if (si == dbCreature) {
		return IS_CREATURE_FLOATING(rl);
	}
	//^2FCF:0128
	return (si == dbMissile || si == dbCloud) ? 1 : 0;
}




//^48AE:03DE
// SPX: Isn't a load misc only for money item ??
void SkWinCore::LOAD_MISCITEM()
{
	//^48AE:03DE
	ENTER(4);
	//^48AE:03E4
	if (glbMiscItemsLoaded != 0)
		return;
	//^48AE:03EE
	glbMiscItemsLoaded = 1;
	//^48AE:03F4
	// SPX: Original code loads 128 (0x80) items, GDAT can contain more!
	X8 max = ITEM_MISC_LOAD_MAX;	// 0x80
	if (SkCodeParam::bUseDM2ExtendedMode)
		max = ITEM_LOAD_MAX_EXTEND_1;
	for (X8 item = 0; item < max; item++) {
		//^48AE:03FB
		if ((QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_MISCELLANEOUS, item, dtWordValue, GDAT_ITEM_STATS_GEN_FLAGS) & ITEM_FLAG_CURRENCY) == 0)
			continue;
		U16 moneyValue = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_MISCELLANEOUS, item, dtWordValue, GDAT_ITEM_STATS_MONEY_VALUE);
		//^48AE:0429
		i16 si;
		for (si = 0; si < glbCountMoneyItems; si++) {
			//^48AE:042D
			if (glbMoneyItemsValueTable[si] >= moneyValue)
				break;
			//^48AE:0437
		}
		//^48AE:043E
		if (si < glbCountMoneyItems) {
			//^48AE:0444
			i16 bp04 = (glbCountMoneyItems -si) << 1;
			MOVE_MEMORY(&glbMoneyItemsValueTable[si], &glbMoneyItemsValueTable[si +1], bp04);
			MOVE_MEMORY(&glbMoneyItemsIDTable[si], &glbMoneyItemsIDTable[si +1], bp04);
		}
		//^48AE:048B
		glbMoneyItemsValueTable[si] = moneyValue;
		glbMoneyItemsIDTable[si] = item +0x100;
		glbCountMoneyItems++;
		//^48AE:04A7
	}
	//^48AE:04B3
	return;
}


//^0CEE:2515
void SkWinCore::SET_ITEMTYPE(ObjectID recordLink, Bit8u itemType)
{
	//^0CEE:2515
	//^0CEE:251A
	ObjectID si = recordLink;
	//^0CEE:251D
	if (si != OBJECT_NULL && si < OBJECT_EFFECT_FIREBALL) {	// oFF80
		//^0CEE:252D
		GenericRecord *_bp04 = GET_ADDRESS_OF_RECORD(si);
		//^0CEE:2539
		switch (si.DBType()) {
			case dbCreature:
				{
					Creature *bp04 = _bp04->castToCreature();

					//^0CEE:2554
					bp04->CreatureType(itemType);
					//^0CEE:255E
					break;
				}
			case dbWeapon:
				{
					Weapon *bp04 = _bp04->castToWeapon();

					//^0CEE:2560
					//^0CEE:25C0
					bp04->ItemType(itemType);

					break;
				}
			case dbCloth:
				{
					Cloth *bp04 = _bp04->castToCloth();

					//^0CEE:2562
					//^0CEE:25C0
					bp04->ItemType(itemType);

					break;
				}
			case dbScroll:
				{
					break;
				}
			case dbPotion:
				{
					Potion *bp04 = _bp04->castToPotion();

					//^0CEE:2564
					bp04->PotionType(itemType);
					//^0CEE:2578
					break;
				}
			case dbContainer:
				{
					Container *bp04 = _bp04->castToContainer();

					//^0CEE:257A
					bp04->ContainerFullType(itemType);
					//^0CEE:25AA
					if (bp04->ContainerType() == 1) {
						//^0CEE:25B8
						bp04->w6_0_f(0xffff);
					}
					break;
				}
			case dbMiscellaneous_item:
				{
					Miscellaneous_item *bp04 = _bp04->castToMisc();

					//^0CEE:25C0
					bp04->ItemType(itemType);

					break;
				}
		}
	}
	//^0CEE:25D4
}



//^0CEE:2391
U16 SkWinCore::GET_DISTINCTIVE_ITEMTYPE(ObjectID recordLink)
{
	// get the object type of record in the form of uniquely identified number.
	// e.g. it always returns 2 if you get any type of weapon Torch record.
	// returns 511 if record is no meaningful to distinct.

	if (SkCodeParam::bDM1Mode == true)
		return GET_DM1_DISTINCTIVE_ITEMTYPE(recordLink);

	//^0CEE:2391
	//^0CEE:2397
	if (recordLink != OBJECT_NULL) {
		//^0CEE:239D
		Bit8u bp01 = QUERY_CLS2_FROM_RECORD(recordLink);
		//^0CEE:23A9
		U16 di = recordLink.DBType();
		//^0CEE:23B4
		U16 si = glbActivationItemRangePerDB[di];
		//^0CEE:23BC
		if ((si & 0x8000) != 0) {
			//^0CEE:23C2
			si &= 0x7fff;
			bp01 = 0;
		}
		//^0CEE:23CA
		return si + bp01;
	}
	else {
		//^0CEE:23D7
		return 511;
	}
}

//^0CEE:2CD2
U16 SkWinCore::QUERY_ITEM_MONEY_VALUE(ObjectID xx)
{
	//^0CEE:2CD2
	ENTER(4);
	//^0CEE:2CD6
	i32 bp04 = QUERY_ITEM_VALUE(xx, 2);
	return (bp04 <= MONEY_MAX_VALUE) ? (U16)bp04 : MONEY_MAX_VALUE;	// 65535
	//^0CEE:2CFD
}
