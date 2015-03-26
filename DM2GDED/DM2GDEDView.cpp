// DM2GDEDView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DM2GDED.h"
#include "DM2GDEDDoc.h"
#include "DM2GDEDView.h"
#include "OSP.h"
#include "F2C.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDW_GRID 0x0100

BOOL AFXAPI AfxIsDescendant(HWND hWndParent, HWND hWndChild);

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDView

IMPLEMENT_DYNCREATE(CDM2GDEDView, CView)

CDM2GDEDView::CDM2GDEDView()
	: m_pEdCtx(NULL)
{

}

CDM2GDEDView::~CDM2GDEDView()
{
	InitEdCtx(NULL);
}

BEGIN_MESSAGE_MAP(CDM2GDEDView, CView)
	//{{AFX_MSG_MAP(CDM2GDEDView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_COMMAND(ID_CTRL_TREEVIEW, OnCtrlTreeview)
	ON_COMMAND(ID_CTRL_GRIDVIEW, OnCtrlGridview)
	//}}AFX_MSG_MAP
	ON_COMMAND_EX(ID_NEXT_PANE, OnNextPane)
	ON_COMMAND_EX(ID_PREV_PANE, OnNextPane)
	ON_UPDATE_COMMAND_UI(ID_NEXT_PANE, OnUpdateNextPane)
	ON_UPDATE_COMMAND_UI(ID_PREV_PANE, OnUpdateNextPane)
	ON_COMMAND(ID_UPDATE_CURRENT_PANE, OnUpdatePane)
	ON_COMMAND(ID_FILE_EXPORT_CSV, OnFileExportCsv)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDView 描画

void CDM2GDEDView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: この位置に描画用のコードを追加してください
}

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDView 診断

#ifdef _DEBUG
void CDM2GDEDView::AssertValid() const
{
	CView::AssertValid();
}

void CDM2GDEDView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDView メッセージ ハンドラ

void CDM2GDEDView::OnGDUpdate(UINT nHint, void *pHint)
{
	if (!m_pEdCtx) return;

	switch (nHint) {
	case PM_V_RELOAD:
		break;
	case PM_V_SEL_TREE_ITEM:
		OnSelTreeItem(*(TSelTreeItem *)pHint);
		break;
	}
}

void CDM2GDEDView::InitEdCtx(CGDEdCtx *pEdCtx)
{
	if (m_pEdCtx)
		m_pEdCtx->RemoveCB(this),
		m_pEdCtx->Release();
	if (m_pEdCtx = pEdCtx)
		m_pEdCtx->AddRef(),
		m_pEdCtx->AddCB(this);

	if (m_pGrid) m_pGrid->InitEdCtx(pEdCtx);
}

CDM2GDEDDoc* CDM2GDEDView::GetDocument()
{
	return static_cast<CDM2GDEDDoc*>(CView::GetDocument());
}

void CDM2GDEDView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	InitEdCtx(GetDocument()->GetEdCtx(FALSE));

	CWnd *pParentWnd;
	if (pParentWnd = GetParent())
		pParentWnd->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
}

void CDM2GDEDView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	
	RepositionBars(0, 0xffff, 0x0100);
	
}

void CDM2GDEDView::OnSelTreeItem(TSelTreeItem &rItem)
{
	m_pGrid->CreateList(rItem.lower, rItem.upper);
}

BOOL CDM2GDEDView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
//	
//	return CView::OnEraseBkgnd(pDC);
}

BOOL CDM2GDEDView::OnNextPane(UINT nID)
{
	m_fr.RotateFocus((nID == ID_NEXT_PANE) ? +1 : -1);
	HWND hWnd = m_fr.FindRecentFocus();
	if (hWnd) ::SetFocus(hWnd);
	return TRUE;
}

void CDM2GDEDView::OnUpdateNextPane(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CDM2GDEDView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	
	HWND hWnd = m_fr.FindRecentFocus();
	if (hWnd) ::SetFocus(hWnd);
}

BOOL CDM2GDEDView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.lpszName = "List";
	
	return CView::PreCreateWindow(cs);
}

