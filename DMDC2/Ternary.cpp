
#include "StdAfx.h"
#include "Ternary.h"

#pragma warning(disable: 4101)

// ->

// cpp

void Line4To24_0(StrucLineXTox &r)
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
		*pNew = BYTE(0x00); pNew++;
		*pNew = BYTE(0x00); pNew++;
		*pNew = BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_DPSoon(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  | BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen | BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   | BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSona(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  | BYTE(0x00)) ^ BYTE(0xFF)) & *pNew; pNew++;
		*pNew = ((c4.rgbGreen | BYTE(0x00)) ^ BYTE(0xFF)) & *pNew; pNew++;
		*pNew = ((c4.rgbRed   | BYTE(0x00)) ^ BYTE(0xFF)) & *pNew; pNew++;
		x1++;
	}
}

void Line4To24_PSon(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbGreen | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbRed   | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SDPona(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) | *pNew) ^ BYTE(0xFF)) & c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) | *pNew) ^ BYTE(0xFF)) & c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) | *pNew) ^ BYTE(0xFF)) & c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_DPon(StrucLineXTox &r)
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
		*pNew = (BYTE(0x00) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (BYTE(0x00) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (BYTE(0x00) | *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PDSxnon(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  ^ *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PDSaon(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  & *pNew) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen & *pNew) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   & *pNew) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SDPnaa(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) & c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) & c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) & c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_PDSxon(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ *pNew) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPna(StrucLineXTox &r)
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
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) & *pNew; pNew++;
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) & *pNew; pNew++;
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) & *pNew; pNew++;
		x1++;
	}
}

void Line4To24_PSDnaon(StrucLineXTox &r)
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
		*pNew = (((*pNew ^ BYTE(0xFF)) & c4.rgbBlue ) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) & c4.rgbGreen) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) & c4.rgbRed  ) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SPna(StrucLineXTox &r)
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
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbBlue ; pNew++;
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbGreen; pNew++;
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_PDSnaon(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  ^ BYTE(0xFF)) & *pNew) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0xFF)) & *pNew) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0xFF)) & *pNew) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PDSonon(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_Pn(StrucLineXTox &r)
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
		*pNew = BYTE(0x00) ^ BYTE(0xFF); pNew++;
		*pNew = BYTE(0x00) ^ BYTE(0xFF); pNew++;
		*pNew = BYTE(0x00) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PDSona(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  | *pNew) ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		*pNew = ((c4.rgbGreen | *pNew) ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		*pNew = ((c4.rgbRed   | *pNew) ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_DSon(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbGreen | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbRed   | *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SDPxnon(StrucLineXTox &r)
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
		*pNew = (((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) | c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) | c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) | c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SDPaon(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) & *pNew) | c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((BYTE(0x00) & *pNew) | c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((BYTE(0x00) & *pNew) | c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSxnon(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  ^ BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSaon(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  & BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen & BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   & BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PSDPSanaxx(StrucLineXTox &r)
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
		*pNew = ((((c4.rgbBlue  & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbBlue ) ^ BYTE(0x00); pNew++;
		*pNew = ((((c4.rgbGreen & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbGreen) ^ BYTE(0x00); pNew++;
		*pNew = ((((c4.rgbRed   & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbRed  ) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_SSPxDSxaxn(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  ^ *pNew) & (BYTE(0x00) ^ c4.rgbBlue )) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ *pNew) & (BYTE(0x00) ^ c4.rgbGreen)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ *pNew) & (BYTE(0x00) ^ c4.rgbRed  )) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SPxPDxa(StrucLineXTox &r)
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
		*pNew = (*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbBlue ); pNew++;
		*pNew = (*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbGreen); pNew++;
		*pNew = (*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbRed  ); pNew++;
		x1++;
	}
}

void Line4To24_SDPSanaxn(StrucLineXTox &r)
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
		*pNew = ((((c4.rgbBlue  & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PDSPaox(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) & c4.rgbBlue ) | *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) & c4.rgbGreen) | *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) & c4.rgbRed  ) | *pNew) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_SDPSxaxn(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  ^ BYTE(0x00)) & *pNew) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0x00)) & *pNew) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0x00)) & *pNew) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PSDPaox(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) & *pNew) | c4.rgbBlue ) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) & *pNew) | c4.rgbGreen) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) & *pNew) | c4.rgbRed  ) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_DSPDxaxn(StrucLineXTox &r)
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
		*pNew = (((*pNew ^ BYTE(0x00)) & c4.rgbBlue ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0x00)) & c4.rgbGreen) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0x00)) & c4.rgbRed  ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PDSox(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  | *pNew) ^ BYTE(0x00); pNew++;
		*pNew = (c4.rgbGreen | *pNew) ^ BYTE(0x00); pNew++;
		*pNew = (c4.rgbRed   | *pNew) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_PDSoan(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  | *pNew) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen | *pNew) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   | *pNew) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSnaa(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ BYTE(0xFF)) & BYTE(0x00)) & *pNew; pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0xFF)) & BYTE(0x00)) & *pNew; pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0xFF)) & BYTE(0x00)) & *pNew; pNew++;
		x1++;
	}
}

void Line4To24_SDPxon(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) ^ *pNew) | c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) | c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) | c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DSna(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  ^ BYTE(0xFF)) & *pNew; pNew++;
		*pNew = (c4.rgbGreen ^ BYTE(0xFF)) & *pNew; pNew++;
		*pNew = (c4.rgbRed   ^ BYTE(0xFF)) & *pNew; pNew++;
		x1++;
	}
}

void Line4To24_SPDnaon(StrucLineXTox &r)
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
		*pNew = (((*pNew ^ BYTE(0xFF)) & BYTE(0x00)) | c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) & BYTE(0x00)) | c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) & BYTE(0x00)) | c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SPxDSxa(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  ^ *pNew) & (BYTE(0x00) ^ c4.rgbBlue ); pNew++;
		*pNew = (c4.rgbGreen ^ *pNew) & (BYTE(0x00) ^ c4.rgbGreen); pNew++;
		*pNew = (c4.rgbRed   ^ *pNew) & (BYTE(0x00) ^ c4.rgbRed  ); pNew++;
		x1++;
	}
}

void Line4To24_PDSPanaxn(StrucLineXTox &r)
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
		*pNew = ((((BYTE(0x00) & c4.rgbBlue ) ^ BYTE(0xFF)) & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((BYTE(0x00) & c4.rgbGreen) ^ BYTE(0xFF)) & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((BYTE(0x00) & c4.rgbRed  ) ^ BYTE(0xFF)) & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SDPSaox(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  & BYTE(0x00)) | *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen & BYTE(0x00)) | *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   & BYTE(0x00)) | *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SDPSxnox(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  ^ BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_DPSxa(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  ^ BYTE(0x00)) & *pNew; pNew++;
		*pNew = (c4.rgbGreen ^ BYTE(0x00)) & *pNew; pNew++;
		*pNew = (c4.rgbRed   ^ BYTE(0x00)) & *pNew; pNew++;
		x1++;
	}
}

void Line4To24_PSDPSaoxxn(StrucLineXTox &r)
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
		*pNew = ((((c4.rgbBlue  & BYTE(0x00)) | *pNew) ^ c4.rgbBlue ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen & BYTE(0x00)) | *pNew) ^ c4.rgbGreen) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   & BYTE(0x00)) | *pNew) ^ c4.rgbRed  ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSana(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew; pNew++;
		*pNew = ((c4.rgbGreen & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew; pNew++;
		*pNew = ((c4.rgbRed   & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew; pNew++;
		x1++;
	}
}

void Line4To24_SSPxPDxaxn(StrucLineXTox &r)
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
		*pNew = (((*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbBlue )) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbGreen)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbRed  )) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SPDSoax(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  | *pNew) & BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen | *pNew) & BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   | *pNew) & BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_PSDnox(StrucLineXTox &r)
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
		*pNew = ((*pNew ^ BYTE(0xFF)) | c4.rgbBlue ) ^ BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) | c4.rgbGreen) ^ BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) | c4.rgbRed  ) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_PSDPxox(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) ^ *pNew) | c4.rgbBlue ) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) | c4.rgbGreen) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) | c4.rgbRed  ) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_PSDnoan(StrucLineXTox &r)
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
		*pNew = (((*pNew ^ BYTE(0xFF)) | c4.rgbBlue ) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) | c4.rgbGreen) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) | c4.rgbRed  ) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PSna(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		*pNew = (c4.rgbGreen ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		*pNew = (c4.rgbRed   ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_SDPnaon(StrucLineXTox &r)
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
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) | c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) | c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) | c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SDPSoox(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  | BYTE(0x00)) | *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen | BYTE(0x00)) | *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   | BYTE(0x00)) | *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_Sn(StrucLineXTox &r)
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
		*pNew = c4.rgbBlue  ^ BYTE(0xFF); pNew++;
		*pNew = c4.rgbGreen ^ BYTE(0xFF); pNew++;
		*pNew = c4.rgbRed   ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SPDSaox(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  & *pNew) | BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen & *pNew) | BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   & *pNew) | BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SPDSxnox(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  ^ *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = (((c4.rgbGreen ^ *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = (((c4.rgbRed   ^ *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SDPox(StrucLineXTox &r)
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
		*pNew = (BYTE(0x00) | *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = (BYTE(0x00) | *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = (BYTE(0x00) | *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SDPoan(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) | *pNew) & c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((BYTE(0x00) | *pNew) & c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((BYTE(0x00) | *pNew) & c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PSDPoax(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) | *pNew) & c4.rgbBlue ) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) | *pNew) & c4.rgbGreen) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) | *pNew) & c4.rgbRed  ) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_SPDnox(StrucLineXTox &r)
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
		*pNew = ((*pNew ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SPDSxox(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ *pNew) | BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) | BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) | BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SPDnoan(StrucLineXTox &r)
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
		*pNew = (((*pNew ^ BYTE(0xFF)) | BYTE(0x00)) & c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) | BYTE(0x00)) & c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) | BYTE(0x00)) & c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PSx(StrucLineXTox &r)
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
		*pNew = c4.rgbBlue  ^ BYTE(0x00); pNew++;
		*pNew = c4.rgbGreen ^ BYTE(0x00); pNew++;
		*pNew = c4.rgbRed   ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_SPDSonox(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = (((c4.rgbGreen | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = (((c4.rgbRed   | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SPDSnaox(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  ^ BYTE(0xFF)) & *pNew) | BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0xFF)) & *pNew) | BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0xFF)) & *pNew) | BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_PSan(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbGreen & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbRed   & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PSDnaa(StrucLineXTox &r)
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
		*pNew = ((*pNew ^ BYTE(0xFF)) & c4.rgbBlue ) & BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) & c4.rgbGreen) & BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) & c4.rgbRed  ) & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_DPSxon(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SDxPDxa(StrucLineXTox &r)
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
		*pNew = (*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbBlue ); pNew++;
		*pNew = (*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbGreen); pNew++;
		*pNew = (*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbRed  ); pNew++;
		x1++;
	}
}

