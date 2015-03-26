#if !defined(AFX_EDITIMAGEFRAGSDLG_H__115490A2_A3CD_4A0F_B28F_C2D61556BFA7__INCLUDED_)
#define AFX_EDITIMAGEFRAGSDLG_H__115490A2_A3CD_4A0F_B28F_C2D61556BFA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditImageFragsDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CEditImageFragsDlg ダイアログ

class CEditImageFragsDlg : public CDialog
{
public:
	// 
	CEditImageFragsDlg(UINT iW, UINT iH, CWnd* pParent = NULL);   // 標準のコンストラクタ
	// 
	UINT GetWidthFrags()
	{
		return 0
			|(m_fW02 ? 0x0200 : 0x0000)
			|(m_fW04 ? 0x0400 : 0x0000)
			|(m_fW08 ? 0x0800 : 0x0000)
			|(m_fW10 ? 0x1000 : 0x0000)
			|(m_fW20 ? 0x2000 : 0x0000)
			|(m_fW40 ? 0x4000 : 0x0000)
			|(m_fW80 ? 0x8000 : 0x0000)
			;
	}
	// 
	UINT GetHeightFrags()
	{
		return 0
			|(m_fH02 ? 0x0200 : 0x0000)
			|(m_fH04 ? 0x0400 : 0x0000)
			|(m_fH08 ? 0x0800 : 0x0000)
			|(m_fH10 ? 0x1000 : 0x0000)
			|(m_fH20 ? 0x2000 : 0x0000)
			|(m_fH40 ? 0x4000 : 0x0000)
			|(m_fH80 ? 0x8000 : 0x0000)
			;
	}

// ダイアログ データ
	//{{AFX_DATA(CEditImageFragsDlg)
	enum { IDD = IDD_IMAGE_FLAGS };
	BOOL	m_fH02;
	BOOL	m_fH04;
	BOOL	m_fH08;
	BOOL	m_fH10;
	BOOL	m_fH20;
	BOOL	m_fH40;
	BOOL	m_fH80;
	BOOL	m_fW02;
	BOOL	m_fW04;
	BOOL	m_fW08;
	BOOL	m_fW10;
	BOOL	m_fW20;
	BOOL	m_fW40;
	BOOL	m_fW80;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditImageFragsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEditImageFragsDlg)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDITIMAGEFRAGSDLG_H__115490A2_A3CD_4A0F_B28F_C2D61556BFA7__INCLUDED_)
