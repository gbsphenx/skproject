// ExportDATComment.cpp: CExportDATComment クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "ExportDATComment.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace
{

CString Add0(CString str)
{
	int nLen = str.GetLength();
	LPSTR psz = str.GetBuffer(nLen + 2);
	psz[nLen] = psz[nLen + 1] = 0;
	str.ReleaseBuffer(nLen + 1);
	return str;
}

};

//////////////////////////////////////////////////////////////////////
// CExportDATComment

bool CExportDATComment::Revert(LPCTSTR pszFile, bool fRevert)
{
	CWinApp *pApp = AfxGetApp();

	if (fRevert) {
		if (false
			|| !LoadProfileStringFrom("DM2GDED.Basis", "Name", strName, "", pszFile)
			|| !LoadProfileStringFrom("DM2GDED.Basis", "Version", strVer, "", pszFile)
			|| !LoadProfileStringFrom("DM2GDED.Basis", "Platform", strPlatform, "", pszFile)
			|| !LoadProfileStringFrom("DM2GDED.Basis", "Author", strAuthor, "", pszFile)
			|| !LoadProfileStringFrom("DM2GDED.Basis", "Date", strDate, "", pszFile)
			|| !LoadProfileSectionFrom("DM2GDED.Basis.Comment", strComment, pszFile)
		) {
			return false;
		}
	} else {
		if (false
			|| !SaveProfileStringTo("DM2GDED.Basis", "Name", strName, pszFile)
			|| !SaveProfileStringTo("DM2GDED.Basis", "Version", strVer, pszFile)
			|| !SaveProfileStringTo("DM2GDED.Basis", "Platform", strPlatform, pszFile)
			|| !SaveProfileStringTo("DM2GDED.Basis", "Author", strAuthor, pszFile)
			|| !SaveProfileStringTo("DM2GDED.Basis", "Date", strDate, pszFile)
			|| !WritePrivateProfileSection("DM2GDED.Basis.Comment", Add0(strComment), pszFile)
		) {
			return false;
		}
	}
	return true;
}
