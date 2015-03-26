// Exp00069.cpp : アプリケーション用クラスの機能定義を行います。
//

#include "stdafx.h"
#include "Exp00069.h"

#include "MainFrm.h"
#include "Exp00069Doc.h"
#include "Exp00069View.h"
#include "SplitStr.h"
#include "DM2HierMapView.h"
#include "CompatDC.h"
#include "CustRichEditCtrl.h"
#include "CurDir.h"
#include "DM2NameDic.h"
#include "HUGHUG.h"
#include "SplashFrame.h"
#include "AfxDeclStatic.h"
#include "ProfStr.h"
#include "OSP.h"

#pragma comment(lib, "Version.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void ReadFontTemplateFrom(LOGFONT &lf, int nID, LPCTSTR lpszDef)
{
	ZeroMemory(&lf, sizeof(lf));
	CString tstr;
	if (!tstr.LoadString(nID) || tstr == "-")
		tstr = lpszDef;
	LPCTSTR lpszDelimit = ";";
	CStringList l;
	SplitStr(tstr, *lpszDelimit, l);
	if (!l.IsEmpty()) strncpy(lf.lfFaceName, l.RemoveHead(), sizeof(lf.lfFaceName));
	if (!l.IsEmpty()) lf.lfHeight = atoi(l.RemoveHead());
	if (!l.IsEmpty()) lf.lfWidth = atoi(l.RemoveHead());
	if (!l.IsEmpty()) (void)l.RemoveHead();
	if (!l.IsEmpty()) (void)l.RemoveHead();
	if (!l.IsEmpty()) lf.lfWeight = atoi(l.RemoveHead());
	if (!l.IsEmpty()) lf.lfItalic = atoi(l.RemoveHead());
	if (!l.IsEmpty()) lf.lfUnderline = atoi(l.RemoveHead());
	if (!l.IsEmpty()) lf.lfStrikeOut = atoi(l.RemoveHead());
	if (!l.IsEmpty()) lf.lfCharSet = atoi(l.RemoveHead());
	if (!l.IsEmpty()) (void)l.RemoveHead();
	if (!l.IsEmpty()) (void)l.RemoveHead();
	if (!l.IsEmpty()) lf.lfQuality = atoi(l.RemoveHead());
	if (!l.IsEmpty()) lf.lfPitchAndFamily = atoi(l.RemoveHead());
}

/////////////////////////////////////////////////////////////////////////////
// CExp00069App

BEGIN_MESSAGE_MAP(CExp00069App, CWinApp)
	//{{AFX_MSG_MAP(CExp00069App)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_OPT_RELOAD_ORNATES, OnOptReloadOrnates)
	ON_COMMAND(ID_OPT_RELOAD_DIC, OnOptReloadDic)
	ON_COMMAND(ID_HELP_SHOWCASE, OnHelpShowcase)
	ON_COMMAND(ID_HELP_FINDERVW, OnHelpFindervw)
	//}}AFX_MSG_MAP
	// 標準のファイル基本ドキュメント コマンド
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExp00069App クラスの構築

CExp00069App::CExp00069App()
{
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CExp00069App オブジェクト

CExp00069App theApp;

CExp00069App *GetApp()
{
	return &theApp;
}

/////////////////////////////////////////////////////////////////////////////
// CExp00069App クラスの初期化

CSplashFrame *splash_frame;

BOOL CExp00069App::InitInstance()
{
	// 標準的な初期化処理
	if (!AfxInitRichEdit())
		return FALSE;
	if (!AfxOleInit())
		return FALSE;
	if (!AfxSocketInit())
		return false;
	HRESULT hr;
	if (FAILED(hr = CoInitialize(NULL)))
		return FALSE;

	Av::Construct();

	m_strWorkDir = CCurDir::GetCurDir();

	m_strCnf = OSP::OSP_JoinPath(OSP::OSP_GetDir(OSP::OSP_GetModuleFileName()), "DMDC2.ini");

	ProfStr::LoadProfileStringFrom("DMDC2", "Assets", m_strAssets, "DM2", GetCnf());

	// 設定が保存される下のレジストリ キーを変更します。
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings(16);  // 標準の INI ファイルのオプションをローﾄﾞします (MRU を含む)

	// ドキュメント テンプレートを登録します。

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CExp00069Doc),
		RUNTIME_CLASS(CMainFrame),       // メイン SDI フレーム ウィンドウ
		RUNTIME_CLASS(CDM2HierMapView));
	AddDocTemplate(pDocTemplate);

	// DDE、file open など標準のシェル コマンドのコマンドラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ->
	const int cx = GetSystemMetrics(SM_CXFULLSCREEN);
	const int cy = GetSystemMetrics(SM_CYFULLSCREEN);

	CSplashFrame frame;
	frame.CreateEx(0, NULL, NULL, 0 |WS_POPUP |WS_DLGFRAME, CRect(0, 0, cx, cy), NULL, 0);
	frame.InitialUpdateFrame(NULL, true);
	frame.UpdateWindow();

	splash_frame = &frame;

