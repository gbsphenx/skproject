//------------------------------------------------------------------------------
// SK-AUDIO
//------------------------------------------------------------------------------

#include <skcore.h>

//^01B0:1ED2
// _01b0_1ed2 renamed IS_SCARD_PRESENT
X16 SkWinCore::IS_SCARD_PRESENT() //#DS=04BF
{
	ENTER(0);
	LOADDS(0x3083);
	return (cd.sc.glbSoundCardType != 0) ? 1 : 0;
}

//^01B0:1BE8
// SPX: _01b0_1be8 renamed _RELOAD_SOUND_BUFFER
X16 SkWinCore::_RELOAD_SOUND_BUFFER(U8 *buff, X16 buffSize, X16 playbackRate) //#DS=04BF
{
	//^01B0:1BE8
	ENTER(0);
	//^01B0:1BEC
	LOADDS(0x3083);
	if (buff == NULL) {
		//^01B0:1C03
		_01b0_18d3_AUDIO(0);
		cd.sc.sndSoundInBuffer = 0;
	}
	else if (buffSize != 0) {
		//^01B0:1C1C
		cd.sc.sndSoundBufferSize = buffSize;
		cd.sc.sndSoundBuffer = buff;
		cd.sc.sndSoundPlaybackFrequency = playbackRate;
		U16 si;
		for (si = 0; si < buffSize; si++) {
			//^01B0:1C39
			cd.sc.sndSoundBuffer[si] = 0;
			//^01B0:1C51
		}
		//^01B0:1C57
        sndSoundToPlayBuffer = buff;
		sndSoundToPlayBufferSize = buffSize;
		if (cd.sc.glbSoundCardType == 3 || cd.sc.glbSoundCardType == 5 || cd.sc.glbSoundCardType == ScardSBlaster)
			//^01B0:1C80
			cd.sc.sndSoundInBuffer = 1;
	}
	//^01B0:1C86
	return cd.sc.sndSoundInBuffer;
}

//^47EB:0003
void SkWinCore::_47eb_0003()
{
	//^47EB:0003
	ENTER(0);
	//^47EB:0006
	_4976_4838 = X8(_01b0_2b1b() CALL_IBMIO);
	if (IS_SCARD_PRESENT() CALL_IBMIO != 0) {
		//^47EB:001F
		_RELOAD_SOUND_BUFFER(
			ALLOC_MEMORY_RAM(_4976_5c7c, afUseUpper, 0x400),
			_4976_5c7c, PLAYBACK_FREQUENCY
			) CALL_IBMIO;
	}
	//^47EB:0046
	return;
}

//^482B:0004
void SkWinCore::_482b_0004()
{
	//^482B:0004
	ENTER(4);
	//^482B:0008
	_4976_5f0a = reinterpret_cast<sk5f0a *>(ALLOC_MEMORY_RAM(_4976_5cae << 4, afUseUpper, 0x400));
	_4976_5f06 = reinterpret_cast<sk5f06 *>(ALLOC_MEMORY_RAM(_4976_5d58 * 7, afUseUpper, 0x400));
	_4976_5f02 = reinterpret_cast<SoundStructure *>(ALLOC_MEMORY_RAM(0xf0, afUseUpper, 0x400));
	U32 bp04;
	glbSoundList = reinterpret_cast<SoundEntryInfo *>(ALLOC_MEMORY_RAM(bp04 = 0x60, afUseUpper, 0x400));
	ZERO_MEMORY(glbSoundList, bp04);
	_4976_5efe = reinterpret_cast<SoundStructure *>(ALLOC_MEMORY_RAM(0x48, afUseUpper, 0x400));
	_47eb_0003();
	//^482B:00BD
	return;
}

//^01B0:1983
// _01b0_1983 renamed _01b0_1983_AUDIO
void SkWinCore::_01b0_1983_AUDIO() //#DS=04BF
{
	ENTER(0);
	LOADDS(0x3083);
	while (IS_SCARD_BUSY() != 0);
	return;
}

//^47EB:02D3
// _47eb_02d3 renamed _47eb_02d3_AUDIO
void SkWinCore::_47eb_02d3_AUDIO(U32 aa)
{
	ENTER(0);
	_01b0_1983_AUDIO();
	return;
}

//^482B:060E
// _482b_060e renamed _482b_060e_AUDIO
void SkWinCore::_482b_060e_AUDIO()
{
	ENTER(0);
	_47eb_02d3_AUDIO(0);
	_4976_49d0 = 0;
	return;
}



//^01B0:18D3
// _01b0_18d3 renamed _01b0_18d3_AUDIO
void SkWinCore::_01b0_18d3_AUDIO(Bit16u xx) //#DS=4976
{
	//^01B0:18D3
	ENTER(2);
	//^01B0:18D8
	Bit8u bp02 = _4976_04ee;
	//^01B0:18E9
	LOADDS(0x3083);	//#DS=04BF
	//^01B0:18EF
	if (cd.sc.sndSoundInBuffer != 0) {
		//^01B0:18F6
		if (sndLockSoundBuffer != 0) {
			//^01B0:18FE
			sndLockSoundBuffer = 1;	
			//^01B0:1908
			while (sndLockSoundBuffer != 0);
			//^01B0:1910
			for (Bit16u si = 0; si < cd.sc.sndSoundBufferSize; si++) {
				//^01B0:1914
				cd.sc.sndSoundBuffer[si] = 0;
				//^01B0:192C
			}
		}
		//^01B0:1933
	}
	else {
		//^01B0:1935
		IBMIO_BLEND_TO_SNDBUFF(&bp02, 1, 1, 6000);
	}
	//^01B0:1949
	return;
}

//^47EB:02C3
// _47eb_02c3 renamed _47eb_02c3_AUDIO
void SkWinCore::_47eb_02c3_AUDIO(sk5f0a *ref)
{
	ENTER(0);
	_01b0_18d3_AUDIO(0) CALL_IBMIO;
	return;
}

//^47EB:00D9
// _47eb_00d9 renamed _47eb_00d9_AUDIO
void SkWinCore::_47eb_00d9_AUDIO(sk5f0a *ref)
{
	//^47EB:00D9
	ENTER(4);
	//^47EB:00DD
	_47eb_02c3_AUDIO(ref);
	//^47EB:00EA
	sk5f0a **bp04 = &_4976_5efa;
	//^47EB:00F2
	while ((*bp04) != NULL && (*bp04) != ref) {
		//^47EB:00F4
		bp04 = &(*bp04)->pv8;
		//^47EB:0107
	}
	//^47EB:0124
	if ((*bp04) == ref) {
		//^47EB:0138
		(*bp04) = ref->pv8;
	}
	//^47EB:014D
	return;
}


