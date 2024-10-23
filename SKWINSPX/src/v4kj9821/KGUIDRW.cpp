#include <StdAfx.h>	// Required for MVC6 compilation

#include <SKVER.H>
#include <KCORE.H>
#include <SKDEBUG.H>

//^0B36:131A
void SkWinCore::DRAW_SIMPLE_STR(sk3f6c *ref, Bit16u rectno, Bit16u clr1, Bit16u clr2, Bit8u *str)
{
	// you can draw:
	// e.g. spell symbols, represented as "<<","Å†","ÅÅ","Åû",")-","áô".
	// e.g. damage amount like "80"

	// their drawStr is regurally scheduled. e.g. you can print damage "80" at portrait/status panel. no another position/text pattern.

	//^0B36:131A
	ENTER(12);
	//^0B36:131E
	i16 bp02;
	i16 bp04;
	SRECT bp0c;
	// bp0c -> bp0c.x
	// bp0a -> bp0c.y
	// bp08 -> bp0c.cx
	// bp06 -> bp0c.cy

	if (QUERY_STR_METRICS(str, &bp02, &bp04) != 0 && QUERY_BLIT_RECT(NULL, &bp0c, rectno, &bp02, &bp04, -1) != 0) {
		//^0B36:135E
		DRAW_STRING(
			reinterpret_cast<Bit8u *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(ref->w0)), 
			ref->w0, 
			ref->rc2.cx, 
			bp0c.x             -ref->rc2.x,
			bp0c.y +bp0c.cy -1 -ref->rc2.y,
			clr1,
			clr2,
			str,
			8
			);
		//^0B36:139D
		_0b36_0d67(ref, &bp0c);
	}
	//^0B36:13AF
	return;
}

//^0B36:10B6
void SkWinCore::DRAW_ICON_PICT_BUFF(
	const Bit8u *buff, 
	sk3f6c *tt, 
	SRECT *rc, 
	i16 srcx, 
	i16 srcy, 
	i16 colorkey, 
	i16 flipmirror, 
	Bit8u *localpal
) {
	// draw icon image such as:
	// a) right part of panel (commander part)
	//		directional/curved arrow, 2x2 floor status panel, command buttons, X button, spell board,
	// b) upper part of panel (status part)
	//		player plate, hand icon, portrait icon
	// c) upper and/or right part of panel
	//		item icon (such as dagger, etc), colored player icon

	//^0B36:10B6
	ENTER(8);
	//^0B36:10BA
	if (rc != NULL) {
		//^0B36:10C2
		SRECT bp08;
		FIRE_BLIT_PICTURE(
			buff,
			QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(tt->w0),
			OFFSET_RECT(tt, rc, &bp08),
			srcx,
			srcy,
			READ_UI16(buff,-4),
			tt->rc2.cx,
			colorkey,
			flipmirror,
			READ_UI16(buff,-6),
			8,
			localpal
			);
		//^0B36:1121
		_0b36_0d67(tt, rc);
	}
	//^0B36:1134
	return;
}

//^0B36:1136
void SkWinCore::DRAW_ICON_PICT_ENTRY(Bit8u cls1, Bit8u cls2, Bit8u cls4, sk3f6c *ss, Bit16u rectno, i16 colorkey)
{
	//^0B36:1136
	ENTER(20);
	//^0B36:113A
	i16 bp0c = 0;
	i16 bp0a = 0;
	//^0B36:1142
	Bit8u *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(cls1, cls2, cls4);
	//^0B36:115C
	SRECT bp14;
	SRECT *bp08 = QUERY_BLIT_RECT(bp04, &bp14, rectno, &bp0a, &bp0c, -1);
	//^0B36:1184
	DRAW_ICON_PICT_BUFF(
		bp04,
		ss,
		bp08,
		bp0a,
		bp0c,
		colorkey,
		0,
		QUERY_GDAT_IMAGE_LOCALPAL(cls1, cls2, cls4)
		);
	//^0B36:11BE
	return;
}

//^29EE:00E0
void SkWinCore::SHOW_ATTACK_RESULT(i16 yourValue)
{
	// yourValue: it means a damage value if positive, it means a failure reason if negative

	// if -1, go fore to attack
	// if -2, hand a empty flask
	// if -3, your meaningless spell
	// if -4, failure wizard spell
	// if -5, failure priest spell
	// if -6, go X-mark to teleport

	//^29EE:00E0
	ENTER(324);
	//^29EE:00E6
	i16 di = yourValue;
	//^29EE:00E9
	_29ee_00a3(1);
	//^29EE:00F0
	Bit8u bp01 = 4;
	Bit8u bp02 = 1;
	//^29EE:00F8
	if (di < 0) {
		//^29EE:00FC
		switch (di) {
			case ATTACK_FAILURE_X_TELEPORT:	// -6
				//^29EE:010F
				bp02 = 0x19; // 0x19 = [icon] move to X-mark to teleport
				break;
			case ATTACK_REQUIRES_HAND_ITEM:	// -2
				//^29EE:011B
				bp02 = 0x17; // 0x17 = [icon] hand a empty flask to cast
				break;
			case ATTACK_FAILURE_NOT_FRONT:	// -1
				//^29EE:0115
				bp02 = 0x18; // 0x18 = [icon] hind player cannot attack physically
				break;
			default:
				//^29EE:0121
				bp01 = 5;
				//^29EE:0125
				if (di == ATTACK_MEANINGLESS_SPELL) {	// -3
					//^29EE:012A
					bp02 = 0x0e; // 0x0e = [icon] unknown spell
				}
				//^29EE:0130
				else if (di == ATTACK_FAILURE_WIZARD) {	// -4
					//^29EE:0135
					bp02 = 0x0c; // 0x0c = [icon] fail wizard spell
				}
				else {	// Supposedly ATTACK_FAILURE_PRIEST !!
					//^29EE:013B
					bp02 = 0x0d; // 0x0d = [icon] fail priest spell
				}
				break;
		}
	}
	//^29EE:013F
	ExtendedPicture bp0144;
	QUERY_GDAT_SUMMARY_IMAGE(&bp0144, 1, bp01, bp02);
	//^29EE:0157
	// SPX: Depending on the amount of damage done, stretch the damage done hit image
	if (di >= 0 && di <= 40) {
		//^29EE:0160
		if (di > 15) {
			//^29EE:0165
			bp0144.w52 = 0x002b;
			bp0144.w54 = 0x003b;
		}
		else {
			//^29EE:0173
			bp0144.w52 = 0x001c;
			bp0144.w54 = 0x002c;
		}
	}
	//^29EE:017F
	_0b36_11c0(QUERY_PICST_IT(&bp0144), &_4976_3f6c, 57, -1);
	//^29EE:019E
	if (di >= 0) {
#if UseAltic
		ATLASSERT(di < 999); di = min(di, 999);
#endif

		//^29EE:01A2
		Bit16u si = 5;
		//^29EE:01A5
		Bit8u bp0a[8];
		bp0a[5] = 0;
		do {
			//^29EE:01A9
			si--;
			//^29EE:01AA
			bp0a[si] = U8(di % 10) + '0';
			//^29EE:01B8
			di = di / 10;
			//^29EE:01BF
		} while (di != 0);

		//^29EE:01C3
		si--;
		bp0a[si] = '!';
		si--;
		bp0a[si] = 0x02;
		//^29EE:01CD
		DRAW_SIMPLE_STR(&_4976_3f6c, 0x39, glbPaletteT16[COLOR_CYAN], glbPaletteT16[COLOR_BLACK], &bp0a[si]);
		//^29EE:01F5
	}
	//^29EE:01F7
	//else if (di == -2) {
	else if (di == ATTACK_REQUIRES_HAND_ITEM) {
		//^29EE:01FC
		DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, (glbWeaponMissileHand == 0) ? GDAT_INTERFACE_BODY_HAND_RIGHT : GDAT_INTERFACE_BODY_HAND_LEFT, &_4976_3f6c, 126, -1);
		//^29EE:021E
		bp01 = 0;
		//^29EE:0222
		// SPX: Search through all weapons to find which projectile fits the current shooter item
		for (; bp01 < 255; bp01++) {
			//^29EE:0224
			Bit16u si = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WEAPONS, bp01, dtWordValue, GDAT_ITEM_WEAPON_PROJECTILE_FLAG); // 0x05
			//^29EE:0238
			// SPX: Check that the weapon is not a launcher (0x8000) and match the shooter flag
			if (si != 0 && (si & WEAPON_FLAGS_SHOOTER) == 0 && (glbWeaponShooterNum & si) != 0) {
				//^29EE:0248
				DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_WEAPONS, bp01, 0x18, &_4976_3f6c, 125, -1);
				//^29EE:025D
				return;
			}
			//^29EE:025F
		}
	}
	//^29EE:0268
	return;
}



//^2759:0541
// SPX: _2759_0541 renamed DISPLAY_RIGHT_PANEL_SQUAD_HANDS
void SkWinCore::DISPLAY_RIGHT_PANEL_SQUAD_HANDS()
{
	//^2759:0541
	ENTER(2);
	//^2759:0545
	i16 bp02 = glbChampionIndex;
	//^2759:054B
	if (bp02 != 0) {
		//^2759:054F
		if (glbChampionAttackDamage == 0 || glbSomeChampionPanelFlag <= 1)
			//^2759:055D
			glbSomeChampionPanelFlag = 0;

		//^2759:0563
		glbChampionIndex = 0;
		glbRightPanelType = RIGHT_PANEL_SQUAD_HANDS;
		glbMagicalMapFlags = 0;
		if (glbObjectForContainer != OBJECT_NULL) {
			//^2759:057C
			PUT_OBJECT_INTO_CONTAINER();
		}
		//^2759:0581
		_4976_531c = 1;
	}
	//^2759:0587
	return;
}


//^2405:05B4
// SPX: _2405_05b4 renamed DRAW_ITEM_ON_WOOD_PANEL
Bit8u *SkWinCore::DRAW_ITEM_ON_WOOD_PANEL(Bit16u player, Bit16u possessionIndex, Picture *ref)
{
	//^2405:05B4
	ENTER(348);
	//^2405:05BA
	if (IS_ITEM_HAND_ACTIVABLE(player, glbChampionSquad[player].Possess(possessionIndex), possessionIndex) == 0) {
		//^2405:05E1
		//^2405:07BE
		return NULL;
	}
	//^2405:05E8
	Bit16u bp0c = _4976_0106 + _4976_010e;
	Bit16u bp0e = _4976_0108 + _4976_0110;
	//^2405:05FC
	Bit16u bp0a = ALLOC_TEMP_CACHE_INDEX();
	//^2405:0604
    Bit8u *bp04 = ALLOC_NEW_PICT(bp0a, bp0c, bp0e, 8);
	//^2405:061D
	ObjectID di = glbChampionSquad[player].Possess(possessionIndex);
	//^2405:0634
	Bit8u bp05;
	Bit8u bp06;
	Bit8u bp07;
	Bit16u bp12;
	if (di == OBJECT_NULL) {
		//^2405:0639
		bp05 = 1;
		bp06 = 2;
		//^2405:0641
		bp07 = (possessionIndex == 1) ? 8 : 7;
		bp12 = 1;
	}
	else {
		//^2405:0657
		bp05 = QUERY_CLS1_FROM_RECORD(di);
		//^2405:0661
		bp06 = QUERY_CLS2_FROM_RECORD(di);
		//^2405:066B
		bp07 = GET_ITEM_ICON_ANIM_FRAME(di, -1, 0);
		//^2405:067A
		bp12 = 0;
	}
	//^2405:067F
	ExtendedPicture bp014c;
	QUERY_GDAT_SUMMARY_IMAGE(&bp014c, bp05, bp06, bp07);
	//^2405:0699
	QUERY_PICST_IT(&bp014c);
	//^2405:06A6
	Bit8u bp0f = glbPaletteT16[COLOR_RED];	// SPX: RED here is used as transparent for hand icon ..
	//^2405:06B1
	FILL_ENTIRE_PICT(bp04 = _0b36_00c3(bp0a, ref), bp0f);
	//^2405:06D5
	if (bp12 != 0) {
		//^2405:06DB
		//^2405:0792
		DRAW_DIALOGUE_PICT(
			QUERY_PICT_BITS(&bp014c),
			bp04,
			ALLOC_TEMP_ORIGIN_RECT(bp014c.width, bp014c.height),
			0,
			0,
			12,
			bp014c.b58
			);
	}
	else {
		//^2405:06F2
		Bit8u bp015c[16];
		for (Bit16u si = 0; si < 16; si++) {
			//^2405:06F6
			bp015c[si] = glbPaletteT16[COLOR_BLACK];
			//^2405:0701
		}
		//^2405:0707
		bp015c[12] = bp0f;
		//^2405:070E
		DRAW_DIALOGUE_PICT(
			QUERY_PICT_BITS(&bp014c),
			bp04,
			ALLOC_TEMP_RECT(_4976_010e, _4976_0110, _4976_0106, _4976_0108),
			0,
			0,
			-1,
			bp015c
			);
		//^2405:0751
		TRANSLATE_PALETTE(bp014c.b58, 1, 0, 1, 16);
		//^2405:0767
		bp04 = _0b36_00c3(bp0a, ref);
		//^2405:077E
		//^2405:0792
		DRAW_DIALOGUE_PICT(
			QUERY_PICT_BITS(&bp014c),
			bp04,
			ALLOC_TEMP_ORIGIN_RECT(_4976_0106, _4976_0108),
			0,
			0,
			12,
			bp014c.b58
			);
	}
	//^2405:07B8
	return bp04;
}


//^0B36:105B
// SPX: _0b36_105b renamed DRAW_GRAY_OVERLAY
void SkWinCore::DRAW_GRAY_OVERLAY(sk3f6c *ref, SRECT *rc, Bit16u xx)
{
	//^0B36:105B
	ENTER(8);
	//^0B36:105F
	if (rc != NULL) {
		//^0B36:1067
		SRECT bp08;
		_44c8_1aca(
			reinterpret_cast<Bit8u *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(ref->w0)),
			OFFSET_RECT(ref, rc, &bp08),
			xx,
			ref->rc2.cx
			);
		//^0B36:10A1
		_0b36_0d67(ref, rc);
	}
	//^0B36:10B4
	return;
}

//^29EE:026C
// SPX: _29ee_026c renamed DRAW_HAND_ACTION_ICONS
void SkWinCore::DRAW_HAND_ACTION_ICONS(Bit16u playerIndex, Bit16u possessionIndex, Bit16u leftOrRight)
{
	//^29EE:026C
	ENTER(48);
	//^29EE:0271
	_29ee_00a3(0);
	//^29EE:0278
	Champion *champion = &glbChampionSquad[playerIndex];
	//^29EE:0289
	Bit16u si = (possessionIndex == 1) ? 0x46 : 0x4a;
	//^29EE:0297
	si += (champion->playerPos() +4 - glbPlayerDir) & 3;
	//^29EE:02AC
	SRECT bp10;
	QUERY_EXPANDED_RECT(si, &bp10);
	//^29EE:02BA
	if (champion->curHP() == 0) {
		//^29EE:02C4
		SRECT bp10;
		FILL_RECT_SUMMARY(&_4976_3f6c, &bp10, glbPaletteT16[COLOR_BLACK]);
	}
	else {
		//^29EE:02DF
		// SPX: Draw background item square
		DRAW_ICON_PICT_ENTRY(0x01, 0x04, (Bit8u(possessionIndex) << 1) + Bit8u(leftOrRight) +2, &_4976_3f6c, si, -1);
		//^29EE:02FD
		Picture bp30;
		Bit8u *bp08 = DRAW_ITEM_ON_WOOD_PANEL(playerIndex, possessionIndex, &bp30);
		//^29EE:0316
		if (bp08 != NULL) {
			//^29EE:031A
			SRECT bp18;
			// SPX: draw item (weapon, shield, flask, etc ..) in hand
			DRAW_ICON_PICT_BUFF(
				bp08,
				&_4976_3f6c,
				CALC_CENTERED_RECT_IN_RECT(&bp18, &bp10, READ_UI16(bp08,-4), READ_UI16(bp08,-2)),
				0,
				0,
				glbPaletteT16[COLOR_RED],
				0,
				NULL
				);
			//^29EE:0360
			FREE_PICT_MEMENT(&bp30);
		}
		//^29EE:036C
		if (champion->handCooldown[possessionIndex] != 0 || glbIsPlayerSleeping != 0) {
			//^29EE:0380
			DRAW_GRAY_OVERLAY(&_4976_3f6c, &bp10, 0);
		}
	}
	//^29EE:0393
	return;
}

//^29EE:058D
// SPX: _29ee_058d renamed DRAW_SQUAD_SPELL_AND_LEADER_ICON
void SkWinCore::DRAW_SQUAD_SPELL_AND_LEADER_ICON(Bit16u player, Bit16u yy)
{
	//^29EE:058D
    ENTER(324);
	//^29EE:0593
	_29ee_00a3(0);
	//^29EE:059A
	Bit16u si = (glbChampionSquad[player].playerPos() +4 - glbPlayerDir) & 3;
	//^29EE:05B6
	SRECT bp0a;
	FILL_RECT_SUMMARY(
		&_4976_3f6c,
		QUERY_EXPANDED_RECT(si + 0x4f, &bp0a),
		glbPaletteT16[COLOR_BLACK]
		);
	//^29EE:05E1
	if (glbChampionSquad[player].curHP() != 0) {
		//^29EE:05F5
		Bit16u di = (si == 1 || si == 2) ? 1 : 0;
		//^29EE:0606
		Bit8u bp01;
		Bit8u bp02;
		if (si <= 1) {
			//^29EE:060B
			bp01 = 10;
			bp02 = 6;
		}
		else {
			//^29EE:0615
			bp01 = 12;
			bp02 = 8;
		}
		//^29EE:061D
		if (player == glbChampionLeader) {
			//^29EE:0626
			bp01++;
		}
		//^29EE:0629
		if (yy != 0) {
			//^29EE:062F
			bp02++;
		}
		//^29EE:0632
		ExtendedPicture bp0144;
		QUERY_GDAT_SUMMARY_IMAGE(&bp0144, 1, 4, bp02);
		//^29EE:0648
		bp0144.mirrorFlip = di;
		//^29EE:064C
		_0b36_11c0(
			QUERY_PICST_IT(&bp0144),
			&_4976_3f6c,
			si + 0x57,
			4
			);
		//^29EE:066F
		if (glbIsPlayerSleeping != 0 || glbChampionSquad[player].herob44 != 0) {
			//^29EE:0687
			bp0144.rc36.x += _4976_3f6c.rc2.x;
			bp0144.rc36.y += _4976_3f6c.rc2.y;
			//^29EE:0695
			DRAW_GRAY_OVERLAY(&_4976_3f6c, &bp0144.rc36, 0);
		}
		//^29EE:06A9
		QUERY_GDAT_SUMMARY_IMAGE(&bp0144, 0x01, 0x04, bp01);
		//^29EE:06BF
		bp0144.mirrorFlip = di;
		//^29EE:06C3
		_0b36_11c0(
			QUERY_PICST_IT(&bp0144),
			&_4976_3f6c,
			si + 0x53,
			4
			);
		//^29EE:06E6
		if (glbIsPlayerSleeping != 0) {
			//^29EE:06ED
			bp0144.rc36.x = _4976_3f6c.rc2.x;
			bp0144.rc36.y = _4976_3f6c.rc2.y;
			//^29EE:06FB
			DRAW_GRAY_OVERLAY(&_4976_3f6c, &bp0144.rc36, 0);
		}
	}
	//^29EE:070F
	return;
}



//^29EE:2048
void SkWinCore::DRAW_MONEYBOX(ObjectID rl)
{
	//^29EE:2048
	ENTER(56);
	//^29EE:204E
	DRAW_ICON_PICT_ENTRY(0x14, QUERY_CLS2_FROM_RECORD(rl), 0x10, &_4976_3f6c, 0x5c, -1);
	i16 bp38[MONEY_ITEM_MAX];
	COUNT_BY_COIN_TYPES(rl, bp38);
	i16 si;
	for (si = 0; si < MONEY_ITEM_MAX; si++) {
		//^29EE:2081
		i16 bp0c = GET_ITEM_ORDER_IN_CONTAINER(rl, si);
		if (bp0c < 0)
			continue;
		i16 di = bp38[bp0c];
		if (di <= 0)
			continue;
		//^29EE:20AC
		U16 bp10 = glbMoneyItemsIDTable[bp0c];
		X8 bp09;
		X8 bp0a;
		U8 *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(
			bp09 = QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(bp10),
			bp0a = GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(bp10),
			0x18
			);
		U8 *bp08 = QUERY_GDAT_IMAGE_LOCALPAL(bp09, bp0a, 0x18);
		i16 bp14 = 0;
		i16 bp12 = 0;
		SRECT bp1c;
		QUERY_BLIT_RECT(bp04, &bp1c, si +0xdd, &bp12, &bp14, -1);
		SRECT bp24;
		bp24.cx = bp1c.cx;
		bp24.cy = bp1c.cy;
		X16 bp0e = (si << 3) +max_value(0, di -32);
		di = min_value(32, di);
		do {
			//^29EE:2158
			bp0e = (bp0e +1) & 31;
			bp24.x = bp1c.x + _4976_0154[bp0e][0];
			bp24.y = bp1c.y + _4976_0154[bp0e][1];
			UNION_RECT(&bp24, &_4976_3f6c.rc2, &bp12, &bp14);
			DRAW_ICON_PICT_BUFF(bp04, &_4976_3f6c, &bp24, bp12, bp14, 12, 0, bp08);
			//^29EE:21C8
		} while (--di > 0);
		//^29EE:21CB
	}
	//^29EE:21D4
	return;
}

//^29EE:21D8
// SPX: _29ee_21d8 renamed DRAW_CONTAINER_PANEL
void SkWinCore::DRAW_CONTAINER_PANEL(ObjectID rl, Bit16u xx)
{
// xx = 1 : right panel / xx = 0 : inventory
	//^29EE:21D8
	ENTER(24);
	//^29EE:21DE
	X8 bp05 = QUERY_CLS2_FROM_RECORD(rl);
	if (xx != 0) {
		//^29EE:21F0
		DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_CONTAINERS, bp05, GDAT_CONTAINER_STAT_10, &_4976_3f6c, 0x5c, -1);
		DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_CONTAINERS, bp05, GDAT_CONTAINER_STAT_12, &_4976_3f6c, 0xe3, 10);
	}
	SRECT bp18;
	if (xx == 0) {
		//^29EE:2224
		QUERY_EXPANDED_RECT(0x5c, &bp18);
	}
	//^29EE:2233
	X16 si;
	for (si = 0; si < CONTAINER_MAX_SLOT; si++) {	// < 8
		//^29EE:2238
		ObjectID di = glbCurrentContainerItems[si];
		if (di != OBJECT_NULL) {
			X16 bp08 = si +0xe5;
			if (xx == 0) {
				//^29EE:2258
				SRECT bp10;
				QUERY_EXPANDED_RECT(bp08, &bp10);
				U8 *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(GDAT_CATEGORY_CONTAINERS, bp05, GDAT_CONTAINER_STAT_10);	// 0x14	.. 0x10
				DRAW_ICON_PICT_BUFF(
					bp04, &_4976_3f6c, &bp10, bp10.x -bp18.x, bp10.y -bp18.y,
					-1, 0, QUERY_GDAT_IMAGE_LOCALPAL(GDAT_CATEGORY_CONTAINERS, bp05, GDAT_CONTAINER_STAT_10)	// 0x14	.. 0x10
					);
			}
			//^29EE:22B9
			DRAW_ICON_PICT_ENTRY(
				QUERY_CLS1_FROM_RECORD(di),
				QUERY_CLS2_FROM_RECORD(di),
				GET_ITEM_ICON_ANIM_FRAME(di, si +0x1e, 1),
				&_4976_3f6c, bp08, 12
				);
		}
		//^29EE:22EC
	}
	//^29EE:22F5
	return;
}

//^29EE:0396
// SPX: _29ee_0396 renamed DRAW_SQUAD_POS_INTERFACE
void SkWinCore::DRAW_SQUAD_POS_INTERFACE()
{
	//^29EE:0396
	ENTER(350);
	//^29EE:039C
	_29ee_00a3(0);
	//^29EE:03A3
	DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_GRAPHICSSET, glbSquadInterfaceMapGfxSet, GDAT_GFXSET_SQUAD_4X, &_4976_3f6c, 47, -1);	// gfx = 0xF5
	//^29EE:03BC
	Bit8u *bp04 = ALLOC_PICT_BUFF(_4976_0118, _4976_011a, afDefault, 4);
	//^29EE:03D6
	SRECT bp14;
	for (Bit16u si = 0; si < glbChampionsCount; si++) {
		//^29EE:03DB
		Bit16u bp06 = (glbChampionSquad[si].playerPos() +4 - glbPlayerDir) & 3;
		//^29EE:03F7
		if (glbChampionSquad[si].curHP() == 0 || bp06 +1 == _4976_5dbc)
			//^29EE:0414
			continue;
		//^29EE:0417
		i16 bp0c = 0;
		i16 bp0a = 0;
		//^29EE:041F
		Bit16u bp08;
		if (QUERY_BLIT_RECT(bp04, &bp14, bp08 = bp06 +53, &bp0a, &bp0c, -1) == NULL)
			//^29EE:044C
			continue;
		//^29EE:044F
		if (glbGlobalSpellEffects.AuraOfSpeed != 0) {
			//^29EE:0456
			i16 di = RAND02();
			//^29EE:045D
			if (di != 0) {
				//^29EE:0461
				bp14.y += di -2;
				//^29EE:0467
				di = RAND02();
				//^29EE:046E
				if (di != 0) {
					//^29EE:0472
					bp14.x += di -2;
				}
			}
		}
		//^29EE:0478
		Bit8u bp015e[16];
		DRAW_ICON_PICT_BUFF(
			QUERY_GDAT_SQUAD_ICON(bp04, Bit8u(si), bp015e),
            &_4976_3f6c,
			&bp14,
			bp0a,
			bp0c,
			12,
			0,
			bp015e
			);
		//^29EE:04B0
		if (glbChampionSquad[si].enchantmentPower == 0)
			//^29EE:04C0
			continue;
		//^29EE:04C3
		ExtendedPicture bp014e;
		// SPX: Check in INTERFACE (0x01) 07 => Party protection animations
		QUERY_PICST_IT(QUERY_GDAT_SUMMARY_IMAGE(&bp014e, GDAT_CATEGORY_INTERFACE_GENERAL, 0x07, glbChampionSquad[si].enchantmentAura));
		//^29EE:04EC
		bp014e.w4 |= 0x10;
		bp014e.width >>= 2;
		//^29EE:04F7
		bp014e.w14 = bp014e.width * ((glbChampionSquad[si].playerDir() +4 - glbPlayerDir) & 3);
		//^29EE:051E
		bp014e.height >>= 2;
		//^29EE:0523
		bp014e.w16 = bp014e.height * (i16(glbGameTick) & 3);
		//^29EE:0535
		_0b36_11c0(&bp014e, &_4976_3f6c, bp08, 12);
		//^29EE:054C
	}
	//^29EE:0556
	FREE_PICT_BUFF(bp04);
	//^29EE:0563
	if (glbIsPlayerSleeping != 0) {
		//^29EE:056A
		DRAW_GRAY_OVERLAY(
			&_4976_3f6c, 
			QUERY_EXPANDED_RECT(47, &bp14),
			0
			);
	}
	//^29EE:0589
	return;
}

//^3929:06E1
void SkWinCore::DRAW_STRONG_TEXT(Bit8u *buff, Bit16u ww, Bit16u cx, Bit16u xx, Bit16u yy, Bit16u clr1, Bit16u fill, Bit8u *str)
{
	// draw bold with shadow single-byte-character-set text such as,
	// a) name "TORHAM"
	// b) HP "651/651", MP "446/446", stamina "228/228", weight "25.0/62.0 KG"

	//^3929:06E1
	ENTER(4);
	//^3929:06E7
	Bit16u di = xx;
	Bit16u si = yy;
	//^3929:06ED
	i16 bp02;
	i16 bp04;
	if (QUERY_STR_METRICS(str, &bp02, &bp04) != 0) {
		//^3929:070B
		if ((fill & 0x4000) == 0) {
			//^3929:0712
			FIRE_FILL_RECT_ANY(
				buff,
				ALLOC_TEMP_RECT(di -1, si -bp04, bp02 +2, bp04 +2),
				fill,
				cx,
				8
				);
		}
		//^3929:0748
		DRAW_STRING(buff, ww, cx, di   , si +1, glbPaletteT16[COLOR_BLACK], glbPaletteT16[COLOR_WHITE] | 0x4000, str, 8); // shadow text, black
		//^3929:077D
		DRAW_STRING(buff, ww, cx, di +1, si +1, glbPaletteT16[COLOR_BLACK], glbPaletteT16[COLOR_WHITE] | 0x4000, str, 8);	// shadow text, black
		//^3929:07B5
		DRAW_STRING(buff, ww, cx, di,    si,    clr1,          fill           | 0x4000, str, 8);
	}
	//^3929:07DD
	return;
}

//^0B36:13B1
void SkWinCore::DRAW_NAME_STR(sk3f6c *ref, Bit16u rectno, Bit16u clr1, Bit16u fill, Bit8u *str)
{
	// draw text:
	// a) player's name "TORHAM", "SAROS" at upper-panel and command bar
	// b) command name "éaÇÈ", "éaÇËóÙÇ≠", "ífÇøêÿÇÈ", "<02>!{UM} {ZO} {BRO} {ROS}"

	//^0B36:13B1
	ENTER(12);
	//^0B36:13B6
	i16 bp02;
	i16 bp04;
	if (QUERY_STR_METRICS(str, &bp02, &bp04) != 0) {
		//^0B36:13D1
		SRECT bp0c;
		if (QUERY_BLIT_RECT(NULL, &bp0c, rectno, &bp02, &bp04, -1) != NULL) {
			//^0B36:13F5
			DRAW_STRONG_TEXT(
				reinterpret_cast<Bit8u *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(ref->w0)),
				ref->w0,
				ref->rc2.cx,
				bp0c.x             -ref->rc2.x,
				bp0c.y +bp0c.cy -1 -ref->rc2.y,
				clr1,
				fill,
				str
				);
            //^0B36:1432
			_0b36_0d67(ref, &bp0c);
		}
	}
	//^0B36:1444
	return;
}

//^29EE:0713
void SkWinCore::DRAW_PLAYER_NAME_AT_CMDSLOT()
{
	//^29EE:0713
	ENTER(0);
	//^29EE:0716
	DRAW_ICON_PICT_ENTRY(0x01, 0x04, 0x14, &_4976_3f6c, 60, -1);
	//^29EE:072C
	DRAW_ICON_PICT_ENTRY(0x01, 0x04, 0x0e, &_4976_3f6c, 59, -1);
	//^29EE:0742
	DRAW_NAME_STR(
		&_4976_3f6c, 
		61, 
		(glbChampionIndex -1 == glbChampionLeader) ? glbPaletteT16[COLOR_ORANGE] : glbPaletteT16[COLOR_WHITE], 
		glbPaletteT16[COLOR_DARKEST_GRAY] | 0x4000, 
		glbChampionTable[glbChampionIndex].firstName
		);
	// SPX: This (9 : 15) controls the color of the char name in the cast spell panel.
	// However, 9 is dark brown for leader, and that was wrong. Leader is to be orange. Non-leader is white
	//^29EE:0789
	return;
}

//^0CAF:0008
i16 SkWinCore::SK_STRLEN(const Bit8u *ref)
{
	ENTER(4);
	const Bit8u *bp04 = ref;
	while (*(ref++) != 0);
	return Bit16u(ref -bp04 -1);
}


//^29EE:093E
// SPX: _29ee_093e renamed DRAW_SPELL_TO_BE_CAST
void SkWinCore::DRAW_SPELL_TO_BE_CAST(Bit16u xx)
{
	//^29EE:093E
	ENTER(8);
	//^29EE:0944
	_29ee_00a3(0);
	//^29EE:094B
	// SPX: Cyan rectangles to be put over spell line
	if (xx != 0) {
		//^29EE:0951
		DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_INTERFACE_GENERAL, 0x05, 0x09, &_4976_3f6c, 252, -1);
	}
	//^29EE:0968
	Champion *champion = &glbChampionTable[glbChampionIndex];
	//^29EE:0979
	Bit8u bp08[2];
    bp08[1] = 0;
	//^29EE:097D
	Bit16u bp06 = SK_STRLEN(champion->GetRunes());
	//^29EE:0991
	Bit16u di = 261;
	//^29EE:0994
	for (Bit16u si = 0; si < bp06; si++) {
		//^29EE:0998
		bp08[0] = champion->GetRunes()[si];
		//^29EE:09A2
		// SPX: Spell runes color
		DRAW_SIMPLE_STR(&_4976_3f6c, di++, glbPaletteT16[COLOR_BLACK], glbPaletteT16[COLOR_LIGHTER_GRAY] | 0x4000, bp08);
		//^29EE:09CB
	}
	//^29EE:09D1
	return;
}

//^29EE:09D5
// SPX: _29ee_09d5 renamed DRAW_SPELL_PANEL
void SkWinCore::DRAW_SPELL_PANEL()
{
	//^29EE:09D5
	ENTER(10);
	//^29EE:09DA
	Champion *champion = &glbChampionTable[glbChampionIndex];
	//^29EE:09EB
	Bit16u bp06 = champion->runesCount;
	//^29EE:09F7
	// SPX: rune class interface
	DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_INTERFACE_GENERAL, 0x05, bp06 +1, &_4976_3f6c, 92, -1);
	//^29EE:0A11
	if (bp06 < 4) {
		//^29EE:0A17
		Bit8u bp0a[2];
		bp0a[1] = 0;
		//^29EE:0A1B
		Bit8u bp07 = Bit8u(bp06 * 6) + 0x60;
		Bit16u si = 255;
		//^29EE:0A2B
		for (bp06 = 0; bp06 < 6; bp06++) {
			//^29EE:0A32
			bp0a[0] = bp07++;
			//^29EE:0A3B
			DRAW_SIMPLE_STR(
				&_4976_3f6c, 
				si++, 
				glbPaletteT16[COLOR_BLACK],
				glbPaletteT16[COLOR_LIGHTER_GRAY] | 0x4000,
				bp0a
				);
			//^29EE:0A64
		}
	}
	//^29EE:0A6D
	DRAW_SPELL_TO_BE_CAST(0);
	//^29EE:0A74
	DRAW_PLAYER_ATTACK_DIR();
	//^29EE:0A78
	return;
}

//^0CAF:00D2
Bit8u *SkWinCore::SK_STRSTR(const Bit8u *xx, const Bit8u *yy)
{
	//^0CAF:00D2
	ENTER(10);
	//^0CAF:00D6
	Bit8u cl = yy[0];
	//^0CAF:00DE
	if (cl != 0) {
		//^0CAF:00E2
		Bit8u bp09;
		for (; (bp09 = xx[0]) != 0; xx++) {
			//^0CAF:00E4
			if (bp09 == cl) {
				//^0CAF:00EB
				const Bit8u *bp04 = xx +1;
				const Bit8u *bp08 = yy +1;
				//^0CAF:0105
				while (bp08[0] != 0 && bp04[0] == bp08[0]) {
					//^0CAF:0107
					bp04++;
					bp08++;
					//^0CAF:010D
				}
				//^0CAF:0124
				if (bp08[0] == 0) {
					//^0CAF:012F
					return const_cast<Bit8u *>(xx);
				}
			}
			//^0CAF:0137
		}
	}
	//^0CAF:0147
	return NULL;
}


//^29EE:0A7B
void SkWinCore::DRAW_CMD_SLOT(Bit16u cmdSlot, Bit8u ww)
{
	//^29EE:0A7B
	//^29EE:0A7F
	Bit16u si = cmdSlot;
	//^29EE:0A82
	_29ee_00a3(0);
	//^29EE:0A89
	if (glbMagicalMapFlags != 0) {
		//^29EE:0A90
		DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_CONTAINERS, glbHoldedContainerType, ((glbItemSelected[si].entry +0xf8) << 1) +ww +0x41, &_4976_3f6c, si +110, -1);
	}
	else {
		//^29EE:0ABC
		DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_INTERFACE_GENERAL, 0x04, ww +0x15, &_4976_3f6c, si +63, -1);
		//^29EE:0ADA
		DRAW_NAME_STR(
			&_4976_3f6c,
			si +66,
			glbPaletteT16[COLOR_WHITE],
			glbPaletteT16[COLOR_BLACK] | 0x4000,
			QUERY_CMDSTR_NAME(glbItemSelected[si].category, glbItemSelected[si].index, glbItemSelected[si].entry)
			);
	}
	//^29EE:0B25
	//^29EE:0B28
	return;
}

//^29EE:0B2B
void SkWinCore::_29ee_0b2b()
{
	//^29EE:0B2B
	//^29EE:0B2F
	for (Bit16u si = 0; si < _4976_53a4; si++) {
		//^29EE:0B33
		DRAW_CMD_SLOT(si, 0);
		//^29EE:0B3C
	}
	//^29EE:0B43
	DRAW_PLAYER_ATTACK_DIR();
	//^29EE:0B47
}

