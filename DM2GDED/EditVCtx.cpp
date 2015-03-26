// EditVCtx.cpp: CEditVLower クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "EditVCtx.h"
#include "ILSp.h"
#include "DM2RTextToken.h"
#include "GDEdCtx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CEditVLower

CGDEdCtx *CEditVLower::GetEdCtx(BOOL fAddRef)
{
	if (m_pEdCtx && fAddRef)
		m_pEdCtx->AddRef();
	return m_pEdCtx;
}

void CEditVLower::SetEdCtx(CGDEdCtx *pEdCtx)
{
//	if (pEdCtx) {
//		pEdCtx->AddRef();
//		pEdCtx->GetOpts(m_aOpt);
//	}
//
//	if (m_pEdCtx)
//		m_pEdCtx->Release();

	m_pEdCtx = pEdCtx;
}

BOOL CEditVLower::Get6CatItemName(int iCat, int iIdx, CString &strText)
{
	CGDEntry pos = CGDEntry::Min;
	switch (iCat) {
	case rttChests:  pos.c1(0x14); break; // CHESTS
	case rttWeapons: pos.c1(0x10); break; // WEAP
	case rttClothes: pos.c1(0x11); break; // CLOT
	case rttScrolls: pos.c1(0x12); break; // SCRO
	case rttPotions: pos.c1(0x13); break; // POTI
	case rttMisc:    pos.c1(0x15); break; // MISC
	}
	pos.c2((BYTE)iIdx);
	pos.c3(0x05);

	CGDEdOpt aOpt;
	m_pEdCtx->GetOpts(aOpt);

	CILSp::GetInlang(aOpt.m_inlang).Transform(pos);

	CGDAT &core = m_pEdCtx->GetGDAT();

	if (core.GetDispTextAt(core.GetValueAt(pos), strText)) {
		return TRUE;
	}
	return FALSE;
}

BOOL CEditVLower::Get4CatItemName(int iCat, int iIdx, CString &strText)
{
	CGDEntry pos = CGDEntry::Min;
	switch (iCat) {
	case rttWeapons: pos.c1(0x10); break;
	case rttClothes: pos.c1(0x11); break;
	case rttMisc:    pos.c1(0x15); break;
	case rttPotions: pos.c1(0x13); break;
	}
	pos.c2((BYTE)iIdx);
	pos.c3(0x05);

	CGDEdOpt aOpt;
	m_pEdCtx->GetOpts(aOpt);

	CILSp::GetInlang(aOpt.m_inlang).Transform(pos);

	CGDAT &core = m_pEdCtx->GetGDAT();

	if (core.GetDispTextAt(core.GetValueAt(pos), strText)) {
		return TRUE;
	}
	return FALSE;
}

BOOL CEditVLower::Get1CatItemName(int iCat, int iIdx, CString &strText)
{
	CGDEntry pos = CGDEntry::Min;
	switch (iCat) {
	case rttCreatures: pos.c1(0x0F); break;
	}
	pos.c2((BYTE)iIdx);
	pos.c3(0x05);

	CGDEdOpt aOpt;
	m_pEdCtx->GetOpts(aOpt);

	CILSp::GetCnlang(aOpt.m_cnlang).Transform(pos);

	CGDAT &core = m_pEdCtx->GetGDAT();

	if (core.GetDispTextAt(core.GetValueAt(pos), strText)) {
		return TRUE;
	}
	return FALSE;
}
