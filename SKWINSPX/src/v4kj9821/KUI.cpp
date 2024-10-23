#include <StdAfx.h>	// Required for MVC6 compilation

#include <SkVer.H>

#include <KCORE.H>
#include <SkDebug.h>

#if defined(_USE_MFC80) || defined(_USE_MFC60)
#include <SkMFC.h>
#include <resource.h>
#endif // defined(_USE_MFC80) || defined(_USE_MFC60)


//^1031:156F
// SPX: _1031_156f renamed ADJUST_UI_EVENT
void SkWinCore::ADJUST_UI_EVENT(MousePosition *ref)
{
	//^1031:156F
	ENTER(6);
	//^1031:1575
	Bit16u si = ref->event;
	//^1031:157C
	SkD((DLV_EUI, "Event UI %03d (%02X) %s triggered.\n", si, si, static_cast<LPCSTR>(getUIEventName(si))));
	//if (si >= 0x74 && si <= 0x7b) { // access to hand item action
	if (si >= UI_EVENTCODE_CHAMPION1_ACTION_HAND_RIGHT && si <= UI_EVENTCODE_CHAMPION4_ACTION_HAND_LEFT) {
		//^1031:1586
		si -= UI_EVENTCODE_CHAMPION1_ACTION_HAND_RIGHT;
		//^1031:1589
		Bit16u bp06 = ((si & 1) != 0) ? 1 : 0;
		//^1031:1599
		i16 di = GET_PLAYER_AT_POSITION(((si >> 1) + glbPlayerDir) & 3);
		//^1031:15AD
		if (false
			|| di < 0 
			|| glbChampionSquad[di +bp06].handCooldown[0] != 0 
			|| IS_ITEM_HAND_ACTIVABLE(di, glbChampionSquad[di].Possess(bp06), bp06) == 0
		) {
			//^1031:15E6
			si = 0;
		}
		else {
			//^1031:15EB
			si = (di << 1) +bp06 +UI_EVENTCODE_CHAMPION1_ACTION_HAND_RIGHT;
		}
	}
	//^1031:15FA
	//else if (si >= 0x5f && si <= 0x62) {
	else if (si >= UI_EVENTCODE_SPELL_OR_LEADER_1 && si <= UI_EVENTCODE_SPELL_OR_LEADER_4) {
		//^1031:160A
		i16 di = GET_PLAYER_AT_POSITION((si -0x5f +glbPlayerDir) & 3);
		//^1031:161F
		if (di < 0) {
			//^1031:1623
			//^1031:15E6
			si = 0;
		}
		else {
			//^1031:1625
			i16 bp04;
			if (si <= UI_EVENTCODE_SPELL_OR_LEADER_2) {
				//^1031:162A
				bp04 = ref->rc6.y + ref->rc6.cy -1 - ref->YPos;
			}
			else {
				//^1031:163C
				bp04 = ref->YPos - ref->rc6.y;
			}
			//^1031:164A
			i16 bp02;
			//if (si == 0x60 || si == 0x61) {
			if (si == UI_EVENTCODE_SPELL_OR_LEADER_2 || si == UI_EVENTCODE_SPELL_OR_LEADER_3) {
				//^1031:1654
				bp02 = ref->XPos - ref->rc6.x;
			}
			else {
				//^1031:1664
				bp02 = ref->rc6.x + ref->rc6.cx -1 - ref->XPos;
			}
			//^1031:1676
			if (bp02 <= bp04) {
				//^1031:167E
				si -= 0x4f;
			}
			//^1031:1683
			else if (glbChampionSquad[di].herob44 != 0) {
				//^1031:1693
				si = 0;
			}
		}
	}
	else {
		//^1031:1607
		return;
	}
	//^1031:1695
	ref->event = si;
	//^1031:169C
	return;
}

