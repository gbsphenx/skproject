// DMEdCtx.h: CDMEdCtx クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DMEDCTX_H__A1EF1853_D4EF_4851_8F3D_A384195FC383__INCLUDED_)
#define AFX_DMEDCTX_H__A1EF1853_D4EF_4851_8F3D_A384195FC383__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RCI.h"
#include "DDAT.h"

struct CDMEdCtxAccessibility
{
	// 
	BOOL bLimitless;
	// 
	CDMEdCtxAccessibility(): bLimitless(FALSE) { }
};


class CDMEdCtxCB
{
public:
	// 
	virtual void OnDMUpdate(UINT nHint, void *pHint) = NULL;
};

class CDMEdCtxInCB
{
	// 
	CArray<CDMEdCtxCB *, CDMEdCtxCB *> arrSink;

public:
	// 
	~CDMEdCtxInCB()
	{
		ASSERT(arrSink.GetSize() == 0);
	}
	// 
	void AddCB(CDMEdCtxCB *pSink);
	// 
	void RemoveCB(CDMEdCtxCB *pSink);

	// 
	void DispatchDMUpdate(UINT nHint, void *pHint);
};

class CDMEdCtx : public CRCI, public CDMEdCtxInCB
{
	// 
	CDDAT *pIf;
	// 
	CDMEdCtxAccessibility m_access;

public:
	// 
	CDMEdCtx();
	// 
	virtual ~CDMEdCtx();
	// 
	CDDAT *GetDDIf() { return pIf; }
	// 
	void GetAccessibility(CDMEdCtxAccessibility &r) { r = m_access; }
	// 
	void SetAccessibility(const CDMEdCtxAccessibility &r);

};

#endif // !defined(AFX_DMEDCTX_H__A1EF1853_D4EF_4851_8F3D_A384195FC383__INCLUDED_)
