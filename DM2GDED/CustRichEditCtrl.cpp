// CustRichEditCtrl.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DM2GDED.h"
#include "CustRichEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustRichEditCtrl

CCustRichEditCtrl::CCustRichEditCtrl()
{

}

CCustRichEditCtrl::~CCustRichEditCtrl()
{

}

BEGIN_MESSAGE_MAP(CCustRichEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CCustRichEditCtrl)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(EN_LINK, OnLink)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustRichEditCtrl メッセージ ハンドラ

void CCustRichEditCtrl::OnLink(NMHDR *pNMHDR, LRESULT *pRes)
{
	ENLINK *pHdr = reinterpret_cast<ENLINK *>(pNMHDR);

	if (pHdr->msg == WM_LBUTTONDOWN) {
		CWaitCursor wc;
		SetSel(pHdr->chrg);
		CString strSel = GetSelText();
		ShellExecute(NULL, "open", strSel, NULL, NULL, SW_SHOW);

		*pRes = TRUE;
		return;
	}

	*pRes = FALSE;
}
