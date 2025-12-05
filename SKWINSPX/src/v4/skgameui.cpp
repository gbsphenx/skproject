//------------------------------------------------------------------------------
// SK-GAME-UI
//	Game User Interface
//------------------------------------------------------------------------------

#include <skcore.h>

#include <skdebug.h>

//^01B0:04E4
// _01b0_04e4 renamed UI_READ_KEY_INPUT
X16 SkWinCore::UI_READ_KEY_INPUT() //#DS=04BF
{
	ENTER(0);
	LOADDS(0x3083);
	while (glbUIKeyReadCount == 0);

	X16 iKeyboardVValue = tlbUIKeyInput[glbKeyboardReadRRIndex];
	glbKeyboardReadRRIndex = (glbKeyboardReadRRIndex + 1) % 10;
	glbUIKeyReadCount--;
	return iKeyboardVValue;
}

//^476D:050E
// _476d_050e renamed SPECIAL_UI_KEY_TRANSFORMATION
Bit16u SkWinCore::SPECIAL_UI_KEY_TRANSFORMATION()
{
	ENTER(0);
	X16 iKeyVal = UI_READ_KEY_INPUT() CALL_IBMIO;
	X16 iKeyCheck = iKeyVal;
	switch (iKeyCheck) {
		case 0x1048://^0534
			iKeyVal = 0x4C; break;	// numpad 5 => forward
		case 0x1050://^0539
			iKeyVal = 0x50; break;	// numpad 2 => backward
		case 0x104B://^053E
			iKeyVal = 0x4B; break;	// numpad 4 => turn left
		case 0x104D://^0543
			iKeyVal = 0x4D; break;	// numpad 6 => turn right
		case 0x124B://^0548
			iKeyVal = 0x4F; break;	// numpad 1 => move left
		case 0x124D://^054D
			iKeyVal = 0x51; break;	// numpad 3 => move right

#if defined (XDMX_EXTENDED_KEYBOARD)
// SPX convenient additions
// Use AZE-QSD as move arrows / before of different keyboards
		case 0x001E:	// q/a
			iKeyVal = 0x4B; break;	// turn left
		case 0x002C:	// w/z
			iKeyVal = 0x4C; break;	// forward
		case 0x0012:	// e
			iKeyVal = 0x4D; break;	// turn right
		case 0x0010:	// z/q
			iKeyVal = 0x4F; break;	// move left
		case 0x001F:	// s
			iKeyVal = 0x50; break;	// backward
		case 0x0020:	// d
			iKeyVal = 0x51; break;	// move right

// Use X as left click
		case 0x002D:	// x
			//iKeyVal = 0x51; break;	// move right
			cd.mk.mice_btn = 1; // left button
			(this->*_int33_mouse_callback)();
// SPX
#endif // XDMX_EXTENDED_KEYBOARD
	}
	return iKeyVal;
}


//^0759:071B
// _0759_071b renamed UI_CONSUME_KEYBOARD_INPUT
void SkWinCore::UI_CONSUME_KEYBOARD_INPUT()
{
	ENTER(0);
	UI_READ_KEY_INPUT();
}



