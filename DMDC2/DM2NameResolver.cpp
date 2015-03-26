// DM2NameResolver_t.cpp: DM2NameResolver_t クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Exp00069.h"
#include "DM2NameResolver.h"
#include "DM2TextRec.h"
#include "DM2NameDic.h"
#include "DM2ScrollRec.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static LPCTSTR g_pszDisplayItemName[16] = {
	"Door",
	"Teleporter",
	"Text",
	"Actuator",
	"Creature",
	"Weapon",
	"Cloth",
	"Scroll",
	"Potion",
	"Chest",
	"Misc",
	"?",
	"?",
	"?",
	"Projectile",
	"?",
};

//////////////////////////////////////////////////////////////////////
// DM2NameResolver_t

bool DM2NameResolver_t::GetStr(BYTE iDB, bool isWall, const DMDBU &rec, DM2Nameo &ref)
{
	iDB &= 0xF;

	ref.text[0] = g_pszDisplayItemName[iDB];
	ref.text[1].Empty();
	ref.text[2].Empty();

	CString &strName = ref.text[1];
	CString &strHint = ref.text[2];

	CDM2NameDic &dict = CDM2NameDic::GetInstance();

	switch (iDB) {
	case 2:
		{
			DM2TextRec_t tr;
			tr.Decode(rec.r2.w2);

			switch (tr.trpt) {
			case trptOrnate:
				strName = dict.GetDATDBName(isWall ? 0x09 : 0x0A, tr.index);
				break;
			case trptCreature:
				strName = dict.GetName(0x4, tr.index);
				break;
			case trptTextg:
				strName = GetExttextStr(tr.index);
				break;
			case trptTexti:
				strName = GetInttextStr(tr.index);
				break;
			}

			strHint.Format("Use: %s", (LPCTSTR)tr.text1);
			break;
		}
	case  3:
		{
			DWORD w2 = rec.r3.w2;
			int t = w2 & 0x7F;
			t |= (isWall ? 128 : 0);
			CDM2ActuHint ah = dict.GetActuHint(t & 127, t & 128);
			strName = ah.pszName;
			switch (ah.a1t) {
			case a1tItem:
				{
					int sc = 0;
					switch (w2 / 0x4000) {
					case 0: sc = 5; break;
					case 1: sc = 6; break;
					case 2: sc = 10; break;
					case 3: sc = 8; break;
					}
					int si = (w2 / 0x80) & 127;
					strHint.Format(
						"Item: %s"
						, (w2 / 0x80 == 511) ? "All" : (LPCSTR)CString(dict.GetName(sc, si))
						);
					break;
				}
			case a1tN:
				{
					int si = w2 / 0x80;
					strHint.Format(
						"Cnt: %d"
						, si
						);
					break;
				}
			case a1tMissile:
				{
					int si = (w2 / 0x80) & 63;
					strHint.Format(
						"Missile: %d"
						, si
						);
					break;
				}
			case a1tCreature:
				{
					int si = (w2 / 0x80) & 255;
					strHint.Format(
						"Creature: %s"
						, (LPCTSTR)CString(dict.GetName(4, si))
						);
					break;
				}
			case a1tHero:
				{
					int si = (w2 / 0x80) & 255;
					strHint.Format(
						"Hero: %s"
						, (LPCTSTR)CString(dict.GetHeroName(si))
						);
					break;
				}
			}
			DWORD w4 = rec.r3.w4;
			int nDelay = (w4 / 0x80) & 31;
			if (nDelay != 0) {
				CString str;
				str.Format("Delay:%d", nDelay);
				if (!strHint.IsEmpty()) strHint += ", ";
				strHint += str;
			}
			{
				int t = (w4 / 64) & 1;
				if (t) {
					if (!strHint.IsEmpty()) strHint += ", ";
					strHint += "Beep";
				}
			}
			{
				int t = (w4 / 32) & 1;
				if (t) {
					if (!strHint.IsEmpty()) strHint += ", ";
					strHint += "Inv";
				}
			}
			{
				int t = (w4 / 8) & 3;
				LPCSTR psz[] = {
					"Turn On",
					"Turn Off",
					"Toggle",
					"Link",
				};
				if (!strHint.IsEmpty()) strHint += ", ";
				strHint += "Act: ";
				strHint += psz[t];
			}
			break;
		}
	case  4:
		{
			int t = (rec.r4.b4);
			strName = dict.GetName(iDB, t);
			break;
		}
	case  5:
	case  6:
	case 10:
		{
			if (iDB == 5) strHint.Format("# of charges: %d", (int)((rec.r5.w2 / 1024) & 15));

			int t = (rec.r5.w2 & 0x7F);
			strName = dict.GetName(iDB, t);
			break;
		}
	case  7:
		{
			DM2ScrollRec_t sr;
			sr.Decode(rec.r7.w2);

			if (sr.fExt) {
				strName = GetExttextStr(sr.index);
			} else {
				strName = GetIntScrollStr(sr.index);
			}

			strHint.Format("Use: %s", (LPCTSTR)sr.text1);
			break;
		}
	case  8:
		{
			int t = (rec.r8.w2 / 256) & 0x7F;
			strHint.Format("Potion power: %d", (int)(rec.r8.w2 & 0xFF));
			strName = dict.GetName(iDB, t);
			break;
		}
	case  9:
		{
			int t = CExp00069App::CHES2ID(rec.r9.w4);
			strName = dict.GetName(iDB, t);
			break;
		}
	}

	strName.Replace("\r\n", "\n");
	strName.Replace('\r', '/');
	strName.Replace('\n', '/');

	return true;
}

CString DM2NameResolver_t::GetInttextStr(UINT x)
{
	return texte.GetStr(x);
}

CString DM2NameResolver_t::GetExttextStr(UINT x)
{
	return CDM2NameDic::GetInstance().GetName(0x2, x);
}

CString DM2NameResolver_t::GetIntScrollStr(UINT x)
{
	HRESULT hr;
	DMDBU rec;
	do {
		if (pDDIf == NULL)
			break;
		if (FAILED(hr = pDDIf->GetDBRecAt(DBIPos_t(2, x), rec)))
			break;

		DM2TextRec_t tr;
		tr.Decode(rec.r2.w2);

		return GetInttextStr(tr.index);

	} while (false);

	return "";
}

void DM2NameResolver_t::EnumTextRecords(CWordArray &vec)
{
	vec.RemoveAll();

	CWordArray keys;
	texte.EnumKey(keys);

	HRESULT hr;
	DMDBU rec;
	const UINT cx = pDDIf->CntDBRecIn(0x2);

	for (UINT x=0; x<cx; x++) {
		if (FAILED(hr = pDDIf->GetDBRecAt(DBIPos_t(0x2, x), rec)))
			continue;
		WORD index = (rec.r2.w2 >> 3);
		for (UINT z=0; z<keys.GetSize(); z++) {
			if (keys[z] == index) {
				vec.Add(x);
				break;
			}
		}
	}
}
