// DM2RTextToken.cpp: CDM2RTextToken クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2RTextToken.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const DM2RTTable _RTTableWide[] = {
	{'W',0x10,  0,127,"Weap",rttWeapons,},
	{'A',0x11,128,255,"Clot",rttClothes,},
	{'J',0x15,256,383,"Misc",rttMisc,},
	{'P',0x13,384,431,"Poti",rttPotions,},
	{'C',0x14,480,507,"Ches",rttChests,},
	{'S',0x12,508,508,"Scro",rttScrolls,},
	{  0,   0,  0,  0, NULL,          0,},
};
const DM2RTTable _RTTableCreature[] = {
	{'C',0x0F,  0,127,"Mons",rttCreatures,},
	{  0,   0,  0,  0, NULL,            0,},
};

//////////////////////////////////////////////////////////////////////
// CDM2RTTokenList

bool CDM2RTTokenList::Parse(LPCTSTR pszText)
{
	ZeroMemory(al, sizeof(al));

	BYTE c;
	int valTo = 0, valFrom = -1;
	int baseVal = -1;
	bool fill = false;

	do {
		c = *(pszText++);

		if ('0' <= c && c <= '9') {
			valTo = 10 * valTo +(c -'0');
			fill = true;
			continue;
		}
		if (c == '-') {
			valFrom = valTo;
			valTo = 0;
			continue;
		}
		if (fill) {
			if (valFrom < 0) valFrom = valTo;

			for (; valFrom <= valTo; valFrom++) {
				UINT val = (baseVal + valFrom);
				al[val / 8] |= 1 << (val % 8);
			}

			valTo = 0;
			valFrom = -1;
			baseVal = -1;
			fill = false;
		}
		switch (c) {
		case 'W': baseVal = 0; break;
		case 'A': baseVal = 128; break;
		case 'J': baseVal = 256; break;
		case 'P': baseVal = 384; break;
		case 'C': baseVal = (forCreature) ? 0 : 480; break;
		case 'S': baseVal = 508; break;
		}
	} while (isalnum(c) || isspace(c) || c == '-');

	return true;
}

CString CDM2RTTokenList::ToStr() const
{
	const DM2RTTable *pt = (forCreature ? _RTTableCreature : _RTTableWide);

	CString strRet;

	for (; pt->cls != 0; pt++) {
		WORD x0 = pt->valFrom;
		WORD x1 = pt->valTo;

		WORD nLastPos;
		bool fCur;

		for (WORD x = x0; x <= x1; x++) {
			bool fNew = (al[x / 8] & (1 << (x % 8))) != 0;
			if (x == x0) {
				fCur = fNew;
				nLastPos = x;
			}
			if (x == x1 || fCur != fNew) {
				if (fCur) {
					CString strTemp; strTemp.Format(
						(nLastPos +1 < x) ? "%c%u-%u" : "%c%u"
						, pt->cls
						, 0U +nLastPos -x0
						, 0U +x -1 -x0
						);
					if (strRet.GetLength() != 0)
						strRet += " ";

					strRet += strTemp;
				}

				fCur = fNew;
				nLastPos = x;
			}
		}
	}

	return strRet;
}

//////////////////////////////////////////////////////////////////////
// CDM2ItemSelList

bool CDM2ItemSelList::Parse(LPCTSTR pszText)
{
	sels.clear();

	BYTE c;
	int valTo = 0, valFrom = -1;
	int baseVal = -1;
	bool fill = false;

	do {
		c = *(pszText++);

		if ('0' <= c && c <= '9') {
			valTo = 10 * valTo +(c -'0');
			fill = true;
			continue;
		}
		if (c == '-') {
			valFrom = valTo;
			valTo = 0;
			continue;
		}
		if (fill) {
			if (valFrom < 0) valFrom = valTo;

			for (; valFrom <= valTo; valFrom++) {
				UINT val = (baseVal + valFrom);
				sels.push_back(val);
			}

			valTo = 0;
			valFrom = -1;
			baseVal = -1;
			fill = false;
		}
		switch (c) {
		case 'W': baseVal = 0; break;
		case 'A': baseVal = 128; break;
		case 'J': baseVal = 256; break;
		case 'P': baseVal = 384; break;
		case 'C': baseVal = (forCreature) ? 0 : 480; break;
		case 'S': baseVal = 508; break;
		}
	} while (isalnum(c) || isspace(c) || c == '-');

	return true;
}

CString CDM2ItemSelList::ToStr() const
{
	const DM2RTTable *pt = (forCreature ? _RTTableCreature : _RTTableWide);

	CString strRet;

	size_t x;

	class Ut {
	public:
		static const DM2RTTable *Find(const DM2RTTable *pt, UINT v) {
			while (pt->cls != 0) {
				if (pt->valFrom <= v && v <= pt->valTo)
					return pt;
				++pt;
			}
			return NULL;
		}
	};

	for (x = 0; x < sels.size(); ) {
		UINT v0 = sels[x];

		const DM2RTTable *pt0 = Ut::Find(pt, v0);
		if (pt0 == NULL) {
			x++;
			continue;
		}

		size_t y;
		UINT v1 = v0;
		for (y = x +1; y < sels.size(); y++) {
			UINT vx = sels[y];

			const DM2RTTable *pt1 = Ut::Find(pt, vx);
			if (pt0 == pt1) {
				if (v1 +1 == vx) {
					v1 = vx;
					continue;
				}
			}
			break;
		}

		if (!strRet.IsEmpty())
			strRet += " ";

		if (y -x == 1) {
			CString strTemp; strTemp.Format(
				"%c%u"
				, pt0->cls
				, v0 -pt0->valFrom
				);
			strRet += strTemp;

			x++;
		}
		else {
			CString strTemp; strTemp.Format(
				"%c%u-%u"
				, pt0->cls
				, v0 -pt0->valFrom
				, v1 -pt0->valFrom
				);
			strRet += strTemp;

			x = y;
		}
	}

	return strRet;
}
