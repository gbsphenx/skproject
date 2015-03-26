#if !defined(AFX_EDITLVDESCDLG_H__7FF7B447_8BE0_4F75_B7C4_F23CA91E4650__INCLUDED_)
#define AFX_EDITLVDESCDLG_H__7FF7B447_8BE0_4F75_B7C4_F23CA91E4650__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditLvDescDlg.h : ヘッダー ファイル
//

#include "DDAT.h"
#include "HexEditCB.h"

/////////////////////////////////////////////////////////////////////////////
// CEditLvDescDlg ダイアログ

class CEditLvDescDlg : public CDialog, CHexEditCB
{
	// 
	void ArrayTrans(CDataExchange *pDX, UINT nID, CString &tstr, CByteArray &arr, BOOL bSaveToArray);
	// 
	void TryHexEdit(CWnd *pTar, LPCTSTR lpszName, UINT nID, const CByteArray &arr);
	// 
	void ShowHex(UINT nID, const CByteArray &arr);
	// 
	void OnApplyHexEditCB(DWORD nCookie);

public:
	// 
	void Revert(const LevelDesc_t &s);
	// 
	void Commit(LevelDesc_t &s);

// コンストラクション
public:
	CEditLvDescDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CEditLvDescDlg)
	enum { IDD = IDD_EDITLVDESC2 };
	CString	m_str10_0_3;
	CString	m_str10_8_B;
	UINT	m_iAltitude;
	UINT	m_cxMap;
	UINT	m_cyMap;
	CString	m_strMonsters;
	UINT	m_xShift;
	UINT	m_yShift;
	UINT	m_w2;
	UINT	m_w4;
	CString	m_strWallDeco;
	UINT	m_n10_4_7;
	UINT	m_n10_C_F;
	UINT	m_n12_8_B;
	UINT	m_n12_C_F;
	UINT	m_n14_0_3;
	UINT	m_n14_4_7;
	UINT	m_n14_8_B;
	UINT	m_n14_C_F;
	//}}AFX_DATA

	CByteArray m_arr10_0_3; // 0~15
	CByteArray m_arr10_8_B; // 0~15
	CByteArray m_arrMonsters; // 0~15
	CByteArray m_arrWallDeco; // 0~15

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditLvDescDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEditLvDescDlg)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDITLVDESCDLG_H__7FF7B447_8BE0_4F75_B7C4_F23CA91E4650__INCLUDED_)
