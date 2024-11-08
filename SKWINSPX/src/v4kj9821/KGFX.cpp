#include <StdAfx.h>	// Required for MVC6 compilation

#include <SkVer.h>

#include <KCORE.H>
#include <SkDebug.h> // for Unr();

//#define UseAltic 0 // SPX: not Altic here

//^0759:016C
void SkWinCore::ANIM_FILL_SEQ_4BPP(U16 offDst, i16 clr, U16 cnt)
{
	ATLASSERT(cnt != 0);

	//^0759:016C
	ENTER(0);
	//^0759:0170
	U8 *esdi = _089c_0354;
	U16 bx = offDst;
	U16 cx = cnt;
	U8 ah = U8(clr);
	bool carry = (bx & 1) != 0;
	bx >>= 1;
	if (!carry) {
		//^0759:0182
		esdi += bx;
	}
	else {
		//^0759:0186
		esdi += bx;
		U8 al = (*esdi & 0xf0) | ah;
		stosb(esdi, al);
		if (--cx == 0) return;
	}
	//^0759:0193
	U8 al = (ah << 4) | ah;
	ah = al;
	bool carry2 = (cx & 1) != 0;
	cx >>= 1;
	bool carry3 = (cx & 1) != 0;
	cx >>= 1;
	if (carry3) {
		stosb(esdi, al);
	}
	U16 ax = al | (ah << 8);
	for (; cx != 0; cx--) stosw(esdi, ax);
	if (carry2) {
		//^0759:01B2
		al = *esdi;
		ax &= 0xf00f;
		al |= U8(ax >> 8);
		stosb(esdi, al);
	}
	//^0759:01BB
	return;
}
//^0759:013B
void SkWinCore::ANIM_SETPIXEL_SEQ_4BPP(U16 offDst, i8 clr)
{
	//^0759:013B
	ENTER(0);
	//^0759:013F
	U8 *esdi = _089c_0354;
	U16 bx = offDst;
	bool carry = (bx & 1) != 0;
	bx >>= 1;
	if (carry) {
		//^0759:014B
		esdi += bx;
		U8 al = *esdi;
		al &= 0xf0;
		al |= clr;
		stosb(esdi, al);
	}
	else {
		//^0759:0158
		esdi += bx;
		U8 al = *esdi;
		al &= 0x0f;
		al |= clr << 4;
		stosb(esdi, al);
	}
	//^0759:0169
	return;
}
//^0759:01BE
void SkWinCore::ANIM_BLIT_TO_MEMORY_ROW_4TO4BPP(U16 offSrc, U16 offDst, U16 width)
{
#if UseAltic
	_4976_5e6a = _089c_0354;
	_4976_5e64 = _089c_0348;

	FIRE_BLIT_TO_MEMORY_ROW_4TO4BPP(offSrc, offDst, width);
#else
	Unr();
#endif
}

//^00EB:0008
void SkWinCore::IBMIO_LOAD_4TO8BPP_PAL(const U8 *pal)
{
	//^00EB:0008
	ENTER(0);
	//^00EB:000D
	U8 *esdi = _00eb_0023;
	const U8 *dssi = pal;
	U16 cx = 16;
	for (; cx != 0; cx--) movsb(esdi, dssi);
	//^00EB:0021
	//^00EB:0033
	return;
}


//^00EB:070C
void SkWinCore::_00eb_070c(U8 *buffSrc, U16 yy, U16 zz, U16 ww) //#DS=04BF
{
	//^00EB:070C
	ENTER(0);
	//^00EB:070F
	LOADDS(0x0c48);
	//^00EB:0715
	_04bf_0cf0 = buffSrc;
	IBMIO_LOAD_4TO8BPP_PAL(_04bf_00dc);
	IBMIO_BLIT_ROW_4TO8BPP(yy, zz, ww);
	//^00EB:073D
	return;
}



//^00EB:0037
void SkWinCore::IBMIO_BLIT_ROW_4TO8BPP(U16 offSrc, U16 offDst, U16 size)
{
	//^00EB:0037
	ENTER(0);
	//^00EB:003C
	U8 *dsbx = _00eb_0023;
	U8 *esdi = _04bf_0e34 + offDst;
	U16 cx = size;
	const U8 *dssi = _04bf_0cf0;
	bool carry1 = (offSrc & 1) != 0;
	offSrc >>= 1;
	dssi += offSrc;
	if (carry1) {
		//^00EB:005C
		U8 al = lodsb(dssi);
		al &= 0x0f;
		al = dsbx[al];
		if (--cx == 0) return;
	}
	//^00EB:0065
	bool carry2 = (cx & 1) != 0;
	cx >>= 1;
	if (cx != 0) {
		//^00EB:0069
		bool carry3 = (cx & 1);
		cx >>= 1;
		if (cx != 0) {
			//^00EB:006E
			U16 bp = 0x0f0f;
			do {
				//^00EB:0073
				U16 ax = lodsw(dssi);
				U16 dx = ax >> 4;
				dx &= bp;
				ax &= bp;
				U8 ah = dsbx[ax & 255];
				U8 al = dsbx[dx & 255];
				U8 dh = dsbx[ax >> 8];
				U8 dl = dsbx[dx >> 8];
				ax = (ah << 8) | al;
				dx = (dh << 8) | dl;
				stosw(esdi, ax);
				stosw(esdi, dx);
				//^00EB:008F
			} while (--cx != 0);
		}
		if (carry3) {
			//^00EB:0095
			U8 al = lodsb(dssi);
			U8 ah = al;
			al >>= 4;
			al = dsbx[al];
			stosb(esdi, al);
			al = ah & 15;
			al = dsbx[al];
			stosb(esdi, al);
		}
	}
	//^00EB:00A6
	if (carry2) {
		//^00EB:00A8
		U8 al = lodsb(dssi);
		al >>= 4;
		al = dsbx[al];
		stosb(esdi, al);
	}
	//^00EB:00B0
	//^00EB:00B2
	return;
}



//^32CB:08C1
void SkWinCore::QUERY_TEMP_PICST(
	U16 mirrorflip, U16 horzStretch, U16 vertStretch, i16 offx, i16 offy, i16 uu, U16 rectno, U16 pp, 
	i16 colorkey1, i16 colorkey2, U8 cls1, U8 cls2, U8 cls4)
{
	SkD((DLV_DBG_GETPIC, "DBG: QUERY_TEMP_PICST(%X,%3d,%3d,%3d,%3d,%04X,%04X,%04X,%2d,%2d,%02X,%02X,%02X)\n"
		, (Bitu)mirrorflip, (Bitu)horzStretch, (Bitu)vertStretch, (Bitu)offx, (Bitu)offy
		, (Bitu)uu, (Bitu)rectno, (Bitu)pp, (Bitu)colorkey1, (Bitu)colorkey2, (Bitu)cls1, (Bitu)cls2, (Bitu)cls4));

	//^32CB:08C1
	ENTER(2);
	//^32CB:08C7
	i16 di = uu;
	QUERY_GDAT_SUMMARY_IMAGE(&glbTempPicture, cls1, cls2, cls4);
	//^32CB:08E2
	if (horzStretch != vertStretch) {
		//^32CB:08EA
		U8 bp01;
		if (di == 2) {
			//^32CB:08EF
			bp01 = 0x14;
			//^32CB:08F3
			goto _08fe;
		}
		//^32CB:08F5
		else if (di == 3) {
			//^32CB:08FA
			bp01 = 0x15;
			//^32CB:08FE
_08fe:
			U16 si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, dtWordValue, bp01);
			//^32CB:0916
			if (si != 0) {
				//^32CB:091A
				horzStretch = ((((si >> 8) << 7) / (si & 255)) +1) >> 1;
			}
		}
	}
	//^32CB:0932
	glbTempPicture.mirrorFlip = mirrorflip;
	glbTempPicture.w52 = horzStretch;
	glbTempPicture.w54 = vertStretch;
	glbTempPicture.w28 += offx;
	glbTempPicture.w30 += offy;
	glbTempPicture.rectNo = rectno;
	glbTempPicture.w26 = pp;
	glbTempPicture.pb44 = _4976_4c16;
	glbTempPicture.colorKeyPassThrough = colorkey1;
	//^32CB:0972
	_32cb_0804(glbTempPicture.b58, di, colorkey1, colorkey2, glbTempPicture.w56);
	//^32CB:0988
	QUERY_PICST_IT(&glbTempPicture);
	//^32CB:0993
	return;
}

//^32CB:0997
void SkWinCore::DRAW_TEMP_PICST()
{
	//^32CB:0997
	ENTER(0);
	//^32CB:099A
	DRAW_PICST(&glbTempPicture);
	//^32CB:09A5
	return;
}


//^0B36:037E
// TODO: gfx related
U8 *SkWinCore::_0b36_037e(U8 *localpal, i8 xx, i16 colorkey1, i16 colorkey2, i16 palentcnt)
{
	//^0B36:037E
	ENTER(8);
	//^0B36:0384
	if (localpal != NULL && xx != 0) {
		//^0B36:0398
		xx = U8(max_value(0, 64 -xx));
		//^0B36:03AF
		for (i16 bp02 = 0; bp02 < palentcnt; bp02++) {
			//^0B36:03B7
			if (bp02 != colorkey1 && bp02 != colorkey2) {
				//^0B36:03CA
				U16 di = _4976_4be2[RCX(256,localpal[bp02])].b0;
				i16 si = 0;
				//^0B36:03E6
				i16 bp04 = max_value((_4976_4bde[RCX(16,di)].pv1[_4976_4be2[localpal[bp02]].b1] * xx) >> 6, 0);
				//^0B36:0433
				for (; _4976_4bde[di].b0 -1 > si; si++) {
					//^0B36:0435
					if (true
						&& _4976_4bde[di].pv1[si] <= bp04
						&& _4976_4bde[di].pv1[si +1] >= bp04
					) {
						//^0B36:046C
						i16 bp06 = bp04 - _4976_4bde[di].pv1[si   ];
						//^0B36:048A
						i16 bp08 = _4976_4bde[di].pv1[si +1] -bp04;
						//^0B36:04A7
						if (bp06 > bp08)
							//^0B36:04AF
							si++;
						//^0B36:04B0
						break;
					}
					//^0B36:04B2
				}
				//^0B36:04CD
				si = min_value(si, _4976_4bde[di].b0 -1);
				//^0B36:04EB

				localpal[bp02] = _4976_4bde[di].pv5[si];
			}
			//^0B36:0508
		}
	}
	//^0B36:03AF
	return localpal;
}

