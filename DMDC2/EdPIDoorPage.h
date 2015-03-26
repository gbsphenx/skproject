#if !defined(AFX_EDPIDOORPAGE_H__3A3EBD06_1542_4D9C_98D1_54C129188AC9__INCLUDED_)
#define AFX_EDPIDOORPAGE_H__3A3EBD06_1542_4D9C_98D1_54C129188AC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPIDoorPage.h : ヘッダー ファイル
//

#include "EdPIDlg.h"
#include "DM2Ornate1ComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CEdPIDoorPage ダイアログ

class CEdPIDoorPage : public CEdPIDlg
{
	// 
	DECLARE_DYNCREATE(CEdPIDoorPage)

public:
	// 
	CEdPIDoorPage();

	// 
	virtual bool IsAcc(UINT nID);
	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

// ダイアログ データ
	//{{AFX_DATA(CEdPIDoorPage)
	enum { IDD = IDD_PI_DOOR };
	CDM2Ornate1ComboBox	m_wndDoor;
	CDM2Ornate1ComboBox	m_wndOrnate;
	BOOL	m_fBashOk;
	BOOL	m_fDestroyOk;
	BOOL	m_fDoorBtn;
	BOOL	m_fDoor2;
	BOOL	m_fA;
	BOOL	m_fB;
	BOOL	m_fC;
	BOOL	m_fD;
	BOOL	m_fE;
	BOOL	m_fF;
	BOOL	m_f9;
	BOOL	m_fVert;
	int		m_iOrnate;
	int		m_iDoor;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CEdPIDoorPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEdPIDoorPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckDoorType();
	afx_msg void OnSelchangeComboDoor();
	afx_msg void OnSelchangeComboOrnate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDPIDOORPAGE_H__3A3EBD06_1542_4D9C_98D1_54C129188AC9__INCLUDED_)
