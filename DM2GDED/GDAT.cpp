// GDAT.cpp: CGDAT クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DM2GDED.h"
#include "GDAT.h"
#include "CompatDC.h"
#include "DM2TTbl.h"
#include "DM2ImageComp.h"
#include "DM2TextEncoder.h"
#include "DM2ImageExch.h"
#include <algorithm>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef _UNICODE
#error _UNICODE NOT SUPPORTED
#endif

#define ENDIAN_TO_ANTI_ENDIAN(F) (!(F))

// 00 00-08-00 00: Chain image tracking info.

#define GDOF_PS_PCAT    (GDFF_LANG_EN_ENC |GDFF_PICFMT_C4C8   |GDFF_SND5SND6|GDFF_PAL_IMAGE|GDFF_LITTLE_ENDIAN|                0)
#define GDOF_PS_MAC     (GDFF_LANG_EN_ENC |GDFF_PICFMT_C4C8   |GDFF_SND5SND6|GDFF_PAL_IMAGE|                 0|                0)
#define GDOF_PS_PCBETA  (GDFF_LANG_EN     |GDFF_PICFMT_SEGA   |GDFF_SND3    |GDFF_PAL_RAW  |GDFF_LITTLE_ENDIAN|GDFF_RESTRICT_PAL)
#define GDOF_PS_MEGA    (GDFF_LANG_EN_ENC |GDFF_PICFMT_SEGA   |GDFF_SND7    |GDFF_PAL_SCENE|                 0|GDFF_RESTRICT_PAL)
#define GDOF_PS_AMIGA   (GDFF_LANG_EN_ENC |GDFF_PICFMT_C4     |GDFF_SND5SND6|GDFF_PAL_IMAGE|                 0|GDFF_RESTRICT_PAL)
#define GDOF_PS_PC9801  (GDFF_LANG_JP1_ENC|GDFF_PICFMT_C4     |            0|GDFF_PAL_IMAGE|GDFF_LITTLE_ENDIAN|                0)
#define GDOF_PS_PSV	    (GDFF_LANG_JP2_ENC|GDFF_PICFMT_C4     |GDFF_SNDA    |GDFF_PAL_IMAGE|GDFF_LITTLE_ENDIAN|                0)
#define GDOF_PS_FMTOWNS (GDFF_LANG_JP2_ENC|GDFF_PICFMT_FMTOWNS|GDFF_SND9    |GDFF_PAL_SCENE|GDFF_LITTLE_ENDIAN|GDFF_RESTRICT_PAL)
#define GDOF_PS_MACJP   (GDFF_LANG_JP2_ENC|GDFF_PICFMT_C4     |GDFF_SND7    |GDFF_PAL_IMAGE|                 0|GDFF_RESTRICT_PAL)

//////////////////////////////////////////////////////////////////////
// 

namespace
{

UI32 R4(UI32 i)
{
	UI32 r = i & 3;
	if (r)
		return i + 4 - r;
	return i;
}

bool ConvFromTo(const CString &s, SizeBuff &t)
{
	if (!t.Alloc(s.GetLength(), (LPCSTR)s))
		return false;
	return true;
}

inline void ConvFromTo(const RGBQUAD &rq, COLORREF &clr)
{
	clr = RGB(rq.rgbRed, rq.rgbGreen, rq.rgbBlue);
}

};

//////////////////////////////////////////////////////////////////////
// CGDEntry

CGDEntry CGDEntry::ERR(-1, -1, -1, -1, -1, -1);
CGDEntry CGDEntry::Min( 0,  0,  0,  0,  0,  0);
CGDEntry CGDEntry::Max(-1, -1, -1, -1, -1, -1);

LPCSTR CGDEntry::GetFriendlyTypeName(BYTE c)
{
	switch (c) {
	case 0x01: return "Image";
	case 0x02: return "Sound";
	case 0x03: return "HMP format";
	case 0x05: return "Text";
	case 0x0B: return "Word value";
	case 0x0C: return "Image offset";

	case 0x00: return "00";
	case 0x04: return "Rect";
	case 0x06: return "06 (raw)";
	case 0x07: return "07 (raw)";
	case 0x08: return "08 (raw)";
	case 0x09: return "Palette (IRGB)";
	case 0x0D: return "Palette (index)";
	case 0x0E: return "0E (raw)";
	}
	return "?";
}

LPCSTR CGDEntry::GetFriendlyClass1Name(BYTE c)
{
	switch (c) {
	case 0x00: return "Technical Data";
	case 0x01: return "Interface - Main Screen";
	case 0x02: return "SPX:Spell Definition"; // forspx:
	case 0x03: return "Messages";
	case 0x04: return "Music (HMP format)";
	case 0x05: return "Main Menu and Credits";
	case 0x06: return "Credits";
	case 0x07: return "Interface - Character Sheet";
	case 0x08: return "Dungeon Graphics";
	case 0x09: return "Wall Ornates";
	case 0x0A: return "Floor Ornates";
	case 0x0B: return "Door Ornates";
	case 0x0C: return "Door Buttons";
	case 0x0D: return "Missiles";
	case 0x0E: return "Doors";
	case 0x0F: return "Creatures";
	case 0x10: return "Weapons";
	case 0x11: return "Clothes";
	case 0x12: return "Scrolls";
	case 0x13: return "Potions";
	case 0x14: return "Chests";
	case 0x15: return "Miscellaneous";
	case 0x16: return "Champions";
	case 0x17: return "Skullkeep and Weather";
	case 0x18: return "Teleporter";
	case 0x19: return "SPX:Creature AI"; // forspx:
	case 0x1A: return "Dialog boxes";
	case 0x1C: return "Japanese Fonts";
	}
	return "";
}

//////////////////////////////////////////////////////////////////////
// CGDATFDH

class CGDATFDH
{
	// 
	struct MemSrcIo8 {
		// 
		const BYTE *pbBegin, *pbPos, *pbEnd;

	public:
		// 
		MemSrcIo8(const void *pv, UINT cb): pbBegin((const BYTE *)pv), pbPos(pbBegin), pbEnd(pbPos + cb)
		{

		}
		// 
		void Rewind()
		{
			pbPos = pbBegin;
		}
		// 
		int ReadByte()
		{
			int r = -1;
			if (pbPos < pbEnd) {
				r = *pbPos; pbPos++;
			}
			return r;
		}
		// 
		bool IsEOF()
		{
			return (pbPos == pbEnd);
		}
		// 
		bool Skip(UINT n)
		{
			if (pbPos + n <= pbEnd) {
				pbPos += n;
				return true;
			}
			return false;
		}
	};
	// 
	struct MemSrcIo4over8 {
		// 
		MemSrcIo8 &rio8;
		// 
		bool fNextByte;
		// 
		BYTE c;

		// 
		inline int ReadByte() { return rio8.ReadByte(); }

	public:
		// 
		MemSrcIo4over8(MemSrcIo8 &rio8): rio8(rio8), fNextByte(true) { }
		// 
		int Read4Bits()
		{
			int r;
			if (fNextByte) {
				if ((r = ReadByte()) < 0) {
					r = -1;
				} else {
					c = r;
					r = (r >> 4) & 0xF;

					fNextByte = false;
				}
			} else {
				r = c & 0xF;

				fNextByte = true;
			}
			return r;
		}
		
	};

	// 
	bool fLE;

