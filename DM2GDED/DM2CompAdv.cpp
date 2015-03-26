// DM2CompAdv.cpp: CDM2CompAdv クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2CompAdv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDM2VwPalAdvisory CDM2VwPalAdvisor::Match(CGDEntry pos, int nScene)
{
#if 1
	CDM2VwPalAdvisory z;
	const CDM2PIPDesc *p = core.GetPIPCtx().FindFirstMatch(pos);

	int &r = z.iSceneKey;

	if ((r = GetOrnateSceneKey(pos)) < 0) {
		if (p) {
			z.ts = p->m_ts;
			z.pos = p->m_pos;
			if ((r = p->m_nColorKey) < 0) {
				if ((r = GetSceneKey(p->m_nScene)) < 0) {

				}
			}
		}
		if (r < 0) {
			if ((r = GetSceneKey(nScene)) < 0) {

			}
		}
	} else {
		z.fOrnateKey = true;
	}
	return z;
#else
//	int r = -1;
//	bool fUseScene = false;
//	bool fIfPal = false;
//
//	switch (pos.c3()) {
//	case 0x01:
//		// MINION MAP CHIP
//		if (pos.c4() == 0xF9 && pos.c5() == 0x00 && pos.c6() == 0x00) {
//			r = 10;
//			fIfPal = true;
//		} else {
//			switch (pos.c1()) {
//			case 0x01: // INTERFACE - MAIN SCREEN
//				{
//					switch (pos.c2()) {
//					case 0x06:
//					case 0x07:
//						{
//							r = 12;
//							break;
//						}
//					}
//					fIfPal = true;
//					break;
//				}
//			case 0x05: // MAIN MENU AND CREDITS
//				{
//					break;
//				}
//			case 0x07: // INTERFACE - CHARACTER SHEET
//				{
//					r = 12;
//					fIfPal = true;
//					break;
//				}
//			case 0x08: // DUNGEON GRAPHICS
//				{
//					nScene = pos.c2();
//
//					fUseScene = true;
//					break;
//				}
//			case 0x09: // WALL ORNATES
//			case 0x0A: // FLOOR ORNATES
//				{
//					pos.c3(0x0B);
//					pos.c4(0x04);
//					pos.c5(0);
//					pos.c6(0);
//					r = core.GetValueAt(pos);
//					fUseScene = true;
//					break;
//				}
//			case 0x0B: // DOOR ORNATES
//				{
//					r = 9;
//					break;
//				}
//			case 0x0D: // MISSILES
//				{
//					r = 10;
//					break;
//				}
//			case 0x0E: // DOORS
//				{
//					pos.c3(0x0B);
//					pos.c4(0x04);
//					pos.c5(0);
//					pos.c6(0);
//					r = core.GetValueAt(pos);
//					break;
//				}
//			case 0x0F: // CREATURES
//				{
//					pos.c3(0x0B);
//					pos.c4(0x04);
//					pos.c5(0);
//					pos.c6(0);
//					r = core.GetValueAt(pos);
//					fUseScene = true;
//					break;
//				}
//			case 0x10: // WEAPONS
//			case 0x11: // CLOTHES
//		//	case 0x12: // SCROLLS
//			case 0x13: // POTIONS
//		//	case 0x14: // CHESTS
//			case 0x15: // MISC
//				{
//					r = 10;
//					if (pos.c4() >= 0x18) r = 12;
//					break;
//				}
//			}
//		}
//		break;
//	}
//	if (fUseScene && !(nScene < 0) && r < 0) {
//		pos.c1(0x08);
//		pos.c2((BYTE)nScene);
//		pos.c3(0x0B);
//		pos.c4(0x64);
//		pos.c5(0);
//		pos.c6(0);
//		r = core.GetValueAt(pos);
//	}
//	CDM2VwPalAdvisory z;
//	z.iPalFrom = fIfPal ? DM2SAPF_INTERFACE : DM2SAPF_DUNGEON;
//	z.iSceneKey = r;
//	return z;
#endif
}
