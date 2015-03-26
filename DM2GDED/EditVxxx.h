// EditVxxx.h: CEditVxxx クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITVXXX_H__D2E6E62A_EC4C_4D60_A20E_529067E0F0FC__INCLUDED_)
#define AFX_EDITVXXX_H__D2E6E62A_EC4C_4D60_A20E_529067E0F0FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEditVPg;
class CWordValCmd;

class CEditVxxx  
{
public:
	// 
	virtual ~CEditVxxx() { }
	// 
	virtual CEditVPg &GetEditValPg() = NULL;
	// 
	virtual CWordValCmd &GetOp() = NULL;
};

class CGDEdCtx;
class CEditVLower;

class CEditVxxxFactoryImpl
{
	// 
	CMap<int, int, CEditVxxx *, CEditVxxx *> staticInstanceMap;

	// 
	CGDEdCtx *pEdCtx;
	// 
	CEditVLower *pEdLower;

public:
	// 
	CEditVxxxFactoryImpl(CGDEdCtx *pEdCtx);
	// 
	~CEditVxxxFactoryImpl();

	// 
	void SetEdCtx(CGDEdCtx *pEdCtx);

	// 
	CEditVxxx &GetVxxx(int wvct);
	// 
	CEditVxxx *CreateVxxx(int wvct);
	// 
	int ParseName(LPCSTR psz);
	// 
	int ParseName2(LPCSTR psz, BYTE c3);
};

#endif // !defined(AFX_EDITVXXX_H__D2E6E62A_EC4C_4D60_A20E_529067E0F0FC__INCLUDED_)
