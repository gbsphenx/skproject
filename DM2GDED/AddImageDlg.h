#if !defined(AFX_ADDIMAGEDLG_H__9AA27745_8E3B_4377_8455_597FFD83354C__INCLUDED_)
#define AFX_ADDIMAGEDLG_H__9AA27745_8E3B_4377_8455_597FFD83354C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddImageDlg.h : ヘッダー ファイル
//

#include "GDAT.h"
#include "EditPCMCB.h"
#include "WVCT.h"
#include "EditVxxx.h"

/////////////////////////////////////////////////////////////////////////////
// CAddImageDlg ダイアログ

class CAddImageDlg : public CDialog
{
	// 
	CGDEntry m_pos;
	// 
	int m_nEntryMask;
	// 
	CEditPCMCB *m_pCB;
	// 
	BOOL m_nEntryCtxFlag;
	// 
	CString m_strNewCaption;
	// 
	CEditVxxx &m_vxxx;
	// 
	bool m_fEditFlags;
	// 
	bool m_fFlags[16];

	// 
	void Construction();
	// 
	CString FormatFlagText(UINT i, LPCTSTR psz);

public:
	// 
	CAddImageDlg(CWnd* pParent = NULL);
	// 
	CAddImageDlg(CEditVxxx &vxxx, CWnd* pParent = NULL);
	// 
	void InitAddImage(CGDEntry pos);
	// 
	void InitAddText(CGDEntry pos);
	// 
	void InitAdd0B(CGDEntry pos);
	// 
	void InitAdd0C(CGDEntry pos);
	// 
	const CGDEntry &GetEntryPos() { return m_pos; }
	// 
	void InitEditText(CGDEntry pos, LPCSTR pszText);
	// 
	void InitEdit0B(CGDEntry pos, UINT nVal);
	// 
	void InitEdit0C(CGDEntry pos, UINT nVal);
	// 
	UINT GetVal();
	// 
	void InitEditPCM(CGDEntry pos, CEditPCMCB *pCB);
	// 
	void InitAddPCM(CGDEntry pos, CEditPCMCB *pCB);
	// 
	void InitAddClass2(CGDEntry pos);
	// 
	void InitAdd04(CGDEntry pos);
	// 
	void InitAdd07(CGDEntry pos);
	// 
	void InitAdd08(CGDEntry pos);
	// 
	void InitAdd09(CGDEntry pos);
	// 
	void InitAdd0E(CGDEntry pos);
	// 
	void InitRename(CGDEntry pos);

	// ダイアログ データ
	//{{AFX_DATA(CAddImageDlg)
	enum { IDD = IDD_ADD_IMAGE };
	CListBox	m_wndFlags;
	CButton	m_wndEditPCM;
	CButton	m_wndOk;
	CEdit	m_wndVal;
	CEdit	m_wndText;
	CEdit	m_wndC6;
	CEdit	m_wndC5;
	CEdit	m_wndC4;
	CEdit	m_wndC3;
	CEdit	m_wndC2;
	CEdit	m_wndC1;
	CString	m_strVal;
	CString	m_strText;
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAddImageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CAddImageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonEdit();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ADDIMAGEDLG_H__9AA27745_8E3B_4377_8455_597FFD83354C__INCLUDED_)
