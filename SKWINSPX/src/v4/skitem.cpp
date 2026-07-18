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
		if (iInvSlot < C13_INVENTORY_BACKPACK_1) {	// 0x0D = 13 = backpack first
			if (iInvSlot < 0) {
				return iFlagsValue & ITEM_EQUIP_FLAG_0400;	// 0x0400
			}
			return _4976_49e8_ItemHoldMask[RCJ(C13_INVENTORY_BACKPACK_1,iInvSlot)] & iFlagsValue;	// _4976_49e8[RCJ(13,si)] & di
		}
		return 0;
	}
	if (iInvSlot < C13_INVENTORY_BACKPACK_1) {	// 0x000D
		if (iInvSlot > C01_INVENTORY_HAND_LAST) {	// 0x0001
			return iFlagsValue & _4976_49e8_ItemHoldMask[RCJ(C13_INVENTORY_BACKPACK_1,iInvSlot)];	// _4976_49e8[RCJ(13,si)]
		}
	}
	// That part would be for arrows within quiver which are moved to scabbard
	if (iInvSlot >= C30_INVENTORY_OVER_1) {	// 0x001E
		if (iInvSlot < C38_INVENTORY_MAX_OVER) {	// 00x26
			if ((iFlagsValue & ITEM_EQUIP_FLAG_CONTAINER_2) != 0) {	// 0x8000
				return 0;
			}
			if (IS_ITEM_FIT_FOR_EQUIP(_4976_3de2[(cd.pi.glbChampionIndex << 1) + (glbSelectedHandAction)], C12_INVENTORY_SCABBARD_1, 0x0000) != 0) {	// 0x000c
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
	ENTER(4);
	if (glbObjectForContainer == OBJECT_NULL)
		return;
	GenericContainerRecord *bp04 = GET_ADDRESS_OF_GENERIC_CONTAINER_RECORD(glbObjectForContainer);
	glbObjectForContainer = OBJECT_NULL;
	i16 si;
	for (si = 0; si < 8; si++) {
		ObjectID di = glbCurrentContainerItems[si];
		if (di != OBJECT_NULL) {
			glbCurrentContainerItems[si] = OBJECT_NULL;
			APPEND_RECORD_TO(di, &bp04->possession, -1, 0);
		}
	}
	return;
}


//^0CEE:29B9
U16 SkWinCore::GET_ITEM_MAX_CHARGE(ObjectID recordLink)
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

//^0CEE:29EC
U16 SkWinCore::ADD_ITEM_CHARGE(ObjectID recordLink, i16 delta)
{
	U16 si = 0;
	if (recordLink == OBJECT_NULL)
		return si;
	GenericRecord *bp04 = (GenericRecord *)GET_ADDRESS_OF_RECORD(recordLink);
	U16 bp06 = recordLink.DBType();
	U16 di;
	switch (bp06) {
		case dbWeapon:
			si = reinterpret_cast<Weapon *>(bp04)->Charges();
			di = 15;	// SPX: could call GET_ITEM_MAX_CHARGE for this
			break;
		case dbCloth:
			si = reinterpret_cast<Cloth *>(bp04)->Charges();
			di = 15;	// SPX: could call GET_ITEM_MAX_CHARGE for this
			break;
		case dbMiscellaneous_item:
			si = reinterpret_cast<Miscellaneous_item *>(bp04)->Compass();
			di = 3;		// SPX: could call GET_ITEM_MAX_CHARGE for this
			break;
		default:
			return si;
	}
	si += delta;
	si = BETWEEN_VALUE(0, si, di);
	switch (bp06) {
		case dbWeapon:
			reinterpret_cast<Weapon *>(bp04)->Charges(si);
			break;
		case dbCloth:
			reinterpret_cast<Cloth *>(bp04)->Charges(si);
			break;
		case dbMiscellaneous_item:
			reinterpret_cast<Miscellaneous_item *>(bp04)->Compass(si);
			break;
		default:
			return si;
	}
	return si;
}


//^24A5:0990
// SPX: _24a5_0990 renamed __CHECK_ROOM_FOR_CONTAINER
void SkWinCore::__CHECK_ROOM_FOR_CONTAINER(ObjectID rl, Container *ref)
{
	ENTER(0);
	if (rl != OBJECT_NULL && rl != glbObjectForContainer) {
		X16 di = 0;
		X16 si = 0;
		while (ref->GetContainedObject() != OBJECT_END_MARKER) {
			if (++di > 8)
				break;
			glbCurrentContainerItems[si++] = ref->GetContainedObject();
			CUT_RECORD_FROM(ref->GetContainedObject(), &ref->w2, -1, 0);
		};
		while (si < 8) {
			glbCurrentContainerItems[si++] = OBJECT_NULL;
		}
		glbObjectForContainer = rl;
	}
	return;
}


//^0CEE:2EFB
U16 SkWinCore::IS_CONTAINER_MAP(ObjectID recordLink)
{
	if (recordLink.DBType() == dbContainer) {
		if (GET_ADDRESS_OF_RECORD9(recordLink)->ContainerType() == 1) {
			return 1;
		}
	}
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
	ENTER(6);
	ObjectID oItemObject = recordLink;	// di
	U8 iGDatCategory = QUERY_CLS1_FROM_RECORD(oItemObject);	// bp01
	U8 iGDatItemId = QUERY_CLS2_FROM_RECORD(oItemObject);	// bp02
	glbChampionBonesIndex = 0xFFFF;
	// SPX: If item is CHAMPION'S BONES : I changed to function to get proper ID depending on DM1 or DM2 mode
	if (iGDatCategory == GDAT_CATEGORY_x15_MISCELLANEOUS && iGDatItemId == GET_CHAMPION_BONES_ITEM_ID()) { // (bp01 == 0x15 && bp02 == 0x00) ==> ITEM == BONES
		Miscellaneous_item* xMiscItem = GET_ADDRESS_OF_RECORDA(oItemObject);	// bp06
		i16 iBonesID = xMiscItem->Who();	// si
		if (iBonesID >= 0 && iBonesID < cd.pi.glbChampionsCount) {
			glbChampionBonesIndex = iBonesID;
		}
	}
	return QUERY_GDAT_ITEM_NAME(iGDatCategory, iGDatItemId);
}


//^0CEE:2773
U16 SkWinCore::IS_MISCITEM_CURRENCY(ObjectID rl)
{
	ENTER(0);
	if (true
		&& rl.DBType() == dbMiscellaneous_item
		&& (QUERY_GDAT_DBSPEC_WORD_VALUE(rl, GDAT_ITEM_STATS_GEN_FLAGS) & ITEM_FLAG_CURRENCY) != 0		// flags_currency = 0x4000
	) {
		return 1;
	}
	return 0;
}



//^0CEE:279D
void SkWinCore::COUNT_BY_COIN_TYPES(ObjectID rlMoneybox, i16 *piCount)
{
	ENTER(2);
	ZERO_MEMORY(piCount, MONEY_ITEM_MAX*2);	// SPX: original 20 = 10*2
	for (ObjectID si = GET_ADDRESS_OF_RECORD9(rlMoneybox)->GetContainedObject(); si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
		if (IS_MISCITEM_CURRENCY(si) == 0)
			continue;
		U16 bp02 = GET_DISTINCTIVE_ITEMTYPE(si);
		for (i16 di = 0; di < glbCountMoneyItems; di++) {
			if (glbMoneyItemsIDTable[di] == bp02) {
				piCount[di] += GET_ADDRESS_OF_RECORDA(si)->Charge() + 1;
			}
		}
	}
	return;
}




//^48AE:04B7
i16 SkWinCore::GET_ITEM_ORDER_IN_CONTAINER(ObjectID rl, i16 xx)
{
	// if rl is money-box:
	//  xx=0, return 5. (6th expensive coin BLUE-GEM)

	ENTER(142);
	U8 bp008e[128];
	U8 *bp0e = QUERY_GDAT_TEXT(GDAT_CATEGORY_x14_CONTAINERS, QUERY_CLS2_FROM_RECORD(rl), 0x40, bp008e);
	if (*bp0e == 0)
		return -1;
	U16 bp02 = 0;
	U16 di = 0;
	U16 bp08 = 0;
	i16 si = -1;
	i16 bp04 = -1;

	while (bp02 == 0) {
		U8 bp09 = *(bp0e++);
		if (bp09 >= '0' && bp09 <= '9') {
			di = di * 10 +bp09 -'0';
			continue;
		}
		switch (bp09) {
			case 'J':
				{
					if (di != 0) {
						bp0e--;
						break;
					}
					bp04 = 256;
					continue;
				}
			case '-':
				{
					si = di;
					di = 0;
					continue;
				}
			case 0x00:
				{
					bp02 = 1;

					break;
				}
		}
		if (si < 0)
			si = di;
		for (; si <= di; bp08++, si++) {
			if (bp08 == xx) {
				for (i16 bp06 = 0; bp06 < glbCountMoneyItems; bp06++) {
					if (glbMoneyItemsIDTable[bp06] == si +bp04) {
						return bp06;
					}
				}
			}
		}
		di = 0;
		si = bp04 = -1;
	}
	return -1;
}



//^24A5:0A06
// SPX _24a5_0a06 renamed MONEY_BOX_SURVEY
// This function will crash if more items are flagged "currency" than possible max (default = 10) (or increase MONEY_ITEM_MAX)
void SkWinCore::MONEY_BOX_SURVEY(ObjectID recordLink)
{
	ENTER(36);
	i32 totalAmount = 0;	//bp06
	i16 moneyTable[MONEY_ITEM_MAX];	//bp24[10]
	COUNT_BY_COIN_TYPES(recordLink, moneyTable);
	U16 bp02 = 618;
	U8 bp10[10];
	for (i16 si = 0; si < MONEY_ITEM_MAX; si++) {
		i16 di = GET_ITEM_ORDER_IN_CONTAINER(recordLink, si);
		if (di >= 0) {
			totalAmount += moneyTable[di] * glbMoneyItemsValueTable[di];
			DRAW_VP_RC_STR(
				bp02++,
				glbPaletteT16[COLOR_LIGHTER_GRAY],
				SK_LTOA10(moneyTable[di], bp10)
				);
		}
	}
	DRAW_VP_RC_STR(
		569,
		glbPaletteT16[COLOR_LIGHTER_GRAY],
		SK_LTOA10(totalAmount, bp10)
		);
	return;
}




//^0CEE:2902
ObjectID SkWinCore::TAKE_COIN_FROM_WALLET(ObjectID rl, i16 xx)
{
	ENTER(10);
	U16 bp0a = glbMoneyItemsIDTable[xx];
	Container *bp08 = GET_ADDRESS_OF_RECORD9(rl);
	ObjectID di = bp08->GetContainedObject();
	ObjectID si = OBJECT_NULL;
	for (; di != OBJECT_END_MARKER; di = GET_NEXT_RECORD_LINK(di)) {
		if (di.DBType() == dbMiscellaneous_item) {
			if (GET_DISTINCTIVE_ITEMTYPE(di) == bp0a)
				si = di;
			continue;
		}
		return OBJECT_NULL;
	}
	if (si != OBJECT_NULL) {
		Miscellaneous_item *bp04 = GET_ADDRESS_OF_RECORDA(si);
		if (bp04->Charge() == 0) {
			CUT_RECORD_FROM(si, &bp08->w2, -1, 0);
		}
		else {
			bp04->Charge(bp04->Charge() -1);
			si = ALLOC_NEW_DBITEM(bp0a);
		}
	}
	return si;
}


//^0CEE:2F2F
U16 SkWinCore::SET_DESTINATION_OF_MINION_MAP(ObjectID rlContainer, i16 xx, i16 yy, U16 zz)
{
	ENTER(4);
	U16 di = glbCurrentMapIndex;
	CHANGE_CURRENT_MAP_TO(zz);
	U16 si = (xx >= 0 && xx < glbCurrentMapWidth && yy >= 0 && yy < glbCurrentMapHeight) ? 1 : 0;
	if (si != 0) {
		Container *bp04 = GET_ADDRESS_OF_RECORD9(rlContainer);
		bp04->SetDestX(xx);
		bp04->SetDestY(yy);
		bp04->SetDestMap(zz);
	}
	CHANGE_CURRENT_MAP_TO(di);
	return si;
}


//^0CEE:2ABC
U16 SkWinCore::QUERY_GDAT_FOOD_VALUE_FROM_RECORD(ObjectID rl)
{
	ENTER(0);
	return QUERY_GDAT_DBSPEC_WORD_VALUE(rl, 3);
}

//^24A5:10B3
U16 SkWinCore::IS_MISCITEM_DRINK_WATER(ObjectID rlMiscItem)
{
	ENTER(0);
	ObjectID si = rlMiscItem;
	if (true
		&& (QUERY_GDAT_DBSPEC_WORD_VALUE(si, 0) & 1) != 0
		&& ADD_ITEM_CHARGE(si, 0) != 0
	) {
		ADD_ITEM_CHARGE(si, -1);
		if (si == cd.pi.glbLeaderHandPossession.object) {
			TAKE_OBJECT(si, 0);
		}
		return 1;
	}
	return 0;
}


//^2FCF:0109
U16 SkWinCore::IS_OBJECT_FLOATING(ObjectID rl)
{
	// return 0 if object can fall down.
	// return 1 if object cannot be fall down.

	ENTER(0);
	U16 si = rl.DBType();
	if (si == dbCreature) {
		return IS_CREATURE_FLOATING(rl);
	}
	return (si == dbMissile || si == dbCloud) ? 1 : 0;
}




//^48AE:03DE
// SPX: Isn't a load misc only for money item ??
void SkWinCore::LOAD_MISCITEM()
{
	ENTER(4);
	if (glbMiscItemsLoaded != 0)
		return;
	glbMiscItemsLoaded = 1;
	// SPX: Original code loads 128 (0x80) items, GDAT can contain more!
	X8 max = ITEM_MISC_LOAD_MAX;	// 0x80
	if (SkCodeParam::bUseDM2ExtendedMode)
		max = ITEM_LOAD_MAX_EXTEND_1;
	for (X8 item = 0; item < max; item++) {
		if ((QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x15_MISCELLANEOUS, item, dtWordValue, GDAT_ITEM_STATS_GEN_FLAGS) & ITEM_FLAG_CURRENCY) == 0)
			continue;
		U16 moneyValue = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x15_MISCELLANEOUS, item, dtWordValue, GDAT_ITEM_STATS_MONEY_VALUE);
		i16 si;
		for (si = 0; si < glbCountMoneyItems; si++) {
			if (glbMoneyItemsValueTable[si] >= moneyValue)
				break;
		}
		if (si < glbCountMoneyItems) {
			i16 bp04 = (glbCountMoneyItems -si) << 1;
			MOVE_MEMORY(&glbMoneyItemsValueTable[si], &glbMoneyItemsValueTable[si +1], bp04);
			MOVE_MEMORY(&glbMoneyItemsIDTable[si], &glbMoneyItemsIDTable[si +1], bp04);
		}
		glbMoneyItemsValueTable[si] = moneyValue;
		glbMoneyItemsIDTable[si] = item +0x100;
		glbCountMoneyItems++;
	}
	return;
}


