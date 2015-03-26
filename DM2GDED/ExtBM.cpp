// ExtBM.cpp: CExtBM クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DM2GDED.h"
#include "ExtBM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace
{

static RGBQUAD g_altPal1[2] = {
	{0x00,0x00,0x00,0x00},
	{0xFF,0xFF,0xFF,0x00},
};

static RGBQUAD g_altPal4[16] = {
	{0x00,0x00,0x00,0x00}, // 0
	{0x00,0x00,0x80,0x00}, // 1
	{0x00,0x80,0x00,0x00}, // 2
	{0x00,0x80,0x80,0x00}, // 3
	{0x80,0x00,0x00,0x00}, // 4
	{0x80,0x00,0x80,0x00}, // 5
	{0x80,0x80,0x00,0x00}, // 6
	{0xC0,0xC0,0xC0,0x00}, // 7
	{0x80,0x80,0x80,0x00}, // 8
	{0x00,0x00,0xFF,0x00}, // 9
	{0x00,0xFF,0x00,0x00}, //10
	{0x00,0xFF,0xFF,0x00}, //11
	{0xFF,0x00,0x00,0x00}, //12
	{0xFF,0x00,0xFF,0x00}, //13
	{0xFF,0xFF,0x00,0x00}, //14
	{0xFF,0xFF,0xFF,0x00}, //15
};

UINT R4(UINT x)
{
	UINT z = x % 4;
	if (!z)
		return x;
	return x + (4 - z);
}

};

//////////////////////////////////////////////////////////////////////
// CExtBM

CExtBM::CExtBM(): CBitmap()
{

}

CExtBM::~CExtBM()
{

}

BOOL CExtBM::SaveTo(HGLOBAL &hMem)
{
	SaveToGlobalMem aMem;

	if (!SaveTo(aMem, false)) return false;

	hMem = aMem.Detach();
	return true;
}

BOOL CExtBM::SaveTo(SizeBuff &fv, bool fToFile)
{
	SaveToFixedVuffer aMem(fv);

	if (!SaveTo(aMem, fToFile)) return false;

	return true;
}

BOOL CExtBM::SaveToFile(LPCTSTR pszFileInto)
{
	CExtBMx::SaveToFM aMem(pszFileInto);

	if (!SaveTo(aMem, true)) return false;

	return true;
}

BOOL CExtBM::SaveTo(SaveToMem &rMem, bool fToFile)
{
	BITMAP bm;
	if (GetObject(sizeof(BITMAP), &bm) != sizeof(BITMAP) || bm.bmBits == NULL)
		return FALSE;
	UINT iPal, nPal = 0;
	switch (bm.bmBitsPixel) {
	default:
		return FALSE;
	case 1:
		nPal = 2;
		break;
	case 4:
		nPal = 16;
		break;
	case 8:
		nPal = 256;
		break;
	case 24:
		break;
	}
	PALETTEENTRY pe[256];
	if (nPal != 0) {
		if (m_Pal.m_hObject == NULL || m_Pal.GetPaletteEntries(0, nPal, pe) != nPal)
			return FALSE;
	}
	UINT nPitchSrc = R4(bm.bmWidthBytes);
	UINT nPitchDst = R4(nPitchSrc);
	int cx = bm.bmWidth;
	int cy = bm.bmHeight;
	UINT cbFileHeader = fToFile ? sizeof(BITMAPFILEHEADER) : 0;
	UINT cbHeader = sizeof(BITMAPINFOHEADER);
	UINT cbPal = sizeof(RGBQUAD) * nPal;
	UINT cbImage = nPitchDst * cy;
	UINT cbBM = cbFileHeader + cbHeader + cbPal + cbImage;

	void *pvAny = rMem.Alloc1ce(cbBM);
	if (pvAny == NULL) return FALSE;

	BYTE *pbPos = (BYTE *)pvAny;
	BYTE *pbEnd = pbPos + cbBM;
	PBITMAPFILEHEADER pbfh = (PBITMAPFILEHEADER)pbPos;
	if (fToFile) {
		pbfh->bfType = *(const WORD *)"BM";
		pbfh->bfSize = cbBM;
		pbfh->bfReserved1 =
		pbfh->bfReserved2 = 0;
		pbfh->bfOffBits = cbFileHeader + cbHeader + cbPal;

		pbPos += sizeof(*pbfh);
	}
	PBITMAPINFOHEADER pbih = (PBITMAPINFOHEADER)pbPos;
	pbih->biSize = sizeof(BITMAPINFOHEADER);
	pbih->biWidth = cx;
	pbih->biHeight = cy;
	pbih->biPlanes = 1;
	pbih->biBitCount = bm.bmBitsPixel;
	pbih->biCompression = BI_RGB;
	pbih->biSizeImage = cbImage;
	pbih->biXPelsPerMeter = 0;
	pbih->biYPelsPerMeter = 0;
	pbih->biClrUsed = pbih->biClrImportant = nPal;
	pbPos += sizeof(BITMAPINFOHEADER);
	RGBQUAD *p4 = (RGBQUAD *)pbPos;
	PPALETTEENTRY ppe = pe;
	for (iPal = 0; iPal < nPal; iPal++, p4++, ppe++) {
		p4->rgbRed = ppe->peRed;
		p4->rgbGreen = ppe->peGreen;
		p4->rgbBlue = ppe->peBlue;
		p4->rgbReserved = 0;
	}
	pbPos += sizeof(RGBQUAD) * nPal;
	BYTE *pbVertDst = pbPos; 
	BYTE *pbVertSrc = (BYTE *)bm.bmBits;
	for (int y = 0; y < cy; y++) {
		memcpy(pbVertDst, pbVertSrc, nPitchSrc);
		pbVertDst += nPitchDst;
		pbVertSrc += nPitchSrc;
	}
	return TRUE;
}

