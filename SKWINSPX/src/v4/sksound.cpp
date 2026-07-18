//#include <StdAfx.h>	// Required for MVC6 compilation
#include <skglobal.h>
#include <skdebug.h>
#include <skver.h>
#include <skcore.h>

//#if defined(_USE_MFC80) || defined(_USE_MFC60)
//#include <SKMFC.h>
//#include <resource.h>
//#endif // defined(_USE_MFC80) || defined(_USE_MFC60)

//^01B0:1C8D
void SkWinCore::_01b0_1c8d(U16 xx) //#DS=04BF
{
	if (sndLockSoundBuffer == 0) {
		switch (cd.sc.glbSoundCardType) {
			case Scard03:
				glbSoundFreq_13ce = 0x16b6;	// 0x16b6 = 5814

				break;
			case Scard05:
				glbSoundFreq_13ce = 0x1779;	// 0x1779 = 6009
			
				break;
			case ScardSBlaster:
				glbSoundFreq_13ce = 0x170e;	// 0x170e = 5902

				break;
		}
		U16 bp02 = 0x001234dc / cd.sc.sndSoundPlaybackFrequency;
		_01b0_13c8 = bp02;
	}
	if (xx > sndLockSoundBuffer)
		sndLockSoundBuffer = xx;
	return;
}

//^01B0:1CF3
// SPX: Isn't caller a playback frequency ?
U16 SkWinCore::IBMIO_SBLASTER_BLEND_TO_SNDBUFF(Bit8u *buff, U16 buffSize, i8 volume, U16 caller) //#DS=04BF
{
#if UseAltic
	//skwin.SndPlayHi(buff, buffSize, volume);
	skWinApp->skwin_SndPlayHi(buff, buffSize, volume);
	return 1;
#else
	U16 bp02 = 0;
	if (sndSoundInBuffer != 0 && glbSoundBlasterBasePort != 0) {
		if (Bit32u(sndSoundPlaybackFrequency * buffSize) / caller <= sndSoundBufferSize) {
			volume >>= 1;
			Bit8u *di = sndSoundToPlayBuffer;
			U16 bx = _01b0_14dd;
			U16 dx = sndSoundToPlayBufferSize;
			Bit8u *si = buff;
			U16 bp06;
			if (glbFreqComparator != caller) {
				U16 bp04 = buffSize;
				bp06 = 0;
				if (glbFreqComparator > caller) {
					if (!SkCodeParam::bUsePowerDebug) // SPX bypass
					ATLASSERT(false);
				}
				else {
					if (!SkCodeParam::bUsePowerDebug) // SPX bypass
					ATLASSERT(false);
				}
			}
			else {
				U16 cx = buffSize;
				bp06 = cx;

				do {
					i16 val = i16(i8(di[bx])) + (((i16(*(si++)) * i16(volume)) << 1) >> 8);

					if (val < -128)
						val = -128;
					if (val > 127)
						val = 127;

					di[bx] = (Bit8u)val;

					bx++;
					if (bx >= dx) {
						bx = 0;
					}
				} while (--cx != 0);
			}
			_01b0_1c8d(bp06);
			bp02 = 1;
		}
	}
	return bp02;
#endif
}

//^01B0:1E15
U16 SkWinCore::IBMIO_BLEND_TO_SNDBUFF(X8* buff, U16 buffSize, U8 iVolume, U16 iFrequency) //#DS=04BF
{
	// it blends your wave data to sndbuff.

	//U8 di = volume;
	//U16 si = caller; iFrequency
	LOADDS(0x3083);
	U16 bp02 = 0;
	if (cd.sc.sndSoundInBuffer != 0) {
		if (cd.sc.glbSoundCardType == Scard03 || cd.sc.glbSoundCardType == Scard05 || cd.sc.glbSoundCardType == ScardSBlaster) {
			bp02 = IBMIO_SBLASTER_BLEND_TO_SNDBUFF(buff, buffSize, iVolume, iFrequency);
		}
	}
	else {
		switch (cd.sc.glbSoundCardType) {
			case ScardTandy:
				BLEND_TO_SNDBUFF_TANDY(buff, buffSize, iVolume, iFrequency);
				break;
			case Scard03:
				BLEND_TO_SNDBUFF_GENERAL(buff, buffSize, iVolume, iFrequency, (U16)Scard03, 0);	// 3
				break;
			case Scard04:
				BLEND_TO_SNDBUFF_SNDCARD_4(buff, buffSize, iVolume, iFrequency);
				break;
			case Scard05:
				BLEND_TO_SNDBUFF_GENERAL(buff, buffSize, iVolume, iFrequency, (U16)Scard05, 1);	// 5
				break;
			case ScardSBlaster:
				BLEND_TO_SNDBUFF_GENERAL(buff, buffSize, iVolume, iFrequency, (U16)ScardSBlaster, 0);	// 6
				break;
			case Scard07:
				BLEND_TO_SNDBUFF_GENERAL(buff, buffSize, iVolume, iFrequency, (U16)Scard07, 1);	// 7
				break;
		}
	}
	return bp02;
}