void Line4To24_SPDSanaxn(StrucLineXTox &r)
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
		*pNew = ((((c4.rgbBlue  & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SDna(StrucLineXTox &r)
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
		*pNew = (*pNew ^ BYTE(0xFF)) & c4.rgbBlue ; pNew++;
		*pNew = (*pNew ^ BYTE(0xFF)) & c4.rgbGreen; pNew++;
		*pNew = (*pNew ^ BYTE(0xFF)) & c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_DPSnaon(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DSPDaox(StrucLineXTox &r)
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
		*pNew = ((*pNew & BYTE(0x00)) | c4.rgbBlue ) ^ *pNew; pNew++;
		*pNew = ((*pNew & BYTE(0x00)) | c4.rgbGreen) ^ *pNew; pNew++;
		*pNew = ((*pNew & BYTE(0x00)) | c4.rgbRed  ) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_PSDPxaxn(StrucLineXTox &r)
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
		*pNew = (((BYTE(0x00) ^ *pNew) & c4.rgbBlue ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ *pNew) & c4.rgbGreen) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ *pNew) & c4.rgbRed  ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SDPxa(StrucLineXTox &r)
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
		*pNew = (BYTE(0x00) ^ *pNew) & c4.rgbBlue ; pNew++;
		*pNew = (BYTE(0x00) ^ *pNew) & c4.rgbGreen; pNew++;
		*pNew = (BYTE(0x00) ^ *pNew) & c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_PDSPDaoxxn(StrucLineXTox &r)
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
		*pNew = ((((*pNew & BYTE(0x00)) | c4.rgbBlue ) ^ *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((*pNew & BYTE(0x00)) | c4.rgbGreen) ^ *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((*pNew & BYTE(0x00)) | c4.rgbRed  ) ^ *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSDoax(StrucLineXTox &r)
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
		*pNew = ((*pNew | c4.rgbBlue ) & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((*pNew | c4.rgbGreen) & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((*pNew | c4.rgbRed  ) & BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_PDSnox(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ BYTE(0xFF)) | *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0xFF)) | *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0xFF)) | *pNew) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_SDPana(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) & *pNew) ^ BYTE(0xFF)) & c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) & *pNew) ^ BYTE(0xFF)) & c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) & *pNew) ^ BYTE(0xFF)) & c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SSPxDSxoxn(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  ^ *pNew) | (BYTE(0x00) ^ c4.rgbBlue )) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ *pNew) | (BYTE(0x00) ^ c4.rgbGreen)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ *pNew) | (BYTE(0x00) ^ c4.rgbRed  )) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PDSPxox(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) ^ c4.rgbBlue ) | *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) ^ c4.rgbGreen) | *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) ^ c4.rgbRed  ) | *pNew) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_PDSnoan(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  ^ BYTE(0xFF)) | *pNew) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0xFF)) | *pNew) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0xFF)) | *pNew) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PDna(StrucLineXTox &r)
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
		*pNew = (*pNew ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		*pNew = (*pNew ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		*pNew = (*pNew ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_DSPnaon(StrucLineXTox &r)
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
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbBlue ) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbGreen) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbRed  ) | *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSDaox(StrucLineXTox &r)
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
		*pNew = ((*pNew & c4.rgbBlue ) | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((*pNew & c4.rgbGreen) | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((*pNew & c4.rgbRed  ) | BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_SPDSxaxn(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  ^ *pNew) & BYTE(0x00)) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ *pNew) & BYTE(0x00)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ *pNew) & BYTE(0x00)) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSonon(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_Dn(StrucLineXTox &r)
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
		*pNew = *pNew ^ BYTE(0xFF); pNew++;
		*pNew = *pNew ^ BYTE(0xFF); pNew++;
		*pNew = *pNew ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSox(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (c4.rgbGreen | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (c4.rgbRed   | BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_DPSoan(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  | BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen | BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   | BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PDSPoax(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) | c4.rgbBlue ) & *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) | c4.rgbGreen) & *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) | c4.rgbRed  ) & *pNew) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_DPSnox(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ BYTE(0xFF)) | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0xFF)) | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0xFF)) | BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_DPx(StrucLineXTox &r)
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
		*pNew = BYTE(0x00) ^ *pNew; pNew++;
		*pNew = BYTE(0x00) ^ *pNew; pNew++;
		*pNew = BYTE(0x00) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_DPSDonox(StrucLineXTox &r)
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
		*pNew = (((*pNew | c4.rgbBlue ) ^ BYTE(0xFF)) | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((*pNew | c4.rgbGreen) ^ BYTE(0xFF)) | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((*pNew | c4.rgbRed  ) ^ BYTE(0xFF)) | BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_DPSDxox(StrucLineXTox &r)
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
		*pNew = ((*pNew ^ c4.rgbBlue ) | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((*pNew ^ c4.rgbGreen) | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((*pNew ^ c4.rgbRed  ) | BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_DPSnoan(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSDnaox(StrucLineXTox &r)
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
		*pNew = (((*pNew ^ BYTE(0xFF)) & c4.rgbBlue ) | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) & c4.rgbGreen) | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) & c4.rgbRed  ) | BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_DPan(StrucLineXTox &r)
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
		*pNew = (BYTE(0x00) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (BYTE(0x00) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (BYTE(0x00) & *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PDSxa(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  ^ *pNew) & BYTE(0x00); pNew++;
		*pNew = (c4.rgbGreen ^ *pNew) & BYTE(0x00); pNew++;
		*pNew = (c4.rgbRed   ^ *pNew) & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_DSPDSaoxxn(StrucLineXTox &r)
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
		*pNew = ((((c4.rgbBlue  & *pNew) | BYTE(0x00)) ^ c4.rgbBlue ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen & *pNew) | BYTE(0x00)) ^ c4.rgbGreen) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   & *pNew) | BYTE(0x00)) ^ c4.rgbRed  ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DSPDoax(StrucLineXTox &r)
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
		*pNew = ((*pNew | BYTE(0x00)) & c4.rgbBlue ) ^ *pNew; pNew++;
		*pNew = ((*pNew | BYTE(0x00)) & c4.rgbGreen) ^ *pNew; pNew++;
		*pNew = ((*pNew | BYTE(0x00)) & c4.rgbRed  ) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_SDPnox(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SDPSoax(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  | BYTE(0x00)) & *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen | BYTE(0x00)) & *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   | BYTE(0x00)) & *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_DSPnox(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbBlue ) ^ *pNew; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbGreen) ^ *pNew; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbRed  ) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_DSx(StrucLineXTox &r)
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
		*pNew = c4.rgbBlue  ^ *pNew; pNew++;
		*pNew = c4.rgbGreen ^ *pNew; pNew++;
		*pNew = c4.rgbRed   ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_SDPSonox(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = (((c4.rgbGreen | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = (((c4.rgbRed   | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_DSPDSonoxxn(StrucLineXTox &r)
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
		*pNew = (((((c4.rgbBlue  | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbBlue ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((((c4.rgbGreen | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbGreen) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((((c4.rgbRed   | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbRed  ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PDSxxn(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSax(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (c4.rgbGreen & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (c4.rgbRed   & BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_PSDPSoaxxn(StrucLineXTox &r)
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
		*pNew = ((((c4.rgbBlue  | BYTE(0x00)) & *pNew) ^ c4.rgbBlue ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen | BYTE(0x00)) & *pNew) ^ c4.rgbGreen) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   | BYTE(0x00)) & *pNew) ^ c4.rgbRed  ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SDPax(StrucLineXTox &r)
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
		*pNew = (BYTE(0x00) & *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = (BYTE(0x00) & *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = (BYTE(0x00) & *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_PDSPDoaxxn(StrucLineXTox &r)
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
		*pNew = ((((*pNew | BYTE(0x00)) & c4.rgbBlue ) ^ *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((*pNew | BYTE(0x00)) & c4.rgbGreen) ^ *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((*pNew | BYTE(0x00)) & c4.rgbRed  ) ^ *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SDPSnoax(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_PDSxnan(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  ^ *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PDSana(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  & *pNew) ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		*pNew = ((c4.rgbGreen & *pNew) ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		*pNew = ((c4.rgbRed   & *pNew) ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_SSDxPDxaxn(StrucLineXTox &r)
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
		*pNew = (((*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbBlue )) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbGreen)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbRed  )) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SDPSxox(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ BYTE(0x00)) | *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0x00)) | *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0x00)) | *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SDPnoan(StrucLineXTox &r)
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
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) & c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) & c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) & c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DSPDxox(StrucLineXTox &r)
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
		*pNew = ((*pNew ^ BYTE(0x00)) | c4.rgbBlue ) ^ *pNew; pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) | c4.rgbGreen) ^ *pNew; pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) | c4.rgbRed  ) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_DSPnoan(StrucLineXTox &r)
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
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbBlue ) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbGreen) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbRed  ) & *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SDPSnaox(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_DSan(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbGreen & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbRed   & *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PDSax(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  & *pNew) ^ BYTE(0x00); pNew++;
		*pNew = (c4.rgbGreen & *pNew) ^ BYTE(0x00); pNew++;
		*pNew = (c4.rgbRed   & *pNew) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_DSPDSoaxxn(StrucLineXTox &r)
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
		*pNew = ((((c4.rgbBlue  | *pNew) & BYTE(0x00)) ^ c4.rgbBlue ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen | *pNew) & BYTE(0x00)) ^ c4.rgbGreen) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   | *pNew) & BYTE(0x00)) ^ c4.rgbRed  ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSDnoax(StrucLineXTox &r)
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
		*pNew = (((*pNew ^ BYTE(0xFF)) | c4.rgbBlue ) & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) | c4.rgbGreen) & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) | c4.rgbRed  ) & BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_SDPxnan(StrucLineXTox &r)
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
		*pNew = (((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) & c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) & c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) & c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SPDSnoax(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  ^ BYTE(0xFF)) | *pNew) & BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0xFF)) | *pNew) & BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0xFF)) | *pNew) & BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_DPSxnan(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  ^ BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SPxDSxo(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  ^ *pNew) | (BYTE(0x00) ^ c4.rgbBlue ); pNew++;
		*pNew = (c4.rgbGreen ^ *pNew) | (BYTE(0x00) ^ c4.rgbGreen); pNew++;
		*pNew = (c4.rgbRed   ^ *pNew) | (BYTE(0x00) ^ c4.rgbRed  ); pNew++;
		x1++;
	}
}

void Line4To24_DPSaan(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  & BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen & BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   & BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSaa(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  & BYTE(0x00)) & *pNew; pNew++;
		*pNew = (c4.rgbGreen & BYTE(0x00)) & *pNew; pNew++;
		*pNew = (c4.rgbRed   & BYTE(0x00)) & *pNew; pNew++;
		x1++;
	}
}

void Line4To24_SPxDSxon(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ *pNew) | (BYTE(0x00) ^ c4.rgbBlue )) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) | (BYTE(0x00) ^ c4.rgbGreen)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) | (BYTE(0x00) ^ c4.rgbRed  )) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSxna(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ BYTE(0x00)) ^ BYTE(0xFF)) & *pNew; pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0x00)) ^ BYTE(0xFF)) & *pNew; pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0x00)) ^ BYTE(0xFF)) & *pNew; pNew++;
		x1++;
	}
}

