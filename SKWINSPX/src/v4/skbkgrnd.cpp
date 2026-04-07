//------------------------------------------------------------------------------
// SK-BACKGROUND
//	Manages the distant graphics, like towers and skullkeep castle
//------------------------------------------------------------------------------

#include <skcore.h>
#include <skdebug.h>
#include <skglobal.h>

//#if defined(_USE_MFC80) || defined(_USE_MFC60)
//#include <skmfc.h>
//#include <resource.h>
//#endif // defined(_USE_MFC80) || defined(_USE_MFC60)


//^3DF7:075F
// SPX: _3df7_075f renamed RETRIEVE_ENVIRONMENT_CMD_CD_FW
X16 SkWinCore::RETRIEVE_ENVIRONMENT_CMD_CD_FW(DistantEnvironment *ref)
{
	ENTER(128);
	U8 bp80[128];
	if (QUERY_GDAT_TEXT(GDAT_CATEGORY_x17_ENVIRONMENT, glbMapGraphicsSet, ref->envImg, bp80)[0] == 0)
		return 0;
	ref->cmCD = QUERY_CMDSTR_TEXT(bp80, reinterpret_cast<const Bit8u *>(EnvCM_CD));
	ref->w6 = ref->w4 = 0;
	ref->cmFW = U8(QUERY_CMDSTR_TEXT(bp80, reinterpret_cast<const Bit8u *>(EnvCM_FW)));
	ref->b9 = ref->b8 = 0x40;
	return 1;
}



//^32CB:56BC
// SPX: _32cb_56bc renamed ENVIRONMENT_DRAW_DISTANT_ELEMENT
void SkWinCore::ENVIRONMENT_DRAW_DISTANT_ELEMENT(DistantEnvironment *ref, X16 dir, X16 xx, X16 yy)
{
	ENTER(6);
	X16 bp02;	// SPX => bp02 = mirror flip
	if (ref->cmCD == 0)
		return;
	if (ref->cmFW == 8 && SET_GRAPHICS_FLIP_FROM_POSITION(1, dir, xx, yy) != 0) {
		bp02 = 1;
	}
	else if (ref->cmFW == 2 && SET_GRAPHICS_FLIP_FROM_POSITION(0x20, dir, xx, yy) != 0) {
		bp02 = 1;
	}
	else if (ref->cmFW == 0x40 && SET_GRAPHICS_FLIP_FROM_POSITION(1, dir, xx, yy) != 0) {
		bp02 = 1;
	}
	else if (ref->cmFW == 0x20 && SET_GRAPHICS_FLIP_FROM_POSITION(0x20, dir, xx, yy) != 0) {
		bp02 = 1;
	}
	else {
		bp02 = 0;
	}

	X16 bp04 = ref->b8;
	X16 bp06 = ref->b9;
	X16 di = ref->w4;
	X16 si = ref->w6;
	if (cd.pi.glbIsPlayerMoving != 0)
	{
		di = CALC_STRETCHED_SIZE(di, 0x34);
		si = CALC_STRETCHED_SIZE(si, 0x34);
		bp04 = CALC_STRETCHED_SIZE(bp04, 0x34);
		bp06 = CALC_STRETCHED_SIZE(bp06, 0x34);
		if (ref->b8 == 0x40) {
			di += _4976_00fe.x;
			if (ref->cmCD == 0x1771) {	// 0x1771 = 6001
				si += _4976_4681;
			}
			else {
				si += _4976_00fe.y;
			}
		}
		else {
			//^32CB:57DD
			si += _4976_467f;
		}
	}
	// SPX: the CD command seems to hold the rectno of the element.
	// CD = 6000, img = 0 for the distant Skullkeep image
	// CD = 6001, img = 99 (0x63) for the distant horizon
	// CD = 6002, img = {100, 101, 102} for distant lightning
	// CD = 6004, img = {103, 104, 105} for the cloudy skies
	// CD = 6005, img = {106, 107, 108} for the wet ground
	U16 iElementRecto = ref->cmCD;	// TODO : original was ref->cmCD & 0x8000 => SPX: I don't get that 0x8000 as it will give rectno 0 then display nothing ...
	SkD((DLV_DBG_DIST, "Distant Element >> CD=%d FW=%d / Img=%02d, rectno=%d\n"
		, ref->cmCD, ref->cmFW
		, ref->envImg
		, iElementRecto
		));
	QUERY_TEMP_PICST(bp02, bp04, bp06, 0, 0, 0, iElementRecto, -1, glbSceneColorKey, -1, GDAT_CATEGORY_x17_ENVIRONMENT, glbMapGraphicsSet, ref->envImg);
	glbTempPicture.w32 += di;
	glbTempPicture.w34 += si;
	DRAW_TEMP_PICST();        
	return;
}

