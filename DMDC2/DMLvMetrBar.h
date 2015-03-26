#if !defined(AFX_DMLVMETRBAR_H__849826E0_29C0_4E22_9466_AF15BDE00B21__INCLUDED_)
#define AFX_DMLVMETRBAR_H__849826E0_29C0_4E22_9466_AF15BDE00B21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMLvMetrBar.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDMLvMetrBar ウィンドウ

class CDMLvMetrBar : public CDialogBar
{
	// 
	CFont m_fontUl;

// コンストラクション
public:
	CDMLvMetrBar();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDMLvMetrBar)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CDMLvMetrBar();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDMLvMetrBar)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	virtual LRESULT HandleInitDialog(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DMLVMETRBAR_H__849826E0_29C0_4E22_9466_AF15BDE00B21__INCLUDED_)
