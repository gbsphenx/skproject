// DM2ItemListBox.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "DM2ItemListBox.h"
#include "CompatDC.h"
#include "ExtBM.h"
#include "DM2NameDic.h"
#include "DMRecFmt.h"
#include "SplitStr.h"
#include "Str2Clipbrd.h"
#include "BMODID.h"
#include "DM2TextRec.h"
#include "DM2ScrollRec.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM2ItemListNode

CDM2ItemListNode *CDM2ItemListNode::MkDBTree(CDDAT *pDDIf, RN rn)
{
	ErodeBase a;
	a.pDDIf = pDDIf;

	return MkErosionNode(a, rn);
}

CDM2ItemListNode *CDM2ItemListNode::MkErosionNode(ErodeBase &r, RN rn)
{
	if (rn == RN_END || rn == RN_NIL) return NULL;
	if (r.s.find(DBIPos_t(rn)) != r.s.end()) return NULL;

	CDM2ItemListNode *p = new CDM2ItemListNode();

	VERIFY(p->Erode(r, rn));
	return p;
}

bool CDM2ItemListNode::Erode(ErodeBase &r, RN rn)
{
	HRESULT hr;
	if (FAILED(hr = r.pDDIf->GetDBRecAt(DBIPos_t(rn), rec)))
		return false;

	iDB = rn.db();
	iDir = rn.room();
	lnt = lntRecord;

	switch (iDB) {
		case 11: case 12: case 13:
			iDB = 3;
			break;
	}

	r.s.insert(DBIPos_t(rn));

	switch (iDB) {
	case 4:
	case 9:
	case 14:
		AddSubNode(MkErosionNode(r, rec.r4.stolen));
		break;
	}

	AddSibNode(MkErosionNode(r, rec.r0.next));
	return true;
}

void CDM2ItemListNode::Clone(CDM2ItemListNode &src, bool fNoSib)
{
	pSib = fNoSib ? NULL : src.pSib->Clone();
	pSub = src.pSub->Clone();
	lnt = src.lnt;
	ap = src.ap;
	iDB = src.iDB;
	iDir = src.iDir;
	rec = src.rec;
}

CDM2ItemListNode *CDM2ItemListNode::Clone()
{
	if (this == NULL) return NULL;

	CDM2ItemListNode *p = new CDM2ItemListNode();
	p->Clone(*this);
	return p;
}

CDM2ItemListNode *CDM2ItemListNode::CloneNoSib()
{
	if (this == NULL) return NULL;

	CDM2ItemListNode *p = new CDM2ItemListNode();
	p->Clone(*this, true);
	return p;
}

/////////////////////////////////////////////////////////////////////////////
// CDM2ItemListNodeX

CString CDM2ItemListNodeX::MkText(Node_t *p, int i, bool fNoSib)
{
	if (p) {
		CString strRes;
		for (int s = i; s != 0; s--) strRes += '\t';
		strRes += CDMRecFmt::Format(p->iDB, p->iDir, p->rec, false) + "\r\n";
		if (p->pSub) strRes += MkText(p->pSub, i +1, false);
		if (p->pSib && !fNoSib) strRes += MkText(p->pSib, i, false);
		return strRes;
	}
	return "";
}

CDM2ItemListNodeX::Node_t *CDM2ItemListNodeX::ParseText(list<CString>::iterator &iterPos, list<CString>::iterator iterEnd, int iDepth)
{
	if (iterPos == iterEnd) return NULL;
	pszPos = *iterPos;
	pszEnd = strchr(pszPos, 0);
	while (true) {
		int c = iDepth;
		for (; c != 0 && pszPos < pszEnd; c--, pszPos++);
		int iDir;
		int iDB;
		if (!ParseDir(iDir)) break;
		if (!ParseMark(':')) break;
		if (!ParseDBName(iDB)) break;
		if (!ParseMark(':')) break;
		DMDBU rec;
		if (!ParseRec(iDB, rec)) break;
		iterPos++;
		Node_t *pSub = ParseText(iterPos, iterEnd, iDepth +1);
		Node_t *pSib = ParseText(iterPos, iterEnd, iDepth);
		Node_t *p = new Node_t();
		p->iDB = (BYTE)iDB;
		p->iDir = (BYTE)iDir;
		p->rec = rec;
		p->pSib = pSib;
		p->pSub = pSub;
		p->lnt = lntRecord;
		return p;
	}
	return NULL;
}

