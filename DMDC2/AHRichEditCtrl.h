#if !defined(AFX_AHRICHEDITCTRL_H__2C624961_038A_4A6E_B4E8_E318602C0B11__INCLUDED_)
#define AFX_AHRICHEDITCTRL_H__2C624961_038A_4A6E_B4E8_E318602C0B11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AHRichEditCtrl.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CAHRichEditCtrl ウィンドウ

struct CAH_Entry {
	CHARRANGE chrg;
	int type;
};

extern const UINT PM_CLICK_AHMT;

class CAHRichEditCtrl : public CRichEditCtrl
{
	// 
	int cEnts;
	// 
	CAH_Entry ents[1000];

	// 
	void mark(LPCTSTR pszWord, int type);

public:
	// 
	CAHRichEditCtrl();
	// 
	bool LoadFromFile(LPCTSTR pszFile, bool isRTF = true);
	// 
	void Mark2Words();

	// 
	static DWORD CALLBACK _fn_LoadFromFile(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAHRichEditCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CAHRichEditCtrl();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CAHRichEditCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	void OnSelChange(SELCHANGE *pNMHDR, LRESULT *pRes);

	void OnSelChange(NMHDR *pNMHDR, LRESULT *pRes) {
		OnSelChange(reinterpret_cast<SELCHANGE *>(pNMHDR), pRes);
	}

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_AHRICHEDITCTRL_H__2C624961_038A_4A6E_B4E8_E318602C0B11__INCLUDED_)
