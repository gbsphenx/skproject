#if !defined(AFX_DM2NIBPAGE_H__9AB5491D_2CD3_4DB7_B121_6546363F3A5F__INCLUDED_)
#define AFX_DM2NIBPAGE_H__9AB5491D_2CD3_4DB7_B121_6546363F3A5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2NIBPage.h : ヘッダー ファイル
//

#include "DM2NICtx.h"
#include "DM2NIPvFrame.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2NIBPage ウィンドウ

class CDM2NIBPage : public CPropertyPage
{
	// 
	DECLARE_DYNAMIC(CDM2NIBPage)

public:
	// 
	explicit CDM2NIBPage(UINT nIDTemplate, UINT nIDCaption = 0): CPropertyPage(nIDTemplate, nIDCaption)
	{
		m_ctx = NULL;
	}

protected:
	// 
	CDM2NICtx *m_ctx;
	// 
	CPropertySheet *m_ppsh;
	// 
	CDM2NIPvFrame m_frame;

	// 
	bool MkCtx();
	// 
	bool MkPSH();
	// 
	bool MkEmbeddedCtl();

	// 
	virtual int GetStage() = NULL;

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDM2NIBPage)
	public:
	virtual BOOL OnSetActive();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDM2NIBPage)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2NIBPAGE_H__9AB5491D_2CD3_4DB7_B121_6546363F3A5F__INCLUDED_)
