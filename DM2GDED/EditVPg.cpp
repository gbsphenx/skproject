// EditVPg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "EditVPg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditVPg

CEditVPg::CEditVPg(CEditVLower &vctx, CWnd* pParent)
	: CDialog()
	, m_vctx(vctx)
{

}

BEGIN_MESSAGE_MAP(CEditVPg, CDialog)
	//{{AFX_MSG_MAP(CEditVPg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditVPg メッセージ ハンドラ