bool CDM2ItemListNodeX::ParseDir(int &iDir)
{
	switch (*pszPos) {
	case 'N': iDir = 0; break;
	case 'E': iDir = 1; break;
	case 'S': iDir = 2; break;
	case 'W': iDir = 3; break;
	default:
		return false;
	}
	pszPos++;
	return true;
}

bool CDM2ItemListNodeX::ParseMark(char c)
{
	if (pszPos >= pszEnd)
		return false;
	if (c != *pszPos)
		return false;
	pszPos++;
	return true;
}

bool CDM2ItemListNodeX::ParseDBName(int &iDB)
{
	if (pszPos + 4 >= pszEnd)
		return false;
	int i = 0;
	for (; i < 16; i++) {
		LPCSTR pszSrc = CDMRecFmt::GetTblName(i, false);
		int cch = strlen(pszSrc);
		if (strncmp(pszPos, pszSrc, cch) == 0) {
			iDB = i;
			pszPos += cch;
			break;
		}
	}
	iDB = i;
	return (i != 16);
}

bool CDM2ItemListNodeX::ParseRec(int iDB, DMDBU &rec)
{
	switch (iDB) {
	case 0:
	case 2:
	case 5:
	case 6:
	case 7:
	case 8:
	case 10:
		if (false
			|| !ParseHexWord(rec.r0.w2)
		) {
			return false;
		}
		break;
	case 1:
		if (false
			|| !ParseHexWord(rec.r1.w2)
			|| !ParseMark(' ')
			|| !ParseHexWord(rec.r1.w4)
		) {
			return false;
		}
		break;
	case 3:
		if (false
			|| !ParseHexWord(rec.r3.w2)
			|| !ParseMark(' ')
			|| !ParseHexWord(rec.r3.w4)
			|| !ParseMark(' ')
			|| !ParseHexWord(rec.r3.w6)
		) {
			return false;
		}
		break;
	case 4:
		if (false
			|| !ParseHexByte(rec.r4.b4)
			|| !ParseMark(' ')
			|| !ParseHexByte(rec.r4.b5)
			|| !ParseMark(' ')
			|| !ParseHexWord(rec.r4.w6[0])
			|| !ParseMark(' ')
			|| !ParseHexWord(rec.r4.w6[1])
			|| !ParseMark(' ')
			|| !ParseHexWord(rec.r4.w6[2])
			|| !ParseMark(' ')
			|| !ParseHexWord(rec.r4.w6[3])
			|| !ParseMark(' ')
			|| !ParseHexWord(rec.r4.w14)
		) {
			return false;
		}
		break;
	case 9:
		if (false
			|| !ParseHexWord(rec.r9.w4)
			|| !ParseMark(' ')
			|| !ParseHexByte(rec.r9.b6[0])
			|| !ParseMark(' ')
			|| !ParseHexByte(rec.r9.b6[1])
		) {
			return false;
		}
		break;
	case 14:
		if (false
			|| !ParseHexByte(rec.r14.b4[0])
			|| !ParseMark(' ')
			|| !ParseHexByte(rec.r14.b4[1])
			|| !ParseMark(' ')
			|| !ParseHexWord(rec.r14.w6)
		) {
			return false;
		}
		break;
	default:
		return false;
	}
	return true;
}

bool CDM2ItemListNodeX::ParseHex1(char c, BYTE &x)
{
	if ('0' <= c && c <= '9') {
		x = c - '0';
	} else if ('A' <= c && c <= 'F') {
		x = c - 'A' + 10;
	} else {
		return false;
	}
	return true;
}

bool CDM2ItemListNodeX::ParseHexByte(BYTE &x)
{
	BYTE a, b;
	if (false
		|| pszEnd < pszPos + 2
		|| !ParseHex1(pszPos[0], a)
		|| !ParseHex1(pszPos[1], b)
	) {
		return false;
	}
	x = (a << 4) | b;
	pszPos += 2;
	return true;
}

bool CDM2ItemListNodeX::ParseHexWord(WORD &x)
{
	BYTE a, b, c, d;
	if (false
		|| pszEnd < pszPos + 4
		|| !ParseHex1(pszPos[0], a)
		|| !ParseHex1(pszPos[1], b)
		|| !ParseHex1(pszPos[2], c)
		|| !ParseHex1(pszPos[3], d)
	) {
		return false;
	}
	x = (((((a << 4) | b) << 4) | c) << 4) | d;
	pszPos += 4;
	return true;
}

