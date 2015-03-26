#if !defined(AFX_EDITIMAGEFRAGSDLG_H__115490A2_A3CD_4A0F_B28F_C2D61556BFA7__INCLUDED_)
#define AFX_EDITIMAGEFRAGSDLG_H__115490A2_A3CD_4A0F_B28F_C2D61556BFA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditImageFragsDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CEditImageFragsDlg �_�C�A���O

class CEditImageFragsDlg : public CDialog
{
public:
	// 
	CEditImageFragsDlg(UINT iW, UINT iH, CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
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

// �_�C�A���O �f�[�^
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


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditImageFragsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEditImageFragsDlg)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ����܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDITIMAGEFRAGSDLG_H__115490A2_A3CD_4A0F_B28F_C2D61556BFA7__INCLUDED_)
