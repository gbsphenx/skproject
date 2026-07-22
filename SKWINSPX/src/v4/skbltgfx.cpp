//------------------------------------------------------------------------------
// SK-BLIT-GFX
//------------------------------------------------------------------------------

#include <skver.h>

#include <skcore.h>
#include <skdebug.h> // for Unr();
#include <skglobal.h>
#include <skcnsole.h>
#include <skparam.h>

#include <string.h>

//#define UseAltic 0 // SPX: not Altic here

//^0759:016C
void SkWinCore::ANIM_FILL_SEQ_4BPP(U16 offDst, i16 clr, U16 cnt)
{
	ATLASSERT(cnt != 0);

	ENTER(0);
	U8 *esdi = _089c_0354;
	U16 bx = offDst;
	U16 cx = cnt;
	U8 ah = U8(clr);
	bool carry = (bx & 1) != 0;
	bx >>= 1;
	if (!carry) {
		esdi += bx;
	}
	else {
		esdi += bx;
		U8 al = (*esdi & 0xf0) | ah;
		stosb(esdi, al);
		if (--cx == 0) return;
	}
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
		al = *esdi;
		ax &= 0xf00f;
		al |= U8(ax >> 8);
		stosb(esdi, al);
	}
	return;
}
//^0759:013B
void SkWinCore::ANIM_SETPIXEL_SEQ_4BPP(U16 offDst, i8 clr)
{
	ENTER(0);
	U8 *esdi = _089c_0354;
	U16 bx = offDst;
	bool carry = (bx & 1) != 0;
	bx >>= 1;
	if (carry) {
		esdi += bx;
		U8 al = *esdi;
		al &= 0xf0;
		al |= clr;
		stosb(esdi, al);
	}
	else {
		esdi += bx;
		U8 al = *esdi;
		al &= 0x0f;
		al |= clr << 4;
		stosb(esdi, al);
	}
	return;
}
//^0759:01BE
void SkWinCore::ANIM_BLIT_TO_MEMORY_ROW_4TO4BPP(U16 offSrc, U16 offDst, U16 width)
{
#if UseAltic
	glbBlit2MemDest = _089c_0354;
	_4976_5e64 = _089c_0348;

	FIRE_BLIT_TO_MEMORY_ROW_4TO4BPP(offSrc, offDst, width);
#else
	Unr();
#endif
}

//^00EB:0008
void SkWinCore::IBMIO_LOAD_4TO8BPP_PAL(const U8 *pal)
{
	ENTER(0);
	U8 *esdi = _00eb_0023;
	const U8 *dssi = pal;
	U16 cx = 16;
	for (; cx != 0; cx--) movsb(esdi, dssi);
	return;
}


//^00EB:070C
// SPX: _00eb_070c renamed IBMIO_BLIT_SOURCE_4TO8BPP
void SkWinCore::IBMIO_BLIT_SOURCE_4TO8BPP(U8 *buffSrc, U16 offSrc, U16 offDst, U16 size) //#DS=04BF
{
	ENTER(0);
	LOADDS(0x0c48);
	glbBlitBufferSource = buffSrc;
	IBMIO_LOAD_4TO8BPP_PAL(_04bf_00dc);
	IBMIO_BLIT_ROW_4TO8BPP(offSrc, offDst, size);
	return;
}



//^00EB:0037
void SkWinCore::IBMIO_BLIT_ROW_4TO8BPP(U16 offSrc, U16 offDst, U16 size)
{
	ENTER(0);
	U8 *dsbx = _00eb_0023;
	U8 *esdi = _04bf_0e34 + offDst;
	U16 cx = size;
	const U8 *dssi = glbBlitBufferSource;
	bool carry1 = (offSrc & 1) != 0;
	offSrc >>= 1;
	dssi += offSrc;
	if (carry1) {
		U8 al = lodsb(dssi);
		al &= 0x0f;
		al = dsbx[al];
		if (--cx == 0) return;
	}
	bool carry2 = (cx & 1) != 0;
	cx >>= 1;
	if (cx != 0) {
		bool carry3 = (cx & 1);
		cx >>= 1;
		if (cx != 0) {
			U16 bp = 0x0f0f;
			do {
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
			} while (--cx != 0);
		}
		if (carry3) {
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
	if (carry2) {
		U8 al = lodsb(dssi);
		al >>= 4;
		al = dsbx[al];
		stosb(esdi, al);
	}
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
	LOGX(("QUERY_TEMP_PICST(MF:%X,%%X%2d %%Y%2d,OX:%3d-OY:%3d,%04X,R=%04X,%04X,CK:%2d+%2d,CLS:%02X-%02X-%02X)"
		, (Bitu)mirrorflip, (Bitu)horzStretch, (Bitu)vertStretch, (Bitu)offx, (Bitu)offy
		, (Bitu)uu, (Bitu)rectno, (Bitu)pp, (Bitu)colorkey1, (Bitu)colorkey2, (Bitu)cls1, (Bitu)cls2, (Bitu)cls4));

	ENTER(2);
	i16 di = uu;
	QUERY_GDAT_SUMMARY_IMAGE(&glbTempPicture, cls1, cls2, cls4);
	if (horzStretch != vertStretch) {
		U8 bp01;
		if (di == 2) {
			bp01 = 0x14;
			goto _08fe;
		}
		else if (di == 3) {
			bp01 = 0x15;
_08fe:
			U16 si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, dtWordValue, bp01);
			if (si != 0) {
				horzStretch = ((((si >> 8) << 7) / (si & 255)) +1) >> 1;
			}
		}
	}
	glbTempPicture.mirrorFlip = mirrorflip;
	glbTempPicture.iXStretch = horzStretch;
	glbTempPicture.iYStretch = vertStretch;
	glbTempPicture.iXOffset += offx;
	glbTempPicture.iYOffset += offy;
	glbTempPicture.rectNo = rectno;
	glbTempPicture.w26 = pp;
	glbTempPicture.pb44 = glbBackBuffViewport;
	glbTempPicture.colorKeyPassThrough = colorkey1;
	PALETTE_SOMETHING_32cb_0804(glbTempPicture.iPal256, di, colorkey1, colorkey2, glbTempPicture.iPaletteSize);
	QUERY_PICST_IT(&glbTempPicture);
	return;
}

//^32CB:0997
void SkWinCore::DRAW_TEMP_PICST()
{
//	ENTER(0);
	DRAW_PICST(&glbTempPicture);
	return;
}


//^0B36:037E
// TODO: gfx related
// SPX: _0b36_037e renamed PALETTE_SOMETHING_0b36_037e
U8 *SkWinCore::PALETTE_SOMETHING_0b36_037e(U8 *localpal, i8 xx, i16 colorkey1, i16 colorkey2, i16 palentcnt)
{
	ENTER(8);
	if (localpal != NULL && xx != 0) {
		xx = U8(max_value(0, 64 -xx));
		for (i16 bp02 = 0; bp02 < palentcnt; bp02++) {
			if (bp02 != colorkey1 && bp02 != colorkey2) {
				U16 di = tblOfsk4be2Palette[RCX(256,localpal[bp02])].b0;
				i16 si = 0;
				i16 bp04 = max_value((tblOfsk4bdePalette[RCX(16,di)].pTabRow1[tblOfsk4be2Palette[localpal[bp02]].b1] * xx) >> 6, 0);
				for (; tblOfsk4bdePalette[di].iRowSize -1 > si; si++) {
					if (true
						&& tblOfsk4bdePalette[di].pTabRow1[si] <= bp04
						&& tblOfsk4bdePalette[di].pTabRow1[si +1] >= bp04
					) {
						i16 bp06 = bp04 - tblOfsk4bdePalette[di].pTabRow1[si];
						i16 bp08 = tblOfsk4bdePalette[di].pTabRow1[si +1] -bp04;
						if (bp06 > bp08)
							si++;
						break;
					}
				}
				si = min_value(si, tblOfsk4bdePalette[di].iRowSize -1);

				localpal[bp02] = tblOfsk4bdePalette[di].pTabRow2[si];
			}
		}
	}
	return localpal;
}

//^32CB:0804
// SPX: _32cb_0804 renamed PALETTE_SOMETHING_32cb_0804
void SkWinCore::PALETTE_SOMETHING_32cb_0804(U8 *localpal, i16 cls4, U16 colorkey1, i16 colorkey2, U16 palentcnt)
{
	ENTER(0);
	U16 di = palentcnt;
	i16 si;
	if (cd.pi.glbIsPlayerMoving != 0) {
		if (cls4 < 0) {
			si = max_value(-_4976_5a88, _4976_421b[RCJ(6,-cls4)]);
			cls4 = 1;
		}
		else {
			si = _4976_4221[RCJ(5,cls4)];
			cls4 += 9;
		}
	}
	else {
		si = _4976_4226[RCJ(5,cls4)];
	}
	//^32CB:0854	// SPX: That part is for translating palette in case of 'fog'
	if (!SkCodeParam::bDisableFogEffect && QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_x08_GRAPHICSSET, glbMapGraphicsSet, dt07, U8(cls4)) != 0) {
		TRANSLATE_PALETTE(localpal, GDAT_CATEGORY_x08_GRAPHICSSET, glbMapGraphicsSet, U8(cls4), di);
		PALETTE_SOMETHING_0b36_037e(localpal, i8(_4976_5a88), colorkey1, colorkey2, di);
	}
	else {
		PALETTE_SOMETHING_0b36_037e(localpal, 64 -U8(((64 -si) * (64 - _4976_5a88)) >> 6), colorkey1, colorkey2, di);
	}
	return;
}