void Line4To24_SPDSnoaxn(StrucLineXTox &r)
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
		*pNew = ((((c4.rgbBlue  ^ BYTE(0xFF)) | *pNew) & BYTE(0x00)) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen ^ BYTE(0xFF)) | *pNew) & BYTE(0x00)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   ^ BYTE(0xFF)) | *pNew) & BYTE(0x00)) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SDPxna(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) & c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) & c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) & c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_PDSPnoaxn(StrucLineXTox &r)
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
		*pNew = ((((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbBlue ) & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbGreen) & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbRed  ) & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DSPDSoaxx(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  | *pNew) & BYTE(0x00)) ^ c4.rgbBlue ) ^ *pNew; pNew++;
		*pNew = (((c4.rgbGreen | *pNew) & BYTE(0x00)) ^ c4.rgbGreen) ^ *pNew; pNew++;
		*pNew = (((c4.rgbRed   | *pNew) & BYTE(0x00)) ^ c4.rgbRed  ) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_PDSaxn(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DSa(StrucLineXTox &r)
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
		*pNew = c4.rgbBlue  & *pNew; pNew++;
		*pNew = c4.rgbGreen & *pNew; pNew++;
		*pNew = c4.rgbRed   & *pNew; pNew++;
		x1++;
	}
}

void Line4To24_SDPSnaoxn(StrucLineXTox &r)
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
		*pNew = ((((c4.rgbBlue  ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DSPnoa(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbBlue ) & *pNew; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbGreen) & *pNew; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbRed  ) & *pNew; pNew++;
		x1++;
	}
}

void Line4To24_DSPDxoxn(StrucLineXTox &r)
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
		*pNew = (((*pNew ^ BYTE(0x00)) | c4.rgbBlue ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0x00)) | c4.rgbGreen) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0x00)) | c4.rgbRed  ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SDPnoa(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) & c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) & c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) & c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SDPSxoxn(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  ^ BYTE(0x00)) | *pNew) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0x00)) | *pNew) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0x00)) | *pNew) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SSDxPDxax(StrucLineXTox &r)
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
		*pNew = ((*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbBlue )) ^ c4.rgbBlue ; pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbGreen)) ^ c4.rgbGreen; pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbRed  )) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_PDSanan(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PDSxna(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ *pNew) ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_SDPSnoaxn(StrucLineXTox &r)
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
		*pNew = ((((c4.rgbBlue  ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSDPoaxx(StrucLineXTox &r)
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
		*pNew = (((BYTE(0x00) | *pNew) & c4.rgbBlue ) ^ BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((BYTE(0x00) | *pNew) & c4.rgbGreen) ^ BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((BYTE(0x00) | *pNew) & c4.rgbRed  ) ^ BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_SPDaxn(StrucLineXTox &r)
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
		*pNew = ((*pNew & BYTE(0x00)) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((*pNew & BYTE(0x00)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((*pNew & BYTE(0x00)) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PSDPSoaxx(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  | BYTE(0x00)) & *pNew) ^ c4.rgbBlue ) ^ BYTE(0x00); pNew++;
		*pNew = (((c4.rgbGreen | BYTE(0x00)) & *pNew) ^ c4.rgbGreen) ^ BYTE(0x00); pNew++;
		*pNew = (((c4.rgbRed   | BYTE(0x00)) & *pNew) ^ c4.rgbRed  ) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_DPSaxn(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  & BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen & BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   & BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSxx(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  ^ BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (c4.rgbGreen ^ BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (c4.rgbRed   ^ BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_PSDPSonoxx(StrucLineXTox &r)
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
		*pNew = ((((c4.rgbBlue  | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbBlue ) ^ BYTE(0x00); pNew++;
		*pNew = ((((c4.rgbGreen | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbGreen) ^ BYTE(0x00); pNew++;
		*pNew = ((((c4.rgbRed   | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbRed  ) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_SDPSonoxn(StrucLineXTox &r)
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
		*pNew = ((((c4.rgbBlue  | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DSxn(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbGreen ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbRed   ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSnax(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ BYTE(0xFF)) & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0xFF)) & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0xFF)) & BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_SDPSoaxn(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  | BYTE(0x00)) & *pNew) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen | BYTE(0x00)) & *pNew) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   | BYTE(0x00)) & *pNew) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SPDnax(StrucLineXTox &r)
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
		*pNew = ((*pNew ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_DSPDoaxn(StrucLineXTox &r)
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
		*pNew = (((*pNew | BYTE(0x00)) & c4.rgbBlue ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew | BYTE(0x00)) & c4.rgbGreen) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew | BYTE(0x00)) & c4.rgbRed  ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DSPDSaoxx(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  & *pNew) | BYTE(0x00)) ^ c4.rgbBlue ) ^ *pNew; pNew++;
		*pNew = (((c4.rgbGreen & *pNew) | BYTE(0x00)) ^ c4.rgbGreen) ^ *pNew; pNew++;
		*pNew = (((c4.rgbRed   & *pNew) | BYTE(0x00)) ^ c4.rgbRed  ) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_PDSxan(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ *pNew) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPa(StrucLineXTox &r)
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
		*pNew = BYTE(0x00) & *pNew; pNew++;
		*pNew = BYTE(0x00) & *pNew; pNew++;
		*pNew = BYTE(0x00) & *pNew; pNew++;
		x1++;
	}
}

void Line4To24_PDSPnaoxn(StrucLineXTox &r)
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
		*pNew = ((((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbBlue ) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbGreen) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbRed  ) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSnoa(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew; pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew; pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew; pNew++;
		x1++;
	}
}

void Line4To24_DPSDxoxn(StrucLineXTox &r)
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
		*pNew = (((*pNew ^ c4.rgbBlue ) | BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ c4.rgbGreen) | BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ c4.rgbRed  ) | BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PDSPonoxn(StrucLineXTox &r)
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
		*pNew = ((((BYTE(0x00) | c4.rgbBlue ) ^ BYTE(0xFF)) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((BYTE(0x00) | c4.rgbGreen) ^ BYTE(0xFF)) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((BYTE(0x00) | c4.rgbRed  ) ^ BYTE(0xFF)) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PDxn(StrucLineXTox &r)
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
		*pNew = (*pNew ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (*pNew ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (*pNew ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DSPnax(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbBlue ) ^ *pNew; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbGreen) ^ *pNew; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbRed  ) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_PDSPoaxn(StrucLineXTox &r)
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
		*pNew = (((BYTE(0x00) | c4.rgbBlue ) & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) | c4.rgbGreen) & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) | c4.rgbRed  ) & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSoa(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  | BYTE(0x00)) & *pNew; pNew++;
		*pNew = (c4.rgbGreen | BYTE(0x00)) & *pNew; pNew++;
		*pNew = (c4.rgbRed   | BYTE(0x00)) & *pNew; pNew++;
		x1++;
	}
}

void Line4To24_DPSoxn(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  | BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen | BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   | BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_D(StrucLineXTox &r)
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
		*pNew = *pNew; pNew++;
		*pNew = *pNew; pNew++;
		*pNew = *pNew; pNew++;
		x1++;
	}
}

void Line4To24_DPSono(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew; pNew++;
		*pNew = ((c4.rgbGreen | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew; pNew++;
		*pNew = ((c4.rgbRed   | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew; pNew++;
		x1++;
	}
}

void Line4To24_SPDSxax(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ *pNew) & BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) & BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) & BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_DPSDaoxn(StrucLineXTox &r)
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
		*pNew = (((*pNew & c4.rgbBlue ) | BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew & c4.rgbGreen) | BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew & c4.rgbRed  ) | BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DSPnao(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbBlue ) | *pNew; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbGreen) | *pNew; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbRed  ) | *pNew; pNew++;
		x1++;
	}
}

void Line4To24_DPno(StrucLineXTox &r)
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
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) | *pNew; pNew++;
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) | *pNew; pNew++;
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) | *pNew; pNew++;
		x1++;
	}
}

void Line4To24_PDSnoa(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ BYTE(0xFF)) | *pNew) & BYTE(0x00); pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0xFF)) | *pNew) & BYTE(0x00); pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0xFF)) | *pNew) & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_PDSPxoxn(StrucLineXTox &r)
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
		*pNew = (((BYTE(0x00) ^ c4.rgbBlue ) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ c4.rgbGreen) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ c4.rgbRed  ) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SSPxDSxox(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ *pNew) | (BYTE(0x00) ^ c4.rgbBlue )) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) | (BYTE(0x00) ^ c4.rgbGreen)) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) | (BYTE(0x00) ^ c4.rgbRed  )) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SDPanan(StrucLineXTox &r)
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
		*pNew = (((BYTE(0x00) & *pNew) ^ BYTE(0xFF)) & c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) & *pNew) ^ BYTE(0xFF)) & c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) & *pNew) ^ BYTE(0xFF)) & c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PSDnax(StrucLineXTox &r)
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
		*pNew = ((*pNew ^ BYTE(0xFF)) & c4.rgbBlue ) ^ BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) & c4.rgbGreen) ^ BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) & c4.rgbRed  ) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_DPSDoaxn(StrucLineXTox &r)
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
		*pNew = (((*pNew | c4.rgbBlue ) & BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew | c4.rgbGreen) & BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew | c4.rgbRed  ) & BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSDPaoxx(StrucLineXTox &r)
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
		*pNew = (((BYTE(0x00) & *pNew) | c4.rgbBlue ) ^ BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((BYTE(0x00) & *pNew) | c4.rgbGreen) ^ BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((BYTE(0x00) & *pNew) | c4.rgbRed  ) ^ BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_SDPxan(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) ^ *pNew) & c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) & c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) & c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PSDPxax(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) ^ *pNew) & c4.rgbBlue ) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) & c4.rgbGreen) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) & c4.rgbRed  ) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_DSPDaoxn(StrucLineXTox &r)
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
		*pNew = (((*pNew & BYTE(0x00)) | c4.rgbBlue ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew & BYTE(0x00)) | c4.rgbGreen) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew & BYTE(0x00)) | c4.rgbRed  ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSnao(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew; pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew; pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew; pNew++;
		x1++;
	}
}

void Line4To24_DSno(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  ^ BYTE(0xFF)) | *pNew; pNew++;
		*pNew = (c4.rgbGreen ^ BYTE(0xFF)) | *pNew; pNew++;
		*pNew = (c4.rgbRed   ^ BYTE(0xFF)) | *pNew; pNew++;
		x1++;
	}
}

void Line4To24_SPDSanax(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = (((c4.rgbGreen & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = (((c4.rgbRed   & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SDxPDxan(StrucLineXTox &r)
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
		*pNew = ((*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbBlue )) ^ BYTE(0xFF); pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbGreen)) ^ BYTE(0xFF); pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbRed  )) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSxo(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  ^ BYTE(0x00)) | *pNew; pNew++;
		*pNew = (c4.rgbGreen ^ BYTE(0x00)) | *pNew; pNew++;
		*pNew = (c4.rgbRed   ^ BYTE(0x00)) | *pNew; pNew++;
		x1++;
	}
}

void Line4To24_DPSano(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  & BYTE(0x00)) ^ BYTE(0xFF)) | *pNew; pNew++;
		*pNew = ((c4.rgbGreen & BYTE(0x00)) ^ BYTE(0xFF)) | *pNew; pNew++;
		*pNew = ((c4.rgbRed   & BYTE(0x00)) ^ BYTE(0xFF)) | *pNew; pNew++;
		x1++;
	}
}