BOOL CExtBM::Attach(HBITMAP hBM)
{
	BITMAP bm;
	if (::GetObject(hBM, sizeof(bm), &bm) == sizeof(bm)) {
		UINT iPal, nPal = -1;
		switch (bm.bmBitsPixel) {
		case 1:
			nPal = 2; break;
		case 4:
			nPal = 16; break;
		case 8:
			nPal = 256; break;
		case 24:
			nPal = 0; break;
		}
		if (nPal != -1) {
			if (nPal != 0) {
				CompatDC dc(NULL);
				HGDIOBJ hOldBM = SelectObject(dc, hBM);
				RGBQUAD r4[256];
				if (GetDIBColorTable(dc, 0, nPal, r4) == nPal) {
					CHAR clp[sizeof(LOGPALETTE) + 256 * sizeof(PALETTEENTRY)];
					PLOGPALETTE plp = (PLOGPALETTE)clp;
					for (iPal = 0; iPal < nPal; iPal++) {
						plp->palPalEntry[iPal].peRed = r4[iPal].rgbRed;
						plp->palPalEntry[iPal].peGreen = r4[iPal].rgbGreen;
						plp->palPalEntry[iPal].peBlue = r4[iPal].rgbBlue;
						plp->palPalEntry[iPal].peFlags = 0;
					}
					plp->palVersion = 0x0300;
					plp->palNumEntries = nPal;
					if (m_Pal.CreatePalette(plp)) {
						SelectObject(dc, hOldBM);

						m_bm = bm;
						return CGdiObject::Attach(hBM);
					}
				}
				SelectObject(dc, hOldBM);
			} else {
				m_bm = bm;
				return CGdiObject::Attach(hBM);
			}
		}
	}
	return FALSE;
}

HBITMAP CExtBM::Detach()
{
	m_Pal.DeleteObject();

	return (HBITMAP)CGdiObject::Detach();
}