//^32CB:54CE
i16 SkWinCore::_32cb_54ce(X16 dir, i16 *xx, i16 *yy, X16 xl, X16 yl)
{
	ENTER(4);
	i16 si;
	X16 di;
	switch (dir) {
		case 0://^54E3
			//^32CB:54E3
			si = *xx -yl;
			di = xl -*xx;
			break;
		case 1://^54F9
			//^32CB:54F9
			si = xl -*xx;
			di = yl -*yy;
			break;
		case 2://^550C
			//^32CB:550C
			si = yl -*yy;
			di = *xx -xl;
			break;
		case 3://^5522
			//^32CB:5522
			si = *xx -xl;
			di = *yy -yl;
			break;
	}
	*xx = di;
	*yy = si;
	if (si < 1)
		return 0;
	U16 bp02 = di * di + si * si;
	if (bp02 <= 2)
		return 1;
	U16 cx = bp02 >> 1;
	U16 bp04;
	do {
		bp04 = cx;
		cx = (cx +(bp02 / cx)) >> 1;
	} while (cx < bp04);
	return bp04;
}

//^32CB:5598
// SPX: _32cb_5598 renamed ENVIRONMENT_SET_DISTANT_ELEMENT
X16 SkWinCore::ENVIRONMENT_SET_DISTANT_ELEMENT(DistantEnvironment *ref, U8 *str, X16 dir, X16 xx, X16 yy)
{
	ENTER(12);
	ref->w4 = ref->w6 = 0;
	X16 bp02 = QUERY_CMDSTR_TEXT(str, reinterpret_cast<const Bit8u *>(EnvCM_MV));
	if (bp02 == 0) {
		ref->b8 = 0x40;
		ref->b9 = 0x40;
		return 1;
	}
	if (bp02 == 1) {
		i16 bp08 = xx + dunMapLocalHeader->MapOffsetX();
		i16 bp0a = yy + dunMapLocalHeader->MapOffsetY();
		X16 bp0c = QUERY_CMDSTR_TEXT(str, reinterpret_cast<const Bit8u *>(EnvCM_XLocation));
		X16 di = QUERY_CMDSTR_TEXT(str, reinterpret_cast<const Bit8u *>(EnvCM_YLocation));
		i16 si = _32cb_54ce(dir, &bp08, &bp0a, bp0c, di);
		if (bp0a >= 1) {
			X16 bp04 = QUERY_CMDSTR_TEXT(str, reinterpret_cast<const Bit8u *>(EnvCM_FD));
			X16 bp06 = max_value(1, 0x40 -(si -bp04));
			bp06 = (((bp06 << 7) >> 6) +1) >> 1;
			ref->w4 = (i32(bp08) * 210) / si;
			ref->b8 = ref->b9 = X8(bp06);
			return 1; // SPX: Element is visible
		}
	}
	return 0; // SPX: Element is not visible (wrong direction)
}

//^32CB:5824
// SPX: _32cb_5824 renamed ENVIRONMENT_DISPLAY_ELEMENTS
void SkWinCore::ENVIRONMENT_DISPLAY_ELEMENTS(X16 dir, X16 xx, X16 yy)
{
	ENTER(134);
	X16 di = xx; // xpos
	X16 si = yy; // ypos
	DistantEnvironment *bp04;
	if (glbRainFlagSomething != 0) {
		bp04 = &_4976_5c2a[0];
		while (bp04->envImg != 0xff) {
			ENVIRONMENT_DRAW_DISTANT_ELEMENT(bp04++, dir, di, si);
		}
	}
	U8 bp05;
	U8 bp0086[128];
	if (_4976_4683 != glbMapGraphicsSet) {
		_4976_4683 = glbMapGraphicsSet;
		_4976_592a = 1;
		ZERO_MEMORY(_4976_592b, 13);
		for (bp05 = 0; bp05 <= 0x63; bp05++) {
			if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_x17_ENVIRONMENT, glbMapGraphicsSet, dtImage, bp05) == 0)
				continue;
			if (QUERY_GDAT_TEXT(GDAT_CATEGORY_x17_ENVIRONMENT, glbMapGraphicsSet, bp05, bp0086)[0] == 0)
				continue;
			_4976_592b[bp05 >> 3] = _4976_592b[bp05 >> 3] | (1 << (bp05 & 7));
			_4976_592a = 0;
		}
	}
	if (_4976_592a != 0)
		return;
	bp04 = &_4976_5c2a[0];
	// SPX: Cool! up to 100 distant elements to have in sky/background!
	// 0x63 contains the horizon graphics ..
	for (bp05 = 0; bp05 <= 0x63; bp05++) {
		if ((_4976_592b[bp05 >> 3] & (1 << (bp05 & 7))) == 0)
			continue;
		QUERY_GDAT_TEXT(GDAT_CATEGORY_x17_ENVIRONMENT, glbMapGraphicsSet, bp05, bp0086);
		if (ENVIRONMENT_SET_DISTANT_ELEMENT(bp04, bp0086, dir, di, si) == 0)
			continue;
		bp04->envImg = bp05;
		bp04->cmCD = QUERY_CMDSTR_TEXT(bp0086, reinterpret_cast<const Bit8u *>(EnvCM_CD2));
		bp04->cmFW = U8(QUERY_CMDSTR_TEXT(bp0086, reinterpret_cast<const Bit8u *>(EnvCM_FW2)));
		ENVIRONMENT_DRAW_DISTANT_ELEMENT(bp04, dir, di, si);
	}
	return;
}