//^32CB:09A7
// SPX: _32cb_09a7 renamed QUERY_MULTILAYERS_PIC
U16 SkWinCore::QUERY_MULTILAYERS_PIC(ExtendedPicture *ref, U8 cls1, U8 cls2, U8 cls4, U16 horzRes, U16 vertRes, i16 zz, U16 mirrorflip, i16 colorkey1, U16 colorkey2)
{
	U16 iRCAlloc;	// si
	ENTER(0);
	QUERY_TEMP_PICST(mirrorflip, horzRes, vertRes, 0, 0, zz, -1, -1, colorkey1, -1, cls1, cls2, cls4);
	COPY_MEMORY(&glbTempPicture, ref, sizeof(ExtendedPicture));
	ALLOC_PICT_MEMENT(ref);
	ref->colorKeyPassThrough = colorkey2;
	SET_ORIGIN_RECT(&ref->rc36, ref->iWidth, ref->iHeight);
	ref->pb44 = ALLOC_NEW_PICT(iRCAlloc = ALLOC_TEMP_CACHE_INDEX(), ref->iWidth, ref->iHeight, 8);
	return iRCAlloc;
}


//^44C8:0762
void SkWinCore::FIRE_UPDATE_BLIT_PALETTE(const U8 *localpal)
{
	ENTER(0);
	U8* pPalette16 = glbBlitPalette16;	// di
	const U8* pLocalPalette = localpal;	// si
	memcpy(pPalette16, pLocalPalette, 16);	// copy localpal into blit palette
	return;
}

//^44C8:0791
void SkWinCore::FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(U16 offSrc, U16 offDst, U16 size)
{
	ENTER(0);
	U8 *bx = glbBlitPalette16;
	U8 *di = &glbBlit2MemDest[offDst];
	U16 cx = size;
	U8 *si = _4976_5e64;
	bool carry = (offSrc & 1) ? true : false;
	si += (offSrc >>= 1);
	if (carry) {
		U8 al = lodsb(si);
		stosb(di, bx[al & 15]);
		cx--;
		if (cx == 0) goto _080a;
	}

	{
		bool carry = (cx & 1) ? true : false;
		cx >>= 1;
		if (cx != 0) {
			bool carry = (cx & 1) ? true : false;
			cx >>= 1;
			if (cx != 0) {
				U16 bp = 0x0f0f;
				do {
					U16 ax = lodsw(si);
					U16 dx = ax >> 4;
					dx &= bp;
					ax &= bp;
					stosb(di, bx[U8(ax >> 8)]);
					stosb(di, bx[U8(dx >> 8)]);
					stosb(di, bx[U8(ax &255)]);
					stosb(di, bx[U8(dx &255)]);
				} while (--cx != 0);
			}
			if (carry) {
				U8 al = lodsb(si);
				stosb(di, bx[al >> 4     ]);
				stosb(di, bx[al      &255]);
			}
		}
		if (carry) {
			U8 al = lodsb(si);
			stosb(di, bx[al >> 4]);
		}
	}
_080a:
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
	ENTER(16);
	FIRE_UPDATE_BLIT_PALETTE(localpal);
	hazeWidth = (hazeWidth +1) & 0xfffe;
	i16 bp08 = prc->cx;
	i16 bp06 = prc->cy;
	i16 bp02 = (prc->y * dstWidth) +prc->x;
	i16 bp04 = (bb * hazeWidth) +aa;
	U16 di = tt;
	U16 si = ss -tt;
	_4976_5e64 = src;
	glbBlit2MemDest = dst;
	if (zz == NULL) {
		for (U16 bp0a = 0; bp0a < bp06; bp02 += dstWidth, bp0a++) {
			i16 bp0e = bp08;
			U16 bp0c;
			for (bp0c = 0; bp0e >= si; bp0e -= si, bp0c += si, si = ss, di = 0) {
				if (colorkey < 0) {
					FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(di, bp02 +bp0c, si);
					continue;
				}
				FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP(di, bp02 +bp0c, si, U8(colorkey));
			}
			if (bp0e > 0) {
				if (colorkey < 0) {
					FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(di, bp02 +bp0c, bp0e);
				}
				else {
					FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP(di, bp02 +bp0c, bp0e, U8(colorkey));
				}
				si -= bp0e;
				di += bp0e;
			}
			if ((bp08 & 1) != 0) {
				if (--si != 0) {
					di++;
				}
				else {
					si = ss;
					di = 0;
				}
			}
		}
		return;
	}
	for (U16 bp0a = 0; bp0a < bp06; bp0a++) {
		for (U16 bp10 = bp08; bp10 > 0; ) {
			U16 bp0c;
			for (bp0c = 0; bp10 > 0 && _44c8_1e1a(zz, bp04 +bp0c) == 0; ) {
				--bp10;
				++bp0c;
				++di;
				--si;
				if (si == 0) {
					di = 0;
					si = ss;
				}
			}
			U16 bp0e;
			for (bp0e = 0; bp10 > 0 && _44c8_1e1a(zz, bp04 +bp0c) != 0; ) {
				++bp0e;
				--bp10;
			}
			if (bp0e > 0) {
				while (bp0e >= si) {
					if (colorkey < 0) {
						FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(di, bp02 +bp0c, si);
					}
					else {
						FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP(di, bp02 +bp0c, si, U8(colorkey));
					}
					bp0e -= si;
					bp0c += si;
					si = ss;
					di = 0;
				}
				if (bp0e > 0) {
					if (colorkey < 0) {
						FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(di, bp02 +bp0c, bp0e);
					}
					else {
						FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP(di, bp02 +bp0c, bp0e, U8(colorkey));
					}
					si -= bp0e;
					di += bp0e;
				}
			}
			if ((bp08 & 1) != 0) {
				if (--si != 0) {
					++di;
				}
				else {
					si = ss;
					di = 0;
				}
			}
			bp04 += hazeWidth;
			bp02 += dstWidth;
		}
	}
	return;
}




//^32CB:0C7D
// yy is index in palette
// SPX: _32cb_0c7d renamed DRAW_RAIN_32cb_0c7d
void SkWinCore::DRAW_RAIN_32cb_0c7d(ExtendedPicture *ref, U16 xx, U16 yy)
{
	ENTER(16);
	if (glbRainStrength == 0) {
		FILL_ENTIRE_PICT(reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(xx)), ref->iPal256[yy]);
		return;
	}
	U8 bp0b;
	U16 bp0a;
	QUERY_RAINFALL_PARAM(&bp0b, &bp0a);
	i16 bp06;
	i16 bp08;
	QUERY_GDAT_IMAGE_METRICS(0x17, glbMapGraphicsSet, bp0b, &bp06, &bp08);
	U16 di = ALLOC_TEMP_CACHE_INDEX();
	ALLOC_NEW_PICT(di, ref->rc36.cx, bp08, 4);
	U16 si = (ref->rc36.cx +1) & 0xfffe;
	U8 *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x17, glbMapGraphicsSet, bp0b);
	FIRE_BLIT_PICTURE(bp04, QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(di), ALLOC_TEMP_ORIGIN_RECT(si, READ_I16(bp04,-2)), 
		0, 0, READ_I16(bp04,-4), si, -1, (bp0a == 1) ? 1 : 0, 4, 4, NULL);
	bp04 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(di));
	U8 *bp10 = _32cb_0649(0x17, glbMapGraphicsSet, bp0b, 0);
	bp10[0] = ref->iPal256[yy];
	si = READ_I16(bp04,-4) * READ_I16(bp04,-2);
	_44c8_20a4(bp04, reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(xx)), NULL, &ref->rc36, 
		si -(RAND() & 31) -16, RAND16(60), ref->iWidth, -1, bp10);
	FREE_TEMP_CACHE_INDEX(di);
	return;
}


