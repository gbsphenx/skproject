#if !defined(AFX_DMOBTREEVIEW_H__519D9A44_4245_4595_A4CF_12810D1DBF77__INCLUDED_)
#define AFX_DMOBTREEVIEW_H__519D9A44_4245_4595_A4CF_12810D1DBF77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMObTreeView.h : ヘッダー ファイル
//

#include "Exp00069Doc.h"

extern BOOL MakeHint(RN k, DBIPos_t pos, const DMDBU *pRec, CString &tstr, RN &kSub);

/////////////////////////////////////////////////////////////////////////////
// CDMObTreeView ビュー

class CDMObTreeView : public CTreeCtrl, CDMEdCtxCB
{
	// 
	struct DBITEMINFO
	{
		// 
		UINT nID;
		// 
		UINT iRealm;
		// 
		BOOL bReadOk;
		// 
		LPCSTR lpszRealm;
	};
	// 
	static DBITEMINFO m_aDBII[];
	// 
	struct ANYITEM
	{
		// 
		virtual ~ANYITEM() { }
	};
	// 
	struct ROOTITEM : public ANYITEM
	{
		// 
		virtual ~ROOTITEM() { }

	};
	// 
	struct ROOTSELITEM : public ROOTITEM
	{
		// 
		UINT lv, x, y;
		// 
		ROOTSELITEM(UINT _lv, UINT _x, UINT _y): lv(_lv), x(_x), y(_y) { }
	};
	// 
	struct ROOTDICITEM : public ROOTITEM
	{
		// 
		UINT db;
		// 
		ROOTDICITEM(UINT _db): db(_db) { }
	};
	// 
	struct ROOTBTITEM : public ROOTITEM
	{
		// 
		DBIPos_t pos;
		// 
		ROOTBTITEM(DBIPos_t _pos): pos(_pos) { }
	};
	// 
	struct ROOTDEVITEM : public ROOTITEM
	{
		// 
		ROOTDEVITEM() { }
	};
	// 
	struct DEFPERSISTITEM : public ANYITEM
	{
		// 
		DBIPos_t pos;
		// 
		DEFPERSISTITEM(DBIPos_t _pos): pos(_pos) { }
	};
	// 
	struct DEFBTITEM : public ANYITEM
	{
		// 
		BOOL bIsDb;
		// 
		DBIPos_t posDb;
		// 
		AtlasPos_t posAt;
		// 
		DEFBTITEM(DBIPos_t s): bIsDb(TRUE), posDb(s) { }
		// 
		DEFBTITEM(AtlasPos_t s): bIsDb(FALSE), posAt(s) { }
	};
	// 
	enum {
		BE_NO = 0,
		BE_SEL,
		BE_DB,
		BE_BT,
		BE_DEV,
	};
	// 
	struct BROWSEHIST {
		// 
		UINT iWhat;
		// 
		UINT iDB;
		// 
		AtlasPos_t pos;
		// 
		DBIPos_t posBT;
	};
	// 
	typedef list<BROWSEHIST> BrowseHistList;
	// 
	enum {
		UR_CHANGE_DBITEM,
		UR_INSERT_DBITEM,
	};
	// 
	struct UPDATEREASON {
		// 
		int iReason;
		// 
		DBIPos_t pos;
		// 
		UINT nSRf;
	};
	// 
	typedef int (CDMObTreeView::*EAI_t)(HTREEITEM, LPARAM);
	// 
	enum {
		IAA_NO=0,
		IAA_SUB_FIRST,
		IAA_SUB_LAST,
		IAA_SIB_PRIOR,
		IAA_SIB_NEXT,
		IAA_MASK = 0xFF,
		IAF_APPEND = 0x0100,
		IAF_MASK = 0x0100,
	};
	// 
	enum {
		BHF_UPDATE = 0x00000001,
	};

	// 
	CFont m_fontNice;
	// 
	HTREEITEM m_tiSel;
	// 
	HTREEITEM m_tiDic;
	// 
	HTREEITEM m_tiBT;
	// 
	HTREEITEM m_hRclick;
	// 
	CMenu m_aMenu;
	// 
	BOOL m_bLimitless;
	// 
	BrowseHistList m_hlBack, m_hlForward;
	// 
	CDMEdCtx *m_pEdCtx;

	// 
	CDDAT &GetDDAT()
	{
		return *m_pEdCtx->GetDDIf();
	}
	// 
	inline BOOL IsLimitless() const { return m_bLimitless; }
	// 
	BROWSEHIST &GetLastHist()
	{
		_ASSERTE(!m_hlBack.empty());
		return m_hlBack.front();
	}

