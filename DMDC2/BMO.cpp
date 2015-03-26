
#include "StdAfx.h"
#include "BMO.h"
#include "CompatDC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBMO

int CBMO::BltBMO(HDC hDC, int tx, int ty, int tcx, int tcy, int sx, int sy, DWORD rop)
{
	ASSERT(sx+tcx <= cx);
	ASSERT(sy+tcy <= cy);
	return StretchDIBits(
		hDC, 
		tx, ty, 
		tcx, tcy, 
		sx, sy, 
		tcx, tcy, 
		pv,
		pbmi, DIB_RGB_COLORS, rop
		);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBMOVec

CBMO *CBMOVec::NewBMO(DWORD key)
{
	UINT i;
	for (i=0; i<BMO_MAX; i++) {
		if (m_vec[i].key == -1 || m_vec[i].key == key) {
			m_vec[i].Close();
			m_vec[i].key = key;
			return &m_vec[i];
		}
	}
	return NULL;
}

void CBMOVec::DeleteBMO(DWORD key)
{
	UINT i;
	for (i=0; i<BMO_MAX; i++) {
		if (m_vec[i].key == key) {
			m_vec[i].Close();
		}
	}
}

void CBMOVec::Clear()
{
	UINT i;
	for (i=0; i<BMO_MAX; i++) {
		m_vec[i].Close();
	}
}

bool CBMOVec::LoadMfBMP(DWORD key, const SizeBuff &fv)
{
	CBMO *p = NewBMO(key);
	if (p == NULL) return false;

	CBM4BMP loader(*p);
	if (loader.LoadFromMem(const_cast<BYTE *>(fv.GetData()), fv.GetSize()))
		return true;
	DeleteBMO(key);
	return false;
}

bool CBMOVec::LoadMfRaw(DWORD key, const SizeBuff &fv)
{
	CBMO *p = NewBMO(key);
	if (p == NULL) return false;

	if (!p->fv.Copy(fv)) return false;

	return true;
}

CBMO *CBMOVec::FindBMO(DWORD key)
{
	UINT i;
	for (i=0; i<BMO_MAX; i++) {
		if (m_vec[i].key == key) {
			return &m_vec[i];
		}
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBMfo

int CBMfo::ReadByte()
{
	int r = -1;
	if (nCur < nLen) {
		r = pData[nCur];
		nCur++;
	}
	return r;
}

size_t CBMfo::Read(void *sz, size_t ch)
{
	ch = min(ch, nLen -nCur);
	memcpy(sz, pData +nCur, ch);
	nCur += ch;
	return ch;
}

bool CBMfo::SurelyRead(void *sz, size_t ch)
{
	if (nLen -nCur < ch)
		return false;
	memcpy(sz, pData +nCur, ch);
	nCur += ch;
	return true;
}

bool CBMfo::Write(const void *sz, size_t ch)
{
	if (nLen -nCur < ch)
		return false;
	memcpy(pData +nCur, sz, ch);
	nCur += ch;
	return true;
}

bool CBMfo::SurelySeekTo(size_t offset)
{
	if (nLen < offset)
		return false;
	nCur = offset;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// XBM4RLE

#define REV_BIH_Y 1

#if 0
#	define RLE_Y(y, cy) (y)
#else
#	define RLE_Y(y, cy) ((cy) -(y) -1)
#endif

#if 0
#	define RGB_Y(y, cy) (y)
#else
#	define RGB_Y(y, cy) ((cy) -(y) -1)
#endif

bool XBM4RLE::Write4(BYTE *pv, BYTE size)
{
	BYTE *pHorz = pVert + pitch * RLE_Y(y, cy) + (x / 2);

	int cc = 0;

	while (size != 0) {
		if (!(y < cy))
			return false;
		if (!(x < cx)) {
			y++;
			x -= cx;
			pHorz = pVert + pitch * RLE_Y(y, cy) + (x / 2);
			continue;
		}

		BYTE c = *pv;
		if (cc & 1) {
			c &= 15;

			pv++;
		} else {
			c /= 16;
		}
		cc++;

		if (x & 1) {
			// ?? yy
			BYTE z = *pHorz;
			z = (z & 0xF0) | (c);
			*pHorz = z;

			pHorz++;
		} else {
			// xx ??
			BYTE z = *pHorz;
			z = (z & 0x0F) | (c << 4);
			*pHorz = z;
		}
		x++;
		size--;
	}

	return true;
}

bool XBM4RLE::Write8(BYTE *pv, BYTE size)
{
	BYTE *pHorz = pVert + pitch * RLE_Y(y, cy) + x;

	int cc = 0;

	while (size != 0) {
		if (!(y < cy))
			return false;
		if (!(x < cx)) {
			y++;
			x -= cx;
			pHorz = pVert + pitch * RLE_Y(y, cy) + x;
			continue;
		}
		*pHorz = pv[cc]; pHorz++;
		cc++;
		x++;
		size--;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBM4BMP

bool CBM4BMP::LoadFromMem(LPVOID pvMem, DWORD cbMem)
{
	CBMfo f(pvMem, cbMem);

	BITMAPFILEHEADER bfh;
	if (!f.SurelyRead(&bfh, sizeof(bfh)))
		return false;
	if (bfh.bfType != *(const WORD *)"BM")
		return false;

	BITMAPINFO *pbmiOld = (BITMAPINFO *)&rbmo.cbmiOld[0];
	if (!f.SurelyRead(pbmiOld, sizeof(BITMAPINFOHEADER)))
		return false;
	if (pbmiOld->bmiHeader.biSize != sizeof(BITMAPINFOHEADER))
		return false;
	int nPalLen = GetPalCnt(pbmiOld->bmiHeader);

	if (!f.SurelyRead(&pbmiOld->bmiColors[0], sizeof(RGBQUAD) * nPalLen))
		return false;
	if (!f.SurelySeekTo(bfh.bfOffBits))
		return false;

	rbmo.cx = pbmiOld->bmiHeader.biWidth;
	rbmo.cy = pbmiOld->bmiHeader.biHeight;
	rbmo.bpp = pbmiOld->bmiHeader.biBitCount;
	rbmo.pitch = RTo4((rbmo.bpp * rbmo.cx + 7) / 8);

	if (rbmo.cx < 1 || rbmo.cy < 1)
		return false;
	if (!rbmo.fv.Alloc(rbmo.pitch * rbmo.cy)) {
		return false;
	}

	const DWORD biCompression = pbmiOld->bmiHeader.biCompression;

	BYTE *pv = (BYTE *)(rbmo.pv = rbmo.fv.GetData());

	rbmo.pbmi = (BITMAPINFO *)rbmo.cbmiNew;
	memcpy(rbmo.cbmiNew, rbmo.cbmiOld, sizeof(rbmo.cbmiNew));
	rbmo.pbmi->bmiHeader.biCompression = BI_RGB;
	rbmo.pbmi->bmiHeader.biSizeImage = 0;
#if REV_BIH_Y
	rbmo.pbmi->bmiHeader.biHeight = -rbmo.pbmi->bmiHeader.biHeight;
#endif

	switch (rbmo.bpp) {
	case 4:
		{
			if (false);
			else if (biCompression == BI_RGB) {
				//
				for (int y = 0, cy = rbmo.cy; y < cy; y++) {
					if (!f.SurelyRead(pv +rbmo.pitch * RGB_Y(y, cy), rbmo.pitch))
						return false;
				}
				//
			}
			else if (biCompression == BI_RLE4) {
				//
				XBM4RLE rle4(0, 0, rbmo.cx, rbmo.cy, rbmo.pitch, pv);
				BYTE buffer[255];
				while (true) {
					int r = f.ReadByte();
					if (r < 0)
						return false;
					if (r == 0) {
						r = f.ReadByte();
						if (r < 0)
							return false;
						if (false);
						else if (r == 0) {
							// end of line
							rle4.y++;
							rle4.x = 0;
						}
						else if (r == 1) {
							// end of bitmap
							break;
						}
						else if (r == 2) {
							// delta
							int sx = f.ReadByte();
							int sy = f.ReadByte();
							if (sx < 0 || sy < 0)
								return false;
							rle4.x += sx;
							rle4.y += sy;
						}
						else {
							int size = (r+1) / 2;
							int skip = (size & 1);
							if (!f.SurelyRead(buffer, size))
								return false;
							if (!rle4.Write4(buffer, r))
								return false;
							if (skip != 0) f.ReadByte();
						}
					} else {
						int x = f.ReadByte();
						if (x < 0)
							return false;
						memset(buffer, x, r);
						if (!rle4.Write4(buffer, r))
							return false;
					}
				}
				//
				printf("");
			}
			else {
				return false;
			}
			break;
		}
	case 8:
		{
			if (false);
			else if (biCompression == BI_RGB) {
				//
				for (int y = 0, cy = rbmo.cy; y < cy; y++) {
					if (!f.SurelyRead(pv +rbmo.pitch * RGB_Y(y, cy), rbmo.pitch))
						return false;
				}
				//
			}
			else if (biCompression == BI_RLE8) {
				//
				XBM4RLE rle8(0, 0, rbmo.cx, rbmo.cy, rbmo.pitch, pv);
				BYTE buffer[255];
				while (true) {
					int r = f.ReadByte();
					if (r < 0)
						return false;
					if (r == 0) {
						r = f.ReadByte();
						if (r < 0)
							return false;
						if (false);
						else if (r == 0) {
							// end of line
							rle8.y++;
							rle8.x = 0;
						}
						else if (r == 1) {
							// end of bitmap
							break;
						}
						else if (r == 2) {
							// delta
							int sx = f.ReadByte();
							int sy = f.ReadByte();
							if (sx < 0 || sy < 0)
								return false;
							rle8.x += sx;
							rle8.y += sy;
						}
						else {
							int skip = (r & 1);
							if (!f.SurelyRead(buffer, r))
								return false;
							if (!rle8.Write8(buffer, r))
								return false;
							if (skip != 0) f.ReadByte();
						}
					} else {
						int x = f.ReadByte();
						if (x < 0)
							return false;
						memset(buffer, x, r);
						if (!rle8.Write8(buffer, r))
							return false;
					}
				}
				//
				printf("");
			}
			else {
				return false;
			}
			break;
		}
	default:
		return false;
	}

	return true;
}

bool CBM4BMP::LoadFromFile(LPCTSTR psz)
{
	fm fmo;
	fmo.Close();
	if (fmo.Open(psz))
		return false;
	LPVOID pvMem = fmo.MapView();
	DWORD cbMem = fmo.GetFileSize();
	return LoadFromMem(pvMem, cbMem);
}
