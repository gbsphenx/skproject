#if !defined(AFX_DM2VWSCENEFRAME_H__7F311662_E26B_4134_86C8_4B392BFCA6B4__INCLUDED_)
#define AFX_DM2VWSCENEFRAME_H__7F311662_E26B_4134_86C8_4B392BFCA6B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2VwSceneFrame.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDM2VwSceneFrame フレーム

class CDM2VwSceneFrame : public CFrameWnd
{
	// 
	DECLARE_DYNCREATE(CDM2VwSceneFrame)

protected:
	// 
	CDM2VwSceneFrame();

public:
	// 
	static CDM2VwSceneFrame *CreateNew(CDocument *pDoc);

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します

	//{{AFX_VIRTUAL(CDM2VwSceneFrame)
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CDM2VwSceneFrame();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDM2VwSceneFrame)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2VWSCENEFRAME_H__7F311662_E26B_4134_86C8_4B392BFCA6B4__INCLUDED_)
