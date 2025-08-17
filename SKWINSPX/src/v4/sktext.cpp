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
	//^3929:0826
	ENTER(0);
	//^3929:082A
	FIRE_HIDE_MOUSE_CURSOR();
	//^3929:082F
	FIRE_FILL_SCREEN_RECT(RECT_BOTTOM_MESSAGE_3_LINES, glbPaletteT16[COLOR_BLACK]);
	//^3929:0842
	FIRE_SHOW_MOUSE_CURSOR();
	//^3929:0847
	_3929_07e1(0, 0);
	//^3929:0851
	for (U16 si = 0; si < 1; si++) {
		//^3929:0855
		tlbTimerTickRemoveHintMessage[si] = -1;
		//^3929:0866
	}
	//^3929:086C
	return;
}


//^3929:0914
void SkWinCore::_3929_0914_HINT_TEXT()
{
	//^3929:0914
	ENTER(0);
	//^3929:0917
	_3929_0826();
	//^3929:091B
	tlbTimerTickRemoveHintMessage[0] = -1;
	//^3929:0927
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
	//^3929:0BA6
	DRAW_STRING(_4976_4964, -1, glbScreenWidth, dstx, dsty+1, clr1, clr2, str, 8);
}

//^3929:0929
// SPX _3929_0929 renamed DRAW_HINT_TEXT
void SkWinCore::DRAW_HINT_TEXT(U16 clr1, U8 *str)
{
	//^3929:0929
	ENTER(8);
	//^3929:092D
	SRECT bp08;
	QUERY_EXPANDED_RECT(RECT_BOTTOM_MESSAGE_3_LINES, &bp08);
	//^3929:093C
	if (_4976_5c06 != 0) {
		//^3929:0943
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
		//^3929:0996
		FIRE_HIDE_MOUSE_CURSOR();
		//^3929:099B
		PRINT_SYSERR_TEXT(
			bp08.x + _4976_475c,
			glbTimerTickRemoveHintMessageIndex * _4976_013a +bp08.y + _4976_0130 - _4976_0136 + _4976_5c12 -1,
			clr1,
            glbPaletteT16[COLOR_BLACK],
			str
			);
		//^3929:09D8
		FIRE_SHOW_MOUSE_CURSOR();
	}
	//^3929:09DD
	tlbTimerTickRemoveHintMessage[glbTimerTickRemoveHintMessageIndex] = glbGameTick + 70;	// is it a clock before message disappear ?
	//^3929:09F9
	return;
}

//^3929:09FB
// SPX: _3929_09fb renamed DISPLAY_HINT_TEXT
void SkWinCore::DISPLAY_HINT_TEXT(Bit16u color, const U8 *str) //#DS=4976
{
	//printf("HINT: %s\n", str);
	//^3929:09FB
	ENTER(102);
	//^3929:0A01
	//U16 si = color;
	U16 absoluteColor = 255;	// from 256 palette
	//^3929:0A04
	if (_4976_5c06 != 0) {	// roll message ?
		//^3929:0A0B
		SCROLLBOX_MESSAGE();
		//^3929:0A10
		FILL_ENTIRE_PICT(_4976_5c08, glbPaletteT16[COLOR_BLACK]);
		//^3929:0A2A
		_4976_5c06 = 0;
	}
	//^3929:0A30
	absoluteColor = glbPaletteT16[color];
	//^3929:0A3B
	for (U16 bp02 = 0; str[bp02] != 0; ) {
		//^3929:0A43
		U8 bp66[100]; memset(bp66,0,100);
		U16 di = _3929_04e2_DRAW_TEXT_STRINGS(str, bp66, &bp02, _4976_013e - _4976_475c);	// _4976_013e - _4976_475c somehow can move text to left (thus out of screen)
		//^3929:0A64
		if (str[bp02] == '\n') {
			//^3929:0A70
			_3929_0914_HINT_TEXT();
			//^3929:0A74
			DRAW_HINT_TEXT(absoluteColor, bp66);
			//^3929:0A81
			_4976_475c = 0;
			//^3929:0A87
			bp02++;
			//^3929:0A8A
			continue;
		}
		//^3929:0A8C
		if (str[bp02] != 0) {
			//^3929:0A98
			DRAW_HINT_TEXT(absoluteColor, bp66);
			//^3929:0AA5
			_3929_0914_HINT_TEXT();
			//^3929:0AA9
			_4976_475c = 12;
			//^3929:0AAF
			continue;
		}
		//^3929:0AB1
		DRAW_HINT_TEXT(absoluteColor, bp66);
		//^3929:0ABE
		_4976_475c += di;
		//^3929:0AC2
	}
	//^3929:0AD1
	if (_4976_5c06 != 0) {
		//^3929:0AD8
		SCROLLBOX_MESSAGE();
		//^3929:0ADD
		FILL_ENTIRE_PICT(_4976_5c08, glbPaletteT16[COLOR_BLACK]);
	}
	//^3929:0AFD
	return;
}

