
#pragma once

#include <atlbase.h>

// from atlcom.h

//////////////////////////////////////////////////////////////////////////////
// CComDispatchDriver / Specialization of CComQIPtr<IDispatch, IID_IDispatch>
class CComDispatchDriver
{
public:
	CComDispatchDriver()
	{
		p = NULL;
	}
	CComDispatchDriver(IDispatch* lp)
	{
		if ((p = lp) != NULL)
			p->AddRef();
	}
	CComDispatchDriver(IUnknown* lp)
	{
		p=NULL;
		if (lp != NULL)
			lp->QueryInterface(IID_IDispatch, (void **)&p);
	}
	~CComDispatchDriver() { if (p) p->Release(); }
	void Release() {if (p) p->Release(); p=NULL;}
	operator IDispatch*() {return p;}
	IDispatch& operator*() {ATLASSERT(p!=NULL); return *p; }
	IDispatch** operator&() {ATLASSERT(p==NULL); return &p; }
	IDispatch* operator->() {ATLASSERT(p!=NULL); return p; }
	IDispatch* operator=(IDispatch* lp){return (IDispatch*)AtlComPtrAssign((IUnknown**)&p, lp);}
	IDispatch* operator=(IUnknown* lp)
	{
		return (IDispatch*)AtlComQIPtrAssign((IUnknown**)&p, lp, IID_IDispatch);
	}
	BOOL operator!(){return (p == NULL) ? TRUE : FALSE;}

	HRESULT GetPropertyByName(LPCOLESTR lpsz, VARIANT* pVar)
	{
		ATLASSERT(p);
		ATLASSERT(pVar);
		DISPID dwDispID;
		HRESULT hr = GetIDOfName(lpsz, &dwDispID);
		if (SUCCEEDED(hr))
			hr = GetProperty(p, dwDispID, pVar);
		return hr;
	}
	HRESULT GetProperty(DISPID dwDispID, VARIANT* pVar)
	{
		ATLASSERT(p);
		return GetProperty(p, dwDispID, pVar);
	}
	HRESULT PutPropertyByName(LPCOLESTR lpsz, VARIANT* pVar)
	{
		ATLASSERT(p);
		ATLASSERT(pVar);
		DISPID dwDispID;
		HRESULT hr = GetIDOfName(lpsz, &dwDispID);
		if (SUCCEEDED(hr))
			hr = PutProperty(p, dwDispID, pVar);
		return hr;
	}
	HRESULT PutProperty(DISPID dwDispID, VARIANT* pVar)
	{
		ATLASSERT(p);
		return PutProperty(p, dwDispID, pVar);
	}
	HRESULT GetIDOfName(LPCOLESTR lpsz, DISPID* pdispid)
	{
		return p->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpsz, 1, LOCALE_USER_DEFAULT, pdispid);
	}
	// Invoke a method by DISPID with no parameters
	HRESULT Invoke0(DISPID dispid, VARIANT* pvarRet = NULL)
	{
		DISPPARAMS dispparams = { NULL, NULL, 0, 0};
		return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	}
	// Invoke a method by name with no parameters
	HRESULT Invoke0(LPCOLESTR lpszName, VARIANT* pvarRet = NULL)
	{
		HRESULT hr;
		DISPID dispid;
		hr = GetIDOfName(lpszName, &dispid);
		if (SUCCEEDED(hr))
			hr = Invoke0(dispid, pvarRet);
		return hr;
	}
	// Invoke a method by DISPID with a single parameter
	HRESULT Invoke1(DISPID dispid, VARIANT* pvarParam1, VARIANT* pvarRet = NULL)
	{
		DISPPARAMS dispparams = { pvarParam1, NULL, 1, 0};
		return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	}
	// Invoke a method by name with a single parameter
	HRESULT Invoke1(LPCOLESTR lpszName, VARIANT* pvarParam1, VARIANT* pvarRet = NULL)
	{
		HRESULT hr;
		DISPID dispid;
		hr = GetIDOfName(lpszName, &dispid);
		if (SUCCEEDED(hr))
			hr = Invoke1(dispid, pvarParam1, pvarRet);
		return hr;
	}
	// Invoke a method by DISPID with two parameters
	HRESULT Invoke2(DISPID dispid, VARIANT* pvarParam1, VARIANT* pvarParam2, VARIANT* pvarRet = NULL)
	{
		CComVariant varArgs[2] = { *pvarParam2, *pvarParam1 };
		DISPPARAMS dispparams = { &varArgs[0], NULL, 2, 0};
		return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	}
	// Invoke a method by name with two parameters
	HRESULT Invoke2(LPCOLESTR lpszName, VARIANT* pvarParam1, VARIANT* pvarParam2, VARIANT* pvarRet = NULL)
	{
		HRESULT hr;
		DISPID dispid;
		hr = GetIDOfName(lpszName, &dispid);
		if (SUCCEEDED(hr))
			hr = Invoke2(dispid, pvarParam1, pvarParam2, pvarRet);
		return hr;
	}
	// Invoke a method by DISPID with N parameters
	HRESULT InvokeN(DISPID dispid, VARIANT* pvarParams, int nParams, VARIANT* pvarRet = NULL)
	{
		DISPPARAMS dispparams = { pvarParams, NULL, nParams, 0};
		return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	}
	// Invoke a method by name with Nparameters
	HRESULT InvokeN(LPCOLESTR lpszName, VARIANT* pvarParams, int nParams, VARIANT* pvarRet = NULL)
	{
		HRESULT hr;
		DISPID dispid;
		hr = GetIDOfName(lpszName, &dispid);
		if (SUCCEEDED(hr))
			hr = InvokeN(dispid, pvarParams, nParams, pvarRet);
		return hr;
	}
	static HRESULT GetProperty(IDispatch* pDisp, DISPID dwDispID,
		VARIANT* pVar)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("CPropertyHelper::GetProperty\n"));
		DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
		return pDisp->Invoke(dwDispID, IID_NULL,
				LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET,
				&dispparamsNoArgs, pVar, NULL, NULL);
	}

	static HRESULT PutProperty(IDispatch* pDisp, DISPID dwDispID,
		VARIANT* pVar)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("CPropertyHelper::PutProperty\n"));
		DISPPARAMS dispparams = {NULL, NULL, 1, 1};
		dispparams.rgvarg = pVar;
		DISPID dispidPut = DISPID_PROPERTYPUT;
		dispparams.rgdispidNamedArgs = &dispidPut;

		if (pVar->vt == VT_UNKNOWN || pVar->vt == VT_DISPATCH || 
			(pVar->vt & VT_ARRAY) || (pVar->vt & VT_BYREF))
		{
			HRESULT hr = pDisp->Invoke(dwDispID, IID_NULL,
				LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUTREF,
				&dispparams, NULL, NULL, NULL);
			if (SUCCEEDED(hr))
				return hr;
		}

		return pDisp->Invoke(dwDispID, IID_NULL,
				LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT,
				&dispparams, NULL, NULL, NULL);
	}

	IDispatch* p;
};