//^29EE:1D03
void SkWinCore::_29ee_1d03(Bit16u xx)
{
	//^29EE:1D03
	ENTER(6);
	//^29EE:1D08
	Bit16u si = xx;
	//^29EE:1D0B
	_29ee_00a3(0);
	//^29EE:1D12
	if (glbChampionIndex > 0 && (glbMagicalMapFlags & 0x0800) != 0) {
		//^29EE:1D24
		si ^= glbMagicalMapFlags;
		si &= 15;
		//^29EE:1D2C
		Bit8u bp06[2];
		bp06[1] = 0;
		Bit16u bp02 = 0;
		//^29EE:1D35
		for (; bp02 < 4; bp02++) {
			//^29EE:1D37
			Bit8u bp03 = 0x49;
			//^29EE:1D3B
			if ((si & 0x0001) != 0) {
				//^29EE:1D41
				bp03++;
			}
			//^29EE:1D44
			si >>= 1;
			//^29EE:1D46
			DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_MAGICAL_MAPS, glbHoldedContainerType, bp03, &_4976_3f6c, bp02 +101, -1);
			//^29EE:1D65
			bp06[0] = bp02 +0x72;
			//^29EE:1D6D
			DRAW_SIMPLE_STR(&_4976_3f6c, bp02 +105, glbPaletteT16[COLOR_BLACK], glbPaletteT16[COLOR_GRAY] | 0x4000, bp06);
			//^29EE:1D99
		}
	}
	//^29EE:1DA2
	return;
}


//^29EE:0C41
void SkWinCore::DRAW_CHIP_OF_MAGIC_MAP(const Bit8u *buff, Bit16u aa, Bit16u xx, Bit16u yy, Bit16u flipMirror, Bit8u *localpal)
{
	// draws:
	// a) parts of minion map's 7x7 atlas view.

	//^29EE:0C41
	ENTER(12);
	//^29EE:0C47
	Bit16u di = flipMirror;
	//^29EE:0C4A
	if (buff != NULL) {
		//^29EE:0C55
		i16 bp02;
		i16 bp04;
		SRECT bp0c;
		if (UNION_RECT(SET_SRECT(&bp0c, xx, yy, glbMagicMapWidth, glbMagicMapHeight), &_4976_53a6, &bp02, &bp04) != NULL) {
			//^29EE:0C8C
			i16 si = glbMagicMapWidth - bp0c.cx;
			//^29EE:0C94
			if (si != 0 && (di & 0x0001) != 0) {
				//^29EE:0C9E
				bp02 = (bp02 != 0) ? 0 : si;
			}
			//^29EE:0CAE
			si = glbMagicMapHeight - bp0c.cy;
			//^29EE:0CB6
			if (si != 0 && (di & 0x0002) != 0) {
				//^29EE:0CC0
				bp04 = (bp04 != 0) ? 0 : si;
			}
			//^29EE:0CD0
			DRAW_ICON_PICT_BUFF(
				buff,
				&_4976_3f6c,
				&bp0c,
				glbMagicMapWidth * aa +bp02,
					            +bp04,
				10,
				di,
				localpal
				);
			printf("");
		}
	}
	//^29EE:0CFD
	return;
}


//^29EE:0D01
void SkWinCore::DRAW_MAP_CHIP(ObjectID recordLink, i16 ss, i16 tt, i16 ww, Bit16u xx, Bit16u yy, Bit8u cc, Bit16u flags)
{
	//^29EE:0D01
	ENTER(108);
	//^29EE:0D07
	Bit16u di = yy;
	//^29EE:0D0A
	Bit8u *bp2e = NULL;
	//^29EE:0D14
	ExtendedTileInfo bp4c;
	SUMMARIZE_STONE_ROOM(&bp4c, ww, ss, tt);
	//^29EE:0D2A
	Bit8u bp5c[16];
	QUERY_DUNGEON_MAP_CHIP_PICT(GDAT_CATEGORY_GRAPHICSSET, cc, &bp2e, bp5c);
	//^29EE:0D41
	if ((flags & 0x0010) != 0) {
		//^29EE:0D48
		SRECT bp3c;
		i16 bp34;
		UNION_RECT(
			SET_SRECT(
				&bp3c, 
				xx - glbMagicMapInterlineX, 
				di - glbMagicMapInterlineY, 
				(glbMagicMapInterlineX << 1) + glbMagicMapWidth, 
				(glbMagicMapInterlineY << 1) + glbMagicMapHeight
				),
			&_4976_53a6,
			&bp34,
			&bp34
		);
		//^29EE:0D91
		FILL_RECT_SUMMARY(&_4976_3f6c, &bp3c, glbPaletteT16[COLOR_RED]);
	}
	//^29EE:0DAD
	Bit16u bp14 = ((bp4c.w2 >> 5) == ttTrickWall && (bp4c.w2 & 1) != 0) ? 1 : 0;
	Bit16u bp20 = 0;
	i16 bp0a;
	Bit8u *bp2a;
	Bit8u bp6c[16];
	i16 bp0e;
	i16 bp0c;

	//^29EE:0DCD
_0dcd:
	switch (bp4c.w0) {
		case 0://^0DDF
		case 7://^0DDF
			//^29EE:0DDF
			{
				if ((flags & 0x0002) != 0 && bp14 != 0 && (Bit16u(glbGameTick) & 2) != 0) {
					//^29EE:0DF7
					bp0a = 1;
					//^29EE:0DFC
					goto _10b7;
				}
				//^29EE:0DFF
				if (bp20 == 0) {
					//^29EE:0E05
					DRAW_CHIP_OF_MAGIC_MAP(bp2e, 0, xx, di, 0, bp5c);
				}
				//^29EE:0E1F
				for (bp0a = 0; bp0a <= 3; bp0a++) {
					//^29EE:0E27
					bp0c = bp4c.w6[bp0a] & 0xff;
					//^29EE:0E3A
					if (bp0c != 0xff) {
						//^29EE:0E41
						if ((flags & 0x0002) != 0 || QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp0c, dtWordValue, GDAT_IMG_MAP_CHIP) == 0) {
							//^29EE:0E5F
							bp0c = QUERY_DUNGEON_MAP_CHIP_PICT(GDAT_CATEGORY_WALL_GFX, Bit8u(bp0c), &bp2a, bp6c);
							//^29EE:0E79
							if (bp0c != 0) {
								//^29EE:0E7D
								bp0e = bp4c.w6[bp0a] >> 10;
								//^29EE:0E90
								bp0c = (((bp0e +1) << 1) > bp0c) ? (bp0a & 1) : ((bp0e << 1) + (bp0a & 1));
								//^29EE:0EB1
								DRAW_CHIP_OF_MAGIC_MAP(bp2a, bp0c, xx, di, _4976_3fa0[bp0a], bp6c);
							}
						}
					}
					//^29EE:0ED4
				}
				//^29EE:0EE0
				break;
			}
		case 1://^0EE3
			{
				//^29EE:0EE3
				if ((bp4c.w2 & 0x0008) != 0) {
					//^29EE:0EEA
					QUERY_DUNGEON_MAP_CHIP_PICT(GDAT_CATEGORY_GRAPHICSSET, _4976_4c5c[dunMapLocalHeader->Level()], &bp2e, bp5c);
					//^29EE:0F12
					//^29EE:18AA
					DRAW_CHIP_OF_MAGIC_MAP(bp2e, 0, xx, di, 0, bp5c);
					//^29EE:18B7
					return;
				}

				goto _0f22;
			}
		case 5://^0F22
			{
				//^29EE:0F22
_0f22:
				DRAW_CHIP_OF_MAGIC_MAP(bp2e, 2, xx, di, 0, bp5c);
				//^29EE:0F3C
				bp0a = bp4c.w6[2] & 0xff;
				//^29EE:0F45
				if (bp0a != 0xff) {
					//^29EE:0F49
					if (true
						&& ((flags & 0x0002) != 0 || QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_FLOOR_GFX, bp0a, dtWordValue, 0xf9) == 0)
						&& (bp0c = QUERY_DUNGEON_MAP_CHIP_PICT(GDAT_CATEGORY_FLOOR_GFX, Bit8u(bp0a), &bp2a, bp6c)) != 0
					) {
						//^29EE:0F85
						bp0e = (bp4c.w6[2] >> 8) / 10;
						//^29EE:0F94
						bp0a = (bp0e +1 > bp0c) ? 0 : bp0e;
						//^29EE:0FA7
						DRAW_CHIP_OF_MAGIC_MAP(bp2a, bp0a, xx, di, 0, bp6c);
					}
				}
				//^29EE:0FC2
				if (bp4c.w0 == 5) {
					//^29EE:0FCB
					bp0a = QUERY_DUNGEON_MAP_CHIP_PICT(GDAT_CATEGORY_TELEPORTERS, 0x00, &bp2a, bp6c);
					//^29EE:0FE3
					if (bp0a != 0) {
						//^29EE:0FEA
						DRAW_CHIP_OF_MAGIC_MAP(bp2a, glbGameTick % bp0a, xx, di, 0, bp6c);
					}
				}	
				//^29EE:1016
				break;
			}
		case 16://^1019
			{
				//^29EE:1019
				bp0a = 11;
				//^29EE:101E
				goto _1025;
			}
		case 17://^1020	// DOOR TILE
			{
				//^29EE:1020
				bp0a = 10;
				//^29EE:1025
_1025:
				Door *bp04 = GET_ADDRESS_OF_RECORD0(bp4c.w6[1]);
				//^29EE:1034
				if (bp04->Button() == 0 && bp04->DoorBit13C() == 0) {
					//^29EE:1053
					bp0a += 2;
				}
				//^29EE:1057
				if (bp4c.w6[0] == _DOOR_STATE__OPENED_ || bp4c.w6[0] == _DOOR_STATE__DESTROYED_) {	// (bp4c.w6[0] == 0 || bp4c.w6[0] == 5) if door is opend or destroyed
					//^29EE:1063
					bp0a += 4;
				}
				//^29EE:1067
				goto _10b7;
			}
		case 18://^1069
		case 19://^1069
			{
				//^29EE:1069
				bp0a = (_0cee_06dc_GET_TILE_DIRECTION(ss, tt) - ww) & 3;
				//^29EE:107F
				if (bp4c.w6[0] != 0) {
					//^29EE:1085
					bp0a += 2;
					bp0a &= 3;
				}
				//^29EE:108E
				bp0a += 6;
				//^29EE:1092
				goto _10b7;
			}
		case 2://^1094
			{
				//^29EE:1094
				bp0a = 3;
				//^29EE:1099
				if (bp4c.w6[0] != 0) {
					//^29EE:109F
					bp0a++;
					//^29EE:10A2
					if ((flags & 0x0002) != 0) {
						//^29EE:10A9
						if ((Bit16u(glbGameTick) & 1) != 0) {
							//^29EE:10B4
							bp0a++;
						}
					}
				}
				//^29EE:10B7
_10b7:
				DRAW_CHIP_OF_MAGIC_MAP(bp2e, bp0a, xx, di, 0, bp5c);

				break;
			}
		case 3://^10D2
		case 4://^10D2
		case 6://^10D2
		case 8://^10D2
		case 9://^10D2
		case 10://^10D2
		case 11://^10D2
		case 12://^10D2
		case 13://^10D2
		case 14://^10D2
		case 15://^10D2
			{
				break;
			}

	}
	//^29EE:10D2
	if ((flags & 0x0010) != 0) {
		//^29EE:10DC
		for (bp0a = 0; bp0a < glbChampionsCount; bp0a++) {
			//^29EE:10E4
			if (glbChampionSquad[bp0a].curHP() != 0) {
				//^29EE:10F8
				Bit8u bp21 = QUERY_CLS1_FROM_RECORD(recordLink);
				Bit8u bp22 = QUERY_CLS2_FROM_RECORD(recordLink);
				//^29EE:1110
				Bit8u *bp32 = QUERY_GDAT_IMAGE_ENTRY_BUFF(bp21, bp22, 0x40);
				//^29EE:1125
				DRAW_CHIP_OF_MAGIC_MAP(
					bp32,
					(glbGlobalSpellEffects.Invisibility != 0) ? 0x09 : (bp0a +5),
					xx,
					di,
					_4976_3fa4[(glbChampionSquad[bp0a].playerPos() +4 -ww) & 3],
					QUERY_GDAT_IMAGE_LOCALPAL(bp21, bp22, 0x40)
					);
				printf("");
			}
			//^29EE:117F
		}
	}
	//^29EE:118E
	if ((flags & 0x0040) != 0) {
		//^29EE:1195
		bp0a = (_4976_5326 != 2) ? ((_4976_5326 != 3) ? 0x02 : 0x0a) : (0x0b);
		//^29EE:11B7
		Bit8u bp21 = QUERY_CLS1_FROM_RECORD(recordLink);
		Bit8u bp22 = QUERY_CLS2_FROM_RECORD(recordLink);
		//^29EE:11CF
		Bit8u *bp32 = QUERY_GDAT_IMAGE_ENTRY_BUFF(bp21, bp22, 0x40);
		//^29EE:11E4
		DRAW_CHIP_OF_MAGIC_MAP(
			bp32,
			bp0a,
			xx,
			di,
			0,
			QUERY_GDAT_IMAGE_LOCALPAL(bp21, bp22, 0x40)
			);
	}
	//^29EE:120E
	if ((flags & 0x00a5) != 0) {
		//^29EE:1218
		ObjectID bp1c = OBJECT_END_MARKER;
		ObjectID bp1a = OBJECT_END_MARKER;
		ObjectID bp18 = OBJECT_END_MARKER;
		//^29EE:1224
		ObjectID si = bp4c.w4;
		//^29EE:1227
		for (; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
			//^29EE:122A
			bp0a = si.DBType();
			//^29EE:1235
			if (bp0a == dbCreature) {
				//^29EE:123A
				Bit16u bp16 = QUERY_CREATURE_AI_SPEC_FLAGS(si);
				//^29EE:1244
				if (QUERY_GDAT_ENTRY_DATA_INDEX(0x0f, QUERY_CLS2_FROM_RECORD(si), dtWordValue, 0xf9) != 0) {
					//^29EE:125F
					bp18 = si;
				}
				else {
					//^29EE:1265
					if ((bp16 & 0x0001)	!= 0) {
						//^29EE:126C
						if ((flags & 0x0080) != 0) {
							//^29EE:1276
							bp18 = si;
						}
					}
					else {
						//^29EE:127C
						if ((flags & 0x0021) != 0) {
							//^29EE:1286
							bp18 = si;
						}
					}
				}
			}
			else {
				//^29EE:128C
				if ((flags & 0x0004) != 0) {
					//^29EE:1296
					if (bp0a == dbMissile) {
						//^29EE:129C
						if (bp1a == OBJECT_END_MARKER) {
							//^29EE:12A5
							bp1a = si;
						}
					}
					//^29EE:12AB
					else if (bp0a == dbCloud) {
						//^29EE:12B1
						if (bp1c == OBJECT_END_MARKER) {
							//^29EE:12BA
							bp1c = si;
						}
					}
					//^29EE:12C0
					else if (bp0a >= dbWeapon && bp0a <= dbMiscellaneous_item) {
						//^29EE:12D2
						bp0a = QUERY_DUNGEON_MAP_CHIP_PICT(QUERY_CLS1_FROM_RECORD(si), QUERY_CLS2_FROM_RECORD(si), &bp2a, bp6c);
						//^29EE:12F6
						if (bp0a != 0) {
							//^29EE:12FA
							bp0c = (si.Dir() - ww) & 3;
							//^29EE:1308
							if (bp4c.w0 == 0 && bp14 == 0) {
								//^29EE:1314
								if (bp0a <= 2)
									//^29EE:1318
									continue;
								//^29EE:131A
								bp0a = (bp0c & 1) +1;
								//^29EE:1324
								bp0c = _4976_3fa0[bp0c];
							}
							else {
								//^29EE:132D
								bp0a = 0;
								//^29EE:1332
								bp0c = _4976_3fa4[bp0c];
							}
							//^29EE:1339
							//^29EE:133E
							DRAW_CHIP_OF_MAGIC_MAP(bp2a, bp0a, xx, di, bp0c, bp6c);
						}
					}
				}
			}
			//^29EE:135A
		}
		//^29EE:136B
		if (bp18 != OBJECT_END_MARKER) {
			//^29EE:1374
			Creature *bp04 = GET_ADDRESS_OF_RECORD4(bp18);
			//^29EE:1383
			Bit16u bp16 = QUERY_CREATURE_AI_SPEC_FLAGS(bp18);
			//^29EE:138F
			Bit8u bp21;
			Bit8u bp22;
			Bit8u *bp32;
			if ((flags & 0x0020) != 0) {
				//^29EE:1396
				bp21 = QUERY_CLS1_FROM_RECORD(recordLink);
				//^29EE:13A2
				bp22 = QUERY_CLS2_FROM_RECORD(recordLink);
				//^29EE:13AE
				bp32 = QUERY_GDAT_IMAGE_ENTRY_BUFF(bp21, bp22, 0x40);
				//^29EE:13C3
				//^29EE:13E6
				//^29EE:1546
				DRAW_CHIP_OF_MAGIC_MAP(
					bp32, 
					(Bit16u(glbGameTick) & 1) +3, 
					xx, 
					di, 
					0, 
					QUERY_GDAT_IMAGE_LOCALPAL(bp21, bp22, 0x40)
					);

				goto _1554;
			}
			//^29EE:13E9
			bp0c = QUERY_DUNGEON_MAP_CHIP_PICT(QUERY_CLS1_FROM_RECORD(bp18), QUERY_CLS2_FROM_RECORD(bp18), &bp2a, bp6c);
			//^29EE:1411
			if (bp0c != 0) {
				//^29EE:1418
				if ((bp16 & 0x0001) != 0) {
					//^29EE:141F
					sk1c9a02c3 *bp08 = _1c9a_02c3(bp04, QUERY_CREATURE_AI_SPEC_FROM_TYPE(bp04->CreatureType()));
					//^29EE:1443
					bp0a = _4937_000f(bp08->w0, &bp08->w2);
					//^29EE:145D
					bp0a = (bp0a >= 4 && bp0a <= 7) ? 4 : 0;
					//^29EE:1474
					if (((bp0a +1) << 1) > bp0c) {
						//^29EE:147F
						bp0a = 0;
					}
					else {
						//^29EE:1486
						bp0a = bp0a << 1;
					}
				}
				else {
					//^29EE:1490
					bp0a = i16(glbGameTick % (bp0c / 2)) << 1;
				}
				//^29EE:14AE
				bp0c = (ww - bp04->b15_0_1());
				//^29EE:14C4
				//^29EE:14E9
				//^29EE:154D
				DRAW_CHIP_OF_MAGIC_MAP(bp2a, (bp0c & 1) + bp0a, xx, di, _4976_3fa0[bp0c], bp6c);
			}
			//^29EE:14EB
			else if ((bp16 & 0x0001) == 0 && (bp16 & 0x2000) == 0) {
				//^29EE:14F9
				bp21 = QUERY_CLS1_FROM_RECORD(recordLink);
				//^29EE:1505
				bp22 = QUERY_CLS2_FROM_RECORD(recordLink);
				//^29EE:1511
				bp32 = QUERY_GDAT_IMAGE_ENTRY_BUFF(bp21, bp22, 0x40);
				//^29EE:1526
				DRAW_CHIP_OF_MAGIC_MAP(
					bp32, 
					Bit16u(glbGameTick & 1), 
					xx, 
					di, 
					0, 
					QUERY_GDAT_IMAGE_LOCALPAL(bp21, bp22, 0x40)
					);
			}
			//^29EE:1554
_1554:
			if ((bp16 & 0x0001) != 0 && (flags & 0x0004) != 0) {
				//^29EE:1568
				if ((_0cee_2df4(bp18) & 0x000f) != 0) {
					//^29EE:1576
					for (si = bp04->w0; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
						//^29EE:157E
						bp0a = si.DBType();
						//^29EE:1589
						if (bp0a >= dbWeapon && bp0a <= dbMiscellaneous_item) {
							//^29EE:1594
							if (QUERY_DUNGEON_MAP_CHIP_PICT(
									QUERY_CLS1_FROM_RECORD(si), 
									QUERY_CLS2_FROM_RECORD(si), 
									&bp2a, bp6c) != 0
							) {
								//^29EE:15B9
								DRAW_CHIP_OF_MAGIC_MAP(
									bp2a,
									0,
									xx,
									di,
									_4976_3fa4[(si.Dir() - ww) & 3],
									bp6c
									);
							}
						}
						//^29EE:15E4
					}
				}
			}
		}
		//^29EE:15F2
		for (si = bp1a; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
			//^29EE:15F8
			if (si.DBType() == dbMissile) {
				//^29EE:1608
				bp0a = QUERY_DUNGEON_MAP_CHIP_PICT(
							QUERY_CLS1_FROM_RECORD(si), 
							QUERY_CLS2_FROM_RECORD(si), 
							&bp2a, bp6c);
				//^29EE:162C
				if (bp0a != 0) {
					//^29EE:1633
					if (bp0a > 3) {
						//^29EE:163B
						bp0c = glbTimersTable[GET_ADDRESS_OF_RECORDE(si)->TimerIndex()].Direction();
						//^29EE:1662
						bp0c = (bp0c - ww) & 3;
						//^29EE:166B
						bp0a = 3;
						//^29EE:1670
						switch (_48ae_011a(si) +1) {
							case 0:
								//^29EE:1686
								//^29EE:16B0
								bp0a = 0;
								break;
							case 1:
								//^29EE:16AE
								//^29EE:16B0
								bp0a = 0;
								break;
							case 2:
								//^29EE:1688
								bp0a += bp0c;
								//^29EE:168E
								bp0a += _4976_3fa8[((bp0a -3) << 2) + ((si.Dir() - ww) & 3)];
								//^29EE:16AC
								break;
							case 3:
								//^29EE:16B0
								bp0a = 0;
								break;
						}
					}
					else {
						//^29EE:16B0
						bp0a = 0;
					}
					//^29EE:16B5
					DRAW_CHIP_OF_MAGIC_MAP(
						bp2a, 
						bp0a, 
						xx,
						di,
						_4976_3fa4[(si.Dir() - ww) & 3],
						bp6c
						);
				}
			}
			//^29EE:16E1
		}
		//^29EE:16F2
		for (si = bp1c; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
			//^29EE:16F7
			if (true
				&& si.DBType() == dbCloud
				&& QUERY_DUNGEON_MAP_CHIP_PICT(
					QUERY_CLS1_FROM_RECORD(si), 
					QUERY_CLS2_FROM_RECORD(si), 
					&bp2a, bp6c) != 0
			) {
				//^29EE:1729
				DRAW_CHIP_OF_MAGIC_MAP(
					bp2a,
					Bit16u(glbGameTick & 1) +1,
					xx,
					di,
					RAND02(),
					bp6c
					);
			}
			//^29EE:174D
		}
		//^29EE:175B
		Bit16u bp1e;
		i16 bp10;
		i16 bp12;
		if (bp4c.w0 == 0 && bp20 == 0) {
			//^29EE:176D
			bp10 = ss;
			bp12 = tt;
			Bit8u *bp26 = NULL;
			//^29EE:1783
			bp0a = glbCurrentMapIndex;
			//^29EE:1786
			while (bp0a == glbCurrentMapIndex && bp0a != -1) {
				//^29EE:1788
				bp0a = LOCATE_OTHER_LEVEL(glbCurrentMapIndex, 0, &ss, &tt, &bp26);
				//^29EE:17A5
			}
			//^29EE:17B7
			if (bp0a >= 0 && bp0a != glbCurrentMapIndex) {
				//^29EE:17C6
				if ((glbMapTileValue[bp0a][ss][tt] >> 5) == 0) {
					//^29EE:17EC
					bp20 = 1;
					bp1e = glbCurrentMapIndex;
					//^29EE:17F7
					CHANGE_CURRENT_MAP_TO(bp0a);
					//^29EE:1800
					SUMMARIZE_STONE_ROOM(&bp4c, ww, ss, tt);
					//^29EE:1816
					goto _0dcd;
				}
			}
		}
		//^29EE:1819
		if (bp20 != 0) {
			//^29EE:181F
			ss = bp10;
			tt = bp12;
			//^29EE:182B
			CHANGE_CURRENT_MAP_TO(bp1e);
			//^29EE:1834
			SUMMARIZE_STONE_ROOM(&bp4c, ww, ss, tt);
		}
		//^29EE:184A
		bp0a = LOCATE_OTHER_LEVEL(glbCurrentMapIndex, -1, &ss, &tt, NULL);
		//^29EE:1869
		if (bp0a >= 0) {
			//^29EE:186D
			bp0a = glbMapTileValue[bp0a][ss][tt];
			//^29EE:188F
			if ((bp0a >> 5) == ttPit && (bp0a & 8) != 0) {
				//^29EE:189D
				DRAW_CHIP_OF_MAGIC_MAP(bp2e, 18, xx, di, 0, bp5c);
			}
		}
	}
	//^29EE:18B7
	return;
}


//^29EE:1DA5
void SkWinCore::DRAW_MAJIC_MAP(ObjectID recordLink)
{
	//^29EE:1DA5
	ENTER(30);
	//^29EE:1DAB
	glbHoldedContainerType = QUERY_CLS2_FROM_RECORD(recordLink);
	glbMagicalMapFlags |= 0x0090;
	//^29EE:1DBD
	Container *bp04 = GET_ADDRESS_OF_RECORD9(recordLink);
	//^29EE:1DCC
	Bit16u bp0e = bp04->b5_5_7();
	//^29EE:1DDC
	Missile *bp08;
	if (bp0e != 3) {
		//^29EE:1DE1
		glbMagicalMapFlags |= 0x0800;
		//^29EE:1DE7
		bp0e = bp04->b5_5_7();
		//^29EE:1DF4
		if (bp0e != 0) {
			//^29EE:1DF8
			bp08 = GET_MISSILE_REF_OF_MINION(bp04->GetContainedObject(), recordLink);
			//^29EE:1E0C
			if (bp08 != NULL) {
				//^29EE:1E10
				glbMagicalMapFlags |= 0x0020;
			}
			//^29EE:1E18
			else if ((glbMagicalMapFlags & 0x0020) != 0) {
				//^29EE:1E20
				glbMagicalMapFlags &= 0xfbdf;
			}
		}
	}
	//^29EE:1E26
	if ((glbMagicalMapFlags & 0x0400) == 0) {
		//^29EE:1E2E
		DRAW_ICON_PICT_ENTRY(0x14, glbHoldedContainerType, 0x10, &_4976_3f6c, 92, -1);
		//^29EE:1E46
		for (Bit16u si = 0; si < _4976_53a4; si++) {
			//^29EE:1E4A
			DRAW_CMD_SLOT(si, 0);
			//^29EE:1E53
		}
		//^29EE:1E5A
		_29ee_1d03(0);
		//^29EE:1E61
		glbMagicalMapFlags |= 0x0400;
	}
	//^29EE:1E67
	Bit16u bp10 = glbPlayerPosX;
	Bit16u di = glbPlayerPosY;
	Bit16u bp12 = glbPlayerMap;
	//^29EE:1E77
	i16 bp16 = 0; // shift-y
	i16 bp14 = 0; // shift-x
	//^29EE:1E7F
	if (bp0e != 0 && bp0e != 3) {
		//^29EE:1E91
		if (bp04->GetDestMap() != MAXMAPS -1) {
			//^29EE:1EA3
			glbMagicalMapFlags |= 0x0040;
		}
		//^29EE:1EA9
		if (bp0e == 1) {
			//^29EE:1EB2
			if (bp08 == NULL) {
				//^29EE:1EBA
				glbMagicalMapFlags &= 0xffbf;
			}
			else {
				//^29EE:1EC3
				bp10 = bp08->GetX();
				di = bp08->GetY();
				bp12 = bp08->GetMap();
				//^29EE:1EE9
				Creature *bp0c = GET_ADDRESS_OF_RECORD4(bp04->GetContainedObject());
				//^29EE:1EFC
				i16 si = QUERY_CREATURE_5x5_POS(bp0c, (glbPlayerDir + _4976_3fc2[bp0c->b15_0_1()]) & 3);
				//^29EE:1F28
				bp14 = si % 5;
				//^29EE:1F31
				bp16 = si / 5;
				//^29EE:1F39
				if (bp14 > 2) {
					//^29EE:1F3F
					si = (glbPlayerDir +1) & 3;
					//^29EE:1F48
					bp10 += glbXAxisDelta[si];
					//^29EE:1F53
					di += glbYAxisDelta[si];
				}
				//^29EE:1F5B
				if (bp16 > 2) {
					//^29EE:1F61
					si = (glbPlayerDir +2) & 3;
					//^29EE:1F6B
					bp10 += glbXAxisDelta[si];
					//^29EE:1F76
					di += glbYAxisDelta[si];
				}
				//^29EE:1F7E
				bp14 = _4976_3fb8[bp14] * (((glbMagicMapInterlineX << 1) + glbMagicMapWidth +1) / 5);
				//^29EE:1FA2
				bp16 = _4976_3fb8[bp16] * (((glbMagicMapInterlineY << 1) + glbMagicMapHeight +1) / 5);
			}
		}
		//^29EE:1FC8
		else if ((glbMagicalMapFlags & 0x0040) != 0) {
			//^29EE:1FD0
			bp10 = bp04->GetDestX();
			//^29EE:1FDD
			di = bp04->GetDestY();
			//^29EE:1FE9
			bp12 = bp04->GetDestMap();
		}
	}
	//^29EE:1FF6
	_29ee_1946(recordLink, bp10, di, bp12, glbPlayerDir, bp14, bp16, glbMagicalMapFlags);
	//^29EE:2015
	if ((glbMagicalMapFlags & 0x0100) != 0 || (glbMagicalMapFlags & 0x0200) == 0) {
		//^29EE:2025
		SRECT bp1e;
		DRAW_GRAY_OVERLAY(&_4976_3f6c, QUERY_EXPANDED_RECT(99, &bp1e), 0);
	}
	//^29EE:2044
	return;
}

//^29EE:078B
void SkWinCore::DRAW_PLAYER_ATTACK_DIR()
{
	//^29EE:078B
	ENTER(346);
	//^29EE:0791
	_29ee_00a3(0);
	//^29EE:0798
	DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_GRAPHICSSET, glbSquadInterfaceMapGfxSet, GDAT_GFXSET_SQUAD_SINGLE, &_4976_3f6c, 93, -1);	// gfx = 0xF6
	//^29EE:07B1
	Bit8u *bp04 = ALLOC_PICT_BUFF(_4976_0118, _4976_011a, afDefault, 4);
	//^29EE:07CD
	i16 bp06 = 0;
	i16 bp08 = 0;
	//^29EE:07D3
	SRECT bp10;
	if (QUERY_BLIT_RECT(bp04, &bp10, 94, &bp06, &bp08, -1) != 0) {
		//^29EE:07FB
		if (glbGlobalSpellEffects.AuraOfSpeed != 0) {
			//^29EE:0802
			Bit16u si = RAND02();
			//^29EE:0809
			if (si != 0) {
				//^29EE:080D
				bp10.y -= 2;
				//^29EE:0813
				si = RAND02();
				//^29EE:081A
				if (si != 0) {
					//^29EE:081E
					bp10.x -= 2;
				}
			}
		}
		//^29EE:0824
		Bit8u bp015a[16];
		Bit16u di;
		QUERY_GDAT_SQUAD_ICON(bp04, Bit8u(di = glbChampionIndex -1), bp015a);
		//^29EE:083F
		DRAW_ICON_PICT_BUFF(bp04, &_4976_3f6c, &bp10, bp06, bp08, 12, 0, bp015a);
		//^29EE:0866
		if (glbChampionSquad[di].enchantmentPower != 0) {
			//^29EE:0879
			ExtendedPicture bp014a;
			QUERY_PICST_IT(QUERY_GDAT_SUMMARY_IMAGE(&bp014a, 0x01, 0x07, glbChampionSquad[di].enchantmentAura));
			//^29EE:08A2
			bp014a.w4 |= 0x0010;
			bp014a.width >>= 2;
			//^29EE:08AD
			bp014a.w14 = bp014a.width * ((glbChampionSquad[di].playerDir() +4 - glbPlayerDir) & 3);
			//^29EE:08D4
			bp014a.height >>= 2;
			//^29EE:08D9
			bp014a.w16 = bp014a.height * (Bit16u(glbGameTick) & 3);
			//^29EE:08EB
			_0b36_11c0(&bp014a, &_4976_3f6c, 94, 12);
		}
	}
	//^29EE:0901
	FREE_PICT_BUFF(bp04);
	//^29EE:090E
	DRAW_ICON_PICT_ENTRY(0x01, 0x04, 0x10, &_4976_3f6c, 96, -1);
	//^29EE:0924
	DRAW_ICON_PICT_ENTRY(0x01, 0x04, 0x12, &_4976_3f6c, 97, -1);
	//^29EE:093A
	return;
}


//^12B4:000D
void SkWinCore::HIGHLIGHT_ARROW_PANEL(Bit16u cls4, Bit16u rectno, Bit16u bright)
{
	//^12B4:000D
	ENTER(52);
	//^12B4:0012
	X16 si = rectno;
	_4976_4eb6 = U8(cls4);
	_4976_4ebc = si;
	_4976_4eb4 = bright;
	if (_4976_4eb4 != 0)
		cls4++;
	//^12B4:002C
	FIRE_HIDE_MOUSE_CURSOR();
	sk3f6c bp34;
	_0b36_0c52(&bp34, si, 1);
	FILL_ENTIRE_PICT(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp34.w0), glbPaletteT16[COLOR_BLACK]);
	DRAW_ICON_PICT_ENTRY(1, 3, U8(cls4), &bp34, si, -1);
	_0b36_0cbe(&bp34, 1);
	FIRE_SHOW_MOUSE_CURSOR();
	WAIT_SCREEN_REFRESH();
	//^12B4:008F
	return;
}

//^12B4:0092
void SkWinCore::_12b4_0092()
{
	//^12B4:0092
	ENTER(0);
	//^12B4:0095
	if (_4976_4eb4 != 0) {
		//^12B4:009C
		HIGHLIGHT_ARROW_PANEL(_4976_4eb6, _4976_4ebc, 0);
	}
	//^12B4:00AD
	return;
}


//------------------------------------------------------------------------------


//^00EB:03E7
void SkWinCore::IBMIO_FILL_HALFTONE_RECT(SRECT *rc) //#DS=04BF
{
	//^00EB:03E7
	ENTER(4);
	//^00EB:03ED
	LOADDS(0x0c48);
	//^00EB:03F3
	for (i16 si = rc->y; rc->y + rc->cy -1 >= si; si++) {
		//^00EB:03FC
		Bit8u *bp04 = &pbVram[si * 320 + rc->x];
		//^00EB:0416
		for (i16 di = rc->x; rc->x + rc->cx -1 >= di; bp04++, di++) {
			//^00EB:041E
			if (((di ^ si) & 1) == 0) {
				//^00EB:0429
				*bp04 = 0;
			}
			//^00EB:0430
		}
		//^00EB:0447
	}
	//^00EB:0458
#if UseAltic
	skwin.UpdateRect(rc->x, rc->y, rc->cx, rc->cy);
#endif
	return;
}

//^44C8:1D11
void SkWinCore::FIRE_FILL_HALFTONE_RECTV(SRECT *rc, Bit16u aa)
{
	//^44C8:1D11
	ENTER(0);
	//^44C8:1D14
	IBMIO_FILL_HALFTONE_RECT(rc) CALL_IBMIO;
	//^44C8:1D24
    return;
}

//^44C8:1DDA
void SkWinCore::FIRE_FILL_HALFTONE_RECTI(Bit16u rectno, Bit16u aa)
{
	//^44C8:1DDA
	ENTER(8);
	//^44C8:1DDE
	SRECT bp08;
	FIRE_FILL_HALFTONE_RECTV(QUERY_EXPANDED_RECT(rectno, &bp08), aa);
}

//------------------------------------------------------------------------------


//^0B36:129A
void SkWinCore::_0b36_129a(sk3f6c *ref, i16 xx, i16 yy, Bit8u clr1, Bit8u clr2, Bit8u *str)
{
	//^0B36:129A
	ENTER(4);
	//^0B36:129E
	i16 bp02;
	i16 bp04;
	if (QUERY_STR_METRICS(str, &bp02, &bp04) != 0) {
		//^0B36:12BA
		DRAW_STRING(
			reinterpret_cast<Bit8u *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(ref->w0)),
			ref->w0,
			ref->rc2.cx,
			xx - ref->rc2.x,
			yy - ref->rc2.y,
			clr1,
			clr2,
			str,
			8
			);
		//^0B36:12F5
		_0b36_0d67(
			ref, 
			ALLOC_TEMP_RECT(xx, yy, bp02, bp04)
			);
	}
	//^0B36:1318
	return;
}