//^32CB:0804
void SkWinCore::_32cb_0804(U8 *localpal, i16 cls4, U16 colorkey1, i16 colorkey2, U16 palentcnt)
{
	//^32CB:0804
	ENTER(0);
	//^32CB:0809
	U16 di = palentcnt;
	//^32CB:080C
	i16 si;
	if (glbIsPlayerMoving != 0) {
		//^32CB:0813
		if (cls4 < 0) {
			//^32CB:0819
			si = max_value(-_4976_5a88, _4976_421b[RCJ(6,-cls4)]);
			//^32CB:0833
			cls4 = 1;
		}
		else {
			//^32CB:083A
			si = _4976_4221[RCJ(5,cls4)];
			//^32CB:0844
			cls4 += 9;
		}
	}
	else {
		//^32CB:084A
		si = _4976_4226[RCJ(5,cls4)];
	}
	//^32CB:0854	// SPX: That part is for translating palette in case of 'fog'
	if (!SkCodeParam::bDisableFogEffect && QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, dt07, U8(cls4)) != 0) {
		//^32CB:086C
		TRANSLATE_PALETTE(localpal, GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, U8(cls4), di);
		//^32CB:0885
		_0b36_037e(localpal, i8(_4976_5a88), colorkey1, colorkey2, di);
	}
	else {
		//^32CB:0892
		_0b36_037e(localpal, 64 -U8(((64 -si) * (64 - _4976_5a88)) >> 6), colorkey1, colorkey2, di);
	}
	//^32CB:08BD
	return;
}


//^32CB:09A7
// SPX: _32cb_09a7 renamed QUERY_MULTILAYERS_PIC
U16 SkWinCore::QUERY_MULTILAYERS_PIC(ExtendedPicture *ref, U8 cls1, U8 cls2, U8 cls4, U16 horzRes, U16 vertRes, i16 zz, U16 mirrorflip, i16 colorkey1, U16 colorkey2)
{
	//^32CB:09A7
	ENTER(0);
	//^32CB:09AB
	QUERY_TEMP_PICST(mirrorflip, horzRes, vertRes, 0, 0, zz, -1, -1, colorkey1, -1, cls1, cls2, cls4);
	//^32CB:09D7
	COPY_MEMORY(&glbTempPicture, ref, sizeof(ExtendedPicture));
	//^32CB:09EE
	ALLOC_PICT_MEMENT(ref);
	//^32CB:09FB
	ref->colorKeyPassThrough = colorkey2;
	//^32CB:0A05
	SET_ORIGIN_RECT(&ref->rc36, ref->width, ref->height);
	//^32CB:0A1F
	U16 si;
	ref->pb44 = ALLOC_NEW_PICT(si = ALLOC_TEMP_CACHE_INDEX(), ref->width, ref->height, 8);
	//^32CB:0A47
	return si;
}


//^44C8:0762
void SkWinCore::FIRE_UPDATE_BLIT_PALETTE(const U8 *localpal)
{
	//^44C8:0762
	ENTER(0);
	//^44C8:0767
	U8 *di = glbBlitPalette16;
	//^44C8:0771
	const U8 *si = localpal;
	//^44C8:0774
	memcpy(di, si, 16);
	//^44C8:078D
	return;
}

//^44C8:0791
void SkWinCore::FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(U16 offSrc, U16 offDst, U16 size)
{
	//^44C8:0791
	ENTER(0);
	//^44C8:0796
	//^44C8:0798
	U8 *bx = glbBlitPalette16;
	//^44C8:079B
	U8 *di = &_4976_5e6a[offDst];
	//^44C8:07A4
	U16 cx = size;
	//^44C8:07A7
	U8 *si = _4976_5e64;
	//^44C8:07AB
	bool carry = (offSrc & 1) ? true : false;
	si += (offSrc >>= 1);
	//^44C8:07B4
	if (carry) {
		//^44C8:07B6
		U8 al = lodsb(si);
		stosb(di, bx[al & 15]);
		cx--;
		//^44C8:07BD
		if (cx == 0) goto _080a;
	}

	{
		//^44C8:07BF
		bool carry = (cx & 1) ? true : false;
		cx >>= 1;
		//^44C8:07C1
		if (cx != 0) {
			//^44C8:07C3
			bool carry = (cx & 1) ? true : false;
			cx >>= 1;
			//^44C8:07C6
			if (cx != 0) {
				//^44C8:07C8
				U16 bp = 0x0f0f;
				do {
					//^44C8:07CD
					U16 ax = lodsw(si);
					U16 dx = ax >> 4;
					dx &= bp;
					ax &= bp;
					//^44C8:07D7
					stosb(di, bx[U8(ax >> 8)]);
					stosb(di, bx[U8(dx >> 8)]);
					stosb(di, bx[U8(ax &255)]);
					stosb(di, bx[U8(dx &255)]);
					//^44C8:07E9
				} while (--cx != 0);
			}
			//^44C8:07ED
			if (carry) {
				//^44C8:07EF
				U8 al = lodsb(si);
				stosb(di, bx[al >> 4     ]);
				stosb(di, bx[al      &255]);
			}
		}
		//^44C8:0801
		if (carry) {
			//^44C8:0803
			U8 al = lodsb(si);
			stosb(di, bx[al >> 4]);
		}
	}
	//^44C8:080A
_080a:
	//^44C8:080D
	return;
}

//	//^44C8:08AE
//	void _44c8_08ae(U16 offSrc, U16 offDst, U16 size, U8 colorkey)
//	{
//		//^44C8:08AE
//		ENTER(0);
//		//^44C8:08B3
//		U8 *bx = glbBlitPalette16;
//		//^44C8:08B8
//		U8 *di = &_4976_5e6a[offDst];
//		//^44C8:08C1
//		U8 dh = colorkey;
//		//^44C8:08C4
//		U16 cx = size;
//		//^44C8:08C7
//		U16 ax = offSrc;
//		bool carry = (ax & 1) ? true : false;
//		ax >>= 1;
//		//^44C8:08CD
//		U8 *si = &_4976_5e64[ax];
//		//^44C8:08D4
//		if (carry) {
//			//^44C8:08D6
//			U8 al = lodsb(si);
//			al &= 15;
//			if (al == dh) {
//				//^44C8:08DD
//				di++;
//			}
//			else {
//				stosb(di, bx[al]);
//			}
//			//^44C8:08E3
//			if (--cx == 0) goto _0931;
//		}
//
//		{
//			//^44C8:08E6
//			bool carry = (cx & 1) ? true : false;
//			cx >>= 1;
//			//^44C8:08E9
//			if (cx != 0) {
//				do {
//					//^44C8:08EB
//					U8 al = lodsb(si);
//					U8 ah = al & 15;
//					al >>= 4;
//					//^44C8:08F4
//					if (al != dh) {
//						//^44C8:08F8
//						if (ah != al) {
//							//^44C8:08FC
//							stosb(di, bx[al]);
//							stosb(di, bx[ah]);
//							//^44C8:0904
//							if (--cx != 0) continue;
//							break;
//						}
//						//^44C8:091D
//						stosb(di, bx[al]);
//						di++;//^44C8:0921
//						if (--cx != 0) continue;
//						break;
//					}
//					//^44C8:0908
//					if (ah != dh) {
//						//^44C8:090C
//						di++;
//						//^44C8:090D
//						stosb(di, bx[ah]);
//						//^44C8:0912
//						if (--cx != 0) continue;
//						break;
//					}
//					//^44C8:0916
//					di += 2;
//					//^44C8:0918
//				} while (--cx != 0);
//			}
//			//^44C8:0923
//			if (carry) {
//				//^44C8:0926
//				U8 al = lodsb(si) >> 4;
//				//^44C8:0927
//				if (al != dh) {
//					//^44C8:092E
//					stosb(di, bx[al]);
//				}
//			}
//		}
//		//^44C8:0931
//_0931:
//		//^44C8:0932
//		return;
//	}


//^44C8:1E43
// Todo : teleporter gfx ???
void SkWinCore::_44c8_1e43(U8 *src, U8 *dst, U8 *zz, SRECT *prc, U16 ss, U16 tt, U16 aa, U16 bb, U16 hazeWidth, U16 dstWidth, i16 colorkey, U8 *localpal)
{
	//^44C8:1E43
	ENTER(16);
	//^44C8:1E49
	FIRE_UPDATE_BLIT_PALETTE(localpal);
	//^44C8:1E55
	hazeWidth = (hazeWidth +1) & 0xfffe;
	//^44C8:1E5F
	i16 bp08 = prc->cx;
	i16 bp06 = prc->cy;
	i16 bp02 = (prc->y * dstWidth) +prc->x;
	i16 bp04 = (bb * hazeWidth) +aa;
	//^44C8:1E89
	U16 di = tt;
	U16 si = ss -tt;
	//^44C8:1E94
	_4976_5e64 = src;
	_4976_5e6a = dst;
	//^44C8:1EAE
	if (zz == NULL) {
		//^44C8:1EB9
		for (U16 bp0a = 0; bp0a < bp06; bp02 += dstWidth, bp0a++) {
			//^44C8:1EC1
			i16 bp0e = bp08;
			//^44C8:1EC7
			U16 bp0c;
			for (bp0c = 0; bp0e >= si; bp0e -= si, bp0c += si, si = ss, di = 0) {
				//^44C8:1ECE
				if (colorkey < 0) {
					//^44C8:1ED4
					FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(di, bp02 +bp0c, si);
					//^44C8:1EE4
					continue;
				}
				//^44C8:1EE6
				FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP(di, bp02 +bp0c, si, U8(colorkey));
				//^44C8:1EFA
			}
			//^44C8:1F0A
			if (bp0e > 0) {
				//^44C8:1F10
				if (colorkey < 0) {
					//^44C8:1F16
					FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(di, bp02 +bp0c, bp0e);
				}
				else {
					//^44C8:1F2A
					FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP(di, bp02 +bp0c, bp0e, U8(colorkey));
				}
				//^44C8:1F40
				si -= bp0e;
				di += bp0e;
			}
			//^44C8:1F46
			if ((bp08 & 1) != 0) {
				//^44C8:1F4D
				if (--si != 0) {
					//^44C8:1F54
					di++;
				}
				else {
					//^44C8:1F57
					si = ss;
					di = 0;
				}
			}
			//^44C8:1F5C
		}
		//^44C8:1F70
		return;
	}
	//^44C8:1F73
	for (U16 bp0a = 0; bp0a < bp06; bp0a++) {
		//^44C8:1F7B
		for (U16 bp10 = bp08; bp10 > 0; ) {
			//^44C8:1F84
			U16 bp0c;
			for (bp0c = 0; bp10 > 0 && _44c8_1e1a(zz, bp04 +bp0c) == 0; ) {
				//^44C8:1F8B
				--bp10;
				++bp0c;
				++di;
				--si;
				//^44C8:1F93
				if (si == 0) {
					//^44C8:1F99
					di = 0;
					si = ss;
				}
				//^44C8:1F9E
			}
			//^44C8:1FBC
			U16 bp0e;
			for (bp0e = 0; bp10 > 0 && _44c8_1e1a(zz, bp04 +bp0c) != 0; ) {
				//^44C8:1FC3
				++bp0e;
				--bp10;
				//^44C8:1FC9
			}
			//^44C8:1FE7
			if (bp0e > 0) {
				//^44C8:1FED
				while (bp0e >= si) {
					//^44C8:1FEF
					if (colorkey < 0) {
						//^44C8:1FF5
						FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(di, bp02 +bp0c, si);
					}
					else {
						//^44C8:2007
						FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP(di, bp02 +bp0c, si, U8(colorkey));
					}
					//^44C8:201B
					bp0e -= si;
					bp0c += si;
					//^44C8:2021
					si = ss;
					di = 0;
					//^44C8:2026
				}
				//^44C8:202B
				if (bp0e > 0) {
					//^44C8:2031
					if (colorkey < 0) {
						//^44C8:2037
						FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(di, bp02 +bp0c, bp0e);
					}
					else {
						//^44C8:204B
						FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP(di, bp02 +bp0c, bp0e, U8(colorkey));
					}
					//^44C8:2061
					si -= bp0e;
					di += bp0e;
				}
			}
			//^44C8:2067
			if ((bp08 & 1) != 0) {
				//^44C8:206E
				if (--si != 0) {
					//^44C8:2075
					++di;
				}
				else {
					//^44C8:2078
					si = ss;
					di = 0;
				}
			}
			//^44C8:207D
			bp04 += hazeWidth;
			bp02 += dstWidth;
			//^44C8:2089
		}
		//^44C8:2092
	}
	//^44C8:20A0
	return;
}




