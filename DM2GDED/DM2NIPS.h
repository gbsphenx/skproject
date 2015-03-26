#if !defined(AFX_DM2NIPS_H__AB990AE9_BC7E_4EB2_86F5_99D39BC6550F__INCLUDED_)
#define AFX_DM2NIPS_H__AB990AE9_BC7E_4EB2_86F5_99D39BC6550F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2NIPS.h : ヘッダー ファイル
//

#include "DM2NICtx.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2NIPS

class CDM2NIPS : public CPropertySheet
{
	// 
	CDM2NICtx &m_ctx;

	// 
	DECLARE_DYNAMIC(CDM2NIPS)

public:
	// 
	CDM2NIPS(CDM2NICtx &rContext, CWnd* pParentWnd = NULL)
		: CPropertySheet(_T("Edit image"), pParentWnd, 0)
		, m_ctx(rContext)
	{
		SetWizardMode();
	}

	// 
	CDM2NICtx *GetCtx() { return &m_ctx; }

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDM2NIPS)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDM2NIPS)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2NIPS_H__AB990AE9_BC7E_4EB2_86F5_99D39BC6550F__INCLUDED_)
