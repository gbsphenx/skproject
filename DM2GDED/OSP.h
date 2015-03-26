
// +--------------------------------------------------
// |
// | OSP.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#ifndef _OSP_H_
#define _OSP_H_

namespace OSP
{
	// 
	extern CString OSP_JoinPath(CString sDir, CString sFile);
	// 
	extern CString OSP_GetTempDir(LPCTSTR pszPrefix);
	// 
	extern CString OSP_GetDir(CString sPath, bool fAddSuffix = true);
	// 
	CString OSP_GetModuleFileName(HINSTANCE hMod = NULL);
	// 
	CString OSP_GetFileName(CString sPath);
	// 
	CString OSP_GetTempFile(LPCTSTR pszExt, LPCTSTR pszPrefix = "tmp");
	// 
	CString OSP_UnixToWin(CString sPath);
	// 
	CString OSP_GetBaseName(CString sPath);
	// 
	CString OSP_RemoveExt(CString strPath);
	// 
	CString OSP_GetExt(CString strPath);

};

#endif // #ifndef _OSP_H_
