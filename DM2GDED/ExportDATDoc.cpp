// ExportDATDoc.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DM2GDED.h"
#include "ExportDATDoc.h"
#include "OSP.h"
#include "ExportDATHintDlg.h"
#include "ExportEditCommentDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace OSP;

namespace
{
	// 
	int CALLBACK fnBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
	{
		switch (uMsg) {
		case BFFM_INITIALIZED: 
			SendMessage(hwnd, BFFM_SETSELECTION, (WPARAM)FALSE, lpData);
			break;
		}
		return 0;
	}

};

/////////////////////////////////////////////////////////////////////////////

class CROStrRet : public STRRET
{
	// 
	LPITEMIDLIST pidl;

public:
	// 
	CROStrRet(LPITEMIDLIST pidl): pidl(pidl)
	{
		ZeroMemory(this, sizeof(STRRET));
	}
	// 
	~CROStrRet()
	{
		if (uType == STRRET_WSTR) CoTaskMemFree(pOleStr);
	}
	// 
	CString GetText() const
	{
		switch (uType) {
		case STRRET_CSTR: return cStr;
		case STRRET_OFFSET: return ((LPCSTR)pidl) + uOffset;
		case STRRET_WSTR: return pOleStr;
		}
		return "";
	}

};

/////////////////////////////////////////////////////////////////////////////
// CExportDATDoc

IMPLEMENT_DYNCREATE(CExportDATDoc, CDocument)

CExportDATDoc::CExportDATDoc()
{
	m_pEdCtx = NULL;
}

BOOL CExportDATDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CExportDATDoc::~CExportDATDoc()
{
	InitEdCtx(NULL);
}

void CExportDATDoc::CreateTree()
{
	if (!m_pEdCtx) return;

	CGDAT &core = m_pEdCtx->GetGDAT();
	list<CGDEntry> l;
	core.Enum(l);
	m_mapTn.clear();
	UINT c1 = -1, c2 = -1;
	while (!l.empty()) {
		CGDEntry &pos = l.front();
		if (c1 != pos.c1() || c2 != pos.c2()) {
			NodeKey k;
			k.c1 = c1 = pos.c1();
			k.c2 = c2 = pos.c2();
			m_mapTn[k] = NodeVal();
		}
		l.pop_front();
	}
}

void CExportDATDoc::DeleteTree()
{
	m_mapTn.clear();
}

void CExportDATDoc::Enum(list<NodeKey> &l)
{
	NodeMap::iterator
		iterPos = m_mapTn.begin(),
		iterEnd = m_mapTn.end();
	for (l.clear(); iterPos != iterEnd; iterPos++) l.push_back(iterPos->first);
}

void CExportDATDoc::InitEdCtx(CGDEdCtx *pEdCtx)
{
	if (m_pEdCtx) {
		m_pEdCtx->Release();
		DeleteTree();
	}
	if (m_pEdCtx = pEdCtx) {
		m_pEdCtx->AddRef();
		CreateTree();
	}
}

BEGIN_MESSAGE_MAP(CExportDATDoc, CDocument)
	//{{AFX_MSG_MAP(CExportDATDoc)
	ON_COMMAND(ID_FILE_EXPORT_NOW, OnFileExportNow)
	ON_COMMAND(ID_FILE_EXPORT_QUICK_NOW, OnFileExportQuickNow)
	ON_COMMAND(ID_EDIT_EXPORT_COMMENT, OnEditExportComment)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportDATDoc 診断

#ifdef _DEBUG
void CExportDATDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CExportDATDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CExportDATDoc シリアライズ

void CExportDATDoc::Serialize(CArchive& ar)
{
	AfxThrowNotSupportedException();
}

/////////////////////////////////////////////////////////////////////////////
// CExportDATDoc コマンド