//^32CB:0C7D
void SkWinCore::_32cb_0c7d(ExtendedPicture *ref, U16 xx, U16 yy)
{
	//^32CB:0C7D
	ENTER(16);
	//^32CB:0C83
	if (glbRainStrength == 0) {
		//^32CB:0C8A
		FILL_ENTIRE_PICT(reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(xx)), ref->b58[yy]);
		//^32CB:0CAA
		return;
	}
	//^32CB:0CAD
	U8 bp0b;
	U16 bp0a;
	QUERY_RAINFALL_PARAM(&bp0b, &bp0a);
	//^32CB:0CBE
	i16 bp06;
	i16 bp08;
	QUERY_GDAT_IMAGE_METRICS(0x17, glbMapGraphicsSet, bp0b, &bp06, &bp08);
	//^32CB:0CDA
	U16 di = ALLOC_TEMP_CACHE_INDEX();
	//^32CB:0CE1
	ALLOC_NEW_PICT(di, ref->rc36.cx, bp08, 4);
	//^32CB:0CF6
	U16 si = (ref->rc36.cx +1) & 0xfffe;
	//^32CB:0D03
	U8 *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x17, glbMapGraphicsSet, bp0b);
	//^32CB:0D1B
	FIRE_BLIT_PICTURE(bp04, QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(di), ALLOC_TEMP_ORIGIN_RECT(si, READ_I16(bp04,-2)), 
		0, 0, READ_I16(bp04,-4), si, -1, (bp0a == 1) ? 1 : 0, 4, 4, NULL);
	//^32CB:0D64
	bp04 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(di));
	//^32CB:0D71
	U8 *bp10 = _32cb_0649(0x17, glbMapGraphicsSet, bp0b, 0);
	//^32CB:0D8A
	bp10[0] = ref->b58[yy];
	//^32CB:0D9A
	si = READ_I16(bp04,-4) * READ_I16(bp04,-2);
	//^32CB:0DA7
	_44c8_20a4(bp04, reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(xx)), NULL, &ref->rc36, 
		si -(RAND() & 31) -16, RAND16(60), ref->width, -1, bp10);
	//^32CB:0DF8
	FREE_TEMP_CACHE_INDEX(di);
	//^32CB:0DFF
	return;
}


//^32CB:28C7
void SkWinCore::QUERY_CREATURE_PICST(U16 xx, i16 iDistToPlayer, Creature *xCreature, CreatureInfoData *xInfo, ObjectID rl)
{
	ENTER(28);
	U16 di = 0;
	static int counter = 0;
	U8 iCreatureType = xCreature->CreatureType();
	sk1c9a02c3* xInfoData = GET_CREATURE_INFO_DATA(xCreature, QUERY_CREATURE_AI_SPEC_FROM_TYPE(iCreatureType)); // bp04
	i16 si = CREATURE_SEQUENCE_4937_000f(xInfoData->iAnimSeq, &xInfoData->iAnimFrame);
	// SPX: there might be an issue here where animation frame is not retrieved correctly. A static object would have si = 4, so that its gets images x10 x12 x13 x12
	//si = 4 + counter;
	//counter = (counter + 1)%4;
	printf("Creature Type %d => %02X %02X => seq = %d\n", iCreatureType, xInfoData->iAnimSeq, xInfoData->iAnimFrame, si);
	U16 bp06 = (xInfo == NULL) ? 0 : xInfo->b7;
	U16 bp0a = ((QUERY_CREATURE_AI_SPEC_FLAGS(rl) & 4) != 0) ? 2 : ((_4976_5aa0 - xCreature->b15_0_1()) & 3);
	U16 bp08 = _4976_5a98[si][bp0a +10];	// _4976_5a98 table has 4+8 bytes, 4 first points to address of item 0653 loaded into mem
	U8 bp0c = _4976_5a98[si][bp0a +2];

	if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_CREATURES, iCreatureType, dtImage, bp0c) == 0) {
		bp0c = (bp0a +2) & 3;
		if ((bp0c & 1) != 0)
			di = 1;
		bp0c = _4976_5a98[si][bp0c +2];
		if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_CREATURES, iCreatureType, dtImage, bp0c) == 0) {
			di = 0;
			bp0c = _4976_5a98[si][4];
		}
	}
	//^32CB:29FA
	else if (false
		|| (bp08 & 1) != 0
		|| ((bp06 & 64) != 0 && (bp06 & 2) != 0 && (bp06 & 1) == 0)
	) {
		//^32CB:2A16
		di = 1;
	}
	//^32CB:2A19
	if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_CREATURES, iCreatureType, dtImage, bp0c) == 0) {
		//^32CB:2A31
		bp0c = bp0a -6;
		//^32CB:2A39
		if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_CREATURES, iCreatureType, dtImage, bp0c) == 0) { // try to get 0xFC ?
			//^32CB:2A4E
			if (true
				&& bp0c == 0xfb
				&& QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_CREATURES, iCreatureType, dtImage, bp0c +2) != 0
			) {
				//^32CB:2A6E
				di = 1;
				//^32CB:2A71
				bp0c = bp0c +2;
			}
			else {
				//^32CB:2A7B
				di = 0;
				//^32CB:2A7D
				bp0c = 0xfc;
			}
		}
	}
	//^32CB:2A81
	i16 bp10 = tlbDistanceStretch[RCJ(5,iDistToPlayer)];
	//^32CB:2A8D
	i16 bp14 = (xInfo != NULL && xInfo->Command == ccmDestroy) ? xInfo->w14 : si;
	//^32CB:2AAB
	U16 bp12 = _4976_5a98[bp14][0];
	//^32CB:2AC1
	if (xx == 3 && _4976_5aa2 != 0) {
		//^32CB:2ACE
		bp12 = _4976_41d0[RCJ(7,glbTargetTypeMoveObject)];
		bp10 = _4976_41d7[RCJ(7,glbTargetTypeMoveObject)];
		bp0a = 0;
	}
	else {
		//^32CB:2AE9
		bp10 = CALC_STRETCHED_SIZE(_4976_5a98[si][bp0a +6], bp10);	// table read from item 653 containing sequence/frame size info
	}
	//^32CB:2B0D
	U16 bp0e = QUERY_CREATURE_BLIT_RECTI(xx, bp12, bp0a) | 0x8000;
	//^32CB:2B24
	i16 bp18, bp16;
	bp18 = bp16 = i8(_4976_5a98[bp14][1]);
	//^32CB:2B3D
	if (bp16 != 0) {
		//^32CB:2B44
		i16 bp1c;
		i16 bp1a;
		switch (bp0a) {
		case 0:
			//^32CB:2B50
			bp1c = -7;
			//^32CB:2B55
			//^32CB:2B5C
			bp1a = 0;
			//^32CB:2B61
			break;

		case 2:
			//^32CB:2B57
			bp1c = 7;
			//^32CB:2B5C
			bp1a = 0;
			//^32CB:2B61
			break;

		case 1:
		case 3:
			//^32CB:2B63
			bp1a = -64;
			bp1c = 0;

			break;
		}
		//^32CB:2B6D
		bp1a = CALC_STRETCHED_SIZE(bp1a, bp16);
		//^32CB:2B7D
		bp18 = CALC_STRETCHED_SIZE(bp1c, bp18);
	}

	//^32CB:2B8D
	QUERY_TEMP_PICST(di, bp10, bp10, _4976_41de[RCJ(8,bp06 & 7)] +bp16, _4976_41de[RCJ(8,(bp06 >> 3) & 7)] +bp18, iDistToPlayer,
		bp0e, -1, CREATURE_GET_COLORKEY(iCreatureType), -1, 0x0f, iCreatureType, bp0c);
	//^32CB:2BDA
	return;
}


//^32CB:01B6
U16 SkWinCore::_32cb_01b6(U16 xx, U16 yy, U16 ss, U16 tt, U16 *ww)
{
	//^32CB:01B6
	ENTER(8);
	//^32CB:01BC
	ObjectID si = GET_CREATURE_AT(ss, tt);
	//^32CB:01CB
	if (si == OBJECT_NULL)
		//^32CB:01D0
		return 0;
	//^32CB:01D5
	Creature *bp04 = GET_ADDRESS_OF_RECORD4(si);
	//^32CB:01E2
	CreatureInfoData *bp08;
	if (bp04->b5 == 255) {
		//^32CB:01EC
		bp08 = NULL;
	}
	else {
		//^32CB:01F8
		bp08 = &glbTabCreaturesInfo[bp04->b5];
	}
	//^32CB:0216
	QUERY_CREATURE_PICST(3, glbTabYAxisDistance[3], bp04, bp08, si);
	//^32CB:0232
	i16 di = glbTempPicture.colorKeyPassThrough;
	//^32CB:0236
	glbTempPicture.colorKeyPassThrough = -2;
	//^32CB:023C
	DRAW_TEMP_PICST();
	//^32CB:0241
	if (_32cb_00f1_DRAW_PICTURE(xx, yy, di) == 0)
		//^32CB:0253
		//^32CB:01D0
		return 0;
	//^32CB:0256
	if (_4976_5a94 < -75) {
		//^32CB:025D
		*ww = 3;
	}
	//^32CB:0267
	else if (_4976_5a94 > 75) {
		//^32CB:026E
		*ww = 1;
	}
	else {
		//^32CB:0278
		*ww = 0;
	}
	//^32CB:0280
	return 1;
}



