// EditVCtx.h: CEditVLower クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITVCTX_H__419BD988_D52F_4129_9320_FC154C4D9CF3__INCLUDED_)
#define AFX_EDITVCTX_H__419BD988_D52F_4129_9320_FC154C4D9CF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GDEdOpt.h"

class CGDEdCtx;

class CEditVLower  
{
public:
//	// 
//	CGDEdOpt m_aOpt;

	// 
	CEditVLower(CGDEdCtx *pEdCtx): m_pEdCtx(NULL)
	{
		SetEdCtx(pEdCtx);
	}
	// 
	virtual ~CEditVLower()
	{
		SetEdCtx(NULL);
	}
	// 
	CGDEdCtx *GetEdCtx(BOOL fAddRef = FALSE);
	// 
	BOOL Get4CatItemName(int iCat, int iIdx, CString &strText);
	// 
	BOOL Get1CatItemName(int iCat, int iIdx, CString &strText);
	// 
	BOOL Get6CatItemName(int iCat, int iIdx, CString &strText);

protected:
	// 
	CGDEdCtx *m_pEdCtx;

	// 
	void SetEdCtx(CGDEdCtx *pEdCtx);
};

#endif // !defined(AFX_EDITVCTX_H__419BD988_D52F_4129_9320_FC154C4D9CF3__INCLUDED_)
