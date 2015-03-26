#if !defined(AFX_EDITVMSITEMSPG_H__2B701687_9DA7_4D59_9F6B_F4F6186555CB__INCLUDED_)
#define AFX_EDITVMSITEMSPG_H__2B701687_9DA7_4D59_9F6B_F4F6186555CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditVMSItemsPg.h : ヘッダー ファイル
//

#include "EditVPg.h"
#include "EditVxxx.h"
#include "DM2RTextToken.h"
#include "AnchorDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CEditVMSItemsPg ダイアログ

class CEditVMSItemsPg : public CEditVPg, CAnchorSupport
{
	// 
	class DropHere : public COleDropTarget {
		// 
		CListCtrl &wndList;
		// 
		CEditVMSItemsPg &parent;
		// 
		CString strMe;

	public:
		// 
		DropHere(CListCtrl &wndList, CEditVMSItemsPg &parent, CString strMe)
			: wndList(wndList)
			, parent(parent)
			, strMe(strMe)
		{

		}

		// 
		class LVUt {
		public:
			static void SetInsertMark(CListCtrl &wndList, int i, bool fErase = false) {
#if WINVER >= 0x0501 // XP or later
				bool fLastItem = (!fErase && i < 0);
				LVINSERTMARK mark;
				ZeroMemory(&mark, sizeof(mark));
				mark.cbSize = sizeof(mark);
				if (fErase) {
					mark.iItem = -1;
				}
				else if (fLastItem) {
					mark.dwFlags = LVIM_AFTER;
					mark.iItem = wndList.GetItemCount() -1;
				}
				else {
					mark.iItem = i;
				}

				wndList.SetInsertMark(&mark);
#endif
			}
		};

		// 
		class DOUt {
		public:
			static CString GetText(COleDataObject* pDataObject, LPCTSTR pszDefault = NULL) {
				HGLOBAL hMem = pDataObject->GetGlobalData(CF_DSPTEXT);
				if (hMem != NULL) {
					PVOID pv = GlobalLock(hMem);
					if (pv != NULL) {
						size_t cb = GlobalSize(hMem);
						CString str((LPCSTR)pv, cb);
						GlobalUnlock(hMem);

						return str;
					}
					GlobalUnlock(hMem);
				}
				return pszDefault;
			}
		};

		//
		virtual DROPEFFECT OnDragEnter(
			CWnd* pWnd,
			COleDataObject* pDataObject,
			DWORD dwKeyState,
			CPoint point 
		) {
			return OnDragOver(pWnd, pDataObject, dwKeyState, point);
		}
		// 
		virtual DROPEFFECT OnDragOver(
			CWnd* pWnd,
			COleDataObject* pDataObject,
			DWORD dwKeyState,
			CPoint point 
		) {
			CString strFrm = DOUt::GetText(pDataObject);
			if (strFrm.GetLength() != 0) {
				if (strFrm == _T("sel") && strMe == _T("all")) { // sel -> all = delete items
					return DROPEFFECT_MOVE|DROPEFFECT_SCROLL;
				}
				else if (strFrm == _T("all") && strMe == _T("all")) { // all -> all = cannot do
					return DROPEFFECT_NONE;
				}
				else { // xxx -> sel = move/copy items
					LVUt::SetInsertMark(wndList, wndList.HitTest(point));

					if ((dwKeyState & MK_SHIFT) != 0)
						return DROPEFFECT_MOVE|DROPEFFECT_SCROLL;

					if ((dwKeyState & MK_CONTROL) != 0)
						return DROPEFFECT_COPY|DROPEFFECT_SCROLL;

					if (strFrm == _T("all")) { // all -> sel = copy default
						return DROPEFFECT_COPY|DROPEFFECT_MOVE|DROPEFFECT_SCROLL;
					}
					else { // sel -> sel = move default
						return DROPEFFECT_MOVE|DROPEFFECT_SCROLL;
					}
				}
			}
			return DROPEFFECT_NONE;
		}
		// 
		virtual void OnDragLeave(
			CWnd* pWnd 
		) {
			LVUt::SetInsertMark(wndList, -1, true);
		}
		// 
		virtual BOOL OnDrop(
			CWnd* pWnd,
			COleDataObject* pDataObject,
			DROPEFFECT dropEffect,
			CPoint point 
		) {
			LVUt::SetInsertMark(wndList, -1, true);

			if ((dropEffect & (DROPEFFECT_COPY|DROPEFFECT_MOVE)) != 0) {
				int i = wndList.HitTest(point);

				HGLOBAL hMem = pDataObject->GetGlobalData(CF_DSPTEXT);
				if (hMem != NULL) {
					PVOID pv = GlobalLock(hMem);
					if (pv != NULL) {
						size_t cb = GlobalSize(hMem);
						CString str((LPCSTR)pv, cb);
						GlobalUnlock(hMem);

						parent.OnDropItems(str, (dropEffect & DROPEFFECT_COPY) != 0, i, *this);
						return true;
					}
					GlobalUnlock(hMem);
				}
			}
			return false;
		}

	};

