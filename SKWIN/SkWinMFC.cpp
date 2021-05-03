// SkWin.cpp : 実装ファイル
//

#include "StdAfx.h"
#include "DME.h"
#include "SkWinMFC.h"
#include <afxmt.h>
#include <math.h>
#include "resource.h"
#include "AboutDlg.h"

#include <SkCodeParam.h>

#define _skwin "skwin"
#define _skwin_ini "SkWin.ini"

#ifdef _USE_MFC60
CComModule _Module;
#endif

// CSkApp

int CSkApp::ML()
{
#if (_MFC_VER) > 0x0600
	ASSERT_VALID(this);
	_AFX_THREAD_STATE* pState = AfxGetThreadState();

	// for tracking the idle time state
	BOOL bIdle = TRUE;
	LONG lIdleCount = 0;

	// acquire and dispatch messages until a WM_QUIT message is received.
	{
		// phase1: check to see if we can do idle work
		while (bIdle &&
			!::PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE))
		{
			// call OnIdle while in bIdle state
			if (!OnIdle(lIdleCount++))
				bIdle = FALSE; // assume "no idle" state
		}

		// phase2: pump messages while available
		do
		{
			MSG wm;
			if (!PeekMessage(&wm, NULL, 0, 0, PM_NOREMOVE))
				break;

			// pump message, but quit on WM_QUIT
			if (!PumpMessage())
				return ExitInstance();

			// reset "no idle" state after pumping "normal" message
			//if (IsIdleMessage(&m_msgCur))
			if (IsIdleMessage(&(pState->m_msgCur)))
			{
				bIdle = TRUE;
				lIdleCount = 0;
			}

		} while (::PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE));
	}

	return 0;
