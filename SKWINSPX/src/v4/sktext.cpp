//------------------------------------------------------------------------------
// SK-TEXT
//------------------------------------------------------------------------------

//#include <StdAfx.h>	// Required for MVC6 compilation
#include <skcore.h>
#include <skglobal.h>
#include <string.h>
#include <skparam.h>

//#if defined(_USE_MFC80) || defined(_USE_MFC60)
//#include <SKMFC.h>
//#include <resource.h>
//#endif // defined(_USE_MFC80) || defined(_USE_MFC60)

//^3929:0826
void SkWinCore::_3929_0826()
{
	ENTER(0);
	FIRE_HIDE_MOUSE_CURSOR();
	FIRE_FILL_SCREEN_RECT(RECT_015_BOTTOM_MESSAGE_3_LINES, glbPaletteT16[COLOR_BLACK]);
	FIRE_SHOW_MOUSE_CURSOR();
	_3929_07e1(0, 0);
	for (U16 si = 0; si < 1; si++) {
		tblTimerTickRemoveHintMessage[si] = -1;
	}
	return;
}


//^3929:0914
void SkWinCore::_3929_0914_HINT_TEXT()
{
	ENTER(0);
	_3929_0826();
	tblTimerTickRemoveHintMessage[0] = -1;
	return;
}
/*
void SkWinCore::_3929_0914_HINT_TEXT()
{
	;
}
*/
/* --> here is implementation of 3929:0914 in Skull, more complete!
void DM2_gfxmain_3929_0914(void)
{
  if (ddat.v1e141e == 2)
  {
    if (ddat.v1e1420 != 0)
      DM2_SCROLLBOX_MESSAGE();
    DM2_FILL_ENTIRE_PICT(gfxsys.pictbuff, palettecolor_to_pixel(paldat.palette[E_COL00]));
    ddat.v1e1420 = 1;
    //m_45942:
    for (i16 i=0; i < 2; i++)
      //m_45933:
      ddat.v1e1408[i] = ddat.v1e1408[i + 1];
    ddat.v1e1408[2] = lcon(0xffffffff);
  }
  else
  {
    //m_45959:
    ddat.v1e1420 = 0;
    ddat.v1e141e++;
  }
}
 */



//^3929:0BA6
void SkWinCore::PRINT_SYSERR_TEXT(Bit16u dstx, Bit16u dsty, Bit16u clr1, Bit16u clr2, Bit8u *str)
{
	DRAW_STRING(_4976_4964, -1, glbScreenWidth, dstx, dsty+1, clr1, clr2, str, 8);
}

//^3929:0929
// SPX _3929_0929 renamed DRAW_HINT_TEXT
void SkWinCore::DRAW_HINT_TEXT(U16 clr1, U8 *str)
{
	ENTER(8);
	SRECT bp08;
	QUERY_EXPANDED_RECT(RECT_015_BOTTOM_MESSAGE_3_LINES, &bp08);
	if (_4976_5c06 != 0) {
		DRAW_STRING(
			_4976_5c08,
			-1,
			_4976_013e,
			_4976_475c,
			_4976_0140 -2 -((QUERY_MBCS_PRESENCE(str) != 0) ? _4976_0136 : _4976_0124),
			clr1,
			glbPaletteT16[COLOR_BLACK],
			str,
			8
			);
	}
	else {
		FIRE_HIDE_MOUSE_CURSOR();
		PRINT_SYSERR_TEXT(
			bp08.x + _4976_475c,
			glbTimerTickRemoveHintMessageIndex * _4976_013a +bp08.y + _4976_0130 - _4976_0136 + _4976_5c12 -1,
			clr1,
            glbPaletteT16[COLOR_BLACK],
			str
			);
		FIRE_SHOW_MOUSE_CURSOR();
	}
	tblTimerTickRemoveHintMessage[glbTimerTickRemoveHintMessageIndex] = glbGameTick + 70;	// is it a clock before message disappear ?
	return;
}

