// DM2GDED.cpp : アプリケーション用クラスの機能定義を行います。
//

#include "stdafx.h"
#include "DM2GDED.h"

#include "MainFrm.h"
#include "DM2GDEDDoc.h"
#include "DM2GDEDView.h"
#include "CustRichEditCtrl.h"
#include "OSP.h"
#include "SplitStr.h"
#include "ChildFrame.h"
#include "OpenFTQueryFileDlg.h"
#include "DM2GDEDTeView.h"
#include "afxwin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace OSP;

inline void convTo(PALETTEENTRY &r, COLORREF clr)
{
	r.peBlue = GetBValue(clr);
	r.peGreen= GetGValue(clr);
	r.peRed  = GetRValue(clr);
	r.peFlags= 0;
}

namespace
{

AFX_STATIC void AFXAPI _AfxAppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
	CDocTemplate* pTemplate, CString* pstrDefaultExt)
{
	ASSERT_VALID(pTemplate);
	ASSERT_KINDOF(CDocTemplate, pTemplate);

	CString strFilterExt, strFilterName;
	if (pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt) &&
	 !strFilterExt.IsEmpty() &&
	 pTemplate->GetDocString(strFilterName, CDocTemplate::filterName) &&
	 !strFilterName.IsEmpty())
	{
		// a file based document template - add to filter list
		ASSERT(strFilterExt[0] == '.');
		if (pstrDefaultExt != NULL)
		{
			// set the default extension
			*pstrDefaultExt = ((LPCTSTR)strFilterExt) + 1;  // skip the '.'
			ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(*pstrDefaultExt);
			ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
		}

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += (TCHAR)'\0';  // next string please
		filter += (TCHAR)'*';
		filter += strFilterExt;
		filter += (TCHAR)'\0';  // next string please
		ofn.nMaxCustFilter++;
	}
}

};

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDDocManager

class CDM2GDEDDocManager : public CDocManager
{
	// 
	BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate)
	{
		COpenFTQueryFileDlg wndExt(bOpenFileDialog);
		CFileDialog wndDlg(bOpenFileDialog);

		CFileDialog &dlgFile = bOpenFileDialog ? (CFileDialog &)wndExt : (CFileDialog &)wndDlg;

		CString title;
		VERIFY(title.LoadString(nIDSTitle));

		dlgFile.m_ofn.Flags |= lFlags;

		CString strFilter;
		CString strDefault;
		if (pTemplate != NULL)
		{
			ASSERT_VALID(pTemplate);
			_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate, &strDefault);
		}
		else
		{
			// do for all doc template
			POSITION pos = m_templateList.GetHeadPosition();
			BOOL bFirst = TRUE;
			while (pos != NULL)
			{
				CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
				_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate,
					bFirst ? &strDefault : NULL);
				bFirst = FALSE;
			}
		}

		// append the "*.*" all files filter
		CString allFilter;
		VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
		strFilter += allFilter;
		strFilter += (TCHAR)'\0';   // next string please
		strFilter += _T("*.*");
		strFilter += (TCHAR)'\0';   // last string
		dlgFile.m_ofn.nMaxCustFilter++;

		dlgFile.m_ofn.lpstrFilter = strFilter;
		dlgFile.m_ofn.lpstrTitle = title;
		dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

		int nResult = dlgFile.DoModal();
		fileName.ReleaseBuffer();
		if (nResult == IDOK && bOpenFileDialog) GetApp()->SetUseDetective(wndExt.m_fUse ? true : false);
		return nResult == IDOK;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDApp

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

CString LoadString(UINT nID)
{
	CString str;
	str.LoadString(nID);
	return str;
}

BOOL SaveProfileStringTo(LPCTSTR pszAppName, LPCTSTR pszKey, LPCTSTR pszValue, LPCTSTR pszFileInto)
{
	if (!WritePrivateProfileString(pszAppName, pszKey, pszValue, pszFileInto))
		return FALSE;
	return TRUE;
}

BOOL LoadProfileStringFrom(LPCTSTR pszAppName, LPCTSTR pszKey, CString &strValue, LPCTSTR pszDefault, LPCTSTR pszFileIn)
{
	TCHAR tc[MAX_PATH] = {""};
	if (GetPrivateProfileString(pszAppName, pszKey, pszDefault, tc, MAX_PATH, pszFileIn) > 0) {
		strValue = tc;
		return TRUE;
	}
	return FALSE;
}

