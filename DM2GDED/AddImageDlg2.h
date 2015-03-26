#if !defined(AFX_ADDIMAGEDLG2_H__D7F0806F_BC07_4372_88AF_6707CC700878__INCLUDED_)
#define AFX_ADDIMAGEDLG2_H__D7F0806F_BC07_4372_88AF_6707CC700878__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddImageDlg2.h : ヘッダー ファイル
//

#include "GDEdCtx.h"
#include "EditVPg.h"
#include "EditVxxx.h"
#include "AnchorDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CAddImageDlg2 ダイアログ

class CAddImageDlg2 : public CAnchorDlg
{
	// 
	CGDEntry m_pos;
	// 
	int m_nEntryMask;
	// 
	CString m_strNewCaption;
	// 
	UINT m_nVal;
	// 
	CString m_strText;
	// 
	CEditVxxx *m_vxxx;
	// 
	CRect m_rcFrame;

	// 
	CEditVPg *m_wndUsingPg;

	// 
	void Construction();
	// 
	void Use(CEditVPg &wndPg, bool fUseText);

public:
	// 
	CAddImageDlg2(CWnd* pParent = NULL);
	// 
	CAddImageDlg2(CEditVxxx *vxxx, CWnd* pParent = NULL);

	// 
	const CGDEntry &GetEntryPos() { return m_pos; }

	// 
	void InitAddText(CGDEntry pos);
	// 
	void InitAdd0B(CGDEntry pos);
	// 
	void InitAdd0C(CGDEntry pos);

	// 
	void InitEditText(CGDEntry pos, LPCSTR pszText);
	// 
	void InitEdit0B(CGDEntry pos, UINT nVal);
	// 
	void InitEdit0C(CGDEntry pos, UINT nVal);

	// 
	void InitRename(CGDEntry pos);
	// 
	void InitAddClass2(CGDEntry pos);

	// 
	UINT GetVal()
	{
		if (!m_wndUsingPg) return 0;

		return m_wndUsingPg->GetVal();
	}
	// 
	CString GetText()
	{
		if (!m_wndUsingPg) return "";

		return m_wndUsingPg->GetText();
	}

// ダイアログ データ
	//{{AFX_DATA(CAddImageDlg2)
	enum { IDD = IDD_ADD_IMAGE2 };
	CButton	m_wndOk;
	CEdit	m_wndC6;
	CEdit	m_wndC5;
	CEdit	m_wndC4;
	CEdit	m_wndC3;
	CEdit	m_wndC2;
	CEdit	m_wndC1;
	CTabCtrl	m_wndTab;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAddImageDlg2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CAddImageDlg2)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ADDIMAGEDLG2_H__D7F0806F_BC07_4372_88AF_6707CC700878__INCLUDED_)
