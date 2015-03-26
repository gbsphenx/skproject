// DMEdCtx.cpp: CDMEdCtx クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "exp00069.h"
#include "DMEdCtx.h"
#include "Exp00069Doc.h"
#include "DDAT2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CDMEdCtxInCB

void CDMEdCtxInCB::AddCB(CDMEdCtxCB *pSink)
{
	arrSink.Add(pSink);
}

void CDMEdCtxInCB::RemoveCB(CDMEdCtxCB *pSink)
{
	for (int i = 0, n = arrSink.GetSize(); i < n; i++) {
		if (arrSink[i] == pSink) {
			arrSink.RemoveAt(i), i--, n--;
		}
	}
}

void CDMEdCtxInCB::DispatchDMUpdate(UINT nHint, void *pHint)
{
	for (int i = 0, n = arrSink.GetSize(); i < n; i++) {
		arrSink[i]->OnDMUpdate(nHint, pHint);
	}
}

//////////////////////////////////////////////////////////////////////
// CDMEdCtx

CDMEdCtx::CDMEdCtx(): CRCI()
{
	pIf = new CDDATWork2(this);
}

CDMEdCtx::~CDMEdCtx()
{
	delete pIf;
}

void CDMEdCtx::SetAccessibility(const CDMEdCtxAccessibility &r)
{
	if (m_access.bLimitless != r.bLimitless) {
		m_access.bLimitless = r.bLimitless;
	}

	DispatchDMUpdate(DPM_CHANGED_CONF1, &m_access);
}
