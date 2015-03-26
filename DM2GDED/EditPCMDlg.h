#if !defined(AFX_EDITPCMDLG_H__78C72E17_5A6F_4503_B93D_D3992C514A61__INCLUDED_)
#define AFX_EDITPCMDLG_H__78C72E17_5A6F_4503_B93D_D3992C514A61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditPCMDlg.h : �w�b�_�[ �t�@�C��
//

#include "SizeVuff.h"
#include "EditPCMCB.h"

/////////////////////////////////////////////////////////////////////////////
// CEditPCMDlg �_�C�A���O

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

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEditPCMDlg)
	enum { IDD = IDD_EDIT_PCM };
	CString	m_strHint;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditPCMDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
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
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDITPCMDLG_H__78C72E17_5A6F_4503_B93D_D3992C514A61__INCLUDED_)
