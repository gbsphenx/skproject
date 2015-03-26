#if !defined(AFX_DM2ORNATE1COMBOBOX_H__67A20897_21F6_4903_A6A0_8CED8A98F82C__INCLUDED_)
#define AFX_DM2ORNATE1COMBOBOX_H__67A20897_21F6_4903_A6A0_8CED8A98F82C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2Ornate1ComboBox.h : ヘッダー ファイル
//

#include "DM2OrnateIdxMig.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2Ornate1ComboBox ウィンドウ


class CDM2Ornate1ComboBox : public CComboBox
{
	// 
	CUIntArray m_v;
	// 
	CDM2OrnateIdxArray m_oia;
	// 
	bool m_fHasNoop;
	// 
	bool m_fHasAddNew1;
	// 
	UINT m_nMaxOrnate;
	// 
	bool m_fAll;
	// 
	int m_t;
	// 
	int m_cy;
	// 
	int m_cyText;
	// 
	bool m_fAddText;

public:
	// 
	CDM2Ornate1ComboBox();
	// 
	void CtlInit();
	// 
	void Reset();
	// 
	void Revert(CDM2OrnateIdxArray &oia, bool fRevert);
	// 
	void Freshen();

	// 
	void Sel(int t);
	// 
	void SelOrnate(bool fWall) { Sel(fWall ? 0x09 : 0x0A); }
	// 
	void SelDoor() { Sel(0x0E); }
	// 
	void SelDoorOrnate() { Sel(0x0B); }
	// 
	void SelCreature() { Sel(0x0F); }
	// 
	void All(int t);
	// 
	void AllDoor() { All(0x0E), m_fHasNoop = true; }
	// 
	bool IsAddNew1(int iSel);
	// 
	int GetOrnateIdx(int iSel);

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDM2Ornate1ComboBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDM2Ornate1ComboBox)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2ORNATE1COMBOBOX_H__67A20897_21F6_4903_A6A0_8CED8A98F82C__INCLUDED_)
