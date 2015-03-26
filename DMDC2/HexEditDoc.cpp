// HexEditDoc.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "HexEditDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHexEditDoc

IMPLEMENT_DYNCREATE(CHexEditDoc, CDocument)

CHexEditDoc::CHexEditDoc()
{
}

BOOL CHexEditDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CHexEditDoc::~CHexEditDoc()
{
}


BEGIN_MESSAGE_MAP(CHexEditDoc, CDocument)
	//{{AFX_MSG_MAP(CHexEditDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHexEditDoc 診断

#ifdef _DEBUG
void CHexEditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHexEditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHexEditDoc シリアライズ

void CHexEditDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: この位置に保存用のコードを追加してください
	}
	else
	{
		// TODO: この位置に読み込み用のコードを追加してください
	}
}

/////////////////////////////////////////////////////////////////////////////
// CHexEditDoc コマンド
