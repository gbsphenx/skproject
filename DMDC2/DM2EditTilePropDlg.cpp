// DM2EditTilePropDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "DM2EditTilePropDlg.h"
#include "DM2NameDic.h"

#include "ExtBM.h"
#include "CompatDC.h"
#include "PWM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// 

class PrintClient_t
{
public:
	// 
	bool Draw(CWnd *pWnd)
	{
		if (pWnd == NULL)
			return false;
		CRect rc;
		pWnd->GetClientRect(&rc);
		CClientDC cDC(pWnd);
		CCompatDC xDC(&cDC);
		CExtBM bm;
		if (!bm.Create(rc.Size(), 24, NULL))
			return false;
		HGDIOBJ hOLD = xDC.SelectObject(bm);
		///
		pWnd->PrintClient(&xDC, 0 |PRF_CHECKVISIBLE |PRF_CHILDREN |PRF_CLIENT |PRF_OWNED);
		pWnd->ValidateRect(NULL);
		//cDC.BitBlt(0, 0, rc.Width(), rc.Height(), &xDC, 0, 0, SRCCOPY);
		///
		xDC.SelectObject(hOLD);
		return true;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CDM2EditTilePropDlg ダイアログ


CDM2EditTilePropDlg::CDM2EditTilePropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDM2EditTilePropDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDM2EditTilePropDlg)
	m_iObjSel = -1;
	m_iTypeSel = -1;
	//}}AFX_DATA_INIT

	m_pDDIf = NULL;
	m_pTree = NULL;

	m_wndActu1.SetPISink(this);
	m_wndActu2.SetPISink(this);
	m_wndActu3.SetPISink(this);
	m_wndActuHint.SetPISink(this);
	m_wndChest.SetPISink(this);
	m_wndCloth.SetPISink(this);
	m_wndCreature.SetPISink(this);
	m_wndMisc.SetPISink(this);
	m_wndPotion.SetPISink(this);
	m_wndScroll.SetPISink(this);
	m_wndText.SetPISink(this);
	m_wndText2.SetPISink(this);
	m_wndWeapon.SetPISink(this);
	m_wndNo.SetPISink(this);
	m_wndTele.SetPISink(this);
	m_wndDoor.SetPISink(this);

	m_wndSheet.Construct(_T(""), this);

	m_wndItemList.SetNameResolver(&m_nameRes);
}

CDM2EditTilePropDlg::~CDM2EditTilePropDlg()
{
	if (m_pTree) delete m_pTree;
}

void CDM2EditTilePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDM2EditTilePropDlg)
	DDX_Control(pDX, IDC_TAB, m_wndTab);
	DDX_Control(pDX, IDC_BUTTON_RECORD_ADD_SUB_LAST, m_wndBtnAddSubLast);
	DDX_Control(pDX, IDC_BUTTON_RECORD_ADD_SUB_FIRST, m_wndBtnAddSubFirst);
	DDX_Control(pDX, IDC_BUTTON_RECORD_REMOVE, m_wndBtnRemove);
	DDX_Control(pDX, IDC_BUTTON_RECORD_INSERT_BEFORE, m_wndBtnInsertBefore);
	DDX_Control(pDX, IDC_BUTTON_RECORD_INSERT_AFTER, m_wndBtnInsertAfter);
	DDX_Control(pDX, IDC_BUTTON_RECORD_APPEND, m_wndBtnAppend);
	DDX_Control(pDX, IDC_BUTTON_RECORD_CLONE, m_wndBtnClone);
	DDX_Control(pDX, IDC_CHECK_DIR_W, m_wndDirW);
	DDX_Control(pDX, IDC_CHECK_DIR_S, m_wndDirS);
	DDX_Control(pDX, IDC_CHECK_DIR_N, m_wndDirN);
	DDX_Control(pDX, IDC_CHECK_DIR_E, m_wndDirE);
	DDX_Control(pDX, IDC_COMBO_OBJ, m_wndObj);
	DDX_Control(pDX, IDC_LIST_ITEMS, m_wndItemList);
	DDX_CBIndex(pDX, IDC_COMBO_OBJ, m_iObjSel);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_iTypeSel);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDM2EditTilePropDlg, CDialog)
	//{{AFX_MSG_MAP(CDM2EditTilePropDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelchangeComboType)
	ON_CBN_SELCHANGE(IDC_COMBO_OBJ, OnSelchangeComboObj)
	ON_LBN_SELCHANGE(IDC_LIST_ITEMS, OnSelchangeListItems)
	ON_BN_CLICKED(IDC_BUTTON_RECORD_REMOVE, OnButtonRecordRemove)
	ON_BN_CLICKED(IDC_BUTTON_RECORD_CLONE, OnButtonRecordClone)
	ON_BN_CLICKED(IDC_BUTTON_RECORD_APPEND, OnButtonRecordAppend)
	ON_BN_CLICKED(IDC_BUTTON_RECORD_INSERT_AFTER, OnButtonRecordInsertAfter)
	ON_BN_CLICKED(IDC_BUTTON_RECORD_INSERT_BEFORE, OnButtonRecordInsertBefore)
	ON_BN_CLICKED(IDC_BUTTON_RECORD_ADD_SUB_FIRST, OnButtonRecordAddSubFirst)
	ON_BN_CLICKED(IDC_BUTTON_RECORD_ADD_SUB_LAST, OnButtonRecordAddSubLast)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	ON_BN_CLICKED(ID_BUTTON_GET_BACK, OnButtonGetBack)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_REFRESH_PROPERTY_PAGE, OnCommand0100)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK_DIR_N, IDC_CHECK_DIR_W, OnCheckDirN)
	ON_COMMAND_RANGE(ID_CONTEXT_COPY_SUB, ID_CONTEXT_PASTE_AT_LAST_IN_SUB, OnContextCopy)
	ON_UPDATE_COMMAND_UI_RANGE(ID_CONTEXT_COPY_SUB, ID_CONTEXT_PASTE_AT_LAST_IN_SUB, OnUpdateContextCopy)
	ON_COMMAND_RANGE(ID_AHMT_ITEMTYPE, ID_AHMT_STR, OnAHMT)
	ON_MESSAGE(PWM_COLLECT_SAMPLER, OnCollectSampler)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2EditTilePropDlg メッセージ ハンドラ

