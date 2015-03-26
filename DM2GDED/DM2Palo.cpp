
#include "StdAfx.h"
#include "DM2Palo.h"
#include "DMGHLci.h"

void CDM2Palo::GetColor(BYTE x, COLORREF &clr)
{
	clr = g_DM2Pal[x];
}

void CDM2Palo::GetColor(BYTE x, RGBQUAD &rq)
{
	rq = g_DM2PalRGBq[x];
}