	// 
	static int ReadDuration(MemSrcIo4over8 &thruIn)
	{
		int nDuration;
		int v = thruIn.Read4Bits();
		if (v < 0) return -1;
		if (v == 0xF) { // F
			int v = thruIn.Read4Bits();
			if (v < 0) return -1;
			if (v == 0xF) { // FF
				int v = thruIn.Read4Bits();
				if (v < 0) return -1;
				if (v == 0xF) { // FFF
					int v4[4] = {
						thruIn.Read4Bits(),
						thruIn.Read4Bits(),
						thruIn.Read4Bits(),
						thruIn.Read4Bits(),
					};
					if (v4[0] < 0 || v4[1] < 0 || v4[2] < 0 || v4[3] < 0) return false;

					nDuration = 0
						| v4[0] * 4096
						| v4[1] * 256
						| v4[2] * 16
						| v4[3]
						;
				} else { // FF?
					nDuration = v + 257;
				}
			} else { // F?
				int v1 = thruIn.Read4Bits();
				if (v1 < 0) return false;

				nDuration = 17 + v * 16 + v1;
			}
		} else { // ?
			nDuration = v + 2;
		}
		return nDuration;
	}
	// 
	bool IsIMG3or4(MemSrcIo8 &rio8, bool fHasPal16, bool fDiff)
	{
		BYTE v[4] = {
			rio8.ReadByte(),
			rio8.ReadByte(),
			rio8.ReadByte(),
			rio8.ReadByte(),
		};
		WORD cx = fLE ? MAKEWORD(v[0], v[1]) : MAKEWORD(v[1], v[0]);
		WORD cy = fLE ? MAKEWORD(v[2], v[3]) : MAKEWORD(v[3], v[2]);
		cx &= 0x01FF;
		cy &= 0x01FF;
		MemSrcIo4over8 rio4(rio8);
		rio4.Read4Bits();
		rio4.Read4Bits();
		rio4.Read4Bits();
		rio4.Read4Bits();
		rio4.Read4Bits();
		if (!fDiff) rio4.Read4Bits();
		DWORD iPixel = 0, nPixels = cx * cy;
		int v1, v2;
		bool fSpill = false;
		while (iPixel < nPixels) {
			if ((v1 = rio4.Read4Bits()) < 0)
				return false;

			switch (v1 & 7) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				fSpill = false;
				break;
			case 6:
				fSpill = true;
				break;
			case 7:
				{
				if ((v2 = rio4.Read4Bits()) < 0)
					return false;

				fSpill = false;
				break;
				}
			}
			int nDuration = 1;
			if (v1 & 8) {
				nDuration = ReadDuration(rio4);
			}

			if (nDuration < 0)
				return false;

			if (fSpill && iPixel < cx)
				return false;

			iPixel += nDuration;
		}
		if (iPixel != nPixels)
			return false;
		if (fHasPal16) {
			for (int x = 0; x < 16; x++)
				if (rio8.ReadByte() < 0)
					return false;
		}
		if (!rio8.IsEOF())
			return false;
		return true;
	}
	// 
	bool IsDM2PCC8(MemSrcIo8 &rio8)
	{
		BYTE v[4] = {
			rio8.ReadByte(),
			rio8.ReadByte(),
			rio8.ReadByte(),
			rio8.ReadByte(),
		};
		WORD cx = fLE ? MAKEWORD(v[0], v[1]) : MAKEWORD(v[1], v[0]);
		WORD cy = fLE ? MAKEWORD(v[2], v[3]) : MAKEWORD(v[3], v[2]);
		if ((cy & 0x7E00) != 0x7C00)
			return false;
		cx &= 0x01FF;
		cy &= 0x01FF;
		rio8.ReadByte();
		BYTE cType = rio8.ReadByte();
		if (cType != 2 && cType != 3)
			return false;
		bool fIsType3 = (cType == 3);

		static const BYTE nTblBits[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

		DWORD iPixel = 0, nPixels = cx * cy;

		while (iPixel < nPixels) {
			int nCmds = rio8.ReadByte();
			if (nCmds < 0) return false;
			for (BYTE iCmd = 0; iCmd < 8; iCmd++) {
				if (nCmds & nTblBits[iCmd]) {
					// Pixel Cmd.
					if (rio8.ReadByte() < 0)
						return false;
				} else {
					// Repeat Cmd.
					int v1 = rio8.ReadByte();
					int v2 = rio8.ReadByte();
					if (v1 < 0 || v2 < 0) return false;
					WORD v = MAKEWORD(v1, v2);
					UINT nLen = (fIsType3 ? (v & 31) : (v & 15)) + 3;
					UINT nBack = fIsType3 ? (v >> 5) : (v >> 4);
					if (iPixel < nBack)
						return false;
					if (nPixels < iPixel + nLen)
						return false;
					iPixel += nLen;
				}
			}
		}

		if (iPixel != nPixels || !rio8.IsEOF())
			return false;
		return true;
	}
	// 
	bool IsSND5or6(MemSrcIo8 &rio8)
	{
		BYTE v[4] = {
			rio8.ReadByte(),
			rio8.ReadByte(),
			rio8.ReadByte(),
			rio8.ReadByte(),
		};
		WORD w0 = fLE ? MAKEWORD(v[0], v[1]) : MAKEWORD(v[1], v[0]);
		if (w0 != 0x2B77)
			return false;
		if (v[2] != 0x08 || v[3] != 0x01)
			return false;
		return true;
	}
	// 
	bool IsSND3or7(MemSrcIo8 &rio8, bool fSND3)
	{
		if (fLE && !fSND3)
			return false;
		BYTE v[2] = {
			rio8.ReadByte(),
			rio8.ReadByte(),
		};
		WORD w0 = fLE ? MAKEWORD(v[0], v[1]) : MAKEWORD(v[1], v[0]);
		if (fSND3) {
			if (!rio8.Skip(w0))
				return false;
		} else {
			if (!rio8.Skip(w0 - 2))
				return false;
		}
		if (!rio8.IsEOF())
			return false;
		return true;
	}
	// 
	bool IsSND9(MemSrcIo8 &rio8)
	{
		if (!fLE)
			return false;
		BYTE v[2] = {
			rio8.ReadByte(),
			rio8.ReadByte(),
		};
		WORD w0 = fLE ? MAKEWORD(v[0], v[1]) : MAKEWORD(v[1], v[0]);
		if (!rio8.Skip(w0 - 2))
			return false;
		if (!rio8.IsEOF())
			return false;
		return true;
	}
	// 
	bool IsIMG2FMTowns(MemSrcIo8 &rio8)
	{
		MemSrcIo4over8 rio4(rio8);
		BYTE v[4] = {
			rio8.ReadByte(),
			rio8.ReadByte(),
			rio8.ReadByte(),
			rio8.ReadByte(),
		};
		WORD cx = fLE ? MAKEWORD(v[0], v[1]) : MAKEWORD(v[1], v[0]);
		WORD cy = fLE ? MAKEWORD(v[2], v[3]) : MAKEWORD(v[3], v[2]);
		cx &= 0x01FF;
		cy &= 0x01FF;

		DWORD iPixel = 0, nPixels = cx * cy;

		while (iPixel < nPixels) {
			int v1, v2;
			if ((v1 = rio4.Read4Bits()) < 0) return false;
			if ((v2 = rio4.Read4Bits()) < 0) return false;

			int iPal;
			int nLen = 0;
			int err = 0;

			if ((v1 & 8) == 0) {
				nLen = v1 +1;
				iPal = v2;

				iPixel += nLen;
			} else if (v1 == 8) {
				nLen = 1;
				nLen += (rio4.Read4Bits() << 4);
				nLen += (rio4.Read4Bits() << 0);
				iPal = v2;

				iPixel += nLen;
			} else if (v1 == 0xC) {
				nLen = 1;
				nLen += (rio4.Read4Bits() <<12);
				nLen += (rio4.Read4Bits() << 8);
				nLen += (rio4.Read4Bits() << 4);
				nLen += (rio4.Read4Bits() << 0);
				iPal = v2;

				iPixel += nLen;
			} else if (v1 == 0xB) {
				nLen = 1;
				nLen += (rio4.Read4Bits() << 4);
				nLen += (rio4.Read4Bits() << 0);
				iPal = v2;

				iPixel += nLen;
				iPixel += 1;
			} else if (v1 == 0xF) {
				nLen = 1;
				nLen += (rio4.Read4Bits() <<12);
				nLen += (rio4.Read4Bits() << 8);
				nLen += (rio4.Read4Bits() << 4);
				nLen += (rio4.Read4Bits() << 0);
				iPal = v2;

				iPixel += nLen;
				iPixel += 1;
			} else if (v1 == 0x9) {
				nLen = 0;
				nLen += (rio4.Read4Bits() << 4);
				nLen += (rio4.Read4Bits() << 0);
				if ((nLen & 1) == 0) {
					// even
					iPixel += 1;
				}
				else {
					// odd
					nLen++;
				}
				for (int t = 0; t < nLen; t++) {
					int n3 = rio4.Read4Bits();
					iPixel += 1;
				}
			} else if (v1 == 0xA) {
				nLen = v1 +1;
				iPixel += nLen;
			} else {
				//ASSERT(false); // won't reach here, so no more break here
				return false;
			}

			if (err != 0)
				return false;
		}
		return iPixel == nPixels;
	}

public:
	// 
	enum {
		itNo,
		itIMG3or4,
		itIMG3or4Pal,
		itDM2PCC8,
		itIMG2FMTowns,
	};
	// 
	enum {
		esNo,
		esSND5or6,
		esSND3,
		esSND7,
		esSND9
	};

	// 
	CGDATFDH(bool fLE): fLE(fLE) { }
	// 
	int DecideImageFormat(const void *pv, UINT cb);
	// 
	int DecideEffectSound(const void *pv, UINT cb);
};

int CGDATFDH::DecideImageFormat(const void *pv, UINT cb)
{
	MemSrcIo8 rio8(pv, cb);
	//
	if (cb < 4) return -1;

	rio8.Rewind();
	if (IsIMG2FMTowns(rio8))
		return itIMG2FMTowns;
	//
	if (cb < 7) return -1;

	rio8.Rewind();
	if (IsIMG3or4(rio8, true, false))
		return itIMG3or4Pal;

	rio8.Rewind();
	if (IsIMG3or4(rio8, true, true))
		return itIMG3or4Pal;

	rio8.Rewind();
	if (IsIMG3or4(rio8, false, false))
		return itIMG3or4;
	//
	if (cb < 8) return -1;

	rio8.Rewind();
	if (IsDM2PCC8(rio8))
		return itDM2PCC8;
	//
	return itNo;
}

int CGDATFDH::DecideEffectSound(const void *pv, UINT cb)
{
	MemSrcIo8 rio8(pv, cb);
	// 
	if (cb < 2) return -1;

	if (IsSND3or7(rio8, true))
		return esSND3;

	rio8.Rewind();
	if (IsSND3or7(rio8, false))
		return esSND7;

	rio8.Rewind();
	if (IsSND9(rio8))
		return esSND9;
	//
	if (cb < 4)
		return false;

	rio8.Rewind();
	if (IsSND5or6(rio8))
		return esSND5or6;
	//
	return esNo;
}

//////////////////////////////////////////////////////////////////////
// CGDAT::RFio

struct CGDAT::RFio
{
	// 
	FILE *f;
	// 
	BOOL fAE;

	// 
	void test16(UI32 &x)
	{
		if (fAE) {
			union {
				UI32 n;
				BYTE b[2];
			} a, b;
			a.n = x;
			b.n = 0;
			b.b[0] = a.b[1];
			b.b[1] = a.b[0];
			x = b.n;
		}
	}
	// 
	void test32(UI32 &x)
	{
		if (fAE) {
			union {
				UI32 n;
				BYTE b[4];
			} a, b;
			a.n = x;
			b.n = 0;
			b.b[0] = a.b[3];
			b.b[1] = a.b[2];
			b.b[2] = a.b[1];
			b.b[3] = a.b[0];
			x = b.n;
		}
	}
	// 
	int Read(void *pv, UINT cb)
	{
		return fread(pv, 1, cb, f);
	}

public:
	// 
	RFio(): f(NULL) { }
	// 
	~RFio()
	{
		Close();
	}
	// 
	void Close()
	{
		if (f)
			fclose(f),
			f = NULL;
	}
	// 
	BOOL OpenRead(LPCSTR pszFileIn, BOOL fAE)
	{
		Close();
		if (this->f = fopen(pszFileIn, "rb")) {
			this->fAE = fAE;
			return TRUE;
		}
		return FALSE;
	}
	// 
	I32 ReadWord()
	{
		UI32 x = 0;
		if (Read(&x, 2) == 2)
			return test16(x), x;
		return -1;
	}
	// 
	I32 ReadDWord()
	{
		UI32 x = 0;
		if (Read(&x, 4) == 4)
			return test32(x), x;
		return -1;
	}
};

//////////////////////////////////////////////////////////////////////
// CGDAT::WFio

struct CGDAT::WFio
{
	// 
	FILE *f;
	// 
	BOOL fAE;

	// 
	void test16(UI32 &x)
	{
		if (fAE) {
			union {
				UI32 n;
				BYTE b[2];
			} a, b;
			a.n = x;
			b.n = 0;
			b.b[0] = a.b[1];
			b.b[1] = a.b[0];
			x = b.n;
		}
	}
	// 
	void test32(UI32 &x)
	{
		if (fAE) {
			union {
				UI32 n;
				BYTE b[4];
			} a, b;
			a.n = x;
			b.n = 0;
			b.b[0] = a.b[3];
			b.b[1] = a.b[2];
			b.b[2] = a.b[1];
			b.b[3] = a.b[0];
			x = b.n;
		}
	}
	// 
	int Write(const void *pv, UINT cb)
	{
		return fwrite(pv, 1, cb, f);
	}

public:
	// 
	WFio(): f(NULL) { }
	// 
	~WFio()
	{
		Close();
	}
	// 
	void Close()
	{
		if (f)
			fclose(f),
			f = NULL;
	}
	// 
	BOOL OpenCreate(LPCSTR pszFileInto, BOOL fAE)
	{
		Close();
		if (this->f = fopen(pszFileInto, "wb")) {
			this->fAE = fAE;
			return TRUE;
		}
		return FALSE;
	}
	// 
	bool WriteByte(UI8 x)
	{
		if (Write(&x, 1) == 1)
			return true;
		return false;
	}
	// 
	bool WriteWord(UI32 x)
	{
		test16(x);
		if (Write(&x, 2) == 2)
			return true;
		return false;
	}
	// 
	bool WriteDWord(UI32 x)
	{
		test32(x);
		if (Write(&x, 4) == 4)
			return true;
		return false;
	}
};

//////////////////////////////////////////////////////////////////////
// CGDAT::BM

struct CGDAT::BM {
	// 
	HBITMAP hbm;