//^32CB:28C7
void SkWinCore::QUERY_CREATURE_PICST(U16 xx, i16 iDistToPlayer, Creature *xCreature, CreatureInfoData *xInfo, ObjectID rl)
{
	ENTER(28);
	U16 iVFlip = 0; // di
	static int counter = 0;
	U8 iCreatureType = xCreature->CreatureType();
	sk1c9a02c3* xInfoData = GET_CREATURE_INFO_DATA(xCreature, QUERY_CREATURE_AI_SPEC_FROM_TYPE(iCreatureType)); // bp04
	i16 iFrameID = CREATURE_SEQUENCE_4937_000f(xInfoData->iAnimSeq, &xInfoData->iAnimInfo); // si
	// SPX: there might be an issue here where animation frame is not retrieved correctly. A static object would have si = 4, so that its gets images x10 x12 x13 x12
	U16 bp06 = (xInfo == NULL) ? 0 : xInfo->b7;
	U16 iFaceDirImg = ((QUERY_CREATURE_AI_SPEC_FLAGS(rl) & 4) != 0) ? 2 : ((_4976_5aa0 - xCreature->b15_0_1()) & 3); // bp0a
	U16 iMirrorFlag = tblCreatureFrameInfo14[iFrameID][iFaceDirImg + 10];	// bp08 / _4976_5a98 table has 2+4+4+4 bytes: 4 * images ID in GDAT, 4 * scales, then 4 * mirror flags (starting at 10th pos as requested here)
	U8 iImageID = tblCreatureFrameInfo14[iFrameID][iFaceDirImg + 2]; // bp0c, standard front is x12

	if (SkCodeParam::bDM2V5Mode) {
		iMirrorFlag = 0;
		if (iFrameID == (i16)0xFFFF)
			iFrameID = 0;
		iImageID = GET_CREATURE_ANIMATION_IMAGE_ID_V5(iCreatureType, xInfoData->iAnimSeq, iFrameID, iFaceDirImg);
	}

	CHANGE_CONSOLE_COLOR(BRIGHT, (rl.DBIndex()%6+2)+8, BLACK);
	SkD((DLV_DBG_CANIM, "Creature Type %d => %04X F:%04X => Table Frame = %d (C: %02X %02X) => IMG = %02X\n", 
		iCreatureType, xInfoData->iAnimSeq, xInfoData->iAnimInfo, iFrameID, xCreature->iAnimSeq, xCreature->iAnimFrame, iImageID));
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);

	if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_x0F_CREATURES, iCreatureType, dtImage, iImageID) == 0) {
		iImageID = (iFaceDirImg +2) & 3;
		if ((iImageID & 1) != 0)
			iVFlip = 1;
		iImageID = tblCreatureFrameInfo14[iFrameID][iImageID + 2];
		if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_x0F_CREATURES, iCreatureType, dtImage, iImageID) == 0) {
			iVFlip = 0;
			iImageID = tblCreatureFrameInfo14[iFrameID][4];
		}
	}
	else if (false
		|| (iMirrorFlag & 1) != 0
		|| ((bp06 & 64) != 0 && (bp06 & 2) != 0 && (bp06 & 1) == 0)
	) {
		iVFlip = 1;
	}
	if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_x0F_CREATURES, iCreatureType, dtImage, iImageID) == 0) {
		iImageID = iFaceDirImg - 6; // Standard iFaceDirImg = 2; then -6 goes to 0xFA, to fetch the "shadow" image
		if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_x0F_CREATURES, iCreatureType, dtImage, iImageID) == 0) { // try to get 0xFC ?
			if (true
				&& iImageID == 0xFB
				&& QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_x0F_CREATURES, iCreatureType, dtImage, iImageID + 2) != 0
			) {
				iVFlip = 1;
				iImageID = iImageID + 2;
			}
			else {
				iVFlip = 0;
				iImageID = 0xFC;
			}
		}
	}
	i16 iImageScaleFromDist = tlbDistanceStretch[RCJ(5,iDistToPlayer)]; // bp10
	i16 iImageScale = iImageScaleFromDist;
	i16 iFrameID2 = (xInfo != NULL && xInfo->Command == ccm13_Destroy) ? xInfo->w14 : iFrameID;	// bp14
	U16 iOffsetPosition = tblCreatureFrameInfo14[iFrameID2][0]; // bp12	-- Standard is 0x0C = 12 = centered
	if (SkCodeParam::bDM2V5Mode)
		iOffsetPosition = CREATURE_GET_ANIMATION_OFFSET_POS_V5(iCreatureType, iFrameID2);
	if (xx == 3 && glbTryMoveObjectOrTable != 0) {
		iOffsetPosition = _4976_41d0[RCJ(7,glbTargetTypeMoveObject)];
		iImageScale = _4976_41d7[RCJ(7,glbTargetTypeMoveObject)];
		iFaceDirImg = 0;
	}
	else {
		i16 iLocalScale = tblCreatureFrameInfo14[iFrameID][iFaceDirImg + 6]; // V4
		if (SkCodeParam::bDM2V5Mode)
			iLocalScale = 0x40;
		iImageScale = CALC_STRETCHED_SIZE(iLocalScale, iImageScaleFromDist);	// table read from item 653 containing sequence/frame size/scale info
	}
	U16 bp0e = QUERY_CREATURE_BLIT_RECTI(xx, iOffsetPosition, iFaceDirImg) | 0x8000;
	i16 bp18 = 0;
	i16 bp16 = 0;
	bp18 = bp16 = i8(tblCreatureFrameInfo14[iFrameID2][1]);
	if (bp16 != 0) {
		i16 bp1c;
		i16 bp1a;
		switch (iFaceDirImg) {
		case 0:	// back
			bp1c = -7;
			bp1a = 0;
			break;

		case 2: // front
			bp1c = 7;
			bp1a = 0;
			break;

		case 1: // left
		case 3: // right
			bp1a = -64;
			bp1c = 0;

			break;
		}
		bp1a = CALC_STRETCHED_SIZE(bp1a, bp16);
		bp18 = CALC_STRETCHED_SIZE(bp1c, bp18);
	}

	bp18 += 16;
	QUERY_TEMP_PICST(iVFlip, iImageScale, iImageScale, _4976_41de[RCJ(8,bp06 & 7)] +bp16, _4976_41de[RCJ(8,(bp06 >> 3) & 7)] +bp18, iDistToPlayer,
		bp0e, -1, CREATURE_GET_COLORKEY(iCreatureType), -1, 0x0f, iCreatureType, iImageID);
	return;
}


//^32CB:01B6
U16 SkWinCore::_32cb_01b6(U16 xx, U16 yy, U16 ss, U16 tt, U16 *ww)
{
	ENTER(8);
	ObjectID rlCreature = GET_CREATURE_AT(ss, tt); // si
	if (rlCreature == OBJECT_NULL)
		return 0;

	Creature* xCreature = GET_ADDRESS_OF_RECORD4(rlCreature); // bp04
	CreatureInfoData* xCreatureInfoData; // bp08
	if (xCreature->iID == 0xFF) {
		xCreatureInfoData = NULL;
	}
	else {
		xCreatureInfoData = &glbTabCreaturesInfo[xCreature->iID];
	}
	//^32CB:0216
	QUERY_CREATURE_PICST(3, glbTabYAxisDistance[3], xCreature, xCreatureInfoData, rlCreature);
	i16 iColorPassThrough = glbTempPicture.colorKeyPassThrough; // di
	glbTempPicture.colorKeyPassThrough = -2;
	DRAW_TEMP_PICST();
	if (_32cb_00f1_DRAW_PICTURE(xx, yy, iColorPassThrough) == 0)
		return 0;
	if (_4976_5a94 < -75) {
		*ww = 3;
	}
	else if (_4976_5a94 > 75) {
		*ww = 1;
	}
	else {
		*ww = 0;
	}
	return 1;
}



