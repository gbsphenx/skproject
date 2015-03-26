#if !defined(AFX_EXTCOMBOBOX_H__F9BC3DCF_3090_41D6_B308_9D3386CB6B41__INCLUDED_)
#define AFX_EXTCOMBOBOX_H__F9BC3DCF_3090_41D6_B308_9D3386CB6B41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtComboBox.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CExtComboBox ウィンドウ

class CExtComboBox : public CComboBox
{
// コンストラクション
public:
	// 
	struct DATA {
		// 
		CString text;
		// 
		CString suffix;
		// 
		LPARAM lp;

		// 
		bool operator <(const DATA &r) const;
	};

	// 
	CFont fontMe;
	// 
	TEXTMETRIC tm;

	// 
	DATA ai[256];

	// 
	CExtComboBox();
	// 
	void Sort();
	// 
	void Reset();

	// 
	LPARAM GetIntData(int x)
	{
		return ai[x & 255].lp;
	}
	// 
	void SetIntData(int x, LPARAM lp)
	{
		ai[x & 255].lp = lp;
	}
	// 
	int FindIntData(LPARAM lp);
	// 
	void SetCount(int cx);

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CExtComboBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CExtComboBox();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CExtComboBox)
	afx_msg int OnCharToItem(UINT nChar, CListBox* pListBox, UINT nIndex);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EXTCOMBOBOX_H__F9BC3DCF_3090_41D6_B308_9D3386CB6B41__INCLUDED_)