//^47EB:0333
void SkWinCore::_47eb_0333(SoundStructure *ref)
{
	U16 bp08 = 0;
	i16 si = ref->b6;
	i16 di = ref->b7;
	U8 *bp04 = &ref->b8;	// volume
	bp04[0] = (i8)(((ref->b5 << 8) / (si * si + di * di + 8)) >> 8);
	Bit8u cl;
	if (si == 0) {
		cl = 8;
	}
	else if (di == 0) {
		if (si < 0) {
			cl = 15;
		}
		else {
			cl = 1;
		}
	}
	else {
		if (si < 0) {
			si = -si;
			bp08 = 1;
		}
		if (di < 0) {
			di = -di;
		}
		U16 bp06 = (si << 11) / di;
		cl = 0;

		while (_4976_49c0[cl++] > bp06);
	}

	if (bp08 != 0) {
		bp04[1] = 16 - cl;
		bp04[2] = cl;
	}
	else {
		bp04[1] = cl;
		bp04[2] = 16 - cl;
	}
	return;
}

//^47EB:014F
// SPX: _47eb_014f renamed PLAY_SOUND
U16 SkWinCore::PLAY_SOUND(U16 xx, SoundStructure *ref)
{
	SkD((DLV_DBG_SND_OUT, "SND: Play sound xx:%02X struct:%04X\n"
		, U16(xx), ref));

	if (xx == 0)
		return 0;
	U16 bp02 = 0;
	SoundStructure* xSoundStruct = ref;	// bp06
	for (; bp02 < xx; bp02++, xSoundStruct++) {
		_47eb_0333(xSoundStruct);
		xSoundStruct->b11 = (Bit8u)bp02;
	}

	U16 bp10 = 0;
	do {
		xSoundStruct = ref;
		U16 bp0c = xSoundStruct->b11;
		for (xSoundStruct++, bp02++; bp02 < xx; bp02++, xSoundStruct++) {
			U16 bp0e = xSoundStruct->b11;
			if (_47eb_02e0(&ref[bp0c], &ref[bp0e]) == 0) {
				WRITE_UI8(xSoundStruct,-1,(Bit8u)bp0e);
				xSoundStruct->b11 = (Bit8u)bp0c;
				bp10 = 1;
			}
			else {
				bp0c = bp0e;
			}
		}
	} while (bp10 != 0);
	U16 si = 0;
	U16 di = 0;
	while (di < _4976_49be) {
		while (si < xx) {
			U16 bp0c = ref[si].b11;
			si++;
			xSoundStruct = &ref[bp0c];
			Bit8u *bp0a = &xSoundStruct->b8;
#if UseAltic
			//skwin.SndPlayLo(_47eb_0048(xSoundStruct->pv0->ps0, 0), xSoundStruct->pv0->w4, xSoundStruct->b6, xSoundStruct->b7);
			skWinApp->skwin_SndPlayLo(_47eb_0048(xSoundStruct->pv0->ps0, 0), xSoundStruct->pv0->w4, xSoundStruct->b6, xSoundStruct->b7);
#else
			if (IBMIO_BLEND_TO_SNDBUFF(_47eb_0048(xSoundStruct->pv0->ps0, 0), xSoundStruct->pv0->w4, *bp0a, PLAYBACK_FREQUENCY) CALL_IBMIO == 0)
				return si;
#endif
		}
		di++;
	}
	return si;
}

//^482B:0101
U16 SkWinCore::QUERY_SND_ENTRY_INDEX(Bit8u cls1, Bit8u cls2, Bit8u cls4)
{
	U16 iSoundEntry = 0;
	for (iSoundEntry = 0; iSoundEntry < _4976_49d4[0][0]; iSoundEntry++) {
		if (tblSoundShortInfo[iSoundEntry].category == cls1) {
			if (tblSoundShortInfo[iSoundEntry].index == cls2) {
				if (tblSoundShortInfo[iSoundEntry].entry == cls4) {
					return iSoundEntry + 1;
				}
			}
		}
	}
	return 0;
}

