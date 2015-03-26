// DM2ImageExch.cpp: CDM2ImageExch クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2ImageExch.h"
#include "CompatDC.h"
#include "DM2Palo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace
{

inline void ConvFromTo(const RGBQUAD &rq, COLORREF &clr)
{
	clr = RGB(rq.rgbRed, rq.rgbGreen, rq.rgbBlue);
}

};

bool CDM2ImageConv::EncodeBMTo8(HBITMAP hbm, DMGHLiIMGM &imgm, const DMGHLciDM2Pal &DM2Pal)
{
	BITMAP bm;
	if (false
		|| GetObject(hbm, sizeof(BITMAP), &bm) != sizeof(BITMAP)
		|| (UINT)bm.bmWidth > 320
		|| (UINT)bm.bmHeight > 200
	) {
		return false;
	}

	CCompatDC dcMem;
	dcMem.SaveDC();
	dcMem.SelectObject(hbm);

	UINT cx = bm.bmWidth, cy = bm.bmHeight;

	imgm.cx = cx;
	imgm.cy = cy;
	imgm.ipf.Clear();
	imgm.fIs8bpp = true;
	imgm.cPal.UseDM2Native(DM2Pal);

#if 1
	for (UINT y = 0; y < cy; y++) {
		for (UINT x = 0; x < cx; x++) {
			COLORREF clr1 = dcMem.GetPixel(x, y);
			int r1 = GetRValue(clr1);
			int g1 = GetGValue(clr1);
			int b1 = GetBValue(clr1);

			BYTE c = 0;
			int dist = INT_MAX;
			for (int a = 0; a < 256; a++) {
				COLORREF clr2 = imgm.cPal.p.e.color[a];
				int r2 = GetRValue(clr2);
				int g2 = GetGValue(clr2);
				int b2 = GetBValue(clr2);

				int dist2 = abs(r1 - r2) * 3 + abs(g1 - g2) * 2 + abs(b1 - b2);
				if (dist2 < dist) {
					dist = dist2;
					c = (BYTE)a;
				}
			}
			imgm.SetPixel(x, y, c);
		}
	}
#else
	ColorDOM dom;

	for (int a = 0; a < 256; a++) {
		dom.AddColor(imgm.cPal.p.e.color[a], (BYTE)a);
	}

	for (UINT y = 0; y < cy; y++) {
		for (UINT x = 0; x < cx; x++) {
			COLORREF clr = dcMem.GetPixel(x, y);
			BYTE c = dom.Search(clr);
			imgm.SetPixel(x, y, c);
		}
	}
#endif

	return true;
}

bool CDM2ImageConv::EncodeBMTo4(HBITMAP hbm, DMGHLiIMGM &imgm, const DMGHLciDM2Pal &DM2Pal)
{
	if (!EncodeBMTo8(hbm, imgm, DM2Pal))
		return false;

	return CCS4(imgm, imgm, DM2Pal);
}

bool CDM2ImageConv::CCS4(DMGHLiIMGM &rSrc, DMGHLiIMGM &rDst, const DMGHLciDM2Pal &DM2Pal)
{
	UINT x, y;
	UINT cx = rSrc.cx;
	UINT cy = rSrc.cy;

	if (cx > DMGHLi_MAX_WIDTH || cy > DMGHLi_MAX_HEIGHT) return false;

	BYTE bDstPal[16];

	if (rSrc.fIs8bpp) {
		UI32 nUsed[256];
		ZeroMemory(nUsed, sizeof(nUsed));

		for (y = 0; y < cy; y++) {
			for (x = 0; x < cx; x++) {
				nUsed[rSrc.GetPixel(x, y)]++;
			}
		}

		multimap<UI32, BYTE, greater<UI32> > mmap;

		for (int t = 0; t < 256; t++) mmap.insert(pair<UI32, BYTE>(nUsed[t], (BYTE)t));

		multimap<UI32, BYTE, greater<UI32> >::iterator
			iterPos = mmap.begin(),
			iterEnd = mmap.end();

		for (int c = 0; c < 16; c++, iterPos++) bDstPal[c] = iterPos->second;

	} else {
		for (int c = 0; c < 16; c++) bDstPal[c] = rSrc.cPal.p.f.index[c];
	}

	return CCS4(rSrc, rDst, bDstPal, DM2Pal);
}

bool CDM2ImageConv::CCS4(DMGHLiIMGM &rSrc, DMGHLiIMGM &rDst, const BYTE bNewPal[16], const DMGHLciDM2Pal &DM2Pal)
{
	UINT x, y;
	UINT cx = rSrc.cx;
	UINT cy = rSrc.cy;

	if (cx > DMGHLi_MAX_WIDTH || cy > DMGHLi_MAX_HEIGHT) return false;

	ColorDOM z;

	for (int c = 0; c < 16; c++) z.AddColor(DM2Pal.clrs[bNewPal[c]], (BYTE)c);

	if (rSrc.fIs8bpp) {
		for (y = 0; y < cy; y++) {
			for (x = 0; x < cx; x++) {
				BYTE p = z.Search(DM2Pal.clrs[rSrc.GetPixel(x, y)]);
				rDst.SetPixel(x, y, p);
			}
		}
	} else {
		for (y = 0; y < cy; y++) {
			for (x = 0; x < cx; x++) {
				BYTE p = z.Search(rSrc.cPal.p.f.color[rSrc.GetPixel(x, y)]);
				rDst.SetPixel(x, y, p);
			}
		}
	}

	rDst.cx = rSrc.cx;
	rDst.cy = rSrc.cy;
	rDst.ipf.Clear();
	rDst.fIs8bpp = false;
	rDst.cPal.UseDM2From16Bytes((UI8 *)bNewPal, DM2Pal);

	return true;
}