//	VERIFY(LoadOrnates());
	VERIFY(LoadOrnates2());
	VERIFY(LoadDic());

	splash_frame = NULL;

	frame.DestroyWindow();
	// <-

	// コマンドラインでディスパッチ コマンドを指定します。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	if (m_pRecentFileList != NULL)
		m_pRecentFileList->m_nMaxDisplayLength = -1;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	int m_iDefSel;

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CCustRichEditCtrl	m_wndRich;
	int		m_iSel;
	CStatic	m_wndVer;
	//}}AFX_DATA

	// ClassWizard 仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelChangeComboSel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_iSel = -1;
	//}}AFX_DATA_INIT

	m_iSel = m_iDefSel = 0;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_RICHEDIT_HINT, m_wndRich);
	DDX_CBIndex(pDX, IDC_COMBO_SEL, m_iSel);
	DDX_Control(pDX, IDC_STATIC_VER, m_wndVer);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_SEL, OnSelChangeComboSel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ダイアログを実行するためのアプリケーション コマンド
void CExp00069App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

DWORD CALLBACK fnEditStreamCallback(DWORD nCookie, LPBYTE pbBuff, LONG cbBuff, LONG *pcb)
{
	CMemFile *f = (CMemFile *)nCookie;
	*pcb = f->Read(pbBuff, cbBuff);
	return 0;
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_iSel = m_iDefSel;

	UpdateData(false);

	OnSelChangeComboSel();

	{
		// DMDC2 DM2_EXTENDED_DATABASE Rev.* ... ****-**-**
		HRSRC hRes = FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
		if (hRes != NULL) {
			HGLOBAL hMem = LoadResource(AfxGetInstanceHandle(), hRes);
			if (hMem != NULL) {
				PVOID pBlockOrg = LockResource(hMem);
				if (pBlockOrg != NULL) {
					DWORD cbRes = SizeofResource(AfxGetInstanceHandle(), hRes);
					LPVOID pBlock = LocalAlloc(LMEM_FIXED, cbRes);
					if (pBlock != NULL) {
						CopyMemory(pBlock, pBlockOrg, cbRes);

						PVOID pVal = NULL;
						UINT nVal = 0;
						CString str;
						m_wndVer.GetWindowText(str);
						if (0 != VerQueryValue(pBlock, _T("\\StringFileInfo\\040904b0\\Rev"), &pVal, &nVal)) {
							str.Replace("*", (LPCTSTR)pVal);
						}
						if (0 != VerQueryValue(pBlock, _T("\\StringFileInfo\\040904b0\\ReleaseDate"), &pVal, &nVal)) {
							str.Replace("####-##-##", (LPCTSTR)pVal);
						}
						m_wndVer.SetWindowText(str);

						LocalFree(pBlock);
					}
				}
			}
		}
	}
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CAboutDlg::OnSelChangeComboSel() 
{
	if (!UpdateData())
		return;

	const UINT nItems = 6;

	static const UINT table[nItems] = {
		IDR_RTF_SHORTCUTS,
		IDR_RTF_DND,
		IDR_RTF_TILEPROP,
		IDR_RTF_FINDERVW,
		IDR_RTF_ABOUT,
		IDR_RTF_CHANGES,
	};

	VERIFY(m_wndRich.LoadFromResource("RTF", MAKEINTRESOURCE(table[m_iSel % nItems])));
	m_wndRich.EnableAutoUrlDetect();
	m_wndRich.EnableLink4Browse();

	UpdateData(false);
}

/////////////////////////////////////////////////////////////////////////////
// CExp00069App メッセージ ハンドラ

void CExp00069App::RegisterIdleCB(COnIdle *pCB)
{
	m_listIdleTar.Add(pCB);
}

void CExp00069App::RevokeIdleCB(COnIdle *pCB)
{
	UINT i, n = m_listIdleTar.GetSize();
	for (i = 0; i < n; i++) {
		if (m_listIdleTar[i] == pCB) {
			m_listIdleTar.RemoveAt(i);
			i--, n--;
		}
	}
}

BOOL CExp00069App::OnIdle(LONG lCount) 
{
	int nMore = 0;

	UINT i, n = m_listIdleTar.GetSize();
	for (i = 0; i < n; i++) {
		nMore += (m_listIdleTar[i]->OnIdleNotify()) ? 1 : 0;
	}
	
	return CWinApp::OnIdle(lCount) || nMore;
}

BOOL CExp00069App::RenderItem16To(CDC *pDC, int x, int y, BYTE db, BYTE idx, int cxTo)
{
	static const BYTE table[16] = {
			-1,
			-1,
			-1,
			-1,
			-1,
			16,
			17,
			18,
			19,
			20,
			21,
			-1,
			-1,
			-1,
			-1,
			-1,
	};

	BYTE index = table[db & 15];
	if (index == -1) return false;

	CBMO *pbmo0 = m_bmovec.FindBMO(4*index +0);
	CBMO *pbmo1 = m_bmovec.FindBMO(4*index +1);

	if (pbmo0 != NULL && pbmo1 != NULL) {
		//
		int px = 16 * ((idx % 16)     );
		int py = 16 * ((idx / 16) % 16);

		int cx = cxTo;

		pDC->SetStretchBltMode(COLORONCOLOR);

		// DSna
		StretchDIBits(*pDC, x, y, cx, cx, px, py, 16, 16, pbmo1->pv, pbmo1->pbmi, DIB_RGB_COLORS, 0x00220326);
		// DSo
		StretchDIBits(*pDC, x, y, cx, cx, px, py, 16, 16, pbmo0->pv, pbmo0->pbmi, DIB_RGB_COLORS, SRCPAINT);

		return TRUE;
	}
	return FALSE;
}

#if 0
//BOOL CExp00069App::RenderItem16To(CDC *pDC, int x, int y, BYTE db, BYTE idx, int cxTo)
//{
//	db = db & 15;
//	HBITMAP hbm[2] = {NULL, NULL};
//	hbm[0] = m_bm[db][0];
//	hbm[1] = m_bm[db][1];
//
//	if (hbm[0] && hbm[1]) {
//		CCompatDC dcMem(pDC);
//		CGdiObject *pOldBM = dcMem.GetCurrentBitmap();
//		//
//		int px = 16 * ((idx % 16));
//		int py = 16 * ((idx / 16) % 16);
//
//		int cx = cxTo;
//
//		dcMem.SelectObject(hbm[1]); pDC->StretchBlt(x, y, cx, cx, &dcMem, px, py, 16, 16, SRCAND);
//		dcMem.SelectObject(hbm[0]); pDC->StretchBlt(x, y, cx, cx, &dcMem, px, py, 16, 16, SRCPAINT);
//		//
//		dcMem.SelectObject(pOldBM);
//		return TRUE;
//	}
//	return FALSE;
//}
#endif

BOOL CExp00069App::RenderOrnate64To(CDC *pDC, int x, int y, BYTE c1, BYTE c2, int cxTo)
{
	BYTE index = -1;

	switch (c1) {
	case 0x09: // WALL ORNATES
		index = 9; break;
	case 0x0A: // FLOOR ORNATES
		index = 10; break;
	case 0x0B: // DOOR ORNATES
		index = 11; break;
	case 0x0E: // DOORS
		index = 14; break;
	case 0x0F: // CREATURES
		index = 15; break;
	}

	CBMO *pbmo0 = m_bmovec.FindBMO(4*index +0);
	CBMO *pbmo1 = m_bmovec.FindBMO(4*index +1);

	if (pbmo0 != NULL && pbmo1 != NULL) {
		//

		int px = 64 * ((c2 % 16));
		int py = 64 * ((c2 / 16) % 16);

		int cx = cxTo;

		pDC->SetStretchBltMode(COLORONCOLOR);

		StretchDIBits(*pDC, x, y, cx, cx, px, py, 64, 64, pbmo1->pv, pbmo1->pbmi, DIB_RGB_COLORS, 0x00220326);
		StretchDIBits(*pDC, x, y, cx, cx, px, py, 64, 64, pbmo0->pv, pbmo0->pbmi, DIB_RGB_COLORS, SRCPAINT);
		//
		return TRUE;
	}
	return FALSE;
}

#if 0
//BOOL CExp00069App::RenderOrnate64To(CDC *pDC, int x, int y, BYTE c1, BYTE c2, int cxTo)
//{
//	HBITMAP hbm[2] = {NULL, NULL};
//	switch (c1) {
//	case 0x09: // WALL ORNATES
//		{
//			hbm[0] = m_bm64x64[1][0];
//			hbm[1] = m_bm64x64[1][1];
//			break;
//		}
//	case 0x0A: // FLOOR ORNATES
//		{
//			hbm[0] = m_bm64x64[0][0];
//			hbm[1] = m_bm64x64[0][1];
//			break;
//		}
//	case 0x0B: // DOOR ORNATES
//		{
//			hbm[0] = m_bm64x64[4][0];
//			hbm[1] = m_bm64x64[4][1];
//			break;
//		}
//	case 0x0E: // DOORS
//		{
//			hbm[0] = m_bm64x64[3][0];
//			hbm[1] = m_bm64x64[3][1];
//			break;
//		}
//	case 0x0F: // CREATURES
//		{
//			hbm[0] = m_bm64x64[2][0];
//			hbm[1] = m_bm64x64[2][1];
//			break;
//		}
//	}
//	if (hbm[0] && hbm[1]) {
//		CCompatDC dcMem(pDC);
//		CGdiObject *pOldBM = dcMem.GetCurrentBitmap();
//		//
//		int px = 64 * ((c2 % 16));
//		int py = 64 * ((c2 / 16) % 16);
//
//		int cx = cxTo;
//
//		pDC->SetStretchBltMode(COLORONCOLOR);
//
//		dcMem.SelectObject(hbm[1]); pDC->StretchBlt(x, y, cx, cx, &dcMem, px, py, 64, 64, SRCAND);
//		dcMem.SelectObject(hbm[0]); pDC->StretchBlt(x, y, cx, cx, &dcMem, px, py, 64, 64, SRCPAINT);
//		//
//		dcMem.SelectObject(pOldBM);
//		return TRUE;
//	}
//	return FALSE;
//}
#endif

bool CExp00069App::LoadOrnates()
{
	CWaitCursor wc;

	int i;

	int nDone = 0, nCount = 0;

	HINSTANCE hInst = AfxGetInstanceHandle();

	for (i = 0; i < 2; i++) m_bmTS[i].DeleteObject();

	for (i = 0; i < 4; i++) m_bmHM[i].DeleteObject();

	for (i = 0; i < 8; i++) m_bmRoom[i].DeleteObject(), m_bmPT[i].DeleteObject();

	for (i = 0; i < 16; i++) m_bmTile[i].DeleteObject();

	for (i = 0; i < 16; i++) for (int j = 0; j < 2; j++) m_bm[i][j].DeleteObject();

	for (i = 0; i < 8; i++) for (int j = 0; j < 2; j++) m_bm64x64[i][j].DeleteObject();

	CHUGHUG HH(m_strWorkDir);

	{
		LPCSTR psz[16][2] = {
			NULL, NULL,
			NULL, NULL,
			NULL, NULL,
			NULL, NULL,
			NULL, NULL,
			"_Weapons_0.256by256.rle", "_Weapons_1.256by256.rle",
			"_Clothes_0.256by256.rle", "_Clothes_1.256by256.rle",
			"_Scrolls_0.256by256.rle", "_Scrolls_1.256by256.rle",
			"_Potions_0.256by256.rle", "_Potions_1.256by256.rle",
			"_Chests_0.256by256.rle", "_Chests_1.256by256.rle",
			"_Misc_0.256by256.rle", "_Misc_1.256by256.rle",
			NULL, NULL,
			NULL, NULL,
			NULL, NULL,
			NULL, NULL,
			NULL, NULL,
		};

		for (i = 0; i < 16; i++) 
			for (int j = 0; j < 2; j++)
				if (psz[i][j] != NULL)
					nCount++, nDone += m_bm[i][j].Attach((HBITMAP)::LoadImage(NULL, HH.Extraction(psz[i][j]), IMAGE_BITMAP, 0, 0, 0 |LR_CREATEDIBSECTION |LR_LOADFROMFILE |LR_VGACOLOR)) ? 1 : 0;
	}

	{
		LPCSTR psz[] = {
			"_RP_0.bmp",
			"_RP_1.bmp",
			"_RP_2.bmp",
			"_RP_3.bmp",
			"_TP_0.bmp",
			"_TP_1.bmp",
			"_TP_2.bmp",
			"_TP_3.bmp",
		};

		for (i = 0; i < 8; i++)
			nCount++, nDone += m_bmRoom[i].Attach((HBITMAP)::LoadImage(NULL, psz[i], IMAGE_BITMAP, 0, 0, 0 |LR_CREATEDIBSECTION |LR_LOADFROMFILE |LR_VGACOLOR)) ? 1 : 0;
	}

	{
		LPCSTR psz[][2] = {
			{"_FloorOrnates_0.1024by1024.rle", "_FloorOrnates_1.1024by1024.rle"},
			{"_WallOrnates_0.1024by1024.rle",  "_WallOrnates_1.1024by1024.rle"}, 
			{"_Creatures_0.1024by1024.rle",    "_Creatures_1.1024by1024.rle"},
			{"_Door_0.1024by1024.rle",         "_Door_1.1024by1024.rle"},
			{"_DoorOrnate_0.1024by1024.rle",   "_DoorOrnate_1.1024by1024.rle"}, 
		};

		for (i = 0; i < 5; i++)
			for (int j = 0; j < 2; j++)
				nCount++, nDone += m_bm64x64[i][j].Attach((HBITMAP)::LoadImage(NULL, HH.Extraction(psz[i][j]), IMAGE_BITMAP, 0, 0, 0 |LR_CREATEDIBSECTION |LR_LOADFROMFILE |LR_VGACOLOR)) ? 1 : 0;
	}

	{
		LPCSTR psz[] = {
			"_Tile01.bmp",
			"_Tile02.bmp",
			"_Tile03.bmp",
			"_Tile04.bmp",
			"_Tile05.bmp",
			"_Tile06.bmp",
			"_Tile07.bmp",
		};

		for (i = 0; i < 7; i++)
			nCount++, nDone += m_bmTile[i].Attach((HBITMAP)::LoadImage(NULL, psz[i], IMAGE_BITMAP, 0, 0, 0 |LR_CREATEDIBSECTION |LR_LOADFROMFILE |LR_VGACOLOR)) ? 1 : 0;
	}

	nCount++, nDone += m_bmPT[1].LoadBitmap(IDB_PT_1) ? 1 : 0;

	nCount++, nDone += m_bmHM[0].LoadBitmap(IDB_HATCHMASK) ? 1 : 0;
	nCount++, nDone += m_bmHM[1].LoadBitmap(IDB_HATCH_HIDMASK) ? 1 : 0;

	nCount++, nDone += m_bmTS[0].Attach((HBITMAP)::LoadImage(NULL, "_TileState.bmp", IMAGE_BITMAP, 0, 0, 0 |LR_CREATEDIBSECTION |LR_LOADFROMFILE |LR_VGACOLOR)) ? 1 : 0;

	{
		BITMAP bm;
		CCompatDC dc;
		CGdiObject *pOldObj = dc.GetCurrentBitmap();

		for (i = 0; i < 16; i++) {
			if (m_bm[i][1].m_hObject != NULL) {
				m_bm[i][1].GetBitmap(bm);
				dc.SelectObject(&m_bm[i][1]);
				dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dc, 0, 0, DSTINVERT);
			}
		}

		for (i = 0; i < 16; i++) {
			if (m_bm64x64[i][1].m_hObject) {
				m_bm64x64[i][1].GetBitmap(bm);
				dc.SelectObject(&m_bm64x64[i][1]);
				dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dc, 0, 0, DSTINVERT);
			}
		}

		dc.SelectObject(pOldObj);
	}

	if (m_bmBlack.m_hObject == NULL) m_bmBlack.LoadBitmap(IDB_BLACK);

	return nDone == nCount;
}