#else
	ASSERT_VALID(this);
	_AFX_THREAD_STATE* pState = AfxGetThreadState();

	// for tracking the idle time state
	BOOL bIdle = TRUE;
	LONG lIdleCount = 0;

	// acquire and dispatch messages until a WM_QUIT message is received.
	{
		// phase1: check to see if we can do idle work
		while (bIdle &&
			!::PeekMessage(&(AfxGetThread()->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE))
		{
			// call OnIdle while in bIdle state
			if (!OnIdle(lIdleCount++))
				bIdle = FALSE; // assume "no idle" state
		}

		// phase2: pump messages while available
		do
		{
			MSG wm;
			if (!PeekMessage(&wm, NULL, 0, 0, PM_NOREMOVE))
				break;

			// pump message, but quit on WM_QUIT
			if (!PumpMessage())
				return ExitInstance();

			// reset "no idle" state after pumping "normal" message
			//if (IsIdleMessage(&m_msgCur))
			if (IsIdleMessage(&(AfxGetThread()->m_msgCur)))
			{
				bIdle = TRUE;
				lIdleCount = 0;
			}

		} while (::PeekMessage(&(AfxGetThread()->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE));
	}

	return 0;
#endif
}

CSkApp theApp;

CString GetSkwinini() {
	TCHAR tc[2048] = {_T("")};
	GetModuleFileName(GetModuleHandle(NULL), tc, 2048);
	CString str = tc;
	int r = str.ReverseFind('\\');
	if (r < 0)	str.Empty();
	else		str = str.Left(r +1);
	str += _skwin_ini;
	return str;
}

// CSkWinMFC

CSkWinMFC::CSkWinMFC()
{
	GetSkwinini() = GetSkwinini();

	sxfact = GetPrivateProfileInt(_skwin, "sxfact", 1, GetSkwinini());
	sblast = GetPrivateProfileInt(_skwin, "sblast", 1, GetSkwinini());
	ptfrm.x = GetPrivateProfileInt(_skwin, "framex", 0, GetSkwinini());
	ptfrm.y = GetPrivateProfileInt(_skwin, "framey", 0, GetSkwinini());
	sbdsbf = GetPrivateProfileInt(_skwin, "sbdsbf", 0, GetSkwinini());
	sbdsaf = GetPrivateProfileInt(_skwin, "sbdsaf", 0, GetSkwinini());
	spfact = GetPrivateProfileInt(_skwin, "spfact", 2, GetSkwinini());
	lang = GetPrivateProfileInt(_skwin, "lang", 0x10, GetSkwinini());
	dung = GetPrivateProfileInt(_skwin, "dung", 5, GetSkwinini());
	gdat_vers = GetPrivateProfileInt(_skwin, "gdat_vers", 0, GetSkwinini());
}

CSkWinMFC::~CSkWinMFC()
{

}

#define IdItemFirst ID_VIDEO_1X
#define IdItemLast ID_Dummy

BEGIN_MESSAGE_MAP(CSkWinMFC, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
#ifdef _USE_MFC60
	ON_COMMAND_RANGE(IdItemFirst, IdItemLast, CSkWinMFC::OnVideo1x)
	ON_UPDATE_COMMAND_UI_RANGE(IdItemFirst, IdItemLast, CSkWinMFC::OnUpdateVideo1x)
#else
	ON_COMMAND_RANGE(IdItemFirst, IdItemLast, &CSkWinMFC::OnVideo1x)
	ON_UPDATE_COMMAND_UI_RANGE(IdItemFirst, IdItemLast, &CSkWinMFC::OnUpdateVideo1x)
#endif
	ON_WM_MOVE()
END_MESSAGE_MAP()

// CSkWinMFC メッセージ ハンドラ

extern U8 vram[65536];
extern U8 _04bf_09f0[256][3];

BYTE vrbi[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];

BYTE tbl_gammac[64];

BYTE Gammac(BYTE v) {
	return tbl_gammac[v];
}

void CSkWinMFC::OnPaint()
{
	CPaintDC dc(this);
	PBITMAPINFO pbi = reinterpret_cast<PBITMAPINFO>(vrbi);
	for (int x=0; x<256; x++) {
		pbi->bmiColors[x].rgbRed   = Gammac(_04bf_09f0[x][0]);
		pbi->bmiColors[x].rgbGreen = Gammac(_04bf_09f0[x][1]);
		pbi->bmiColors[x].rgbBlue  = Gammac(_04bf_09f0[x][2]);
		pbi->bmiColors[x].rgbReserved = 0;
	}
	if (sxfact == 1) {
		SetDIBitsToDevice(dc, 0, 0, 320, 200, 0, 0, 0, 200, vram, pbi, DIB_RGB_COLORS);
	}
	else {
		StretchDIBits(dc, 0, 0, 320*sxfact, 200*sxfact, 0, 0, 320, 200, vram, pbi, DIB_RGB_COLORS, SRCCOPY);
	}
}

bool CSkWinMFC::ML() {
	if (m_hWnd == NULL || !IsWindow(m_hWnd))
		return false;
	theApp.ML();
	return true;
}

// SPX: added to handle options from command line
void CSkWinMFC::ProcessArgs(int argc, char** argv)
{
	unsigned int iArgIndex = 0;
	while (iArgIndex < argc)
	{
		if (argv[iArgIndex] == NULL)
			break;

		if (!strcmp(argv[iArgIndex], "-en"))
			lang = 0x10;	// english
		else if (!strcmp(argv[iArgIndex], "-en"))
			lang = 0x10;	// english
		else if (!strcmp(argv[iArgIndex], "-jp"))
			lang = 0x20;	// japanese
		else if (!strcmp(argv[iArgIndex], "-de"))
			lang = 0x30;	// german
		else if (!strcmp(argv[iArgIndex], "-fr"))
			lang = 0x40;	// french
		else if (!strcmp(argv[iArgIndex], "-es"))
			lang = 0x50;	// spanish
		else if (!strcmp(argv[iArgIndex], "-it"))
			lang = 0x60;	// italian

		else if (!strcmp(argv[iArgIndex], "-vx"))
			gdat_vers = 0;
		else if (!strcmp(argv[iArgIndex], "-v1"))
			gdat_vers = 1;
		else if (!strcmp(argv[iArgIndex], "-v2"))
			gdat_vers = 2;
		else if (!strcmp(argv[iArgIndex], "-v3"))
			gdat_vers = 3;
		else if (!strcmp(argv[iArgIndex], "-v4"))
			gdat_vers = 4;
		else if (!strcmp(argv[iArgIndex], "-v5"))
			gdat_vers = 5;
		else if (!strcmp(argv[iArgIndex], "-cartoon"))
			gdat_vers = 6;	// v5 + cartoon

		else if (!strcmp(argv[iArgIndex], "-dm1"))
			dung = 1;
		else if (!strcmp(argv[iArgIndex], "-csb"))
			dung = 2;
//		else if (!strcmp(argv[iArgIndex], "-tq"))
//			dung = 3;
		else if (!strcmp(argv[iArgIndex], "-beta"))
			dung = 4;
		else if (!strcmp(argv[iArgIndex], "-dm2"))
			dung = 5;

		iArgIndex++;
	}
}

int CSkWinMFC::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	PBITMAPINFO pbi = reinterpret_cast<PBITMAPINFO>(vrbi);
	pbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbi->bmiHeader.biWidth = 320;
	pbi->bmiHeader.biHeight = -200;
	pbi->bmiHeader.biBitCount = 8;
	pbi->bmiHeader.biCompression = BI_RGB;
	pbi->bmiHeader.biSizeImage = 320*200;
	pbi->bmiHeader.biPlanes = 1;
	pbi->bmiHeader.biXPelsPerMeter = pbi->bmiHeader.biYPelsPerMeter = 0;
	pbi->bmiHeader.biClrImportant = pbi->bmiHeader.biClrUsed = 256;

	//for (int x=0; x<64; x++) tbl_gammac[x] = BYTE(pow((double)x / 63.0, 0.5) * 255.0);
	for (int x=0; x<64; x++) tbl_gammac[x] = BYTE((double)x / 63.0 * 255.0);

	ZeroMemory(xMiceInput, sizeof(xMiceInput));
	cntMiceIn = 0;

	selectSBlast();

	return 0;
}

void CSkWinMFC::OnDestroy()
{
	CFrameWnd::OnDestroy();
}

void CSkWinMFC::processMinput(UINT nFlags, CPoint point)
{
	point.x /= sxfact;
	point.y /= sxfact;

	CSkMinput *p = allocMinput();
	if (p != NULL) {
		p->btn = 0
			| (((nFlags & MK_LBUTTON) != 0) ? 1 : 0)
			| (((nFlags & MK_RBUTTON) != 0) ? 2 : 0)
			;
		p->x = (U16)point.x;
		p->y = (U16)point.y;
		// TRACE3("Mice(%d,%3d,%3d)\n", p->btn, p->x, p->y);
	}
}

void CSkWinMFC::OnMouseMove(UINT nFlags, CPoint point)
{
	processMinput(nFlags, point);

	CFrameWnd::OnMouseMove(nFlags, point);
}

void CSkWinMFC::OnLButtonDown(UINT nFlags, CPoint point)
{
	processMinput(nFlags, point);

	CFrameWnd::OnLButtonDown(nFlags, point);
}

void CSkWinMFC::OnLButtonUp(UINT nFlags, CPoint point)
{
	processMinput(nFlags, point);

	CFrameWnd::OnLButtonUp(nFlags, point);
}

void CSkWinMFC::OnRButtonDown(UINT nFlags, CPoint point)
{
	processMinput(nFlags, point);

	CFrameWnd::OnRButtonDown(nFlags, point);
}

void CSkWinMFC::OnRButtonUp(UINT nFlags, CPoint point)
{
	processMinput(nFlags, point);

	CFrameWnd::OnRButtonUp(nFlags, point);
}

BOOL CSkWinMFC::OnEraseBkgnd(CDC* pDC)
{
	const int cxvw = 320 * sxfact;
	const int cyvw = 320 * sxfact;

	int a = pDC->SaveDC();
	pDC->ExcludeClipRect(0, 0, cxvw, cyvw);
	BOOL r = CFrameWnd::OnEraseBkgnd(pDC);
	pDC->RestoreDC(a);
	return r;
}

void CSkWinMFC::UpdateRect(i16 x, i16 y, i16 cx, i16 cy)
{
	if (!IsWindow(*this)) return;

	x *= sxfact;
	y *= sxfact;
	cx *= sxfact;
	cy *= sxfact;
	RedrawWindow(CRect(x, y, x +cx, y +cy), NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE);
}

void CSkWinMFC::SndPlayLo(const U8 *buff, U32 buffSize, i8 dX, i8 dY)
{
	sblast_sys::PlayBuff pb;
	pb.pbBuff = (BYTE *)buff;
	pb.cbBuff = buffSize;
	pb.dX = dX;
	pb.dY = dY;
	pb.af = sbdsaf;
	pb.bf = sbdsbf;

	switch (sblast) {
		case 1:
			{
				__int8 vol = 16 -(__int8)(sqrt(float(dX*dX +dY*dY)) * 3);
				if (vol < 1) break;
				SndPlayHi(buff, buffSize, vol);
				break;
			}
		case 2:
			{
				sbds.Play(pb);
				break;
			}
		case 3:
			{
				sboa.Play(pb);
				break;
			}
	}
}

// SPX: called in "Windows Multimedia" sound mode
void CSkWinMFC::SndPlayHi(const U8 *buff, U32 buffSize, i8 vol)
{
	U32 nSamples = buffSize;
	
	// SPX: added this small block to have random frequencies
	int iBasePlaybackFrequency = SKWIN_PLAYBACK_FREQUENCY;	// 6000
	int iPlaybackFrequency = iBasePlaybackFrequency;	
	if (SkCodeParam::bUseVaryingPlaybackFrequency)
	{
		int iRandomValue = rand()%500 - 250;
		iPlaybackFrequency += iRandomValue;
	}
	// SPX

#pragma pack(push, 1)

	struct {
		DWORD riff;
		DWORD n1;
		DWORD wave;
		DWORD fmt;
		DWORD n2;

		WORD wFormatTag, nChannels;
		DWORD nSamplesPerSec, nAvgBytesPerSec;
		WORD nBlockAlign, wBitsPerSample, cbSize;

		DWORD fact;
		DWORD n3;
		DWORD n4;

		DWORD data;
		DWORD n5;
	}	z = {
		*(DWORD *)"RIFF",
		4+(8)+(2+2+4+4+2+2+2)+(8)+(4)+(8)+(nSamples),
		*(DWORD *)"WAVE",
		*(DWORD *)"fmt ",
		(2+2+4+4+2+2+2),
		1,
		1,
		iPlaybackFrequency,
		iPlaybackFrequency,
		1,
		8,
		0,
		*(DWORD *)"fact",
		4,
		nSamples,
		*(DWORD *)"data",
		nSamples,
	};

#pragma pack(pop)

	static int cnt = 0;
	static BYTE buffInt[65536];

	if (cnt != 0) PlaySound(NULL, NULL, SND_PURGE);
	CMemFile f;
	f.Write(&z, sizeof(z));
	i8 c;
	for (U32 x=0; x<buffSize; x++) {
		c = buff[x];
		c = max(-127, min(127, i16(c) * vol / 16));
		c += (i8)0x80;
		f.Write(&c, 1);
	}
	f.SeekToBegin();
	f.Read(buffInt, 65536);
	PlaySound((LPCSTR)buffInt, NULL, SND_ASYNC|SND_MEMORY);
	cnt |= 1;
}

void CSkWinMFC::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	processKinput(nChar, true);

	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSkWinMFC::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	processKinput(nChar, false);

	CFrameWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CSkWinMFC::processKinput(UINT nChar, bool press)
{
	CSkKinput *p = allocKinput();
	if (p != NULL) {
		BYTE v = 0;
		switch (nChar) {
		case VK_ESCAPE: v = 1; break;
		case '1': v = 2; break;
		case '2': v = 3; break;
		case '3': v = 4; break;
		case '4': v = 5; break;
		case '5': v = 6; break;
		case '6': v = 7; break;
		case '7': v = 8; break;
		case '8': v = 9; break;
		case '9': v = 10; break;

		case VK_BACK: v = 14; break;
		case VK_TAB: v = 15; break;

		case VK_RETURN: v = 28; break;

		case VK_NUMPAD7: v = 71; break;
		case VK_NUMPAD8: v = 72; break;
		case VK_NUMPAD9: v = 73; break;
		case VK_NUMPAD4: v = 75; break;
		case VK_NUMPAD5: v = 76; break;
		case VK_NUMPAD6: v = 77; break;
		case VK_NUMPAD1: v = 79; break;
		case VK_NUMPAD2: v = 80; break;
		case VK_NUMPAD3: v = 81; break;

		case 'Q': v = 16; break;
		case 'W': v = 17; break;
		case 'E': v = 18; break;
		case 'R': v = 19; break;
		case 'T': v = 20; break;
		case 'Y': v = 21; break;
		case 'U': v = 22; break;
		case 'I': v = 23; break;
		case 'O': v = 24; break;
		case 'P': v = 25; break;

		case 'A': v = 30; break;
		case 'S': v = 31; break;
		case 'D': v = 32; break;
		case 'F': v = 33; break;
		case 'G': v = 34; break;
		case 'H': v = 35; break;
		case 'J': v = 36; break;
		case 'K': v = 37; break;
		case 'L': v = 38; break;

		case 'Z': v = 44; break;
		case 'X': v = 45; break;
		case 'C': v = 46; break;
		case 'V': v = 47; break;
		case 'B': v = 48; break;
		case 'N': v = 49; break;
		case 'M': v = 50; break;

		case VK_SPACE: v = 57; break;
		}
		p->raw = (press) ? (v) : (v | 0x80);
	}
}

void CSkWinMFC::OnVideo1x(UINT nID) {
	int sxfact = -1;
	int sblast = -1;
	int sbdsbf = -1;
	int sbdsaf = -1;
	int spfact = -1;
	int lang = -1;
	int dung = -1;
	int gdat_vers = -1;
	switch (nID) {
		case ID_VIDEO_1X: sxfact = 1; break;
		case ID_VIDEO_2X: sxfact = 2; break;
		case ID_VIDEO_3X: sxfact = 3; break;
		case ID_VIDEO_4X: sxfact = 4; break;

		case ID_SOUND_NO: sblast = 0; break;
		case ID_SOUND_WINMM: sblast = 1; break;
		case ID_SOUND_DS: sblast = 2; break;
		case ID_SOUND_OPENAL: sblast = 3; break;
		case ID_DS_BF_0: sbdsbf = 0; break;
		case ID_DS_BF_HARD: sbdsbf = 1; break;
		case ID_DS_BF_SOFT: sbdsbf = 2; break;
		case ID_DS_AF_0: sbdsaf = 0; break;
		case ID_DS_AF_FULL: sbdsaf = 1; break;
		case ID_DS_AF_LIGHT: sbdsaf = 2; break;

		case ID_MIDI_MUSIC_ENABLED: SkCodeParam::bMIDIMusicEnabled = !SkCodeParam::bMIDIMusicEnabled; break;

		case ID_SPEED_1ULTRAFAST: spfact = 0; break;
		case ID_SPEED_2FAST: spfact = 1; break;
		case ID_SPEED_3NORMAL: spfact = 2; break;
		case ID_SPEED_4SLOW: spfact = 3; break;
		case ID_SPEED_5VERYSLOW: spfact = 4; break;
		case ID_SPEED_6GLACIAL: spfact = 5; break;

		case ID_LANG_ENGLISH: lang = 0x10; break;
		case ID_LANG_JAPANESE: lang = 0x20; break;
		case ID_LANG_GERMAN: lang = 0x30; break;
		case ID_LANG_FRENCH: lang = 0x40; break;
		case ID_LANG_SPANISH: lang = 0x50; break;	// SPX: Additional value
		case ID_LANG_ITALIAN: lang = 0x60; break;	// SPX: Additional value

		case ID_DUNGEON_DUNGEON_MASTER:		dung = 1; break;
		case ID_DUNGEON_CHAOS_STRIKES_BACK: dung = 2; break;
		case ID_DUNGEON_THERON_QUEST:		dung = 3; break;
		case ID_DUNGEON_SKULLKEEP_BETA:		dung = 4; break;
		case ID_DUNGEON_LEGEND_SKULLKEEP:	dung = 5; break;

		case ID_VERSION_NO_SPECIFIC: gdat_vers = 0; break;
		case ID_VERSION_V1: gdat_vers = 1; break;
		case ID_VERSION_V2: gdat_vers = 2; break;
		case ID_VERSION_V3: gdat_vers = 3; break;
		case ID_VERSION_V4: gdat_vers = 4; break;
		case ID_VERSION_V5_CLASSIC: gdat_vers = 5; break;
		case ID_VERSION_V5_CARTOON: gdat_vers = 6; break;


		case ID_EXTRA_DOUBLE_STEP_MOVE: enableDoubleStepMove = !enableDoubleStepMove; break;
		case ID_FOG_SWITCH: SkCodeParam::bDisableFogEffect = !SkCodeParam::bDisableFogEffect; break;
		case ID_DEBUG_BREAK_DOOR: SkCodeParam::bWeakDoors = !SkCodeParam::bWeakDoors; break;
		case ID_DEBUG_WALK_OVER_PIT: SkCodeParam::bWalkOverPits = !SkCodeParam::bWalkOverPits; break;
		case ID_USE_CUSTOM_SPELLS: SkCodeParam::bUseCustomSpells = !SkCodeParam::bUseCustomSpells; break;
		case ID_EXTRA_SOUNDS: SkCodeParam::bUseExtendedSound = !SkCodeParam::bUseExtendedSound; break;
		case ID_SOUND_FOOTSTEP: SkCodeParam::bUsePlayerWalkSound = !SkCodeParam::bUsePlayerWalkSound; break;
	}
	if (sxfact != -1) {
		this->sxfact = sxfact;
		CRect rc;
		GetClientRect(&rc);
		ClientToScreen(&rc);
		rc.right = rc.left + 320 * sxfact;
		rc.bottom = rc.top + 200 * sxfact;
		AdjustWindowRectEx(&rc, GetStyle(), true, GetExStyle());
		MoveWindow(&rc);
		Invalidate();

		CString str;
		str.Format("%d", sxfact); WritePrivateProfileString(_skwin, "sxfact", str, GetSkwinini());
	}
	if (sblast != -1) {
		this->sblast = sblast;
		CString str;
		str.Format("%d", sblast); WritePrivateProfileString(_skwin, "sblast", str, GetSkwinini());
		selectSBlast();
	}
	if (sbdsbf != -1) {
		this->sbdsbf = sbdsbf;
		CString str;
		str.Format("%d", sbdsbf); WritePrivateProfileString(_skwin, "sbdsbf", str, GetSkwinini());
	}
	if (sbdsaf != -1) {
		this->sbdsaf = sbdsaf;
		CString str;
		str.Format("%d", sbdsaf); WritePrivateProfileString(_skwin, "sbdsaf", str, GetSkwinini());
	}
	if (spfact != -1) {
		this->spfact = spfact;
		CString str;
		str.Format("%d", spfact); WritePrivateProfileString(_skwin, "spfact", str, GetSkwinini());
	}
	if (lang != -1) {
		this->lang = lang;
		CString str;
		str.Format("%d", lang); WritePrivateProfileString(_skwin, "lang", str, GetSkwinini());

		AfxMessageBox(_T("You need restart skwin to take effect!"), MB_ICONEXCLAMATION);
	}
	if (dung != -1) {
		this->dung = dung;
		CString str;
		str.Format("%d", dung); WritePrivateProfileString(_skwin, "dung", str, GetSkwinini());

		AfxMessageBox(_T("You need restart skwin to take effect!"), MB_ICONEXCLAMATION);
	}
	if (gdat_vers != -1) {
		this->gdat_vers = gdat_vers;
		CString str;
		str.Format("%d", gdat_vers); WritePrivateProfileString(_skwin, "gdat_vers", str, GetSkwinini());

		AfxMessageBox(_T("You need restart skwin to take effect!"), MB_ICONEXCLAMATION);
	}

}

void CSkWinMFC::OnUpdateVideo1x(CCmdUI *pCmdUI) {
	switch (pCmdUI->m_nID) {
		case ID_VIDEO_1X: pCmdUI->SetRadio(sxfact == 1); break;
		case ID_VIDEO_2X: pCmdUI->SetRadio(sxfact == 2); break;
		case ID_VIDEO_3X: pCmdUI->SetRadio(sxfact == 3); break;
		case ID_VIDEO_4X: pCmdUI->SetRadio(sxfact == 4); break;

		case ID_SOUND_NO: pCmdUI->SetRadio(sblast == 0); break;
		case ID_SOUND_WINMM: pCmdUI->SetRadio(sblast == 1); break;
		case ID_SOUND_DS: pCmdUI->SetRadio(sblast == 2); break;
		case ID_SOUND_OPENAL: pCmdUI->SetRadio(sblast == 3); break;
		case ID_DS_BF_0: pCmdUI->SetRadio(sbdsbf == 0); break;
		case ID_DS_BF_HARD: pCmdUI->SetRadio(sbdsbf == 1); break;
		case ID_DS_BF_SOFT: pCmdUI->SetRadio(sbdsbf == 2); break;
		case ID_DS_AF_0: pCmdUI->SetRadio(sbdsaf == 0); break;
		case ID_DS_AF_FULL: pCmdUI->SetRadio(sbdsaf == 1); break;
		case ID_DS_AF_LIGHT: pCmdUI->SetRadio(sbdsaf == 2); break;

		case ID_MIDI_MUSIC_ENABLED: pCmdUI->SetRadio(SkCodeParam::bMIDIMusicEnabled); break;

		case ID_SPEED_1ULTRAFAST: pCmdUI->SetRadio(spfact == 0); break;
		case ID_SPEED_2FAST: pCmdUI->SetRadio(spfact == 1); break;
		case ID_SPEED_3NORMAL: pCmdUI->SetRadio(spfact == 2); break;
		case ID_SPEED_4SLOW: pCmdUI->SetRadio(spfact == 3); break;
		case ID_SPEED_5VERYSLOW: pCmdUI->SetRadio(spfact == 4); break;
		case ID_SPEED_6GLACIAL: pCmdUI->SetRadio(spfact == 5); break;

		case ID_LANG_ENGLISH: pCmdUI->SetRadio(lang == 0x10); break;
		case ID_LANG_JAPANESE: pCmdUI->SetRadio(lang == 0x20); break;
		case ID_LANG_GERMAN: pCmdUI->SetRadio(lang == 0x30); break;
		case ID_LANG_FRENCH: pCmdUI->SetRadio(lang == 0x40); break;
		case ID_LANG_SPANISH: pCmdUI->SetRadio(lang == 0x50); break;	// SPX: Add usage of Spanish
		case ID_LANG_ITALIAN: pCmdUI->SetRadio(lang == 0x60); break;	// SPX: Add usage of Italian

		case ID_DUNGEON_DUNGEON_MASTER: pCmdUI->SetRadio(dung == 1); break;
		case ID_DUNGEON_CHAOS_STRIKES_BACK: pCmdUI->SetRadio(dung == 2); break;
		case ID_DUNGEON_THERON_QUEST: pCmdUI->SetRadio(dung == 3); break;
		case ID_DUNGEON_SKULLKEEP_BETA: pCmdUI->SetRadio(dung == 4); break;
		case ID_DUNGEON_LEGEND_SKULLKEEP: pCmdUI->SetRadio(dung == 5); break;

		case ID_VERSION_NO_SPECIFIC: pCmdUI->SetRadio(gdat_vers == 0); break;
		case ID_VERSION_V1: pCmdUI->SetRadio(gdat_vers == 1); break;
		case ID_VERSION_V2: pCmdUI->SetRadio(gdat_vers == 2); break;
		case ID_VERSION_V3: pCmdUI->SetRadio(gdat_vers == 3); break;
		case ID_VERSION_V4: pCmdUI->SetRadio(gdat_vers == 4); break;
		case ID_VERSION_V5_CLASSIC: pCmdUI->SetRadio(gdat_vers == 5); break;
		case ID_VERSION_V5_CARTOON: pCmdUI->SetRadio(gdat_vers == 6); break;


		case ID_EXTRA_DOUBLE_STEP_MOVE: pCmdUI->SetCheck(enableDoubleStepMove); break;
		case ID_FOG_SWITCH: pCmdUI->SetCheck(SkCodeParam::bDisableFogEffect); break;
		case ID_DEBUG_BREAK_DOOR: pCmdUI->SetCheck(SkCodeParam::bWeakDoors); break;
		case ID_DEBUG_WALK_OVER_PIT: pCmdUI->SetCheck(SkCodeParam::bWalkOverPits); break;
		case ID_USE_CUSTOM_SPELLS: pCmdUI->SetCheck(SkCodeParam::bUseCustomSpells); break;
		case ID_EXTRA_SOUNDS: pCmdUI->SetCheck(SkCodeParam::bUseExtendedSound); break;
		case ID_SOUND_FOOTSTEP: pCmdUI->SetCheck(SkCodeParam::bUsePlayerWalkSound); break;
	}
}

BOOL CSkWinMFC::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs)) return false;

	cs.hMenu = LoadMenu(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MENU1));
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.x = ptfrm.x;
	cs.y = ptfrm.y;
	return true;
}

