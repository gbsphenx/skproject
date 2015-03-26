// DM2NIMan.cpp: CDM2NIMan クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2NIMan.h"
#include "DM2ImageExch.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

bool CDM2NIMan::Load(CGDEntry pos, CDM2NICtx &r)
{
	const CDM2PIPDesc *p = core.GetPIPCtx().FindFirstMatch(pos);

	if (p == NULL) return true;

	int iColorKey = p->m_nColorKey;
	int iColorKey2 = p->m_nColorKey2;
	int iScene = p->m_nScene;

	if (iScene < 0 && p->m_fDepend) iScene = pos.c2();

	r.ps = CDM2NICtx::psNo;

	switch (p->m_ts) {
	case piptsScene:
		{
			if (iColorKey < 0) {
				iColorKey = GetOrnateSceneKey(pos, true);
				if (iColorKey < 0) {
					iColorKey = GetSceneKey(iScene);
				}
			}
			if (iColorKey2 < 0) {
				iColorKey2 = GetOrnateSceneKey(pos, false);
			}
			break;
		}
	}

	bool fRestrictedPal = core.IsPalRestricted();

	if (fRestrictedPal) {
		switch (p->m_ts) {
		case piptsScene:
			{
				COLORREF clr[16];
				if (false
					|| core.GetScenePalette(iScene, 1, clr)
					|| core.GetScenePalette(iScene, 0, clr)
				) {
					if (CDM2ImageConv().CCS4(clr, r.cPal, core.GetDM2Pal())) {
						r.ps = CDM2NICtx::psForce;
					}
				}
				break;
			}
		case piptsColorPal:
			{
				COLORREF clr[16];
				if (core.ReadRawColorPalette(r.posColorPal = p->m_pos, 0, clr)) {
					if (CDM2ImageConv().CCS4(clr, r.cPal, core.GetDM2Pal())) {
						r.ps = CDM2NICtx::psShould;
					}
				}
				break;
			}
		case piptsIf:
			{
				COLORREF clr[16];
				if (core.GetInterfacePalette(clr)) {
					if (CDM2ImageConv().CCS4(clr, r.cPal, core.GetDM2Pal())) {
						r.ps = CDM2NICtx::psForce;
					}
				}
				break;
			}
		}
	}

	if (!((r.iColorSrcKey = iColorKey) < 0)) r.iColorKey = iColorKey;
	if (!((r.iColorSrcKey2 = iColorKey2) < 0)) r.iColorKey2 = iColorKey2;

	return true;
}
