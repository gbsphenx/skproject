#if !defined(AFX_EXTCOMBOBOX_H__F9BC3DCF_3090_41D6_B308_9D3386CB6B41__INCLUDED_)
#define AFX_EXTCOMBOBOX_H__F9BC3DCF_3090_41D6_B308_9D3386CB6B41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtComboBox.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CExtComboBox �E�B���h�E

class CExtComboBox : public CComboBox
{
// �R���X�g���N�V����
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

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CExtComboBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CExtComboBox();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CExtComboBox)
	afx_msg int OnCharToItem(UINT nChar, CListBox* pListBox, UINT nIndex);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EXTCOMBOBOX_H__F9BC3DCF_3090_41D6_B308_9D3386CB6B41__INCLUDED_)
