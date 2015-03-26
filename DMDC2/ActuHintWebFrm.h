#if !defined(AFX_ACTUHINTWEBFRM_H__DC05ADC8_C96D_40F8_8F10_64DF05836AAC__INCLUDED_)
#define AFX_ACTUHINTWEBFRM_H__DC05ADC8_C96D_40F8_8F10_64DF05836AAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActuHintWebFrm.h : ヘッダー ファイル
//

#include "ActuHintWebVw.h"

/////////////////////////////////////////////////////////////////////////////
// CActuHintWebFrm フレーム

class CActuHintWebFrm : public CFrameWnd
{
public:
	// 
	CActuHintWebVw m_vw;

	// 
	CActuHintWebFrm();
	virtual ~CActuHintWebFrm();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CActuHintWebFrm)
	protected:
	virtual void PostNcDestroy();
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CActuHintWebFrm)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ACTUHINTWEBFRM_H__DC05ADC8_C96D_40F8_8F10_64DF05836AAC__INCLUDED_)