//^3E74:0AE3
void SkWinCore::FREE_PICT_ENTRY(Bit8u *buff)
{
	//^3E74:0AE3
	//^3E74:0AE7
	if (_4976_5d76 == 0) {
		//^3E74:0AF1
		sk5cfc_image *bp04 = reinterpret_cast<sk5cfc_image *>(&_4976_5cfc);
		//^3E74:0AF9
		for (; (void *)PTR_PADD(buff,-int(sizeof(sk5cfc_image))) != (void *)bp04->pv0; ) {
			//^3E74:0AFB
			bp04 = bp04->pv0;
		}
		//^3E74:0B31
		bp04->pv0 = bp04->pv0->pv0;
		//^3E74:0B48
		Bit32u bp08 = CALC_IMAGE_BYTE_LENGTH(buff) +0x1e;
		//^3E74:0B60
		if (reinterpret_cast<SkImage *>(PTR_PADD(buff,-int(sizeof(sk5cfc_image))))->AllocLower() == 0) {
			//^3E74:0B7C
			DEALLOC_UPPER_MEMORY(bp08);
		}
		else {
			//^3E74:0B88
			DEALLOC_LOWER_MEMORY(bp08);
		}
	}
	//^3E74:0B94
	return;
}


//^098D:0599 
// SPX: ww is the method to know how to start drawing image from ref point (can be ornate position)
// 0 => centered
// 1 => point is corner left/upper (0,0)
// 2 => point is corner right/upper (x,0)
// 3 => point is corner right/bottom (x,y)
// 4 => point is corner left/bottom (0,y)
// 5 => centered from top (image is then moved to lower)
// 6 => centered from right (image is then moved to left)
// 7 => centered from bottom (image is then moved to upper)
// 8 => centered from left (image is then moved to right)
SRECT *SkWinCore::QUERY_BLIT_RECT(Bit8u *buff, SRECT *rect, Bit16u rectno, i16 *yourcx, i16 *yourcy, i16 ww) //#DS=4976?
{
	SkD((DLV_RCT, "RCT: -> QUERY_BLIT_RECT(%p,%p(     x,     x,     x,     x),%4d,%3d,%3d,%3d)\n"
		, buff, rect, (Bitu)(rectno & 0x7fff), (Bitu)*yourcx, (Bitu)*yourcy, (Bitu)ww));

	//^098D:0599
	//^098D:059F
	i16 di;

	Bit16u bp24 = 0;
	SRECT bp1a;
	Bit16u bp12;
	Bit16u bp10;
	i16 bp0c;
	i16 bp0a;
	SRECT *bp08;
	SRECT *bp04;

	//^098D:05A4
	if (rectno == 0xffff) {
		//^098D:05AA
		return NULL;
	}
	//^098D:05B1
	Bit16u si = rectno & 0x8000;
	//^098D:05B9
	if (si != 0) {
		//^098D:05BD
		rectno &= 0x7fff;
	}
	//^098D:05C6
	bp04 = QUERY_RECT(glbRectNoTable.pb0, rectno);
	//^098D:05DE
	if (bp04 == NULL) {
		//^098D:05AA
		return NULL;
	}
	//^098D:05E4
	SET_SRECT(&bp1a, -10000, -10000, 20000, 20000);
	//^098D:05FC
	SRECT bp22;
	Bit16u bp0e;
	if (ww == -1) {
		//^098D:0602
		bp0e = bp04->x;
	}
	else {
		//^098D:060D
		COPY_MEMORY(bp04, &bp22, sizeof(SRECT));
		//^098D:0624
		bp22.x = bp0e = ww;
		//^098D:062D
		bp04 = &bp22;
	}
	//^098D:0636
	if (bp0e <= 8) {
		//^098D:063C
		bp0a = bp04->cx;
		bp0c = bp04->cy;
	}
	else {
		//^098D:064F
		if (bp0e == 9) {
			//^098D:05AA
			return NULL;
		}
		//^098D:0658
		bp0e -= 10;
		bp0a = 0;
		bp0c = 0;
	}
	//^098D:0666
	if (si != 0) {
		//^098D:066A
		bp0a += *yourcx;
		bp0c += *yourcy;
		*yourcx = 0;
		*yourcy = 0;
	}
	//^098D:068C
	if (buff == NULL && (*yourcx <= 0 || *yourcy <= 0)) {
		//^098D:06A6
		//^098D:05AA
		return NULL;
	}
	//^098D:06A9
	bp10 = 0;
	//^098D:06AE
	for (; bp04->y != 0; ) {
		//^098D:06B1
		if (bp04->x >= 10 && bp04->x <= 18) {
			//^098D:06C6
			bp08 = QUERY_RECT(glbRectNoTable.pb0, bp04->y);
			//^098D:06DF
			if (bp08 == NULL)
				break;
			//^098D:06E6
			si = bp08->cx;
			di = bp08->cy;
			bp12 = bp08->x;
			bp24 = bp08->y;
			//^098D:06FE
			bp08 = QUERY_RECT(glbRectNoTable.pb0, bp08->y);
			//^098D:0717
			if (bp08 == NULL)
				break;
			//^098D:071E
			switch (bp12) {
				default:
					//^098D:072D
					//^098D:05AA
					return NULL;

				case 0:
					//^098D:0730
					di -= (bp08->cy +1) >> 1;
					//^098D:073C
					si -= (bp08->cx +1) >> 1;
					//^098D:0748
					break;

				case 5:
					//^098D:073C
					si -= (bp08->cx +1) >> 1;
					//^098D:0748
					break;

				case 3:
					//^098D:074A
					di -= bp08->cy -1;
					//^098D:0754
					si -= bp08->cx -1;
					//^098D:075C
					//^098D:0746
					break;

				case 2:
					//^098D:0754
					si -= bp08->cx -1;
					//^098D:075C
					//^098D:0746
					break;

				case 6:
					//^098D:075E
					si -= bp08->cx -1;
					//^098D:0768
					di -= (bp08->cy +1) >> 1;
					//^098D:0772
					break;

				case 8:
					//^098D:0768
					di -= (bp08->cy +1) >> 1;
					//^098D:0772
					break;

				case 7:
					//^098D:0774
					si -= (bp08->cx +1) >> 1;
					//^098D:0780
					di -= bp08->cy -1;
					//^098D:0788
					break;

				case 4:
					//^098D:0780
					di -= bp08->cy -1;
					//^098D:0788
					break;

				case 1:
					//^098D:078A
					break;
			}

			//^098D:078A
			bp1a.x += si;
			//^098D:078D
			if (bp1a.x < si) {
				//^098D:0794
				bp1a.x = si;
			}
			//^098D:0797
			if (bp08->cx + si <= bp1a.x + bp1a.cx -1) {
				//^098D:07AB
				bp1a.cx = bp08->cx - bp1a.x + si;
			}
			//^098D:07B7
			bp1a.y += di;
			//^098D:07BA
			if (bp1a.y < di) {
				//^098D:07C1
				bp1a.y = di;
			}
			//^098D:07C4
			if (bp08->cy + di <= bp1a.y + bp1a.cy -1) {
				//^098D:07D8
				bp1a.cy = bp08->cy - bp1a.y + di;
			}
			//^098D:07E4
			switch (bp04->x) {
				default:
					//^098D:07F9
					//^098D:05AA
					return NULL;

				case 10:
					//^098D:07FC
					di += (bp08->cy +1) >> 1;
					//^098D:0808
					si += (bp08->cx +1) >> 1;
					//^098D:0814
					break;

				case 15:
					//^098D:0808
					si += (bp08->cx +1) >> 1;
					//^098D:0814
					break;

				case 13:
					//^098D:0816
					di += bp08->cy -1;
					//^098D:0820
					si += bp08->cx -1;
					//^098D:0814
					break;

				case 12:
					//^098D:0820
					si += bp08->cx -1;
					//^098D:0814
					break;

				case 16:
					//^098D:082A
					si += bp08->cx -1;
					//^098D:0834
					di += (bp08->cy +1) >> 1;
					//^098D:083E
					break;

				case 18:
					//^098D:0834
					di += (bp08->cy +1) >> 1;
					//^098D:083E
					break;

				case 17:
					//^098D:0840
					si += (bp08->cx +1) >> 1;
					//^098D:084C
					di += bp08->cy -1;

					break;

				case 14:
					//^098D:084C
					di += bp08->cy -1;

					break;

				case 11:
					//^098D:0856
					break;
			}
			//^098D:0856
			bp0a += si + bp04->cx;
			//^098D:0862
			bp0c += di + bp04->cy;
			//^098D:086B
		}
		else {
			//^098D:086E
			bp08 = QUERY_RECT(glbRectNoTable.pb0, bp04->y);
			//^098D:088A
			if (bp08 == NULL)
				break;
			//^098D:0891
			bp24 = bp04->y;
			si = bp08->cx;
			di = bp08->cy;
			//^098D:08A6
			if (bp08->x == 1) {
				//^098D:08AC
				bp0a += si;
				bp0c += di;
				bp1a.x += si;
				bp1a.y += di;
			}
			//^098D:08BB
			else if (bp08->x == 9) {
				//^098D:08C7
				switch (bp04->x) {
					case 0:
						//^098D:08DC
						si = bp04->cx - ((si +1) >> 1);
						//^098D:08EC
						//^098D:0960
						di = bp04->cy - ((di +1) >> 1);
						break;

					case 1:
						//^098D:08EE
						si = bp04->cx;
						//^098D:08F5
						di = bp04->cy;
						break;

					case 2:
						//^098D:08FE
						si = bp04->cx - (si -1);
						//^098D:0908
						//^098D:0939
						//^098D:08F5
						di = bp04->cy;
						break;

					case 3:
						//^098D:090A
						si = bp04->cx - (si -1);
						//^098D:0918
						di = bp04->cy - (di -1);
						//^098D:0922
						//^098D:096C
						break;

					case 4:
						//^098D:0924
						si = bp04->cx;
						//^098D:092B
						//^098D:0918
						di = bp04->cy - (di -1);
						//^098D:0922
						//^098D:096C
						break;

					case 5:
						//^098D:092D
						si = bp04->cx - ((si +1) >> 1);
						//^098D:0939
						//^098D:093D
						//^098D:08F5
						di = bp04->cy;
						break;

					case 6:
						//^098D:093F
						si = bp04->cx - (si -1);
						//^098D:0949
						//^098D:08E8
						break;

					case 7:
						//^098D:094B
						si = bp04->cx - ((si +1) >> 1);
						//^098D:0957
						//^098D:0914
						//^098D:0918
						di = bp04->cy - (di -1);
						//^098D:0922
						//^098D:096C
						break;

					case 8:
						//^098D:0959
						si = bp04->cx;
						//^098D:0960
						di = bp04->cy - ((di +1) >> 1);
						break;
				}
				//^098D:0970
				if (bp10 != 0) {
					//^098D:0976
					bp10 = 0;
					bp0a += si;
					bp0c += di;
					bp1a.x += si;
					bp1a.y += di;
				}
				//^098D:0987
				if (bp1a.x < si) {
					//^098D:098C
					bp1a.x = si;
				}
				//^098D:098F
				if (bp08->cx + si <= bp1a.x + bp1a.cx -1) {
					//^098D:09A3
					bp1a.cx = bp08->cx - bp1a.x + si;
				}
				//^098D:09AF
				if (bp1a.y < di) {
					//^098D:09B4
					bp1a.y = di;
				}
				//^098D:09B7
				if (bp08->cy + di <= bp1a.y + bp1a.cy -1) {
					//^098D:09CB
					bp1a.cy = bp08->cy - bp1a.y + di;
				}
			}
			else {
				//^098D:09D9
				if (bp08->x <= 8) {
					//^098D:09E2
					bp10 = 1;
				}
			}
		}
		//^098D:09E7
		bp04 = bp08;
		//^098D:09F3
	}
	//^098D:0A00
	si = *yourcx;
	//^098D:0A08
	if (si == 0) {
		//^098D:0A0C
		si = READ_UI16(buff,-4);
	}
	//^098D:0A13
	di = *yourcy;
	//^098D:0A1B
	if (di == 0) {
		//^098D:0A1F
		di = READ_UI16(buff,-2);
	}
	//^098D:0A26
	switch (bp0e) {
		default:
			//^098D:0A35
			//^098D:05AA
			return NULL;

		case 0:
			//^098D:0A38
			rect->x = bp0a -((si +1) >> 1);
			//^098D:0A48
			//^098D:0AB1
			rect->y = bp0c -((di +1) >> 1);
			break;

		case 1:
			//^098D:0A4A
			rect->x = bp0a;
			//^098D:0A53
			rect->y = bp0c;
			break;

		case 2:
			//^098D:0A5F
			rect->x = bp0a - (si -1);
			//^098D:0A53
			rect->y = bp0c;
			break;

		case 3:
			//^098D:0A64
			rect->x = bp0a - (si -1);
			//^098D:0A72
			rect->y = bp0c - (di -1);
			//^098D:0A7D
			//^098D:0ABE
			break;

		case 4:
			//^098D:0A7F
			rect->x = bp0a;
			//^098D:0A88
			//^098D:0A72
			rect->y = bp0c - (di -1);
			//^098D:0A7D
			//^098D:0ABE
			break;

		case 5:
			//^098D:0A8A
			rect->x = bp0a - ((si +1) >> 1);
			//^098D:0A9A
			//^098D:0A53
			rect->y = bp0c;
			break;

		case 6:
			//^098D:0A9C
			rect->x = bp0a - (si -1);
			//^098D:0A9F
			//^098D:0A3D
			//^098D:0A48
			//^098D:0AB1
			rect->y = bp0c - ((di +1) >> 1);
			break;

		case 7:
			//^098D:0AA1
			rect->x = bp0a - ((si +1) >> 1);
			//^098D:0A72
			rect->y = bp0c - (di -1);
			//^098D:0A7D
			//^098D:0ABE
			break;

		case 8:
			//^098D:0AA8
			rect->x = bp0a;
			//^098D:0AB1
			rect->y = bp0c - ((di +1) >> 1);
			break;
	}
	//^098D:0AC2
	if (_4976_4bca.cx != 0) {
		//^098D:0AC9
		COPY_MEMORY(&_4976_4bca, &bp1a, sizeof(SRECT));
	}
	//^098D:0ADE
	if (_4976_4bc8 != 0 && glbIsPlayerMoving != 0 && bp24 == 3) {
		//^098D:0AF2
		if (UNION_RECT(&bp1a, &_4976_00fe, yourcx, yourcy) == NULL) {
			//^098D:05AA
			return NULL;
		}
	}
	//^098D:0B15
	bp0a = bp1a.x - rect->x;
	//^098D:0B21
	if (bp0a <= 0) {
		//^098D:0B25
		*yourcx = 0;
		//^098D:0B33
		rect->cx = min_value(si, bp1a.cx + bp0a);
	}
	else {
		//^098D:0B37
		*yourcx = bp0a;
		//^098D:0B40
		rect->x = bp1a.x;
		//^098D:0B49
		rect->cx = min_value(si - bp0a, bp1a.cx);
	}
	//^098D:0B60
	bp0c = bp1a.y - rect->y;
	//^098D:0B6D
	if (bp0c <= 0) {
		//^098D:0B71
		*yourcy = 0;
		//^098D:0B79
		rect->cy = min_value(di, bp1a.cy + bp0c);
	}
	else {
		//^098D:0B83
		*yourcy = bp0c;
		//^098D:0B8C
		rect->y = bp1a.y;
		//^098D:0B96
		rect->cy = min_value(di - bp0c, bp1a.cy);
	}
	//^098D:0BAD
	if (rect->cx <= 0 || rect->cy <= 0)
		return NULL;
	//^098D:0BC1

	SkD((DLV_RCT, "RCT: <- QUERY_BLIT_RECT(%p,%p(%6d,%6d,%6d,%6d),%4d,%3d,%3d,%3d)\n"
		, buff, rect, (Bitu)rect->x, (Bitu)rect->y, (Bitu)rect->cx, (Bitu)rect->cy
		, (Bitu)rectno, (Bitu)*yourcx, (Bitu)*yourcy, (Bitu)ww));

	return rect;
}