//^1031:0D9E
// SPX: _1031_0d9e renamed TRANSMIT_UI_EVENT
Bit8u *SkWinCore::TRANSMIT_UI_EVENT(MousePosition *xx)
{
	//^1031:0D9E
	ENTER(0);
	//^1031:0DA3
	ADJUST_UI_EVENT(xx);
	//^1031:0DB0
	Bit16u si = xx->event;
	//^1031:0DB7
	if (si == 0 || si > UI_EVENTCODE_LAST_EVENT) {
		//^1031:0DC1
		return 0;
	}
	Bit16u di;
	//^1031:0DC8
	//if ((si >= 0x14 && si < 0x42) || (si >= 0x7d && si <= 0x81) || (si >= 0x10 && si <= 0x13)) {
	if ((si >= UI_EVENTCODE_ITEM_SLOT_FIRST && si < UI_EVENTCODE_ITEM_SLOT_LAST) 
		|| (si >= UI_EVENTCODE_TAKE_CHAMPION_TOP_LEFT && si <= UI_EVENTCODE_UPDATE_CHAMPION_POSITION) 
		|| (si >= UI_EVENTCODE_010 && si <= UI_EVENTCODE_013)) {
		//^1031:0DE7
		di = 1;
		goto _0f23;
	}
	//^1031:0DED
	//else if (si >= 0x01 && si <= 0x06) {
	else if (si >= UI_EVENTCODE_ARROW_DIR_FIRST && si <= UI_EVENTCODE_ARROW_DIR_LAST) {
		//^1031:0DF7
		di = (si - UI_EVENTCODE_ARROW_DIR_FIRST) * 15 +2;
		goto _0f23;
	}
	//^1031:0E04
	//else if (si >= 0x74 && si <= 0x7b) {
	else if (si >= UI_EVENTCODE_CHAMPION1_ACTION_HAND_RIGHT && si <= UI_EVENTCODE_CHAMPION4_ACTION_HAND_LEFT) {
		//^1031:0E0E
		di = ((si -UI_EVENTCODE_CHAMPION1_ACTION_HAND_RIGHT) & 1) * 5 +0x5c;
		goto _0f23;
	}
	//^1031:0E21
	//else if (si >= 0x5f && si <= 0x62) {
	else if (si >= UI_EVENTCODE_SPELL_OR_LEADER_1 && si <= UI_EVENTCODE_SPELL_OR_LEADER_4) {
		//^1031:0E2B
		di = (si -UI_EVENTCODE_SPELL_OR_LEADER_1) * 5 +0x66;
		goto _0f23;
	}
	//^1031:0E3B
	//else if (si == 0x70) {
	else if (si == UI_EVENTCODE_HAND_RUNE_QUIT) {
		//^1031:0E40
		di = 0x7a;
		goto _0f23;
	}
	//^1031:0E46
	//else if (si == 0x5d || si == 0x5e) {
	else if (si == UI_EVENTCODE_CHAMPION_TURN_LEFT || si == UI_EVENTCODE_CHAMPION_TURN_RIGHT) {
		//^1031:0E50
		di = ((si -UI_EVENTCODE_CHAMPION_TURN_LEFT) << 3) +0x82;
		goto _0f23;
	}
	//^1031:0E5D
	//else if (si >= 0x65 && si <= 0x6a) {
	else if (si >= UI_EVENTCODE_ADD_RUNE_1 && si <= UI_EVENTCODE_ADD_RUNE_6) {
		//^1031:0E67
		di = 0x92;
		goto _0f23;
	}
	//^1031:0E6D
	//else if (si == 0x6b) {
	else if (si == UI_EVENTCODE_REMOVE_RUNE) {
		//^1031:0E72
		di = 0x96;
		goto _0f23;
	}
	//^1031:0E78
	//else if (si == 0x6c) {
	else if (si == UI_EVENTCODE_VALIDATE_SPELL) {
		//^1031:0E7D
		di = 0x9e;
		goto _0f23;
	}
	//^1031:0E83
	//else if (si >= 0x71 && si <= 0x73) {
	else if (si >= UI_EVENTCODE_HAND_ACTION_1 && si <= UI_EVENTCODE_HAND_ACTION_3) {
		//^1031:0E8D
		di = (si -UI_EVENTCODE_HAND_ACTION_1) * 5 +0xa2;
		goto _0f23;
	}
	//^1031:0E9C
	//else if (si == 0x0b || si == 0xa1) {
	else if (si == UI_EVENTCODE_RETURN_VIEWPORT || si == UI_EVENTCODE_EXIT_CRYOCELL) {
		//^1031:0EA7
		di = 0xb1;
		goto _0f23;
	}
	//^1031:0EAC
	//else if (si == 0x91) {
	else if (si == UI_EVENTCODE_SLEEP) {
		//^1031:
		di = 0xb6;
		goto _0f23;
	}
	//^1031:0EB7
	//else if (si == 0x8c) {
	else if (si == UI_EVENTCODE_DISK_OP) {
		//^1031:0EBD
		di = 0xbb;
		goto _0f23;
	}
	//^1031:0EC2
	//else if (si >= 0x97 && si <= 0x9a) {
	else if (si >= UI_EVENTCODE_MAGIC_MAP_RUNE_1 && si <= UI_EVENTCODE_MAGIC_MAP_RUNE_4) {
		//^1031:0ECE
		di = (si -UI_EVENTCODE_MAGIC_MAP_RUNE_1) * 7 +0xc0;
		goto _0f23;
	}
	//^1031:0EDF
	//else if (si >= 0xdb && si <= 0xde) {
	else if (si >= UI_EVENTCODE_DIALOG_BUTTON_1 && si <= UI_EVENTCODE_DIALOG_BUTTON_4) {
		//^1031:0EEB
		switch (_4976_4bd2 -1) {
			case 0:
			case 1:
				//^1031:0EFC
				di = 0xe3;
				break;
			case 2:
				//^1031:0F01
				if (si < UI_EVENTCODE_DIALOG_BUTTON_2) {
					//^1031:0F05
					//^1031:0EFC
					di = 0xe3;
				}
				else {
					//^1031:0F07
					di = 0xdc;
				}
				break;
			case 3:
				//^1031:0F07
				di = 0xdc;
				break;
			case 4:
			case 5:
				//^1031:0F0C
				if (si <= UI_EVENTCODE_DIALOG_BUTTON_2) {
					//^1031:0F10
					//^1031:0F07
					di = 0xdc;
					break;
				}
				//^1031:0F12
				di = 1;
				break;
			default:
				//^1031:0F12
				di = 1;
				break;
		}
		goto _0f23;
	}
	//^1031:0F17
	//else if (si == 0x50) {
	else if (si == UI_EVENTCODE_CLICK_VIEWPORT) {
		//^1031:0F1C
		di = 0xea;
		goto _0f23;
	}
	else {
		//^1031:0F21
		di = 0;
	}
	//^1031:0F23
_0f23:
	return &_4976_18b9[RCJ(236,di)];
}



//^2C1D:073D
ObjectID SkWinCore::REMOVE_OBJECT_FROM_HAND()
{
	// CSBwinSimilarity: TAG01576a,RemoveObjectFromHand

	//^2C1D:073D
	ENTER(0);
	//^2C1D:0741
	ObjectID si = glbLeaderHandPossession.object;
	//^2C1D:0746
	if (si != OBJECT_NULL) {
		//^2C1D:074B
		glbLeaderItemFlags = 0;
		glbLeaderItemWeight = 0;
		glbLeaderHandPossession.object = OBJECT_NULL;
		_4976_57de = 0xff;
		//^2C1D:0762
		FIRE_HIDE_MOUSE_CURSOR();
		_2405_00e7();
		_443c_0434();
		FIRE_SHOW_MOUSE_CURSOR();
		//^2C1D:0776
		PROCESS_ITEM_BONUS(glbChampionLeader, si, -1, -1);
// SPX: that part is for giving item sound when put it back to inventory or on ground
		if (SkCodeParam::bUseExtendedSound)
			QUEUE_NOISE_GEN2(QUERY_CLS1_FROM_RECORD(si), QUERY_CLS2_FROM_RECORD(si), SOUND_ITEM_PUT_DOWN, 0xFE, glbPlayerPosX, glbPlayerPosY, 1, 0, 0);
	}
	//^2C1D:079E
	return si;
}

//^2C1D:06B5
void SkWinCore::TAKE_OBJECT(ObjectID rl, U16 xx)
{
	// CSBwinSimilarity: TAG0156c4,TakeObject

	//^2C1D:06B5
	ENTER(0);
	//^2C1D:06B9
	ObjectID si = rl;
	//^2C1D:06BC
	if (si != OBJECT_NULL) {
		//^2C1D:06C1
		si.ClearDir();
		//^2C1D:06C8
		glbLeaderItemFlags = QUERY_GDAT_DBSPEC_WORD_VALUE(si, 0);
		//^2C1D:06D5
		glbLeaderItemWeight = QUERY_ITEM_WEIGHT(si);
		glbLeaderHandPossession.object = si;
		//^2C1D:06E3
		DRAW_ITEM_IN_HAND(&glbLeaderHandPossession);
		//^2C1D:06EE
		FIRE_HIDE_MOUSE_CURSOR();
		//^2C1D:06F3
		DISPLAY_TAKEN_ITEM_NAME(si);
		//^2C1D:06FA
		if (xx != 0) {
			//^2C1D:0700
			glbShowMousePointer = 1;
		}
		else {
			//^2C1D:0708
			_443c_0434();
		}
		//^2C1D:070D
		FIRE_SHOW_MOUSE_CURSOR();
		//^2C1D:0712
		PROCESS_ITEM_BONUS(glbChampionLeader, si, -1, 1);
		//^2C1D:0722
		PLACE_OR_REMOVE_OBJECT_IN_ROOM(glbPlayerPosX, glbPlayerPosY, -1, 1, FCT_PLACE_ON, 0); // NOTE: it is PLACE_ON or REMOVE_OFF ??? item is taken off its place!

// SPX: that part is for giving item sound when taking it
		if (SkCodeParam::bUseExtendedSound)
			QUEUE_NOISE_GEN2(QUERY_CLS1_FROM_RECORD(si), QUERY_CLS2_FROM_RECORD(si), SOUND_ITEM_TAKE, 0xFE, glbPlayerPosX, glbPlayerPosY, 1, 0, 0);
	}
	//^2C1D:073A
	return;
}


