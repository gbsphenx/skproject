#if !defined(AFX_EDITSTANDBYDLG_H__E51D7BFB_F8F2_4A3F_8B9C_7CAFA1D07992__INCLUDED_)
#define AFX_EDITSTANDBYDLG_H__E51D7BFB_F8F2_4A3F_8B9C_7CAFA1D07992__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditStandbyDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CEditStandbyDlg ダイアログ

class CEditStandbyDlg : public CDialog
{
// コンストラクション
public:
	CEditStandbyDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CEditStandbyDlg)
	enum { IDD = IDD_WAIT_EDIT };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditStandbyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEditStandbyDlg)
	afx_msg void OnClose();
	afx_msg void OnRetry();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDITSTANDBYDLG_H__E51D7BFB_F8F2_4A3F_8B9C_7CAFA1D07992__INCLUDED_)