void Line4To24_PSa(StrucLineXTox &r)
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
		*pNew = c4.rgbBlue  & BYTE(0x00); pNew++;
		*pNew = c4.rgbGreen & BYTE(0x00); pNew++;
		*pNew = c4.rgbRed   & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_SPDSnaoxn(StrucLineXTox &r)
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
		*pNew = ((((c4.rgbBlue  ^ BYTE(0xFF)) & *pNew) | BYTE(0x00)) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen ^ BYTE(0xFF)) & *pNew) | BYTE(0x00)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   ^ BYTE(0xFF)) & *pNew) | BYTE(0x00)) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SPDSonoxn(StrucLineXTox &r)
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
		*pNew = ((((c4.rgbBlue  | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PSxn(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbGreen ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbRed   ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SPDnoa(StrucLineXTox &r)
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
		*pNew = ((*pNew ^ BYTE(0xFF)) | BYTE(0x00)) & c4.rgbBlue ; pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) | BYTE(0x00)) & c4.rgbGreen; pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) | BYTE(0x00)) & c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SPDSxoxn(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  ^ *pNew) | BYTE(0x00)) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ *pNew) | BYTE(0x00)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ *pNew) | BYTE(0x00)) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SDPnax(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_PSDPoaxn(StrucLineXTox &r)
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
		*pNew = (((BYTE(0x00) | *pNew) & c4.rgbBlue ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) | *pNew) & c4.rgbGreen) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) | *pNew) & c4.rgbRed  ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SDPoa(StrucLineXTox &r)
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
		*pNew = (BYTE(0x00) | *pNew) & c4.rgbBlue ; pNew++;
		*pNew = (BYTE(0x00) | *pNew) & c4.rgbGreen; pNew++;
		*pNew = (BYTE(0x00) | *pNew) & c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SPDoxn(StrucLineXTox &r)
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
		*pNew = ((*pNew | BYTE(0x00)) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((*pNew | BYTE(0x00)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((*pNew | BYTE(0x00)) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSDxax(StrucLineXTox &r)
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
		*pNew = ((*pNew ^ c4.rgbBlue ) & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((*pNew ^ c4.rgbGreen) & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((*pNew ^ c4.rgbRed  ) & BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_SPDSaoxn(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  & *pNew) | BYTE(0x00)) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen & *pNew) | BYTE(0x00)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   & *pNew) | BYTE(0x00)) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_S(StrucLineXTox &r)
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
		*pNew = c4.rgbBlue ; pNew++;
		*pNew = c4.rgbGreen; pNew++;
		*pNew = c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SDPono(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) | *pNew) ^ BYTE(0xFF)) | c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) | *pNew) ^ BYTE(0xFF)) | c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) | *pNew) ^ BYTE(0xFF)) | c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SDPnao(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) | c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) | c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) | c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SPno(StrucLineXTox &r)
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
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbBlue ; pNew++;
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbGreen; pNew++;
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_PSDnoa(StrucLineXTox &r)
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
		*pNew = ((*pNew ^ BYTE(0xFF)) | c4.rgbBlue ) & BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) | c4.rgbGreen) & BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) | c4.rgbRed  ) & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_PSDPxoxn(StrucLineXTox &r)
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
		*pNew = (((BYTE(0x00) ^ *pNew) | c4.rgbBlue ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ *pNew) | c4.rgbGreen) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ *pNew) | c4.rgbRed  ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PDSnax(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ BYTE(0xFF)) & *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0xFF)) & *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0xFF)) & *pNew) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_SPDSoaxn(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  | *pNew) & BYTE(0x00)) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen | *pNew) & BYTE(0x00)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   | *pNew) & BYTE(0x00)) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SSPxPDxax(StrucLineXTox &r)
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
		*pNew = ((*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbBlue )) ^ c4.rgbBlue ; pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbGreen)) ^ c4.rgbGreen; pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbRed  )) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_DPSanan(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PSDPSaoxx(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  & BYTE(0x00)) | *pNew) ^ c4.rgbBlue ) ^ BYTE(0x00); pNew++;
		*pNew = (((c4.rgbGreen & BYTE(0x00)) | *pNew) ^ c4.rgbGreen) ^ BYTE(0x00); pNew++;
		*pNew = (((c4.rgbRed   & BYTE(0x00)) | *pNew) ^ c4.rgbRed  ) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_DPSxan(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_PDSPxax(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) ^ c4.rgbBlue ) & *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) ^ c4.rgbGreen) & *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) ^ c4.rgbRed  ) & *pNew) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_SDPSaoxn(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  & BYTE(0x00)) | *pNew) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen & BYTE(0x00)) | *pNew) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   & BYTE(0x00)) | *pNew) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSDanax(StrucLineXTox &r)
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
		*pNew = (((*pNew & c4.rgbBlue ) ^ BYTE(0xFF)) & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((*pNew & c4.rgbGreen) ^ BYTE(0xFF)) & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((*pNew & c4.rgbRed  ) ^ BYTE(0xFF)) & BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_SPxDSxan(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ *pNew) & (BYTE(0x00) ^ c4.rgbBlue )) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) & (BYTE(0x00) ^ c4.rgbGreen)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) & (BYTE(0x00) ^ c4.rgbRed  )) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SPDnao(StrucLineXTox &r)
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
		*pNew = ((*pNew ^ BYTE(0xFF)) & BYTE(0x00)) | c4.rgbBlue ; pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) & BYTE(0x00)) | c4.rgbGreen; pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) & BYTE(0x00)) | c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SDno(StrucLineXTox &r)
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
		*pNew = (*pNew ^ BYTE(0xFF)) | c4.rgbBlue ; pNew++;
		*pNew = (*pNew ^ BYTE(0xFF)) | c4.rgbGreen; pNew++;
		*pNew = (*pNew ^ BYTE(0xFF)) | c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SDPxo(StrucLineXTox &r)
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
		*pNew = (BYTE(0x00) ^ *pNew) | c4.rgbBlue ; pNew++;
		*pNew = (BYTE(0x00) ^ *pNew) | c4.rgbGreen; pNew++;
		*pNew = (BYTE(0x00) ^ *pNew) | c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SDPano(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) & *pNew) ^ BYTE(0xFF)) | c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) & *pNew) ^ BYTE(0xFF)) | c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) & *pNew) ^ BYTE(0xFF)) | c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_PDSoa(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  | *pNew) & BYTE(0x00); pNew++;
		*pNew = (c4.rgbGreen | *pNew) & BYTE(0x00); pNew++;
		*pNew = (c4.rgbRed   | *pNew) & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_PDSoxn(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DSPDxax(StrucLineXTox &r)
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
		*pNew = ((*pNew ^ BYTE(0x00)) & c4.rgbBlue ) ^ *pNew; pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) & c4.rgbGreen) ^ *pNew; pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) & c4.rgbRed  ) ^ *pNew; pNew++;
		x1++;
	}
}

void Line4To24_PSDPaoxn(StrucLineXTox &r)
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
		*pNew = (((BYTE(0x00) & *pNew) | c4.rgbBlue ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) & *pNew) | c4.rgbGreen) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) & *pNew) | c4.rgbRed  ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SDPSxax(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ BYTE(0x00)) & *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0x00)) & *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0x00)) & *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_PDSPaoxn(StrucLineXTox &r)
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
		*pNew = (((BYTE(0x00) & c4.rgbBlue ) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) & c4.rgbGreen) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) & c4.rgbRed  ) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SDPSanax(StrucLineXTox &r)
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
		*pNew = (((c4.rgbBlue  & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = (((c4.rgbGreen & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = (((c4.rgbRed   & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SPxPDxan(StrucLineXTox &r)
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
		*pNew = ((*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbBlue )) ^ BYTE(0xFF); pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbGreen)) ^ BYTE(0xFF); pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbRed  )) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_SSPxDSxax(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ *pNew) & (BYTE(0x00) ^ c4.rgbBlue )) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) & (BYTE(0x00) ^ c4.rgbGreen)) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) & (BYTE(0x00) ^ c4.rgbRed  )) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_DSPDSanaxxn(StrucLineXTox &r)
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
		*pNew = (((((c4.rgbBlue  & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbBlue ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((((c4.rgbGreen & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbGreen) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((((c4.rgbRed   & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbRed  ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line4To24_DPSao(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  & BYTE(0x00)) | *pNew; pNew++;
		*pNew = (c4.rgbGreen & BYTE(0x00)) | *pNew; pNew++;
		*pNew = (c4.rgbRed   & BYTE(0x00)) | *pNew; pNew++;
		x1++;
	}
}

void Line4To24_DPSxno(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ BYTE(0x00)) ^ BYTE(0xFF)) | *pNew; pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0x00)) ^ BYTE(0xFF)) | *pNew; pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0x00)) ^ BYTE(0xFF)) | *pNew; pNew++;
		x1++;
	}
}

void Line4To24_SDPao(StrucLineXTox &r)
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
		*pNew = (BYTE(0x00) & *pNew) | c4.rgbBlue ; pNew++;
		*pNew = (BYTE(0x00) & *pNew) | c4.rgbGreen; pNew++;
		*pNew = (BYTE(0x00) & *pNew) | c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_SDPxno(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) | c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) | c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) | c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_DSo(StrucLineXTox &r)
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
		*pNew = c4.rgbBlue  | *pNew; pNew++;
		*pNew = c4.rgbGreen | *pNew; pNew++;
		*pNew = c4.rgbRed   | *pNew; pNew++;
		x1++;
	}
}

void Line4To24_SDPnoo(StrucLineXTox &r)
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
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) | c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) | c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) | c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line4To24_P(StrucLineXTox &r)
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
		*pNew = BYTE(0x00); pNew++;
		*pNew = BYTE(0x00); pNew++;
		*pNew = BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_PDSono(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  | *pNew) ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		*pNew = ((c4.rgbGreen | *pNew) ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		*pNew = ((c4.rgbRed   | *pNew) ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_PDSnao(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ BYTE(0xFF)) & *pNew) | BYTE(0x00); pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0xFF)) & *pNew) | BYTE(0x00); pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0xFF)) & *pNew) | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_PSno(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		*pNew = (c4.rgbGreen ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		*pNew = (c4.rgbRed   ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_PSDnao(StrucLineXTox &r)
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
		*pNew = ((*pNew ^ BYTE(0xFF)) & c4.rgbBlue ) | BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) & c4.rgbGreen) | BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) & c4.rgbRed  ) | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_PDno(StrucLineXTox &r)
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
		*pNew = (*pNew ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		*pNew = (*pNew ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		*pNew = (*pNew ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_PDSxo(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  ^ *pNew) | BYTE(0x00); pNew++;
		*pNew = (c4.rgbGreen ^ *pNew) | BYTE(0x00); pNew++;
		*pNew = (c4.rgbRed   ^ *pNew) | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_PDSano(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  & *pNew) ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		*pNew = ((c4.rgbGreen & *pNew) ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		*pNew = ((c4.rgbRed   & *pNew) ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_PDSao(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  & *pNew) | BYTE(0x00); pNew++;
		*pNew = (c4.rgbGreen & *pNew) | BYTE(0x00); pNew++;
		*pNew = (c4.rgbRed   & *pNew) | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_PDSxno(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ *pNew) ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_DPo(StrucLineXTox &r)
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
		*pNew = BYTE(0x00) | *pNew; pNew++;
		*pNew = BYTE(0x00) | *pNew; pNew++;
		*pNew = BYTE(0x00) | *pNew; pNew++;
		x1++;
	}
}

void Line4To24_DPSnoo(StrucLineXTox &r)
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
		*pNew = ((c4.rgbBlue  ^ BYTE(0xFF)) | BYTE(0x00)) | *pNew; pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0xFF)) | BYTE(0x00)) | *pNew; pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0xFF)) | BYTE(0x00)) | *pNew; pNew++;
		x1++;
	}
}

