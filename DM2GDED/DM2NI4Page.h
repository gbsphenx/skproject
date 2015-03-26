#if !defined(AFX_DM2NI4PAGE_H__7CAFB188_1788_414D_802E_AD0C564DBC26__INCLUDED_)
#define AFX_DM2NI4PAGE_H__7CAFB188_1788_414D_802E_AD0C564DBC26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2NI4Page.h : ヘッダー ファイル
//

#include "DM2NIBPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2NI4Page ダイアログ

class CDM2NI4Page : public CDM2NIBPage
{
	// 
	void UpdateVw();
	// 
	DECLARE_DYNCREATE(CDM2NI4Page)

// コンストラクション
public:
	CDM2NI4Page();
	~CDM2NI4Page();

protected:
	// 
	virtual int GetStage() { return 3; }

// ダイアログ データ
	//{{AFX_DATA(CDM2NI4Page)
	enum { IDD = IDD_PP_NI4 };
		// メモ - ClassWizard はこの位置にデータ メンバを追加します。
		//    この位置に生成されるコードを編集しないでください。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDM2NI4Page)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDM2NI4Page)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2NI4PAGE_H__7CAFB188_1788_414D_802E_AD0C564DBC26__INCLUDED_)