bool CDM2EditTilePropDlg::Init(CDDAT *pDDIf, AtlasPos_t ap)
{
	m_pDDIf = pDDIf;
	m_ap = ap;

	m_pTree = NULL;

	AtlasBit_t ab;
	HRESULT hr;
	if (FAILED(hr = pDDIf->GetAtlasBitAt(ap, ab))) return false;
	if (FAILED(hr = pDDIf->ScanOrnate(ap.Z(), m_oim))) return false;

	AllMapDesc_t am;
	if (FAILED(hr = pDDIf->GetAllMapDesc(am))) return false;

	m_nameRes.texte.Load(am.fvTextData);
	m_nameRes.pDDIf = m_pDDIf;

	m_fTileIsWall = CDDAT::IsTileWall(ab.c);

	m_pTree = new CDM2ItemListNode();
	m_pTree->lnt = lntRoom;
	m_pTree->ap = ap;
	m_pTree->pSub = m_pTree->MkDBTree(pDDIf, ab.r);
	return true;
}

BOOL CDM2EditTilePropDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_wndItemList.Append(m_pTree, m_fTileIsWall);
	m_wndItemList.Rebuild();

	m_pActive = NULL;
	m_iOldSel = m_iNewSel = -1;

//	Tab_RemoveAll();
//	Tab_Add(m_wndNo);

	UINT n = m_wndItemList.GetCount();
	if (2 <= n) {
		m_wndItemList.SetCurSel(1);
		OnSelchangeListItems();
	} else if (1 <= n) {
		m_wndItemList.SetCurSel(0);
		OnSelchangeListItems();
	}

	m_wndBackBtn.Create(this);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDM2EditTilePropDlg::OnSelchangeComboType() 
{
	if (!UpdateData()) return;

	m_wndObj.ResetContent();

	// "Pad/text"
	// "Actuator"
	// "Creature"
	// "Item: Weapon"
	// "Item: Cloth"
	// "Item: Scroll"
	// "Item: Potion"
	// "Item: Chest"
	// "Item: Misc"

	int iDBType = -1;
	bool fWall = false;
	int t = 0;

	switch (m_iTypeSel) {
	case 0: iDBType = 2, t = 1; break;
	case 1: iDBType = 3, fWall = true; break;
	case 2: iDBType = 4; break;
	case 3: iDBType = 5; break;
	case 4: iDBType = 6; break;
	case 5: iDBType = 7; break;
	case 6: iDBType = 8; break;
	case 7: iDBType = 9; break;
	case 8: iDBType = 10; break;
	}

	if (iDBType < 0) return;

	CString str;

	switch (t) {
	case 0:
		{
			for (int i = 0; i < 128; i++) {
				int v = (m_fTileIsWall && fWall) ? 128 : 0;
				LPCSTR psz = CDM2NameDic::GetInstance().GetName(iDBType, i +v);
				m_wndObj.AddString(psz ? psz : "");
			}
			break;
		}
	case 1:
		{
			CString strText;
			for (int i = 0; i < 256; i++) {
				strText.Format("%d", i);
				m_wndObj.AddString(strText);
			}
			break;
		}
	}

	m_wndObj.SetCurSel(0);
}

