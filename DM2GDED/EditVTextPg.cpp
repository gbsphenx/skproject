// EditVTextPg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "EditVTextPg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditVTextPg ダイアログ


void CEditVTextPg::Construction()
{
	//{{AFX_DATA_INIT(CEditVTextPg)
	m_strText = _T("");
	//}}AFX_DATA_INIT
}


void CEditVTextPg::DoDataExchange(CDataExchange* pDX)
{
	CEditVPg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditVTextPg)
	DDX_Text(pDX, IDC_EDIT_VAL, m_strText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditVTextPg, CEditVPg)
	//{{AFX_MSG_MAP(CEditVTextPg)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditVTextPg メッセージ ハンドラ

int CEditVTextPg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	m_strText = m_strInitText;

	if (CEditVPg::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}