//^3929:0B5A
// SPX : _3929_0b5a renamed DISPLAY_HINT_NEW_LINE
void SkWinCore::DISPLAY_HINT_NEW_LINE()
{
	//^3929:0B5A
	ENTER(0);
	//^3929:0B5D
	DISPLAY_HINT_TEXT(COLOR_BLACK, const_cast<U8 *>(strEndLine));
	//^3929:0B6A
	return;
}

//^2405:00C9
// SPX: _2405_00c9 renamed DISPLAY_TAKEN_ITEM_NAME
void SkWinCore::DISPLAY_TAKEN_ITEM_NAME(ObjectID rl)
{
	//^2405:00C9
	ENTER(0);
	//^2405:00CC
	DISPLAY_HINT_NEW_LINE();
	//^2405:00D1
	DISPLAY_HINT_TEXT(COLOR_LIGHTER_GRAY, GET_ITEM_NAME(rl));
	//^2405:00E5
	return;
}



//^32CB:0E03
void SkWinCore::DRAW_TEXT_TO_BACKBUFF(i16 xx, i16 yy, U8 *str)
{
	//^32CB:0E03
	ENTER(24);
	//^32CB:0E07
	if (QUERY_MBCS_PRESENCE(str) == 0) {
		//^32CB:0E18
		for (U8 *bp04 = str; *bp04 != 0; bp04++) {
			//^32CB:0E26
			if (*bp04 >= 'A' && *bp04 <= 'Z') {
				//^32CB:0E35
				*bp04 = *bp04 -0x40;
			}
			//^32CB:0E42
			else if (*bp04 >= 0x7b) {
				//^32CB:0E4B
				*bp04 = *bp04 -0x60;
			}
			//^32CB:0E52
		}
	}
	//^32CB:0E5E
	i16 bp06;
	i16 bp08;
	if (QUERY_STR_METRICS(str, &bp06, &bp08) != 0) {
		//^32CB:0E7A
		U8 bp18[16];
		COPY_MEMORY(glbPaletteT16, bp18, 16);
		//^32CB:0E93
		_0b36_037e(bp18, U8(_4976_5a88), -1, -1, 16);
		//^32CB:0EAA
		DRAW_STRONG_TEXT(_4976_4c16, -1, _4976_00f6, xx -(bp06 >> 1), yy, bp18[15], bp18[0]|0x4000, str);
	}
	//^32CB:0EE3
	return;
}


//^2066:33C4
void SkWinCore::_2066_33c4(U8 *str, U16 yy) 
{
	//^2066:33C4
	ENTER(0);
	//^2066:33C7
	SK_STRCPY(str, _4976_3b68);
	str[5] = yy +0x30;
	//^2066:33E5
	return;
}

