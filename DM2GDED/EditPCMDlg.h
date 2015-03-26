#if !defined(AFX_EDITPCMDLG_H__78C72E17_5A6F_4503_B93D_D3992C514A61__INCLUDED_)
#define AFX_EDITPCMDLG_H__78C72E17_5A6F_4503_B93D_D3992C514A61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditPCMDlg.h : ヘッダー ファイル
//

#include "SizeVuff.h"
#include "EditPCMCB.h"

/////////////////////////////////////////////////////////////////////////////
// CEditPCMDlg ダイアログ

class CEditPCMDlg : public CDialog
{
	// 
	class MemSrcIoLE
	{
		// 
		BYTE *pbBegin, *pbPos, *pbEnd;

	public:
		// 
		MemSrcIoLE(BYTE *pbBegin, BYTE *pbEnd): pbBegin(pbBegin), pbPos(pbBegin), pbEnd(pbEnd)
		{

		}
		// 
		int ReadWord()
		{
			int r = -1;
			if (pbPos + 1 < pbEnd) {
				r = *(WORD *)pbPos;
				pbPos += 2;
			}
			return r;
		}
		// 
		int ReadDWord()
		{
			int r = -1;
			if (pbPos + 3 < pbEnd) {
				r = *(DWORD *)pbPos;
				pbPos += 4;
			}
			return r;
		}
		// 
		int SeekForward(int n)
		{
			if (n < 0) {
				if (pbPos + n < pbBegin)
					n = pbBegin -pbPos;
			} else {
				if (pbEnd < pbPos + n)
					n = pbEnd -pbPos;
			}
			pbPos += n;
			return pbPos - pbBegin;
		}
		// 
		UINT GetPosition() const { return pbPos - pbBegin; }

	};

public:
	// 
	CEditPCMDlg(CWnd* pParent = NULL);
	// 
	void Init(CEditPCMCB *pCB)
	{
		m_pCB = pCB;
	}

protected:
	// 
	CEditPCMCB *m_pCB;
	// 
	SizeBuff m_fvPlaySound;

	// 
	BOOL SavePCMTo(HGLOBAL &hMem);
	// 
	void RevertStatus();
	// 
	bool LoadRIFFWav(BYTE *pbPos, BYTE *pbEnd);

// ダイアログ データ
	//{{AFX_DATA(CEditPCMDlg)
	enum { IDD = IDD_EDIT_PCM };
	CString	m_strHint;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditPCMDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEditPCMDlg)
	afx_msg void OnButtonSndPlay();
	afx_msg void OnButtonSndCopy();
	afx_msg void OnButtonSndPaste();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonSndFromFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDITPCMDLG_H__78C72E17_5A6F_4503_B93D_D3992C514A61__INCLUDED_)