//^0B36:0A3F
void SkWinCore::DRAW_PICST(ExtendedPicture *ref)
{
	//^0B36:0A3F
	ENTER(12);
	//^0B36:0A45
	if (ref->width <= 0 || ref->height <= 0)
		//^0B36:0A59
		return;

	//^0B36:0A5C
	U8 *bp04 = QUERY_PICT_BITS(ref);
	//^0B36:0A6E
	U16 iRectNo = ref->rectNo;	// U16 bp06
	//^0B36:0A78
	i16 bp08;
	i16 bp0a;
	if (iRectNo == 0xFFFF) {
		//^0B36:0A7D
		bp08 = ref->w32;
		bp0a = ref->w34;
	}
	else {
		//^0B36:0A8D
		if ((iRectNo & 0x8000) != 0 || ref->w28 != 0 || ref->w30 != 0) {
			//^0B36:0AA5
			iRectNo = iRectNo | 0x8000;
			bp08 = ref->w32 + ref->w28;
			bp0a = ref->w34 + ref->w30;
		}
		else {
			//^0B36:0AC6
			bp08 = ref->width;
			bp0a = ref->height;
		}
		//^0B36:0AD4
		if (QUERY_BLIT_RECT(bp04, &ref->rc36, iRectNo, &bp08, &bp0a, ref->w26) == 0) {
			//^0B36:0B07
			return;
		}
	}
	//^0B36:0B0A
	bp08 += ref->w14;
	bp0a += ref->w16;
	i16 si = READ_I16(bp04,-4);
	i16 di = ref->rc36.cx +bp08;
	//^0B36:0B2E
	if (si > di && (ref->mirrorFlip & 0x0001) != 0) {
		//^0B36:0B3A
		si -= di;	
	}
	else {
		//^0B36:0B3E
		si = 0;
	}
	//^0B36:0B40

	//if ((ref->w50 & 0x0001) != 0)
	if ((ref->mirrorFlip & DRAW_FLAG_FLIP_1) != 0)
		//^0B36:0B4B
		bp08 = 0;
	//^0B36:0B50
	bp08 += si;
	//^0B36:0B53
	si = READ_I16(bp04,-2);
	//^0B36:0B5A
	di = ref->rc36.cy +bp0a;
	//^0B36:0B66
	//if (si > di && (ref->w50 & 0x0002) != 0) {
	if (si > di && (ref->mirrorFlip & DRAW_FLAG_FLIP_2) != 0) {
		//^0B36:0B72
		si -= di;
	}
	else {
		//^0B36:0B76
		si = 0;
	}
	//^0B36:0B78
	if ((ref->mirrorFlip & DRAW_FLAG_FLIP_2) != 0)	// (ref->w50 & 0x0002) != 0)
		//^0B36:0B83
		bp0a = 0;
	//^0B36:0B88
	bp0a += si;
	//^0B36:0B8B
	U16 bp0c;
	if (ref->pb44 == _4976_4964) {
		//^0B36:0BA2
		si = glbScreenWidth;
		di = glbScreenHeight;
		//^0B36:0BAA
		bp0c = 8;
	}
	else {
		//^0B36:0BB1
		si = READ_I16(ref->pb44,-4);
		//^0B36:0BBC
		di = READ_I16(ref->pb44,-2);
		//^0B36:0BC7
		bp0c = READ_I16(ref->pb44,-6);
	}
	//^0B36:0BD5
	if (ref->colorKeyPassThrough != -2) {
		//^0B36:0BDF
		FIRE_BLIT_PICTURE(
			bp04,								// *src
			ref->pb44,							// *dst
			&ref->rc36,							// SRECT *rc
			bp08,								// srcx
			bp0a,								// srcy
			READ_I16(bp04,-4),					// srcPitch
			si,									// dstPitch
			ref->colorKeyPassThrough,			// colorkey
			ref->mirrorFlip,					// mirrorFlip
			READ_I16(bp04,-6),					// srcBpp
			bp0c,								// dstBpp
			(ref->w56 == 0) ? NULL : ref->b58	// local pal
			);
	}
	//^0B36:0C3D
	ref->w32 = bp08;
	ref->w34 = bp0a;
	//^0B36:0C4E
	return;
}

//^0B36:1688
//void SkWinCore::DRAW_STATIC_PIC(Bit8u cls1, Bit8u cls2, Bit8u cls4, Bit16u rectno, i16 colorkey)
void SkWinCore::DRAW_STATIC_PIC(Bit8u iCategory, Bit8u iItemNo, Bit8u iEntry, Bit16u rectno, i16 colorkey)
{
	// draw an statical image such as inventory plate.

	//^0B36:1688
	ENTER(314);
	//^0B36:168C
	ExtendedPicture bp013a;
	QUERY_GDAT_SUMMARY_IMAGE(&bp013a, iCategory, iItemNo, iEntry);
	//^0B36:16A5
	bp013a.colorKeyPassThrough = colorkey;
	bp013a.rectNo = rectno;
	//^0B36:16B3
	bp013a.pb44 = _4976_4c16;
	//^0B36:16C2
	bp013a.w28 = 0;
	bp013a.w30 = 0;
	//^0B36:16CE
	DRAW_PICST(QUERY_PICST_IT(&bp013a));
	//^0B36:16E2
	return;
}

//^2E62:064A
// SPX: _2e62_064a renamed DRAW_CHARSHEET_OPTION_ICON
void SkWinCore::DRAW_CHARSHEET_OPTION_ICON(Bit8u cls4, Bit16u rectno, Bit16u zz)
{
	//^2E62:064A
	ENTER(0);
	//^2E62:064D
	if ((_4976_581a & zz) != 0)
		//^2E62:0656
		cls4++;
	//^2E62:0659
	DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, cls4, rectno, -1);
	//^2E62:066E
	return;
}

//^2E62:00A3
void SkWinCore::DRAW_PLAYER_3STAT_PANE(Bit16u player, Bit16u xx)
{
	//^2E62:00A3
	ENTER(2);
	//^2E62:00A8
	U16 si = player;
	//^2E62:00AB
	if (_4976_3ff0.w0 != 0xffff)
		//^2E62:00B0
		return;
	//^2E62:00B2
	// SPX: Choose between 0 (normal champion panel) or 1 (dead champion panel)
	Bit8u bp01 = (glbChampionSquad[si].curHP() == 0) ? 1 : (((si +1) == glbChampionInventory) ? 9 : 0);
	//^2E62:00DB
	_0b36_0c52(&_4976_3ff0, si +161, xx);
	//^2E62:00F0
	DRAW_ICON_PICT_ENTRY(
		0x01,
		0x02,
		bp01,
		&_4976_3ff0,
		si + 161,
		-1
		);
	//^2E62:010C
	if (xx == 0)
		//^2E62:0112
		_4976_3ff0.w10 = 0;
	//^2E62:0118
	return;
}

//^2E62:061D
// SPX: _2e62_061d renamed DRAW_CHAMPION_PICTURE
void SkWinCore::DRAW_CHAMPION_PICTURE(Bit16u player)
{
	//^2E62:061D
	ENTER(0);
	//^2E62:0620
	DRAW_ICON_PICT_ENTRY(
		GDAT_CATEGORY_CHAMPIONS,	// 0x16
		glbChampionSquad[player].HeroType(),
		0x00,
        &_4976_3ff0,
		player +173,
		-1
		);
	//^2E62:0648
	if (bDM1PortraitsActivated)	 // SPX attempt to draw savegame portraits
	{
		i16 bp0c = 0;
		i16 bp0a = 0;
		Bit8u *bp04 = xDM1PortraitsData[player];
		Bit16u rectno = player + 173;
		sk3f6c* ss = &_4976_3ff0;
		i16 colorkey = -1;
		SRECT bp14;
		SRECT *bp08 = QUERY_BLIT_RECT(bp04, &bp14, rectno, &bp0a, &bp0c, -1);
		/*DRAW_ICON_PICT_BUFF(
			bp04,
			ss,
			bp08,
			bp0a,
			bp0c,
			colorkey,
			0,
			QUERY_GDAT_IMAGE_LOCALPAL(GDAT_CATEGORY_CHAMPIONS, 0, 0)
			);*/
		//IBMIO_BLIT_TO_SCREEN_8TO8BPP(
		//	bp04, __vram, bp08, 0, 0, 32, 240, -1);
	}


	return;
}

//^2E62:011B
void SkWinCore::DRAW_PLAYER_3STAT_HEALTH_BAR(U16 player)
{
	//^2E62:011B
	ENTER(32);
	//^2E62:0121
	SRECT bp0c;
	_0b36_0d67(
		&_4976_3ff0,
		QUERY_EXPANDED_RECT(player +185, &bp0c)
		);
	//^2E62:0143
	Champion *bp04 = &glbChampionSquad[player];
	//^2E62:0154
	i16 bp20[3][2];
	bp20[0][0] = bp04->curHP();
	bp20[0][1] = bp04->maxHP();
	bp20[1][0] = bp04->curStamina();
	bp20[1][1] = bp04->maxStamina();
	bp20[2][0] = bp04->curMP();
	bp20[2][1] = max_value(bp04->maxMP(), bp04->curMP());	// SPX Max because of MANA modifier
	//^2E62:018C
	U16 di = player +193;
	//^2E62:0194
	for (i16 si = 0; si < 3; si++, di += 4) {
		//^2E62:0199
		if (bp20[si][1] != 0) {
			//^	
			if (SCALE_RECT(
				di,
				&bp0c,
				10000,
				(bp20[si][0] * i32(10000)) / bp20[si][1]
				) != NULL
			){
				//^2E62:01F2
				SRECT bp14;
				COPY_MEMORY(&bp0c, &bp14, 8);
				//^2E62:0208
				bp14.x += _4976_0114;
				bp14.y += _4976_0116;
				//^2E62:0214
				FILL_RECT_SUMMARY(
					&_4976_3ff0,
					&bp14,
					glbPaletteT16[COLOR_BLACK]
				);
#if DM2_EXTENDED_MODE == 1
				FILL_RECT_SUMMARY(
					&_4976_3ff0,
					&bp0c,
					QUERY_3STAT_BAR_COLOR(player, glbPaletteT16[glbChampionColor[player]])
					);
#else
				//^2E62:022F
				FILL_RECT_SUMMARY(
					&_4976_3ff0,
					&bp0c,
					glbPaletteT16[glbChampionColor[player]]
					);
#endif
			}
		}
		//^2E62:0255
	}
	//^2E62:0261
	return;
}

//^2E62:0572
void SkWinCore::DRAW_PLAYER_DAMAGE(Bit16u player)
{
	//^2E62:0572
	ENTER(0);
	//^2E62:0576
	U16 si = player;
	//^2E62:0579
	DRAW_ICON_PICT_ENTRY(
		0x01,
		0x02,
		0x03,
		&_4976_3ff0,
		si +177,
		10
		);
	//^2E62:0593
	DRAW_SIMPLE_STR(
		&_4976_3ff0,
		si +177,
		glbPaletteT16[COLOR_WHITE],
		glbPaletteT16[COLOR_RED],
		FMT_NUM(glbChampionSquad[si].damageSuffered, 0, 3)
		);
	//^2E62:05D1
	return;
}

//^2E62:02EA

void SkWinCore::DRAW_CUR_MAX_HMS(U16 rectno, i16 curVal, i16 maxVal)
{
// SPX: original code below
	if (!SkCodeParam::bUseDM2ExtendedMode)
	{	
		ATLASSERT(curVal >= 0 && curVal <= 999);
		ATLASSERT(maxVal >= 0 && maxVal <= 999);

		//^2E62:02EA
		ENTER(8);
		//^2E62:02EE
		U8 bp08[8];	// 3 + slash + 3 + eol
		SK_STRCPY(bp08, FMT_NUM(curVal, 1, 3)); // format cur
		//^2E62:030B
		SK_STRCAT(bp08, strSlash); // add slash
		//^2E62:031C
		SK_STRCAT(bp08, FMT_NUM(maxVal, 1, 3)); // format max
		//^2E62:0339
		DRAW_LOCAL_TEXT(rectno, glbPaletteT16[COLOR_LIGHTER_GRAY], glbPaletteT16[COLOR_DARK_GRAY] | 0x4000, bp08);
		//^2E62:035E
		return;
	}
// SPX: modified function one to handle 4 char string in basic stats
	ATLASSERT(curVal >= 0 && curVal <= 9999);
	ATLASSERT(maxVal >= 0 && maxVal <= 9999);
	ENTER(8);
	U8 bp08[12];	// 4 + slash + 4 + eol
	SK_STRCPY(bp08, FMT_NUM(curVal, 1, 4)); // format cur
	SK_STRCAT(bp08, strSlash); // add slash
	SK_STRCAT(bp08, FMT_NUM(maxVal, 1, 4)); // format max
	DRAW_LOCAL_TEXT(rectno, glbPaletteT16[COLOR_LIGHTER_GRAY], glbPaletteT16[COLOR_DARK_GRAY] | 0x4000, bp08);
	return;
}

//^2E62:0360
void SkWinCore::DRAW_PLAYER_3STAT_TEXT(Champion *ref)
{
	//^2E62:0360
	ENTER(0);
	//^2E62:0363
	DRAW_CUR_MAX_HMS(550, ref->curHP(), ref->maxHP());
	//^2E62:0378
	DRAW_CUR_MAX_HMS(
		551,
		ref->curStamina() / 10,
		ref->maxStamina() / 10
		);
	//^2E62:039E
	DRAW_CUR_MAX_HMS(
		552,
		ref->curMP(),
		ref->maxMP()
		);
	//^2E62:03B3
	return;
}


//^2E62:05D4
// SPX: _2e62_05d4 renamed DRAW_EYE_MOUTH_COLORED_RECTANGLE
void SkWinCore::DRAW_EYE_MOUTH_COLORED_RECTANGLE(Bit8u cls4, Bit16u rectno)
{
	//^2E62:05D4
	ENTER(8);
	//^2E62:05D8
	SRECT bp08;
	_2405_011f(rectno, &bp08);
	//^2E62:05E8
	DRAW_DIALOGUE_PARTS_PICT(
		QUERY_GDAT_IMAGE_ENTRY_BUFF(0x01, 0x02, cls4),
        &bp08,
		12,
		QUERY_GDAT_IMAGE_LOCALPAL(0x01, 0x02, cls4)
		);
	//^2E62:061B
	return;
}

//^24A5:105B
// SPX: _24a5_105b renamed DRAW_CRYOCELL_LEVER
void SkWinCore::DRAW_CRYOCELL_LEVER(Bit16u leverIsOn)
{
	//^24A5:105B
	ENTER(0);
	//^24A5:105E
	// SPX: 0xFA image of lever up (waiting user to pull down) / 0xFB image of lever down : champion has been selected
	DRAW_STATIC_PIC(GDAT_CATEGORY_WALL_GFX, GDAT_WALL_ORNATE__CRYOCELL, (leverIsOn != 0) ? 0xfb : 0xfa, 0x1ee, -1);
	if (leverIsOn != 0) {
		//^24A5:1082
		CHANGE_VIEWPORT_TO_INVENTORY(0);
		// SPX: If lever is down, play the open cryocell sound
		QUEUE_NOISE_GEN1(GDAT_CATEGORY_WALL_GFX, GDAT_WALL_ORNATE__CRYOCELL, 0xfb, 0xff, 0xc8, glbPlayerPosX, glbPlayerPosY, 0);
	}
	else {
		//^24A5:10AB
		glbInventorySubpanel = 7;
	}
	//^24A5:10B1
	return;
}

//^24A5:0E82
void SkWinCore::DRAW_POWER_STAT_BAR(i16 curval, U16 rectno, U16 color, i16 vv, U16 ww)
{
	// color
	//   0 to  15: glbPaletteT16[color]
	// 256 to 511: pure 256

	//^24A5:0E82
	ENTER(24);
	//^24A5:0E88
	i16 di = curval;
	//^24A5:0E8B
	//i16 si = (di < -512) ? 8 : ((di < 0) ? 11 : zz);
#if DM2_EXTENDED_MODE == 1
	i16 si = (di < FOOD_WARNING) ? QUERY_FOOD_WATER_BAR_COLOR(0x0A, COLOR_RED) : ((di < 0) ? QUERY_FOOD_WATER_BAR_COLOR(0x09, COLOR_YELLOW) : color);
#else
	i16 si = (di < FOOD_WARNING) ? COLOR_RED : ((di < 0) ? COLOR_YELLOW : color);
#endif
	//^24A5:0EA2
	di -= vv;
	//^24A5:0EA5
	SRECT bp08;
	if (SCALE_RECT(
		rectno,
		&bp08,
		//(i32(10000) * di) / (2048 - vv),
		(i32(10000) * di) / (FOOD_MAX - vv),
		10000
		) != NULL
	) {
		//^24A5:0EE1
		SRECT bp18;
		SRECT bp10;
		if (ww != 0) {
			//^24A5:0EE7
			QUERY_EXPANDED_RECT(rectno, &bp10);
			//^24A5:0EF7
			COPY_MEMORY(&bp10, &bp18, 8);
			//^24A5:0F0D
			if ((bp08.x +bp08.cx -1) == (bp10.x +bp10.cx -1)) {
				//^24A5:0F1F
				ww = 0;
			}
			else {
				//^24A5:0F26
				bp10.cx = (bp10.x +bp10.cx -1) -(bp08.x +bp08.cx -1);
				//^24A5:0F39
				bp10.x = bp08.x +bp08.cx +0;
			}
		}
		else {
			//^24A5:0F47
			COPY_MEMORY(&bp08, &bp18, 8);
		}
		//^24A5:0F5D
		bp18.x += _4976_0142;
		bp18.y += _4976_0142;
		//^24A5:0F66
		FIRE_FILL_BACKBUFF_RECT(&bp18, glbPaletteT16[COLOR_BLACK]);
		//^24A5:0F7D
#if DM2_EXTENDED_MODE == 1
		FIRE_FILL_BACKBUFF_RECT(&bp08, ((si & 0x0100) == 0x0000) ? glbPaletteT16[si] : (si & 0x00FF));
#else
		FIRE_FILL_BACKBUFF_RECT(&bp08, glbPaletteT16[si]);
#endif
		//^24A5:0F94
		if (ww != 0) {
			//^24A5:0F9A
			FIRE_FILL_BACKBUFF_RECT(&bp10, glbPaletteT16[ww]);
		}
	}
	//^24A5:0FB4
	return;
}

#if DM2_EXTENDED_MODE == 1
// kkdf2: ext for emetar
i16 SkWinCore::QUERY_FOOD_WATER_BAR_COLOR(U8 cls4, i16 def_color) {
	RawEntry *bp04 = QUERY_GDAT_ENTRYPTR(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0, dtWordValue, cls4);
	if (bp04 != NULL) {
		return 256 + bp04->data;
	}
	return def_color;
}

// kkdf2: ext for emetar
i16 SkWinCore::QUERY_3STAT_BAR_COLOR(U8 cls4, i16 def_color) {
	RawEntry *bp04 = QUERY_GDAT_ENTRYPTR(GDAT_CATEGORY_INTERFACE_GENERAL, GDAT_INTERFACE_CLASS_CHAMPION_FACING, dtWordValue, cls4);
	if (bp04 != NULL) {
		return bp04->data;
	}
	return def_color;
}
#endif

//^24A5:0FB8
// SPX: _24a5_0fb8 renamed DRAW_FOOD_WATER_POISON_PANEL
void SkWinCore::DRAW_FOOD_WATER_POISON_PANEL()
{
	//^24A5:0FB8
	ENTER(4);
	//^24A5:0FBC
	Champion *pChampion = &glbChampionTable[glbChampionInventory];
	//^24A5:0FCD
	glbInventorySubpanel = 1;
	//^24A5:0FD3
	DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, GDAT_INTERFACE_CHAR_FOOD_WATER_PANEL, 494, -1);
#if DM2_EXTENDED_MODE == 1
	DRAW_POWER_STAT_BAR(pChampion->curFood(), 496, QUERY_FOOD_WATER_BAR_COLOR(0x06, COLOR_BROWN), -1024, 0);
	DRAW_POWER_STAT_BAR(pChampion->curWater(), 497, QUERY_FOOD_WATER_BAR_COLOR(0x07, COLOR_BLUE), -1024, 0);
#else
	//^24A5:0FE6
	DRAW_POWER_STAT_BAR(pChampion->curFood(), 496, COLOR_BROWN, -1024, 0);
	//^24A5:0FFE
	DRAW_POWER_STAT_BAR(pChampion->curWater(), 497, COLOR_BLUE, -1024, 0);
#endif
	//^24A5:1016
	DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, GDAT_INTERFACE_FOOD_TEXT, 500, 12);	// food
	//^24A5:1029
	DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, GDAT_INTERFACE_WATER_TEXT, 501, 12);	// water
	//^24A5:103C
	if (pChampion->PoisonValue != 0) {
		//^24A5:1046
		DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, GDAT_INTERFACE_POISON_TEXT, 502, 12); // poison
		
		// TODO SPX: In PC-DM2, the POISON value has its power bar.
		//	But it requires a rectno for it, which does not exists in PC9821 version ?!
		//if (bUseSuperMode)
		//	DRAW_POWER_STAT_BAR(pChampion->PoisonValue, 497, COLOR_RED, -1024, 0);
	}
#if (DM2_EXTENDED_MODE == 1)
	if (pChampion->PlagueValue != 0) 
	{
		DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, GDAT_INTERFACE_PLAGUED_TEXT, 502, 12); // plagued
	}
#endif
	//^24A5:1059
	return;
}

//^24A5:000F
void SkWinCore::DRAW_GUIDED_STR(const U8 *ref)
{
	// drawtext. draw str for item survey. increment y by text height automatically.

	//^24A5:000F
	ENTER(136);
	//^24A5:0014
	if (*ref == 12) {
		//^24A5:001D
		ref++;
		//^24A5:0020
		QUERY_TOPLEFT_OF_RECT(556, &_4976_52d8, &_4976_52da);
	}
	//^24A5:0033
	if (*ref == 0)
		//^24A5:003C
		return;
	//^24A5:003F
	if (QUERY_MBCS_PRESENCE(ref) != 0) {
		//^24A5:0050
		DRAW_VP_STR(_4976_52d8, _4976_52da, glbPaletteT16[COLOR_LIGHTER_GRAY], ref);
		//^24A5:0071
		_4976_52da += _4976_013a;
		//^24A5:0078
		return;
	}
	//^24A5:007B
	U8 bp0088[128];
	SK_STRCPY(bp0088, ref);
	//^24A5:008F
	U8 *bp04 = bp0088;
	//^24A5:0099
	for (i16 si = 0; *bp04 != 0; ) {
		//^24A5:009E
		U8 *bp08;
		if (SK_STRLEN(bp04) > 18) {
			//^24A5:00B0
			bp08 = bp04 +17;
			//^24A5:00BF
			while (*bp08 != ' ') bp08--;
			//^24A5:00CD
			*bp08 = 0;
			//^24A5:00D1
			si = 1;
		}
		else {
			//^24A5:00D6
			bp08 = NULL;
		}
		//^24A5:00E0
		DRAW_VP_STR(_4976_52d8, _4976_52da, glbPaletteT16[COLOR_LIGHTER_GRAY], bp04);
		//^24A5:0101
		_4976_52da += glbPanelStatsYDelta;
		//^24A5:0108
		if (si != 0 && bp08 != NULL) {
			//^24A5:0114
			si = 0;
			//^24A5:0116
			bp08++;
			//^24A5:0119
			bp04 = bp08;
		}
		else {
			//^24A5:0127
			*bp04 = 0;
		}
		//^24A5:012E
	}
	//^24A5:013A
	return;
}

//^24A5:0732
void SkWinCore::_24a5_0732(i16 xx, i16 yy, U8 *str)
{
	//^24A5:0732
	ENTER(208);
	//^24A5:0737
	U16 bp04 = 0;
	U16 bp02;
	if (QUERY_STR_METRICS(str, &bp02, &bp04) == 0)
		return;
	//^24A5:075B
	U8 bp00d0[200];
	if (QUERY_MBCS_PRESENCE(str) == 0) {
		//^24A5:076C
		X16 si = 0;
		bp00d0[0] = 2;
		bp00d0[1] = 0x20;
		U8 bp05;
		while ((bp05 = str[si]) != 0) {
			//^24A5:077A
			if (bp05 >= 0x41 && bp05 <= 0x5a) {
				//^24A5:0786
				bp05 = bp05 -0x40;
			}
			else if (bp05 >= 0x7b) {
				//^24A5:0793
				bp05 = bp05 -0x60;
			}
			//^24A5:079B
			bp00d0[si +2] = bp05;
			si++;
			//^24A5:07AC
		}
		//^24A5:07B9
		bp00d0[si +2] = 0;
		str = bp00d0;
	}
	//^24A5:07CD
	DRAW_VP_STR(xx -(bp02 >> 1), yy, glbPaletteT16[COLOR_BLACK], str);
	//^24A5:07F3
	return;
}

//^24A5:07F6
void SkWinCore::DRAW_SCROLL_TEXT(ObjectID rl)
{
	//^24A5:07F6
	ENTER(418);
	//^24A5:07FC
	U8 bp00da[200];
	U8 bp01a2[200];
	U8 *bp04;
	ATLASSERT(rl.DBType() == dbScroll);
	QUERY_MESSAGE_TEXT(bp04 = bp00da, rl, 0x8002);
	glbInventorySubpanel = 5;
	DRAW_STATIC_PIC(0x07, 0, 0x01, 0x1ee, -1);
	DRAW_STATIC_PIC(0x12, 0, 0x10, 0x1ee, 12);
	X16 si;
	for (si = 0; *bp04 != 0; bp04++) {
		//^24A5:0848
		if (*bp04 == 0xa)
			si++;
		//^24A5:0852
	}
	//^24A5:085E
	if (bp04[-1] != 0xa) {
		si++;
	}
	else if (bp04[-2] == 0xa) {
		si--;
	}
	//^24A5:0873
	i16 bp08;
	i16 bp0a;
	QUERY_TOPLEFT_OF_RECT(0x230, &bp08, &bp0a);
	SRECT bp12;
	QUERY_EXPANDED_RECT(0x1ee, &bp12);
	X16 di;
	if (QUERY_MBCS_PRESENCE(bp00da) == 0) {
		//^24A5:08A9
		di = glbPanelStatsYDelta;
		bp0a -= ((di * si -_4976_0124 -1) >> 1) - _4976_011e;
	}
	else {
		//^24A5:08BD
		di = _4976_013a;
		bp0a -= ((di * si - _4976_0136 -1) >> 1) - _4976_0130;
	}
	//^24A5:08D2
	U16 bp06 = 0;
	while (bp00da[bp06] != 0) {
		//^24A5:08D9
		_3929_04e2_DRAW_TEXT_STRINGS(bp00da, bp01a2, &bp06, bp12.cx);
		_24a5_0732(bp08, bp0a, bp01a2);
		bp0a += di;
		if (bp00da[bp06] == 0xa)
			bp06++;
		//^24A5:091D
	}
	//^24A5:092C
	return;
}

//^2405:02E8
// SPX: _2405_02e8 renamed DRAW_ITEM_ICON
void SkWinCore::DRAW_ITEM_ICON(ObjectID recordLink, i16 xx, U16 yy, U16 zz, U16 ww)
{
	//^2405:02E8
	ENTER(20);
	//^2405:02EE
	i16 si = xx;
	//^2405:02F1
	U8 bp0b;
	U8 bp0c;
	U8 bp09;
	if (recordLink == OBJECT_NULL) {
		//^2405:02F7
		bp0b = 7;
		bp0c = 0;
		//^2405:02FF
		bp09 = _4976_3b74[si].b2 +U8(yy);
	}
	else {
		//^2405:030D
		bp0b = QUERY_CLS1_FROM_RECORD(recordLink);
		bp0c = QUERY_CLS2_FROM_RECORD(recordLink);
		//^2405:0325
		bp09 = (si < 8)
			? GET_ITEM_ICON_ANIM_FRAME(recordLink, si & 1, 1)
			: GET_ITEM_ICON_ANIM_FRAME(recordLink, si -8, 1);
	}
	//^2405:0348
	U16 di = _4976_3b74[si].w0;
	//^2405:0351
	if (si < 0x26) {
		//^2405:0359
		SRECT bp14;
		QUERY_EXPANDED_RECT(di, &bp14);
		//^2405:0367
		U8 *bp04;
		if (si < 8) {
			//^2405:036C
			bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x01, 0x02, 0x00);
			//^2405:0380
			DRAW_ICON_PICT_BUFF(
				bp04,
				&_4976_3ff0,
				&bp14,
				bp14.x - _4976_3ff0.rc2.x,
				bp14.y - _4976_3ff0.rc2.y,
				-1,
				0,
                QUERY_GDAT_IMAGE_LOCALPAL(0x01, 0x02, 0x00)
				);
		}
		else {
			//^2405:03BD
			bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x07, 0x00, 0x00);
			//^2405:03D1
			DRAW_DIALOGUE_PICT(
				bp04,
				_4976_4c16,
				&bp14,
				bp14.x,
				bp14.y,
				-1,
				QUERY_GDAT_IMAGE_LOCALPAL(0x07, 0x00, 0x00)
				);
		}
	}
	//^2405:0404
	if (ww != 0 && si < 14) {
		//^2405:0415
		SRECT bp14;
		_2405_011f(di, &bp14);
		//^2405:0422
		U8 bp0a = (zz != 0)
			? 6
			: (yy != 0)
				? 5
				: 4;
		//^2405:043E
		U8 *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x01, 0x02, bp0a);
		//^2405:0454
		U8 *bp08 = QUERY_GDAT_IMAGE_LOCALPAL(0x01, 0x02, bp0a);
		//^2405:046A
		if (si < 8) {
			//^2405:046F
			DRAW_ICON_PICT_BUFF(
				bp04,
				&_4976_3ff0,
				&bp14,
				0,
				0,
				12,
				0,
				bp08
				);
		}
		//^2405:0496
		DRAW_DIALOGUE_PARTS_PICT(
			bp04,
			&bp14,
			12,
			bp08
			);
	}
	//^2405:04B1
	if (bp09 == 0xff)
		//^2405:04B5
		return;
	//^2405:04B7
	if (si < 8) {
		//^2405:04BC
		DRAW_ICON_PICT_ENTRY(
			bp0b,
			bp0c,
			bp09,
			&_4976_3ff0,
			di,
			12
			);
	}
	else {
		//^2405:04D9
		DRAW_STATIC_PIC(
			bp0b,
			bp0c,
			bp09,
			di,
			12
			);
	}
	//^2405:04F0
	return;
}

//^24A5:0AD2
// SPX: _24a5_0ad2 renamed DRAW_CONTAINER_SURVEY
void SkWinCore::DRAW_CONTAINER_SURVEY(Container *ref)
{
	//^24A5:0AD2
	ENTER(0);
	//^24A5:0AD7
	U16 si = 0;
	//^24A5:0AD9
	//^24A5:0AE0
	for (ObjectID di = ref->GetContainedObject(); di != OBJECT_END_MARKER; di = GET_NEXT_RECORD_LINK(di), si++) {
		//^24A5:0AE2
		if (si >= 8)	// 8 = MAX items in container (CHEST / BAG / QUIVER)
			//^24A5:0AE5
			break;
		//^24A5:0AE7
		DRAW_ITEM_ICON(di, si +47, 0, 0, 0);
		//^24A5:0AFC
	}
	//^24A5:0B0B
	return;
}



//^24A5:0B0F
// SPX: _24a5_0b0f renamed DRAW_ITEM_STATS_BAR
void SkWinCore::DRAW_ITEM_STATS_BAR(U16 rectno, i16 curVal, i16 maxVal, U8 chr, U16 color)
{
	//^24A5:0B0F
	ENTER(10);
	//^24A5:0B14
	i16 si = curVal;
	//^24A5:0B17
	SRECT bp08;
	// bp08: x
	// bp06: y
	// bp04: cx
	// bp02: cy
	if (QUERY_EXPANDED_RECT(rectno, &bp08) == 0)
		//^24A5:0B2B
		//^24A5:0C32
		return;
	//^24A5:0B2E
	si = (i32(si) << 11) / maxVal;
	//^24A5:0B47
	DRAW_POWER_STAT_BAR(si, rectno, color, 0, 1);
	//^24A5:0B5A
	U8 bp0a[2];
	bp0a[1] = 0;
	bp0a[0] = chr;	// Will be the rune associated with the power bar, i.e. KU for weapon
	//^24A5:0B64
	bp08.cy -= 2;
	//^24A5:0B68
	DRAW_STRONG_TEXT(
		_4976_4c16,
		-1,
		_4976_00f6,
		bp08.x -9,
		bp08.y +bp08.cy,
		glbPaletteT16[color],
		glbPaletteT16[COLOR_BLACK] | 0x4000,
		bp0a
		);
	//^24A5:0BA8
	bp0a[0] = RUNE_LO;	// 0x60
	//^24A5:0BAC
	DRAW_STRONG_TEXT(
		_4976_4c16,
		-1,
		_4976_00f6,
		bp08.x +3,
		bp08.y +bp08.cy -1,
        glbPaletteT16[COLOR_WHITE],
		glbPaletteT16[COLOR_BLACK] | 0x4000,
		bp0a
		);
	//^24A5:0BEA
	bp0a[0] = RUNE_MON;	// 0x65
	//^24A5:0BEE
	DRAW_STRONG_TEXT(
		_4976_4c16,
		-1,
		_4976_00f6,
		bp08.x +bp08.cx -1 - _4976_011e -1,
		bp08.y +bp08.cy -1,
		glbPaletteT16[COLOR_WHITE],
		glbPaletteT16[COLOR_BLACK] | 0x4000,
		bp0a
		);
	//^24A5:0C32
	return;
}

//^24A5:0C35
U16 SkWinCore::DRAW_ITEM_SURVEY(ObjectID recordLink, Bit16u xx)
{
	// You click eye icon with the item which you wanna know about, in item inventory.

	//^24A5:0C35
	ENTER(148);
	//^24A5:0C3B
	ObjectID si = recordLink;
	//^24A5:0C3E
	ItemStatBarPos bp0e = glbItemStatBarPos;
	//^24A5:0C4F
	if (si == OBJECT_NULL)
		//^24A5:0C54
		return 0;
	//^24A5:0C59
	GenericRecord *_bp04 = GET_ADDRESS_OF_RECORD(si);
	//^24A5:0C66
	DRAW_GUIDED_STR(_4976_3d7d);
	//^24A5:0C70
	U16 bp08 = si.DBType();
	//^24A5:0C7B
	if (bp08 == dbScroll) {
		//^24A5:0C80
		DRAW_SCROLL_TEXT(si);
		//^24A5:0C86
		//^24A5:0E7B
		return 1;
	}
	//^24A5:0C89
	glbInventorySubpanel = 3;
	//^24A5:0C8F
	if (xx == 0) {
		//^24A5:0C95
		//^24A5:0C54
		return 0;
	}
	//^24A5:0C97
	DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, 0x01, 0x01ee, -1);
	//^24A5:0CAA
	U8 bp09;
	U8 bp0a;
	if (QUERY_GDAT_ENTRY_IF_LOADABLE(
		bp09 = QUERY_CLS1_FROM_RECORD(si),
		bp0a = QUERY_CLS2_FROM_RECORD(si),
		dtImage,
		0x11
		) != 0
	) {
		//^24A5:0CD0
		DRAW_STATIC_PIC(bp09, bp0a, 0x11, 0x01ee, 12);
	}
	//^24A5:0CE7
	DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, 0x05, 0x01f8, 12);
	//^24A5:0CFA
	DRAW_VP_RC_STR(0x1fa, glbPaletteT16[COLOR_LIGHTER_GRAY], GET_ITEM_NAME(si));
	//^24A5:0D19
	DRAW_ITEM_ICON(si, 0x2e, 0, 0, 0);
	//^24A5:0D2A
	U16 bp06 = QUERY_ITEM_WEIGHT(si);
	//^24A5:0D34
	glbItemWeightKg = bp06 / 10;
	//^24A5:0D3E
	glbItemWeightDg = bp06 % 10;
	//^24A5:0D49
	U8 bp0094[128];
    DRAW_GUIDED_STR(QUERY_GDAT_TEXT(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, 0x1f, bp0094));			
	//^24A5:0D65
	if (bp08 == dbContainer) {
		//^24A5:0D6B
		if (IS_CONTAINER_CHEST(si) != 0) {
			//^24A5:0D76
			DRAW_CONTAINER_SURVEY(_bp04->castToContainer());
			//^24A5:0D82
			//^24A5:0E7B
			return 1;
		}
		//^24A5:0D85
		if (IS_CONTAINER_MONEYBOX(si) == 0) {
			//^24A5:0D90
			//^24A5:0E7B
			return 1;
		}
		//^24A5:0D93
		MONEY_BOX_SURVEY(si);	//
		//^24A5:0D99
		return 1;
	}
	//^24A5:0D9C
	U16 bp10 = 0;
	//^24A5:0DA1
	if (_2759_0155(si) != 0) {
		//^24A5:0DAC
		i16 di = ADD_ITEM_CHARGE(si, 0);
		//^24A5:0DB8
		if (di != 0) {
			//^24A5:0DBC
			DRAW_ITEM_STATS_BAR(
				bp0e.pos[bp10++],
				di,
				GET_MAX_CHARGE(si),
				RUNE_GOR,
				COLOR_CYAN
				);
		}
	}
//--- SPX: Info about the power bars displayed for items
// For weapons, power is displayed either using STRENGTH value (KU symbol) or THROW value (ROS symbol) for missiles such as arrows and shooters such as bows.
//	Clothings and armours also display a KU symbol for their armour strength.
//	A potion have a NETA symbol for its power level.
//	These bars are displayed in BROWN color.

//	An item with charges will have a CYAN GOR bar with the number of remaining charges


	//^24A5:0DE0
	U8 rune;		// bp11 This holds the rune to be displayed along the power bar
	U16 max;	//bp14
	i16 value;		//di
	switch (si.DBType()) {
		case dbPotion: // 8
			{
				Potion *bp04 = _bp04->castToPotion();

				//^24A5:0DFA
				rune = RUNE_NETA;
				//^24A5:0DFE
				value = bp04->PotionPower();
				//^24A5:0E0A
				max = 0x00ff;
				//^24A5:0E0F
				break;
			}
		case dbWeapon: // 5
			{
				//^24A5:0E11
				rune = RUNE_KU;
				max = 0x0050;
				//^24A5:0E1A
				value = QUERY_GDAT_DBSPEC_WORD_VALUE(si, GDAT_ITEM_WEAPON_MELEE_STRENGTH);		//SPX: 0x08 Attack strength
				//^24A5:0E26
				if (value == 0) {
					//^24A5:0E2A
					value = QUERY_GDAT_DBSPEC_WORD_VALUE(si, GDAT_ITEM_WEAPON_THROW_STRENGTH);	//SPX: 0x09 Throw strength
					//^24A5:0E36
					rune = RUNE_ROS;
					max = 0x0064;
				}
				//^24A5:0E3F
				break;
			}
		case dbCloth: // 6
			{
				//^24A5:0E41
				rune = RUNE_KU;
				if (SkCodeParam::bUseDM2ExtendedMode)
					rune = RUNE_YA;
				//^24A5:0E45
				value = QUERY_GDAT_DBSPEC_WORD_VALUE(si, GDAT_ITEM_STATS_ARMOR_CLASS) & 0xff;	//SPX: 0x0B Armor value
				//^24A5:0E54
				max = 0x00c8;
				break;	// SPX: fixed here this missing break
			}
		default:
			{
				//^24A5:0DF7
				//^24A5:0E7B
				value = 0;
				//return 1;
			}
	}
	//^24A5:0E59
	// SPX: Original behaviour
	if (!SkCodeParam::bUseDM2ExtendedMode) {
		if (value != 0) {
			//^24A5:0E5D
			DRAW_ITEM_STATS_BAR(bp0e.pos[bp10], value, max, rune, COLOR_BROWN);
		}
	}
	// SPX: little modif to have different colors
	else if (SkCodeParam::bUseDM2ExtendedMode && value != 0)
	{
		U16 color = COLOR_BROWN;
		switch (rune)
		{
			case RUNE_KU:	// Hand to hand weapon
				color = COLOR_RED;
				break;
			case RUNE_ROS:	// Projectile weapon
				color = COLOR_ORANGE;
				break;
			case RUNE_NETA:	// Potion
				color = COLOR_BLUE;
				break;
		}
		DRAW_ITEM_STATS_BAR(bp0e.pos[bp10], value, max, rune, color);
	}

