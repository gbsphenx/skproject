
#ifndef _DTREEUSING_H_
#define _DTREEUSING_H_

#include "GDEdCtx.h"
#include "ILSp.h"

class CDTreeUsing : protected CGDEdCtxCB
{
public:
	// 
	CDTreeUsing(): m_pEdCtx(NULL) { }
	// 
	virtual ~CDTreeUsing()
	{
		InitEdCtx(NULL);
	}

protected:
	// 
	CGDEdCtx *m_pEdCtx;
	// 
	CGDEdOpt m_aEdOpt;
	// 
	CILSp m_ilspInlang, m_ilspCnlang;

	// 
	void InitEdCtx(CGDEdCtx *pEdCtx);
	// 
	CString FormatClass2Name(CGDEntry pos);

	// 
	virtual void OnGDUpdate(UINT nHint, void *pHint) = NULL;

};

#endif // #ifndef _DTREEUSING_H_