//^2C1D:0889
// SPX: _2c1d_0889 renamed CLICK_ITEM_SLOT
void SkWinCore::CLICK_ITEM_SLOT(Bit16u xx)
{
	//^2C1D:0889
	ENTER(4);
	//^2C1D:088F
	if (glbChampionLeader == -1)
		//^2C1D:0896
		return;
	//^2C1D:0899
	U16 si;
	U16 di;
	// SPX: xx < 8 => item slot for champions hand (2*4)
	if (xx < 8) {
		//^2C1D:089F
		if (glbNextChampionNumber != 0)
			//^2C1D:08A6
			return;
		//^2C1D:08A9
		di = xx >> 1;
		//^2C1D:08B0
		if (di >= glbChampionsCount)
			//^2C1D:08B6
			return;
		//^2C1D:08B9
		if (di +1 == glbChampionInventory)
			//^2C1D:08C0
			return;
		//^2C1D:08C3
		if (glbChampionSquad[di].curHP() == 0)
			//^2C1D:08D3
			return;
		//^2C1D:08D6
		si = xx & 1;
	}
	else {
		//^2C1D:08E0
		// SPX: translate to have the inventory position
		si = xx -8;
		//^2C1D:08E8
		di = ((si >= INVENTORY_MAX_SLOT) ? glbChampionIndex : glbChampionInventory) -1;
	}
	//^2C1D:08F8
	ObjectID bp02 = glbLeaderHandPossession.object;
	//^2C1D:08FE
	ObjectID bp04 = (si >= INVENTORY_MAX_SLOT) ? glbCurrentContainerItems[si - INVENTORY_MAX_SLOT] : glbChampionSquad[di].Possess(si);
	//^2C1D:0926
	if (bp04 == OBJECT_NULL && bp02 == OBJECT_NULL)
		//^2C1D:0932
		return;
	//^2C1D:0935
	if (bp02 != OBJECT_NULL) {
		//^2C1D:093B
		if (IS_ITEM_FIT_FOR_EQUIP(bp02, si, 0) == 0)
			//^2C1D:094D
			return;
	}
	//^2C1D:0950
	FIRE_HIDE_MOUSE_CURSOR();
	//^2C1D:0955
	if (bp02 != OBJECT_NULL)
		//^2C1D:095B
		REMOVE_OBJECT_FROM_HAND();
	//^2C1D:095F
	if (bp04 != OBJECT_NULL) {
		//^2C1D:0965
		REMOVE_POSSESSION(di, si);
		//^2C1D:096D
		TAKE_OBJECT(bp04, 1);
	}
	//^2C1D:0978
	if (bp02 != OBJECT_NULL) {
		//^2C1D:097E
		EQUIP_ITEM_TO_INVENTORY(di, bp02, si);
	}
	//^2C1D:098A
	_2e62_0cfa(0);
	//^2C1D:0992
	//if (si == 11 || si == 6 || si == 12 || ((si < 7 || si > 9) && si >= 30)) {
	if (si == INVENTORY_POUCH_1 || si == INVENTORY_POUCH_2 || si == INVENTORY_SCABBARD_1 
		|| ((si < INVENTORY_SCABBARD_2 || si > INVENTORY_SCABBARD_4) && si >= INVENTORY_MAX_SLOT)) {
		//^2C1D:09B0
		glbSomeChampionPanelFlag = 1;
		//^2C1D:09B6
		UPDATE_RIGHT_PANEL(0);
	}
	//^2C1D:09BE
	if (glbShowMousePointer != 0) {
		//^2C1D:09C5
		glbShowMousePointer = 0;
		//^2C1D:09CB
		_443c_0434();
	}
	//^2C1D:09D0
	FIRE_SHOW_MOUSE_CURSOR();
	//^2C1D:09D5
	return;
}



//^2759:28DB
void SkWinCore::CLICK_MONEYBOX(Bit16u xx) 
{
	// you clicked the money box.
	// 1) put in the hodling coin
	// 2) take out the hold coin

	//^2759:28DB
	ENTER(2);
	//^2759:28E1
	if (glbRightPanelType != RIGHT_PANEL_MONEY_BOX)	// 4
		return;
	//^2759:28E8
	X16 bp02 = glbChampionIndex -1;
	ObjectID si = glbChampionSquad[bp02].Possess(glbSelectedHandAction);
	ObjectID di;
	if (glbLeaderHandPossession.object != OBJECT_NULL) {
		//^2759:2909
		if (ADD_COIN_TO_WALLET(si, glbLeaderHandPossession.object) != 0) {
			REMOVE_OBJECT_FROM_HAND();
			goto _2945;
		}
	}
	else {
		//^2759:2920
		di = TAKE_COIN_FROM_WALLET(si, GET_ITEM_ORDER_IN_CONTAINER(si, xx));
		if (di != OBJECT_NULL) {
			//^2759:293B
			TAKE_OBJECT(di, 0);
			//^2759:2945
_2945:
			glbSomeChampionPanelFlag = 1;
			UPDATE_RIGHT_PANEL(0);
		}
	}
	//^2759:2952
	CALC_PLAYER_WEIGHT(bp02);
	//^2759:295B
	return;
}




//^01B0:087A
void SkWinCore::IBMIO_MOUSE_SET_CAPTURE() //#DS=04BF
{
#if UseAltic
	sysMousePositionCaptured++;
#else
//		//^01B0:087A
//		ENTER(0);
//		//^01B0:087D
//		LOADDS(0x3083);
//		//^01B0:0883
//		__asm pushf
//		__asm cli
//		__asm inc ds:[sysMousePositionCaptured]
//		__asm popf
//		//^01B0:088B
//		return;
#endif
}

//^443C:03F4
void SkWinCore::FIRE_MOUSE_SET_CAPTURE()
{
	//^443C:03F4
	ENTER(0);
	//^443C:03F7
	IBMIO_MOUSE_SET_CAPTURE() INDIRECT_CALL;
	//^443C:03FF
	return;
}