//^01B0:0008
void SkWinCore::MOVE_MEMORY(const void *buffSrc, void *buffDst, Bit16u size)
{
	memmove(buffDst, buffSrc, size);
}

//^00EB:01EB
void SkWinCore::MOVE_MEMORY_WITH_DELTA(Bit16u offSrc, Bit16u offDst, Bit16u size) //#DS=04BF
{
	//^00EB:01EB
	//^00EB:01EE
	MOVE_MEMORY(
		_04bf_0cf0 +offSrc,
		_04bf_0e34 +offDst,
		size
		);
	//^00EB:021F
	return;
}

//^00EB:0221
void SkWinCore::COPY_MEMORY_WITH_DELTA_AND_TRANSPARENT(Bit16u offSrc, Bit16u offDst, Bit16u size, Bit8u colorkey) //#DS=04BF
{
	//^00EB:0221
	//^00EB:0224
	Bit8u dh = colorkey;
	Bit8u *di = _04bf_0e34 + offDst;
	Bit16u cx = size;
	Bit8u *si = _04bf_0cf0 + offSrc;

	do {
		//^00EB:0242
		Bit8u al = *(si++);
		//^00EB:0243
		if (al != colorkey) {
			//^00EB:0247;
			*(di++) = al;
		}
		else {
			//^00EB:024C
			di++;
		}
		//^00EB:024D
	} while (--cx != 0);
	//^00EB:024F
	return;
}

//^00EB:0812
void SkWinCore::MOVE_MEMORY_STRETCH_13TO16(Bit16u offSrc, Bit16u offDst)
{
	// compose 0.5 stepping viewport. (123% larger)

	//^00EB:0812
	//^00EB:0817
	Bit8u *di = _04bf_0e34 +offDst;
	Bit8u *si = _04bf_0cf0 +offSrc;
	Bit16u cx = 14;

	// DS:SI	E000:871B -> E000:8728 (+13)
	// ES:DI	A000:3340 -> A000:3350 (+16)

	// [DS:SI] in  E000:8728  A4 A7 A2 A7 AB AB A6 A9 A9 A9 A9 A9 A8
	// [ES:DI] out A000:3350  A4 A7 A2 A2 A7 AB AB A6 A9 A9 A9 A9 A9 A9 A9 A8

	// [DS:SI] in  E000:87FB  00 01 02 03 04 05 06 07 08 09 0A 0B 0C
	// [ES:DI] out A000:3480  00 01 02 02 03 04 05 06 07 07 08 09 0A 0B 0B 0C

	do {
		//^00EB:082C
		*di = *si; di++; si++;
		*di = *si; di++; si++;
		*di = *si; di++; 
		*di = *si; di++; si++;
		*di = *si; di++; si++;
		*di = *si; di++; si++;
		*di = *si; di++; si++;
		*di = *si; di++; si++;
		*di = *si; di++; 
		*di = *si; di++; si++;
		*di = *si; di++; si++;
		*di = *si; di++; si++;
		*di = *si; di++; si++;
		*di = *si; di++; si++;
		*di = *si; di++; 
		*di = *si; di++; si++;
		//^00EB:083E
	} while (--cx != 0);
	//^00EB:0840
	return;
}


//^098D:0D3F
SRECT *SkWinCore::SCALE_RECT(Bit16u rectno, SRECT *rc, Bit16u horzResolution, Bit16u vertResolution)
{
	// horzResolution, vertResolution: 10,000 is fair. 5,000 is half. 20,000 is double.

	//^098D:0D3F
	//^098D:0D45
	Bit16u di = horzResolution;
	Bit16u si = vertResolution;
	//^098D:0D4B
	SRECT *bp04 = QUERY_RECT(&glbRectNoTable, rectno);
	//^098D:0D63
	if (bp04 != NULL) {
		//^098D:0D6A
		if (bp04->y != 0) {
			//^098D:0D77
			bp04 = QUERY_RECT(&glbRectNoTable, bp04->y);
			//^098D:0D93
			if (bp04 != NULL) {
				//^098D:0D9A
				if (bp04->x == 9) {
					//^098D:0DA6
					i16 bp06 = (di == 10000) ? (bp04->cx) : (i16)((i32(bp04->cx) * di) / 10000);
					//^098D:0DD6
					i16 bp08 = (si == 10000) ? (bp04->cy) : (i16)((i32(bp04->cy) * si) / 10000);
					//^098D:0E09
					if (bp06 == 0 && di != 0) {
						//^098D:0E13
						bp06 = 1;
					}
					//^098D:0E18
					if (bp08 == 0 && si != 0) {
						//^098D:0E22
						bp08 = 1;
					}
					//^098D:0E27
					if (bp06 > 0 && bp08 > 0) {
						//^098D:0E33
						return QUERY_BLIT_RECT(NULL, rc, rectno, &bp06, &bp08, -1);
					}
				}
			}
		}
	}
	//^098D:0E55
	return NULL;
}

//^098D:0E5D
SRECT *SkWinCore::QUERY_EXPANDED_RECT(Bit16u rectno, SRECT *rc)
{
	//^098D:0E5D
	//^098D:0E60
	return SCALE_RECT(rectno, rc, 10000, 10000);
}


