#if !defined(AFX_ANCHORDLG_H__9F959C12_9B72_4AFD_A82C_692A14300619__INCLUDED_)
#define AFX_ANCHORDLG_H__9F959C12_9B72_4AFD_A82C_692A14300619__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnchorDlg.h : �w�b�_�[ �t�@�C��
//

#define acrLeft		1
#define acrTop		2
#define acrRight	4
#define acrBottom	8

#define MAX_ANCHORINF 64U

class CAnchorInf
{
public:
	// 
	WORD nID;
	// 
	BYTE acr;
	// 
	int vl2r, vt2b, vr2l, vb2t;
};

class CAnchorSupport
{
public:
	// 
	CAnchorSupport(CWnd *pWndMe): pWndMe(pWndMe), nInf(0) { }
	// 
	virtual ~CAnchorSupport() { }

	// 
	void CaptureLayout(BYTE acrDefault = 0);
	// 
	void SetAnchor(UINT nIDCtrl, BYTE acr);
	// 
	void Resized();
	// 
	void Moved();

protected:
	// 
	CAnchorInf aInf[MAX_ANCHORINF];
	// 
	UINT nInf;
	// 
	CRect rcParentThen;
	// 
	CWnd *pWndMe;

	// 
	void GetScreenClientRect(PRECT prc)
	{
		pWndMe->GetClientRect(prc);
		pWndMe->ClientToScreen(prc);
	}

};

/////////////////////////////////////////////////////////////////////////////
// CAnchorDlg �E�B���h�E

#pragma warning(push)
#pragma warning(disable: 4355)

class CAnchorDlg : public CDialog, CAnchorSupport
{
public:
	// 
	CAnchorDlg(LPCTSTR pszTemplate, CWnd *pParentWnd = NULL): CDialog(pszTemplate, pParentWnd)
		, CAnchorSupport(this)
	{
		nInf = 0;
	}
	// 
	CAnchorDlg(UINT nIDTemplate, CWnd *pParentWnd = NULL): CDialog(nIDTemplate, pParentWnd)
		, CAnchorSupport(this)
	{
		nInf = 0;
	}
	// 
	virtual ~CAnchorDlg()
	{

	}

	// 
	void CaptureLayout(BYTE acrDefault = 0)
	{
		CAnchorSupport::CaptureLayout(acrDefault);
	}
	// 
	void SetAnchor(UINT nIDCtrl, BYTE acr)
	{
		CAnchorSupport::SetAnchor(nIDCtrl, acr);
	}
	// 
	void Resized()
	{
		CAnchorSupport::Resized();
	}
	// 
	void Moved()
	{
		CAnchorSupport::Moved();
	}

	// 
	void GetScreenClientRect(PRECT prc)
	{
		GetClientRect(prc);
		ClientToScreen(prc);
	}

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CAnchorDlg)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CAnchorDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	DECLARE_DYNAMIC(CAnchorDlg)
};

#pragma warning(pop)

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_ANCHORDLG_H__9F959C12_9B72_4AFD_A82C_692A14300619__INCLUDED_)
