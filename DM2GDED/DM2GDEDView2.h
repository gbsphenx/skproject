// DM2GDEDView2.h : CDM2GDEDView2 クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM2GDEDVIEW_H__814F6A29_4B54_4500_95DF_FF20B3C264E3__INCLUDED_)
#define AFX_DM2GDEDVIEW_H__814F6A29_4B54_4500_95DF_FF20B3C264E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DGridCtrl.h"
#include "EditPCMCB.h"
#include "HexEditCB.h"

class CEntryItemCell;

class CDM2GDEDView2 : public CView, protected CGDEdCtxCB, protected CEditPCMCB
{
	// 
	struct MemDestIo {
		// 
		BYTE *pbPos, *pbEnd;

		// 
		MemDestIo(BYTE *pbBegin, BYTE *pbEnd): pbPos(pbBegin), pbEnd(pbEnd) { }
		// 
		void WriteByte(UINT x)
		{
			if (pbPos < pbEnd) {
				*pbPos = x;
				pbPos++;
			}
		}
		// 
		void WriteWord(UINT x)
		{
			if (pbPos + 1 < pbEnd) {
				pbPos[0] = LOBYTE(x);
				pbPos[1] = HIBYTE(x);
				pbPos += 2;
			}
		}
		// 
		void WriteDWord(UI32 x)
		{
			if (pbPos + 3 < pbEnd) {
				pbPos[0] = LOBYTE(LOWORD(x));
				pbPos[1] = HIBYTE(LOWORD(x));
				pbPos[2] = LOBYTE(HIWORD(x));
				pbPos[3] = HIBYTE(HIWORD(x));
				pbPos += 4;
			}
		}
	};
	// 
	struct HexEditCtx : CHexEditSink {
		// 
		HexEditCtx(): pCtx(NULL), pos(CGDEntry::ERR) { }
		// 
		HexEditCtx(CGDEdCtx *pCtx, CGDEntry pos): pCtx(pCtx), pos(pos)
		{
			pCtx->AddRef();
		}
		// 
		~HexEditCtx()
		{
			if (pCtx)
				pCtx->Release();
		}

	protected:
		// 
		CGDEntry pos;
		// 
		CGDEdCtx *pCtx;

		// 
		virtual void OnHexEditApply(CHexEditCB *pSender);
	};

	// 
	CGDEdCtx *m_pEdCtx;
	// 
	CDGridCtrl m_wndGrid;
	// 
	SizeBuff m_fvWav;
	// 
	CCellID m_rclkSel;
	// 
	CGDEntry m_lower;
	// 
	SizeBuff m_fvPlaySound;
	// 
	CGDEdOpt m_aEdOpt;
	// 
	HexEditCtx m_aHexEdCtx;

protected:
	// 
	CDM2GDEDView2();
	// 
	virtual void OnGDUpdate(UINT nHint, void *pHint);
	// 
	virtual void OnSelTreeItem(TSelTreeItem &rItem);
	// 
	void InitEdCtx(CGDEdCtx *pEdCtx)
	{
		if (m_pEdCtx)
			m_pEdCtx->RemoveCB(this),
			m_pEdCtx->Release();
		if (m_pEdCtx = pEdCtx)
			m_pEdCtx->GetOpts(m_aEdOpt),
			m_pEdCtx->AddRef(),
			m_pEdCtx->AddCB(this);
	}
	// 
	void UpdateCell(int iRow, CGDAT &core, const CGDEntry *ppos, SIZE &sizeCell, BOOL fUpdate = FALSE);
	// 
	void UpdateAddEntry(const CGDEntry &pos);
	// 
	void SetupCell(int iRow, CGDAT &core, const CGDEntry &pos);
	// 
	bool ChkCtx();
	// 
	CEntryItemCell *GetEntryItemCellAt(int iRow);
	// 
	bool CreateWav(SizeBuff &fvRaw, BOOL fLE);
	// 
	void OnAddCtx(BYTE iCtx);
	// 
	void OnEditCtx(BYTE iCtx, int iRow = -1);
	// 
	void ShowCellMenu(CPoint point, int iCmd);
	// 
	void OnDblClkItem(int iCol, int iRow);
	// 
	void OnRclickItem(int iCol, int iRow, CPoint point);
	// 
	void OnSetOpt();
	// 
	void DoHexEdit(SizeBuff &fv);
	// 
	void RenameEntry(int iRow);
	// 
	void DeleteRow(int iRow);
	// 
	bool IsInSameScheme(const CGDEntry &r1, const CGDEntry &r2)
	{
		return r1.c1() == r2.c1() && r1.c2() == r2.c2();
	}

	// 
	virtual bool CreateRIFFWave(SizeBuff &fv);
	// 
	virtual bool SetPCM11025(BYTE *pbWav, UINT cbWav);

	// 
	DECLARE_DYNCREATE(CDM2GDEDView2)

public:
	// 
	CDM2GDEDDoc* GetDocument();
	// 
	void OnDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	// 
	void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	// 
	void OnGridKeydown(NMHDR* pNMHDR, LRESULT* pResult);

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDM2GDEDView2)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // 構築後の最初の１度だけ呼び出されます。
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CDM2GDEDView2();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDM2GDEDView2)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPopupEditImage();
	afx_msg void OnPopupAddImage();
	afx_msg void OnPopupDeleteEntry();
	afx_msg void OnPopupAddText();
	afx_msg void OnPopupAdd0B();
	afx_msg void OnPopupAdd0C();
	afx_msg void OnPopupEditText();
	afx_msg void OnPopupEdit0B();
	afx_msg void OnPopupEdit0C();
	afx_msg void OnPopupAddPcm();
	afx_msg void OnPopupEditPcm();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnUpdateNextPane(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePrevPane(CCmdUI* pCmdUI);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPopupEditRaw();
	afx_msg void OnPopupAdd04();
	afx_msg void OnPopupAdd07();
	afx_msg void OnPopupAdd09();
	afx_msg void OnPopupRenameEntry();
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DM2GDEDView2.cpp ファイルがデバッグ環境の時使用されます。
inline CDM2GDEDDoc* CDM2GDEDView2::GetDocument()
   { return (CDM2GDEDDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2GDEDVIEW_H__814F6A29_4B54_4500_95DF_FF20B3C264E3__INCLUDED_)
