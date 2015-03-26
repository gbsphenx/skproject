#if !defined(AFX_STATICIMAGEPV_H__31A9BAA2_F211_40CC_92AE_AD5724CFA6E1__INCLUDED_)
#define AFX_STATICIMAGEPV_H__31A9BAA2_F211_40CC_92AE_AD5724CFA6E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticImagePv.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CStaticImagePv ウィンドウ

class CStaticImagePv : public CStatic
{
public:
	// 
	CStaticImagePv();
	// 
	virtual ~CStaticImagePv();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CStaticImagePv)
	//}}AFX_VIRTUAL

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CStaticImagePv)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STATICIMAGEPV_H__31A9BAA2_F211_40CC_92AE_AD5724CFA6E1__INCLUDED_)
