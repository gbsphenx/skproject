//------------------------------------------------------------------------------
// SK-UI-EVENT
//------------------------------------------------------------------------------

//#include <StdAfx.h>	// Required for MVC6 compilation

#include <skver.h>

#include <skcore.h>
#include <skdebug.h>
#include <skglobal.h>
#include <skparam.h>

//#if defined(_USE_MFC80) || defined(_USE_MFC60)
//#include <SKMFC.h>
//#include <resource.h>
//#endif // defined(_USE_MFC80) || defined(_USE_MFC60)


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
		i16 di = GET_PLAYER_AT_POSITION(((si >> 1) + cd.pi.glbPlayerDir) & 3);
		//^1031:15AD
		if (false
			|| di < 0 
			|| glbChampionSquad[di + bp06].handCooldown[0] != 0 
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
		i16 di = GET_PLAYER_AT_POSITION((si -0x5f + cd.pi.glbPlayerDir) & 3);
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
		di = ((si -UI_EVENTCODE_CHAMPION1_ACTION_HAND_RIGHT) & 1) * 5 + 0x5c;
		goto _0f23;
	}
	//^1031:0E21
	//else if (si >= 0x5f && si <= 0x62) {
	else if (si >= UI_EVENTCODE_SPELL_OR_LEADER_1 && si <= UI_EVENTCODE_SPELL_OR_LEADER_4) {
		//^1031:0E2B
		di = (si -UI_EVENTCODE_SPELL_OR_LEADER_1) * 5 + 0x66;
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
		di = ((si - UI_EVENTCODE_CHAMPION_TURN_LEFT) << 3) + 0x82;
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
	return &tlbUIEventTable[RCJ(236,di)];
}



