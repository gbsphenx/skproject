// EdPIActuHintPage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EdPIActuHintPage.h"
#include "OSP.h"
#include "PWM.h"

using namespace OSP;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdPIActuHintPage ダイアログ

IMPLEMENT_DYNCREATE(CEdPIActuHintPage, CEdPIDlg)

CEdPIActuHintPage::CEdPIActuHintPage(): CEdPIDlg(CEdPIActuHintPage::IDD)
{
	//{{AFX_DATA_INIT(CEdPIActuHintPage)
	m_iSel = -1;
	//}}AFX_DATA_INIT
}

void CEdPIActuHintPage::DoDataExchange(CDataExchange* pDX)
{
	CEdPIDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdPIActuHintPage)
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_wndButton);
	DDX_Control(pDX, IDC_STATIC_WCAREA, m_wndArea);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_wndSel);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_iSel);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEdPIActuHintPage, CEdPIDlg)
	//{{AFX_MSG_MAP(CEdPIActuHintPage)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelchangeComboType)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_COMMAND(ID_CONTEXT_EDIT_FP, OnContextEditFp)
	ON_COMMAND(ID_CONTEXT_EDIT_NOTEPAD, OnContextEditNotepad)
	ON_BN_CLICKED(IDC_BUTTON_SAMPLER, OnButtonSampler)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(PM_CLICK_AHMT, OnClickAHMT)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdPIActuHintPage メッセージ ハンドラ

bool CEdPIActuHintPage::RevertRecord(DMDBU &rec, bool fRevert)
{
	if (fRevert) {
		m_iActuType = (rec.r3.w2 & 127);
	}
	return true;
}

void CEdPIActuHintPage::AfterRevert()
{
	if (m_iDisplayActuType != (BYTE)m_iActuType) loadHintRtf();
}

BOOL CEdPIActuHintPage::OnInitDialog() 
{
	CEdPIDlg::OnInitDialog();
	
	m_lockWarp = false;
	m_iDisplayActuType = -1;

	int iTileBase = (TileIsWall() ? 128 : 0);
	for (int i = 0; i < 128; i++) {
		m_wndSel.AddString(CDM2NameDic::GetInstance().GetName(3, i |iTileBase));
	}
	m_wndSel.SetCurSel(m_iActuType);
	m_wndSel.SetFocus();

	CRect rc;
	m_wndArea.GetWindowRect(&rc);
	ScreenToClient(&rc);
	VERIFY(m_webfrm.Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_GROUP, rc, this, NULL, 0, NULL));
	m_webfrm.m_vw.SetOwner(this);
	m_webfrm.SetDlgCtrlID(0x0100);
	
	return FALSE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

static CString CompleteFP(CString f)
{
	CString fp1 = OSP_JoinPath(OSP_GetDir(OSP_GetModuleFileName(NULL)), f);
	if (GetFileAttributes(fp1) != (DWORD)-1)
		return fp1;
	return f;
}

void CEdPIActuHintPage::loadHintRtf()
{
	BYTE iActuType = m_iActuType; 
	if (iActuType == 255) return;

	m_wndSel.SetCurSel(iActuType);

	CString strPath = GetApp()->GetWorkDir();
	CString str1; str1.Format(TileIsWall() ? "Hint_Actu\\%02XW.htm" : "Hint_Actu\\%02XF.htm", 0U +iActuType); str1 = CompleteFP(str1);
	CString str2; str2.Format("Hint_Actu\\%02X.htm", 0U +iActuType); str2 = CompleteFP(str2);
	CString strfnf; strfnf = "Hint_Actu\\fnf.htm"; strfnf = CompleteFP(strfnf);

	CActuHintWebVw &vw = m_webfrm.m_vw;

	if (!::IsWindow(vw))
		return;

	m_iDisplayActuType = iActuType;

	if (false);
	else if (GetFileAttributes(str1) != 0xFFFFFFFF) {
		vw.Navigate2((str1));
	}
	else if (GetFileAttributes(str2) != 0xFFFFFFFF) {
		vw.Navigate2((str2));
	}
	else if (GetFileAttributes(strfnf) != 0xFFFFFFFF) {
		vw.Navigate2((strfnf));
	}
	else {

	}
}

void CEdPIActuHintPage::OnSelchangeComboType() 
{
	m_iActuType = m_wndSel.GetCurSel();

	m_lockWarp = true;
	loadHintRtf();
	m_lockWarp = false;
}

void CEdPIActuHintPage::OnCancel() 
{
	GetParent()->SendMessage(WM_COMMAND, IDCANCEL);
}

LPARAM CEdPIActuHintPage::OnClickAHMT(WPARAM wp, LPARAM lp)
{
	if (!m_lockWarp) {
		if (wp != 0) LockedSendNotifyToParent(wp);
	}
	return 0;
}

void CEdPIActuHintPage::OnButtonEdit() 
{
	CMenu m;
	if (!m.LoadMenu(IDR_ACTUHINT)) return;
	CMenu *p = m.GetSubMenu(0);
	if (p == NULL) return;

	CRect rc;
	m_wndButton.GetWindowRect(&rc);

	p->TrackPopupMenu(TPM_LEFTBUTTON, rc.left, rc.top, this);
}

static CString ExtractFilePathFromUrl(CString strUri) {
	USES_CONVERSION;
	WCHAR wc[260];
	DWORD cch = 0;
	HRESULT hr = CoInternetParseUrl(T2CW(strUri), PARSE_PATH_FROM_URL, 0, wc, 260, &cch, 0);
	if (FAILED(hr))
		return "";
	return wc;
}

void CEdPIActuHintPage::OnContextEditFp() 
{
	CString strFP = ExtractFilePathFromUrl(m_webfrm.m_vw.GetLocationURL());
	if (strFP.IsEmpty()) { AfxMessageBox(IDS_ERR_URL_OF_INTERNET); return; }

	ShellExecute(*this, "EDIT", strFP, NULL, NULL, SW_SHOW);
}

void CEdPIActuHintPage::OnContextEditNotepad() 
{
	CString strFP = ExtractFilePathFromUrl(m_webfrm.m_vw.GetLocationURL());
	if (strFP.IsEmpty()) { AfxMessageBox(IDS_ERR_URL_OF_INTERNET); return; }

	ShellExecute(*this, "open", "NOTEPAD.exe", strFP, NULL, SW_SHOW);
}

void CEdPIActuHintPage::OnButtonSampler() 
{
	if (AfxMessageBox(IDS_WARN_COLLECT_SAMPLES, MB_OKCANCEL) != IDOK)
		return;

	CWnd *p = GetOwner();
	if (p != NULL) {
		p->SendMessage(PWM_COLLECT_SAMPLER, m_iActuType | (TileIsWall() ? 0x0100 : 0));
	}
}
