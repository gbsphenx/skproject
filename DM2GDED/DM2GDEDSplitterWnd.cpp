// DM2GDEDSplitterWnd.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2GDEDSplitterWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDSplitterWnd

CDM2GDEDSplitterWnd::CDM2GDEDSplitterWnd()
{
	m_iX = m_iY = -1;
}

BEGIN_MESSAGE_MAP(CDM2GDEDSplitterWnd, CFlatSplitterWnd)
	//{{AFX_MSG_MAP(CDM2GDEDSplitterWnd)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDSplitterWnd メッセージ ハンドラ

CWnd* CDM2GDEDSplitterWnd::GetActivePane(int* pRow, int* pCol)
{
	if (m_iX < 0 || m_iY < 0) return CFlatSplitterWnd::GetActivePane(pRow, pCol);

	if (pRow) *pRow = m_iY;
	if (pCol) *pCol = m_iX;

	return GetPane(m_iY, m_iX);
}

void CDM2GDEDSplitterWnd::SetActivePane(int row, int col, CWnd* pWnd)
{
	CWnd* pPane = pWnd == NULL ? GetPane(row, col) : pWnd;
	if (pPane->IsKindOf(RUNTIME_CLASS(CWnd)))
	{
		m_iX = col;
		m_iY = row;

		pPane->SetFocus();
	}
	else
	{
		TRACE0("Warning: Next pane is not a view - calling SetFocus.\n");
		pPane->SetFocus();
	}
}
