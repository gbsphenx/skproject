#if !defined(AFX_TSDATASOURCE_H__A6FD9FC1_2C46_4DEE_AE9D_438AE1567538__INCLUDED_)
#define AFX_TSDATASOURCE_H__A6FD9FC1_2C46_4DEE_AE9D_438AE1567538__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TsDataSource.h : ヘッダー ファイル
//

#include "ShowcaseData.h"

/////////////////////////////////////////////////////////////////////////////
// CTsDataSource コマンド ターゲット

class CTsDataSource : public COleDataSource
{
public:
	// 
	static UINT cfShowcase;

	// 
	CTsDataSource();
	// 
	void CacheShowcase(ShowcaseData &ref);

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTsDataSource)
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTsDataSource)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TSDATASOURCE_H__A6FD9FC1_2C46_4DEE_AE9D_438AE1567538__INCLUDED_)