bool CDM2ItemListNodeX::Copy(CDM2ItemListNode *p, CString &strRet, bool fNoSib)
{
	strRet = MkText(p, 0, fNoSib);

	return true;
}

CDM2ItemListNodeX::Node_t *CDM2ItemListNodeX::Paste(CString strText)
{
	strText.Replace("\r\n", "\n");
	strText.Replace("\r", "\n");
	list<CString> listText;
	SplitStr(strText, CString("\n"), listText);
	return ParseText(listText.begin(), listText.end(), 0);
}

/////////////////////////////////////////////////////////////////////////////
// CDM2ItemListBox

CDM2ItemListBox::CDM2ItemListBox()
{
	m_pRoot = NULL;
	m_pNameRes = NULL;
}

CDM2ItemListBox::~CDM2ItemListBox()
{
	Close();
}

BEGIN_MESSAGE_MAP(CDM2ItemListBox, CListBox)
	//{{AFX_MSG_MAP(CDM2ItemListBox)
	ON_WM_CONTEXTMENU()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2ItemListBox メッセージ ハンドラ

void CDM2ItemListBox::Rebuild()
{
	{
		CCompatDC dc;
		dc.SelectStockObject(DEFAULT_GUI_FONT);
		dc.GetTextMetrics(&m_tm);
	}

	m_cyLine = 0 +m_tm.tmHeight +m_tm.tmInternalLeading +m_tm.tmExternalLeading;
	m_yLine = 0 +m_tm.tmInternalLeading;
	m_cyIcon = 32;

	m_vec.clear();
	m_vec.reserve(1000);

	Walk(m_pRoot, 0, vector<BOOL>());

	SetItemHeight(0, 4+_MAX(3*m_cyLine, m_cyIcon));

	int nOld = GetCount();
	int nNew = m_vec.size();

	if (nOld < nNew) {
		int n = nNew - nOld;
		for (; n != 0; n--) AddString(NULL);
	} else {
		int n = nOld - nNew, i = nOld - 1;
		for (; n != 0; n--, i--) DeleteString(i);
	}

	Invalidate();
}

void CDM2ItemListBox::Walk(CDM2ItemListNode *p, int iIndent, vector<BOOL> v)
{
	while (p) {
		Liso a;
		a.p = p;
		a.iIndent = iIndent;

		for (int i = 0; i < 32 && i < v.size(); i++) a.fSib[i] = v[i];

		a.fSib[iIndent] = p->pSib ? 1 : 0;

		m_vec.push_back(a);

		if (p->pSub) {
			v.push_back(p->pSib ? 1 : 0);
			Walk(p->pSub, iIndent +1, v);
			v.pop_back();
		}

		p = p->pSib;
	}
}