void CDM2EditTilePropDlg::OnSelchangeComboObj() 
{

}

void CDM2EditTilePropDlg::OnSelchangeListItems() 
{
	CDM2ItemListNode *p = m_wndItemList.GetSelNode(-1);

	BYTE iDir = 0;
	bool fIsRecord = false;
	bool fHaveSubAdd = false;
	CDM2ListNodeType lnt = lntNo;
	BYTE iDB = -1;

	if (p) {
		iDir = p->iDir;
		fIsRecord = (p->lnt == lntRecord);
		fHaveSubAdd = (p->iDB == 4) || (p->iDB == 9) || (p->iDB == 14);
		lnt = p->lnt;
		iDB = p->iDB;
	}

	m_wndDirN.EnableWindow(fIsRecord);
	m_wndDirE.EnableWindow(fIsRecord);
	m_wndDirS.EnableWindow(fIsRecord);
	m_wndDirW.EnableWindow(fIsRecord);

	m_wndBtnRemove.EnableWindow(fIsRecord);
	m_wndBtnClone.EnableWindow(fIsRecord);

	m_wndDirN.SetCheck(fIsRecord && iDir == 0);
	m_wndDirE.SetCheck(fIsRecord && iDir == 1);
	m_wndDirS.SetCheck(fIsRecord && iDir == 2);
	m_wndDirW.SetCheck(fIsRecord && iDir == 3);

	m_wndBtnAddSubFirst.EnableWindow(fHaveSubAdd);
	m_wndBtnAddSubLast.EnableWindow(fHaveSubAdd);

	m_iNewSel = m_wndItemList.GetCurSel();

	if (m_iOldSel != m_iNewSel) {

		Tab_RemoveAll();
		m_wndBackBtn.Hide();

		m_iOldSel = m_iNewSel;

		switch (lnt) {
		case lntRoom:
			{
				Tab_Add(m_wndNo);
				break;
			}
		case lntRecord:
			{
				if (p) {
					switch (iDB) {
					case  0: Tab_Add(m_wndDoor); break;
					case  1: Tab_Add(m_wndTele); break;
					case  2:
						Tab_Add(m_wndText);
						Tab_Add(m_wndText2);
						break;
					case  3:
						Tab_Add(m_wndActuHint);
						Tab_Add(m_wndActu1);
						Tab_Add(m_wndActu2);
						Tab_Add(m_wndActu3);
						break;
					case  4: Tab_Add(m_wndCreature); break;
					case  5: Tab_Add(m_wndWeapon); break;
					case  6: Tab_Add(m_wndCloth); break;
					case  7: Tab_Add(m_wndScroll); break;
					case  8: Tab_Add(m_wndPotion); break;
					case  9: Tab_Add(m_wndChest); break;
					case 10: Tab_Add(m_wndMisc); break;
					default:
						Tab_Add(m_wndNo);
						break;
					}
				}
				break;
			}
		}
	}
}

void CDM2EditTilePropDlg::OnButtonRecordRemove() 
{
	ForceInactivate();

	m_wndItemList.RemoveSelNode();

	int iSel = m_wndItemList.GetCurSel();
	if (iSel < 0) {
		m_wndItemList.SetCurSel(m_wndItemList.GetCount() - 1);
	}

	ForceActivate();
}

void CDM2EditTilePropDlg::OnButtonRecordClone() 
{
	m_wndItemList.CloneSelNode();
}

void CDM2EditTilePropDlg::OnButtonRecordAppend() 
{
	if (!UpdateData()) return;

	CDM2ItemListNode *p = MkNode();
	VERIFY(m_wndItemList.InsertNode(p, -1, lnawAppend));
}

void CDM2EditTilePropDlg::OnButtonRecordInsertAfter() 
{
	if (!UpdateData()) return;

	CDM2ItemListNode *p = MkNode();
	VERIFY(m_wndItemList.InsertNode(p, -1, lnawInsertAfter));
}