bool CExp00069App::LoadOrnates2()
{
	CWaitCursor wc;

	m_bmovec.Clear();

	CFile fi;
	{
		CString strBoxf = OSP::OSP_JoinPath(GetWorkDirAt(m_strAssets), "DMDC2B01.bin");
		if (!fi.Open(strBoxf, 0 |CFile::modeRead)) {
			strBoxf = GetNeighborFile("DMDC2B01.bin");
			if (!fi.Open(strBoxf, 0 |CFile::modeRead)) return false;
		}
	}

	DWORD key;

	TRY {
		CArchive ar(&fi, CArchive::load);
		while (true) {
			if (splash_frame != NULL) {
				splash_frame->m_horz.nCur = (int)fi.GetPosition();
				splash_frame->m_horz.nMax = (int)fi.GetLength();
				splash_frame->m_horz.Invalidate();
				splash_frame->UpdateWindow();
			}

			ar >> key;
			if (key == (DWORD)-1)
				break;
			DWORD size;
			ar >> size;
			if (size > 200000)
				return false;
			SizeBuff fv;
			if (!fv.Alloc(size))
				return false;
			if (ar.Read(fv.GetData(), size) != size)
				return false;
			if (key & 0x8000) {
				if (!m_bmovec.LoadMfRaw(key, fv)) {
					printf("");
					//return false;
				}
			} else {
				if (!m_bmovec.LoadMfBMP(key, fv)) {
					printf("");
					//return false;
				}
			}
			ar.Flush();
		}

		ar.Close();
		fi.Close();
		return true;
	} CATCH_ALL(e) {
		printf("");
	} END_CATCH_ALL;

	return false;
}

