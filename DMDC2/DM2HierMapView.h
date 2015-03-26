#if !defined(AFX_DM2HIERMAPVIEW_H__4E4E8D2D_14D0_4D16_9A97_EF1F55ED9784__INCLUDED_)
#define AFX_DM2HIERMAPVIEW_H__4E4E8D2D_14D0_4D16_9A97_EF1F55ED9784__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2HierMapView.h : ヘッダー ファイル
//

#include "ExtBM.h"
#include "DMEdCtx.h"
#include "RCT2.h"
#include "MainFrmIndir.h"
#include "TriViewNe.h"
#include "DMDBRMask.h"
#include "EdTileDoorDlg.h"
#include "EdTileFalseDlg.h"
#include "EdTilePitDlg.h"
#include "EdTileStairsDlg.h"
#include "EdTileTeleDlg.h"
#include "EdTileDefaultDlg.h"
#include "DMTarTrack.h"
#include "OffSc.h"
#include "ShowcaseData.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2HierTileEd

class CDM2HierTileEd
{
	// 
	CEdTileDlg *m_pSite;

public:
	// 
	CEdTileDoorDlg m_wndDoor;
	// 
	CEdTileFalseDlg m_wndFalse;
	// 
	CEdTilePitDlg m_wndPit;
	// 
	CEdTileStairsDlg m_wndStairs;
	// 
	CEdTileTeleDlg m_wndTele;
	// 
	CEdTileDefaultDlg m_wndDefault;

	// 
	CDM2HierTileEd()
	{
		m_pSite = NULL;
	}
	// 
	void Destroy()
	{
		m_pSite = NULL;

		m_wndDoor.DestroyWindow();
		m_wndFalse.DestroyWindow();
		m_wndPit.DestroyWindow();
		m_wndStairs.DestroyWindow();
		m_wndTele.DestroyWindow();
		m_wndDefault.DestroyWindow();
	}
	// 
	BOOL DoSiteModal(BYTE cTile, CRect rectMin, CRect rectMax, CWnd *pParentWnd = NULL);
	// 
	BYTE GetTileNewVal();
};

/////////////////////////////////////////////////////////////////////////////
// CDM2HierMapView ビュー

class CDM2HierMapView : public CScrollView, CDMEdCtxCB, public CInterViewInfluential
{
	// 
	struct TileSt {
		// 
		BYTE nTile;
		// 
		BYTE nZ;
		// 
		BYTE iMap;
		// 
		BYTE iZone;
		// 
		DWORD iT4w;

		// 
		TileSt(): nTile(0xFF), nZ(0xFF), iMap(0xFF), iT4w(0), iZone(0xFF)
		{

		}
	};
	// 
	struct LevelSt {
		// 
		CRect rc;
		// 
		BYTE nZ;

		// 
		bool PtInRect(int x, int y)
		{
			return (rc.left <= x && x < rc.right && rc.top <= y && y < rc.bottom);
		}
		// 
		CPoint Global2Local(CPoint pt)
		{
			return CPoint(pt.x - rc.left, pt.y - rc.top);
		}
	};
	// 
	typedef CArray<LevelSt, LevelSt> LevelStList;
	// 
	struct BTHL {
		// 
		CPoint pt;

		// 
		BTHL()
		{
			pt.x = pt.y = -1;
		}
		// 
		bool HasAny() const
		{
			return pt.x != -1 || pt.y != -1;
		}
	};
	// 
	struct BTileSt {
		// 
		char iMap;

		// 
		BTileSt()
		{
			iMap = -1;
		}
	};
	// 
	struct TileInSel {
		// 
		int lv, x, y;
	};

	// 
	CDMEdCtx *m_pEdCtx;
	// 
	int m_iLv;
	// 
	TileSt m_aTS[287][287];
	// 
	CRCT2 m_rct;
	// 
	LevelStList m_listLS;
	// 
	int m_iLevelSt;
	// 
	CPoint m_ptCurSel;
	// 
	CByteArray m_listLSIdx;
	// 
	int m_iLSIdx;
	// 
	CMainFrameInfluential *m_pMainFrmIf;
	// 
	CDDAT *m_pDDIf;
	// 
	bool m_fNoTriViewMessage;
	// 
	bool m_fBuildTS;
	// 
	CDM2HierTileEd m_tileEd;
	// 
	AtlasPos_t m_apSite;
	// 
	bool m_fDispTileVal;
	// 
	CDMTarTrack m_tarTrack;
	// 
	BTHL m_bthl;
	// 
	CDMTarTrackLev m_ttlv;

	// 
	bool m_fEraseTileRec;
	// 
	UINT m_iEditLvTool;

	// 
	COffScreen m_offsc;
	// 
	WORD m_cVram1[85 +1][85 +1];
	// 
	DWORD m_cVram2[85 +1][85 +1];

	// 
	COleDropTarget m_dropTar;
	// 
	CPoint m_ptDnDcur;
	// 
	TileInSel m_tileInSel;