void CDM2EditTilePropDlg::OnButtonRecordInsertBefore() 
{
	if (!UpdateData()) return;

	ForceInactivate();

	CDM2ItemListNode *p = MkNode();
	VERIFY(m_wndItemList.InsertNode(p, -1, lnawInsertBefore));

	ForceActivate();
}

CDM2ItemListNode *CDM2EditTilePropDlg::MkNode()
{
	int iDB = -1;
	bool fCreature = false;
	switch (m_iTypeSel) {
	case 0: iDB = 2; break;
	case 1: iDB = 3; break;
	case 2: iDB = 4; break;
	case 3: iDB = 5; break;
	case 4: iDB = 6; break;
	case 5: iDB = 7; break;
	case 6: iDB = 8; break;
	case 7: iDB = 9; break;
	case 8: iDB = 10; break;
	}
	CDM2ItemListNode *p = new CDM2ItemListNode();
	if (iDB >= 0) {
		p->lnt = lntRecord;
		p->iDB = iDB;
		p->iDir = 0;
		switch (iDB) {
		case 2:
			p->rec.r2.w2 = (m_iObjSel & 0xFF) * 8 +2;
			break;
		case 3:
			p->rec.r3.w2 = (m_iObjSel & 0x7F);
			break;
		case 4:
			p->rec.r4.b4 = (m_iObjSel & 0xFF);
			break;
		case 5:
		case 6:
		case 7:
		case 10:
			p->rec.r5.w2 = (m_iObjSel & 0xFF);
			break;
		case 8:
			p->rec.r8.w2 = (m_iObjSel & 0xFF) * 256;
			break;
		case 9:
			p->rec.r9.w4 = CExp00069App::ID2CHES(m_iObjSel);
			break;
		}
		return p;
	}
	delete p;
	return NULL;
}

void CDM2EditTilePropDlg::OnCheckDirN(UINT nID) 
{
	int iDir = -1;
	switch (nID) {
	case IDC_CHECK_DIR_N: iDir = 0; break;
	case IDC_CHECK_DIR_E: iDir = 1; break;
	case IDC_CHECK_DIR_S: iDir = 2; break;
	case IDC_CHECK_DIR_W: iDir = 3; break;
	}
	if (iDir < 0) return;

	m_wndDirN.SetCheck(iDir == 0);
	m_wndDirE.SetCheck(iDir == 1);
	m_wndDirS.SetCheck(iDir == 2);
	m_wndDirW.SetCheck(iDir == 3);

	CDM2ItemListNode *pNode = m_wndItemList.GetSelNode();

	if (pNode && pNode->lnt == lntRecord) {
		pNode->iDir = iDir;
		m_wndItemList.Rebuild();
	}
}

void CDM2EditTilePropDlg::OnButtonRecordAddSubFirst() 
{
	if (!UpdateData()) return;

	CDM2ItemListNode *p = MkNode();
	VERIFY(m_wndItemList.InsertNode(p, -1, lnawSubFirst));
}

void CDM2EditTilePropDlg::OnButtonRecordAddSubLast() 
{
	if (!UpdateData()) return;

	CDM2ItemListNode *p = MkNode();
	VERIFY(m_wndItemList.InsertNode(p, -1, lnawSubLast));
}

void CDM2EditTilePropDlg::OnOK() 
{
	if (m_pActive) {
		if (!m_pActive->OnKillActive()) return;
	}

	CDM2ItemListNode *p = m_wndItemList.GetSelNode(0);

	if (p != NULL) {
		if (!WriteBack(p)) {
			AfxMessageBox(IDS_ERR_DB_AT_MAX_CAP, 0 |MB_ICONEXCLAMATION);
			return;
		}
	}
	
	CDialog::OnOK();
}

bool CDM2EditTilePropDlg::WriteBack(CDM2ItemListNode *p)
{
	if (p->lnt != lntRoom) return false;
	AtlasPos_t ap = p->ap;

	CDDATUndoLock lock(*m_pDDIf, TRUE);

	AtlasBit_t ab;
	HRESULT hr;
	VERIFY(SUCCEEDED(hr = m_pDDIf->GetAtlasBitAt(ap, ab)));
	//VERIFY(SUCCEEDED(hr = m_pDDIf->RemoveDBRecAssoc(DBIPos_t(ab.r), TRUE, TRUE)));
	ab.r = MkBackTree(p->pSub);
	if (ab.r == RN_NIL) return false;
	if (ab.r == RN_END) {
		ab.c &= ~0x10;
		ab.r = RN_NIL;
	} else {
		ab.c |= 0x10;
	}
	VERIFY(SUCCEEDED(hr = m_pDDIf->SetAtlasBitAt(ap, ab)));
	if (FAILED(hr = m_pDDIf->UpdateOrnate(ap.Z(), m_oim))) return false;
	lock.Unlock(true);

	return true;
}