//^482B:018B
void SkWinCore::QUEUE_NOISE_GEN1(Bit8u cls1, Bit8u cls2, Bit8u cls4, Bit8u xx, Bit8u yy, i16 xpos, i16 ypos, i16 tickDelta)
{
	if (tickDelta > 0 && glbCurrentMapIndex != glbMap_4c28 && glbCurrentMapIndex != glbMap_4976_4c12) {
		return;
	}
	if (_4976_49d0 == 20) {
		return;
	}
	U16 bp0e = QUERY_SND_ENTRY_INDEX(cls1, cls2, cls4);
	if (bp0e == 0) {
		return;
	}
	if (cd.pi.glbIsPlayerSleeping != 0) {
		yy >>= 1;
	}
	if (tickDelta > 1) {
		U16 bp10 = 0;
		i16 si;
		for (si = 0; si < 8; si++) {
			if (glbSoundList[si].w0 == 0) {
				bp10 = 1;
				break;
			}
		}
		if (bp10 == 0) {
			return;
		}
		SoundEntryInfo *snd = &glbSoundList[si];	//*bp04
		snd->w0 = 1;
		snd->category = cls1;
		snd->index = cls2;
		snd->entry = cls4;
		snd->w8 = xx;
		snd->w10 = yy;
		snd->b5 = (Bit8u)glbCurrentMapIndex;
		snd->b6 = (Bit8u)xpos;
		snd->b7 = (Bit8u)ypos;
		Timer bp1a;
		bp1a.SetMap(glbCurrentMapIndex);
		bp1a.SetTick(glbGameTick +tickDelta -1);
		bp1a.TimerType(ttySound);
		bp1a.actor = xx;
		bp1a.value = si;
		QUEUE_TIMER(&bp1a);
		return;
	}
	sk5f0a *bp08 = &_4976_5f0a[tblSoundShortInfo[bp0e -1].w0];
	if (tickDelta > 0) {
		xpos += dunMapLocalHeader->MapOffsetX() - dunMapsHeaders[cd.pi.glbPlayerMap].MapOffsetX() - cd.pi.glbPlayerPosX;
		ypos += dunMapLocalHeader->MapOffsetY() - dunMapsHeaders[cd.pi.glbPlayerMap].MapOffsetY() - cd.pi.glbPlayerPosY;
	}
	else {
		xpos -= cd.pi.glbPlayerPosX;
		ypos -= cd.pi.glbPlayerPosY;
	}
	i16 si;
	switch (cd.pi.glbPlayerDir) {
		case 1:
			si = xpos;
			xpos = ypos;
			ypos = -si;
			break;
		case 2:
			xpos = -xpos;
			ypos = -ypos;
			break;
		case 3:
			si = xpos;
			xpos = -ypos;
			ypos = si;

			break;
	}
	SoundStructure *bp0c;
	i16 di;
	if (tickDelta < 0) {
		if (_4976_49d2 == 6)
			return;
		bp0c = _4976_5efe;
		di = _4976_49d2;
	}
	else {
		bp0c = _4976_5f02;
		di = _4976_49d0;
	}
	for (si=0; si < di; si++) {
		if (bp0c[si].pv0 != bp08) {
			if (bp08->pv0 != bp0c[si].pv0)
				continue;
		}
		if (bp0c[si].b6 == xpos && bp0c[si].b7 == ypos)
			return;

	}
	bp0c[di].pv0 = &_4976_5f0a[tblSoundShortInfo[bp0e -1].w0];
    bp0c[di].b4 = xx;
	bp0c[di].b5 = yy;
	bp0c[di].b6 = (i8)xpos;
	bp0c[di].b7 = (i8)ypos;
	if (tickDelta == 0) {
		PLAY_SOUND(1, &bp0c[di]);
	}
	else if (tickDelta > 0) {
		_4976_49d0++;
	}
	else {
		_4976_49d2++;
	}
	return;
}