//^24A5:174D
// SPX: _24a5_174d renamed CLICK_INVENTORY_EYE
void SkWinCore::CLICK_INVENTORY_EYE()
{
	//^24A5:174D
	ENTER(0);
	//^24A5:1750
	FIRE_MOUSE_SET_CAPTURE();
	//^24A5:1755
	glbShowItemStats = 1;
	//^24A5:175B
	_443c_08ab(&_4976_5da0, &_4976_5da2, &_4976_5dae.rc4.cy);
	//^24A5:176F
	if ((_4976_5dae.rc4.cy & 2) == 0) {
		//^24A5:1777
		FIRE_MOUSE_RELEASE_CAPTURE();
		//^24A5:177C
		glbShowItemStats = 0;
	}
	else {
		//^24A5:1784
		_1031_098e();
		//^24A5:1789
		FIRE_HIDE_MOUSE_CURSOR();
		//^24A5:178E
		SLEEP_SEVERAL_TIME(8);
	}
	//^24A5:1796
	return;
}


//^121E:0003
// SPX: _121e_0003 renamed CLICK_WALL
void SkWinCore::CLICK_WALL(U16 iClickSide)
{
	//^121E:0003
	ENTER(2);
	//^121E:0009
	U16 iDirDelta = 1;	// bp02
	//^121E:000E
	i16 iPosX = glbPlayerPosX;	// di
	i16 iPosY = glbPlayerPosY;	// si
	//^121E:0016
	switch (iClickSide) {
		case 1: // click on left side
			//^121E:002A
			iPosX += glbXAxisDelta[(glbPlayerDir +3) & 3];
			iPosY += glbYAxisDelta[(glbPlayerDir +3) & 3];
			//^121E:004C
			break;

		case 3:	// click in front of player
			//^121E:004E
			iPosX += glbXAxisDelta[glbPlayerDir];
			iPosY += glbYAxisDelta[glbPlayerDir];
			//^121E:0062
			iDirDelta = 2;
			//^121E:0067
			break;

		case 2: // click on right side
			//^121E:0069
			iPosX += glbXAxisDelta[(glbPlayerDir +1) & 3];
			iPosY += glbYAxisDelta[(glbPlayerDir +1) & 3];
			//^121E:0087
			iDirDelta = 3;

			break;
	}
	//^121E:008C
	if (iPosX >= 0 && iPosX < glbCurrentMapWidth && iPosY >= 0 && iPosY < glbCurrentMapHeight) {
		//^121E:00A0
		MOVE_RECORD_AT_WALL(iPosX, iPosY, (glbPlayerDir + iDirDelta) & 3, OBJECT_NULL, glbLeaderHandPossession.object);
	}
	//^121E:00BA
	return;
}

