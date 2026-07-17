//------------------------------------------------------------------------------
// SK-GRAPHICS.DAT
//	Graphics.dat entries/items/data management
//------------------------------------------------------------------------------

//#include <StdAfx.h>	// Required for MVC6 compilation
#include <skcore.h>

#include <skglobal.h>

#include <skparam.h>
#include <skdebug.h>
//#if defined(_USE_MFC80) || defined(_USE_MFC60)
//#include <SKMFC.h>
//#include <resource.h>
//#endif // defined(_USE_MFC80) || defined(_USE_MFC60)

//#include <DME.h>

//^3E74:1562
SkEnt4 SkWinCore::MAKE_ENT4(U8 iGDatCls1Category, U8 iGDatCls2MainItemId, U8 iGDatCls3DataType, U8 iGDatCls4EntryId)
{
	ENTER(4);
	SkEnt4 xGDatEntry;	// b4
	xGDatEntry.cls1(iGDatCls1Category);
	xGDatEntry.cls2(iGDatCls2MainItemId);
	xGDatEntry.cls3(iGDatCls3DataType);
	xGDatEntry.cls4(iGDatCls4EntryId);
	return xGDatEntry;
}

//^482B:0624
// SPX: _482b_0624 renamed SOUND_ENTRY
void SkWinCore::SOUND_ENTRY(U8 iGDatCls1Category, U8 iGDatCls2MainItemId, U8 iGDatCls4EntryId)
{
	ENTER(4);
//#ifndef __DJGPP__
	if (QUERY_SND_ENTRY_INDEX(iGDatCls1Category, iGDatCls2MainItemId, iGDatCls4EntryId) == 0 && _4976_49d4[0][0] < _4976_5d58) {
		SoundShortInfo* xSoundInfo = &tblSoundShortInfo[_4976_49d4[0][0]];	// bp04
		xSoundInfo->category = iGDatCls1Category;
		xSoundInfo->index = iGDatCls2MainItemId;
		xSoundInfo->entry = iGDatCls4EntryId;
		xSoundInfo->iGDatRawIndex = 0xFFFF;
		_4976_49d4[0][0]++;
	}
//#endif	
	return;
}



