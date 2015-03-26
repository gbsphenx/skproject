// EditDM2FileDescDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EditDM2FileDescDlg.h"
#include "DMTeleLocFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditDM2FileDescDlg ダイアログ


CEditDM2FileDescDlg::CEditDM2FileDescDlg(CDDAT *pDDIf, CWnd* pParent /*=NULL*/)
	: CDialog(CEditDM2FileDescDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditDM2FileDescDlg)
	m_iDir = -1;
	m_iSeed = 0;
	m_iX = 0;
	m_iY = 0;
	m_nCntMap = 0;
	//}}AFX_DATA_INIT

	m_pDDIf = pDDIf;
}


void CEditDM2FileDescDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditDM2FileDescDlg)
	DDX_Control(pDX, IDC_LIST_TEXT, m_wndList);
	DDX_Control(pDX, IDC_SPIN_CNT_MAP, m_wndSpin);
	DDX_CBIndex(pDX, IDC_COMBO_DIR, m_iDir);
	DDX_Text(pDX, IDC_EDIT_SEED, m_iSeed);
	DDV_MinMaxUInt(pDX, m_iSeed, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_X, m_iX);
	DDV_MinMaxUInt(pDX, m_iX, 0, 31);
	DDX_Text(pDX, IDC_EDIT_Y, m_iY);
	DDV_MinMaxUInt(pDX, m_iY, 0, 31);
	DDX_Text(pDX, IDC_EDIT_CNT_MAPS, m_nCntMap);
	DDV_MinMaxUInt(pDX, m_nCntMap, 0, 64);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditDM2FileDescDlg, CDialog)
	//{{AFX_MSG_MAP(CEditDM2FileDescDlg)
	ON_BN_CLICKED(IDC_BUTTON_REF, OnButtonRef)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditDM2FileDescDlg メッセージ ハンドラ

void CEditDM2FileDescDlg::OnButtonRef() 
{
	if (!UpdateData()) return;

	CDMTeleLocFrm frm(m_pDDIf, AtlasPos_t(0, m_iX, m_iY), false);
	if (IDOK == frm.DoModal(this)) {
		AtlasPos_t ap = frm.m_view.GetAP();
		m_iX = ap.X();
		m_iY = ap.Y();
		UpdateData(FALSE);
	}
}

BOOL CEditDM2FileDescDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_wndSpin.SetRange(0, 64);

	LOGFONT lfNice;
	ReadFontTemplateFrom(lfNice, IDS_FONTTEMP_TEXTINPUT, ";10;;;;;;;;1;;;;1");
	m_fontNice.DeleteObject();
	m_fontNice.CreateFontIndirect(&lfNice);

	m_wndList.SetFont(&m_fontNice);

	m_wndList.SetExtendedStyle(m_wndList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	m_wndList.InsertColumn(0, "Addr", LVCFMT_LEFT, 100);
	m_wndList.InsertColumn(1, "Text", LVCFMT_LEFT, 100);

	const int cx = m_texte.vec.GetSize();

	CString text, temp;
	int posoff = 0;
	char esc = 0;
	char cap = 0;

	for (int x=0; x<cx; ) {
		BYTE c = m_texte.vec[x];

		if (cap == 0) {
			if (esc == 0) {
				if (c <= 25) {
					text += char('A' + c);
				}
				else if (c == 26) text += ' ';
				else if (c == 27) text += '.';
				else if (c == 28) text += '/';
				else if (c == 29) esc = 1;
				else if (c == 30) esc = 2;
				else if (c == 31) cap = 1;
			}
			else if (esc == 1) {
				if (c <= 23) text += "?";
				else if (c <= 31) text += char('0' + c - 24);

				esc = 0;
			}
			else {
				if (c == 0) text += "AND ";
				else if (c == 1) text += "!";
				else if (c == 2) text += "THE ";
				else if (c == 3) text += "YOU ";
				else text += "?";

				esc = 0;
			}
		}

		x++;

		if (((x % 3) == 0 && cap != 0) || (x + 1 == cx)) {
			temp.Format("%04lX", posoff / 3);
			int r = m_wndList.InsertItem(m_wndList.GetItemCount(), temp);
			m_wndList.SetItemText(r, 1, text);

			text.Empty();
			posoff = x;
			cap = 0;
		}
	}

	m_wndList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_wndList.SetColumnWidth(1, LVSCW_AUTOSIZE);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
