// EditPCMDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DM2GDED.h"
#include "EditPCMDlg.h"
#include "fm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib, "msacm32.lib")

namespace
{

BOOL ConvWav(PWAVEFORMATEX pwfexSrc, PWAVEFORMATEX pwfexDst, DWORD nSuggest, BYTE *pbPos, BYTE *pbEnd, SizeBuff &fvDst)
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
		ash.pbSrc = pbPos;
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

/////////////////////////////////////////////////////////////////////////////
// CEditPCMDlg ダイアログ


CEditPCMDlg::CEditPCMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditPCMDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditPCMDlg)
	m_strHint = _T("");
	//}}AFX_DATA_INIT

	m_pCB = NULL;
}

void CEditPCMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditPCMDlg)
	DDX_Text(pDX, IDC_EDIT_HINT, m_strHint);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEditPCMDlg, CDialog)
	//{{AFX_MSG_MAP(CEditPCMDlg)
	ON_BN_CLICKED(IDC_BUTTON_SND_PLAY, OnButtonSndPlay)
	ON_BN_CLICKED(IDC_BUTTON_SND_COPY, OnButtonSndCopy)
	ON_BN_CLICKED(IDC_BUTTON_SND_PASTE, OnButtonSndPaste)
	ON_BN_CLICKED(IDC_BUTTON_SND_FROM_FILE, OnButtonSndFromFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditPCMDlg メッセージ ハンドラ

void CEditPCMDlg::OnButtonSndPlay() 
{
	if (!m_pCB) return;
	if (!m_pCB->CreateRIFFWave(m_fvPlaySound)) return;

	PlaySound(
		(LPCTSTR)m_fvPlaySound.GetData(), 
		NULL, 
		SND_ASYNC|SND_MEMORY
		);
}

void CEditPCMDlg::OnButtonSndCopy() 
{
	HGLOBAL hMem = 0;
	if (SavePCMTo(hMem)) {
		if (OpenClipboard()) {
			EmptyClipboard();
			if (SetClipboardData(CF_WAVE, hMem))
				hMem = NULL;
			CloseClipboard();
		}
		if (hMem)
			GlobalFree(hMem);
	}
}

void CEditPCMDlg::OnButtonSndPaste() 
{
	if (OpenClipboard()) {
		HANDLE hMem = GetClipboardData(CF_WAVE);
		if (!hMem) hMem = GetClipboardData(CF_RIFF);

		if (hMem) {
			BYTE *pbWav = (BYTE *)GlobalLock(hMem);
			DWORD cbWav = GlobalSize(hMem);

			UpdateWindow();
			CWaitCursor wc;
			if (LoadRIFFWav(pbWav, pbWav + cbWav)) {
				RevertStatus();
			}
		}
		CloseClipboard();
	}
}

BOOL CEditPCMDlg::SavePCMTo(HGLOBAL &hMem)
{
	SizeBuff fvRIFFWave;
	if (m_pCB && m_pCB->CreateRIFFWave(fvRIFFWave)) {
		BYTE *pbWav = (BYTE *)fvRIFFWave.GetData();
		UINT cbWav = fvRIFFWave.GetSize();
		if (hMem = GlobalAlloc(GMEM_FIXED, cbWav)) {
			memcpy(hMem, pbWav, cbWav);
			return TRUE;
		}
	}
	return FALSE;
}

void CEditPCMDlg::RevertStatus()
{
	if (!UpdateData()) return;

	UINT cbWav = 0;
	SizeBuff fvRIFFWave;
	if (m_pCB && m_pCB->CreateRIFFWave(fvRIFFWave)) {
		cbWav = fvRIFFWave.GetSize();
	}

	m_strHint.FormatMessage(IDS_STR_FMT_PCM,
		0U + cbWav
		);

	UpdateData(FALSE);
}

BOOL CEditPCMDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	RevertStatus();
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CEditPCMDlg::OnOK() 
{
	CDialog::OnOK();
}

bool CEditPCMDlg::LoadRIFFWav(BYTE *pbPos, BYTE *pbEnd)
{
	MemSrcIoLE io(pbPos, pbEnd);
	if (io.ReadDWord() != FOURCC_RIFF)
		return false;

	DWORD nLeft = io.ReadDWord();
	if (nLeft < 4)
		return false;
	if (io.ReadDWord() != mmioFOURCC('W','A','V','E'))
		return false;
	nLeft -= 4;
	BYTE *pbFMT = 0;
	DWORD cbFMT = 0;
	BYTE *pbDATA = 0;
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

	WAVEFORMATEX wfexDst;
	wfexDst.wFormatTag = 1;
	wfexDst.nChannels = 1;
	wfexDst.nAvgBytesPerSec = 11025;
	wfexDst.nSamplesPerSec = 11025;
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
		for (; pbPos < pbEnd; pbPos++) *pbPos += 0x80;

		if (m_pCB && m_pCB->SetPCM11025(pbBegin, pbEnd - pbBegin)) {
			return true;
		}
		return false;
	}

#if 0
	HACMSTREAM has = NULL;
	try {
		mmr = acmFormatSuggest(
			NULL,
			pwfexSrc,
			&wfexDst,
			sizeof(WAVEFORMATEX),
			ACM_FORMATSUGGESTF_NCHANNELS|ACM_FORMATSUGGESTF_NSAMPLESPERSEC|ACM_FORMATSUGGESTF_WBITSPERSAMPLE|ACM_FORMATSUGGESTF_WFORMATTAG
			);
		if (mmr != 0) throw mmr;
		mmr = acmStreamOpen(
			&has,
			NULL,
			pwfexSrc,
			&wfexDst,
			NULL,
			0,
			0,
			ACM_STREAMOPENF_NONREALTIME
			);
		if (mmr != 0) throw mmr;
		DWORD cbDst = 0;
		mmr = acmStreamSize(
			has,
			cbDATA,
			&cbDst,
			ACM_STREAMSIZEF_SOURCE
			);
		if (mmr != 0) throw mmr;
		if (!fvDst.Alloc(cbDst)) throw MMRESULT();
		ACMSTREAMHEADER ash;
		ZeroMemory(&ash, sizeof(ash));
		ash.cbStruct = sizeof(ash);
		ash.pbSrc = pbDATA;
		ash.cbSrcLength = ash.cbSrcLengthUsed = cbDATA;
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

			mmr = acmStreamUnprepareHeader(
				has,
				&ash,
				0
				);
			mmr = acmStreamClose(has, 0);

			return false;
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
#endif
}

void CEditPCMDlg::OnButtonSndFromFile() 
{
	CFileDialog wndDlg(
		TRUE, 
		"wav", 
		NULL, 
		OFN_ENABLESIZING |OFN_EXPLORER |OFN_FILEMUSTEXIST |OFN_HIDEREADONLY |OFN_LONGNAMES |OFN_PATHMUSTEXIST |OFN_READONLY,
		LoadString(IDS_STR_FILTER_WAV),
		this
		);
	int r = wndDlg.DoModal();
	if (r != IDOK)
		return;
	fm fmo;
	if (fmo.Open(wndDlg.GetPathName())) {
		LPVOID pvAny = fmo.MapView();
		DWORD cbAny = fmo.GetFileSize();
		BYTE *pbPos = (BYTE *)pvAny;
		BYTE *pbEnd = pbPos + cbAny;

		UpdateWindow();
		CWaitCursor wc;
		if (LoadRIFFWav(pbPos, pbEnd)) {
			RevertStatus();
		}
		fmo.UnmapView();
		fmo.Close();
	}
}