//^3E74:0AE3
void SkWinCore::FREE_PICT_ENTRY(U8* xPictureBuffer)
{
	SkD((DLV_MEM, "FREE_PICT_ENTRY: S(sk5cfc_image) = %d || BUFF = %08X\n", sizeof(sk5cfc_image), xPictureBuffer));
	//LOG_HEXA((X8*)buff, 8);
	//LOG_HEXA((X8*)buff-int(sizeof(sk5cfc_image)), sizeof(sk5cfc_image));
	if (_4976_5d76 == 0) {
		sk5cfc_image *bp04 = reinterpret_cast<sk5cfc_image *>(&gblLinkedImageRoot);
		SkD((DLV_MEM, "FREE_PICT_ENTRY: bp04 = %08X / bp04->pNextImage = %08X / BUFF-S(sk5cfc) = %08X\n", bp04, bp04->pNextImage, PTR_PADD(xPictureBuffer,-int(sizeof(sk5cfc_image)))));
		LOGX(("FREE_PICT_ENTRY: bp04 = %p / bp04->pNextImage = %p / BUFF-S(sk5cfc) = %p", bp04, bp04->pNextImage, PTR_PADD(xPictureBuffer,-int(sizeof(sk5cfc_image)))));
		for (; (void *)PTR_PADD(xPictureBuffer,-int(sizeof(sk5cfc_image))) != (void *)bp04->pNextImage; ) {
			bp04 = bp04->pNextImage;
		}
		bp04->pNextImage = bp04->pNextImage->pNextImage;
		U32 bp08 = CALC_IMAGE_BYTE_LENGTH(xPictureBuffer) + 0x1E;	// bp08 what is this x1E exactly ???
		SkD((DLV_MEM, "FREE_PICT_ENTRY ImageLen + 0x1E = %08X => Size = %05d\n", xPictureBuffer, bp08));
		LOGX(("FREE_PICT_ENTRY ImageLen + 0x1E = %p => Size = %05d", xPictureBuffer, bp08));
		// SPX: some struct* adaptation
		//{
			//SkImage* xImage = reinterpret_cast<SkImage *>(PTR_PADD(xPictureBuffer,-int(sizeof(sk5cfc_image))));
			SkImageDealloc* xImage = reinterpret_cast<SkImageDealloc *>(PTR_PADD(xPictureBuffer,-int(sizeof(sk5cfc_image))));
			//LOG_HEXA((X8*)xImage, sizeof(SkImage*) + 4);
		//}
		LOGX(("FREE_PICT_ENTRY SkImage %p / pv0 %p", xImage, xImage->pNextImage));
//		if (reinterpret_cast<SkImage *>(PTR_PADD(buff,-int(sizeof(sk5cfc_image))))->AllocLower() == 0) {	// original 32-bits
		if (reinterpret_cast<SkImageDealloc *>(PTR_PADD(xPictureBuffer,-int(sizeof(sk5cfc_image))))->AllocLower() == 0) { // fix for 32/64 bits
			DEALLOC_UPPER_MEMORY(bp08);
		}
		else {
			DEALLOC_LOWER_MEMORY(bp08);
		}
	}
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
SRECT *SkWinCore::QUERY_BLIT_RECT(Bit8u *buff, SRECT *rect, U16 rectno, i16 *yourcx, i16 *yourcy, i16 ww) //#DS=4976?
{
	SkD((DLV_RCT, "RCT: -> QUERY_BLIT_RECT(%p,%p(     x,     x,     x,     x),%4d,%3d,%3d,%3d)\n"
		, buff, rect, (Bitu)(rectno & 0x7fff), (Bitu)*yourcx, (Bitu)*yourcy, (Bitu)ww));

	i16 di = 0;

	U16 bp24 = 0;
	SRECT spRect1;	// bp1a
	U16 bp12 = 0;
	U16 bp10 = 0;
	i16 bp0c = 0;
	i16 bp0a = 0;
	SRECT *spRect2 = NULL; // bp08
	SRECT *spRectMain = NULL;	// bp04

	if (rectno == 0xFFFF) {
		return NULL;
	}

	U16 si = 0;
	U16 iRectnoFlag = rectno & 0x8000;	// si
	if (iRectnoFlag != 0) {
		rectno &= 0x7FFF;
	}
	spRectMain = QUERY_RECT(glbRectNoTable.pNextTable, rectno);
	if (spRectMain == NULL) {
		return NULL;
	}
	SET_SRECT(&spRect1, -C10000_RECTZONE_SCALE_FULL, -C10000_RECTZONE_SCALE_FULL, C20000_RECTZONE_SCALE_DOUBLE, C20000_RECTZONE_SCALE_DOUBLE);
	SRECT bp22;
	U16 bp0e;
	if (ww == -1) {
		bp0e = spRectMain->x;
	}
	else {
		COPY_MEMORY(spRectMain, &bp22, sizeof(SRECT));
		bp22.x = bp0e = ww;
		spRectMain = &bp22;
	}
	if (bp0e <= 8) {
		bp0a = spRectMain->cx;
		bp0c = spRectMain->cy;
	}
	else {
		if (bp0e == 9) {
			return NULL;
		}
		bp0e -= 10;
		bp0a = 0;
		bp0c = 0;
	}
	if (iRectnoFlag != 0) {
		bp0a += *yourcx;
		bp0c += *yourcy;
		*yourcx = 0;
		*yourcy = 0;
	}
	if (buff == NULL && (*yourcx <= 0 || *yourcy <= 0)) {
		return NULL;
	}
	bp10 = 0;
	for (; spRectMain->y != 0; ) {
		if (spRectMain->x >= 10 && spRectMain->x <= 18) {
			spRect2 = QUERY_RECT(glbRectNoTable.pNextTable, spRectMain->y);
			if (spRect2 == NULL)
				break;
			si = spRect2->cx;
			di = spRect2->cy;
			bp12 = spRect2->x;
			bp24 = spRect2->y;
			spRect2 = QUERY_RECT(glbRectNoTable.pNextTable, spRect2->y);
			if (spRect2 == NULL)
				break;
			switch (bp12) {
				default:
					return NULL;

				case 0:
					di -= (spRect2->cy +1) >> 1;
					si -= (spRect2->cx +1) >> 1;
					break;

				case 5:
					si -= (spRect2->cx +1) >> 1;
					break;

				case 3:
					di -= spRect2->cy -1;
					si -= spRect2->cx -1;
					break;

				case 2:
					si -= spRect2->cx -1;
					break;

				case 6:
					si -= spRect2->cx -1;
					di -= (spRect2->cy +1) >> 1;
					break;

				case 8:
					di -= (spRect2->cy +1) >> 1;
					break;

				case 7:
					si -= (spRect2->cx +1) >> 1;
					di -= spRect2->cy -1;
					break;

				case 4:
					di -= spRect2->cy -1;
					break;

				case 1:
					break;
			}

			spRect1.x += si;
			if (spRect1.x < si) {
				spRect1.x = si;
			}
			if (spRect2->cx + si <= spRect1.x + spRect1.cx -1) {
				spRect1.cx = spRect2->cx - spRect1.x + si;
			}
			spRect1.y += di;
			if (spRect1.y < di) {
				spRect1.y = di;
			}
			if (spRect2->cy + di <= spRect1.y + spRect1.cy -1) {
				spRect1.cy = spRect2->cy - spRect1.y + di;
			}
			switch (spRectMain->x) {
				default:
					return NULL;

				case 10:
					di += (spRect2->cy +1) >> 1;
					si += (spRect2->cx +1) >> 1;
					break;

				case 15:
					si += (spRect2->cx +1) >> 1;
					break;

				case 13:
					di += spRect2->cy -1;
					si += spRect2->cx -1;
					break;

				case 12:
					si += spRect2->cx -1;
					break;

				case 16:
					si += spRect2->cx -1;
					di += (spRect2->cy +1) >> 1;
					break;

				case 18:
					di += (spRect2->cy +1) >> 1;
					break;

				case 17:
					si += (spRect2->cx +1) >> 1;
					di += spRect2->cy -1;

					break;

				case 14:
					di += spRect2->cy -1;

					break;

				case 11:
					break;
			}
			bp0a += si + spRectMain->cx;
			bp0c += di + spRectMain->cy;
		}
		else {
			spRect2 = QUERY_RECT(glbRectNoTable.pNextTable, spRectMain->y);
			if (spRect2 == NULL)
				break;
			bp24 = spRectMain->y;
			si = spRect2->cx;
			di = spRect2->cy;
			if (spRect2->x == 1) {
				bp0a += si;
				bp0c += di;
				spRect1.x += si;
				spRect1.y += di;
			}
			else if (spRect2->x == 9) {
				switch (spRectMain->x) {
					case 0:
						si = spRectMain->cx - ((si +1) >> 1);
						di = spRectMain->cy - ((di +1) >> 1);
						break;

					case 1:
						si = spRectMain->cx;
						di = spRectMain->cy;
						break;

					case 2:
						si = spRectMain->cx - (si -1);
						di = spRectMain->cy;
						break;

					case 3:
						si = spRectMain->cx - (si -1);
						di = spRectMain->cy - (di -1);
						break;

					case 4:
						si = spRectMain->cx;
						di = spRectMain->cy - (di -1);
						break;

					case 5:
						si = spRectMain->cx - ((si +1) >> 1);
						di = spRectMain->cy;
						break;

					case 6:
						si = spRectMain->cx - (si -1);
						break;

					case 7:
						si = spRectMain->cx - ((si +1) >> 1);
						di = spRectMain->cy - (di -1);
						break;

					case 8:
						si = spRectMain->cx;
						di = spRectMain->cy - ((di +1) >> 1);
						break;
				}
				if (bp10 != 0) {
					bp10 = 0;
					bp0a += si;
					bp0c += di;
					spRect1.x += si;
					spRect1.y += di;
				}
				if (spRect1.x < si) {
					spRect1.x = si;
				}
				if (spRect2->cx + si <= spRect1.x + spRect1.cx -1) {
					spRect1.cx = spRect2->cx - spRect1.x + si;
				}
				if (spRect1.y < di) {
					spRect1.y = di;
				}
				if (spRect2->cy + di <= spRect1.y + spRect1.cy -1) {
					spRect1.cy = spRect2->cy - spRect1.y + di;
				}
			}
			else {
				if (spRect2->x <= 8) {
					bp10 = 1;
				}
			}
		}
		spRectMain = spRect2;
	}
	si = *yourcx;
	if (si == 0) {
		si = READ_IMGBUFF_WIDTH(buff);	// READ_UI16(buff,-4);
	}
	di = *yourcy;
	if (di == 0) {
		di = READ_IMGBUFF_HEIGHT(buff);	// READ_UI16(buff,-2);
	}
	switch (bp0e) {
		default:
			return NULL;

		case 0:
			rect->x = bp0a -((si +1) >> 1);
			rect->y = bp0c -((di +1) >> 1);
			break;

		case 1:
			rect->x = bp0a;
			rect->y = bp0c;
			break;

		case 2:
			rect->x = bp0a - (si -1);
			rect->y = bp0c;
			break;

		case 3:
			rect->x = bp0a - (si -1);
			rect->y = bp0c - (di -1);
			break;

		case 4:
			rect->x = bp0a;
			rect->y = bp0c - (di -1);
			break;

		case 5:
			rect->x = bp0a - ((si +1) >> 1);
			rect->y = bp0c;
			break;

		case 6:
			rect->x = bp0a - (si -1);
			rect->y = bp0c - ((di +1) >> 1);
			break;

		case 7:
			rect->x = bp0a - ((si +1) >> 1);
			rect->y = bp0c - (di -1);
			break;

		case 8:
			rect->x = bp0a;
			rect->y = bp0c - ((di +1) >> 1);
			break;
	}
	if (glb_4976_4bca_Rect.cx != 0) {
		COPY_MEMORY(&glb_4976_4bca_Rect, &spRect1, sizeof(SRECT));
	}
	if (_4976_4bc8 != 0 && cd.pi.glbIsPlayerMoving != 0 && bp24 == 3) {
		if (UNION_RECT(&spRect1, &_4976_00fe, yourcx, yourcy) == NULL) {
			return NULL;
		}
	}
	bp0a = spRect1.x - rect->x;
	if (bp0a <= 0) {
		*yourcx = 0;
		rect->cx = min_value(si, spRect1.cx + bp0a);
	}
	else {
		*yourcx = bp0a;
		rect->x = spRect1.x;
		rect->cx = min_value(si - bp0a, spRect1.cx);
	}
	bp0c = spRect1.y - rect->y;
	if (bp0c <= 0) {
		*yourcy = 0;
		rect->cy = min_value(di, spRect1.cy + bp0c);
	}
	else {
		*yourcy = bp0c;
		rect->y = spRect1.y;
		rect->cy = min_value(di - bp0c, spRect1.cy);
	}
	if (rect->cx <= 0 || rect->cy <= 0)
		return NULL;

	SkD((DLV_RCT, "RCT: <- QUERY_BLIT_RECT(%p,%p(%6d,%6d,%6d,%6d),%4d,%3d,%3d,%3d)\n"
		, buff, rect, (Bitu)rect->x, (Bitu)rect->y, (Bitu)rect->cx, (Bitu)rect->cy
		, (Bitu)rectno, (Bitu)*yourcx, (Bitu)*yourcy, (Bitu)ww));

	return rect;
}


//^01B0:0008
void SkWinCore::MOVE_MEMORY(const void *buffSrc, void *buffDst, U16 size)
{
	memmove(buffDst, buffSrc, size);
}

//^00EB:01EB
void SkWinCore::MOVE_MEMORY_WITH_DELTA(U16 offSrc, U16 offDst, U16 size) //#DS=04BF
{
	MOVE_MEMORY(
		glbBlitBufferSource + offSrc,
		_04bf_0e34 + offDst,
		size
		);
	return;
}

//^00EB:0221
void SkWinCore::COPY_MEMORY_WITH_DELTA_AND_TRANSPARENT(U16 offSrc, U16 offDst, U16 size, Bit8u colorkey) //#DS=04BF
{
	Bit8u dh = colorkey;
	Bit8u *di = _04bf_0e34 + offDst;
	U16 cx = size;
	Bit8u *si = glbBlitBufferSource + offSrc;

	do {
		Bit8u al = *(si++);
		if (al != colorkey) {
			*(di++) = al;
		}
		else {
			di++;
		}
	} while (--cx != 0);
	return;
}

//^00EB:0812
void SkWinCore::MOVE_MEMORY_STRETCH_13TO16(U16 offSrc, U16 offDst)
{
	// compose 0.5 stepping viewport. (123% larger)

	Bit8u *di = _04bf_0e34 + offDst;
	Bit8u *si = glbBlitBufferSource + offSrc;
	U16 cx = 14;

	// DS:SI	E000:871B -> E000:8728 (+13)
	// ES:DI	A000:3340 -> A000:3350 (+16)

	// [DS:SI] in  E000:8728  A4 A7 A2 A7 AB AB A6 A9 A9 A9 A9 A9 A8
	// [ES:DI] out A000:3350  A4 A7 A2 A2 A7 AB AB A6 A9 A9 A9 A9 A9 A9 A9 A8

	// [DS:SI] in  E000:87FB  00 01 02 03 04 05 06 07 08 09 0A 0B 0C
	// [ES:DI] out A000:3480  00 01 02 02 03 04 05 06 07 07 08 09 0A 0B 0B 0C

	do {
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
	} while (--cx != 0);
	return;
}


//^098D:0D3F
SRECT *SkWinCore::SCALE_RECT(U16 rectno, SRECT *rc, U16 horzResolution, U16 vertResolution)
{
	// horzResolution, vertResolution: 10,000 is fair. 5,000 is half. 20,000 is double.

	U16 di = horzResolution;
	U16 si = vertResolution;
	SRECT *bp04 = QUERY_RECT(&glbRectNoTable, rectno);
	if (bp04 != NULL) {
		if (bp04->y != 0) {
			bp04 = QUERY_RECT(&glbRectNoTable, bp04->y);
			if (bp04 != NULL) {
				if (bp04->x == 9) {
					i16 bp06 = (di == C10000_RECTZONE_SCALE_FULL) ? (bp04->cx) : (i16)((i32(bp04->cx) * di) / C10000_RECTZONE_SCALE_FULL);
					i16 bp08 = (si == C10000_RECTZONE_SCALE_FULL) ? (bp04->cy) : (i16)((i32(bp04->cy) * si) / C10000_RECTZONE_SCALE_FULL);
					if (bp06 == 0 && di != 0) {
						bp06 = 1;
					}
					if (bp08 == 0 && si != 0) {
						bp08 = 1;
					}
					if (bp06 > 0 && bp08 > 0) {
						return QUERY_BLIT_RECT(NULL, rc, rectno, &bp06, &bp08, -1);
					}
				}
			}
		}
	}
	return NULL;
}

//^098D:0E5D
SRECT *SkWinCore::QUERY_EXPANDED_RECT(U16 rectno, SRECT *rc)
{
	return SCALE_RECT(rectno, rc, C10000_RECTZONE_SCALE_FULL, C10000_RECTZONE_SCALE_FULL);
}


//^0B36:1599
void SkWinCore::DRAW_DIALOGUE_PICT(Bit8u *srcImage, Bit8u *dstImage, SRECT *rect, U16 srcx, U16 srcy, U16 colorkey, Bit8u localpal[16])
{
	if (SkCodeParam::bUsePowerDebug) {
		// 1st pass, check for valid image pointer
		if (!CheckSafePointer(srcImage) || !CheckSafePointer(dstImage))
			return;
		// 2nd pass, check for valid bpp for each image pointer (if not, the pointer may be messed, then it will crash)
		if ( ( ((U16 *)srcImage)[-3] != 4 && ((U16 *)srcImage)[-3] != 8) ||
			( ((U16 *)dstImage)[-3] != 4 && ((U16 *)dstImage)[-3] != 8) )
			return;
	}
	// draw the:
	// a) pre rendered dialogue to screen

	FIRE_BLIT_PICTURE(
		srcImage,
		dstImage,
		rect,
		srcx,
		srcy,
		((U16 *)srcImage)[-2], // src-pitch
		((U16 *)dstImage)[-2], // dst-pitch
		colorkey,
		0,
		((U16 *)srcImage)[-3], // src-bpp
		((U16 *)dstImage)[-3], // dst-bpp
		localpal
		);
}


//^3E74:0C16
void SkWinCore::FREE_PICT_BUFF(U8* xImageBuffer)
{
	// READ_UI16(buff,-6) -> bpp(4 or 8)
	// READ_UI16(buff,-4) -> width
	// READ_UI16(buff,-2) -> height
/*
	DEALLOC_UPPER_MEMORY(
		(	(READ_UI16(xImageBuffer,-6) == 4)	// bpp
			? (((READ_UI16(xImageBuffer,-4) +1) & 0xFFFE) >> 1)	// if 4 bpp, round up width to byte align.
			:   (READ_UI16(xImageBuffer,-4)     & 0xFFFF)		// if 8 bpp, already byte aligned.
		) * READ_UI16(xImageBuffer,-2) +6
	);*/

	DEALLOC_UPPER_MEMORY(
		(	(READ_IMGBUFF_BPP(xImageBuffer) == IMG_4_BPP)	// bpp
			? (((READ_IMGBUFF_WIDTH(xImageBuffer) +1) & 0xFFFE) >> 1)	// if 4 bpp, round up width to byte align.
			:   (READ_IMGBUFF_WIDTH(xImageBuffer)     & 0xFFFF)		// if 8 bpp, already byte aligned.
		) * READ_IMGBUFF_HEIGHT(xImageBuffer) +6
	);
}


//^098D:0C13
void SkWinCore::TRIM_BLIT_RECT(i16 xx, i16 yy, i16 ss, i16 tt)
{
	ENTER(0);
	SET_SRECT(&glb_4976_4bca_Rect, xx, yy, _4976_00f6 -ss -xx, _4976_00f8 -tt -yy);
	return;
}


//^00EB:05C7
// SPX: _00eb_05c7 renamed SET_GRAPHICS_RGB_PALETTE
X16 SkWinCore::SET_GRAPHICS_RGB_PALETTE(U8 (*pal)[4], X16 yy) //#DS=04BF
{
	ENTER(0);
	LOADDS(0x0C48);
	i16 si;
	for (si = 0; si < 0x100; si++) {
		glbPaletteRGB[si][0] = pal[si][1] >> 2;
		glbPaletteRGB[si][1] = pal[si][2] >> 2;
		glbPaletteRGB[si][2] = pal[si][3] >> 2;
	}
	if (glbUpdatePalette == 1)
		IBMIO_UPDATE_PALETTE_SET();
	return 1;
}

//^44C8:1BAF
// SPX: _44c8_1baf renamed SET_RGB_PALETTE_FROM_DATA
void SkWinCore::SET_RGB_PALETTE_FROM_DATA(U8* xPalette)
{
	ENTER(0);
	glbPaletteIRGBLoaded = SET_GRAPHICS_RGB_PALETTE(reinterpret_cast<U8 (*)[4]>(xPalette), 0);
	return;
}


//^0B36:00C3
// SPX: _0b36_00c3 renamed QUERY_PICST_IMAGE_FROM_MEMENT_CACHE
X8* SkWinCore::QUERY_PICST_IMAGE_FROM_MEMENT_CACHE(i16 iCacheIndex, Picture *ref)
{
	ENTER(4);
	X8* xImageDataBuff = reinterpret_cast<Bit8u *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(iCacheIndex)); // bp04
	if (SkCodeParam::bUsePowerDebug && !CheckSafePointer(xImageDataBuff))
		return NULL;
	if (ref != NULL) {
		ref->pb0 = xImageDataBuff;
		ref->w14 = 0;
		ref->w16 = 0;
		ref->iWidth = READ_IMGBUFF_WIDTH(xImageDataBuff);	// READ_UI16(xImageDataBuff,-4);
		ref->iHeight = READ_IMGBUFF_HEIGHT(xImageDataBuff);	// READ_UI16(xImageDataBuff,-2);
		ref->iBpp = READ_IMGBUFF_BPP(xImageDataBuff);	// READ_UI16(xImageDataBuff,-6);
        ref->w12 = iCacheIndex;
		ref->w4 = 8;
	}
	return xImageDataBuff;
}