//^1031:111E
void SkWinCore::HANDLE_UI_EVENT_1031_111e(Bit16u xx)
{
	
	//^1031:111E
	ENTER(72);
	//^1031:1124
	i16 di = xx;
	//^1031:1127
	if (_4976_4df4 != 0) {
		//^1031:112E
		if (_4976_4df4 <= di) {
			//^1031:1134
			di -= _4976_4df4;
			//^1031:1138
			_4976_4df4 = 0;
		}
		else {
			//^1031:1140
			_4976_4df4 -= di;
			//^1031:113E
			return;
		}
	}
	//^1031:1147
	if (_4976_4e62 != 0 || _4976_4e48 != 0 || _4976_4e64 != 0 || glbPtrTransmittedUIEvent == NULL)
		//^1031:116E
		return;
	//^1031:1171
	sk3f6c bp48;
	bp48.w0 = 0xffff;
	Bit8u bp05;
	do {
		//^1031:1176
		bp05 = *(glbPtrTransmittedUIEvent++);
		//^1031:1184
		i16 si;
		SkD((DLV_DBG_UI, "HANDLEUI %04X => %04X\n", bp05, (bp05 & 0xff3f)));
		switch (bp05 & 0xff3f) {
			case 0:
				{
					//^1031:119A
					_4976_4df4 = *glbPtrTransmittedUIEvent;
					//^1031:11A6
					glbPtrTransmittedUIEvent++;
					//^1031:11AA
					break;
				}
			case 4:
			case 6:
				{
					//^1031:11AD
					si = glbUIClickEventLast + 1; // si
					//^1031:11B3
					if (si > 2)
						//^1031:11B8
						si = 0;
					//^1031:11BA
					if (glbUIClickEventIndex == si) {
						//^1031:11C0
						if (_4976_4e62 == 0)
							//^1031:11C7
							_4976_4e62 = 1;
						//^1031:11CD
						glbPtrTransmittedUIEvent--;
						//^1031:11D1
						goto _1534;
					}
					//^1031:11D4
					tlbUIClickEvents[glbUIClickEventLast] = glbMousePosition;
					//^1031:11ED
					glbUIClickEventLast = si;
					//^1031:11F1
					if ((bp05 & 0xff3f) == 4)
						//^1031:11FE
						break;

					goto _1201;
				}
			case 1:
				{
					//^1031:1201
_1201:
					_4976_4e62 = glbUIClickEventLast - glbUIClickEventIndex;
					//^1031:120B
					if (_4976_4e62 >= 0)
						//^1031:120F
						break;
					//^1031:1212
					_4976_4e62 += 3;
					//^1031:1217
					break;
				}
			case 2:
				{
					//^1031:121A
					_4976_4e48 = 1;
					//^1031:1220
					break;
				}
			case 3:
				{
					//^1031:1223
					_4976_4e64 = 1;
					//^1031:1229
					break;
				}
			case 5:
				{
					//^1031:122C
					if (HANDLE_UI_EVENT(&glbMousePosition) != 0) {
						//^1031:123B
						if (cd.gg.glbRefreshViewport == 0)
							//^1031:1242
							break;
						//^1031:1245
						goto _1201;
					}
					//^1031:1247
					glbPtrTransmittedUIEvent--;
					//^1031:124B
					break;
				}
			case 7:
				{
					//^1031:124E
					Bit8u bp06 = *(glbPtrTransmittedUIEvent++);
					//^1031:125C
					Bit8u bp07 = *(glbPtrTransmittedUIEvent++);
					//^1031:126A
					Bit8u bp08 = *(glbPtrTransmittedUIEvent++);
					//^1031:1278
					Bit8u *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(bp06, bp07, bp08);
					//^1031:128F
					SRECT bp14;
					_1031_10c8(&bp48, &bp14, READ_UI16(bp04,-4), READ_UI16(bp04,-2));
					//^1031:12AB
					DRAW_ICON_PICT_BUFF(
						QUERY_GDAT_IMAGE_ENTRY_BUFF(bp06, bp07, bp08),
						&bp48,
						&bp14,
						0,
						0,
						*(glbPtrTransmittedUIEvent++),
						0,
						QUERY_GDAT_IMAGE_LOCALPAL(bp06, bp07, bp08)
						);
					//^1031:12FC
					break;
				}
			case 8:
				{
					//^1031:12FF
					si = GET_PLAYER_AT_POSITION((*(glbPtrTransmittedUIEvent++) + cd.pi.glbPlayerDir) & 3);
					//^1031:131C
					if (si < 0)
						//^1031:1320
						break;
					//^1031:1323
					DRAW_SQUAD_SPELL_AND_LEADER_ICON(si, 1);
					//^1031:132D
					_0b36_0cbe(&_4976_3f6c, 1);
					//^1031:133B
					break;
				}
			case 9:
				{
					//^1031:133E
					if (cd.pi.glbChampionIndex > 0) {
						//^1031:1345
						si = cd.pi.glbChampionIndex - 1;
					}
					//^1031:134D
					//else if (glbMousePosition.event >= 0x74 && glbMousePosition.event <= 0x7b) {
					else if (glbMousePosition.event >= UI_EVENTCODE_CHAMPION1_ACTION_HAND_RIGHT && glbMousePosition.event <= UI_EVENTCODE_CHAMPION4_ACTION_HAND_LEFT) {
						//^1031:135B
						si = (glbMousePosition.event -UI_EVENTCODE_CHAMPION1_ACTION_HAND_RIGHT) >> 1;
					}
					else {
						//^1031:1365
						//^1031:11A6
						glbPtrTransmittedUIEvent++;
						//^1031:11AA
						break;
					}
					//^1031:1368
					DRAW_HAND_ACTION_ICONS(si, *(glbPtrTransmittedUIEvent++), 1);
					//^1031:1381
					goto _148a;
				}
			case 10:
				{
					//^1031:1384
					Bit8u *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x01, 0x05, 0x08);
					//^1031:1398
					SRECT bp14;
					_1031_10c8(&bp48, &bp14, READ_UI16(bp04,-4), READ_UI16(bp04,-2));
					//^1031:13B4
					DRAW_ICON_PICT_BUFF(
						QUERY_GDAT_IMAGE_ENTRY_BUFF(0x01, 0x05, 0x08),
						&bp48,
						&bp14,
						0,
						0,
						-1,
						0,
						QUERY_GDAT_IMAGE_LOCALPAL(0x01, 0x05, 0x08)
						);
					//^1031:13EE
					_4976_4df6[0] = (glbChampionTable[cd.pi.glbChampionIndex].runesCount * 6) +U8(glbMousePosition.event) -5;
					//^1031:140C
					i16 bp0a;
					i16 bp0c;
					QUERY_STR_METRICS(_4976_4df6, &bp0a, &bp0c);
					//^1031:1422
					CALC_CENTERED_RECT_IN_RECT(&bp14, &bp48.rc2, bp0a, bp0c);
					//^1031:143A
					_0b36_129a(
						&bp48, 
						bp14.x, 
						bp14.y +bp14.cy -1, 
						glbPaletteT16[COLOR_BLACK], 
						glbPaletteT16[COLOR_CYAN], 
						_4976_4df6
						);
					//^1031:1467
					break;
				}
			case 11:
				{
					//^1031:146A
					DRAW_SPELL_TO_BE_CAST(1);
					//^1031:1471
					//^1031:14D4
					goto _148a;
				}
			case 12:
				{
					//^1031:1473
					DRAW_CMD_SLOT(*(glbPtrTransmittedUIEvent++), 1);
					//^1031:148A
_148a:
					_0b36_0cbe(&_4976_3f6c, 1);
					//^1031:1498
					break;
				}
			case 13:
				{
					//^1031:149A
					si = glbChampionInventory;
					//^1031:149F
					if (si != 0) {
						//^1031:14A3
						_4976_581a |= *glbPtrTransmittedUIEvent;
						//^1031:14B0
						REFRESH_PLAYER_STAT_DISP(--si);
					}
					//^1031:14BD
					//^1031:11A6
					glbPtrTransmittedUIEvent++;
					//^1031:11AA
					break;
				}
			case 14:
				{
					//^1031:14C0
					_29ee_1d03(*(glbPtrTransmittedUIEvent++));
					//^1031:14D4
					goto _148a;
				}
			case 15:
			case 16:
				{
					//^1031:14D6
					_4976_4bd5 = *glbPtrTransmittedUIEvent; glbPtrTransmittedUIEvent++;
					_4976_4bd6 = *glbPtrTransmittedUIEvent; glbPtrTransmittedUIEvent++;
					_4976_4bd4 = *glbPtrTransmittedUIEvent; glbPtrTransmittedUIEvent++;
					//^1031:1500
					_0aaf_01db(_4976_4df8, 1);
					//^1031:150D
					bp48.w0 = 0xffff;
					//^1031:1512
					_0aaf_002f();

					break;
				}
		}
		//^1031:1517
		if ((bp05 & 0x80) != 0) {
			//^1031:151D
			glbPtrTransmittedUIEvent = NULL;
			//^1031:1529
			break;
		}
		//^1031:152B
	} while ((bp05 & 0x40) != 0);

	//^1031:1534
_1534:
	if (bp48.w0 != 0xffff) {
		//^1031:153A
		_0b36_0cbe(&bp48, 1);
	}
	//^1031:1549
	return;
}