	// 
	BM(): hbm(NULL) { }
	// 
	~BM()
	{
		if (hbm) DeleteObject(hbm);
	}
	// 
	HBITMAP Detach()
	{
		HBITMAP hbmRet = hbm;
		hbm = NULL;
		return hbmRet;
	}
	// 
	bool CreateDM2BM4BPP(HDC hDC, UINT cx, UINT cy, const DMGHLciPAL &rPal, BYTE *&pbBits)
	{
		CHAR cbi[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 16];
		ZeroMemory(&cbi, sizeof(cbi));
		PBITMAPINFO pbi = (PBITMAPINFO)cbi;
		pbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		pbi->bmiHeader.biWidth = cx;
		pbi->bmiHeader.biHeight = cy;
		pbi->bmiHeader.biPlanes = 1;
		pbi->bmiHeader.biBitCount = 4;
		pbi->bmiHeader.biClrUsed = 16;
		pbi->bmiHeader.biClrImportant = 16;
		for (UINT iPal = 0; iPal < 16; iPal++) {
			COLORREF clr = rPal.p.f.color[iPal];
			RGBQUAD &rgb4 = pbi->bmiColors[iPal];
			rgb4.rgbBlue    = GetBValue(clr);
			rgb4.rgbGreen   = GetGValue(clr);
			rgb4.rgbRed     = GetRValue(clr);
		}
		PVOID pvBits = NULL;
		hbm = CreateDIBSection(
			hDC,
			pbi,
			DIB_RGB_COLORS,
			&pvBits,
			NULL,
			0
			);
		if (hbm != NULL) {
			pbBits = (PBYTE)pvBits;
			return true;
		}
		return false;
	}
	// 
	bool CreateDM2BM8BPP(HDC hDC, UINT cx, UINT cy, BYTE *&pbBits, const DMGHLciDM2Pal &DM2Pal)
	{
		CHAR cbi[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];
		ZeroMemory(&cbi, sizeof(cbi));
		PBITMAPINFO pbi = (PBITMAPINFO)cbi;
		pbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		pbi->bmiHeader.biWidth = cx;
		pbi->bmiHeader.biHeight = cy;
		pbi->bmiHeader.biPlanes = 1;
		pbi->bmiHeader.biBitCount = 8;
		pbi->bmiHeader.biClrUsed = 256;
		pbi->bmiHeader.biClrImportant = 256;
		for (UINT iPal = 0; iPal < 256; iPal++) {
			RGBQUAD &rgb4 = pbi->bmiColors[iPal];
			rgb4.rgbBlue    = GetBValue(DM2Pal.clrs[iPal]);
			rgb4.rgbGreen   = GetGValue(DM2Pal.clrs[iPal]);
			rgb4.rgbRed     = GetRValue(DM2Pal.clrs[iPal]);
		}
		PVOID pvBits = NULL;
		hbm = CreateDIBSection(
			hDC,
			pbi,
			DIB_RGB_COLORS,
			&pvBits,
			NULL,
			0
			);
		if (hbm != NULL) {
			pbBits = (PBYTE)pvBits;
			return true;
		}
		return false;
	}
};

//////////////////////////////////////////////////////////////////////
// CGDAT

void CGDAT::Close()
{
	m_listRawRef.RemoveAll();
	m_listRaw.clear();
	m_mapTrack.clear();
	m_c6.clear();
	m_listModEnt.clear();
	m_mapIdx2Entry.clear();
}

BOOL CGDAT::SaveTo(LPCTSTR pszFileInto)
{
	return saveTo(pszFileInto);
}

BOOL CGDAT::saveTo(LPCTSTR pszFileInto)
{
	if (!makeSure())
		return FALSE;

	WFio io;
	if (!io.OpenCreate(pszFileInto, ENDIAN_TO_ANTI_ENDIAN(m_fLE)))
		return FALSE;
	//
	UI32 iRawEnt;
	UI32 nRawEnts = m_listRaw.size();
	UI32 nDirEnts = m_c6.size();
	UI32 cbE0 = 0x14 + 8 * m_c6.size();
	RawDataList::iterator iterRawPos;
	C3Map::iterator iterDirPos;

	if (nRawEnts > 65535 || nDirEnts > 65535)
		return FALSE;
	if (cbE0 > 1024 * 1024)
		return FALSE;

	io.WriteWord(0x8005);
	io.WriteWord(nRawEnts);
	io.WriteDWord(cbE0);

	for (iRawEnt = 1, iterRawPos = m_listRaw.begin(), iterRawPos++; iRawEnt < nRawEnts; iRawEnt++, iterRawPos++) {
		RawData &rr = *iterRawPos;
		UI32 cbRaw = rr.fv.GetSize();
		if (cbRaw > 65535)
			return FALSE;
		io.WriteWord(cbRaw);
	}

	for (iRawEnt = 0, iterRawPos = m_listRaw.begin(); iRawEnt < nRawEnts; iRawEnt++, iterRawPos++) {
		RawData &rr = *iterRawPos;
		BYTE *pb = (BYTE *)rr.fv.GetData();
		UI32 cb = rr.fv.GetSize();
		if (io.Write(pb, cb) != cb)
			return FALSE;
	}

	io.Close();
	//
	return TRUE;
}

BOOL CGDAT::LoadFrom(LPCTSTR pszFileIn, UINT nGDOF)
{
	Close();
	if (loadFrom(pszFileIn, nGDOF)) {
		return TRUE;
	}
	Close();
	return FALSE;
}

BOOL CGDAT::loadFrom(LPCTSTR pszFileIn, UINT nGDOF)
{
	RFio io;
	if (!io.OpenRead(pszFileIn, ENDIAN_TO_ANTI_ENDIAN(TRUE)))
		return FALSE;
	WORD w;
	if ((w = io.ReadWord()) == 0x8005 || w == 0x8004) {
		m_fLE = true;
	} else {
		if (!io.OpenRead(pszFileIn, ENDIAN_TO_ANTI_ENDIAN(FALSE)))
			return FALSE;
		if ((w = io.ReadWord()) == 0x8005 || w == 0x8004) {
			m_fLE = false;
		} else {
			return FALSE;
		}
	}
	I32 nEntries = io.ReadWord();
	if (nEntries < 1)
		return FALSE;

	I32 cbE0 = io.ReadDWord();
	if (cbE0 < 0x14)
		return FALSE;
	list<UI32> listSize;
	I32 iEntry;
	for (iEntry = 1; iEntry < nEntries; iEntry++) {
		I32 cbEntry = io.ReadWord();
		if (cbEntry < 0)
			return FALSE;
		listSize.push_back(cbEntry);
	}

	m_listRaw.push_back(RawData());

	SizeBuff &fvE0 = m_listRaw.front().fv;
	if (fvE0.Alloc(cbE0) == NULL)
		return FALSE;
	if (io.Read(fvE0.GetData(), cbE0) != cbE0)
		return FALSE;
	for (iEntry = 1; iEntry < nEntries; iEntry++, listSize.pop_front()) {
		I32 cbEntry = listSize.front();
		m_listRaw.push_back(RawData());
		RawData &r = m_listRaw.back();
		// TRACE("#%4d at %10u (%6u)\n", iEntry, ftell(io.f), cbEntry); Sleep(50);
		if (!r.fv.Alloc(cbEntry))
			return FALSE;
		if (io.Read(r.fv.GetData(), cbEntry) != cbEntry)
			return FALSE;
	}
	//
	io.Close();

	m_nGDOF = nGDOF & (~GDFF_DETECTIVE);

	if (nGDOF & GDFF_DETECTIVE) {
		if (!Detective())
			return FALSE;
	}

	if (m_fLE) m_nGDOF |= GDFF_LITTLE_ENDIAN;
	else m_nGDOF &= ~GDFF_LITTLE_ENDIAN;

	SetMode(m_nGDOF);

	VERIFY(makeUnsure());

	return TRUE;
}

CGDAT::RawData *CGDAT::GetRawDataAt(UINT iIdx)
{
	RawDataList::iterator
		iterPos = m_listRaw.begin(),
		iterEnd = m_listRaw.end();
	for (; iterPos != iterEnd; iterPos++, iIdx--) {
		if (iIdx == 0)
			return &*iterPos;
	}
	return NULL;
}

CGDAT::C3 *CGDAT::FindC3Entry(CGDEntry f, BOOL fAdd)
{
	C3Map::iterator
		iterPos = m_c6.find(f),
		iterEnd = m_c6.end();
	if (iterPos != iterEnd)
		return &iterPos->second;
	if (!fAdd)
		return NULL;
	m_c6[f].f = f;
	return &m_c6[f];
}

CGDEntry CGDAT::GetHeadPos()
{
	C3Map::iterator
		iterPos = m_c6.begin(),
		iterEnd = m_c6.end();
	if (iterPos != iterEnd)
		return iterPos->first;
	return C6::ERR;
}

CGDEntry CGDAT::GetNextPos(const CGDEntry &r)
{
	C3Map::iterator
		iterPos = m_c6.find(r),
		iterEnd = m_c6.end();
	if (iterPos != iterEnd)
		iterPos++;
	if (iterPos != iterEnd)
		return iterPos->first;
	return C6::ERR;
}

CGDEntry CGDAT::GetLowerPos(const CGDEntry &r)
{
	C3Map::iterator
		iterPos = m_c6.lower_bound(r),
		iterEnd = m_c6.end();
	if (iterPos != iterEnd)
		return iterPos->first;
	return C6::ERR;
}

I32 CGDAT::GetValueAt(const CGDEntry &r)
{
	C3Map::iterator
		iterPos = m_c6.find(r),
		iterEnd = m_c6.end();
	if (iterPos != iterEnd)
		return iterPos->second.x;
	return -1;
}

I32 CGDAT::GetCls3ValueAt(const CGDEntry &r)
{
	C3Map::iterator
		iterPos = m_c6.lower_bound(r),
		iterEnd = m_c6.end();
	if (iterPos != iterEnd
		&& iterPos->first.c1() == r.c1()
		&& iterPos->first.c2() == r.c2()
		&& iterPos->first.c3() == r.c3()
	) {
		return iterPos->second.x;
	}
	return -1;
}

bool CGDAT::ReadEntry(int i, SizeBuff &rBuff)
{
	RawData *pRaw = GetRawDataAt(i);
	if (pRaw) {
		rBuff = pRaw->fv;
		return true;
	}
	return false;
}

void CGDAT::Track(int iIdx, int nTracked[DMGHLi_MAX_TRACK])
{
	CUIntArray cIdx;
	UINT iTar = iIdx;
	while (true) {
		cIdx.InsertAt(0, iTar);
		TrackChainMap::iterator
			iterPos = m_mapTrack.find(iTar),
			iterEnd = m_mapTrack.end();
		if (iterPos == iterEnd)
			break;
		iTar = iterPos->second;
	}

	UINT i = 0;
	for (; i < DMGHLi_MAX_TRACK && i < cIdx.GetSize(); i++) nTracked[i] = cIdx[i];
	nTracked[i] = 0;
}

HBITMAP CGDAT::GetImgAt(UINT iIdx)
{
	DMGHLiIMGM imgm;
	return GetImgAt(iIdx, imgm);
}

HBITMAP CGDAT::GenImg4(DMGHLiIMGM &imgm)
{
	CCompatDC dc;
	BM bm;
	BYTE *pbBits = NULL;
	UINT x, y, cx = imgm.cx, cy = imgm.cy;
	if (bm.CreateDM2BM4BPP(dc, cx, cy, imgm.cPal, pbBits)) {
		UINT nPitch = R4((cx + 1) / 2);
		BYTE *pbVert = pbBits + nPitch * (cy - 1);
		for (y = 0; y < cy; y++, pbVert -= nPitch) {
			BYTE *pbHorz = pbVert;
			for (x = 0; x < cx; x += 2, pbHorz++) {
				*pbHorz = 0
					| ((imgm.GetPixel(x +1, y) & 0xF) << 0)
					| ((imgm.GetPixel(x +0, y) & 0xF) << 4)
					;
			}
		}
		return bm.Detach();
	}
	return NULL;
}

HBITMAP CGDAT::GenImg8(DMGHLiIMGM &imgm)
{
	CCompatDC dc;
	BM bm;
	BYTE *pbBits = NULL;
	UINT x, y, cx = imgm.cx, cy = imgm.cy;
	const DMGHLciDM2Pal &DM2Pal = GetDM2Pal();
	if (bm.CreateDM2BM8BPP(dc, cx, cy, pbBits, DM2Pal)) {
		UINT nPitch = R4(cx);
		BYTE *pbVert = pbBits + nPitch * (cy - 1);
		for (y = 0; y < cy; y++, pbVert -= nPitch) {
			BYTE *pbHorz = pbVert;
			for (x = 0; x < cx; x++, pbHorz++) {
				*pbHorz = imgm.GetPixel(x, y);
			}
		}
		return bm.Detach();
	}
	return NULL;
}