//^0B36:004B
X8* SkWinCore::QUERY_PICST_IMAGE(Picture* xPicture)
{
	ENTER(4);
	X8* xImageDataBuff = QUERY_GDAT_IMAGE_ENTRY_BUFF(xPicture->iGDatCls1Category, xPicture->iGDatCls2MainItemId, xPicture->iGDatCls4EntryId);	// bp04
	if (SkCodeParam::bUseFixedMode && xImageDataBuff == NULL)
		xImageDataBuff = QUERY_GDAT_IMAGE_ENTRY_BUFF(GDAT_CATEGORY_x15_MISCELLANEOUS, GDAT_ITEM_DEFAULT_INDEX, GDAT_ITEM_DEFAULT_INDEX); // Get Yukman!
	if (SkCodeParam::bUseFixedMode && xImageDataBuff == NULL)
		return NULL;
	xPicture->pb0 = xImageDataBuff;
	xPicture->w14 = 0;
	xPicture->w16 = 0;
	xPicture->iWidth = READ_IMGBUFF_WIDTH(xImageDataBuff);		// READ_UI16(xImageDataBuff,-4);
	xPicture->iHeight = READ_IMGBUFF_HEIGHT(xImageDataBuff);	// READ_UI16(xImageDataBuff,-2);
	xPicture->iBpp = READ_IMGBUFF_BPP(xImageDataBuff);	// bpp	READ_UI16(xImageDataBuff,-6);
	xPicture->w4 = 4;
	return xImageDataBuff;
}