//^44C8:1688
void SkWinCore::DECODE_IMG3_OVERLAY(U8 *blitUnderlay, IMG3 *blitSrc, U8 *blitDst)
{
	// TODO: SUSPECTED;

	ENTER(24);
	_4976_5e64 = reinterpret_cast<U8 *>(blitSrc);
	_4976_5e6a = blitDst;
	_4976_5e6e = blitUnderlay;
	X16 bp02 = blitSrc->Width();
	X16 bp04 = blitSrc->Height();
	X16 bp06 = (bp02 +1)&0xfffe;
	X16 bp08 = bp06 * bp04;
	_4976_5dc2 = 8;
	i16 bp0e = 0;
	U8 bp16[5];

	// SPX: on custom GDAT2, it happens that images have w = 0 and h = 0; in that case, might be better to do nothing here
	if (SkCodeParam::bUsePowerDebug)
	{
		if (bp02 == 0 || bp04 == 0)
			return;
	}

	for (; bp0e < 5; bp0e++) {
		bp16[bp0e] = READ_IMG3_NIBBLE();
	}
	if (bp02 != bp06) {
		X16 bp0c = bp06 -bp02;
		X16 di = 0;
		X16 bp0a = bp02;
		do {
			X8 bp0f = ((_4976_5dc2 & 1) != 0)
				? _4976_5e64[_4976_5dc2 >> 1] & 0xf
				: _4976_5e64[_4976_5dc2 >> 1] >> 4;
			_4976_5dc2++;
			X8 bp10 = bp0f & 7;
			U16 si;
			if (bp10 == 6) {
				si = ((bp0f & 8) != 0) ? READ_IMG3_DURATION() : 1;
				ATLASSERT(di +U32(si) < bp08); // fill range must be inside picture
				while (si >= bp0a) {
					SPILL_IMG3_PIXELS(di, di -bp06, bp0a);
					di += bp0a +bp0c;
					si -= bp0a;
					bp0a = bp02;
				}
				if (si > 0) {
#if UseAltic
					SPILL_IMG3_PIXELS(di, di -bp06, si);
#else
					SPILL_IMG3_PIXELS(di, di -bp06, bp0a);
#endif
					di += si;
					bp0a -= si;
				}
				continue;
			}
			if (bp10 == 5) {
				si = ((bp0f & 8) != 0) ? READ_IMG3_DURATION() : 1;
				ATLASSERT(di +U32(si) < bp08); // fill range must be inside picture
				while (si >= bp0a) {
					TRANSPARENT_IMG3_PIXELS(di, bp0a);
					di += bp0a +bp0c;
					si -= bp0a;
					bp0a = bp02;
				}
				if (si > 0) {
#if UseAltic
					TRANSPARENT_IMG3_PIXELS(di, si);
#else
					TRANSPARENT_IMG3_PIXELS(di, bp0a);
#endif
					di += si;
					bp0a -= si;
				}
				continue;
			}
			X8 bp17;
			if (bp10 < 5) {
				bp17 = bp16[bp10];
			}
			else {
				bp17 = ((_4976_5dc2 & 1) != 0) ? (_4976_5e64[_4976_5dc2 >> 1] & 0xf) : (_4976_5e64[_4976_5dc2 >> 1] >> 4);
				_4976_5dc2++;
			}
			si = ((bp0f & 8) != 0) ? READ_IMG3_DURATION() : 1;
			ATLASSERT(di +U32(si) < bp08); // fill range must be inside picture
			while (si >= bp0a) {
				FIRE_FILL_4BPP_PICT_LINE(di, bp17, bp0a);
				di += bp0a +bp0c;
				si -= bp0a;
				bp0a = bp02;
			}
			if (si > 0) {
				FIRE_FILL_4BPP_PICT_LINE(di, bp17, si);
				di += si;
				bp0a -= si;
			}
		} while (di < bp08);
		ATLASSERT(di <= bp08); // if error, memory broken!
		return;
	}
	else {
		X16 di = 0;
		do {
			X8 bp0f = ((_4976_5dc2 & 1) != 0) ? (_4976_5e64[_4976_5dc2 >> 1] & 15) : (_4976_5e64[_4976_5dc2 >> 1] >> 4);
			_4976_5dc2++;
			X8 bp10 = bp0f & 7;
			X8 bp17;
			X16 si;
			if (bp10 == 6) {
				si = ((bp0f & 8) != 0) ? READ_IMG3_DURATION() : 1;
				SPILL_IMG3_PIXELS(di, di -bp06, si);
				di += si;
				continue;
			}
			else if (bp10 == 5) {
				si = ((bp0f & 8) != 0) ? READ_IMG3_DURATION() : 1;
				TRANSPARENT_IMG3_PIXELS(di, si);
				di += si;
				continue;
			}
			else if (bp10 < 5) {
				bp17 = bp16[bp10];
			}
			else {
				bp17 = ((_4976_5dc2 & 1) != 0) ? (_4976_5e64[_4976_5dc2 >> 1] & 15) : (_4976_5e64[_4976_5dc2 >> 1] >> 4);
				_4976_5dc2++;
			}
			if ((bp0f & 8) != 0) {
				si = READ_IMG3_DURATION();
				FIRE_FILL_4BPP_PICT_LINE(di, bp17, si);
				di += si;
			}
			else {
				_44c8_1202(di++, bp17);
			}
		} while (di < bp08);
		ATLASSERT(di <= bp08); // if error, memory broken!
	}
	return;
}

void SkWinCore::DECODE_IMG9(IMG3 *xx, U8 *pOut) {
	const U32 cx = xx->Width();
	const U32 cy = xx->Height();

	U32 iPix = 0;
	const U32 nPix = cx * cy;

	// a kind of LZSS?
	const U8 typex = reinterpret_cast<U8 *>(xx)[6];

	const U8 *pIn = reinterpret_cast<U8 *>(xx) + 8;

	while (iPix < nPix) {
		U8 command = *pIn;
		pIn++;
		for (int t = 0; t < 8 && iPix < nPix; t++, command >>= 1) {
			if (0 != (command & 1)) {
				// pixel
				pOut[iPix] = *pIn;
				iPix++;
				pIn++;
			}
			else {
				// get back
				int A = pIn[0];
				int B = pIn[1];
				pIn += 2;
				int negative_offset, copy_length;
				if (typex == 2) {
					int Y = A & 0xF;
					int X = A >> 4;
					negative_offset = X + 16 * B;
					copy_length = Y + 3;
				}
				else { // typex == 3
					int Y = A & 0x1F;
					int X = A >> 5;
					negative_offset = X + 8 * B;
					copy_length = Y + 3;
				}
				for (int w = 0; w < copy_length; w++, iPix++) {
					pOut[iPix] = pOut[iPix -negative_offset];
				}
			}
		}
	}
	ATLASSERT(iPix == nPix);
}

