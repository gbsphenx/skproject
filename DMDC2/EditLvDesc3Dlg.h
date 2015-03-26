#if !defined(AFX_EDITLVDESC3DLG_H__FB1904F3_B702_44F9_8193_EDB36E7E9B54__INCLUDED_)
#define AFX_EDITLVDESC3DLG_H__FB1904F3_B702_44F9_8193_EDB36E7E9B54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditLvDesc3Dlg.h : ヘッダー ファイル
//

#include "DM2OrnateIdxMig.h"
#include "DM2Ornate1ComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CEditLvDesc3Dlg ダイアログ

class CEditLvDesc3Dlg : public CDialog
{
	// 
	CDM2OrnateIdxMig m_oim;

	// 
	bool CtlRevert(bool fRevert);
	// 
	void OnSel(CDM2Ornate1ComboBox &wndBox, int f, CDM2OrnateIdxArray &v, int &i);
	// 
	void OnReplace(CDM2Ornate1ComboBox &wndBox, int f, CDM2OrnateIdxArray &v, int &i);
	// 
	void OnRT(CDM2Ornate1ComboBox &wndBox, CDM2OrnateIdxArray &v, int &i);

	// 
	void ModifyBit(WORD &w2, WORD m, BOOL f)
	{
		if (f) {
			w2 |= m;
		} else {
			w2 &=~m;
		}
	}

public:
	// 
	CEditLvDesc3Dlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	// 
	bool Revert(LevelDesc_t &s, bool fRevert);

// ダイアログ データ
	//{{AFX_DATA(CEditLvDesc3Dlg)
	enum { IDD = IDD_EDITLVDESC3 };
	CSpinButtonCtrl	m_wndDepth;
	CSpinButtonCtrl	m_wndDifficulty;
	CSpinButtonCtrl	m_wndHeight;
	CSpinButtonCtrl	m_wndWidth;
	CSpinButtonCtrl	m_wndY;
	CSpinButtonCtrl	m_wndX;
	CDM2Ornate1ComboBox	m_wndCS;
	CDM2Ornate1ComboBox	m_wndWO;
	CDM2Ornate1ComboBox	m_wndFO;
	CDM2Ornate1ComboBox	m_wndDS;
	CDM2Ornate1ComboBox	m_wndDO;
	BOOL	m_fDoorType;
	int		m_iDO;
	int		m_iDS;
	int		m_iFO;
	int		m_iWO;
	int		m_iScene;
	UINT	m_cy;
	UINT	m_cx;
	UINT	m_shiftX;
	UINT	m_shiftY;
	int		m_iCS;
	BOOL	m_f1;
	BOOL	m_f2;
	BOOL	m_f3;
	BOOL	m_f4;
	BOOL	m_f5;
	UINT	m_nAltitude;
	UINT	m_nDifficulty;
	int		m_iDoorSel;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditLvDesc3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEditLvDesc3Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboDS();
	afx_msg void OnSelchangeComboDo();
	afx_msg void OnSelchangeComboFo();
	afx_msg void OnSelchangeComboWo();
	afx_msg void OnSelchangeComboCs();
	afx_msg void OnButtonReplaceWo();
	afx_msg void OnButtonReplaceFo();
	afx_msg void OnButtonReplaceDo();
	afx_msg void OnButtonReplaceCs();
	afx_msg void OnButtonRtWo();
	afx_msg void OnButtonRtFo();
	afx_msg void OnButtonRtDo();
	afx_msg void OnButtonRtCs();
	afx_msg void OnRadioDoor1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDITLVDESC3DLG_H__FB1904F3_B702_44F9_8193_EDB36E7E9B54__INCLUDED_)