void Line4To24_PSo(StrucLineXTox &r)
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
		*pNew = c4.rgbBlue  | BYTE(0x00); pNew++;
		*pNew = c4.rgbGreen | BYTE(0x00); pNew++;
		*pNew = c4.rgbRed   | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_PSDnoo(StrucLineXTox &r)
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
		*pNew = ((*pNew ^ BYTE(0xFF)) | c4.rgbBlue ) | BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) | c4.rgbGreen) | BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) | c4.rgbRed  ) | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line4To24_DPSoo(StrucLineXTox &r)
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
		*pNew = (c4.rgbBlue  | BYTE(0x00)) | *pNew; pNew++;
		*pNew = (c4.rgbGreen | BYTE(0x00)) | *pNew; pNew++;
		*pNew = (c4.rgbRed   | BYTE(0x00)) | *pNew; pNew++;
		x1++;
	}
}

void Line4To24_1(StrucLineXTox &r)
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
		*pNew = BYTE(0xFF); pNew++;
		*pNew = BYTE(0xFF); pNew++;
		*pNew = BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_0(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = BYTE(0x00); pNew++;
		*pNew = BYTE(0x00); pNew++;
		*pNew = BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_DPSoon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  | BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen | BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   | BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSona(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  | BYTE(0x00)) ^ BYTE(0xFF)) & *pNew; pNew++;
		*pNew = ((c4.rgbGreen | BYTE(0x00)) ^ BYTE(0xFF)) & *pNew; pNew++;
		*pNew = ((c4.rgbRed   | BYTE(0x00)) ^ BYTE(0xFF)) & *pNew; pNew++;
		x1++;
	}
}

void Line8To24_PSon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbGreen | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbRed   | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SDPona(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) | *pNew) ^ BYTE(0xFF)) & c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) | *pNew) ^ BYTE(0xFF)) & c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) | *pNew) ^ BYTE(0xFF)) & c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_DPon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (BYTE(0x00) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (BYTE(0x00) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (BYTE(0x00) | *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PDSxnon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  ^ *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PDSaon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  & *pNew) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen & *pNew) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   & *pNew) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SDPnaa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) & c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) & c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) & c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_PDSxon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ *pNew) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPna(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) & *pNew; pNew++;
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) & *pNew; pNew++;
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) & *pNew; pNew++;
		x1++;
	}
}

void Line8To24_PSDnaon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((*pNew ^ BYTE(0xFF)) & c4.rgbBlue ) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) & c4.rgbGreen) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) & c4.rgbRed  ) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SPna(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbBlue ; pNew++;
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbGreen; pNew++;
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_PDSnaon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  ^ BYTE(0xFF)) & *pNew) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0xFF)) & *pNew) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0xFF)) & *pNew) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PDSonon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_Pn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = BYTE(0x00) ^ BYTE(0xFF); pNew++;
		*pNew = BYTE(0x00) ^ BYTE(0xFF); pNew++;
		*pNew = BYTE(0x00) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PDSona(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  | *pNew) ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		*pNew = ((c4.rgbGreen | *pNew) ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		*pNew = ((c4.rgbRed   | *pNew) ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_DSon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbGreen | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbRed   | *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SDPxnon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) | c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) | c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) | c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SDPaon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) & *pNew) | c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((BYTE(0x00) & *pNew) | c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((BYTE(0x00) & *pNew) | c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSxnon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  ^ BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSaon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  & BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen & BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   & BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PSDPSanaxx(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((((c4.rgbBlue  & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbBlue ) ^ BYTE(0x00); pNew++;
		*pNew = ((((c4.rgbGreen & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbGreen) ^ BYTE(0x00); pNew++;
		*pNew = ((((c4.rgbRed   & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbRed  ) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_SSPxDSxaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  ^ *pNew) & (BYTE(0x00) ^ c4.rgbBlue )) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ *pNew) & (BYTE(0x00) ^ c4.rgbGreen)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ *pNew) & (BYTE(0x00) ^ c4.rgbRed  )) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SPxPDxa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbBlue ); pNew++;
		*pNew = (*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbGreen); pNew++;
		*pNew = (*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbRed  ); pNew++;
		x1++;
	}
}

void Line8To24_SDPSanaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((((c4.rgbBlue  & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PDSPaox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) & c4.rgbBlue ) | *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) & c4.rgbGreen) | *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) & c4.rgbRed  ) | *pNew) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_SDPSxaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  ^ BYTE(0x00)) & *pNew) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0x00)) & *pNew) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0x00)) & *pNew) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PSDPaox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) & *pNew) | c4.rgbBlue ) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) & *pNew) | c4.rgbGreen) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) & *pNew) | c4.rgbRed  ) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_DSPDxaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((*pNew ^ BYTE(0x00)) & c4.rgbBlue ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0x00)) & c4.rgbGreen) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0x00)) & c4.rgbRed  ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PDSox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  | *pNew) ^ BYTE(0x00); pNew++;
		*pNew = (c4.rgbGreen | *pNew) ^ BYTE(0x00); pNew++;
		*pNew = (c4.rgbRed   | *pNew) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_PDSoan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  | *pNew) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen | *pNew) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   | *pNew) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSnaa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ BYTE(0xFF)) & BYTE(0x00)) & *pNew; pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0xFF)) & BYTE(0x00)) & *pNew; pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0xFF)) & BYTE(0x00)) & *pNew; pNew++;
		x1++;
	}
}

void Line8To24_SDPxon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) ^ *pNew) | c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) | c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) | c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DSna(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  ^ BYTE(0xFF)) & *pNew; pNew++;
		*pNew = (c4.rgbGreen ^ BYTE(0xFF)) & *pNew; pNew++;
		*pNew = (c4.rgbRed   ^ BYTE(0xFF)) & *pNew; pNew++;
		x1++;
	}
}

void Line8To24_SPDnaon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((*pNew ^ BYTE(0xFF)) & BYTE(0x00)) | c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) & BYTE(0x00)) | c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) & BYTE(0x00)) | c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SPxDSxa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  ^ *pNew) & (BYTE(0x00) ^ c4.rgbBlue ); pNew++;
		*pNew = (c4.rgbGreen ^ *pNew) & (BYTE(0x00) ^ c4.rgbGreen); pNew++;
		*pNew = (c4.rgbRed   ^ *pNew) & (BYTE(0x00) ^ c4.rgbRed  ); pNew++;
		x1++;
	}
}

void Line8To24_PDSPanaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((((BYTE(0x00) & c4.rgbBlue ) ^ BYTE(0xFF)) & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((BYTE(0x00) & c4.rgbGreen) ^ BYTE(0xFF)) & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((BYTE(0x00) & c4.rgbRed  ) ^ BYTE(0xFF)) & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SDPSaox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  & BYTE(0x00)) | *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen & BYTE(0x00)) | *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   & BYTE(0x00)) | *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SDPSxnox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  ^ BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_DPSxa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  ^ BYTE(0x00)) & *pNew; pNew++;
		*pNew = (c4.rgbGreen ^ BYTE(0x00)) & *pNew; pNew++;
		*pNew = (c4.rgbRed   ^ BYTE(0x00)) & *pNew; pNew++;
		x1++;
	}
}

void Line8To24_PSDPSaoxxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((((c4.rgbBlue  & BYTE(0x00)) | *pNew) ^ c4.rgbBlue ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen & BYTE(0x00)) | *pNew) ^ c4.rgbGreen) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   & BYTE(0x00)) | *pNew) ^ c4.rgbRed  ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSana(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew; pNew++;
		*pNew = ((c4.rgbGreen & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew; pNew++;
		*pNew = ((c4.rgbRed   & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew; pNew++;
		x1++;
	}
}

void Line8To24_SSPxPDxaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbBlue )) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbGreen)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbRed  )) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SPDSoax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  | *pNew) & BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen | *pNew) & BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   | *pNew) & BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_PSDnox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew ^ BYTE(0xFF)) | c4.rgbBlue ) ^ BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) | c4.rgbGreen) ^ BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) | c4.rgbRed  ) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_PSDPxox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) ^ *pNew) | c4.rgbBlue ) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) | c4.rgbGreen) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) | c4.rgbRed  ) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_PSDnoan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((*pNew ^ BYTE(0xFF)) | c4.rgbBlue ) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) | c4.rgbGreen) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) | c4.rgbRed  ) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PSna(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		*pNew = (c4.rgbGreen ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		*pNew = (c4.rgbRed   ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_SDPnaon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) | c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) | c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) | c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SDPSoox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  | BYTE(0x00)) | *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen | BYTE(0x00)) | *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   | BYTE(0x00)) | *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_Sn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = c4.rgbBlue  ^ BYTE(0xFF); pNew++;
		*pNew = c4.rgbGreen ^ BYTE(0xFF); pNew++;
		*pNew = c4.rgbRed   ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SPDSaox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  & *pNew) | BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen & *pNew) | BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   & *pNew) | BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SPDSxnox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  ^ *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = (((c4.rgbGreen ^ *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = (((c4.rgbRed   ^ *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SDPox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (BYTE(0x00) | *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = (BYTE(0x00) | *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = (BYTE(0x00) | *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SDPoan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) | *pNew) & c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((BYTE(0x00) | *pNew) & c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((BYTE(0x00) | *pNew) & c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PSDPoax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) | *pNew) & c4.rgbBlue ) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) | *pNew) & c4.rgbGreen) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) | *pNew) & c4.rgbRed  ) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_SPDnox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SPDSxox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ *pNew) | BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) | BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) | BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SPDnoan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((*pNew ^ BYTE(0xFF)) | BYTE(0x00)) & c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) | BYTE(0x00)) & c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) | BYTE(0x00)) & c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PSx(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = c4.rgbBlue  ^ BYTE(0x00); pNew++;
		*pNew = c4.rgbGreen ^ BYTE(0x00); pNew++;
		*pNew = c4.rgbRed   ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_SPDSonox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = (((c4.rgbGreen | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = (((c4.rgbRed   | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SPDSnaox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  ^ BYTE(0xFF)) & *pNew) | BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0xFF)) & *pNew) | BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0xFF)) & *pNew) | BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_PSan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbGreen & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbRed   & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PSDnaa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew ^ BYTE(0xFF)) & c4.rgbBlue ) & BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) & c4.rgbGreen) & BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) & c4.rgbRed  ) & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_DPSxon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SDxPDxa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbBlue ); pNew++;
		*pNew = (*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbGreen); pNew++;
		*pNew = (*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbRed  ); pNew++;
		x1++;
	}
}

