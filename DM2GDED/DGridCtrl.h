#if !defined(AFX_DGRIDCTRL_H__38BEA2D4_1854_43DE_B9C6_E2D2D6664229__INCLUDED_)
#define AFX_DGRIDCTRL_H__38BEA2D4_1854_43DE_B9C6_E2D2D6664229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DGridCtrl.h : ヘッダー ファイル
//

#include "GridCtrl.h"
#include "GDEdCtx.h"
#include "EditPCMCB.h"
#include "HexEditCB.h"
#include "WVCT.h"
#include "ExtBM.h"
#include "EditVxxx.h"

#define GVN_KEYDOWN LVN_KEYDOWN

typedef struct tagGV_KEYDOWN {
    NMHDR hdr;
    int   iRow;
    int   iColumn;
    WORD  wVKey;
} GV_KEYDOWN;

class CDCellEntryItem;
class CDCellImage;

/////////////////////////////////////////////////////////////////////////////
// CDGridCtrl ウィンドウ

class CDGridCtrl : public CGridCtrl, protected CGDEdCtxCB, protected CEditPCMCB
{
	// 
	CGDEdCtx *m_pEdCtx;
	// 
	CGDEdOpt m_aEdOpt;
	// 
	SizeBuff m_fvWav, m_fvPlaySound;
	// 
	DECLARE_DYNCREATE(CDGridCtrl);

public:
	// 
	CDGridCtrl();
	// 
	void InitEdCtx(CGDEdCtx *pEdCtx)
	{
		if (m_pEdCtx)
			m_pEdCtx->RemoveCB(this),
			m_pEdCtx->Release();
		if (m_pEdCtx = pEdCtx)
			m_pEdCtx->AddRef(),
			m_pEdCtx->AddCB(this);
	}
	// 
	void CreateList(CGDEntry lower, CGDEntry upper, BOOL fReadOnly = FALSE);
	// 
	void UpdateAllEntryContents();
	// 
	BOOL IsRO() const { return m_fRO; }
	// 
	BOOL IsSndOk() const
	{
		if (m_pEdCtx)
			return m_pEdCtx->GetGDAT().IsSndPCM();
		return FALSE;
	}
	// 
	void ActivateEntry(const CGDEntry &pos, int col);
	// 
	CEditVxxx &GetVxxx(const CGDEntry &pos);
	// 
	CEditVxxx &GetRawVxxx(const CGDEntry &pos);

protected:
	// 
	struct CellProcessing {
		// 
		int nWidth;

		// 
		CellProcessing(): nWidth(0) { }
	} m_ci;

	// 
	CGDEntry m_lower, m_upper;
	// 
	BOOL m_fRO;

	// 
	void FormatEntryContents(const CGDEntry &pos, CString &strVal, const CDM2DescItem *pItem);
	// 
	void FormatEntryName(const CGDEntry &pos, CString &strVal);
	// 
	void FormatEntryType(const CGDEntry &pos, CString &strVal);

	// 
	void UpdateEntryContents(UINT iEntry, const CGDEntry *pposNew = NULL);
	// 
	void UpdateSingleEntryContents(UINT iEntry, const CGDEntry *pposNew = NULL);
	// 
	void ReflectCellProcessing();
	// 
	void MakeEntryContentsEmpty(UINT iEntry);

	// 
	UINT Conv2Row(UINT iEntry) { return 1 + iEntry; }
	// 
	UINT Conv2Entry(UINT iRow) { return iRow - 1; }

	// 
	CDCellEntryItem *GetEntryItem(UINT iEntry, UINT iCol);
	// 
	CDCellEntryItem *GetEntryGenericContents(UINT iEntry);
	// 
	CDCellImage *GetEntryImageContents(UINT iEntry);
	// 
	int GetEntryItemCount();
	// 
	CGDEntry GetEntryItemID(UINT iEntry);
	// 
	void SetEntryItemCount(UINT nEntries);
	// 
	bool IsEntryItemChecked(UINT iEntry);

	// 
	void RenameEntryItem();
	// 
	void DeleteEntryItem();
	// 
	void ExportEntryItem(bool fViaFile, bool fExport, bool fForceRaw = false);

