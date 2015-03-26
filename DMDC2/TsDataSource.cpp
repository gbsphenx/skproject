// TsDataSource.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "TsDataSource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTsDataSource

UINT CTsDataSource::cfShowcase = RegisterClipboardFormat("DMDC2.Showcase");

CTsDataSource::CTsDataSource()
{

}

void CTsDataSource::CacheShowcase(ShowcaseData &ref)
{
	CSharedFile f;
	f.Write(&ref, sizeof(ref));

	CacheGlobalData(cfShowcase, f.Detach());
}

/////////////////////////////////////////////////////////////////////////////
// CTsDataSource メッセージ ハンドラ

#if 0
BEGIN_MESSAGE_MAP(CTsDataSource, CCmdTarget)
	//{{AFX_MSG_MAP(CTsDataSource)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif
