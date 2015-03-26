// WVCT.cpp: CWVCF クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "WVCT.h"
#include "GDEdCtx.h"
#include "SplitStr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#if 0
//	
//	//////////////////////////////////////////////////////////////////////
//	// CWordValCmd_HintFlags
//	
//	CString CWordValCmd_HintFlags::Format(WORD x)
//	{
//		return Flags_Format(x);
//	}
//	
//	bool CWordValCmd_HintFlags::Parse(LPCSTR pszText, WORD &x)
//	{
//		return Flags_Parse(pszText, x);
//	}
//	
//	CString CWordValCmd_HintFlags::FormatDisp(WORD x)
//	{
//		static WORD mask[16] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
//	
//		CString strHint;
//		list<CString>::iterator
//			iterPos = listHint.begin(),
//			iterEnd = listHint.end();
//		UINT i;
//		for (i = 0; iterPos != iterEnd && i < 16; i++, iterPos++) {
//			if (x & mask[i]) {
//				if (!iterPos->IsEmpty()) {
//					if (!strHint.IsEmpty())
//						strHint += ", ";
//					strHint += *iterPos;
//				}
//			}
//		}
//		return strHint;
//	}
//	
//	void CWordValCmd_HintFlags::SetParm1(LPCTSTR psz)
//	{
//		SplitStr(psz, '/', listHint);
//	}
//	
//	LPCTSTR CWordValCmd_HintFlags::GetHintText(int x)
//	{
//		list<CString>::iterator
//			iterPos = listHint.begin(),
//			iterEnd = listHint.end();
//		UINT i;
//		for (i = 0; i != x && iterPos != iterEnd; i++, iterPos++);
//		if (iterPos == iterEnd)
//			return NULL;
//		return *iterPos;
//	}
//	
//	//////////////////////////////////////////////////////////////////////
//	// CWordValCmd_B5353
//	
//	CString CWordValCmd_B5353::FormatB3(int iTextIdx, LPCTSTR pszDefText, UINT x)
//	{
//		if (x & 8)
//			return "";
//		LPCTSTR pszText = GetHintText(iTextIdx);
//		if (pszText == NULL)
//			pszText = pszDefText;
//		CString strRet;
//		strRet.Format("%s: %d", pszText, x);
//		return strRet;
//	}
//	
//	CString CWordValCmd_B5353::FormatDisp(WORD x)
//	{
//		UINT iLo = (x >> 0) & 15;
//		UINT iHi = (x >> 8) & 15;
//	
//		CString strRet, strText;
//	
//		strRet = FormatB3(0, "First byte", iLo);
//	
//		strText = FormatB3(1, "Second byte", iHi);
//		if (!strText.IsEmpty() && !strRet.IsEmpty()) strRet += ", ";
//		strRet += strText;
//	
//		return strRet;
//	}
//	
//	CString CWordValCmd_B5353::Format(WORD x)
//	{
//		return Unsigned_Format(x);
//	}
//	
//	bool CWordValCmd_B5353::Parse(LPCSTR pszText, WORD &x)
//	{
//		return Unsigned_Parse(pszText, x);
//	}
//	
//	//////////////////////////////////////////////////////////////////////
//	// CWordValCmd_PrizeItem
//	
//	CString CWordValCmd_PrizeItem::FormatDisp(WORD x)
//	{
//		CString strRet;
//		LPCTSTR psz = "?";
//		switch ((x >> 14) & 3) {
//		case 0: psz = "Weapons"; break;
//		case 1: psz = "Clothes"; break;
//		case 2: psz = "Miscellaneous"; break;
//		case 3: psz = "Potions"; break;
//		}
//		int nMin = 1 +((x >> 0) & 15);
//		int nMax = 0 +((x >> 4) & 7);
//		strRet.Format(
//			"%s: %02X, Count: %d (+%d)"
//			, psz
//			, 0 +((x >> 7) & 127)
//			, nMin
//			, nMax
//			);
//		return strRet;
//	}
//	
//	//////////////////////////////////////////////////////////////////////
//	// CWordValCmd_KeyItem
//	
//	CString CWordValCmd_KeyItem::FormatDisp(WORD x)
//	{
//		CString strRet;
//		LPCTSTR psz = "?";
//		switch ((x >> 7) & 3) {
//		case 0: psz = "Weapons"; break;
//		case 1: psz = "Clothes"; break;
//		case 2: psz = "Miscellaneous"; break;
//		case 3: psz = "Potions"; break;
//		}
//		strRet.Format(
//			"%s: %02X"
//			, psz
//			, 0 +((x >> 0) & 127)
//			);
//		return strRet;
//	}

#endif
