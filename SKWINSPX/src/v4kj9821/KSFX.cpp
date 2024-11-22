#include <StdAfx.h>	// Required for MVC6 compilation
#include <KCORE.H>
#include <SkDebug.h>
#include <SkVer.h>

#if defined(_USE_MFC80) || defined(_USE_MFC60)
#include <SkMFC.h>
#include <resource.h>
#endif // defined(_USE_MFC80) || defined(_USE_MFC60)

//^01B0:1C8D
void SkWinCore::_01b0_1c8d(Bit16u xx) //#DS=04BF
{
	//^01B0:1C8D
	//^01B0:1C91
	if (sndLockSoundBuffer == 0) {
		//^01B0:1C9B
		switch (glbSoundCardType) {
			case 3:
				//^01B0:1CAF
				glbSoundFreq_13ce = 0x16b6;	// 0x16b6 = 5814

				break;
			case 5:
				//^01B0:1CB8
				glbSoundFreq_13ce = 0x1779;	// 0x1779 = 6009
			
				break;
			case ScardSBlaster:
				//^01B0:1CC1
				glbSoundFreq_13ce = 0x170e;	// 0x170e = 5902

				break;
		}
		//^01B0:1CC8
		Bit16u bp02 = 0x001234dc / sndSoundPlaybackFrequency;
		//^01B0:1CDB
		_01b0_13c8 = bp02;
	}
	//^01B0:1CE2
	if (xx > sndLockSoundBuffer)
		//^01B0:1CEC
		sndLockSoundBuffer = xx;
	//^01B0:1CF0
	return;
}

//^01B0:1CF3
// SPX: Isn't caller a playback frequency ?
Bit16u SkWinCore::IBMIO_SBLASTER_BLEND_TO_SNDBUFF(Bit8u *buff, Bit16u buffSize, i8 volume, Bit16u caller) //#DS=04BF
{
#if UseAltic
	skwin.SndPlayHi(buff, buffSize, volume);
	return 1;
#else
	//^01B0:1CF3
	//^01B0:1CF9
	Bit16u bp02 = 0;
	//^01B0:1CFE
	if (sndSoundInBuffer != 0 && glbSoundBlasterBasePort != 0) {
		//^01B0:1D12
		if (Bit32u(sndSoundPlaybackFrequency * buffSize) / caller <= sndSoundBufferSize) {
			//^01B0:1D36
			volume >>= 1;
			//^01B0:1D3B
			Bit8u *di = sndSoundToPlayBuffer;
			Bit16u bx = _01b0_14dd;
			Bit16u dx = sndSoundToPlayBufferSize;
			Bit8u *si = buff;
			//^01B0:1D51
			Bit16u bp06;
			if (glbFreqComparator != caller) {
				//^01B0:1D5A
				Bit16u bp04 = buffSize;
				bp06 = 0;
				//^01B0:1D65
				if (glbFreqComparator > caller) {
					//^01B0:1D95
					if (!SkCodeParam::bUsePowerDebug) // SPX bypass
					ATLASSERT(false);
				}
				else {
					//^01B0:1DCB
					if (!SkCodeParam::bUsePowerDebug) // SPX bypass
					ATLASSERT(false);
				}
			}
			else {
				//^01B0:1D6A
				Bit16u cx = buffSize;
				//^01B0:1D6D
				bp06 = cx;

				do {
					//^01B0:1D70
					i16 val = i16(i8(di[bx])) + (((i16(*(si++)) * i16(volume)) << 1) >> 8);

					if (val < -128)
						val = -128;
					if (val > 127)
						val = 127;

					di[bx] = (Bit8u)val;

					//^01B0:1D87
					bx++;
					//^01B0:1D88
					if (bx >= dx) {
						bx = 0;
					}
					//^01B0:1D8F
				} while (--cx != 0);
			}
			//^01B0:1DFF
			//^01B0:1E01
			_01b0_1c8d(bp06);
			//^01B0:1E09
			bp02 = 1;
		}
	}
	//^01B0:1E0E
	return bp02;
#endif
}

