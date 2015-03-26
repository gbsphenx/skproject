// DM2SkCmdList.cpp: CDM2SkCmdList クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2SkCmdList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define PA0 IDI_PaBALL
#define PA1 IDI_PaPush
#define PA2 IDI_PaPull
#define PA3 IDI_PaOH
#define PA4 IDI_PaDES
#define PA5 IDI_PaEW
#define PA6 IDI_PaVEN
#define PA7 IDI_PaZO

const CDM2SkCM g_DM2SkPA[] = {
	{0,"Fireball",PA0},
	{2,"Lightning",PA0},
	{3,"Dispell",PA5},
	{4,"Zo Spell",PA7},
	{6,"Poison bolt",PA6},
	{7,"Poison cloud",PA3},
	{9,"Push spell",PA1},
	{10,"Pull spell",PA2},

	{-1,NULL,-1},
};

#define CM0 IDI_CmXXX
#define CM1 IDI_CmDEF
#define CM2 IDI_CmWIZ
#define CM3 IDI_CmATT
#define CM4 IDI_CmEAT
#define CM5 IDI_CmMIN
#define CM6 IDI_CmWAY
#define CM7 IDI_CmMAP

const CDM2SkCM g_DM2SkCM[] = {
	{1,"Block ",CM1},
	{2,"Invisibility (OH EW SAR) ",CM2},
	{3,"Launch Missle specified in PA parametr ",CM2},
	{4,"Physical damage(#4)",CM3},
	{5,"Confuse ",CM3},
	{6,"Darkness (DES EW SAR) ",CM2},
	{7,"Spell Reflection (ZO BRO ROS) ",CM2},
	{8,"Physical damage(#8)",CM3},
	{9,"Aura of Speed (OH IR ROS) ",CM2},
	{10,"Use Rope (Climb Down) ",CM0},
	{11,"Freeze life ",CM0},
	{12,"Aura of Dexterity (OH EW ROS) ",CM2},
	{13,"Aura of Wisdom (OH EW DAIN) ",CM2},
	{14,"Aura of Vitality (OH EW NETA) ",CM2},
	{15,"Aura of Strength (OH EW KU) ",CM2},
	{16,"Consume ",CM4},
	{17,"Porch",CM6},
	{32,"Launch associated projective (Bow/Sling) ",CM3},
	{33,"Spellshield (YA IR DAIN) ",CM2},
	{34,"Fireshield (FUL BRO NETA) ",CM2},
	{35,"Shield (YA IR) ",CM2},
	{36,"Healing ",CM2},
	{38,"Light (covers both FUL and FUL IR RA) ",CM2},
	{42,"Throw the active item ",CM3},
		{44,"Mark",CM7},
		{45,"Call (C)arry",CM7},
		{46,"Call (F)etch",CM7},
		{47,"Call (S)cout minion",CM7},
		{48,"(K)ill minion",CM7},
	{49,"Attack Minion (ZO EW KU) ",CM5},
	{50,"Guard Minion (ZO EW NETA) ",CM5},
	{54,"Teleport ",CM0},
		{56,"Unused (#56)",CM7},
		{57,"Unused (#57)",CM7},
	{-1,NULL,-1},
};

namespace
{

LPCSTR g_pszSkTextCmdTbl[DM2SKCMD_MAX_CMD] = {
	"CM",
	"SK",
	"LV",
	"BZ",
	"TR",
	"TA",
	"EX",
	"PB",
	"DM",
	//
	"AT",
	"HN",
	"NC",
	"RP",
	//
	"PA",
	"SD",
	"ST",
	"WH",
};

};

//////////////////////////////////////////////////////////////////////
// CDM2SkCMUtil

LPCSTR CDM2SkCMUtil::GetNameOfId(int iID)
{
	for (int x = 0; g_DM2SkCM[x].pszName != NULL; x++)
		if (g_DM2SkCM[x].i == iID)
			return g_DM2SkCM[x].pszName;

	return NULL;
}

//////////////////////////////////////////////////////////////////////
// CDM2SkPAUtil

LPCSTR CDM2SkPAUtil::GetNameOfId(int iID)
{
	for (int x = 0; g_DM2SkPA[x].pszName != NULL; x++)
		if (g_DM2SkPA[x].i == iID)
			return g_DM2SkPA[x].pszName;

	return NULL;
}

//////////////////////////////////////////////////////////////////////
// CDM2SkCmdList

CDM2SkCmdList::CDM2SkCmdList()
{
	ZeroMemory(nVal, sizeof(nVal));
}

LPCSTR CDM2SkCmdList::GetName(UINT i)
{
	if (i < DM2SKCMD_MAX_CMD) return g_pszSkTextCmdTbl[i];

	return "";
}

int CDM2SkCmdList::LookUpName(LPCSTR pszPos, LPCSTR pszEnd)
{
	int n = pszEnd - pszPos;
	for (UINT i = 0; i < DM2SKCMD_MAX_CMD; i++) {
		if (memcmp(GetName(i), pszPos, n) == 0) return i;
	}
	return -1;
}

bool CDM2SkCmdList::Parse(LPCSTR pszPos)
{
	ZeroMemory(nVal, sizeof(nVal));
	LPCSTR pszBegin = pszPos;
	LPCSTR pszEnd = strchr(pszBegin, '\0');
	if ((pszPos = strchr(pszPos, ':')) == NULL)
		return false;
	strName = CString(pszBegin, pszPos - pszBegin);
	pszPos++;
	LPCSTR pszOrg;
	while (pszPos < pszEnd) {
		pszOrg = pszPos;
		pszPos += 2;
		int iCmd = LookUpName(pszOrg, pszPos);
		if (iCmd >= 0) {
			pszOrg = pszPos;
			int x;
			if (ReadInt(pszPos, pszEnd, x)) {
				nVal[iCmd] = x;
			} else {
				pszPos = pszOrg;
			}
		}
	}
	return true;
}

bool CDM2SkCmdList::ReadInt(LPCSTR &pszPos, LPCSTR pszEnd, int &x)
{
	int i = 0;
	int s = 1;
	x = 0;
	if (*pszPos == '-') {
		s = -s;
		pszPos++;
	}
	while (pszPos < pszEnd && isdigit((BYTE)*pszPos)) {
		x = x * 10 + (*pszPos - '0');
		i++;
		pszPos++;
	}
	if (i == 0)
		return false;
	x = x * s;
	return true;
}

CString CDM2SkCmdList::Mk()
{
	CString strRet = strName + ":";

	CString strText;
	for (int i = 0; i < DM2SKCMD_MAX_CMD; i++) {
		if (nVal[i] != 0) {
			strText.Format("%s%d", GetName(i), nVal[i]);
			strRet += strText;
		}
	}

	return strRet;
}

//////////////////////////////////////////////////////////////////////
// CDM2SkCmdDesc

void CDM2SkCmdDesc::RemoveAll()
{
	for (UINT i = 0; i < DM2SKCMD_MAX_CMD; i++) strDesc[i].Empty();
}

void CDM2SkCmdDesc::LoadFrom(LPCTSTR pszFileIn)
{
	for (UINT i = 0; i < DM2SKCMD_MAX_CMD; i++) {
		CString strText;
		LoadProfileStringFrom("DM2PC_CommandTextDesc", CDM2SkCmdList::GetName(i), strText, "", pszFileIn);
		strDesc[i] = strText;
	}
}

LPCTSTR CDM2SkCmdDesc::GetDesc(UINT i) const
{
	if (i < DM2SKCMD_MAX_CMD)
		return strDesc[i];
	return NULL;
}