//^3E74:0A77
void SkWinCore::FREE_PICT6(SkImage *ref)
{
	ENTER(0);
	if (_4976_5d76 == 0) {
		if (READ_UI16(ref,-6 +4) == afDefault) {	// afDefault == 0 (upper first)
			DEALLOC_UPPER_MEMORY(READ_UI32(ref,-6 +0));
		}
		else {
			DEALLOC_LOWER_MEMORY(READ_UI32(ref,-6 +0));
		}
	}
	return;
}

//^3E74:52F4
i16 SkWinCore::QUERY_GDAT_PICT_OFFSET(Bit8u cls1, Bit8u cls2, Bit8u cls4)
{
	ENTER(8);
	U16 di = 0;
	RawEntry* bp04 = QUERY_GDAT_ENTRYPTR(cls1, cls2, dtImage, cls4);
	if (bp04 == NULL) {
		return 0;
	}
	else {
		U16 si = bp04->data;
		SkImage *bp04;
		if (glbShelfMemoryTable[si].Absent()) {
			di = 1;
			bp04 = reinterpret_cast<SkImage *>(QUERY_GDAT_ENTRY_DATA_PTR(cls1, cls2, dtImage, cls4));
		}
		else {
			bp04 = reinterpret_cast<SkImage *>(REALIZE_GRAPHICS_DATA_MEMORY(glbShelfMemoryTable[si]));
		}
		i16 bp06 = bp04->Xoffset();
		i16 bp08;
		if (bp06 == -32) {
			si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, dtImageOffset, cls4);
		}
		else {
			bp08 = bp04->Yoffset();
#if DM2_EXTENDED_MODE == 1
			if (bp08 == -32 || bp08 == 31) {
				bp08 = 0;
			}
#else
			if (bp08 == -32) {
				bp08 = 0;
			}
#endif
			si = (bp06 << 8) | Bit8u(bp08);
		}
		if (di != 0) {
			FREE_PICT6(bp04);
		}
		return si;
	}
}

//^0B36:0520
//ExtendedPicture *SkWinCore::QUERY_GDAT_SUMMARY_IMAGE(ExtendedPicture *ref, Bit8u cls1, Bit8u cls2, Bit8u cls4)
ExtendedPicture *SkWinCore::QUERY_GDAT_SUMMARY_IMAGE(ExtendedPicture* xExtPicture, U8 iGDatCls1Category, U8 iGDatCls2MainItemId, U8 iGDatCls4EntryId)
{
	SkD((DLV_DBG_GETPIC, "DBG: QUERY_GDAT_SUMMARY_IMAGE(%p,%02X,%02X,%02X)\n"
		, xExtPicture, (Bitu)iGDatCls1Category, (Bitu)iGDatCls2MainItemId, (Bitu)iGDatCls4EntryId));

	ENTER(8);
	ZERO_MEMORY(xExtPicture, sizeof(ExtendedPicture));	// 314
	xExtPicture->w12 = -1;
	xExtPicture->iGDatCls1Category = iGDatCls1Category;
	xExtPicture->iGDatCls2MainItemId = iGDatCls2MainItemId;
	xExtPicture->iGDatCls3DataType = dtImage;
	xExtPicture->iGDatCls4EntryId = iGDatCls4EntryId;
	xExtPicture->iYStretch = 64;	// default 100% of stretch
	xExtPicture->iXStretch = 64;	// default 100% of stretch
	xExtPicture->rectNo = 0xFFFF;
	xExtPicture->w26 = 0xFFFF;
	xExtPicture->pb44 = _4976_4964;
	xExtPicture->colorKeyPassThrough = -1;
	if (iGDatCls1Category != 0xFF) {
		xExtPicture->iRawDataIndex = QUERY_GDAT_ENTRY_DATA_INDEX(iGDatCls1Category, iGDatCls2MainItemId, dtImage, iGDatCls4EntryId);
		i16 bp02 = QUERY_GDAT_ENTRY_DATA_INDEX(iGDatCls1Category, iGDatCls2MainItemId, dtImageOffset, 0xFE);
		if (bp02 != 0) {
			xExtPicture->iXOffset += i8(bp02 >> 8);
			xExtPicture->iYOffset += i8(bp02);
		}
		bp02 = QUERY_GDAT_PICT_OFFSET(iGDatCls1Category, iGDatCls2MainItemId, iGDatCls4EntryId);
		if (bp02 != 0) {
			xExtPicture->iXOffset += i8(bp02 >> 8);
			xExtPicture->iYOffset += i8(bp02);
		}
		U8* xLocalPal = QUERY_GDAT_IMAGE_LOCALPAL(iGDatCls1Category, iGDatCls2MainItemId, iGDatCls4EntryId);	// bp06
		if (xLocalPal == NULL) {
			xExtPicture->iPaletteSize = C256_GDAT_PALETTE_SIZE_256;
			xLocalPal = xExtPicture->iPal256;
			i16 iColorIdx = 0;	// bp08
			for (; iColorIdx < C256_GDAT_PALETTE_SIZE_256; iColorIdx++) {
				xLocalPal[iColorIdx] = (U8)iColorIdx;
			}
		}
		else {
			xExtPicture->iPaletteSize = C016_GDAT_PALETTE_SIZE_16;
			COPY_MEMORY(xLocalPal, xExtPicture->iPal256, C016_GDAT_PALETTE_SIZE_16);
		}
	}
	return xExtPicture;
}


//^3E74:58BF
void SkWinCore::ALLOC_IMAGE_MEMENT(U8 iGDatCls1Category, U8 iGDatCls2MainItemId, U8 iGDatCls4EntryId)
{
	ENTER(4);
	U16 iRawDataIndex = QUERY_GDAT_ENTRY_DATA_INDEX(iGDatCls1Category, iGDatCls2MainItemId, dtImage, iGDatCls4EntryId);	// si
	if (iRawDataIndex != 0xFFFF && glbShelfMemoryTable[iRawDataIndex].Absent()) {
		iRawDataIndex = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x15_MISCELLANEOUS,0xFE,dtImage,0xFE);	// yukman		
	}
	if (iRawDataIndex != 0xFFFF) {
		U16 iMementIdx = QUERY_MEMENTI_FROM(iRawDataIndex);	// di
		if (iMementIdx == 0xFFFF) {
			if (glbShelfMemoryTable[iRawDataIndex].Absent()) {
				return;
			}
			SkImage* xImage = reinterpret_cast<SkImage *>(REALIZE_GRAPHICS_DATA_MEMORY(glbShelfMemoryTable[iRawDataIndex]));	// bp04
			if (xImage->Yoffset() != -32) {
				return;
			}
			if (xImage->BitsPixel() != IMG_8_BPP) {
				return;
			}
			glbRawDataNoAllocated = iRawDataIndex;
			return;
		}
		else {
			MEMENT_3e74_4549(iMementIdx);
		}
	}
	return;
}

//^0B36:018F
void SkWinCore::ALLOC_PICT_MEMENT(Picture* xPicture)
{
	ENTER(0);
	if ((xPicture->w4 & 0x0004) != 0) {
		ALLOC_IMAGE_MEMENT(xPicture->iGDatCls1Category, xPicture->iGDatCls2MainItemId, xPicture->iGDatCls4EntryId);
	}
	else if ((xPicture->w4 & 0x0008) != 0) {
		_3e74_583a_MEMENTI(xPicture->w12);
	}

}

