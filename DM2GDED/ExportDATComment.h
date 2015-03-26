// ExportDATComment.h: CExportDATComment クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPORTDATCOMMENT_H__B658AA5E_905D_43B1_9961_092FCD57F3B7__INCLUDED_)
#define AFX_EXPORTDATCOMMENT_H__B658AA5E_905D_43B1_9961_092FCD57F3B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CExportDATComment
{
public:
	// 
	CString strName, strVer, strPlatform, strAuthor, strDate, strComment;

	// 
	bool Revert(LPCTSTR pszFile, bool fRevert);
};

#endif // !defined(AFX_EXPORTDATCOMMENT_H__B658AA5E_905D_43B1_9961_092FCD57F3B7__INCLUDED_)