//^3929:09FB
// SPX: _3929_09fb renamed DISPLAY_HINT_TEXT
void SkWinCore::DISPLAY_HINT_TEXT(Bit16u color, const U8 *str) //#DS=4976
{
	ENTER(102);
	//U16 si = color;
	U16 absoluteColor = 255;	// white from 256 palette
	if (_4976_5c06 != 0) {	// roll message ?
		SCROLLBOX_MESSAGE();
		FILL_ENTIRE_PICT(_4976_5c08, glbPaletteT16[COLOR_BLACK]);
		_4976_5c06 = 0;
	}
	absoluteColor = glbPaletteT16[color];
	for (U16 bp02 = 0; str[bp02] != 0; ) {
		U8 bp66[100]; memset(bp66,0,100);
		U16 di = _3929_04e2_DRAW_TEXT_STRINGS(str, bp66, &bp02, _4976_013e - _4976_475c);	// _4976_013e - _4976_475c somehow can move text to left (thus out of screen)
		if (str[bp02] == '\n') {
			_3929_0914_HINT_TEXT();
			DRAW_HINT_TEXT(absoluteColor, bp66);
			_4976_475c = 0;
			bp02++;
			continue;
		}
		if (str[bp02] != 0) {
			DRAW_HINT_TEXT(absoluteColor, bp66);
			_3929_0914_HINT_TEXT();
			_4976_475c = 12;
			continue;
		}
		DRAW_HINT_TEXT(absoluteColor, bp66);
		_4976_475c += di;
	}
	if (_4976_5c06 != 0) {
		SCROLLBOX_MESSAGE();
		FILL_ENTIRE_PICT(_4976_5c08, glbPaletteT16[COLOR_BLACK]);
	}
	return;
}

//^3929:0B5A
// SPX : _3929_0b5a renamed DISPLAY_HINT_NEW_LINE
void SkWinCore::DISPLAY_HINT_NEW_LINE()
{
	ENTER(0);
	DISPLAY_HINT_TEXT(COLOR_BLACK, const_cast<U8 *>(strEndLine));
	return;
}

//^2405:00C9
// SPX: _2405_00c9 renamed DISPLAY_TAKEN_ITEM_NAME
void SkWinCore::DISPLAY_TAKEN_ITEM_NAME(ObjectID rl)
{
	ENTER(0);
	DISPLAY_HINT_NEW_LINE();
	DISPLAY_HINT_TEXT(COLOR_LIGHTER_GRAY, GET_ITEM_NAME(rl));
	return;
}



//^32CB:0E03
void SkWinCore::DRAW_TEXT_TO_BACKBUFF(i16 xx, i16 yy, U8 *str)
{
	ENTER(24);
	if (QUERY_MBCS_PRESENCE(str) == 0) {
		for (U8 *bp04 = str; *bp04 != 0; bp04++) {
			if (*bp04 >= 'A' && *bp04 <= 'Z') {
				*bp04 = *bp04 -0x40;
			}
			else if (*bp04 >= 0x7B) {
				*bp04 = *bp04 - 0x60;
			}
		}
	}
	i16 bp06;
	i16 bp08;
	if (QUERY_STR_METRICS(str, &bp06, &bp08) != 0) {
		U8 bp18[16];
		COPY_MEMORY(glbPaletteT16, bp18, 16);
		PALETTE_SOMETHING_0b36_037e(bp18, U8(_4976_5a88), -1, -1, 16);
		DRAW_STRONG_TEXT(glbBackBuffViewport, -1, _4976_00f6, xx -(bp06 >> 1), yy, bp18[15], bp18[0]|0x4000, str);
	}
	return;
}


//^2066:33C4
// SPX: _2066_33c4 renamed COPY_STRING_2066_33c4
void SkWinCore::COPY_STRING_2066_33c4(U8 *str, U16 yy) 
{
	ENTER(0);
	SK_STRCPY(str, _4976_3b68);
	str[5] = yy + 0x30;
	return;
}

