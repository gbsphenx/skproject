#if !defined(AFX_DM2EDITTILEPROPDLG_H__2AD1AB19_B95A_4595_9209_E686664B8845__INCLUDED_)
#define AFX_DM2EDITTILEPROPDLG_H__2AD1AB19_B95A_4595_9209_E686664B8845__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2EditTilePropDlg.h : ヘッダー ファイル
//

#include "DM2ItemListBox.h"
#include "EdPIRTSink.h"
#include "EdPIActu1.h"
#include "EdPIActu2Page.h"
#include "EdPIActu3Page.h"
#include "EdPIActuHintPage.h"
#include "EdPIChestPage.h"
#include "EdPIClothPage.h"
#include "EdPICreaturePage.h"
#include "EdPIMiscPage.h"
#include "EdPIPotionPage.h"
#include "EdPIScrollPage.h"
#include "EdPITextPage.h"
#include "EdPIText2Page.h"
#include "EdPIWeapon.h"
#include "EdPINoPage.h"
#include "EdPITele.h"
#include "EdPIDoorPage.h"
#include "DM2OrnateIdxMig.h"
#include "DM2NameResolver.h"
#include "GetBackButton.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2EditTilePropDlg ダイアログ

class CDM2EditTilePropDlg : public CDialog, public CEdPIRTSink
{
	// 
	CDDAT *m_pDDIf;
	// 
	CDM2ItemListNode *m_pTree;
	// 
	bool m_fTileIsWall;
	// 
	AtlasPos_t m_ap;

	// 
	CEdPIActu1Page m_wndActu1;
	// 
	CEdPIActu2Page m_wndActu2;
	// 
	CEdPIActu3Page m_wndActu3;
	// 
	CEdPIActuHintPage m_wndActuHint;
	// 
	CEdPIChestPage m_wndChest;
	// 
	CEdPIClothPage m_wndCloth;
	// 
	CEdPICreaturePage m_wndCreature;
	// 
	CEdPIMiscPage m_wndMisc;
	// 
	CEdPIPotionPage m_wndPotion;
	// 
	CEdPIScrollPage m_wndScroll;
	// 
	CEdPITextPage m_wndText;
	// 
	CEdPIText2Page m_wndText2;
	// 
	CEdPIWeaponPage m_wndWeapon;
	// 
	CPropertySheet m_wndSheet;
	// 
	CEdPINoPage m_wndNo;
	// 
	CEdPITelePage m_wndTele;
	// 
	CEdPIDoorPage m_wndDoor;

	// 
	CArray<CEdPIDlg *, CEdPIDlg *> m_vecTab;
	// 
	CEdPIDlg *m_pActive;
	// 
	int m_iOldSel;
	// 
	int m_iNewSel;
	// 
	CDM2OrnateIdxMig m_oim;

	// 
	DM2NameResolver_t m_nameRes;

	// 
	CGetBackButton m_wndBackBtn;

	// 
	CDM2ItemListNode *MkNode();
	// 
	bool WriteBack(CDM2ItemListNode *p);
	// 
	RN MkBackTree(CDM2ItemListNode *p);

	// 
	void Tab_RemoveAll();
	// 
	bool Tab_Add(CEdPIDlg &wnd);

	// 
	void OnCommand0100();
	// 
	void ForceInactivate();
	// 
	void ForceActivate();

public:
	// 
	CDM2EditTilePropDlg(CWnd* pParent = NULL);
	// 
	~CDM2EditTilePropDlg();
	// 
	bool Init(CDDAT *pDDIf, AtlasPos_t ap);

	// 
	virtual BYTE GetDB();
	// 
	virtual bool GetRecord(DMDBU &rec);
	// 
	virtual bool SetRecord(const DMDBU &rec);
	// 
	virtual bool TileIsWall();
	// 
	virtual CDDAT *GetDDIf() { return m_pDDIf; }
	// 
	virtual bool GetTilePos(AtlasPos_t &ap) { ap = m_ap; return true; }
	// 
	virtual CDM2OrnateIdxMig *GetOIM() { return &m_oim; }
	// 
	virtual DM2NameResolver_t *GetNameRes() { return &m_nameRes; }

// ダイアログ データ
	//{{AFX_DATA(CDM2EditTilePropDlg)
	enum { IDD = IDD_EDIT_TILEPROP };
	CTabCtrl	m_wndTab;
	CButton	m_wndBtnAddSubLast;
	CButton	m_wndBtnAddSubFirst;
	CButton	m_wndBtnRemove;
	CButton	m_wndBtnInsertBefore;
	CButton	m_wndBtnInsertAfter;
	CButton	m_wndBtnAppend;
	CButton	m_wndBtnClone;
	CButton	m_wndDirW;
	CButton	m_wndDirS;
	CButton	m_wndDirN;
	CButton	m_wndDirE;
	CComboBox	m_wndObj;
	CDM2ItemListBox	m_wndItemList;
	int		m_iObjSel;
	int		m_iTypeSel;
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDM2EditTilePropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDM2EditTilePropDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboType();
	afx_msg void OnSelchangeComboObj();
	afx_msg void OnSelchangeListItems();
	afx_msg void OnButtonRecordRemove();
	afx_msg void OnButtonRecordClone();
	afx_msg void OnButtonRecordAppend();
	afx_msg void OnButtonRecordInsertAfter();
	afx_msg void OnButtonRecordInsertBefore();
	afx_msg void OnButtonRecordAddSubFirst();
	afx_msg void OnButtonRecordAddSubLast();
	virtual void OnOK();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonGetBack();
	//}}AFX_MSG
	afx_msg void OnCheckDirN(UINT nID);
	afx_msg void OnContextCopy(UINT nID);
	afx_msg void OnUpdateContextCopy(CCmdUI* pCmdUI);
	afx_msg void OnAHMT(UINT nID);
	afx_msg LRESULT OnCollectSampler(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2EDITTILEPROPDLG_H__2AD1AB19_B95A_4595_9209_E686664B8845__INCLUDED_)