void CExp00069App::OnOptReloadOrnates() 
{
	BOOL f = LoadOrnates2();

	CWnd *p = AfxGetMainWnd();
	if (p) p->Invalidate();

	if (!f) AfxMessageBox(IDS_STR_FAIL_LOAD_ORNATES, 0 |MB_ICONEXCLAMATION);
}

bool CExp00069App::LoadDic()
{
	CWaitCursor wc;

	int nCount = 0, nDone = 0;
	nCount++, nDone += CDM2NameDic::GetInstance().Load(static_cast<LPCTSTR>(GetWorkDirAt(m_strAssets))) ? 1 : 0;
	return nDone == nCount;
}

void CExp00069App::OnOptReloadDic() 
{
	bool f = LoadDic();

	if (!f) AfxMessageBox(IDS_STR_FAIL_LOAD_NAME_TBL, 0 |MB_ICONEXCLAMATION);
}

void CExp00069App::OnHelpShowcase() 
{
	CAboutDlg aboutDlg;
	aboutDlg.m_iDefSel = 1;
	aboutDlg.DoModal();
}

void CExp00069App::OnHelpFindervw() 
{
	CAboutDlg aboutDlg;
	aboutDlg.m_iDefSel = 3;
	aboutDlg.DoModal();
}

CString CExp00069App::GetWorkDirAt(CString suffix) const {
	return OSP::OSP_JoinPath(GetWorkDir(), suffix);
}

void CExp00069App::SetAssets(CString s) {
	ProfStr::SaveProfileStringTo("DMDC2", "Assets", (m_strAssets = s), GetCnf());
	OnOptReloadDic();
	OnOptReloadOrnates();
}
