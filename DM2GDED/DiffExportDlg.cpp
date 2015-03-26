// DiffExportDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "DiffExportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const DWORD nDefOpenFlags = 0
	|OFN_ENABLESIZING
	|OFN_EXPLORER
	|OFN_FILEMUSTEXIST
	|OFN_HIDEREADONLY
	|OFN_LONGNAMES
	|OFN_PATHMUSTEXIST
	;

/////////////////////////////////////////////////////////////////////////////
// CB4F

class CB4F
{
	// 
	static int CALLBACK fnBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
	{
		switch (uMsg) {
		case BFFM_INITIALIZED: 
			SendMessage(hwnd, BFFM_SETSELECTION, (WPARAM)FALSE, lpData);
			break;
		}
		return 0;
	}

public:
	// 
	CString m_strFolder;
	// 
	CString m_strTitle;
	// 
	CString m_strCurDir;

	// 
	void SetCurDir()
	{
		TCHAR tcCurDir[MAX_PATH] = {""};
		GetCurrentDirectory(MAX_PATH, tcCurDir);
		m_strCurDir = tcCurDir;
	}
	// 
	bool DoModal()
	{
		bool fRet = false;

		HRESULT hr;
		do {
			IShellFolderPtr pshf;
			if (FAILED(hr = SHGetDesktopFolder(&pshf))) break;

			LPITEMIDLIST pidl = NULL;
			if (FAILED(hr = pshf->ParseDisplayName(GetActiveWindow(), NULL, _bstr_t(m_strCurDir), NULL, &pidl, NULL))) break;

			TCHAR tcDir[MAX_PATH] = {""}, tcDispName[MAX_PATH] = {""};
			BROWSEINFO bi;
			ZeroMemory(&bi, sizeof(bi));
			bi.hwndOwner = GetActiveWindow();
			bi.pszDisplayName = tcDispName;
			bi.lpszTitle = m_strTitle;
			bi.ulFlags = 0
				|BIF_EDITBOX |BIF_RETURNONLYFSDIRS
#if WINVER >= 0x501
				|BIF_NEWDIALOGSTYLE |BIF_USENEWUI
#endif
				;
			bi.lpfn = fnBrowseCallbackProc;
			bi.lParam = (LPARAM)pidl;

			LPITEMIDLIST pidlSel = SHBrowseForFolder(&bi);

			m_strFolder.Empty();

			if (pidlSel != NULL) {
				if (SUCCEEDED(hr = SHGetPathFromIDList(pidlSel, tcDir))) {
					m_strFolder = tcDir;
					fRet = true;
				}
			}

			if (pidl) CoTaskMemFree(pidl);
			if (pidlSel) CoTaskMemFree(pidlSel);
		} while (false);

		return fRet;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CDiffExportDlg ダイアログ

CDiffExportDlg::CDiffExportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDiffExportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiffExportDlg)
	m_strFileMod = _T("");
	m_strFileOrg = _T("");
	m_strOutput = _T("");
	//}}AFX_DATA_INIT

#ifdef _DEBUG
	m_strFileOrg = _T("H:\\Data\\TEMP\\Dev\\DM\\DM2PCAT\\DATA\\GRAPHICS.DAT");
	m_strOutput = _T("H:\\Data\\TEMP\\Dev\\DM\\TESTz\\Diff");
#endif
}


void CDiffExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiffExportDlg)
	DDX_Text(pDX, IDC_EDIT_FILE_MOD, m_strFileMod);
	DDX_Text(pDX, IDC_EDIT_FILE_ORG, m_strFileOrg);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_strOutput);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiffExportDlg, CDialog)
	//{{AFX_MSG_MAP(CDiffExportDlg)
	ON_BN_CLICKED(IDC_BUTTON_REF_ORG, OnButtonRefOrg)
	ON_BN_CLICKED(IDC_BUTTON_REF_OUT, OnButtonRefOut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiffExportDlg メッセージ ハンドラ

void CDiffExportDlg::OnButtonRefOrg() 
{
	if (!UpdateData()) return;

	CFileDialog wndDlg(TRUE, _T("dat"), m_strFileOrg, nDefOpenFlags, "GRAPHICS.DAT (*.dat)|*.dat||", this);

	if (wndDlg.DoModal() == IDOK) {
		m_strFileOrg = wndDlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CDiffExportDlg::OnButtonRefOut() 
{
	if (!UpdateData()) return;

	CB4F wndDlg;

	wndDlg.m_strTitle.LoadString(IDS_STR_SEL_EXPORT_DEST);
	wndDlg.m_strCurDir = m_strOutput;

	if (wndDlg.m_strCurDir.IsEmpty()) wndDlg.SetCurDir();

	if (wndDlg.DoModal()) {
		m_strOutput = wndDlg.m_strFolder;
		UpdateData(FALSE);
	}
}
