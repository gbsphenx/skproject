
#pragma once

#include "GDEdDef.h"

struct CGDEdOpt
{
	int m_wvct;
	// 
	int m_inlang;
	// 
	int m_cnlang;

	// 
	CGDEdOpt()
	{
		m_wvct = wvctUnsigned;
		m_inlang = inlangNo;
		m_cnlang = cnlangNo;
	}
	// 
	void LoadFrom(LPCTSTR pszFile);
	// 
	void SaveTo(LPCTSTR pszFile);
};
