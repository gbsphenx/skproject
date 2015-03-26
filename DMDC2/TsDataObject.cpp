// TsDataObject.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "TsDataObject.h"
#include "TsDataSource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTsDataObject

CTsDataObject::CTsDataObject()
{

}

bool CTsDataObject::GetShowcase(ShowcaseData &ref)
{
	HGLOBAL hMem = GetGlobalData(CTsDataSource::cfShowcase);
	CSharedFile f;
	f.SetHandle(hMem);
	int r = f.Read(&ref, sizeof(ref));
	f.Detach();

	if (r != sizeof(ref))
		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CTsDataObject メッセージ ハンドラ

#if 0
BEGIN_MESSAGE_MAP(CTsDataObject, CCmdTarget)
	//{{AFX_MSG_MAP(CTsDataObject)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif
