// DM2BSwDomain.h: CDM2BSwDomain クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM2BSWDOMAIN_H__C6F27B40_83AB_423F_B223_60E43E11ED0F__INCLUDED_)
#define AFX_DM2BSWDOMAIN_H__C6F27B40_83AB_423F_B223_60E43E11ED0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDM2BSwDomain  
{
	// 
	BYTE m_bDomain[32];

	// 
	LPCSTR m_pszPos, m_pszEnd;

	// 
	bool ParseHxx()
	{
		if (true
			&& m_pszPos + 3 <= m_pszEnd
			&& m_pszPos[0] == '$'
			&& tolower(m_pszPos[1]) == 'x'
			&& tolower(m_pszPos[2]) == 'x'
		) {
			m_pszPos += 3;
			return true;
		}
		return false;
	}
	// 
	bool ParseHvv(BYTE &x)
	{
		BYTE bH, bL;
		if (true
			&& m_pszPos + 3 <= m_pszEnd
			&& HexToVal(m_pszPos[1], bH)
			&& HexToVal(m_pszPos[2], bL)
		) {
			m_pszPos += 3;
			x = (bH << 4) | bL;
			return true;
		}
		return false;
	}

public:
	// 
	CDM2BSwDomain()
	{
		ZeroMemory(m_bDomain, sizeof(m_bDomain));
	}
	// 
	void Set(BYTE x)
	{
		m_bDomain[x / 8] |= 1 << (x % 8);
	}
	// 
	void SetRange(BYTE x, BYTE y)
	{
		for (; x <= y; x++) Set(x);
	}
	// 
	bool IsSet(BYTE x) const
	{
		return (m_bDomain[x / 8] & (1 << (x % 8))) ? true : false;
	}
	// 
	bool Parse(LPCSTR &pszPos, LPCSTR pszEnd);
	// 
	bool Parse(LPCSTR pszPos)
	{
		return Parse(pszPos, _tcschr(pszPos, 0));
	}

	// 
	static bool HexToVal(BYTE x, BYTE &v)
	{
		if ('0' <= x && x <= '9') {
			v = x - '0';
			return true;
		}
		x = (BYTE)tolower(x);
		if ('a' <= x && x <= 'f') {
			v = x - 'a' + 10;
			return true;
		}
		return false;
	}
	// 
	static bool HexHexToVal(LPCSTR psz, BYTE &v)
	{
		BYTE iH, iL;
		if (!HexToVal(psz[0], iH) || !HexToVal(psz[1], iL))
			return false;
		v = (iH << 4) | iL;
		return true;
	}

};

#endif // !defined(AFX_DM2BSWDOMAIN_H__C6F27B40_83AB_423F_B223_60E43E11ED0F__INCLUDED_)