//^2066:398A
void SkWinCore::_2066_398a(i16 xx)
{
	int iColorText = COLOR_ORANGE;
	int iColorTextHighlighted = COLOR_YELLOW;
	//^2066:398A
	ENTER(10);
	//^2066:3990

	if (SkCodeParam::bDM2V5Mode) {
		iColorText = COLOR_YELLOW;
		iColorTextHighlighted = COLOR_WHITE;
	}

	SRECT bp0a;
	QUERY_EXPANDED_RECT(451, &bp0a);
	//^2066:39A0
	i16 di = bp0a.y +4;
	//^2066:39A8
	for (i16 si = 0; si < 10; si++) {
		//^2066:39AC
		U16 bp02 = (si == xx) ? glbPaletteT16[iColorTextHighlighted] : glbPaletteT16[iColorText];
		//^2066:39C8
		DRAW_VP_STR(bp0a.x, di, bp02, _4976_5250[si].sSavegameName);
		//^2066:39EB
		di += glbPanelStatsYDelta;
		//^2066:39EF
	}
	//^2066:39F5
	return;
}
//^2066:3820
void SkWinCore::_2066_3820(U8 *xx, U16 yy)
{
	//^2066:3820
	ENTER(8);
	//^2066:3824
	SRECT bp08;
	QUERY_EXPANDED_RECT(0x1c5, &bp08);
	//^2066:3834
	DRAW_DIALOGUE_PICT(QUERY_GDAT_IMAGE_ENTRY_BUFF(0x1a, 0x81, 0x00), _4976_4c16, &bp08, 
		bp08.x, bp08.y, -1, QUERY_GDAT_IMAGE_LOCALPAL(0x1a, 0x81, 0x00));
	//^2066:3873
	DRAW_VP_STR(bp08.x, bp08.y +4, glbPaletteT16[COLOR_YELLOW], xx);
	//^2066:3896
	if (yy != 0) {
		//^2066:389C
		bp08.x += SK_STRLEN(xx) * _4976_0126;
		bp08.cx = _4976_0126;
		bp08.cy -= 4;
		//^2066:38BA
		FIRE_FILL_BACKBUFF_RECT(&bp08, glbPaletteT16[COLOR_ORANGE]);
	}
	//^2066:38D2
	_0aaf_002f();
	//^2066:38D7
	return;
}


//^3E74:5B7C
void SkWinCore::_3e74_5b7c(U16 xx)
{
	//^3E74:5B7C
	ENTER(20);
	//^3E74:5B80
	_476d_05b6(xx);
	//^3E74:5B89
	if (_4976_484d++ == _4976_484f[RCJ(4,_4976_484d)]) {
		//^3E74:5B9F
		if (_4976_484f[RCJ(4,_4976_484d)] != 0)
			//^3E74:5BAA
			return;
		//^3E74:5BAD
		const U8 *bp04 = _4976_4853;
		//^3E74:5BB5
		DISPLAY_HINT_TEXT(COLOR_WHITE, bp04);
		//^3E74:5BC5
		U8 bp14[16];
		DISPLAY_HINT_TEXT(COLOR_WHITE, SK_LTOA10(2, bp14));
		//^3E74:5BE2
		_3929_0b01(15, 46);
		//^3E74:5BED
		_3929_0b20(15, 8);
		//^3E74:5BFB
		_3929_0b20(15, glbCRAMSize);
		//^3E74:5C0D
		_3929_0b20(15, _4976_5ea6);
		//^3E74:5C1F
		_3929_0b20(15, _4976_5d66);
		//^3E74:5C31
		bp04 = _4976_485a;
		//^3E74:5C39
		SK_STRCPY(bp14, bp04);
		//^3E74:5C4C
		if (_4976_5bee[0] != 0)
			//^3E74:5C53
			bp14[0] = 0x47;
		//^3E74:5C57
		if (_4976_5bee[1] != 0)
			//^3E74:5C5E
			bp14[1] = 0x53;
		//^3E74:5C62
		if (_4976_5bee[2] != 0)
			//^3E74:5C69
			bp14[2] = 0x45;
		//^3E74:5C6D
		DISPLAY_HINT_TEXT(COLOR_WHITE, bp14);
	}
	//^3E74:5C7C
	_4976_484d = 0;
	//^3E74:5C82
	return;
}