//^0CEE:2515
void SkWinCore::SET_ITEMTYPE(ObjectID recordLink, Bit8u itemType)
{
	ObjectID si = recordLink;
	if (si != OBJECT_NULL && si < OBJECT_EFFECT_FIREBALL) {	// oFF80
		GenericRecord *_bp04 = GET_ADDRESS_OF_RECORD(si);
		switch (si.DBType()) {
			case dbCreature:
				{
					Creature *bp04 = _bp04->castToCreature();
					bp04->CreatureType(itemType);
					break;
				}
			case dbWeapon:
				{
					Weapon *bp04 = _bp04->castToWeapon();
					bp04->ItemType(itemType);
					break;
				}
			case dbCloth:
				{
					Cloth *bp04 = _bp04->castToCloth();
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
					bp04->PotionType(itemType);
					break;
				}
			case dbContainer:
				{
					Container *bp04 = _bp04->castToContainer();
					bp04->ContainerFullType(itemType);
					if (bp04->ContainerType() == 1) {
						bp04->w6_0_f(0xffff);
					}
					break;
				}
			case dbMiscellaneous_item:
				{
					Miscellaneous_item *bp04 = _bp04->castToMisc();
					bp04->ItemType(itemType);
					break;
				}
		}
	}
}



//^0CEE:2391
U16 SkWinCore::GET_DISTINCTIVE_ITEMTYPE(ObjectID recordLink)
{
	// get the object type of record in the form of uniquely identified number.
	// e.g. it always returns 2 if you get any type of weapon Torch record.
	// returns 511 if record is no meaningful to distinct.

	if (SkCodeParam::bDM1Mode == true)
		return GET_DM1_DISTINCTIVE_ITEMTYPE(recordLink);

	if (recordLink != OBJECT_NULL) {
		U8 bp01 = QUERY_CLS2_FROM_RECORD(recordLink);
		U16 di = recordLink.DBType();
		U16 si = glbActivationItemRangePerDB[di];
		if ((si & 0x8000) != 0) {
			si &= 0x7fff;
			bp01 = 0;
		}
		return si + bp01;
	}
	else {
		return 511;
	}
}

//^0CEE:2CD2
U16 SkWinCore::QUERY_ITEM_MONEY_VALUE(ObjectID oItem)
{
	ENTER(4);
	i32 iMoneyValue = QUERY_ITEM_VALUE(oItem, 2);	// bp04
	return (iMoneyValue <= MONEY_MAX_VALUE) ? (U16)iMoneyValue : MONEY_MAX_VALUE;	// 65535
}
