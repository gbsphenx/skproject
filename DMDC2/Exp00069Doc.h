// Exp00069Doc.h : CExp00069Doc クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXP00069DOC_H__9382887F_2711_4017_9D5B_2088C2308CF0__INCLUDED_)
#define AFX_EXP00069DOC_H__9382887F_2711_4017_9D5B_2088C2308CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DDAT.h"
#include "DMEdCtx.h"
#include "DDAT2.h"
#include "PM.h"

#define IMAE_FIRST	1U
#define IMAE_LAST	2U
#define IMAE_BEFORE	3U
#define IMAE_AFTER	4U
#define IMAE_FREE	5U
#define IMAE_SUBFIRST	6U
#define IMAE_SUBLAST	7U

#define IMAF_RECHAIN	1U
#define IMAF_WARNUI	2U
#define IMAF_UPDATEUI	4U
#define IMAF_RECHAINFIRST	8U

using namespace std;

struct SelectLvXY_t
{
	// 
	AtlasPos_t pos;
};

struct ChangeLevelDesc_t
{
	// 
	UINT iMapAt;
};

struct RepaintAtlas_t
{
	// 
	UINT iMapAt;
};

struct DeleteMap_t
{
	// 
	UINT iMapAt;
};

struct ChangeDBRec_t
{
	// 
	DBIPos_t pos;
	// 
	UINT nSRf;
};

struct InsertMap_t
{
	// 
	UINT iMapAt;
};

struct InsertDBRec_t
{
	// 
	DBIPos_t pos;
};

struct DeleteDBRec_t
{
	// 
	DBIPos_t pos;
};

class CExp00069Doc : public CDocument, public CDMEdCtxCB
{
	// 
	CDMEdCtx *m_pEdCtx;
	// 
	CDMEdCtxAccessibility m_conf;
	// 
	BOOL m_fLE;
	// 
	BOOL m_fCheckSum;
	// 
	BOOL m_fCheckDesc;
	// 
	bool m_fDM2_EXTENDED_MAP;

	// 
	friend CArchive &operator >>(CArchive &ar, CExp00069Doc &r);
	// 
	friend CArchive &operator <<(CArchive &ar, CExp00069Doc &r);

	// 
	virtual void OnDMUpdate(UINT nHint, void *pHint);

public:
	// 
	CDDAT &GetDDAT()
	{
		return *m_pEdCtx->GetDDIf();
	}
	// 
	CDMEdCtx *GetEdCtx(BOOL fAddRef)
	{
		if (m_pEdCtx && fAddRef)
			m_pEdCtx->AddRef();

		return m_pEdCtx;
	}
	// 
	virtual BOOL IsModified();

	// 
	bool SettleLoad(CMemFile &f);
	bool SettleSave(CMemFile &f);

protected: // シリアライズ機能のみから作成します。
	CExp00069Doc();
	DECLARE_DYNCREATE(CExp00069Doc)

// アトリビュート
public:

// オペレーション
public:

//オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CExp00069Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CExp00069Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CExp00069Doc)
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnAppDev();
	afx_msg void OnPopupLimitless();
	afx_msg void OnUpdatePopupLimitless(CCmdUI* pCmdUI);
	afx_msg void OnEditWorldShift();
	afx_msg void OnDebugMove2actu2();
	//}}AFX_MSG
	afx_msg BOOL OnOptxxx(UINT nID);
	afx_msg void OnUpdateOptxxx(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EXP00069DOC_H__9382887F_2711_4017_9D5B_2088C2308CF0__INCLUDED_)
