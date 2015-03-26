#if !defined(AFX_ANCHORDLG_H__9F959C12_9B72_4AFD_A82C_692A14300619__INCLUDED_)
#define AFX_ANCHORDLG_H__9F959C12_9B72_4AFD_A82C_692A14300619__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnchorDlg.h : ヘッダー ファイル
//

#define acrLeft		1
#define acrTop		2
#define acrRight	4
#define acrBottom	8

#define MAX_ANCHORINF 64U

class CAnchorInf
{
public:
	// 
	WORD nID;
	// 
	BYTE acr;
	// 
	int vl2r, vt2b, vr2l, vb2t;
};

class CAnchorSupport
{
public:
	// 
	CAnchorSupport(CWnd *pWndMe): pWndMe(pWndMe), nInf(0) { }
	// 
	virtual ~CAnchorSupport() { }

	// 
	void CaptureLayout(BYTE acrDefault = 0);
	// 
	void SetAnchor(UINT nIDCtrl, BYTE acr);
	// 
	void Resized();
	// 
	void Moved();

protected:
	// 
	CAnchorInf aInf[MAX_ANCHORINF];
	// 
	UINT nInf;
	// 
	CRect rcParentThen;
	// 
	CWnd *pWndMe;

	// 
	void GetScreenClientRect(PRECT prc)
	{
		pWndMe->GetClientRect(prc);
		pWndMe->ClientToScreen(prc);
	}

};

/////////////////////////////////////////////////////////////////////////////
// CAnchorDlg ウィンドウ

#pragma warning(push)
#pragma warning(disable: 4355)

class CAnchorDlg : public CDialog, CAnchorSupport
{
public:
	// 
	CAnchorDlg(LPCTSTR pszTemplate, CWnd *pParentWnd = NULL): CDialog(pszTemplate, pParentWnd)
		, CAnchorSupport(this)
	{
		nInf = 0;
	}
	// 
	CAnchorDlg(UINT nIDTemplate, CWnd *pParentWnd = NULL): CDialog(nIDTemplate, pParentWnd)
		, CAnchorSupport(this)
	{
		nInf = 0;
	}
	// 
	virtual ~CAnchorDlg()
	{

	}

	// 
	void CaptureLayout(BYTE acrDefault = 0)
	{
		CAnchorSupport::CaptureLayout(acrDefault);
	}
	// 
	void SetAnchor(UINT nIDCtrl, BYTE acr)
	{
		CAnchorSupport::SetAnchor(nIDCtrl, acr);
	}
	// 
	void Resized()
	{
		CAnchorSupport::Resized();
	}
	// 
	void Moved()
	{
		CAnchorSupport::Moved();
	}

	// 
	void GetScreenClientRect(PRECT prc)
	{
		GetClientRect(prc);
		ClientToScreen(prc);
	}

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CAnchorDlg)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CAnchorDlg)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	DECLARE_DYNAMIC(CAnchorDlg)
};

#pragma warning(pop)

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ANCHORDLG_H__9F959C12_9B72_4AFD_A82C_692A14300619__INCLUDED_)