//--- NEW CODE -------------------------------------
	// SPX: Added code to display hidden stats such as magical bonuses
	if (SkCodeParam::bUseSuperInfoEye)
	{
		char str[32];
		Bit16u	yy = 95;
		i32	itemValue = 0;
		i16 statValue = 0;
		U8 strMoneyValue[11];
		U8* pstr;

		if (!SkCodeParam::bUseDM2ExtendedMode)
			return 1;

		printf("Number of charges = %d\n", ADD_ITEM_CHARGE(si, 0));

		// Money value
		memset(strMoneyValue, 0, 10);
		statValue = QUERY_GDAT_DBSPEC_WORD_VALUE(si, GDAT_ITEM_STATS_MONEY_VALUE);
		itemValue += statValue;
		statValue = QUERY_GDAT_DBSPEC_WORD_VALUE(si, GDAT_ITEM_BONUS_MONEY_PER_CHARGE);
		itemValue += (statValue * ADD_ITEM_CHARGE(si, 0));
		SK_LTOA10(itemValue, strMoneyValue);
		pstr = strMoneyValue;
		while (*pstr != 0)
			pstr++;
		*pstr = '$';
		
		DRAW_VP_RC_STR(
			569,
			glbPaletteT16[COLOR_YELLOW],
			strMoneyValue
			);

		if ( bp10 != 0 || value != 0)
			yy += 2;
		yy += ((bp10 + (value != 0 ? 1 : 0))*12);
		
		// Bonus display
		for (U8 bonus = GDAT_ITEM_STATS_FOOD_VALUE; bonus <= GDAT_ITEM_STATS_MISSILE_SPELL; bonus++)
		{
			if (bonus == GDAT_ITEM_STATS_EQUIP_FLAGS
				|| bonus == GDAT_ITEM_STATS_ANIM_FLAGS
				|| bonus == GDAT_ITEM_STATS_ARMOR_CLASS
				|| bonus == GDAT_ITEM_WEAPON_PROJECTILE_FLAG
				|| bonus == GDAT_ITEM_WEAPON_MELEE_STRENGTH)
				continue;
			value = QUERY_GDAT_DBSPEC_WORD_VALUE(si, bonus);
			if (value != 0)
			{
				char sign = '+';
				if (value < 0)
				{
					sign = '-';
					value = -value;
				}
				sprintf(str, "%c%d %s", sign, value, getStatBonusName(bonus));
				DRAW_STRONG_TEXT(
					_4976_4c16,
					-1,
					_4976_00f6,
					100,
					yy,
					glbPaletteT16[COLOR_WHITE],
					glbPaletteT16[COLOR_BLACK] | 0x4000,
					(Bit8u*) str
					);
				yy += 7;
			}
		}
	} // SPX: End of hidden stats block

	//^24A5:0E7B
	return 1;
}


//^2E62:03B5
U16 SkWinCore::_2e62_03b5(Bit16u player, Bit16u itemNo, Bit16u yy)
{
	//^2E62:03B5
	ENTER(8);
	//^2E62:03BB
	U16 bp04 = 0;
	U16 bp06 = 0;
	//^2E62:03C5
	U16 bp08 = yy;
	//^2E62:03CB
	U16 si;
	if (player +1 != glbChampionInventory) {
		//^2E62:03D5
		if (itemNo > INVENTORY_HAND_LAST || player +1 == glbNextChampionNumber)
			//^2E62:03E5
			//^2E62:056C
			return 0;
		//^2E62:03E8
		si = (player << 1) + itemNo;
	}
	else {
		//^2E62:03F2
		si = itemNo +8;
	}
	//^2E62:03FA
	ObjectID di = glbChampionSquad[player].Possess(itemNo);
	//^2E62:040F
	if (itemNo <= INVENTORY_HAND_LAST) {	// <= 1
		//^2E62:0415
		if (player +1 == glbChampionIndex && itemNo == glbSelectedHandAction)
			//^2E62:0428
			bp04 = 1;
		//^2E62:042D
		if ((!_4976_3b74[si].b5_0_0()) != (!bp04)) {
			//^2E62:044B
			bp08 = 1;
			//^2E62:0450
			_4976_3b74[si].b5 ^= 1;
		}

	}
	//^2E62:045A
	if (itemNo <= INVENTORY_BODYPART_LAST) {	// <= 5
		//^2E62:0460
		if ((glbChampionSquad[player].bodyFlag & (1 << itemNo)) != 0)
			//^2E62:0478
			bp06 = 1;
		//^2E62:047D
		if ((!_4976_3b74[si].b5_1_1()) != (!bp06)) {
			//^2E62:049B
			bp08 = 1;
			//^2E62:04A0
			_4976_3b74[si].b5 ^= 2;
		}
	}
	//^2E62:04AA
	if (di != OBJECT_NULL) {
		//^2E62:04AF
		U8 bp01;
		if ((QUERY_GDAT_DBSPEC_WORD_VALUE(di, 0x00) & 0x8000) != 0) {
			//^2E62:04BE
			bp01 = GET_ITEM_ICON_ANIM_FRAME(di, itemNo, 1);
		}
		else {
			//^2E62:04C2
			bp01 = GET_ITEM_ICON_ANIM_FRAME(di, itemNo, 0);
		}
        //^2E62:04D3
		if (_4976_3b74[si].b3 != bp01) {
			//^2E62:04E1
			yy = 1;
			//^2E62:04E6
			_4976_3b74[si].b3 = bp01;
		}
		//^2E62:04F2
		U8 bp02 = QUERY_CLS2_FROM_RECORD(di);
		//^2E62:04FC
		if (bp02 != _4976_3b74[si].b4) {
			//^2E62:0507
			yy = 1;
			//^2E62:050C
			_4976_3b74[si].b4 = bp02;
		}
	}
	//^2E62:0515
	if (yy != 0 || bp08 != 0 || _4976_3b74[si].w6 != di) {
		//^2E62:052C
		if (di == 0xffff || si < 8)
			//^2E62:0536
			bp08 = 1;
		//^2E62:053B
		if (si < 8) {
			//^2E62:0540
			DRAW_PLAYER_3STAT_PANE(player, 0);
		}
		//^2E62:054B
		_4976_3b74[si].w6 = di;
		//^2E62:0554
		DRAW_ITEM_ICON(di, si, bp06, bp04, bp08);
		//^2E62:0567
		return 1;
	}
	//^2E62:056C
	return 0;
}

//^3929:0C37
void SkWinCore::DRAW_LOCAL_TEXT(Bit16u rectno, Bit16u clr1, Bit16u clr2, Bit8u *str)
{
	// draw local positioned text.
	// a) health, mana and stamina: "770/770"

	//^3929:0C37
	ENTER(12);
	//^3929:0C3B
	i16 bp02;
	i16 bp04;
	if (QUERY_STR_METRICS(str, &bp02, &bp04) != 0) {
		//^3929:0C56
		SRECT bp0c;
		if (QUERY_BLIT_RECT(NULL, &bp0c, rectno, &bp02, &bp04, -1) != 0)
		{
			//^3929:0C7A
			// SPX: Note, the bp0c.y does not match exactly the character sheet. (for static texts)
			// The charsheet image can be changed.
			bp0c.y -= 1;
			DRAW_STRONG_TEXT(
				_4976_4c16,
				-1,
				_4976_00f6,
				bp0c.x,
				bp0c.y +bp0c.cy,
				clr1,
				clr2,
				str
				);
		}
	}
	//^3929:0CA6
	return;
}


//^0CD5:0018
void SkWinCore::FILL_STR(Bit8u *buff, Bit16u count, Bit8u value, Bit16u delta)
{
	//^0CD5:0018
	//^0CD5:0021
	for (Bit16u si = 0, di = 0; di < count; si += delta, di++) {
		buff[si] = value;
	}
}

//^2E62:0265
U8 *SkWinCore::FMT_NUM(U16 value, U16 clean, U16 keta)
{
	//^2E62:0265
	ENTER(6);
	//^2E62:026A
	U16 si = value;
	//^2E62:026D
	if (clean != 0) {
		//^2E62:0273
		FILL_STR(glbStrBufferNumber, 4, ' ', 1);
	}
	//^2E62:0285
	U8 *bp04 = &glbStrBufferNumber[4];
	//^2E62:028D
	*bp04 = 0;
	//^2E62:0294
	if (si == 0) {
		//^2E62:0298
		*(--bp04) = '0';
		//^2E62:02A2
	}
	else {
		//^2E62:02C5
		U16 bp06;
		while ((bp06 = si) != 0) {
			//^2E62:02A4
			bp04--;
			//^2E62:02A7
			si = si / 10;
			//^2E62:02B2
			*bp04 = U8(bp06) + '0' -(si * 10);
			//^2E62:02C5
		};
	}
	//^2E62:02CE
	if (clean != 0) {
		//^2E62:02D4
		return &glbStrBufferNumber[4 - keta];
	}
	//^2E62:02E1
	return bp04;
}

//^0CAF:00A4
U8 *SkWinCore::SK_STRCPY(U8 *strTo, const U8 *strFrom) {
	return reinterpret_cast<U8 *>(strcpy(reinterpret_cast<char *>(strTo), reinterpret_cast<const char *>(strFrom)));
}

//^0CAF:0032
U8 *SkWinCore::SK_STRCAT(U8 *strTo, const U8 *strFrom) {
	return reinterpret_cast<U8 *>(strcat(reinterpret_cast<char *>(strTo), reinterpret_cast<const char *>(strFrom)));
}

//^24A5:1532
// SPX: _24a5_1532 replaced by DRAW_SKILL_PANEL
void SkWinCore::DRAW_SKILL_PANEL()
{
	//^24A5:1532
	ENTER(144);
	//^24A5:1538
	U16 di = glbChampionInventory -1;
	//^24A5:153E
	Champion *bp04 = &glbChampionSquad[di];
	//^24A5:154C
	glbInventorySubpanel = 2;
	//^24A5:1552
	DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, 0x01, 494, -1);	// blank stone panel
	//^24A5:1565
	i16 bp0c;
	i16 bp10;
	QUERY_TOPLEFT_OF_RECT(557, &bp0c, &bp10);
	//^24A5:157A
	U8 bp0090[128];
	U16 bp06;

	if (SkCodeParam::bUseSuperInfoEye && SkCodeParam::bUseDM2ExtendedMode)
	{
		glbPanelStatsYDelta = 6;		// Instead of 7, it is Y delta between each attribute text string
	}

	for (bp06 = 0; bp06 <= 3; bp06++) {
		//^24A5:1582
		i16 si = min_value(SkLvlArchmaster, QUERY_PLAYER_SKILL_LV(di, bp06, 1));	// skill level caped 16
		//^24A5:159C
		if (si != SkLvlNone) {	// (si != 1)
			//^24A5:15A1
			_4976_52e4 = bp06;
			//^24A5:15A7
			QUERY_GDAT_TEXT(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00
				, tSkillLevelNameGDAT2Entry[RCJ(15,si -SkLvlNeophyte)], bp0090);	// [RCJ(15,si -2)]
			//^24A5:15BE
			DRAW_VP_STR(bp0c, bp10, glbPaletteT16[(glbChampionMajorSkillsLevel[di][bp06] != 0) ? COLOR_LIGHT_GREEN : COLOR_LIGHTER_GRAY], bp0090);
			//^24A5:15F5
			glbChampionTable->lastName[9 +bp06] = glbChampionMajorSkillsLevel[di][bp06];
			//^24A5:1608
			bp10 += glbPanelStatsYDelta;
		}
		//^24A5:160E
	}

	//^24A5:161A
	i16 bp0e;
	QUERY_TOPLEFT_OF_RECT(559, &bp0e, &bp10);
	//^24A5:162F
	// SPX: Display attributes (STR, DEX, WIS, VIT, AF, AM)
	// SPX: The first is luck; the GDAT2 resource 07-00 20-00-00 (TXT) must be defined as "LUCK".
	U16 attributeStart = ATTRIBUTE_FIRST+1;	// Start at 1=STR
	if (SkCodeParam::bShowLuck || (SkCodeParam::bUseSuperInfoEye && SkCodeParam::bUseDM2ExtendedMode))
	{
		attributeStart = ATTRIBUTE_FIRST;	// Start at 0=LUCK
	}

	for (bp06 = attributeStart; bp06 <= ATTRIBUTE_LAST; bp06++) {
		//^24A5:1637
		DRAW_VP_STR(
			bp0c,
			bp10,
			glbPaletteT16[COLOR_LIGHTER_GRAY],
			QUERY_GDAT_TEXT(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, bp06 +0x20, bp0090)
			);
		//^24A5:166A
		U16 si = GET_PLAYER_ABILITY(bp04, bp06, 0);			// Current
		//^24A5:167F
		U16 bp0a = GET_PLAYER_ABILITY(bp04, bp06, 1);		// Max
		//^24A5:1695
		U16 bp08 = (si < bp0a) ? 8 : ((si > bp0a) ? 7 : 13);
		// SPX: 8 = red, 7 = green, 13 = grey
		//^24A5:16B2
		DRAW_VP_STR(
			bp0e,
			bp10,
			glbPaletteT16[bp08],
			FMT_NUM(si, 1, 3)
			);
		//^24A5:16DC
		//SK_STRCPY(bp0090, &_4976_3d7f);	// SPX: What's this? We do want to concat /
		SK_STRCPY(bp0090, strSlash);		// SPX: Fixed version with /
		//^24A5:16EE
		SK_STRCAT(bp0090, FMT_NUM(bp0a, 1, 3));
		//^24A5:170D
        DRAW_VP_STR(
			_4976_0126 * 3 +bp0e,
			bp10,
			glbPaletteT16[COLOR_LIGHTER_GRAY],
			bp0090
			);
		//^24A5:1737
		bp10 += glbPanelStatsYDelta;
		//^24A5:173D
	}
	//^24A5:1749
	return;
}


//^2E62:0670
void SkWinCore::REFRESH_PLAYER_STAT_DISP(i16 player)
{
	//^2E62:0670
	ENTER(164);
	//^2E62:0676
	Bit16u bp14 = 0;
	Bit16u bp16 = 0;
	Bit16u bp18 = 0;
	Bit16u bp1a = 0;
	Bit16u bp1c = 0;
	//^2E62:068F
	Champion *champion = &glbChampionSquad[player];	//*bp04
	//^2E62:06A0
	Bit16u si = champion->heroFlag;
	//^2E62:06A7
	Bit16u bp08 = (player +1 == glbChampionInventory) ? 1 : 0; // 1=draw entire inventory panel
	//^2E62:06BB
	if (bp08 != 0 && _4976_022c != 0)
		//^2E62:06C6
		si |= 0x8000;
	//^2E62:06CA
	if (glbChampionInventory != glbLastChampionInventory || (si & 0x8000) != 0) {
		//^2E62:06D9
		if (bp08 != 0) {
			//^2E62:06DF
			DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, 0x00, 0x04, -1);	// char sheet general screen
			//^2E62:06F1
			bp1c = 1;
			bp18 = 1;
			//^2E62:06FB
			glbLastChampionInventory = glbChampionInventory;
			//^2E62:0701
			bp14 = 15;
			//^2E62:0706
			si |= 0x7c00;
		}
		else {
			//^2E62:070C
			glbLastChampionInventory = 0;
		}
	}
	//^2E62:0714
	else if (bp08 != 0) {
		//^2E62:071A
		bp14 = _4976_581a ^ _4976_5804;
	}
	//^2E62:0724
	if (bp14 != 0) {
		//^2E62:072A
		if (glbNextChampionNumber == 0) {
			//^2E62:0731
			if ((bp14 & 1) != 0) {
				//^2E62:0738
				DRAW_CHARSHEET_OPTION_ICON(0x0013, 0x0267, 0x0001);	// 0x13: char name bar
			}
			//^2E62:074A
			if ((bp14 & 8) != 0) {
				//^2E62:0751
				DRAW_CHARSHEET_OPTION_ICON(0x000f, 0x0232, 0x0008);	// 0x0F: blue diskette
			}
			//^2E62:075F
			if ((bp14 & 4) != 0) {
				//^2E62:0766
				DRAW_CHARSHEET_OPTION_ICON(0x000d, 0x0234, 0x0004);	// 0x0D: Zzz
			}
		}
		//^2E62:0774
		if ((bp14 & 2) != 0) {
			//^2E62:077B
			DRAW_CHARSHEET_OPTION_ICON(0x000b, 0x0236, 0x0002);		// 0x0B: Cancel red cross

		}
		//^2E62:0789
		bp18 = 1;
		//^2E62:078E
		_4976_5804 = _4976_581a;
	}
	//^2E62:0794
	if ((si & 0x4000) != 0) {
		//^2E62:079A
		DRAW_PLAYER_3STAT_PANE(player, 1);
		//^2E62:07A5
		bp1a = 1;
		//^2E62:07AA
		if (champion->curHP() != 0) {
			//^2E62:07B4
			if (bp08 != 0) {
				//^2E62:07BA
				DRAW_CHAMPION_PICTURE(player);
				//^2E62:07C2
				si |= 0x0800;
			}
			else {
				//^2E62:07C8
				si |= 0x0c00;
			}
			//^2E62:07CC
			bp16 = 1;
		}
		else {
			//^2E62:07D3
			DRAW_NAME_STR(
				&_4976_3ff0,
				player +165,
				glbPaletteT16[COLOR_WHITE],
				glbPaletteT16[COLOR_DARK_GRAY] |0x4000,
				champion->firstName
				);
		}
	}
	//^2E62:0801
	if (champion->curHP() != 0) {
		//^2E62:080E
		if ((si & 0x0800) != 0) {	// Refresh stats bar
			//^2E62:0817
			DRAW_PLAYER_3STAT_PANE(player, 0);
			//^2E62:0822
			bp1a = 1;
			//^2E62:0827
			DRAW_PLAYER_3STAT_HEALTH_BAR(player);
			//^2E62:082F
			if (champion->timerIndex != TIMER_NONE) {
				//^2E62:0839
				DRAW_PLAYER_DAMAGE(player);
			}
			//^2E62:0841
			if (bp08 != 0) {
				//^2E62:084A
				SRECT bp24;
				QUERY_EXPANDED_RECT(549, &bp24);
				//^2E62:085A
				DRAW_DIALOGUE_PICT(
					QUERY_GDAT_IMAGE_ENTRY_BUFF(0x07, 0x00, 0x00),
					_4976_4c16,
					&bp24,
					bp24.x,
					bp24.y,
					-1,
					QUERY_GDAT_IMAGE_LOCALPAL(0x07, 0x00, 0x00)
					);
				//^2E62:0897
				DRAW_PLAYER_3STAT_TEXT(champion);
				//^2E62:08A3
				// SPX: bp0d holds the color number (rectangle?) around the mouth or eye.
				Bit8u colorRectangle; //bp0d;
#if (DM2_EXTENDED_MODE == 0)
				if (champion->curFood() < 0 || champion->curWater() < 0 || champion->PoisonValue != 0) {
#elif (DM2_EXTENDED_MODE == 1)
				if (champion->curFood() < 0 || champion->curWater() < 0 || champion->PoisonValue != 0 || champion->PlagueValue != 0) {
#endif
					//^2E62:08BB
					colorRectangle = 5; // i am hunger! or poisoned!
				}
				else {
					//^2E62:08C1
					colorRectangle = 4;
				}
				//^2E62:08C5
				// SPX: Display colored rectangle around mouth
				DRAW_EYE_MOUTH_COLORED_RECTANGLE(colorRectangle, 545);
				//^2E62:08D2
				colorRectangle = 4;
				//^2E62:08D6
				// SPX: review skills from STR to AM
				for (i16 di = ATTRIBUTE_STRENGTH; di <= ATTRIBUTE_LAST; di++) {	// for (i16 di = 1; di <= 6; di++)
					//^2E62:08DB
					if (champion->attributes[di][ATTRIBUTE_CURRENT] < champion->attributes[di][ATTRIBUTE_MAX]) {
						//^2E62:08F7
						colorRectangle = 5; // level gained!
						// SPX: bp0d = 5 changes color to red
						//^2E62:08FB
						break;
					}
					//^2E62:08FD
				}
				//^2E62:0903
				// SPX: Display colored rectangle around eye
				DRAW_EYE_MOUTH_COLORED_RECTANGLE(colorRectangle, 546);
				//^2E62:0910
				bp18 = 1;
			}
		}
		//^2E62:0915
		ObjectID bp12;
		if (bp08 != 0) {
			//^2E62:091E
			if (glbShowItemStats != 0) {
				//^2E62:0925
				bp12 = glbLeaderHandPossession.object;
			}
			//^2E62:092A
			else if (_4976_4bfe != 0) {
				//^2E62:0931
				bp12 = OBJECT_NULL;
			}
			else {
				//^2E62:0938
				bp12 = champion->Possess(INVENTORY_HAND_LEFT);
			}
			//^2E62:0940
			//^2E62:0943
			if (false
				|| _4976_3d30 != bp12 
				|| _4976_4bfe != _4976_5806 
				|| glbShowItemStats != _4976_5808
				|| (si & 0x2000) != 0
			) {
				//^2E62:0966
				DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, 0x25, 545, -1);	// Mouth
				//^2E62:0979
				DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, (glbShowItemStats != 0) ? 0x21 : 0x20, 546, -1); // Eye
				//^2E62:0998
				if (glbShowItemStats != 0) {
					//^2E62:099F
					if (bp12 == OBJECT_NULL) {
						//^2E62:09A5
						DRAW_SKILL_PANEL();
					}
					else {
						//^2E62:09AC
						DRAW_ITEM_SURVEY(bp12, 1);
					}
				}
				else {
					//^2E62:09BA
					if (glbNextChampionNumber != 0) {
						//^2E62:09C1
						if (_4976_4bfe == 0) {
							//^2E62:09CA
							DRAW_CRYOCELL_LEVER(0);
						}
						else {
							//^2E62:09C8
							goto _09e4;
						}
					}
					//^2E62:09D4
					else if (DRAW_ITEM_SURVEY(bp12, 0) == 0) {
						//^2E62:09E4
_09e4:
						DRAW_FOOD_WATER_POISON_PANEL();
					}
				}
				//^2E62:09E9
				_4976_3d30 = bp12;
				_4976_5808 = glbShowItemStats;
				_4976_5806 = _4976_4bfe;
				//^2E62:09FB
				goto _0a25;
			}
			else if (_4976_3d30 != OBJECT_NULL) {
				//^2E62:0A04
				if ((QUERY_GDAT_DBSPEC_WORD_VALUE(_4976_3d30, 0x00) & 0x0020) != 0) {
					//^2E62:0A16
					DRAW_ITEM_SURVEY(_4976_3d30, glbShowItemStats);
					//^2E62:0A25
_0a25:
					bp18 = 1;
				}
			}
			//^2E62:0A2A
			for (i16 bp0c = 0; bp0c < 30; bp0c++) {
				//^2E62:0A31
				bp18 |= _2e62_03b5(player, bp0c, bp1c);
				//^2E62:0A44
			}
		}
		else {
			//^2E62:0A4F
			for (i16 bp0c = 0; bp0c <= 1; bp0c++) {
				//^2E62:0A56
				bp1a |= _2e62_03b5(player, bp0c, bp16);
				//^2E62:0A69
			}
		}
		//^2E62:0A72
		U16 di;
		Bit8u bp00a4[128];
		if ((si & 0x0400) != 0) {
			//^2E62:0A7B
			di = glbPaletteT16[(player == glbChampionLeader && glbNextChampionNumber == 0) ? COLOR_ORANGE : COLOR_WHITE];
			if (SkCodeParam::bDM1Mode)
				di = glbPaletteT16[(player == glbChampionLeader && glbNextChampionNumber == 0) ? COLOR_YELLOW : COLOR_ORANGE];
			//^2E62:0AA0
			if (bp08 != 0) {
				//^2E62:0AA9
				SK_STRCPY(bp00a4, &champion->firstName[0]);
				//^2E62:0ABD
				U8 bp09 = champion->lastName[0];
				if (bp09 != '.' && bp09 != ';' && bp09 != '-') {
					//^2E62:0AD9
					SK_STRCAT(bp00a4, _4976_4756);
				}
				//^2E62:0AEF
				SK_STRCAT(bp00a4, &champion->firstName[8]);
				DRAW_LOCAL_TEXT(553, di, (glbNextChampionNumber != 0) ? glbPaletteT16[COLOR_DARK_GRAY] : 0x4000, bp00a4);
			}
			else {
				//^2E62:0B37
				DRAW_PLAYER_3STAT_PANE(player, 0);
				bp1a = 1;
				DRAW_NAME_STR(&_4976_3ff0, player +165, di, glbPaletteT16[COLOR_DARK_GRAY] | 0x4000, champion->firstName);
			}
		}
		//^2E62:0B6F
		if (bp08 != 0 && (si & 0x1000) != 0) {
			//^2E62:0B81
			SRECT bp24;
			QUERY_EXPANDED_RECT(554, &bp24);
			//^2E62:0B91
			DRAW_DIALOGUE_PICT(
				QUERY_GDAT_IMAGE_ENTRY_BUFF(0x07, 0x00, 0x00),
				_4976_4c16,
				&bp24,
				bp24.x,
				bp24.y,
				-1,
				QUERY_GDAT_IMAGE_LOCALPAL(0x07, 0x00, 0x00)
				);
			//^2E62:0BCE
			U16 bp10 = GET_PLAYER_WEIGHT(player);
			//^2E62:0BDA
			di = MAX_LOAD(champion);
			//^2E62:0BE9
			U16 bp06;
			if (di < bp10) {
				//^2E62:0BEE
				bp06 = 8;
			}
			else {
				//^2E62:0BF5
				bp06 = ((i32(bp10) << 3) > (i16(di) * i32(5))) ? 11 : 13;
			}
			//^2E62:0C2C
			_4976_52e0 = di = bp10 / 10;
			//^2E62:0C3B
			_4976_52fa = bp10 - di * 10;
			//^2E62:0C4B
			_4976_52f6 = MAX_LOAD(champion) / 10;
			//^2E62:0C62
            QUERY_GDAT_TEXT(0x07, 0x00, 0x2a, bp00a4);
			//^2E62:0C76
			DRAW_LOCAL_TEXT(555, glbPaletteT16[bp06], glbPaletteT16[COLOR_DARK_GRAY] | 0x4000, bp00a4);
			//^2E62:0C9E
			bp18 = 1;
		}
	}
	//^2E62:0CA3
	if (bp1a != 0) {
		//^2E62:0CA9
		_0b36_0cbe(&_4976_3ff0, 1);
	}
	//^2E62:0CB7
	if (bp18 != 0) {
		//^2E62:0CBD
		_4976_022c = 0;
		//^2E62:0CC3
 		CHANGE_VIEWPORT_TO_INVENTORY(0);
	}
	//^2E62:0CCB
	champion->heroFlag &= 0x03ff;	// 0x03FF
	//^2E62:0CD4
	return;
}

//^2405:04F4
// SPX: _2405_04f4 renamed DRAW_ITEM_IN_HAND
void SkWinCore::DRAW_ITEM_IN_HAND(LeaderPossession *ref)
{
	//^2405:04F4
	ENTER(8);
	//^2405:04F8
	U8 bp05 = QUERY_CLS1_FROM_RECORD(ref->object);
	//^2405:0507
	U8 bp06 = QUERY_CLS2_FROM_RECORD(ref->object);
	//^2405:0516
	U8 bp07 = GET_ITEM_ICON_ANIM_FRAME(ref->object, -1, 1);
	//^2405:052A
	U8 *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(bp05, bp06, bp07);
	//^2405:0541
	COPY_MEMORY(
		QUERY_GDAT_IMAGE_LOCALPAL(bp05, bp06, bp07),
		ref->b6,
		16
		);
	//^2405:056D
	FIRE_BLIT_PICTURE(
		bp04,
		ref->pb2,
        ALLOC_TEMP_ORIGIN_RECT(_4976_0106, _4976_0108),
		0,
		0,
		READ_I16(bp04,-4),
		_4976_0106,
		-1,
		0,
		4,
		4,
		NULL
		);
	//^2405:05B2
	return;
}



//^2E62:0CFA
void SkWinCore::_2e62_0cfa(U16 xx)
{
	//^2E62:0CFA
	ENTER(4);
	//^2E62:0CFF
	for (U16 si = 0; si < glbChampionsCount; si++) {
		//^2E62:0D03
		if (si +1 != glbChampionInventory) {
			//^2E62:0D0C
			REFRESH_PLAYER_STAT_DISP(si);
		}
		//^2E62:0D12
	}
	//^2E62:0D19
	if (glbChampionInventory != 0) {
		//^2E62:0D20
		U16 si = glbChampionInventory -1;
		//^2E62:0D26
		Champion *champion = &glbChampionSquad[si];
		//^2E62:0D34
		if (champion->enchantmentPower != 0 && champion->enchantmentAura >= ENCHANTMENT_AURA_FIRST && champion->enchantmentAura <= ENCHANTMENT_AURA_LAST) {
			//^2E62:0D4F
			champion->heroFlag |= (glbShowItemStats != 0 && glbLeaderHandPossession.object == OBJECT_NULL) ? CHAMPION_FLAG_3000 : CHAMPION_FLAG_1000;	// 0x3000 or 0x1000
		}
		//^2E62:0D6C
		REFRESH_PLAYER_STAT_DISP(glbChampionInventory -1);
	}
	//^2E62:0D76
	UPDATE_RIGHT_PANEL(xx);
	//^2E62:0D7F
	return;
}

//^443C:0004
void SkWinCore::_443c_0004(U8 *buffSrc, U8 *buffDst, U8 *localpal)
{
	//^443C:0004
	ENTER(20);
	//^443C:0009
	U8 *bp04 = ALLOC_PICT_BUFF(16, 16, afDefault, 4);
	//^443C:001F
	U8 bp14[16];
	for (i16 si = 0; si < 16; si++) {
		//^443C:0023
		bp14[si] = glbPaletteT16[COLOR_BLACK];
		//^443C:002D
	}
	//^443C:0033
	bp14[12] = localpal[12];
	//^443C:003D
	FILL_ENTIRE_PICT(buffDst, localpal[12]);
	//^443C:0052
	DRAW_DIALOGUE_PICT(buffSrc, buffDst, &_4976_4942, 0, 0, 12, bp14);
	//^443C:0075
	DRAW_DIALOGUE_PICT(buffSrc, buffDst, &_4976_494a, 0, 0, 12, localpal);
	//^443C:0099
	FREE_PICT_BUFF(bp04);
	//^443C:00A6
	return;
}

//^443C:0434
void SkWinCore::_443c_0434()
{
	//^443C:0434
	ENTER(4);
	//^443C:0438
	_4976_5dac = (glbLeaderHandPossession.object != OBJECT_NULL) ? 1 : 0;
	//^443C:0449
	if (_4976_5dac != 0) {
		//^443C:044D
		U8 *bp04 = ALLOC_PICT_BUFF(18, 18, afDefault, 8);
		//^443C:0463
		_443c_0004(glbLeaderHandPossession.pb2, bp04, glbLeaderHandPossession.b6);
		//^443C:047C
		IBMIO_SET_CURSOR_PATTERN(2, bp04, 8, 8, 18, 18, 8, NULL, glbLeaderHandPossession.b6[12]) INDIRECT_CALL;
		//^443C:04A3
		FREE_PICT_BUFF(bp04);
	}
	//^443C:04B0
	_443c_040e();
	//^443C:04B4
	return;
}




//^0B36:1757
// SPX: ressembles pretty much DRAW_STATIC_PIC (_0B36_1688) 
// SPX: _0B36_1757 renamed DRAW_TRANSPARENT_STATIC_PIC
void SkWinCore::DRAW_TRANSPARENT_STATIC_PIC(U8 cls1, U8 cls2, U8 cls4, U16 rectno, U16 colorkey)
{
	//^0B36:1757
	ENTER(314);
	//^0B36:175B
	ExtendedPicture bp013a;
	QUERY_GDAT_SUMMARY_IMAGE(&bp013a, cls1, cls2, cls4);
	//^0B36:1774
	bp013a.colorKeyPassThrough = colorkey;
	bp013a.rectNo = rectno;
	//^0B36:1782
	DRAW_PICST(QUERY_PICST_IT(&bp013a));
	//^0B36:1796
	return;
}


//^32CB:0F82
// _32cb_0f82 renamed _32cb_0f82_SHOP_GLASS
void SkWinCore::_32cb_0f82_SHOP_GLASS(Actuator *ref, U8 cls4, i16 bb, i16 cellPos, U16 horzStretch, U16 vertStretch, U16 rectno, U16 gg, U16 colorkey1)
{
	//^32CB:0F82
	ENTER(58);
	//^32CB:0F88
	Actuator *bp0c = NULL;
	//^32CB:0F92
	U16 bp26;
	for (ObjectID di = GET_TILE_RECORD_LINK(ref->Xcoord(), ref->Ycoord()); di != OBJECT_END_MARKER && (bp26 = di.DBType()) <= dbActuator; di = GET_NEXT_RECORD_LINK(di)) {
		//^32CB:0FB4
		if (bp26 == dbActuator) {
			//^32CB:0FBA
			bp0c = GET_ADDRESS_OF_ACTU(di);
			//^32CB:0FC7
			if (bp0c->ActuatorType() == ACTUATOR_FLOOR_TYPE__SHOP) // 0x30 -> 'Shop exhibition'
				//^32CB:0FD4
				break;
		}
		//^32CB:0FD6
	}
	//^32CB:0FF4
	i16 si = bp0c->ActuatorData();
	U16 bp24 = si;
	//^32CB:1006
	U8 bp11 = U8(ref->ActuatorData());
	//^32CB:1016
	if (ref->ActiveStatus() == 0 || ref->OnceOnlyActuator() != 0) {
		//^32CB:1032
		U16 bp14 = (ref->Delay() * 3) +16;
		//^32CB:104A
		U8 *bp04 = ALLOC_MEMORY_RAM(64, afDefault, 1024);
		//^32CB:1061
		if (QUERY_CREATURES_ITEM_MASK(bp11, U8(bp14), bp04, 0) != 0) {
			//^32CB:107F
			U16 *bp10 = reinterpret_cast<U16 *>(QUERY_GDAT_ENTRY_DATA_BUFF(GDAT_CATEGORY_WALL_GFX, cls4, dt08, GDAT_WALL_ORNATE__DATA_13));
			//^32CB:1097
			i16 bp28;
			if (bp10 != NULL) {
				//^32CB:109B
				bp28 = QUERY_GDAT_ENTRY_DATA_LENGTH(GDAT_CATEGORY_WALL_GFX, cls4, dt08, GDAT_WALL_ORNATE__DATA_13) >> 1;
				//^32CB:10B2
				for (si = 0; si < bp28; si++) {
					//^32CB:10B6
					if ((bp10[si] & 511) == bp24)
						//^32CB:10C8
						break;
				}
			}
			do {
				//^32CB:10D0
				si += (ref->OnceOnlyActuator() != 0 || _4976_5a96 < 0) ? 1 : -1;
				//^32CB:10F2
				if (bp10 != NULL) {
					//^32CB:10FA
					if (si < 0) {
						//^32CB:10FE
						si = bp28 -1;
					}
					//^32CB:1106
					else if (si >= bp28) {
						//^32CB:110B
						si = 0;
					}
					//^32CB:110D
					bp24 = bp10[si] & 511;
				}
				else {
					//^32CB:111E
					si &= 511;
					bp24 = si;
				}
				//^32CB:1124
				//^32CB:1127
			} while (false
				|| (bp04[bp24 >> 3] & (1 << (bp24 & 7))) == 0
				|| (true
					&& (QUERY_GDAT_ENTRY_DATA_INDEX(QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(bp24), GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(bp24), dtWordValue, 0) & 0x1000) != 0
					&& IS_DISTINCTIVE_ITEM_ON_ACTUATOR(ref, bp24, 1) == 0
					&& IS_DISTINCTIVE_ITEM_ON_ACTUATOR(bp0c, bp24, 0) == 0
					&& _2fcf_16ff(bp24) == 0
					)
				);
			//^32CB:11A2
			ref->OnceOnlyActuator(0);
		}
		//^32CB:11AA
		ref->ActiveStatus(1);
		//^32CB:11B2
		bp0c->ActiveStatus(1);
		//^32CB:11BA
		bp0c->ActuatorData(bp24);
		//^32CB:11CD
		DEALLOC_UPPER_MEMORY(64);
	}
	//^32CB:11D8
	U16 bp18 = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, cls4, dtImageOffset, GDAT_WALL_ORNATE__DATA_FD);	// 0xFD
	//^32CB:11EE
	QUERY_TEMP_PICST(0, horzStretch, vertStretch, i8(bp18 >> 8), i8(bp18), bb, rectno, gg, colorkey1, -1, QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(bp24),
		GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(bp24), 0x00);
	//^32CB:122E
	DRAW_TEMP_PICST();
	//^32CB:1232
	if (true
		&& IS_DISTINCTIVE_ITEM_ON_ACTUATOR(ref, bp24, 1) == 0
		&& IS_DISTINCTIVE_ITEM_ON_ACTUATOR(bp0c, bp24, 0) == 0
	) {
		//^32CB:1264
		U16 *bp10 = reinterpret_cast<U16 *>(QUERY_GDAT_ENTRY_DATA_BUFF(GDAT_CATEGORY_WALL_GFX, cls4, dt08, GDAT_WALL_ORNATE__DATA_13));
		//^32CB:127C
		if (bp10 != NULL) {
			//^32CB:1280
			i16 bp28 = QUERY_GDAT_ENTRY_DATA_LENGTH(GDAT_CATEGORY_WALL_GFX, cls4, dt08, GDAT_WALL_ORNATE__DATA_13) >> 1;
			//^32CB:1297
			for (si = 0; si < bp28; si++) {
				//^32CB:129B
				if ((bp10[si] & 511) == bp24)
					//^32CB:12AD
					break;
			}
			//^32CB:12B5
			ObjectID di;
			if ((bp10[si] & 0x8000) != 0 && (di = ALLOC_NEW_DBITEM_DEEPLY(bp24)) != OBJECT_NULL) {
				//^32CB:12D5
				APPEND_RECORD_TO(di, NULL, ref->Xcoord(), ref->Ycoord());
			}
		}
		else {
			//^32CB:12FD
			QUERY_TEMP_PICST(0, horzStretch, vertStretch, i8(bp18 >> 8), i8(bp18), bb, rectno, gg, colorkey1, -1, 9, cls4, 0x11); // sold out icon
			//^32CB:132F
			DRAW_TEMP_PICST();
		}
	}
	//^32CB:1333
	QUERY_TEMP_PICST(0, horzStretch, vertStretch, 0, 0, bb, rectno, gg, colorkey1, -1, 9, cls4, 0x0f); // shop panel nav board
	//^32CB:135C
	DRAW_TEMP_PICST();
	//^32CB:1360
	if (cellPos == 3) {
		//^32CB:1366
		i16 bp2a;
		i16 bp2c;
		QUERY_TOPLEFT_OF_RECT(rectno, &bp2a, &bp2c);
		//^32CB:137B
		U16 bp18 = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, cls4, dtImageOffset, 0xf2);
		//^32CB:1391
		bp2a += i8(bp18 >> 8);
		bp2c += i8(bp18);
		//^32CB:139F
		DRAW_TEXT_TO_BACKBUFF(bp2a, bp2c, QUERY_GDAT_ITEM_NAME(QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(bp24), GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(bp24)));
	}
	//^32CB:13C9
	if (cellPos == 3) {
		//^32CB:13D2
		U16 bp22 = _48ae_05ae_CREATURE(bp24, bp11, ref->ShopItemPoolNo(), 1, 1, -1); // price for buy?
		//^32CB:13F8
		U16 bp20 = _48ae_05ae_CREATURE(bp24, bp11, ref->ShopItemPoolNo(), 0, 1, -1); // price for sell?
		//^32CB:141E
		for (i16 bp16 = 0; bp16 < 2; bp16++) {
			//^32CB:1426
			U16 bp18 = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, cls4, dtImageOffset, (bp16 != 0) ? GDAT_WALL_ORNATE__DATA_F0 : GDAT_WALL_ORNATE__DATA_F1);
			//^32CB:1446
			i16 bp2a = i8(bp18 >> 8);
			i16 bp2c = i8(bp18);
			//^32CB:1454
			U16 bp1a = (bp16 != 0) ? bp22 : bp20;
			//^32CB:1465
			if (bp1a <= 0)
				//^32CB:1469
				continue;
			//^32CB:146C
			U8 bp3a[16];
			i16 bp1e;
			_48ae_0767_MONEY_VALUE(bp1a, 6, bp3a, &bp1e, bp16);
			//^32CB:1486
			if (bp1e <= 0)
				//^32CB:148C
				continue;
			//^32CB:148F
			U16 bp1c = ALLOC_TEMP_CACHE_INDEX();
			//^32CB:1497
			ALLOC_NEW_PICT(bp1c, bp1e << 3, 7, 4);
			//^32CB:14AD
			QUERY_TEMP_PICST(0, 64, 64, 0, 0, bb, -1, gg, -1, -1, 0x09, cls4, 0x10);
			//^32CB:14D2
			U8 *bp08 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp1c));
			//^32CB:14E1
			SRECT bp34;
			SET_ORIGIN_RECT(&bp34, 8, 7);
			//^32CB:14F2
			for (si = 0; si < bp1e; si++) {
				//^32CB:14F6
				DRAW_DIALOGUE_PICT(glbTempPicture.pb0, bp08, &bp34, bp3a[si] << 3, 0, -1, NULL);
				//^32CB:1522
				bp34.x += 8;
				//^32CB:1526
			}
			//^32CB:152C
			DRAW_DIALOGUE_PICT(bp08, _4976_4c16, QUERY_BLIT_RECT(bp08, &bp34, rectno|0x8000, &bp2a, &bp2c, gg), 
				0, 0, colorkey1, glbTempPicture.b58);
			//^32CB:1576
			FREE_TEMP_CACHE_INDEX(bp1c);
			//^32CB:157F
		}
	}
	//^32CB:158B
	return;
}

//^32CB:3F0D
// SPX: _32cb_3f0d renamed DRAW_ALCOVE_ITEMS
void SkWinCore::DRAW_ALCOVE_ITEMS(U16 xx)
{
	//^32CB:3F0D
	ENTER(14);
	//^32CB:3F13
	X16 si = xx;
	ObjectID di = tblCellTilesRoom[si].xsrd.id4();
	if (di == OBJECT_END_MARKER)
		return;
	//^32CB:3F2F
	X16 bp08 = 2;
	//^32CB:3F34
	do {
		if (di.Dir() != ((glbPlayerDir +2) & 3))
			return;
		//^32CB:3F4A
		X16 bp0a = di.DBType();
		if (bp0a >= dbWeapon && bp0a <= dbMiscellaneous_item) {
			//^32CB:3F61
			DRAW_ITEM(di, si, 0, 0, bp08, NULL, 1, 0, 1);
			bp08++;
			if (bp08 >= 0xe)
				bp08 = 2;
			if (si == 3)
				//^32CB:3F91
				MAKE_PUT_DOWN_ITEM_CLICKABLE_ZONE(8, di, si, 3);
		}
		//^32CB:3FA1
		else if (bp0a == dbCloud && _4976_4463[RCJ(23,si)] != 0xff) {
			//^32CB:3FB4
			Cloud *bp04 = GET_ADDRESS_OF_RECORDF(di);
			U8 bp05 = bp04->CloudType();
			X16 bp0c = RAND02();
			if (bp05 == 0x64) {
				//^32CB:3FDC
				X16 bp0e = _4976_41ed[RCJ(17,_4976_4463[RCJ(23,si)])];
				QUERY_TEMP_PICST(bp0c, bp0e, bp0e, 0, 0, 0, QUERY_RECTNO_FOR_WALL_ORNATE(si, 0, 0xffff), 0xffff, 10, -1, 0x0d, bp05, 0xc);
				DRAW_TEMP_PICST();
			}
			//^32CB:4020
			else if (bp05 == 0x65) {
				_32cb_2cf3(bp05, _4976_41e6[RCJ(7,_4976_4463[RCJ(23,si)])], bp0c, QUERY_RECTNO_FOR_WALL_ORNATE(si, 12, 0));
				DRAW_TEMP_PICST();
			}
		}
		//^32CB:4054
	} while ((di = GET_NEXT_RECORD_LINK(di)) != OBJECT_END_MARKER);
	//^32CB:4065
	return;
}

//^32CB:15B8
// SPX: _32cb_15b8 renamed DRAW_WALL_ORNATE
i16 SkWinCore::DRAW_WALL_ORNATE(i16 cellPos, i16 yy, i16 zz)
{
	//^32CB:15B8
	ENTER(854);

#if XDM1_EXTENDED_SEETHRUWALLS == 1
	{
		if (glbGlobalSpellEffects.SeeThruWalls > 0 && cellPos == 3)	// front D1 wall
		{
			return -1;	// in case of see thru, don't show ornate at all
		}
	}
#endif


	//^32CB:15BE
	i16 bp14 = (yy <= -1)
		? 4
		: ((yy >= 1)
			? 6
			: 5
		);
	//^32CB:15DD
	bp14 = tblCellTilesRoom[cellPos].xsrd.w6[RCJ(4,bp14 -3)];	// get the ornate gfx id ?
	//^32CB:15F9
	i16 bp28 = bp14 >> 8;	// upper part (flags)
	//^32CB:15FF
	i16 iYDist = glbTabYAxisDistance[RCJ(23,cellPos)];	// i16 di
	//^32CB:1609
	U8 bp1f = U8(bp14) & 0xff; // lower part : gfx id
	//^32CB:1611
	if (bp1f == 0xff)
		//^32CB:1617
		return -1;
	//^32CB:161D
	U16 bp2a = (bp1f == 0) ? 1 : 0;
	//^32CB:162D
	U16 alcoveType = GET_WALL_ORNATE_ALCOVE_TYPE(bp1f);	// U16 bp22
	//^32CB:163A
	U16 iDoNotFlip = 0; // U16 bp24 = 0; SPX: fixed value init
	if (bp2a == 0) {
		//^32CB:1640
		iDoNotFlip = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp1f, dtWordValue, GDAT_WALL_ORNATE__DO_NOT_FLIP);	// Has some role for general graphics flip
	}
	//^32CB:1655
	U16 iFlipImage = 0;	// U16 bp0e = 0; use flip or not
	//^32CB:165A
	U16 si = 0; // defaulting to 0
	if (bp2a != 0 || (si = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp1f, dtWordValue, GDAT_IMG_COLORKEY_1)) == 0)
		//^32CB:1678
		si = glbSceneColorKey;
	//^32CB:167C
	U16 iOrnatePos = 0;	// U16 bp1a
	U16 iRefPoint = 0;	// U16 bp1e
	if (bp2a != 0 || (iOrnatePos = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp1f, dtWordValue, GDAT_WALL_ORNATE__POSITION)) == 0) {
		//^32CB:169B
		iOrnatePos = ORNATE_POS__VCENTERED_HCENTERED;	// SPX: that must be the default ornate position (default = 12)
		iRefPoint = 0;	// 0 = point is centered
	}
	else {	// values in GDAT are 1 to 25. shift it back to 0 to 24.
		//^32CB:16A7
		iRefPoint = iOrnatePos >> 8;	 // SPX: value in GDAT is 2 byte. Upper byte is how to draw image from ornate position
		iOrnatePos = (iOrnatePos & 255) -1;
	}
	//^32CB:16BA
	U16 iRectno = QUERY_RECTNO_FOR_WALL_ORNATE(cellPos, iOrnatePos, (yy != 0) ? 1 : 0);	// U16 bp1c; recto
	//^32CB:16D9
	U16 iStretchVertical = 0;	// U16 bp18
	U16 iStretchHorizontal = 0;	// U16 bp16
	iStretchVertical = iStretchHorizontal = tlbDistanceStretch[RCJ(5,iYDist)];
	
	//^32CB:16E5
	if (alcoveType == WALL_ORNATE_OBJECT__CRYOCELL && yy == 0) {	// bp22 == 3 && yy == 0
		//^32CB:16F7
		U16 bp34 = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp1f, dtImageOffset, GDAT_GFXSET_DATA_FD);	// 0x09 .. .. 0xFD
		// bp34 holds the offset x and y for the image
		//^32CB:170D
		// SPX: U8(_4976_5a80[cellPos].x2.w14) holds the portrait Champion ID to be displayed under the Champion Cell/Mirror
		QUERY_TEMP_PICST(iFlipImage, iStretchHorizontal, iStretchVertical, i8(bp34 >> 8), i8(bp34), iYDist, iRectno, iRefPoint, -1, -1, GDAT_CATEGORY_CHAMPIONS, U8(tblCellTilesRoom[cellPos].xsrd.xvalue), 1);
		//^32CB:174C
		if (zz == 0)
			//^32CB:1752
			glbTempPicture.colorKeyPassThrough = -2;
		//^32CB:1758
		DRAW_TEMP_PICST();
		//^32CB:175C
		if (zz == 0)
			//^32CB:1762
			//^32CB:1F38
			return si;
		//^32CB:1765
		if (iYDist == 1) {
			//^32CB:176A
			MAKE_BUTTON_CLICKABLE(&glbTempPicture.rc36, 6, U8(cellPos));
		}
	}
	//^32CB:177A
	if (iYDist == 2 && (yy <= -2 || yy >= 2))
		//^32CB:178B
		iStretchHorizontal = 0x72;	// 0x72 = 114	=> 178%
	//^32CB:1792
	else if (iYDist == 3 && (yy <= -2 || yy >= 2))
		//^32CB:17A3
		iStretchHorizontal = 0x4C;	// 0x4C = 76 => 118%
	if (SkCodeParam::bDM1Mode && iYDist >= 4)	// SPX: DM1 mode, do not display anything more than D3
		return -1;
	//^32CB:17A8
	if (bp2a != 0) {
		U8 bDrawSideTextPanel = 1; // default for DM2 mode
		//^32CB:17B1
		U8 bp20;
		if (yy == 0) {
			//^32CB:17B7
			bp20 = 0xFC;
		}
		else if (yy <= -1) {
			//^32CB:17C3
			bp20 = 0xFD;
		}
		else {
			//^32CB:17C9
			// xFD is the L-side text panel, then xFE would be the R-side text panel if they were not symmetric
			if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, dtImage, 0xFE) != 0) { // SPX: GDAT2 never has 0xFE image ???
				//^32CB:17E0
				bp20 = 0xFE;
			}
			else {
				//^32CB:17E6
				bp20 = 0xFD;
				iFlipImage = 1;	// do flip
			}
		}
		//^32CB:17EF
		QUERY_TEMP_PICST(iFlipImage, iStretchHorizontal, iStretchVertical, 0, 0, iYDist, iRectno, iRefPoint, si, -1, GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, bp20);
		//^32CB:1817
		if (zz == 0)
			//^32CB:181D
			glbTempPicture.colorKeyPassThrough = -2;
		// SPX: this draws the text panel or default gibberish side text. In case of DM1, the gibberish must not be displayed if the text is not visible.
		if (SkCodeParam::bDM1Mode)
		{
			ObjectID xTextObject = tblCellTilesRoom[cellPos].xsrd.xvalue;
			if (!IS_OBJECT_VISIBLE_TEXT(xTextObject))
				bDrawSideTextPanel = 0;
		}

		if (bDrawSideTextPanel == 1) // SPX: added this condition
		//^32CB:1823
			DRAW_TEMP_PICST();
		//^32CB:1827
		if (yy != 0)
			//^32CB:182D
			//^32CB:1F38
			return si;
		//^32CB:1830
		ObjectID bp32 = tblCellTilesRoom[cellPos].xsrd.xvalue;
		//^32CB:1845
		if (bp32 == OBJECT_NULL)
			//^32CB:184A
			//^32CB:1F38
			return si;
		//^32CB:184D
		ExtendedPicture bp01d6;
		QUERY_GDAT_SUMMARY_IMAGE(&bp01d6, GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, bp20);
		//^32CB:1865
		i16 bp2c = bp01d6.w28;
		i16 bp2e = bp01d6.w30;
		//^32CB:1873
		i16 bp48;
		i16 bp4a;
		QUERY_GDAT_IMAGE_METRICS(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, bp20, &bp48, &bp4a);
		//^32CB:188F
		U16 bp26 = ALLOC_TEMP_CACHE_INDEX();
		//^32CB:1897
		U8 bp009c[80];
		QUERY_MESSAGE_TEXT(bp009c, bp32, 2);
		//^32CB:18AA
		U16 bp36 = 0;
		U16 bp38 = 1;
		//^32CB:18B4
		U8 bp45;
		while ((bp45 = bp009c[bp36++]) != 0) {
			//^32CB:18B6
			if (bp45 == vbLf)
				//^32CB:18BC
				bp38++;
		}
		//^32CB:18D5
		U16 bp4c = QUERY_MBCS_PRESENCE(bp009c);
		//^32CB:18E5
		U16 bp3a;
		U16 bp3c;
		if (bp4c != 0) {
			//^32CB:18E9
			bp3a = _4976_012e;
			bp3c = _4976_0130 +2;
			si = 0;
		}
		else {
			//^32CB:18FB
			bp3a = _4976_012a;
			bp3c = _4976_012c +2;
		}
		//^32CB:1909
		U8 *bp08 = ALLOC_NEW_PICT(bp26, bp48, bp4a, 4);
		//^32CB:1922
		FILL_ENTIRE_PICT(bp08, si);
		//^32CB:1931
		SRECT rc44;
		rc44.y = (bp4a >> 1) - (bp3c * bp38 >> 1);
		//^32CB:1945
		if (bp4c != 0) {
			//^32CB:194E
			for (U16 bp36 = 0; bp009c[bp36] != 0; ) {
				//^32CB:1956
				if (bp4a - (_4976_0130 + _4976_0136 +1) <= rc44.y)
					//^32CB:1968
					break;
				//^32CB:196B
				U8 bp0356[384];
				i16 bp30 = _3929_04e2_DRAW_TEXT_STRINGS(bp009c, bp0356, &bp36, bp48 -1);
				//^32CB:198C
				if (bp30 != 0) {
					//^32CB:1995
					rc44.x = (bp48 >> 1) - (bp30 >> 1);
					//^32CB:19A4
					if (rc44.y >= 0 && rc44.x >= 0 && bp30 <= bp48) {
						//^32CB:19C1
						DRAW_STRING(bp08, bp26, bp48, rc44.x   , rc44.y + _4976_0130 +1, 1, 0x4000, bp0356, 4);
						//^32CB:19EE
						DRAW_STRING(bp08, bp26, bp48, rc44.x +1, rc44.y + _4976_0130 +1, 1, 0x4000, bp0356, 4);
						//^32CB:1A1D
						DRAW_STRING(bp08, bp26, bp48, rc44.x   , rc44.y + _4976_0130   , 2, 0x4000, bp0356, 4);
					}
				}
				//^32CB:1A49
				if (bp009c[bp36] == vbLf)
					//^32CB:1A58
					bp36++;
				//^32CB:1A5B
				rc44.y += bp3c;
				//^32CB:1A61
			}
		}
		else {
			//^_1a76
			U8 *bp0c = QUERY_GDAT_IMAGE_ENTRY_BUFF(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, GDAT_GFXSET_LETTERS);	// 0x08 .. 0x03
			//^32CB:1A8C
			rc44.cx = bp3a;
			//^32CB:1A92
			rc44.cy = READ_I16(bp0c,-2);
			//^32CB:1A9C
			U8 *bp08 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp26));
			//^32CB:1AAB
			U8 *bp04 = bp009c;

			do {
				//^32CB:1AB5
				U16 bp36 = 0;
				//^32CB:1ABA
				while (bp04[bp36] != 0 && bp04[bp36] != vbLf)
					//^32CB:1ABC
					bp36++;
				//^32CB:1AD7
				i16 bp30 = bp3a * bp36;
				//^32CB:1AE0
				if (bp36 != 0) {
					//^32CB:1AE6
					rc44.x = (bp48 >> 1) - (bp30 >> 1);
					//^32CB:1AF5
					if (rc44.x < 0) {
						//^32CB:1AFB
						bp04 += bp36;
					}
					else {
						//^32CB:1AF9
						while (bp36-- != 0) {
							//^32CB:1B03
							DRAW_DIALOGUE_PICT(
								bp0c,
								bp08,
								&rc44,
								SKCHR_TO_SCRIPTCHR(*(bp04++)) * bp3a,
								0,
								si,
								QUERY_GDAT_IMAGE_LOCALPAL(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, GDAT_GFXSET_LETTERS)	// 0x08 .. 0x03
								);
							//^32CB:1B46
							rc44.x += bp3a;
							//^32CB:1B49
						}
					}
				}
				//^32CB:1B56
				rc44.y += bp3c;
				//^32CB:1B5C
			} while (*(bp04++) != 0);
		}
		//^32CB:1B6B
		QUERY_GDAT_SUMMARY_IMAGE(&bp01d6, 0xff, 0x00, 0x00);
		//^32CB:1B80
		bp01d6.w12 = bp26;
		bp01d6.w6 = 0xffff;
		bp01d6.w52 = iStretchHorizontal;
		bp01d6.w54 = iStretchVertical;
		//^32CB:1B9B
		if (bp4c != 0) {
			//^32CB:1BA1
			bp01d6.b58[1] = glbPaletteT16[COLOR_BLACK];
			bp01d6.b58[2] = glbPaletteT16[COLOR_BROWN];
		}
		else {
			//^32CB:1BB6
			COPY_MEMORY(
				QUERY_GDAT_IMAGE_LOCALPAL(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, GDAT_GFXSET_LETTERS),	// 08 .. 03
				bp01d6.b58,
				16
				);
		}
		//^32CB:1BDA
		bp01d6.w56 = 16;
		//^32CB:1BE1
		_32cb_0804(bp01d6.b58, U8(iYDist), si, -1, bp01d6.w56);
		//^32CB:1BF3
		bp01d6.pb44 = _4976_4c16;
		//^32CB:1C02
		bp01d6.rectNo = iRectno;
		bp01d6.w26 = iRefPoint;
		bp01d6.w28 = bp2c;
		bp01d6.w30 = bp2e;
		bp01d6.colorKeyPassThrough = si;
		//^32CB:1C22
		DRAW_PICST(QUERY_PICST_IT(&bp01d6));
		//^32CB:1C38
		FREE_TEMP_CACHE_INDEX(bp26);
		//^32CB:1C41
		//^32CB:1F38
		return si;
	}
	//^32CB:1C44
	U8 iImageEntry;	// U8 bp20; which image entry. 1 = front / 0 = side. Add default init to FRONT
	if (yy == 0) {	// Front image
		//^32CB:1C4A
		iImageEntry = GDAT_WALL_IMAGE__VIEW_FRONT;	// = 1;	front view
		//^32CB:1C4E
		if (iDoNotFlip == 0) {
			//^32CB:1C54
			if ((glbTabYAxisDistance[RCJ(23,cellPos)] & 1) != 0) {
				//^32CB:1C5E
				iFlipImage = glbGeneralFlipGraphics;
			}
			else {
				//^32CB:1C66
				iFlipImage = glbGeneralFlipGraphics ^ 1;
			}
		}
	}
	else {	// Side image
		//^32CB:1C71
		iImageEntry = GDAT_WALL_IMAGE__VIEW_SIDE_LEFT;	// = 0;	 side view on left
		//^32CB:1C75
		if (yy >= 1) {
			//^32CB:1C7B
			if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_WALL_GFX, bp1f, dtImage, U8(bp28) +2) != 0) {
				//^32CB:1C95
				iImageEntry = GDAT_WALL_IMAGE__VIEW_SIDE_RIGHT;	// = 2; side view on right
			}
			else {
				//^32CB:1C9B
				iFlipImage = 1;	// flip image
			}
		}
		// SPX: experimentation with side D0 new image
		if (SkCodeParam::bDM1Mode && iYDist == 0)
		{
			U16 iTestSideD0Entry = iImageEntry +U8(bp28) + 0x80;
			if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_WALL_GFX, bp1f, dtImage, iTestSideD0Entry) != 0)
			{
				iStretchVertical = 0x40;
				iStretchHorizontal = 0x40;
				iImageEntry = iImageEntry + 0x80;
			}
		}
	}
	//^32CB:1CA0
	iImageEntry = iImageEntry +U8(bp28);
#if DM2_EXTENDED_MODE == 1
	if (SkCodeParam::bUseExtendedSound && bp28 == 4 /*&& SkCodeParam::bForceOrnateSound == true*/)	// 4 = first anim of a loop (if several)
	{
		int iRandNoise = 0;
		int iMaxLoopSoundsAvailable = 9;
		while(iMaxLoopSoundsAvailable > 0)
		{
			if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_WALL_GFX, bp1f, dtSnd, SOUND_ACTIVATION_LOOP + iMaxLoopSoundsAvailable) != 0)
				break;
			iMaxLoopSoundsAvailable--;
		}
		if (iMaxLoopSoundsAvailable > 0)
			iRandNoise = RAND()%iMaxLoopSoundsAvailable;

//		i16 iYDist = glbTabYAxisDistance[RCJ(23,cellPos)];	
		i16 iXDist = glbTabXAxisDistance[RCJ(23,cellPos)];
		QUEUE_NOISE_GEN2(GDAT_CATEGORY_WALL_GFX, bp1f, SOUND_ACTIVATION_LOOP + iRandNoise, 0xFE, glbPlayerPosX+iXDist, glbPlayerPosY+iYDist, 0, 140, 200);
	}
#endif
	//^32CB:1CA9
	SRECT *bp12;
	if (yy == 0 && QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp1f, dtWordValue, GDAT_WALL_ORNATE__WINDOW) != 0) {
		//^32CB:1CCB
		QUERY_TEMP_PICST(iFlipImage, iStretchHorizontal, iStretchVertical, 0, 0, iYDist, iRectno, iRefPoint, -3, -3, GDAT_CATEGORY_WALL_GFX, bp1f, iImageEntry);
		//^32CB:1CF4
		QUERY_TEMP_PICST(iFlipImage, iStretchHorizontal, iStretchVertical, glbTempPicture.w28, glbTempPicture.w30, iYDist, iRectno, iRefPoint, si, -1, GDAT_CATEGORY_WALL_GFX, bp1f, 200);
		//^32CB:1D1F
		if (zz == 0)
			//^32CB:1D25
			glbTempPicture.colorKeyPassThrough = -1;
		//^32CB:1D2B
		DRAW_TEMP_PICST(); // draw window contents (outside)
		//^32CB:1D2F
		ExtendedPicture bp0310;
		U16 bp26 = QUERY_MULTILAYERS_PIC(
			&bp0310, GDAT_CATEGORY_WALL_GFX, bp1f, iImageEntry, iStretchHorizontal, iStretchVertical, iYDist, iFlipImage, si,
			QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp1f, dtWordValue, GDAT_IMG_WALL_COLORKEY_2)	// 0x11 is colorkey2 for seeing outside through window (0xC8 image)
			);
		//^32CB:1D67
		_32cb_0c7d(&bp0310, bp26, si);
		//^32CB:1D78
		bp0310.pb44 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp26));
		//^32CB:1D89
		DRAW_PICST(&bp0310);
		//^32CB:1D96
		FREE_PICT_MEMENT(&bp0310);
		//^32CB:1DA3
		bp0310.colorKeyPassThrough = bp0310.b58[si];
		bp0310.pb44 = _4976_4c16;
		bp0310.rectNo = iRectno;
		bp0310.w26 = iRefPoint;
		bp0310.w56 = 0;
		//^32CB:1DD0
		_0b36_00c3(bp26, &bp0310);
		//^32CB:1DE1
		DRAW_PICST(&bp0310);
		//^32CB:1DEE
		bp12 = &bp0310.rc36;
		//^32CB:1DF8
		FREE_TEMP_CACHE_INDEX(bp26);
	}
	else {	// No window, standard wall ornate to display
		//^32CB:1E03
		QUERY_TEMP_PICST(iFlipImage, iStretchHorizontal, iStretchVertical, 0, 0, iYDist, iRectno, iRefPoint, si, -1, GDAT_CATEGORY_WALL_GFX, bp1f, iImageEntry);
		//^32CB:1E2B
		if (zz == 0)
			//^32CB:1E31
			glbTempPicture.colorKeyPassThrough = -2;
		//^32CB:1E37
		DRAW_TEMP_PICST();
		//^32CB:1E3B
		bp12 = &glbTempPicture.rc36;
		//^32CB:1E43
		if (zz == 0)
			//^32CB:1E49
			//^32CB:1F38
			return si;
	}
	//^32CB:1E4C
	if (alcoveType != WALL_ORNATE_OBJECT__CRYOCELL && (cellPos == 1 || cellPos == 2 || cellPos == 3)) {
		//^32CB:1E64
		MAKE_BUTTON_CLICKABLE(bp12, 6, U8(cellPos));
	}
	//^32CB:1E76
	if (alcoveType == WALL_ORNATE_OBJECT__NONE || yy != 0)	// bp22 == 0
		//^32CB:1E85
		//^32CB:1F38
		return si;
	//^32CB:1E88
	COPY_MEMORY(&glbTempPicture, &_4976_5aa8, 314);
	//^32CB:1E9D
	// SPX: This is an overlay on front of wall decoration. This is only used for the shop panel
	if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_WALL_GFX, bp1f, dtImage, GDAT_WALL_ORNATE__OVERLAY) != 0) {
		//^32CB:1EB3
		if (alcoveType == WALL_ORNATE_OBJECT__SHOP_GLASS) {	// (bp22 == 2)
			//^32CB:1EB9
			_32cb_0f82_SHOP_GLASS(
				GET_ADDRESS_OF_ACTU(tblCellTilesRoom[cellPos].xsrd.xvalue), 
				bp1f, iYDist, cellPos, iStretchHorizontal, iStretchVertical, iRectno, iRefPoint, si
				);
			//^32CB:1EEF
			//^32CB:1F38
			return si;
		}
		//^32CB:1EF1
		QUERY_TEMP_PICST(iFlipImage, iStretchHorizontal, iStretchVertical, 0, 0, iYDist, iRectno, iRefPoint, si, -1, GDAT_CATEGORY_WALL_GFX, bp1f, GDAT_WALL_ORNATE__OVERLAY);
		//^32CB:1F17
		if (zz == 0)
			//^32CB:1F1D
			glbTempPicture.colorKeyPassThrough = -2;
		//^32CB:1F23
		DRAW_TEMP_PICST();
		//^32CB:1F27
		//^32CB:1F38
		return si;
	}
	//^32CB:1F29
	if (alcoveType == WALL_ORNATE_OBJECT__ALCOVE)	// (bp22 == 1)
		DRAW_ALCOVE_ITEMS(cellPos);
	//^32CB:1F38
	return si;
}

//^0B36:1446
// _0b36_1446 renamed _0b36_1446_BLIT_PICTURE
i16 SkWinCore::_0b36_1446_BLIT_PICTURE(Picture *ref, i16 xx, i16 yy, U16 colorkey)
{
	//^0B36:1446
	ENTER(20);
	//^0B36:144C
	U8 *bp04 = QUERY_PICT_BITS(ref);
	//^0B36:145E
	if (bp04 == NULL)
		//^0B36:1466
		return -1;
	//^0B36:146C
	U16 si = ref->w22;
	//^0B36:1473
	U16 bp0e = ALLOC_TEMP_CACHE_INDEX();
	//^0B36:147B
	ALLOC_NEW_PICT(bp0e, (si == 4) ? 2 : 1, 1, si);
	//^0B36:1497
	U16 bp10 = ALLOC_TEMP_CACHE_INDEX();
	//^0B36:149F
	U8 *bp0c = ALLOC_NEW_PICT(bp10, (si == 4) ? 2 : 1, 1, si);
	//^0B36:14C1
	U8 *bp08 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp0e));
	//^0B36:14D0
	FILL_ENTIRE_PICT(bp08, colorkey);
	//^0B36:14E1
	FILL_ENTIRE_PICT(bp0c, colorkey);
	//^0B36:14F2
	FIRE_BLIT_PICTURE(bp04 = QUERY_PICT_BITS(ref), bp08, ALLOC_TEMP_ORIGIN_RECT(1, 1), xx, yy, 
		READ_I16(bp04,-4), (si == 4) ? 2 : 1, colorkey, 0, si, si, NULL);
	//^0B36:1547
	U16 bp14 = 1;
	//^0B36:154C
	U16 bp12 = CALC_IMAGE_BYTE_LENGTH(bp08);
	//^0B36:155B
	for (U16 di = 0; di < bp12; di++) {
		//^0B36:155F
		if (*(bp08++) != *(bp0c++)) {
			//^0B36:1573
			bp14 = 0;
			//^0B36:1578
			break;
		}
		//^0B36:157A
	}
	//^0B36:1580
	FREE_TEMP_CACHE_INDEX(bp0e);
	//^0B36:1589
	FREE_TEMP_CACHE_INDEX(bp10);
	//^0B36:1592
	return bp14;
}

//^32CB:00F1
// _32cb_00f1 renamed _32cb_00f1_DRAW_PICTURE
U16 SkWinCore::_32cb_00f1_DRAW_PICTURE(U16 xx, U16 yy, i16 zz)
{
	//^32CB:00F1
	ENTER(10);
	//^32CB:00F7
	U16 si = xx;
	U16 di = yy;
	//^32CB:00FD
	if (PT_IN_RECT(&glbTempPicture.rc36, si, di) == 0) {
		//^32CB:010F
		return 0;
	}
	//^32CB:0114
	// SPX: This test mirror flip (horizontal)
	if ((glbTempPicture.mirrorFlip & 1) != 0) {
		//^32CB:011C
		si = glbTempPicture.rc36.x + glbTempPicture.rc36.cx -1 -si + glbTempPicture.w32;
	}
	else {
		//^32CB:0128
		si = si - glbTempPicture.rc36.x + glbTempPicture.w32;
	}
	//^32CB:0134
	// SPX: Try to display mirror flip on y axis .. . This does nothing!!
	//di = glbTempPicture.rc36.y + glbTempPicture.rc36.cy -1 -di + glbTempPicture.w34;
	di = di - glbTempPicture.rc36.y + glbTempPicture.w34;
	//^32CB:0140
	U8 *bp0a = QUERY_PICT_BITS(&glbTempPicture);
	//^32CB:0151
	U16 bp04 = READ_I16(bp0a,-4) >> 1;
	//^32CB:015D
	_4976_5a94 = ((si - glbTempPicture.w14 -bp04) * 100) / bp04;
	//^32CB:0172
	U16 bp06 = READ_I16(bp0a,-2) >> 1;
	//^32CB:017B
	_4976_5a96 = ((di -bp06) * 100) / bp06;
	//^32CB:018C
	i16 bp02 = _0b36_1446_BLIT_PICTURE(&glbTempPicture, si, di, zz);
	//^32CB:01A0
	if (bp02 == -1 || bp02 == 1)
		//^32CB:01AC
		//^32CB:010F
		return 0;
	//^32CB:01AF
	return 1;
}

//^32CB:0287
// SPX: _32cb_0287 renamed _32cb_0287_DRAW_W_ORNATE
U16 SkWinCore::_32cb_0287_DRAW_W_ORNATE(U16 xx, U16 yy, U16 zz)
{
	//^32CB:0287
	ENTER(10);
	//^32CB:028D
	i16 si = xx;
	//^32CB:0290
	U16 di = 0;
	//^32CB:0292
	if (si > 3)
		//^32CB:0297
		return 0;
	//^32CB:029C
	CellTileDetail *bp0a = tblCellTilesRoom;
	//^32CB:02A9
	tblCellTilesRoom = reinterpret_cast<CellTileDetail *>(ALLOC_MEMORY_RAM(72, afDefault, 1024));
	//^32CB:02C1
	i16 bp04 = _4976_5a9c;
	i16 bp06 = _4976_5a9e;
	//^32CB:02CD
	CALC_VECTOR_W_DIR(_4976_5aa0, _4976_40e8[si][1], _4976_40e8[si][0], &bp04, &bp06);
	//^32CB:02F7
	tblCellTilesRoom[si].posx = U8(bp04);
	//^32CB:030A
	tblCellTilesRoom[si].posy = U8(bp06);
	//^32CB:031E
	SUMMARIZE_STONE_ROOM(&tblCellTilesRoom[si].xsrd, _4976_5aa0, bp04, bp06);
	//^32CB:0344
	i16 bp02 = DRAW_WALL_ORNATE(si, glbTabXAxisDistance[RCJ(23,si)], 0);
	//^32CB:0358
	if (bp02 >= 0) {
		//^32CB:035C
		di = _32cb_00f1_DRAW_PICTURE(yy, zz, bp02);
	}
	//^32CB:036E
	DEALLOC_UPPER_MEMORY(72);
	//^32CB:0379
	tblCellTilesRoom = bp0a;
	//^32CB:0386
	return (si == 3 || glbTempPicture.width < 32 || glbTempPicture.height < 32) ? 1 : di;
}


