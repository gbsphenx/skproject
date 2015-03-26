
#include "StdAfx.h"
#include "OSP.h"

namespace OSP
{
	// 
	CString OSP_JoinPath(CString sDir, CString sFile)
	{
		int i = sDir.ReverseFind('\\');
		if (i + 1 != sDir.GetLength())
			return sDir + "\\" + sFile;
		return sDir + sFile;
	}
	// 
	CString OSP_GetTempDir(LPCTSTR pszPrefix)
	{
		TCHAR tcDir[MAX_PATH] = {""};
		GetTempPath(MAX_PATH, tcDir);
		int iSeed = (int)time(NULL);
		if (pszPrefix == NULL || !*pszPrefix) pszPrefix = "~osp";
		CString strNewDir, strAdd;
		for (int i = 0; i < 4096; i++, iSeed++) {
			strAdd.Format("%s%x", pszPrefix, iSeed % 4096U);
			strNewDir = OSP_JoinPath(tcDir, strAdd);
			if (CreateDirectory(strNewDir, NULL)) return strNewDir;
		}
		return "";
	}
	// 
	CString OSP_GetDir(CString sPath)
	{
		int i = sPath.ReverseFind('\\');
		if (i < 0)
			return "";
		return sPath.Left(i + 1);
	}
	// 
	CString OSP_GetModuleFileName(HINSTANCE hMod)
	{
		TCHAR tc[MAX_PATH] = {""};
		if (hMod == NULL) hMod = GetModuleHandle(NULL);
		GetModuleFileName(hMod, tc, MAX_PATH);
		return tc;
	}
	// 
	CString OSP_GetFileName(CString sPath)
	{
		int i = sPath.ReverseFind('\\');
		if (i < 0)
			return sPath;
		return (LPCTSTR)sPath + i + 1;
	}
	// 
	CString OSP_GetTempFile(LPCTSTR pszExt, LPCTSTR pszPrefix)
	{
		TCHAR tc[MAX_PATH] = {""};
		GetTempPath(MAX_PATH, tc);
		TCHAR tcFile[MAX_PATH] = {""};
		GetTempFileName(tc, pszPrefix, 0, tcFile);
		return CString(tcFile) + pszExt;
	}
};
