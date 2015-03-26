// DM2PIPDescList.cpp: CDM2PIPDescList クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2PIPDescList.h"
#include "CSVrw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace
{

bool ReadHex(LPCSTR psz, int &x)
{
	LPSTR pszEnd = NULL;
	x = strtol(psz, &pszEnd, 16);
	if (pszEnd[0] != 0) return false;
	return true;
}

bool ReadInt(LPCSTR psz, int &x)
{
	LPSTR pszEnd = NULL;
	x = strtol(psz, &pszEnd, 10);
	if (pszEnd[0] != 0) return false;
	return true;
}

bool ReadGDEntry(LPCSTR psz, CGDEntry &pos)
{
	if (false
		|| psz[0] != '$'
		|| !CDM2BSwDomain::HexHexToVal(&psz[ 1], pos.c[0])
		|| !CDM2BSwDomain::HexHexToVal(&psz[ 3], pos.c[1])
		|| !CDM2BSwDomain::HexHexToVal(&psz[ 5], pos.c[2])
		|| !CDM2BSwDomain::HexHexToVal(&psz[ 7], pos.c[3])
		|| !CDM2BSwDomain::HexHexToVal(&psz[ 9], pos.c[4])
		|| !CDM2BSwDomain::HexHexToVal(&psz[11], pos.c[5])
	) {
		return false;
	}
	return true;
}

};

//////////////////////////////////////////////////////////////////////
// CDM2PIPDesc

bool CDM2PIPDesc::Parse(CString *(pstr[]))
{
	if (false
		|| !m_aDomain[0].Parse(*pstr[0])
		|| !m_aDomain[1].Parse(*pstr[1])
		|| !m_aDomain[2].Parse(*pstr[2])
	) {
		return false;
	}
	for (int i = 3; pstr[i] != NULL; i++) {
		if (!ParseTS(*pstr[i]))
			return false;
	}
	if (m_ts == piptsNo)
		return false;
	return true;
}

bool CDM2PIPDesc::ParseTS(LPCSTR psz)
{
	if (!psz || !*psz) return true;

	if (strncmp(psz, "S:", 2) == 0 && ReadInt(&psz[2], m_nScene)) {
		m_ts = piptsScene;
		return true;
	}
	if (strncmp(psz, "COLOR_KEY:", 10) == 0 && ReadInt(&psz[10], m_nColorKey)) {
		return true;
	}
	if (strncmp(psz, "COLOR_KEY2:", 11) == 0 && ReadInt(&psz[11], m_nColorKey2)) {
		return true;
	}
	if (strcmp(psz, "INTERFACE_PALETTE") == 0) {
		m_ts = piptsIf;
		return true;
	}
	if (strncmp(psz, "COLOR_PALETTE:", 14) == 0 && ReadGDEntry(&psz[14], m_pos)) {
		m_ts = piptsColorPal;
		return true;
	}
	if (strcmp(psz, "S_DEPEND") == 0) {
		m_ts = piptsScene;
		m_fDepend = true;
		return true;
	}
	return false;
}

bool CDM2PIPDesc::Match(CGDEntry pos) const
{
	if (true
		&& pos.c3() == 0x01
		&& m_aDomain[0].IsSet(pos.c1())
		&& m_aDomain[1].IsSet(pos.c2())
		&& m_aDomain[2].IsSet(pos.c4())
	)  {
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
// CDM2PIPDescList

void CDM2PIPDescList::RemoveAll()
{
	m_list.clear();
}

bool CDM2PIPDescList::LoadFrom(LPCTSTR pszFileIn)
{
	if (!pszFileIn) return true;

	CSVR_t f;
	while (true) {
		if (!f.Open(pszFileIn))
			break;
		while (f.ReadNextLine()) {
			CString str[16];
			CString *(pstr[16]);
			int i;
			for (i = 0; i < 16; i++) {
				if (!f.ReadNextToken(str[i]))
					break;
				pstr[i] = &str[i];
			}
			pstr[i] = NULL;
			if (i < 3)
				continue;
			CDM2PIPDesc a;
			if (!a.Parse(pstr))
				continue;
			m_list.push_back(a);
		}
		return true;
	}
	return false;
}

const CDM2PIPDesc *CDM2PIPDescList::FindFirstMatch(const CGDEntry &pos) const
{
	List::const_iterator
		iterPos = m_list.begin(),
		iterEnd = m_list.end();
	for (; iterPos != iterEnd; iterPos++) {
		if (iterPos->Match(pos))
			return &*iterPos;
	}
	return NULL;
}
