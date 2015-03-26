#if !defined(AFX_DM2WORLDSHIFTDLG_H__A772733A_C1A3_46D2_8F07_7EEC968F6906__INCLUDED_)
#define AFX_DM2WORLDSHIFTDLG_H__A772733A_C1A3_46D2_8F07_7EEC968F6906__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2WorldShiftDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDM2WorldShiftDlg ダイアログ

class CDM2WorldShiftDlg : public CDialog
{
// コンストラクション
public:
	CDM2WorldShiftDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDM2WorldShiftDlg)
	enum { IDD = IDD_WORLD_SHIFT };
	CSpinButtonCtrl	m_wndY;
	CSpinButtonCtrl	m_wndX;
	int		m_nX;
	int		m_nY;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDM2WorldShiftDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDM2WorldShiftDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2WORLDSHIFTDLG_H__A772733A_C1A3_46D2_8F07_7EEC968F6906__INCLUDED_)
