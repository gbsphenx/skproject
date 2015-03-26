// EditDMFileDescDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EditDMFileDescDlg.h"
#include "NamedWndBox.h"
#include "HexEditFrame.h"
#include "HexEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditDMFileDescDlg ダイアログ


CEditDMFileDescDlg::CEditDMFileDescDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditDMFileDescDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditDMFileDescDlg)
	m_nRandomGraphicsSeed = 0;
	m_nMinObjListSize = 0;
	m_nSPPx = 0;
	m_nSPPy = 0;
	m_nMaps = 0;
	m_nPlayerDir = -1;
	//}}AFX_DATA_INIT
}


void CEditDMFileDescDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditDMFileDescDlg)
	DDX_Text(pDX, IDC_EDIT_RGS, m_nRandomGraphicsSeed);
	DDV_MinMaxUInt(pDX, m_nRandomGraphicsSeed, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_MOLS, m_nMinObjListSize);
	DDV_MinMaxUInt(pDX, m_nMinObjListSize, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_SPP_X, m_nSPPx);
	DDV_MinMaxUInt(pDX, m_nSPPx, 0, 255);
	DDX_Text(pDX, IDC_EDIT_SPP_Y, m_nSPPy);
	DDV_MinMaxUInt(pDX, m_nSPPy, 0, 255);
	DDX_Text(pDX, IDC_EDIT_TOTALMAPCNT, m_nMaps);
	DDV_MinMaxUInt(pDX, m_nMaps, 0, 255);
	DDX_CBIndex(pDX, IDC_COMBO_PLAYERDIR, m_nPlayerDir);
	//}}AFX_DATA_MAP
	if (!pDX->m_bSaveAndValidate) {
		ShowHex(IDC_EDIT_TEXT_DATA, m_arrTextData);
	}
}

BEGIN_MESSAGE_MAP(CEditDMFileDescDlg, CDialog)
	//{{AFX_MSG_MAP(CEditDMFileDescDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditDMFileDescDlg メッセージ ハンドラ

void CEditDMFileDescDlg::ShowHex(UINT nID, const CByteArray &arr)
{
	TCHAR tcText[51];
	tcText[0] = 0;
	LPTSTR lpszTo = tcText;
	const BYTE *pbPos = arr.GetData();
	const BYTE *pbEnd = pbPos + arr.GetSize();
	static char tbl[] = {"0123456789ABCDEF"};
	int i = 0, n = 16;
	if (n != 0) {
		while (pbPos != pbEnd && n != 0) {
			lpszTo[0] = tbl[(*pbPos) >> 4];
			lpszTo[1] = tbl[(*pbPos) & 15];
			lpszTo[2] = ' ';
			lpszTo += 3; pbPos++;
			i++; n--;
		}
		if (pbPos != pbEnd) {
			lpszTo[-1] = lpszTo[0] = lpszTo[1] = '.';
			lpszTo += 3;
		}
		lpszTo[-1] = 0;
	}
	SetDlgItemText(nID, tcText);
}

void CEditDMFileDescDlg::Revert(const AllMapDesc_t &d)
{
	m_nMinObjListSize = d.nMinObjectListSize;
	m_nSPPx = d.nStartX;
	m_nSPPy = d.nStartY;
	m_nPlayerDir = d.nStartDir;
	m_nRandomGraphicsSeed = d.nRandomGraphicsSeed;

	m_arrTextData.SetSize(d.arrTextData.GetSize() * 2);
	memcpy(m_arrTextData.GetData(), d.arrTextData.GetData(), m_arrTextData.GetSize());
}

void CEditDMFileDescDlg::Commit(AllMapDesc_t &d)
{
	d.nMinObjectListSize = m_nMinObjListSize;
	d.nStartX = m_nSPPx;
	d.nStartY = m_nSPPy;
	d.nStartDir = m_nPlayerDir;
	d.nRandomGraphicsSeed = m_nRandomGraphicsSeed;

	d.arrTextData.SetSize(m_arrTextData.GetSize() / 2);
	memcpy(d.arrTextData.GetData(), m_arrTextData.GetData(), d.arrTextData.GetSize() * 2);
}

BOOL CEditDMFileDescDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_F2) {
			CWnd *pActive = GetFocus();
			switch (pActive->GetDlgCtrlID()) {
			case IDC_EDIT_TEXT_DATA:
				TryHexEdit(pActive, "TextData", 0x0100, m_arrTextData);
				break;
			}
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CEditDMFileDescDlg::TryHexEdit(CWnd *pTar, LPCTSTR lpszName, UINT nID, const CByteArray &arr)
{
	CWnd *pExist;
	if (pExist = g_boxWnd.FindNameFromBox("LVDESC", lpszName)) {
		pExist->SetActiveWindow();
		pExist->ShowWindow(SW_RESTORE);
		return;
	}
	CHexEditFrame *pFrame;
	if (!(pFrame = CHexEditFrame::Create1(this)))
		return;
	CHexEditView *pView;
	if (pView = dynamic_cast<CHexEditView *>(pFrame->GetActiveView())) {
		pView->SetBinaryData(arr, this, nID);
	}
	g_boxWnd.Add2Box(pFrame, this, "LVDESC", lpszName);
}

void CEditDMFileDescDlg::OnApplyHexEditCB(DWORD nCookie)
{
	if (!UpdateData())
		return;
	LPCTSTR lpszName = NULL;
	CByteArray *parr = NULL;
	switch (nCookie) {
	case 0x0100:
		lpszName = "TextData";
		parr = &m_arrTextData;
		break;
	}
	CHexEditFrame *pFrame = static_cast<CHexEditFrame *>(g_boxWnd.FindNameFromBox("LVDESC", lpszName));
	if (pFrame) {
		CHexEditView *pView = static_cast<CHexEditView* >(pFrame->GetActiveView());
		if (pView) {
			pView->GetBinaryData(*parr);
			UpdateData(FALSE);
		}
	}
}
