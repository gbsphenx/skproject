// ActuHintWebVw.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "resource.h"
#include "ActuHintWebVw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActuHintWebVw

CActuHintWebVw::CActuHintWebVw()
{
	//{{AFX_DATA_INIT(CActuHintWebVw)
		// メモ: ClassWizard はこの位置にメンバの初期化処理を追加します。
	//}}AFX_DATA_INIT
}

CActuHintWebVw::~CActuHintWebVw()
{
}

void CActuHintWebVw::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActuHintWebVw)
		// メモ: ClassWizard はこの位置に DDX と DDV の呼び出しコードを追加します。
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CActuHintWebVw, CHtmlView)
	//{{AFX_MSG_MAP(CActuHintWebVw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActuHintWebVw diagnostics

/////////////////////////////////////////////////////////////////////////////
// CActuHintWebVw message handlers

void CActuHintWebVw::PostNcDestroy() 
{
	m_pBrowserApp->Release();
}

BOOL CActuHintWebVw::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CHtmlView::PreCreateWindow(cs))
		return false;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return true;
}

void CActuHintWebVw::OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel) 
{
//	*Cancel = true;
	
	CHtmlView::OnNewWindow2(ppDisp, Cancel);
}

extern const UINT PM_CLICK_AHMT;

void CActuHintWebVw::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel) 
{
#define MATCH(STR, WM) if (CString(lpszURL).CompareNoCase("app:" STR) == 0) { \
		pOwner->SendMessage(PM_CLICK_AHMT, (WM), (LPARAM)(HWND)*this); \
		*pbCancel = true; \
		return; \
	}

	CWnd *pOwner = GetOwner();
	if (pOwner != NULL) {
		MATCH("ItemType", ID_AHMT_ITEMTYPE);
		MATCH("ItemItem", ID_AHMT_ITEMITEM);
		MATCH("Count", ID_AHMT_COUNT);
		MATCH("Missile", ID_AHMT_MISSILE);
		MATCH("Creature", ID_AHMT_CREATURE);
		MATCH("Hero", ID_AHMT_HERO);
		MATCH("Ornate", ID_AHMT_ORNATE);
		MATCH("Sw", ID_AHMT_SW);
		MATCH("MessageType", ID_AHMT_MESSAGETYPE);
		MATCH("Inverse", ID_AHMT_INVERSE);
		MATCH("Beep", ID_AHMT_BEEP);
		MATCH("Delay", ID_AHMT_DELAY);
		MATCH("Locator", ID_AHMT_LOCATOR);
		MATCH("Strength", ID_AHMT_STR);
	}
	
	CHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}
