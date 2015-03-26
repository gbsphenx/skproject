#if !defined(AFX_EDPINOPAGE_H__8C198EBC_7DA2_46CE_9BE4_9803B7D3EB2C__INCLUDED_)
#define AFX_EDPINOPAGE_H__8C198EBC_7DA2_46CE_9BE4_9803B7D3EB2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPINoPage.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CEdPINoPage ダイアログ

#include "EdPIDlg.h"

class CEdPINoPage : public CEdPIDlg
{
	// 
	DECLARE_DYNCREATE(CEdPINoPage)

public:
	// 
	CEdPINoPage();

	// 
	virtual bool Revert(bool)
	{
		return true;
	}
	// 
	virtual bool RevertRecord(DMDBU &, bool)
	{
		return true;
	}

// ダイアログ データ
	//{{AFX_DATA(CEdPINoPage)
	enum { IDD = IDD_PI_NO };
		// メモ - ClassWizard はこの位置にデータ メンバを追加します。
		//    この位置に生成されるコードを編集しないでください。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CEdPINoPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEdPINoPage)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDPINOPAGE_H__8C198EBC_7DA2_46CE_9BE4_9803B7D3EB2C__INCLUDED_)