void CDM2ItemListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	UINT iID = lpDrawItemStruct->itemID;

	if (m_vec.size() <= iID) return;

	CRect rcorg = lpDrawItemStruct->rcItem;
	CRect rc(0, 0, rcorg.Width(), rcorg.Height());

	BOOL fSel = lpDrawItemStruct->itemState & ODS_SELECTED;

	CPoint cp = rc.CenterPoint();

	int iIndent = m_vec[iID].iIndent;
	BOOL *fSib = m_vec[iID].fSib;
	CDM2ItemListNode *p = m_vec[iID].p;

	CDC *pDC_org = CDC::FromHandle(lpDrawItemStruct->hDC);
	if (pDC_org == NULL) return;

	if (!Prepare_offscreen(rc.Size())) return;

	CCompatDC offscrDC(pDC_org);
	CDC *pDC = &offscrDC;
	HGDIOBJ hOLD_offscr = offscrDC.SelectObject(m_offscr);

	pDC->SelectStockObject(DEFAULT_GUI_FONT);

	pDC->FillSolidRect(rc, GetSysColor(fSel ? COLOR_HIGHLIGHT : COLOR_WINDOW));
	pDC->SetTextColor(GetSysColor(fSel ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT));
	pDC->SetBkColor(GetSysColor(fSel ? COLOR_HIGHLIGHT : COLOR_WINDOW));
	pDC->SetBkMode(TRANSPARENT);

	const int cxLineBoard = 16;

	{
		int m = pDC->GetROP2();
		pDC->SetROP2(R2_NOT);

		CRect rc2 = rc;

		rc2.right = rc2.left + cxLineBoard;

		int n = 1+iIndent;
		for (int x = 0; x < n; x++) {
			int lt = 0;
			if (x == n - 1) {
				if (fSib[x]) {
					lt = (x == 0) ? Lcs : Lpcs;
				} else {
					lt = (x == 0) ? Lc : Lpc;
				}
			} else {
				if (fSib[x]) {
					lt = Lps;
				} else {
					lt = L0;
				}
			}

			int f = 0;
			switch (lt) {
			case L0: f = 0; break;
			case Lc: f = 2; break;
			case Lcs: f = 2|4; break;
			case Lpcs: f = 1|2|4; break;
			case Lps: f = 1|4; break;
			case Lpc: f = 1|2; break;
			}

			CPoint pt = rc2.CenterPoint();

			if (f & 1) {
				pDC->MoveTo(pt.x, rc2.top);
				pDC->LineTo(pt.x, pt.y);
			}
			if (f & 2) {
				pDC->MoveTo(pt.x, pt.y);
				pDC->LineTo(rc2.right-1, pt.y);

				const int v1 = 3, v2 = v1;

				pDC->MoveTo(pt.x-v1, pt.y-v1); pDC->LineTo(pt.x+v2, pt.y-v1);
				pDC->MoveTo(pt.x+v1, pt.y-v1); pDC->LineTo(pt.x+v1, pt.y+v2);
				pDC->MoveTo(pt.x+v1, pt.y+v1); pDC->LineTo(pt.x-v2, pt.y+v1);
				pDC->MoveTo(pt.x-v1, pt.y+v1); pDC->LineTo(pt.x-v1, pt.y-v2);
			}
			if (f & 4) {
				pDC->MoveTo(pt.x, pt.y);
				pDC->LineTo(pt.x, rc2.bottom);
			}

			rc2.OffsetRect(cxLineBoard, 0);
		}

		pDC->SetROP2(m);
	}

	rc.OffsetRect(cxLineBoard * (1+iIndent), 0);

	switch (p->lnt) {
	case lntRoom:
		{
			CCompatDC dc(pDC);
			int vy = cp.y - 16;

			{
				CBMO *pbmo = GetApp()->FindBMO(4 * BMODID_TILEPORT0);
				if (pbmo != NULL) {
					SetDIBitsToDevice(*pDC, rc.left+1, vy, 32, 32, 0, 0, 0, 32, pbmo->pv, pbmo->pbmi, DIB_RGB_COLORS);
				}

				rc.OffsetRect(34, 0);
			}

			CString strText;
			strText.Format("ROOM\n(%3d,%2d,%2d)\n---\n"
				, 0 + p->ap.Z()
				, 0 + p->ap.Y()
				, 0 + p->ap.X()
				);
			pDC->DrawText(strText, rc, 0 |DT_LEFT |DT_TOP |DT_NOPREFIX);
			break;
		}
	case lntRecord:
		{
			CExtBM bm;
			CCompatDC dc(pDC);

			int vy;
			int t = -1;
			int tv = -1;
			bool fItem = false;

			int iDir = p->iDir & 3;

			BYTE iDB = p->iDB % 16;
			BYTE fakeDB = iDB;
			BYTE tourDB = -1;
			switch (iDB) {
			case  2:
				{
					t = (p->rec.r2.w2 / 8) & 255;
					break;
				}
			case  3:
				{
					break;
				}
			case  4:
				{
					t = p->rec.r4.b4;
					break;
				}
			case  5:
			case  6:
			case  7:
			case  8:
			case  9:
			case 10:
				{
					switch (iDB) {
					case  5:
					case  6:
					case 10:
						t = (p->rec.r5.w2 & 0x7F);
						break;
					case  7:
						t = 0;
						break;
					case  8:
						t = (p->rec.r8.w2 / 256) & 0x7F;
						break;
					case  9:
						t = CExp00069App::CHES2ID(p->rec.r9.w4);
						break;
					}
					fItem = true;
					break;
				}
			}

			vy = cp.y - 16;

			{
				CBMO *pbmo = GetApp()->FindBMO(4 * (BMODID_RP0 +iDir + ((fItem && !m_fTileIsWall && iIndent == 1) ? 0 : 4)));
				if (pbmo != NULL) {
					SetDIBitsToDevice(*pDC, rc.left+1, vy, 32, 32, 0, 0, 0, 32, pbmo->pv, pbmo->pbmi, DIB_RGB_COLORS);
				}
			}

			rc.OffsetRect(34, 0);

			if (t >= 0) {
				int k1 = -1, k2 = -1;
				switch (iDB) {
				case 2: {
					DM2TextRec_t tr;
					tr.Decode(p->rec.r2.w2);

					switch (tr.trpt) {
					case trptCreature: k1 = 0x0F; break;
					case trptOrnate:   k1 = m_fTileIsWall ? 0x09 : 0x0A; break;
					}

					break;
				}
				case 7: {
					k2 = iDB;
					t = 0;
					break;
				}
				case 4: k1 = 0x0F; break;
				case 5:
				case 6:
				case 8:
				case 9:
				case 10: k2 = iDB; break;
				}
				if (k1 >= 0) {
					GetApp()->RenderOrnate64To(pDC, rc.left+1, vy, k1, t, 32);
				}
				if (k2 >= 0) {
					GetApp()->RenderItem16To(pDC, rc.left+1, vy, k2, t, 32);
				}
			}

			rc.OffsetRect(34, 0);

			DM2Nameo o;
			if (m_pNameRes != NULL) {
				m_pNameRes->GetStr(iDB, m_fTileIsWall, p->rec, o);
			}

			CString strText;
			strText.Format("%s\n%s\n%s"
				, (LPCTSTR)o.text[0]
				, (LPCTSTR)o.text[1]
				, (LPCTSTR)o.text[2]
				);
			pDC->DrawText(strText, rc, 0 |DT_LEFT |DT_TOP |DT_NOPREFIX);
			break;
		}
	}

	pDC_org->BitBlt(rcorg.left, rcorg.top, rcorg.Width(), rcorg.Height(), pDC, 0, 0, SRCCOPY);

	offscrDC.SelectObject(hOLD_offscr);
}

