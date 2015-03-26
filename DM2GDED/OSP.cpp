
// +--------------------------------------------------
// |
// | OSP.cpp
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

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
	CString OSP_GetDir(CString sPath, bool fAddSuffix)
	{
		int i = sPath.ReverseFind('\\');
		if (i < 0)
			return "";
		return sPath.Left(i + (fAddSuffix ? 1 : 0));
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
	// 
	CString OSP_UnixToWin(CString sPath)
	{
		sPath.Replace('/', '\\');
		return sPath;
	}
	// 
	CString OSP_GetBaseName(CString sPath)
	{
		sPath = OSP_GetFileName(sPath);
		int i = sPath.ReverseFind('.');
		if (i < 0 || i == 0)
			return sPath;
		return sPath.Mid(0, i);
	}
	// 
	CString OSP_RemoveExt(CString sPath)
	{
		int i = sPath.ReverseFind('\\');
		int t = sPath.ReverseFind('.');
		if (i < t && t != 0) {
			return sPath.Mid(0, t);
		}
		return sPath;
	}
	// 
	CString OSP_GetExt(CString sPath)
	{
		int i = sPath.ReverseFind('\\');
		int t = sPath.ReverseFind('.');
		if (t < 0 || t < i)
			return "";
		return sPath.Mid(t);
	}
};
