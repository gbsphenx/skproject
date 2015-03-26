
#ifndef _OSP_H_
#define _OSP_H_

namespace OSP
{
	// 
	extern CString OSP_JoinPath(CString sDir, CString sFile);
	// 
	extern CString OSP_GetTempDir(LPCTSTR pszPrefix);
	// 
	extern CString OSP_GetDir(CString sPath);
	// 
	CString OSP_GetModuleFileName(HINSTANCE hMod = NULL);
	// 
	CString OSP_GetFileName(CString sPath);
	// 
	CString OSP_GetTempFile(LPCTSTR pszExt, LPCTSTR pszPrefix = "tmp");

};

#endif // #ifndef _OSP_H_