//^482B:00BF
//void SkWinCore::QUEUE_NOISE_GEN2(Bit8u cls1, Bit8u cls2, Bit8u cls4, Bit8u cls2alt, i16 xpos, i16 ypos, U16 tickDelta, Bit8u ss, Bit8u tt)
void SkWinCore::QUEUE_NOISE_GEN2(Bit8u iGDatCategory, Bit8u iGDatItemId, Bit8u iSoundID, Bit8u iGDatItemIdAlt, i16 xpos, i16 ypos, U16 tickDelta, Bit8u ss, Bit8u tt)
{
	// SPX: In the case of sound at cls2 is not found, use the default with cls2alt
	QUEUE_NOISE_GEN1(
		iGDatCategory,
		(QUERY_SND_ENTRY_INDEX(iGDatCategory, iGDatItemId, iSoundID) != 0) ? iGDatItemId : iGDatItemIdAlt,
		iSoundID,
		ss,
		tt,
		xpos,
		ypos,
		tickDelta
		);
	//SkD((DLV_BUGHERE,"Player sound (%02X,%02X,%02X) alt (%02X) to (%02d,%02d) / Delta = %02d / ss/tt = %02X,%02X\n"
	//	, (Bitu)iGDatCategory, (Bitu)iGDatItemId, (Bitu)cls4, (Bitu)iGDatItemIdAlt, xpos, ypos, tickDelta, ss, tt) );
	return;		
}

//^482B:05BF
// SOUND_482b_05bf is called with 0 or 1
void SkWinCore::SOUND_482b_05bf(U16 xx)
{
	ENTER(0);
	if (xx != 0) {	// xx == 1
		if (_4976_49d2 > 0) {
			PLAY_SOUND(_4976_49d2, _4976_5efe);
		}
		_4976_49d2 = 0;
		return;
	}
	if (_4976_49d0 > 0) {
		PLAY_SOUND(_4976_49d0, _4976_5f02);
	}
	_4976_49d0 = 0;
	return;
}

//^01B0:1997
// SPX: _01b0_1997 renamed BLEND_TO_SNDBUFF_TANDY
void SkWinCore::BLEND_TO_SNDBUFF_TANDY(void *ref, U16 xx, U16 yy, U16 zz) { // TODO: Unr
	Unr();
}

void SkWinCore::_01b0_0ec3(X16 xx) { // TODO: Unr
	Unr();
}

//^01B0:1A6D
// SPX: _01b0_1a6d renamed BLEND_TO_SNDBUFF_GENERAL
void SkWinCore::BLEND_TO_SNDBUFF_GENERAL(U8 *buff, U16 buffSize, U16 iVolume, U16 iFrequency, U16 iSoundCard, U16 tt)
{
	ENTER(6);
	//X16 si = volume;
	//U16 di = caller;
	if (iVolume == 0)
		return;
	if (iFrequency > 0x1f40 && tt != 0) {
		iFrequency >>= 1;
		buffSize >>= 1;
		_01b0_14d9 = 1;
	}
	else {
		_01b0_14d9 = 0;
	}
	if (cd.sc.glbSoundBlasterBasePort == 0)
		return;
	if (cd.sc._04bf_04f1 != 0) {
		if (_04bf_04f3 != 0) {
			U16 bp02;
			for (bp02 = 0; bp02 < 0x100; bp02++) {
				X8 bp05 = (iSoundCard == Scard07)
					? _04bf_03d6[RCJ(220,(bp02 * 220) >> 8)]
					: ((((bp02 +0x80) * iVolume) >> 8) +0x80)
					;
				_01b0_13d8[bp02] = bp05;
			}
		}
		cd.sc._04bf_04f1 =  iVolume;
		if (iSoundCard == Scard07) {
			_01b0_0ec3(iVolume);
            _04bf_04f3 = 0;
		}
	}
	bool jne = (sndLockSoundBuffer != 0);
	sndSoundToPlayBuffer = buff;
	sndLockSoundBuffer = buffSize;
	if (!jne) {
		U16 bp04;
		switch (iSoundCard) {
		case Scard05://^1B51	// Sound card type 5
			glbSoundFreq_13ce = 0x1551;	// 0x1551 = 5457 (*2 = 10914)
			goto _1b7b;
		case Scard03://^1B5A	// Sound card type 3
			glbSoundFreq_13ce = 0x1624;	// 0x1624 = 5668 (*2 = 11336)
			goto _1b7b;
		case Scard07://^1B63	// Sound card type 7
			outportb(_01b0_14db, 0x40);
			glbSoundFreq_13ce = 0x15e9;
			goto _1b7b;
		case ScardSBlaster://^1B74	// Sound card type 6
			glbSoundFreq_13ce = 0x159e;	// 0x159e = 5534 (*2 = 11068)
_1b7b:
			bp04 = 0x001234dc / iFrequency;
			_01b0_13c8 = bp04;
		case 4://^1B92
			break;
		}
	}
	return;
}