int CDM2GDEDView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CGDEdCtx *pEdCtx = GetDocument()->GetEdCtx(FALSE);

	CCreateContext *pContext = (CCreateContext *)lpCreateStruct->lpCreateParams;

	if (false
		|| !m_wndSplitter.CreateStatic(this, 1, 2, 0 |WS_CHILD |WS_VISIBLE, 0x0100)
		|| !m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CDTreeDock), CSize(200, 200), pContext)
		|| !m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CDGridCtrl), CSize(200, 200), pContext)
	) {
		return -1;
	}
	m_pVw = static_cast<CDTreeDock *>(m_wndSplitter.GetPane(0, 0));
	m_pVw->Init(pEdCtx);
	m_pVw->ModifyStyle(0, WS_TABSTOP);
	m_pGrid = static_cast<CDGridCtrl *>(m_wndSplitter.GetPane(0, 1));
	m_pGrid->EnableSelection(FALSE);
	m_pGrid->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
	m_pGrid->ModifyStyle(0, WS_TABSTOP);

	m_fr.listHwnd.push_back(m_pVw->GetSafeHwnd());
	m_fr.listHwnd.push_back(m_pGrid->GetSafeHwnd());

	return 0;
}

void CDM2GDEDView::PostNcDestroy() 
{
	if (m_pGrid) { delete m_pGrid; m_pGrid = NULL; }

	CView::PostNcDestroy();
}

void CDM2GDEDView::OnCtrlTreeview() 
{
	CWnd *pWnd = m_wndSplitter.GetPane(0, 0);
	if (pWnd && pWnd->IsWindowEnabled()) {
		pWnd->SetFocus();
	}
}

void CDM2GDEDView::OnCtrlGridview() 
{
	CWnd *pWnd = m_wndSplitter.GetPane(0, 1);
	if (pWnd && pWnd->IsWindowEnabled()) {
		pWnd->SetFocus();
	}
}

void CDM2GDEDView::OnUpdatePane()
{
	m_fr.MoveFocusTo(::GetFocus());
}

void CDM2GDEDView::OnFileExportCsv() {
	CGDAT &core = m_pEdCtx->GetGDAT();

	CFileDialog wndDlg(false, "csv", "Many.csv", OFN_ENABLESIZING|OFN_EXPLORER|OFN_LONGNAMES|OFN_PATHMUSTEXIST, _T("*.csv|*.csv||"));
	if (wndDlg.DoModal() != IDOK)
		return;

	CString strDir = OSP::OSP_GetDir(wndDlg.GetPathName());

	ExportCsv(OSP::OSP_JoinPath(strDir, _T("_Chests.Dic.csv")), 0x14);
	ExportCsv(OSP::OSP_JoinPath(strDir, _T("_Clothes.Dic.csv")), 0x11);
	ExportCsv(OSP::OSP_JoinPath(strDir, _T("_Creatures.Dic.csv")), 0x0F);
	ExportCsv(OSP::OSP_JoinPath(strDir, _T("_Heroes.Dic.csv")), 0x16);
	ExportCsv(OSP::OSP_JoinPath(strDir, _T("_Messages.Dic.csv")), 0x03);
	ExportCsv(OSP::OSP_JoinPath(strDir, _T("_Misc.Dic.csv")), 0x15);
	ExportCsv(OSP::OSP_JoinPath(strDir, _T("_Potions.Dic.csv")), 0x13);
	ExportCsv(OSP::OSP_JoinPath(strDir, _T("_Scrolls.Dic.csv")), 0x12);
	ExportCsv(OSP::OSP_JoinPath(strDir, _T("_Weapons.Dic.csv")), 0x10);

	AfxMessageBox(IDS_EXPORT_DONE, MB_ICONINFORMATION);
}

void CDM2GDEDView::ExportCsv(CString fpcsv, int v1) {
	CString strBody;
	CF2C f2c;
	f2c.InitEdCtx(m_pEdCtx);
	CGDAT &core = m_pEdCtx->GetGDAT();
	switch (v1) {
		case 0x14: // chests
		case 0x11: // clothes
		case 0x0F: // creatures
		case 0x16: // champions
		case 0x15: // misc
		case 0x13: // potions
		case 0x12: // scrolls
		case 0x10: // weapons
			for (int v2=0; v2<256; v2++) {
				strBody.AppendFormat(
					_T("\"%s\"\r\n")
					, static_cast<LPCTSTR>(f2c.FormatClass2Name(CGDEntry(v1, v2, 0, 0, 0, 0)))
					);
			}
			break;
		case 0x03: // messages
			for (int v2=0; v2<256; v2++) {
				strBody.AppendFormat(
					_T("\"%s\"\r\n")
					, static_cast<LPCTSTR>(f2c.FormatMessage(v2))
					);
			}
			break;
	}
	CFile f;
	if (f.Open(fpcsv, CFile::modeCreate|CFile::modeReadWrite)) {
		CStringA str = strBody;
		f.Write(static_cast<LPCSTR>(str), str.GetLength());
		f.Close();
	}
}