//^0B36:1599
void SkWinCore::DRAW_DIALOGUE_PICT(Bit8u *srcImage, Bit8u *dstImage, SRECT *rect, Bit16u srcx, Bit16u srcy, Bit16u colorkey, Bit8u localpal[16])
{
	// draw the:
	// a) pre rendered dialogue to screen

	//^0B36:1599
	FIRE_BLIT_PICTURE(
		srcImage,
		dstImage,
		rect,
		srcx,
		srcy,
		((Bit16u *)srcImage)[-2], // src-pitch
		((Bit16u *)dstImage)[-2], // dst-pitch
		colorkey,
		0,
		((Bit16u *)srcImage)[-3], // src-bpp
		((Bit16u *)dstImage)[-3], // dst-bpp
		localpal
		);
}


//^3E74:0C16
void SkWinCore::FREE_PICT_BUFF(Bit8u *buff)
{
	// READ_UI16(buff,-6) -> bpp(4 or 8)
	// READ_UI16(buff,-4) -> width
	// READ_UI16(buff,-2) -> height

	//^3E74:0C16
	DEALLOC_UPPER_MEMORY(
		(	(READ_UI16(buff,-6) == 4) 
			? (((READ_UI16(buff,-4) +1) & 0xfffe) >> 1)	// if 4 bpp, round up width to byte align.
			:   (READ_UI16(buff,-4)     & 0xffff)		// if 8 bpp, already byte aligned.
		) * READ_UI16(buff,-2) +6
	);
}


//^098D:0C13
void SkWinCore::TRIM_BLIT_RECT(i16 xx, i16 yy, i16 ss, i16 tt)
{
	//^098D:0C13
	ENTER(0);
	//^098D:0C16
	SET_SRECT(&_4976_4bca, xx, yy, _4976_00f6 -ss -xx, _4976_00f8 -tt -yy);
	//^098D:0C43
	return;
}


//^00EB:05C7
// SPX: _00eb_05c7 renamed SET_GRAPHICS_RGB_PALETTE
X16 SkWinCore::SET_GRAPHICS_RGB_PALETTE(U8 (*pal)[4], X16 yy) //#DS=04BF
{
	//^00EB:05C7
	ENTER(0);
	//^00EB:05CB
	LOADDS(0x0C48);
	i16 si;
	for (si = 0; si < 0x100; si++) {
		//^00EB:05D5
		glbPaletteRGB[si][0] = pal[si][1] >> 2;
		//^00EB:05F7
		glbPaletteRGB[si][1] = pal[si][2] >> 2;
		//^00EB:0619
		glbPaletteRGB[si][2] = pal[si][3] >> 2;
		//^00EB:063B
	}
	//^00EB:0642
	if (glbUpdatePalette == 1)
		//^00EB:0649
		IBMIO_UPDATE_PALETTE_SET();
	//^00EB:064E
	return 1;
}

//^44C8:1BAF
// SPX: _44c8_1baf renamed SET_RGB_PALETTE_FROM_DATA
void SkWinCore::SET_RGB_PALETTE_FROM_DATA(U8 *pal)
{
	//^44C8:1BAF
	ENTER(0);
	//^44C8:1BB2
	glbPaletteIRGBLoaded = SET_GRAPHICS_RGB_PALETTE(reinterpret_cast<U8 (*)[4]>(pal), 0);
	//^44C8:1BC8
	return;
}


//^0B36:00C3
// TODO: Image related ?
Bit8u *SkWinCore::_0b36_00c3(i16 xx, Picture *ref)
{
	//^0B36:00C3
	ENTER(4);
	//^0B36:00C7
	Bit8u *bp04 = reinterpret_cast<Bit8u *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(xx));
	//^0B36:00D6
	if (ref != NULL) {
		//^0B36:00DE
		ref->pb0 = bp04;
		//^0B36:00EE
		ref->w14 = 0;
		ref->w16 = 0;
		//^0B36:00FA
		ref->width = READ_UI16(bp04,-4);
		//^0B36:0108
		ref->height = READ_UI16(bp04,-2);
		//^0B36:0116
		ref->w22 = READ_UI16(bp04,-6);
		//^0B36:0124
        ref->w12 = xx;
		//^0B36:012B
		ref->w4 = 8;
	}
	//^0B36:0131
	return bp04;
}

//^0B36:004B
Bit8u *SkWinCore::QUERY_PICST_IMAGE(Picture *ref)
{
	//^0B36:004B
	ENTER(4);
	//^0B36:004F
	Bit8u *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(ref->b8, ref->b9, ref->b11);
	if (SkCodeParam::bUseFixedMode && bp04 == NULL)
		bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(GDAT_CATEGORY_MISCELLANEOUS, GDAT_ITEM_DEFAULT_INDEX, GDAT_ITEM_DEFAULT_INDEX); // Get Yukman!
	if (SkCodeParam::bUseFixedMode && bp04 == NULL)
		return NULL;
	//^0B36:006F
	ref->pb0 = bp04;
	//^0B36:007F
	ref->w14 = 0;
	ref->w16 = 0;
	//^0B36:008B
	ref->width = READ_UI16(bp04,-4);
	//^0B36:0099
	ref->height = READ_UI16(bp04,-2);
	//^0B36:00A7
	ref->w22 = READ_UI16(bp04,-6);
	//^0B36:00B5
	ref->w4 = 4;
	//^0B36:00BB
	return bp04;
}


//^3E74:0A77
void SkWinCore::FREE_PICT6(SkImage *ref)
{
	//^3E74:0A77
	ENTER(0);
	//^3E74:0A7A
	if (_4976_5d76 == 0) {
		//^3E74:0A81
		if (READ_UI16(ref,-6 +4) == afDefault) {
			//^3E74:0A9D
			DEALLOC_UPPER_MEMORY(READ_UI32(ref,-6 +0));
		}
		else {
			//^3E74:0ABF
			DEALLOC_LOWER_MEMORY(READ_UI32(ref,-6 +0));
		}
	}
	//^3E74:0AE1
	return;
}

//^3E74:52F4
i16 SkWinCore::QUERY_GDAT_PICT_OFFSET(Bit8u cls1, Bit8u cls2, Bit8u cls4)
{
	//^3E74:52F4
	ENTER(8);
	//^3E74:52FA
	Bit16u di = 0;
	//^3E74:52FC
	RawEntry *bp04 = QUERY_GDAT_ENTRYPTR(cls1, cls2, dtImage, cls4);
	//^3E74:5317
	if (bp04 == NULL) {
		//^3E74:5325
		return 0;
	}
	else {
		//^3E74:532A
		Bit16u si = bp04->data;
		//^3E74:5331
		SkImage *bp04;
		if (glbShelfMemoryTable[si].Absent()) {
			//^3E74:534E
			di = 1;
			//^3E74:5351
			bp04 = reinterpret_cast<SkImage *>(QUERY_GDAT_ENTRY_DATA_PTR(cls1, cls2, dtImage, cls4));
		}
		else {
			//^3E74:5368
			bp04 = reinterpret_cast<SkImage *>(REALIZE_GRAPHICS_DATA_MEMORY(glbShelfMemoryTable[si]));
		}
		//^3E74:5386
		i16 bp06 = bp04->Xoffset();
		//^3E74:5392
		i16 bp08;
		if (bp06 == -32) {
			//^3E74:5397
			si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, dtImageOffset, cls4);
		}
		else {
			//^3E74:53AE
			bp08 = bp04->Yoffset();
#if DM2_EXTENDED_MODE == 1
			if (bp08 == -32 || bp08 == 31) {
				bp08 = 0;
			}
#else
			//^3E74:53BB
			if (bp08 == -32) {
				//^3E74:53C0
				bp08 = 0;
			}
#endif
			//^3E74:53C5
			si = (bp06 << 8) | Bit8u(bp08);
		}
		//^3E74:53D2
		if (di != 0) {
			//^3E74:53D8
			FREE_PICT6(bp04);
		}
		//^3E74:53E4
		return si;
	}
}

//^0B36:0520
//ExtendedPicture *SkWinCore::QUERY_GDAT_SUMMARY_IMAGE(ExtendedPicture *ref, Bit8u cls1, Bit8u cls2, Bit8u cls4)
ExtendedPicture *SkWinCore::QUERY_GDAT_SUMMARY_IMAGE(ExtendedPicture *ref, Bit8u iCategory, Bit8u iItemNo, Bit8u iEntry)
{
	SkD((DLV_DBG_GETPIC, "DBG: QUERY_GDAT_SUMMARY_IMAGE(%p,%02X,%02X,%02X)\n"
		, ref, (Bitu)iCategory, (Bitu)iItemNo, (Bitu)iEntry));

	//^0B36:0520
	ENTER(8);
    //^0B36:0524
	ZERO_MEMORY(ref, 314);
	//^0B36:0537
	ref->w12 = -1;
	ref->b8 = iCategory;
	ref->b9 = iItemNo;
	ref->b10 = dtImage;
	ref->b11 = iEntry;
	ref->w54 = 64;
	ref->w52 = 64;
	ref->rectNo = 0xffff;
	ref->w26 = 0xffff;
	ref->pb44 = _4976_4964;
	ref->colorKeyPassThrough = -1;
	//^0B36:0586
	if (iCategory != 0xff) {
		//^0B36:058F
		ref->w6 = QUERY_GDAT_ENTRY_DATA_INDEX(iCategory, iItemNo, dtImage, iEntry);
		//^0B36:05AC
		i16 bp02 = QUERY_GDAT_ENTRY_DATA_INDEX(iCategory, iItemNo, dtImageOffset, 0xFE);
		//^0B36:05C4
		if (bp02 != 0) {
			//^0B36:05C8
			ref->w28 += i8(bp02 >> 8);
			//^0B36:05D3
			ref->w30 += i8(bp02);
		}
		//^0B36:05DB
		bp02 = QUERY_GDAT_PICT_OFFSET(iCategory, iItemNo, iEntry);
		//^0B36:05F2
		if (bp02 != 0) {
			//^0B36:05F6
			ref->w28 += i8(bp02 >> 8);
			//^0B36:0601
			ref->w30 += i8(bp02);
		}
		//^0B36:0609
		Bit8u *bp06 = QUERY_GDAT_IMAGE_LOCALPAL(iCategory, iItemNo, iEntry);
		//^0B36:0623
		if (bp06 == NULL) {
			//^0B36:062B
			ref->w56 = 256;
			//^0B36:0634
			bp06 = ref->b58;
			//^0B36:0643
			i16 bp08 = 0;
			//^0B36:0648
			for (; bp08 < 256; bp08++) {
				//^0B36:064A
				bp06[bp08] = (Bit8u)bp08;
				//^0B36:0656
			}
		}
		else {
			//^0B36:0662
			ref->w56 = 16;
			//^0B36:066B
			COPY_MEMORY(bp06, ref->b58, 16);
		}
	}
	//^0B36:0687
	return ref;
}


