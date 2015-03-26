#if !defined(AFX_CUSTRICHEDITCTRL_H__147BDB37_E0DA_43D1_8805_7B1350DFE1A8__INCLUDED_)
#define AFX_CUSTRICHEDITCTRL_H__147BDB37_E0DA_43D1_8805_7B1350DFE1A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustRichEditCtrl.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CCustRichEditCtrl ウィンドウ

class CCustRichEditCtrl : public CRichEditCtrl
{
// コンストラクション
public:
	CCustRichEditCtrl();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCustRichEditCtrl)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CCustRichEditCtrl();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CCustRichEditCtrl)
	//}}AFX_MSG
	void OnLink(NMHDR *pNMHDR, LRESULT *pRet);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CUSTRICHEDITCTRL_H__147BDB37_E0DA_43D1_8805_7B1350DFE1A8__INCLUDED_)
