
#pragma once

#include "GDEdCtx.h"
#include "ILSp.h"

class CF2C
{
public:
	// 
	CF2C(): m_pEdCtx(NULL) {
	}
	// 
	~CF2C() {
		InitEdCtx(NULL);
	}
	// 
	void InitEdCtx(CGDEdCtx *pEdCtx);
	// 
	CString FormatClass2Name(CGDEntry pos);
	// 
	CString FormatMessage(int cls4);

protected:
	// 
	CGDEdCtx *m_pEdCtx;
	// 
	CGDEdOpt m_aEdOpt;
	// 
	CILSp m_ilspInlang, m_ilspCnlang;

};
