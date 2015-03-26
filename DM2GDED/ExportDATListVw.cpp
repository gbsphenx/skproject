// ExportDATListVw.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "DM2GDED.h"
#include "ExportDATListVw.h"
#include "ExportDATDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDW_GRID 0x0100

/////////////////////////////////////////////////////////////////////////////
// CExportDATListVw

IMPLEMENT_DYNCREATE(CExportDATListVw, CView)

CExportDATListVw::CExportDATListVw(): m_pEdCtx(NULL)
{

}

CExportDATListVw::~CExportDATListVw()
{
	InitEdCtx(NULL);
}

BEGIN_MESSAGE_MAP(CExportDATListVw, CView)
	//{{AFX_MSG_MAP(CExportDATListVw)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportDATListVw �`��

void CExportDATListVw::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���̈ʒu�ɕ`��p�̃R�[�h��ǉ����Ă�������
}

/////////////////////////////////////////////////////////////////////////////
// CExportDATListVw �f�f

#ifdef _DEBUG
void CExportDATListVw::AssertValid() const
{
	CView::AssertValid();
}

void CExportDATListVw::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CExportDATListVw ���b�Z�[�W �n���h��

void CExportDATListVw::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	switch (lHint) {
	case PM_V_SEL_EXPTREEITEM:
		{
			TSelExpTreeItem *pAny = reinterpret_cast<TSelExpTreeItem *>(pHint);
			OnSelectItem(*pAny);
			break;
		}
	}
}

CExportDATDoc *CExportDATListVw::GetDocument()
{
	return static_cast<CExportDATDoc *>(CView::GetDocument());
}

void CExportDATListVw::OnSelectItem(TSelExpTreeItem &rAny)
{
	m_wndGrid.CreateList(rAny.lower, rAny.upper, TRUE);
}

void CExportDATListVw::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	InitEdCtx(GetDocument()->GetEdCtx(FALSE));

	CGDAT &core = GetGDAT();

	if (m_wndGrid.m_hWnd == NULL) {
		m_wndGrid.Create(CRect(0, 0, 0, 0), this, IDW_GRID);
		m_wndGrid.EnableSelection(FALSE);
	}

	RepositionBars(0, 0xffff, IDW_GRID);
	
}

void CExportDATListVw::InitEdCtx(CGDEdCtx *pEdCtx)
{
	if (m_pEdCtx)
		m_pEdCtx->Release();
	if (m_pEdCtx = pEdCtx)
		m_pEdCtx->AddRef();

	m_wndGrid.InitEdCtx(m_pEdCtx);
}

CGDAT &CExportDATListVw::GetGDAT()
{
	ASSERT(m_pEdCtx);
	return m_pEdCtx->GetGDAT();
}

void CExportDATListVw::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	RepositionBars(0, 0xffff, IDW_GRID);
	
}
