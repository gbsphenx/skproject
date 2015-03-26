// DiffExportExec.h: CDiffExportExec クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIFFEXPORTEXEC_H__890BC9EE_837C_4C59_B54C_CA896B5ECB1E__INCLUDED_)
#define AFX_DIFFEXPORTEXEC_H__890BC9EE_837C_4C59_B54C_CA896B5ECB1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GDAT.h"

class CDiffExportExec  
{
public:
	// 
	bool Exec(CString strFileOrg, CGDAT &rMod, CString strOutput);

};

#endif // !defined(AFX_DIFFEXPORTEXEC_H__890BC9EE_837C_4C59_B54C_CA896B5ECB1E__INCLUDED_)
