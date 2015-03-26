#if !defined(AFX_DM2ITEMLISTBOX_H__05ACE4BE_1CBD_4C94_8D3D_5C3510C2B9F5__INCLUDED_)
#define AFX_DM2ITEMLISTBOX_H__05ACE4BE_1CBD_4C94_8D3D_5C3510C2B9F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2ItemListBox.h : ヘッダー ファイル
//

#include "DDAT.h"
#include <vector>
#include "DM2NameResolver.h"

enum CDM2ListNodeType {
	lntNo,
	lntRecord,
	lntRoom,
};

enum CDM2ListNodeAddWay {
	lnawAppend,
	lnawInsertAfter,
	lnawInsertBefore,
	lnawSubFirst,
	lnawSubLast,
};

enum CDM2ListNodeCopyWay {
	lncwSub,
	lncwSubSib,
};

enum CDM2ListNodePasteWay {
	lnpwAtFirst,
	lnpwAtLast,
	lnpwBefore,
	lnpwAfter,
	lnpwAtFirstInSub,
	lnpwAtLastInSub,
};

class CDM2ItemListNode
{
	// 
	struct ErodeBase {
		// 
		CDDAT *pDDIf;
		// 
		set<DBIPos_t> s;
	};

	// 
	bool Erode(ErodeBase &r, RN rn);
	// 
	CDM2ItemListNode *MkErosionNode(ErodeBase &r, RN rn);

	// 
	CDM2ItemListNode(const CDM2ItemListNode &src);
	// 
	void operator =(const CDM2ItemListNode &src);

	// 
	void Clone(CDM2ItemListNode &src, bool fNoSib = false);

public:
	// 
	CDM2ItemListNode *pSib;
	// 
	CDM2ItemListNode *pSub;

	// 
	CDM2ListNodeType lnt;

	// 
	AtlasPos_t ap;

	// 
	BYTE iDB;
	// 
	BYTE iDir;
	// 
	DMDBU rec;