BOOL CGDAT::GetRawDataAt(UINT iIdx, SizeBuff &rBuff)
{
	RawData *pRaw = GetRawDataAt(iIdx);
	if (pRaw) {
		rBuff = pRaw->fv;
		return TRUE;
	}
	return FALSE;
}

BOOL CGDAT::GetTextAt(UINT iIdx, CString &strText)
{
	RawData *pRaw = GetRawDataAt(iIdx);
	if (pRaw) {
		LPCSTR pszPos = (LPCSTR)pRaw->fv.GetData();
		LPCSTR pszEnd = pszPos + pRaw->fv.GetSize();
		if (GetDM2TextEncoder(m_nDM2TET)->Decode(pszPos, pszEnd, strText)) {
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CGDAT::GetDispTextAt(UINT iIdx, CString &strText)
{
	RawData *pRaw = GetRawDataAt(iIdx);
	if (pRaw) {
		strText.Empty();
		BYTE *pbPos = (BYTE *)pRaw->fv.GetData();
		BYTE *pbEnd = pbPos + pRaw->fv.GetSize();
		if (m_fEN) {
			for (UINT iChr = 0; pbPos < pbEnd; ) {
				BYTE x = (*pbPos);
				if (m_fTextEnc) x = (x + iChr) ^ 0xFF;
				iChr++; pbPos++;

				if (x == 0) break;
				strText += (x == '\n') ? "_" : g_pszTextTblE0[x];
			}
		} else {
			LPCSTR *pszTblJ0 = g_pszTextTblJ0;
			LPCSTR *pszTblJ1 = g_pszTextTblJ1;
			if (m_nDM2TET == GDFF_LANG_JP2_ENC) {
				pszTblJ0 = g_pszTextTblJ0_PSV,
				pszTblJ1 = g_pszTextTblJ1_PSV;
			}

			for (UINT iChr = 0; pbPos < pbEnd; ) {
				BYTE x = (*pbPos);
				if (m_fTextEnc) x = (x + iChr) ^ 0xFF;
				iChr++; pbPos++;

				if (x == 0) break;
				if (x == 0xF0 && pbPos < pbEnd) {
					BYTE x = (*pbPos + iChr) ^ 0xFF; iChr++; pbPos++;
					strText += pszTblJ1[x];
				} else {
					strText += (x == '\n') ? "_" : pszTblJ0[x];
				}
			}
		}
	} else {
		return FALSE;
	}
	return TRUE;
}

HBITMAP CGDAT::GetImgAt(UINT iIdx, DMGHLiIMGM &imgm)
{
	if (ReadImgx(iIdx, imgm)) {
		if (imgm.fIs8bpp)
			return GenImg8(imgm);
		return GenImg4(imgm);
	}
	return NULL;
}

BOOL CGDAT::SetRawDataFor(const CGDEntry &r, const SizeBuff &fv)
{
	C3Map::iterator
		iterPos = m_c6.find(r),
		iterEnd = m_c6.end();
	if (iterPos == iterEnd)
		return false;
	C3 &r3 = iterPos->second;
	if (r3.fIsBin) {
		UINT x = r3.x;
		m_listRawRef[x]--;
		r3.x = -1;
		r3.fIsBin = FALSE;

		UINT nRef = m_listRawRef[x];
		if (nRef == 0) {
			m_mapTrack.erase(x);
		}
	}
	UINT iRaw, nRaw = m_listRaw.size();
	for (iRaw = 0; iRaw < nRaw; iRaw++) {
		if (m_listRawRef[iRaw] == 0) {
			break;
		}
	}
	r3.x = iRaw;
	r3.fIsBin = TRUE;
	AddModifiedEnt(r);
	if (r.c3() == 1 && m_nGDRIPT == GDRIPT_RAW) {
		m_mapIdx2Entry[iRaw] = r;
	}
	ASSERT(m_mapTrack.find(iRaw) == m_mapTrack.end());
	m_mapTrack.erase(iRaw);
	if (iRaw == nRaw) {
		m_listRawRef.Add(1);
		m_listRaw.push_back(RawData());
		m_listRaw.back().fv = fv;
		return true;
	} else {
		m_listRawRef[iRaw]++;
		RawData *pRaw = GetRawDataAt(iRaw);
		pRaw->fv = fv;
		return true;
	}
}

BOOL CGDAT::EnumL4(list<CGDEntry> &rList, BYTE c1, BYTE c2)
{
	CGDEntry lower(c1, c2, 0, 0, 0, 0);
	CGDEntry upper(c1, c2,-1,-1,-1,-1);

	return Enum(rList, lower, upper);
}

BOOL CGDAT::Enum(list<CGDEntry> &rList, const CGDEntry &lower, const CGDEntry &upper)
{
	C3Map::iterator
		iterPos = m_c6.lower_bound(lower),
		iterEnd = m_c6.upper_bound(upper);
	rList.clear();
	for (; iterPos != iterEnd; iterPos++) {
		rList.push_back(iterPos->first);
	}
	return TRUE;
}

BOOL CGDAT::Enum(list<CGDEntry> &rList)
{
	C3Map::iterator
		iterPos = m_c6.begin(),
		iterEnd = m_c6.end();
	rList.clear();
	for (; iterPos != iterEnd; iterPos++) {
		rList.push_back(iterPos->first);
	}
	return TRUE;
}

BOOL CGDAT::AddEntry(const CGDEntry &pos, UINT iIdx, BOOL fIsBin)
{
	return AddEntry(pos, iIdx);
}

BOOL CGDAT::AddEntry(const CGDEntry &pos, UINT iIdx)
{
	C3Map::iterator
		iterPos = m_c6.find(pos),
		iterEnd = m_c6.end();
	if (iterPos != iterEnd)
		return FALSE;
	C3 &r3 = m_c6.insert(C3Map::value_type(pos, C3(pos))).first->second;
	r3.fIsBin = IsEntryBin(pos); //fIsBin;
	r3.x = iIdx;
	if (r3.fIsBin && r3.x < m_listRawRef.GetSize()) {
		m_listRawRef[r3.x]++;
	} else {
		r3.fIsBin = FALSE;
	}
	AddModifiedEnt(pos);
	return TRUE;
}

BOOL CGDAT::DeleteEntry(const CGDEntry &pos)
{
	C3Map::iterator
		iterPos = m_c6.find(pos),
		iterEnd = m_c6.end();
	if (iterPos == iterEnd)
		return FALSE;
	Unassoc(iterPos->second);
	m_c6.erase(iterPos);
	AddModifiedEnt(pos);
	return TRUE;
}

void CGDAT::Unassoc(C3 &c3)
{
	if (c3.f.c3() == 1 && m_nGDRIPT == GDRIPT_RAW) {
		m_mapIdx2Entry.erase(c3.x);
	}
	if (c3.fIsBin) {
		m_listRawRef[c3.x]--;

		c3.fIsBin = FALSE;
	}
}

BOOL CGDAT::AddTextEntry(const CGDEntry &pos, LPCSTR pszText)
{
	SizeBuff fv;
	if (!EncodeText(pszText, fv))
		return FALSE;
	return AddBinEntry(pos, fv);
}

BOOL CGDAT::AddBinEntry(const CGDEntry &pos, const SizeBuff &fv)
{
	C3Map::iterator
		iterPos = m_c6.find(pos),
		iterEnd = m_c6.end();
	if (iterPos != iterEnd)
		return FALSE;
	UINT iIdx = m_listRaw.size();
	C3 &r3 = m_c6.insert(C3Map::value_type(pos, C3(pos))).first->second;
	r3.fIsBin = FALSE;
	r3.x = -1;

	VERIFY(SetRawDataFor(pos, fv));

	if (pos.c3() == 1 && m_nGDRIPT == GDRIPT_RAW) {
		m_mapIdx2Entry[GetValueAt(pos)] = pos;
	}

	AddModifiedEnt(pos);
	return TRUE;
}

BOOL CGDAT::SetTextFor(const CGDEntry &pos, LPCSTR pszText)
{
	SizeBuff fv;
	if (!EncodeText(pszText, fv))
		return FALSE;
	return SetRawDataFor(pos, fv);
}

BOOL CGDAT::EncodeText(LPCSTR pszText, SizeBuff &fv)
{
	if (GetDM2TextEncoder(m_nDM2TET)->Encode(pszText, strchr(pszText, 0), fv)) {
		return TRUE;
	}
	return FALSE;
}

BOOL CGDAT::SetValueAt(const CGDEntry &pos, UI32 nVal, BOOL fIsBin)
{
	C3Map::iterator
		iterPos = m_c6.find(pos),
		iterEnd = m_c6.end();
	if (iterPos == iterEnd)
		return FALSE;
	C3 &r3 = iterPos->second;
	Unassoc(r3);

	r3.fIsBin = fIsBin;
	r3.x = nVal;
	if (r3.fIsBin && r3.x < m_listRawRef.GetSize()) {
		m_listRawRef[r3.x]++;
	} else {
		r3.fIsBin = FALSE;
	}
	AddModifiedEnt(pos);
	return TRUE;
}

void CGDAT::SetModified(bool fModified)
{
	m_fModified = fModified;
}

bool CGDAT::IsModified()
{
	return m_fModified;
}

BOOL CGDAT::RenameEntry(const CGDEntry &rExist, const CGDEntry &rNew)
{
	C3Map::iterator
		iterSrcPos = m_c6.find(rExist),
		iterDstPos = m_c6.find(rNew),
		iterEnd = m_c6.end();
	if (iterSrcPos == iterEnd || iterDstPos != iterEnd)
		return FALSE;
	//FIXME:カウンタの減少と増減を行うように汁!!
	C3 &rSrc = iterSrcPos->second;
	m_c6[rNew] = rSrc;
	m_c6[rNew].f = rNew;
	m_c6.erase(iterSrcPos);
	AddModifiedEnt(rExist);
	AddModifiedEnt(rNew);
	return TRUE;
}

void CGDAT::AddModifiedEnt(const CGDEntry &pos)
{
	m_listModEnt.push_back(pos);

	SetModified();
}

void CGDAT::EnumModifiedEnt(list<CGDEntry> &rList)
{
	ModifiedEntryList::iterator
		iterPos = m_listModEnt.begin(),
		iterEnd = m_listModEnt.end();
	for (; iterPos != iterEnd; iterPos++) {
		rList.remove(*iterPos);
		rList.push_back(*iterPos);
	}
	m_listModEnt.clear();
}

bool CGDAT::GrabNextModifiedEnt(CGDEntry &pos)
{
	if (m_listModEnt.empty())
		return false;
	pos = m_listModEnt.front();
	m_listModEnt.remove(pos);
	return true;
}

BOOL CGDAT::GetRawImageDataAt(UINT iIdx, SizeBuff &rBuff, int &nGDRIDT)
{
	ImgTyp t = DecideType(iIdx);
	switch (t) {
	case typeExtDM1:
	case typeU4:
		{
			nGDRIDT = GDRIDT_C4;
			DMGHLiIMGM imgm;
			if (!ReadImgx(iIdx, imgm) || !MakeExpImgC4(imgm, rBuff))
				return FALSE;
			return TRUE;
		}
	case typeDM2PCC8:
		{
			if (t == typeDM2PCC8 && m_fLE) {
				nGDRIDT = GDRIDT_C8;
				break;
			}
		}
	case typeU8:
		{
			nGDRIDT = GDRIDT_C8;
			DMGHLiIMGM imgm;
			if (!ReadImgx(iIdx, imgm) || !MakeExpImgC8(imgm, rBuff))
				return FALSE;
			return TRUE;
		}
	default:
		return FALSE;
	}
	return GetRawDataAt(iIdx, rBuff);
}

bool CGDAT::IsImageTracked(UINT iIdx)
{
	TrackChainMap::iterator
		iterPos = m_mapTrack.find(iIdx),
		iterEnd = m_mapTrack.end();
	if (iterPos == iterEnd)
		return false;
	return true;
}

bool CGDAT::MakeExpImgC4(DMGHLiIMGM &imgm, SizeBuff &fv)
{
	return CDM2ImageComp(IsLE()).MakeC4Auto(imgm, fv);
}

bool CGDAT::MakeExpImgC8(DMGHLiIMGM &imgm, SizeBuff &fv)
{
	return CDM2ImageComp(IsLE()).MakeC8Auto(imgm, fv);
}

BOOL CGDAT::DeleteEntries(CGDEntryMatch &rPred)
{
	C3Map::iterator
		iterPos = m_c6.begin(),
		iterEnd = m_c6.end();
	while (iterPos != iterEnd) {
		CGDEntry pos = iterPos->first;
		if (rPred(pos)) {
			Unassoc(iterPos->second);
			iterPos = m_c6.erase(iterPos);
			AddModifiedEnt(pos);
		} else {
			iterPos++;
		}
	}
	return TRUE;
}

bool CGDAT::GetSegaOwnPalAt(CGDEntry pos, bool fUseHisOne, OwnPal &rOwnPal)
{
	return false;
}

bool CGDAT::GetOwnPalAt(CGDEntry pos, bool fUseHisOne, OwnPal &rOwnPal)
{
	pos.c3(0x0D);

	if (fUseHisOne) {

	} else {
		pos.c4(0xFE);
		pos.c5(0x00);
		pos.c6(0x00);
	}

	SizeBuff fv;
	if (!GetRawDataAt(GetValueAt(pos), fv) || fv.GetSize() != 16)
		return false;
	memcpy(rOwnPal.iIdxes, fv.GetData(), 16);
	return true;
}

bool CGDAT::FindImagePalette(int iIdx, DMGHLciPAL &rPal)
{
	switch (m_nGDRIPT) {
	case GDRIPT_IMAGE:
		{
			break;
		}
	case GDRIPT_RAW:
		{
			Idx2EntryMap::iterator
				iterPos = m_mapIdx2Entry.find(iIdx),
				iterEnd = m_mapIdx2Entry.end();
			if (iterPos != iterEnd) {
				OwnPal aOwnPal;
				CGDEntry pos = iterPos->second;
				if (false
					|| GetOwnPalAt(pos, true, aOwnPal)
					|| GetOwnPalAt(pos, false, aOwnPal)
				) {
					rPal.UseDM2From16Bytes((const UI8 *)aOwnPal.iIdxes, GetDM2Pal());
					return true;
				}
			}
			return true;
		}
	case GDRIPT_SCENE:
		{
			const DMGHLciDM2Pal &pal = GetDM2Pal();
			for (int i = 0; i < 16; i++) {
				BYTE x = m_bMainScenePal[i];
				rPal.p.f.index[i] = x;
				rPal.p.f.color[i] = pal.clrs[x];
			}
			return true;
		}
	}
	return false;
}

BOOL CGDAT::GetExcTextAt(UINT iIdx, SizeBuff &rBuff, int &nGDRTDT)
{
	CString strText;
	if (!GetTextAt(iIdx, strText))
		return false;
	if (!ConvFromTo(strText, rBuff))
		return false;
	switch (m_nDM2TET) {
	case DM2TET_EN_PLAIN:
	case DM2TET_EN:
		nGDRTDT = GDRTDT_EN;
		break;
	case DM2TET_JA1:
	case DM2TET_JA2:
		nGDRTDT = GDRTDT_JP; break;
	}
	return true;
}

BOOL CGDAT::AddExcTextEntry(const CGDEntry &pos, const SizeBuff &fv, int nGDRTDT)
{
	LPCSTR pszPos = (LPCSTR)fv.GetData(), pszEnd = pszPos + fv.GetSize();
	SizeBuff fvRaw;
	switch (nGDRTDT) {
	case GDRTDT_EN:
		if (!GetDM2TextEncoder(m_fTextEnc ? DM2TET_EN : DM2TET_EN_PLAIN)->Encode(pszPos, pszEnd, fvRaw)) return false;
		break;
	case GDRTDT_JP:
		if (!GetDM2TextEncoder(m_nDM2TET)->Encode(pszPos, pszEnd, fvRaw)) return false;
		break;
	default:
		return false;
	}
	return AddBinEntry(pos, fvRaw);
}

I32 CGDAT::GetSizeOfRawData(UINT iIdx)
{
	RawData *pRaw = GetRawDataAt(iIdx);
	if (pRaw)
		return pRaw->fv.GetSize();
	return -1;
}

bool CGDAT::GetScenePalette(int nScene, int iDarkness, RGBQUAD rq[16])
{
	// DM2 PC
	// DM2 Macintosh
	// DM2 PC-9801
	// - Any Scene Palette exist
	// DM2 PC Beta
	// - Scene Palette acquired as 08-xx-09-00-00-00 (64 bytes) --- Should be ignored
	// DM2 Amiga
	// DM2 Sega
	// - Scene Palette acquired as 08-xx-09-00-04-00 (64 bytes) --- Should be acquired

	CGDEntry pos1(0x08, nScene, 0x09, 0x00, 0x04, 0x00);
	CGDEntry pos2(0x08, nScene, 0x09, 0x00, 0x00, 0x00);
	SizeBuff fv;
	if ((ReadRawEntry(pos1, fv) || ReadRawEntry(pos2, fv)) && fv.GetSize() == 64*6) {
		const UI8 *p = fv.GetData() + 64 * __min(5U, (UINT)iDarkness);
		for (int i = 0; i < 16; i++, p += 4) {
			rq[i].rgbRed  = p[1],
			rq[i].rgbGreen= p[2],
			rq[i].rgbBlue = p[3],
			rq[i].rgbReserved = 0;
		}
		return true;
	}
	return false;
}

bool CGDAT::GetInterfacePalette(RGBQUAD rq[16])
{
	// DM2 PC
	// DM2 Macintosh
	// DM2 PC-9801
	// - It 'll be 01-00-0D-FE-08-FF ? (16 bytes) --- Should be ignored
	// DM2 PC Beta
	// - It 'll be 01-00-0D-FE-00-00 ? (16 bytes) --- Should be ignored
	// DM2 Amiga
	// DM2 Sega
	// - It 'll be 01-00-09-00-04-FF ? (64 bytes) --- Should be acquired

	CGDEntry pos1(0x01, 0x00, 0x09, 0x00, 0x04, 0xFF);
	CGDEntry pos2(0x01, 0x00, 0x0D, 0xFE, 0x00, 0x00);
	SizeBuff fv;
	if ((ReadRawEntry(pos1, fv)) && fv.GetSize() == 64*1) {
		const UI8 *p = fv.GetData();
		for (int i = 0; i < 16; i++, p += 4) {
			rq[i].rgbRed  = p[1],
			rq[i].rgbGreen= p[2],
			rq[i].rgbBlue = p[3],
			rq[i].rgbReserved = 0;
		}
		return true;
	}
	return false;
}

bool CGDAT::GetScenePalette(int nScene, int iDarkness, COLORREF clr[16])
{
	RGBQUAD rq[16];
	if (GetScenePalette(nScene, iDarkness, rq)) {
		for (int i = 0; i < 16; i++)
			ConvFromTo(rq[i], clr[i]);
		return true;
	}
	return false;
}

bool CGDAT::GetInterfacePalette(COLORREF clr[16])
{
	RGBQUAD rq[16];
	if (GetInterfacePalette(rq)) {
		for (int i = 0; i < 16; i++)
			ConvFromTo(rq[i], clr[i]);
		return true;
	}
	return false;
}

BOOL CGDAT::ReadRawEntry(const CGDEntry &pos, SizeBuff &fv)
{
	I32 x = GetValueAt(pos);
	if (x < 0)
		return false;
	return GetRawDataAt(x, fv);
}

BOOL CGDAT::GetImgAt(const CGDEntry &pos, DMGHLiIMGM &imgm, HBITMAP *phbm)
{
	if (phbm) *phbm = NULL;

	I32 x = GetValueAt(pos);
	if (x < 0)
		return false;
	if (ReadImgx(x, imgm)) {
		const CDM2PIPDesc *p = m_pipctx.FindFirstMatch(pos);
		if (!imgm.fIs8bpp && p && m_nGDRIPT == GDRIPT_SCENE) {
			COLORREF clr[16];
			int nScene = p->m_nScene;
			switch (p->m_ts) {
			case piptsScene:
				if (nScene < 0) nScene = pos.c2();
				if (false
					|| GetScenePalette(nScene, 1, clr)
					|| GetScenePalette(nScene, 0, clr)
				) {
					CDM2ImageConv a;
					a.CCS4(clr, imgm.cPal, GetDM2Pal());
				}
				break;
			case piptsColorPal:
				if (ReadRawColorPalette(p->m_pos, 0, clr)) {
					CDM2ImageConv a;
					a.CCS4(clr, imgm.cPal, GetDM2Pal());
				}
				break;
			case piptsIf:
				if (GetInterfacePalette(clr)) {
					CDM2ImageConv a;
					a.CCS4(clr, imgm.cPal, GetDM2Pal());
				}
				break;
			}
		}
		if (phbm) {
			HBITMAP &hbm = *phbm;
			if (imgm.fIs8bpp) {
				hbm = GenImg8(imgm);
			} else {
				hbm = GenImg4(imgm);
			}
		}
		return true;
	}
	return false;
}

BOOL CGDAT::LoadToPIPCtxFrom(LPCSTR pszFileIn, bool fFirst)
{
	if (fFirst) m_pipctx.RemoveAll();

	return m_pipctx.LoadFrom(pszFileIn);
}

bool CGDAT::ReadRawColorPalette(const CGDEntry &pos, int iDarkness, COLORREF clr[16])
{
	RGBQUAD rq[16];
	if (ReadRawColorPalette(pos, iDarkness, rq)) {
		for (int i = 0; i < 16; i++)
			ConvFromTo(rq[i], clr[i]);
		return true;
	}
	return false;
}

bool CGDAT::ReadRawColorPalette(const CGDEntry &pos, int iDarkness, RGBQUAD rq[16])
{
	RawData *p = GetRawDataAt(GetValueAt(pos));
	if (p && 64*(1+iDarkness) <= p->fv.GetSize()) {
		const UI8 *z = p->fv.GetData() +64*iDarkness;
		for (int i = 0; i < 16; i++, z += 4) {
			rq[i].rgbRed  = z[1],
			rq[i].rgbGreen= z[2],
			rq[i].rgbBlue = z[3],
			rq[i].rgbReserved = 0;
		}
		return true;
	}
	return false;
}

HBITMAP CGDAT::GetImgAt(const CGDEntry &pos, DMGHLiIMGM &imgm)
{
	HBITMAP hbm = NULL;
	if (GetImgAt(pos, imgm, &hbm)) {
		return hbm;
	}
	return NULL;
}

HBITMAP CGDAT::GetImgAt(const CGDEntry &pos)
{
	DMGHLiIMGM imgm;
	return GetImgAt(pos, imgm);
}

BOOL CGDAT::MakeBinEntry(const CGDEntry &pos, const SizeBuff &fv)
{
	C3Map::iterator
		iterPos = m_c6.find(pos),
		iterEnd = m_c6.end();
	if (iterPos == iterEnd) {
		UINT iIdx = m_listRaw.size();
		C3 &r3 = m_c6.insert(C3Map::value_type(pos, C3(pos))).first->second;
		r3.fIsBin = FALSE;
		r3.x = -1;
	}

	VERIFY(SetRawDataFor(pos, fv));

	if (pos.c3() == 1 && m_nGDRIPT == GDRIPT_RAW) {
		m_mapIdx2Entry[GetValueAt(pos)] = pos;
	}

	AddModifiedEnt(pos);
	return TRUE;
}

bool CGDAT::GetPicIPF(const CGDEntry &pos, DMGHLiPicIPF &ipf)
{
	I32 iIdx = GetValueAt(pos);
	if (iIdx < 0) return false;

	RawData *pRaw = GetRawDataAt(iIdx);
	if (pRaw == NULL) return false;
	const UI8 *pbPos = pRaw->fv.GetData();
	const UI8 *pbEnd = pbPos + pRaw->fv.GetSize();

	bool fLE = m_fLE;

	ImgTyp it = DecideType(iIdx);

	switch (it) {
	case typeExtDM1:
		{
			if (!ipf.LoadFromC4(pbPos, fLE))
				return false;
			break;
		}
	case typeU4:
		{
			if (!ipf.LoadFromU4(pbPos, fLE))
				return false;
			break;
		}
	case typeDM2PCC8:
		{
			if (!ipf.LoadFromC8(pbPos, fLE))
				return false;
			break;
		}
	case typeU8:
		{
			if (!ipf.LoadFromU8(pbPos, fLE))
				return false;
			break;
		}
	}

	if (ipf.fUse0c) {
		CGDEntry pos0c(pos.c1(), pos.c2(), 0xC, pos.c4(), pos.c5(), 0);

		I32 xy = GetValueAt(pos0c);
		if (xy == -1) {
			pos0c.c5(0);
			xy = GetValueAt(pos0c);
		}
		if (xy != -1) {
			ipf.x = (UI8)(xy >> 8);
			ipf.y = (UI8)(xy >> 0);
		}
	}

	return true;
}

bool CGDAT::SetPicIPF(const CGDEntry &pos, DMGHLiPicIPF ipf, bool fStrict)
{
	I32 iIdx = GetValueAt(pos);
	if (iIdx < 0) return false;

	RawData *pRaw = GetRawDataAt(iIdx);
	if (pRaw == NULL) return false;
	SizeBuff fv  = pRaw->fv;
	UI8 *pbPos = fv.GetData();
	UI8 *pbEnd = pbPos + fv.GetSize();

	bool fLE = m_fLE;

	ImgTyp it = DecideType(iIdx);

	if (!fStrict) {
		switch (it) {
		case typeExtDM1:
			{
				if (true
					&& -30 <= ipf.x && ipf.x <= 30
					&& -30 <= ipf.y && ipf.y <= 30
				) {
					ipf.fUse0c = false;
				} else {
					ipf.fUse0c = true;
				}
				break;
			}
		case typeDM2PCC8:
			{
				ipf.fUse0c = false;
				break;
			}
		case typeU4:
		case typeU8:
			{
				if (ipf.x != 0 || ipf.y != 0) {
					ipf.fUse0c = true;
				} else {
					ipf.fUse0c = false;
				}
			}
		}
	}

	switch (it) {
	case typeExtDM1:
		{
			if (!ipf.SaveToC4(pbPos, fLE))
				return false;
			break;
		}
	case typeU4:
		{
			if (!ipf.SaveToU4(pbPos, fLE))
				return false;
			break;
		}
	case typeDM2PCC8:
		{
			if (!ipf.SaveToC8(pbPos, fLE))
				return false;
			break;
		}
	case typeU8:
		{
			if (!ipf.SaveToU8(pbPos, fLE))
				return false;
			break;
		}
	}

	if (!SetRawDataFor(pos, fv))
		return false;

	CGDEntry pos0c_x(pos.c1(), pos.c2(), 0xC, pos.c4(), pos.c5(), 0);
	CGDEntry pos0c_0(pos.c1(), pos.c2(), 0xC, pos.c4(),        0, 0);

	if (ipf.fUse0c) {
		// Format of 0C entry
		//
		// FEDCBA98 76543210
		// xxxxxxxx yyyyyyyy
		// 
		// y: 8-bit signed y-offset
		// x: 8-bit signed x-offset

		UI16 xy = MAKEWORD(ipf.y, ipf.x);
		if (true
			&& !SetValueAt(pos0c_x, xy, false)
			&& !SetValueAt(pos0c_0, xy, false)
		) {
			AddEntry(pos0c_x, xy, false);
		}
	} else {
		DeleteEntry(pos0c_0);
		DeleteEntry(pos0c_x);
	}
	return true;
}

bool CGDAT::IsImage4bpp(const CGDEntry &pos)
{
	I32 iIdx = GetValueAt(pos);
	if (iIdx < 0) return false;

	ImgTyp it = DecideType(iIdx);

	switch (it) {
	case typeExtDM1:
	case typeExtDM1towns:
		return true;
	case typeU4:
		return true;
	case typeDM2PCC8:
		return false;
	case typeU8:
		return false;
	}
	return false;
}

bool CGDAT::makeSure()
{
	if (m_fChainImageTrack && m_mapTrack.size()) {
		C6 k(0,0,8,0,0,0);
		C3 *p3 = FindC3Entry(k, TRUE);
		if (p3 != NULL && p3->fIsBin) {
			RawData *pRaw = GetRawDataAt(p3->x);
			if (!pRaw || !pRaw->fv.Alloc(4 * m_mapTrack.size()))
				return FALSE;
			UI8 *pEntry = pRaw->fv.GetData();

			TrackChainMap::iterator
				iterPos = m_mapTrack.begin(),
				iterEnd = m_mapTrack.end();
			for (; iterPos != iterEnd; iterPos++, pEntry += 4) {
				UI32 x = iterPos->first;
				UI32 y = iterPos->second;
				if (m_fLE) {
					pEntry[0] = LOBYTE(x);
					pEntry[1] = HIBYTE(x);
					pEntry[2] = LOBYTE(y);
					pEntry[3] = HIBYTE(y);
				} else {
					pEntry[0] = HIBYTE(x);
					pEntry[1] = LOBYTE(x);
					pEntry[2] = HIBYTE(y);
					pEntry[3] = LOBYTE(y);
				}
			}
		}
	}

	UI32 nRawEnts = m_listRaw.size();
	UI32 iDirEnt;
	UI32 nDirEnts = m_c6.size();
	UI32 cbE0 = 0x14 + 8 * m_c6.size();
	RawDataList::iterator iterRawPos;
	C3Map::iterator iterDirPos;

	if (nRawEnts > 65535 || nDirEnts > 65535)
		return false;
	if (cbE0 > 1024 * 1024)
		return false;
	if (m_listRaw.size() < 1)
		return false;

	SizeBuff &fvE0 = m_listRaw.front().fv;

	if (!fvE0.Alloc(cbE0)) return false;

	UI8 *pbE0 = fvE0.GetData();

	BYTE bE0Hdr[0x14] = {
		0x80,0x01,0x00,0x00,0x00,0x07,0x54,0x01,0x49,0x01,0x44,0x01,0x53,0x01,0x46,0x01,
		0x47,0x01,0x50,0x02,
	};
	bE0Hdr[2] = HIBYTE(nDirEnts);
	bE0Hdr[3] = LOBYTE(nDirEnts);

	memcpy(pbE0, bE0Hdr, 0x14); pbE0 += 0x14;

	for (iDirEnt = 0, iterDirPos = m_c6.begin(); iDirEnt < nDirEnts; iDirEnt++, iterDirPos++) {
		CGDEntry pos = iterDirPos->first;
		C3 &c3 = iterDirPos->second;
		ASSERT(pos == c3.f);
		UINT iIdx = c3.x;
		memcpy(pbE0, pos.c, 6); pbE0 += 6;
		pbE0[0] = HIBYTE(iIdx); pbE0++;
		pbE0[0] = LOBYTE(iIdx); pbE0++;
	}

	return true;
}

I32 CGDAT::GetRawDataCount()
{
	return m_listRaw.size();
}

bool CGDAT::MakeSure()
{
	return makeSure();
}

UINT CGDAT::GetGDOF()
{
	return m_nGDOF;
}

bool CGDAT::PutRawDataFor(UINT iIdx, const SizeBuff &fv)
{
	RawData *pRaw = GetRawDataAt(iIdx);
	if (pRaw != NULL) {
		pRaw->fv = fv;
		return true;
	}
	return false;
}

bool CGDAT::AppendRawData(const SizeBuff &fv)
{
	m_listRawRef.Add(0);
	m_listRaw.push_back(RawData());
	m_listRaw.back().fv = fv;
	return true;
}

bool CGDAT::makeUnsure()
{
	MkC3Map();

	I32 nEntries = m_listRaw.size();

	m_mapTrack.clear();

	{
		C6 k(0,0,8,0,0,0);
		C3 *p3 = FindC3Entry(k, FALSE);
		if (p3 != NULL && p3->fIsBin) {
			RawData *pRaw = GetRawDataAt(p3->x);
			BYTE *pbPos = (BYTE *)pRaw->fv.GetData();
			BYTE *pbEnd = pbPos + pRaw->fv.GetSize();
			while (pbPos + 3 < pbEnd) {
				UINT x = m_fLE ? (pbPos[0] + pbPos[1] * 256) : (pbPos[0] * 256 + pbPos[1]);
				UINT y = m_fLE ? (pbPos[2] + pbPos[3] * 256) : (pbPos[2] * 256 + pbPos[3]);
				if (x < nEntries && y < nEntries) {
					m_mapTrack[x] = y;
					m_listRawRef[y]++;
				}
				pbPos += 4;
			}

			m_fChainImageTrack = true;
		} else {
			m_fChainImageTrack = false;
		}
	}

	m_mapIdx2Entry.clear();

	switch (m_nGDRIPT) {
	case GDRIPT_RAW:
		{
			C3Map::iterator
				iterPos = m_c6.begin(),
				iterEnd = m_c6.end();
			for (; iterPos != iterEnd; iterPos++) {
				const CGDEntry &pos = iterPos->first;
				switch (pos.c3()) {
				case 0x01:
					{
						I32 iRaw = GetValueAt(pos);
						if (iRaw < 0)
							break;
						if (m_mapIdx2Entry.find(iRaw) != m_mapIdx2Entry.end())
							break;
						m_mapIdx2Entry[iRaw] = pos;
					}
				}
			}
			break;
		}
	case GDRIPT_SCENE:
		{
			CGDEntry c6(0x01,0x00,0x09,0x00,0x04,0xFF);

			RawData *pRaw = GetRawDataAt(GetValueAt(c6));
			if (pRaw && pRaw->fv.GetSize() == 64*1) {
				RGBQUAD rq[16];
				const UI8 *p = pRaw->fv.GetData();
				for (int i = 0; i < 16; i++, p += 4)
					rq[i].rgbRed  = p[1],
					rq[i].rgbGreen= p[2],
					rq[i].rgbBlue = p[3],
					rq[i].rgbReserved = 0;
				DMGHLciPAL aPal;
				CDM2ImageConv().CCS4(rq, aPal, GetDM2Pal());
				memcpy(m_bMainScenePal, aPal.p.f.index, 16);
			}
			break;
		}
	}

	{
		m_nGDRSLT = 0;

		// 0F-xx: Creatures
		// # 00-00-00: English, PC Beta
		// # 00-F0-00: English, PC/Amiga

		// 10-xx: WEAP
		// 11-xx: CLOT
		// 12-xx: SCRO
		// 13-xx: POTI
		// 14-xx: CHES
		// 15-xx: MISC
		// # 18-00-00: English, PC Beta
		// # 18-10-00: English
		// # 18-20-00: Japanese
		// # 18-30-00: German
		// # 18-40-00: French

		C3Map::iterator
			iterPos = m_c6.begin(),
			iterEnd = m_c6.end();
		for (; iterPos != iterEnd; iterPos++) {
			const CGDEntry &pos = iterPos->first;
			if (pos.c3() == 0x05) {
				UINT nDivision = 0x10000 * pos.c4() + 0x100 * pos.c5() + pos.c6();
				switch (pos.c1()) {
				case 0x0F:
					{
						switch (nDivision) {
						case 0x000000: m_nGDRSLT |= GDRSLT_C_EN_PLAIN; break;
						case 0x00F000: m_nGDRSLT |= GDRSLT_C_EN; break;
						}
						break;
					}
				case 0x10:
				case 0x11:
				case 0x12:
				case 0x13:
				case 0x14:
				case 0x15:
					{
						switch (nDivision) {
						case 0x180000: m_nGDRSLT |= GDRSLT_I_EN_PLAIN; break;
						case 0x181000: m_nGDRSLT |= GDRSLT_I_EN; break;
						case 0x182000: m_nGDRSLT |= GDRSLT_I_JP; break;
						case 0x183000: m_nGDRSLT |= GDRSLT_I_DE; break;
						case 0x184000: m_nGDRSLT |= GDRSLT_I_FR; break;
						}
						break;
					}
				}
			}
		}
	}

	return true;
}

bool CGDAT::MakeUnsure()
{
	bool f = makeUnsure();

	SetModified();
	return f;
}

void CGDAT::SetMode(UINT nGDOF)
{
//	m_fLE = (nGDOF & GDFF_LITTLE_ENDIAN) ? true : false;
	m_fRestrictPal = (nGDOF & GDFF_RESTRICT_PAL) ? true : false;

	switch (nGDOF & GDFF_LANG_MASK) {
	default:
	case GDFF_LANG_EN: m_fEN = true; m_fTextEnc = false; m_nDM2TET = DM2TET_EN_PLAIN; break;
	case GDFF_LANG_EN_ENC: m_fEN = true; m_fTextEnc = true; m_nDM2TET = DM2TET_EN; break;
	case GDFF_LANG_JP1_ENC: m_fEN = false; m_fTextEnc = true; m_nDM2TET = DM2TET_JA1; break;
	case GDFF_LANG_JP2_ENC: m_fEN = false; m_fTextEnc = true; m_nDM2TET = DM2TET_JA2; break;
	}
	switch (nGDOF & GDFF_SND_MASK) {
	default:
	case 0:             m_nGDRSDT = 0; break;
	case GDFF_SND5SND6: m_nGDRSDT = GDRSDT_SND5SND6; break;
	case GDFF_SND3:     m_nGDRSDT = GDRSDT_SND3; break;
	case GDFF_SND7:     m_nGDRSDT = GDRSDT_SND7; break;
	case GDFF_SNDA:     m_nGDRSDT = GDRSDT_SNDA; break;
	case GDFF_SND9:     m_nGDRSDT = GDRSDT_SND9; break;
	}
	switch (nGDOF & GDFF_PAL_MASK) {
	default:
	case GDFF_PAL_IMAGE:    m_nGDRIPT = GDRIPT_IMAGE; break;
	case GDFF_PAL_RAW:      m_nGDRIPT = GDRIPT_RAW; break;
	case GDFF_PAL_SCENE:    m_nGDRIPT = GDRIPT_SCENE; break;
	}
	switch (nGDOF & GDFF_PICFMT_MASK) {
	default:
	case GDFF_PICFMT_C4:		m_nGDRPIC = GDRPIC_C4; break;
	case GDFF_PICFMT_C4C8:		m_nGDRPIC = GDRPIC_C4C8; break;
	case GDFF_PICFMT_SEGA:		m_nGDRPIC = GDRPIC_SEGA; break;
	case GDFF_PICFMT_FMTOWNS:	m_nGDRPIC = GDRPIC_FMTOWNS; break;
	}
}

bool CGDAT::FindNearestEntry(CGDEntry &pos)
{
	pos.c5(0);
	pos.c6(0);
	CGDEntry posOrg = pos;
	pos = GetLowerPos(pos);
	if (true
		&& pos.c1() == posOrg.c1()
		&& pos.c2() == posOrg.c2()
		&& pos.c3() == posOrg.c3()
		&& pos.c4() == posOrg.c4()
	) {
		return true;
	}
	return false;
}

CGDAT::RawData *CGDAT::FindRawDataAtNearestEntry(CGDEntry pos)
{
	if (!FindNearestEntry(pos)) return NULL;

	return GetRawDataAt(GetValueAt(pos));
}

int CGDAT::StrictImageTypeAtNearestEntry(CGDEntry pos)
{
	RawData *p = FindRawDataAtNearestEntry(pos);
	if (p) {
		int it = CGDATFDH(m_fLE).DecideImageFormat(p->fv.GetData(), p->fv.GetSize());
		return it;
	}
	return -1;
}

int CGDAT::StrictEffectSndAtNearestEntry(CGDEntry pos)
{
	RawData *p = FindRawDataAtNearestEntry(pos);
	if (p) {
		int it = CGDATFDH(m_fLE).DecideEffectSound(p->fv.GetData(), p->fv.GetSize());
		return it;
	}
	return -1;
}

void CGDAT::MkC3Map()
{
	m_listRawRef.RemoveAll();

	m_c6.clear();

	if (m_listRaw.empty()) return;

	I32 nEntries = m_listRaw.size();

	m_listRawRef.SetSize(nEntries);
	m_listRawRef[0] = 1;

	SizeBuff &fvE0 = m_listRaw.front().fv;

	UI32 cbE0 = fvE0.GetSize();

	const BYTE *pbE0 = (BYTE *)fvE0.GetData();
	UI32 iDEntry, nDEntries = pbE0[2] * 256U + pbE0[3];
	nDEntries = __min(nDEntries, (cbE0 - 0x14) / 8);

	for (iDEntry = 0; iDEntry < nDEntries; iDEntry++) {
		const BYTE *pbDEntry = pbE0 + 0x14 + 8 * iDEntry;
		UINT iIdx = pbDEntry[6] * 256U + pbDEntry[7];

		C6 k(pbDEntry);
		C3 *p3 = FindC3Entry(k, TRUE);

		switch (k.c[2]) {
		case 0x00: // 対策用
		case 0x01: // Image
		case 0x02: // Sound
		case 0x03: // Raw Music
		case 0x04:
		case 0x05: // Text
		case 0x06: // 対策用
		case 0x07: // ANYRAW
		case 0x08:
		case 0x09:
		case 0x0D: // 対策用
		case 0x0E:
			if (iIdx < nEntries) {
				p3->fIsBin = TRUE;
				m_listRawRef[iIdx]++;
			} else {
				p3->fIsBin = FALSE;
			}
			break;
		default:
			p3->fIsBin = FALSE;
		}
		p3->x = iIdx;

		if (false) {
			CString strText;
			strText.Format("%02X %02X %02X %02X %02X %02X %5u\n"
				, 0U +k.c1()
				, 0U +k.c2()
				, 0U +k.c3()
				, 0U +k.c4()
				, 0U +k.c5()
				, 0U +k.c6()
				, 0U +p3->x
				);
			TRACE0(strText);
		}
	}
}

bool CGDAT::Detective()
{
	MkC3Map();

#if 0
//	CGDEntry poses[10] = {
//		CGDEntry(0x07,0x00,1,0x00,0,0),
//		CGDEntry(0x07,0x00,1,0x06,0,0),
//		CGDEntry(0x07,0x00,1,0x07,0,0),
//		CGDEntry(0x07,0x00,1,0x08,0,0),
//		CGDEntry(0x01,0x02,1,0x00,0,0),
//		CGDEntry(0x01,0x02,1,0x01,0,0),
//		CGDEntry(0x01,0x02,1,0x09,0,0),
//		CGDEntry(0x01,0x05,1,0x01,0,0),
//		CGDEntry(0x01,0x05,1,0x0C,0,0),
//		CGDEntry(0x01,0x05,1,0x0E,0,0),
//	};
//	int i;
//	int it;
//	int nFormats[4] = {0, 0, 0};
//	for (i = 0; i < 10; i++) {
//		CGDEntry pos = poses[i];
//		switch (it = StrictImageTypeAtNearestEntry(pos)) {
//		case CGDATFDH::itIMG3or4: nFormats[0]++; break;
//		case CGDATFDH::itIMG3or4Pal: nFormats[1]++; break;
//		case CGDATFDH::itDM2PCC8: nFormats[2]++; break;
//		}
//		pos.c3(0x0D);
//		nFormats[3] += FindNearestEntry(pos) ? 1 : 0;
//	}
#endif

	int it = StrictImageTypeAtNearestEntry(CGDEntry(0x07,0x00,0x01,0x00,0,0));

	bool fIMG3or4 = (it == CGDATFDH::itIMG3or4);
	bool fIMG3or4Pal = (it == CGDATFDH::itIMG3or4Pal);
	bool fDM2PCC8 = (it == CGDATFDH::itDM2PCC8);
	bool fIMG2FMTowns = (it == CGDATFDH::itIMG2FMTowns);

	bool fAttPal = FindNearestEntry(CGDEntry(0x07,0x00,0x0D,0x00,0,0));

	bool fItPC9801 =  ((fIMG3or4Pal) && !fAttPal);
	bool fItPCAT =    ((fIMG3or4Pal || fDM2PCC8) && !fAttPal);
	bool fItPCBeta =  ((fIMG3or4) && !fDM2PCC8 && fAttPal);
	bool fItMega =    ((fIMG3or4) && !fDM2PCC8 && !fAttPal);
	bool fItAmiga =   ((fIMG3or4Pal) && !fAttPal);
	bool fItMac =     ((fIMG3or4Pal || fDM2PCC8) && !fAttPal);
	bool fItPSV =     ((fIMG3or4Pal) && !fAttPal);
	bool fItFMTowns = ((fIMG2FMTowns) && !fDM2PCC8 && !fAttPal);
	bool fItMacJP =   ((fIMG3or4Pal) && !fAttPal);

	int es = StrictEffectSndAtNearestEntry(CGDEntry(0x03,0x00,0x02,0x88,0,0));
	if (es < 0)
		es = StrictEffectSndAtNearestEntry(CGDEntry(0x03,0x00,0x02,0x00,0,0));

	bool fSND3 = (es == CGDATFDH::esSND3);
	bool fSND5or6 = (es == CGDATFDH::esSND5or6);
	bool fSND7 = (es == CGDATFDH::esSND7);
	bool fSND9 = (es == CGDATFDH::esSND9);
	bool fSNDNo = (es == CGDATFDH::esNo);

	static const CGDEntry chkmarks[] = {
		CGDEntry(0x01,0x00,0x06, 0x00,0,0), //0001         PC9801     ,Mega          ,PS/V
		CGDEntry(0x01,0x00,0x06, 0x0A,0,0), //0002                               ,Mac
		CGDEntry(0x01,0x00,0x07, 0x00,0,0), //0004  PCBeta,PC9801,PCAT,Mega,Amiga,Mac,PS/V
		CGDEntry(0x01,0x00,0x07, 0x01,0,0), //0008  PCBeta,PC9801,PCAT           ,Mac,PS/V
		CGDEntry(0x01,0x00,0x07, 0x02,0,0), //0010  PCBeta,PC9801,PCAT           ,Mac,PS/V
		CGDEntry(0x01,0x00,0x07, 0x0A,0,0), //0020         PC9801     ,Mega          ,PS/V
		CGDEntry(0x01,0x00,0x07, 0xC8,0,0), //0040         PC9801,PCAT     ,Amiga,Mac,PS/V
		CGDEntry(0x01,0x00,0x07, 0xFE,0,0), //0080  PCBeta                           
		CGDEntry(0x01,0x00,0x09, 0x00,0,0), //0100                    ,Mega,Amiga    
		CGDEntry(0x01,0x00,0x09, 0x01,0,0), //0200                    ,Mega,Amiga    
		CGDEntry(0x01,0x00,0x09, 0xFE,0,0), //0400         PC9801,PCAT           ,Mac,PS/V
		CGDEntry(0x00,0x00,0x05, 0x00,0,0), //0800                                    PS/V
	};

	// The version code? (00-00-0B-01-00-00)
	// 
	// PC ver	0x0006
	// Amiga	0x002B
	// Mac		0x0006
	// PCBeta	(NULL)
	// Sega		0x000A
	// PCDemo	0x0006
	// PC9801	0x000B
	// PS/V		0x000B
	// FMTowns	0x000A
	// Mac JP	0x000B

	DWORD nMask = 0;
	{
		bool f;
		CGDEntry pos;
		for (int i = 0; i < 12; i++) f = FindNearestEntry(pos = chkmarks[i]), nMask |= f ? (1 << i) : 0;
	}

	if (m_fLE) {
		if (false) {

		} else if (fSND9 && fItPSV && nMask == 0xC7D) {
			m_nGDOF = GDOF_PS_PSV;
		} else if (fSNDNo && fItPC9801 && nMask == 0x47D) {
			m_nGDOF = GDOF_PS_PC9801;
		} else if (fSND3 && fItPCBeta && nMask == 0x09C) {
			m_nGDOF = GDOF_PS_PCBETA;
		} else if (fSND5or6 && fItPCAT && nMask == 0x45C) {
			m_nGDOF = GDOF_PS_PCAT;
		} else if (fSND9 && fItFMTowns && nMask == 0x365) {
			m_nGDOF = GDOF_PS_FMTOWNS;
		} else {
			return false;
		}
	} else {
		if (false) {

		} else if (fSND5or6 && fItMac && nMask == 0x45E) {
			m_nGDOF = GDOF_PS_MAC;
		} else if (fSND7 && fItMega && nMask == 0x325) {
			m_nGDOF = GDOF_PS_MEGA;
		} else if (fSND5or6 && fItAmiga && nMask == 0x344) {
			m_nGDOF = GDOF_PS_AMIGA;
		} else if (fSND7 && fItMacJP && nMask == 0x47D) {
			m_nGDOF = GDOF_PS_MACJP;
		} else {
			return false;
		}
	}
	return true;
}

void CGDAT::ReplicateSubclass(const CGDEntry &posFrom, const CGDEntry &posTo)
{
	list<CGDEntry> vecent;
	if (Enum(vecent, CGDEntry(posFrom.c1(), posFrom.c2(), 0, 0, 0, 0), CGDEntry(posFrom.c1(), posFrom.c2(), 0xFF, 0xFF, 0xFF, 0xFF))) {
		list<CGDEntry>::iterator iter = vecent.begin();
		while(iter != vecent.end()) {
			AddEntry(
				CGDEntry(posTo.c1(), posTo.c2(), iter->c3(), iter->c4(), iter->c5(), iter->c6()),
				GetValueAt(*iter)
				);
			iter++;
		}
	}
}

bool CGDAT::IsEntryBin(const CGDEntry &pos)
{
	switch (pos.c[2]) {
	case 0x00: // 対策用
	case 0x01: // Image
	case 0x02: // Sound
	case 0x03: // Raw Music
	case 0x04:
	case 0x05: // Text
	case 0x06: // 対策用
	case 0x07: // ANYRAW
	case 0x08:
	case 0x09:
	case 0x0D: // 対策用
	case 0x0E:
		return true;
	default:
		return false;
	}
}

UINT CGDAT::CountEnts(const CGDEntry &lower, const CGDEntry &upper)
{
	C3Map::iterator
		iterPos = m_c6.lower_bound(lower),
		iterEnd = m_c6.upper_bound(upper);
	UINT cnt = 0;
	for (; iterPos != iterEnd; iterPos++) {
		cnt++;
	}
	return cnt;
}

bool CGDAT::Mark4x4ToImage(const CGDEntry &pos) {
	DMGHLiIMGM imgm;
	if (GetImgAt(pos, imgm, NULL)) {
		int F=8;
		for (int bx=0;bx<4;bx++)
			for (int by=0;by<4;by++)
				for (int y=0;y<F;y++)
					for (int x=0;x<F;x++)
						imgm.SetPixel(__max(0, (int)(imgm.cx -1 -bx*F -x)), __max(0, (int)(imgm.cy -1 -by*F -y)), 15- bx -4*by);
		SizeBuff fv;
		if (MakeExpImgC4(imgm, fv)) {
			if (SetRawDataFor(pos, fv)) {
				return true;
			}
		}
	}
	return false;
}

void CGDAT::DeleteSubclass(BYTE c1, BYTE c2) {
	list<CGDEntry> vecent;
	if (Enum(vecent, CGDEntry(c1, c2, 0, 0, 0, 0), CGDEntry(c1, c2, 0xFF, 0xFF, 0xFF, 0xFF))) {
		list<CGDEntry>::iterator iter = vecent.begin();
		while (iter != vecent.end()) {
			DeleteEntry(*iter);
			iter++;
		}
	}
}

bool CGDAT::ExportRange(LPCTSTR pszFileInto, const CGDEntry &lower, const CGDEntry &upper) {
	RawDataList listRaw;
	UI32 cbE0;
	UI32 nDirEnts;
	{
		C3Map c6;
		C3Map::iterator
			iterPos = m_c6.lower_bound(lower),
			iterEnd = m_c6.upper_bound(upper);
		while (iterPos != iterEnd) {
			c6.insert(*iterPos);
			iterPos++;
		}

		SizeBuff &fvE0 = listRaw.insert(listRaw.begin(), RawData())->fv;

		cbE0 = 0x14 + 8 * c6.size();
		nDirEnts = c6.size();

		if (!fvE0.Alloc(cbE0)) return false;

		UI8 *pbE0 = fvE0.GetData();

		BYTE bE0Hdr[0x14] = {
			0x80,0x01,0x00,0x00,0x00,0x07,0x54,0x01,0x49,0x01,0x44,0x01,0x53,0x01,0x46,0x01,
			0x47,0x01,0x50,0x02,
		};
		bE0Hdr[2] = HIBYTE(nDirEnts);
		bE0Hdr[3] = LOBYTE(nDirEnts);

		memcpy(pbE0, bE0Hdr, 0x14); pbE0 += 0x14;

		C3Map::iterator iterDirPos;
		UI32 iDirEnt;
		for (iDirEnt = 0, iterDirPos = c6.begin(); iDirEnt < nDirEnts; iDirEnt++, iterDirPos++) {
			CGDEntry pos = iterDirPos->first;
			C3 &c3 = iterDirPos->second;
			ASSERT(pos == c3.f);
			UINT iIdx = c3.x;
			if (c3.fIsBin) {
				RawData raw;
				raw.fv = GetRawDataAt(iIdx)->fv;
				listRaw.push_back(raw);
				iIdx = listRaw.size() -1;
			}
			memcpy(pbE0, pos.c, 6); pbE0 += 6;
			pbE0[0] = HIBYTE(iIdx); pbE0++;
			pbE0[0] = LOBYTE(iIdx); pbE0++;
		}
	}

	WFio io;
	if (!io.OpenCreate(pszFileInto, ENDIAN_TO_ANTI_ENDIAN(m_fLE)))
		return FALSE;
	//
	UI32 iRawEnt;
	UI32 nRawEnts = listRaw.size();
	RawDataList::iterator iterRawPos;
	C3Map::iterator iterDirPos;

	if (nRawEnts > 65535 || nDirEnts > 65535)
		return FALSE;
	if (cbE0 > 1024 * 1024)
		return FALSE;

	io.WriteWord(0x8005);
	io.WriteWord(nRawEnts);
	io.WriteDWord(cbE0);

	for (iRawEnt = 1, iterRawPos = listRaw.begin(), iterRawPos++; iRawEnt < nRawEnts; iRawEnt++, iterRawPos++) {
		RawData &rr = *iterRawPos;
		UI32 cbRaw = rr.fv.GetSize();
		if (cbRaw > 65535)
			return FALSE;
		io.WriteWord(cbRaw);
	}

	for (iRawEnt = 0, iterRawPos = listRaw.begin(); iRawEnt < nRawEnts; iRawEnt++, iterRawPos++) {
		RawData &rr = *iterRawPos;
		BYTE *pb = (BYTE *)rr.fv.GetData();
		UI32 cb = rr.fv.GetSize();
		if (io.Write(pb, cb) != cb)
			return FALSE;
	}

	io.Close();
	//
	return TRUE;
}

bool CGDAT::Copyfrm(CGDAT &cores, const CGDEntry &lower, const CGDEntry &upper, bool copyAtts) {
	if (copyAtts) {
		m_nGDRPIC = cores.m_nGDRPIC;
		m_fLE = cores.m_fLE;
		m_fEN = cores.m_fEN;
		m_fTextEnc = cores.m_fTextEnc;
		m_nDM2TET = cores.m_nDM2TET;
		m_nGDRSLT = cores.m_nGDRSLT;
		m_nGDRSDT = cores.m_nGDRSDT;
		m_nGDRIPT = cores.m_nGDRIPT;
		m_nGDOF = cores.m_nGDOF;
		m_fRestrictPal = cores.m_fRestrictPal;
	}

	if (m_listRaw.empty()) {
		m_listRaw.push_back(RawData());
		m_listRawRef.Add(1);
	}

	C3Map::iterator
		iterPos = cores.m_c6.lower_bound(lower),
		iterEnd = cores.m_c6.upper_bound(upper);
	while (iterPos != iterEnd) {
		if (iterPos->second.fIsBin) {
			RawData *p = cores.GetRawDataAt(iterPos->second.x);
			if (p != NULL) {
				AddBinEntry(iterPos->first, p->fv);
			}
		}
		else {
			AddEntry(iterPos->first, iterPos->second.x, false);
		}
		iterPos++;
	}

	return true;
}

const DMGHLciDM2Pal &CGDAT::GetDM2Pal() {
	RawData *prd = FindRawDataAtNearestEntry(CGDEntry(0x01, 0x00, 0x09, 0xFE, 0x08, 0x00));
	if (prd == NULL)
		prd = FindRawDataAtNearestEntry(CGDEntry(0x01, 0x00, 0x07, 0xFE, 0x08, 0x00));
	if (prd == NULL) {
		for (int x=0; x<256; x++)
			m_DM2Pal.clrs[x] = g_DM2Pal[x];
	}
	else {
		const SizeBuff &fv = prd->fv;
		int cx = fv.GetSize() / 4;
		const BYTE *p = fv.GetData();
		for (int x=0; x<cx; x++) {
			BYTE i = p[4*x];
			m_DM2Pal.clrs[i] = RGB(
				p[4*x +1],
				p[4*x +2],
				p[4*x +3]
				);
		}
	}
	return m_DM2Pal;
}