void CSkWinMFC::PostNcDestroy()
{

}

void CSkWinMFC::OnMove(int x, int y)
{
	CFrameWnd::OnMove(x, y);

	CRect rc;
	GetWindowRect(&rc);
	CString str;
	str.Format("%d", rc.left); WritePrivateProfileString(_skwin, "framex", str, GetSkwinini());
	str.Format("%d", rc.top); WritePrivateProfileString(_skwin, "framey", str, GetSkwinini());
}

void CSkWinMFC::selectSBlast()
{
	PlaySound(NULL, NULL, SND_PURGE);
	sbds.Close();
	sboa.Close();

	if (sblast == 2) sbds.Open(*this);
	if (sblast == 3) sboa.Open();
}

void CSkWinMFC::ShowMessage(const char *psz) {
#ifdef _USE_MFC60
	USES_CONVERSION;
	AfxMessageBox(A2T(const_cast<char *>(psz)), MB_ICONERROR|MB_TASKMODAL);
#else
	AfxMessageBox(CA2T(psz), MB_ICONERROR|MB_TASKMODAL);
#endif
}

bool CSkWinMFC::AskMe(const char *psz) {
#ifdef _USE_MFC60
	USES_CONVERSION;
	if (AfxMessageBox(A2T(const_cast<char *>(psz)), MB_ICONEXCLAMATION|MB_TASKMODAL|MB_YESNO) == IDYES)
		return true;
	return false;
#else
	if (AfxMessageBox(CA2T(psz), MB_ICONEXCLAMATION|MB_TASKMODAL|MB_YESNO) == IDYES)
		return true;
	return false;
#endif
}

BEGIN_MESSAGE_MAP(CSkApp, CWinApp)
#ifdef _USE_MFC60
	ON_COMMAND(ID_HELP_ABOUT, CSkApp::OnHelpAbout)
#else
	ON_COMMAND(ID_HELP_ABOUT, &CSkApp::OnHelpAbout)
#endif
END_MESSAGE_MAP()

void CSkApp::OnHelpAbout()
{
	CAboutDlg wndDlg;
	wndDlg.DoModal();
}
