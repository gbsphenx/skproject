#if !defined(AFX_HEXEDITFRAME_H__6AADE43C_DAF9_4F0B_8A42_7672E66846CE__INCLUDED_)
#define AFX_HEXEDITFRAME_H__6AADE43C_DAF9_4F0B_8A42_7672E66846CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HexEditFrame.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CHexEditFrame フレーム

class CHexEditFrame : public CFrameWnd
{
	// 
	HWND m_hWndModalTar;
public:
	// 
	static CHexEditFrame *Create1(CWnd *pParentWnd);
	// 
	int StayModal()
	{
		return RunModalLoop(MLF_SHOWONIDLE);
	}

	DECLARE_DYNCREATE(CHexEditFrame)
protected:
	CHexEditFrame();           // 動的生成に使用されるプロテクト コンストラクタ。

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CHexEditFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CHexEditFrame();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CHexEditFrame)
	afx_msg void OnAppClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_HEXEDITFRAME_H__6AADE43C_DAF9_4F0B_8A42_7672E66846CE__INCLUDED_)