//^121E:0422
void SkWinCore::CLICK_VWPT(i16 xx, i16 yy)
{
	//^121E:0422
	ENTER(38);
	//^121E:0428
	U16 di = yy;
	//^121E:042B
	if (_4976_4ddc != 0) {
		//^121E:0432
		_4976_4ddc = 0;
		//^121E:0438
		PUSH_PULL_RIGID_BODY(glbTargetTypeMoveObject);
		//^121E:0442
		_29ee_000f();
	}
	//^121E:044A
	xx -= _4976_00e8;
	//^121E:0450
	di -= _4976_00ea;
	//^121E:0454
	U16 bp0c = glbPlayerPosX;
	//^121E:045A
	U16 bp0e = glbPlayerPosY;
	//^121E:0460
	bp0c += glbXAxisDelta[glbPlayerDir];
	//^121E:046D
	bp0e += glbYAxisDelta[glbPlayerDir];
	//^121E:047A
	ObjectID bp0a = glbLeaderHandPossession.object;
	//^121E:0480
	Door *bp04;
	U16 si;
	for (si = 0; si < _4976_4dde; si++) {
		//^121E:0485
		if (PT_IN_RECT(&_4976_4d1a[si].rc0, xx, di) != 0) {
			//^121E:04A4
			switch (_4976_4d1a[si].b11 -1) {
				case 3:
					//^121E:04C5
					if (glbChampionLeader == -1)
						//^121E:04CC
						break;
					//^121E:04CF
					U16 bp16;
					U16 bp18;
					if (_4976_4d1a[si].b10 == 3) {
						//^121E:04DF
						bp16 = bp0c;
						bp18 = bp0e;
					}
					else {
						//^121E:04EA
						bp16 = glbPlayerPosX;
						bp18 = glbPlayerPosY;
					}
					//^121E:04F3
					//^121E:04F6
					bp04 = GET_ADDRESS_OF_TILE_RECORD(U8(bp16), U8(bp18))->castToDoor();
					//^121E:0509
					if (bp04->Button() != 0) {
						//^121E:051D
						if (bp0a == OBJECT_NULL) {
							//^121E:0526
							// SPX: This plays the TICK sound when activating a door button
							QUEUE_NOISE_GEN1(GDAT_CATEGORY_MESSAGES, 0x00, SOUND_STD_ACTIVATION_MESSAGE, 0x8c, 0x80, glbPlayerPosX, glbPlayerPosY, 1);
							//^121E:0545
							Timer bp22;
							INVOKE_MESSAGE(bp16, bp18, 0, 2, glbGameTick +1);
							//^121E:0566
							bp22.SetMap(glbCurrentMapIndex);
							bp22.SetTick(glbGameTick +1);
							//^121E:0589
							bp22.TimerType(ttyDoorButton);
							bp22.actor = TIMER_ACTOR__00;
							//^121E:0591
							bp22.id6(GET_TILE_RECORD_LINK(bp16, bp18));
							//^121E:05A1
							QUEUE_TIMER(&bp22);
							//^121E:05AD
							bp04->w2_b_b(1);
							//^121E:05B5
							_4976_4e5c = 1;
						}
						//^121E:05BB
						continue;
					}
					else {
						//^121E:05BE
						bp16 += glbXAxisDelta[(glbPlayerDir +1) & 3];
						//^121E:05D0
						bp18 += glbYAxisDelta[(glbPlayerDir +1) & 3];
						//^121E:05E2
						MOVE_RECORD_AT_WALL(bp16, bp18, (glbPlayerDir + 3) & 3, -1, glbLeaderHandPossession.object);
						//^121E:0600
						continue;
					}

				case 5:
					//^121E:0603
					U16 bp24;
					if (_32cb_0287_DRAW_W_ORNATE(bp24 = _4976_4d1a[si].b10, xx, di) == 0)
						//^121E:0626
						continue;
					//^121E:0629
					CLICK_WALL(bp24);	// When player puts item on wall
					//^121E:0631
					return;

				case 0:
				case 1:
				case 2:
					//^121E:0634
					if (bp0a == OBJECT_NULL) {
						//^121E:063A
						if (_4976_4d1a[si].w8 == 0xffff)
							//^121E:0648
							continue;
						//^121E:064A
						_121e_013a(xx, di, si);
						//^121E:0653
						//^121E:07B7
						//^121E:07BA
						return;
					}
					//^121E:0656
					if (_4976_4d1a[si].b11 == 3) {
						//^121E:0666
						if (_121e_03ae(xx, di, bp0c, bp0e, 0, 2, 3) == 0)
							//^121E:067F
							continue;
						//^121E:0681
						return;
					}
					//^121E:0684
					if (_4976_4d1a[si].b11 == 1)
						//^121E:0692
						continue;
					//^121E:0694
					U16 bp26;
					for (bp26 = 0; bp26 <= 1; bp26++) {
						//^121E:069B
						if (_121e_03ae(xx, di, glbPlayerPosX, glbPlayerPosY, 0, bp26, 1) != 0)
							//^121E:06B9
							return;
					}

					break;

				case 4:
					break;
			}
		}
		//^121E:06C5
	}
	//^121E:06CF
	si = GET_TILE_VALUE(bp0c, bp0e);
	//^121E:06E0
	U16 bp12 = ((si >> 5) == ttTrickWall && ((si >> 5) & 1) != 0 && ((si >> 5) & 4) == 0) ? 1 : 0;
	//^121E:06FF
	U16 bp14;
	if (bp0a == OBJECT_NULL) {
		//^121E:0708
		if (_4976_4dda == 0 && (_098d_02a2(765, xx, di) == 0 || _098d_02a2(766, xx, di) != 0)) {
			//^121E:0735
			PLAYER_TESTING_WALL(bp12, bp0c, bp0e);
			//^121E:0742
			//^121E:07B7
			//^121E:07BA
			return;
		}
		//^121E:0744
		ObjectID bp10 = GET_CREATURE_AT(bp0c, bp0e);
		//^121E:0754
		if (bp10 == OBJECT_NULL)
			//^121E:0759
			return;
		//^121E:075C
		AIDefinition *bp08 = QUERY_CREATURE_AI_SPEC_FROM_RECORD(bp10);
		//^121E:076E
		if (bp08->IsStaticObject() == 0)
			//^121E:0775
			return;
		//^121E:0778
		if (_32cb_01b6(xx, di, bp0c, bp0e, &bp14) == 0)
			//^121E:0793
			return;
		//^121E:0796
		if (GET_CREATURE_WEIGHT(bp10) != 255) {
			//^121E:07A4
			if (bp14 != 0) {
				//^121E:07AA
				ROTATE_CREATURE(bp10, 0, bp14);
				//^121E:07BA
				return;
			}
			//^121E:07BD
			_4976_4ddc = 1;
			//^121E:07C3
			_4976_4dbe = _4976_4dc0 = xx;
			//^121E:07CD
			_29ee_000f();
			//^121E:07D2
			glbTargetTypeMoveObject = 6;
			//^121E:07D8
			return;
		}
		//^121E:07DB
		// SPX: A default sound like punch.
		QUEUE_NOISE_GEN2(GDAT_CATEGORY_CREATURES, QUERY_CLS2_FROM_RECORD(bp10), SOUND_STD_DEFAULT, 0xfe, bp0c, bp0e, 0, 0x8c, 0x80);
		//^121E:0803
		return;
	}
	//^121E:0806
	for (si = 0; si <= 1; si++) {
		//^121E:080A
		if (_121e_03ae(xx, di, glbPlayerPosX, glbPlayerPosY, 0, si, 1) != 0)
			//^121E:0826
			return;
		//^121E:0829
		if (_098d_02a2(si +760, xx, di) != 0 && _121e_0222(glbPlayerPosX, glbPlayerPosY, si) != 0)
			//^121E:0853
			return;
		//^121E:0856
	}
	//^121E:085C
	if (_4976_4dda == 0 && bp12 == 0)
		//^121E:0869
		return;
    //^121E:086C
	for (si = 2; si <= 3; si++) {
		//^121E:0871
		if (_121e_03ae(xx, di, bp0c, bp0e, 3, si, 1) != 0)
			//^121E:088B
			return;
		//^121E:088E
		if (_098d_02a2(si +760, xx, di) != 0 && _121e_0222(bp0c, bp0e, si) != 0)
			//^121E:08B6
			return;
		//^121E:08B9
	}
	//^121E:08BF
	ObjectID bp10 = GET_CREATURE_AT(bp0c, bp0e);
	//^121E:08CF
	if (bp10 != OBJECT_NULL) {
		//^121E:08D4
		AIDefinition *bp08 = QUERY_CREATURE_AI_SPEC_FROM_RECORD(bp10);
		//^121E:08E3
		if (bp08->IsStaticObject() != 0) {
			//^121E:08ED
			for (si = 0; bp08->w30 > si; si++) {
				//^121E:08F1
				if (_121e_03ae(xx, di, bp0c, bp0e, 3, si +2, 2) != 0)
					//^121E:090D
					return;
				//^121E:090F
				if (_098d_02a2(_0cee_2e09(bp10) +si, xx, di) != 0) {
					//^121E:092B
					if (_121e_0222(bp0c, bp0e, si +6) != 0)
						//^121E:0940
						return;
				}
				//^121E:0942
			}
		}
	}
	//^121E:0951
	_121e_0351(xx, di);
	//^121E:095D
	return;
}

//^1031:16FD
// SPX: _1031_16fd renamed CLICK_MAGICAL_MAP_AT
void SkWinCore::CLICK_MAGICAL_MAP_AT(Bit16u ww, i16 xx, i16 yy)
{
	//^1031:16FD
	ENTER(24);
	//^1031:1703
	i16 di = yy;
	//^1031:1709
	ObjectID bp0c = glbChampionTable[glbChampionIndex].Possess(glbSelectedHandAction);
	//^1031:171F
	Container *bp04 = GET_ADDRESS_OF_RECORD9(bp0c);
	//^1031:172E
	if (ww != UI_EVENTCODE_MAGIC_MAP || bp04->b5_5_7() != 1)
		//^1031:1736
		return;
	//^1031:174E
	SRECT bp18;
	QUERY_EXPANDED_RECT(99, &bp18);
	//^1031:175D
	xx -= bp18.x - _4976_53a0;
	di -= bp18.y - _4976_53a2;
	//^1031:1770
	if (false
		|| xx % (glbMagicMapWidth + glbMagicMapInterlineX) < glbMagicMapInterlineX
		|| di % (glbMagicMapHeight + glbMagicMapInterlineY) < glbMagicMapInterlineY
		|| GET_MISSILE_REF_OF_MINION(bp04->GetContainedObject(), bp0c) == NULL
	) {
		//^1031:17B2
		return;
	}
	//^1031:17B5
	xx = xx / (glbMagicMapWidth + glbMagicMapInterlineX);
	di = di / (glbMagicMapHeight + glbMagicMapInterlineY);
	//^1031:17D5
	i16 bp0e = _4976_5364;
	//^1031:17DC
	i16 bp10 = _4976_5365;
	//^1031:17E3
	CALC_VECTOR_W_DIR(_4976_539f, 3 -di, xx -3, &bp0e, &bp10);
	//^1031:1807
	i16 si = LOCATE_OTHER_LEVEL(_4976_539e, 0, &bp0e, &bp10, NULL);
	//^1031:1826
	if (si < 0)
		//^1031:182A
		return;
	//^1031:182D
	i16 bp0a = glbCurrentMapIndex;
	//^1031:1833
	CHANGE_CURRENT_MAP_TO(si);
	//^1031:183A
	U16 bp06 = GET_TILE_VALUE(bp0e, bp10);
	//^1031:184C
	U16 bp08 = bp06 >> 5;
	//^1031:1854
	if (true
		&& bp08 != ttWall
		&& bp08 != ttMapExit
		&& (bp08 != ttTrickWall || (bp06 & 5) != 0)
	) {
		//^1031:1876
		if (true
			&& bp0e == _4976_536a
			&& bp10 == _4976_536c
			&& si == _4976_5318
		) {
			//^1031:188E
			if (_4976_5326 == 1 && _4976_533c >= 0) {
				//^1031:189C
				bp0e = _4976_5328;
				bp10 = _4976_532a;
				//^1031:18A8
				si = _4976_533c;
			}
			//^1031:18AE
			else if (_4976_533e >= 0 && (_4976_5326 == 1 || _4976_5326 == 3)) {
				//^1031:18C3
				bp0e = _4976_5320;
				bp10 = _4976_5322;
				//^1031:18CC
				si = _4976_533e;
			}
		}
		//^1031:18D3
		SET_DESTINATION_OF_MINION_MAP(bp0c, bp0e, bp10, si);
		//^1031:18E5
		_1c9a_0247(bp04->GetContainedObject());
		//^1031:18F2
		UPDATE_RIGHT_PANEL(0);
	}
	//^1031:18FA
	CHANGE_CURRENT_MAP_TO(bp0a);
	//^1031:1903
	return;
}

