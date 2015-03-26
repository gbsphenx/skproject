// DM2DescriptCtx.cpp: CDM2DescriptCtx クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2DescriptCtx.h"
#include "CSVrw.h"
#include "GDEdCtx.h"
#include "WVCT.h"
#include "SplitStr.h"
#include "EditVxxx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace
{

static BYTE g_bL2RTbl[8] = {1, 2, 4, 8, 16, 32, 64, 128};

};

//////////////////////////////////////////////////////////////////////
// CDM2DescItem

CDM2DescItem::CDM2DescItem()
: vxxx(NULL), fDynSel(false)
{
	ZeroMemory(c1m, sizeof(c1m));
	ZeroMemory(c2m, sizeof(c2m));
	ZeroMemory(c3, sizeof(c3));
	ZeroMemory(f3, sizeof(f3));
}

CDM2DescItem::~CDM2DescItem()
{
	setVxxx(NULL);
}

bool CDM2DescItem::Parse(CEditVxxxFactoryImpl &vxxxfac, LPCSTR psz1, LPCSTR psz2, LPCSTR psz3, LPCSTR psz4, LPCSTR psz5, LPCSTR psz6, LPCSTR psz7, LPCSTR psz8)
{
	LPCSTR pszPos, pszEnd;

	// 1
	pszEnd = strchr(pszPos = psz1, 0);
	while (pszPos[0] == '$' && pszPos + 3 <= pszEnd) {
		if (!isxdigit(pszPos[1]) || !isxdigit(pszPos[2]))
			break;
		int x = Hex2Val(pszPos[1]) * 16 + Hex2Val(pszPos[2]);
		c1m[x / 8] |= g_bL2RTbl[x % 8];
		if (pszPos[3] == ';') {
			pszPos += 4;
		} else {
			pszPos += 3;
			break;
		}
	}

	// 2
	pszEnd = strchr(pszPos = psz2, 0);
	if (pszPos == pszEnd) {
		memset(c2m, -1, sizeof(c2m));
	} else {
		while (pszPos[0] == '$' && pszPos + 3 <= pszEnd) {
			if (!isxdigit(pszPos[1]) || !isxdigit(pszPos[2]))
				break;
			int x = Hex2Val(pszPos[1]) * 16 + Hex2Val(pszPos[2]);
			c2m[x / 8] |= g_bL2RTbl[x % 8];
			if (pszPos[3] == ';') {
				pszPos += 4;
			} else {
				pszPos += 3;
				break;
			}
		}
	}

	// 3
	pszPos = psz3;
	if (pszPos[0] != '$' || !isxdigit(pszPos[1]) || !isxdigit(pszPos[2]))
		return false;
	c3[0] = Hex2Val(pszPos[1]) * 16 + Hex2Val(pszPos[2]);
	f3[0] = true;

	// 4
	pszPos = psz4;
	if (pszPos[0] != '$')
		return false;
	pszPos++;
	if (false
		|| !ParseHex2OrXX(pszPos, c3[1], f3[1])
		|| !ParseHex2OrXX(pszPos, c3[2], f3[2])
		|| !ParseHex2OrXX(pszPos, c3[3], f3[3])
	) {
		return false;
	}

	// 5
	strText = psz5;

	// 6
	int wvct = vxxxfac.ParseName2(psz6, c3[0]);

	setVxxx(vxxxfac.CreateVxxx(wvct));

	// 7
	vxxx->GetOp().SetParm1(psz7);

	// 8
	strProg = psz8;
	strProg.TrimLeft();
	strProg.TrimRight();

	fDynSel = !strProg.IsEmpty();

	return true;
}

bool CDM2DescItem::Match(CGDEntry pos) const
{
	BYTE c1 = pos.c1();
	if (!(c1m[c1 / 8] & g_bL2RTbl[c1 % 8]))
		return false;

	BYTE c2 = pos.c2();
	if (!(c2m[c2 / 8] & g_bL2RTbl[c2 % 8]))
		return false;

	for (int i = 0; i < 4; i++) {
		if (f3[i] && pos.c[2 + i] != c3[i])
			return false;
	}

	return true;
}

CEditVxxx &CDM2DescItem::GetVxxx() const
{
	ASSERT(vxxx != NULL);
	return *vxxx;
}

//////////////////////////////////////////////////////////////////////
// CDM2DescriptCtx

CDM2DescriptCtx::~CDM2DescriptCtx()
{
	RemoveAll();
}

void CDM2DescriptCtx::RemoveAll()
{
	while (!listRule.empty()) {
		delete listRule.front();
		listRule.pop_front();
	}
}

bool CDM2DescriptCtx::LoadFrom(CEditVxxxFactoryImpl &vxxxfac, LPCTSTR pszFileIn)
{
	while (true) {
		CSVR_t csvr;
		if (!csvr.Open(pszFileIn))
			break;
		while (csvr.ReadNextLine()) {
			const int MAXCOLS = 8;
			CString str[MAXCOLS];
			int iColumn = 0;
			for (; iColumn < MAXCOLS && csvr.ReadNextToken(str[iColumn]); iColumn++);
			if (iColumn >= 5) {
				CDM2DescItem *p = new CDM2DescItem();
				if (p->Parse(vxxxfac, str[0], str[1], str[2], str[3], str[4], str[5], str[6], str[7])) {
					listRule.push_back(p);
				} else {
					delete p;
				}
			}
		}
		csvr.Close();
		return true;
	};
	return false;
}

const CDM2DescItem *CDM2DescriptCtx::FindItem(const CGDEntry &pos)
{
	RuleList::iterator
		iterPos = listRule.begin(),
		iterEnd = listRule.end();
	for (; iterPos != iterEnd; iterPos++) {
		CDM2DescItem *p = *iterPos;
		if (p->Match(pos))
			return p;
	}
	return NULL;
}

const CDM2DescItem *CDM2DescriptCtx::FindItem2(const CGDEntry &pos, CGDAT &core)
{
	RuleList::iterator
		iterPos = listRule.begin(),
		iterEnd = listRule.end();
	for (; iterPos != iterEnd; iterPos++) {
		CDM2DescItem *p = *iterPos;
		if (p->Match(pos)) {
			if (!p->HasProg())
				return p;
			LPCSTR pszProg = p->GetProg();
			if (strncmp(pszProg, "forth:", 6) == 0) {
				try {
					CDM2EntForth forth(pszProg +6, pos, core);
					if (forth.Run() && forth.pop() != 0)
						return p;
				}
				catch (runtime_error) {

				}
			}
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// CDM2EntForth

bool CDM2EntForth::Process()
{
	if (tok == "C1") {
		push(pos.c1());
		return true;
	}
	else if (tok == "C2") {
		push(pos.c2());
		return true;
	}
	else if (tok == "GETVALUE") {
		int c6 = pop();
		int c5 = pop();
		int c4 = pop();
		int c3 = pop();
		int c2 = pop();
		int c1 = pop();
		CGDEntry ent((BYTE)c1, (BYTE)c2, (BYTE)c3, (BYTE)c4, (BYTE)c5, (BYTE)c6);
		push(core.GetCls3ValueAt(ent));
		return true;
	}
	return false;
}