//^01B0:1E15
Bit16u SkWinCore::IBMIO_BLEND_TO_SNDBUFF(Bit8u *buff, Bit16u buffSize, Bit8u volume, Bit16u caller) //#DS=04BF
{
	// it blends your wave data to sndbuff.

	//^01B0:1E15
	//^01B0:1E1B
	Bit8u di = volume;
	Bit16u si = caller;
	//^01B0:1E21
	LOADDS(0x3083);
	//^01B0:1E27
	Bit16u bp02 = 0;
	//^01B0:1E2C
	if (sndSoundInBuffer != 0) {
		//^01B0:1E33
		if (glbSoundCardType == 3 || glbSoundCardType == 5 || glbSoundCardType == ScardSBlaster) {
			//^01B0:1E48
			bp02 = IBMIO_SBLASTER_BLEND_TO_SNDBUFF(buff, buffSize, di, si);
		}
	}
	else {
		//^01B0:1E5F
		switch (glbSoundCardType) {
			case ScardTandy:
				//^01B0:1E71
				BLEND_TO_SNDBUFF_TANDY(buff, buffSize, di, si);
				//^01B0:1E80
				break;
			case 3:
				//^01B0:1E82
				BLEND_TO_SNDBUFF_GENERAL(buff, buffSize, di, si, 3, 0);

				break;
			case 4:
				//^01B0:1E88
				BLEND_TO_SNDBUFF_SNDCARD_4(buff, buffSize, di, si);
				//^01B0:1E9A
				break;
			case 5:
				//^01B0:1E9C
				BLEND_TO_SNDBUFF_GENERAL(buff, buffSize, di, si, 5, 1);

				break;
			case ScardSBlaster:
				//^01B0:1EA2
				BLEND_TO_SNDBUFF_GENERAL(buff, buffSize, di, si, 6, 0);

				break;
			case 7:
				//^01B0:1EA8
				BLEND_TO_SNDBUFF_GENERAL(buff, buffSize, di, si, 7, 1);

				break;
		}
	}
	//^01B0:1EBE
	return bp02;
}

//^47EB:0333
void SkWinCore::_47eb_0333(SoundStructure *ref)
{
	//^47EB:0333
	//^47EB:0339
	Bit16u bp08 = 0;
	//^47EB:033E
	i16 si = ref->b6;
	i16 di = ref->b7;
	//^47EB:034F
	U8 *bp04 = &ref->b8;	// volume
	//^47EB:035E
	bp04[0] = (i8)(((ref->b5 << 8) / (si * si + di * di + 8)) >> 8);
	//^47EB:0388
	Bit8u cl;
	if (si == 0) {
		//^47EB:038C
		cl = 8;
	}
	//^47EB:0390
	else if (di == 0) {
		//^47EB:0394
		if (si < 0) {
			//^47EB:0398
			cl = 15;
		}
		else {
			//^47EB:039C
			cl = 1;
		}
	}
	else {
		//^47EB:03A0
		if (si < 0) {
			//^47EB:03A4
			si = -si;
			bp08 = 1;
		}
		//^47EB:03AF
		if (di < 0) {
			//^47EB:03B3
			di = -di;
		}
		//^47EB:03B9
		Bit16u bp06 = (si << 11) / di;
		cl = 0;

		//^47EB:03C7
		while (_4976_49c0[cl++] > bp06);
	}

	//^47EB:03DA
	if (bp08 != 0) {
		//^47EB:03E0
		bp04[1] = 16 - cl;
		bp04[2] = cl;
	}
	else {
		//^47EB:03EF
		bp04[1] = cl;
		bp04[2] = 16 - cl;
	}
	//^47EB:03FC
	//^47EB:0400
	return;
}