//^32CB:3672
// SPX: _32cb_3672 renamed DRAW_ITEM
void SkWinCore::DRAW_ITEM(ObjectID rl, i16 xx, U16 yy, U16 zz, i16 vv, Creature *ref, U16 ww, U16 ss, U16 tt)
{
	//^32CB:3672
	ENTER(34);
	//^32CB:3678
	U16 si = zz;
	//^32CB:367B
	i16 bp18 = 12; // defaulting to 12
	if (ref != NULL) {
		//^32CB:3686
		AIDefinition *bp04 = QUERY_CREATURE_AI_SPEC_FROM_TYPE(ref->CreatureType());
		//^32CB:369A
		U16 bp22 = bp04->w30;
		//^32CB:36A4
		U16 di = ((bp22 & 15) > 1) ? 0 : 1;
		//^32CB:36B5
		i16 bp20 = QUERY_GDAT_CREATURE_WORD_VALUE(ref->CreatureType(), 0x41);
		//^32CB:36C9
		if (bp20 == 0) {
			//^32CB:36CF
			bp20 = _4976_4398[RCJ(2,di)];
		}
		//^32CB:36D7
		si -= bp20;
		//^32CB:36DA
		U16 bp1e = (rl.Dir() - _4976_5aa0) & 3;
		//^32CB:36EA
		bp18 = (xx == 3 && _4976_5aa2 != 0) ? _4976_41d0[RCJ(7,glbTargetTypeMoveObject)] : 12;
		//^32CB:370A
		if (_32cb_35c1(&xx, &bp18, _4976_4388[RCJ(4,di)][RCJ(4,bp1e)], _4976_4388[RCJ(4,2 +di)][RCJ(4,bp1e)]) == 0)
			//^32CB:373B
			return;
	}
	//^32CB:373E
	else if (tt == 0) {
		//^32CB:3744
		bp18 = QUERY_OBJECT_5x5_POS(rl, _4976_5aa0);
	}
	//^32CB:3755
	i16 bp12 = glbTabYAxisDistance[RCJ(23,xx)];
	//^32CB:3760
	U16 bp0a;
	U16 bp06;
	U16 bp1a;
	if (tt != 0) {
		//^32CB:3766
		bp0a = 0;
		//^32CB:376B
		bp1a = QUERY_GDAT_ENTRY_DATA_INDEX(_4976_5aa8.b8, _4976_5aa8.b9, dtImageOffset, 0xfd);
		//^32CB:3783
		yy = _4976_5aa8.w28 + i8(bp1a >> 8) + _4976_41de[RCJ(8,_4976_41b0[RCJ(16,vv)][0])];
		//^32CB:37A4
		si = _4976_41de[RCJ(8,_4976_41b0[RCJ(16,vv)][1])] + (i8(bp1a) + _4976_5aa8.w30);
		//^32CB:37C6
		bp06 = _4976_5aa8.rectNo;
	}
	else {
		//^32CB:37CB
		bp0a = 4 -(bp18 / 5);
		//^32CB:37DC
		if (xx == 0 && bp0a < 2)
			//^32CB:37E8
			return;
		//^32CB:37EB
		bp06 = QUERY_CREATURE_BLIT_RECTI(xx, bp18, 0) | 0x8000;
	}
	//^32CB:3801
	U16 bp1c = 0;
	//^32CB:3806
	U8 bp0d = QUERY_CLS1_FROM_RECORD(rl);
	//^32CB:3812
	U8 bp0e = QUERY_CLS2_FROM_RECORD(rl);
	//^32CB:381E
	if (bp0d == 0x14 && bp0e < 8) {
		//^32CB:382A
		if (ss == 0) {
			//^32CB:3830
			ss = ss +GET_ADDRESS_OF_RECORD9(rl)->IsOpened();
		}
		//^32CB:384C
		bp1c = 1;
	}
	//^32CB:3851
	U8 bp0f = _4976_4380[RCJ(4,ss)][0];
	//^32CB:385D
	U16 bp08 = 0;
	//^32CB:3862
	i16 bp14 = glbTabXAxisDistance[RCJ(23,xx)];
	//^32CB:386D
	if (bp14 == 0) {
		//^32CB:3871
		i16 bp16 = bp18 % 5;
		//^32CB:387D
		U16 bp10;
		if (true
			&& (bp16 == 2 || tt != 0)
			&& QUERY_GDAT_ENTRY_IF_LOADABLE(bp0d, bp0e, dtImage, U8(bp10 = _4976_4380[RCJ(4,ss)][1])) != 0
		) {
			//^32CB:38AD
			bp0f = U8(bp10);
		}
		//^32CB:38B5
		else if (bp1c != 0 && tt == 0 && bp16 > 2) {
			//^32CB:38C7
			bp08 = 1;
		}
	}
	//^32CB:38CE
	else if (bp1c != 0 && bp14 == 1) {
		//^32CB:38DA
		bp08 = 1;
	}
	//^32CB:38DF
	if (vv != -1) {
		//^32CB:38E5
		yy += _4976_41de[RCJ(8,_4976_41b0[RCJ(16,vv)][0])];
		//^32CB:38FA
		if (ref == NULL) {
			//^32CB:3902
			si += _4976_41de[RCJ(8,_4976_41b0[RCJ(16,vv)][1])];
		}
	}
	//^32CB:3916
	bp1a = QUERY_GDAT_ENTRY_DATA_INDEX(bp0d, 0xfe, dtImageOffset, bp0f);
	//^32CB:392E
	if (bp1a != 0) {
		//^32CB:3932
		yy += i8(bp1a >> 8);
		//^32CB:3939
		si += i8(bp1a);
	}
	//^32CB:393F
	U16 bp0c = _4976_418e[RCJ(6,bp12)][1 +bp0a];
	//^32CB:3951
	QUERY_TEMP_PICST(bp08, bp0c, bp0c, yy, si, bp12, bp06, 0, 10, -1, bp0d, bp0e, bp0f);
	//^32CB:397D
	if (ww == 0)
		//^32CB:3983
		glbTempPicture.colorKeyPassThrough = -2;
	//^32CB:3989
	DRAW_TEMP_PICST();
	//^32CB:398D
	return;
}



//^32CB:4E1C
// SPX: _32cb_4e1c renamed DRAW_STAIRS_FRONT
void SkWinCore::DRAW_STAIRS_FRONT(i16 xx)
{
	//^32CB:4E1C
	ENTER(4);
	//^32CB:4E22
	U8 gfxset = glbMapGraphicsSet;	// bp04
	U16 colorkey = glbSceneColorKey;		// di
	X16 si = (tblCellTilesRoom[xx].xsrd.w6[0] != 0) ? 1 : 0;
	i16 bp02 = _4976_45ae[RCJ(16,xx)][RCJ(2,si)];
	if (bp02 < 0)
		return;
	//^32CB:4E5F
	X8 bp03 = tlbGraphicsStairsFront1[RCJ(16,xx)][RCJ(2,si)];
	if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_GRAPHICSSET, gfxset, dtImage, bp03) != 0) {
		//^32CB:4E80
		DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, gfxset, bp03, bp02, colorkey, 0);
		return;
	}
	//^32CB:4E99
	QUERY_TEMP_PICST(1, 64, 64, 0, 0, 0, bp02, -1, colorkey, -1, 8, gfxset, tlbGraphicsStairsFront2[RCJ(16,xx)][RCJ(2,si)]);
	DRAW_TEMP_PICST();
	//^32CB:4EC8
	return;
}


//^2066:3697
// SPX: _2066_3697 renamed __OPEN_DIALOG_PANEL
void SkWinCore::__OPEN_DIALOG_PANEL(U8 cls2, U16 yy)
{
	//^2066:3697
	ENTER(104);
	//^2066:369B
	_4976_5250 = reinterpret_cast<sksave_header_asc *>(ALLOC_MEMORY_RAM(420, afDefault, 1024));
	//^2066:36B4
	U8 bp6a[40];
	// SPX: Get dialog box button 1 text
	U8 *bp18 = QUERY_GDAT_TEXT(0x1a, cls2, 0x00, bp6a); // LOAD
	//^2066:36CF
	U8 bp40[40];
	// SPX: get dialog box button 2 text
	U8 *bp14 = QUERY_GDAT_TEXT(0x1a, cls2, 0x01, bp40); // CANCEL
	//^2066:36EA
	U8 *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x1a, cls2, 0x00);
	//^2066:3700
	U8 *bp08;
	if (_4976_5d76 != 0) {
		//^2066:3707
		bp08 = QUERY_GDAT_IMAGE_LOCALPAL(0x1a, cls2, 0x00);
	}
	else {
		//^2066:371C
		bp08 = bp04 +CALC_IMAGE_BYTE_LENGTH(bp04);
	}
	//^2066:3737
	SRECT bp10;
	DRAW_DIALOGUE_PARTS_PICT(bp04, QUERY_EXPANDED_RECT(4, &bp10), -1, bp08);
	//^2066:375E
	FREE_PICT_ENTRY(bp04);
	//^2066:376B
	FIRE_FADE_SCREEN(1);
	// RESUME SCREEN with list of savegame, version number, LOAD and CANCEL button
	//^2066:3773
	DRAW_VP_RC_STR(0x1c2, glbPaletteT16[COLOR_GRAY], strVersionNumber); // _4976_3b6f replaced by strVersionNumber
	//^2066:378D
	DRAW_VP_RC_STR(0x1d2, glbPaletteT16[COLOR_ORANGE], bp18);	// LOAD text
	//^2066:37A9
	DRAW_VP_RC_STR(0x1d3, glbPaletteT16[COLOR_ORANGE], bp14);	// CANCEL text
	//^2066:37C5
	_2066_38d9();
	//^2066:37CA
	_4976_4bd2 = yy;
	//^2066:37D0
	_1031_0675(4);
	//^2066:37D8
	_4976_523a = 0;
	//^2066:37DE
	while (glbMouseVisibility > 0) {
		//^2066:37E0
		FIRE_SHOW_MOUSE_CURSOR();
		//^2066:37E5
		_4976_523a++;
		//^2066:37E9
	}
	//^2066:37F0
	return;
}


//^2481:01D8
void SkWinCore::SHOW_CREDITS()
{
	//^2481:01D8
	ENTER(0);
	//^2481:01DC
	X16 si = SCREEN_CREDITS_TIMER;		// SPX: 1800 time before going back to main screen
	FIRE_SELECT_PALETTE_SET(0);
	FIRE_HIDE_MOUSE_CURSOR();
	DRAW_GAMELOAD_DIALOGUE_TO_SCREEN(glbImageCreditScreen, 2, -1, _4976_52b6);
	FIRE_SELECT_PALETTE_SET(1);
	FIRE_SHOW_MOUSE_CURSOR();
	_1031_0541(1);
	do {
		MessageLoop(true); // credits

		//^2481:021D
		MAIN_LOOP();
		if (glbUIEventCode == UI_EVENTCODE_QUIT_CREDITS) // 239
			break;
		//^2481:022A
		SLEEP_SEVERAL_TIME(1);
		//^2481:0232
	} while (--si != 0);
	//^2481:0239
	glbSpecialScreen = _MENU_SCREEN__SHOW_CREDITS;;	// 218 (0xDA)
	_1031_0541(0);
	FIRE_HIDE_MOUSE_CURSOR();
	//^2481:024C
	return;
}


//^0B36:1647
void SkWinCore::DRAW_DIALOGUE_PARTS_PICT(Bit8u *buffsrc, SRECT *rc, i16 colorkey, Bit8u *localpal)
{
	// the back buffer is an off-screen buffer of the dungeon viewport (224x136x8bpp)

	// what you draw:
	// a) dialogue window
	// b) dialogue button
	// c) 18x18 bevelled icon frame

	//^0B36:1647
	//^0B36:164A
	FIRE_BLIT_PICTURE(
		buffsrc,
		_4976_4c16,
		rc,
		0,
		0,
		READ_UI16(buffsrc,-4),
		_4976_00f6,
		colorkey,
		0,
		READ_UI16(buffsrc,-6),
		8,
		localpal
		);
	//^0B36:1686
	return;
}


//^3929:0BD7
void SkWinCore::DRAW_VP_RC_STR(Bit16u rectno, Bit16u clr1, const U8 *str)
{
	// draw string to viewport, within specified rectangle.

	// like dialog ver: "V1.0"
	// like buttons: "SAVE", "QUIT", "RESULT", ...
	// like item name: "SCOUT MAP", ...

	//^3929:0BD7
	//^3929:0BDB
	i16 bp02;
	i16 bp04;
	if (QUERY_STR_METRICS(str, &bp02, &bp04) != 0) {
		//^3929:0BF6
		SRECT bp0c;
		if (QUERY_BLIT_RECT(NULL, &bp0c, rectno, &bp02, &bp04, -1) != 0) {
			//^3929:0C1A
			DRAW_VP_STR(bp0c.x, bp0c.y + bp0c.cy -1, clr1, str);
		}
	}
	//^3929:0C35
	return;
}



//^0B36:16E4
void SkWinCore::DRAW_GAMELOAD_DIALOGUE_TO_SCREEN(Bit8u *buffsrc, Bit16u rectno, i16 colorkey, Bit8u localpal[16])
{
	//^0B36:16E4
	//^0B36:16E8
	i16 bp02 = 0;
	i16 bp04 = 0;
	SRECT bp0c;
	//^0B36:16F2
	if (QUERY_BLIT_RECT(buffsrc, &bp0c, rectno, &bp02, &bp04, -1) != NULL) {
		//^0B36:1718
		FIRE_BLIT_PICTURE(
			buffsrc,
			_4976_4964,
			&bp0c,
			bp02,
			bp04,
			*(Bit16u *)&buffsrc[-4],
			glbScreenWidth,
			colorkey,
			0,
			*(Bit16u *)&buffsrc[-6],
			8,
			localpal
			);
	}
	//^0B36:1755
	return;
}

//^29EE:000F
void SkWinCore::_29ee_000f()
{
	//^29EE:000F
	ENTER(54);
	//^29EE:0014
	U8 bp01 = (_4976_4ddc != 0) ? 0xe : 2;
	sk3f6c bp36;
	_0b36_0c52(&bp36, 9, 1);
	FILL_ENTIRE_PICT(reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp36.w0)), glbPaletteT16[COLOR_BLACK]);
	//^29EE:0053
	i16 si = 0x28;
	for (; si < 0x2e; bp01 += 2, si++) {
		//^29EE:0058
		DRAW_ICON_PICT_ENTRY(0x1, 0x3, bp01, &bp36, si, -1);
	}
	//^29EE:007E
	_0b36_0cbe(&bp36, 1);
	if (_4976_4ddc != 0) {
		_1031_0541(6);
	}
	else {
		_1031_0541(5);
	}
	//^29EE:00A0
	return;
}


//^0AAF:081B
void SkWinCore::DRAW_DIALOGUE_PROGRESS(X32 xx)
{
	//^0AAF:081B
	ENTER(8);
	//^0AAF:081F
	if (_4976_4bd8 == 0)
		return;
	//^0AAF:0826
	SRECT bp08;
	QUERY_EXPANDED_RECT(0x1da, &bp08);
	bp08.cx = i16((bp08.cx * xx) / 1000);
	if (bp08.cx <= 0)
		return;
	FIRE_FILL_BACKBUFF_RECT(&bp08, glbPaletteT16[COLOR_ORANGE]);
	_0aaf_002f();
	//^0AAF:0878
	return;
}


//^32CB:076E
void SkWinCore::DRAW_DUNGEON_GRAPHIC(U8 cls1, U8 cls2, U8 cls4, X16 rectno, i16 colorkey, U16 mirrorflip)
{
	//^32CB:076E
	ENTER(314);
	//^32CB:0773
	X16 si = rectno;
	ExtendedPicture bp013a;
	QUERY_GDAT_SUMMARY_IMAGE(&bp013a, U8(cls1), U8(cls2), U8(cls4));
	_0b36_037e(bp013a.b58, U8(_4976_5a88), colorkey, -1, bp013a.w56);
	//^32CB:07AB
	bp013a.colorKeyPassThrough = colorkey;
	bp013a.rectNo = si;
	bp013a.pb44 = _4976_4c16;
	bp013a.mirrorFlip = mirrorflip;
	if (glbIsPlayerMoving != 0) {
		if (si == 700) {
			bp013a.w34 += _4976_00fa;
		}
		else if (si == 701) {
			bp013a.w34 += _4976_00fc;
		}
	}
	// SPX : special fix DM1
	if (SkCodeParam::bDM1Mode && rectno == 754) // roof door slit D1
		bp013a.w30 -= 3;
	//^32CB:07EB
	DRAW_PICST(QUERY_PICST_IT(&bp013a));
	//^32CB:0801
	return;
}

//^32CB:3991
// SPX: _32cb_3991 renamed DRAW_PUT_DOWN_ITEM
void SkWinCore::DRAW_PUT_DOWN_ITEM(ObjectID rl, i16 cellPos, i16 dir, Creature *ref)
{
	//^32CB:3991
	ENTER(22);
	//^32CB:3997
	ObjectID di = rl;
	if (di == OBJECT_END_MARKER)
		return;
	//^32CB:39A2
	i16 bp0a = glbTabYAxisDistance[RCJ(23,cellPos)];
	if (bp0a > 3) // ignore D4
		return;
	X16 si = 0;
	X16 bp10;
	X16 bp12;
	if (ref != NULL) {
		//^32CB:39C0
		AIDefinition *bp04 = QUERY_CREATURE_AI_SPEC_FROM_TYPE(ref->CreatureType());
		bp10 = bp04->w30;
		bp12 = bp04->w32;
	}
	do {
		i16 bp0c = di.DBType();
		if (bp0c < dbWeapon || bp0c > dbMiscellaneous_item)
			continue;
		//^32CB:3A02
		if (((di.Dir() - _4976_5aa0) & 3) != dir)
			continue;
		//^32CB:3A16
		ObjectID bp06;
		X16 bp08;
		X16 bp0e;
		i16 bp14;
		i16 bp16;
		if (ref != NULL) {
			if ((bp10 & 0x400) == 0 && ((bp10 >> 4) & 15) == 0) {
				//^32CB:3A33
				QUERY_EXPANDED_RECT(bp12, &_4976_4d1a[_4976_4dde].rc0);
				_4976_4d1a[_4976_4dde].b11 = 2;
				_4976_4d1a[_4976_4dde].w8 = di;
				_4976_4d1a[_4976_4dde].b10 = X8(cellPos);
				_4976_4dde++;
				return;
			}
			//^32CB:3A80
			DRAW_ITEM(di, cellPos, 0, 0, si, ref, 1, 0, 0);
			si++;
			si &= 15;
			if ((U8(_48ae_01af(bp10, 0)) & (1 << dir)) != 0) {
				//^32CB:3ABE
				bp08 = dir +4;
				bp0e = 2;
				goto _3b77;
			}
		}
		else {
			//^32CB:3ACF
			DRAW_ITEM(di, cellPos, 0, 0, si, NULL, 1, 0, 0);
			si++;
			si &= 15;
			if (cellPos == 0 || (cellPos == 3 && dir > 1)) {
				//^32CB:3B06
				bp14 = tblCellTilesRoom[cellPos].posx;
				bp16 = tblCellTilesRoom[cellPos].posy;
				bp06 = _1c9a_03cf(&bp14, &bp16, di.Dir());
				if (bp06 == OBJECT_NULL || IS_CREATURE_FLOATING(bp06) != 0 || (_0cee_2df4(bp06) & 0x2000) != 0) {
					//^32CB:3B6C
					bp08 = dir;
					bp0e = 1;
					//^32CB:3B77
_3b77:
					MAKE_PUT_DOWN_ITEM_CLICKABLE_ZONE(bp08, di, cellPos, bp0e);
				}
			}
		}
		//^32CB:3B88
	} while ((di = GET_NEXT_RECORD_LINK(di)) != OBJECT_END_MARKER);
	//^32CB:3B99
	return;
}


//^32CB:2BE6
void SkWinCore::SUMMARY_DRAW_CREATURE(ObjectID rl, i16 cellPos, U32 ss)
{
	// SkD((DLV_BUGHERE, "DBG: SUMMARY_DRAW_CREATURE(%04X,%5d,%08X)\n"
	//	, rl.w, cellPos, ss));

	//^32CB:2BE6
	ENTER(14);
	//^32CB:2BEC
	ObjectID di = rl;
	X16 si = cellPos;
	if (glbIsPlayerMoving != 0 && si == 0)
		return;
	//^32CB:2C00
	i16 bp0e = glbTabYAxisDistance[RCJ(23,si)];
	//^32CB:2C08
	for (; di != OBJECT_END_MARKER; di = GET_NEXT_RECORD_LINK(di)) {
		//^32CB:2C0B
		if (di.DBType() != dbCreature)
			continue;
		//^32CB:2C1B
		Creature *bp04 = GET_ADDRESS_OF_RECORD4(di);
		CreatureInfoData *bp08;
		if (bp04->b5_0_7() == 0xff) {
			//^32CB:2C32
			bp08 = 0;
		}
		else {
			//^32CB:2C3E
			bp08 = &glbTabCreaturesInfo[bp04->b5_0_7()];
		}
		//^32CB:2C5C
		AIDefinition *bp0c = QUERY_CREATURE_AI_SPEC_FROM_TYPE(bp04->CreatureType());
		QUERY_CREATURE_PICST(si, bp0e, bp04, bp08, di);
		DRAW_TEMP_PICST();
		if (bp0c->IsStaticObject() == 0 || bp0c->w30_0_3() == 0 || bp0e >= 4)
			continue;
		//^32CB:2CA4
		if (tblCellTilesRoom[si].xsrd.w0 == 0x11) {
			//^32CB:2CB8
			ss = 0x3ff;
		}
		//^32CB:2CC2
		_32cb_3e08(bp04->GetPossessionObject(), si, ss, bp04);
		//^32CB:2CDE
	}
	//^32CB:2CEF
	return;
}


//^32CB:4681
void SkWinCore::DRAW_DEFAULT_DOOR_BUTTON(U8 cls1, U8 cls2, U8 cls4, i16 iViewportCell)	// U8 cls1, U8 cls2, U8 cls4, i16 ww
{
	//^32CB:4681
	ENTER(4);
	//^32CB:4687
	X16 iViewportCellLocal = iViewportCell;	// X16 di = ww
	i16 iButtonRectno = tlbRectnoDoorButton[RCJ(14,iViewportCellLocal)]; // i16 si
	if (iButtonRectno < 0 || cls2 == 0xff)
		return;
	//^32CB:469B
    X16 iYDistance = glbTabYAxisDistance[RCJ(23,iViewportCellLocal)]; // X16 bp04
	X16 iStretchScale = tlbDistanceStretch[RCJ(5,iYDistance)];	// X16 bp02
	QUERY_TEMP_PICST(0, iStretchScale, iStretchScale, 0, 0, iYDistance, 
		QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOOR_BUTTONS, cls2, dtWordValue, 8) * 5 + iButtonRectno +1950,
		-1, glbSceneColorKey, -1, cls1, cls2, cls4
		);
	DRAW_TEMP_PICST();
	//^32CB:46F9
	if (iButtonRectno == 3 || iButtonRectno == 4) {	// 3 is D1, 4 is D0
		//^32CB:4703
		MAKE_BUTTON_CLICKABLE(&glbTempPicture.rc36, 4, U8(iViewportCellLocal));
	}
	//^32CB:4711
	return;
}

//^32CB:4715
// SPX: _32cb_4715 renamed DRAW_DOOR_FRAMES
void SkWinCore::DRAW_DOOR_FRAMES(i16 iViewportCell, X16 iDoorFrameDisplayFlags)	// i16 xx, X16 yy
{
	//^32CB:4715
	ENTER(16);
	//^32CB:471B
	//i16 si = iViewportCell; // si
	X8 gfxset = glbMapGraphicsSet;	// bp0f
	X16 colorkey = glbSceneColorKey;		// di
	ObjectID bp0e = tblCellTilesRoom[iViewportCell].xsrd.w6[1];
	//yy = 7;
	Door *door = GET_ADDRESS_OF_RECORD0(bp0e);	//*bp04
	// SPX: 0x40 = GDAT_DOOR_NO_FRAMES entry. Only used for the ROOTS door type (BETA)
	if (QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, glbMapDoorType[door->DoorType()], dtWordValue, GDAT_DOOR_NO_FRAMES) == 0) {
		//^32CB:4770
		if ((iDoorFrameDisplayFlags & DRAW_DOOR_FLAG_CEILING_SLIT) != 0) { // (0x01) ceiling slit
			//^32CB:4777
			X8 bp05 = tlbGraphicsDoorRoofSlits[RCJ(14,iViewportCell)];	// 0x12 to 0x17 is door ceiling slit
			if (bp05 != 0xff) {
				//^32CB:4782
				// tlbRectnoDoorRoofSlits
				DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, gfxset, bp05, tlbRectnoDoorRoofSlits[RCJ(14,iViewportCell)], colorkey, DRAW_FLAG_FLIP_NONE);
			}
		}
		//^32CB:479B
		X16 iLocalCell;	// X16 bp0c
		X16 iFrameLeft;	// X16 bp08
		X16 iFrameRight;	// X16 bp0a
		if (glbGeneralFlipGraphics != 0) {
			//^32CB:47A2
			iLocalCell = tlbDoorSideFramesReorder[RCJ(23,iViewportCell)];
			iFrameLeft = 1;	// 1 = right
			iFrameRight = 0;	// 0 = left
		}
		else {
			//^32CB:47B6
			iLocalCell = iViewportCell;
			iFrameLeft = 0;	// 0 = left
			iFrameRight = 1;	// 1 = right
		}
		//^32CB:47C3
		if ((iDoorFrameDisplayFlags & DRAW_DOOR_FLAG_LEFT_FRAME) != 0) {	// (0x02) left door frame
			//^32CB:47CA
			// X8 bp05 = tlbGraphicsDoorSideFrames[RCJ(14,bp0c)][RCJ(2,bp08)]
			X8 iDoorFrameGfx = tlbGraphicsDoorSideFrames[RCJ(14,iLocalCell)][RCJ(2,iFrameLeft)];	// door side frames (28 slots divided into 2 parts (2 * 14))
			if (iDoorFrameGfx != 0xFF) {
				//^32CB:47DD
				QUERY_TEMP_PICST(0, 64, 64, 0, 0, 0, QUERY_CREATURE_BLIT_RECTI(iViewportCell, 10, 0), 4, colorkey, -1, GDAT_CATEGORY_GRAPHICSSET, gfxset, iDoorFrameGfx); // door frame left
				// SPX: DM1 frame adjustment for pixel perfect
				if (SkCodeParam::bDM1Mode) {
					glbTempPicture.w28 -= 2;
					glbTempPicture.w30 += 4;
				}
				//else { // DM2 V5 adjustment
				//	glbTempPicture.w28 -= 10;
				//	glbTempPicture.w30 += 5;
				//}
				DRAW_TEMP_PICST();
			}
		}
		//^32CB:480E
		if ((iDoorFrameDisplayFlags & DRAW_DOOR_FLAG_RIGHT_FRAME) != 0) {	// (0x04) right door frame (holding button)
			//^32CB:4818
			// X8 bp05 = tlbGraphicsDoorSideFrames[RCJ(14,bp0c)][RCJ(2,bp0a)];
			X8 iDoorFrameGfx = tlbGraphicsDoorSideFrames[RCJ(14,iLocalCell)][RCJ(2,iFrameRight)];
			if (iDoorFrameGfx != 0xFF) {
				//^32CB:482E
				QUERY_TEMP_PICST(1, 64, 64, 0, 0, 0, QUERY_CREATURE_BLIT_RECTI(iViewportCell, 14, 0), 3, colorkey, -1, GDAT_CATEGORY_GRAPHICSSET, gfxset, iDoorFrameGfx); // door frame right
				// SPX: DM1 frame adjustment for pixel perfect
				if (SkCodeParam::bDM1Mode) {
					glbTempPicture.w28 += 2;
					glbTempPicture.w30 += 4;
				}
				//else { // DM2 V5 adjustment
				//	glbTempPicture.w28 += 10;
				//	glbTempPicture.w30 += 5;
				//}
				DRAW_TEMP_PICST();
				//^32CB:485F
				if (door->Button() != 0) {
					//^32CB:4870
					// SPX: Here, it is always the default 0 ... How could we custom this?
					// At least, it could be set along the current wallset index? or door type ? ...
					DRAW_DEFAULT_DOOR_BUTTON(GDAT_CATEGORY_DOOR_BUTTONS, 0, door->ButtonState() * 5, iViewportCell);
					// Image for released button is 0
					// Image for pushed button is 5 because there are 5 rectnos for buttons depending on distance.
				}
				else {
					//^32CB:4887
					// In case there is a custom button (from wall ornates gfx)
					if (tblCellTilesRoom[iViewportCell].xsrd.w6[2] != 0xff) {
						//^32CB:489C
						DRAW_DEFAULT_DOOR_BUTTON(GDAT_CATEGORY_WALL_GFX, U8(tblCellTilesRoom[iViewportCell].xsrd.w6[2]) & 0xff, U8(tblCellTilesRoom[iViewportCell].xsrd.w6[2] >> 8) +1, iViewportCell);
					}
				}
			}
		}
	}
	//^32CB:48D1
	return;
}

//^32CB:48D5
X16 SkWinCore::_32cb_48d5(U16 xx, i16 yy) 
{
	//^32CB:48D5
	ENTER(0);
	//^32CB:48D9
	i16 si = (((xx << 7) +64) / yy) >> 1;
	if (CALC_STRETCHED_SIZE(si, yy) < xx)
		//^32CB:48FB
		return si +1;
	//^32CB:4900
	return si;
}

//^32CB:4905
// SPX: _32cb_4905 renamed DRAW_DOOR
void SkWinCore::DRAW_DOOR(i16 iCellPos, X16 yy, X16 zz, X32 aa)	// i16 xx, X16 yy, X16 zz, X32 aa
{
	//^32CB:4905
	ENTER(348);
	//^32CB:490B
	if (yy != 0) {
		//^32CB:4911
		DRAW_DOOR_FRAMES(iCellPos, yy);
	}
	//^32CB:491D
	if (aa != 0) {
		//^32CB:4925
		DRAW_STATIC_OBJECT(iCellPos, aa, 0);
	}
	
	//^32CB:4937
	U16 iDoorState = tblCellTilesRoom[iCellPos].xsrd.w6[0];	// U16 bp06
	X16 iStretchDual;	// X16 si
	if (iDoorState != 0) {	// 0 = open. 1 - 3 = intermediate state. 4 = closed. 5 = destroyed
		//^32CB:4955
		ObjectID bp0c = tblCellTilesRoom[iCellPos].xsrd.w6[1];
		i16 iYDist = glbTabYAxisDistance[RCJ(23,iCellPos)];	// i16 bp08
		if (iYDist <= 3) {
			//^32CB:497D
			i16 iInvertedYDist = tlbInvertedDoorDistance[RCJ(5,iYDist)];	// i16 bp18
			if (iInvertedYDist >= 0) {
				//^32CB:498F
				i16 iDoorPosRectno = tlbRectnoDoorPosition[RCJ(16,iCellPos)];	// i16 bp1a
				if (iDoorPosRectno >= 0) {
					//^32CB:49A2
					Door *xDoor = GET_ADDRESS_OF_RECORD0(bp0c);	// Door *bp04
					U8 iDoorGDATIndex = glbMapDoorType[xDoor->DoorType()];	// U8 bp0e
					//printf("DRAW DOOR : type to draw : %02d => %02d\n", xDoor->DoorType(), glbMapDoorType[xDoor->DoorType()] );
					X16 iDoorColorPassThrough = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, iDoorGDATIndex, dtWordValue, GDAT_IMG_COLORKEY_1);	// X16 bp12
					if (iDoorColorPassThrough != 0)
					{
						// SPX BEGIN
						U8 mirroredDoor = 0;	// by default there is no mirror/flip, so value = 0
						// SPX TEST: Here we can make pseudo animated door like DM1 force field, by playing with the mirror bit
						if (SkCodeParam::bUseDM2ExtendedMode)
						{
							mirroredDoor = QUERY_GDAT_DOOR_IS_MIRRORED(iDoorGDATIndex);
							if (mirroredDoor)
							{
								// RAND02 => 0 to 3
								// Flips: 0 = none, 1 = x, 2 = y, 3 = x & y
								mirroredDoor = RAND02();
							}
						}
						// SPX END

						//^32CB:49DE
						U8 iDoorImg = U8(iYDist) -1;	// U8 bp0f / Check image no from distance; note: distance 0 would give image 0xFF.
						// Door at Y=1 uses image 0 ; Y=2 => image 1 ; Y=3 => image 2 ; Y=0 has normally no specific image, it is image 0 streched.
						iStretchDual = 0x40;	// si = 0x40 = 64 <=> 100%
						X16 iLightPalette = 0;	// X16 bp0a	// 0 = light -> 4 = dark
						if (iYDist == 0 || QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_DOORS, iDoorGDATIndex, dtImage, iDoorImg) == 0) {	// 0xe
							//^32CB:4A09
							if (iYDist != 0) {
								//^32CB:4A0F
								iLightPalette = iYDist;
								iStretchDual = tlbDistanceStretch[RCJ(5,iLightPalette)];	// SPX: iLightPalette is used whereas mostly iYDist is used instead. Aren't they just the same?
							}
							else {
								//^32CB:4A21
								iStretchDual = 0x71;	// si = 0x71 = 113 <=> 176%
							}
							//^32CB:4A24
							iDoorImg = 0;
						}
						//^32CB:4A28
						X16 iOrnateIndex = xDoor->OrnateIndex();	// X16 di
						i16 iCacheNo;	// i16 bp16
#if (XDM1_EXTENDED_SEETHRUWALLS == 1)
						if (iOrnateIndex != 0 || iDoorState == _DOOR_STATE__DESTROYED_ || glbGlobalSpellEffects.SeeThruWalls > 0) {	// + window spell effect
#else
						if (iOrnateIndex != 0 || iDoorState == _DOOR_STATE__DESTROYED_) {	// If there is any ornate or door is destroyed
#endif
							//^32CB:4A43
							ExtendedPicture xPicture;	// ExtendedPicture bp015c;
							// Get door graphics
							//bp16 = QUERY_MULTILAYERS_PIC(&bp015c, GDAT_CATEGORY_DOORS, bp0e, bp0f, si, si, bp0a, 0, bp12, -1);	// 0xe
							iCacheNo = QUERY_MULTILAYERS_PIC(&xPicture, GDAT_CATEGORY_DOORS, iDoorGDATIndex, iDoorImg, iStretchDual, iStretchDual, iLightPalette, mirroredDoor, iDoorColorPassThrough, -1);	// 0xe
							DRAW_PICST(&xPicture);
							FREE_PICT_MEMENT(&xPicture);
							X16 iStretchHorizontal;	// X16 bp20
							X16 iStretchVertical;	// X16 bp22
							if (iStretchDual == 0x40) {
								//^32CB:4A88
								i16 bp1c;
								i16 bp1e;
								
								QUERY_GDAT_IMAGE_METRICS(GDAT_CATEGORY_DOORS, iDoorGDATIndex, 0, &bp1c, &bp1e);	// 0xe
								iStretchHorizontal = _32cb_48d5(glbTempPicture.width, bp1c);
								iStretchVertical = _32cb_48d5(glbTempPicture.height, bp1e);
								
							}
							else {
								//^32CB:4AC4
								iStretchVertical = iStretchHorizontal = iStretchDual;
							}
							//^32CB:4ACC
							// If there is an ornate on the door ...
							if (iOrnateIndex != 0) {
								//^32CB:4AD0
								iOrnateIndex--;
								U8 iOrnateGDATIndex = glbMapDoorOrnatesList[iOrnateIndex];	// (bp0d) Get the real ornate gfx from the current map door ornate list.
								// SPX: Get ornate
								X16 iColorTransparencyOverlay = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOOR_GFX, iOrnateGDATIndex, dtWordValue, GDAT_IMG_COLORKEY_1);	// X16 bp14
								if (iColorTransparencyOverlay == 0) {
									//^32CB:4AEE
									iColorTransparencyOverlay = 9;	// SPX: This is the "colorkey" index (generally not set in any GDAT2).
								}
								//^32CB:4AF3
								// SPX: draw the ornate over the door // added for 
								//QUERY_TEMP_PICST(0, bp20, bp22, 0, 0, bp08, 
								
								if (!SkCodeParam::bUseDM2ExtendedMode)
									QUERY_TEMP_PICST(mirroredDoor, iStretchHorizontal, iStretchVertical, 0, 0, iYDist, 
										(QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOOR_GFX, iOrnateGDATIndex, dtWordValue, GDAT_DOOR_ORNATE__POSITION) << 2) + iInvertedYDist +0x7d0,
										-1, iColorTransparencyOverlay, -1, GDAT_CATEGORY_DOOR_GFX, iOrnateGDATIndex, GDAT_DOOR_DECORATION_MASK
										);
								else if (SkCodeParam::bUseDM2ExtendedMode)
								{	// SPX: use the colorkey 10 (by default) or what's in GDAT instead of -1 for activating 'pass through' bitmaps
									X16 iColorPassThrough = 10;	// 10 is standard DARK GREEN
									iColorPassThrough = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOOR_GFX, iDoorGDATIndex, dtWordValue, GDAT_IMG_COLORKEY_1);
									if (iColorPassThrough == 0)
										iColorPassThrough = 10;
									QUERY_TEMP_PICST(mirroredDoor, iStretchHorizontal, iStretchVertical, 0, 0, iYDist, 
										(QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOOR_GFX, iOrnateGDATIndex, dtWordValue, GDAT_DOOR_ORNATE__POSITION) << 2) + iInvertedYDist +0x7d0,
										-1, iColorTransparencyOverlay, iColorPassThrough, GDAT_CATEGORY_DOOR_GFX, iOrnateGDATIndex, GDAT_DOOR_DECORATION_MASK
										);
								}

								glbTempPicture.pb44 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(iCacheNo));
								DRAW_TEMP_PICST();
							}
							//^32CB:4B45
							if (iDoorState == _DOOR_STATE__DESTROYED_) {	// (iDoorState == 5) If door is destroyed .. then draw destroyed mask over any ornate
								//^32CB:4B4B
								X16 iColorTransparencyOverlay = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, iDoorGDATIndex, dtWordValue, GDAT_IMG_DOOR_COLORKEY_2);	// X16 bp14
								if (iColorTransparencyOverlay == 0)
									iColorTransparencyOverlay = 9;	// 9 is standard CYAN

								// SPX: (2016-10-30) Get the destroyed door mask and use default one if available
								if (!SkCodeParam::bUseFixedMode)
								QUERY_TEMP_PICST(0, iStretchHorizontal, iStretchVertical, 0, 0, iYDist, 
									(QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, iDoorGDATIndex, dtWordValue, GDAT_DOOR_DESTROYED_MASK_POSITION) << 2) + iInvertedYDist +0x7d0,
									-1, iColorTransparencyOverlay, iDoorColorPassThrough, GDAT_CATEGORY_DOORS, iDoorGDATIndex, GDAT_DOOR_DESTROYED_MASK
									);
								else if (SkCodeParam::bUseFixedMode)
								{
									U16 iDoorDestroyedGDATIndex = iDoorGDATIndex;
									// Check if there is a destroyed mask for the current door
									U16 iDoorDestroyedMask = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, iDoorGDATIndex, dtImage, GDAT_DOOR_DESTROYED_MASK);
									if (iDoorDestroyedMask == (U16)-1) // not found, get the default one
										iDoorDestroyedGDATIndex = GDAT_ITEM_DEFAULT_INDEX;
									QUERY_TEMP_PICST(0, iStretchHorizontal, iStretchVertical, 0, 0, iYDist, 
										(QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, iDoorGDATIndex, dtWordValue, GDAT_DOOR_DESTROYED_MASK_POSITION) << 2) + iInvertedYDist +0x7d0,
										-1, iColorTransparencyOverlay, iDoorColorPassThrough, GDAT_CATEGORY_DOORS, iDoorDestroyedGDATIndex, GDAT_DOOR_DESTROYED_MASK
										);								
								}
								// End SPX: (2016-10-30) 
								
								glbTempPicture.pb44 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(iCacheNo));
								DRAW_TEMP_PICST();
							}
