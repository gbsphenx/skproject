// Exp00069View.h : CExp00069View クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXP00069VIEW_H__AB9FC1DE_C8E0_43E5_A6D9_7F821CFFFA4A__INCLUDED_)
#define AFX_EXP00069VIEW_H__AB9FC1DE_C8E0_43E5_A6D9_7F821CFFFA4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DIBits.h"
#include "DMCITree.h"
#include "ExtBM.h"

class CExp00069View : public CView, CDMEdCtxCB
{
	// 
	struct HLATSTAT
	{
		// 
		DWORD nTickStart;
		// 
		CPoint cpTar;
		// 
		CRect rcTar;
		// 
		CRect rcInvalid;
		// 
		BOOL bDisable;
	};

	// 
	UINT m_iLv;
	// 
	CSize m_sizeAtlas;
	// 
	CFont m_fontNice;
	// 
	void DoDraw(CDC &dc);
	// 
	CDIBits m_bmTile[3];
	// 
	UINT m_nMaskCapture;
	// 
	UINT m_iTile;
	// 
	CRect m_rcVp;
	// 
	CRect m_rcVpRange;
	// 
	BOOL m_bSelTile;
	// 
	AtlasPos_t m_aSelPos;
	// 
	HLATSTAT m_hlat;
	// 
	BOOL m_bForceHexTile;
	// 
	CDMEdCtx *m_pEdCtx;

	// 
	BOOL DoSetMapBit(UINT x, UINT y);
	// 
	CSize CalcSizeCell()
	{
		int nx = 16 * (1 << m_iTile);
		return CSize(nx, nx);
	}
	// 
	CPoint CalcBitPtFrom(CPoint pt)
	{
		int nx = CalcSizeCell().cx;
		pt.x /= nx;
		pt.y /= nx;
		pt += m_rcVp.TopLeft();
		if (pt.x < 0 || pt.y < 0 || pt.x >= 32 || pt.y >= 32)
			return CPoint(-1, -1);
		return pt;
	}
	// 
	CRect CalcRectForAtlasPos(AtlasPos_t pos)
	{
		return CalcRectForAtlasPos(CPoint(pos.Y(), pos.X()));
	}
	// 
	CRect CalcRectForAtlasPos(CPoint ptAt)
	{
		int nx = CalcSizeCell().cx;
		ptAt.x -= m_rcVp.left;
		ptAt.y -= m_rcVp.top;
		ptAt.x *= nx;
		ptAt.y *= nx;
		return CRect(ptAt, CalcSizeCell());
	}
	// 
	void RedrawMapBit(UINT x, UINT y)
	{
		x -= m_rcVp.left;
		y -= m_rcVp.top;
		CSize size = CalcSizeCell();
		CRect rc(size.cx*(x-1), size.cy*(y-1), size.cx*(x+2), size.cy*(y+2));
		InvalidateRect(&rc);
	}
	// 
	void RecalcScroll();
	// 
	void ScrollSetPos(int x, int y);
	// 
	void ScrollUpdate();
	// 
	void SelectedNewMap()
	{
		UINT nLvs = GetDDAT().CntMap();
		if (nLvs <= m_iLv) {
			m_rcVpRange.SetRectEmpty();
			m_sizeAtlas.cx = m_sizeAtlas.cy = 0;
			return;
		}
		CSize size;
		HRESULT hr;
		LevelDesc_t desc;
		VERIFY(SUCCEEDED(hr = GetDDAT().GetMapDescAt(m_iLv, desc)));
		m_rcVpRange.left = 
		m_rcVpRange.top = 0;
		m_rcVpRange.right = (m_sizeAtlas.cy = desc.cyLv);
		m_rcVpRange.bottom = (m_sizeAtlas.cx = desc.cxLv);
	}
	// 
	CDDAT &GetDDAT()
	{
		return GetDocument()->GetDDAT();
	}
	// 
	void DisableHlat()
	{
		m_hlat.bDisable = TRUE;
	}
	// 
	bool CheckIntegrity();
	// 
	CDMCIIf *GetDMCIIf();

	// 
	virtual void OnDMUpdate(UINT nHint, void *pHint);

protected: // シリアライズ機能のみから作成します。
	CExp00069View();
	DECLARE_DYNCREATE(CExp00069View)

// アトリビュート
public:
	CExp00069Doc* GetDocument();

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CExp00069View)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CExp00069View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CExp00069View)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnBtnB10();
	afx_msg void OnUpdateBtnB10(CCmdUI* pCmdUI);
	afx_msg void OnEditLvdesc();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnViewForceHexTile();
	afx_msg void OnUpdateViewForceHexTile(CCmdUI* pCmdUI);
	afx_msg void OnEditDmfiledesc();
	afx_msg void OnEditRepair5thbit();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditOptimize();
	afx_msg void OnEditCheckIntegrity();
	afx_msg void OnEditUnduplicateDb();
	//}}AFX_MSG
	afx_msg void OnViewSize1x(UINT nID);
	afx_msg void OnUpdateViewSize1x(CCmdUI* pCmdUI);
	afx_msg void OnBtnB0(UINT nID);
	afx_msg void OnUpdateBtnB0(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
public:
};

#ifndef _DEBUG  // Exp00069View.cpp ファイルがデバッグ環境の時使用されます。
inline CExp00069Doc* CExp00069View::GetDocument()
   { return (CExp00069Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EXP00069VIEW_H__AB9FC1DE_C8E0_43E5_A6D9_7F821CFFFA4A__INCLUDED_)