BOOL LoadProfileSectionFrom(LPCTSTR pszAppName, CString &strValue, LPCTSTR pszFileIn)
{
	const UINT cbVuff = 8192;
	TCHAR tcVuff[cbVuff] = {""};
	if (GetPrivateProfileSection(pszAppName, tcVuff, cbVuff, pszFileIn) > 0) {
		strValue = tcVuff; 
		return TRUE;
	}
	return FALSE;
}

int GetProfileIntFrom(LPCTSTR pszAppName, LPCTSTR pszKey, int nDefault, LPCTSTR pszFileIn)
{
	return GetPrivateProfileInt(pszAppName, pszKey, nDefault, pszFileIn);
}

BOOL SaveProfileIntTo(LPCTSTR pszAppName, LPCTSTR pszKey, int nValue, LPCTSTR pszFileIn)
{
	CString strVal; strVal.Format("%d", nValue);

	return WritePrivateProfileString(pszAppName, pszKey, strVal, pszFileIn);
}

LPCTSTR GetCnfFilePath()
{
	return GetApp()->GetCnfFilePath();
}

int CalcWidthOfUIText(int cx)
{
	CFont font;
	font.CreateStockObject(DEFAULT_GUI_FONT);

	CClientDC screen(NULL);
	CFont *pFontOld = screen.SelectObject(&font);
	CSize size = screen.GetTextExtent(
		"Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s",
		cx
		);
	screen.SelectObject(pFontOld);
	return size.cx;
}

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDApp

BEGIN_MESSAGE_MAP(CDM2GDEDApp, CWinApp)
	//{{AFX_MSG_MAP(CDM2GDEDApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_OPT_USE_DETECTIVE, OnOptUseDetective)
	ON_UPDATE_COMMAND_UI(ID_OPT_USE_DETECTIVE, OnUpdateOptUseDetective)
	//}}AFX_MSG_MAP
	// 標準のファイル基本ドキュメント コマンド
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDApp クラスの構築

CDM2GDEDApp::CDM2GDEDApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CDM2GDEDApp オブジェクト

CDM2GDEDApp theApp;

CDM2GDEDApp *GetApp() { return &theApp; }

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDApp クラスの初期化

BOOL CDM2GDEDApp::InitInstance()
{
	if (!AfxInitRichEdit())
		return FALSE;
	if (!AfxOleInit())
		return FALSE;
	HRESULT hr;
	if (FAILED(hr = CoInitialize(NULL)))
		return FALSE;

	InitCommonControls();

	m_strCnf = OSP_JoinPath(OSP_GetDir(OSP_GetModuleFileName()), "DM2GDED.ini");

	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さく
	// したければ以下の特定の初期化ルーチンの中から不必要なもの
	// を削除してください。

	// 設定が保存される下のレジストリ キーを変更します。
	// TODO: この文字列を、会社名または所属など適切なものに
	// 変更してください。
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings(_AFX_MRU_MAX_COUNT);  // 標準の INI ファイルのオプションをローﾄﾞします (MRU を含む)

	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。

	CDocTemplate* pDocTemplate;

	pDocTemplate = new CMultiDocTemplate(
		IDR_GDEDCTX,
		RUNTIME_CLASS(CDM2GDEDDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CDM2GDEDTeView));
	AddDocTemplate(pDocTemplate);

	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// DDE、file open など標準のシェル コマンドのコマンドラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	RevertCnf(true);

	if (cmdInfo.m_nShellCommand != CCommandLineInfo::FileNew) {
		if (!ProcessShellCommand(cmdInfo))
			return FALSE;
	}

	// メイン ウィンドウが初期化されたので、表示と更新を行います。
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

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CCustRichEditCtrl	m_wndEdit;
	CCustRichEditCtrl	m_wndChanges;
	CStatic m_wndVer;
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_RICHEDIT_THX, m_wndEdit);
	DDX_Control(pDX, IDC_RICHEDIT_CHANGES, m_wndChanges);
	DDX_Control(pDX, IDC_STATIC_VER, m_wndVer);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

