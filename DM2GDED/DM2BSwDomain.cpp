// DM2BSwDomain.cpp: CDM2BSwDomain クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2BSwDomain.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CDM2BSwDomain

bool CDM2BSwDomain::Parse(LPCSTR &pszPos, LPCSTR pszEnd)
{
	m_pszPos = pszPos;
	m_pszEnd = pszEnd;

	while (m_pszPos < m_pszEnd) {
		BYTE x1, x2;
		if (ParseHxx()) {
			// $xx
			FillMemory(m_bDomain, sizeof(m_bDomain), -1);
		} else if (ParseHvv(x1)) {
			if (m_pszPos[0] == '-') {
				m_pszPos++;
				if (!ParseHvv(x2)) return false;
				// $??-$??
				SetRange(x1, x2);
			} else {
				// $??
				Set(x1);
			}
		}
		if (m_pszPos < m_pszEnd) {
			if (m_pszPos[0] != ';')
				return false;
			m_pszPos++;
		}
	}
	return true;
}