//^47EB:014F
// SPX: _47eb_014f renamed PLAY_SOUND
Bit16u SkWinCore::PLAY_SOUND(Bit16u xx, SoundStructure *ref)
{
	SkD((DLV_DBG_SND_OUT, "SND: Play sound xx:%02X struct:%04X\n"
		, U16(xx), ref));

	//^47EB:014F
	//^47EB:0155
	if (xx == 0)
		//^47EB:015B
		return 0;
	//^47EB:0160
	Bit16u bp02 = 0;
	SoundStructure *bp06 = ref;
	//^47EB:0171
	for (; bp02 < xx; bp02++, bp06++) {
		//^47EB:0173
		_47eb_0333(bp06);
		//^47EB:0180
		bp06->b11 = (Bit8u)bp02;
		//^47EB:018A
	}

	Bit16u bp10 = 0;
	do {
		//^47EB:0199
		bp06 = ref;
		//^47EB:01AA
		Bit16u bp0c = bp06->b11;
		//^47EB:01B6
		for (bp06++, bp02++; bp02 < xx; bp02++, bp06++) {
			//^47EB:01C1
			Bit16u bp0e = bp06->b11;
			//^47EB:01CD
			if (_47eb_02e0(&ref[bp0c], &ref[bp0e]) == 0) {
				//^47EB:01F8
				WRITE_UI8(bp06,-1,(Bit8u)bp0e);
				//^47EB:0202
				bp06->b11 = (Bit8u)bp0c;
				//^47EB:0209
				bp10 = 1;
			}
			else {
				//^47EB:0210
				bp0c = bp0e;
			}
			//^47EB:0216
		}
		//^47EB:0225
	} while (bp10 != 0);
	//^47EB:022E
	U16 si = 0;
	U16 di = 0;
	//^47EB:0232
	while (di < _4976_49be) {
		while (si < xx) {
			//^47EB:0234
			Bit16u bp0c = ref[si].b11;
			si++;
			//^47EB:024A
			bp06 = &ref[bp0c];
			//^47EB:025D
			Bit8u *bp0a = &bp06->b8;
#if UseAltic
			skwin.SndPlayLo(_47eb_0048(bp06->pv0->ps0, 0), bp06->pv0->w4, bp06->b6, bp06->b7);
#else
			//^47EB:026C
			if (IBMIO_BLEND_TO_SNDBUFF(_47eb_0048(bp06->pv0->ps0, 0), bp06->pv0->w4, *bp0a, PLAYBACK_FREQUENCY) CALL_IBMIO == 0)
			//^47EB:02A7
				return si;
#endif
		}
		//^47EB:02AE
		di++;
	}
	//^47EB:02B5
	return si;
}

//^482B:0101
Bit16u SkWinCore::QUERY_SND_ENTRY_INDEX(Bit8u cls1, Bit8u cls2, Bit8u cls4)
{
	//^482B:0101
	//^482B:0105
	for (Bit16u si=0; si < _4976_49d4[0][0]; si++) {
		//^482B:0109
		if (_4976_5f06[si].category == cls1) {
			//^482B:011F
			if (_4976_5f06[si].index == cls2) {
				//^482B:0135
				if (_4976_5f06[si].entry == cls4) {
					//^482B:014B
					return si +1;
				}
			}
		}
		//^482B:0150
	}
	//^482B:0157
	return 0;
}

