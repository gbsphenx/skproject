#if !defined(AFX_EDITIMGFLGSDLG_H__DAEC97EB_59D0_4718_BF19_D42E6DC46DF3__INCLUDED_)
#define AFX_EDITIMGFLGSDLG_H__DAEC97EB_59D0_4718_BF19_D42E6DC46DF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditImgFlgsDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CEditImgFlgsDlg ダイアログ

class CEditImgFlgsDlg : public CDialog
{
	// 
	int m_nMin, m_nMax;

public:
	// 
	bool m_f4bpp;

	// 
	CEditImgFlgsDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	// 
	int GetX()
	{
		return m_nX;
	}
	// 
	int GetY()
	{
		return m_nY;
	}

// ダイアログ データ
	//{{AFX_DATA(CEditImgFlgsDlg)
	enum { IDD = IDD_EDIT_IMG_FLG };
	CEdit	m_wndPosY;
	CEdit	m_wndPosX;
	CSpinButtonCtrl	m_wndSpinY;
	CSpinButtonCtrl	m_wndSpinX;
	int		m_nX;
	int		m_nY;
	BOOL	m_fUse0c;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditImgFlgsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEditImgFlgsDlg)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDITIMGFLGSDLG_H__DAEC97EB_59D0_4718_BF19_D42E6DC46DF3__INCLUDED_)