DWORD CALLBACK fnEditStreamCallback(DWORD nCookie, LPBYTE pbBuff, LONG cbBuff, LONG *pcb)
{
	CMemFile *f = (CMemFile *)nCookie;
	*pcb = f->Read(pbBuff, cbBuff);
	return 0;
}

// ダイアログを実行するためのアプリケーション コマンド
void CDM2GDEDApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	{
		CMemFile file;
		HRSRC hRes = FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_RTF_ABOUT), "RTF");
		if (hRes) {
			HGLOBAL hMem = LoadResource(AfxGetInstanceHandle(), hRes);
			if (hRes) {
				LPVOID pvData = LockResource(hMem);
				DWORD cbData = SizeofResource(AfxGetInstanceHandle(), hRes);
				file.Write(pvData, cbData);
				file.SeekToBegin();
			}
		}
		m_wndEdit.SendMessage(EM_AUTOURLDETECT, TRUE, 0);
		EDITSTREAM es;
		es.dwCookie = (DWORD)&file;
		es.dwError = 0;
		es.pfnCallback = fnEditStreamCallback;
		m_wndEdit.StreamIn(SF_RTF, es);
		m_wndEdit.SetEventMask(m_wndEdit.GetEventMask() | ENM_LINK);
	}

	{
		CMemFile file;
		HRSRC hRes = FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_RTF_CHANGES), "RTF");
		if (hRes) {
			HGLOBAL hMem = LoadResource(AfxGetInstanceHandle(), hRes);
			if (hRes) {
				LPVOID pvData = LockResource(hMem);
				DWORD cbData = SizeofResource(AfxGetInstanceHandle(), hRes);
				file.Write(pvData, cbData);
				file.SeekToBegin();
			}
		}
		m_wndChanges.SendMessage(EM_AUTOURLDETECT, TRUE, 0);
		EDITSTREAM es;
		es.dwCookie = (DWORD)&file;
		es.dwError = 0;
		es.pfnCallback = fnEditStreamCallback;
		m_wndChanges.StreamIn(SF_RTF, es);
		m_wndChanges.SetEventMask(m_wndChanges.GetEventMask() | ENM_LINK);
	}

#pragma comment(lib, "Version.lib")
	{
		// DM2GDED DM2_EXTENDED_DATABASE Rev.* ... ****-**-**
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

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDApp メッセージ ハンドラ

BOOL CDM2GDEDApp::OnIdle(LONG lCount) 
{
	int nMore = 0;

	UINT i, n = m_listIdleTar.GetSize();
	for (i = 0; i < n; i++) {
		nMore += (m_listIdleTar[i]->OnIdleNotify()) ? 1 : 0;
	}
	
	return CWinApp::OnIdle(lCount) || nMore;
}

void CDM2GDEDApp::RegisterIdleCB(COnIdle *pCB)
{
	m_listIdleTar.Add(pCB);
}

void CDM2GDEDApp::RevokeIdleCB(COnIdle *pCB)
{
	UINT i, n = m_listIdleTar.GetSize();
	for (i = 0; i < n; i++) {
		if (m_listIdleTar[i] == pCB) {
			m_listIdleTar.RemoveAt(i);
			i--, n--;
		}
	}
}

int CDM2GDEDApp::ExitInstance() 
{
	RevertCnf(false);

	for (; !m_temp.IsEmpty(); ) DeleteFile(m_temp.RemoveHead());
	
	return CWinApp::ExitInstance();
}

void CDM2GDEDApp::OnOptUseDetective() { m_fUseDetective = !m_fUseDetective; }

void CDM2GDEDApp::OnUpdateOptUseDetective(CCmdUI* pCmdUI) { pCmdUI->SetCheck(m_fUseDetective); }

void CDM2GDEDApp::RevertCnf(bool fRevert)
{
	if (fRevert) {
		m_fUseDetective = GetProfileIntFrom("DM2GDED", "detective_on_open", 1, m_strCnf) ? true : false;
	} else {
		SaveProfileIntTo("DM2GDED", "detective_on_open", m_fUseDetective ? 1 : 0, m_strCnf);
	}
}

BOOL CDM2GDEDApp::InitApplication() 
{
	m_pDocManager = new CDM2GDEDDocManager();
	
	return CWinApp::InitApplication();
}