//^2066:398A
void SkWinCore::_2066_398a(i16 xx)
{
	int iColorText = COLOR_ORANGE;
	int iColorTextHighlighted = COLOR_YELLOW;
	ENTER(10);

	if (SkCodeParam::bDM2V5Mode) {
		iColorText = COLOR_YELLOW;
		iColorTextHighlighted = COLOR_WHITE;
	}

	SRECT bp0a;
	QUERY_EXPANDED_RECT(451, &bp0a);
	i16 di = bp0a.y +4;
	for (i16 si = 0; si < 10; si++) {
		U16 bp02 = (si == xx) ? glbPaletteT16[iColorTextHighlighted] : glbPaletteT16[iColorText];
		DRAW_VP_STR(bp0a.x, di, bp02, _4976_5250[si].sSavegameName);
		di += glbPanelStatsYDelta;
	}
	return;
}
//^2066:3820
// _2066_3820 renamed DIALOG_2066_3820
void SkWinCore::DIALOG_2066_3820(U8 *xx, U16 yy)
{
	ENTER(8);
	SRECT bp08;
	QUERY_EXPANDED_RECT(RECT_453, &bp08);
	DRAW_DIALOGUE_PICT(QUERY_GDAT_IMAGE_ENTRY_BUFF(GDAT_CATEGORY_x1A_DIALOG_BOXES, 0x81, 0x00), glbBackBuffViewport, &bp08, 
		bp08.x, bp08.y, -1, QUERY_GDAT_IMAGE_LOCALPAL(GDAT_CATEGORY_x1A_DIALOG_BOXES, 0x81, 0x00));
	DRAW_VP_STR(bp08.x, bp08.y +4, glbPaletteT16[COLOR_YELLOW], xx);
	if (yy != 0) {
		bp08.x += SK_STRLEN(xx) * _4976_0126;
		bp08.cx = _4976_0126;
		bp08.cy -= 4;
		FIRE_FILL_BACKBUFF_RECT(&bp08, glbPaletteT16[COLOR_ORANGE]);
	}
	_0aaf_002f();
	return;
}


//^3E74:5B7C
void SkWinCore::_3e74_5b7c_KEYBOARD(U16 xx)
{
	ENTER(20);
	_476d_05b6_KEYBOARD(xx);
	if (_4976_484d++ == _4976_484f[RCJ(4,_4976_484d)]) {
		if (_4976_484f[RCJ(4,_4976_484d)] != 0)
			return;
		const U8 *bp04 = _4976_4853;
		DISPLAY_HINT_TEXT(COLOR_WHITE, bp04);
		U8 bp14[16];
		DISPLAY_HINT_TEXT(COLOR_WHITE, SK_LTOA10(2, bp14));
		_3929_0b01(15, 46);
		_3929_0b20(15, 8);
		_3929_0b20(15, glbCRAMSize);
		_3929_0b20(15, glbEMSMemPool);
		_3929_0b20(15, _4976_5d66);
		bp04 = _4976_485a;
		SK_STRCPY(bp14, bp04);
		if (_4976_5bee[0] != 0)
			bp14[0] = 0x47;
		if (_4976_5bee[1] != 0)
			bp14[1] = 0x53;
		if (_4976_5bee[2] != 0)
			bp14[2] = 0x45;
		DISPLAY_HINT_TEXT(COLOR_WHITE, bp14);
	}
	_4976_484d = 0;
	return;
}