//^3E74:58BF
void SkWinCore::ALLOC_IMAGE_MEMENT(Bit8u cls1, Bit8u cls2, Bit8u cls4)
{
	//^3E74:58BF
	ENTER(4);
	//^3E74:58C5
	Bit16u si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, dtImage, cls4);
	//^3E74:58DC
	if (si != 0xffff && glbShelfMemoryTable[si].Absent()) {
		//^3E74:58FC
		si = QUERY_GDAT_ENTRY_DATA_INDEX(0x15,0xfe,dtImage,0xfe);			
	}
	//^3E74:590F
	if (si != 0xffff) {
		//^3E74:5914
		Bit16u di = QUERY_MEMENTI_FROM(si);
		//^3E74:591C
		if (di == 0xffff) {
			if (glbShelfMemoryTable[si].Absent()) {
				//^3E74:593C
				return;
			}
			//^3E74:593E
			SkImage *bp04 = reinterpret_cast<SkImage *>(REALIZE_GRAPHICS_DATA_MEMORY(glbShelfMemoryTable[si]));
			//^3E74:595C
			if (bp04->Yoffset() != -32) {
				//^3E74:5969
				return;
			}
			//^3E74:596B
			if (bp04->BitsPixel() != 8) {
				//^3E74:5980
				return;
			}
			//^3E74:5982
			_4976_4807 = si;
			//^3E74:5986
			return;
		}
		else {
			//^3E74:5988
			_3e74_4549(di);
		}
	}
	//^3E74:598E
	return;
}

//^0B36:018F
void SkWinCore::ALLOC_PICT_MEMENT(Picture *ref)
{
	//^0B36:018F
	ENTER(0);
	//^0B36:0192
	if ((ref->w4 & 0x0004) != 0) {
		//^0B36:019D
		ALLOC_IMAGE_MEMENT(ref->b8, ref->b9, ref->b11);
	}
	//^0B36:01B6
	else if ((ref->w4 & 0x0008) != 0) {
		//^0B36:01C1
		_3e74_583a(ref->w12);
	}

}

//^0B36:068F
Bit32u SkWinCore::CALC_PICT_ENT_HASH(ExtendedPicture *ref)
{
	//^0B36:068F
	ENTER(0);
	//^0B36:0692
	return (Bit32u(ref->w6 & 0x1fff) << 12) | ((ref->w52 & 0x007f) << 5) | (ref->w54 & 0x001f);
}

//^3E74:5992
void SkWinCore::FREE_IMAGE_MEMENT(Bit8u cls1, Bit8u cls2, Bit8u cls4)
{
	//^3E74:5992
	ENTER(0);
	//^3E74:5997
	Bit16u si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, dtImage, cls4);
	//^3E74:59AE
	if (si == 0xffff || glbShelfMemoryTable[si].Absent()) {
		//^3E74:59CE
		si = QUERY_GDAT_ENTRY_DATA_INDEX(0x15,0xfe,dtImage,0xfe);
	}
	//^3E74:59E1
	if (si == 0xffff)
		//^3E74:59E4
		return;
	//^3E74:59E6
	if (_4976_4807 == si) {
		//^3E74:59EC
		_4976_4807 = 0xffff;
	}
	//^3E74:59F2
	Bit16u di = QUERY_MEMENTI_FROM(si);
	//^3E74:59FA
	if (di != 0xffff) {
		//^3E74:59FF
		RECYCLE_MEMENTI(di, 0);
	}
	//^3E74:5A08
	return;
}

//^0B36:01CD
void SkWinCore::FREE_PICT_MEMENT(Picture *ref)
{
	//^0B36:01CD
	ENTER(0);
	//^0B36:01D0
	if ((ref->w4 & 0x0004) != 0) {
		//^0B36:01DB
		FREE_IMAGE_MEMENT(ref->b8, ref->b9, ref->b11);
	}
	//^0B36:01F4
	else if ((ref->w4 & 0x0008) != 0) {
		//^0B36:01FF
		_3e74_585a(ref->w12, 0);
	}

}

//^44C8:2307
// TODO: related gfx ?
void SkWinCore::_44c8_2307(X16 xx, X16 yy, X16 zz, X16 ww)
{
	//^44C8:2307
	ENTER(4);
	//^44C8:230D
	U8 *bp04 = _4976_5e64 + xx;
	U16 di = zz >> 1;
	U16 cx = ww + yy;
	U16 si = yy;
	do {
		//^44C8:2331
		_4976_5e6a[si] = bp04[di >> 7];
		di += zz;
		si++;
	} while (si < cx);
	//^44C8:234D
	return;
}


//^44C8:2351
void SkWinCore::_44c8_2351(Bit8u *xx, Bit8u *yy, Bit16u ss, Bit16u tt, Bit16u uu, Bit16u vv)
{
	//^44C8:2351
	ENTER(10);
	//^44C8:2357
	U16 di = uu;
	_4976_5e64 = xx;
	_4976_5e6a = yy;
	U16 bp04 = (i32(ss) << 7) / di;
	U16 bp06 = (i32(tt) << 7) / vv;
	U16 bp02 = bp06 >> 1;
	U16 si = 0;
	X16 bp08 = 0; // defaulting to 0
	X16 bp0a = 0; // defaulting to 0
	for (; si < vv; bp0a = bp08, bp02 += bp06, si++) {
		//^44C8:23B2
		bp08 = bp02 >> 7;
		if (bp0a == bp08 && si > 0) {
			//^44C8:23C7
			_4976_5e64 = yy;
			_44c8_0b8d((si -1) * di, si * di, di);
			_4976_5e64 = xx;
		}
		else {
			//^44C8:23F6
			_44c8_2307(bp08 * ss, si * di, bp04, di);
		}
		//^44C8:240D
	}
	//^44C8:241F
	return;
}

//^44C8:000F
void SkWinCore::FIRE_BLIT_TO_MEMORY_ROW_4TO4BPP(Bit16u offSrc, Bit16u offDst, Bit16u width)
{
	//^44C8:000F
	ENTER(0);
	//^44C8:0015
	Bit8u *di = _4976_5e6a;
	Bit8u *si = _4976_5e64;
	Bit16u cx = width;

	bool carry1 = (offSrc & 1) ? true : false;
	offSrc >>= 1;
	bool carry2;
	if (!carry1) {
		//^44C8:0028
		si += offSrc;
		//^44C8:002D
		carry2 = (offDst & 1) ? true : false;
		offDst >>= 1;
		if (!carry2) {
			//^44C8:0034
			di += offDst;

			//^44C8:0036
_0036:
			bool carry3 = (cx & 1) ? true : false;
			cx >>= 1;
			{
				//^44C8:0039
				bool carry4 = (cx & 1) ? true : false;
				cx >>= 1;
				//^44C8:003B
				if (carry4) {
					//^44C8:003D
					*di = *si; di++; si++;
				}
				//^44C8:003E
				while (cx != 0) {
					//^44C8:0043
					*di = *si; di++; si++;
					*di = *si; di++; si++;
					cx--;
				}
			}
			//^44C8:0049
			if (carry3) {
				*di = (*si & 0xf0) | (*di & 0x0f); si++; di++;
			}
		}
		else {
			//^44C8:00B5
			di += offSrc;
			//^44C8:00B7
			Bit8u bh = *di;
			//^44C8:00BA
			cx++;
			bool carry = (cx & 1) ? true : false;
			cx >>= 1;
			//^44C8:00BE
			if (cx != 0) {
				//^44C8:00C0
				bool carry = (cx & 1) ? true : false;
				//^44C8:00C2
				if (carry) {
					//^44C8:00C4
					Bit8u al = *si; si++;
					//^44C8:00C6
					*di = (bh & 0xf0) | (al >> 4); di++;
					bh = al << 4;
				}
				//^44C8:00D6
				if (cx != 0) {
					//^44C8:00D8
					bh &= 0xf0;
					do {
						// AL   AH   BL   BH
						// m34  m56       m12
						//                m10
						// m63  m45
						//           m63
						// m10            m63
						//           m60  m03
						// m13
						//                m60
						// ------------------
						// m13  m45       m60

						//^44C8:00DB
						Bit8u al = *si; si++;
						Bit8u ah = *si; si++;
						*di =  bh       | (al >> 4); di++;
						*di = (al << 4) | (ah >> 4); di++;
						bh = ah << 4;
						//^44C8:00EC
					} while (--cx != 0);
				}
			}
			//^44C8:00EE
			if (carry) {
				//^44C8:00F1
				*di = (*di & 0x0f) | bh; di++;
			}
		}
	}
	else {
		//^44C8:0068
        si += offSrc;
		{
			//^44C8:006A
			bool carry = (offDst & 1) ? true : false;
			offDst >>= 1;
			//^44C8:006F
			if (carry) {
				//^44C8:0059
				di += offDst;
				//^44C8:005B
				*di = (*si & 0x0f) | (*di & 0xf0); si++; di++;
				//^44C8:0065
				cx--;
				//^44C8:0066
				goto _0036;
			}
		}
		//^44C8:0071
		di += offDst;
		//^44C8:0073
		Bit8u bh = *si; si++;
		{
			//^44C8:0076
			bool carry = (cx & 1) ? true : false;
			cx >>= 1;
			//^44C8:0079
			if (cx != 0) {
				//^44C8:007B
				bool carry = (cx & 1) ? true : false;
				cx >>= 1;
				//^44C8:007D
				if (carry) {
					//^44C8:007F
					Bit8u bl = *si;
					*di = (*si >> 4) | (bh << 4); di++; si++;
					bh = bl;
				}
				//^44C8:008C
				if (cx != 0) {
					//^44C8:008E
					bh &= 0x0f;
					do {
						// AL   AH   BL   BH
						// m23  m45  ?    m01
						//           m45
						//      m01       m45
						//           m05  m40
						//      m41
						//                m05
						// m12  m34
						// ------------------
						// m12  m34       m05

						//^44C8:0091
						Bit8u al = *si; si++;
						Bit8u ah = *si; si++;
						*di = (bh << 4) | (al >> 4); di++;
						*di = (al << 4) | (ah >> 4); di++;
						bh = ah & 0x0f;
						//^44C8:00A2
					} while (--cx != 0);
				}
			}
			//^44C8:00A4
			if (carry) {
				//^44C8:00A7
				*di = (*di & 0x0f) | (bh << 4);
			}
		}
	}
	//^44C8:00F9
	return;
}

