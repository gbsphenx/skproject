#if !defined(AFX_CUSTRICHEDITCTRL_H__147BDB37_E0DA_43D1_8805_7B1350DFE1A8__INCLUDED_)
#define AFX_CUSTRICHEDITCTRL_H__147BDB37_E0DA_43D1_8805_7B1350DFE1A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustRichEditCtrl.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CCustRichEditCtrl �E�B���h�E

class CCustRichEditCtrl : public CRichEditCtrl
{
// �R���X�g���N�V����
public:
	CCustRichEditCtrl();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CCustRichEditCtrl)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CCustRichEditCtrl();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CCustRichEditCtrl)
	//}}AFX_MSG
	void OnLink(NMHDR *pNMHDR, LRESULT *pRet);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_CUSTRICHEDITCTRL_H__147BDB37_E0DA_43D1_8805_7B1350DFE1A8__INCLUDED_)
