
#include "StdAfx.h"
#include "DM2TextEncImpl.h"
#include "DM2TTbl.h"

namespace
{

CDM2TextSearchTri g_mapTextEn(g_pszTextTblE0);
CDM2TextSearchTri g_mapTextJa(g_pszTextTblJ0, g_pszTextTblJ1);

};

//////////////////////////////////////////////////////////////////////
// CDM2TextSearchTri

CDM2TextSearchTri::CDM2TextSearchTri(LPCSTR *pEn0)
{
	m_pTree = new TREE();

	UINT i;
	for (i = 0; i < 256; i++) Add(pEn0[i], 0x0000 +i);
}

CDM2TextSearchTri::CDM2TextSearchTri(LPCSTR *pJp0, LPCSTR *pJp1)
{
	m_pTree = new TREE();

	UINT i;
	for (i = 0; i < 256; i++) Add(pJp0[i], 0x0000 +i);
	for (i = 0; i < 256; i++) Add(pJp1[i], 0x0100 +i);
}

CDM2TextSearchTri::~CDM2TextSearchTri()
{
	delete m_pTree;
}

bool CDM2TextSearchTri::Add(LPCSTR pszPos, LPCSTR pszEnd, WORD v)
{
	TREE *pTree = m_pTree;
	while (pszPos != pszEnd) {
		BYTE x = *pszPos;
		UINT iChrHi = x / 16;
		UINT iChrLo = x % 16;
		pTree = WalkTree(pTree, iChrHi, true);
		pTree = WalkTree(pTree, iChrLo, true);
		pszPos++;
	}
	if (pTree != NULL && pTree->v.IsOk() == false) {
		pTree->v = ELEM(v);
	}
	return false;
}

CDM2TextSearchTri::TREE *CDM2TextSearchTri::WalkTree(CDM2TextSearchTri::TREE *pTree, BYTE c, bool fAdd)
{
	if (!pTree) return NULL;

	TREE *&ppTree = pTree->p[c % 16];
	if (ppTree == NULL && fAdd) {
		ppTree = new TREE();
	}
	return ppTree;
}

