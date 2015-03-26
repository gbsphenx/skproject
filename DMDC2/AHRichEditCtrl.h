#if !defined(AFX_AHRICHEDITCTRL_H__2C624961_038A_4A6E_B4E8_E318602C0B11__INCLUDED_)
#define AFX_AHRICHEDITCTRL_H__2C624961_038A_4A6E_B4E8_E318602C0B11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AHRichEditCtrl.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CAHRichEditCtrl �E�B���h�E

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

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CAHRichEditCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CAHRichEditCtrl();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
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
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_AHRICHEDITCTRL_H__2C624961_038A_4A6E_B4E8_E318602C0B11__INCLUDED_)