#if (XDM1_EXTENDED_SEETHRUWALLS == 1)
							if (glbGlobalSpellEffects.SeeThruWalls > 0 && iYDist == 1 && yy == 7) {	// If Window spell is active, and display only if distance = 1 and just in front
								
								X16 iColorTransparencyOverlay = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, iDoorGDATIndex, dtWordValue, GDAT_IMG_DOOR_COLORKEY_2);	// X16 bp14
								if (iColorTransparencyOverlay == 0)
									iColorTransparencyOverlay = 9;	// 9 is standard CYAN
								
								{
									U16 iDoorSeeThruGDATIndex = iDoorGDATIndex;
									
									U16 iDoorSeeThruMask = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, iDoorGDATIndex, dtImage, GDAT_DOOR_SEE_THRU_MASK);
									if (iDoorSeeThruMask == (U16)-1) // not found, get the default one
										iDoorSeeThruGDATIndex = GDAT_ITEM_DEFAULT_INDEX;
									QUERY_TEMP_PICST(0, iStretchHorizontal, iStretchVertical, 0, 0, iYDist, 
										(QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, iDoorGDATIndex, dtWordValue, GDAT_DOOR_DESTROYED_MASK_POSITION) << 2) + iInvertedYDist +0x7d0,
										-1, iColorTransparencyOverlay, iDoorColorPassThrough, GDAT_CATEGORY_DOORS, iDoorSeeThruGDATIndex, GDAT_DOOR_SEE_THRU_MASK
										);								
								}
								
								glbTempPicture.pb44 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(iCacheNo));
								DRAW_TEMP_PICST();
							}
#endif // XDM1_EXTENDED_SEETHRUWALLS
							//^32CB:4BBC
							xPicture.colorKeyPassThrough = xPicture.b58[iDoorColorPassThrough];
							xPicture.w56 = 0;
							_0b36_00c3(iCacheNo, &xPicture);
							// SPX: that draws the main door (without ornate)
							COPY_MEMORY(&xPicture, &glbTempPicture, sizeof(ExtendedPicture));
							glbTempPicture.pb44 = _4976_4c16;
						}
						else {
							//^32CB:4C0C
							iCacheNo = -1;
							//QUERY_TEMP_PICST(0, si, si, 0, 0, bp0a, -1, -1, bp12, -1, 0xe, bp0e, bp0f);
							QUERY_TEMP_PICST(mirroredDoor, iStretchDual, iStretchDual, 0, 0, iLightPalette, -1, -1, iDoorColorPassThrough, -1, GDAT_CATEGORY_DOORS, iDoorGDATIndex, iDoorImg);
						}
						//^32CB:4C36
						// SPX: this part change position of door when in intermediate state
						if (iDoorState < 4) {	// 4 = closed. < 4 => intermediate state. 0 = opened
							//^32CB:4C3C
							iDoorPosRectno = iDoorPosRectno + iDoorState;
							if (xDoor->OpeningDir() == 0) // 0 = horizontal
							{
								X16 iWidth = 0;	// SPX: added this to not reuse "di" variable, already used for ornate index
								//^32CB:4C56
								glbTempPicture.width >>= 1;
								iWidth = glbTempPicture.width;
								glbTempPicture.w4 |= 0x10;
								glbTempPicture.w14 = glbTempPicture.w14 + iWidth;
								glbTempPicture.rectNo = iDoorPosRectno + 6;
								DRAW_TEMP_PICST();	// will draw RIGHT part of the door
								glbTempPicture.w14 = glbTempPicture.w14 + iWidth;
								iDoorPosRectno += 3;
							}
						}
						// Explanation on door rectno offsets:
						// 0 = closed
						// 1 - 3 = vertical positions (º, Ω, æ closed)
						// 4 - 6 = horizontal left positions (º, Ω, æ closed)	=> hence +3 on rectno for horizontal opening
						// 7 - 9 = horizontal right positions (º, Ω, æ closed)	=> hence +6 on rectno for horizontal opening
						//^32CB:4C86
						glbTempPicture.rectNo = iDoorPosRectno;
						glbTempPicture.w30++; // SPX: added this to shift down 1 pixel to get pixel precise regarding DM1 door position
						DRAW_TEMP_PICST();	// draw the door or LEFT part for horizontal opening
						if (iCacheNo >= 0) {
							//^32CB:4C96
							FREE_TEMP_CACHE_INDEX(iCacheNo);
						}
					}
				}
			}
		}
	}
	//^32CB:4C9F
	if (zz != 0) {
		//^32CB:4CA5
		DRAW_DOOR_FRAMES(iCellPos, zz);
	}
	//^32CB:4CB1
	if (aa != 0) {
		//^32CB:4CB9
		_32cb_2d8c(tblCellTilesRoom[iCellPos].xsrd.w4, iCellPos, aa);
	}
	//^32CB:4CDB
	return;
}

//^32CB:4CDF
// SPX: _32cb_4cdf renamed DRAW_DOOR_TILE
void SkWinCore::DRAW_DOOR_TILE(i16 iCellPos)	// i16 xx
{
	//^32CB:4CDF
	ENTER(6);
	//^32CB:4CE5
	//i16 si = xx; <= iCellPos
	if (_4976_455e[RCJ(16,iCellPos)] == 0)
		return;
	X32 bp04 = 0x1000;
	if (tblCellTilesRoom[iCellPos].xsrd.w6[0] != 5) {
		//^32CB:4D13
		bp04 = 0x1000;
	}
	// SPX: draw objects on door tile behind the door
	DRAW_STATIC_OBJECT(iCellPos, 0x3ff, (iCellPos != 0) ? 1 : 0);
	X16 bp06 = 0;
	X16 di = 0;
	switch (iCellPos) {
		case 0://^4D4E	// SPX: player is on the tile of the door
			//^32CB:4D4E
			if (bp04 != 0) {
				//^32CB:4D56
				DRAW_STATIC_OBJECT(iCellPos, bp04, 0);
			}
			DRAW_DOOR(iCellPos, 6, 0, 0);
			_32cb_2d8c(tblCellTilesRoom[iCellPos].xsrd.w4, iCellPos, 0x01ffffff);
			return;
		case 7://^4D96
			//^32CB:4D96
			di = 1;
            bp06 = 4;
			goto _4dc0;
		case 8://^4DA0
			//^32CB:4DA0
			di = 1;
			bp06 = 2;
			goto _4dc0;
		case 12://^4DA2
			//^32CB:4DA2
			di = 2;
			bp06 = 4;
			goto _4dc0;
		case 13://^4DA7
			//^32CB:4DA7
			di = 4;
			bp06 = 2;
			goto _4dc0;
		case 11://^4DAC
			//^32CB:4DAC
			di = 6;
			goto _4dc0;
		case 4://^4DB1
			//^32CB:4DB1
			di = 1;
            bp06 = 4;
			goto _4dc0;
		case 5://^4DB3
			di = 1;
			bp06 = 2;
			goto _4dc0;
		case 3://^4DBD
		case 6://^4DBD
			di = 7;
			goto _4dc0;
		case 14://^4DC0
		case 15://^4DC0
			//^32CB:4DC0
_4dc0:
			DRAW_DOOR(iCellPos, di, bp06, bp04);
			break;
		case 1://^4DD2
		case 2://^4DD2
		case 9://^4DD2
		case 10://^4DD2
			break;
	}
	//^32CB:4DD2
	// SPX: draw objects on door tile before the door
	DRAW_STATIC_OBJECT(iCellPos, 0x01fffc00 & (~bp04), 1);
	_32cb_3edd(iCellPos);
	//^32CB:4DF8
	return;
}

//^32CB:4ECC
// SPX: _32cb_4ecc renamed DRAW_STAIRS_SIDE
void SkWinCore::DRAW_STAIRS_SIDE(i16 xx)
{
	//^32CB:4ECC
	ENTER(4);
	//^32CB:4ED2
	i16 di = xx;
	U8 gfxset = glbMapGraphicsSet;	// bp02
	i16 bp04 = glbSceneColorKey;
	if (di > 8)
		return;
	//^32CB:4EE6
	X16 si = (tblCellTilesRoom[di].xsrd.w6[0] != 0) ? 1 : 0;
	X8 bp01 = tlbGraphicsStairsSide[RCJ(9,di)][RCJ(2,si)];	// bp01	, table containing 0xC7 to 0xD2 = side stairs (wood ramp)
	if (bp01 != 0xff) {
		//^32CB:4F12
		DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, gfxset, bp01, _4976_4600[RCJ(9,di)][RCJ(2,si)], bp04, 0);
	}
	//^32CB:4F37
	return;
}


//^32CB:245A
// SPX: _32cb_245a renamed DRAW_PIT_TILE
void SkWinCore::DRAW_PIT_TILE(i16 xx)
{
	//^32CB:245A
	ENTER(6);
	//^32CB:2460
	i16 si = xx;
	X8 bp03 = glbMapGraphicsSet;
	X16 bp06 = glbSceneColorKey;
	i16 bp02 = _4976_4282[RCJ(16,si)];
	if (bp02 < 0)
		return;
	if (si < 11 || tblCellTilesRoom[si].xsrd.w6[0] == 0) {
		//^32CB:2497
		X16 di = tlbGraphicsFlip[RCJ(16,si)];	// di
		if (si == 0) {
			//^32CB:24A3
			di = (SET_GRAPHICS_FLIP_FROM_POSITION(1, _4976_5aa0, _4976_5a9c, _4976_5a9e) != 0) ? 1 : 0;
		}
		//^32CB:24C4
		DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, bp03
			, (tblCellTilesRoom[si].xsrd.w6[0] != 0) ? tlbGraphicsPitHidden[RCJ(16,si)] : tlbGraphicsPitNormal[RCJ(16,si)], bp02, bp06, di);
	}
	//^32CB:24F7
	return;
}
//^32CB:1F3E
// SPX: _32cb_1f3e renamed DRAW_EXTERNAL_TILE
X16 SkWinCore::DRAW_EXTERNAL_TILE(i16 xx)
{
	//^32CB:1F3E
	ENTER(356);
	//^32CB:1F44
	i16 si = xx;
	X8 bp14 = _4976_4230[RCJ(23,si)];
	if (bp14 == 0xff)
		return 0;
	//^32CB:1F55
	i16 bp06 = tblCellTilesRoom[si].posx;
	i16 bp08 = tblCellTilesRoom[si].posy;
	i16 bp18 = glbTabYAxisDistance[RCJ(23,si)];
	//^32CB:1F86
	TELE_inf bp2a; // bp2a-bp26
	if (bp18 > 3 && GET_TELEPORTER_DETAIL(&bp2a, U8(bp06), U8(bp08)) == 0)
		return 0;
	//^32CB:1FA6
	U16 bp02 = tblCellTilesRoom[si].xsrd.w6[2];
	X16 bp16 = bp02 >> 8;
	bp14 += X8(bp16);
	X8 iFloorOrnateID = X8(bp02);	// bp13
	if (iFloorOrnateID == 0xFF || iFloorOrnateID == 0)
		return 0;
	//^32CB:1FDF
	X16 bp04 = 0;
	if (glbTabXAxisDistance[RCJ(23,si)] == 0) {
		//^32CB:1FEB
		if (QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, dtWordValue, GDAT_FLOOR_ORNATE__DO_NOT_FLIP) == 0) {
			//^32CB:2001
			bp04 = glbGeneralFlipGraphics;
			if ((bp18 & 1) == 0) {
				//^32CB:200E
				bp04 ^= 1;
			}
		}
	}
	//^32CB:2015
	else if (glbTabXAxisDistance[RCJ(23,si)] >= 1) {
		//^32CB:201C
		bp04 = 1;
	}
	//^32CB:2021
	X16 bp0e = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, dtWordValue, GDAT_IMG_COLORKEY_1); // 4
	if (bp0e == 0) {
		//^32CB:203A
		bp0e = glbSceneColorKey;
	}
	//^32CB:2040
	X16 bp0a = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, dtWordValue, GDAT_FLOOR_ORNATE__POSITION); // 5
	X16 bp12;
	if (bp0a == 0) {
		//^32CB:2059
		bp0a = ORNATE_POS__VCENTERED_HCENTERED; // bp0a = 12;
		bp12 = 0;
	}
	else {
		//^32CB:2065
		bp12 = bp0a >> 8;
		bp0a = (bp0a & 0xff) -1;
	}
	//^32CB:2078
	X16 bp0c = QUERY_CREATURE_BLIT_RECTI(si, bp0a, 0);
	X16 bp10 = 0x40; // bp10
	if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, dtImage, bp14) == 0) {
		//^32CB:20A6
		bp14 = _4976_4247[RCJ(23,si)] +X8(bp16);
		bp10 = tlbDistanceStretch[RCJ(5,bp18)];
		// SPX: if drawing player tile and there is a floor image at front D0, we try to get it
		if (SkCodeParam::bUseDM2ExtendedMode && bp18 == 0)
		{
			if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, dtImage, bp14 + 0x80) == 1)
			{
				bp14 = bp14 + 0x80;
				bp10 = 0x40; // 100% because we want to display front D0 at its real size.
			}
		}
	}
	//^32CB:20BC
	X16 bp1a;
	X16 bp22;
	X16 bp24;
	i16 di;
	X16 bp20;
	ExtendedPicture bp0164;
	X16 bp1c;
	if (glbTabXAxisDistance[RCJ(23,si)] == 0 && bp18 != 0) {
		//^32CB:20CF
		X16 bp1e = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, dtWordValue, GDAT_FLOOR_ORNATE__X63);
		if ((bp1c = GET_TELEPORTER_DETAIL(&bp2a, U8(bp06), U8(bp08))) != 0 || bp1e != 0) {
			//^32CB:2107
			QUERY_TEMP_PICST(bp04, bp10, bp10, 0, 0, bp18, bp0c, bp12, -3, -3, GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, bp14);
			QUERY_TEMP_PICST(bp04, bp10, bp10, glbTempPicture.w28, glbTempPicture.w30, bp18, bp0c, bp12, bp0e, -1, GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, bp14 +0xc8);
			DRAW_TEMP_PICST();
			//^32CB:2168
			bp06 = bp2a.b2;
			bp08 = bp2a.b3;
			bp1a = _4976_5aa0;
			_4976_5aa0 = bp2a.b1;
			bp22 = glbCurrentMapIndex;
			CHANGE_CURRENT_MAP_TO(bp2a.b4);
			_4976_4212 = 1;
			bp24 = glbTabYAxisDistance[RCJ(23,si)];
			CALC_VECTOR_W_DIR(_4976_5aa0, -bp24, 0, &bp06, &bp08);
			//^32CB:21C4
			for (di = 4; di >= bp24; di--) {
				//^32CB:21C9
				_32cb_4185_TILE_ROOM(bp06, bp08, _4976_422b[RCJ(5,di)], _4976_5aa0);
				//^32CB:21E1
			}
			//^32CB:21E7
			for (di = 4; di > bp24; di--) {
				//^32CB:21EC
				DRAW_STATIC_OBJECT(_4976_422b[RCJ(5,di)], 0x01ffffff, 1);
				//^32CB:2201
			}
			//^32CB:2207
			CHANGE_CURRENT_MAP_TO(bp22);
			_4976_4212 = 0;
			_4976_5aa0 = bp1a;
			DRAW_STATIC_OBJECT(si, 0x000003ff, 1);
			bp20 = QUERY_MULTILAYERS_PIC(
				&bp0164, GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, bp14, bp10, bp10, bp18, bp04, bp0e,
				QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, dtWordValue, GDAT_IMG_FLOOR_COLORKEY_2)	// 0xa
				);
			if (bp1e != 0) {
				//^32CB:226E
				_32cb_0c7d(&bp0164, bp20, bp0e);
			}
			else {
				//^32CB:2283
				FILL_ENTIRE_PICT(reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp20)), bp0164.b58[bp0e]);
			}
			//^32CB:22A5
			bp0164.pb44 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp20));
			DRAW_PICST(&bp0164);
			FREE_PICT_MEMENT(&bp0164);
			bp0164.colorKeyPassThrough = bp0164.b58[bp0e];
			bp0164.pb44 = _4976_4c16;
			bp0164.rectNo = bp0c;
			bp0164.w26 = bp12;
			bp0164.w56 = 0;
			_0b36_00c3(bp20, &bp0164);
			DRAW_PICST(&bp0164);
			FREE_TEMP_CACHE_INDEX(bp20);
			return bp1c;
		}
	}
	//^32CB:2331
	QUERY_TEMP_PICST(bp04, bp10, bp10, 0, 0, bp18, bp0c, bp12, bp0e, -1, GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, bp14);
	DRAW_TEMP_PICST();
	//^32CB:2361
	return 0;
}
//^32CB:2367
// SPX: _32cb_2367 renamed DRAW_PIT_ROOF
void SkWinCore::DRAW_PIT_ROOF(i16 xx)
{
	//^32CB:2367
	ENTER(10);
	//^32CB:236D
	i16 si = xx;
	X8 gfxset = glbMapGraphicsSet;	//bp07
	X16 bp0a = glbSceneColorKey;
	if (si > 8 || (glbSceneFlags & 1) == 0)
		return;
	//^32CB:238F
	i16 bp04 = tblCellTilesRoom[si].posx;
	i16 bp06 = tblCellTilesRoom[si].posy;
	i16 bp02 = LOCATE_OTHER_LEVEL(glbCurrentMapIndex, -1, &bp04, &bp06, NULL);
	if (bp02 < 0)
		return;
	//^32CB:23DB
	bp02 = glbMapTileValue[bp02][bp04][bp06];
	if ((bp02 >> 5) != 2 || (bp02 & 8) == 0)
		return;
	//^32CB:240B
	X16 di = _4976_4279[RCJ(9,si)];
	if (si == 0) {
		//^32CB:2417
		di = (SET_GRAPHICS_FLIP_FROM_POSITION(0x20, _4976_5aa0, _4976_5a9c, _4976_5a9e) != 0) ? 1 : 0;
	}
	//^32CB:2438
	DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, gfxset, tlbGraphicsPitRoof[RCJ(9,si)], _4976_425e[RCJ(9,si)], bp0a, di);
	//^32CB:2456
	return;
}

//^32CB:3143
// SPX: _32cb_3143 renamed DRAW_FLYING_ITEM
void SkWinCore::DRAW_FLYING_ITEM(ObjectID rl, i16 cellPos, X16 _5x5)
{
	//^32CB:3143
	ENTER(32);
	//^32CB:3149
	if (rl == OBJECT_END_MARKER)
		return;
	//^32CB:3152
	i16 di = DIR_FROM_5x5_POS(_5x5);
	if (di == -1)
		return;
	//^32CB:3165
	X16 bp1a = glbTabYAxisDistance[RCJ(23,cellPos)];
	if (bp1a == 0 && di >= 2)
		return;
	//^32CB:317E
	i16 bp1c = glbTabXAxisDistance[RCJ(23,cellPos)];
	if (_4976_415b[RCJ(23,cellPos)] < 0)
		return;
	//^32CB:3193
	X16 bp1e = tblCellTilesRoom[cellPos].posx;
	X16 bp20 = tblCellTilesRoom[cellPos].posy;
	X16 si;
	do {
		//^32CB:31BE
		if (QUERY_OBJECT_5x5_POS(rl, _4976_5aa0) != _5x5)
			continue;
		//^32CB:31D4
		if (rl.DBType() != dbMissile)
			continue;
        //^32CB:31E5
		i16 bp08 = QUERY_CREATURE_BLIT_RECTI(cellPos, _5x5, 0);
        X16 bp0c = _48ae_011a(rl);
		if (bp0c == 0xffff) {
			//^32CB:320A
			ObjectID bp18 = ObjectID(GET_ADDRESS_OF_RECORDE(rl)->GetMissileObject(), rl.Dir());
			DRAW_ITEM(bp18, cellPos, 0, 0xffa4, -1, NULL, 1, 0, 0);
			continue;
		}
		//^32CB:324D
		Missile *bp04 = GET_ADDRESS_OF_RECORDE(rl);
		U8 bp12 = QUERY_CLS1_FROM_RECORD(rl);
		U8 bp11 = QUERY_CLS2_FROM_RECORD(rl);
		X16 bp0e = (bp12 == 0xd) ? 1 : 0;
		X16 bp10;
		i16 bp16;
		//^32CB:3284
		if ((bp0e == 0 || bp04->EnergyRemaining() == 0xff) && bp1a == 0) {
			//^32CB:3298
			bp10 = bp16 = 0x40;
		}
		else {
			//^32CB:32A3
			bp16 = (bp1a << 1) -(di >> 1);
			if (bp16 < 0)
				continue;
			//^32CB:32B8
			bp10 = bp16 = _4976_41a9[RCJ(7,bp16)];
			if (bp0e != 0) {
				//^32CB:32CD
				bp10 = max_value(8, CALC_STRETCHED_SIZE((((bp04->EnergyRemaining() << 7) / 255) +1) >> 1, bp10));
			}
		}
		//^32CB:32FB
		si = 0;
		X8 bp13;
		if (bp0c == 3) {
			//^32CB:3303
			bp13 = 8;
		}
		else {
			//^32CB:330A
			X16 bp0a = glbTimersTable[bp04->TimerIndex()].Direction();
			if ((bp0a & 1) != (_4976_5aa0 & 1)) {
				//^32CB:3338
				bp13 = 0xc;
				if (bp0c == 0) {
					if (di == 0 || di == 3) {
						//^32CB:334B
						si |= 1;
					}
					//^32CB:334F
					if (((bp1e +bp20) & 1) == 0) {
						si ^= 1;
					}
				}
				//^32CB:3364
				else if (((_4976_5aa0 +1) & 3) == bp0a) {
					si |= 1;
				}
			}
			//^32CB:3376
			else {
				if (bp0c == 0) {
					if (((bp1e +bp20) & 1) != 0) {
						//^32CB:3387
						si |= 2;
						if (di < 2) {
							//^32CB:3390
							bp13 = 8;
						}
						else {
							//^32CB:3396
							bp13 = 9;
						}
					}
					//^32CB:339C
					else if (di >= 2) {
						//^32CB:33A1
						//^32CB:3390
						bp13 = 8;
					}
					else {
						//^32CB:33A3
						bp13 = 9;
					}
				}
				//^32CB:33A9
				else if (bp0c == 2 || (bp0c == 1 && bp0a != _4976_5aa0)) {
					//^32CB:33BE
					//^32CB:3390
					bp13 = 8;
				}
				else {
					//^32CB:33C0
					bp13 = 10;
				}
				//^32CB:33C4
				if (bp1c < 1 && (bp1c != 0 || (di != 1 && di != 2))) {
					//^32CB:33DA
					si |= 1;
				}
				//^32CB:33DE
				if ((di & 1) != 0 && bp12 == 0xd) {
					//^32CB:33EA
					si |= 2;
				}
			}
		}
		//^32CB:33EE
		X16 bp06;
		if (bp12 == 0xd) {
			//^32CB:33F4
			bp06 = QUERY_GDAT_ENTRY_DATA_INDEX(0xd, bp11, dtWordValue, 0x01);
		}
		else {
			bp06 = 3;
		}
		//^32CB:3410
		QUERY_TEMP_PICST(si & bp06, bp10, bp10, 0, 0, bp1a, bp08 | 0x8000, 0, 10, -1, bp12, bp11, bp13);
		glbTempPicture.w32 += CALC_STRETCHED_SIZE(0, bp16);
		glbTempPicture.w34 = CALC_STRETCHED_SIZE(-92, bp16);
		DRAW_TEMP_PICST();
		//^32CB:3467
	} while ((rl = GET_NEXT_RECORD_LINK(rl)) != OBJECT_END_MARKER);
	//^32CB:347B
	return;
}

//^32CB:3B9D
// SPX: _32cb_3b9d renamed DRAW_STATIC_OBJECT
void SkWinCore::DRAW_STATIC_OBJECT(i16 xx, X32 ss, X16 ww)
{
	//^32CB:3B9D
	ENTER(28);
	
	//^32CB:3BA3
	i16 si = xx;
	if (si > 15 || _4976_43e5[RCJ(16,si)] == 0)
		return;
	//^32CB:3BB8
	X16 bp1c = tblCellTilesRoom[si].xsrd.w0;
	ObjectID bp16 = tblCellTilesRoom[si].xsrd.w4;
	X16 bp0c = _4976_43f5[RCJ(16,si)][0];
	X16 bp0e = _4976_43f5[RCJ(16,si)][1];
	const U8 *displayOrder;	// *bp04 
	if (glbTabXAxisDistance[RCJ(23,si)] < 0) { // at left
		//^32CB:3BFF
		displayOrder = tlbDisplayOrderLeft;
	}
	//^32CB:3C06
	else if (glbTabXAxisDistance[RCJ(23,si)] == 0) { // at center
		//^32CB:3C0D
		displayOrder = tlbDisplayOrderCenter;
	}
	else { // at right
		//^32CB:3C14
		displayOrder = tlbDisplayOrderRight;
	}
	//^32CB:3C1C
	for (X16 di = 0; ((si == 0) ? 15 : 25) > di; di++) {
		//^32CB:3C21
		X16 bp08 = displayOrder[di];
		X32 bp1a = (U32(1) << bp08) & ss;
		if (bp1a == 0)
			continue;
		//^32CB:3C4C
		// SPX: some problem here : if (*_4976_5be2)[si] is zero on a door (open) then no item will be drawn!
		if (((*_4976_5be2)[si] & bp1a) != 0) {
			//^32CB:3C68
			DRAW_PUT_DOWN_ITEM(bp16, si, DIR_FROM_5x5_POS(bp08), NULL);
		}
		//^32CB:3C81
		X16 bp10 = _4976_4415[RCJ(25,bp08)][0] +bp0c;
		X16 bp12 = bp0e -_4976_4415[RCJ(25,bp08)][1];
		if (bp1c == 0 || _4976_5a84[bp10][bp12] == si) {
			//^32CB:3CC8
			U8 bp13 = _4976_5aa4[bp10][bp12];
			if (bp13 != 0xff) {
				//^32CB:3CE6
				if (((1 << bp08) & ss) != 0) {
					//^32CB:3D04
					SUMMARY_DRAW_CREATURE(tblCellTilesRoom[bp13].xsrd.w4, bp13, ss);
					_4976_5aa4[bp10][bp12] = 0xff;
					if ((*_4976_5a7c)[si] != 0) {
						//^32CB:3D54
						U16 bp06;
						for (bp06 = di -7; bp06 < di; bp06++) {
							//^32CB:3D5E
							X16 bp0a = displayOrder[bp06];
							if (((1 << bp0a) & *(*_4976_5a7c)) != 0) {
								//^32CB:3D99
								DRAW_FLYING_ITEM(bp16, si, bp0a);
							}
							//^32CB:3DA7
						}
					}
				}
			}
		}
		//^32CB:3DAF
		if (((*_4976_5a7c)[si] & bp1a) != 0) {
			//^32CB:3DCB
			DRAW_FLYING_ITEM(bp16, si, bp08);
		}
		//^32CB:3DD9
	}
	//^32CB:3DED
	if (ww != 0) {
		//^32CB:3DF3
		_32cb_2d8c(bp16, si, ss);
	}
	//^32CB:3E04
	return;
}

//^32CB:24FB
// SPX: _32cb_24fb renamed DRAW_TELEPORTER_TILE; though it can display REFLECTOR
void SkWinCore::DRAW_TELEPORTER_TILE(i16 xx, X16 cls1, X16 cls2)
{
	//^32CB:24FB
	ENTER(60);
	//^32CB:2501
	if (xx > 15)
		return;
	//^32CB:250A
	X8 *bp04 = _4976_42f2[RCJ(23,xx)];
	X16 bp0e = _4976_42d2[RCJ(16,xx)];
	X16 bp10 = 0;
	X8 bp13 = bp04[1] & 0x7f;
	X16 bp16 = ((bp13 & 0x80) != 0) ? 1 : 0;
	i16 bp18 = bp04[2];
	i16 bp1a = bp04[3];
	SRECT bp22;
	if (QUERY_BLIT_RECT(NULL, &bp22, bp0e, &bp18, &bp1a, -1) == NULL)
		return;
	//^32CB:2583
	U8 *bp08;
	Picture bp3c;
	U8 *bp0c;
	if (bp04[1] == 0xff) {
		bp08 = NULL;
	}
	else {
		//^32CB:259A
		bp3c.b8 = U8(cls1);
		bp3c.b9 = U8(cls2);
		bp3c.b11 = bp13;
		bp08 = QUERY_PICST_IMAGE(&bp3c);
		ALLOC_PICT_MEMENT(&bp3c);
		if (bp16 != 0) {
			//^32CB:25D3
			X16 bp12 = ALLOC_TEMP_CACHE_INDEX();
			X16 bp24;
			X16 si;
			bp0c = ALLOC_NEW_PICT(bp12, si = READ_UI16(bp08,-4), bp24 = READ_UI16(bp08,-2), 4);
			bp08 = QUERY_PICT_BITS(&bp3c);
			X16 di = READ_UI16(bp08,-4) & 1;
			if (di != 0)
				//^32CB:2622
				di = 2 -di;
			//^32CB:2629
			FIRE_BLIT_PICTURE(
				bp08, bp0c, 
				ALLOC_TEMP_ORIGIN_RECT(si -di, bp24), 0, 0, 
				si, si, -1, 1, 4, 4, NULL);
			FREE_PICT_MEMENT(&bp3c);
			_0b36_00c3(bp12, &bp3c);
			ALLOC_PICT_MEMENT(&bp3c);
		}
	}
	//^32CB:2688
    bp0c = QUERY_GDAT_IMAGE_ENTRY_BUFF(U8(cls1), U8(cls2), 0x14);
	if (bp04[1] != 0xff) {
		//^32CB:26AA
		bp08 = QUERY_PICT_BITS(&bp3c);
	}
	//^32CB:26BC
	_44c8_20a4(
		bp0c, 
		_4976_4c16, 
		bp08, 
		&bp22, 
		(bp04[0] +RAND01()) << 4, 
		RAND() & 255,
		_4976_00f6, 
		bp10, 
		_32cb_0649(U8(cls1), U8(cls2), 0x14, bp10)
		);
	if (bp04[1] != 0xff) {
		//^32CB:2720
		FREE_PICT_MEMENT(&bp3c);
	}
	//^32CB:272C
	return;
}

//^32CB:4F3B
void SkWinCore::DRAW_WALL(i16 iViewportCell)	// i16 xx
{
	//^32CB:4F3B
	ENTER(12);
	//^32CB:4F41
	U8 iMapGfx = glbMapGraphicsSet;	// bp02
	i16 iColorkey1 = glbSceneColorKey;	// bp06
	i8 iXDist = glbTabXAxisDistance[RCJ(23,iViewportCell)];	// bp07
	i16 iYDist = glbTabYAxisDistance[RCJ(23,iViewportCell)];	// bp0c
	//^32CB:4F5F
	U8 bp01 = (iYDist > 1) ? GDAT_WALL_ORNATE__DATA_6F : GDAT_WALL_ORNATE__DATA_6E;	// SPX TODO: unknown GDAT values! never seen them in any GDAT
	X16 si = 0;
	X16 di = 0;
	U16 bp0a = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, dtWordValue, bp01);
	// SPX: in case gfxset (custom dungeon) is >= 6, bypass these values.
	if (SkCodeParam::bUseFixedMode && glbMapGraphicsSet >= 0 && glbMapGraphicsSet < 6) {
		if (iYDist > 1) { // dist 2,3,4
			//^32CB:4F91
			if (tblCellTilesRoom[4].xsrd.w0 == 0 && (di = (bp0a >> 8)) == 0) {
				//^32CB:4FA5
				di = _4976_463c_tblGfxSet[RCJ(6,glbMapGraphicsSet)];
			}
			//^32CB:4FB2
			if (tblCellTilesRoom[5].xsrd.w0 == 0) {
				//^32CB:4FBD
				si = bp0a & 0xff;
				if (si == 0) {
					//^32CB:4FC9
					si = _4976_4648_tblGfxSet[RCJ(6,glbMapGraphicsSet)];
				}
			}
		}
		//^32CB:4FD8
		else if (iYDist > 0) { // dist 1
			//^32CB:4FDE
			if (tblCellTilesRoom[0].xsrd.w0 == 0 && (di = (bp0a >> 8)) == 0) {
				//^32CB:4FF5
				di = _4976_4624_tblGfxSet[RCJ(6,glbMapGraphicsSet)];
			}
			//^32CB:5002
			if (tblCellTilesRoom[2].xsrd.w0 == 0 && (si = (bp0a & 0xff)) == 0) {
				//^32CB:5019
				si = _4976_4630_tblGfxSet[RCJ(6,glbMapGraphicsSet)];
			}
		}
	}
	//^32CB:5026
	if (di != 0 || si != 0) {
		//^32CB:502E
		TRIM_BLIT_RECT(di, 0, si, 0);
	}
	//^32CB:503C
	X16 bFlip = (iXDist <= 0) ? 0 : 1;	// (bp04) 0 = left / 1 = right
	//^32CB:504E
	if (iViewportCell >= 0x10) { // wall at D4
		//^32CB:5054
		if (iXDist == -2 || iXDist == 2) {
			//^32CB:5060
			bFlip = 0;	// left
		}
		bFlip ^= glbGeneralFlipGraphics;
		bp01 = 0x32;	// 0x32 : D4 wall front
	}
	//^32CB:5071
	else if (glbGeneralFlipGraphics != 0) {
		//^32CB:5078
		bp01 = tlbDoorSideFramesReorder[RCJ(23,iViewportCell)] -80;
		if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_GRAPHICSSET, iMapGfx, dtImage, bp01) == 0) {
			if (iXDist == 0) {
				//^32CB:509F
				bFlip = 1;	// right
			}
			//^32CB:50A4
			bp01 = bp01 +0x72;	// 0x72 is pit side D2 ...
		}
	}
	else {
		//^32CB:50AE
		bp01 = iViewportCell +0x22;	// 0x23 is wall side D0
	}

	// SPX: 2016-11-11 : attempt having See Through Wall masks on wall
#if (XDM1_EXTENDED_SEETHRUWALLS == 1)	// Check for possible thru wall gfx
	{
	
		if (glbGlobalSpellEffects.SeeThruWalls > 0 && iViewportCell == 3)	// front D1 wall
		{
			i16 iColorkey2 = -1;
			ExtendedPicture xExtPicWall;
			i16 iPicture = 0;
			bp01 = 0x025;	// D1 wall

			iColorkey1 = 10;	// would be dark green / pass through (see what's behind wall)
			iColorkey2 = 9;		// would be cyan / transparent (see wall)
			
			iPicture = QUERY_MULTILAYERS_PIC(&xExtPicWall, GDAT_CATEGORY_GRAPHICSSET, iMapGfx, bp01, 0x40, 0x40, 0, bFlip, iColorkey1, -1);
			//iPicture = QUERY_MULTILAYERS_PIC(&xExtPicWall, GDAT_CATEGORY_GRAPHICSSET, 0xFE, 0xF2, 0x40, 0x40, 0, bFlip, iColorkey1, -1); // test wall
			DRAW_PICST(&xExtPicWall);
			FREE_PICT_MEMENT(&xExtPicWall);

			X16 iScale = 0x40;
			X16 bp20 = 0x40;
			X16 bp22 = 0x40;;
			//--- Put the metrics of that image to the TEMP image
			if (iScale == 0x40)
			{
				i16 bp1c;
				i16 bp1e;
				
				QUERY_GDAT_IMAGE_METRICS(GDAT_CATEGORY_GRAPHICSSET, iMapGfx, bp01, &bp1c, &bp1e);	// 0xe
				bp20 = _32cb_48d5(glbTempPicture.width, bp1c);
				bp22 = _32cb_48d5(glbTempPicture.height, bp1e);
				
			}
			else {
				bp22 = bp20 = iScale;
			}

			//------ part to get and draw the see thru walls
			if (1)
			{
				U16 iWallSetIndex = iMapGfx;
			
				U16 iWallSeeThruMask = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, iMapGfx, dtImage, GDAT_GFXSET_SEE_THRU);
				if (iWallSeeThruMask == (U16)-1) // not found, get the default one
					iWallSetIndex = GDAT_ITEM_DEFAULT_INDEX;	// 0xFE
				
				i16 iRectNo = QUERY_CREATURE_BLIT_RECTI(3, 11, 0);	// That gets currently the best result for positionning 'see through wall' mask

				QUERY_TEMP_PICST(bFlip, bp20, bp22, 0, 0, 
					(glbIsPlayerMoving != 0) ? -iYDist : 0, 
					iRectNo,
					-1, iColorkey2, iColorkey1,
					GDAT_CATEGORY_GRAPHICSSET, iWallSetIndex, GDAT_GFXSET_SEE_THRU
					);

				glbTempPicture.pb44 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(iPicture));
				DRAW_TEMP_PICST();
			}
			//------

			//--- Get correct pass through color
			xExtPicWall.colorKeyPassThrough = xExtPicWall.b58[iColorkey1];
			xExtPicWall.w56 = 0;
			_0b36_00c3(iPicture, &xExtPicWall);
			COPY_MEMORY(&xExtPicWall, &glbTempPicture, sizeof(ExtendedPicture));
			glbTempPicture.pb44 = _4976_4c16;
			glbTempPicture.rectNo = 3 + 0x2BE;
			DRAW_TEMP_PICST();
			if (iPicture >= 0)
				FREE_TEMP_CACHE_INDEX(iPicture);
		}
		else
		{
			QUERY_TEMP_PICST(bFlip, 0x40, 0x40, 0, 0, (glbIsPlayerMoving != 0) ? -iYDist : 0, iViewportCell +0x2be, 0xffff, iColorkey1, -1, GDAT_CATEGORY_GRAPHICSSET, iMapGfx, bp01);
			DRAW_TEMP_PICST(); // draw wall
		}

	}
