#if !defined(AFX_SHOWCASEDOCK_H__CA11594C_B9C4_4F7F_BEB0_4153849FE0E7__INCLUDED_)
#define AFX_SHOWCASEDOCK_H__CA11594C_B9C4_4F7F_BEB0_4153849FE0E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShowcaseDock.h : ヘッダー ファイル
//

#include "scbarcf.h"
#include "ShowcaseFrame.h"

/////////////////////////////////////////////////////////////////////////////
// CShowcaseDock ウィンドウ

class CShowcaseDock : public CSizingControlBarCF
{
public:
	// 
	CShowcaseFrame m_frame;

	// 
	CShowcaseDock();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CShowcaseDock)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CShowcaseDock();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CShowcaseDock)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SHOWCASEDOCK_H__CA11594C_B9C4_4F7F_BEB0_4153849FE0E7__INCLUDED_)
