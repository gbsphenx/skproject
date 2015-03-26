#if !defined(AFX_DMTELELOCFRM_H__35FDB0A3_43B0_4066_B966_47CFAF5E64D3__INCLUDED_)
#define AFX_DMTELELOCFRM_H__35FDB0A3_43B0_4066_B966_47CFAF5E64D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMTeleLocFrm.h : ヘッダー ファイル
//

#include "DMTeleLocVw.h"
#include "DDAT.h"

/////////////////////////////////////////////////////////////////////////////
// CDMTeleLocFrm フレーム

class CDMTeleLocFrm : public CFrameWnd
{
	// 
	CPtrArray m_arrayWndz;
	// 
	int m_iModalStateStack;

public:
	// 
	CDMTeleLocVw m_view;

	// 
	CDMTeleLocFrm(CDDAT *pDDIf, AtlasPos_t ap, bool fMovable): m_view(pDDIf, ap, fMovable)
	{
		m_iModalStateStack = 0;
	}
	// 
	int DoModal(CWnd *pParentWnd = NULL);
	// 
	void EnterModalState();
	// 
	void LeaveModalState();

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDMTeleLocFrm)
	protected:
	virtual void PostNcDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDMTeleLocFrm)
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DMTELELOCFRM_H__35FDB0A3_43B0_4066_B966_47CFAF5E64D3__INCLUDED_)