#else // end XDM1_EXTENDED_SEETHRUWALLS
	//^32CB:50B6
	QUERY_TEMP_PICST(bFlip, 0x40, 0x40, 0, 0, (glbIsPlayerMoving != 0) ? -iYDist : 0, iViewportCell +0x2be, 0xffff, iColorkey1, -1, GDAT_CATEGORY_GRAPHICSSET, iMapGfx, bp01);
	DRAW_TEMP_PICST(); // draw wall
#endif
	_098d_0c45();
	//^32CB:50FA
	return;
}
//^32CB:50FE
// SPX: _32cb_50fe renamed DRAW_WALL_TILE
void SkWinCore::DRAW_WALL_TILE(i16 iViewportCell)	// i16 xx
{
	//^32CB:50FE
	ENTER(0);
	//^32CB:5102
	i16 iLocalViewportCell = iViewportCell;	//i16 si = xx;
	DRAW_WALL(iLocalViewportCell);
	switch (_4976_4654[RCJ(23,iLocalViewportCell)]) {
		case 1: //^_5121	// front vision
			//^32CB:5121
			DRAW_WALL_ORNATE(iLocalViewportCell, 0, 1);
			break;
		case 3: //^_5127	// side
			//^32CB:5127
			DRAW_WALL_ORNATE(iLocalViewportCell, 0, 1);
			goto _5133;
		case 2: //^_5133	// side
			//^32CB:5133
_5133:
            DRAW_WALL_ORNATE(iLocalViewportCell, glbTabXAxisDistance[RCJ(23,iLocalViewportCell)], 1);
			break;
	}
	//^32CB:5143
	return;
}

//^32CB:5146
// SPX: _32cb_5146 renamed DRAW_DUNGEON_TILES
void SkWinCore::DRAW_DUNGEON_TILES()
{
	//^32CB:5146
	ENTER(10);
	//^32CB:514C
	U32 bp04 = 0x01ffffff;
	i16 bp08 = 0;
	for (; bp08 < 20; bp08++) {
		static const char * pszTblDrawOrder = {"D4L2,D4R2,D4L1,D4R1,D4  ,D3L2,D3R2,D3L1,D3R1,D3  ,D2L2,D2R2,D2L1,D2R1,D2  ,D1L1,D1R1,D1  ,D0L1,D0R1"};
		// D4L2| 0|19
		// D4R2| 1|20
		// D4L1| 2|17
		// D4R1| 3|18
		// D4  | 4|16
		// D3L2| 5|14
		// D3R2| 6|15
		// D3L1| 7|12
		// D3R1| 8|13
		// D3  | 9|11
		// D2L2|10| 9
		// D2R2|11|10
		// D2L1|12| 7
		// D2R1|13| 8
		// D2  |14| 6
		// D1L1|15| 4
		// D1R1|16| 5
		// D1  |17| 3
		// D0L1|18| 1
		// D0R1|19| 2
		SkD((DLV_DBG_CELL, "DBG: Summary#%02d (%.4s)\n", (Bitu)bp08, &pszTblDrawOrder[5*bp08]));

		//^32CB:515E
		IBMIO_USER_INPUT_CHECK();
		i16 si;
		X16 bp06 = tblCellTilesRoom[si = _4976_466b[RCJ(20,bp08)]].xsrd.w0;
		ObjectID di = tblCellTilesRoom[si].xsrd.id4();
		if (si > 15) { // at D4
			//^32CB:5196
			X16 bp0a = bp06;
			switch (bp0a) {
				case 0x00://^51B5
					//^32CB:51B5
					if (di != OBJECT_END_MARKER) {
						//^32CB:51BA
						SUMMARY_DRAW_CREATURE(di, si, 0x01ffffff);
					}
					//^32CB:51C8
					DRAW_WALL_TILE(si);
					continue;
				case 0x01://^51D1
				case 0x02://^51D1
				case 0x05://^51D1
					//^32CB:51D1
					DRAW_EXTERNAL_TILE(si);
					goto _51d7;
				case 0x11://^51D7
					//^32CB:51D7
_51d7:
					if (di == OBJECT_END_MARKER)
						continue;
					//^32CB:51DF
					SUMMARY_DRAW_CREATURE(di, si, 0x01ffffff);
					continue;
			}
			continue;
		}
		//^32CB:51F0
		if (si == 3) // at D1F
			//^32CB:51F5
			_4976_4dda = bp06;
		switch (bp06) {
			case 0://^520D // wall
				//^32CB:520D
				if (di != OBJECT_END_MARKER) {
					//^32CB:5212
					SUMMARY_DRAW_CREATURE(di, si, 0x01ffffff);
				}
				//^32CB:5220
				//^32CB:5222
				DRAW_WALL_TILE(si);
				continue;
			case 17://^5224
				//^32CB:5224
				DRAW_DOOR_TILE(si);
				continue;
			case 18://^522B
				//^32CB:522B
				DRAW_STAIRS_SIDE(si);
				goto _52d1;
			case 19://^5233
				//^32CB:5233
				DRAW_STAIRS_FRONT(si);
				goto _52d1;
			case 2://^523B // pit
				//^32CB:523B
				DRAW_PIT_TILE(si);
				goto _52b1;
			case 16://^5243	// seeing door frame slot in front from door tile
				//^32CB:5243
				if (si == 3) {
					//^32CB:5248
					// Check if this door type has door frames of not. if so, then draw the door frame
					if (QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, glbMapDoorType[GET_ADDRESS_OF_RECORD0(tblCellTilesRoom[3].xsrd.w6[2])->DoorType()], dtWordValue, GDAT_DOOR_NO_FRAMES) == 0) {
						//^32CB:5279
						QUERY_TEMP_PICST(
							glbGeneralFlipGraphics, 0x2b, 0x2b, 0, 0, 2, QUERY_CREATURE_BLIT_RECTI(3, 2, 0), 0xffff, 
							glbSceneColorKey, -1, GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, 6
							);
						DRAW_TEMP_PICST();
					}
				}
				goto _52b1;
			case 1://^52B1 // floor
			case 5://^52B1 // teleporter
				//^32CB:52B1
_52b1:
				bp04 = (DRAW_EXTERNAL_TILE(si) != 0) ? 0x01fffc00 : 0x01ffffff;
				DRAW_PIT_ROOF(si);
				//^32CB:52D1
_52d1:
				DRAW_STATIC_OBJECT(si, bp04, 1);
				if (bp06 == 5)	// teleporter
				{
					//^32CB:52E8
					DRAW_TELEPORTER_TILE(si, GDAT_CATEGORY_TELEPORTERS, 0); // DRAW_TELEPORTER_TILE(si, 0x18, 0)
				}
				break;
			case 3://^52F4 // stair case
			case 4://^52F4 // door
			case 6://^52F4 // false wall
			case 7://^52F4 // map exit
			case 8://^52F4
			case 9://^52F4
			case 10://^52F4
			case 11://^52F4
			case 12://^52F4
			case 13://^52F4
			case 14://^52F4
			case 15://^52F4
				break;
		}
		//^32CB:52F4
	}
	//^32CB:5300
	return;
}

//^3E74:5A0C
void SkWinCore::COPY_SKIMAGE_HEADER(const U8 *xx, U8 *yy)
{
	//^3E74:5A0C
	ENTER(0);
	//^3E74:5A0F
	COPY_MEMORY(
		&xx[-6],
		&yy[-6],
		6
		);
	//^3E74:5A41
	return;
}

//^0B36:15E5
void SkWinCore::APPLY_MIRROR_FLIP_TO(const U8 *blitSrc, U8 *blitDst, X16 mirrorFlip)
{
	//^0B36:15E5
	ENTER(2);
	//^0B36:15EB
	X16 si = READ_UI16(blitSrc,-4);
	X16 bp02 = READ_UI16(blitSrc,-2);
	X16 di = READ_UI16(blitSrc,-6);
	COPY_SKIMAGE_HEADER(blitSrc, blitDst);
	FIRE_BLIT_PICTURE(blitSrc, blitDst, ALLOC_TEMP_ORIGIN_RECT(si, bp02), 0, 0, si, si, -1, mirrorFlip, di, di, NULL);
	//^0B36:1643
	return;
}

//^32CB:0753
void SkWinCore::DRAW_MIRRORED_PIC(const U8 *blitSrc, U8 *blitDst)
{
	//^32CB:0753
	ENTER(0);
	//^32CB:0756
	APPLY_MIRROR_FLIP_TO(blitSrc, blitDst, 1);
	//^32CB:076C
	return;
}

//^32CB:0B11
// SPX: _32cb_0b11 renamed DRAW_RAIN
void SkWinCore::DRAW_RAIN()
{
	//^32CB:0B11
	ENTER(20);
	//^32CB:0B16
	if (glbRainStrength == 0 || tlbRainScene[RCJ(5,glbRainSceneType)][2] == 0)
		return;
	//^32CB:0B31
	i16 bp0a = _4976_00f6;
	i16 bp0c = _4976_00f8;
	SRECT bp14;
	if (QUERY_BLIT_RECT(NULL, &bp14, 0x2be, &bp0a, &bp0c, -1) == NULL)
		return;
	//^32CB:0B64
	U8 bp07;
	U16 bp06;
	QUERY_RAINFALL_PARAM(&bp07, &bp06);
	U8 *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x17, glbMapGraphicsSet, bp07);
	ALLOC_IMAGE_MEMENT(GDAT_CATEGORY_ENVIRONMENT, glbMapGraphicsSet, bp07);
	X16 si;
	if (bp06 == 1) {
		//^32CB:0BA5
		si = ALLOC_TEMP_CACHE_INDEX();
		ALLOC_NEW_PICT(si, READ_UI16(bp04,-4), READ_UI16(bp04,-2), 4);
		bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x17, glbMapGraphicsSet, bp07);
		DRAW_MIRRORED_PIC(bp04, reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(si)));
		bp04 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(si));
	}
	//^32CB:0BFD
	_44c8_20a4(bp04, _4976_4c16, NULL, &bp14, READ_UI16(bp04,-4) * READ_UI16(bp04,-2) -40 -(RAND() & 0x1f), RAND() & 255, _4976_00f6, 0,
		_32cb_0649(GDAT_CATEGORY_ENVIRONMENT, glbMapGraphicsSet, bp07, 0)
		);
	if (bp06 == 1) {
		//^32CB:0C61
		FREE_TEMP_CACHE_INDEX(si);
	}
	//^32CB:0C68
	FREE_IMAGE_MEMENT(GDAT_CATEGORY_ENVIRONMENT, glbMapGraphicsSet, bp07);
	//^32CB:0C7A
	return;
}

//^32CB:5340
// SPX: _32cb_5340 renamed DRAW_PLAYER_TILE (which display the current tile the player is on)
void SkWinCore::DRAW_PLAYER_TILE()
{
	//^32CB:5340
	ENTER(14);
	//^32CB:5346
	X16 bp06;
	X16 bp0e;
	bp06 = bp0e = tblCellTilesRoom->xsrd.w0;
	X16 di;
	X16 si;
	switch (bp0e) {
		case 0x10://^536D
			{
				//^32CB:536D
				Door *door = GET_ADDRESS_OF_RECORD0(tblCellTilesRoom->xsrd.w6[1]);	//*bp04
				if (QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, glbMapDoorType[door->DoorType()], dtWordValue, GDAT_DOOR_NO_FRAMES) != 0)
					break;
				//^32CB:53A6
				DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, GDAT_GFXSET_DOOR_FRAME_FRONT_D1, QUERY_CREATURE_BLIT_RECTI(0, 2, 0), glbSceneColorKey, glbGeneralFlipGraphics);
				break;
			}
		case 0x13://^53CE	// on stairs tile (coming from stairs)
			//^32CB:53CE
			si = tblCellTilesRoom->xsrd.w6[0];
			DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet
//					, (si != 0) ? 0x39 : 0x4D
				, (si != 0) ? GDAT_GFXSET_STAIRS_RAMP_UP_S0_L : GDAT_GFXSET_STAIRS_RAMP_DOWN_S0_L
				, (si != 0) ? 0x32b : 0x338, glbSceneColorKey, 0);
			DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet
//					, (si != 0) ? 0x3a : 0x4e
				, (si != 0) ? GDAT_GFXSET_STAIRS_RAMP_UP_S0_R : GDAT_GFXSET_STAIRS_RAMP_DOWN_S0_R
				, (si != 0) ? 0x32c : 0x339, glbSceneColorKey, 0);
			break;
		case 0x02://^542E
			//^32CB:542E
			DRAW_PIT_TILE(0);
			break;
		case 0x11://^5437
			//^32CB:5437
			DRAW_DOOR_TILE(0);
			di = 0;
			goto _5495;
	}
	//^32CB:5442
	DRAW_PIT_ROOF(0);
	TELE_inf bp0c;
	if ((di = GET_TELEPORTER_DETAIL(&bp0c, tblCellTilesRoom->posx, tblCellTilesRoom->posy)) == 0) {
		//^32CB:546B
		DRAW_EXTERNAL_TILE(0); // this is where floor pad is displayed
	}
	//^32CB:5472
	DRAW_STATIC_OBJECT(0, 0x01ffffff, 0);
	if (bp06 == 5)
	{
		//^32CB:5488
		DRAW_TELEPORTER_TILE(0, 0x18, 0);
	}
	//^32CB:5495
_5495:
	DRAW_RAIN();
	if (di != 0) {
		//^32CB:549D
		DRAW_EXTERNAL_TILE(0);
	}
	//^32CB:54A4
	_32cb_2d8c(tblCellTilesRoom->xsrd.w4, 0, 0x01ffffff);
	//^32CB:54BA
	return;
}

//^32CB:5D0D
// SPX: _32cb_5d0d renamed DISPLAY_VIEWPORT
void SkWinCore::DISPLAY_VIEWPORT(Bit16u dir, i16 xx, i16 yy)
{
	//^32CB:5D0D
	ENTER(6);
	//^32CB:5D13
	IBMIO_USER_INPUT_CHECK();
	// SPX: glbLightLevel is between 0 (light) and 5 (dark). Palette is thereafter controlled by value between 0 (light) and 64 (dark)
	// Having *10 makes 0 to 50 -> loss of darkest colors. Having *13 makes 0 to 65 -> full range (full darkness)
	_4976_5a88 = glbLightLevel * 10;	
	//_4976_5a88 = glbLightLevel * 12;	// SPX: * 13 seems more appropriate to get darkest values
	_4976_5aa0 = dir;
	_4976_5a9c = xx;
	_4976_5a9e = yy;
	tblCellTilesRoom = reinterpret_cast<CellTileDetail *>(ALLOC_MEMORY_RAM(sizeof(CellTileDetail) * 23, afDefault, 1024));
	_4976_5a84 = reinterpret_cast<U8 (*)[17]>(ALLOC_MEMORY_RAM(358, afZeroMem, 1024));
	_4976_5aa4 = reinterpret_cast<U8 (*)[17]>(ALLOC_MEMORY_RAM(358, afDefault, 1024));
	FILL_STR(*_4976_5aa4, 358, 255, 1);
	_4976_5a7c = reinterpret_cast<U32 (*)[23]>(ALLOC_MEMORY_RAM(4*23, afZeroMem, 1024));
	_4976_5be2 = reinterpret_cast<U32 (*)[23]>(ALLOC_MEMORY_RAM(4*23, afZeroMem, 1024));
	_4976_4bc8 = 1;
	INIT_BACKBUFF();
	_4976_4dde = 9;
	i16 si;

	SkD((DLV_MOVE, "DISPLAY_VIEWPORT: dir:%d x:%d y:%d darkness:%d\n"
		, dir
		, xx
		, yy
		, _4976_5a88));
	

	//^32CB:5DDB
	for (si = 0; si < 9; si++) {
		//^32CB:5DDF
		_4976_4d1a[si].w8 = OBJECT_NULL;
		//^32CB:5DEE
	}
	//^32CB:5DF4
	IBMIO_USER_INPUT_CHECK();
	_4976_5aa2 = (_4976_4ddc != 0 || glbTableToMove != OBJECT_NULL) ? 1 : 0;
	CHANCE_TABLE_OPERATION();
	//^32CB:5E15
	for (si = 0x16; si >= 0; si--) {
		//^32CB:5E1A
		_32cb_4185_TILE_ROOM(xx, yy, si, _4976_5aa0);
		//^32CB:5E2C
	}

	//   |--|--|--|--|--|--|--|
	// 4 |21|19|17|16|18|20|22|
	//   |--|--|--|--|--|--|--|
	// 3 |  |14|12|11|13|15|  |
	//   |--|--|--|--|--|--|--|
	// 2 |  | 9| 7| 6| 8|10|  |
	//   |--|--|--|--|--|--|--|
	// 1 |  |  | 4| 3| 5|  |  |
	//   |--|--|--|--|--|--|--|
	// 0 |  |  | 1| 0| 2|  |  |
	//   |--|--|--|--|--|--|--|
	SkD((DLV_DBG_CELL, "DBG: U r at (%2u,%2u,%2u) %u\n"
		"|-------|-------|-------|-------|-------|-------|-------|\n"
		"|       |%02X %04X|%02X %04X|%02X %04X|%02X %04X|%02X %04X|       |\n"
		"|-------|-------|-------|-------|-------|-------|-------|\n"
		"|       |%02X %04X|%02X %04X|%02X %04X|%02X %04X|%02X %04X|       |\n"
		"|-------|-------|-------|-------|-------|-------|-------|\n"
		"|       |%02X %04X|%02X %04X|%02X %04X|%02X %04X|%02X %04X|       |\n"
		"|-------|-------|-------|-------|-------|-------|-------|\n"
		"|       |       |%02X %04X|%02X %04X|%02X %04X|       |       |\n"
		"|-------|-------|-------|-------|-------|-------|-------|\n"
		"|       |       |%02X %04X|%02X %04X|%02X %04X|       |       |\n"
		"|-------|-------|-------|-------|-------|-------|-------|\n"
		, (Bitu)glbPlayerMap, (Bitu)glbPlayerPosX, (Bitu)glbPlayerPosY, (Bitu)glbPlayerDir
		, (Bitu)tblCellTilesRoom[19].xsrd.w0, (Bitu)tblCellTilesRoom[19].xsrd.w4
		, (Bitu)tblCellTilesRoom[17].xsrd.w0, (Bitu)tblCellTilesRoom[17].xsrd.w4
		, (Bitu)tblCellTilesRoom[16].xsrd.w0, (Bitu)tblCellTilesRoom[16].xsrd.w4
		, (Bitu)tblCellTilesRoom[18].xsrd.w0, (Bitu)tblCellTilesRoom[18].xsrd.w4
		, (Bitu)tblCellTilesRoom[20].xsrd.w0, (Bitu)tblCellTilesRoom[20].xsrd.w4

		, (Bitu)tblCellTilesRoom[14].xsrd.w0, (Bitu)tblCellTilesRoom[14].xsrd.w4
		, (Bitu)tblCellTilesRoom[12].xsrd.w0, (Bitu)tblCellTilesRoom[12].xsrd.w4
		, (Bitu)tblCellTilesRoom[11].xsrd.w0, (Bitu)tblCellTilesRoom[11].xsrd.w4
		, (Bitu)tblCellTilesRoom[13].xsrd.w0, (Bitu)tblCellTilesRoom[13].xsrd.w4
		, (Bitu)tblCellTilesRoom[15].xsrd.w0, (Bitu)tblCellTilesRoom[15].xsrd.w4

		, (Bitu)tblCellTilesRoom[ 9].xsrd.w0, (Bitu)tblCellTilesRoom[ 9].xsrd.w4
		, (Bitu)tblCellTilesRoom[ 7].xsrd.w0, (Bitu)tblCellTilesRoom[ 7].xsrd.w4
		, (Bitu)tblCellTilesRoom[ 6].xsrd.w0, (Bitu)tblCellTilesRoom[ 6].xsrd.w4
		, (Bitu)tblCellTilesRoom[ 8].xsrd.w0, (Bitu)tblCellTilesRoom[ 8].xsrd.w4
		, (Bitu)tblCellTilesRoom[10].xsrd.w0, (Bitu)tblCellTilesRoom[10].xsrd.w4

		, (Bitu)tblCellTilesRoom[ 4].xsrd.w0, (Bitu)tblCellTilesRoom[ 4].xsrd.w4
		, (Bitu)tblCellTilesRoom[ 3].xsrd.w0, (Bitu)tblCellTilesRoom[ 3].xsrd.w4
		, (Bitu)tblCellTilesRoom[ 5].xsrd.w0, (Bitu)tblCellTilesRoom[ 5].xsrd.w4

		, (Bitu)tblCellTilesRoom[ 1].xsrd.w0, (Bitu)tblCellTilesRoom[ 1].xsrd.w4
		, (Bitu)tblCellTilesRoom[ 0].xsrd.w0, (Bitu)tblCellTilesRoom[ 0].xsrd.w4
		, (Bitu)tblCellTilesRoom[ 2].xsrd.w0, (Bitu)tblCellTilesRoom[ 2].xsrd.w4
		));

#if DLV_DBG_CEL2
	{
		char mass[21*54 +1];
		// 54 x 21
		strcpy(mass,
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"----+----+-----+----+----+ +----+----+-----+----+----\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"----+----+-----+----+----+ +----+----+-----+----+----\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"----+----+-----+----+----+ +----+----+-----+----+----\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"----+----+-----+----+----+ +----+----+-----+----+----\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\0"
			);
		static const U8 tblX[] = {0,1,2,3, 5,6,7,8, 10,11,12,13,14, 16,17,18,19, 21,22,23,24};
		static const U8 tblY[] = {0, 2,3,4,5, 7,8,9,10, 12,13,14,15, 17,18,19,20};
		for (int yc=0; yc<17; yc++) {
			int xc;
			for (xc=0; xc<21; xc++) {
				U8 v = U8(_4976_5aa4[xc][16 -yc]);
				char t = '.';
				if (0 <= v && v <= 9) t = v + '0';
				else if (10 <= v && v <= 15) t = v + 'A' -10;

				mass[54*tblY[yc] +tblX[xc]] = t;
			}
			for (xc=0; xc<21; xc++) {
				U8 v = U8(_4976_5a84[xc][16 -yc] & 15);
				char t = '.';
				if (0 <= v && v <= 9) t = v + '0';
				else if (10 <= v && v <= 15) t = v + 'A' -10;

				mass[54*tblY[yc] +28 +tblX[xc]] = t;
			}
		}
		SkD((DLV_DBG_CEL2, "DBG: L=enemy map, R=movable map\n%s\n", mass));
	}
#endif

	//^32CB:5E31
	X16 di = 0;
	X16 bp02 = 0;
	X8 bp03 = 0xff;
	if (tblCellTilesRoom[7].xsrd.w0 == 0 && tblCellTilesRoom[6].xsrd.w0 == 0 && tblCellTilesRoom[8].xsrd.w0 == 0) // wall at D2
		//^32CB:5E57
		bp03 = 0x71;
	if (tblCellTilesRoom[4].xsrd.w0 == 0 && tblCellTilesRoom[3].xsrd.w0 == 0 && tblCellTilesRoom[5].xsrd.w0 == 0) // wall at D1
		//^32CB:5E74
		bp03 = 0x70;
	//^32CB:5E78
	if (bp03 != 0xff) { // trim ceil(bp02) and floor(di) to omit hidden area by drawing wall
		U16 bp06 = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, dtWordValue, bp03);
		di = bp06 >> 8;
		bp02 = bp06 & 0xff;
		// SPX: if custom dungeon/GDAT use more than 6 gfxsets, better bypass these values from tables
		if (SkCodeParam::bUseFixedMode && glbMapGraphicsSet >= 0 && glbMapGraphicsSet <= 5)
		{
			if (di == 0) {
				if (bp03 == 0x71) {
					di = _4976_4718_tblGfxSet[RCJ(6,glbMapGraphicsSet)]; // D2 to D0
				}
				else {
					//^32CB:5EBC
					di = _4976_4700_tblGfxSet[RCJ(6,glbMapGraphicsSet)]; // D1 to D0
				}
			}
			//^32CB:5EC9
			if (bp02 == 0) {
				if (bp03 == 0x71) {
					//^32CB:5ED5
					bp02 = _4976_470c_tblGfxSet[RCJ(6,glbMapGraphicsSet)]; // envisible D2 to D0
				}
				else {
					//^32CB:5EE4
					bp02 = _4976_46f4_tblGfxSet[RCJ(6,glbMapGraphicsSet)]; // envisible D1 to D0
				}
			}
		}
	}
	//^32CB:5EF4
	TRIM_BLIT_RECT(0, 0, 0, bp02);
	if (SET_GRAPHICS_FLIP_FROM_POSITION(0x20, dir, xx, yy) != 0) {
		//^32CB:5F1B
		DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, GDAT_GFXSET_CEIL, 0x2bc, -1, 1); // draw ceil
	}
	else {
		//^32CB:5F1F
		DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, GDAT_GFXSET_CEIL, 0x2bc, -1, 0); // draw ceil
	}
	//^32CB:5F35
	_098d_0c45();
	TRIM_BLIT_RECT(0, di, 0, 0);
	if (SET_GRAPHICS_FLIP_FROM_POSITION(1, dir, xx, yy) != 0) {
		//^32CB:5F5F
		DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, GDAT_GFXSET_FLOOR, 0x2bd, -1, 1); // draw floor
	}
	else {
		//^32CB:5F63
		DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, GDAT_GFXSET_FLOOR, 0x2bd, -1, 0); // draw floor
	}
	//^32CB:5F79
	_098d_0c45();
	glbGeneralFlipGraphics = SET_GRAPHICS_FLIP_FROM_POSITION(0, dir, xx, yy);
	IBMIO_USER_INPUT_CHECK();
	ENVIRONMENT_DISPLAY_ELEMENTS(dir, xx, yy);
	_32cb_5c67();
	_32cb_5a8f();
	DRAW_DUNGEON_TILES(); // render walls, flying items
	IBMIO_USER_INPUT_CHECK();
	DRAW_PLAYER_TILE(); // render current player tile
	IBMIO_USER_INPUT_CHECK();
#if UseAltic
	DEALLOC_UPPER_MEMORY(92);
	DEALLOC_UPPER_MEMORY(92);
	DEALLOC_UPPER_MEMORY(358);
	DEALLOC_UPPER_MEMORY(358);
	DEALLOC_UPPER_MEMORY(414);
#else
	DEALLOC_UPPER_MEMORY(414+358+358+92+92);
#endif
	//^32CB:5FCE
	return;
}

//^2481:0002
void SkWinCore::_2481_0002()
{
	//^2481:0002
	ENTER(0);
	//^2481:0005
	FIRE_SELECT_PALETTE_SET(0);
	if (_4976_3d2c != 0) {
		//^2481:0014
		FIRE_BLIT_PICTURE(
			glbImageMenuScreen, _4976_4964, ALLOC_TEMP_ORIGIN_RECT(glbScreenWidth, glbScreenHeight),
			0, 0, glbScreenWidth, glbScreenWidth, -1, 0, 8, 8, NULL);
	}
	else {
		//^2481:0057
		DRAW_GAMELOAD_DIALOGUE_TO_SCREEN(glbImageMenuScreen, 2, -1, _4976_52ba);
	}
	//^2481:0073
	FIRE_SELECT_PALETTE_SET(1);
	//^2481:007B
	return;
}


//^0B36:0A3F
void SkWinCore::DRAW_DEF_PICT(ExtendedPicture *ref)
{
	// draw almost all pictures:
	// a) dungeon viewport: wallset/scene graphics, ornate graphics, spell graphics,
	//		flying items (passing/coming/crossing/placing)
	// b) inventory screen: wall paper, zzz button, X button, "water" text, "food" text, 
	//		resurrection lever, disk button, and so on
	// c) inventory icons: eye, mouth, item icons

	//^0B36:0A3F
	ENTER(12);
	//^0B36:0A45
	if (ref->width <= 0 || ref->height <= 0)
		//^0B36:0A59
		return;
	//^0B36:0A5C
	Bit8u *bp04 = QUERY_PICT_BITS(ref);
	//^0B36:0A6E
	Bit16u bp06 = ref->rectNo;
	//^0B36:0A78
	i16 bp08;
	i16 bp0a;
	if (bp06 == 0xffff) {
		//^0B36:0A7D
		bp08 = ref->w32;
		bp0a = ref->w34;
	}
	else {
		//^0B36:0A8D
		if ((bp06 & 0x8000) == 0 || ref->w28 != 0 || ref->w30 != 0) {
			//^0B36:0AA5
			bp06 |= 0x8000;
			//^0B36:0AAE
			bp08 = ref->w32 + ref->w28;
			bp0a = ref->w34 + ref->w30;
		}
		else {
			//^0B36:0AC6
			bp08 = ref->width;
			bp0a = ref->height;
		}
		//^0B36:0AD4
		if (QUERY_BLIT_RECT(bp04, &ref->rc36, bp06, &bp08, &bp0a, ref->w26) == NULL)
			//^0B36:0B07
			return;
	}
	//^0B36:0B0A
	bp08 += ref->w14;
	bp0a += ref->w16;
	i16 si = READ_UI16(bp04,-4);
	i16 di = ref->rc36.cx +bp08;
	//^0B36:0B2E
	if (si > di && (ref->mirrorFlip & 0x0001) != 0) {
		//^0B36:0B3A
		si -= di;
	}
	else {
		//^0B36:0B3E
		si = 0;
	}
	//^0B36:0B40
	if ((ref->mirrorFlip & 0x0001) != 0) {
		//^0B36:0B4B
		bp08 = 0;
	}
	//^0B36:0B50
	bp08 += si;
	//^0B36:0B53
	si = READ_I16(bp04,-2);
	//^0B36:0B5A
	di = ref->rc36.cy +bp0a;
	//^0B36:0B66
	if (si > di && (ref->mirrorFlip & 0x0002) != 0) {
		//^0B36:0B72
		si -= di;
	}
	else {
		//^0B36:0B76
		si = 0;
	}
	//^0B36:0B78
	if ((ref->mirrorFlip & 0x0002) != 0) {
		//^0B36:0B83
		bp0a = 0;
	}
	//^0B36:0B88
	bp0a += si;
	//^0B36:0B8B
	Bit16u bp0c;
	if (ref->pb44 == _4976_4964) {
		//^0B36:0BA2
		si = glbScreenWidth;
		di = glbScreenHeight;
		bp0c = 8;
	}
	else {
		//^0B36:0BB1
		si = READ_I16(ref->pb44,-4);
		//^0B36:0BBC
		di = READ_I16(ref->pb44,-2);
		//^0B36:0BC7
		bp0c = READ_I16(ref->pb44,-6);
	}
	//^0B36:0BD5
	if (ref->colorKeyPassThrough != -2) {
		//^0B36:0BDF
		FIRE_BLIT_PICTURE(
			bp04,
			ref->pb44,
			&ref->rc36,
			bp08,
			bp0a,
			READ_UI16(bp04,-4),
			si,
			ref->colorKeyPassThrough,
			ref->mirrorFlip,
			READ_UI16(bp04,-6),
			bp0c,
			(ref->w56 == 0) ? NULL : ref->b58
			);
	}
	//^0B36:0C3D
	ref->w32 = bp08;
	ref->w34 = bp0a;
	//^0B36:0C4E
	return;
}

//^0B36:0139
Bit8u *SkWinCore::QUERY_PICT_BITS(Picture *ref)
{
	//^0B36:0139
	ENTER(0);
	//^0B36:013D
	Bit16u si = ref->w4;
	//^0B36:0144
	if ((si & 0x0004) != 0) {
		//^0B36:014A
		return ref->pb0 = QUERY_GDAT_IMAGE_ENTRY_BUFF(ref->b8, ref->b9, ref->b11);
	}
	//^0B36:016D
	else if ((si & 0x0008) != 0) {
		//^0B36:0173
		return ref->pb0 = QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(ref->w12);
	}
	else {
		//^0B36:0182
		return ref->pb0;
	}
}

//^0B36:11C0
// TODO: image related ?
void SkWinCore::_0b36_11c0(ExtendedPicture *xx, sk3f6c *yy, Bit16u ss, i16 colorkey2)
{
	//^0B36:11C0
    ENTER(12);
	//^0B36:11C4
	xx->pb44 = reinterpret_cast<Bit8u *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(yy->w0));
	//^0B36:11DB
	SRECT bp08;
	i16 bp0a;
	i16 bp0c;
	if (ss == 0xffff) {
		ATLASSERT(false); // FTL's miss logic?

		//^0B36:11E1
		COPY_MEMORY(&xx->rc36, &bp08, 8);
		//^0B36:11FC
		goto _123d;
	}
	else {
		//^0B36:11FE
		bp0a = xx->width;
		bp0c = xx->height;
		//^0B36:120F
		if (QUERY_BLIT_RECT(QUERY_PICT_BITS(xx), &bp08, ss, &bp0a, &bp0c, -1) != NULL) {
			//^0B36:123D
_123d:
			xx->width += bp0a;
			xx->height += bp0c;
			//^0B36:124E
			OFFSET_RECT(yy, &bp08, &xx->rc36);
			//^0B36:126A
			xx->rectNo = 0xffff;
			xx->colorKeyPassThrough = colorkey2;
			//^0B36:127A
			DRAW_DEF_PICT(xx);
			//^0B36:1286
			_0b36_0d67(yy, &bp08);
		}
	}
	//^0B36:1298
	return;
}

//^0B36:0036
i16 SkWinCore::CALC_STRETCHED_SIZE(i16 val, i16 fact64)
{
	//^0B36:0036
	ENTER(0);
	//^0B36:0039
	return (i16(val * fact64) + (fact64 >> 1)) >> 6;
}


//------------------------------------------------------------------------------


//^44C8:08AE
void SkWinCore::FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP(Bit16u offSrc, Bit16u offDst, Bit16u width, i16 colorkey)
{
	// TODO: ìØàÍîüêîÇÃà⁄êAñ‚ëË
	//^44C8:08AE
	ENTER(0);
	//^44C8:08B3
	const Bit8u *bx = glbBlitPalette16;
	Bit8u *di = _4976_5e6a + offDst;
	Bit8u dh = Bit8u(colorkey);
	Bit16u cx = width;
	bool carry = (offSrc & 1) ? true : false;
	offSrc >>= 1;
	Bit8u *si = _4976_5e64 + offSrc;
	//^44C8:08D4
	if (carry) {
		//^44C8:08D6
		Bit8u al = *si; si++;
		al &= 0x0f;
		if (al == dh) {
			di++;
		}
		else {
			*di = bx[al]; di++;
		}
		//^44C8:08E3
		if (--cx == 0)
			//^44C8:08E4
            return;
	}

	{
		//^44C8:08E6
		bool carry = (cx & 1) ? true : false;
		cx >>= 1;
		//^44C8:08E9
		if (cx != 0) {
			//^44C8:08EB
			do {
				Bit8u al = *si; si++;
				Bit8u ah = al & 0x0f;
				al >>= 4;
				if (al != dh) {
					//^44C8:08F8
					if (ah != dh) {
						//^44C8:08FC
						*di = bx[al]; di++;
						*di = bx[ah]; di++;
						//^44C8:0904
						continue;
					}
					else {
						//^44C8:091D
						*di = bx[al]; di++;
						di++;
						//^44C8:0921
						continue;
					}
				}
				//^44C8:0908
				else if (ah != dh) {
					//^44C8:090C
					di++;
					*di = bx[ah]; di++;
					//^44C8:0912
					continue;
				}
				else {
					//^44C8:0916
					di++;
					di++;
					//^44C8:0918
					continue;
				}
				//^44C8:0918
			} while (--cx != 0);
		}
		//^44C8:0923
		if (carry) {
			//^44C8:0926
			Bit8u al = *si; si++;
			al >>= 4;
			if (al != dh) {
				*di = bx[al]; di++;
			}
		}
	}
	//^44C8:0931
	return;
}


//^44C8:1ACA
void SkWinCore::_44c8_1aca(Bit8u *buff, SRECT *rc, Bit16u xx, Bit16u yy)
{
	//^44C8:1ACA
	ENTER(332);
	//^44C8:1AD0
	if (rc != NULL) {
		//^44C8:1ADB
		Bit16u bp08 = xx ^ 15;
		//^44C8:1AE4
		Bit8u bp014c[320];
		_4976_5e64 = bp014c;
		_4976_5e6a = buff;
		//^44C8:1AFC
		Bit16u bp04 = rc->cx;
		Bit16u bp02 = rc->cy;
		//^44C8:1B0D
		if (bp04 > 0 && bp02 > 0) {
			//^44C8:1B1F
			Bit16u bp06 = (bp04 +1) >> 1;
			//^44C8:1B28
			Bit8u bp0b = Bit8u(xx) << 4 | Bit8u(bp08);
			//^44C8:1B34
			Bit16u si;
			for (si = 0; si < bp06; si++) {
				//^44C8:1B38
				bp014c[si] = bp0b;
				//^44C8:1B3F
				bp014c[si+160] = bp0b ^ 0xff;
				//^44C8:1B45
			}
			//^44C8:1B4B
			Bit16u di = rc->y * yy +rc->x;
			//^44C8:1B5A
			Bit16u bp0a = ((di / yy) ^ di) & 1;
			//^44C8:1B67
			for (si = 0; si < bp02; si++) {
				//^44C8:1B6B
				_4976_5e64 = &bp014c[bp0a * 160];
				//^44C8:1B80
				bp0a ^= 1;
				//^44C8:1B84
				FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP(0, di, bp04, bp08);
				//^44C8:1B95
				di += yy;
				//^44C8:1B98
			}
		}
	}
	//^44C8:1B9E
	return;
}