//^2759:13F5
// SPX: _2759_13f5 renamed CLICK_MAGICAL_MAP_RUNE
void SkWinCore::CLICK_MAGICAL_MAP_RUNE(Bit16u rune)
{
	//^2759:13F5
	ENTER(4);
	//^2759:13FB
	U16 si = _4976_3e1e[RCJ(4,rune)];
	//^2759:1409
	U16 di = 1 << rune;
	//^2759:1410
	Champion *champion;
	if ((glbMagicalMapFlags & di) != 0) {
		//^2759:1416
		si = -si;
		//^2759:141C
		goto _1445;
	}
	//^2759:141E
	champion = &glbChampionTable[glbChampionIndex];
	//^2759:142F
	if (champion->curMP() >= si) {
		//^2759:1438
		champion->curMP(champion->curMP() -si);
		//^2759:143F
		champion->heroFlag |= CHAMPION_FLAG_0800;	// 0x0800
		//^2759:1445
_1445:
		glbMagicalMapFlags ^= di;
		_4976_531a += si;
	}
	//^2759:144D
	return;
}

//^1031:1951
Bit16u SkWinCore::HANDLE_UI_EVENT(MousePosition *ref)
{
	// CSBwinSimilarity: TAG01a7b2,_HandleMouseEvents

	// process user input event such as mouse click and keyboard pressure.

	//^1031:1951
	ENTER(46);
	//^1031:1957
	Bit16u si = ref->event;
	Bit16u bp04 = ref->XPos;
	Bit16u bp06 = ref->YPos;
	//^1031:196B
	//if (si >= 0x03 && si <= 0x06) {
	if (si >= UI_EVENTCODE_MOVE_FIRST && si <= UI_EVENTCODE_MOVE_LAST) {
		//^1031:1975
		if (_4976_4c00 != 0)
			//^1031:197A
			goto _19b1;
		//^1031:197C
		if (glbPlayerThrowCounter != 0 && ((glbPlayerDir +si -3) & 3) == _4976_4c0c)
			//^1031:197A
			goto _19b1;
	}
	//^1031:1994
	if (glbIsPlayerMoving != 0) {
		//^1031:199B
		//if (si == 0x01 || si == 0x02 || si == 0x03)
		if (si == UI_EVENTCODE_TURN_LEFT || si == UI_EVENTCODE_TURN_RIGHT || si == UI_EVENTCODE_MOVE_FORWARD)
			//^1031:19A8
			goto _19b1;
	}
	//^1031:19AA
	Bit16u di;
	if (_4976_4dfe != 0) {
		//^1031:19B1
_19b1:
		_4976_4e5c = 1;
		//^1031:19B7
        di = 0;
		//^1031:19B9
		//^1031:1E64
		return di;
	}
	//^1031:19BC
	di = 1;
	//^1031:19BF
	_4976_4e5c = 0;
	//^1031:19C5
	i16 bp02;
	if (si == UI_EVENTCODE_RELEASE_MOUSE_BUTTON) {	// 0xE3
		//^1031:19CB
		_1031_050c();
	}
	//^1031:19D2
	//else if (si == 0xe1) {
	else if (si == UI_EVENTCODE_MOVE_OBJECT) {	// 0xE1
		//^1031:19D8
		_1031_04f5();
	}
	//^1031:19DF
	else if (si == UI_EVENTCODE_TURN_RIGHT || si == UI_EVENTCODE_TURN_LEFT) {
		//^1031:19E9
		PERFORM_TURN_SQUAD(si);  // 1="TURN LEFT", 2="TURN RIGHT" at arrow panel
	}
	//^1031:19F2
	else if (si >= UI_EVENTCODE_MOVE_FIRST && si <= UI_EVENTCODE_MOVE_LAST) {
		//^1031:19FC
		PERFORM_MOVE(si);  // 3="MOVE FORWARD", 4="MOVE RIGHT", 5="MOVE BACK", 6="MOVE LEFT"
	}
	//^1031:1A05
	// SPX: Covers all item clickable slots : champions hands, inventory, container. x42-x14 = d46
	//else if (si >= 0x14 && si <= 0x42) {
	else if (si >= UI_EVENTCODE_ITEM_SLOT_FIRST && si <= UI_EVENTCODE_ITEM_SLOT_LAST) {
		//^1031:1A0F
		CLICK_ITEM_SLOT(si - UI_EVENTCODE_ITEM_SLOT_FIRST);
	}
	//^1031:1A1D
	// SPX: When dropping item on stats bar
	//else if (si >= 0xea && si <= 0xed) {
	else if (si >= UI_EVENTCODE_CLICK_STATS_BAR_1 && si <= UI_EVENTCODE_CLICK_STATS_BAR_4) {
		//^1031:1A29
		PUT_ITEM_TO_PLAYER(si - UI_EVENTCODE_CLICK_STATS_BAR_1);
		// SPX: extended mode : clicking on stats bar will give player main stats :)
		if (SkCodeParam::bUseDM2ExtendedMode && glbLeaderHandPossession.object == OBJECT_NULL)
		{
			U8 message[64];
			U8 player = si - UI_EVENTCODE_CLICK_STATS_BAR_1;
			sprintf((char*)message, "HP %d / %d | STA %d / %d | MANA %d / %d\n",
				glbChampionSquad[player].curHP(), glbChampionSquad[player].maxHP(),
				glbChampionSquad[player].curStamina()/10, glbChampionSquad[player].maxStamina()/10,
				glbChampionSquad[player].curMP(), glbChampionSquad[player].maxMP());
			DISPLAY_HINT_TEXT(glbChampionColor[player], message);
		}
	}
	//^1031:1A37
	//else if (si >= 0x7d && si <= 0x81) {
	else if (si >= UI_EVENTCODE_TAKE_CHAMPION_TOP_LEFT && si <= UI_EVENTCODE_UPDATE_CHAMPION_POSITION) {
		//^1031:1A42
		if (si == UI_EVENTCODE_UPDATE_CHAMPION_POSITION) {
			//^1031:1A48
			CHAMPION_SQUAD_RECOMPUTE_POSITION();
		}
		else {
			//^1031:1A4F
			CHANGE_PLAYER_POS(si - UI_EVENTCODE_TAKE_CHAMPION_TOP_LEFT);
		}
		//^1031:1A5B
		_1031_0667();
		//^1031:1A5F
		goto _1ab8;
	}
	//^1031:1A61
	// SPX: One for each moneybox part?
	else if (si >= UI_EVENTCODE_MONEYBOX_1 && si <= UI_EVENTCODE_MONEYBOX_6) {	// (si >= 0xe4 && si <= 0xe9)
		//^1031:1A6D
		CLICK_MONEYBOX(si - UI_EVENTCODE_MONEYBOX_1);	// -0xE4
	}
	//^1031:1A7B
	else if (si == UI_EVENTCODE_CHAMPION_TURN_LEFT || si == UI_EVENTCODE_CHAMPION_TURN_RIGHT) {	// (si == 0x5d || si == 0x5e)
		//^1031:1A85
		bp02 = (si == UI_EVENTCODE_CHAMPION_TURN_LEFT) ? -1 : 1;
		//^1031:1A95
		glbChampionTable[glbChampionIndex].playerDir((glbChampionTable[glbChampionIndex].playerDir() +bp02) & 3);
		//^1031:1AB8
_1ab8:
		UPDATE_RIGHT_PANEL(0);
	}
	//^1031:1AC2
	else if (si >= UI_EVENTCODE_010 && si <= UI_EVENTCODE_013) {	// (si >= 0x10 && si <= 0x13)
		//^1031:1ACC
		SELECT_CHAMPION_LEADER(GET_PLAYER_AT_POSITION((si -UI_EVENTCODE_010 + glbPlayerDir) & 3));
		//^1031:1AE5
		_2e62_0cfa(0);
	}
	//^1031:1AEF
	//else if (si >= 0x07 && si <= 0x0b) {
	else if (si >= UI_EVENTCODE_VIEW_CHAMPION_1 && si <= UI_EVENTCODE_RETURN_VIEWPORT) {
		//^1031:1AF9
		if (glbNextChampionNumber != 0 && si <= UI_EVENTCODE_RETURN_VIEWPORT) {
			//^1031:1B05
			//^1031:1DDE
			_2f3f_04ea_CHAMPION(glbPlayerPosX, glbPlayerPosY, glbPlayerDir, glbPlayerMap, 0xa1);
		}
		else {
			//^1031:1B0B
			Bit16u bp02 = si -UI_EVENTCODE_VIEW_CHAMPION_1;
			//^1031:1B13
			if (bp02 == 4 || bp02 < glbChampionsCount) {
				//^1031:1B21
				if (glbNextChampionNumber == 0) {
					//^1031:1B2B
					INTERFACE_CHAMPION(bp02);
					// SPX TODO: This is switch viewport/champion view?
				}
			}
		}
	}
	//^1031:1B37
	//else if (si == 0x52) {
	else if (si == UI_EVENTCODE_VIEW_LEADER) {
		//^1031:1B3C
		if (glbChampionLeader != 0xffff) {
			//^1031:1B46
			INTERFACE_CHAMPION(glbChampionLeader);
		}
	}
	//^1031:1B53
	else if (si == UI_EVENTCODE_VALIDATE_SPELL) {
		//^1031:1B58
		_4976_4e5c = TRY_CAST_SPELL();
		//^1031:1B5D
		//^1031:1BA9
	}
	//^1031:1B5F
	else if (si == UI_EVENTCODE_REMOVE_RUNE) {
		//^1031:1B64
		REMOVE_RUNE_FROM_TAIL();
	}
	//^1031:1B6C
	else if (si >= UI_EVENTCODE_ADD_RUNE_1 && si <= UI_EVENTCODE_ADD_RUNE_6) {
		//^1031:1B76
		ADD_RUNE_TO_TAIL(si - UI_EVENTCODE_ADD_RUNE_1);
	}
	//^1031:1B84
	//else if (si == 0x70) {
	else if (si == UI_EVENTCODE_HAND_RUNE_QUIT) {
		//^1031:1B89
		PROCEED_COMMAND_SLOT(-1);
	}
	//^1031:1B93
	//else if (si >= 0x71 && si <= 0x73) {
	else if (si >= UI_EVENTCODE_HAND_ACTION_1 && si <= UI_EVENTCODE_HAND_ACTION_3) {
		//^1031:1B9D
		_4976_4e5c = PROCEED_COMMAND_SLOT(si -UI_EVENTCODE_HAND_ACTION_1);
	}
	//^1031:1BAF
	//else if (si >= 0x74 && si <= 0x7b) {
	else if (si >= UI_EVENTCODE_CHAMPION1_ACTION_HAND_RIGHT && si <= UI_EVENTCODE_CHAMPION4_ACTION_HAND_LEFT) {
		//^1031:1BB9
		si -= UI_EVENTCODE_CHAMPION1_ACTION_HAND_RIGHT;
		//^1031:1BBC
		ACTIVATE_ACTION_HAND(si >> 1, ((si & 1) != 0) ? 1 : 0);
	}
	//^1031:1BD6
	//else if (si >= 0x5f && si <= 0x62) {
	else if (si >= UI_EVENTCODE_SPELL_OR_LEADER_1 && si <= UI_EVENTCODE_SPELL_OR_LEADER_4) {
		//^1031:1BE0
		i16 bp02 = GET_PLAYER_AT_POSITION((si -UI_EVENTCODE_SPELL_OR_LEADER_1 + glbPlayerDir) & 3);
		//^1031:1BF6
		if (bp02 >= 0) {
			//^1031:1BFD
			SET_SPELLING_CHAMPION(bp02);
		}
	}
	//^1031:1C09
	//else if (si == 0x46) {
	else if (si == UI_EVENTCODE_MOUTH) {
		//^1031:1C0E
		PLAYER_CONSUME_OBJECT(0xffff, OBJECT_NULL, -1);
	}
	//^1031:1C1B
	//else if (si == 0x47) {
	else if (si == UI_EVENTCODE_EYE) {
		//^1031:1C20
		CLICK_INVENTORY_EYE();
	}
	//^1031:1C28
	//else if (si == 0x50) {
	else if (si == UI_EVENTCODE_CLICK_VIEWPORT) {
		//^1031:1C2D
		CLICK_VWPT(bp04, bp06);
	}
	//^1031:1C3D
	//else if (si == 0x96) {
	else if (si == UI_EVENTCODE_MAGIC_MAP) {
		//^1031:1C43
		CLICK_MAGICAL_MAP_AT(si, bp04, bp06);
	}
	//^1031:1C54
	// SPX: Clicking runes on magical maps (runes = KU, ROS, DAIN, NETA)
	//else if (si >= 0x97 && si <= 0x9a) {
	else if (si >= UI_EVENTCODE_MAGIC_MAP_RUNE_1 && si <= UI_EVENTCODE_MAGIC_MAP_RUNE_4) {
		//^1031:1C60
		CLICK_MAGICAL_MAP_RUNE(si -UI_EVENTCODE_MAGIC_MAP_RUNE_1);
	}
	//^1031:1C6F
	//else if (si == 0x93) {
	else if (si == UI_EVENTCODE_PAUSE) {
		//^1031:1C75
		_4976_4c02 = 0;
		//^1031:1C7B
		_38c8_0002();
		//^1031:1C80
		FILL_ENTIRE_PICT(_4976_4c16, glbPaletteT16[COLOR_BLACK]);
		//^1031:1C9A
		Bit8u bp2e[40];
		// SPX: text = GAME PAUSED
		DRAW_VP_RC_STR(
			6,
			glbPaletteT16[COLOR_CYAN],
			QUERY_GDAT_TEXT(0x01, 0x00, 0x12, bp2e)
			);
		//^1031:1CC4
		CHANGE_VIEWPORT_TO_INVENTORY(0);
		//^1031:1CCC
		_1031_0675(3);
		//^1031:1CD3
		_1031_098e();
	}
	//^1031:1CDA
	//else if (si == 0x94) {
	else if (si == UI_EVENTCODE_END_PAUSE) {
		//^1031:1CE0
		if (_4976_4c02 == 0) {
			//^1031:1CEA
			_4976_4c02 = 1;
			//^1031:1CF0
			_38c8_0060();
			//^1031:1CF5
			_1031_06a5();
			//^1031:1CF9
			_1031_098e();
		}
	}
	//^1031:1D00
	else if (true
		&& glbShowItemStats == 0 
		&& _4976_4bfe == 0 
		&& _4976_4c3e == 0
		&& _4976_4ddc == 0
	) {
		//^1031:1D28
		//if (si == 0x91) {
		if (si == UI_EVENTCODE_SLEEP) {
			//^1031:1D2E
			if (glbNextChampionNumber == 0) {
				//^1031:1D38
				_4976_3d59 = 0;
				//^1031:1D3E
				if (glbChampionInventory != 0) {
					//^1031:1D45
					INTERFACE_CHAMPION(4);
				}
				//^1031:1D4D
				_4976_3d59 = 1;
				//^1031:1D53
				DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
				//^1031:1D58
				_38c8_0002();
				//^1031:1D5D
				glbIsPlayerSleeping = 1;
				//^1031:1D63
				if (glbIsPlayerMoving != 0) {
					//^1031:1D6A
					PERFORM_MOVE(glbPlayerLastMove);
					//^1031:1D74
					glbIsPlayerMoving = 0;
				}
				//^1031:1D7A
				DRAW_WAKE_UP_TEXT();
				//^1031:1D7E
				CHANGE_VIEWPORT_TO_INVENTORY(0);
				//^1031:1D86
				glbTickSpeed = 1;
				//^1031:1D8C
                _1031_0541(2);
				//^1031:1D93
				_1031_098e();
			}
		}
		//^1031:1D9A
		//else if (si == 0x92) {
		else if (si == UI_EVENTCODE_WAKE) {
			//^1031:1DA0
			RESUME_FROM_WAKE();
		}
		//^1031:1DA8
		//else if (si == 0x8c) {
		else if (si == UI_EVENTCODE_DISK_OP) {
			//^1031:1DAE
			if (glbChampionsCount > 0 && glbNextChampionNumber == 0) {
				//^1031:1DC2
				_4976_4e62--;
				//^1031:1DC6
				GAME_SAVE_MENU();
				//^1031:1DCB
                _4976_4e62++;
			}
		}
		//^1031:1DD1
		//else if (si == 0xa0 || si == 0xa1) {
		else if (si == UI_EVENTCODE_REVIVE_CHAMPION || si == UI_EVENTCODE_EXIT_CRYOCELL) {
			//^1031:1DDD
            _2f3f_04ea_CHAMPION(glbPlayerPosX, glbPlayerPosY, glbPlayerDir, glbPlayerMap, si);
		}
		//^1031:1DF8
		//else if (si == 0xd7) {
		else if (si == UI_EVENTCODE_START_NEW_GAME) {
			//^1031:1DFE

			// "NEW" at main menu
			glbSpecialScreen = _MENU_SCREEN__LOAD_NEW_GAME;
		}
		//^1031:1E06
		//else if (si == 0xd8) {
		else if (si == UI_EVENTCODE_0D8) {
			//^1031:1E0C
			glbSpecialScreen = _MENU_SCREEN__LOAD_NEW_GAME;
			_4976_5bea = 1;
		}
		//^1031:1E1A
		//else if (si == 0xd9) {
		else if (si == UI_EVENTCODE_RESUME_GAME) {
			//^1031:1E20
			// "RESUME" at main menu
			glbSpecialScreen = _MENU_SCREEN__RESUME_GAME_SELECT;
		}
		//^1031:1E28
		//else if (si == 0xda) {
		else if (si == UI_EVENTCODE_SHOW_CREDITS) {
			//^1031:1E2E
			SHOW_CREDITS();
		}
		//^1031:1E35
		//else if (si == 0xdf) {
		else if (si == UI_EVENTCODE_0DF) {
			//^1031:1E3B
			_4976_5bf2 = 1;
		}
		//^1031:1E43
		//else if (si == 0xe0) {
		else if (si == UI_EVENTCODE_QUIT_GAME) {
			//^1031:1E49
			SK_PREPARE_EXIT();
		}
	}
	//^1031:1E4E
	//if (glbUIEventCode >= 0xa5 && glbUIEventCode <= 0xc6)
	if (glbUIEventCode >= UI_EVENTCODE_0A5 && glbUIEventCode <= UI_EVENTCODE_0C6)
		//^1031:1E5E
		_4976_4e5c = 1;
	//^1031:1E66
	return di;
}


