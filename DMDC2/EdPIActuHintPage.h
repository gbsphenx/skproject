#if !defined(AFX_EDPIACTUHINTPAGE_H__1E8EA11A_BB41_4435_9FC2_99766D35DFF1__INCLUDED_)
#define AFX_EDPIACTUHINTPAGE_H__1E8EA11A_BB41_4435_9FC2_99766D35DFF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPIActuHintPage.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CEdPIActuHintPage ダイアログ

#include "EdPIDlg.h"
#include "AHRichEditCtrl.h"
#include "ActuHintWebFrm.h"

class CEdPIActuHintPage : public CEdPIDlg
{
	// 
	BYTE m_iActuType;
	// 
	BYTE m_lockWarp;
	// 
	CActuHintWebFrm m_webfrm;
	// 
	BYTE m_iDisplayActuType;

	// 
	void loadHintRtf();

	// 
	DECLARE_DYNCREATE(CEdPIActuHintPage)

public:
	// 
	CEdPIActuHintPage();

	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

	virtual void AfterRevert();

// ダイアログ データ
	//{{AFX_DATA(CEdPIActuHintPage)
	enum { IDD = IDD_PI_ACTUHINT };
	CButton	m_wndButton;
	CStatic	m_wndArea;
	CComboBox	m_wndSel;
	int		m_iSel;
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEdPIActuHintPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEdPIActuHintPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboType();
	virtual void OnCancel();
	afx_msg void OnButtonEdit();
	afx_msg void OnContextEditFp();
	afx_msg void OnContextEditNotepad();
	afx_msg void OnButtonSampler();
	//}}AFX_MSG
	LPARAM OnClickAHMT(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDPIACTUHINTPAGE_H__1E8EA11A_BB41_4435_9FC2_99766D35DFF1__INCLUDED_)
