// DM2RawContConv.cpp: CDM2RCConv クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2RawContConv.h"
#include "GDATDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define SPS_SND5SND6 (11025U)
#define SPS_SND3     (6000U)
#define SPS_SND7     (6000U)
#define SPS_SNDA     (6000U)
#define SPS_SND9     (6000U)

namespace
{

BOOL ConvWav(PWAVEFORMATEX pwfexSrc, PWAVEFORMATEX pwfexDst, DWORD nSuggest, const BYTE *pbPos, const BYTE *pbEnd, SizeBuff &fvDst)
{
	MMRESULT mmr;
	HACMSTREAM has = NULL;
	try {
		mmr = acmFormatSuggest(
			NULL,
			pwfexSrc,
			pwfexDst,
			sizeof(WAVEFORMATEX),
			nSuggest //ACM_FORMATSUGGESTF_NCHANNELS|ACM_FORMATSUGGESTF_NSAMPLESPERSEC|ACM_FORMATSUGGESTF_WBITSPERSAMPLE|ACM_FORMATSUGGESTF_WFORMATTAG
			);
		if (mmr != 0) throw mmr;
		mmr = acmStreamOpen(
			&has,
			NULL,
			pwfexSrc,
			pwfexDst,
			NULL,
			0,
			0,
			ACM_STREAMOPENF_NONREALTIME
			);
		if (mmr != 0) throw mmr;
		DWORD cbSrc = pbEnd - pbPos;
		DWORD cbDst = 0;
		mmr = acmStreamSize(
			has,
			cbSrc,
			&cbDst,
			ACM_STREAMSIZEF_SOURCE
			);
		if (mmr != 0) throw mmr;
		if (!fvDst.Alloc(cbDst)) throw MMRESULT();
		ACMSTREAMHEADER ash;
		ZeroMemory(&ash, sizeof(ash));
		ash.cbStruct = sizeof(ash);
		ash.pbSrc = const_cast<BYTE *>(pbPos);
		ash.cbSrcLength = ash.cbSrcLengthUsed = cbSrc;
		ash.pbDst = (BYTE *)fvDst.GetData();
		ash.cbDstLength = cbDst;
		mmr = acmStreamPrepareHeader(
			has,
			&ash,
			0
			);
		if (mmr != 0) throw mmr;
		try {
			mmr = acmStreamConvert(
				has,
				&ash,
				ACM_STREAMCONVERTF_START|ACM_STREAMCONVERTF_END
				);
			if (mmr != 0) throw mmr;

			fvDst.Alloc(ash.cbDstLengthUsed);

			mmr = acmStreamUnprepareHeader(
				has,
				&ash,
				0
				);
			mmr = acmStreamClose(has, 0);

			return true;
		} catch (MMRESULT mmr) {
			mmr;
			mmr = acmStreamUnprepareHeader(
				has,
				&ash,
				0
				);
		}
	} catch (MMRESULT mmr) {
		mmr;
		if (has)
			mmr = acmStreamClose(has, 0);
	}
	return false;
}

};

//////////////////////////////////////////////////////////////////////
// CDM2RCConv

