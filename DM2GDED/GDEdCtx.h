// GDEdCtx.h: CGDEdCtx クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GDEDCTX_H__7CFD9E2E_9DCF_4E15_ADCD_0690189B2372__INCLUDED_)
#define AFX_GDEDCTX_H__7CFD9E2E_9DCF_4E15_ADCD_0690189B2372__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GDAT.h"
#include "RCI.h"
#include "DM2DescriptCtx.h"
#include "GDEdDef.h"
#include "DM2SkCmdList.h"
#include "EditVxxx.h"

class CGDEdCtxCB
{
public:
	// 
	virtual void OnGDUpdate(UINT nHint, void *pHint) = NULL;
};

class CGDEdCtxInCB
{
	// 
	CArray<CGDEdCtxCB *, CGDEdCtxCB *> arrSink;

public:
	// 
	~CGDEdCtxInCB()
	{
		ASSERT(arrSink.GetSize() == 0);
	}
	// 
	void AddCB(CGDEdCtxCB *pSink);
	// 
	void RemoveCB(CGDEdCtxCB *pSink);

	// 
	void DispatchGDUpdate(UINT nHint, void *pHint);
};

#include "GDEdOpt.h"

class CGDEdCtx : public CRCI, public CGDEdCtxInCB
{
	// 
	CGDAT m_core;
	// 
	CGDEdOpt m_opts;
	// 
	CDM2DescriptCtx m_dctx;
	// 
	CDM2SkCmdDesc m_scdesc;
	// 
	CEditVxxxFactoryImpl m_vxxxfactory;

public:
	// 
	CGDEdCtx();
	// 
	virtual ~CGDEdCtx();
	// 
	CGDAT &GetGDAT() { return m_core; }
	// 
	void GetOpts(CGDEdOpt &rOpts) { rOpts = m_opts; }
	// 
	void SetOpts(const CGDEdOpt &rOpts) { m_opts = rOpts; }
	// 
	void LoadFrom(LPCTSTR pszFileIn);
	// 
	void LoadDescFrom(LPCTSTR pszFileIn);
	// 
	CDM2DescriptCtx &GetDescriptCtx() { return m_dctx; }
	// 
	CDM2SkCmdDesc &GetSkCmdDesc() { return m_scdesc; }
	// 
	CEditVxxxFactoryImpl &GetVxxxFactory() { return m_vxxxfactory; }

};

#endif // !defined(AFX_GDEDCTX_H__7CFD9E2E_9DCF_4E15_ADCD_0690189B2372__INCLUDED_)