void CExportDATDoc::OnFileExportNow() 
{
	HRESULT hr;
	do {
		IShellFolderPtr pshf;
		if (FAILED(hr = SHGetDesktopFolder(&pshf))) break;

		TCHAR tcCurDir[MAX_PATH] = {""};
		GetCurrentDirectory(MAX_PATH, tcCurDir);
		_bstr_t bstrCurDir = tcCurDir;

		LPITEMIDLIST pidl = NULL;
		if (FAILED(hr = pshf->ParseDisplayName(GetActiveWindow(), NULL, bstrCurDir, NULL, &pidl, NULL))) break;

		CString strCaption;
		TCHAR tcDir[MAX_PATH] = {""}, tcDispName[MAX_PATH] = {""};
		BROWSEINFO bi;
		ZeroMemory(&bi, sizeof(bi));
		bi.hwndOwner = GetActiveWindow();
		bi.pszDisplayName = tcDispName;
		bi.lpszTitle = (strCaption = LoadString(IDS_STR_SEL_EXPORT_DEST));
		bi.ulFlags = BIF_EDITBOX |BIF_NEWDIALOGSTYLE |BIF_RETURNONLYFSDIRS |BIF_USENEWUI;
		bi.lpfn = fnBrowseCallbackProc;
		bi.lParam = (LPARAM)pidl;

		LPITEMIDLIST pidlSel = SHBrowseForFolder(&bi);

		if (pidlSel != NULL) {
			CROStrRet aName(pidlSel);
			if (SUCCEEDED(hr = pshf->GetDisplayNameOf(pidlSel, SHGDN_FORPARSING, &aName))) {
				ExportToDir(aName.GetText());
			}
		}

		if (pidl) CoTaskMemFree(pidl);
		if (pidlSel) CoTaskMemFree(pidlSel);
	} while(false);
}

void CExportDATDoc::OnFileExportQuickNow() 
{
	CString strDir = OSP_GetTempDir("~tmp");

	ExportToDir(strDir);
}

bool CExportDATDoc::SetCheck(NodeKey k, bool fCheck)
{
	NodeMap::iterator
		iterPos = m_mapTn.find(k),
		iterEnd = m_mapTn.end();
	if (iterPos != iterEnd) {
		iterPos->second.fCheck = fCheck;
		return true;
	}
	return false;
}

BOOL CExportDATDoc::ExportToDir(LPCTSTR pszDir, bool fAllowExplore)
{
	if (TryExportToDir(pszDir)) {
		if (fAllowExplore) {
			CString strMsg;
			AfxFormatString1(strMsg, IDS_INFO_EXPLORE_EXPORT_DIR, pszDir);
			int r = AfxMessageBox(strMsg, MB_ICONINFORMATION|MB_YESNO);
			if (r == IDYES) {
				CString strCmd;
				strCmd.Format("EXPLORER.exe /E,\"%s\"", pszDir);
				WinExec(strCmd, SW_SHOW);
			}
		}
		return TRUE;
	}

	CString strMsg;
	AfxFormatString1(strMsg, IDS_ERR_EXPORT_DIR, pszDir);
	int r = AfxMessageBox(strMsg, MB_ICONEXCLAMATION);

	return FALSE;
}

bool CExportDATDoc::TryExportToDir(LPCTSTR pszDir)
{
	CExportDATHintDlg wndDlg;

	CGDAT &core = m_pEdCtx->GetGDAT();

	NodeMap::iterator
		iterPos = m_mapTn.begin(),
		iterEnd = m_mapTn.end();
	for (; iterPos != iterEnd; iterPos++) {
		NodeVal &rVal = iterPos->second;
		if (rVal.fCheck) {
			const NodeKey &rKey = iterPos->first;
			list<CGDEntry> listKey;
			core.EnumL4(listKey, rKey.c1, rKey.c2);
			wndDlg.Add4Export(listKey);
		}
	}
	if (wndDlg.ExportTo(core, m_aHintExp, pszDir)) {
		return TRUE;
	}
	return FALSE;
}

void CExportDATDoc::OnEditExportComment() 
{
	CExportEditCommentDlg wndDlg(CWnd::GetActiveWindow());
	wndDlg.Revert(m_aHintExp, true);
	int r = wndDlg.DoModal();
	if (r == IDOK) {
		wndDlg.Revert(m_aHintExp, false);
	}
}
