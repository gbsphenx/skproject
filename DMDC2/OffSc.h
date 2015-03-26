
#ifndef _OFFSC_H_
#define _OFFSC_H_

#include "SizeVuff.h"

#include "BMO.h"

#define _MY_TERNARY 0

struct StrucLineXTox {
	const BYTE *pSrc;
	BYTE *pNew;
	RGBQUAD *p4;
	int srcx;
	int newx;
	int cx;
};

class COffScreen
{
	// 
	typedef void (*LineXTox)(StrucLineXTox &r);

	// 
	SizeBuff fv;
	// 
	int cx, cy, bpp, pitch;
	// 
	char cbi[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];
	// 
	BITMAPINFO *pbi;
	// 
	void *pvmap;

	// 
	CBitmap cbm;

#if _MY_TERNARY
	// 
	static void Line4To24(StrucLineXTox &r);
	// 
	static void Line8To24(StrucLineXTox &r);
	// 
	static void Line4To24_DSa(StrucLineXTox &r);
	// 
	static void Line8To24_DSa(StrucLineXTox &r);
	// 
	static void Line4To24_DSo(StrucLineXTox &r);
	// 
	static void Line8To24_DSo(StrucLineXTox &r);
#endif

	// 
	static int RTo4(int r)
	{
		return (r + 3) & (~3);
	}

public:
	// 
	~COffScreen()
	{
		Close();
	}
	// 
	void Clear();
	// 
	void Close();
	// 
	bool Create24Screen();
	// 
	int FlipMe(int newx, int newy, CBMO &src, int srcx, int srcy, int srccx, int srccy, DWORD rop);
	// 
	int FlipTo(HDC hDC, int newx, int newy, int cx, int cy, int srcx, int srcy);

};

#endif // _OFFSC_H_
