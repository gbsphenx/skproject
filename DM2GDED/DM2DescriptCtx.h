// DM2DescriptCtx.h: CDM2DescriptCtx クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM2DESCRIPTCTX_H__BA2C7C0A_B537_478B_8AD3_0B46EB1B9CCC__INCLUDED_)
#define AFX_DM2DESCRIPTCTX_H__BA2C7C0A_B537_478B_8AD3_0B46EB1B9CCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GDAT.h"
#include "WVCT.h"
#include "GDEdDef.h"
#include "EditVxxx.h"
#include "ForthInt.h"

class CDM2DescItem
{
	// 
	BYTE c1m[256/8];
	// 
	BYTE c2m[256/8];
	// c3 --- c3+c4+c5+c6
	BYTE c3[4];
	// Test flags for c3+c4+c5+c6
	bool f3[4];
	// 
	CString strText, strFlags;
	// 
	CEditVxxx *vxxx;
	// 
	bool fDynSel;
	// 
	CString strProg;

	// 
	CDM2DescItem(const CDM2DescItem &);
	// 
	void operator =(const CDM2DescItem &);

	// 
	static BYTE Hex2Val(BYTE c)
	{
		if ('0' <= c && c <= '9') return c - '0';
		if ('A' <= c && c <= 'F') return c - 'A' + 10;
		if ('a' <= c && c <= 'f') return c - 'a' + 10;
		return 0;
	}
	// 
	static bool ParseHex2OrXX(LPCSTR &pszPos, BYTE &b, bool &f)
	{
		if (false
			|| pszPos[0] == 'x' && pszPos[1] == 'x'
			|| pszPos[0] == 'X' && pszPos[1] == 'X'
		) {
			f = false;
			pszPos += 2;
			return true;
		}
		if (isxdigit(pszPos[0]) && isxdigit(pszPos[1])) {
			f = true;
			b = Hex2Val(pszPos[0]) * 16 + Hex2Val(pszPos[1]);
			pszPos += 2;
			return true;
		}
		return false;
	}

	// 
	void setVxxx(CEditVxxx *vxxx) {
		if (this->vxxx != NULL)
			delete this->vxxx;
		this->vxxx = vxxx;
	}

public:
	// 
	CDM2DescItem();
	// 
	~CDM2DescItem();
	// 
	bool Parse(CEditVxxxFactoryImpl &vxxxfac, LPCSTR psz1, LPCSTR psz2, LPCSTR psz3, LPCSTR psz4, LPCSTR psz5, LPCSTR psz6, LPCSTR psz7, LPCSTR psz8);
	// 
	bool Match(CGDEntry pos) const;
	// 
	LPCTSTR GetText() const { return strText; }
	// 
	LPCTSTR GetFlags() const { return strFlags; }
	// 
	DWORD GetWvct() const;
	// 
	CEditVxxx &GetVxxx() const;
	// 
	LPCTSTR GetProg() const { return strProg; }
	// 
	bool HasProg() const { return fDynSel; }

};

class CDM2DescriptCtx  
{
	// 
	typedef list<CDM2DescItem *> RuleList;
	// 
	RuleList listRule;

	// 
	CDM2DescriptCtx(const CDM2DescriptCtx &);
	// 
	void operator =(const CDM2DescriptCtx &);

public:
	// 
	CDM2DescriptCtx() { }
	// 
	~CDM2DescriptCtx();
	// 
	void RemoveAll();
	// 
	bool LoadFrom(CEditVxxxFactoryImpl &vxxxfac, LPCTSTR pszFileIn);
	// 
	const CDM2DescItem *FindItem(const CGDEntry &pos);
	// 
	const CDM2DescItem *FindItem2(const CGDEntry &pos, CGDAT &core);

};

class CDM2EntForth : public CForthInt
{
	// 
	CGDEntry pos;
	// 
	CGDAT &core;

public:
	// 
	virtual bool Process();

	// 
	CDM2EntForth(LPCSTR psz, CGDEntry pos, CGDAT &core)
		: CForthInt(psz, 0, strlen(psz))
		, pos(pos)
		, core(core)
	{

	}
};

#endif // !defined(AFX_DM2DESCRIPTCTX_H__BA2C7C0A_B537_478B_8AD3_0B46EB1B9CCC__INCLUDED_)
