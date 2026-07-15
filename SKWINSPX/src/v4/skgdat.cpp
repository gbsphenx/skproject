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
			return reinterpret_cast<U8 *>(&xMemEnt[1]); // +18 bytes
		}
	}
	else {
		sk5cfc_image *bp04 = _4976_5cfc.pv0;
		while (bp04 != NULL) {
			if (bp04->iGDatRawDataIdx == iLocalGDatIdx) {
				return (U8 *)&bp04[1]; // +14 bytes
			}
			bp04 = bp04->pv0;
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
	shelf_memory bp10 = glbShelfMemoryTable[iLocalGDatIdx];
	IMG3* xImage;	// bp08
	if (bp10.Absent()) {
		xImage = reinterpret_cast<IMG3 *>(QUERY_GDAT_DYN_BUFF(iLocalGDatIdx, reinterpret_cast<U16 *>(&bp16), (_4976_5d76 != 0) ? 1 : (!allocUpper)));
	}
	else {
		xImage = reinterpret_cast<IMG3 *>(REALIZE_GRAPHICS_DATA_MEMORY(bp10));
	}
	if (xImage->OffsetY() == -32) { // uncompressed 4bpp/8bpp. but 8bpp is untested!
		if (bp16 < 0) {
			U8* bp04 = MEMENT_3e74_0245(iLocalGDatIdx, allocUpper);
			return bp04;
		}
		else {
			U8* bp04 = PTR_PADD(xImage,+10);
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
		mement *bp04 = ALLOC_LOWER_CPXHEAP(bp0c);
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
		sk5cfc_image *bp04 = reinterpret_cast<sk5cfc_image *>(ALLOC_MEMORY_RAM(bp0c +sizeof(sk5cfc_image), (allocUpper != 0) ? afDefault : afUseLower, 8));
		bp04->pv0 = _4976_5cfc.pv0;
		_4976_5cfc.pv0 = bp04;
		bp04++;
		bp04[-1].w6 = (allocUpper != 0) ? 0 : 2;
		bp04[-1].iGDatRawDataIdx = iLocalGDatIdx;
#if DM2_EXTENDED_MODE == 1
		bp04[-1].w8 = xImage->GetBitsCount();
#else
		bp04[-1].w8 = IMG_4_BPP;
#endif
		bp04[-1].width = bp12;
		bp04[-1].height = bp14;
		COPY_MEMORY(
			PTR_PADD(xImage,+QUERY_GDAT_RAW_DATA_LENGTH(iLocalGDatIdx) -16),
			PTR_PADD(bp04,+bp0c -16),
			16
			);
		_bp04 = reinterpret_cast<U8 *>(bp04);
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
	if (bp10.Absent()) {
		DEALLOC_BIGPOOL_STRUCT_BEFORE(reinterpret_cast<U8 *>(xImage));
	}
	if (bp1e != 0) {
		FREE_PICT_ENTRY(bp1a);
	}
	return _bp04;
}
