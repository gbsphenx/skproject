#if !defined(AFX_DM2EDPICFRAME_H__CA28D1AC_1232_4E31_AAB8_B2CF837C07DF__INCLUDED_)
#define AFX_DM2EDPICFRAME_H__CA28D1AC_1232_4E31_AAB8_B2CF837C07DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2EdPicFrame.h : ヘッダー ファイル
//

#include "DM2RPtr.h"
#include "GDEdCtx.h"
#include "DM2VwPicVwPal.h"
#include "ToolBarEx.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2EdPicFrame フレーム

class CDM2EdPicFrame : public CFrameWnd
{
	// 
	typedef map<CDocument *, CDM2EdPicFrame *> DocMap;

	// 
	CDM2SPtr<CGDEdCtx> m_pEdCtx;
	// 
	CDM2VwPicVwPal m_wndVwPal;
	// 
	CToolBarEx m_wndToolBar;

	// 
	static DocMap m_map;

	// 
	DECLARE_DYNAMIC(CDM2EdPicFrame)

public:
	// 
	CDM2EdPicFrame(CGDEdCtx *pEdCtx);
	// 
	CDM2VwPicVwPal &GetVwPal() { return m_wndVwPal; }
	// 
	static CDM2EdPicFrame *CreateNew(CDocument *pDoc, CGDEdCtx *pEdCtx, CGDEntry pos);
	// 
	static void OnCloseDocument(CDocument *pDoc);

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDM2EdPicFrame)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CDM2EdPicFrame();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDM2EdPicFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2EDPICFRAME_H__CA28D1AC_1232_4E31_AAB8_B2CF837C07DF__INCLUDED_)
