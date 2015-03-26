
#if !defined(AFX_HUGHUG_H__4D381CA6_BCA2_42C9_9789_D26C017EBCBC__INCLUDED_)
#define AFX_HUGHUG_H__4D381CA6_BCA2_42C9_9789_D26C017EBCBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CurDir.h"
#include <comdef.h>

struct CTempKeeper : IUnknown
{
	// 
	CString strTemp;

	// 
	STDMETHOD(QueryInterface)(
		REFIID riid,
		LPVOID *ppvObj
		)
	{
		if (ppvObj == NULL)
			return E_POINTER;
		*ppvObj = NULL;
		if (riid == IID_IUnknown) {
			*ppvObj = reinterpret_cast<LPVOID>((LPUNKNOWN)this);
			return S_OK;
		}
		return E_NOINTERFACE;
	}
	// 
	STDMETHOD_(ULONG, AddRef)()
	{
		++nLocked;

		return nLocked;
	}
	// 
	STDMETHOD_(ULONG, Release)()
	{
		if (--nLocked == 0) {
			delete this;
			return 0;
		}
		return nLocked;
	}

	// 
	static CTempKeeper *CreateInstance(LPCTSTR psz)
	{
		return new CTempKeeper(psz);
	}

protected:
	// 
	LONG nLocked;

	// 
	CTempKeeper(LPCTSTR psz)
		: nLocked(1)
		, strTemp(psz)
	{

	}
	// 
	~CTempKeeper()
	{
		ASSERT(nLocked == 0);

		VERIFY(DeleteFile(strTemp));
	}
};

class CHUGHUG  
{
	// 
	void SetAsTemp(LPCTSTR psz)
	{
		pUnk = CTempKeeper::CreateInstance(strFile = psz);
	}

public:
	// 
	CString strBaseDir;
	// 
	IUnknownPtr pUnk;
	// 
	CString strFile;

	// 
	CHUGHUG(LPCTSTR strBaseDir)
		: strBaseDir(strBaseDir)
	{

	}
	// 
	bool Extract(LPCTSTR pszFileName);
	// 
	CString Extraction(LPCTSTR pszFileName);
};

#endif // !defined(AFX_HUGHUG_H__4D381CA6_BCA2_42C9_9789_D26C017EBCBC__INCLUDED_)