bool CDM2RCConv::DecodeRIFFWave(const BYTE *pbPos, UINT cb, CDM2RCConvMallocIf &rMalloc)
{
	bool fIsSnd5Snd6 = (nGDRSDT == GDRSDT_SND5SND6);
	bool fIsSnd3 = (nGDRSDT == GDRSDT_SND3);
	bool fIsSnd7 = (nGDRSDT == GDRSDT_SND7);
	bool fIsSnd9 = (nGDRSDT == GDRSDT_SNDA);
	bool fIsSnd10 = (nGDRSDT == GDRSDT_SND9);
	if (!fIsSnd5Snd6 && !fIsSnd3 && !fIsSnd7 && !fIsSnd9 && !fIsSnd10)
		return false;

	bool fIsSigned = fIsSnd5Snd6 || fIsSnd7 || fIsSnd9;

	MemSrcIoLE io(pbPos, pbPos + cb);
	if (io.ReadDWord() != FOURCC_RIFF)
		return false;

	DWORD nLeft = io.ReadDWord();
	if (nLeft < 4)
		return false;
	if (io.ReadDWord() != mmioFOURCC('W','A','V','E'))
		return false;
	nLeft -= 4;
	const BYTE *pbFMT = 0;
	DWORD cbFMT = 0;
	const BYTE *pbDATA = 0;
	DWORD cbDATA = 0;
	while (nLeft >= 8) {
		DWORD nType = io.ReadDWord();
		DWORD nSize = io.ReadDWord();
		nLeft -= 8;
		if (nType == mmioFOURCC('f','m','t',' ')) {
			pbFMT = pbPos + io.GetPosition();
			cbFMT = nSize;
		} else if (nType == mmioFOURCC('d','a','t','a')) {
			pbDATA = pbPos + io.GetPosition();
			cbDATA = nSize;
		}
		io.SeekForward(nSize);

		if (nLeft < nSize) {
			nLeft = 0;
		} else {
			nLeft -= nSize;
		}
	}
	if (pbFMT == NULL || pbDATA == NULL)
		return false;

	PWAVEFORMATEX pwfexSrc = (PWAVEFORMATEX)pbFMT;
	WAVEFORMATEX wfexSrc;
	if (cbFMT < sizeof(WAVEFORMATEX)) {
		if (cbFMT != sizeof(PCMWAVEFORMAT))
			return false;
		ZeroMemory(&wfexSrc, sizeof(wfexSrc));
		memcpy(&wfexSrc, pbFMT, sizeof(PCMWAVEFORMAT));
		pwfexSrc = &wfexSrc;
	}

	UINT nSamplesPerSec = SPS_SND5SND6;
	switch (nGDRSDT) {
	case GDRSDT_SND3: nSamplesPerSec = SPS_SND3; break;
	case GDRSDT_SND7: nSamplesPerSec = SPS_SND7; break;
	case GDRSDT_SNDA: nSamplesPerSec = SPS_SNDA; break;
	case GDRSDT_SND9: nSamplesPerSec = SPS_SND9; break;
	}

	WAVEFORMATEX wfexDst;
	wfexDst.wFormatTag = 1;
	wfexDst.nChannels = 1;
	wfexDst.nAvgBytesPerSec = nSamplesPerSec;
	wfexDst.nSamplesPerSec = nSamplesPerSec;
	wfexDst.nBlockAlign = 1;
	wfexDst.wBitsPerSample = 8;
	wfexDst.cbSize = 0;
	WAVEFORMATEX wfexDstTest = wfexDst;

	SizeBuff fvDst;
	DWORD nMostSuggest = ACM_FORMATSUGGESTF_NCHANNELS|ACM_FORMATSUGGESTF_NSAMPLESPERSEC|ACM_FORMATSUGGESTF_WBITSPERSAMPLE|ACM_FORMATSUGGESTF_WFORMATTAG;
	if (!ConvWav(pwfexSrc, &wfexDstTest, nMostSuggest, pbDATA, pbDATA + cbDATA, fvDst)) {
		DWORD nLessSuggest = ACM_FORMATSUGGESTF_WFORMATTAG;
		SizeBuff fvDstTemp;
		if (!ConvWav(pwfexSrc, &wfexDstTest, nLessSuggest, pbDATA, pbDATA + cbDATA, fvDstTemp)) {
			return false;
		}
		BYTE *pbNewPos = (BYTE *)fvDstTemp.GetData();
		BYTE *pbNewEnd = pbNewPos + fvDstTemp.GetSize();
		if (!ConvWav(&wfexDstTest, &wfexDst, nMostSuggest, pbNewPos, pbNewEnd, fvDst)) {
			return false;
		}
	}

	{
		BYTE *pbBegin = (BYTE *)fvDst.GetData();
		BYTE *pbPos = pbBegin;
		BYTE *pbEnd = pbPos + fvDst.GetSize();
		UINT cbWav = pbEnd - pbPos;

		if (fIsSigned) for (; pbPos < pbEnd; pbPos++) *pbPos += 0x80;

		UINT nSndPrefix = fIsSnd5Snd6 ? 4 : 2;
		void *pvSnd = rMalloc.Alloc(cbWav + nSndPrefix);
		if (pvSnd) {
			BYTE *pbSnd = (BYTE *)pvSnd;
			if (fIsSnd5Snd6) {
				memcpy(pbSnd, fLE ? "\x77\x2B\x08\x01" : "\x2B\x77\x08\x01", 4);
			} else if (fIsSnd7) {
				pbSnd[0] = fLE ? LOBYTE(cbWav +2) : HIBYTE(cbWav +2);
				pbSnd[1] = fLE ? HIBYTE(cbWav +2) : LOBYTE(cbWav +2);
			} else if (fIsSnd9 || fIsSnd10) {
				pbSnd[0] = fLE ? LOBYTE(cbWav +2) : HIBYTE(cbWav +2);
				pbSnd[1] = fLE ? HIBYTE(cbWav +2) : LOBYTE(cbWav +2);
			} else {
				pbSnd[0] = fLE ? LOBYTE(cbWav) : HIBYTE(cbWav);
				pbSnd[1] = fLE ? HIBYTE(cbWav) : LOBYTE(cbWav);
			}
			memcpy(pbSnd +nSndPrefix, pbBegin, cbWav);
			return true;
		}

		return false;
	}
}

