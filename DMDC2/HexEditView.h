#if !defined(AFX_HEXEDITVIEW_H__7D1ECBDA_FC07_4D48_B9CD_F13B431A4755__INCLUDED_)
#define AFX_HEXEDITVIEW_H__7D1ECBDA_FC07_4D48_B9CD_F13B431A4755__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HexEditView.h : ヘッダー ファイル
//

#include "HexEditCB.h"

/////////////////////////////////////////////////////////////////////////////
// CHexEditView ビュー

class CHexEditView : public CView
{
	// 
	CFont m_fontText;
	// 
	TEXTMETRIC m_tmText;
	// 
	CByteArray m_arrData;
	// 
	UINT m_iOffset;
	// 
	UINT m_iCursor;
	// 
	BOOL m_bCaretState;
	// 
	CRect m_rcBarTop, m_rcBarLeft, m_rcWork;
	// 
	BOOL m_bOverwrite;
	// 
	BOOL m_iLaterBit;
	// 
	BOOL m_bChrPane;
	// 
	CHexEditCB *m_cb;
	// 
	DWORD m_nCookie;
	// 
	CRect m_rcVp, m_rcVpRange;
	// 
	UINT m_iSelFrom;

	// 
	void DoDraw(CDC &dc);
	// 
	void ActivateCaret(BOOL bShow);
	// 
	void Resize();
	// 
	void RefreshCaretPos();
	// 
	inline UINT GetDataSize() { return m_arrData.GetSize(); }
	// 
	void RecalcVisibleRange()
	{
		CRect rcUni(m_rcBarLeft.left, m_rcWork.top, m_rcWork.right, m_rcWork.bottom);
		const int cxUnit = zeroTo1(m_tmText.tmAveCharWidth);
		const int cyUnit = zeroTo1(m_tmText.tmHeight + m_tmText.tmInternalLeading + m_tmText.tmExternalLeading);
		int cx = (rcUni.Width()) / cxUnit;
		int cy = (rcUni.Height()) / cyUnit;
		m_rcVp.right = m_rcVp.left + cx;
		m_rcVp.bottom = m_rcVp.top + cy;
	}
	// 
	void BinaryResize()
	{
		InvalidateRect(NULL);
		UINT n = GetDataSize();
		m_rcVpRange.bottom = (n + 16) / 16;
		RecalcScroll();
	}
	// 
	void RecalcScroll()
	{
		CPoint pointNew;
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_DISABLENOSCROLL|SIF_PAGE|SIF_POS|SIF_RANGE;

		si.nMin = m_rcVpRange.left;
		si.nMax = m_rcVpRange.right;
		si.nPage = m_rcVp.Width();
		si.nPos = m_rcVp.left;
		si.nPos = __min(si.nPos, si.nMax - (int)si.nPage);
		si.nPos = __max(si.nPos, si.nMin);
		si.nMax = __max(si.nMin, si.nMax - 1);
		SetScrollInfo(SB_HORZ, &si);
		pointNew.x = si.nPos;

		si.nMin = m_rcVpRange.top;
		si.nMax = m_rcVpRange.bottom;
		si.nPage = m_rcVp.Height();
		si.nPos = m_rcVp.top;
		si.nPos = __min(si.nPos, si.nMax - (int)si.nPage);
		si.nPos = __max(si.nPos, si.nMin);
		si.nMax = __max(si.nMin, si.nMax - 1);
		SetScrollInfo(SB_VERT, &si);
		pointNew.y = si.nPos;

		m_rcVp.right -= m_rcVp.left;
		m_rcVp.bottom -= m_rcVp.top;
		m_rcVp.right += (m_rcVp.left = pointNew.x);
		m_rcVp.bottom += (m_rcVp.top = pointNew.y);
	}
	// 
	void SetScroll(int x, int y)
	{
		int ox = m_rcVp.left, oy = m_rcVp.top;

		m_rcVp -= m_rcVp.TopLeft();
		m_rcVp += CPoint(x, y);

		RecalcScroll();

		m_iOffset = m_rcVp.top * 16;

		if (ox != m_rcVp.left || oy != m_rcVp.top) {
			InvalidateRect(NULL);

			Resize();
			RefreshCaretPos();
		}
	}
	// 
	void SetViewportVisible()
	{
		int y = (m_iCursor) / 16;
		if (m_rcVp.bottom <= y) {
			y = y - m_rcVp.Height() + 1;
		} else if (y < m_rcVp.top) {

		} else {
			y = m_rcVp.top;
		}
		int x;
		if (m_bChrPane) {
			x = 10 + 51 + ((m_iCursor) % 16);
			if (m_rcVp.right <= x) {
				x = x - m_rcVp.Width() + 1;
			} else if (x < m_rcVp.left) {

			} else {
				x = m_rcVp.left;
			}
		} else {
			x = 11 + ((m_iCursor) % 16) * 3;
			if (m_rcVp.right <= x) {
				x = x - m_rcVp.Width() + 1 + 1;
			} else if (x < m_rcVp.left) {

			} else {
				x = m_rcVp.left;
			}
		}
		SetScroll(x, y);
	}
	// 
	BOOL KillSelected()
	{
		if (m_iSelFrom < m_iCursor) {
			UINT n = m_iCursor - m_iSelFrom;
			m_arrData.RemoveAt(m_iSelFrom, n);

			m_iCursor = m_iSelFrom;
		} else if (m_iCursor < m_iSelFrom) {
			UINT n = m_iSelFrom - m_iCursor;
			m_arrData.RemoveAt(m_iCursor, n);
			m_iSelFrom = m_iCursor;
		} else {
			return FALSE;
		}
		return TRUE;
	}
	// 
	inline int zeroTo1(int i) { if (i == 0) return 1; return i; }

public:
	// 
	void SetBinaryData(const CByteArray &arr, CHexEditCB *pCB, DWORD nCookie)
	{
		m_nCookie = nCookie;
		m_cb = pCB;
		m_arrData.Copy(arr);

		BinaryResize();
		RefreshCaretPos();
	}
	// 
	void GetBinaryData(CByteArray &arr)
	{
		arr.Copy(m_arrData);
	}

protected:
	CHexEditView();           // 動的生成に使用されるプロテクト コンストラクタ
	DECLARE_DYNCREATE(CHexEditView)

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CHexEditView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドしました。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CHexEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CHexEditView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnFileApply();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_HEXEDITVIEW_H__7D1ECBDA_FC07_4D48_B9CD_F13B431A4755__INCLUDED_)