	// 
	virtual void OnGDUpdate(UINT nHint, void *pHint);
	// 
	void OnSetOpt();

	// 
	void OnAddCtx(BYTE c3);
	// 
	void OnEditCtx(BYTE c3);
	// 
	void OnEditCtx2(BYTE c3, bool editRaw = false);

	// 
	void OnAddEntryItem(CGDEntry pos);
	// 
	void OnDeleteEntryItem(CGDEntry pos);
	// 
	void OnUpdateEntryItem(CGDEntry pos);

	// 
	CGDAT &GetGDAT();
	// 
	LRESULT SendKeyDownToParent(int nRow, int nCol, WORD wVKey) const;
	// 
	virtual bool CreateRIFFWave(SizeBuff &fv);
	// 
	virtual bool SetPCM11025(BYTE *pbWav, UINT cbWav);
	// 
	bool CreateWav(SizeBuff &fvRaw, BOOL fLE);
	// 
	void DoHexEdit(SizeBuff &fv);
	// 
	void ResetFocusCell(CCellID ci);
	// 
	bool IsInSameVw(const CGDEntry &r1, const CGDEntry &r2)
	{
		return r1.c1() == r2.c1() && r1.c2() == r2.c2();
	}
	// 
	void OnEditImage();
	// 
	void OnEditImage2();
	// 
	void OnViewImage2();
	// 
	void InvalidateAfterRow(UINT iRow);
	// 
	void OnDblClkCell(int nRow, int nCol);
	// 
	void OnOpenImageExternally(bool fEdit);
	// 
	void OnEditImageInternally(CGDEntry posSrc, CExtBM &bmSrc);
	// 
	void UpdateDynSelEntries();

protected:
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
		// 
		void operator =(const HexEditCtx &s)
		{
			pos = s.pos;

			if (pCtx)
				pCtx->Release();
			if (pCtx = s.pCtx)
				pCtx->AddRef();
		}

	private:
		// 
		HexEditCtx(const HexEditCtx &s);

	protected:
		// 
		CGDEntry pos;
		// 
		CGDEdCtx *pCtx;

		// 
		virtual void OnHexEditApply(CHexEditCB *pSender);
	};

	// 
	HexEditCtx m_aHexEdCtx;

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDGridCtrl)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CDGridCtrl();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDGridCtrl)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextAdd04();
	afx_msg void OnContextAdd07();
	afx_msg void OnContextAdd09();
	afx_msg void OnContextAdd0b();
	afx_msg void OnContextAdd0c();
	afx_msg void OnContextAddImage();
	afx_msg void OnContextAddPcm();
	afx_msg void OnContextAddText();
	afx_msg void OnContextDeleteEntry();
	afx_msg void OnContextRenameEntry();
	afx_msg void OnContextEdit0b();
	afx_msg void OnContextEdit0c();
	afx_msg void OnContextEditImage();
	afx_msg void OnContextEditPcm();
	afx_msg void OnContextEditRaw();
	afx_msg void OnContextEditText();
	afx_msg void OnSysColorChange();
	afx_msg void OnPopupImportRaw();
	afx_msg void OnPopupExportRaw();
	afx_msg void OnPopupExportWav();
	afx_msg void OnPopupImportWav();
	afx_msg void OnPopupCopyPcm();
	afx_msg void OnPopupPastePcm();
	afx_msg void OnPopupPlayEntry();
	afx_msg void OnPopupViewImage();
	afx_msg void OnPopupExportBmp();
	afx_msg void OnPopupImportBmp();
	afx_msg void OnPopupPasteBmp();
	afx_msg void OnPopupCopyBmp();
	afx_msg void OnPopupEditImageIPF();
	afx_msg void OnContextAdd08();
	afx_msg void OnContextAdd0e();
	afx_msg void OnContextEditImageExternal();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnContextEditRawText();
	afx_msg void OnContextEditRaw0B();
	afx_msg void OnContextEditRaw0C();
	afx_msg void OnPopupEditChampStat();
	afx_msg void OnPopupCopySeled();
	afx_msg void OnPopupPasteItems();
	afx_msg void OnPopupMark4x4ToImage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DGRIDCTRL_H__38BEA2D4_1854_43DE_B9C6_E2D2D6664229__INCLUDED_)
