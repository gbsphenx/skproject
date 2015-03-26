// CurDir.h: CCurDir クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CURDIR_H__23B9417F_A268_4CB7_BAE6_63776E9A45BE__INCLUDED_)
#define AFX_CURDIR_H__23B9417F_A268_4CB7_BAE6_63776E9A45BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCurDir  
{
	// 
	CString strCurDir;

public:
	// 
	CCurDir()
	{
		strCurDir = GetCurDir();
	}
	// 
	explicit CCurDir(const CCurDir &s)
	{
		strCurDir = GetCurDir();

		SetCurrentDirectory(s.GetDir());
	}
	// 
	~CCurDir()
	{
		if (strCurDir.IsEmpty()) return;

		SetCurrentDirectory(strCurDir);
	}
	// 
	CString GetDir() const
	{
		return strCurDir;
	}
	// 
	static CString GetCurDir()
	{
		TCHAR tc[MAX_PATH] = "";
		GetCurrentDirectory(MAX_PATH, tc);
		return tc;
	}
};

#endif // !defined(AFX_CURDIR_H__23B9417F_A268_4CB7_BAE6_63776E9A45BE__INCLUDED_)
