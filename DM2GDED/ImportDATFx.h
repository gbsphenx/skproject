// ImportDATFx.h: CImportDATFx クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMPORTDATFX_H__0C0D4361_E2B3_4473_AE1D_955F7D035079__INCLUDED_)
#define AFX_IMPORTDATFX_H__0C0D4361_E2B3_4473_AE1D_955F7D035079__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GDAT.h"
#include "ImportResDlg.h"

#define FXIT_DM2_C4_LE		1
#define FXIT_DM2_C8_LE		2
#define FXIT_VALUE			3
#define FXIT_RAW			4
#define FXIT_TEXT_EN_ENC	5
#define FXIT_TEXT_JA_ENC	6

class CImportDATFx  
{
	// 
	struct FxImpEntry {
		// 
		UINT nFXIT;

		// 
		UINT nVal;
		// 
		CString strFile;

		// 
		FxImpEntry(): nFXIT(-1), nVal(-1) { }
	};
	// 
	typedef map<CGDEntry, FxImpEntry> FxImpEntryMap;

	// 
	FxImpEntryMap m_map;
	// 
	CImportResRecList &m_res;

public:
	// 
	enum {
		irOk,
		irERR,
		irPartial,
	};

	// 
	CImportDATFx(CImportResRecList &rRec): m_res(rRec) { }
	// 
	BOOL Load(LPCSTR pszFileIn);
	// 
	BOOL PreImport(CGDAT &rDAT, bool fOverwrite);
	// 
	BOOL Import(CGDAT &rDAT);

};

#endif // !defined(AFX_IMPORTDATFX_H__0C0D4361_E2B3_4473_AE1D_955F7D035079__INCLUDED_)
