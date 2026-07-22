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

	X16 iKeyboardVValue = tblUIKeyInput[glbKeyboardReadRRIndex];
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
	}
#if defined (XDMX_EXTENDED_KEYBOARD)	// WASD
	if (_4976_4e62 == 0) {	// not in the savegame panel to write a savegame name
		switch (iKeyCheck) {
	// SPX convenient additions
	// Use AZE-QSD as move arrows / before of different keyboards
/*			case 0x001E:	// q/a
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
*/
	// Use X as left click
			case 0x002D:	// x
				//iKeyVal = 0x51; break;	// move right
				cd.mk.mice_btn = 1; // left button
				(this->*_int33_mouse_callback)();
		}
	}
// SPX
#endif // XDMX_EXTENDED_KEYBOARD


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
void SkWinCore::HANDLE_UI_EVENT_1031_111e(U16 xx)
{
	
	ENTER(72);
	i16 di = xx;	// di
	if (_4976_4df4 != 0) {
		if (_4976_4df4 <= di) {
			di -= _4976_4df4;
			_4976_4df4 = 0;
		}
		else {
			_4976_4df4 -= di;
			return;
		}
	}
	if (_4976_4e62 != 0 || _4976_4e48 != 0 || _4976_4e64 != 0 || glbPtrTransmittedUIEvent == NULL)
		return;
	sk3f6c bp48;
	bp48.w0 = 0xFFFF;
	U8 iRawUIEvent;	// bp05
	do {
		iRawUIEvent = *(glbPtrTransmittedUIEvent++);	// bp05
		i16 si;
		SkD((DLV_DBG_UI, "HANDLEUI %04X => %04X\n", iRawUIEvent, (iRawUIEvent & 0xFF3F)));
		printf("INPUT %d : HANDLEUI %04X => %04X\n", xx, iRawUIEvent, (iRawUIEvent & 0xFF3F));
		switch (iRawUIEvent & 0xFF3F) {
			case 0:	// 0:
				{
					_4976_4df4 = *glbPtrTransmittedUIEvent;
					glbPtrTransmittedUIEvent++;
					break;
				}
			case 4:	// 4:
			case 6:	// 6: click viewport / sleep
				{
					si = glbUIClickEventLast + 1; // si
					if (si > 2)
						si = 0;
					if (glbUIClickEventIndex == si) {
						if (_4976_4e62 == 0)
							_4976_4e62 = 1;
						glbPtrTransmittedUIEvent--;
						goto _1534;
					}
					tlbUIClickEvents[glbUIClickEventLast] = glbMousePosition;
					glbUIClickEventLast = si;
					if ((iRawUIEvent & 0xFF3F) == 4)
						break;

					goto _1201;
				}
			case 1:	// 1: 
				{
_1201:
					_4976_4e62 = glbUIClickEventLast - glbUIClickEventIndex;
					if (_4976_4e62 >= 0)
						break;
					_4976_4e62 += 3;
					break;
				}
			case 2:	// 2:
				{
					_4976_4e48 = 1;
					break;
				}
			case 3:	// 3:
				{
					_4976_4e64 = 1;
					break;
				}
			case 5:	// 5: click on inventory item or dialog button
				{
					if (HANDLE_UI_EVENT(&glbMousePosition) != 0) {
						if (cd.gg.glbRefreshViewport == 0)
							break;
						goto _1201;
					}
					glbPtrTransmittedUIEvent--;
					break;
				}
			case 7:	// 7: move arrow
				{
					U8 iImgCls1Category = *(glbPtrTransmittedUIEvent++);	// bp06
					U8 iImgCls2MainId = *(glbPtrTransmittedUIEvent++);	// bp07
					U8 iImgCls4EntryId = *(glbPtrTransmittedUIEvent++);	// bp08
					U8* xImgMoveArrow = QUERY_GDAT_IMAGE_ENTRY_BUFF(iImgCls1Category, iImgCls2MainId, iImgCls4EntryId); // bp04
					printf("EVENT for ARROW IMG BUFF : %d %d %d\n", iImgCls1Category, iImgCls2MainId, iImgCls4EntryId);
					SRECT xRect;	// bp14
					//_1031_10c8(&bp48, &xRect, READ_UI16(bp04,-4), READ_UI16(bp04,-2));
					SOME_COPY_RECT_1031_10c8(&bp48, &xRect, READ_IMGBUFF_WIDTH(xImgMoveArrow), READ_IMGBUFF_HEIGHT(xImgMoveArrow));
					DRAW_ICON_PICT_BUFF(
						QUERY_GDAT_IMAGE_ENTRY_BUFF(iImgCls1Category, iImgCls2MainId, iImgCls4EntryId),
						&bp48,
						&xRect,
						0,
						0,
						*(glbPtrTransmittedUIEvent++),
						0,
						QUERY_GDAT_IMAGE_LOCALPAL(iImgCls1Category, iImgCls2MainId, iImgCls4EntryId)
						);
					break;
				}
			case 8:	// 8: select spell caster or leader
				{
					si = GET_PLAYER_AT_POSITION((*(glbPtrTransmittedUIEvent++) + cd.pi.glbPlayerDir) & 3);
					if (si < 0)
						break;
					DRAW_SQUAD_SPELL_AND_LEADER_ICON(si, 1);
					_0b36_0cbe(&_4976_3f6c, 1);
					break;
				}
			case 9:	// 9 : champion hand action
				{
					if (cd.pi.glbChampionIndex > 0) {
						si = cd.pi.glbChampionIndex - 1;
					}
					//else if (glbMousePosition.event >= 0x74 && glbMousePosition.event <= 0x7b) {
					else if (glbMousePosition.event >= UI_EVENTCODE_x74_CHAMPION1_ACTION_HAND_RIGHT && glbMousePosition.event <= UI_EVENTCODE_x7B_CHAMPION4_ACTION_HAND_LEFT) {
						si = (glbMousePosition.event -UI_EVENTCODE_x74_CHAMPION1_ACTION_HAND_RIGHT) >> 1;
					}
					else {
						glbPtrTransmittedUIEvent++;
						break;
					}
					DRAW_HAND_ACTION_ICONS(si, *(glbPtrTransmittedUIEvent++), 1);
					goto _148a;
				}
			case 10:	// 10: push rune cell button (add rune)
				{
					U8* xImgCellBuffer = QUERY_GDAT_IMAGE_ENTRY_BUFF(GDAT_CATEGORY_x01_INTERFACE_GENERAL, GDAT_INTERFACE_SUBCAT_x05_SPELLMENU, GDAT_x08_RUNE_CELL__PUSHED);	// // 0x01, 0x05, 0x08
					SRECT xRect;	// bp14
					//_1031_10c8(&bp48, &bp14, READ_UI16(bp04,-4), READ_UI16(bp04,-2));
					SOME_COPY_RECT_1031_10c8(&bp48, &xRect, READ_IMGBUFF_WIDTH(xImgCellBuffer), READ_IMGBUFF_HEIGHT(xImgCellBuffer));
					DRAW_ICON_PICT_BUFF(
						QUERY_GDAT_IMAGE_ENTRY_BUFF(GDAT_CATEGORY_x01_INTERFACE_GENERAL, GDAT_INTERFACE_SUBCAT_x05_SPELLMENU, GDAT_x08_RUNE_CELL__PUSHED),	// 0x01, 0x05, 0x08
						&bp48,
						&xRect,
						0,
						0,
						-1,
						0,
						QUERY_GDAT_IMAGE_LOCALPAL(GDAT_CATEGORY_x01_INTERFACE_GENERAL, GDAT_INTERFACE_SUBCAT_x05_SPELLMENU, GDAT_x08_RUNE_CELL__PUSHED)
						);
					_4976_4df6[0] = (glbChampionTable[cd.pi.glbChampionIndex].runesCount * 6) + U8(glbMousePosition.event) - 5;
					i16 bp0a;
					i16 bp0c;
					QUERY_STR_METRICS(_4976_4df6, &bp0a, &bp0c);
					CALC_CENTERED_RECT_IN_RECT(&xRect, &bp48.rc2, bp0a, bp0c);
					DRAW_STUFF_0b36_129a(
						&bp48, 
						xRect.x, 
						xRect.y + xRect.cy - 1, 
						glbPaletteT16[C00_COLOR_BLACK], 
						glbPaletteT16[C04_COLOR_CYAN], 
						_4976_4df6
						);
					break;
				}
			case 11:	// 11: validate spell
				{
					DRAW_SPELL_TO_BE_CAST(1);
					goto _148a;
				}
			case 12:	// 12: hand action
				{
					DRAW_CMD_SLOT(*(glbPtrTransmittedUIEvent++), 1);
_148a:
					_0b36_0cbe(&_4976_3f6c, 1);
					break;
				}
			case 13:	// 13: return to viewport
				{
					si = glbChampionInventory;
					if (si != 0) {
						_4976_581a |= *glbPtrTransmittedUIEvent;
						REFRESH_PLAYER_STAT_DISP(--si);
					}
					glbPtrTransmittedUIEvent++;
					break;
				}
			case 14:	// 14: magic map button
				{
					DRAW_SOME_MAGICAL_MAP_STRING_29ee_1d03(*(glbPtrTransmittedUIEvent++));
					goto _148a;
				}
			case 15:	// dialog button
			case 16:
				{
					glbGDATItemCls1Category = *glbPtrTransmittedUIEvent; glbPtrTransmittedUIEvent++;
					glbGDATItemCls2MainItemId = *glbPtrTransmittedUIEvent; glbPtrTransmittedUIEvent++;
					glbGDATItemCls4EntryId = *glbPtrTransmittedUIEvent; glbPtrTransmittedUIEvent++;
					RECT_0aaf_01db(_4976_4df8, 1);
					bp48.w0 = 0xffff;
					_0aaf_002f();

					break;
				}
		}
		if ((iRawUIEvent & 0x80) != 0) {
			glbPtrTransmittedUIEvent = NULL;
			break;
		}
	} while ((iRawUIEvent & 0x40) != 0);

_1534:
	if (bp48.w0 != 0xffff) {
		_0b36_0cbe(&bp48, 1);
	}
	return;
}