//^44C8:20E5
void SkWinCore::_44c8_20e5(Bit16u srcOff, Bit16u dstOff, Bit16u srcWidth, Bit16u dstWidth)
{
	ATLASSERT(srcWidth != 0);

	//^44C8:20E5
	ENTER(0);
	//^44C8:20EA
    Bit8u *di = _4976_5e6a + (dstOff >> 1);
	Bit16u cx = srcWidth >> 1;
	Bit16u dx = (dstWidth +1) >> 1;
	Bit8u *si = _4976_5e64 + (srcOff >> 1);
	Bit8u bh = 0;
	do {
		//^44C8:2111
		Bit8u ah = si[cx >> 8];
		if ((cx & 0x80) == 0) {
			ah &= 0xf0;
		}
		else {
			ah <<= 4;
		}
		cx += srcWidth;

		Bit8u al = si[cx >> 8];
		if ((cx & 0x80) == 0) {
			al >>= 4;
		}
		else {
			al &= 0x0f;
		}
		*di = al | ah; di++;
		cx += srcWidth;
	} while (--dx != 0);
	//^44C8:213E
	return;
}


//^44C8:2143
void SkWinCore::_44c8_2143(Bit16u xx, Bit16u yy, Bit16u ss, Bit16u tt)
{
	//^44C8:2143
	ENTER(4);
	//^44C8:2149
	U16 si = ss >> 1;
	U16 bp04 = ((tt +yy) +1) >> 1;
	U16 di = yy >> 1;
	//^44C8:2163
	while (di < bp04) {
		//^44C8:2166
		U16 bp02 = (si >> 7) +xx;
		U8 al;
		U8 dl = al = _4976_5e64[bp02 >> 1];
		si += ss;
		if ((bp02 & 1) != 0) {
			//^44C8:2188
			al &= 0xF;
		}
		else {
			//^44C8:218F
			al >>= 4;
		}
		//^44C8:2196
		al = _4976_5dbe[al];
		al <<= 4;
		U8 cl = al;
		bp02 = (si >> 7) +xx;
		al = _4976_5e64[bp02 >> 1];
		dl = al;
		si += ss;
		if ((bp02 & 1) != 0) {
			//^44C8:21C6
			al &= 0xF;
		}
		else {
			//^44C8:21CD
			al >>= 4;
		}
		//^44C8:21D4
		al = cl | _4976_5dbe[al];
		_4976_5e6a[di] = al;
		di++;
		//^44C8:21E7
	}
	//^44C8:21EF
	return;
}

#pragma warning(push)
#pragma warning(disable: 4700)

//^44C8:21F3
void SkWinCore::FIRE_STRETCH_BLIT_TO_MEMORY_4TO4BPP(Bit8u *src, Bit8u *dst, Bit16u srcWidth, Bit16u srcHeight, Bit16u dstWidth, Bit16u dstHeight, U8 *aa)
{
	//^44C8:21F3
	ENTER(12);
	//^44C8:21F9
	_4976_5e64 = src;
	//^44C8:2206
	_4976_5e6a = dst;
	//^44C8:2213
	_4976_5dbe = aa;
	//^44C8:2220
	Bit16u bp08 = (dstWidth +1) & 0xfffe;
	//^44C8:222D
	Bit16u bp0a = (srcWidth +1) & 0xfffe;
	//^44C8:2234
	Bit16u bp04 = (i32(i16(srcWidth)) << 7) / i16(dstWidth);
	//^44C8:224F
	Bit16u bp06 = (i32(i16(srcHeight)) << 7) / i16(dstHeight);
	//^44C8:226A
	Bit16u bp02 = bp06 >> 1;
	//^44C8:226F
	Bit16u bp0c = 0; // defaulting to 0
	for (Bit16u si = 0; si < dstHeight; si++) {
		//^44C8:2274
		Bit16u di = bp02 >> 7;
		//^44C8:227C
		if (bp0c == di && si > 0) {
			//^44C8:2285
			_4976_5e64 = dst;
			//^44C8:2292
			FIRE_BLIT_TO_MEMORY_ROW_4TO4BPP((si -1) * bp08, si * bp08, dstWidth);
			//^44C8:22A9
			_4976_5e64 = src;
		}
		//^44C8:22B8
		else if (aa == 0) {
			//^44C8:22C0
			_44c8_20e5(di * bp0a, si * bp08, bp04, dstWidth);
		}
		else {
			//^44C8:22D8
			_44c8_2143(di * bp0a, si * bp08, bp04, dstWidth);
		}
		//^44C8:22F1
		bp0c = di;
		//^44C8:22F4
		bp02 += bp06;
		//^44C8:22FA
	}
	//^44C8:2303
	return;
}
#pragma warning(pop)

//^0B36:06D3
ExtendedPicture *SkWinCore::QUERY_PICST_IT(ExtendedPicture *ref)
{
	// query image with some capacity
	// - stretch

	//^0B36:06D3
	ENTER(364);
	//^0B36:06D9
	Bit16u bp12 = (ref->w52 != 64 || ref->w54 != 64) ? 1 : 0;
	Bit16u bp14 = 0;
	//^0B36:06F9
	Bit16u bp16 = (ref->w6 == 0xffff && ref->w12 >= 0) ? 1 : 0;
	//^0B36:0714
	if (bp12 != 0) {
		//^0B36:071A
		ref->w28 = CALC_STRETCHED_SIZE(ref->w28, ref->w52);
		//^0B36:0732
		ref->w30 = CALC_STRETCHED_SIZE(ref->w30, ref->w54);
	}
	//^0B36:0747
	if ((ref->mirrorFlip & 0x0001) != 0) {
		//^0B36:0752
		ref->w28 = - ref->w28;
	}
	//^0B36:075C
	if (ref->colorKeyPassThrough == -3)
		//^0B36:0766
		//^0B36:0A35
		return ref;
	//^0B36:0769
	if (bp12 == 0 && bp14 == 0) {
		//^0B36:0775
		if (bp16 != 0) {
			//^0B36:077B
			//^0B36:0788
			_0b36_00c3(ref->w12, ref);
			//^0B36:0A35
			return ref;
		}
		else {
			//^0B36:0792
			QUERY_PICST_IMAGE(ref);
			//^0B36:079E
			//^0B36:0A35
			return ref;
		}
	}
	//^0B36:07A1
	Bit16u bp18;
	Bit8u *bp04;
	Bit8u *bp08;
	Picture bp32;
	if (bp16 != 0) {
		//^0B36:07A7
		bp18 = ALLOC_TEMP_CACHE_INDEX();
		//^0B36:07AF
		COPY_MEMORY(ref, &bp32, 24);
		//^0B36:07C6
		Bit8u *bp08 = _0b36_00c3(bp32.w12, &bp32);
		//^0B36:07DB
		ALLOC_PICT_MEMENT(&bp32);
		//^0B36:07E6
		bp04 = NULL;
	}
	else {
		//^0B36:07F3
		Bit32u bp10 = CALC_PICT_ENT_HASH(ref);
		//^0B36:0805
		if (ADD_CACHE_HASH(bp10, &bp18) != 0) {
			//^0B36:081C
			//^0B36:0788
			_0b36_00c3(bp18, ref);
			//^0B36:0A35
			return ref;
		}
		else {
			//^0B36:0828
			bp04 = NULL;
			bp08 = NULL;
			//^0B36:0838
			if (bp08 == NULL) {
				//^0B36:0840
				COPY_MEMORY(ref, &bp32, 24);
				//^0B36:0857
				bp08 = QUERY_PICST_IMAGE(&bp32);
				//^0B36:0868
				ALLOC_PICT_MEMENT(&bp32);
			}
		}
	}
	//^0B36:0873
	if ((bp32.w4 & 0x0010) != 0) {
		//^0B36:087D
		ExtendedPicture bp016c;
		COPY_MEMORY(ref, &bp016c, 314);
		//^0B36:0896
		bp016c.mirrorFlip = 0;
		bp016c.w54 = 64;
		bp016c.w52 = 64;
		//^0B36:08A7
		Bit16u bp1a;
		if (ADD_CACHE_HASH(CALC_PICT_ENT_HASH(&bp016c), &bp1a) == 0) {
			//^0B36:08C6
			bp08 = ALLOC_NEW_PICT(bp1a, bp32.width, bp32.height, bp32.w22);
			//^0B36:08E0
			Bit8u *bp0c = QUERY_PICT_BITS(&bp32);
			//^0B36:08F1
			FIRE_BLIT_PICTURE(
				bp0c,
				bp08,
				ALLOC_TEMP_ORIGIN_RECT(bp32.width, bp32.height),
				bp32.w14,
				bp32.w16,
				READ_UI16(bp0c,-4),
				READ_UI16(bp08,-4),
                -1,
				0,
				bp32.w22,
				bp32.w22,
				NULL
				);
		}
		//^0B36:0936
		FREE_PICT_MEMENT(&bp32);
		//^0B36:0941
		Bit8u *bp08 = _0b36_00c3(bp1a, &bp32);
		//^0B36:0956
		ALLOC_PICT_MEMENT(&bp32);
	}
	//^0B36:0961
	if (bp12 != 0) {
		//^0B36:096A
		i16 di = CALC_STRETCHED_SIZE(bp32.width, ref->w52);
		//^0B36:097C
		i16 si = CALC_STRETCHED_SIZE(bp32.height, ref->w54);
		//^0B36:098E
		if (di <= 0 || si <= 0) {
			//^0B36:0996
			ref->width = di;
			ref->height = si;
		}
		else {
			//^0B36:09A3
			ALLOC_NEW_PICT(bp18, di, si, bp32.w22);
			//^0B36:09B3
			bp08 = QUERY_PICT_BITS(&bp32);
			//^0B36:09C4
			bp04 = _0b36_00c3(bp18, ref);
			//^0B36:09DA
			if (bp32.w22 == 8) {
				//^0B36:09E0
				_44c8_2351(bp08, bp04, bp32.width, bp32.height, di, si);
			}
			else {
				//^0B36:09FE
				FIRE_STRETCH_BLIT_TO_MEMORY_4TO4BPP(bp08, bp04, bp32.width, bp32.height, di, si, NULL);
			}
		}
	}
	//^0B36:0A1E
	FREE_PICT_MEMENT(&bp32);
	//^0B36:0A29
	_3e74_585a(bp18, 0);
	//^0B36:0A35
	return ref;
}


//^0B36:0334
Bit8u *SkWinCore::TRANSLATE_PALETTE(Bit8u *localpal, Bit8u cls1, Bit8u cls2, Bit8u cls4, i16 palentcnt)
{
	//^0B36:0334
	ENTER(4);
	//^0B36:0339
	Bit8u *bp04 = QUERY_GDAT_ENTRY_DATA_PTR(cls1, cls2, dt07, cls4);
	//^0B36:0355
	for (i16 si = 0; si < palentcnt; si++) {
		//^0B36:0359
		localpal[si] = bp04[localpal[si]];
		//^0B36:036F
	}
	//^0B36:0375
	return localpal;
}

