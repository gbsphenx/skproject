#if !defined(AFX_SPLASHVW_H__5AD9C2DA_A39C_44BF_8E33_A06DF64F337E__INCLUDED_)
#define AFX_SPLASHVW_H__5AD9C2DA_A39C_44BF_8E33_A06DF64F337E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplashVw.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSplashVw ビュー

class CSplashVw : public CScrollView
{
	// 
	CBitmap m_bm;
	// 
	BITMAP m_bmp;

public:
	// 
	CSplashVw();
	// 
	virtual ~CSplashVw();
	// 
	void PostNcDestroy() { }

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSplashVw)
	protected:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドしました。
	virtual void OnInitialUpdate();     // 構築後、最初の処理。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSplashVw)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SPLASHVW_H__5AD9C2DA_A39C_44BF_8E33_A06DF64F337E__INCLUDED_)