//^482B:018B
void SkWinCore::QUEUE_NOISE_GEN1(Bit8u cls1, Bit8u cls2, Bit8u cls4, Bit8u xx, Bit8u yy, i16 xpos, i16 ypos, i16 tickDelta)
{
	//^482B:018B
	//^482B:0191
	if (tickDelta > 0 && glbCurrentMapIndex != glbMap_4c28 && glbCurrentMapIndex != glbMap_4976_4c12) {
		//^482B:01A6
		return;
	}
	//^482B:01A9
	if (_4976_49d0 == 20) {
		//^482B:01B0
		return;
	}
	//^482B:01B3
	Bit16u bp0e = QUERY_SND_ENTRY_INDEX(cls1, cls2, cls4);
	//^482B:01C9
	if (bp0e == 0) {
		//^482B:01CD
		return;
	}
	//^482B:01D0
	if (glbIsPlayerSleeping != 0) {
		//^482B:01D7
		yy >>= 1;
	}
	//^482B:01DA
	if (tickDelta > 1) {
		//^482B:01E3
		Bit16u bp10 = 0;
		//^482B:01E8
		i16 si;
		for (si = 0; si < 8; si++) {
			//^482B:01EC
			if (glbSoundList[si].w0 == 0) {
				//^482B:01FF
				bp10 = 1;
				//^482B:0204
				break;
			}
			//^482B:0206
		}
		//^482B:020C
		if (bp10 == 0) {
			//^482B:0212
			return;
		}
		//^482B:0215
		SoundEntryInfo *snd = &glbSoundList[si];	//*bp04
		//^482B:022C
		snd->w0 = 1;
		snd->category = cls1;
		snd->index = cls2;
		snd->entry = cls4;
		snd->w8 = xx;
		snd->w10 = yy;
		snd->b5 = (Bit8u)glbCurrentMapIndex;
		snd->b6 = (Bit8u)xpos;
		snd->b7 = (Bit8u)ypos;
		//^482B:026C
		Timer bp1a;
		bp1a.SetMap(glbCurrentMapIndex);
		bp1a.SetTick(glbGameTick +tickDelta -1);
		bp1a.TimerType(ttySound);
		bp1a.actor = xx;
		bp1a.value = si;
		//^482B:02A8
		QUEUE_TIMER(&bp1a);
		//^482B:02B4
		return;
	}
	//^482B:02B7
	sk5f0a *bp08 = &_4976_5f0a[_4976_5f06[bp0e -1].w0];
	//^482B:02DC
	if (tickDelta > 0) {
		//^482B:02E2
		xpos += dunMapLocalHeader->MapOffsetX() - dunMapsHeaders[glbPlayerMap].MapOffsetX() - glbPlayerPosX;
		//^482B:0308
		ypos += dunMapLocalHeader->MapOffsetY() - dunMapsHeaders[glbPlayerMap].MapOffsetY() - glbPlayerPosY;
	}
	else {
		//^482B:0330
		xpos -= glbPlayerPosX;
		ypos -= glbPlayerPosY;
	}
	//^482B:033C
	i16 si;
	switch (glbPlayerDir) {
		case 1:
			//^482B:0350
			si = xpos;
			xpos = ypos;
            //^482B:035B
			ypos = -si;
			//^482B:0360
			break;
		case 2:
			//^482B:0362
			xpos = -xpos;
			//^482B:036D
			//^482B:035B
			ypos = -ypos;
			//^482B:0360
			break;
		case 3:
			//^482B:036F
			si = xpos;
			xpos = -ypos;
			ypos = si;

			break;
	}
	//^482B:037D
	SoundStructure *bp0c;
	i16 di;
	if (tickDelta < 0) {
		//^482B:0383
		if (_4976_49d2 == 6)
			//^482B:038A
			return;
		//^482B:038D
		bp0c = _4976_5efe;
		di = _4976_49d2;
	}
	else {
		//^482B:03A0
		bp0c = _4976_5f02;
		di = _4976_49d0;
	}
	//^482B:03B1
	for (si=0; si < di; si++) {
		//^482B:03B5
		if (bp0c[si].pv0 != bp08) {
			//^482B:03D2
			if (bp08->pv0 != bp0c[si].pv0)
				//^482B:03F8
				continue;
		}
        //^482B:03FA
		if (bp0c[si].b6 == xpos && bp0c[si].b7 == ypos)
			//^482B:0426
			return;

		//^482B:0429
	}
	//^482B:042E
	bp0c[di].pv0 = &_4976_5f0a[_4976_5f06[bp0e -1].w0];
	//^482B:0467
    bp0c[di].b4 = xx;
	bp0c[di].b5 = yy;
	bp0c[di].b6 = (i8)xpos;
	bp0c[di].b7 = (i8)ypos;
	//^482B:04B3
	if (tickDelta == 0) {
		//^482B:04B9
		PLAY_SOUND(1, &bp0c[di]);
	}
	//^482B:04D5
	else if (tickDelta > 0) {
		//^482B:04DB
		_4976_49d0++;
	}
	else {
		//^482B:04E1
		_4976_49d2++;
	}
	//^482B:04E5
	return;
}

//^482B:00BF
//void SkWinCore::QUEUE_NOISE_GEN2(Bit8u cls1, Bit8u cls2, Bit8u cls4, Bit8u cls2alt, i16 xpos, i16 ypos, Bit16u tickDelta, Bit8u ss, Bit8u tt)
void SkWinCore::QUEUE_NOISE_GEN2(Bit8u cls1, Bit8u cls2, Bit8u iSoundID, Bit8u cls2alt, i16 xpos, i16 ypos, Bit16u tickDelta, Bit8u ss, Bit8u tt)
{
	//^482B:00BF
	//^482B:00C2
	// SPX: In the case of sound at cls2 is not found, use the default with cls2alt
	QUEUE_NOISE_GEN1(
		cls1,
		(QUERY_SND_ENTRY_INDEX(cls1, cls2, iSoundID) != 0) ? cls2 : cls2alt,
		iSoundID,
		ss,
		tt,
		xpos,
		ypos,
		tickDelta
		);
	//^482B:00FF
	//SkD((DLV_BUGHERE,"Player sound (%02X,%02X,%02X) alt (%02X) to (%02d,%02d) / Delta = %02d / ss/tt = %02X,%02X\n"
	//	, (Bitu)cls1, (Bitu)cls2, (Bitu)cls4, (Bitu)cls2alt, xpos, ypos, tickDelta, ss, tt) );

	return;		
}

//^482B:05BF
void SkWinCore::_482b_05bf(Bit16u xx)
{
	//^482B:05BF
	ENTER(0);
	//^482B:05C2
	if (xx != 0) {
		if (_4976_49d2 > 0) {
			PLAY_SOUND(_4976_49d2, _4976_5efe);
		}
		_4976_49d2 = 0;
		return;
	}
	if (_4976_49d0 > 0) {
		PLAY_SOUND(_4976_49d0, _4976_5f02);
	}
	//^482B:0606
	_4976_49d0 = 0;
	//^482B:060C
	return;
}

