// DM2NIBPage.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2NIBPage.h"
#include "DM2NIPS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM2NIBPage

IMPLEMENT_DYNAMIC(CDM2NIBPage, CPropertyPage)

BEGIN_MESSAGE_MAP(CDM2NIBPage, CPropertyPage)
	//{{AFX_MSG_MAP(CDM2NIBPage)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2NIBPage ���b�Z�[�W �n���h��

bool CDM2NIBPage::MkCtx()
{
	CDM2NIPS *p = DYNAMIC_DOWNCAST(CDM2NIPS, GetParent());
	if (p) m_ctx = p->GetCtx();
	return m_ctx != NULL;
}

bool CDM2NIBPage::MkPSH()
{
	return (m_ppsh = static_cast<CDM2NIPS *>(GetParentOwner())) != NULL;
}

bool CDM2NIBPage::MkEmbeddedCtl()
{
	if (!MkCtx()) return false;

	if (m_frame.m_hWnd == NULL) {
		CRect rc;
		::GetClientRect(::GetDlgItem(*this, IDC_STATIC_FRAME), &rc);

		if (m_frame.Create(m_ctx, this, 0 |WS_CHILD |WS_VISIBLE, rc, 0x0100)) {
			m_frame.m_view.SetOwner(this);
			return true;
		}
		return false;
	}
	return true;
}

BOOL CDM2NIBPage::OnSetActive() 
{
	VERIFY(MkEmbeddedCtl());

	if (MkPSH()) {
		int i = m_ppsh->GetPageIndex(this);
		int n = m_ppsh->GetPageCount();

		DWORD nBack = (i != 0) ? PSWIZB_BACK : 0;
		if (i + 1 == n) m_ppsh->SetWizardButtons(0 |nBack |PSWIZB_FINISH);
		else m_ppsh->SetWizardButtons(0 |nBack |PSWIZB_NEXT);
	}
	
	return CPropertyPage::OnSetActive();
}
