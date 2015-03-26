// DiffImportExec.h: CDiffImportExec クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIFFIMPORTEXEC_H__F26C05EB_9154_4F98_8113_404084E5B8EA__INCLUDED_)
#define AFX_DIFFIMPORTEXEC_H__F26C05EB_9154_4F98_8113_404084E5B8EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GDAT.h"

class CDiffImportExec  
{
public:
	// 
	bool Exec(CGDAT &rOrg, CString strFileDiff);

};

#endif // !defined(AFX_DIFFIMPORTEXEC_H__F26C05EB_9154_4F98_8113_404084E5B8EA__INCLUDED_)