//^01B0:1BA1
// SPX: _01b0_1ba1 renamed BLEND_TO_SNDBUFF_SNDCARD_4
void SkWinCore::BLEND_TO_SNDBUFF_SNDCARD_4(void *ref, U16 xx, U16 yy, U16 zz) { // TODO: Unr
	Unr();
}

//^47EB:02E0
U16 SkWinCore::_47eb_02e0(SoundStructure *xx, SoundStructure *yy)
{
	ENTER(0);
	U16 si;
	if (xx->b4 > yy->b4)
		return si = 1;
	if (yy->b4 == yy->b4) {
		U8 cl = xx->b8;
		U8 dl = yy->b8;
		if (cl >= dl)
			return si = 1;
		return si = 0;
	}
	return si = 0;
}


//^482B:015C
// SPX: looks like searching next available index in the _4976_49d4 table ?
// SPX: _482b_015c renamed AUDIO_482b_015c
U16 SkWinCore::AUDIO_482b_015c(U16 iGDatRawIndex)
{
	ENTER(0);
	for (U16 iSndIdx = 0; iSndIdx < _4976_49d4[0][0]; iSndIdx++) {	// si
		if (tblSoundShortInfo[iSndIdx].iGDatRawIndex == iGDatRawIndex) {
			return iSndIdx + 1;
		}
	}
	return 0;
}



//^482B:07C2
// SPX: _482b_07c2 renamed AUDIO_482b_07c2
void SkWinCore::AUDIO_482b_07c2(U16 xx)
{
	ENTER(0);
	U16 iSndIdx;	// si
	for (iSndIdx = 0; _4976_49d4[0][0]-- > iSndIdx; ) {
		U16 iDatRawIdx = tblSoundShortInfo[_4976_49d4[0][0]].iGDatRawIndex;	// di
		if (iDatRawIdx != 0xFFFF) {
			if (AUDIO_482b_015c(iDatRawIdx) == 0) {
				_47eb_00d9_AUDIO(&_4976_5f0a[tblSoundShortInfo[_4976_49d4[0][0]].w0]);
				_4976_49d4[0][1]--;
			}
		}
	}
	_4976_49d4[0][0] = iSndIdx;
	_4976_49d0 = 0;
	return;
}


//^482B:0684
// SPX: _482b_0684 renamed AUDIO_482b_0684
void  SkWinCore::AUDIO_482b_0684()
{
	ENTER(6);
	for (U16 iSndIdx = 0; iSndIdx < _4976_49d4[0][0]; iSndIdx++) {	// si
		SoundShortInfo* xSoundInfo = &tblSoundShortInfo[iSndIdx];	// bp04
		if (xSoundInfo->iGDatRawIndex != 0xFFFF)
			continue;
		X16 iRawDatIdx =  QUERY_GDAT_ENTRY_DATA_INDEX(xSoundInfo->category, xSoundInfo->index, dtSnd, xSoundInfo->entry);	// di
		X16 bp06 = AUDIO_482b_015c(iRawDatIdx);
		if (bp06 != 0) {
			xSoundInfo->w0 = tblSoundShortInfo[bp06 -1].w0;
			xSoundInfo->iGDatRawIndex = iRawDatIdx;
			continue;
		}
		if (_4976_49d4[0][1] >= _4976_5cae)
			break;
		xSoundInfo->iGDatRawIndex = iRawDatIdx;
		xSoundInfo->w0 = _4976_49d4[0][1];
		_4976_5f0a[xSoundInfo->w0].ps0 = GET_SHELFMEM_FROM_GDAT_INDEX(xSoundInfo->category, xSoundInfo->index, dtSnd, xSoundInfo->entry) + (2);
		_4976_5f0a[xSoundInfo->w0].w4 = QUERY_GDAT_ENTRY_DATA_LENGTH(xSoundInfo->category, xSoundInfo->index, dtSnd, xSoundInfo->entry) -2;
		_4976_5f0a[xSoundInfo->w0].w6 = SOUND_FREQUENCY_5500;	// 5500
		_47eb_00a4(&_4976_5f0a[xSoundInfo->w0]);
		_4976_49d4[0][1]++;
	}
	return;
}