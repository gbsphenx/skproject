#if !defined(AFX_TSDATAOBJECT_H__A2A7C114_A01A_4725_8667_19ED82DFEEDB__INCLUDED_)
#define AFX_TSDATAOBJECT_H__A2A7C114_A01A_4725_8667_19ED82DFEEDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TsDataObject.h : ヘッダー ファイル
//

#include "ShowcaseData.h"

/////////////////////////////////////////////////////////////////////////////
// CTsDataObject コマンド ターゲット

class CTsDataObject : public COleDataObject
{
public:
	// 
	CTsDataObject();

	// 
	bool GetShowcase(ShowcaseData &ref);

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTsDataObject)
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTsDataObject)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TSDATAOBJECT_H__A2A7C114_A01A_4725_8667_19ED82DFEEDB__INCLUDED_)
