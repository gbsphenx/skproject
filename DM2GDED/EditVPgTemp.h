
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CEditVChampStatPg

class CEditVChampStatPg : public CEditVPg, public CWordValCmd_Signed, public CEditVxxx {
public:
	// 
	DECL_WVCT_SPEC(wvctChampStat)
	// 
	CEditVChampStatPg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVPg(vctx, pParent) { }

	// 
	virtual CEditVPg &GetEditValPg() { return *this; }
	// 
	virtual CWordValCmd &GetOp() { return *this; }

	// 
	virtual UINT GetVal()
	{
		return 0;
	}
	// 
	virtual LPCTSTR GetTemplateName() { return NULL; }

};