//^3929:04E2
// _3929_04e2 renamed _3929_04e2_DRAW_TEXT_STRINGS
Bit16u SkWinCore::_3929_04e2_DRAW_TEXT_STRINGS(const Bit8u *strIn, Bit8u *strOut, Bit16u *textPos, i16 maxWidth)
{
	//^3929:04E2
	//^3929:04E8
	Bit16u si = *textPos;
	i16 bp02 = 0;
	Bit16u di = 0;
	Bit16u bp04 = 0xffff;
    Bit16u bp06 = 0;
	Bit16u bp08 = 0;
	//^3929:0504
	Bit8u bp0d;
	skxxxf *bp0c;
	if (QUERY_MBCS_PRESENCE(strIn) != 0 || _4976_475e != 0xff) {
		//^3929:051E
		if (_4976_475e == 0xff) {
			//^3929:0525
			_4976_475e = 1;
		}
		else {
			//^3929:052C
			strOut[di] = 2; di++;
			//^3929:0534
			strOut[di] = 0x20 + _4976_475e;
			//^3929:053C
            di++;
		}

		while (true) {
			//^3929:053D
			strOut[di] = 0;
			//^3929:0544
			if (strIn[si] == vbLf || strIn[si] == 0) {
				//^3929:0553
				if (strIn[si] == 0) {
					_4976_475e = 0xff;
				}
				//^3929:0564
				break;
			}
			//^3929:0567
			bp0d = strIn[si];
			//^3929:0570
			Bit8u bp0e;
			if (bp0d >= 0xf0) {
				//^3929:0574
				bp0e = strIn[si +1];
			}
			else {
				//^3929:057D
				if (bp0d < 0x20) {
					//^3929:0583
					strOut[di] = strIn[si]; si++; di++;
					//^3929:0591
					if (bp0d == 2) {
						//^3929:0597
						_4976_475e = (strOut[di] = strIn[si]) + 0xe0; si++; di++;
					}
					//^3929:05AA
					continue;
				}
				//^3929:05AC
				bp0e = bp0d;
				bp0d = 0;
				//^3929:05B6
				if (strIn[si] == 0x20) {
					//^3929:05BF
					bp04 = si;
					bp06 = di;
					bp08 = bp02;
				}
			}
			//^3929:05CB
			Bit8u bp0f;
			bp0c = QUERY_CHAR_METRICS(_4976_475e, bp0d, bp0e, &bp0f);
			//^3929:05E9
			if (bp0c == NULL) {
				//^3929:05ED
				si++;
				if (bp0d != 0) {
					//^3929:05F7
					si++;
				}
				//^3929:05F8
				continue;
			}
			//^3929:05FB
			if (i16(bp0c->w6) + _4976_0132 + bp02 <= maxWidth) {
				//^3929:060E
				if (bp02 == 0) {
					//^3929:0614
					bp02 = - _4976_0132;
				}
				//^3929:061C
				bp02 += bp0c->w6 + _4976_0132;
				//^3929:062A
				if (bp0d != 0) {
					//^3929:0630
					strOut[di] = strIn[si]; si++; di++;
				}
				//^3929:063E
				strOut[di] = strIn[si]; si++;
				//^3929:064B
				di++; continue;
			}
			//^3929:064E
			if (bp04 != 0xffff) {
				//^3929:0654
_0654:
				*textPos = bp04 +1;
				//^3929:065E
				strOut[bp06] = 0;
				//^3929:0668
				return bp08;
			}
			//^3929:066D
			break;
		}
	}
	else {
		while (true) {
			//^3929:066F
			strOut[di] = 0;
			//^3929:0676
			if (strIn[si] == vbLf || strIn[si] == 0)
				//^3929:0685
				break;
			//^3929:0687
			if (strIn[si] == 0x20) {
				//^3929:0690
				bp04 = si;
				bp06 = di;
				bp08 = bp02;
			}
			//^3929:069C
			if (bp02 + _4976_0126 <= maxWidth) {
				//^3929:06A8
				if (bp02 == 0) {
					//^3929:06AE
					bp02 = - _4976_0120;
				}
				//^3929:06B6
				bp02 += _4976_0126;
				//^3929:06BC
				strOut[di] = strIn[si]; si++; di++;
				//^3929:06CA
				continue;
			}
			//^3929:06CC
			if (bp04 == 0xffff) {
				//^3929:06D0
				break;
			}
			goto _0654;
		}
	}
	//^3929:06D4
	*textPos = si;
	//^3929:06DA
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
	//^3929:00E6
	Bit8u bp0b = *str++;
	//^3929:00F8
	Bit8u bp0c;
	if (bp0b >= 0xF0) {
		//^3929:00FC
		bp0c = *str;
		str++;
	}
	else {
		//^3929:010A
		if (bp0b < 0x20) {
			//^3929:0110
			if (bp0b != 0x02)
				return str;
			//^3929:0119
			*ww = *str + 0xE0;
			str++;
			return str;
		}
		//^3929:012D
		bp0c = bp0b;
		bp0b = 0;
	}
	//^3929:0137
	Bit8u bp0d;
	skxxxf *bp04 = QUERY_CHAR_METRICS(*ww, bp0b, bp0c, &bp0d);
	if (bp04 == NULL)
		return str;
	//^3929:015F
	Bit16u bp18 = _4976_5ca8;
	_4976_5ca8 = -_4976_5d76;
	//^3929:0170
	// SPX: add protection here : if the graphics.dat does not contain japanese font, replacing the missing font with default image (yukman) will lead into a crash
	// Therefore, we check if the font exist; if not, just leave for now.
	int iItemSize = QUERY_GDAT_ENTRY_DATA_LENGTH(GDAT_CATEGORY_INTERFACE_GENERAL, 0x01, dtImage, 0x00);
	if (iItemSize <= 0)
	//	return (const Bit8u*) "?";	// this will lead to an infinite loop when displaying HIT damage
		return str;

	Bit8u *bp08 = QUERY_GDAT_IMAGE_ENTRY_BUFF(GDAT_CATEGORY_INTERFACE_GENERAL, *ww, bp0d);	// 0x1C
	//^3929:018B
	Bit8u bp2a[16]; // 2a-1b
	Bit16u bp0a;
	if (dstbpp != 4) {
		//^3929:0191
		bp2a[ 0] = (Bit8u)clr2;
		bp2a[15] = (Bit8u)clr1;
		//^3929:019D
		bp0a = ((clr2 & 0x4000) != 0) ? 0 : -1;
		//^3929:01A6
	}
	else {
		//^3929:01B1
		if ((clr2 & 0x4000) != 0) {
			//^3929:01B8
			bp0a = clr2 = (clr1 == 0) ? 1 : 0;
		}
		else {
			//^3929:01CD
			bp0a = -1;
		}
		//^3929:01D2
		Bit8u *bp12;
		Bit16u di;
		Bit16u si;
		Bit32u bp16;
		if ((clr1 == 0 && clr2 == 0) || _4976_5d76 == 0) {
			//^3929:01E5
			bp08 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x001c, *ww, bp0d);
			//^3929:0200
			if (clr1 != 0 || clr2 != 15) {
				//^3929:020F
				Bit8u *bp12 = bp08;
				Bit16u di = ((Bit16u *)bp08)[-2];
				Bit16u si = ((Bit16u *)bp08)[-1];
				//^3929:0226
				//^3929:02E5
				goto _02e5;
			}
		}
		else {
			//^3929:0229
			bp16 = (QUERY_GDAT_ENTRY_DATA_INDEX(0x1c, *ww, 1, bp0d) & 0x1fff) 
				| 0x4000 
				| ((clr2 & 0x000f) << 4) 
				| ((clr1 & 0x000f)     );
			//^3929:026D
			Bit16u bp1a;
			if (ADD_CACHE_HASH(bp16, &bp1a) != 0) {
				//^3929:0284
				bp08 = QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp1a);
				//^3929:0293
			}
			else {
				//^3929:0295
				di = (bp04->w2 * bp04->w6);
				si = (bp04->w4 * bp04->w8);
				ALLOC_NEW_PICT(bp1a, di, si, 4);
				//^3929:02BB
				bp12 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x1c, *ww, bp0d);
				//^3929:02D6
				bp08 = QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp1a);
				//^3929:02E5
