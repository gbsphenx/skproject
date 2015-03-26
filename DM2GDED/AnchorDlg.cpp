// AnchorDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "AnchorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnchorSupport

void CAnchorSupport::CaptureLayout(BYTE acrDefault)
{
	GetScreenClientRect(&rcParentThen);

	CWnd *pWnd = pWndMe->GetWindow(GW_CHILD);
	nInf = 0;
	for (; pWnd != NULL && nInf < MAX_ANCHORINF; pWnd = pWnd->GetNextWindow()) {
		int id = pWnd->GetDlgCtrlID();
		if (id >= 65535U) continue;

		CAnchorInf &rInf = aInf[nInf];
		CRect rc;
		pWnd->GetWindowRect(&rc);
		rInf.nID = id;
		rInf.acr = acrDefault;
		rInf.vl2r = rc.left - rcParentThen.left;
		rInf.vt2b = rc.top - rcParentThen.top;
		rInf.vr2l = rcParentThen.right - rc.right;
		rInf.vb2t = rcParentThen.bottom - rc.bottom;

		nInf++;
	}
}

void CAnchorSupport::SetAnchor(UINT nIDCtrl, BYTE acr)
{
	for (UINT x = 0; x < nInf; x++) {
		if (aInf[x].nID == nIDCtrl) {
			aInf[x].acr = acr;
			break;
		}
	}
}

void CAnchorSupport::Resized()
{
	CRect rcNew;
	GetScreenClientRect(&rcNew);

	const int tol = rcParentThen.left - rcNew.left;
	const int tor = rcNew.right - rcParentThen.right;
	const int tot = rcParentThen.top - rcNew.top;
	const int tob = rcNew.bottom - rcParentThen.bottom;

	if (tol != 0 || tor != 0 || tot != 0 || tob != 0) {
		CRect rcIn;
		pWndMe->GetClientRect(&rcIn);

		HDWP hDWP = BeginDeferWindowPos(nInf);

		for (UINT x = 0; x < nInf; x++) {
			CAnchorInf &rInf = aInf[x];
			BYTE acr = rInf.acr;

			CRect rc;

			bool L = (acr & acrLeft  ) ? true : false;
			bool T = (acr & acrTop   ) ? true : false;
			bool R = (acr & acrRight ) ? true : false;
			bool B = (acr & acrBottom) ? true : false;

			rInf.vl2r += (L && R) ? (0) : (0
				+((L) ? (0) : (tol))
				+((R) ? (tor) : (0))
				);
			rInf.vr2l += (L && R) ? (0) : (0
				+((L) ? (tor) : (0))
				+((R) ? (0) : (tol))
				);
			rInf.vt2b += (T && B) ? (0) : (0
				+((T) ? (0) : (tot))
				+((B) ? (tob) : (0))
				);
			rInf.vb2t += (T && B) ? (0) : (0
				+((T) ? (tob) : (0))
				+((B) ? (0) : (tot))
				);

			rc.left = rInf.vl2r;
			rc.top = rInf.vt2b;
			rc.right = rcIn.right - rInf.vr2l;
			rc.bottom = rcIn.bottom - rInf.vb2t;

			rc.left = min(rc.left, rc.right);
			rc.right = max(rc.left, rc.right);
			rc.top = min(rc.top, rc.bottom);
			rc.bottom = max(rc.top, rc.bottom);

			DeferWindowPos(hDWP, pWndMe->GetDlgItem(rInf.nID)->GetSafeHwnd(), NULL, 
				rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOOWNERZORDER | SWP_NOZORDER
				);
		}

		EndDeferWindowPos(hDWP);
	}

	rcParentThen = rcNew;
}

void CAnchorSupport::Moved()
{
	Resized();
}

/////////////////////////////////////////////////////////////////////////////
// CAnchorDlg

IMPLEMENT_DYNAMIC(CAnchorDlg, CDialog)

BEGIN_MESSAGE_MAP(CAnchorDlg, CDialog)
	//{{AFX_MSG_MAP(CAnchorDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnchorDlg メッセージ ハンドラ