//^2C1D:073D
ObjectID SkWinCore::REMOVE_OBJECT_FROM_HAND()
{
	// CSBwinSimilarity: TAG01576a,RemoveObjectFromHand

	//^2C1D:073D
	ENTER(0);
	//^2C1D:0741
	ObjectID si = cd.pi.glbLeaderHandPossession.object;
	//^2C1D:0746
	if (si != OBJECT_NULL) {
		//^2C1D:074B
		glbLeaderItemFlags = 0;
		glbLeaderItemWeight = 0;
		cd.pi.glbLeaderHandPossession.object = OBJECT_NULL;
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
			QUEUE_NOISE_GEN2(QUERY_CLS1_FROM_RECORD(si), QUERY_CLS2_FROM_RECORD(si), SOUND_ITEM_PUT_DOWN, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1, 0, 0);
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
		cd.pi.glbLeaderHandPossession.object = si;
		//^2C1D:06E3
		DRAW_ITEM_IN_HAND(&cd.pi.glbLeaderHandPossession);
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
		PLACE_OR_REMOVE_OBJECT_IN_ROOM(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, -1, 1, FCT_PLACE_ON, 0); // NOTE: it is PLACE_ON or REMOVE_OFF ??? item is taken off its place!

// SPX: that part is for giving item sound when taking it
		if (SkCodeParam::bUseExtendedSound)
			QUEUE_NOISE_GEN2(QUERY_CLS1_FROM_RECORD(si), QUERY_CLS2_FROM_RECORD(si), SOUND_ITEM_TAKE, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1, 0, 0);
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
		if (cd.pi.glbNextChampionNumber != 0)
			//^2C1D:08A6
			return;
		//^2C1D:08A9
		di = xx >> 1;
		//^2C1D:08B0
		if (di >= cd.pi.glbChampionsCount)
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
		di = ((si >= INVENTORY_MAX_SLOT) ? cd.pi.glbChampionIndex : glbChampionInventory) -1;
	}
	//^2C1D:08F8
	ObjectID bp02 = cd.pi.glbLeaderHandPossession.object;
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
	X16 bp02 = cd.pi.glbChampionIndex -1;
	ObjectID si = glbChampionSquad[bp02].Possess(glbSelectedHandAction);
	ObjectID di;
	if (cd.pi.glbLeaderHandPossession.object != OBJECT_NULL) {
		//^2759:2909
		if (ADD_COIN_TO_WALLET(si, cd.pi.glbLeaderHandPossession.object) != 0) {
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
	ENTER(0);
	IBMIO_MOUSE_SET_CAPTURE() INDIRECT_CALL;
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
	MOUSE_STATE_443c_08ab(&_4976_5da0, &_4976_5da2, &_4976_5dae.rc4.cy);
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
	i16 iPosX = cd.pi.glbPlayerPosX;	// di
	i16 iPosY = cd.pi.glbPlayerPosY;	// si
	//^121E:0016
	switch (iClickSide) {
		case 1: // click on left side
			//^121E:002A
			iPosX += glbXAxisDelta[(cd.pi.glbPlayerDir +3) & 3];
			iPosY += glbYAxisDelta[(cd.pi.glbPlayerDir +3) & 3];
			//^121E:004C
			break;

		case 3:	// click in front of player
			//^121E:004E
			iPosX += glbXAxisDelta[cd.pi.glbPlayerDir];
			iPosY += glbYAxisDelta[cd.pi.glbPlayerDir];
			//^121E:0062
			iDirDelta = 2;
			//^121E:0067
			break;

		case 2: // click on right side
			//^121E:0069
			iPosX += glbXAxisDelta[(cd.pi.glbPlayerDir +1) & 3];
			iPosY += glbYAxisDelta[(cd.pi.glbPlayerDir +1) & 3];
			//^121E:0087
			iDirDelta = 3;

			break;
	}
	//^121E:008C
	if (iPosX >= 0 && iPosX < glbCurrentMapWidth && iPosY >= 0 && iPosY < glbCurrentMapHeight) {
		//^121E:00A0
		MOVE_RECORD_AT_WALL(iPosX, iPosY, (cd.pi.glbPlayerDir + iDirDelta) & 3, OBJECT_NULL, cd.pi.glbLeaderHandPossession.object);
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
	if (glbTryPushPullObject != 0) {
		glbTryPushPullObject = 0;
		PUSH_PULL_RIGID_BODY(glbTargetTypeMoveObject);
		DRAW_ARROW_PANEL();
	}
	//^121E:044A
	xx -= _4976_00e8;
	//^121E:0450
	di -= _4976_00ea;
	//^121E:0454
	U16 bp0c = cd.pi.glbPlayerPosX;
	//^121E:045A
	U16 bp0e = cd.pi.glbPlayerPosY;
	//^121E:0460
	bp0c += glbXAxisDelta[cd.pi.glbPlayerDir];
	//^121E:046D
	bp0e += glbYAxisDelta[cd.pi.glbPlayerDir];
	//^121E:047A
	ObjectID rlHandPossession = cd.pi.glbLeaderHandPossession.object;	// bp0a
	//^121E:0480
	Door* xDoor;	// bp04
	U16 si;
	for (si = 0; si < glbViewportClickableRectCount; si++) {
		//^121E:0485
		if (PT_IN_RECT(&tblViewportClickableRectangles[si].rc0, xx, di) != 0) {
			//^121E:04A4
			switch (tblViewportClickableRectangles[si].b11 -1) {
				case 3:
					//^121E:04C5
					if (glbChampionLeader == -1)
						//^121E:04CC
						break;
					//^121E:04CF
					U16 bp16;
					U16 bp18;
					if (tblViewportClickableRectangles[si].b10 == 3) {
						//^121E:04DF
						bp16 = bp0c;
						bp18 = bp0e;
					}
					else {
						//^121E:04EA
						bp16 = cd.pi.glbPlayerPosX;
						bp18 = cd.pi.glbPlayerPosY;
					}
					//^121E:04F3
					//^121E:04F6
					xDoor = GET_ADDRESS_OF_TILE_RECORD(U8(bp16), U8(bp18))->castToDoor();
					//^121E:0509
					if (xDoor->Button() != 0) {
						//^121E:051D
						if (rlHandPossession == OBJECT_NULL) {
							//^121E:0526
							// SPX: This plays the TICK sound when activating a door button
							QUEUE_NOISE_GEN1(GDAT_CATEGORY_MESSAGES, 0x00, SOUND_STD_ACTIVATION_MESSAGE, 0x8C, 0x80, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1);
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
							xDoor->w2_b_b(1);
							//^121E:05B5
							cd.gg.glbRefreshViewport = 1;
						}
#ifdef XDMX_EXTENDED_FEATURES
						HANDLE_KEY_ON_DOOR_BUTTON(xDoor, rlHandPossession, bp16, bp18);
#endif // XDMX_EXTENDED_FEATURES
						//^121E:05BB
						continue;
					}
					else {
						//^121E:05BE
						bp16 += glbXAxisDelta[(cd.pi.glbPlayerDir +1) & 3];
						//^121E:05D0
						bp18 += glbYAxisDelta[(cd.pi.glbPlayerDir +1) & 3];
						//^121E:05E2
						MOVE_RECORD_AT_WALL(bp16, bp18, (cd.pi.glbPlayerDir + 3) & 3, -1, cd.pi.glbLeaderHandPossession.object);
						//^121E:0600
						continue;
					}

				case 5:
					//^121E:0603
					U16 bp24;
					if (_32cb_0287_DRAW_W_ORNATE(bp24 = tblViewportClickableRectangles[si].b10, xx, di) == 0)
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
					if (rlHandPossession == OBJECT_NULL) {
						//^121E:063A
						if (tblViewportClickableRectangles[si].w8 == 0xffff)
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
					if (tblViewportClickableRectangles[si].b11 == 3) {
						//^121E:0666
						if (_121e_03ae(xx, di, bp0c, bp0e, 0, 2, 3) == 0)
							//^121E:067F
							continue;
						//^121E:0681
						return;
					}
					//^121E:0684
					if (tblViewportClickableRectangles[si].b11 == 1)
						//^121E:0692
						continue;
					//^121E:0694
					U16 bp26;
					for (bp26 = 0; bp26 <= 1; bp26++) {
						//^121E:069B
						if (_121e_03ae(xx, di, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 0, bp26, 1) != 0)
							//^121E:06B9
							return;
					}

					break;

				case 4:
					break;
#if defined (XDMX_EXTENDED_FEATURES)
				case 6: // SPX: new added case for special interwall door
					{
						int iSwitchResult = 0;
						printf("Interwall door clicked!\n");
						// Then we would switch the door open <-> closed there
						iSwitchResult = TRY_SWITCH_INTERWALL_DOOR_STATUS(cd.pi.glbPlayerMap, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, cd.pi.glbPlayerDir);
						if (iSwitchResult != 0) {
							int iKeyFound = 1;
							// Check and take common key if available
							iKeyFound = CONSUME_ANY_FROM_HAND(DB_CATEGORY_MISC_ITEM, 1);
							if (iKeyFound) {
								UNLOCK_INTERWALL_DOOR(cd.pi.glbPlayerMap, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, cd.pi.glbPlayerDir);
								iSwitchResult = TRY_SWITCH_INTERWALL_DOOR_STATUS(cd.pi.glbPlayerMap, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, cd.pi.glbPlayerDir);
								DISPLAY_HINT_TEXT(COLOR_LIGHT_GREEN, (const U8*) "DOOR UNLOCKED.\n");
							}
							else	// If no key, tells it's locked
								DISPLAY_HINT_TEXT(COLOR_LIGHT_GREEN, (const U8*) "THE DOOR IS LOCKED\n");
						}
						if (iSwitchResult == 0)
							QUEUE_NOISE_GEN1(GDAT_CATEGORY_MESSAGES, 0x00, SOUND_STD_INTERWALL_DOOR, 0x8C, 0x80, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1);

						break;
					}
#endif // XDMX_EXTENDED_FEATURES
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
	if (rlHandPossession == OBJECT_NULL) {
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
			glbTryPushPullObject = 1;
			//^121E:07C3
			_4976_4dbe = _4976_4dc0 = xx;
			//^121E:07CD
			DRAW_ARROW_PANEL();
			//^121E:07D2
			glbTargetTypeMoveObject = 6;
			//^121E:07D8
			return;
		}
		//^121E:07DB
		// SPX: A default sound like punch.
		QUEUE_NOISE_GEN2(GDAT_CATEGORY_CREATURES, QUERY_CLS2_FROM_RECORD(bp10), SOUND_STD_DEFAULT, 0xFE, bp0c, bp0e, 0, 0x8C, 0x80);
		//^121E:0803
		return;
	}
	//^121E:0806
	for (si = 0; si <= 1; si++) {
		//^121E:080A
		if (_121e_03ae(xx, di, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 0, si, 1) != 0)
			//^121E:0826
			return;
		//^121E:0829
		if (_098d_02a2(si +760, xx, di) != 0 && CREATURE_121e_0222(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, si) != 0)
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
		if (_098d_02a2(si +760, xx, di) != 0 && CREATURE_121e_0222(bp0c, bp0e, si) != 0)
			//^121E:08B6
			return;
		//^121E:08B9
	}
	//^121E:08BF
	ObjectID bp10 = GET_CREATURE_AT(bp0c, bp0e);
	//^121E:08CF
	if (bp10 != OBJECT_NULL) {
		//^121E:08D4
		AIDefinition* xAIDef = QUERY_CREATURE_AI_SPEC_FROM_RECORD(bp10);	// bp08
		//^121E:08E3
		if (xAIDef->IsStaticObject() != 0) {
			//^121E:08ED
			for (si = 0; xAIDef->wc30 > si; si++) {
				//^121E:08F1
				if (_121e_03ae(xx, di, bp0c, bp0e, 3, si +2, 2) != 0)
					//^121E:090D
					return;
				//^121E:090F
				if (_098d_02a2(QUERY_CREATURE_AI_W32_FROM_RECORD(bp10) +si, xx, di) != 0) {
					//^121E:092B
					if (CREATURE_121e_0222(bp0c, bp0e, si +6) != 0)
						//^121E:0940
						return;
				}
				//^121E:0942
			}
		}
	}
	//^121E:0951
	_121e_0351_THROW_LEFT_OR_RIGHT(xx, di);
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
	ObjectID bp0c = glbChampionTable[cd.pi.glbChampionIndex].Possess(glbSelectedHandAction);
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
		_1c9a_0247_FREE_CACHE_FROM_CREATURE(bp04->GetContainedObject());
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
	champion = &glbChampionTable[cd.pi.glbChampionIndex];
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
U16 SkWinCore::HANDLE_UI_EVENT(MousePosition *ref)
{
	// CSBwinSimilarity: TAG01a7b2,_HandleMouseEvents
	// process user input event such as mouse click and keyboard pressure.

	ENTER(46);
	U16 si = ref->event;
	U16 bp04 = ref->XPos;
	U16 bp06 = ref->YPos;
	if (si >= UI_EVENTCODE_MOVE_FIRST && si <= UI_EVENTCODE_MOVE_LAST) { // >= 0x03 and <= 0x06
		if (_4976_4c00 != 0)
			goto _19b1;
		if (glbPlayerThrowCounter != 0 && ((cd.pi.glbPlayerDir + si -3) & 3) == _4976_4c0c)
			goto _19b1;
	}
	if (cd.pi.glbIsPlayerMoving != 0) {
		if (si == UI_EVENTCODE_TURN_LEFT || si == UI_EVENTCODE_TURN_RIGHT || si == UI_EVENTCODE_MOVE_FORWARD)	// 0x01 or 0x02 or 0x03
			goto _19b1;
	}
	U16 di;
	if (glbEndCounter != 0) {
_19b1:
		cd.gg.glbRefreshViewport = 1;
        di = 0;
		return di;
	}
	di = 1;
	cd.gg.glbRefreshViewport = 0;
	i16 bp02;
	if (si == UI_EVENTCODE_RELEASE_MOUSE_BUTTON) {	// 0xE3
		_1031_050c();
	}
	else if (si == UI_EVENTCODE_MOVE_OBJECT) {	// 0xE1
		CLEAR_TRY_PUSH_PULL_OBJECT();
	}
	else if (si == UI_EVENTCODE_TURN_RIGHT || si == UI_EVENTCODE_TURN_LEFT) { // 0x02 or 0x01
		PERFORM_TURN_SQUAD(si);  // 1="TURN LEFT", 2="TURN RIGHT" at arrow panel
	}
	else if (si >= UI_EVENTCODE_MOVE_FIRST && si <= UI_EVENTCODE_MOVE_LAST) { // >= 0x03 and <= 0x06
		PERFORM_MOVE(si);  // 3="MOVE FORWARD", 4="MOVE RIGHT", 5="MOVE BACK", 6="MOVE LEFT"
	}
	// SPX: Covers all item clickable slots : champions hands, inventory, container. x42-x14 = d46
	else if (si >= UI_EVENTCODE_ITEM_SLOT_FIRST && si <= UI_EVENTCODE_ITEM_SLOT_LAST) {	// (si >= 0x14 && si <= 0x42)
		CLICK_ITEM_SLOT(si - UI_EVENTCODE_ITEM_SLOT_FIRST);
	}
	// SPX: When dropping item on stats bar
	else if (si >= UI_EVENTCODE_CLICK_STATS_BAR_1 && si <= UI_EVENTCODE_CLICK_STATS_BAR_4) { // (si >= 0xEA && si <= 0xED)
		PUT_ITEM_TO_PLAYER(si - UI_EVENTCODE_CLICK_STATS_BAR_1);
		// SPX: extended mode : clicking on stats bar will give player main stats :)
		if (SkCodeParam::bUseDM2ExtendedMode && cd.pi.glbLeaderHandPossession.object == OBJECT_NULL)
		{
			U8 message[64];
			U8 player = si - UI_EVENTCODE_CLICK_STATS_BAR_1;
			sprintf((char*)message, "%s: HP %d / %d |STA %d / %d |MANA %d / %d\n",
				glbChampionSquad[player].firstName,
				glbChampionSquad[player].curHP(), glbChampionSquad[player].maxHP(),
				glbChampionSquad[player].curStamina()/10, glbChampionSquad[player].maxStamina()/10,
				glbChampionSquad[player].curMP(), glbChampionSquad[player].maxMP());
			DISPLAY_HINT_TEXT(glbChampionColor[player], message);
		}
	}
	else if (si >= UI_EVENTCODE_TAKE_CHAMPION_TOP_LEFT && si <= UI_EVENTCODE_UPDATE_CHAMPION_POSITION) { // (si >= 0x7D && si <= 0x81)
		if (si == UI_EVENTCODE_UPDATE_CHAMPION_POSITION) {
			CHAMPION_SQUAD_RECOMPUTE_POSITION();
		}
		else {
			CHANGE_PLAYER_POS(si - UI_EVENTCODE_TAKE_CHAMPION_TOP_LEFT);
		}
		_1031_0667();
		goto _1ab8;
	}
	// SPX: One for each moneybox part?
	else if (si >= UI_EVENTCODE_MONEYBOX_1 && si <= UI_EVENTCODE_MONEYBOX_6) {	// (si >= 0xe4 && si <= 0xe9)
		CLICK_MONEYBOX(si - UI_EVENTCODE_MONEYBOX_1);	// -0xE4
	}
	else if (si == UI_EVENTCODE_CHAMPION_TURN_LEFT || si == UI_EVENTCODE_CHAMPION_TURN_RIGHT) {	// (si == 0x5d || si == 0x5e)
		bp02 = (si == UI_EVENTCODE_CHAMPION_TURN_LEFT) ? -1 : 1;
		glbChampionTable[cd.pi.glbChampionIndex].playerDir((glbChampionTable[cd.pi.glbChampionIndex].playerDir() +bp02) & 3);
_1ab8:
		UPDATE_RIGHT_PANEL(0);
	}
	else if (si >= UI_EVENTCODE_010 && si <= UI_EVENTCODE_013) {	// (si >= 0x10 && si <= 0x13)
		SELECT_CHAMPION_LEADER(GET_PLAYER_AT_POSITION((si -UI_EVENTCODE_010 + cd.pi.glbPlayerDir) & 3));
		_2e62_0cfa(0);
	}
	else if (si >= UI_EVENTCODE_VIEW_CHAMPION_1 && si <= UI_EVENTCODE_RETURN_VIEWPORT) { // >= 0x07 and <= 0x0B
		if (cd.pi.glbNextChampionNumber != 0 && si <= UI_EVENTCODE_RETURN_VIEWPORT) {
			REVIVE_CHAMPION(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, cd.pi.glbPlayerDir, cd.pi.glbPlayerMap, 0xa1);
		}
		else {
			U16 bp02 = si - UI_EVENTCODE_VIEW_CHAMPION_1;
			if (bp02 == 4 || bp02 < cd.pi.glbChampionsCount) {
				if (cd.pi.glbNextChampionNumber == 0) {
					INTERFACE_CHAMPION(bp02);
					// SPX TODO: This is switch viewport/champion view?
				}
			}
		}
	}
	else if (si == UI_EVENTCODE_VIEW_LEADER) {	// 0x52
		if (glbChampionLeader != 0xffff) {
			INTERFACE_CHAMPION(glbChampionLeader);
		}
	}
	else if (si == UI_EVENTCODE_VALIDATE_SPELL) {	// 0x6C
		cd.gg.glbRefreshViewport = TRY_CAST_SPELL();
	}
	else if (si == UI_EVENTCODE_REMOVE_RUNE) {	// 0x6B
		REMOVE_RUNE_FROM_TAIL();
	}
	else if (si >= UI_EVENTCODE_ADD_RUNE_1 && si <= UI_EVENTCODE_ADD_RUNE_6) { // >= 0x65 and <= 0x6A
		ADD_RUNE_TO_TAIL(si - UI_EVENTCODE_ADD_RUNE_1);
	}
	else if (si == UI_EVENTCODE_HAND_RUNE_QUIT) { // 0x70
		PROCEED_COMMAND_SLOT(-1);
	}
	else if (si >= UI_EVENTCODE_HAND_ACTION_1 && si <= UI_EVENTCODE_HAND_ACTION_3) {	// >= 0x71 and <= 0x73
		cd.gg.glbRefreshViewport = PROCEED_COMMAND_SLOT(si -UI_EVENTCODE_HAND_ACTION_1);
	}
	else if (si >= UI_EVENTCODE_CHAMPION1_ACTION_HAND_RIGHT && si <= UI_EVENTCODE_CHAMPION4_ACTION_HAND_LEFT) {	// >= 0x74 and <= 0x7B
		si -= UI_EVENTCODE_CHAMPION1_ACTION_HAND_RIGHT;
		ACTIVATE_ACTION_HAND(si >> 1, ((si & 1) != 0) ? 1 : 0);
	}
	else if (si >= UI_EVENTCODE_SPELL_OR_LEADER_1 && si <= UI_EVENTCODE_SPELL_OR_LEADER_4) {	// >= 0x5F and <= 0x62
		i16 iChampionIndex = GET_PLAYER_AT_POSITION((si -UI_EVENTCODE_SPELL_OR_LEADER_1 + cd.pi.glbPlayerDir) & 3); // bp02
		if (iChampionIndex >= 0) {
			SET_SPELLING_CHAMPION(iChampionIndex);
		}
	}
	else if (si == UI_EVENTCODE_MOUTH) {	// 0x46
		PLAYER_CONSUME_OBJECT(0xFFFF, OBJECT_NULL, -1);
	}
	else if (si == UI_EVENTCODE_EYE) {	// 0x47
		CLICK_INVENTORY_EYE();
	}
	else if (si == UI_EVENTCODE_CLICK_VIEWPORT) {	// 0x50
		CLICK_VWPT(bp04, bp06);
	}
	else if (si == UI_EVENTCODE_MAGIC_MAP) {	// 0x96
		CLICK_MAGICAL_MAP_AT(si, bp04, bp06);
	}
	// SPX: Clicking runes on magical maps (runes = KU, ROS, DAIN, NETA)
	else if (si >= UI_EVENTCODE_MAGIC_MAP_RUNE_1 && si <= UI_EVENTCODE_MAGIC_MAP_RUNE_4) {	// >= 0x97 and <= 0x9A
		//^1031:1C60
		CLICK_MAGICAL_MAP_RUNE(si -UI_EVENTCODE_MAGIC_MAP_RUNE_1);
	}
	else if (si == UI_EVENTCODE_PAUSE) {	// 0x93
		_4976_4c02 = 0;
		_38c8_0002();
		FILL_ENTIRE_PICT(_4976_4c16, glbPaletteT16[COLOR_BLACK]);
		Bit8u bp2e[40];
		// SPX: text = GAME PAUSED
		DRAW_VP_RC_STR(
			6,
			glbPaletteT16[COLOR_CYAN],
			QUERY_GDAT_TEXT(0x01, 0x00, 0x12, bp2e)
			);
		CHANGE_VIEWPORT_TO_INVENTORY(0);
		_1031_0675(3);
		_1031_098e();
	}
	else if (si == UI_EVENTCODE_END_PAUSE) {	// 0x94
		if (_4976_4c02 == 0) {
			_4976_4c02 = 1;
			_38c8_0060();
			_1031_06a5();
			_1031_098e();
		}
	}
	//^1031:1D00
	else if (true
		&& glbShowItemStats == 0 
		&& _4976_4bfe == 0 
		&& _4976_4c3e == 0
		&& glbTryPushPullObject == 0
	) {
		if (si == UI_EVENTCODE_SLEEP) {	// 0x91
			if (cd.pi.glbNextChampionNumber == 0) {
				_4976_3d59 = 0;
				if (glbChampionInventory != 0) {
					INTERFACE_CHAMPION(4);
				}
				_4976_3d59 = 1;
				DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
				_38c8_0002();
				cd.pi.glbIsPlayerSleeping = 1;
				if (cd.pi.glbIsPlayerMoving != 0) {
					PERFORM_MOVE(cd.pi.glbPlayerLastMove);
					cd.pi.glbIsPlayerMoving = 0;
				}
				DRAW_WAKE_UP_TEXT();
				CHANGE_VIEWPORT_TO_INVENTORY(0);
				glbTickSpeed = 1;
                _1031_0541(2);
				_1031_098e();
			}
		}
		else if (si == UI_EVENTCODE_WAKE) {	// 0x92
			RESUME_FROM_WAKE();
		}
		else if (si == UI_EVENTCODE_DISK_OP) {	// 0x8C
			if (cd.pi.glbChampionsCount > 0 && cd.pi.glbNextChampionNumber == 0) {
				_4976_4e62--;
				GAME_SAVE_MENU();
                _4976_4e62++;
			}
		}
		else if (si == UI_EVENTCODE_REVIVE_CHAMPION || si == UI_EVENTCODE_EXIT_CRYOCELL) { // 0xA0 or 0xA1
            REVIVE_CHAMPION(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, cd.pi.glbPlayerDir, cd.pi.glbPlayerMap, si);
		}
		else if (si == UI_EVENTCODE_START_NEW_GAME) {	// 0xD7
			// "NEW" at main menu
			cd.mo.glbSpecialScreen = _MENU_SCREEN__LOAD_NEW_GAME;
		}
		else if (si == UI_EVENTCODE_0D8) {	// 0xD8
			cd.mo.glbSpecialScreen = _MENU_SCREEN__LOAD_NEW_GAME;
			_4976_5bea = 1;
		}
		else if (si == UI_EVENTCODE_RESUME_GAME) { // 0xD9
			// "RESUME" at main menu
			cd.mo.glbSpecialScreen = _MENU_SCREEN__RESUME_GAME_SELECT;
		}
		else if (si == UI_EVENTCODE_SHOW_CREDITS) { // 0xDA
			SHOW_CREDITS();
		}
		else if (si == UI_EVENTCODE_0DF) { // 0xDF
			_4976_5bf2 = 1;
		}
		else if (si == UI_EVENTCODE_QUIT_GAME) { // 0xE0
			SK_PREPARE_EXIT();
		}
	}

	if (glbUIEventCode >= UI_EVENTCODE_0A5 && glbUIEventCode <= UI_EVENTCODE_0C6)	// >= 0xa5 and <= 0xc6)
		cd.gg.glbRefreshViewport = 1;
	return di;
}


