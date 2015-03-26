#if !defined(AFX_EDTILEDLG_H__62768D9A_EBE8_4E48_8580_95D2E593CAE0__INCLUDED_)
#define AFX_EDTILEDLG_H__62768D9A_EBE8_4E48_8580_95D2E593CAE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdTileDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CEdTileDlg ウィンドウ

class CEdTileDlg : public CDialog, public COnIdle
{
public:
	// 
	BYTE m_cTile;
	// 
	BYTE m_cTileNew;
	// 
	BOOL m_fSiteUpdating;
	// 
	BOOL m_fSiteLow;
	// 
	BOOL m_fSiteHook;

	// 
	void Construction()
	{

	}
	// 
	CEdTileDlg(LPCTSTR psz, CWnd *p): CDialog(psz, p)
	{
		Construction();
	}
	// 
	CEdTileDlg(UINT n, CWnd *p): CDialog(n, p)
	{
		Construction();
	}
	// 
	virtual BOOL Create(BYTE cTile, CWnd *pParentWnd)
	{
		m_cTile = m_cTileNew = cTile;

		return CDialog::Create(m_lpszTemplateName, pParentWnd);
	}
	// 
	void Modify(BYTE m, BYTE v)
	{
		m_cTile &=~m;
		m_cTile ^= v;
	}
	// 
	void Set(BYTE m, BOOL f)
	{
		Modify(m, f ? m : 0);
	}
	// 
	void Cnt(
		int i,
		BOOL f1, BOOL f2, BOOL f3, BOOL f4, BOOL f5, BOOL f6, BOOL f7, BOOL f8
	);
	// 
	virtual void OnOK();
	// 
	virtual void OnCancel();
	// 
	virtual BOOL OnIdleNotify();
	// 
	void SendNotifyToParent(UINT nCode)
	{
		CWnd *p;
		if (p = GetParent()) {
			p->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), nCode), (LPARAM)(HWND)*this);
		}
	}

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CEdTileDlg)
	public:
	//}}AFX_VIRTUAL

// インプリメンテーション
public:

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CEdTileDlg)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDTILEDLG_H__62768D9A_EBE8_4E48_8580_95D2E593CAE0__INCLUDED_)
