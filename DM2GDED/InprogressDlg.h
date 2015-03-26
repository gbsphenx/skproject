#if !defined(AFX_INPROGRESSDLG_H__A75A1A83_63EA_4C43_BDB7_AC78FA050F95__INCLUDED_)
#define AFX_INPROGRESSDLG_H__A75A1A83_63EA_4C43_BDB7_AC78FA050F95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InprogressDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CInprogressDlg ダイアログ

class CInprogressDlg : public CDialog
{
	// 
	int m_iOldPos;

public:
	// 
	CInprogressDlg(CWnd* pParent = NULL);
	// 
	bool DoModeless(CWnd *pParent)
	{
		if (Create(IDD, pParent)) {
			SetWindowPos(NULL, 0, 0, 0, 0, 0 |SWP_NOMOVE |SWP_NOSIZE |SWP_SHOWWINDOW);
			UpdateWindow();
			return true;
		}
		return false;
	}
	// 
	void SetHintMinMax(int iMin, int iMax)
	{
		if (m_wndHint.m_hWnd) {
			m_wndHint.SetRange32(iMin, iMax);
			m_wndHint.SetPos(m_iOldPos = 0);
		}
		if (m_hWnd) UpdateWindow();
	}
	// 
	void SetHintPos(int iPos)
	{
		if (m_iOldPos != iPos) {
			m_iOldPos = iPos;
			if (m_wndHint.m_hWnd) {
				m_wndHint.SetPos(iPos);
			}
			if (m_hWnd) UpdateWindow();
		}
	}
	// 
	void Close()
	{
		if (m_hWnd) {
			MSG wm;
			while (PeekMessage(&wm, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE)) Sleep(0);
			while (PeekMessage(&wm, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE)) Sleep(0);
		}

		DestroyWindow();
	}

// ダイアログ データ
	//{{AFX_DATA(CInprogressDlg)
	enum { IDD = IDD_INPROGRESS };
	CProgressCtrl	m_wndHint;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CInprogressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CInprogressDlg)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CInprogressSPH
{
	// 
	CInprogressDlg &wndDlg;
	// 
	int iStg, nStg, nStgMax, iPos;

public:
	// 
	CInprogressSPH(CInprogressDlg &wndDlg): wndDlg(wndDlg) { }
	// 
	void Init(int nStg)
	{
		this->iStg = 0;
		this->nStg = nStg;
	}
	// 
	void StepStage()
	{
		iStg++;
		wndDlg.SetHintMinMax(0, nStg);
		wndDlg.SetHintPos(iStg);
	}
	// 
	void SetStageMaxStep(int nMax)
	{
		iPos = nMax * iStg;
		wndDlg.SetHintMinMax(0, nMax * nStg);
		wndDlg.SetHintPos(iPos);
	}
	// 
	void Step()
	{
		iPos++;
		wndDlg.SetHintPos(iPos);
	}
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_INPROGRESSDLG_H__A75A1A83_63EA_4C43_BDB7_AC78FA050F95__INCLUDED_)