_02e5:
				Bit8u bp3a[16]; // 3a-2b
				bp3a[ 0] = (Bit8u)clr2;
				bp3a[15] = (Bit8u)clr1;
				//^3929:02F1
				FIRE_STRETCH_BLIT_TO_MEMORY_4TO4BPP(bp12, bp08, di, si, di, si, bp3a);
			}
			//^3929:030E
			if (_4976_5d76 != 0) {
				//^3929:0315
				_3e74_585a(bp1a, 0);
			}
		}
	}
	//^3929:0321
	if (zz >= 0) {
		//^3929:0327
		dst = QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(zz);
	}
	//^3929:0336
	bp0c -= bp04->b1;
	//^3929:0340
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
	//^3929:03AE
	*xx += bp04->w6 + _4976_0132;
	//^3929:03BF
	FREE_PICT_ENTRY(bp08);
	//^3929:03CC
	_4976_5ca8 = bp18;
	//^3929:03D2
	return str;
}


//^470A:0008
Bit8u *SkWinCore::QUERY_FONT(i8 input, Bit8u fore, Bit8u back)
{
	//^470A:0008
	Bit8u bp02 = fore << 4;
	Bit8u bp03 = back << 4;
	Bit16u si = 0;
	Bit16u di = 0;
	for (; di < 6; di++) {
		//^470A:0026
		U8 bp01 = _4976_5c0e[(di << 7) + input];
		i8 bp06;
		for (bp06 = 0; bp06 < 3; bp06++) {
			//^470A:0046
			_4976_5e72[si++] = (((bp01 & 0x10) != 0) ? bp02 : bp03) | (((bp01 & 0x08) != 0) ? fore : back);
			bp01 <<= 2;
			//^470A:0071
		}
		//^470A:007A
	}
	//^470A:0080
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
	//^470A:0089
	Bit16u di = dstbpp;
	//^470A:0089
	if (*str == 0)
		return;
	//^470A:00A0
	Bit16u si;
	if ((clr2 & 0x4000) != 0) {
		//^470A:00A7
		clr2 &= 0xBFFF;
		si = clr2;
	}
	else {
		//^470A:00B4
		si = -1;
	}
	//^470A:00B7
	SRECT bp08;
	bp08.x = dstx;
	//^470A:00BD
	if (QUERY_MBCS_PRESENCE(str) != 0) {
		//^470A:00CE
		Bit8u bp09 = 1;
		dsty -= 14;
		//^470A:00D6
		if (si != 0xFFFF) {
			//^470A:00DB
            clr2 |= 0x4000;
		}
		do {
			//^470A:00E4
			str = DRAW_MBCS_STR(str, dst, ss, &bp09, clr1, clr2, &dstx, &dsty, dstpitch, di);
			//^470A:011E
		} while (*str != 0);
	}
	else {
		//^470A:0127
		Bit8u bp1a[16];
		if (di != 4) {
			//^470A:012C
			bp1a[0] = (Bit8u)clr1;
			bp1a[1] = (Bit8u)clr2;
			clr1 = 0;
			clr2 = 1;
			//^470A:0142
			if (si != 0xFFFF) {
				si = 1;
			}
		}
		//^470A:014A
		if (str[0] == 0x02 && str[1] == 0x20) {
			str += 2;
		}
		//^470A:015E
		// bp08 = bp08.x
		// bp06 = bp08.y
		// bp04 = bp08.dstcx
		// bp02 = bp08.dstcy
		bp08.cy = _4976_011e;
		bp08.y = (dsty + _4976_0124) - (bp08.cy - 1);
		bp08.cx = _4976_0126;
		do {
			//^470A:017A
			FIRE_BLIT_PICTURE(QUERY_FONT(*str, (Bit8u)clr1, (Bit8u)clr2), dst, &bp08, 0, 0, 5, dstpitch, si, 0, 4, di, bp1a);
			//^470A:01B7
			bp08.x += _4976_0126;
			str++;
			//^470A:01C0
		} while (*str != 0);
	}
	//^470A:01C9
}

