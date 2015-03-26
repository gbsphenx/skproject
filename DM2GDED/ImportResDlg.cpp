// ImportResDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "ImportResDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{

void FormatEntryDisp(CString &strText, const CGDEntry &pos)
{
	strText.Format("%02X %02X-%02X-%02X %02X %02X"
		, 0U+pos.c1()
		, 0U+pos.c2()
		, 0U+pos.c3()
		, 0U+pos.c4()
		, 0U+pos.c5()
		, 0U+pos.c6()
		);
}

};

/////////////////////////////////////////////////////////////////////////////
// CImportResRec

/////////////////////////////////////////////////////////////////////////////
// CImportResRecList

void CImportResRecList::appendText(LPCTSTR pszText)
{
	push_back(CImportResRec(pszText));
}

void CImportResRecList::ResAddSubject(LPCTSTR pszText)
{
	CString str;
	str.Format("==========\n%s\n", pszText);
	appendText(str);
}

void CImportResRecList::ResAddText(LPCTSTR pszText)
{
	appendText(pszText);
}

void CImportResRecList::ResAddEntryImpRes(const CGDEntry &pos, BOOL fRes)
{
	CString str;
	FormatEntryDisp(str, pos);

	CString strText;
	strText.Format(fRes
		? "Import %s --- OK\n----------"
		: "Import %s --- ERR\n----------"
		, (LPCTSTR)str
		);
	appendText(strText);
}

void CImportResRecList::ResAddUnsup8()
{
	appendText("ERR: 8-bit Compressed image not supported");
}

void CImportResRecList::ResAddKillEntry(const CGDEntry &pos, bool fFromDAT)
{
	CString str;
	FormatEntryDisp(str, pos);

	CString strText;
	strText.Format("%s removed", (LPCTSTR)str);
	push_back(CImportResRec(strText));
}

CString CImportResRecList::ResToStr()
{
	UINT n = 0;
	iterator iterPos;
	for (iterPos = begin(); iterPos != end(); iterPos++) n += iterPos->strText.GetLength() + 1;

	CString strText;
	LPTSTR pszText = strText.GetBuffer(n + 1);
	*pszText = 0;
	for (iterPos = begin(); iterPos != end(); iterPos++) strcat(pszText, iterPos->strText), strcat(pszText, "\n"), pszText = _tcschr(pszText, 0);

	strText.ReleaseBuffer();
	strText.Replace("\n", "\r\n");
	return strText;
}

/////////////////////////////////////////////////////////////////////////////
// CImportResDlg ダイアログ

CImportResDlg::CImportResDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportResDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportResDlg)
	m_strLog = _T("");
	m_strRes = _T("");
	//}}AFX_DATA_INIT
}


void CImportResDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportResDlg)
	DDX_Text(pDX, IDC_EDIT_LOG, m_strLog);
	DDX_Text(pDX, IDC_STATIC_RES, m_strRes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportResDlg, CDialog)
	//{{AFX_MSG_MAP(CImportResDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportResDlg メッセージ ハンドラ

int CImportResDlg::DoModal(LPCTSTR pszRes)
{
	m_strRes = pszRes;
	m_strLog = ResToStr();

	return CDialog::DoModal();
}
