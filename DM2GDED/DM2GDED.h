// DM2GDED.h : DM2GDED アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_DM2GDED_H__E7AFFD49_BBB5_455E_B87D_D98FD9357900__INCLUDED_)
#define AFX_DM2GDED_H__E7AFFD49_BBB5_455E_B87D_D98FD9357900__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

// SPX define _MAX and _MIN because not found in original code ? for compilation using MS VS 2008
#ifndef _MAX
#define _MAX(_a_,_b_) ( (_a_) > (_b_) ? (_a_) : (_b_) )
#endif

#ifndef _MIN
#define _MIN(_a_,_b_) ( (_a_) < (_b_) ? (_a_) : (_b_) )
#endif
// SPX

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDApp:
// このクラスの動作の定義に関しては DM2GDED.cpp ファイルを参照してください。
//

class CDM2GDEDApp;

extern
void ReadFontTemplateFrom(LOGFONT &lf, int nID, LPCTSTR lpszDef);
extern
CString LoadString(UINT nID);
extern
CDM2GDEDApp *GetApp();
extern
BOOL SaveProfileStringTo(LPCTSTR pszAppName, LPCTSTR pszKey, LPCTSTR pszValue, LPCTSTR pszFileInto);
extern
BOOL LoadProfileStringFrom(LPCTSTR pszAppName, LPCTSTR pszKey, CString &strValue, LPCTSTR pszDefault, LPCTSTR pszFileIn);
extern
BOOL LoadProfileSectionFrom(LPCTSTR pszAppName, CString &strValue, LPCTSTR pszFileIn);
extern
int GetProfileIntFrom(LPCTSTR pszAppName, LPCTSTR pszKey, int nDefault, LPCTSTR pszFileIn);
extern
BOOL SaveProfileIntTo(LPCTSTR pszAppName, LPCTSTR pszKey, int nValue, LPCTSTR pszFileIn);
extern
LPCTSTR GetCnfFilePath();
extern
int CalcWidthOfUIText(int cx);

class COnIdle
{
public:
	// 
	virtual BOOL OnIdleNotify() = NULL;
};

class CDM2GDEDApp : public CWinApp
{
	// 
	CArray<COnIdle *, COnIdle *> m_listIdleTar;
	// 
	CString m_strCnf;
	// 
	CStringList m_temp;

	// 
	bool m_fUseDetective;

	// 
	void RevertCnf(bool fRevert);

public:
	// 
	CDM2GDEDApp();
	// 
	void RegisterIdleCB(COnIdle *pCB);
	// 
	void RevokeIdleCB(COnIdle *pCB);
	// 
	LPCTSTR GetCnfFilePath() { return m_strCnf; }
	// 
	void DeleteOnClose(LPCTSTR psz) { m_temp.AddTail(psz); }
	// 
	bool GetUseDetective() { return m_fUseDetective; }
	// 
	void SetUseDetective(bool f) { m_fUseDetective = f; }

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDM2GDEDApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual int ExitInstance();
	virtual BOOL InitApplication();
	//}}AFX_VIRTUAL

// インプリメンテーション
	//{{AFX_MSG(CDM2GDEDApp)
	afx_msg void OnAppAbout();
	afx_msg void OnOptUseDetective();
	afx_msg void OnUpdateOptUseDetective(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2GDED_H__E7AFFD49_BBB5_455E_B87D_D98FD9357900__INCLUDED_)
