// DMObTreeData.cpp: CDMObTreeData クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "exp00069.h"
#include "DMObTreeData.h"
#include "SplitStr.h"
#include "DMRecFmt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CDMObTreeData

CDMObTreeData::CDMObTreeData()
{
	pRoot = NULL;
}

CDMObTreeData::~CDMObTreeData()
{
	Close();
}

bool CDMObTreeData::Create(CDDAT &rDAT, RN rn)
{
	pDAT = &rDAT;
	aLoop = CDMDBRMask();

	pRoot = Conv(rn);
	return true;
}

CDMObTreeData::Node *CDMObTreeData::Conv(RN rn)
{
	if (rn == RN_END || rn == RN_NIL)
		return NULL;
	if (aLoop.IsSet(rn))
		return NULL;
	aLoop.Set(rn);

	HRESULT hr;
	DMDBU rec;
	if (SUCCEEDED(hr = pDAT->GetDBRecAt(DBIPos_t(rn), rec))) {
		Node *p = new Node();
		p->iDB = rn.db();
		p->iDir = rn.room();
		p->rec = rec;
		switch (rn.db()) {
		case 4:
		case 9:
		case 14:
			p->pSub = Conv(p->rec.r4.stolen);
		}
		p->pSib = Conv(p->rec.r0.next);
		return p;
	}
	return NULL;
}

void CDMObTreeData::Close()
{
	if (pRoot) {
		delete pRoot;
		pRoot = NULL;
	}
}

CString CDMObTreeData::Mk()
{
	return MkNode(pRoot, 0);
}

CString CDMObTreeData::MkNode(Node *p, int i)
{
	if (p) {
		CString strRes;
		for (int s = i; s != 0; s--) strRes += '\t';
		strRes += CDMRecFmt::Format(p->iDB, p->iDir, p->rec, false) + "\r\n";
		if (p->pSub) strRes += MkNode(p->pSub, i +1);
		if (p->pSib) strRes += MkNode(p->pSib, i);
		return strRes;
	}
	return "";
}

bool CDMObTreeData::CreateFromText(LPCTSTR psz)
{
	Close();
	CString strText = psz;
	strText.Replace("\r\n", "\n");
	strText.Replace("\r", "\n");
	list<CString> listText;
	SplitStr(strText, CString('\n'), listText);
	pRoot = ConvText(listText.begin(), listText.end(), 0);
	return pRoot != NULL;
}

CDMObTreeData::Node *CDMObTreeData::ConvText(list<CString>::iterator &iterPos, list<CString>::iterator iterEnd, int iDepth)
{
	if (iterPos == iterEnd) return NULL;
	m_pszPos = *iterPos;
	m_pszEnd = strchr(m_pszPos, 0);
	while (true) {
		int c = iDepth;
		for (; c != 0 && m_pszPos < m_pszEnd; c--, m_pszPos++);
		int iDir;
		int iDB;
		if (!ParseDir(iDir)) break;
		if (!ParseMark(':')) break;
		if (!ParseDBName(iDB)) break;
		if (!ParseMark(':')) break;
		DMDBU rec;
		if (!ParseRec(iDB, rec)) break;
		iterPos++;
		Node *pSub = ConvText(iterPos, iterEnd, iDepth +1);
		Node *pSib = ConvText(iterPos, iterEnd, iDepth);
		Node *p = new Node();
		p->iDB = (BYTE)iDB;
		p->iDir = (BYTE)iDir;
		p->rec = rec;
		p->pSib = pSib;
		p->pSub = pSub;
		return p;
	}
	return NULL;
}

bool CDMObTreeData::ParseDir(int &iDir)
{
	switch (*m_pszPos) {
	case 'N': iDir = 0; break;
	case 'E': iDir = 1; break;
	case 'S': iDir = 2; break;
	case 'W': iDir = 3; break;
	default:
		return false;
	}
	m_pszPos++;
	return true;
}

bool CDMObTreeData::ParseMark(char c)
{
	if (m_pszPos >= m_pszEnd)
		return false;
	if (c != *m_pszPos)
		return false;
	m_pszPos++;
	return true;
}

bool CDMObTreeData::ParseDBName(int &iDB)
{
	if (m_pszPos + 4 >= m_pszEnd)
		return false;
	int i = 0;
	for (; i < 16; i++) {
		if (strncmp(m_pszPos, CDMRecFmt::GetTblName(i, false), 4) == 0) {
			iDB = i;
			m_pszPos += 4;
			break;
		}
	}
	iDB = i;
	return (i != 16);
}

bool CDMObTreeData::ParseRec(int iDB, DMDBU &rec)
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

bool CDMObTreeData::ParseHex1(char c, BYTE &x)
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

bool CDMObTreeData::ParseHexByte(BYTE &x)
{
	BYTE a, b;
	if (false
		|| m_pszEnd < m_pszPos + 2
		|| !ParseHex1(m_pszPos[0], a)
		|| !ParseHex1(m_pszPos[1], b)
	) {
		return false;
	}
	x = (a << 4) | b;
	m_pszPos += 2;
	return true;
}

bool CDMObTreeData::ParseHexWord(WORD &x)
{
	BYTE a, b, c, d;
	if (false
		|| m_pszEnd < m_pszPos + 4
		|| !ParseHex1(m_pszPos[0], a)
		|| !ParseHex1(m_pszPos[1], b)
		|| !ParseHex1(m_pszPos[2], c)
		|| !ParseHex1(m_pszPos[3], d)
	) {
		return false;
	}
	x = (((((a << 4) | b) << 4) | c) << 4) | d;
	m_pszPos += 4;
	return true;
}

bool CDMObTreeData::InsertTo(CDDAT &rDAT, RN rn, RN &rnAppended)
{
	m_pDAT = &rDAT;
	rnAppended = MkRec(pRoot, rn);
	return (rnAppended != RN_NIL);
}

RN CDMObTreeData::MkRec(Node *p, RN rnLast)
{
	if (p == NULL) return RN_END;
	DBIPos_t pos;
	HRESULT hr;
	RN rn;

	switch (p->iDB) {
	case 4:
	case 9:
	case 14:
		rn = p->rec.r4.stolen = MkRec(p->pSub, RN_END);
		if (rn == RN_NIL) return RN_NIL;
		break;
	}

	rn = MkRec(p->pSib, rnLast);
	if (rn == RN_NIL) return RN_NIL;
	if (rn == RN_END) rn = rnLast;
	p->rec.r0.next = rn;

	if (FAILED(hr = m_pDAT->AddDBRecToEmpty(p->iDB, p->rec, pos)))
		return RN_NIL;

	rn = RNco(pos.db(), pos.idx(), p->iDir);
	return rn;
}