CDM2ItemListNode *CDM2ItemListBox::GetSelNode(int iSel)
{
	if ((iSel = GetValCurSel(iSel)) < 0) return NULL;

	return m_vec[iSel].p;
}

BOOL CDM2ItemListBox::RemoveSelNode(int iSel)
{
	if ((iSel = GetValCurSel(iSel)) < 0) return FALSE;

	Remove1Node(GetSelNode(iSel));
	Rebuild();

	SendParentNotify(LBN_SELCHANGE);
	return TRUE;
}

void CDM2ItemListBox::Remove1Node(CDM2ItemListNode *pTar, CDM2ItemListNode *pWalk)
{
	if (pWalk == NULL)
		pWalk = m_pRoot;
	if (pWalk == NULL) return;

	while (pWalk != NULL) {
		if (pWalk->pSub == pTar) {
			CDM2ItemListNode *pNewSub = pWalk->pSub->pSib;
			pWalk->pSub->pSib = NULL;
			delete pWalk->pSub;
			pWalk->pSub = pNewSub;
		} else if (pWalk->pSub != NULL) {
			Remove1Node(pTar, pWalk->pSub);
		}
		if (pWalk->pSib == pTar) {
			CDM2ItemListNode *pNewSib = pTar->pSib;
			pTar->pSib = NULL;
			delete pTar;
			pWalk->pSib = pNewSib;
			pWalk = pNewSib;
		} else {
			pWalk = pWalk->pSib;
		}
	}
}

BOOL CDM2ItemListBox::CloneSelNode(int iSel)
{
	if ((iSel = GetValCurSel(iSel)) < 0) return FALSE;

	Clone1Node(GetSelNode(iSel));
	Rebuild();

	SendParentNotify(LBN_SELCHANGE);
	return TRUE;
}