void Line8To24_SPDSanaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((((c4.rgbBlue  & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SDna(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (*pNew ^ BYTE(0xFF)) & c4.rgbBlue ; pNew++;
		*pNew = (*pNew ^ BYTE(0xFF)) & c4.rgbGreen; pNew++;
		*pNew = (*pNew ^ BYTE(0xFF)) & c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_DPSnaon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DSPDaox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew & BYTE(0x00)) | c4.rgbBlue ) ^ *pNew; pNew++;
		*pNew = ((*pNew & BYTE(0x00)) | c4.rgbGreen) ^ *pNew; pNew++;
		*pNew = ((*pNew & BYTE(0x00)) | c4.rgbRed  ) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_PSDPxaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((BYTE(0x00) ^ *pNew) & c4.rgbBlue ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ *pNew) & c4.rgbGreen) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ *pNew) & c4.rgbRed  ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SDPxa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (BYTE(0x00) ^ *pNew) & c4.rgbBlue ; pNew++;
		*pNew = (BYTE(0x00) ^ *pNew) & c4.rgbGreen; pNew++;
		*pNew = (BYTE(0x00) ^ *pNew) & c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_PDSPDaoxxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((((*pNew & BYTE(0x00)) | c4.rgbBlue ) ^ *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((*pNew & BYTE(0x00)) | c4.rgbGreen) ^ *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((*pNew & BYTE(0x00)) | c4.rgbRed  ) ^ *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSDoax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew | c4.rgbBlue ) & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((*pNew | c4.rgbGreen) & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((*pNew | c4.rgbRed  ) & BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_PDSnox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ BYTE(0xFF)) | *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0xFF)) | *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0xFF)) | *pNew) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_SDPana(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) & *pNew) ^ BYTE(0xFF)) & c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) & *pNew) ^ BYTE(0xFF)) & c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) & *pNew) ^ BYTE(0xFF)) & c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SSPxDSxoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  ^ *pNew) | (BYTE(0x00) ^ c4.rgbBlue )) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ *pNew) | (BYTE(0x00) ^ c4.rgbGreen)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ *pNew) | (BYTE(0x00) ^ c4.rgbRed  )) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PDSPxox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) ^ c4.rgbBlue ) | *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) ^ c4.rgbGreen) | *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) ^ c4.rgbRed  ) | *pNew) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_PDSnoan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  ^ BYTE(0xFF)) | *pNew) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0xFF)) | *pNew) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0xFF)) | *pNew) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PDna(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (*pNew ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		*pNew = (*pNew ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		*pNew = (*pNew ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_DSPnaon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbBlue ) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbGreen) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbRed  ) | *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSDaox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew & c4.rgbBlue ) | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((*pNew & c4.rgbGreen) | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((*pNew & c4.rgbRed  ) | BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_SPDSxaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  ^ *pNew) & BYTE(0x00)) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ *pNew) & BYTE(0x00)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ *pNew) & BYTE(0x00)) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSonon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_Dn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = *pNew ^ BYTE(0xFF); pNew++;
		*pNew = *pNew ^ BYTE(0xFF); pNew++;
		*pNew = *pNew ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (c4.rgbGreen | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (c4.rgbRed   | BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_DPSoan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  | BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen | BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   | BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PDSPoax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) | c4.rgbBlue ) & *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) | c4.rgbGreen) & *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) | c4.rgbRed  ) & *pNew) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_DPSnox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ BYTE(0xFF)) | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0xFF)) | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0xFF)) | BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_DPx(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = BYTE(0x00) ^ *pNew; pNew++;
		*pNew = BYTE(0x00) ^ *pNew; pNew++;
		*pNew = BYTE(0x00) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_DPSDonox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((*pNew | c4.rgbBlue ) ^ BYTE(0xFF)) | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((*pNew | c4.rgbGreen) ^ BYTE(0xFF)) | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((*pNew | c4.rgbRed  ) ^ BYTE(0xFF)) | BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_DPSDxox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew ^ c4.rgbBlue ) | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((*pNew ^ c4.rgbGreen) | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((*pNew ^ c4.rgbRed  ) | BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_DPSnoan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSDnaox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((*pNew ^ BYTE(0xFF)) & c4.rgbBlue ) | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) & c4.rgbGreen) | BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) & c4.rgbRed  ) | BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_DPan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (BYTE(0x00) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (BYTE(0x00) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (BYTE(0x00) & *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PDSxa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  ^ *pNew) & BYTE(0x00); pNew++;
		*pNew = (c4.rgbGreen ^ *pNew) & BYTE(0x00); pNew++;
		*pNew = (c4.rgbRed   ^ *pNew) & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_DSPDSaoxxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((((c4.rgbBlue  & *pNew) | BYTE(0x00)) ^ c4.rgbBlue ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen & *pNew) | BYTE(0x00)) ^ c4.rgbGreen) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   & *pNew) | BYTE(0x00)) ^ c4.rgbRed  ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DSPDoax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew | BYTE(0x00)) & c4.rgbBlue ) ^ *pNew; pNew++;
		*pNew = ((*pNew | BYTE(0x00)) & c4.rgbGreen) ^ *pNew; pNew++;
		*pNew = ((*pNew | BYTE(0x00)) & c4.rgbRed  ) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_SDPnox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SDPSoax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  | BYTE(0x00)) & *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen | BYTE(0x00)) & *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   | BYTE(0x00)) & *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_DSPnox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbBlue ) ^ *pNew; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbGreen) ^ *pNew; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbRed  ) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_DSx(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = c4.rgbBlue  ^ *pNew; pNew++;
		*pNew = c4.rgbGreen ^ *pNew; pNew++;
		*pNew = c4.rgbRed   ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_SDPSonox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = (((c4.rgbGreen | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = (((c4.rgbRed   | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_DSPDSonoxxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((((c4.rgbBlue  | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbBlue ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((((c4.rgbGreen | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbGreen) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((((c4.rgbRed   | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbRed  ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PDSxxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (c4.rgbGreen & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (c4.rgbRed   & BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_PSDPSoaxxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((((c4.rgbBlue  | BYTE(0x00)) & *pNew) ^ c4.rgbBlue ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen | BYTE(0x00)) & *pNew) ^ c4.rgbGreen) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   | BYTE(0x00)) & *pNew) ^ c4.rgbRed  ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SDPax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (BYTE(0x00) & *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = (BYTE(0x00) & *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = (BYTE(0x00) & *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_PDSPDoaxxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((((*pNew | BYTE(0x00)) & c4.rgbBlue ) ^ *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((*pNew | BYTE(0x00)) & c4.rgbGreen) ^ *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((*pNew | BYTE(0x00)) & c4.rgbRed  ) ^ *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SDPSnoax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_PDSxnan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  ^ *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PDSana(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  & *pNew) ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		*pNew = ((c4.rgbGreen & *pNew) ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		*pNew = ((c4.rgbRed   & *pNew) ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_SSDxPDxaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbBlue )) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbGreen)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbRed  )) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SDPSxox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ BYTE(0x00)) | *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0x00)) | *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0x00)) | *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SDPnoan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) & c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) & c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) & c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DSPDxox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew ^ BYTE(0x00)) | c4.rgbBlue ) ^ *pNew; pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) | c4.rgbGreen) ^ *pNew; pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) | c4.rgbRed  ) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_DSPnoan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbBlue ) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbGreen) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbRed  ) & *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SDPSnaox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_DSan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbGreen & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbRed   & *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PDSax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  & *pNew) ^ BYTE(0x00); pNew++;
		*pNew = (c4.rgbGreen & *pNew) ^ BYTE(0x00); pNew++;
		*pNew = (c4.rgbRed   & *pNew) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_DSPDSoaxxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((((c4.rgbBlue  | *pNew) & BYTE(0x00)) ^ c4.rgbBlue ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen | *pNew) & BYTE(0x00)) ^ c4.rgbGreen) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   | *pNew) & BYTE(0x00)) ^ c4.rgbRed  ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSDnoax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((*pNew ^ BYTE(0xFF)) | c4.rgbBlue ) & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) | c4.rgbGreen) & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((*pNew ^ BYTE(0xFF)) | c4.rgbRed  ) & BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_SDPxnan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) & c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) & c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) & c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SPDSnoax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  ^ BYTE(0xFF)) | *pNew) & BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0xFF)) | *pNew) & BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0xFF)) | *pNew) & BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_DPSxnan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  ^ BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SPxDSxo(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  ^ *pNew) | (BYTE(0x00) ^ c4.rgbBlue ); pNew++;
		*pNew = (c4.rgbGreen ^ *pNew) | (BYTE(0x00) ^ c4.rgbGreen); pNew++;
		*pNew = (c4.rgbRed   ^ *pNew) | (BYTE(0x00) ^ c4.rgbRed  ); pNew++;
		x1++;
	}
}

void Line8To24_DPSaan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  & BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen & BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   & BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSaa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  & BYTE(0x00)) & *pNew; pNew++;
		*pNew = (c4.rgbGreen & BYTE(0x00)) & *pNew; pNew++;
		*pNew = (c4.rgbRed   & BYTE(0x00)) & *pNew; pNew++;
		x1++;
	}
}

void Line8To24_SPxDSxon(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ *pNew) | (BYTE(0x00) ^ c4.rgbBlue )) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) | (BYTE(0x00) ^ c4.rgbGreen)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) | (BYTE(0x00) ^ c4.rgbRed  )) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSxna(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ BYTE(0x00)) ^ BYTE(0xFF)) & *pNew; pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0x00)) ^ BYTE(0xFF)) & *pNew; pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0x00)) ^ BYTE(0xFF)) & *pNew; pNew++;
		x1++;
	}
}

void Line8To24_SPDSnoaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((((c4.rgbBlue  ^ BYTE(0xFF)) | *pNew) & BYTE(0x00)) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen ^ BYTE(0xFF)) | *pNew) & BYTE(0x00)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   ^ BYTE(0xFF)) | *pNew) & BYTE(0x00)) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SDPxna(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) & c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) & c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) & c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_PDSPnoaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbBlue ) & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbGreen) & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbRed  ) & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DSPDSoaxx(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  | *pNew) & BYTE(0x00)) ^ c4.rgbBlue ) ^ *pNew; pNew++;
		*pNew = (((c4.rgbGreen | *pNew) & BYTE(0x00)) ^ c4.rgbGreen) ^ *pNew; pNew++;
		*pNew = (((c4.rgbRed   | *pNew) & BYTE(0x00)) ^ c4.rgbRed  ) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_PDSaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DSa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = c4.rgbBlue  & *pNew; pNew++;
		*pNew = c4.rgbGreen & *pNew; pNew++;
		*pNew = c4.rgbRed   & *pNew; pNew++;
		x1++;
	}
}

void Line8To24_SDPSnaoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((((c4.rgbBlue  ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DSPnoa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbBlue ) & *pNew; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbGreen) & *pNew; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbRed  ) & *pNew; pNew++;
		x1++;
	}
}

