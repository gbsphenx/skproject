#if !defined(AFX_EDITHEROSTATDLG_H__FD524B30_ECB5_4428_973F_4B2B902FFB14__INCLUDED_)
#define AFX_EDITHEROSTATDLG_H__FD524B30_ECB5_4428_973F_4B2B902FFB14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditHeroStatDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CEditHeroStatDlg ダイアログ

class CEditHeroStatDlg : public CDialog
{
	// 
	void revert();
	void commit();

	void setVal(UINT nID, UINT val, UINT minVal, UINT maxVal, UINT step);
	void getValW(UINT nID, void *pw, UINT , UINT , UINT );

public:
	// 
	BYTE m_bData[52];
	bool m_fLE;

public:
	CEditHeroStatDlg(bool fLE, CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CEditHeroStatDlg)
	enum { IDD = IDD_EDIT_HEROSTAT };
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditHeroStatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEditHeroStatDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonYukman();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDITHEROSTATDLG_H__FD524B30_ECB5_4428_973F_4B2B902FFB14__INCLUDED_)