//^3929:04E2
// _3929_04e2 renamed _3929_04e2_DRAW_TEXT_STRINGS
Bit16u SkWinCore::_3929_04e2_DRAW_TEXT_STRINGS(const Bit8u *strIn, Bit8u *strOut, Bit16u *textPos, i16 maxWidth)
{
	Bit16u si = *textPos;
	i16 bp02 = 0;
	Bit16u di = 0;
	Bit16u bp04 = 0xffff;
    Bit16u bp06 = 0;
	Bit16u bp08 = 0;
	Bit8u bp0d;
	skxxxf *bp0c;
	if (QUERY_MBCS_PRESENCE(strIn) != 0 || _4976_475e != 0xff) {
		if (_4976_475e == 0xff) {
			_4976_475e = 1;
		}
		else {
			strOut[di] = 2; di++;
			strOut[di] = 0x20 + _4976_475e;
            di++;
		}

		while (true) {
			strOut[di] = 0;
			if (strIn[si] == vbLf || strIn[si] == 0) {
				if (strIn[si] == 0) {
					_4976_475e = 0xff;
				}
				break;
			}
			bp0d = strIn[si];
			Bit8u bp0e;
			if (bp0d >= 0xf0) {
				bp0e = strIn[si +1];
			}
			else {
				if (bp0d < 0x20) {
					strOut[di] = strIn[si]; si++; di++;
					if (bp0d == 2) {
						_4976_475e = (strOut[di] = strIn[si]) + 0xe0; si++; di++;
					}
					continue;
				}
				bp0e = bp0d;
				bp0d = 0;
				if (strIn[si] == 0x20) {
					bp04 = si;
					bp06 = di;
					bp08 = bp02;
				}
			}
			Bit8u bp0f;
			bp0c = QUERY_CHAR_METRICS(_4976_475e, bp0d, bp0e, &bp0f);
			if (bp0c == NULL) {
				si++;
				if (bp0d != 0) {
					si++;
				}
				continue;
			}
			if (i16(bp0c->w6) + _4976_0132 + bp02 <= maxWidth) {
				if (bp02 == 0) {
					bp02 = - _4976_0132;
				}
				bp02 += bp0c->w6 + _4976_0132;
				if (bp0d != 0) {
					strOut[di] = strIn[si]; si++; di++;
				}
				strOut[di] = strIn[si]; si++;
				di++; continue;
			}
			if (bp04 != 0xffff) {
_0654:
				*textPos = bp04 +1;
				strOut[bp06] = 0;
				return bp08;
			}
			break;
		}
	}
	else {
		while (true) {
			strOut[di] = 0;
			if (strIn[si] == vbLf || strIn[si] == 0)
				break;
			if (strIn[si] == 0x20) {
				bp04 = si;
				bp06 = di;
				bp08 = bp02;
			}
			if (bp02 + _4976_0126 <= maxWidth) {
				if (bp02 == 0) {
					bp02 = - _4976_0120;
				}
				bp02 += _4976_0126;
				strOut[di] = strIn[si]; si++; di++;
				continue;
			}
			if (bp04 == 0xffff) {
				break;
			}
			goto _0654;
		}
	}
	*textPos = si;
	return bp02;
}

//^3929:00E6
const Bit8u *SkWinCore::DRAW_MBCS_STR(
	const Bit8u *str, 
	Bit8u *dst, 
	i16 zz, 
	Bit8u *ww, 
	Bit16u clr1, 
	Bit16u clr2, 
	Bit16u *xx, 
	Bit16u *yy, 
	Bit16u dstpitch, 
	Bit16u dstbpp
) {
	Bit8u bp0b = *str++;
	Bit8u bp0c;
	if (bp0b >= 0xF0) {
		bp0c = *str;
		str++;
	}
	else {
		if (bp0b < 0x20) {
			if (bp0b != 0x02)
				return str;
			*ww = *str + 0xE0;
			str++;
			return str;
		}
		bp0c = bp0b;
		bp0b = 0;
	}
	Bit8u bp0d;
	skxxxf *bp04 = QUERY_CHAR_METRICS(*ww, bp0b, bp0c, &bp0d);
	if (bp04 == NULL)
		return str;
	Bit16u bp18 = glbGDatFloppyFlag;
	glbGDatFloppyFlag = -_4976_5d76;
	// SPX: add protection here : if the graphics.dat does not contain japanese font, replacing the missing font with default image (yukman) will lead into a crash
	// Therefore, we check if the font exist; if not, just leave for now.
	int iItemSize = QUERY_GDAT_ENTRY_DATA_LENGTH(GDAT_CATEGORY_x01_INTERFACE_GENERAL, 0x01, dtImage, 0x00);
	if (iItemSize <= 0)
	//	return (const Bit8u*) "?";	// this will lead to an infinite loop when displaying HIT damage
		return str;

	Bit8u *bp08 = QUERY_GDAT_IMAGE_ENTRY_BUFF(GDAT_CATEGORY_x01_INTERFACE_GENERAL, *ww, bp0d);	// 0x1C
	Bit8u bp2a[16]; // 2a-1b
	Bit16u bp0a;
	if (dstbpp != IMG_4_BPP) {	// IMG_8_BPP
		bp2a[ 0] = (Bit8u)clr2;
		bp2a[15] = (Bit8u)clr1;
		bp0a = ((clr2 & 0x4000) != 0) ? 0 : -1;
	}
	else {	// IMG_4_BPP
		if ((clr2 & 0x4000) != 0) {
			bp0a = clr2 = (clr1 == 0) ? 1 : 0;
		}
		else {
			bp0a = -1;
		}
		Bit8u *bp12;
		Bit16u di;
		Bit16u si;
		Bit32u bp16;
		if ((clr1 == 0 && clr2 == 0) || _4976_5d76 == 0) {
			bp08 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x001c, *ww, bp0d);
			if (clr1 != 0 || clr2 != 15) {
				Bit8u *bp12 = bp08;
				Bit16u di = ((Bit16u *)bp08)[-2];
				Bit16u si = ((Bit16u *)bp08)[-1];
				goto _02e5;
			}
		}
		else {
			bp16 = (QUERY_GDAT_ENTRY_DATA_INDEX(0x1c, *ww, 1, bp0d) & 0x1fff) 
				| 0x4000 
				| ((clr2 & 0x000f) << 4) 
				| ((clr1 & 0x000f)     );
			Bit16u bp1a;
			if (ADD_CACHE_HASH(bp16, &bp1a) != 0) {
				bp08 = QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp1a);
			}
			else {
				di = (bp04->w2 * bp04->w6);
				si = (bp04->w4 * bp04->w8);
				ALLOC_NEW_PICT(bp1a, di, si, IMG_4_BPP);
				bp12 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x1c, *ww, bp0d);
				bp08 = QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp1a);
