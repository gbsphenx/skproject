
#ifndef _BMO_H_
#define _BMO_H_

#include "FM.h"
#include "SizeVuff.h"

struct CBMO {
	// 
	DWORD key;

	// 
	void *pv;
	// 
	BITMAPINFO *pbmi;
	// 
	int cx, cy, pitch;
	// 
	char bpp;
	// 
	char cbmiNew[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];
	// 
	char cbmiOld[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];

	// 
	SizeBuff fv;

	// 
	CBMO()
	{
		Close();
	}
	// 
	void Close()
	{
		key = -1;
		pv = NULL;
		pbmi = NULL;
		cx = cy = pitch = 0;
		bpp = 0;
		fv.Free();
	}
	// 
	int BltBMO(HDC hDC, int tx, int ty, int tcx, int tcy, int sx, int sy, DWORD rop);

private:
	// 
	CBMO(const CBMO &);
	// 
	void operator =(const CBMO &);

};

#define BMO_MAX 500

class CBMOVec
{
public:
	// 
	CBMO m_vec[BMO_MAX];

	// 
	void Clear();
	// 
	bool LoadMfBMP(DWORD key, const SizeBuff &fv);
	// 
	bool LoadMfRaw(DWORD key, const SizeBuff &fv);
	// 
	CBMO *NewBMO(DWORD key);
	// 
	void DeleteBMO(DWORD key);
	// 
	CBMO *FindBMO(DWORD key);
};

class CBMfo
{
public:
	// 
	BYTE *pData;
	// 
	size_t nCur, nLen;

	// 
	CBMfo(): pData(NULL), nCur(0), nLen(0) { }
	// 
	CBMfo(void *sz, size_t ch): pData((BYTE *)sz), nCur(0), nLen(ch) { }

	// 
	virtual int ReadByte();
	// 
	virtual size_t Read(void *sz, size_t ch);
	// 
	virtual bool SurelyRead(void *sz, size_t ch);
	// 
	virtual bool Write(const void *sz, size_t ch);
	// 
	virtual bool SurelySeekTo(size_t offset);
};

class XBM4RLE
{
public:
	// 
	int x, y, cx, cy, pitch;
	// 
	BYTE *pVert;

	//  
	XBM4RLE(int x, int y, int cx, int cy, int pitch, BYTE *pVert)
		: x(x), y(y), cx(cx), cy(cy), pitch(pitch), pVert(pVert)
	{

	}

	// 
	bool Write8(BYTE *pv, BYTE size);
	// 
	bool Write4(BYTE *pv, BYTE size);
};

class CBM4BMP
{
	// 
	CBMO &rbmo;

	// 
	static int GetPalCnt(const BITMAPINFOHEADER &rbih)
	{
		int r = 0;
		switch (rbih.biBitCount) {
		case 1:
		case 4:
		case 8:
			r = rbih.biClrUsed;
			if (r == 0)
				r = 1 << rbih.biBitCount;
			break;
		}
		return r;
	}
	// 
	static int RTo4(int r)
	{
		return (r + 3) & (~3);
	}

public:
	// 
	CBM4BMP(CBMO &rbmo): rbmo(rbmo) { }
	// 
	bool LoadFromMem(LPVOID pvMem, DWORD cbMem);
	// 
	bool LoadFromFile(LPCTSTR psz);

};

#endif // _BMO_H_