	// 
	void ReadChainInto(HTREEITEM h, HTREEITEM hAfter, DBIPos_t pos, int nDepth = 0);
	// 
	HTREEITEM InsertRootSel(AtlasPos_t k);
	// 
	HTREEITEM InsertRootDic(UINT i, LPCTSTR lpszText);
	// 
	HTREEITEM InsertPersistItem(LPCTSTR lpszText, HTREEITEM hParent, HTREEITEM hInsertAfter, DBIPos_t pos);
	// 
	HTREEITEM InsertRootBT(DBIPos_t pos);
	// 
	HTREEITEM InsertBTItem(LPCTSTR lpszText, HTREEITEM hParent, HTREEITEM hInsertAfter, DBIPos_t posTar);
	// 
	HTREEITEM InsertBTItem(LPCTSTR lpszText, HTREEITEM hParent, HTREEITEM hInsertAfter, AtlasPos_t posTar);
	// 
	HTREEITEM InsertRootDev(LPCTSTR lpszText);
	// 
	BOOL ModifyPersistItem(LPCTSTR lpszText, HTREEITEM hItem, DBIPos_t pos);
	// 
	BOOL AddNewItemAt(UINT nIAA, HTREEITEM hAssoc);
	// 
	BOOL AddNewSelItemAt(ANYITEM *pAny, UINT nIAA, HTREEITEM hAssoc);
	// 
	BOOL AddNewDicItemAt(ROOTDICITEM *pRoot, UINT nIAA, HTREEITEM hAssoc);
	// 
	ROOTITEM *GetAssocRootAt(HTREEITEM h);
	// 
	ANYITEM *GetAnyAssocAt(HTREEITEM h);
	// 
	void BrowseDb(UINT i, UINT iBHF);
	// 
	void BrowseMap(AtlasPos_t k, UINT iBHF);
	// 
	void BrowseBT(DBIPos_t pos, UINT iBHF);
	// 
	void UpdateItemsIn(UPDATEREASON &rReason);
	// 
	void UpdateDescentItemsIn(HTREEITEM h1, UPDATEREASON &rReason);
	// 
	HTREEITEM WalkAndInsertPersistItem(HTREEITEM hParent, HTREEITEM hInsertAfter, DBIPos_t &pos, UINT nFlags, int nDepth = 0);
	// 
	BOOL ModifyNextLinkOfItem(HTREEITEM hItem, int iDelta, RN rn, RN &rnFollow);
	// 
	void BrowseAgain();
	// 
	void MakeNotSelected();
	// 
	BOOL WalkAndFindOriginalReferer(const DBIPos_t &pos, list<AtlasPos_t> &referer, int iDepth);
	// 
	void BrowseDevView(UINT iBHF);

	// 
	const DBITEMINFO &GetDbItemInfo(UINT nID)
	{
		int i;
		for (i=0; m_aDBII[i].nID != 0; i++) {
			if (m_aDBII[i].nID == nID) break;
		}
		return m_aDBII[i];
	}

	// 
	virtual void OnDMUpdate(UINT, void *);
	// 
	void Init(CDMEdCtx *pEdCtx);
	// 
	void InitView();
	// 
	void UpdateConf1();

public:
	// 
	BOOL Create(CDMEdCtx *pEdCtx, DWORD style, CWnd *pParentWnd, UINT nID)
	{
		if (CTreeCtrl::Create(style, CRect(0, 0, 0, 0), pParentWnd, nID)) {
			Init(pEdCtx);
			return TRUE;
		}
		return FALSE;
	}
	// 
	CDMObTreeView();
	// 
	virtual ~CDMObTreeView();
	// 
	DECLARE_DYNAMIC(CDMObTreeView)

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDMObTreeView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDMObTreeView)
	afx_msg void OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPopupInsertItemFirst();
	afx_msg void OnPopupInsertItemLast();
	afx_msg void OnPopupEditItemHere();
	afx_msg void OnPopupInsertItemBefore();
	afx_msg void OnPopupInsertItemAfter();
	afx_msg void OnPopupDeleteItemHere();
	afx_msg void OnPopupInsertSubitemAfter();
	afx_msg void OnPopupInsertSubitemBefore();
	afx_msg void OnPopupInsertItemAfterAppend();
	afx_msg void OnPopupInsertItemBeforeAppend();
	afx_msg void OnPopupInsertItemFirstAppend();
	afx_msg void OnPopupInsertItemLastAppend();
	afx_msg void OnPopupInsertSubitemFirstAppend();
	afx_msg void OnPopupInsertSubitemLastAppend();
	afx_msg void OnViewBrowseBack();
	afx_msg void OnUpdateViewBrowseBack(CCmdUI* pCmdUI);
	afx_msg void OnViewBrowseForward();
	afx_msg void OnUpdateViewBrowseForward(CCmdUI* pCmdUI);
	afx_msg void OnPopupBtItemHere();
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnViewDev();
	//}}AFX_MSG
	afx_msg void OnPopupAllDb0(UINT nID);
	afx_msg void OnUpdatePopupAllDb0(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNoCheckNoEnable(CCmdUI *pCmdUI);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DMOBTREEVIEW_H__519D9A44_4245_4595_A4CF_12810D1DBF77__INCLUDED_)