bool CDM2ImageConv::CCS4(DMGHLiIMGM &rSrc, DMGHLiIMGM &rDst, COLORREF clr[16], const DMGHLciDM2Pal &DM2Pal)
{
	ColorDOM z;

	for (int c = 0; c < 256; c++) z.AddColor(DM2Pal.clrs[c], (BYTE)c);

	BYTE bNewPal[16];

	for (int i = 0; i < 16; i++) {
		BYTE p = z.Search(clr[i]);
		bNewPal[i] = p;
	}

	return CCS4(rSrc, rDst, bNewPal, DM2Pal);
}

bool CDM2ImageConv::CCS4(const COLORREF clr[16], DMGHLciPAL &rPal, const DMGHLciDM2Pal &DM2Pal)
{
	ColorDOM z;

	for (int c = 0; c < 256; c++) z.AddColor(DM2Pal.clrs[c], (BYTE)c);

	BYTE bNewPal[16];

	for (int i = 0; i < 16; i++) {
		BYTE p = z.Search(clr[i]);
		bNewPal[i] = p;
	}

	rPal.UseDM2From16Bytes((UI8 *)bNewPal, DM2Pal);

	return true;
}

bool CDM2ImageConv::CCS4(const RGBQUAD rq[16], DMGHLciPAL &rPal, const DMGHLciDM2Pal &DM2Pal)
{
	COLORREF clr[16];

	for (int i = 0; i < 16; i++) ConvFromTo(rq[i], clr[i]);

	return CCS4(clr, rPal, DM2Pal);
}

bool CDM2ImageConv::EncodeToBM4(DMGHLiIMGM &imgm, HBITMAP &hbm, const DMGHLciDM2Pal &DM2Pal)
{
	if (imgm.fIs8bpp) return false;
	void *ppv;
	UINT cx = imgm.cx;
	UINT cy = imgm.cy;
	int nPitch;
	hbm = CreateDIB(cx, cy, imgm.cPal, false, ppv, nPitch, DM2Pal);
	if (hbm) {
		UINT x, y;
		PBYTE pbDestVert = (PBYTE)ppv + (cy - 1) * nPitch;
		for (y = 0; y < cy; y++, pbDestVert -= nPitch) {
			PBYTE pbDestHorz = pbDestVert;
			for (x = 0; x < cx; x += 2, pbDestHorz++) {
				BYTE p[2] = {
					imgm.GetPixel(x+0, y) & 0x0F,
					imgm.GetPixel(x+1, y) & 0x0F,
				};
				*pbDestHorz = (p[0] << 4) | (p[1]);
			}
		}
		return true;
	}
	return false;
}

bool CDM2ImageConv::EncodeToBM8(DMGHLiIMGM &imgm, HBITMAP &hbm, const DMGHLciDM2Pal &DM2Pal)
{
	void *ppv;
	UINT cx = imgm.cx;
	UINT cy = imgm.cy;
	int nPitch;
	hbm = CreateDIB(cx, cy, imgm.cPal, true, ppv, nPitch, DM2Pal);
	if (hbm) {
		UINT x, y;
		PBYTE pbDestVert = (PBYTE)ppv + (cy - 1) * nPitch;
		for (y = 0; y < cy; y++, pbDestVert -= nPitch) {
			PBYTE pbDestHorz = pbDestVert;
			for (x = 0; x < cx; x++, pbDestHorz++) {
				*pbDestHorz = imgm.GetPixel(x, y);
			}
		}
		return true;
	}
	return false;
}

bool CDM2ImageConv::EncodeToBM(DMGHLiIMGM &imgm, HBITMAP &hbm, const DMGHLciDM2Pal &DM2Pal)
{
	if (imgm.fIs8bpp)
		return EncodeToBM8(imgm, hbm, DM2Pal);
	return EncodeToBM4(imgm, hbm, DM2Pal);
}

HBITMAP CDM2ImageConv::CreateDIB(int cx, int cy, const DMGHLciPAL &rPal, bool fBM8bpp, void *&ppv, int &nPitch, const DMGHLciDM2Pal &DM2Pal)
{
	CHAR c[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];
	PBITMAPINFO pBI = (PBITMAPINFO)c;
	ZeroMemory(pBI, sizeof(c));
	pBI->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pBI->bmiHeader.biWidth = cx;
	pBI->bmiHeader.biHeight = cy;
	pBI->bmiHeader.biPlanes = 1;
	pBI->bmiHeader.biBitCount = fBM8bpp ? 8 : 4;
	pBI->bmiHeader.biCompression = BI_RGB;
	if (fBM8bpp) {
		nPitch = ((cx + 3) / 4) * 4;
		for (int i = 0; i < 256; i++) CDM2Palo::Conv(DM2Pal.clrs[i], pBI->bmiColors[i]);
	} else {
		nPitch = ((cx + 7) / 8) * 4;
		for (int i = 0; i < 16; i++) CDM2Palo::Conv(rPal.p.f.color[i], pBI->bmiColors[i]);
	}
	CCompatDC dc;
	HBITMAP hbm = CreateDIBSection(
		dc,
		pBI,
		DIB_RGB_COLORS,
		&ppv,
		NULL,
		0
		);
	return hbm;
}