//^01B0:1997
// SPX: _01b0_1997 renamed BLEND_TO_SNDBUFF_TANDY
void SkWinCore::BLEND_TO_SNDBUFF_TANDY(void *ref, Bit16u xx, Bit16u yy, Bit16u zz) { // TODO: Unr
	Unr();
}

void SkWinCore::_01b0_0ec3(X16 xx) { // TODO: Unr
	Unr();
}

//^01B0:1A6D
// SPX: _01b0_1a6d renamed BLEND_TO_SNDBUFF_GENERAL
void SkWinCore::BLEND_TO_SNDBUFF_GENERAL(U8 *buff, Bit16u buffSize, Bit16u volume, Bit16u caller, Bit16u ss, Bit16u tt)
{
	//^01B0:1A6D
	ENTER(6);
	//^01B0:1A73
	X16 si = volume;
	U16 di = caller;
	if (si == 0)
		return;
	//^01B0:1A80
	if (di > 0x1f40 && tt != 0) {
		//^01B0:1A8C
		di >>= 1;
		buffSize >>= 1;
		_01b0_14d9 = 1;
	}
	else {
		//^01B0:1AA3
		_01b0_14d9 = 0;
	}
	//^01B0:1AAA
	if (glbSoundBlasterBasePort == 0)
		return;
	//^01B0:1AB4
	if (_04bf_04f1 != 0) {
		//^01B0:1ABC
		if (_04bf_04f3 != 0) {
			//^01B0:1AC3
			U16 bp02;
			for (bp02 = 0; bp02 < 0x100; bp02++) {
				//^01B0:1ACA
				X8 bp05 = (ss == 7)
					? _04bf_03d6[RCJ(220,(bp02 * 220) >> 8)]
					: ((((bp02 +0x80) * si) >> 8) +0x80)
					;
				_01b0_13d8[bp02] = bp05;
				//^01B0:1B00
			}
		}
		//^01B0:1B0A
		_04bf_04f1 =  si;
		if (ss == 7) {
			//^01B0:1B14
			_01b0_0ec3(si);
            _04bf_04f3 = 0;
		}
	}
	//^01B0:1B20
	bool jne = (sndLockSoundBuffer != 0);
	sndSoundToPlayBuffer = buff;
	sndLockSoundBuffer = buffSize;
	if (!jne) {
		//^01B0:1B3F
		U16 bp04;
		switch (ss) {
		case 5://^1B51	// Sound card type 5
			//^01B0:1B51
			glbSoundFreq_13ce = 0x1551;	// 0x1551 = 5457 (*2 = 10914)
			goto _1b7b;
		case 3://^1B5A	// Sound card type 3
			//^01B0:1B5A
			glbSoundFreq_13ce = 0x1624;	// 0x1624 = 5668 (*2 = 11336)
			goto _1b7b;
		case 7://^1B63	// Sound card type 7
			//^01B0:1B63
			outportb(_01b0_14db, 0x40);
			glbSoundFreq_13ce = 0x15e9;
			goto _1b7b;
		case ScardSBlaster://^1B74	// Sound card type 6
			//^01B0:1B74
			glbSoundFreq_13ce = 0x159e;	// 0x159e = 5534 (*2 = 11068)
			//^01B0:1B7B
_1b7b:
			bp04 = 0x001234dc / di;
			_01b0_13c8 = bp04;
		case 4://^1B92
			break;
		}
	}
	//^01B0:1B92
	//^01B0:1B93
	return;
}

//^01B0:1BA1
// SPX: _01b0_1ba1 renamed BLEND_TO_SNDBUFF_SNDCARD_4
void SkWinCore::BLEND_TO_SNDBUFF_SNDCARD_4(void *ref, Bit16u xx, Bit16u yy, Bit16u zz) { // TODO: Unr
	Unr();
}

//^47EB:02E0
Bit16u SkWinCore::_47eb_02e0(SoundStructure *xx, SoundStructure *yy)
{
	//^47EB:02E0
	ENTER(0);
	//^47EB:02E4
	U16 si;
	if (xx->b4 > yy->b4)
		//^47EB:02F4
		return si = 1;
	//^47EB:02F9
	if (yy->b4 == yy->b4) {
		//^47EB:0309
		U8 cl = xx->b8;
		U8 dl = yy->b8;
		if (cl >= dl)
			return si = 1;
		return si = 0;
	}
	//^47EB:032C
	return si = 0;
}