#if !defined(AFX_DM2EDPICVIEW_H__D029FE87_5151_4FFA_8F0B_EF0DAD10B5EC__INCLUDED_)
#define AFX_DM2EDPICVIEW_H__D029FE87_5151_4FFA_8F0B_EF0DAD10B5EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2EdPicView.h : ヘッダー ファイル
//

#include "DM2RPtr.h"
#include "GDEdCtx.h"
#include "DM2SIIVCtx.h"
#include "DM2VwPicVwPal.h"
#include "DM2PIPDescList.h"
#include "RCT2.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2EdPicView ビュー

class CDM2EdPicView : public CScrollView
{
	// 
	CDM2SPtr<CGDEdCtx> m_pEdCtx;
	// 
	CDM2SPtr<CDM2SIIVCtx> m_pSIIVCtx;
	// 
	int m_nDM2SIIVT;
	// 
	int m_nScene;
	// 
	DMGHLiIMGM m_imgs[3];
	// 
	bool m_fEditing;
	// 
	CRCT2 m_rct;
	// 
	bool m_fTransparent;
	// 
	bool m_fBestMatch;
	// 
	bool m_fMkAltPal;
	// 
	bool m_fUse8bpp;
	// 
	int m_iKeyIdx;
	// 
	list<CGDEntry> m_listNext, m_listBack;
	// 
	CRect m_rcPic;
	// 
	CRect m_rcPicIn;
	// 
	CSize m_sizeImage;
	// 
	int m_iStage1;
	// 
	int m_iStage2;
	// 
	bool m_fConv4bpp;
	// 
	bool m_fTweakTp;
	// 
	int m_iKeyFrom, m_iKeyTo;
	// 
	int m_nTarScene;
	// 
	CDM2PIPTS m_ts, m_tsOrg;
	// 
	CGDEntry m_posColorPal;
	// 
	bool m_fPalConvMaster;
	// 
	bool m_fOverrideSceneKey;
	// 
	int m_fx;
	// 
	int m_fy;

	// 
	int m_nInit;

	// 
	void OnResize();
	// 
	void OnReloadImage();
	// 
	void OnFreshImage();
	// 
	void OnFreshScene();
	// 
	void OnRedrawImage();

	// 
	CGDEntry GetCurPos()
	{
		if (m_listNext.empty()) return CGDEntry::ERR;
		return m_listNext.front();
	}
	// 
	bool SelectSR(int iDir, bool fTest = false);
	// 
	void SetNewColorKeyAt(CPoint pt);
	// 
	DMGHLiIMGM &GetFinalImg()
	{
		return m_imgs[m_iStage2];
	}
	// 
	bool IsOrgImage8bpp()
	{
		return m_imgs[0].fIs8bpp;
	}
	// 
	bool IsPasted()
	{
		return true;
	}
	// 
	bool IsSceneSelable()
	{
		return m_fEditing && m_fBestMatch && (m_ts == piptsScene);
	}
	// 
	CDM2VwPicVwPal &GetVwPal();
	// 
	bool SaveNow();
	// 
	bool GetModified()
	{
		return false;
	}
	// 
	void UpdateEntry(CGDEntry pos);

public:
	// 
	CDM2EdPicView(CGDEdCtx *pEdCtx);
	// 
	void Select(CGDEntry pos, BOOL fAddHist = TRUE);
	// 
	bool DoSave(bool fQuery);
	// 
	DECLARE_DYNAMIC(CDM2EdPicView)

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDM2EdPicView)
	protected:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドしました。
	virtual void OnInitialUpdate();     // 構築後、最初の処理。
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CDM2EdPicView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDM2EdPicView)
	afx_msg void OnFileCloseView();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste4bpp();
	afx_msg void OnUpdateEditPaste4bpp(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste8bpp();
	afx_msg void OnUpdateEditPaste8bpp(CCmdUI* pCmdUI);
	afx_msg void OnFileModePreview();
	afx_msg void OnUpdateFileModePreview(CCmdUI* pCmdUI);
	afx_msg void OnFileModeEdit();
	afx_msg void OnUpdateFileModeEdit(CCmdUI* pCmdUI);
	afx_msg void OnMoveBack();
	afx_msg void OnUpdateMoveBack(CCmdUI* pCmdUI);
	afx_msg void OnMoveFront();
	afx_msg void OnUpdateMoveFront(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnFileSavePic();
	afx_msg void OnUpdateFileSavePic(CCmdUI* pCmdUI);
	afx_msg void OnEditImageFlgs();
	afx_msg void OnUpdateEditImageFlgs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnSceneNo(UINT nID);
	afx_msg void OnUpdateSceneNo(CCmdUI* pCmdUI);
	afx_msg void OnViewEnableTransparent(UINT nID);
	afx_msg void OnUpdateViewEnableTransparent(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2EDPICVIEW_H__D029FE87_5151_4FFA_8F0B_EF0DAD10B5EC__INCLUDED_)
