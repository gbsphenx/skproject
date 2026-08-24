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
	ENTER(0);
	LOADDS(0x3083);
	if (buff == NULL) {
		_01b0_18d3_AUDIO(0);
		cd.sc.sndSoundInBuffer = 0;
	}
	else if (buffSize != 0) {
		cd.sc.sndSoundBufferSize = buffSize;
		cd.sc.sndSoundBuffer = buff;
		cd.sc.sndSoundPlaybackFrequency = playbackRate;
		U16 si;
		for (si = 0; si < buffSize; si++) {
			cd.sc.sndSoundBuffer[si] = 0;
		}
        sndSoundToPlayBuffer = buff;
		sndSoundToPlayBufferSize = buffSize;
		if (cd.sc.glbSoundCardType == 3 || cd.sc.glbSoundCardType == 5 || cd.sc.glbSoundCardType == ScardSBlaster)
			cd.sc.sndSoundInBuffer = 1;
	}
	return cd.sc.sndSoundInBuffer;
}

//^47EB:0003
// SPX: _47eb_0003 renamed ALLOC_AUDIO_RAM_BUFFER
void SkWinCore::ALLOC_AUDIO_RAM_BUFFER()
{
	ENTER(0);
	glbAudioUnknown = X8(IBMIO_AUDIO_ZERO() CALL_IBMIO);
	if (IS_SCARD_PRESENT() CALL_IBMIO != 0) {
		_RELOAD_SOUND_BUFFER(
			ALLOC_MEMORY_RAM(cd.sc.glbMemRAMAudioStuff, afUseUpper, 0x400),
			cd.sc.glbMemRAMAudioStuff, C05500_SOUND_PLAYBACK_FREQUENCY
			) CALL_IBMIO;
		// SPX: with initial alloc of C05500_SOUND_PLAYBACK_FREQUENCY, that may be not enough for 6000 / 11025 / 22050 Hz samples ?!
	}
	return;
}

//^482B:0004
// SPX: _482b_0004 renamed ALLOC_AUDIO_TABLES
void SkWinCore::ALLOC_AUDIO_TABLES()
{
	cd.sc.tblSoundShelf = reinterpret_cast<SoundShelfChained *>(ALLOC_MEMORY_RAM(cd.sc.glbSndDataLoaded * sizeof(SoundShelfChained), afUseUpper, 0x400));	// << 4 (=*16) => replace with sizeof
	cd.sc.tblSoundShortInfo = reinterpret_cast<SoundShortInfo *>(ALLOC_MEMORY_RAM(cd.sc.glbGDatSoundRawDatCount * (sizeof(SoundShortInfo)), afUseUpper, 0x400));	// *7 = size of SoundShortInfo
	cd.sc._4976_5f02 = reinterpret_cast<SoundStructure *>(ALLOC_MEMORY_RAM(20*sizeof(SoundStructure), afUseUpper, 0x400));	// 0xF0 = 240 bytes for (SoundStructure = 12 bytes) => 20 items  
	U32 iMemInitVal = 0; // bp04
	cd.sc.glbSoundList = reinterpret_cast<SoundEntryInfo *>(ALLOC_MEMORY_RAM(iMemInitVal = (C08_MAX_SOUND_QUEUE_LIST*sizeof(SoundEntryInfo)), afUseUpper, 0x400));	// 0x60 = 96 bytes for (SoundEntryInfo = 12 bytes) => 8 items
	ZERO_MEMORY(cd.sc.glbSoundList, iMemInitVal);
	cd.sc._4976_5efe = reinterpret_cast<SoundStructure *>(ALLOC_MEMORY_RAM(6*sizeof(SoundStructure), afUseUpper, 0x400));	// 0x48 = 72 bytes for (SoundStructure = 12 bytes) => 6 items
	ALLOC_AUDIO_RAM_BUFFER();
	return;
}
/*
void DM2_SOUND6(void)
{
  dm2sound.sndptr4 = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(16 * dm2sound.v1e0ace, true);
  dm2sound.xsndptr2 = XUPCAST(s_ssound, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(7 * unsignedlong(dm2_dballochandler.v1e0ad6), true));
  dm2sound.xsndptr5 = XUPCAST(s_sfx, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(lcon(0xf0), true));
  dm2sound.sndptr1 = XUPCAST(s_sizee, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(lcon(0x70), true));	// 0x70 is more than 0x60 (v4), meaning the s_sizee is 14 bytes
  DM2_ZERO_MEMORY(DOWNCAST(s_sizee, dm2sound.sndptr1), lcon(0x70));
  dm2sound.xsndptr3 = XUPCAST(s_sfx, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(lcon(0x48), true));
  dm2sound.sndptr6 = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(unsignedlong(dm2_dballochandler.v1e0ad4));
}
*/


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
void SkWinCore::_01b0_18d3_AUDIO(U16 xx) //#DS=4976
{
	ENTER(2);
	U8 bp02 = cd.sc._4976_04ee;	// &bp02 is supposedly a buffer ? this looks weird as U8 and not U8*
	LOADDS(0x3083);	//#DS=04BF
	if (cd.sc.sndSoundInBuffer != 0) {
		if (cd.sc.sndLockSoundBuffer != 0) {
			cd.sc.sndLockSoundBuffer = 1;	
			while (cd.sc.sndLockSoundBuffer != 0);
			for (U16 iSndBuffIdx = 0; iSndBuffIdx < cd.sc.sndSoundBufferSize; iSndBuffIdx++) {	// si
				cd.sc.sndSoundBuffer[iSndBuffIdx] = 0;
				// why not memset for this ?
			}
		}
	}
	else {
		IBMIO_BLEND_TO_SNDBUFF(&bp02, 1, 1, C06000_SOUND_PLAYBACK_FREQUENCY);
	}
	return;
}

//^47EB:02C3
// _47eb_02c3 renamed _47eb_02c3_AUDIO
void SkWinCore::_47eb_02c3_AUDIO(SoundShelfChained *pRefChainedSound)
{
	ENTER(0);
	_01b0_18d3_AUDIO(0) CALL_IBMIO;	// not fully implemented ??
	return;
}

//^47EB:00D9
// _47eb_00d9 renamed _47eb_00d9_AUDIO
void SkWinCore::_47eb_00d9_AUDIO(SoundShelfChained *pRefChainedSound)
{
	ENTER(4);
	_47eb_02c3_AUDIO(pRefChainedSound);
	SoundShelfChained **ppChainedSound = &_4976_5efa;	// bp04
	while ((*ppChainedSound) != NULL && (*ppChainedSound) != pRefChainedSound) {
		ppChainedSound = &(*ppChainedSound)->pv8;
	}
	if ((*ppChainedSound) == pRefChainedSound) {
		(*ppChainedSound) = pRefChainedSound->pv8;
	}
	return;
}


