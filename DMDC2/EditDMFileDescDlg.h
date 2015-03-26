#if !defined(AFX_EDITDMFILEDESCDLG_H__F4785292_4B61_4252_8687_45BE52A40CA9__INCLUDED_)
#define AFX_EDITDMFILEDESCDLG_H__F4785292_4B61_4252_8687_45BE52A40CA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditDMFileDescDlg.h : ヘッダー ファイル
//

#include "DDAT.h"
#include "HexEditCB.h"

/////////////////////////////////////////////////////////////////////////////
// CEditDMFileDescDlg ダイアログ

class CEditDMFileDescDlg : public CDialog, CHexEditCB
{
	// 
	void ShowHex(UINT nID, const CByteArray &arr);
	// 
	void TryHexEdit(CWnd *pTar, LPCTSTR lpszName, UINT nID, const CByteArray &arr);
	// 
	void OnApplyHexEditCB(DWORD nCookie);

public:
	// 
	void Revert(const AllMapDesc_t &d);
	// 
	void Commit(AllMapDesc_t &d);

// コンストラクション
public:
	CEditDMFileDescDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CEditDMFileDescDlg)
	enum { IDD = IDD_EDITDMFILEDESC };
	UINT	m_nRandomGraphicsSeed;
	UINT	m_nMinObjListSize;
	UINT	m_nSPPx;
	UINT	m_nSPPy;
	UINT	m_nMaps;
	int		m_nPlayerDir;
	//}}AFX_DATA
	CByteArray m_arrTextData;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditDMFileDescDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEditDMFileDescDlg)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDITDMFILEDESCDLG_H__F4785292_4B61_4252_8687_45BE52A40CA9__INCLUDED_)
