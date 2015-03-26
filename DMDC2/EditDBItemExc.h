// EditDBItemExc.h: CEditDBItemExc クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITDBITEMEXC_H__72A08B78_3114_4F70_B44F_A0A9D8FA7388__INCLUDED_)
#define AFX_EDITDBITEMEXC_H__72A08B78_3114_4F70_B44F_A0A9D8FA7388__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Exp00069Doc.h"

class CEditDBItemExchanger
{
public:
	// 
	union Item
	{
		struct t0 {
			WORD w2;
		} r0;
		struct t1 {
			WORD w2, w4;
		} r1;
		struct t2 {
			WORD w2;
		} r2;
		struct t3 {
			WORD w2, w4, w6;
		} r3;
		struct t4 {
			WORD w2;
			BYTE b4, b5;
			WORD w6[4], w14;
		} r4;
		struct t5 {
			WORD w2;
		} r5;
		struct t6 {
			WORD w2;
		} r6;
		struct t7 {
			WORD w2;
		} r7;
		struct t8 {
			WORD w2;
		} r8;
		struct t9 {
			WORD w2;
			WORD w4;
			BYTE b6[2];
		} r9;
		struct t10 {
			WORD w2;
		} r10;
		struct t14 {
			WORD w2;
			BYTE b4[2];
			WORD w6;
		} r14;
		struct t15 {
			WORD w2;
		} r15;
	};
	// 
	Item m_item;

	// 
	CEditDBItemExchanger()
	{
		ZeroMemory(&m_item, sizeof(m_item));
	}
	// 
	BOOL ReadFrom(UINT i, const DMDBU &rec)
	{
		switch (i) {
		case 0:
		{
			m_item.r0.w2 = rec.r0.w2;
			break;
		}
		case 1:
		{
			m_item.r1.w2 = rec.r1.w2;
			m_item.r1.w4 = rec.r1.w4;
			break;
		}
		case 2:
		{
			m_item.r2.w2 = rec.r2.w2;
			break;
		}
		case 3:
		case 11:
		case 12:
		case 13:
		{
			m_item.r3.w2 = rec.r3.w2;
			m_item.r3.w4 = rec.r3.w4;
			m_item.r3.w6 = rec.r3.w6;
			break;
		}
		case 4:
		{
			m_item.r4.w2 = rec.r4.stolen.r.w;
			m_item.r4.b4 = rec.r4.b4;
			m_item.r4.b5 = rec.r4.b5;
			for (int v=0; v<4; v++) m_item.r4.w6[v] = rec.r4.w6[v];
			m_item.r4.w14 = rec.r4.w14;
			break;
		}
		case 5:
		{
			m_item.r5.w2 = rec.r5.w2;
			break;
		}
		case 6:
		{
			m_item.r6.w2 = rec.r6.w2;
			break;
		}
		case 7:
		{
			m_item.r7.w2 = rec.r7.w2;
			break;
		}
		case 8:
		{
			m_item.r8.w2 = rec.r8.w2;
			break;
		}
		case 9:
		{
			m_item.r9.w2 = rec.r9.stolen.r.w;
			m_item.r9.w4 = rec.r9.w4;
			for (int v=0; v<2; v++) m_item.r9.b6[v] = rec.r9.b6[v];
			break;
		}
		case 10:
		{
			m_item.r10.w2 = rec.r10.w2;
			break;
		}
		case 14:
		{
			m_item.r14.w2 = rec.r14.stolen.r.w;
			for (int v=0; v<2; v++) m_item.r14.b4[v] = rec.r14.b4[v];
			m_item.r14.w6 = rec.r14.w6;
			break;
		}
		case 15:
		{
			m_item.r15.w2 = rec.r15.w2;
			break;
		}
		}
		return TRUE;
	}
	// 
	bool SaveAs(UINT i, DMDBU &rec, bool fNew)
	{
		switch (i) {
		case 0:
		{
			if (fNew) rec.r0.next.r.w = 0xfffe;
			rec.r0.w2 = m_item.r0.w2;
			break;
		}
		case 1:
		{
			if (fNew) rec.r1.next.r.w = 0xfffe;
			rec.r1.w2 = m_item.r1.w2;
			rec.r1.w4 = m_item.r1.w4;
			break;
		}
		case 2:
		{
			if (fNew) rec.r2.next.r.w = 0xfffe;
			rec.r2.w2 = m_item.r2.w2;
			break;
		}
		case 3:
		case 11:
		case 12:
		case 13:
		{
			if (fNew) rec.r3.next.r.w = 0xfffe;
			rec.r3.w2 = m_item.r3.w2;
			rec.r3.w4 = m_item.r3.w4;
			rec.r3.w6 = m_item.r3.w6;
			break;
		}
		case 4:
		{
			if (fNew) rec.r4.next.r.w = 0xfffe;
			rec.r4.stolen.r.w = m_item.r4.w2;
			rec.r4.b4 = m_item.r4.b4;
			rec.r4.b5 = m_item.r4.b5;
			for (int v=0; v<4; v++) rec.r4.w6[v] = m_item.r4.w6[v];
			rec.r4.w14 = m_item.r4.w14;
			break;
		}
		case 5:
		{
			if (fNew) rec.r5.next.r.w = 0xfffe;
			rec.r5.w2 = m_item.r5.w2;
			break;
		}
		case 6:
		{
			if (fNew) rec.r6.next.r.w = 0xfffe;
			rec.r6.w2 = m_item.r6.w2;
			break;
		}
		case 7:
		{
			if (fNew) rec.r7.next.r.w = 0xfffe;
			rec.r7.w2 = m_item.r7.w2;
			break;
		}
		case 8:
		{
			if (fNew) rec.r8.next.r.w = 0xfffe;
			rec.r8.w2 = m_item.r8.w2;
			break;
		}
		case 9:
		{
			if (fNew) rec.r9.next.r.w = 0xfffe;
			rec.r9.stolen.r.w = m_item.r9.w2;
			rec.r9.w4 = m_item.r9.w4;
			for (int v=0; v<2; v++) rec.r9.b6[v] = m_item.r9.b6[v];
			break;
		}
		case 10:
		{
			if (fNew) rec.r10.next.r.w = 0xfffe;
			rec.r10.w2 = m_item.r10.w2;
			break;
		}
		case 14:
		{
			if (fNew) rec.r14.next.r.w = 0xfffe;
			rec.r14.stolen.r.w = m_item.r14.w2;
			for (int v=0; v<2; v++) rec.r14.b4[v] = m_item.r14.b4[v];
			rec.r14.w6 = m_item.r14.w6;
			break;
		}
		case 15:
		{
			if (fNew) rec.r15.next.r.w = 0xfffe;
			rec.r15.w2 = m_item.r15.w2;
			break;
		}
		default: return false;
		}
		return true;
	}
};

#endif // !defined(AFX_EDITDBITEMEXC_H__72A08B78_3114_4F70_B44F_A0A9D8FA7388__INCLUDED_)
