// ExportEditCommentDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "ExportEditCommentDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{

void _Exc(CString &str1, CString &str2, bool f2To1)
{
	if (f2To1) {
		str1 = str2;
	} else {
		str2 = str1;
	}
}

}

/////////////////////////////////////////////////////////////////////////////
// CExportEditCommentDlg ダイアログ


CExportEditCommentDlg::CExportEditCommentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExportEditCommentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportEditCommentDlg)
	m_strAuthor = _T("");
	m_strComment = _T("");
	m_strDate = _T("");
	m_strName = _T("");
	m_strPlatform = _T("");
	m_strVer = _T("");
	//}}AFX_DATA_INIT
}


void CExportEditCommentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportEditCommentDlg)
	DDX_Text(pDX, IDC_EDIT_AUTHOR, m_strAuthor);
	DDX_Text(pDX, IDC_EDIT_COMMENTS, m_strComment);
	DDX_Text(pDX, IDC_EDIT_DATE, m_strDate);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_PLATFORM, m_strPlatform);
	DDX_Text(pDX, IDC_EDIT_VER, m_strVer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportEditCommentDlg, CDialog)
	//{{AFX_MSG_MAP(CExportEditCommentDlg)
	ON_BN_CLICKED(IDC_BUTTON_TODAY, OnButtonToday)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportEditCommentDlg メッセージ ハンドラ

void CExportEditCommentDlg::OnButtonToday() 
{
	if (!UpdateData()) return;

	time_t t = time(NULL);
	struct tm atm = *localtime(&t);

	m_strDate.Format("%02d/%02d/%04d"
		, 1U +atm.tm_mon
		, 0U +atm.tm_mday
		, 1900U +atm.tm_year
		);

	UpdateData(FALSE);
}

void CExportEditCommentDlg::Revert(CExportDATComment &rEx, bool fRevert)
{
	_Exc(m_strAuthor, rEx.strAuthor, fRevert);
	_Exc(m_strComment, rEx.strComment, fRevert);
	_Exc(m_strDate, rEx.strDate, fRevert);
	_Exc(m_strName, rEx.strName, fRevert);
	_Exc(m_strPlatform, rEx.strPlatform, fRevert);
	_Exc(m_strVer, rEx.strVer, fRevert);
}