void Line8To24_DSPDxoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((*pNew ^ BYTE(0x00)) | c4.rgbBlue ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0x00)) | c4.rgbGreen) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ BYTE(0x00)) | c4.rgbRed  ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SDPnoa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) & c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) & c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) & c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SDPSxoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  ^ BYTE(0x00)) | *pNew) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ BYTE(0x00)) | *pNew) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ BYTE(0x00)) | *pNew) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SSDxPDxax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbBlue )) ^ c4.rgbBlue ; pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbGreen)) ^ c4.rgbGreen; pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbRed  )) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_PDSanan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PDSxna(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ *pNew) ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) ^ BYTE(0xFF)) & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_SDPSnoaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((((c4.rgbBlue  ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSDPoaxx(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((BYTE(0x00) | *pNew) & c4.rgbBlue ) ^ BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((BYTE(0x00) | *pNew) & c4.rgbGreen) ^ BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((BYTE(0x00) | *pNew) & c4.rgbRed  ) ^ BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_SPDaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew & BYTE(0x00)) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((*pNew & BYTE(0x00)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((*pNew & BYTE(0x00)) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PSDPSoaxx(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  | BYTE(0x00)) & *pNew) ^ c4.rgbBlue ) ^ BYTE(0x00); pNew++;
		*pNew = (((c4.rgbGreen | BYTE(0x00)) & *pNew) ^ c4.rgbGreen) ^ BYTE(0x00); pNew++;
		*pNew = (((c4.rgbRed   | BYTE(0x00)) & *pNew) ^ c4.rgbRed  ) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_DPSaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  & BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen & BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   & BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSxx(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  ^ BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (c4.rgbGreen ^ BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (c4.rgbRed   ^ BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_PSDPSonoxx(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((((c4.rgbBlue  | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbBlue ) ^ BYTE(0x00); pNew++;
		*pNew = ((((c4.rgbGreen | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbGreen) ^ BYTE(0x00); pNew++;
		*pNew = ((((c4.rgbRed   | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbRed  ) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_SDPSonoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((((c4.rgbBlue  | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DSxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbGreen ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbRed   ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSnax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ BYTE(0xFF)) & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0xFF)) & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0xFF)) & BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_SDPSoaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  | BYTE(0x00)) & *pNew) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen | BYTE(0x00)) & *pNew) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   | BYTE(0x00)) & *pNew) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SPDnax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_DSPDoaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((*pNew | BYTE(0x00)) & c4.rgbBlue ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew | BYTE(0x00)) & c4.rgbGreen) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew | BYTE(0x00)) & c4.rgbRed  ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DSPDSaoxx(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  & *pNew) | BYTE(0x00)) ^ c4.rgbBlue ) ^ *pNew; pNew++;
		*pNew = (((c4.rgbGreen & *pNew) | BYTE(0x00)) ^ c4.rgbGreen) ^ *pNew; pNew++;
		*pNew = (((c4.rgbRed   & *pNew) | BYTE(0x00)) ^ c4.rgbRed  ) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_PDSxan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ *pNew) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) & BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = BYTE(0x00) & *pNew; pNew++;
		*pNew = BYTE(0x00) & *pNew; pNew++;
		*pNew = BYTE(0x00) & *pNew; pNew++;
		x1++;
	}
}

void Line8To24_PDSPnaoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbBlue ) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbGreen) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbRed  ) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSnoa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew; pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew; pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0xFF)) | BYTE(0x00)) & *pNew; pNew++;
		x1++;
	}
}

void Line8To24_DPSDxoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((*pNew ^ c4.rgbBlue ) | BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ c4.rgbGreen) | BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew ^ c4.rgbRed  ) | BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PDSPonoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((((BYTE(0x00) | c4.rgbBlue ) ^ BYTE(0xFF)) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((BYTE(0x00) | c4.rgbGreen) ^ BYTE(0xFF)) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((((BYTE(0x00) | c4.rgbRed  ) ^ BYTE(0xFF)) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PDxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (*pNew ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (*pNew ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (*pNew ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DSPnax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbBlue ) ^ *pNew; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbGreen) ^ *pNew; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbRed  ) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_PDSPoaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((BYTE(0x00) | c4.rgbBlue ) & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) | c4.rgbGreen) & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) | c4.rgbRed  ) & *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSoa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  | BYTE(0x00)) & *pNew; pNew++;
		*pNew = (c4.rgbGreen | BYTE(0x00)) & *pNew; pNew++;
		*pNew = (c4.rgbRed   | BYTE(0x00)) & *pNew; pNew++;
		x1++;
	}
}

void Line8To24_DPSoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  | BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen | BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   | BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_D(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = *pNew; pNew++;
		*pNew = *pNew; pNew++;
		*pNew = *pNew; pNew++;
		x1++;
	}
}

void Line8To24_DPSono(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew; pNew++;
		*pNew = ((c4.rgbGreen | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew; pNew++;
		*pNew = ((c4.rgbRed   | BYTE(0x00)) ^ BYTE(0xFF)) | *pNew; pNew++;
		x1++;
	}
}

void Line8To24_SPDSxax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ *pNew) & BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) & BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) & BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_DPSDaoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((*pNew & c4.rgbBlue ) | BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew & c4.rgbGreen) | BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew & c4.rgbRed  ) | BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DSPnao(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbBlue ) | *pNew; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbGreen) | *pNew; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & c4.rgbRed  ) | *pNew; pNew++;
		x1++;
	}
}

void Line8To24_DPno(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) | *pNew; pNew++;
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) | *pNew; pNew++;
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) | *pNew; pNew++;
		x1++;
	}
}

void Line8To24_PDSnoa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ BYTE(0xFF)) | *pNew) & BYTE(0x00); pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0xFF)) | *pNew) & BYTE(0x00); pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0xFF)) | *pNew) & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_PDSPxoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((BYTE(0x00) ^ c4.rgbBlue ) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ c4.rgbGreen) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ c4.rgbRed  ) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SSPxDSxox(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ *pNew) | (BYTE(0x00) ^ c4.rgbBlue )) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) | (BYTE(0x00) ^ c4.rgbGreen)) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) | (BYTE(0x00) ^ c4.rgbRed  )) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SDPanan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((BYTE(0x00) & *pNew) ^ BYTE(0xFF)) & c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) & *pNew) ^ BYTE(0xFF)) & c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) & *pNew) ^ BYTE(0xFF)) & c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PSDnax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew ^ BYTE(0xFF)) & c4.rgbBlue ) ^ BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) & c4.rgbGreen) ^ BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) & c4.rgbRed  ) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_DPSDoaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((*pNew | c4.rgbBlue ) & BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew | c4.rgbGreen) & BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew | c4.rgbRed  ) & BYTE(0x00)) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSDPaoxx(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((BYTE(0x00) & *pNew) | c4.rgbBlue ) ^ BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((BYTE(0x00) & *pNew) | c4.rgbGreen) ^ BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((BYTE(0x00) & *pNew) | c4.rgbRed  ) ^ BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_SDPxan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) ^ *pNew) & c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) & c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) & c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PSDPxax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) ^ *pNew) & c4.rgbBlue ) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) & c4.rgbGreen) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) & c4.rgbRed  ) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_DSPDaoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((*pNew & BYTE(0x00)) | c4.rgbBlue ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew & BYTE(0x00)) | c4.rgbGreen) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((*pNew & BYTE(0x00)) | c4.rgbRed  ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSnao(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew; pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew; pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0xFF)) & BYTE(0x00)) | *pNew; pNew++;
		x1++;
	}
}

void Line8To24_DSno(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  ^ BYTE(0xFF)) | *pNew; pNew++;
		*pNew = (c4.rgbGreen ^ BYTE(0xFF)) | *pNew; pNew++;
		*pNew = (c4.rgbRed   ^ BYTE(0xFF)) | *pNew; pNew++;
		x1++;
	}
}

void Line8To24_SPDSanax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbBlue ; pNew++;
		*pNew = (((c4.rgbGreen & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbGreen; pNew++;
		*pNew = (((c4.rgbRed   & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SDxPDxan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbBlue )) ^ BYTE(0xFF); pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbGreen)) ^ BYTE(0xFF); pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) & (*pNew ^ c4.rgbRed  )) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSxo(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  ^ BYTE(0x00)) | *pNew; pNew++;
		*pNew = (c4.rgbGreen ^ BYTE(0x00)) | *pNew; pNew++;
		*pNew = (c4.rgbRed   ^ BYTE(0x00)) | *pNew; pNew++;
		x1++;
	}
}

void Line8To24_DPSano(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  & BYTE(0x00)) ^ BYTE(0xFF)) | *pNew; pNew++;
		*pNew = ((c4.rgbGreen & BYTE(0x00)) ^ BYTE(0xFF)) | *pNew; pNew++;
		*pNew = ((c4.rgbRed   & BYTE(0x00)) ^ BYTE(0xFF)) | *pNew; pNew++;
		x1++;
	}
}

