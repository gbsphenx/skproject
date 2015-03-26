// DM2TeVol.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "exp00069.h"
#include "DM2TeVol.h"
#include "ComDispatchDriver.h"
#include "SplitStr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace
{

CString GetMonikerDisplayName(LPBC pBC, LPMONIKER pMon)
{
	LPOLESTR psz = NULL;
	HRESULT hr;
	CString str;
	if (SUCCEEDED(hr = pMon->GetDisplayName(pBC, NULL, &psz))) {
		str = CString(psz).Mid(1);
		CoTaskMemFree(psz);
	}
	return str;
}

};

bool CDM2TeVol::Enumerate()
{
	m.clear();

	IRunningObjectTablePtr pROT;
	HRESULT hr;
	IBindCtxPtr pBC;
	IEnumMonikerPtr pDic;
	IEnumMonikerPtr pLocDic;
	if (true
		&& !FAILED(hr = GetRunningObjectTable(0, &pROT))
		&& !FAILED(hr = CreateBindCtx(0, &pBC))
		&& !FAILED(hr = pROT->EnumRunning(&pDic))
	) {
		IMonikerPtr pMonx, pMon1, pMon2;
		IUnknownPtr pObj;
		while ((hr = pDic->Next(1, &pMonx, NULL)) == S_OK) {
			if (false
				|| FAILED(hr = pMonx->Enum(true, &pLocDic))
				|| pLocDic == NULL
				|| pLocDic->Next(1, &pMon1, NULL) != S_OK
				|| GetMonikerDisplayName(pBC, pMon1) != "DM2GDED_Rev_1"
				|| pLocDic->Next(1, &pMon2, NULL) != S_OK
				|| pROT->GetObject(pMonx, &pObj)
			) {
				continue;
			}
			CDM2TeComm a;
			a.pAx = pObj;
			m[GetMonikerDisplayName(pBC, pMon2)] = a;
		}
		return true;
	}
	return false;
}

bool CDM2TeCommTrop::Enumerate(CDM2TeComm &r)
{
	for (int i = 0; i < 256; i++) strMes[i].Empty();

	CComDispatchDriver pAx = r.pAx;
	if (pAx == NULL) return false;

	try {
		_variant_t vtRet;
		_variant_t vt1((LONG)iEC);
		HRESULT hr;
		if (SUCCEEDED(hr = pAx.Invoke1(L"GetMessageText", &vt1, &vtRet))) {
			CString strText = (BSTR)_bstr_t(vtRet);
			list<CString> listMes;
			SplitStr(strText, '\b', listMes);
			for (int x = 0; x < 256 && !listMes.empty(); x++, listMes.pop_front()) strMes[x] = listMes.front();
			return true;
		}
	} catch (_com_error err) {

	}
	return false;
}
