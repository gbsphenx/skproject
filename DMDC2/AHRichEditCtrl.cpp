// AHRichEditCtrl.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "AHRichEditCtrl.h"
#include "DM2NameDic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT PM_CLICK_AHMT = RegisterWindowMessage("PM_CLICK_AHMT");

/////////////////////////////////////////////////////////////////////////////
// CAHRichEditCtrl

CAHRichEditCtrl::CAHRichEditCtrl()
{
}

CAHRichEditCtrl::~CAHRichEditCtrl()
{
}

bool CAHRichEditCtrl::LoadFromFile(LPCTSTR pszFile, bool isRTF)
{
	FILE *f = fopen(pszFile, "rt");
	if (f != NULL) {
		EDITSTREAM es;
		es.dwCookie = reinterpret_cast<DWORD_PTR>(f);
		es.dwError = 0;
		es.pfnCallback = _fn_LoadFromFile;
		if (StreamIn(isRTF ? SF_RTF : SF_TEXT, es)) {
			fclose(f);
			return true;
		}
		fclose(f);
	}
	return false;
}

DWORD CALLBACK CAHRichEditCtrl::_fn_LoadFromFile(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	FILE *f = reinterpret_cast<FILE *>(dwCookie);
	*pcb = fread(pbBuff, 1, cb, f);
	return 0;
}

#if !(WINVER >= 0x0500)
const int COLOR_HOTLIGHT = 26;
#endif

void CAHRichEditCtrl::mark(LPCTSTR pszWord, int type)
{
	FINDTEXTEX ft;
	ft.chrg.cpMin = 0;
	ft.chrg.cpMax = GetTextLength();
	ft.lpstrText = const_cast<LPTSTR>(pszWord);
	CHARFORMAT cf;
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_UNDERLINE; // |CFM_COLOR
	cf.dwEffects = CFE_UNDERLINE|CFE_AUTOCOLOR;
	cf.crTextColor = GetSysColor(COLOR_HOTLIGHT);

	while (true) {
		long pos = FindText(FR_WHOLEWORD, &ft);
		if (pos < 0) break;

		if (1000 <= cEnts) break;
		ents[cEnts].chrg = ft.chrgText;
		ents[cEnts].type = type;
		cEnts++;

		SetSel(ft.chrgText);
		SetSelectionCharFormat(cf);
		ft.chrg.cpMin = pos +1;
	}

	SetSel(0, 0);
}

void CAHRichEditCtrl::Mark2Words()
{
	cEnts = 0;

	mark("ItemType*", ID_AHMT_ITEMTYPE);
	mark("ItemItem*", ID_AHMT_ITEMITEM);
	mark("Count*", ID_AHMT_COUNT);
	mark("Missile*", ID_AHMT_MISSILE);
	mark("Creature*", ID_AHMT_CREATURE);
	mark("Hero*", ID_AHMT_HERO);
	mark("Ornate*", ID_AHMT_ORNATE);
	mark("Sw*", ID_AHMT_SW);
	mark("MessageType*", ID_AHMT_MESSAGETYPE);
	mark("Inverse*", ID_AHMT_INVERSE);
	mark("Beep*", ID_AHMT_BEEP);
	mark("Delay*", ID_AHMT_DELAY);
	mark("Locator*", ID_AHMT_LOCATOR);
	mark("Strength*", ID_AHMT_STR);
}

BEGIN_MESSAGE_MAP(CAHRichEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CAHRichEditCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(EN_SELCHANGE, OnSelChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAHRichEditCtrl メッセージ ハンドラ

void CAHRichEditCtrl::OnSelChange(SELCHANGE *pNMHDR, LRESULT *pRes)
{
	int cp = pNMHDR->chrg.cpMin;

	for (int x=0; x<cEnts; x++) {
		CAH_Entry &r = ents[x];
		if (r.chrg.cpMin <= cp && cp < r.chrg.cpMax) {
			SetSel(r.chrg);
			if (GetParent() != NULL)
				GetParent()->SendMessage(PM_CLICK_AHMT, r.type, (LPARAM)(HWND)*this);
			return;
		}
	}
}

int CAHRichEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	cEnts = 0;
	
	return 0;
}

void CAHRichEditCtrl::PreSubclassWindow() 
{
	SetEventMask(GetEventMask() |ENM_SELCHANGE);

	cEnts = 0;
	
	CRichEditCtrl::PreSubclassWindow();
}