bool CDM2TextSearchTri::Match(LPCSTR &pszBegin, LPCSTR pszEnd, UINT &v)
{
	TREE *pTreeNext = m_pTree;
	TREE *pTree;

	LPCSTR pszPos = pszBegin;

	while (pszPos != pszEnd) {
		pTree = pTreeNext;
		BYTE x = *pszPos;
		UINT iChrHi = (x) / 16;
		UINT iChrLo = (x) % 16;
		pTreeNext = WalkTree(pTreeNext, iChrHi, false);
		pTreeNext = WalkTree(pTreeNext, iChrLo, false);

		if (pTreeNext == NULL) break;

		pszPos++;
	}

	if (pTreeNext == NULL) pTreeNext = pTree;

	if (pTreeNext != m_pTree && pTreeNext->v.IsOk()) {
		v = pTreeNext->v.v;
		pszBegin = pszPos;
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
// CDM2TextEncoderBase

bool CDM2TextEncoderBase::Encode(LPCSTR pszPos, LPCSTR pszEnd, WIo &wio, TEncBase &rTE)
{
	CDM2TextSearchTri &rTri = GetSearchTri();

	while (pszPos != pszEnd) {
		UINT nVal = '?';
		if (!rTri.Match(pszPos, pszEnd, nVal)) {
			pszPos++;
		}
		if (nVal & 0x0100) {
			// Use tbl2
			if (!wio.WriteByte(rTE.Encode(0xF0, true))) return false;
		} else {
			// Use tbl1
		}
		if (!wio.WriteByte(rTE.Encode(nVal & 0xFF, true))) return false;
	}
	return true;
}

bool CDM2TextEncoderBase::Encode(LPCSTR pszPos, LPCSTR pszEnd, SizeBuff &fv, TEncBase &rTE)
{
	if (pszEnd == NULL) pszEnd = strchr(pszPos, 0);

	BYTE cTemp[1024];
	MemWIo wio(cTemp, cTemp + sizeof(cTemp));
	if (Encode(pszPos, pszEnd, wio, rTE)) {
		if (fv.Alloc(wio.GetPosition(), cTemp))
			return true;
	}
	return false;
}

bool CDM2TextEncoderBase::Decode(LPCSTR pszPos, LPCSTR pszEnd, WIo &wio, TDecBase &rTD)
{
	LPCSTR *pszTbl1 = GetDecodeTbl1();
	LPCSTR *pszTbl2 = GetDecodeTbl2();
	while (pszPos < pszEnd) {
		BYTE v1 = rTD.Decode(*pszPos, true);
		if (v1 == 0xF0 && pszTbl2 != NULL) {
			// Use tbl2
			pszPos++;
			BYTE v2 = rTD.Decode(*pszPos, true);
			if (!wio.WriteString(pszTbl2[v2])) return false;
		} else {
			// Use tbl1
			if (!wio.WriteString(pszTbl1[v1])) return false;
		}
		pszPos++;
	}
	return true;
}

bool CDM2TextEncoderBase::Decode(LPCSTR pszPos, LPCSTR pszEnd, CString &strText, TDecBase &rTD)
{
	BYTE cTemp[1024];
	MemWIo wio(cTemp, cTemp + sizeof(cTemp));
	if (Decode(pszPos, pszEnd, wio, rTD)) {
		strText = CString((LPCSTR)cTemp, wio.GetPosition());
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
// CDM2TextEncoderEn

CDM2TextEncoderEn::CDM2TextEncoderEn(bool fPlain): m_fPlain(fPlain)
{

}

bool CDM2TextEncoderEn::Encode(LPCSTR pszPos, LPCSTR pszEnd, SizeBuff &fv)
{
	return CDM2TextEncoderBase::Encode(
		pszPos, 
		pszEnd, 
		fv, 
		m_fPlain ? (TEncBase &)TEncNo() : (TEncBase &)TEncDM2()
		);
}

bool CDM2TextEncoderEn::Decode(LPCSTR pszPos, LPCSTR pszEnd, CString &strText)
{
	return CDM2TextEncoderBase::Decode(
		pszPos, 
		pszEnd, 
		strText, 
		m_fPlain ? (TDecBase &)TDecNo() : (TDecBase &)TDecDM2()
		);
}

CDM2TextSearchTri &CDM2TextEncoderEn::GetSearchTri()
{
	return g_mapTextEn;
}

LPCSTR *CDM2TextEncoderEn::GetDecodeTbl1() { return g_pszTextTblE0; }

LPCSTR *CDM2TextEncoderEn::GetDecodeTbl2() { return NULL; }

//////////////////////////////////////////////////////////////////////
// CDM2TextEncoderJa

CDM2TextEncoderJa::CDM2TextEncoderJa()
{

}

bool CDM2TextEncoderJa::Encode(LPCSTR pszPos, LPCSTR pszEnd, SizeBuff &fv)
{
	return CDM2TextEncoderBase::Encode(
		pszPos, 
		pszEnd, 
		fv, 
		TEncDM2()
		);
}

bool CDM2TextEncoderJa::Decode(LPCSTR pszPos, LPCSTR pszEnd, CString &strText)
{
	return CDM2TextEncoderBase::Decode(
		pszPos, 
		pszEnd, 
		strText, 
		TDecDM2()
		);
}

CDM2TextSearchTri &CDM2TextEncoderJa::GetSearchTri()
{
	return g_mapTextJa;
}

LPCSTR *CDM2TextEncoderJa::GetDecodeTbl1() { return g_pszTextTblJ0; }

LPCSTR *CDM2TextEncoderJa::GetDecodeTbl2() { return g_pszTextTblJ1; }

//////////////////////////////////////////////////////////////////////
// CDM2TextEncoderJa_PSV

LPCSTR *CDM2TextEncoderJa_PSV::GetDecodeTbl1() { return g_pszTextTblJ0_PSV; }

LPCSTR *CDM2TextEncoderJa_PSV::GetDecodeTbl2() { return g_pszTextTblJ1_PSV; }