RN CDM2EditTilePropDlg::MkBackTree(CDM2ItemListNode *p)
{
	if (p == NULL) return RN_END;

	ASSERT(p->lnt == lntRecord);

	DMDBU rec = p->rec;

	RN rn;
	if ((rn = MkBackTree(p->pSib)) == RN_NIL) return RN_NIL;
	rec.r0.next = rn;

	switch (p->iDB) {
	case 4:
	case 9:
	case 14:
		{
			if ((rn = MkBackTree(p->pSub)) == RN_NIL) return RN_NIL;
			rec.r4.stolen = rn;
			break;
		}
	}

	HRESULT hr;
	DBIPos_t pos;
	if (FAILED(hr = m_pDDIf->AddDBRecToEmpty(p->iDB, rec, pos)))
		return RN_NIL;

	return RNco(pos.db(), pos.idx(), p->iDir);
}

void CDM2EditTilePropDlg::OnContextCopy(UINT nID) 
{
	switch (nID) {
	case ID_CONTEXT_COPY_SUB:
		m_wndItemList.CopyNode(lncwSub);
		break;
	case ID_CONTEXT_COPY_SUB_SIB:
		m_wndItemList.CopyNode(lncwSubSib);
		break;
	case ID_CONTEXT_PASTE_AT_LAST:
		m_wndItemList.PasteNode(lnpwAtLast);
		break;
	case ID_CONTEXT_PASTE_AT_FIRST:
		m_wndItemList.PasteNode(lnpwAtFirst);
		break;
	case ID_CONTEXT_PASTE_AFTER:
		m_wndItemList.PasteNode(lnpwAfter);
		break;
	case ID_CONTEXT_PASTE_BEFORE:
		ForceInactivate();
		m_wndItemList.PasteNode(lnpwBefore);
		ForceActivate();
		break;
	case ID_CONTEXT_PASTE_AT_FIRST_IN_SUB:
		m_wndItemList.PasteNode(lnpwAtFirstInSub);
		break;
	case ID_CONTEXT_PASTE_AT_LAST_IN_SUB:
		m_wndItemList.PasteNode(lnpwAtLastInSub);
		break;
	}
}

void CDM2EditTilePropDlg::OnUpdateContextCopy(CCmdUI* pCmdUI) 
{

}

BYTE CDM2EditTilePropDlg::GetDB()
{
	CDM2ItemListNode *p = m_wndItemList.GetSelNode(m_iOldSel);
	if (p && p->lnt == lntRecord) {
		return p->iDB;
	}
	return 0xFF;
}

bool CDM2EditTilePropDlg::GetRecord(DMDBU &rec)
{
	CDM2ItemListNode *p = m_wndItemList.GetSelNode(m_iOldSel);
	if (p && p->lnt == lntRecord) {
		rec = p->rec;
		return true;
	}
	return false;
}

bool CDM2EditTilePropDlg::SetRecord(const DMDBU &rec)
{
	CDM2ItemListNode *p = m_wndItemList.GetSelNode(m_iOldSel);
	if (p && p->lnt == lntRecord) {
		p->rec = rec;
		m_wndItemList.Invalidate();
		return true;
	}
	return false;
}

void CDM2EditTilePropDlg::Tab_RemoveAll()
{
	VERIFY(m_wndTab.DeleteAllItems());

	if (m_pActive) VERIFY(m_pActive->OnKillActive());

	for (int i = 0, n = m_vecTab.GetSize(); i < n; i++) {
		m_vecTab[i]->DestroyWindow();
	}
	m_vecTab.RemoveAll();

	m_pActive = NULL;
}

bool CDM2EditTilePropDlg::Tab_Add(CEdPIDlg &wndDlg)
{
	if (!wndDlg.Create(this)) return false;

	wndDlg.SetDlgCtrlID(IDR_MAINFRAME);

	CString strText;
	wndDlg.GetWindowText(strText);

	int x = m_wndTab.GetItemCount();
	m_wndTab.InsertItem(x, strText);

	CRect rc;
	m_wndTab.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_wndTab.AdjustRect(FALSE, &rc);

	wndDlg.ModifyStyle(WS_CAPTION, 0, 0);

	wndDlg.SetWindowPos(&m_wndTab, rc.left, rc.top, rc.Width(), rc.Height(), 0 |SWP_NOOWNERZORDER);

	if (m_pActive == NULL) {
		m_pActive = &wndDlg;

		if (m_pActive->OnSetActive()) {
			m_pActive->ShowWindow(SW_SHOW);
			m_pActive->EnableWindow();
		} else {
			m_pActive->ShowWindow(SW_HIDE);
		}
	}
	m_vecTab.Add(&wndDlg);

	wndDlg.SetOwner(this);

	return true;
}

