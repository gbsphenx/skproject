// DM2SelOrnateImage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "DM2SelOrnateImage.h"
#include "SplitStr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM2SelOrnateImageDlg ダイアログ

CDM2SelOrnateImageDlg::CDM2SelOrnateImageDlg(int t, CWnd* pParent /*=NULL*/)
	: CDialog(CDM2SelOrnateImageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDM2SelOrnateImageDlg)
	m_iOrnate = -1;
	//}}AFX_DATA_INIT

	m_t = t;
}

CDM2SelOrnateImageDlg::CDM2SelOrnateImageDlg(bool fDoor, bool fWall, CWnd* pParent /*=NULL*/)
	: CDialog(CDM2SelOrnateImageDlg::IDD, pParent)
{
	m_iOrnate = -1;

	m_t = fDoor
		? (fWall ? tDoorOrnates : tDoors)
		: (fWall ? tWallOrnates : tFloorOrnates)
		;
}

void CDM2SelOrnateImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDM2SelOrnateImageDlg)
	DDX_Control(pDX, IDC_TREE_CLASSES, m_wndTree);
	DDX_Control(pDX, IDC_COMBO_ORNATE, m_wndOrnate);
	DDX_CBIndex(pDX, IDC_COMBO_ORNATE, m_iOrnate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDM2SelOrnateImageDlg, CDialog)
	//{{AFX_MSG_MAP(CDM2SelOrnateImageDlg)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CLASSES, OnSelchangedTreeClasses)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2SelOrnateImageDlg メッセージ ハンドラ

BOOL CDM2SelOrnateImageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_wndOrnate.All(m_t);
	m_wndOrnate.Freshen();
	m_wndOrnate.SetCurSel((m_iOrnate < 0) ? 0 : m_iOrnate);

	MkTree();
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDM2SelOrnateImageDlg::MkTree()
{
	//m_wndTree.SetBkColor(RGB(0, 0, 0));
	//m_wndTree.SetTextColor(RGB(255, 255, 255));

	switch (m_t) {
	case 9: case 10: break;
	default:
		m_wndTree.ShowWindow(SW_HIDE);
		return;
	}

	CDM2OrnateTreeDic &t = CDM2OrnateTreeDic::Get1((m_t == 9) ? 0 : 1);
	const CStringArray &sa = t.GetTree();

	UINT i, n = sa.GetSize();
	for (i = 0; i < n; i++) {
		CStringList ss;
		SplitStr(sa[n - i - 1], '/', ss);

		HTREEITEM h = m_wndTree.GetNextItem(NULL, TVGN_ROOT);
		HTREEITEM hParent = NULL;
		CString strClass;
		while (!ss.IsEmpty()) {
			CString str = ss.RemoveHead();
			if (str.IsEmpty()) continue;

			if (!strClass.IsEmpty()) strClass += "/";
			strClass += str;

			while (h != NULL) {
				if (hParent != NULL && hParent == h) {
					h = m_wndTree.GetChildItem(h);
				}
				if (m_wndTree.GetItemText(h) == str) {
					if (m_wndTree.GetItemData(h) & 0x4000)
						break;
				}
				h = m_wndTree.GetNextSiblingItem(h);
			}

			if (h == NULL) {
				h = m_wndTree.InsertItem(str, (hParent == NULL) ? TVI_ROOT : hParent, TVI_FIRST);
			}

			if (!(m_wndTree.GetItemData(h) & 0x4000)) {
				CUIntArray va;
				t.FindNames(strClass, va);

				UINT z = 0;
				for (z = 0; z < va.GetSize(); z++) {
					HTREEITEM hItem = m_wndTree.InsertItem(t.GetName(va[z]), h);
					m_wndTree.SetItemData(hItem, 0x8000 | va[z]);
				}
				m_wndTree.SetItemData(h, 0x4000);
			}
			hParent = h;
		}
	}
}

void CDM2SelOrnateImageDlg::OnSelchangedTreeClasses(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	LPARAM lParam = pNMTreeView->itemNew.lParam;
	if (lParam & 0x8000) {
		m_wndOrnate.SetCurSel(lParam & 0xFF);
	}
	
	*pResult = 0;
}
