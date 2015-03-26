// TriViewNe.cpp: CTriViewNe クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "exp00069.h"
#include "TriViewNe.h"
#include "DM2HierMapView.h"
#include <typeinfo.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CInterViewInfluential

bool CInterViewInfluential::Send(CObject *pUnk, UINT nHint, void *pHint)
{
	CInterViewInfluential *pIf;
	if (pIf = dynamic_cast<CInterViewInfluential *>(pUnk)) {
		pIf->OnTriViewMessage(nHint, pHint);
		return true;
	}
	return false;
}
