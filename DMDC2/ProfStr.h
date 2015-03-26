
#ifndef _PROFSTR_H_
#define _PROFSTR_H_

class ProfStr {
public:
	static BOOL SaveProfileStringTo(LPCTSTR pszAppName, LPCTSTR pszKey, LPCTSTR pszValue, LPCTSTR pszFileInto)
	{
		if (!WritePrivateProfileString(pszAppName, pszKey, pszValue, pszFileInto))
			return FALSE;
		return TRUE;
	}

	static BOOL LoadProfileStringFrom(LPCTSTR pszAppName, LPCTSTR pszKey, CString &strValue, LPCTSTR pszDefault, LPCTSTR pszFileIn)
	{
		TCHAR tc[MAX_PATH] = {""};
		if (GetPrivateProfileString(pszAppName, pszKey, pszDefault, tc, MAX_PATH, pszFileIn) > 0) {
			strValue = tc;
			return TRUE;
		}
		return FALSE;
	}

	static BOOL LoadProfileSectionFrom(LPCTSTR pszAppName, CString &strValue, LPCTSTR pszFileIn)
	{
		const UINT cbVuff = 8192;
		TCHAR tcVuff[cbVuff] = {""};
		if (GetPrivateProfileSection(pszAppName, tcVuff, cbVuff, pszFileIn) > 0) {
			strValue = tcVuff; 
			return TRUE;
		}
		return FALSE;
	}

	static int GetProfileIntFrom(LPCTSTR pszAppName, LPCTSTR pszKey, int nDefault, LPCTSTR pszFileIn)
	{
		return GetPrivateProfileInt(pszAppName, pszKey, nDefault, pszFileIn);
	}

	static BOOL SaveProfileIntTo(LPCTSTR pszAppName, LPCTSTR pszKey, int nValue, LPCTSTR pszFileIn)
	{
		CString strVal; strVal.Format("%d", nValue);

		return WritePrivateProfileString(pszAppName, pszKey, strVal, pszFileIn);
	}
};

#endif