void CDM2EditTilePropDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int i = m_wndTab.GetCurSel();

	m_pActive = m_vecTab[i];

	if (m_pActive->OnSetActive()) {
		m_pActive->ShowWindow(SW_SHOW);
		m_pActive->EnableWindow();
	} else {
		m_pActive->ShowWindow(SW_HIDE);
	}
	
	*pResult = 0;
}

void CDM2EditTilePropDlg::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 1;

	m_wndBackBtn.Hide();

	if (m_pActive) {
		if (!m_pActive->OnKillActive()) return;
		m_pActive->ShowWindow(SW_HIDE);
		m_pActive = NULL;
	}
	
	*pResult = 0;
}

bool CDM2EditTilePropDlg::TileIsWall()
{
	return m_fTileIsWall;
}

void CDM2EditTilePropDlg::OnCommand0100()
{
	if (m_pActive) {
		m_pActive->OnKillActive();
		m_wndItemList.Invalidate();
	}
}

void CDM2EditTilePropDlg::ForceInactivate()
{
	if (m_pActive) {
		if (!m_pActive->OnKillActive()) return;
		m_pActive = NULL;
		m_iOldSel = -1;
	}
}

void CDM2EditTilePropDlg::ForceActivate()
{
	OnSelchangeListItems();
}

void CDM2EditTilePropDlg::OnAHMT(UINT nID)
{
	int i = -1, id = -1;
	switch (nID) {
	case ID_AHMT_ITEMTYPE: i=1; id=IDC_COMBO_ITEM_TYPE; break;
	case ID_AHMT_ITEMITEM: i=1; id=IDC_COMBO_ITEM_NAME; break;
	case ID_AHMT_COUNT: i=1; id=IDC_EDIT_N; break;
	case ID_AHMT_MISSILE: i=1; id=IDC_COMBO_MISSILE; break;
	case ID_AHMT_CREATURE: i=1; id=IDC_COMBO_CREATURE; break;
	case ID_AHMT_HERO: i=1; id=IDC_COMBO_HERO; break;
	case ID_AHMT_ORNATE: i=2; id=IDC_COMBO_ORNATE; break;
	case ID_AHMT_SW: i=2; id=IDC_CHECK_SW_IS_ON; break;
	case ID_AHMT_MESSAGETYPE: i=2; id=IDC_STATIC_MT; break;
	case ID_AHMT_INVERSE: i=2; id=IDC_CHECK_INVERSE; break;
	case ID_AHMT_BEEP: i=2; id=IDC_CHECK_BEEP; break;
	case ID_AHMT_DELAY: i=2; id=IDC_EDIT_DELAY; break;
	case ID_AHMT_LOCATOR: i=3; id=IDC_EDIT_X; break;
	case ID_AHMT_STR: i=3; id=IDC_SLIDER_STR; break;
	}
	if (i < 0) return;

	LRESULT lr = 0;
	OnSelchangingTab(NULL, &lr);
	m_wndTab.SetCurSel(i);
	OnSelchangeTab(NULL, &lr);

	CWnd *p = NULL;
	if (i == 1) p = &m_wndActu1;
	if (i == 2) p = &m_wndActu2;
	if (i == 3) p = &m_wndActu3;
	if (p == NULL) return;
	p = p->GetDlgItem(id);
	if (!(p->GetStyle() & WS_TABSTOP)) {
		p = GetNextDlgTabItem(p, false);
		if (p == NULL) return;
	}
	p->SetFocus();

	m_wndBackBtn.Show(p);
}

void CDM2EditTilePropDlg::OnButtonGetBack() 
{
	LRESULT lr = 0;
	OnSelchangingTab(NULL, &lr);
	m_wndTab.SetCurSel(0);
	OnSelchangeTab(NULL, &lr);
}

LRESULT CDM2EditTilePropDlg::OnCollectSampler(WPARAM wp, LPARAM lp)
{
	EndDialog(0x1000|wp);
	return 0;
}