	// 
	bool m_fSpcMove;
	HCURSOR m_hMoveCursor;
	CPoint m_ptMoveFirstCursor;
	CPoint m_ptMoveFirstPos;

	bool m_fSpcAltMove;

	bool m_fSnapTL;

	// 
	void InitEdCtx(CDMEdCtx *pEdCtx)
	{
		if (m_pEdCtx)
			m_pEdCtx->RemoveCB(this),
			m_pEdCtx->Release();
		if (m_pEdCtx = pEdCtx)
			m_pEdCtx->AddRef(),
			m_pEdCtx->AddCB(this);
	}
	// 
	void BuildTS();
	// 
	bool SelectLevelStAt(CPoint point);
	// 
	bool SelectLevel(UINT iMap, bool fMoveToCenter = false);
	// 
	void SetPntTile(CPoint point, bool fNoTileSel = false);
	// 
	void UpdatePntTile(CPoint point);
	// 
	void UpdatePntTile(AtlasPos_t ap);
	// 
	inline bool IsHidMasked(BYTE c)
	{
		if (m_iLevelSt < 0) return false;
		return m_iLevelSt != c;
	}
	// 
	bool MkMainFrmIf()
	{
		if (m_pMainFrmIf = CMainFrameInfluential::Cast(GetParentFrame())) {
			m_fEraseTileRec = (m_pMainFrmIf->GetEditToolF() & 1) ? true : false;
			m_iEditLvTool = (m_pMainFrmIf->GetEditLvTool());
			return true;
		}
		return false;
	}
	// 
	bool MkDDIf()
	{
		if (m_pEdCtx != NULL && (m_pDDIf = m_pEdCtx->GetDDIf()) != NULL) {
			return true;
		}
		return false;
	}
	// 
	bool CalcAPFromClient(CPoint pt, AtlasPos_t &ap);
	// 
	DWORD CalcT4w(RN rn, BYTE c, CDMDBRMask &mask);
	// 
	BOOL ScrollTileXY(int dx, int dy);
	// 
	void MarkBuildTS();
	// 
	BOOL RemoveTileSpec(AtlasBit_t &ab);
	// 
	CDDAT &GetDDIf() { return *m_pEdCtx->GetDDIf(); }
	// 
	BOOL AddTileSpec(AtlasBit_t &ab);
	// 
	void DrawArrow(CDC *pDC, CPoint p1, CPoint p2, int iArrow, LPCTSTR pszSrcText, LPCTSTR pszDestText);
	// 
	void BTTile(AtlasPos_t ap);
	// 
	bool Deselect();
	// 
	void OnReview();
	// 
	bool VwMap_DoorSw(RN rn);
	// 
	bool VwMap_TeleF(RN rn);
	// 
	bool VwMap_ScanOrnate(RN rn, const BYTE iArray[15], BYTE iOrnate[4], BYTE iCreature[4], bool fWall);
	// 
	void Blit7x7(CDC &dcTo, int tx, int ty, CDC &dcFrom, int sx, int sy, CDC &dcMask, int mx, int my, int f);

protected:
	// 
	CDM2HierMapView();
	// 
	void OnDMUpdate(UINT nHint, void *pHint);
	// 
	void OnTriViewMessage(UINT nHint, void *pHint);

	// 
	BOOL OnReloadOrnates(UINT nID);

	// 
	TileInSel *IsTileInSel(POINT pt);
	// 
	bool AddShowcaseEntityTo(AtlasPos_t pos, ShowcaseData sc);

	// 
	DECLARE_DYNCREATE(CDM2HierMapView)

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDM2HierMapView)
	public:
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	protected:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドしました。
	virtual void OnInitialUpdate();     // 構築後、最初の処理。
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CDM2HierMapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDM2HierMapView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnViewDisplayTileVal();
	afx_msg void OnUpdateViewDisplayTileVal(CCmdUI* pCmdUI);
	afx_msg void OnEditDmFileDesc();
	afx_msg void OnEditLvDesc();
	afx_msg void OnUpdateEditLvDesc(CCmdUI* pCmdUI);
	afx_msg void OnEditAddMap();
	afx_msg void OnEditOptimize();
	afx_msg void OnEditMkTopvw();
	afx_msg void OnEditOptimizeFlat();
	afx_msg void OnOptSettle();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnViewSnapToTL();
	afx_msg void OnUpdateViewSnapToTL(CCmdUI* pCmdUI);
	afx_msg void OnCommand0100();
	afx_msg void OnViewTlv0(UINT nID);
	afx_msg void OnUpdateViewTlv0(CCmdUI* pCmdUI);
	afx_msg void OnEditRotyN(UINT nID);
	afx_msg void OnUpdateEditRotyN(CCmdUI* pCmdUI);
	afx_msg void OnEditBuildlabyrinth();
	afx_msg void OnOptAssets(UINT nID);
	afx_msg void OnUpdateOptAssets(CCmdUI *p);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2HIERMAPVIEW_H__4E4E8D2D_14D0_4D16_9A97_EF1F55ED9784__INCLUDED_)