void Line8To24_PSa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = c4.rgbBlue  & BYTE(0x00); pNew++;
		*pNew = c4.rgbGreen & BYTE(0x00); pNew++;
		*pNew = c4.rgbRed   & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_SPDSnaoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((((c4.rgbBlue  ^ BYTE(0xFF)) & *pNew) | BYTE(0x00)) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen ^ BYTE(0xFF)) & *pNew) | BYTE(0x00)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   ^ BYTE(0xFF)) & *pNew) | BYTE(0x00)) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SPDSonoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((((c4.rgbBlue  | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbGreen | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((((c4.rgbRed   | *pNew) ^ BYTE(0xFF)) | BYTE(0x00)) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PSxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbGreen ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (c4.rgbRed   ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SPDnoa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew ^ BYTE(0xFF)) | BYTE(0x00)) & c4.rgbBlue ; pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) | BYTE(0x00)) & c4.rgbGreen; pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) | BYTE(0x00)) & c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SPDSxoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  ^ *pNew) | BYTE(0x00)) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen ^ *pNew) | BYTE(0x00)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   ^ *pNew) | BYTE(0x00)) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SDPnax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_PSDPoaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((BYTE(0x00) | *pNew) & c4.rgbBlue ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) | *pNew) & c4.rgbGreen) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) | *pNew) & c4.rgbRed  ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SDPoa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (BYTE(0x00) | *pNew) & c4.rgbBlue ; pNew++;
		*pNew = (BYTE(0x00) | *pNew) & c4.rgbGreen; pNew++;
		*pNew = (BYTE(0x00) | *pNew) & c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SPDoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew | BYTE(0x00)) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = ((*pNew | BYTE(0x00)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = ((*pNew | BYTE(0x00)) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSDxax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew ^ c4.rgbBlue ) & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((*pNew ^ c4.rgbGreen) & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = ((*pNew ^ c4.rgbRed  ) & BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_SPDSaoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  & *pNew) | BYTE(0x00)) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen & *pNew) | BYTE(0x00)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   & *pNew) | BYTE(0x00)) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_S(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = c4.rgbBlue ; pNew++;
		*pNew = c4.rgbGreen; pNew++;
		*pNew = c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SDPono(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) | *pNew) ^ BYTE(0xFF)) | c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) | *pNew) ^ BYTE(0xFF)) | c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) | *pNew) ^ BYTE(0xFF)) | c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SDPnao(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) | c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) | c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) & *pNew) | c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SPno(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbBlue ; pNew++;
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbGreen; pNew++;
		*pNew = (BYTE(0x00) ^ BYTE(0xFF)) | c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_PSDnoa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew ^ BYTE(0xFF)) | c4.rgbBlue ) & BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) | c4.rgbGreen) & BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) | c4.rgbRed  ) & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_PSDPxoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((BYTE(0x00) ^ *pNew) | c4.rgbBlue ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ *pNew) | c4.rgbGreen) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) ^ *pNew) | c4.rgbRed  ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PDSnax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ BYTE(0xFF)) & *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0xFF)) & *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0xFF)) & *pNew) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_SPDSoaxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  | *pNew) & BYTE(0x00)) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen | *pNew) & BYTE(0x00)) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   | *pNew) & BYTE(0x00)) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SSPxPDxax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbBlue )) ^ c4.rgbBlue ; pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbGreen)) ^ c4.rgbGreen; pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbRed  )) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_DPSanan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PSDPSaoxx(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  & BYTE(0x00)) | *pNew) ^ c4.rgbBlue ) ^ BYTE(0x00); pNew++;
		*pNew = (((c4.rgbGreen & BYTE(0x00)) | *pNew) ^ c4.rgbGreen) ^ BYTE(0x00); pNew++;
		*pNew = (((c4.rgbRed   & BYTE(0x00)) | *pNew) ^ c4.rgbRed  ) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_DPSxan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0x00)) & *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_PDSPxax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) ^ c4.rgbBlue ) & *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) ^ c4.rgbGreen) & *pNew) ^ BYTE(0x00); pNew++;
		*pNew = ((BYTE(0x00) ^ c4.rgbRed  ) & *pNew) ^ BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_SDPSaoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  & BYTE(0x00)) | *pNew) ^ c4.rgbBlue ) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbGreen & BYTE(0x00)) | *pNew) ^ c4.rgbGreen) ^ BYTE(0xFF); pNew++;
		*pNew = (((c4.rgbRed   & BYTE(0x00)) | *pNew) ^ c4.rgbRed  ) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSDanax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((*pNew & c4.rgbBlue ) ^ BYTE(0xFF)) & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((*pNew & c4.rgbGreen) ^ BYTE(0xFF)) & BYTE(0x00)) ^ *pNew; pNew++;
		*pNew = (((*pNew & c4.rgbRed  ) ^ BYTE(0xFF)) & BYTE(0x00)) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_SPxDSxan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ *pNew) & (BYTE(0x00) ^ c4.rgbBlue )) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) & (BYTE(0x00) ^ c4.rgbGreen)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) & (BYTE(0x00) ^ c4.rgbRed  )) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SPDnao(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew ^ BYTE(0xFF)) & BYTE(0x00)) | c4.rgbBlue ; pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) & BYTE(0x00)) | c4.rgbGreen; pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) & BYTE(0x00)) | c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SDno(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (*pNew ^ BYTE(0xFF)) | c4.rgbBlue ; pNew++;
		*pNew = (*pNew ^ BYTE(0xFF)) | c4.rgbGreen; pNew++;
		*pNew = (*pNew ^ BYTE(0xFF)) | c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SDPxo(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (BYTE(0x00) ^ *pNew) | c4.rgbBlue ; pNew++;
		*pNew = (BYTE(0x00) ^ *pNew) | c4.rgbGreen; pNew++;
		*pNew = (BYTE(0x00) ^ *pNew) | c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SDPano(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) & *pNew) ^ BYTE(0xFF)) | c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) & *pNew) ^ BYTE(0xFF)) | c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) & *pNew) ^ BYTE(0xFF)) | c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_PDSoa(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  | *pNew) & BYTE(0x00); pNew++;
		*pNew = (c4.rgbGreen | *pNew) & BYTE(0x00); pNew++;
		*pNew = (c4.rgbRed   | *pNew) & BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_PDSoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbGreen | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = ((c4.rgbRed   | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DSPDxax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew ^ BYTE(0x00)) & c4.rgbBlue ) ^ *pNew; pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) & c4.rgbGreen) ^ *pNew; pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) & c4.rgbRed  ) ^ *pNew; pNew++;
		x1++;
	}
}

void Line8To24_PSDPaoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((BYTE(0x00) & *pNew) | c4.rgbBlue ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) & *pNew) | c4.rgbGreen) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) & *pNew) | c4.rgbRed  ) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SDPSxax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ BYTE(0x00)) & *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0x00)) & *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0x00)) & *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_PDSPaoxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((BYTE(0x00) & c4.rgbBlue ) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) & c4.rgbGreen) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		*pNew = (((BYTE(0x00) & c4.rgbRed  ) | *pNew) ^ BYTE(0x00)) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SDPSanax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((c4.rgbBlue  & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbBlue ; pNew++;
		*pNew = (((c4.rgbGreen & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbGreen; pNew++;
		*pNew = (((c4.rgbRed   & BYTE(0x00)) ^ BYTE(0xFF)) & *pNew) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SPxPDxan(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbBlue )) ^ BYTE(0xFF); pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbGreen)) ^ BYTE(0xFF); pNew++;
		*pNew = ((*pNew ^ BYTE(0x00)) & (BYTE(0x00) ^ c4.rgbRed  )) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_SSPxDSxax(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ *pNew) & (BYTE(0x00) ^ c4.rgbBlue )) ^ c4.rgbBlue ; pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) & (BYTE(0x00) ^ c4.rgbGreen)) ^ c4.rgbGreen; pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) & (BYTE(0x00) ^ c4.rgbRed  )) ^ c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_DSPDSanaxxn(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (((((c4.rgbBlue  & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbBlue ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((((c4.rgbGreen & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbGreen) ^ *pNew) ^ BYTE(0xFF); pNew++;
		*pNew = (((((c4.rgbRed   & *pNew) ^ BYTE(0xFF)) & BYTE(0x00)) ^ c4.rgbRed  ) ^ *pNew) ^ BYTE(0xFF); pNew++;
		x1++;
	}
}

void Line8To24_DPSao(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  & BYTE(0x00)) | *pNew; pNew++;
		*pNew = (c4.rgbGreen & BYTE(0x00)) | *pNew; pNew++;
		*pNew = (c4.rgbRed   & BYTE(0x00)) | *pNew; pNew++;
		x1++;
	}
}

void Line8To24_DPSxno(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ BYTE(0x00)) ^ BYTE(0xFF)) | *pNew; pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0x00)) ^ BYTE(0xFF)) | *pNew; pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0x00)) ^ BYTE(0xFF)) | *pNew; pNew++;
		x1++;
	}
}

void Line8To24_SDPao(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (BYTE(0x00) & *pNew) | c4.rgbBlue ; pNew++;
		*pNew = (BYTE(0x00) & *pNew) | c4.rgbGreen; pNew++;
		*pNew = (BYTE(0x00) & *pNew) | c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_SDPxno(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) | c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) | c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) ^ *pNew) ^ BYTE(0xFF)) | c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_DSo(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = c4.rgbBlue  | *pNew; pNew++;
		*pNew = c4.rgbGreen | *pNew; pNew++;
		*pNew = c4.rgbRed   | *pNew; pNew++;
		x1++;
	}
}

void Line8To24_SDPnoo(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) | c4.rgbBlue ; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) | c4.rgbGreen; pNew++;
		*pNew = ((BYTE(0x00) ^ BYTE(0xFF)) | *pNew) | c4.rgbRed  ; pNew++;
		x1++;
	}
}

void Line8To24_P(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = BYTE(0x00); pNew++;
		*pNew = BYTE(0x00); pNew++;
		*pNew = BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_PDSono(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  | *pNew) ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		*pNew = ((c4.rgbGreen | *pNew) ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		*pNew = ((c4.rgbRed   | *pNew) ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_PDSnao(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ BYTE(0xFF)) & *pNew) | BYTE(0x00); pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0xFF)) & *pNew) | BYTE(0x00); pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0xFF)) & *pNew) | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_PSno(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		*pNew = (c4.rgbGreen ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		*pNew = (c4.rgbRed   ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_PSDnao(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew ^ BYTE(0xFF)) & c4.rgbBlue ) | BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) & c4.rgbGreen) | BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) & c4.rgbRed  ) | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_PDno(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (*pNew ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		*pNew = (*pNew ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		*pNew = (*pNew ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_PDSxo(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  ^ *pNew) | BYTE(0x00); pNew++;
		*pNew = (c4.rgbGreen ^ *pNew) | BYTE(0x00); pNew++;
		*pNew = (c4.rgbRed   ^ *pNew) | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_PDSano(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  & *pNew) ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		*pNew = ((c4.rgbGreen & *pNew) ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		*pNew = ((c4.rgbRed   & *pNew) ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_PDSao(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  & *pNew) | BYTE(0x00); pNew++;
		*pNew = (c4.rgbGreen & *pNew) | BYTE(0x00); pNew++;
		*pNew = (c4.rgbRed   & *pNew) | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_PDSxno(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ *pNew) ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		*pNew = ((c4.rgbGreen ^ *pNew) ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		*pNew = ((c4.rgbRed   ^ *pNew) ^ BYTE(0xFF)) | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_DPo(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = BYTE(0x00) | *pNew; pNew++;
		*pNew = BYTE(0x00) | *pNew; pNew++;
		*pNew = BYTE(0x00) | *pNew; pNew++;
		x1++;
	}
}

void Line8To24_DPSnoo(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((c4.rgbBlue  ^ BYTE(0xFF)) | BYTE(0x00)) | *pNew; pNew++;
		*pNew = ((c4.rgbGreen ^ BYTE(0xFF)) | BYTE(0x00)) | *pNew; pNew++;
		*pNew = ((c4.rgbRed   ^ BYTE(0xFF)) | BYTE(0x00)) | *pNew; pNew++;
		x1++;
	}
}

void Line8To24_PSo(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = c4.rgbBlue  | BYTE(0x00); pNew++;
		*pNew = c4.rgbGreen | BYTE(0x00); pNew++;
		*pNew = c4.rgbRed   | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_PSDnoo(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = ((*pNew ^ BYTE(0xFF)) | c4.rgbBlue ) | BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) | c4.rgbGreen) | BYTE(0x00); pNew++;
		*pNew = ((*pNew ^ BYTE(0xFF)) | c4.rgbRed  ) | BYTE(0x00); pNew++;
		x1++;
	}
}

void Line8To24_DPSoo(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = (c4.rgbBlue  | BYTE(0x00)) | *pNew; pNew++;
		*pNew = (c4.rgbGreen | BYTE(0x00)) | *pNew; pNew++;
		*pNew = (c4.rgbRed   | BYTE(0x00)) | *pNew; pNew++;
		x1++;
	}
}

void Line8To24_1(StrucLineXTox &r)
{
	int x1 = r.srcx, x2 = x1 + r.cx;
	BYTE *pNew = r.pNew + 3 * r.newx;

	while (x1 != x2) {
		RGBQUAD c4 = r.p4[r.pSrc[x1]];
		*pNew = BYTE(0xFF); pNew++;
		*pNew = BYTE(0xFF); pNew++;
		*pNew = BYTE(0xFF); pNew++;
		x1++;
	}
}