	// 
	CDM2ItemListNode()
	{
		pSib = NULL;
		pSub = NULL;

		lnt = lntNo;

		ZeroMemory(&rec, sizeof(rec));
	}
	// 
	~CDM2ItemListNode()
	{
		Close();
	}
	// 
	void Close()
	{
		if (pSib) delete pSib; pSib = NULL;
		if (pSub) delete pSub; pSub = NULL;
	}
	// 
	CDM2ItemListNode *AddSubNode(CDM2ItemListNode *p)
	{
		if (p == NULL) return NULL;
		if (pSub) {
			return pSub->AddSibNode(p);
		}
		return pSub = p;
	}
	// 
	CDM2ItemListNode *AddSibNode(CDM2ItemListNode *p)
	{
		if (p == NULL) return NULL;
		if (pSib) {
			return pSib->AddSibNode(p);
		}
		return pSib = p;
	}
	// 
	void AttachSibNode(CDM2ItemListNode *p) { AddSibNode(p); }
	// 
	void JoinSibNode(CDM2ItemListNode *pNew, int nDistance)
	{
		CDM2ItemListNode *pWalk = this;
		for (; pWalk->pSib != NULL && nDistance > 0; pWalk = pWalk->pSib, nDistance--);
		pNew->AttachSibNode(pWalk->pSib);
		pWalk->pSib = pNew;
	}
	// 
	CDM2ItemListNode *MkDBTree(CDDAT *pDDIf, RN rn);
	// 
	CDM2ItemListNode *Clone();
	// 
	CDM2ItemListNode *CloneNoSib();
	// 
	bool IsContainer() const
	{
		return iDB == 4 || iDB == 9 || iDB == 14;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CDM2ItemListNodeX

class CDM2ItemListNodeX
{
	// 
	typedef CDM2ItemListNode Node_t;

	// 
	CDMDBRMask aLoop;
	// 
	LPCSTR pszPos;
	// 
	LPCSTR pszEnd;

	// 
	CString MkText(Node_t *p, int i, bool fNoSib);
	// 
	Node_t *ParseText(list<CString>::iterator &iterPos, list<CString>::iterator iterEnd, int iDepth);
	// 
	bool ParseDir(int &iDir);
	// 
	bool ParseMark(char c);
	// 
	bool ParseDBName(int &iDB);
	// 
	bool ParseRec(int iDB, DMDBU &rec);
	// 
	bool ParseHexByte(BYTE &x);
	// 
	bool ParseHexWord(WORD &x);
	// 
	bool ParseHex1(char c, BYTE &x);

public:
	// 
	bool Copy(CDM2ItemListNode *p, CString &strRet, bool fNoSib = false);
	// 
	Node_t *Paste(CString str);
};

/////////////////////////////////////////////////////////////////////////////
// CDM2ItemListBox ウィンドウ

class CDM2ItemListBox : public CListBox
{
	// 
	struct Liso {
		// 
		CDM2ItemListNode *p;
		// 
		int iIndent;
		// 
		BOOL fSib[32];

		// 
		Liso()
		{
			ZeroMemory(fSib, sizeof(fSib));
		}
	};
	// 
	typedef vector<Liso> LisoArray;
	// 
	enum {
		L0,
		Lc,
		Lcs,
		Lpcs,
		Lps,
		Lpc,
	};

	// 
	CDM2ItemListNode *m_pRoot;
	// 
	LisoArray m_vec;
	// 
	TEXTMETRIC m_tm;
	// 
	int m_cyLine;
	// 
	int m_yLine;
	// 
	int m_cyIcon;
	// 
	bool m_fTileIsWall;
	// 
	CExtBM m_offscr;
	// 
	DM2NameResolver_t *m_pNameRes;

	// 
	void Walk(CDM2ItemListNode *p, int iIndent, vector<BOOL> v);
	// 
	void Remove1Node(CDM2ItemListNode *p, CDM2ItemListNode *pRoot = NULL);
	// 
	void SendParentNotify(UINT nCode)
	{
		CWnd *p = GetParent();
		if (p) p->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), nCode), (LPARAM)(HWND)*this);
	}
	// 
	int GetValCurSel(int iSel)
	{
		if (iSel < 0)
			iSel = GetCurSel();
		if (iSel < 0 || m_vec.size() <= iSel)
			iSel = -1;
		return iSel;
	}
	// 
	void Clone1Node(CDM2ItemListNode *pWhat, CDM2ItemListNode *pWalk = NULL);
	// 
	BOOL Insert1Node(CDM2ItemListNode *pWhat, CDM2ItemListNode *pWhere, CDM2ItemListNode *pWalk, CDM2ListNodeAddWay lnaw);
	// 
	void Insert1Node_Sub(CDM2ItemListNode *pWhat, CDM2ItemListNode *pWalk, CDM2ListNodeAddWay lnaw);
	// 
	void Insert1Node_Sib(CDM2ItemListNode *pWhat, CDM2ItemListNode *pWalk, CDM2ListNodeAddWay lnaw, bool fNoSib = false);
	// 
	bool Prepare_offscreen(CSize size);

public:
	// 
	CDM2ItemListBox();
	// 
	virtual ~CDM2ItemListBox();
	// 
	void Close()
	{
		if (m_pRoot) delete m_pRoot; m_pRoot = NULL;
	}
	// 
	void Append(CDM2ItemListNode *p, bool fTileIsWall)
	{
		p = p->Clone();

		if (m_pRoot == NULL) {
			m_pRoot = p;
		} else {
			m_pRoot->AddSibNode(p);
		}

		m_fTileIsWall = fTileIsWall;
	}
	// 
	void Rebuild();
	// 
	CDM2ItemListNode *GetSelNode(int iSel = -1);
	// 
	BOOL RemoveSelNode(int iSel = -1);
	// 
	BOOL CloneSelNode(int iSel = -1);
	// 
	BOOL InsertNode(CDM2ItemListNode *pWhat, int iSel, CDM2ListNodeAddWay lnaw);
	// 
	BOOL CopyNode(CDM2ListNodeCopyWay lncw, int iSel = -1);
	// 
	BOOL PasteNode(CDM2ListNodePasteWay lncw, int iSel = -1);

	// 
	void SetNameResolver(DM2NameResolver_t *pNameRes)
	{
		m_pNameRes = pNameRes;
	}

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDM2ItemListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

public:

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDM2ItemListBox)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2ITEMLISTBOX_H__05ACE4BE_1CBD_4C94_8D3D_5C3510C2B9F5__INCLUDED_)
