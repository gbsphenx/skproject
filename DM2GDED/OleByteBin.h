
#pragma once

class COleByteBin
{
	// 
	SAFEARRAY *MkSa(LPCVOID pData, DWORD nData)
	{
		SAFEARRAYBOUND sab[1];
		sab[0].lLbound = 0;
		sab[0].cElements = nData;
		LPSAFEARRAY psa = SafeArrayCreate(VT_UI1, 1, sab);
		if (psa != NULL) {
			LPVOID pLoc;
			HRESULT hr;
			if (SUCCEEDED(hr = SafeArrayAccessData(psa, &pLoc))) {
				CopyMemory(pLoc, pData, nData);
				VERIFY(SUCCEEDED(hr = SafeArrayUnaccessData(psa)));
				return psa;
			}
			VERIFY(SUCCEEDED(hr = SafeArrayDestroy(psa)));
		}
		return NULL;
	}
	// 
	LPSAFEARRAY MkSaFromVt(const VARIANT &vt)
	{
		LPCVARIANT pvt = &vt;
		if (pvt->vt == (0 |VT_VARIANT |VT_BYREF))
			pvt = pvt->pvarVal;
		if (pvt->vt == (0 |VT_UI1 |VT_ARRAY))
			return pvt->parray;
		return NULL;
	}

public:
	// 
	bool MkVt(VARIANT &vt, LPCVOID pData, DWORD nData)
	{
		HRESULT hr;
		VERIFY(SUCCEEDED(hr = VariantClear(&vt)));
		vt.parray = MkSa(pData, nData);

		if (vt.parray == NULL) return false;

		vt.vt = 0 |VT_UI1 |VT_ARRAY;
		return true;
	}
	// 
	bool MkFromVt(const VARIANT &var, SizeBuff &fv)
	{
		LPSAFEARRAY psa = MkSaFromVt(var);
		HRESULT hr;
		VARTYPE vt;
		LONG nMin, nMax;
		if (true
			&& psa != NULL 
			&& SafeArrayGetDim(psa) == 1 
			&& SUCCEEDED(hr = SafeArrayGetVartype(psa, &vt))
			&& vt == VT_UI1
			&& SUCCEEDED(hr = SafeArrayGetLBound(psa, 1, &nMin))
			&& SUCCEEDED(hr = SafeArrayGetUBound(psa, 1, &nMax))
		) {
			fv.Free();
			UINT nLen = nMax - nMin + 1;
			if (nLen == 0) {
				return true;
			} else {
				void *pv;
				if (SUCCEEDED(hr = SafeArrayAccessData(psa, &pv))) {
					fv.Alloc(nLen, pv);
					VERIFY(SUCCEEDED(hr = SafeArrayUnaccessData(psa)));

					return fv.GetData() != NULL;
				}
				return false;
			}
		}
		return false;
	}

};
