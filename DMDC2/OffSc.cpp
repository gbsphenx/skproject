
#include "StdAfx.h"
#include "OffSc.h"
#include "CompatDC.h"
#include "Ternary.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void COffScreen::Close()
{
	fv.Free();
	cx = cy = bpp = pitch = 0;
	pbi = NULL;
	pvmap = NULL;

	cbm.DeleteObject();
}

bool COffScreen::Create24Screen()
{
	int csx = GetSystemMetrics(SM_CXSCREEN);
	int csy = GetSystemMetrics(SM_CYSCREEN);

	Close();

	ZeroMemory(cbi, sizeof(cbi));

	pbi = (BITMAPINFO *)&cbi[0];

	BITMAPINFO &bi = *pbi;
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = csx;
	bi.bmiHeader.biHeight = csy;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 24;
	bi.bmiHeader.biCompression = BI_RGB;

	cx = csx;
	cy = csy;
	bpp = 24;
	pitch = RTo4(3 * csx);

	bi.bmiHeader.biSizeImage = pitch * cy;

#if 0
	if (!fv.Alloc(pitch * cy)) return false;

	pvmap = fv.GetData();
#else
	HBITMAP hbm = CreateDIBSection(
		NULL,
		pbi,
		DIB_RGB_COLORS,
		&pvmap,
		NULL,
		0
		);
	if (hbm == NULL) return false;

	cbm.Attach(hbm);
#endif

	return true;
}

int COffScreen::FlipMe(int newx, int newy, CBMO &src, int srcx, int srcy, int srccx, int srccy, DWORD rop)
{
	if (pvmap == NULL || newx < 0 || newy < 0 || srcx < 0 || srcy < 0 || srccx < 0 || srccy < 0)
		return -1;

	LineXTox fn = NULL;

	switch (rop) {
	case SRCCOPY:
		switch (bpp) {
		case 24:
			switch (src.bpp) {
			case 4: fn = Line4To24_S; break;
			case 8: fn = Line8To24_S; break;
			default:
				return -1;
			}
			break;
		default:
			return -1;
		}
		break;
	case SRCAND:
		switch (bpp) {
		case 24:
			switch (src.bpp) {
			case 4: fn = Line4To24_DSa; break;
			case 8: fn = Line8To24_DSa; break;
			default:
				return -1;
			}
			break;
		default:
			return -1;
		}
		break;
	case SRCPAINT:
		switch (bpp) {
		case 24:
			switch (src.bpp) {
			case 4: fn = Line4To24_DSo; break;
			case 8: fn = Line8To24_DSo; break;
			default:
				return -1;
			}
			break;
		default:
			return -1;
		}
		break;
	default:
		return -1;
	}

	srccx = min(srccx, cx - newx); srccx = max(0, srccx);
	srccy = min(srccy, cy - newy); srccy = max(0, srccy);

	BYTE *pNewVert = (BYTE *)pvmap + pitch * (cy -newy -1);
	BYTE *pSrcVert = (BYTE *)src.pv + src.pitch * (srcy);

	StrucLineXTox var;
	var.cx = srccx;
	var.newx = newx;
	var.p4 = &src.pbmi->bmiColors[0];
	var.pNew = pNewVert;
	var.pSrc = pSrcVert;
	var.srcx = srcx;

	int y = 0;
	for (; y < srccy; y++) {
		fn(var);

		var.pNew -= pitch;
		var.pSrc += src.pitch;
	}

	return srccy;
}

#if _MY_TERNARY
void COffScreen::Line4To24(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	const BYTE *pSrc = r.pSrc;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		BYTE c = pSrc[x1 / 2];
		if (x1 & 1) {
			c &= 15;
		} else {
			c /= 16;
		}
		RGBQUAD c4 = r.p4[c];
		*pNew = c4.rgbBlue; pNew++;
		*pNew = c4.rgbGreen; pNew++;
		*pNew = c4.rgbRed; pNew++;
		x1++;
	}
}

void COffScreen::Line4To24_DSa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	const BYTE *pSrc = r.pSrc;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		BYTE c = pSrc[x1 / 2];
		if (x1 & 1) {
			c &= 15;
		} else {
			c /= 16;
		}
		RGBQUAD c4 = r.p4[c];
		*pNew &= c4.rgbBlue; pNew++;
		*pNew &= c4.rgbGreen; pNew++;
		*pNew &= c4.rgbRed; pNew++;
		x1++;
	}
}

void COffScreen::Line4To24_DSo(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	const BYTE *pSrc = r.pSrc;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		BYTE c = pSrc[x1 / 2];
		if (x1 & 1) {
			c &= 15;
		} else {
			c /= 16;
		}
		RGBQUAD c4 = r.p4[c];
		*pNew |= c4.rgbBlue; pNew++;
		*pNew |= c4.rgbGreen; pNew++;
		*pNew |= c4.rgbRed; pNew++;
		x1++;
	}
}

void COffScreen::Line8To24(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = c4.rgbBlue; pNew++;
		*pNew = c4.rgbGreen; pNew++;
		*pNew = c4.rgbRed; pNew++;
		x1++;
	}
}

void COffScreen::Line8To24_DSa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew &= c4.rgbBlue; pNew++;
		*pNew &= c4.rgbGreen; pNew++;
		*pNew &= c4.rgbRed; pNew++;
		x1++;
	}
}

void COffScreen::Line8To24_DSo(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew |= c4.rgbBlue; pNew++;
		*pNew |= c4.rgbGreen; pNew++;
		*pNew |= c4.rgbRed; pNew++;
		x1++;
	}
}
#endif

int COffScreen::FlipTo(HDC hDC, int newx, int newy, int newcx, int newcy, int srcx, int srcy)
{
	int r = 0;
	if (cbm.m_hObject != NULL) {
		CCompatDC dcMem(CDC::FromHandle(hDC));
		HGDIOBJ hbmOld = dcMem.SelectObject((HGDIOBJ)cbm);
		r = BitBlt(
			hDC,
			newx,
			newy,
			newcx,
			newcy,
			dcMem,
			srcx,
			srcy,
			SRCCOPY
			);
		dcMem.SelectObject(hbmOld);
	} else {
		r = SetDIBitsToDevice(
			hDC,
			newx,
			newy,
			newcx,
			newcy,
			srcx,
			cy -srcy -newcy,
			0,
			cy,
			pvmap,
			pbi,
			DIB_RGB_COLORS
			);
	}

	return r;
}

void COffScreen::Clear()
{
	ZeroMemory(pvmap, pitch * cy);
}
