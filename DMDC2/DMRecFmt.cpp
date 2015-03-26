// DMRecFmt.cpp: CDMRecFmt クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "exp00069.h"
#include "DMRecFmt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CDMRecFmt

LPCTSTR CDMRecFmt::pszNameTbl[2][16] = {
	{
	"DOOR",
	"TELE",
	"TEXT",
	"ACTU",
	"MONS",
	"WEAP",
	"CLOT",
	"SCRO",
	"POTI",
	"CHES",
	"MISC",
	 "ACTU",
	 "ACTU",
	 "ACTU",
	"PROJ",
	"----",
	},
	{
	"DOOR",
	"TELE",
	"TEXT",
	"ACTU1",
	"MONS",
	"WEAP",
	"CLOT",
	"SCRO",
	"POTI",
	"CHES",
	"MISC",
	 "ACTU2",
	 "ACTU3",
	 "ACTU4",
	"PROJ",
	"----",
	},
};

CString CDMRecFmt::Format(BYTE iDB, BYTE iDir, DMDBU rec, bool actu1)
{

	iDB = iDB & 15;
	iDir = iDir & 3;

	CString strRes[2];
	switch (iDir) {
	case 0: strRes[0] = 'N'; break;
	case 1: strRes[0] = 'E'; break;
	case 2: strRes[0] = 'S'; break;
	case 3: strRes[0] = 'W'; break;
	}
	switch (iDB) {
	case  0: strRes[1].Format("%04X", 0U +rec.r0.w2); break;
	case  1: strRes[1].Format("%04X %04X", 0U +rec.r1.w2, 0U +rec.r1.w4); break;
	case  2: strRes[1].Format("%04X", 0U +rec.r2.w2); break;
	case 11: case 12: case 13:
	case  3: strRes[1].Format("%04X %04X %04X", 0U +rec.r3.w2, 0U +rec.r3.w4, 0U +rec.r3.w6); break;
	case  4: strRes[1].Format("%02X %02X %04X %04X %04X %04X %04X", 0U +rec.r4.b4, 0U +rec.r4.b5, 0U +rec.r4.w6[0], 0U +rec.r4.w6[1], 0U +rec.r4.w6[2], 0U +rec.r4.w6[3], 0U +rec.r4.w14); break;
	case  5: strRes[1].Format("%04X", 0U +rec.r5.w2); break;
	case  6: strRes[1].Format("%04X", 0U +rec.r6.w2); break;
	case  7: strRes[1].Format("%04X", 0U +rec.r7.w2); break;
	case  8: strRes[1].Format("%04X", 0U +rec.r8.w2); break;
	case  9: strRes[1].Format("%04X %02X %02X", 0U +rec.r9.w4, 0U +rec.r9.b6[0], 0U +rec.r9.b6[1]); break;
	case 10: strRes[1].Format("%04X", 0U +rec.r10.w2); break;
	case 14: strRes[1].Format("%02X %02X %04X", 0U +rec.r14.b4[0], 0U +rec.r14.b4[1], 0U +rec.r14.w6); break;
	}
	CString strRet;
	strRet.Format("%s:%s:%s", (LPCTSTR)strRes[0], pszNameTbl[actu1 ? 1 : 0][iDB], (LPCTSTR)strRes[1]);
	return strRet;
}

LPCSTR CDMRecFmt::GetTblName(BYTE i, bool actu1)
{
	return pszNameTbl[actu1 ? 1 : 0][i & 15];
}
