// Exp00069.h : EXP00069 アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_EXP00069_H__859B3A4C_4D4A_4BFF_A76F_4F627DFC256B__INCLUDED_)
#define AFX_EXP00069_H__859B3A4C_4D4A_4BFF_A76F_4F627DFC256B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

#include "ExtBM.h"
#include "DM2OrnateTreeDic.h"
#include "BMO.h"

void ReadFontTemplateFrom(LOGFONT &lf, int nID, LPCTSTR lpszDef);

class COnIdle
{
public:
	// 
	virtual BOOL OnIdleNotify() = NULL;
};

/////////////////////////////////////////////////////////////////////////////
// CExp00069App:
// このクラスの動作の定義に関しては Exp00069.cpp ファイルを参照してください。
//

class CExp00069App : public CWinApp
{
	// 
	CArray<COnIdle *, COnIdle *> m_listIdleTar;
	// 
	CExtBM m_bm[16][2];
	// 
	CExtBM m_bmRoom[8];
	// 
	CExtBM m_bmPT[8];
	// 
	CExtBM m_bmTile[16];
	// 
	CExtBM m_bmHM[4];
	// 
	CExtBM m_bmDoor[2];
	// 
	CExtBM m_bm64x64[8][2];
	// 
	CExtBM m_bmTS[2];
	// 
	CExtBM m_bmBlack;
	// 
	CString m_strWorkDir;
	// 
	CString m_strAssets;
	// 
	CString m_strCnf;

	// 
	CBMOVec m_bmovec;

public:
	// 
	static CString GetNeighborFile(CString str)
	{
		TCHAR tc[256];
		tc[0] = 0;
		GetModuleFileName(GetModuleHandle(NULL), tc, 256);
		return JoinPath(GetDir(tc), str);
	}
	// 
	static CString GetDir(CString str)
	{
		int i = str.ReverseFind('\\');
		if (i < 0)
			return str;
		return str.Left(i + 1);
	}
	// 
	static CString JoinPath(CString s1, CString s2)
	{
		if (s1.ReverseFind('\\') != s1.GetLength() - 1)
			s1 += "\\";
		if (s2[0] == '\\')
			s2 = (LPCTSTR)s2.Mid(1);
		return s1 + s2;
	}
	// 
	LPCTSTR GetWorkDir() const { return m_strWorkDir; }
	// 
	CString GetWorkDirAt(CString suffix) const;
	// 
	CString GetCnf() const { return m_strCnf; }
	// 
	HBITMAP GetAnyBM(HBITMAP hbm)
	{
		if (hbm)
			return hbm;
		return m_bmBlack;
	}
	// 
	HBITMAP GetRoomBM(BYTE i)
	{
		return GetAnyBM(m_bmRoom[i & 7]);
	}
	// 
	HBITMAP GetPTBM(BYTE i)
	{
		return GetAnyBM(m_bmPT[i & 7]);
	}
	// 
	HBITMAP GetTileBM(BYTE i)
	{
		return GetAnyBM(m_bmTile[i & 15]);
	}
	// 
	HBITMAP GetHMBM(BYTE i = 0)
	{
		return GetAnyBM(m_bmHM[i & 3]);
	}
	// 
	BOOL RenderOrnate64To(CDC *pDC, int x, int y, BYTE c0, BYTE c1, int cx = 64);
	// 
	BOOL RenderItem16To(CDC *pDC, int x, int y, BYTE db, BYTE idx, int cx = 16);
	// 
	HBITMAP GetTileState(BYTE i = 0)
	{
		return GetAnyBM(m_bmTS[i & 1]);
	}

	// 
	CBMO *FindBMO(DWORD key)
	{
		return m_bmovec.FindBMO(key);
	}

	// 
	static WORD ID2CHES(WORD x)
	{
		return (x % 8) * 0x2000 + ((x / 8) % 4) * 0x0002;
	}
	// 
	static WORD CHES2ID(WORD x)
	{
		return (x / 0x2000) | (((x & 0x0007) / 0x0002) * 8);
	}

	// 
	CExp00069App();
	// 
	void RegisterIdleCB(COnIdle *pCB);
	// 
	void RevokeIdleCB(COnIdle *pCB);
	// 
	bool LoadOrnates();
	// 
	bool LoadOrnates2();
	// 
	bool LoadDic();

	// 
	CString GetAssets() const {
		return m_strAssets;
	}
	// 
	void SetAssets(CString s);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CExp00069App)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// インプリメンテーション
	//{{AFX_MSG(CExp00069App)
	afx_msg void OnAppAbout();
	afx_msg void OnOptReloadOrnates();
	afx_msg void OnOptReloadDic();
	afx_msg void OnHelpShowcase();
	afx_msg void OnHelpFindervw();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CExp00069App *GetApp();

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EXP00069_H__859B3A4C_4D4A_4BFF_A76F_4F627DFC256B__INCLUDED_)