void CDM2ItemListBox::Clone1Node(CDM2ItemListNode *pWhat, CDM2ItemListNode *pWalk)
{
	if (pWalk == NULL) pWalk = m_pRoot;
	if (pWalk == NULL) return;

	while (pWalk != NULL) {
		if (pWalk->pSub == pWhat) {
			CDM2ItemListNode *pNewSib = pWalk->pSub->CloneNoSib();
			pNewSib->pSib = pWalk->pSub;
			pWalk->pSub = pNewSib;
		} else if (pWalk->pSub != NULL) {
			Clone1Node(pWhat, pWalk->pSub);
		}
		if (pWalk->pSib == pWhat) {
			CDM2ItemListNode *pNewSib = pWalk->pSib->CloneNoSib();
			pNewSib->pSib = pWalk->pSib;
			pWalk->pSib = pNewSib;
			if (pWalk->pSib)
				pWalk = pWalk->pSib;
		}
		pWalk = pWalk->pSib;
	}
}

BOOL CDM2ItemListBox::InsertNode(CDM2ItemListNode *pWhat, int iSel, CDM2ListNodeAddWay lnaw)
{
	if (pWhat == NULL) return FALSE;

	ASSERT(pWhat->pSib == NULL);

	BOOL r = Insert1Node(pWhat, GetSelNode(iSel), m_pRoot, lnaw);

	Rebuild();

	return r;
}

BOOL CDM2ItemListBox::Insert1Node(CDM2ItemListNode *pWhat, CDM2ItemListNode *pWhere, CDM2ItemListNode *pWalk, CDM2ListNodeAddWay lnaw)
{
	if (pWhere == NULL) return FALSE;

	if (pWhere == m_pRoot) {
		Insert1Node_Sib(pWhat, pWhere, lnaw, true);
		return TRUE;
	}

	while (pWalk != NULL) {
		if (pWalk->pSub == pWhere) {
			Insert1Node_Sub(pWhat, pWalk, lnaw);
			return TRUE;
		} else if (pWalk->pSub != NULL) {
			if (Insert1Node(pWhat, pWhere, pWalk->pSub, lnaw))
				return TRUE;
		}
		if (pWalk->pSib == pWhere) {
			Insert1Node_Sib(pWhat, pWalk, lnaw);
			return TRUE;
		} else {
			pWalk = pWalk->pSib;
		}
	}
	return FALSE;
}

void CDM2ItemListBox::Insert1Node_Sub(CDM2ItemListNode *pWhat, CDM2ItemListNode *pWalk, CDM2ListNodeAddWay lnaw)
{
	switch (lnaw) {
	case lnawAppend:
		{
			if (pWalk->pSub == NULL) {
				pWalk->pSub = pWhat;
			} else {
				pWalk->pSub->AddSibNode(pWhat);
			}
			break;
		}
	case lnawInsertBefore:
		{
			pWhat->AttachSibNode(pWalk->pSub);
			pWalk->pSub = pWhat;
			break;
		}
	case lnawInsertAfter:
		{
			if (pWalk->pSub == NULL) {
				pWalk->pSub = pWhat;
			} else {
				pWalk->pSub->JoinSibNode(pWhat, 0);
			}
			break;
		}
	case lnawSubFirst:
		{
			pWalk = pWalk->pSub;

			if (pWalk->pSub == NULL) {
				pWalk->pSub = pWhat;
			} else {
				pWhat->AttachSibNode(pWalk->pSub);
				pWalk->pSub = pWhat;
			}
			break;
		}
	case lnawSubLast:
		{
			pWalk = pWalk->pSub;

			if (pWalk->pSub == NULL) {
				pWalk->pSub = pWhat;
			} else {
				pWalk->pSub->AttachSibNode(pWhat);
			}
			break;
		}
	default:
		{
			ASSERT(FALSE);
		}
	}
}

void CDM2ItemListBox::Insert1Node_Sib(CDM2ItemListNode *pWhat, CDM2ItemListNode *pWalk, CDM2ListNodeAddWay lnaw, bool fNoSib)
{
	switch (lnaw) {
	case lnawAppend:
		{
			if (fNoSib) {
				pWalk->AddSubNode(pWhat);
			} else {
				pWalk->AttachSibNode(pWhat);
			}
			break;
		}
	case lnawInsertAfter:
		{
			if (fNoSib) {
				pWalk->AddSubNode(pWhat);
			} else {
				pWalk->JoinSibNode(pWhat, 1);
			}
			break;
		}
	case lnawInsertBefore:
		{
			if (fNoSib) {
				pWalk->AddSubNode(pWhat);
			} else {
				pWalk->JoinSibNode(pWhat, 0);
			}
			break;
		}
	case lnawSubFirst:
		{
			if (!fNoSib) pWalk = pWalk->pSib;

			if (pWalk->pSub == NULL) {
				pWalk->pSub = pWhat;
			} else {
				pWhat->AttachSibNode(pWalk->pSub);
				pWalk->pSub = pWhat;
			}
			break;
		}
	case lnawSubLast:
		{
			if (!fNoSib) pWalk = pWalk->pSib;

			if (pWalk->pSub == NULL) {
				pWalk->pSub = pWhat;
			} else {
				pWalk->pSub->AttachSibNode(pWhat);
			}
			break;
		}
	default:
		{
			ASSERT(FALSE);
		}
	}
}