_02e5:
				Bit8u bp3a[16]; // 3a-2b
				bp3a[ 0] = (Bit8u)clr2;
				bp3a[15] = (Bit8u)clr1;
				FIRE_STRETCH_BLIT_TO_MEMORY_4TO4BPP(bp12, bp08, di, si, di, si, bp3a);
			}
			if (_4976_5d76 != 0) {
				_3e74_585a_CACHE(bp1a, 0);
			}
		}
	}
	if (zz >= 0) {
		dst = QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(zz);
	}
	bp0c -= bp04->b1;
	FIRE_BLIT_PICTURE(
		bp08,
		dst,
		ALLOC_TEMP_RECT(
			*xx,
			*yy,
			bp04->w6,
			bp04->w8
			),
		(bp0c % bp04->w2) * bp04->w6,
		(bp0c / bp04->w2) * bp04->w8,
		READ_UI16(bp08,-4),
		dstpitch,
		bp0a,
		0,
		4,
		dstbpp,
		bp2a
		);
	*xx += bp04->w6 + _4976_0132;
	FREE_PICT_ENTRY(bp08);
	glbGDatFloppyFlag = bp18;
	return str;
}


//^470A:0008
Bit8u *SkWinCore::QUERY_FONT(i8 input, Bit8u fore, Bit8u back)
{
	Bit8u bp02 = fore << 4;
	Bit8u bp03 = back << 4;
	Bit16u si = 0;
	Bit16u di = 0;
	for (; di < 6; di++) {
		U8 bp01 = _4976_5c0e[(di << 7) + input];
		i8 bp06;
		for (bp06 = 0; bp06 < 3; bp06++) {
			_4976_5e72[si++] = (((bp01 & 0x10) != 0) ? bp02 : bp03) | (((bp01 & 0x08) != 0) ? fore : back);
			bp01 <<= 2;
		}
	}
	return _4976_5e72;
}

