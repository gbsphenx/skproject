// ActuHintWebFrm.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "exp00069.h"
#include "ActuHintWebFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActuHintWebFrm

CActuHintWebFrm::CActuHintWebFrm()
{
}

CActuHintWebFrm::~CActuHintWebFrm()
{
}

BEGIN_MESSAGE_MAP(CActuHintWebFrm, CFrameWnd)
	//{{AFX_MSG_MAP(CActuHintWebFrm)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActuHintWebFrm ���b�Z�[�W �n���h��

void CActuHintWebFrm::PostNcDestroy() 
{

}

BOOL CActuHintWebFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if (m_vw.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0,0,0,0), this, AFX_IDW_PANE_FIRST, NULL))
		return true;
	return false;
}

BOOL CActuHintWebFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return false;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return true;
}
