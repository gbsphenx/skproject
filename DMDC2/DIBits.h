// DIBits.h: CDIBits クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIBITS_H__D0AEC1C4_7A4D_4CC2_BBCB_EEF468ED50B9__INCLUDED_)
#define AFX_DIBITS_H__D0AEC1C4_7A4D_4CC2_BBCB_EEF468ED50B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDIBits  
{
	// 
	class Rle4WB
	{
		// 
		BYTE *P;
		// 
		int i;
	public:
		// 
		Rle4WB(BYTE *_P) { P = _P; i = 0; }
		// 
		Rle4WB &operator <<(BYTE r)
		{
			if (i)	*P = (*P & 0xF0) | ((r & 0x0F) << 0), P++;
			else	*P = (*P & 0x0F) | ((r & 0x0F) << 4);
			i ^= 1;
			return *this;
		}
	};

	// 
	CByteArray m_BI;
	// 
	CByteArray m_Bits;
	// 
	BITMAP m_BM;
	// 
	CPalette m_Pal;

	// 
	SIZE_T CalcSizeImage(CSize size, int nBitCount)
	{
		SIZE_T cy = CalcWidthBytes(size.cx, nBitCount) * abs(size.cy);
		return cy;
	}
	// 
	SIZE_T CalcWidthBytes(int cx, int nBitCount)
	{
		SIZE_T wb = cx * nBitCount / 8;
		if (!wb) wb = 1;
		wb = (wb + 3) & (~3);
		return wb;
	}
	// 
	BOOL ReadRLE8Bits(CArchive &ar);
	// 
	BOOL ReadRGBBits(CArchive &ar);
	// 
	inline SIZE_T RoundTo4(SIZE_T i) { return (i + 3) & ~3; }
	// 
	BOOL CreateAssociatedPalette();
public:
	// 
	CDIBits();
	// 
	virtual ~CDIBits();
	// 
	BOOL CreateDIBits24(CSize size, LONG xPelsPerMeter = 75, LONG yPelsPerMeter = 75);
	// 
	BOOL CreateDIBitsPR(CSize size, int nBitCount, const RGBQUAD *lpColour, int nColour = 0, LONG xPelsPerMeter = 75, LONG yPelsPerMeter = 75);
	// 
	LPBYTE GetPointer() { return (LPBYTE)m_BM.bmBits; }
	// 
	LPBYTE GetPointerAt(UINT x, INT y)
	{
		x = x * m_BM.bmBitsPixel / 8;
		if (m_BM.bmHeight < 0)
			y = -y;
		if (y < 0)
				y = m_BM.bmWidthBytes * (abs(m_BM.bmHeight) - y - 1);
		else	y = m_BM.bmWidthBytes * y;
		return GetPointer() + y + x;
	}
	// 
	const BITMAP &GetBitmap();
	// 
	virtual BOOL Load(LPCTSTR lpszIn);
	// 
	BOOL Draw(CDC &dc, CRect rcDest = CRect(), CRect rcSrc = CRect());
	// 
	BOOL ReadRLE4Bits(CArchive &ar);
	// 
	BOOL Store(LPCTSTR lpszFile);
	// 
	CPalette &GetPal() { return m_Pal; }
	// 
	BOOL IsLoaded() const;

};

extern BOOL DIBits_RealizePalette(CDC &dc, CDIBits &r, BOOL fForceBackground = FALSE);

#endif // !defined(AFX_DIBITS_H__D0AEC1C4_7A4D_4CC2_BBCB_EEF468ED50B9__INCLUDED_)