BOOL CDM2ItemListBox::CopyNode(CDM2ListNodeCopyWay lncw, int iSel)
{
	if ((iSel = GetValCurSel(iSel)) < 0) return FALSE;

	bool fNoSib = (lncw == lncwSub);

	CDM2ItemListNode *pNode = GetSelNode(iSel);

	if (!fNoSib && pNode != NULL) pNode = pNode->pSub;

	CDM2ItemListNodeX x;
	CString strText;
	VERIFY(x.Copy(pNode, strText, fNoSib));
	CStr2Clipbrd z;
	VERIFY(z.SetClipboardTextData2(strText));

	return FALSE;
}

BOOL CDM2ItemListBox::PasteNode(CDM2ListNodePasteWay lnpw, int iSel)
{
	CDM2ItemListNode *pNode;
	if ((pNode = GetSelNode()) == NULL) return FALSE;

	CString strText;
	CStr2Clipbrd z;
	if (!z.GetClipboardTextData2(strText)) return FALSE;
	CDM2ItemListNodeX x;
	CDM2ItemListNode *pNew = x.Paste(strText);
	if (pNew == NULL) return FALSE;

	switch (lnpw) {
	case lnpwAtFirstInSub:
		Insert1Node(pNew, pNode, m_pRoot, lnawSubFirst);
		pNew = NULL;
		break;
	case lnpwAtLastInSub:
		Insert1Node(pNew, pNode, m_pRoot, lnawSubLast);
		pNew = NULL;
		break;
	}
	if (pNew != NULL) {
		switch (lnpw) {
		case lnpwAtFirst:
			ASSERT(FALSE); break;
		case lnpwAtLast:
			ASSERT(FALSE); break;
		case lnpwBefore:
			Insert1Node(pNew, pNode, m_pRoot, lnawInsertBefore);
			pNew = NULL;
			break;
		case lnpwAfter:
			Insert1Node(pNew, pNode, m_pRoot, lnawInsertAfter);
			pNew = NULL;
			break;
		}
	}
	if (pNew != NULL) delete pNew;

	Rebuild();
	return TRUE;
}

void CDM2ItemListBox::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CDM2ItemListNode *pNode;
	if ((pNode = GetSelNode()) == NULL) return;

	int iSub = 0;
	if (pNode->lnt == lntRecord) {
		iSub = 2;
		if (pNode->IsContainer())
			iSub = 1;
	}

	CMenu a;
	a.LoadMenu(IDR_DM2ITEMLIST);
	CMenu *p = a.GetSubMenu(iSub);
	if (p) {
		p->TrackPopupMenu(0 |TPM_RIGHTBUTTON, point.x, point.y, GetParent());
	}
	
}

bool CDM2ItemListBox::Prepare_offscreen(CSize size)
{
	BITMAP bm;
	if (m_offscr.GetBitmap(bm) == sizeof(BITMAP)) {
		if (size.cx <= bm.bmWidth && size.cy <= bm.bmHeight)
			return true;
	}

	m_offscr.DeleteObject();

	if (!m_offscr.Create(size, 24, NULL))
		return false;

	return true;
}

BOOL CDM2ItemListBox::OnEraseBkgnd(CDC* pDC) 
{
	CRect rc;
	GetClientRect(&rc);
	CRect rcItem;
	GetItemRect(GetCount() -1, &rcItem);

	if (rcItem.bottom < rc.bottom) {
		pDC->FillSolidRect(
			rc.left, 
			rcItem.bottom, 
			rc.right -rc.left, 
			rc.bottom -rcItem.bottom,
			GetSysColor(COLOR_WINDOW)
			);
	}

	return true;
//	return CListBox::OnEraseBkgnd(pDC);
}