bool CDM2RCConv::EncodeRIFFWave(const BYTE *pbPos, UINT cb, CDM2RCConvMallocIf &rMalloc)
{
	bool fIsSnd5Snd6 = (nGDRSDT == GDRSDT_SND5SND6);
	bool fIsSnd3 = (nGDRSDT == GDRSDT_SND3);
	bool fIsSnd7 = (nGDRSDT == GDRSDT_SND7);
	bool fIsSnd9 = (nGDRSDT == GDRSDT_SNDA);
	bool fIsSnd10 = (nGDRSDT == GDRSDT_SND9);
	UINT nSamplesPerSec = 0;
	bool fIsSigned = false;

	if (fIsSnd5Snd6) {
		if (cb < 4)
			return false;
		if (fLE)
		{
			if (pbPos[0] != 0x77 || pbPos[1] != 0x2B || pbPos[2] != 0x08 || pbPos[3] != 0x01)
				return false;
		}
		else
		{
			if (pbPos[0] != 0x2B || pbPos[1] != 0x77 || pbPos[2] != 0x08 || pbPos[3] != 0x01)
				return false;
		}
		nSamplesPerSec = SPS_SND5SND6; fIsSigned = true;
	} else if (fIsSnd7) {
		if (cb < 2)
			return false;
		if (fLE)
			return false;
		if (256 * pbPos[0] + pbPos[1] != cb) {
			return false;
		}
		nSamplesPerSec = SPS_SND7; fIsSigned = true;
	} else if (fIsSnd9) {
		if (cb < 2)
			return false;
		if (!fLE)
			return false;
		if (pbPos[0] + 256 * pbPos[1] != cb) {
			return false;
		}
		nSamplesPerSec = SPS_SNDA; fIsSigned = true;
	} else if (fIsSnd10) {
		if (cb < 2)
			return false;
		if (!fLE)
			return false;
		if (pbPos[0] + 256 * pbPos[1] != cb) {
			return false;
		}
		nSamplesPerSec = SPS_SND9; fIsSigned = false;
	} else if (fIsSnd3) {
		if (cb < 2)
			return false;
		if (fLE) {
			if (pbPos[0] + 256 * pbPos[1] != cb - 2)
				return false;
		} else {
			if (256 * pbPos[0] + pbPos[1] != cb - 2)
				return false;
		}
		nSamplesPerSec = SPS_SND3; fIsSigned = false;
	} else {
		return false;
	}

	UINT nSndPrefix = fIsSnd5Snd6 ? 4 : 2;
	UINT nWavLen = cb - nSndPrefix;
	UINT nMaxLength = (8+4)+(8+18)+(8+4)+(8+nWavLen);
	void *pvWav;
	if (!(pvWav = rMalloc.Alloc(nMaxLength)))
		return false;
	MemDestIo io((BYTE *)pvWav, (BYTE *)pvWav + nMaxLength);
	io.WriteDWord(0x46464952U); // "RIFF"
	io.WriteDWord(nMaxLength - 8);
	io.WriteDWord(0x45564157U); // "WAVE"
	io.WriteDWord(0x20746D66U); // "fmt "
	io.WriteDWord(18); // sizeof(WAVEFORMATEX)
	io.WriteWord (1); // WAVEFORMATEX.wFormatTag
	io.WriteWord (1); // WAVEFORMATEX.nChannels
	io.WriteDWord(nSamplesPerSec); // WAVEFORMATEX.nSamplesPerSec
	io.WriteDWord(nSamplesPerSec); // WAVEFORMATEX.nAvgBytesPerSec
	io.WriteWord (1); // WAVEFORMATEX.nBlockAlign
	io.WriteWord (8); // WAVEFORMATEX.wBitsPerSample
	io.WriteWord (0); // WAVEFORMATEX.cbSize
	io.WriteDWord(0x74636166U); // "fact"
	io.WriteDWord(4);
	io.WriteDWord(nWavLen); // total sample count
	io.WriteDWord(0x61746164U); // "data"
	io.WriteDWord(nWavLen);
	if (fIsSigned) {
		for (UINT iSample = 0, nSamples = nWavLen; iSample < nSamples; iSample++)
		{
			BYTE x = pbPos[nSndPrefix + iSample];
			x = (x + 0x80);
			io.WriteByte(x);
		}
	} else {
		for (UINT iSample = 0, nSamples = nWavLen; iSample < nSamples; iSample++)
		{
			BYTE x = pbPos[nSndPrefix + iSample];
			io.WriteByte(x);
		}
	}
	return true;
}