//^44C8:1395
void SkWinCore::DECODE_IMG3_UNDERLAY(IMG3 *xx, U8 *yy)
{
	ENTER(24);
	_4976_5e64 = reinterpret_cast<U8 *>(xx);
	_4976_5e6a = yy;
	X16 bp02 = xx->Width();
	X16 bp04 = xx->Height();
	X16 bp06 = (bp02 +1)&0xfffe;
	X16 bp08 = bp06 * bp04;
	_4976_5dc2 = 8;
	X16 bp0e = 0;
	X8 bp16[6];

	// SPX: on custom GDAT2, it happens images have w = 0 and h = 0; in that case, might be better to do nothing here
	if (SkCodeParam::bUsePowerDebug)
	{
		if (bp02 == 0 || bp04 == 0)
			return;
	}

	while (bp0e < 6) {
		bp16[bp0e] = READ_IMG3_NIBBLE();
		bp0e++;
	}
	X16 si = 0;	// SPX: added init to zero
	U16 di = 0; // SPX: added init to zero
	if (bp02 != bp06) {
		X16 bp0c = bp06 -bp02;
		si = 0;
		X16 bp0a = bp02;
		do {
			X8 bp0f = ((_4976_5dc2 & 1) != 0) ? (_4976_5e64[_4976_5dc2 >> 1] & 15) : (_4976_5e64[_4976_5dc2 >> 1] >> 4);
			_4976_5dc2++;
			X8 bp10 = bp0f & 7;
			if (bp10 == 6) {
				di = ((bp0f & 8) != 0) ? (READ_IMG3_DURATION()) : 1;
				while (di >= bp0a) {
					SPILL_IMG3_PIXELS(si, si -bp06, bp0a);
					si += bp0a +bp0c;
					di -= bp0a;
					bp0a = bp02;
				}
				if (di > 0) {
					SPILL_IMG3_PIXELS(si, si -bp06, bp0a);
					si += di;
					bp0a -= di;
				}
			}
			else {
				X8 bp17;
				if (bp10 < 6) {
					bp17 = bp16[bp10];
				}
				else {
					bp17 = ((_4976_5dc2 & 1) != 0) ? (_4976_5e64[_4976_5dc2 >> 1] & 15) : (_4976_5e64[_4976_5dc2 >> 1] >> 4);
					_4976_5dc2++;
				}
				di = ((bp0f & 8) != 0) ? READ_IMG3_DURATION() : 1;
				while (di >= bp0a) {
					FIRE_FILL_4BPP_PICT_LINE(si, bp17, bp0a);
					si += bp0a +bp0c;
					di -= bp0a;
					bp0a = bp02;
				}
				if (di > 0) {
					FIRE_FILL_4BPP_PICT_LINE(si, bp17, di);
					si += di;
					bp0a -= di;
				}
			}
		} while (si < bp08);
		if (SkCodeParam::bDebugNoImageDecodingAssert == false)
			ATLASSERT(si <= bp08); // if error, memory broken!
	}
	else {
		si = 0;
		do {
			// SPX: _4976_5e64 would be an image and _4976_5dc2 the cursor ?
			X8 bp0f = ((_4976_5dc2 & 1) != 0) ? (_4976_5e64[_4976_5dc2 >> 1] & 0xf) : (_4976_5e64[_4976_5dc2 >> 1] >> 4);
			_4976_5dc2++;
			X8 bp10 = bp0f & 7;
			X8 bp17;
			if (bp10 == 6) {
				di = ((bp0f & 8) != 0) ? READ_IMG3_DURATION() : 1;
				SPILL_IMG3_PIXELS(si, si -bp06, di);
				si += di;
				continue;
			}
			else if (bp10 < 6) {
				bp17 = bp16[bp10];
			}
			else {
				bp17 = ((_4976_5dc2 & 1) != 0) ? (_4976_5e64[_4976_5dc2 >> 1] & 15) : (_4976_5e64[_4976_5dc2 >> 1] >> 4);
				_4976_5dc2++;
			}
			if ((bp0f & 8) != 0) {
				di = READ_IMG3_DURATION();
				FIRE_FILL_4BPP_PICT_LINE(si, bp17, di);
				si += di;
			}
			else {
				_44c8_1202(si++, bp17);
			}
		} while (si < bp08);
		if (SkCodeParam::bDebugNoImageDecodingAssert == false)
			ATLASSERT(si <= bp08); // if error, memory broken!
	}

	return;
}