	// 
	CDM2ItemSelList m_tl;
	// 
	bool m_fUseCatAll;
	// 
	CString m_strText;
	// 
	CString m_strTextRet;

	// 
	CString m_strDispTemp;

	// 
	CImageList m_il;

	// 
	DropHere m_drop2sel, m_drop2all;

	// 
	void Commit();
	// 
	void AddItems(int mask = -1);

public:
	// 
	void OnDropItems(CString str, bool fCopy, int iInsertAt, DropHere &dropTo);

public:
	// 
	CEditVMSItemsPg(CEditVLower &vctx, CWnd* pParent = NULL)
		: CEditVPg(vctx, pParent)
		, CAnchorSupport(this)
		, m_drop2sel(m_wndSel, *this, _T("sel"))
		, m_drop2all(m_wndAll, *this, _T("all"))
	{

	}
	// 
	virtual void Construction();
	// 
	virtual UINT GetVal()
	{
		return 0;
	}
	// 
	virtual LPCTSTR GetText() { return m_strTextRet = (m_strText + "<00>"); }
	// 
	virtual LPCTSTR GetTemplateName() { return MAKEINTRESOURCE(IDD); }
	// 
	void SelectCat(bool fUseCatA)
	{
		ASSERT(!IsWindow(m_hWnd));

		m_fUseCatAll = fUseCatA;
	}

	// 
	class SourceMe : public COleDataSource {
	public:
		// 
		void SetText(LPCSTR psz) {
			CSharedFile f(GMEM_MOVEABLE | GMEM_SHARE);
			f.Write(psz, strlen(psz) +1);
			CacheGlobalData(CF_DSPTEXT, f.Detach());
		}
	};

// ダイアログ データ
	//{{AFX_DATA(CEditVMSItemsPg)
	enum { IDD = IDD_EDITV_MSITEMS };
	CListCtrl	m_wndSel;
	CListCtrl	m_wndAll;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditVMSItemsPg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEditVMSItemsPg)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnGetdispinfoListSel(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnBeginDragListSel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBeginDragListAll(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnKeyDownListSel(NMHDR *pNMHDR, LRESULT *pResult);
};

class CEditVMSItemsAPg : public CEditVMSItemsPg, public CWordValCmd_Unsigned, public CEditVxxx
{
public:
	// 
	DECL_WVCT_SPEC(wvctMSItemsA)

	// 
	CEditVMSItemsAPg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVMSItemsPg(vctx, pParent)
	{
		SelectCat(true);
	}

	// 
	virtual CEditVPg &GetEditValPg() { return *this; }
	// 
	virtual CWordValCmd &GetOp() { return *this; }

};

class CEditVMSItemsCPg : public CEditVMSItemsPg, public CWordValCmd_Unsigned, public CEditVxxx
{
public:
	// 
	DECL_WVCT_SPEC(wvctMSItemsC)

	// 
	CEditVMSItemsCPg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVMSItemsPg(vctx, pParent)
	{
		SelectCat(false);
	}

	// 
	virtual CEditVPg &GetEditValPg() { return *this; }
	// 
	virtual CWordValCmd &GetOp() { return *this; }

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDITVMSITEMSPG_H__2B701687_9DA7_4D59_9F6B_F4F6186555CB__INCLUDED_)
