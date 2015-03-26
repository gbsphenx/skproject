// EditVFlagsPg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "EditVFlagsPg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditVFlagsPg ダイアログ


void CEditVFlagsPg::Construction()
{
	//{{AFX_DATA_INIT(CEditVFlagsPg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CEditVFlagsPg::DoDataExchange(CDataExchange* pDX)
{
	CEditVPg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditVFlagsPg)
	DDX_Control(pDX, IDC_LIST_FLAGS, m_wndList);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) {
		m_nFlags = 0;
		for (int i = 0; i < 16; i++) {
			if (m_wndList.GetSel(i) > 0)
				m_nFlags |= (1 << i);
		}
	} else {
		for (int i = 0; i < 16; i++)
			m_wndList.SetSel(i, m_nFlags & (1 << i));
	}
}


BEGIN_MESSAGE_MAP(CEditVFlagsPg, CEditVPg)
	//{{AFX_MSG_MAP(CEditVFlagsPg)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditVFlagsPg メッセージ ハンドラ

int CEditVFlagsPg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditVPg::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

BOOL CEditVFlagsPg::OnInitDialog() 
{
	CEditVPg::OnInitDialog();
	
	CString strText;
	for (int i = 0; i < 16; i++) {
		LPCTSTR pszText = m_strHintText[i];
		if (!pszText || !*pszText) pszText = NULL;
		strText.Format(pszText ? "$%04X - %s" : "$%04X", (UINT)(1 << i), pszText);
		m_wndList.AddString(strText);
	}

	m_nFlags = m_nInitVal;

	UpdateData(FALSE);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/////////////////////////////////////////////////////////////////////////////
// CEditVBinaryFlagsPg メッセージ ハンドラ

CString CEditVBinaryFlagsPg::Format(WORD x)
{
	CHAR c[17];
	WORD wTbl[16] = {0x8000,0x4000,0x2000,0x1000,0x0800,0x0400,0x0200,0x0100,0x0080,0x0040,0x0020,0x0010,0x0008,0x0004,0x0002,0x0001};
	for (int i = 0; i < 16; i++) {
		c[i] = ((x & wTbl[i]) != 0) ? '1' : '0';
	}
	c[16] = 0;
	return c;
}

CString CEditVBinaryFlagsPg::FormatDisp(WORD x)
{
	static WORD mask[16] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};

	CString strHint;
	UINT i;
	for (i = 0; i < 16; i++) {
		if (x & mask[i]) {
			if (!m_strHintText[i].IsEmpty()) {
				if (!strHint.IsEmpty())
					strHint += ", ";
				strHint += m_strHintText[i];
			}
		}
	}
	return strHint;
}

bool CEditVBinaryFlagsPg::Parse(LPCSTR pszText, WORD &x)
{
	LPSTR pszTerm;
	x = strtol(pszText, &pszTerm, 2);
	return (*pszTerm == 0);
}

void CEditVBinaryFlagsPg::SetParm1(LPCTSTR psz)
{
	int i = 0;

	for (; i < 16; i++) {
		LPCTSTR pszPos = _tcschr(psz, '/');
		if (pszPos == NULL) {
			m_strHintText[i] = psz;
			i++;
			break;
		}
		m_strHintText[i] = CString(psz, pszPos - psz);
		psz = pszPos + 1;
	}
	for (; i < 16; i++) m_strHintText[i].Empty();
}