//^3929:0B6C
void SkWinCore::DRAW_VP_STR(Bit16u dstx, Bit16u dsty, Bit16u clr1, const Bit8u *str)
{
	// draw text to dungeon/inventory viewport.
	// draw dialogue text such as.
	// a) version: "V1.0"
	// b) button text: "A6 86 97 A8" [ロート゛] (=load), "8B 84 A7 92 A5" [キャンセル] (=cancel)
	// c) savegame name: "0 GAMEE", "GAME 1 STRONG", "2 UNUSED", "3 UNUSED", ...

	//^3929:0B6C
	//^3929:0B6F
	DRAW_STRING(
		_4976_4c16,
		-1,
		_4976_00f6,
		dstx,
		dsty,
		clr1,
		glbPaletteT16[COLOR_DARKEST_GRAY] | 0x4000,
		str,
		8
		);
	//^3929:0BA4
	return;
}



//^0CEE:159B
void SkWinCore::QUERY_MESSAGE_TEXT(U8 *str, ObjectID rl, Bit16u ww)
{
	//^0CEE:159B
	ENTER(154);
	//^0CEE:15A1
	U16 si = 0;
	//^0CEE:15A3
	U8 bp009a[200];	memset(bp009a, 0, 200);// U8 bp009a[128]; SPX: extended to 200, else it'll crash for some DM1 texts. Added also init
	U8 *bp04 = bp009a;
	//^0CEE:15AD
	U8 bp19 = 0xff;
	//^0CEE:15B1
	Text *bp10;
	U16 bp18;
	if (rl.DBType() == dbScroll) {
		//^0CEE:15BF
		Scroll *_bp10 = GET_ADDRESS_OF_RECORD7(rl);
		//^0CEE:15CD
		U16 bp14 = _bp10->w2_a_f();
		//^0CEE:15DD
		if (bp14 > 1) {	// original: if (bp14 != 0) // fixed to > 1 as it is done in DMDC2
			//^0CEE:15E1
			bp19 = U8(bp14);	// get text number reference which is within GDAT
			//^0CEE:15E4
			goto _1678;
		}
		//^0CEE:15E7
		// SPX: else, get text reference from dungeon.dat
		bp10 = &reinterpret_cast<Text *>(static_cast<U8 *>(glbDBObjectData[dbText]))[_bp10->ReferredText() & 0x03ff];
	}
	else {
		ATLASSERT(rl.DBType() == dbText);

		//^0CEE:1609
		bp10 = GET_ADDRESS_OF_RECORD2(rl);
	}
	//^0CEE:1617
	//^0CEE:161A
	bp18 = bp10->TextMode();
	//^0CEE:1626
	if (bp18 == 0 || (bp18 == 1 && bp10->SimpleTextExtUsage() == 14)) {
		//^0CEE:164B
		if (bp10->TextVisibility() != 0 || (ww & 0x8000) != 0) {
			//^0CEE:1663
			if (bp18 == 1) {
				//^0CEE:1669
				bp19 = bp10->TextIndex() & 0xff;
			}
			//^0CEE:167B
_1678:
			ww = ww & 0x7fff;
			//^0CEE:1684
			U8 bp15;
			if (ww == 1) {
				//^0CEE:168A
				*(bp04++) = vbLf;
				//^0CEE:1694
				bp15 = ' ';
			}
			//^0CEE:1698
			bp15 = vbLf;
			//^0CEE:169C
			if (bp19 != 0xff) {
				//^0CEE:16A2
				if (SkCodeParam::bBWMode) {
#if (XDMX_BLOODWYCH_ENGINE == 1)
					bp04 += SK_STRLEN(QUERY_BWDAT_TEXT(bp19, bp04));
#endif // XDMX_BLOODWYCH_ENGINE
				}
				else // normal DM2 mode
				// SPX: text message stored in GDAT and not DUNGEON.DAT
					bp04 += SK_STRLEN(QUERY_GDAT_TEXT(GDAT_CATEGORY_MESSAGES, 0x00, bp19, bp04));
			}
			else {
				//^0CEE:16C7
				U16 bp14 = 0;
				//^0CEE:16CC
				U16 di = 0;
				//^0CEE:16CE
				U16 *bp0c = &dunTextData[bp10->TextIndex()];

				while (true) {
					//^0CEE:16ED
					U16 bp12;
					if (bp14 == 0) {
						//^0CEE:16F3
						si = *(bp0c++);
						//^0CEE:16FD
						bp12 = (si >> 10) & 0x1f;
					}
					//^0CEE:1704
					else if (bp14 == 1) {
						//^0CEE:170A
						bp12 = (si >> 5) & 0x1f;
					}
					else {
						//^0CEE:1711
						bp12 = si & 0x1f;
					}
					//^0CEE:1713
					//^0CEE:1719
					bp14 = ++bp14 % 3;
					//^0CEE:1729
					if (di != 0) {
						//^0CEE:172D
						*bp04 = 0;
						//^0CEE:1734
						const U8 *bp08;
						if (di == 30) {
							//^0CEE:1739
							bp08 = reinterpret_cast<const Bit8u *>(_4976_0262[bp12]);
						}
						else {
							//^0CEE:1744
							bp08 = reinterpret_cast<const Bit8u *>(_4976_0362[bp12]);
						}
						//^0CEE:174C
						//^0CEE:1752
						SK_STRCAT(bp04, bp08);
						//^0CEE:1766
						bp04 += SK_STRLEN(bp08);
						//^0CEE:1776
						di = 0;
						//^0CEE:1778
						continue;
					}
					//^0CEE:177B
					if (bp12 < 0x1c) {
						//^0CEE:1781
						if (bp12 == 0x1a) {
							//^0CEE:1787
							bp12 = 0x20;
							//^0CEE:179F
							//^0CEE:178C
							*(bp04++) = U8(bp12);
							//^0CEE:17B9
							continue;
						}
						//^0CEE:178E
						else if (bp12 == 0x1b) {
							//^0CEE:1794
							bp12 = 0x2e;
							//^0CEE:1799
							//^0CEE:179F
							*(bp04++) = U8(bp12);
							//^0CEE:17B9
							continue;
						}
						else {
							//^0CEE:179B
							bp12 += 0x41;
							//^0CEE:179F
							*(bp04++) = U8(bp12);
							//^0CEE:17B9
							continue;
						}
					}
					//^0CEE:17A7
					else if (bp12 == 0x1c) {
						//^0CEE:17AD
						*(bp04++) = bp15;
						//^0CEE:17B9
						continue;
					}
					//^0CEE:17BC
					else if (bp12 <= 0x1e) {
						//^0CEE:17C2
						di = bp12;
						//^0CEE:17C5
						continue;
					}
					else {
						//^0CEE:17C0
						break;
					}
				}
			}
		}
	}
	//^0CEE:17C8
	*bp04 = 0;
	//^0CEE:17CF
	FORMAT_SKSTR(bp009a, str);
	//^0CEE:17E3
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


	//^2759:006D
	ENTER(2);
	//^2759:0073
	i16 si = 0;
	while ((cmdStr = SK_STRSTR(cmdStr, name)) != NULL) {
		//^2759:0077
		cmdStr += SK_STRLEN(name);
		//^2759:0087
		if (cmdStr[0] == '=') {
			//^2759:0090
			cmdStr++;
		}
		//^2759:0093
		Bit16u di = 0;
		//^2759:0095
		if (cmdStr[0] == '-') {
			//^2759:009E
			cmdStr++;
			di = 1;
		}

		Bit8u bp01;
		while ((bp01 = cmdStr[0]) >= '0' && bp01 <= '9') {
			//^2759:00A6
			si = bp01 +(si * 10) - '0';
			cmdStr++;
			//^2759:00BD
		}
		//^2759:00D0
		if (di != 0) {
			//^2759:00D4
			si = -si;
		}
		//^2759:00DA
	}
	//^2759:00FB
	return si;
}

//^2759:0101
Bit16u SkWinCore::QUERY_CMDSTR_ENTRY(Bit8u cls1, Bit8u cls2, Bit8u cls4, Bit16u cmdNum)
{
	//^2759:0101
	ENTER(132);
	//^2759:0105
	Bit8u bp0084[128];
	Bit8u *bp04;
	if (cmdNum >= 18 || *(bp04 = QUERY_GDAT_TEXT(cls1, cls2, cls4, bp0084)) == 0) {
		//^2759:0135
		return 0;
	}
	else {
		//^2759:0139
		return QUERY_CMDSTR_TEXT(bp04, reinterpret_cast<const Bit8u *>(ItemCommandsTable[cmdNum]));
	}
}

