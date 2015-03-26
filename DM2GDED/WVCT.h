// WVCT.h: CWVCF クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WVCT_H__0D981393_AA8B_4BA0_B0A4_C8516E74ED99__INCLUDED_)
#define AFX_WVCT_H__0D981393_AA8B_4BA0_B0A4_C8516E74ED99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GDEdDef.h"

#define DECL_WVCT_SPEC(X)					\
	enum { wvct = X };						\
	virtual int GetWvct() { return wvct; }	\

#define DECL_WVCT_PRE()						\
	virtual int GetWvct() = NULL;			\


class CWordValCmd
{
public:
	// 
	virtual ~CWordValCmd() { }

	// 
	virtual int GetWvct() = NULL;

	// 
	virtual CString Format(WORD x) = 0;
	// 
	virtual CString FormatDisp(WORD x) = 0;
	// 
	virtual bool Parse(LPCSTR pszText, WORD &x) = 0;
	// 
	virtual LPCTSTR GetHintText(int i) { return NULL; }
	// 
	virtual void SetParm1(LPCSTR psz) { }

protected:
	// 
	CString Flags_Format(WORD x)
	{
		CHAR c[17];
		WORD wTbl[16] = {0x8000,0x4000,0x2000,0x1000,0x0800,0x0400,0x0200,0x0100,0x0080,0x0040,0x0020,0x0010,0x0008,0x0004,0x0002,0x0001};
		for (int i = 0; i < 16; i++) {
			c[i] = ((x & wTbl[i]) != 0) ? '1' : '0';
		}
		c[16] = 0;
		return c;
	}
	// 
	bool Flags_Parse(LPCSTR pszText, WORD &x)
	{
		LPSTR pszTerm;
		x = static_cast<WORD>(strtol(pszText, &pszTerm, 2));
		return (*pszTerm == 0);
	}
	// 
	virtual CString Unsigned_Format(WORD x)
	{
		CString strRet;
		strRet.Format("%u", 0U +x);
		return strRet;
	}
	// 
	virtual bool Unsigned_Parse(LPCSTR pszText, WORD &x)
	{
		LPSTR pszTerm;
		x = static_cast<WORD>(strtol(pszText, &pszTerm, 0));
		return (*pszTerm == 0);
	}

};

typedef CWordValCmd CWVCF;

struct CWordValCmd_Signed : CWordValCmd {
	// 
	DECL_WVCT_SPEC(wvctSigned)

	// 
	virtual CString Format(WORD x)
	{
		CString strRet;
		strRet.Format("%d", 0U +(__int16)x);
		return strRet;
	}
	// 
	virtual CString FormatDisp(WORD x) { return Format(x); }
	// 
	virtual bool Parse(LPCSTR pszText, WORD &x)
	{
		LPSTR pszTerm;
		x = (__int16)strtol(pszText, &pszTerm, 0);
		return (*pszTerm == 0);
	}
};

struct CWordValCmd_Unsigned : CWordValCmd {
	// 
	DECL_WVCT_SPEC(wvctUnsigned)

	// 
	virtual CString Format(WORD x)
	{
		CString strRet;
		strRet.Format("%u", 0U +x);
		return strRet;
	}
	// 
	virtual CString FormatDisp(WORD x) { return Format(x); }
	// 
	virtual bool Parse(LPCSTR pszText, WORD &x)
	{
		LPSTR pszTerm;
		x = static_cast<WORD>(strtol(pszText, &pszTerm, 0));
		return (*pszTerm == 0);
	}
};

struct CWordValCmd_Hex : CWordValCmd {
	// 
	DECL_WVCT_SPEC(wvctHex)

	// 
	virtual CString Format(WORD x)
	{
		CString strRet;
		strRet.Format("%04X", 0U +x);
		return strRet;
	}
	// 
	virtual CString FormatDisp(WORD x) { return Format(x); }
	// 
	virtual bool Parse(LPCSTR pszText, WORD &x)
	{
		LPSTR pszTerm;
		x = static_cast<WORD>(strtol(pszText, &pszTerm, 16));
		return (*pszTerm == 0);
	}
};

struct CWordValCmd_Flags : CWordValCmd {
	// 
	DECL_WVCT_SPEC(wvctFlags)

	// 
	virtual CString Format(WORD x)
	{
		CHAR c[17];
		WORD wTbl[16] = {0x8000,0x4000,0x2000,0x1000,0x0800,0x0400,0x0200,0x0100,0x0080,0x0040,0x0020,0x0010,0x0008,0x0004,0x0002,0x0001};
		for (int i = 0; i < 16; i++) {
			c[i] = ((x & wTbl[i]) != 0) ? '1' : '0';
		}
		c[16] = 0;
		return c;
	}
	// 
	virtual CString FormatDisp(WORD x) { return Format(x); }
	// 
	virtual bool Parse(LPCSTR pszText, WORD &x)
	{
		LPSTR pszTerm;
		x = static_cast<WORD>(strtol(pszText, &pszTerm, 2));
		return (*pszTerm == 0);
	}
};

#if 0
//	
//	struct CWordValCmd_HintFlags : CWordValCmd {
//		// 
//		DECL_WVCT_SPEC(wvctHintFlags)
//	
//		// 
//		virtual CString Format(WORD x);
//		// 
//		virtual CString FormatDisp(WORD x);
//		// 
//		virtual bool Parse(LPCSTR pszText, WORD &x);
//		// 
//		virtual LPCTSTR GetHintText(int i);
//	
//		// 
//		void SetParm1(LPCTSTR psz);
//	
//	protected:
//		// 
//		list<CString> listHint;
//	
//	};
//	
//	struct CWordValCmd_KeyItem : CWordValCmd_Unsigned {
//		// 
//		DECL_WVCT_SPEC(wvctKeyItem)
//	
//		// 
//		virtual CString FormatDisp(WORD x);
//	};
//	
//	struct CWordValCmd_PrizeItem : CWordValCmd_Unsigned {
//		// 
//		DECL_WVCT_SPEC(wvctPrizeItem)
//	
//		// 
//		virtual CString FormatDisp(WORD x);
//	};
//	
//	struct CWordValCmd_MSItemsA : CWordValCmd_Unsigned {
//		// 
//		DECL_WVCT_SPEC(wvctPrizeItem)
//	};
//	
//	struct CWordValCmd_MSItemsC : CWordValCmd_Unsigned {
//		// 
//		DECL_WVCT_SPEC(wvctPrizeItem)
//	};
//	
//	struct CWordValCmd_B5353 : CWordValCmd_HintFlags {
//		// 
//		DECL_WVCT_SPEC(wvctPrizeItem)
//	
//		// 
//		virtual CString Format(WORD x);
//		// 
//		virtual CString FormatDisp(WORD x);
//		// 
//		virtual bool Parse(LPCSTR pszText, WORD &x);
//	
//	protected:
//		// 
//		CString FormatB3(int iTextIdx, LPCTSTR pszDefText, UINT x);
//	};
//	
//	struct CWordValCmd_SKText : CWordValCmd_Unsigned {
//		// 
//		DECL_WVCT_SPEC(wvctCommand)
//	};
//	
//	struct CWordValCmd_IconAnim : CWordValCmd_Hex {
//		// 
//		DECL_WVCT_SPEC(wvctIconAnim)
//	};

#endif

#endif // !defined(AFX_WVCT_H__0D981393_AA8B_4BA0_B0A4_C8516E74ED99__INCLUDED_)
