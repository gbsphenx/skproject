#if !defined(AFX_DM2NIPVFRAME_H__3486CC2B_C1F9_40EF_8464_76AD6567B6EB__INCLUDED_)
#define AFX_DM2NIPVFRAME_H__3486CC2B_C1F9_40EF_8464_76AD6567B6EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2NIPvFrame.h : �w�b�_�[ �t�@�C��
//

#include "DM2NIPvView.h"
#include "DM2NICtx.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2NIPvFrame �t���[��

class CDM2NIPvFrame : public CFrameWnd
{
public:
	// 
	CDM2NIPvView m_view;

	// 
	CDM2NIPvFrame();
	// 
	bool Create(CDM2NICtx *pContext, CWnd *pParentWnd, DWORD nStyle, CRect rc, UINT nID)
	{
		if (CFrameWnd::Create(NULL, _T("Pv frame"), nStyle, rc, pParentWnd, NULL, 0, NULL)) {
			if (m_view.Create(pContext, this, 0 |WS_VISIBLE |WS_CHILD, AFX_IDW_PANE_FIRST)) {
				InitialUpdateFrame(NULL, TRUE);
				RecalcLayout();
				return true;
			}
		}
		return false;
	}

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDM2NIPvFrame)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDM2NIPvFrame)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DM2NIPVFRAME_H__3486CC2B_C1F9_40EF_8464_76AD6567B6EB__INCLUDED_)
