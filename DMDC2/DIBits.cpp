// DIBits.cpp: CDIBits クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DIBits.h"

// 17:56 2002/12/22
// 23:02 2002/12/22

//////////////////////////////////////////////////////////////////////
// ...

namespace
{

void SureRead(CFile &f, LPVOID lpv, SIZE_T n)
{
	if (f.Read(lpv, n) != n) AfxThrowFileException(CFileException::endOfFile, -1, f.GetFileName());
}

void SureRead(CArchive &f, LPVOID lpv, SIZE_T n)
{
	if (f.Read(lpv, n) != n) AfxThrowArchiveException(CArchiveException::endOfFile, NULL);
}

};

BOOL DIBits_RealizePalette(CDC &dc, CDIBits &r, BOOL fForceBackground)
{
	CPalette *lpOldPal = dc.SelectPalette(&r.GetPal(), fForceBackground);
	UINT n = dc.RealizePalette();
	return n != GDI_ERROR;
}

//////////////////////////////////////////////////////////////////////
// CDIBits

CDIBits::CDIBits()
{
	
}

CDIBits::~CDIBits()
{

}

BOOL CDIBits::CreateDIBits24(
	CSize size, 
	LONG xPelsPerMeter, 
	LONG yPelsPerMeter)
{
	TRY
		SIZE_T cbBits = CalcSizeImage(size, 24);
		m_BI.SetSize(sizeof(BITMAPINFOHEADER));
		LPBITMAPINFO lpBI =	(LPBITMAPINFO)m_BI.GetData();
		lpBI->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpBI->bmiHeader.biWidth = size.cx;
		lpBI->bmiHeader.biHeight = size.cy;
		lpBI->bmiHeader.biPlanes = 1;
		lpBI->bmiHeader.biBitCount = 24;
		lpBI->bmiHeader.biCompression = BI_RGB;
		lpBI->bmiHeader.biSizeImage = cbBits;
		lpBI->bmiHeader.biXPelsPerMeter = xPelsPerMeter;
		lpBI->bmiHeader.biYPelsPerMeter = yPelsPerMeter;
		lpBI->bmiHeader.biClrUsed =
		lpBI->bmiHeader.biClrImportant = 0;
		m_Bits.SetSize(cbBits);
		m_BM.bmType = 0;
		m_BM.bmWidth = size.cx;
		m_BM.bmHeight = size.cy;
		m_BM.bmWidthBytes = CalcWidthBytes(size.cx, 24);
		m_BM.bmPlanes = 1;
		m_BM.bmBitsPixel = 24;
		m_BM.bmBits = m_Bits.GetData();
		return TRUE;
	CATCH_ALL(e)

	END_CATCH_ALL
	return FALSE;
}

BOOL CDIBits::CreateDIBitsPR(
	CSize size, 
	int nBitCount,
	const RGBQUAD *lpColour, 
	int nColour, 
	LONG xPelsPerMeter, 
	LONG yPelsPerMeter)
{
	TRY
		if (nBitCount != 1 && nBitCount != 4 && nBitCount != 8) return FALSE;
		int nMaxPal = 1 << nBitCount;
		SIZE_T cbBits = CalcSizeImage(size, nBitCount);
		m_BI.SetSize(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * nMaxPal);
		LPBITMAPINFO lpBI =	(LPBITMAPINFO)m_BI.GetData();
		lpBI->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpBI->bmiHeader.biWidth = size.cx;
		lpBI->bmiHeader.biHeight = size.cy;
		lpBI->bmiHeader.biPlanes = 1;
		lpBI->bmiHeader.biBitCount = nBitCount;
		lpBI->bmiHeader.biCompression = BI_RGB;
		lpBI->bmiHeader.biSizeImage = cbBits;
		lpBI->bmiHeader.biXPelsPerMeter = xPelsPerMeter;
		lpBI->bmiHeader.biYPelsPerMeter = yPelsPerMeter;
		lpBI->bmiHeader.biClrUsed = nMaxPal;
		lpBI->bmiHeader.biClrImportant = (nColour = __min(nMaxPal, __max(0, nColour)));
		ZeroMemory(lpBI->bmiColors, sizeof(RGBQUAD) * nMaxPal);
		RGBQUAD *lpTarColour = lpBI->bmiColors;
		for (; nColour != 0; nColour--, lpTarColour++, lpColour++) *lpTarColour = *lpColour;
		m_Bits.SetSize(cbBits);
		m_BM.bmType = 0;
		m_BM.bmWidth = size.cx;
		m_BM.bmHeight = size.cy;
		m_BM.bmWidthBytes = CalcWidthBytes(size.cx, nBitCount);
		m_BM.bmPlanes = 1;
		m_BM.bmBitsPixel = nBitCount;
		m_BM.bmBits = m_Bits.GetData();
		return TRUE;
	CATCH_ALL(e)
#ifdef _DEBUG
		e->ReportError(MB_ICONINFORMATION);
#endif
	END_CATCH_ALL
	return FALSE;
}

const BITMAP &CDIBits::GetBitmap()
{
	return m_BM;
}

BOOL CDIBits::Load(LPCTSTR lpszIn)
{
	TRY
		CFile fileIn(lpszIn, CFile::modeRead|CFile::shareDenyWrite);
		BITMAPFILEHEADER bfh;
		SureRead(fileIn, &bfh, sizeof(bfh));
		if (bfh.bfType != *(WORD *)"BM")
			return FALSE;
		DWORD biSize;
		CArchive ar(&fileIn, CArchive::load);
		ar >> biSize;
		switch (biSize) {
		case sizeof(BITMAPINFOHEADER):
		{
			LONG biWidth, biHeight, biXPels, biYPels;
			WORD biPlanes, biBitCount;
			DWORD biCompression, biSizeImage, biClrUsed, biClrImportant;
			ar	>> biWidth >> biHeight >> biPlanes >> biBitCount >> biCompression >> biSizeImage
				>> biXPels >> biYPels >> biClrUsed >> biClrImportant;
			switch (biBitCount) {
			case 1:
				if (biCompression == BI_RGB) break;
				return FALSE;
			case 4:
				if (biCompression == BI_RGB || biCompression == BI_RLE4) break;
				return FALSE;
			case 8:
				if (biCompression == BI_RGB || biCompression == BI_RLE8) break;
				return FALSE;
			}
			RGBQUAD clr[256], *clrTar = clr;
			UINT nMaxPal = 1 << biBitCount;
			UINT nClr = __min(nMaxPal, biClrUsed);
			for (; nClr != 0; nClr--, clrTar++) SureRead(ar, clrTar, sizeof(RGBQUAD));
			switch (biBitCount) {
			case 1:
			case 4:
			case 8:
				if (!CreateDIBitsPR(CSize(biWidth, biHeight), biBitCount, clr, biClrImportant, biXPels, biYPels))
					return FALSE;
				break;
			case 24:
				if (!CreateDIBits24(CSize(biWidth, biHeight), biXPels, biYPels))
					return FALSE;
				break;
			}
			ar.Flush();
			fileIn.Seek(bfh.bfOffBits, CFile::begin);
			SIZE_T cbBits = CalcSizeImage(CSize(biWidth, biHeight), biBitCount);
			switch (biCompression) {
			case BI_RGB:
				if (!ReadRGBBits(ar)) return FALSE;
				break;
			case BI_RLE4:
				if (!ReadRLE4Bits(ar)) return FALSE;
				break;
			case BI_RLE8:
				if (!ReadRLE8Bits(ar)) return FALSE;
				break;
			}
			switch (biBitCount) {
			case 1:
			case 4:
			case 8:
				if (!CreateAssociatedPalette())
					return FALSE;
				break;
			};
			break;
		}
		case sizeof(BITMAPCOREHEADER):
		{
			break;
		}
		}
		return TRUE;
	CATCH_ALL(e)
#ifdef _DEBUG
//		e->ReportError(MB_ICONINFORMATION);
#endif
	END_CATCH_ALL
	return FALSE;
}

BOOL CDIBits::ReadRLE8Bits(CArchive &ar)
{
	BITMAP bm = GetBitmap();
	UINT cx = abs(bm.bmWidth);
	INT cy = abs(bm.bmHeight);
	for (INT y = 0; y < cy; y++) {
		LPBYTE lpHorz = GetPointerAt(0, y);
		for (UINT x = 0; ; ) {
			BYTE bx, by;
			ar >> bx;
			if (bx == 0) {
				ar >> bx;
				if (bx == 0)
					break;
				if (bx == 1)
					return TRUE;
				if (bx == 2) {
					ar >> bx >> by;
					lpHorz = GetPointerAt(x = (x + bx), y = (y + by));
					break;
				} else {
					UINT i;
					for (i = 0; i < bx && x < cx; i++, lpHorz++) { ar >> by; *lpHorz = by; x++; }
					for (; i & 1; i++) ar >> bx;
				}
			} else {
				ar >> by;
				for (UINT i = 0; i < bx && x < cx; i++, lpHorz++) { *lpHorz = by; x++; }
			}
		}
	}
	return TRUE;
}

BOOL CDIBits::ReadRGBBits(CArchive &ar)
{
	BITMAP bm = GetBitmap();
	INT cy = abs(bm.bmHeight);
	UINT cxBytes = bm.bmWidthBytes;
	for (INT y = 0; y < cy; y++) {
		SureRead(ar, GetPointerAt(0, y), cxBytes);
	}
	return TRUE;
}

BOOL CDIBits::Draw(
	CDC &dc,
	CRect rcDest,
	CRect rcSrc)
{
	const BITMAP &bm = GetBitmap();
	if (rcSrc.IsRectEmpty())
		rcSrc.SetRect(0, 0, bm.bmWidth, bm.bmHeight);
	if (rcDest.IsRectEmpty())
		rcDest = rcSrc;
	int n = StretchDIBits(
		dc,
		rcDest.left,
		rcDest.top,
		rcDest.Width(),
		rcDest.Height(),
		rcSrc.left,
		rcSrc.top,
		rcSrc.Width(),
		rcSrc.Height(),
		m_Bits.GetData(),
		(LPBITMAPINFO)m_BI.GetData(),
		DIB_RGB_COLORS,
		SRCCOPY);
	if (!n)
		return FALSE;
	return TRUE;
}

BOOL CDIBits::ReadRLE4Bits(CArchive &ar)
{
	BITMAP bm = GetBitmap();
	UINT cx = abs(bm.bmWidth);
	INT cy = abs(bm.bmHeight);
	DWORD nRetry = 0;
	for (INT y = 0; y < cy; y++) {
		Rle4WB wb(GetPointerAt(0, y));
		for (UINT x = 0; ; ) {
			BYTE bx, by;
			ar >> bx;
			if (bx == 0) {
				ar >> bx;
				if (bx == 0)
					break;
				if (bx == 1)
					return TRUE;
				if (bx == 2) {
					ar >> bx >> by;
					wb = Rle4WB(GetPointerAt(x + bx, y + by));
				} else {
					for (UINT i = 0; i < bx && x < cx; i++, x++) {
						if (i & 1) {
							wb << ((by >> 0) & 0x0F);
						} else {
							ar >> by;
							wb << ((by >> 4) & 0x0F);
						}
					}
					if ((bx / 2) & 1)
						ar >> by;
				}
			} else {
				ar >> by;
				for (UINT i = 0; i < bx && x < cx; i++, x++) {
					if (i & 1)	wb << (BYTE)((by >> 0) & 0x0F);
					else		wb << (BYTE)((by >> 4) & 0x0F);
				}
			}
		}
	}
	return TRUE;
}

BOOL CDIBits::Store(LPCTSTR lpszFile)
{
	TRY
		CFile fileInto(lpszFile, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyWrite);
		BITMAPFILEHEADER bfh;
		DWORD cbBFH = sizeof(bfh);
		DWORD cbBI = m_BI.GetSize();
		DWORD cbBits = m_Bits.GetSize();
		ZeroMemory(&bfh, sizeof(bfh));
		bfh.bfType = *(WORD *)"BM";
		bfh.bfSize = cbBFH + cbBI + cbBits;
		bfh.bfOffBits = cbBFH + cbBI;
		fileInto.Write(&bfh, sizeof(bfh));
		fileInto.Seek(cbBFH, CFile::begin);
		fileInto.Write(m_BI.GetData(), m_BI.GetSize());
		fileInto.Seek(bfh.bfOffBits, CFile::begin);
		fileInto.Write(m_Bits.GetData(), m_Bits.GetSize());
		fileInto.Close();
		return TRUE;
	CATCH_ALL(e)
#ifdef _DEBUG
		e->ReportError(MB_ICONEXCLAMATION);
#endif
	END_CATCH_ALL
	return FALSE;
}

BOOL CDIBits::CreateAssociatedPalette()
{
	m_Pal.DeleteObject();

	const BITMAPINFO *bi = (const BITMAPINFO *)m_BI.GetData();
	const BITMAP &bm = GetBitmap();
	if (!bi || bm.bmBitsPixel > 8) return FALSE;
	UINT nMaxPal = 1 << bm.bmBitsPixel;
	CHAR cPal[sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * 255];
	LPLOGPALETTE lpPal = (LPLOGPALETTE)cPal;
	lpPal->palVersion = 0x0300;
	lpPal->palNumEntries = nMaxPal;
	for (UINT i = 0; i < nMaxPal; i++) {
		*(DWORD *)&lpPal->palPalEntry[i] =
			MAKELONG(MAKEWORD(bi->bmiColors[i].rgbRed, bi->bmiColors[i].rgbGreen), MAKEWORD(bi->bmiColors[i].rgbBlue, PC_NOCOLLAPSE));
	}
	if (!m_Pal.CreatePalette(lpPal))
		return FALSE;
	return TRUE;
}

BOOL CDIBits::IsLoaded() const
{
	return m_BI.GetSize() != 0;
}