BOOL CExtBM::Create(CSize size, int nBitsPixel, const RGBQUAD *p4, int n4)
{
	RGBQUAD *p4Alt = NULL;
	UINT iPal, nPal;
	switch (nBitsPixel) {
	case 1:
		nPal = 2; p4Alt = g_altPal1; break;
	case 4:
		nPal = 16; p4Alt = g_altPal4; break;
	case 8:
		nPal = 256; break;
	case 24:
		nPal = 0; break;
	default:
		return FALSE;
	}
	if (n4 != 0)
		nPal = __min(nPal, (UINT)n4);
	if (p4 == NULL)
		p4 = p4Alt;
	CHAR cbi[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)];
	PBITMAPINFO pbi = (PBITMAPINFO)cbi;
	pbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbi->bmiHeader.biWidth = size.cx;
	pbi->bmiHeader.biHeight = size.cy;
	pbi->bmiHeader.biPlanes = 1;
	pbi->bmiHeader.biBitCount = nBitsPixel;
	pbi->bmiHeader.biCompression = BI_RGB;
	pbi->bmiHeader.biSizeImage = 0;
	pbi->bmiHeader.biXPelsPerMeter = 0;
	pbi->bmiHeader.biYPelsPerMeter = 0;
	pbi->bmiHeader.biClrUsed = pbi->bmiHeader.biClrImportant = nPal;
	for (iPal = 0; iPal < nPal; iPal++) {
		pbi->bmiColors[iPal] = p4[iPal];
	}
	PVOID pvBits;
	CompatDC dc(NULL);
	HBITMAP hBM = CreateDIBSection(
		dc,
		pbi,
		DIB_RGB_COLORS,
		&pvBits,
		NULL,
		0
		);
	if (hBM != NULL) {
		if (Attach(hBM)) {
			return TRUE;
		}
		::DeleteObject(hBM);
	}
	return FALSE;
}

BOOL CExtBM::LoadFrom(HGLOBAL hMem)
{
	LoadFromGlobalMem aMem(hMem);

	if (!LoadFrom(aMem, false)) return false;

	return true;
}

BOOL CExtBM::LoadFrom(const void *pvPos, UINT cbPos, bool fFromFile)
{
	LoadFromRangedMem aMem((const BYTE *)pvPos, cbPos);

	if (!LoadFrom(aMem, fFromFile)) return false;

	return true;
}

BOOL CExtBM::LoadFrom(LoadFromMem &rMem, bool fFromFile)
{
	if (fFromFile) {
		const BITMAPFILEHEADER *pbfh = (const BITMAPFILEHEADER *)rMem.ReadNextLock(sizeof(BITMAPFILEHEADER));
		if (pbfh == NULL) return false;
		if (pbfh->bfType != *(const WORD *)"BM") return false;
	}
	const BITMAPINFOHEADER *pbih = (const BITMAPINFOHEADER *)rMem.ReadNextLock(sizeof(BITMAPINFOHEADER));
	if (pbih == NULL) return false;
	CHAR cbi[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)];
	BITMAPINFO &bi = (BITMAPINFO &)cbi;
	BITMAPINFOHEADER &bih = bi.bmiHeader;
	bih = *pbih;
	if (bih.biSize == sizeof(BITMAPINFOHEADER)) {
		int cx = bih.biWidth;
		int cy = bih.biHeight;
		int nPal = bih.biClrUsed;
		int nBPP = bih.biBitCount;
		if (nBPP <= 8) {
			UINT nPalSize = sizeof(RGBQUAD) * nPal;
			const void *pvPalSrc = rMem.ReadNextLock(nPalSize);
			if (nPalSize <= 0 || pvPalSrc == NULL) return false;
			memcpy(bi.bmiColors, pvPalSrc, nPalSize);
		}
		if (!Create(CSize(cx, cy), nBPP, bi.bmiColors, nPal)) return false;
		const void *pvBitsSrc = rMem.ReadNextLock(bih.biSizeImage);
		CompatDC dcMem(NULL);
		HGDIOBJ hOldObj = dcMem.SelectObject(m_hObject);
		int nLines = SetDIBitsToDevice(
			dcMem,
			0, 0, cx, cy,
			0, 0,
			0, cy,
			pvBitsSrc, &bi,
			DIB_RGB_COLORS
			);
		dcMem.SelectObject(hOldObj);
		if (nLines != abs(cy)) return false;
		return true;
	}
	return false;
}

BOOL CExtBM::DeleteObject()
{
	if (m_hObject == NULL)
		return FALSE;
	return ::DeleteObject(Detach());
}

BYTE *CExtBM::GetPointer()
{
	return (BYTE *)m_bm.bmBits;
}

BOOL CExtBM::GetBitmap(BITMAP &bm)
{
	if (m_hObject == NULL)
		return FALSE;
	bm = m_bm;
	return TRUE;
}

int CExtBM::BitsPixel()
{
	if (m_hObject == NULL)
		return 0;
	return m_bm.bmBitsPixel;
}

CPalette &CExtBM::GetPalette()
{
	return m_Pal;
}

CSize CExtBM::GetSize()
{
	return CSize(m_bm.bmWidth, m_bm.bmHeight);
}
