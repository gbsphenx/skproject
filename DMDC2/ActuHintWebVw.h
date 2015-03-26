#if !defined(AFX_ACTUHINTWEBVW_H__0A2A0D13_95F6_4B02_B482_15B0E0BC197A__INCLUDED_)
#define AFX_ACTUHINTWEBVW_H__0A2A0D13_95F6_4B02_B482_15B0E0BC197A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActuHintWebVw.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CActuHintWebVw HTML ビュー

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

class CActuHintWebVw : public CHtmlView
{
public:
	// 
	CActuHintWebVw();
	virtual ~CActuHintWebVw();

// html Data
public:
	//{{AFX_DATA(CActuHintWebVw)
		// メモ: この位置に ClassWizard によってデータ メンバが追加されます。
	//}}AFX_DATA

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します
	//{{AFX_VIRTUAL(CActuHintWebVw)
	public:
	virtual void OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel);
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	virtual void PostNcDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CActuHintWebVw)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ACTUHINTWEBVW_H__0A2A0D13_95F6_4B02_B482_15B0E0BC197A__INCLUDED_)
