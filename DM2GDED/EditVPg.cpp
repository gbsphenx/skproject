// EditVPg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "dm2gded.h"
#include "EditVPg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditVPg

CEditVPg::CEditVPg(CEditVLower &vctx, CWnd* pParent)
	: CDialog()
	, m_vctx(vctx)
{

}

BEGIN_MESSAGE_MAP(CEditVPg, CDialog)
	//{{AFX_MSG_MAP(CEditVPg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditVPg ���b�Z�[�W �n���h��
