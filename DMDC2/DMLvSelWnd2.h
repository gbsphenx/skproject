#if !defined(AFX_DMLVSELWND2_H__CD9DCFCB_1844_4421_8FBB_9C321F1A647B__INCLUDED_)
#define AFX_DMLVSELWND2_H__CD9DCFCB_1844_4421_8FBB_9C321F1A647B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMLvSelWnd2.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDMLvSelWnd2 ウィンドウ

class CDMLvSelWnd2 : public CControlBar
{
	// 
	enum {
		otyL,
		otyLL,
		otyBox,
		otyRR,
		otyR,
	};
	// 
	struct Obj {
		// 
		int oty;
		// 
		CRect rc;
		// 
		CString strText;
		// 
		int iIdx;

		// 
		Obj() { }
		// 
		Obj(int oty, CRect rc): oty(oty), rc(rc), iIdx(-1) { }
		// 
		Obj(int oty, CRect rc, LPCSTR pszText, int iIdx): oty(oty), rc(rc), strText(pszText), iIdx(iIdx) { }
	};
	// 
	typedef CList<Obj, Obj> ObjList;

	// 
	TEXTMETRIC m_tm;
	// 
	CFont m_fontNice;
	// 
	int m_iOffset;
	// 
	int m_nItems;
	// 
	ObjList m_list;
	// 
	bool m_fHorz;
	// 
	CSize m_sizeBox;
	// 
	CRect m_rcWc;
	// 
	CRect m_rcBorder;
	// 
	CRect m_rcUse;
	// 
	int m_cxBorder;
	// 
	int m_cxBox;
	// 
	int m_nBoxItems;
	// 
	int m_nUserItems;
	// 
	int m_iSel;

	// 
	void Recalc();
	// 
	void Resized();
	// 
	void Reassign();
	// 
	void SendNotifyToParent(UINT nCode)
	{
		CWnd *p;
		if (p = GetParent()) {
			p->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), nCode), (LPARAM)(HWND)*this);
		}
	}
	// 
	int ValidateSel(int iSel)
	{
		return _MAX(0, _MIN(m_nItems - 1, iSel));
	}
	// 
	int ValidateOffset(int iOffset)
	{
		return _MAX(0, _MIN(m_nItems - m_nUserItems, iOffset));
	}

public:
	// 
	CDMLvSelWnd2();
	// 
	void SetCount(int n);
	// 
	int GetPos();
	// 
	int GetCount();
	// 
	void SetPos(int iPos);
	// 
	BOOL Create(CWnd *p, DWORD nStyle, DWORD nBarStyle, UINT nID)
	{
		m_dwStyle = nBarStyle;
		if (CControlBar::Create(
			AfxRegisterWndClass(0 |CS_HREDRAW|CS_VREDRAW, LoadCursor(NULL, IDC_ARROW), NULL, NULL),
			"",
			nStyle,
			CRect(0, 0, 0, 0),
			p,
			nID
			)
		) {
			return TRUE;
		}
		return FALSE;
	}

	// 
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
	{

	}
	// 
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDMLvSelWnd2)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CDMLvSelWnd2();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDMLvSelWnd2)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DMLVSELWND2_H__CD9DCFCB_1844_4421_8FBB_9C321F1A647B__INCLUDED_)
