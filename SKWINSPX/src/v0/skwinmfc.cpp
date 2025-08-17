//------------------------------------------------------------------------------
// SKWINMFC
//	Specific code for Win MFC windows
//	This is derivated from original SKWin code
//	Note: this code is not compiled for non-MFC platforms.
//------------------------------------------------------------------------------

//..............................................................................

#include <skver.h>

//..............................................................................


#if (_MSC_VER >= 1200) || defined(_USE_MFC80) || defined(_USE_MFC60)
	#include <stdafx.h>	// Required for MSVC6 compilation
	//#include <windows.h>
	#include <afxwin.h>
	#include <objbase.h>
	#pragma comment(lib, "ole32.lib")
	#include <skmfcres.h>
	#include <AboutDlg.h>

	#include <afx.h>

	#ifdef _USE_MFC80
	# include <atlcore.h>
	# include <atlcoll.h>
	# include <atlstr.h>
	# include <atlcom.h>
	#else
	# include <atlbase.h>
	extern CComModule _Module;
	# include <atlcom.h>
	# define ATLVERIFY VERIFY // use mfc's one
	#endif // _USE_MFC80

#endif

#include <skwinmfc.h>
#include <skparam.h>

//..............................................................................

#define _skwin "skwin"
#define _skwin_ini "SkWin.ini"

//#ifdef _USE_MFC60
//CComModule _Module;
//#endif

//##############################################################################

#if !defined (__NO_MFC__)
#if (_MSC_VER >= 1200) || defined(_USE_MFC80) || defined(_USE_MFC60)

//------------------------------------------------------------------------------
// AboutDlg.cpp : 実装ファイル
//
//#include <stdafx.h>
#include <skmfcres.h>
#include <AboutDlg.h>

// CAboutDlg ダイアログ

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{

}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CAboutDlg メッセージ ハンドラ

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

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


//------------------------------------------------------------------------------

