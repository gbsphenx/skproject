#if !defined(AFX_DM2GDEDTEVIEW_H__AC51CABA_5EF3_4F22_BCB0_3E3F5C518E54__INCLUDED_)
#define AFX_DM2GDEDTEVIEW_H__AC51CABA_5EF3_4F22_BCB0_3E3F5C518E54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2GDEDTeView.h : ヘッダー ファイル
//

#include "DM2GDEDView.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDTeView ビュー

class CDM2GDEDTeView : public CDM2GDEDView
{
	// 
	DWORD m_nCookie;
	// 
	CGDEdCtx *m_pEdCtx;

	// 
	DECLARE_DYNCREATE(CDM2GDEDTeView)

public:
	//{{AFX_VIRTUAL(CDM2GDEDTeView)
	public:
	virtual void OnFinalRelease();
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

protected:
	// 
	CDM2GDEDTeView();

	// 
	bool ConnectAx(bool fReconnect = true);

	// 
	BSTR theGetMesText(LONG iEC);
	// 
	void theSetMesText(LONG iEC, LONG iIdx, LPCTSTR pszText);
	// 
	BOOL theIsLE();
	// 
	VARIANT theEnumerate();

	// 
	bool MkGDEdCtx()
	{
		return (m_pEdCtx = CDM2GDEDView::MkGDEdCtx()) != NULL;
	}

protected:
	virtual ~CDM2GDEDTeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//{{AFX_MSG(CDM2GDEDTeView)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//{{AFX_DISPATCH(CDM2GDEDTeView)
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_DM2GDEDTEVIEW_H__AC51CABA_5EF3_4F22_BCB0_3E3F5C518E54__INCLUDED_)