//^0B36:01CD
void SkWinCore::FREE_PICT_MEMENT(Picture* xPicture)
{
	ENTER(0);
	if ((xPicture->w4 & 0x0004) != 0) {
		FREE_IMAGE_MEMENT(xPicture->iGDatCls1Category, xPicture->iGDatCls2MainItemId, xPicture->iGDatCls4EntryId);
	}
	else if ((xPicture->w4 & 0x0008) != 0) {
		_3e74_585a_CACHE(xPicture->w12, 0);
	}

}

//^0B36:068F
Bit32u SkWinCore::CALC_PICT_ENT_HASH(ExtendedPicture *xExtPicture)
{
	ENTER(0);
	return (Bit32u(xExtPicture->iRawDataIndex & 0x1FFF) << 12) | ((xExtPicture->iXStretch & 0x007f) << 5) | (xExtPicture->iYStretch & 0x001f);
}

//^3E74:5992
void SkWinCore::FREE_IMAGE_MEMENT(U8 iGDatCls1Category, U8 iGDatCls2MainItemId, U8 iGDatCls4EntryId)
{
	ENTER(0);
	U16 iRawDataNo = QUERY_GDAT_ENTRY_DATA_INDEX(iGDatCls1Category, iGDatCls2MainItemId, dtImage, iGDatCls4EntryId);	// si
	if (iRawDataNo == 0xFFFF || glbShelfMemoryTable[iRawDataNo].Absent()) {
		iRawDataNo = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x15_MISCELLANEOUS, 0xFE, dtImage, 0xFE);	// default yukman
	}
	if (iRawDataNo == 0xFFFF)
		return;
	if (glbRawDataNoAllocated == iRawDataNo) {
		glbRawDataNoAllocated = 0xFFFF;
	}
	U16 iMemEntIdx = QUERY_MEMENTI_FROM(iRawDataNo);	// di
	if (iMemEntIdx != 0xFFFF) {
		RECYCLE_MEMENTI(iMemEntIdx, 0);
	}
	return;
}



//^44C8:2307
// TODO: related gfx ?
void SkWinCore::_44c8_2307(X16 xx, X16 yy, X16 zz, X16 ww)
{
	ENTER(4);
	U8 *bp04 = _4976_5e64 + xx;
	U16 di = zz >> 1;
	U16 cx = ww + yy;
	U16 si = yy;
	do {
		glbBlit2MemDest[si] = bp04[di >> 7];
		di += zz;
		si++;
	} while (si < cx);
	return;
}


//^44C8:2351
void SkWinCore::_44c8_2351(Bit8u *xx, Bit8u *yy, U16 ss, U16 tt, U16 uu, U16 vv)
{
	ENTER(10);
	U16 di = uu;
	_4976_5e64 = xx;
	glbBlit2MemDest = yy;
	U16 bp04 = (i32(ss) << 7) / di;
	U16 bp06 = (i32(tt) << 7) / vv;
	U16 bp02 = bp06 >> 1;
	U16 si = 0;
	X16 bp08 = 0; // defaulting to 0
	X16 bp0a = 0; // defaulting to 0
	for (; si < vv; bp0a = bp08, bp02 += bp06, si++) {
		bp08 = bp02 >> 7;
		if (bp0a == bp08 && si > 0) {
			_4976_5e64 = yy;
			_44c8_0b8d((si -1) * di, si * di, di);
			_4976_5e64 = xx;
		}
		else {
			_44c8_2307(bp08 * ss, si * di, bp04, di);
		}
	}
	return;
}

//^44C8:000F
void SkWinCore::FIRE_BLIT_TO_MEMORY_ROW_4TO4BPP(U16 offSrc, U16 offDst, U16 width)
{
	ENTER(0);
	U8* di = glbBlit2MemDest;
	U8* si = _4976_5e64;
	U16 cx = width;

	bool carry1 = (offSrc & 1) ? true : false;
	offSrc >>= 1;
	bool carry2;
	if (!carry1) {
		si += offSrc;
		carry2 = (offDst & 1) ? true : false;
		offDst >>= 1;
		if (!carry2) {
			di += offDst;

_0036:
			bool carry3 = (cx & 1) ? true : false;
			cx >>= 1;
			{
				bool carry4 = (cx & 1) ? true : false;
				cx >>= 1;
				if (carry4) {
					*di = *si; di++; si++;
				}
				while (cx != 0) {
					*di = *si; di++; si++;
					*di = *si; di++; si++;
					cx--;
				}
			}
			if (carry3) {
				*di = (*si & 0xf0) | (*di & 0x0f); si++; di++;
			}
		}
		else {
			di += offSrc;
			Bit8u bh = *di;
			cx++;
			bool carry = (cx & 1) ? true : false;
			cx >>= 1;
			if (cx != 0) {
				bool carry = (cx & 1) ? true : false;
				if (carry) {
					Bit8u al = *si; si++;
					*di = (bh & 0xf0) | (al >> 4); di++;
					bh = al << 4;
				}
				if (cx != 0) {
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

						Bit8u al = *si; si++;
						Bit8u ah = *si; si++;
						*di =  bh       | (al >> 4); di++;
						*di = (al << 4) | (ah >> 4); di++;
						bh = ah << 4;
					} while (--cx != 0);
				}
			}
			if (carry) {
				*di = (*di & 0x0f) | bh; di++;
			}
		}
	}
	else {
        si += offSrc;
		{
			bool carry = (offDst & 1) ? true : false;
			offDst >>= 1;
			if (carry) {
				di += offDst;
				*di = (*si & 0x0f) | (*di & 0xf0); si++; di++;
				cx--;
				goto _0036;
			}
		}
		di += offDst;
		Bit8u bh = *si; si++;
		{
			bool carry = (cx & 1) ? true : false;
			cx >>= 1;
			if (cx != 0) {
				bool carry = (cx & 1) ? true : false;
				cx >>= 1;
				if (carry) {
					Bit8u bl = *si;
					*di = (*si >> 4) | (bh << 4); di++; si++;
					bh = bl;
				}
				if (cx != 0) {
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

						Bit8u al = *si; si++;
						Bit8u ah = *si; si++;
						*di = (bh << 4) | (al >> 4); di++;
						*di = (al << 4) | (ah >> 4); di++;
						bh = ah & 0x0f;
					} while (--cx != 0);
				}
			}
			if (carry) {
				*di = (*di & 0x0f) | (bh << 4);
			}
		}
	}
	return;
}

