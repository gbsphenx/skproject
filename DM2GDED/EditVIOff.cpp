// EditVIOff.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "EditVIOff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditVIOff プロパティ ページ

void CEditVIOff::Construction()
{
	//{{AFX_DATA_INIT(CEditVIOff)
	m_offx = 0;
	m_offy = 0;
	//}}AFX_DATA_INIT
}

CEditVIOff::~CEditVIOff()
{

}

void CEditVIOff::DoDataExchange(CDataExchange* pDX)
{
	CEditVPg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditVIOff)
	DDX_Control(pDX, IDC_EDIT_POS_Y, m_wndEditY);
	DDX_Control(pDX, IDC_EDIT_POS_X, m_wndEditX);
	DDX_Control(pDX, IDC_SPIN_POS_Y, m_wndSpinY);
	DDX_Control(pDX, IDC_SPIN_POS_X, m_wndSpinX);
	DDX_Text(pDX, IDC_EDIT_POS_X, m_offx);
	DDV_MinMaxInt(pDX, m_offx, -128, 127);
	DDX_Text(pDX, IDC_EDIT_POS_Y, m_offy);
	DDV_MinMaxInt(pDX, m_offy, -128, 127);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEditVIOff, CEditVPg)
	//{{AFX_MSG_MAP(CEditVIOff)
	ON_EN_CHANGE(IDC_EDIT_POS_X, OnChangeEditPosX)
	ON_EN_CHANGE(IDC_EDIT_POS_Y, OnChangeEditPosY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditVIOff メッセージ ハンドラ

BOOL CEditVIOff::OnInitDialog() 
{
	CEditVPg::OnInitDialog();
	
	m_nFlags = m_nInitVal;

	m_offx = __int8(m_nFlags >> 8);
	m_offy = __int8(m_nFlags);

	UpdateData(false);

	m_wndSpinX.SetRange(-128, 127);
	m_wndSpinY.SetRange(-128, 127);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CEditVIOff::OnChangeEditPosX() 
{
	if (!IsWindow(m_wndEditX)) return;

	CString text;
	m_wndEditX.GetWindowText(text);
	int val = max(-128, min(127, _ttoi(text)));

	m_nFlags &= 0x00ff;
	m_nFlags |= (__int8(val) << 8);
}

void CEditVIOff::OnChangeEditPosY() 
{
	if (!IsWindow(m_wndEditY)) return;

	CString text;
	m_wndEditY.GetWindowText(text);
	int val = max(-128, min(127, _ttoi(text)));

	m_nFlags &= 0xff00;
	m_nFlags |= (__int8(val) << 0);
}