CSkWinMFC::CSkWinMFC()
{
	xVRAM = NULL;
	xVGAPalette = NULL;

	GetSkwinini() = GetSkwinini();
	printf("SKini file = %s\n", GetSkwinini());

	sxfact = GetPrivateProfileInt(_skwin, "sxfact", 1, GetSkwinini());
	sblast = GetPrivateProfileInt(_skwin, "sblast", 1, GetSkwinini());
	ptfrm.x = GetPrivateProfileInt(_skwin, "framex", 0, GetSkwinini());
	ptfrm.y = GetPrivateProfileInt(_skwin, "framey", 0, GetSkwinini());
	sbdsbf = GetPrivateProfileInt(_skwin, "sbdsbf", 0, GetSkwinini());
	sbdsaf = GetPrivateProfileInt(_skwin, "sbdsaf", 0, GetSkwinini());
	spfact = GetPrivateProfileInt(_skwin, "spfact", 2, GetSkwinini());
//	lang = GetPrivateProfileInt(_skwin, "lang", _GDAT_LANG_ENGLISH_, GetSkwinini());
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

//..............................................................................

static X8 iLocalPaletteRGB[256][3];

BYTE vrbi[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];

BYTE tbl_gammac[64];

BYTE Gammac(BYTE v) {
	return tbl_gammac[v];
}

void CSkWinMFC::OnPaint()
{
	CPaintDC dc(this);
	PBITMAPINFO pbi = reinterpret_cast<PBITMAPINFO>(vrbi);
	X8* pVRAMData;
	pVRAMData = xVRAM->GET_VIDEO_ARRAY();

	for (int x=0; x<256; x++) {
		pbi->bmiColors[x].rgbRed   = Gammac(iLocalPaletteRGB[x][0]);
		pbi->bmiColors[x].rgbGreen = Gammac(iLocalPaletteRGB[x][1]);
		pbi->bmiColors[x].rgbBlue  = Gammac(iLocalPaletteRGB[x][2]);
		pbi->bmiColors[x].rgbReserved = 0;
	}
	if (sxfact == 1) {
		SetDIBitsToDevice(dc, 0, 0, 320, 200, 0, 0, 0, 200, pVRAMData, pbi, DIB_RGB_COLORS);
	}
	else {
		StretchDIBits(dc, 0, 0, 320*sxfact, 200*sxfact, 0, 0, 320, 200, pVRAMData, pbi, DIB_RGB_COLORS, SRCCOPY);
	}
}

bool CSkWinMFC::ML() {
	if (m_hWnd == NULL || !IsWindow(m_hWnd))
		return false;
	theApp.ML();
	return true;
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

	//selectSBlast();

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

void CSkWinMFC::SetVRAM(SkVRAM* xVRAM)
{
	this->xVRAM = xVRAM;
}

void CSkWinMFC::SetVGAPaletteRGB(X8 *xVGAPalette)
{
	this->xVGAPalette = xVGAPalette;
    for (UINT i = 0; i < 256; i++) {
        iLocalPaletteRGB[i][0] = (U8) (xVGAPalette[i * 4 + 0]>>2);
        iLocalPaletteRGB[i][1] = (U8) (xVGAPalette[i * 4 + 1]>>2);
        iLocalPaletteRGB[i][2] = (U8) (xVGAPalette[i * 4 + 2]>>2);
	}
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
		case ID_VIDEO_5X: sxfact = 5; break;

		case ID_SOUND_NO: sblast = 0; break;
		case ID_SOUND_WINMM: sblast = 1; break;
		case ID_SOUND_DS: sblast = 2; break;
		case ID_SOUND_OPENAL: sblast = 3; break;
		case ID_SOUND_ALLEGRO: sblast = 4; break;

		case ID_DS_BF_0: sbdsbf = 0; break;
		case ID_DS_BF_HARD: sbdsbf = 1; break;
		case ID_DS_BF_SOFT: sbdsbf = 2; break;
		case ID_DS_AF_0: sbdsaf = 0; break;
		case ID_DS_AF_FULL: sbdsaf = 1; break;
		case ID_DS_AF_LIGHT: sbdsaf = 2; break;

		case ID_MIDI_MUSIC_ENABLED: SkCodeParam::bMIDIMusicEnabled = true; SkCodeParam::bWAVMusicEnabled = false; break;
		case ID_WAV_MUSIC_ENABLED: SkCodeParam::bWAVMusicEnabled = true; SkCodeParam::bMIDIMusicEnabled = false; break;

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

		case ID_EXTRA_DOUBLE_STEP_MOVE: SkCodeParam::bEnableDoubleStepMove = !SkCodeParam::bEnableDoubleStepMove; break;
		case ID_FOG_SWITCH: SkCodeParam::bDisableFogEffect = !SkCodeParam::bDisableFogEffect; break;
		case ID_DEBUG_BREAK_DOOR: SkCodeParam::bWeakDoors = !SkCodeParam::bWeakDoors; break;
		case ID_DEBUG_WALK_OVER_PIT: SkCodeParam::bWalkOverPits = !SkCodeParam::bWalkOverPits; break;
		case ID_USE_CUSTOM_SPELLS: SkCodeParam::bUseCustomSpells = !SkCodeParam::bUseCustomSpells; break;
		case ID_EXTRA_SOUNDS: SkCodeParam::bUseExtendedSound = !SkCodeParam::bUseExtendedSound; break;
		case ID_SOUND_FOOTSTEP: SkCodeParam::bUsePlayerWalkSound = !SkCodeParam::bUsePlayerWalkSound; break;
		case ID_SUPER_INFO_EYE: SkCodeParam::bUseSuperInfoEye = !SkCodeParam::bUseSuperInfoEye; SkCodeParam::bShowLuck = SkCodeParam::bUseSuperInfoEye; break;
		case ID_SUPER_MODE: SkCodeParam::bUseSuperMode = !SkCodeParam::bUseSuperMode; break;
		case ID_DEBUG_INFINITE_SPELL: SkCodeParam::bDebugInfiniteSpells = !SkCodeParam::bDebugInfiniteSpells; break;
		case ID_DEBUG_NO_TIMER: SkCodeParam::bNoTimer = !SkCodeParam::bNoTimer; break;

		case ID_DEBUG_GFX_NO_WALLS: SkCodeParam::bDebugGFXNoWalls = !SkCodeParam::bDebugGFXNoWalls; break;
		case ID_DEBUG_GFX_NO_DOORS: SkCodeParam::bDebugGFXNoDoors = !SkCodeParam::bDebugGFXNoDoors; break;
		case ID_DEBUG_GFX_NO_CREATURES: SkCodeParam::bDebugGFXNoCreatures = !SkCodeParam::bDebugGFXNoCreatures; break;
		case ID_DEBUG_GFX_NO_INTERWALLS: SkCodeParam::bDebugGFXNoInterWalls = !SkCodeParam::bDebugGFXNoInterWalls; break;
	
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
		//selectSBlast();
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

}

void CSkWinMFC::OnUpdateVideo1x(CCmdUI *pCmdUI) {
	switch (pCmdUI->m_nID) {
		case ID_VIDEO_1X: pCmdUI->SetRadio(sxfact == 1); break;
		case ID_VIDEO_2X: pCmdUI->SetRadio(sxfact == 2); break;
		case ID_VIDEO_3X: pCmdUI->SetRadio(sxfact == 3); break;
		case ID_VIDEO_4X: pCmdUI->SetRadio(sxfact == 4); break;
		case ID_VIDEO_5X: pCmdUI->SetRadio(sxfact == 5); break;

		case ID_SOUND_NO: pCmdUI->SetRadio(sblast == 0); break;
		case ID_SOUND_WINMM: pCmdUI->SetRadio(sblast == 1); break;
		case ID_SOUND_DS: pCmdUI->SetRadio(sblast == 2); break;
		case ID_SOUND_OPENAL: pCmdUI->SetRadio(sblast == 3); break;
		case ID_SOUND_ALLEGRO: pCmdUI->SetRadio(sblast == 4); break;
		case ID_DS_BF_0: pCmdUI->SetRadio(sbdsbf == 0); break;
		case ID_DS_BF_HARD: pCmdUI->SetRadio(sbdsbf == 1); break;
		case ID_DS_BF_SOFT: pCmdUI->SetRadio(sbdsbf == 2); break;
		case ID_DS_AF_0: pCmdUI->SetRadio(sbdsaf == 0); break;
		case ID_DS_AF_FULL: pCmdUI->SetRadio(sbdsaf == 1); break;
		case ID_DS_AF_LIGHT: pCmdUI->SetRadio(sbdsaf == 2); break;

		case ID_MIDI_MUSIC_ENABLED: pCmdUI->SetRadio(SkCodeParam::bMIDIMusicEnabled); break;
		case ID_WAV_MUSIC_ENABLED: pCmdUI->SetRadio(SkCodeParam::bWAVMusicEnabled); break;

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

		case ID_EXTRA_DOUBLE_STEP_MOVE: pCmdUI->SetCheck(SkCodeParam::bEnableDoubleStepMove); break;
		case ID_FOG_SWITCH: pCmdUI->SetCheck(SkCodeParam::bDisableFogEffect); break;
		case ID_DEBUG_BREAK_DOOR: pCmdUI->SetCheck(SkCodeParam::bWeakDoors); break;
		case ID_DEBUG_WALK_OVER_PIT: pCmdUI->SetCheck(SkCodeParam::bWalkOverPits); break;
		case ID_USE_CUSTOM_SPELLS: pCmdUI->SetCheck(SkCodeParam::bUseCustomSpells); break;
		case ID_EXTRA_SOUNDS: pCmdUI->SetCheck(SkCodeParam::bUseExtendedSound); break;
		case ID_SOUND_FOOTSTEP: pCmdUI->SetCheck(SkCodeParam::bUsePlayerWalkSound); break;
		case ID_SUPER_INFO_EYE: pCmdUI->SetCheck(SkCodeParam::bUseSuperInfoEye); break;
		case ID_SUPER_MODE: pCmdUI->SetCheck(SkCodeParam::bUseSuperMode); break;
		case ID_DEBUG_INFINITE_SPELL: pCmdUI->SetCheck(SkCodeParam::bDebugInfiniteSpells); break;
		case ID_DEBUG_NO_TIMER: pCmdUI->SetCheck(SkCodeParam::bNoTimer); break;

		case ID_DEBUG_GFX_NO_WALLS: pCmdUI->SetCheck(SkCodeParam::bDebugGFXNoWalls); break;
		case ID_DEBUG_GFX_NO_INTERWALLS: pCmdUI->SetCheck(SkCodeParam::bDebugGFXNoInterWalls); break;
		case ID_DEBUG_GFX_NO_DOORS: pCmdUI->SetCheck(SkCodeParam::bDebugGFXNoDoors); break;
		case ID_DEBUG_GFX_NO_CREATURES: pCmdUI->SetCheck(SkCodeParam::bDebugGFXNoCreatures); break;
	
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


//##############################################################################
#endif // _MSC_VER / _USE_MFC80 / _USE_MFC60
#endif // not __NO_MFC__


//------------------------------------------------------------------------------