//^44C8:20E5
void SkWinCore::_44c8_20e5(U16 srcOff, U16 dstOff, U16 srcWidth, U16 dstWidth)
{
	ATLASSERT(srcWidth != 0);

	ENTER(0);
    U8* di = glbBlit2MemDest + (dstOff >> 1);
	U16 cx = srcWidth >> 1;
	U16 dx = (dstWidth +1) >> 1;
	U8* si = _4976_5e64 + (srcOff >> 1);
	Bit8u bh = 0;
	do {
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
	return;
}


//^44C8:2143
void SkWinCore::_44c8_2143(U16 xx, U16 yy, U16 ss, U16 tt)
{
	ENTER(4);
	U16 si = ss >> 1;
	U16 bp04 = ((tt +yy) +1) >> 1;
	U16 di = yy >> 1;
	while (di < bp04) {
		U16 bp02 = (si >> 7) +xx;
		U8 al;
		U8 dl = al = _4976_5e64[bp02 >> 1];
		si += ss;
		if ((bp02 & 1) != 0) {
			al &= 0xF;
		}
		else {
			al >>= 4;
		}
		al = _4976_5dbe[al];
		al <<= 4;
		U8 cl = al;
		bp02 = (si >> 7) +xx;
		al = _4976_5e64[bp02 >> 1];
		dl = al;
		si += ss;
		if ((bp02 & 1) != 0) {
			al &= 0xF;
		}
		else {
			al >>= 4;
		}
		al = cl | _4976_5dbe[al];
		glbBlit2MemDest[di] = al;
		di++;
	}
	return;
}

#pragma warning(push)
#pragma warning(disable: 4700)

//^44C8:21F3
void SkWinCore::FIRE_STRETCH_BLIT_TO_MEMORY_4TO4BPP(Bit8u *src, Bit8u *dst, U16 srcWidth, U16 srcHeight, U16 dstWidth, U16 dstHeight, U8 *aa)
{
	if (SkCodeParam::bUsePowerDebug && (!CheckSafePointer(src) || !CheckSafePointer(dst) ))
		return;
	ENTER(12);
	_4976_5e64 = src;
	glbBlit2MemDest = dst;
	_4976_5dbe = aa;
	U16 bp08 = (dstWidth +1) & 0xFFFE;
	U16 bp0a = (srcWidth +1) & 0xFFFE;
	U16 bp04 = (i32(i16(srcWidth)) << 7) / i16(dstWidth);
	U16 bp06 = (i32(i16(srcHeight)) << 7) / i16(dstHeight);
	U16 bp02 = bp06 >> 1;
	U16 bp0c = 0; // defaulting to 0
	for (U16 si = 0; si < dstHeight; si++) {
		U16 di = bp02 >> 7;
		if (bp0c == di && si > 0) {
			_4976_5e64 = dst;
			FIRE_BLIT_TO_MEMORY_ROW_4TO4BPP((si -1) * bp08, si * bp08, dstWidth);
			_4976_5e64 = src;
		}
		else if (aa == 0) {
			_44c8_20e5(di * bp0a, si * bp08, bp04, dstWidth);
		}
		else {
			_44c8_2143(di * bp0a, si * bp08, bp04, dstWidth);
		}
		bp0c = di;
		bp02 += bp06;
	}
	return;
}
#pragma warning(pop)

//^0B36:06D3
ExtendedPicture *SkWinCore::QUERY_PICST_IT(ExtendedPicture* xExtPicture)
{
	// query image with some capacity
	// - stretch

	ENTER(364);
	U16 iIsStretched = (xExtPicture->iXStretch != 64 || xExtPicture->iYStretch != 64) ? 1 : 0;	// bp12
	U16 bp14 = 0;
	U16 bp16 = (xExtPicture->iRawDataIndex == GDAT_xFFFF_RAW_DATA_INDEX__NONE && xExtPicture->w12 >= 0) ? 1 : 0;
	if (iIsStretched != 0) {
		xExtPicture->iXOffset = CALC_STRETCHED_SIZE(xExtPicture->iXOffset, xExtPicture->iXStretch);
		xExtPicture->iYOffset = CALC_STRETCHED_SIZE(xExtPicture->iYOffset, xExtPicture->iYStretch);
	}
	if ((xExtPicture->mirrorFlip & 0x0001) != 0) {
		xExtPicture->iXOffset = - xExtPicture->iXOffset;
	}
	if (xExtPicture->colorKeyPassThrough == -3)
		return xExtPicture;
	if (iIsStretched == 0 && bp14 == 0) {
		if (bp16 != 0) {
			QUERY_PICST_IMAGE_FROM_MEMENT_CACHE(xExtPicture->w12, xExtPicture);
			return xExtPicture;
		}
		else {
			QUERY_PICST_IMAGE(xExtPicture);
			return xExtPicture;
		}
	}
	U16 bp18;
	Bit8u *bp04;
	Bit8u *bp08;
	Picture bp32;
	if (bp16 != 0) {
		bp18 = ALLOC_TEMP_CACHE_INDEX();
		COPY_MEMORY(xExtPicture, &bp32, sizeof(Picture));	// 24 is size of Picture
		Bit8u *bp08 = QUERY_PICST_IMAGE_FROM_MEMENT_CACHE(bp32.w12, &bp32);
		ALLOC_PICT_MEMENT(&bp32);
		bp04 = NULL;
	}
	else {
		Bit32u bp10 = CALC_PICT_ENT_HASH(xExtPicture);
		if (ADD_CACHE_HASH(bp10, &bp18) != 0) {
			QUERY_PICST_IMAGE_FROM_MEMENT_CACHE(bp18, xExtPicture);
			return xExtPicture;
		}
		else {
			bp04 = NULL;
			bp08 = NULL;
			if (bp08 == NULL) {
				COPY_MEMORY(xExtPicture, &bp32, sizeof(Picture));	// 24 is size of Picture
				bp08 = QUERY_PICST_IMAGE(&bp32);
				ALLOC_PICT_MEMENT(&bp32);
			}
		}
	}
	if ((bp32.w4 & 0x0010) != 0) {
		ExtendedPicture bp016c;
		COPY_MEMORY(xExtPicture, &bp016c, sizeof(ExtendedPicture));	// 314 is ExtendedPicture size
		bp016c.mirrorFlip = 0;
		bp016c.iYStretch = 64;
		bp016c.iXStretch = 64;
		U16 bp1a;
		if (ADD_CACHE_HASH(CALC_PICT_ENT_HASH(&bp016c), &bp1a) == 0) {
			bp08 = ALLOC_NEW_PICT(bp1a, bp32.iWidth, bp32.iHeight, bp32.iBpp);	// bp08
			U8* bp0c = QUERY_PICT_BITS(&bp32);	// bp0c
			FIRE_BLIT_PICTURE(
				bp0c,
				bp08,
				ALLOC_TEMP_ORIGIN_RECT(bp32.iWidth, bp32.iHeight),
				bp32.w14,
				bp32.w16,
				READ_IMGBUFF_WIDTH(bp0c),	// READ_UI16(bp0c,-4)
				READ_IMGBUFF_WIDTH(bp08),	// READ_UI16(bp08,-4)
                -1,
				0,
				bp32.iBpp,
				bp32.iBpp,
				NULL
				);
		}
		FREE_PICT_MEMENT(&bp32);
		Bit8u *bp08 = QUERY_PICST_IMAGE_FROM_MEMENT_CACHE(bp1a, &bp32);
		ALLOC_PICT_MEMENT(&bp32);
	}
	if (iIsStretched != 0) {
		i16 iWidthStretched = CALC_STRETCHED_SIZE(bp32.iWidth, xExtPicture->iXStretch);	// di
		i16 iHeightStretched = CALC_STRETCHED_SIZE(bp32.iHeight, xExtPicture->iYStretch);	// si
		if (iWidthStretched <= 0 || iHeightStretched <= 0) {
			xExtPicture->iWidth = iWidthStretched;
			xExtPicture->iHeight = iHeightStretched;
		}
		else {
			ALLOC_NEW_PICT(bp18, iWidthStretched, iHeightStretched, bp32.iBpp);
			bp08 = QUERY_PICT_BITS(&bp32);
			bp04 = QUERY_PICST_IMAGE_FROM_MEMENT_CACHE(bp18, xExtPicture);
			if (bp32.iBpp == IMG_8_BPP) {
				_44c8_2351(bp08, bp04, bp32.iWidth, bp32.iHeight, iWidthStretched, iHeightStretched);
			}
			else {
				FIRE_STRETCH_BLIT_TO_MEMORY_4TO4BPP(bp08, bp04, bp32.iWidth, bp32.iHeight, iWidthStretched, iHeightStretched, NULL);
			}
		}
	}
	FREE_PICT_MEMENT(&bp32);
	_3e74_585a_CACHE(bp18, 0);
	return xExtPicture;
}


//^32CB:2CF3
// SPX: _32cb_2cf3 renamed _32cb_2cf3
void SkWinCore::DRAW_SOME_CLOUD_EXPLOSION(U8 cls2, U16 scale64, U16 mirrorFlip, U16 rectno)
{
	ENTER(0);
	U16 si = scale64;
	si = BETWEEN_VALUE(8, si & 0xFFFE, 64);
	QUERY_GDAT_SUMMARY_IMAGE(&glbTempPicture, GDAT_CATEGORY_x0D_SPELL_MISSILES, cls2, C65_GDAT_IMG_SPELL_EXPLOSION_FRONT);	// 0x0d, cls, 0x41
	glbTempPicture.w32 = glbTempPicture.iXOffset;
	glbTempPicture.w34 = glbTempPicture.iYOffset;
	glbTempPicture.iYOffset = glbTempPicture.iXOffset = 0;
	glbTempPicture.w26 = 0;
	glbTempPicture.rectNo = rectno;
	glbTempPicture.colorKeyPassThrough = 10;	// hard set color key!
	glbTempPicture.pb44 = glbBackBuffViewport;
	glbTempPicture.mirrorFlip = mirrorFlip;
	glbTempPicture.iYStretch = glbTempPicture.iXStretch = si;



	PALETTE_SOMETHING_0b36_037e(glbTempPicture.iPal256, i8(_4976_5a88), 10, -1, glbTempPicture.iPaletteSize);
	QUERY_PICST_IT(&glbTempPicture);

	// SPX: it seems to be missing the displacement from GDAT, else, a cloud/explosion appears on the floor
	if (SkCodeParam::bUseFixedMode) {
		glbTempPicture.iXOffset = 0;	// default
		glbTempPicture.iYOffset = -55;	// default
		X16 iDisplacementWord = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x0D_SPELL_MISSILES, cls2, dtImageOffset, C65_GDAT_IMG_SPELL_EXPLOSION_FRONT);
		if (iDisplacementWord != 0) {
			glbTempPicture.iXOffset = ((i8)(iDisplacementWord>>8));
			glbTempPicture.iYOffset = ((i8)(iDisplacementWord&0xFF));
		}
	}

	return;
}

//^0B36:0334
X8* SkWinCore::TRANSLATE_PALETTE(X8* localpal, U8 iGDatCategory, U8 iGDatItemId, U8 iGDatEntryId, i16 palentcnt)
{
	ENTER(4);
	SkD((SkCodeParam::bDebugPrint, "TRANSPAL: %8x, %d %d %d (%d)\n", localpal, iGDatCategory, iGDatItemId, iGDatEntryId, palentcnt));
	// 1-0-1 is a 256 palette of grey, replacing all other hues (so it is precomputed from the standard color palette), used for replacing weapons in hand with greys
	X8* xTranslatePalette = QUERY_GDAT_ENTRY_DATA_PTR(iGDatCategory, iGDatItemId, dtRaw7, iGDatEntryId);	// bp04
	SkD((SkCodeParam::bDebugPrint, "xTranslatePalette = %8x\n", xTranslatePalette));
	
	if (SkCodeParam::bUsePowerDebug && !CheckSafePointer(xTranslatePalette))	// if palette is not found, don't translate (and don't crash)
		return localpal;

	for (U16 iPalIndex = 0; iPalIndex < palentcnt; iPalIndex++) {
		localpal[iPalIndex] = xTranslatePalette[localpal[iPalIndex]];
	}
	return localpal;
}

