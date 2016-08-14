// ILSp.cpp: CILSp �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "ILSp.h"
#include "GDEdCtx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace
{

CILSp g_ilspInlangNo		(CGDEntry::ERR, CGDEntry::ERR);
CILSp g_ilspInlangEnPlain	(CGDEntry(0xFF,0xFF,0xFF,0x00,0x00,0x00),CGDEntry(0x00,0x00,0x00,0x18,0x00,0x00));
CILSp g_ilspInlangEn		(CGDEntry(0xFF,0xFF,0xFF,0x00,0x00,0x00),CGDEntry(0x00,0x00,0x00,0x18,0x10,0x00));
CILSp g_ilspInlangJa		(CGDEntry(0xFF,0xFF,0xFF,0x00,0x00,0x00),CGDEntry(0x00,0x00,0x00,0x18,0x20,0x00));
CILSp g_ilspInlangDe		(CGDEntry(0xFF,0xFF,0xFF,0x00,0x00,0x00),CGDEntry(0x00,0x00,0x00,0x18,0x30,0x00));
CILSp g_ilspInlangFr		(CGDEntry(0xFF,0xFF,0xFF,0x00,0x00,0x00),CGDEntry(0x00,0x00,0x00,0x18,0x40,0x00));
CILSp g_ilspCnlangNo		(CGDEntry::ERR, CGDEntry::ERR);
CILSp g_ilspCnlangEnPlain	(CGDEntry(0xFF,0xFF,0xFF,0x00,0x00,0x00),CGDEntry(0x00,0x00,0x00,0x00,0x00,0x00));
CILSp g_ilspCnlangEn		(CGDEntry(0xFF,0xFF,0xFF,0x00,0x00,0x00),CGDEntry(0x00,0x00,0x00,0x00,0xF0,0x00));

};

//////////////////////////////////////////////////////////////////////
// CILSp

CILSp &CILSp::GetInlang(int inlang)
{
	switch (inlang) {
	case inlangEn: return g_ilspInlangEn;
	case inlangEnPlain: return g_ilspInlangEnPlain;
	case inlangJa: return g_ilspInlangJa;
	case inlangDe: return g_ilspInlangDe;
	case inlangFr: return g_ilspInlangFr;

	default:
	case inlangNo: return g_ilspInlangNo;
	}
}

CILSp &CILSp::GetCnlang(int cnlang)
{
	switch (cnlang) {
	case cnlangEn: return g_ilspCnlangEn;
	case cnlangEnPlain: return g_ilspCnlangEnPlain;

	default:
	case cnlangNo: return g_ilspCnlangNo;
	}
}