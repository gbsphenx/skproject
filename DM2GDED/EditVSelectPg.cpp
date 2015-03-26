// EditVSelectPg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "EditVSelectPg.h"
#include "OSP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditVSelectPg ダイアログ


void CEditVSelectPg::Construction()
{
	//{{AFX_DATA_INIT(CEditVSelectPg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CEditVSelectPg::DoDataExchange(CDataExchange* pDX)
{
	CEditVPg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditVSelectPg)
	DDX_Control(pDX, IDC_LIST_SELECT, m_wndList);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) {
		int i = m_wndList.GetNextItem(-1, LVNI_SELECTED);
		if (i >= 0 && i < m_arrSels.GetSize()) {
			m_nFlags = m_arrSels[i].iVal;
		}
	} else {
		for (int i = 0; i < m_arrSels.GetSize(); i++) {
			if (m_arrSels[i].iVal == m_nFlags) {
				m_wndList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
				break;
			}
		}
	}
}


BEGIN_MESSAGE_MAP(CEditVSelectPg, CEditVPg)
	//{{AFX_MSG_MAP(CEditVSelectPg)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SELECT, OnListSelectDblClk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditVSelectPg メッセージ ハンドラ

int CEditVSelectPg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditVPg::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

BOOL CEditVSelectPg::OnInitDialog() 
{
	CEditVPg::OnInitDialog();
	
	m_il.DeleteImageList();

	VERIFY(m_il.Create(16, 16, ILC_COLOR32|ILC_MASK, 0, 10));

	m_wndList.SetImageList(&m_il, LVSIL_SMALL);

	for (int i = 0; i < m_arrSels.GetSize(); i++) {
		const Sel &sel = m_arrSels[i];
		int iImage = -1;
		CString strImage = sel.strImage;
		if (!strImage.IsEmpty()) {
			if (_taccess(strImage, 0) != 0) {
				CString strBaseDir = OSP::OSP_GetDir(OSP::OSP_GetModuleFileName());
				{
					CString strTest = strBaseDir + _T("res\\") + strImage;
					if (_taccess(strTest, 0) == 0) {
						strImage = strTest;
					}
				}
				{
					CString strTest = strBaseDir + strImage;
					if (_taccess(strTest, 0) == 0) {
						strImage = strTest;
					}
				}
			}
			HICON hIcon = reinterpret_cast<HICON>(LoadImage(NULL, strImage, IMAGE_ICON, 16, 16, LR_LOADFROMFILE|LR_VGACOLOR));
			if (hIcon != NULL) {
				iImage = m_il.Add(hIcon);
			}
		}

		m_wndList.InsertItem(i, sel.strText, iImage);
	}

	m_nFlags = m_nInitVal;

	UpdateData(FALSE);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/////////////////////////////////////////////////////////////////////////////
// CEditVSelectPg メッセージ ハンドラ

CString CEditVSelectPg::FormatDisp(WORD x)
{
	for (int i = 0; i < m_arrSels.GetSize(); i++) {
		if (m_arrSels[i].iVal == x)
			return m_arrSels[i].strText;
	}
	return _T("");
}

bool CEditVSelectPg::Parse(LPCSTR pszText, WORD &x)
{
	LPSTR pszTerm;
	x = (WORD)strtol(pszText, &pszTerm, 10);
	return (*pszTerm == 0);
}

void CEditVSelectPg::SetParm1(LPCTSTR psz)
{
	m_arrSels.RemoveAll();

	LPCTSTR pszCur = psz;
	while (true) {
		LPCTSTR pszNext = _tcschr(pszCur, '|');
		LPCTSTR pszNex2 = (pszNext == NULL) ? _tcschr(pszCur, 0) : pszNext;

		CString strPair(pszCur, pszNex2 -pszCur);

		int iSep = strPair.Find('=');
		if (iSep >= 0) {
			CString strLeft(strPair, iSep);
			Sel sel;
			sel.iVal = strtol(static_cast<LPCSTR>(strLeft), NULL, 10);
			sel.strText = static_cast<LPCSTR>(strPair) +iSep +1;
			int iAst = strLeft.Find('*');
			if (iAst >= 0) {
				sel.strImage = strLeft.Mid(iAst +1);
			}

			m_arrSels.Add(sel);
		}

		if (pszNext != NULL) {
			pszCur = pszNext +1;
			continue;
		}
		break;
	}
}

void CEditVSelectPg::OnListSelectDblClk(NMHDR *pNotifyStruct, LRESULT *result)
{
	if (m_wndList.GetSelectedCount() == 1) {
		OnOK();
	}
}