//^470A:0089
void SkWinCore::DRAW_STRING(
	Bit8u *dst, 
	Bit16u ss, 
	Bit16u dstpitch, 
	Bit16u dstx, 
	Bit16u dsty, 
	Bit16u clr1,					// front color (cyan, white, etc..)
	Bit16u clr2,					// shadow color (generally black)
	const Bit8u *str, 
	Bit16u dstbpp
) {
	Bit16u di = dstbpp;
	if (*str == 0)
		return;
	Bit16u si;
	if ((clr2 & 0x4000) != 0) {
		clr2 &= 0xBFFF;
		si = clr2;
	}
	else {
		si = -1;
	}
	SRECT bp08;
	bp08.x = dstx;
	if (QUERY_MBCS_PRESENCE(str) != 0) {
		Bit8u bp09 = 1;
		dsty -= 14;
		if (si != 0xFFFF) {
            clr2 |= 0x4000;
		}
		do {
			str = DRAW_MBCS_STR(str, dst, ss, &bp09, clr1, clr2, &dstx, &dsty, dstpitch, di);
		} while (*str != 0);
	}
	else {
		Bit8u bp1a[16];
		if (di != 4) {
			bp1a[0] = (Bit8u)clr1;
			bp1a[1] = (Bit8u)clr2;
			clr1 = 0;
			clr2 = 1;
			if (si != 0xFFFF) {
				si = 1;
			}
		}
		if (str[0] == 0x02 && str[1] == 0x20) {
			str += 2;
		}
		// bp08 = bp08.x
		// bp06 = bp08.y
		// bp04 = bp08.dstcx
		// bp02 = bp08.dstcy
		bp08.cy = _4976_011e;
		bp08.y = (dsty + _4976_0124) - (bp08.cy - 1);
		bp08.cx = _4976_0126;
		do {
			FIRE_BLIT_PICTURE(QUERY_FONT(*str, (Bit8u)clr1, (Bit8u)clr2), dst, &bp08, 0, 0, 5, dstpitch, si, 0, 4, di, bp1a);
			bp08.x += _4976_0126;
			str++;
		} while (*str != 0);
	}
}

//^3929:0B6C
void SkWinCore::DRAW_VP_STR(Bit16u dstx, Bit16u dsty, Bit16u clr1, const Bit8u *str)
{
	// draw text to dungeon/inventory viewport.
	// draw dialogue text such as.
	// a) version: "V1.0"
	// b) button text: "A6 86 97 A8" [ロート゛] (=load), "8B 84 A7 92 A5" [キャンセル] (=cancel)
	// c) savegame name: "0 GAMEE", "GAME 1 STRONG", "2 UNUSED", "3 UNUSED", ...

	DRAW_STRING(
		glbBackBuffViewport,
		-1,
		_4976_00f6,
		dstx,
		dsty,
		clr1,
		glbPaletteT16[COLOR_DARKEST_GRAY] | 0x4000,
		str,
		8
		);
	return;
}



