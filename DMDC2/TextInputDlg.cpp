// TextInputDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "TextInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextInputDlg ダイアログ


CTextInputDlg::CTextInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTextInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTextInputDlg)
	m_strText = _T("");
	//}}AFX_DATA_INIT
}


void CTextInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextInputDlg)
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_wndEdit);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_strText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextInputDlg, CDialog)
	//{{AFX_MSG_MAP(CTextInputDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextInputDlg メッセージ ハンドラ

BOOL CTextInputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	LOGFONT lf;
	ReadFontTemplateFrom(lf, IDS_FONTTEMP_TEXTINPUT, "Courier;;;;;;;;;;;;;1");
	VERIFY(fontMe.CreateFontIndirect(&lf));
	m_wndEdit.SetFont(&fontMe);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
