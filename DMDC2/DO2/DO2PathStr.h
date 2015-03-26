#pragma once

namespace DO2
{

class CDO2PathStr
{
	// 
	TCHAR tcPath[MAX_PATH + 1];

public:
	// 
	CDO2PathStr()
	{
		tcPath[0] = 0;
	}
	// 
	CDO2PathStr(LPCTSTR psz)
	{
		if (psz == NULL) {
			tcPath[0] = 0; return;
		}

		_tcsncpy(tcPath, psz, MAX_PATH)[MAX_PATH] = 0;
	}
	// 
	CDO2PathStr(LPCTSTR psz, UINT n)
	{
		if (psz == NULL) {
			tcPath[0] = 0; return;
		}

		if (n < MAX_PATH)
			n = MAX_PATH;

		_tcsncpy(tcPath, psz, n)[n] = 0;
	}
	// 
	CDO2PathStr(const CDO2PathStr &s)
	{
		tcPath[0] = 0;

		*this = s;
	}
	// 
	const CDO2PathStr &operator =(const CDO2PathStr &s)
	{
		CopyMemory(tcPath, s.tcPath, sizeof(tcPath));
		return *this;
	}
	// 
	operator LPCTSTR() const
	{
		return tcPath;
	}
	// 
	const CDO2PathStr &operator +=(LPCTSTR psz)
	{
		LPTSTR pszPos = tcPath;
		LPTSTR pszEnd = tcPath + MAX_PATH;

		for (; pszPos[0] != 0 && pszPos < pszEnd; pszPos++);
		for (; psz[0] != 0 && pszPos < pszEnd; psz++, pszPos++) pszPos[0] = psz[0];

		*pszPos = _T('\0');

		return *this;
	}
	// 
	CDO2PathStr operator +(LPCTSTR psz) const
	{
		return CDO2PathStr(*this) += psz;
	}
	// 
	const CDO2PathStr &operator ^=(LPCTSTR psz)
	{
		LPCTSTR pszMark = _tcsrchr(tcPath, _T('\\'));
		if (pszMark == NULL || _tcsinc(pszMark)[0] != _T('\0')) *this += _T("\\");

		*this += psz;

		return *this;
	}
	// 
	CDO2PathStr operator ^(LPCTSTR psz) const
	{
		return CDO2PathStr(*this) ^= psz;
	}
	// 
	CDO2PathStr GetDir() const
	{
		LPCTSTR pszPos = _tcsrchr(tcPath, '\\');

		return CDO2PathStr(tcPath, pszPos - tcPath + 1);
	}
	// 
	CDO2PathStr GetFileName() const
	{
		LPCTSTR pszPos = _tcsrchr(tcPath, '\\');

		return CDO2PathStr(pszPos + 1);
	}
	// 
	static CDO2PathStr GetCurrentDir()
	{
		CDO2PathStr x;
		GetCurrentDirectory(MAX_PATH, x.tcPath);
		return x;
	}
	// 
	static CDO2PathStr GetModuleFileName(HMODULE h = NULL)
	{
		if (h == NULL)
			h = GetModuleHandle(NULL);
		CDO2PathStr x;
		::GetModuleFileName(h, x.tcPath, MAX_PATH);
		return x;
	}

};

};