//^0CEE:159B
void SkWinCore::QUERY_MESSAGE_TEXT(U8 *str, ObjectID rl, Bit16u ww)
{
	ENTER(154);
	U16 si = 0;
	U8 bp009a[200];	memset(bp009a, 0, 200);// U8 bp009a[128]; SPX: extended to 200, else it'll crash for some DM1 texts. Added also init
	U8 *bp04 = bp009a;
	U8 bp19 = 0xff;
	Text *bp10;
	U16 bp18;
	if (rl.DBType() == dbScroll) {
		Scroll *_bp10 = GET_ADDRESS_OF_RECORD7(rl);
		U16 bp14 = _bp10->w2_a_f();
		if (bp14 > 1) {	// original: if (bp14 != 0) // fixed to > 1 as it is done in DMDC2
			bp19 = U8(bp14);	// get text number reference which is within GDAT
			goto _1678;
		}
		// SPX: else, get text reference from dungeon.dat
		bp10 = &reinterpret_cast<Text *>(static_cast<U8 *>(glbDBObjectData[dbText]))[_bp10->ReferredText() & 0x03ff];
	}
	else {
		ATLASSERT(rl.DBType() == dbText);

		bp10 = GET_ADDRESS_OF_RECORD2(rl);
	}
	bp18 = bp10->TextMode();
	if (bp18 == 0 || (bp18 == 1 && bp10->SimpleTextExtUsage() == 14)) {
		if (bp10->TextVisibility() != 0 || (ww & 0x8000) != 0) {
			if (bp18 == 1) {
				bp19 = bp10->TextIndex() & 0xff;
			}
_1678:
			ww = ww & 0x7fff;
			U8 bp15;
			if (ww == 1) {
				*(bp04++) = vbLf;
				bp15 = ' ';
			}
			bp15 = vbLf;
			if (bp19 != 0xff) {
				if (SkCodeParam::bBWMode) {
#if (XDMX_BLOODWYCH_ENGINE == 1)
					bp04 += SK_STRLEN(QUERY_BWDAT_TEXT(bp19, bp04));
#endif // XDMX_BLOODWYCH_ENGINE
				}
				else // normal DM2 mode
				// SPX: text message stored in GDAT and not DUNGEON.DAT
					bp04 += SK_STRLEN(QUERY_GDAT_TEXT(GDAT_CATEGORY_x03_MESSAGES, 0x00, bp19, bp04));
			}
			else {
				U16 bp14 = 0;
				U16 di = 0;
				U16 *bp0c = &dunTextData[bp10->TextIndex()];

				while (true) {
					U16 bp12;
					if (bp14 == 0) {
						si = *(bp0c++);
						bp12 = (si >> 10) & 0x1f;
					}
					else if (bp14 == 1) {
						bp12 = (si >> 5) & 0x1f;
					}
					else {
						bp12 = si & 0x1f;
					}
					bp14 = ++bp14 % 3;
					if (di != 0) {
						*bp04 = 0;
						const U8 *bp08;
						if (di == 30) {
							bp08 = reinterpret_cast<const Bit8u *>(_4976_0262[bp12]);
						}
						else {
							bp08 = reinterpret_cast<const Bit8u *>(_4976_0362[bp12]);
						}
						SK_STRCAT(bp04, bp08);
						bp04 += SK_STRLEN(bp08);
						di = 0;
						continue;
					}
					if (bp12 < 0x1c) {
						if (bp12 == 0x1a) {
							bp12 = 0x20;
							*(bp04++) = U8(bp12);
							continue;
						}
						else if (bp12 == 0x1b) {
							bp12 = 0x2e;
							*(bp04++) = U8(bp12);
							continue;
						}
						else {
							bp12 += 0x41;
							*(bp04++) = U8(bp12);
							continue;
						}
					}
					else if (bp12 == 0x1c) {
						*(bp04++) = bp15;
						continue;
					}
					else if (bp12 <= 0x1e) {
						di = bp12;
						continue;
					}
					else {
						break;
					}
				}
			}
		}
	}
	*bp04 = 0;
	FORMAT_SKSTR(bp009a, str);
	return;
}



//^2759:006D
Bit16u SkWinCore::QUERY_CMDSTR_TEXT(const Bit8u *cmdStr, const Bit8u *name)
{
	// returns value of name in cmdstr.
	// for example, returns 0 if you query "WH" for "ﾚﾉ:CM4SK4BZ2TR3TA-2EX5PB38DM40".

	// cmdStr: e.g. "ﾚﾉ:CM4SK4BZ2TR3TA-2EX5PB38DM40".
	// name: e.g. "WH"

	// Note: BETA uses another format: "FREEZE:CM=11 ST=10 SK=14 BZ=20 TR=3 TA=-10 EX=22"


	ENTER(2);
	i16 si = 0;
	while ((cmdStr = SK_STRSTR(cmdStr, name)) != NULL) {
		cmdStr += SK_STRLEN(name);
		if (cmdStr[0] == '=') {
			cmdStr++;
		}
		Bit16u di = 0;
		if (cmdStr[0] == '-') {
			cmdStr++;
			di = 1;
		}

		Bit8u bp01;
		while ((bp01 = cmdStr[0]) >= '0' && bp01 <= '9') {
			si = bp01 +(si * 10) - '0';
			cmdStr++;
		}
		if (di != 0) {
			si = -si;
		}
	}
	return si;
}

//^2759:0101
Bit16u SkWinCore::QUERY_CMDSTR_ENTRY(Bit8u cls1, Bit8u cls2, Bit8u cls4, Bit16u cmdNum)
{
	ENTER(132);
	Bit8u bp0084[128];
	Bit8u *bp04;
	if (cmdNum >= 18 || *(bp04 = QUERY_GDAT_TEXT(cls1, cls2, cls4, bp0084)) == 0) {
		return 0;
	}
	else {
		return QUERY_CMDSTR_TEXT(bp04, reinterpret_cast<const Bit8u *>(ItemCommandsTable[cmdNum]));
	}
}

