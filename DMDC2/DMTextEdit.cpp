
#include "StdAfx.h"
#include "DMTextEdit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

bool DMTextEnc_t::Decode()
{
	char esc = 0;

	while (xSrc < cxSrc) {
		BYTE c = pSrc[xSrc];

		LPCSTR psz = NULL;
		char c1 = -1;
		char c2 = -1;

		if (c == 31 && accepteof) {
			break;
		}

		if (esc == 0) {
			if (c <= 25) {
				c1 = 'A' + c;
			}
			else if (c == 26) c1 = ' ';
			else if (c == 27) c1 = '.';
			else if (c == 28) c1 = '/';
			else if (c == 29) esc = 1;
			else if (c == 30) esc = 2;
			else if (c == 31) c1 = 0;
		}
		else if (esc == 1) {
			if (c <= 23) c1 = '*', c2 = 'A' + c;
			else if (c <= 31) c1 = '0' + c - 24;

			esc = 0;
		}
		else {
			if (c == 0) psz = "AND ";
			else if (c == 1) psz = "!";
			else if (c == 2) psz = "THE ";
			else if (c == 3) psz = "YOU ";
			else c1 = ' ';

			esc = 0;
		}

		if (psz != NULL) {
			while (*psz != 0) {
				if (xDst < cxDst) pDst[xDst] = *psz, xDst++;
				else return false;
				psz++;
			}
		}
		else if (c1 != -1) {
			if (xDst < cxDst) pDst[xDst] = c1, xDst++;
			else return false;

			if (c2 != -1) {
				if (xDst < cxDst) pDst[xDst] = c2, xDst++;
				else return false;
			}
		}

		xSrc++;
	}

	if (capeof) {
		if (xDst < cxDst) pDst[xDst] = 0, xDst++;
		else return false;
	}

	return true;
}

bool DMTextEnc_t::Encode()
{
	char esc = 0;

	while (xSrc < cxSrc) {
		BYTE c = toupper(pSrc[xSrc]);

		char c1 = -1;
		char c2 = -1;

		if (esc == 0) {
			if ('A' <= c && c <= 'Z') {
				c1 = c - 'A';
			}
			else if ('0' <= c && c <= '9') {
				c1 = DMtcESC1;
				c2 = c - '0' + 24;
			}
			else if (c == '!') {
				c1 = DMtcESC2;
				c2 = 1;
			}
			else if (c == '.') {
				c1 = DMtcDot;
			}
			else if (c == '\n') {
				c1 = DMtcLF;
			}
			else if (c == '*') {
				esc = 1;
			}
			else if (c == 0 && accepteof) {
				break;
			}
			else {
				c1 = DMtcSPC;
			}
		}
		else if (esc == 1) {
			if ('A' <= c && c <= 'X') {
				c1 = DMtcESC1;
				c2 = c - 'A';
			}
			else {
				c1 = DMtcSPC;
			}
		}

		if (c1 != -1) {
			if (cxDst < xDst) pDst[xDst] = c1, xDst++;
			else return false;

			if (c2 != -1) {
				if (cxDst < xDst) pDst[xDst] = c2, xDst++;
				else return false;
			}
		}

		xSrc++;
	}

	if (capeof) {
		if (cxDst < xDst) pDst[xDst] = 31, xDst++;
		else return false;
	}

	return true;
}

void DMTextEdit_t::Load(const SizeBuff &fvTextData)
{
	const WORD *pwPos = (const WORD *)fvTextData.GetData();
	const WORD *pwEnd = pwPos + (fvTextData.GetSize() / 2);
	vec.SetSize(0, 3 * (pwEnd - pwPos));
	for (; pwPos < pwEnd; pwPos++) {
		WORD w = *pwPos;
		vec.Add((w >> 10) & 0x1F);
		vec.Add((w >>  5) & 0x1F);
		vec.Add((w      ) & 0x1F);
	}
}

bool DMTextEdit_t::Save(SizeBuff &fvTextData)
{
	int cx = (vec.GetSize() + 2) / 3;

	if (!fvTextData.Alloc(cx*2))
		return false;
	WORD *pwPos = (WORD *)fvTextData.GetData();
	WORD *pwEnd = pwPos + cx;

	for (; (vec.GetSize() % 3) != 0; vec.Add(DMtcEND));

	const BYTE *pbPos = vec.GetData();

	for (; pwPos < pwEnd; pwPos++, pbPos += 3) {
		WORD w = 0
			|((pbPos[0] & 0x1F) << 10)
			|((pbPos[1] & 0x1F) <<  5)
			|((pbPos[2] & 0x1F)      )
			;
		*pwPos = w;
	}
	return true;
}

int DMTextEdit_t::AddStr(LPCSTR psz)
{
	if (vec.GetSize() > 0) {
		if (vec[vec.GetSize() -1] != DMtcEND) {
			vec.Add(DMtcEND);
		}
	}

	for (; (vec.GetSize() % 3) != 0; vec.Add(31));

	int r = vec.GetSize() / 3;

	while (*psz != 0) {
		char c = toupper(*psz);
		if ('A' <= c && c <= 'Z') {
			c = c - 'A';
		}
		else if ('0' <= c && c <= '9') {
			vec.Add(DMtcESC1);
			c = c - '0' + 24;
		}
		else if (c == '!') {
			vec.Add(DMtcESC2);
			c = 1;
		}
		else if (c == '.') {
			c = DMtcDot;
		}
		else if (c == '\n') {
			c = DMtcLF;
		}
		else {
			c = DMtcSPC;
		}
		vec.Add(c);
		psz = _tcsninc(psz, 1);
	}
	vec.Add(DMtcEND);

	return r;
}

CString DMTextEdit_t::GetStr(UINT x)
{
	DMTextEnc_t e;
	e.pSrc = vec.GetData();
	e.xSrc = 3*x;
	e.cxSrc = vec.GetSize();

	BYTE c[200];
	e.pDst = c;
	e.xDst = 0;
	e.cxDst = 200;

	if (e.Decode())
		return c;
	return "";
}

void DMTextEdit_t::EnumKey(CWordArray &res)
{
	res.RemoveAll();

	const BYTE *pbPos = vec.GetData();
	UINT x = 0;
	UINT cx = vec.GetSize();

	bool cap = true;

	while (x < cx) {
		BYTE c1 =              pbPos[x  ];
		BYTE c2 = (x+1 < cx) ? pbPos[x+1] : -1;
		BYTE c3 = (x+2 < cx) ? pbPos[x+2] : -1;

		if (cap) {
			cap = false;
			res.Add(x / 3);
		}

		if (c1 == DMtcEND || c2 == DMtcEND || c3 == DMtcEND)
			cap = true;

		x += 3;
	}
}
