// TriViewNe.h: CTriViewNe クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIVIEWNE_H__E23A7352_E575_4928_8BCB_84765E248C85__INCLUDED_)
#define AFX_TRIVIEWNE_H__E23A7352_E575_4928_8BCB_84765E248C85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CInterViewInfluential
{
public:
	// 
	virtual void OnTriViewMessage(UINT nHint, void *pHint) = NULL;

	// 
	static bool Send(CObject *pUnk, UINT nHint, void *pHint);
};

#endif // !defined(AFX_TRIVIEWNE_H__E23A7352_E575_4928_8BCB_84765E248C85__INCLUDED_)