//^44C8:19BD
void SkWinCore::DECODE_IMG3_UNDERLAY_LOCAL(IMG3 *xx, U8 *yy)
{
	ENTER(0);
	DECODE_IMG3_UNDERLAY(xx, yy);
	return;
}

//^3E74:4B48
U8* SkWinCore::EXTRACT_GDAT_IMAGE(U16 iGDatIndex, i16 allocUpper)
{
	SkD((DLV_DBG_GETPIC,"DBG: EXTRACT_GDAT_IMAGE(%4u,%u)\n", (Bitu)iGDatIndex, (Bitu)allocUpper));
	LOGX(("EXTRACT_GDAT_IMAGE(%4u,%u)", (Bitu)iGDatIndex, (Bitu)allocUpper));
	if (SkCodeParam::bUseFixedMode && iGDatIndex >= 65535)
		return NULL;

	U16 iLocalGDatIdx = iGDatIndex;	// di
	i16 bp16 = -1;
	if (_4976_5d76 != 0) {
		U16 iMemEntIdx = QUERY_MEMENTI_FROM(iLocalGDatIdx);	// si
		if (iMemEntIdx != 0xFFFF) {
			mement* xMemEnt;	// bp04
			if (allocUpper != 0) {
				MEMENT_3e74_4549(iMemEntIdx);
				xMemEnt = tblMementsPointers[iMemEntIdx];
			}
			else {
				xMemEnt = GET_MEMENT_FROM_MEMENTINDEX(iMemEntIdx);
			}
			return reinterpret_cast<U8 *>(&xMemEnt[1]); // +18 bytes / pointer to the image buffer
		}
	}
	else {
		sk5cfc_image* xLinkedImage = gblLinkedImageRoot.pNextImage;	// bp04
		while (xLinkedImage != NULL) {
			if (xLinkedImage->iGDatRawDataIdx == iLocalGDatIdx) {
				return (U8 *)&xLinkedImage[1]; // +14 bytes
			}
			xLinkedImage = xLinkedImage->pNextImage;
		}
	}
	U16 bp1e = 0;
	U8 *bp1a;
	U16 bp1c;
	if (_4976_5d78 != 0) {
		bp1c = TRACK_UNDERLAY(iLocalGDatIdx);
		SkD((DLV_DBG_GETPIC,"DBG: Ulay %4d -> %4d\n", (Bitu)i16(bp1c), (Bitu)i16(iLocalGDatIdx)));
		if (bp1c != 0xffff) {
			bp1e = 1;
			bp1a = EXTRACT_GDAT_IMAGE(bp1c, (_4976_5d76 != 0) ? 1 : (!allocUpper));
		}
	}
	shelf_memory xShelfMem = glbShelfMemoryTable[iLocalGDatIdx];	// bp10
	IMG3* xImage;	// bp08
	if (xShelfMem.Absent()) {
		xImage = reinterpret_cast<IMG3 *>(QUERY_GDAT_DYN_BUFF(iLocalGDatIdx, reinterpret_cast<U16 *>(&bp16), (_4976_5d76 != 0) ? 1 : (!allocUpper)));
	}
	else {
		xImage = reinterpret_cast<IMG3 *>(REALIZE_GRAPHICS_DATA_MEMORY(xShelfMem));
	}
	if (xImage->OffsetY() == -32) { // uncompressed 4bpp/8bpp. but 8bpp is untested!
		if (bp16 < 0) {
			U8* bp04 = MEMENT_3e74_0245(iLocalGDatIdx, allocUpper);
			return bp04;
		}
		else {
			printf("Sizeof(IMG3) = %d\n", sizeof(IMG3));
			U8* bp04 = PTR_PADD(xImage,+10);	// what is this +10 ??? IMG3 is said to takes 10 bytes, but it should be 6??
			return bp04;
		}
	}
	U16 bp12 = xImage->Width();
	U16 bp14 = xImage->Height();
#if DM2_EXTENDED_MODE == 1
	i32 bp0c = xImage->GetImageSize();
#else
	i32 bp0c = (((bp12 +1) & 0xFFFE) >> 1) * bp14;
#endif
	if (_4976_5d76 == 0) {
		bp0c += 16;
	}
	U16 si;
	U8* _bp04;	// _bp04
	if (_4976_5d76 != 0) {
		if ((bp0c & 1) != 0) {
			bp0c++;
		}
		bp0c += sizeof(mement) + sizeof(i32);
		mement* bp04 = ALLOC_LOWER_CPXHEAP(bp0c);	// bp04
		if (bp16 >= 0) {
			xImage = reinterpret_cast<IMG3 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp16));
		}
		si = FIND_FREE_MEMENTI();
		tblRawDataToMement[iLocalGDatIdx] = si;
		ATLASSERT(tblMementsPointers[si] == NULL);
		tblMementsPointers[si] = bp04;
		bp04->setCacheIndex(iLocalGDatIdx);
#if DM2_EXTENDED_MODE == 1
		bp04->w12(xImage->GetBitsCount());
#else
		bp04->w12(4);
#endif
		bp04->w14(bp12);
		bp04->w16(bp14);
		bp04++; // +18 bytes
		_bp04 = reinterpret_cast<U8 *>(bp04);

		// SkD((DLV_DBG_GETPIC,"DBG: CPX Alloc #%02d (Raw #%04d)\n", si, di));
	}
	else {
		sk5cfc_image* xLinkedImage = reinterpret_cast<sk5cfc_image *>(ALLOC_MEMORY_RAM(bp0c +sizeof(sk5cfc_image), (allocUpper != 0) ? afDefault : afUseLower, 8));	// bp04
		LOGX(("Linked Image (%p) pv0 = %p", xLinkedImage, gblLinkedImageRoot.pNextImage));
		xLinkedImage->pNextImage = gblLinkedImageRoot.pNextImage;
		gblLinkedImageRoot.pNextImage = xLinkedImage;
		xLinkedImage++;	// what's for ? ...
		// xLinkedImage++ goes +14 (= sizeof(sk5cfc_image)), but from there, xLinkedImage[-1] goes to the original xLinkedImage->pv0
		LOGX(("Linked Image++ (%p) Linked Image[-1] (%p) / Linked Image pv0 = %p (sizeof(sk5cfc_image) = %X)", xLinkedImage, &xLinkedImage[-1], xLinkedImage[-1], sizeof(sk5cfc_image)));

		
		xLinkedImage[-1].iMemPool = (allocUpper != 0) ? 0 : 2;
		xLinkedImage[-1].iGDatRawDataIdx = iLocalGDatIdx;
#if DM2_EXTENDED_MODE == 1
		xLinkedImage[-1].iBpp = xImage->GetBitsCount();
#else
		xLinkedImage[-1].iBpp = IMG_4_BPP;
#endif
		xLinkedImage[-1].iWidth = bp12;
		xLinkedImage[-1].iHeight = bp14;
		
		/*
		xLinkedImage->pv0->w6 = (allocUpper != 0) ? 0 : 2;
		xLinkedImage->pv0->iGDatRawDataIdx = iLocalGDatIdx;
#if DM2_EXTENDED_MODE == 1
		xLinkedImage->pv0->iBpp = xImage->GetBitsCount();
#else
		xLinkedImage->pv0->iBpp = IMG_4_BPP;
#endif
		xLinkedImage->pv0->iWidth = bp12;
		xLinkedImage->pv0->iHeight = bp14;
		*/
// rewrite without the ++/-1
		COPY_MEMORY(	// what is this -16 ??? related to sk5cfc_image size ???
			PTR_PADD(xImage,+QUERY_GDAT_RAW_DATA_LENGTH(iLocalGDatIdx) -16),
			PTR_PADD(xLinkedImage,+bp0c -16),
			16
			);
		_bp04 = reinterpret_cast<U8 *>(xLinkedImage);
	}
	U16 bp20;
	if (bp1e != 0) {
		if (_4976_5d76 != 0) {
			bp20 = QUERY_MEMENTI_FROM(bp1c);
			bp1a = reinterpret_cast<U8 *>(&tblMementsPointers[bp20][1]); // +18 bytes
		}
		DECODE_IMG3_OVERLAY(bp1a, xImage, _bp04);
	}
	else {
#if DM2_EXTENDED_MODE == 1
		switch (xImage->Getpf()) {
			case pfC8:
				DECODE_IMG9(xImage, _bp04);
				break;
			case pfC4:
				DECODE_IMG3_UNDERLAY_LOCAL(xImage, _bp04);
				break;
		}
#else
		DECODE_IMG3_UNDERLAY_LOCAL(xImage, _bp04);
#endif
	}
	if (_4976_5d76 != 0) {
		if (allocUpper == 0) {
			RECYCLE_MEMENTI(si, 0);
		}
		if (bp16 >= 0) {
			_3e74_585a_CACHE(bp16, 0);
		}
		if (bp1e == 0)
			return _bp04;
		RECYCLE_MEMENTI(bp20, 0);
		return _bp04;
	}
	if (xShelfMem.Absent()) {
		DEALLOC_BIGPOOL_STRUCT_BEFORE(reinterpret_cast<U8 *>(xImage));
	}
	if (bp1e != 0) {
		FREE_PICT_ENTRY(bp1a);
	}
	return _bp04;
}

//^3929:0CA8
void SkWinCore::KANJI_FONT_LOAD(X8 cls2)
{
	ENTER(660);
	X16 bp0e = 0;
	X16 bp10 = 0x20;
	X16 si = 0xef;
	skxxxf bp0294[64];
	skxxxf *bp08 = bp0294;
	U8 bp0b;
	U8 *bp04;
	for (bp0b = 0; (bp04 = QUERY_GDAT_ENTRY_DATA_BUFF(GDAT_CATEGORY_x1C_JAPANESE_FONT, cls2, dtImage, bp0b)) != NULL; ) {
		X16 bp0a = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x1C_JAPANESE_FONT, cls2, dtWordValue, bp0b);
		if (bp0a != 0) {
			bp0e = bp0a >> 8;
			bp10 = U8(bp0a);
		}
		bp0a = QUERY_GDAT_PICT_OFFSET(GDAT_CATEGORY_x1C_JAPANESE_FONT, cls2, bp0b);
		bp08->b0 = X8(bp0e);
		bp08->b1 = X8(bp10);
		bp08->w6 = i8(bp0a >> 8);
		bp08->w2 = i16(READ_UI16(bp04,+0) & 0x3FF) / bp08->w6;
		bp08->w8 = i8(bp0a);
		bp08->w4 = i16(READ_UI16(bp04,+2) & 0x3FF) / bp08->w8;
		bp0b++;
		if (bp0b >= 0x40)
			continue;
		bp10 += bp08->w2 * bp08->w4;
		if (bp10 > si) {
			if (bp0e == 0) {
				bp0e = 0xEF;
				si = 0xFF;
			}
			bp0e++;
			bp10 = 0x20;
		}
		bp08++;
	}
	_4976_5bf8[cls2] = bp0b;
	U32 bp14 = bp0b * 10;
	COPY_MEMORY(
		bp0294, 
		_4976_5bfa[cls2] = reinterpret_cast<skxxxf *>(ALLOC_MEMORY_RAM(bp14, afUseUpper, 0x400)),
		bp14
		);
	return;
}


//^470A:0003
// TODO: this does nothing ?!
// SPX: _470a_0003 renamed FONT_LOAD_NOTHING
void SkWinCore::FONT_LOAD_NOTHING()
{
	ENTER(0);
	return;
}

//^3929:0E16
void SkWinCore::_3929_0e16_FONT_LOAD()
{
	ENTER(8);
	_3929_07e1(0, 0);
	_4976_5c08 = ALLOC_PICT_BUFF(_4976_013e, _4976_0140, afUseUpper, 8);
	_4976_5c0e = ALLOC_MEMORY_RAM(0x300, afUseUpper, 0x400);
	LOAD_GDAT_ENTRY_DATA_TO(GDAT_CATEGORY_x01_INTERFACE_GENERAL, 0x0, dt07, 0x0, _4976_5c0e);	// default font
	U16 iMessageNo = 0; // si
	for (iMessageNo = 0; iMessageNo < 1; iMessageNo++) {
		//tlbTimerTickRemoveHintMessage[iMessageNo] = 0xffffffff;
		tblTimerTickRemoveHintMessage[iMessageNo] = -1;
	}
	SRECT bp08;
	QUERY_EXPANDED_RECT(RECT_015_BOTTOM_MESSAGE_3_LINES, &bp08); // 00 00|B4 00|40 01|14 00 (0,180,320,20)
	_4976_5c12 = (bp08.cy - (_4976_013a - _4976_0134)) >> 1;
	KANJI_FONT_LOAD(1);
	FONT_LOAD_NOTHING();
	_4976_4750 = 1;
	return;
}
