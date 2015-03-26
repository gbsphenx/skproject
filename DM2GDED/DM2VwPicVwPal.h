#if !defined(AFX_DM2VWPICVWPAL_H__14993E80_E59D_42B5_80B7_1FB8C611374F__INCLUDED_)
#define AFX_DM2VWPICVWPAL_H__14993E80_E59D_42B5_80B7_1FB8C611374F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2VwPicVwPal.h : ヘッダー ファイル
//

#include "DMGHLci.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2VwPicVwPal ウィンドウ

class CDM2VwPicVwPal : public CControlBar
{
	// 
	struct RenderContext {
		// 
		int ty;

		// 
		RenderContext()
		{
			ty = 0;
		}
	};

	// 
	COLORREF m_clrPal[3][16];
	// 
	bool m_fPalAvail[3];
	// 
	int m_iKeys[3];
	// 
	TEXTMETRIC m_tm;
	// 
	int m_cyText;
	// 
	int m_cyBox;
	// 
	int m_yTextLead;

public:
	// 
	CDM2VwPicVwPal();
	// 
	bool Create(DWORD dwExStyle, DWORD dwStyle, DWORD dwBarStyle, CWnd *pParentWnd, UINT nID)
	{
		LPCTSTR pszClassName = AfxRegisterWndClass(0 |CS_PARENTDC |CS_HREDRAW |CS_VREDRAW, GetApp()->LoadStandardCursor(IDC_ARROW), CreateSolidBrush(RGB(0,0,0)), NULL);

		m_dwStyle = dwBarStyle;
		if (CControlBar::CreateEx(dwExStyle, pszClassName, "", dwStyle, CRect(0, 0, 0, 0), pParentWnd, nID)) {
			return true;
		}
		return false;
	}
	// 
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz)
	{
		RenderContext a;
		Render(NULL, a);

		CRect rc(0, 0, (bStretch && bHorz) ? 2046 : 480, a.ty+1);
		CalcWindowRect(&rc, CWnd::adjustOutside);

		return rc.Size();
	}

	// 
	void Reinit(bool fIs8bpp, int iKey)
	{
		EnableWindow(!fIs8bpp);

		int i;
		for (i = 0; i < 3; i++) m_fPalAvail[i] = false, m_iKeys[i] = iKey;
	}
	// 
	void SetPal(int iStage, COLORREF clr[16])
	{
		if (m_fPalAvail[iStage] = (clr != NULL)) {
			memcpy(m_clrPal[iStage], clr, 4 *16);
		}
	}
	// 
	void SetKey(int iStage, int iKey)
	{
		m_iKeys[iStage] = iKey;
	}

protected:
	// 
	void OnResize();
	// 
	void Render(CDC *pDC, RenderContext &r);
	// 
	void RenderPal(CDC *pDC, RenderContext &r, int i);
	// 
	void Render(CDC *pDC);

	// 
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
	{

	}

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDM2VwPicVwPal)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CDM2VwPicVwPal();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDM2VwPicVwPal)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2VWPICVWPAL_H__14993E80_E59D_42B5_80B7_1FB8C611374F__INCLUDED_)
