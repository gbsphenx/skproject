
#ifndef _DMGHLI_H_
#define _DMGHLI_H_

// @ 1:52 2003/08/09

#include "DMGHLc.h"
#include "DMGHLt.h"
#include "SizeVuff.h"
#include <crtdbg.h>

#define DMGHLi_MAX_ENTRIES (6144U)
#define DMGHLi_MAX_WIDTH 320
#define DMGHLi_MAX_HEIGHT 200
#define DMGHLi_MAX_TRACK 16

extern const COLORREF g_DM2Pal[256];
extern const RGBQUAD g_DM2PalRGBq[256];

struct DMGHLciDM2Pal
{
	// 
	COLORREF clrs[256];
};

struct DMGHLciPAL
{
	// 
	union {
		// 
		struct {
			// 
			COLORREF color[256];
		} e;
		// 
		struct {
			// 
			COLORREF color[16];
			// 
			BYTE index[16];
		} f;
	} p;

	// 
	void UseDM2From16Bytes(const UI8 *p16, const DMGHLciDM2Pal &pal);
	// 
	void UseDM2Native(const DMGHLciDM2Pal &pal);
};

struct DMGHLiIPF
{
	// 4-bit compressed
	// 
	// struct {
	//     WORD cx; // Include 0x8000 to mean "Use 0c instead"
	//     WORD cy;
	// 
	//     BYTE body[...];
	//     ...
	// };
	// 
	//     fedc ba98 7654 3210
	// cx: XXXX XX-- ---- ---- >>> Signed XXXXX (-31 to 31)
	// cy: YYYY YY-- ---- ---- >>> Signed YYYYY (-31 to 31)
	// 
	// If XXXXX = 10000 (-32), then "Use 0c instead"

	// 8-bit compressed
	// 
	// struct {
	//     WORD cx; // Include 0x8000 to mean "Use 0c instead"
	//     WORD cy; // Always include 0x7C00
	//     BYTE fx, fy;
	//     BYTE type;
	//     BYTE zero;
	// 
	//     BYTE body[...];
	// };
	// 
	//     7654 3210
	// fx: XXXX XXXX >>> Signed XXXXXXX (-128 to 127)
	// fy: YYYY YYYY >>> Signed YYYYYYY (-128 to 127)

	// 4-bit plain
	// 
	// struct {
	//     WORD cx; // Include 0x8000 to mean "Use 0c instead"
	//     WORD cy; // Always include 0x8000 (Meaning "plain format")
	//     BYTE bpp; // Always be 4
	//     BYTE zero;
	//     WORD copy_of_cx; // =cx
	//     WORD copy_of_cy; // =cy
	// 
	//     BYTE body[...];
	// };

	// 8-bit plain
	// 
	// struct {
	//     WORD cx; // Include 0x8000 to mean "Use 0c instead"
	//     WORD cy; // Always include 0x8000 (Meaning "plain format")
	//     BYTE bpp; // Always 8
	//     BYTE zero;
	//     WORD copy_of_cx; // =cx
	//     WORD copy_of_cy; // =cy
	// 
	//     BYTE body[...];
	// };

	// 
	I8 x;
	// 
	I8 y;

	// 
	DMGHLiIPF(): x(0), y(0) { }
	// 
	void Init()
	{
		x = y = 0;
	}
};

struct DMGHLiEntryIPF : DMGHLiIPF { };

struct DMGHLiPicIPF
{
	// 
	bool fUse0c;
	// 
	I8 x;
	// 
	I8 y;

	// 
	DMGHLiPicIPF(): x(0), y(0), fUse0c(false) { }
	// 
	void Clear() { x = y = 0; }
	// 
	bool LoadFromC4(const UI8 cHdr[], bool fLE);
	bool LoadFromC8(const UI8 cHdr[], bool fLE);
	bool LoadFromU4(const UI8 cHdr[], bool fLE);
	bool LoadFromU8(const UI8 cHdr[], bool fLE);
	bool SaveToC4(UI8 cHdr[], bool fLE) const;
	bool SaveToC8(UI8 cHdr[], bool fLE) const;
	bool SaveToU4(UI8 cHdr[], bool fLE) const;
	bool SaveToU8(UI8 cHdr[], bool fLE) const;

protected:
	// 
	I8 Extend6To8(UI8 fx);
};

struct DMGHLiIMGM
{
	// for 4-bit Image
	// 0000 PPPP
	// ||||
	// |||+-- 0x10: Transparent
	// ||+--- 0x20: -
	// |+---- 0x40: -
	// +----- 0x80: -
	// for 8-bit Image: Direct to DM2 palette index
	UI8 cPix[DMGHLi_MAX_HEIGHT][DMGHLi_MAX_WIDTH];
	// 
	UI32 cx, cy;
	// 
	DMGHLiPicIPF ipf;
	// 
	DMGHLciPAL cPal;
	// 
	bool fIs8bpp;

	// 
	DMGHLiIMGM()
	{
		cx = cy = 0;
	}

	// 
	inline UI8 GetPixel(int x, int y) const
	{
		return cPix[y][x];
	}
	// 
	inline void SetPixel(int x, int y, UI8 c)
	{
		cPix[y][x] = c;
	}
};

struct DMGHLiIMGMPixelWriter
{	// 
	DMGHLiIMGM &imgm;
	// 
	int cx, cy, x, y;

	// 
	DMGHLiIMGMPixelWriter(DMGHLiIMGM &imgm): imgm(imgm), cx(imgm.cx), cy(imgm.cy), x(0), y(0)
	{

	}
	// 
	bool WritePixels(int n, UI8 p)
	{
		while (n > 0 && y < cy)
		{
			imgm.SetPixel(x, y, p);
			x++;
			if (x >= cx)
			{
				x -= cx; y++;
			}
			n--;
		}
		return (n == 0);
	}
	// 
	bool StillPixels(int n)
	{
		while (n > 0 && y < cy)
		{
			x++;
			if (x >= cx)
			{
				x -= cx; y++;
			}
			n--;
		}
		return (n == 0);
	}
	// 
	bool SpillPixels(int n)
	{
		while (n > 0 && y < cy && 0 < y)
		{
			imgm.SetPixel(x, y, imgm.GetPixel(x, y - 1));
			x++;
			if (x >= cx)
			{
				x -= cx; y++;
			}
			n--;
		}
		return (n == 0);
	}
	// 
	bool ProjectPixels(int nBack, int nLength)
	{
		int iPixel = y * cx + x;
		if (iPixel < nBack) return false;
		int nPixels = cx * cy;
		if (nPixels < iPixel + nLength) return false;

		iPixel -= nBack;

		int tx = iPixel % cx;
		int ty = iPixel / cx;

		while (nLength > 0 && y < cy)
		{
			imgm.SetPixel(x, y, imgm.GetPixel(tx, ty));

			x++;
			if (x >= cx)
			{
				x -= cx; y++;
			}

			tx++;
			if (tx >= cx)
			{
				tx -= cx; ty++;
			}

			nLength--;
		}
		return (nLength == 0);
	}
	// 
	bool IsEndOfImage()
	{
		return x == 0 && y == cy;
	}

};

class DMGHLi8Bits
{
	// 
	const UI8 *pbPos;
	// 
	const UI8 *pbEnd;
	// 
	const UI8 *pbBegin;

public:
	// 
	DMGHLi8Bits(const void *pv, int cb)
	{
		pbEnd = (pbBegin = pbPos = (const UI8 *)pv) + cb;
	}
	// 
	int ReadByte()
	{
		int r = -1;
		if (pbPos < pbEnd) {
			r = *pbPos;
			pbPos++;
		}
		return r;
	}
	// 
	int GetLength()
	{
		return pbEnd - pbBegin;
	}
	// 
	int GetPosition()
	{
		return pbPos - pbBegin;
	}
};

class DMGHLiL2R4Bits
{
	// 
	const UI8 *pbPos;
	// 
	const UI8 *pbEnd;
	// 
	const UI8 *pbBegin;
	// 
	int nValue;
	// 
	int nBits;

	// 
	int ReadByte()
	{
		int r = -1;
		if (pbPos < pbEnd) {
			r = *pbPos;
			pbPos++;
		}
		return r;
	}

public:
	// 
	DMGHLiL2R4Bits(const void *pv, int cb)
	{
		pbEnd = (pbBegin = pbPos = (const UI8 *)pv) + cb;
		nBits = 0;
	}
	// 
	int Read4Bits()
	{
		int r = -1;
		if (nBits == 0)
		{
			if ((nValue = ReadByte()) < 0) return -1;
			nBits = 4;
			r = (nValue >> 4) & 0xF;
		}
		else
		{
			nBits = 0;
			r = (nValue >> 0) & 0xF;
		}
		return r;
	}
	// 
	int GetLength()
	{
		return pbEnd - pbBegin;
	}
	// 
	int GetPosition()
	{
		return pbPos - pbBegin;
	}
};

class DMGHLciF
{
public:
	// 
	virtual bool ReadEntry(int i, SizeBuff &rBuff) = NULL;
	// 
	virtual bool IsLE() = NULL;
	// 
	virtual void Track(int iIdx, int nTracked[DMGHLi_MAX_TRACK]) = NULL;
	// 
	virtual bool IsPalIndependent() = NULL;
	// 
	virtual bool FindImagePalette(int iIdx, DMGHLciPAL &rPal) = NULL;
	// 
	virtual bool IsFMTowns() = NULL;
	// 
	virtual bool IsSega() = NULL;
	// 
	virtual bool IsC8U8Allowed() = NULL;
	// 
	virtual const DMGHLciDM2Pal &GetDM2Pal() = NULL;

};

class DMGHLciFDM2 : public DMGHLciF
{
	// 
	bool ReadImgU8(int i, DMGHLiIMGM &imgm)
	{
		SizeBuff cSrc;
		if (!ReadEntry(i, cSrc)) return false;

		bool fLE = IsLE();

		int cbSrc = cSrc.GetSize();
		if (cbSrc < 10) return false;
		UI8 *pbPos = cSrc.GetData();
		UI8 *pbEnd = pbPos + cbSrc;
		int rcx = fLE ? (pbPos[0] | pbPos[1] * 256) : (pbPos[0] * 256 | pbPos[1]);
		int rcy = fLE ? (pbPos[2] | pbPos[3] * 256) : (pbPos[2] * 256 | pbPos[3]);
		int cx = (rcx) & 0x03FF;
		int cy = (rcy) & 0x03FF;
		if (cx > DMGHLi_MAX_WIDTH || cx < 1 || cy > DMGHLi_MAX_HEIGHT || cy < 1) return false;
		if (cbSrc < 10 + cx * cy) return false;
		imgm.cx = cx;
		imgm.cy = cy;
		imgm.ipf.x = 0; // rcx & 0xFC00;
		imgm.ipf.y = 0; // rcy & 0x7C00;
		imgm.cPal.UseDM2Native(GetDM2Pal());
		imgm.fIs8bpp = true;
		DMGHLiIMGMPixelWriter pw(imgm);
		DMGHLi8Bits thruIn(pbPos +10, pbEnd -pbPos -10);

		for (int y=0; y<cy; y++) {
			for (int x=0; x<cx; x++) {
				pw.WritePixels(1, thruIn.ReadByte());
			}
		}

		return true;
	}
	// 
	bool ReadImgU4(int i, DMGHLiIMGM &imgm)
	{
		SizeBuff cSrc;
		if (!ReadEntry(i, cSrc)) return false;

		bool fLE = IsLE();

		int cbSrc = cSrc.GetSize();
		if (cbSrc < 10+16) return false;
		UI8 *pbPos = cSrc.GetData();
		UI8 *pbEnd = pbPos + cbSrc;
		int rcx = fLE ? (pbPos[0] | pbPos[1] * 256) : (pbPos[0] * 256 | pbPos[1]);
		int rcy = fLE ? (pbPos[2] | pbPos[3] * 256) : (pbPos[2] * 256 | pbPos[3]);
		int cx = (rcx) & 0x03FF;
		int cy = (rcy) & 0x03FF;
		if (cx > DMGHLi_MAX_WIDTH || cx < 1 || cy > DMGHLi_MAX_HEIGHT || cy < 1) return false;
		if (cbSrc < 10 + (cx+1)/2 * cy + 16) return false;
		imgm.cx = cx;
		imgm.cy = cy;
		imgm.cPal.UseDM2From16Bytes(pbEnd - 16, GetDM2Pal());
		imgm.ipf.x = 0; // rcx & 0xFC00;
		imgm.ipf.y = 0; // rcy & 0xFC00;
		imgm.fIs8bpp = false;
		DMGHLiIMGMPixelWriter pw(imgm);
		DMGHLiL2R4Bits thruIn(cSrc.GetData() +10, cSrc.GetSize() -10 -16);

		for (int y=0; y<cy; y++) {
			int x=0;
			for (; x<cx; x++) {
				pw.WritePixels(1, thruIn.Read4Bits());
			}
			if (x & 1) thruIn.Read4Bits(); // each scanline is byte-aligned
		}

		return true;
	}
	// 
	bool ReadImgU4towns(int i, DMGHLiIMGM &imgm);
	// 
	bool ReadImgDM2PCC8(int i, DMGHLiIMGM &imgm)
	{
		SizeBuff cSrc;
		if (!ReadEntry(i, cSrc)) return false;

		bool fLE = IsLE();

		int cbSrc = cSrc.GetSize();
		if (cbSrc < 8) return false;
		UI8 *pbPos = cSrc.GetData();
		UI8 *pbEnd = pbPos + cbSrc;
		int rcx = fLE ? (pbPos[0] | pbPos[1] * 256) : (pbPos[0] * 256 | pbPos[1]);
		int rcy = fLE ? (pbPos[2] | pbPos[3] * 256) : (pbPos[2] * 256 | pbPos[3]);
		int cx = (rcx) & 0x03FF;
		int cy = (rcy) & 0x03FF;
		if (cx > DMGHLi_MAX_WIDTH || cx < 1 || cy > DMGHLi_MAX_HEIGHT || cy < 1) return false;
		imgm.cx = cx;
		imgm.cy = cy;
		imgm.ipf.LoadFromC8(pbPos, fLE);
		imgm.cPal.UseDM2Native(GetDM2Pal());
		imgm.fIs8bpp = true;
		DMGHLiIMGMPixelWriter pw(imgm);
		DMGHLi8Bits thruIn(pbPos +8, pbEnd -pbPos -8);

		int iPal;
		bool fIsType3 = (pbPos[6] == 3);

		static const int nTblBits[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

		while (!pw.IsEndOfImage()) {
			int nCmdBits = thruIn.ReadByte();
			for (int iCmd = 0; iCmd < 8 && !pw.IsEndOfImage(); iCmd++) {
				if (nCmdBits & nTblBits[iCmd]) {
					if ((iPal = thruIn.ReadByte()) < 0) return false;
					if (!pw.WritePixels(1, iPal)) return false;
				} else {
					int n1 = thruIn.ReadByte();
					int n2 = thruIn.ReadByte();
					if (n1 < 0 || n2 < 0) return false;
					int w = (n1 | n2 * 256);
					int nLen = (fIsType3 ? (w & 31) : (w & 15)) + 3;
					int nBack = fIsType3 ? (w >> 5) : (w >> 4);
					if (!pw.ProjectPixels(nBack, nLen)) return false;
				}
			}
		}
		return thruIn.GetPosition() == thruIn.GetLength();
	}
	// 
	bool ReadImgDM2C4(int i, DMGHLiIMGM &imgm, bool fPrimaryPlane)
	{
		if (IsPalIndependent()) {
			return ReadImgSegaC4(i, imgm, fPrimaryPlane);
		} else {
			return ReadImgExtDM1(i, imgm, fPrimaryPlane);
		}
	}
	// 
	bool ReadImgSegaC4(int i, DMGHLiIMGM &imgm, bool fPrimaryPlane)
	{
		SizeBuff cSrc;
		if (!ReadEntry(i, cSrc)) return false;

		bool fLE = IsLE();

		int cbSrc = cSrc.GetSize();
		if (cbSrc < 7) return false;
		UI8 *pbPos = cSrc.GetData();
		UI8 *pbEnd = pbPos + cbSrc;
		int rcx = fLE ? (pbPos[0] | pbPos[1] * 256) : (pbPos[0] * 256 | pbPos[1]);
		int rcy = fLE ? (pbPos[2] | pbPos[3] * 256) : (pbPos[2] * 256 | pbPos[3]);
		int cx = (rcx) & 0x03FF;
		int cy = (rcy) & 0x03FF;
		if (cx > DMGHLi_MAX_WIDTH || cx < 1 || cy > DMGHLi_MAX_HEIGHT || cy < 1) return false;
		imgm.cx = cx;
		imgm.cy = cy;
		imgm.ipf.LoadFromC4(pbPos, fLE);
		imgm.fIs8bpp = false;
		if (!FindImagePalette(i, imgm.cPal)) return false;
		DMGHLiIMGMPixelWriter pw(imgm);
		DMGHLiL2R4Bits thruIn(cSrc.GetData() +4, cSrc.GetSize() -4);
		int iLocalPal6[6] = {
			thruIn.Read4Bits(),
			thruIn.Read4Bits(),
			thruIn.Read4Bits(),
			thruIn.Read4Bits(),
			thruIn.Read4Bits(),
			fPrimaryPlane ? thruIn.Read4Bits() : 0,
		};

		int iPal;
		int n1, n2;
		UI8 nTransparentState = fPrimaryPlane ? 0x00 : 0x10;

		while (!pw.IsEndOfImage()) {
			if ((n1 = thruIn.Read4Bits()) < 0) return false;

			switch (n1 & 7) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
				iPal = iLocalPal6[n1 & 7];
				break;
			case 5:
				iPal = iLocalPal6[n1 & 7] | nTransparentState;
				break;
			case 6:
				iPal = 0x20;
				break;
			case 7:
				{
				if ((n2 = thruIn.Read4Bits()) < 0) return false;

				iPal = n2;
				break;
				}
			}
			int nDuration = 1;
			if (n1 & 8) {
				nDuration = ReadDuration(thruIn);
			}
			//_RPT2(_CRT_WARN, "%X,%5u\n", iPal&15, nDuration);

			if (iPal & 0x10) {
				if (!pw.StillPixels(nDuration)) return false;
			} else if (iPal & 0x20) {
				if (!pw.SpillPixels(nDuration)) return false;
			} else {
				if (!pw.WritePixels(nDuration, (UI8)iPal)) return false;
			}
		}
		return thruIn.GetPosition() == thruIn.GetLength();
	}
	// 
	bool ReadImgExtDM1(int i, DMGHLiIMGM &imgm, bool fPrimaryPlane)
	{
		SizeBuff cSrc;
		if (!ReadEntry(i, cSrc)) 
			return false;

		bool fLE = IsLE();

		int cbSrc = cSrc.GetSize();
		if (cbSrc < 7+16) 
			return false;
		UI8 *pbPos = cSrc.GetData();
		UI8 *pbEnd = pbPos + cbSrc;
		int rcx = fLE ? (pbPos[0] | pbPos[1] * 256) : (pbPos[0] * 256 | pbPos[1]);
		int rcy = fLE ? (pbPos[2] | pbPos[3] * 256) : (pbPos[2] * 256 | pbPos[3]);
		int cx = (rcx) & 0x03FF;
		int cy = (rcy) & 0x03FF;
		if (cx > DMGHLi_MAX_WIDTH || cx < 1 || cy > DMGHLi_MAX_HEIGHT || cy < 1) 
			return false;
		imgm.cx = cx;
		imgm.cy = cy;
		imgm.cPal.UseDM2From16Bytes(pbEnd - 16, GetDM2Pal());
		imgm.ipf.LoadFromC4(pbPos, fLE);
		imgm.fIs8bpp = false;
		DMGHLiIMGMPixelWriter pw(imgm);
		DMGHLiL2R4Bits thruIn(cSrc.GetData() +4, cSrc.GetSize() -4 -16);
		int iLocalPal6[6];
		iLocalPal6[0] = thruIn.Read4Bits();
		iLocalPal6[1] = thruIn.Read4Bits();
		iLocalPal6[2] = thruIn.Read4Bits();
		iLocalPal6[3] = thruIn.Read4Bits();
		iLocalPal6[4] = thruIn.Read4Bits();
		iLocalPal6[5] = fPrimaryPlane ? thruIn.Read4Bits() : 0;

		int iPal;
		int n1, n2;
		UI8 nTransparentState = fPrimaryPlane ? 0x00 : 0x10;

		while (!pw.IsEndOfImage()) {
			if ((n1 = thruIn.Read4Bits()) < 0) 
				return false;

			switch (n1 & 7) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
				iPal = iLocalPal6[n1 & 7];
				break;
			case 5:
				iPal = iLocalPal6[n1 & 7] | nTransparentState;
				break;
			case 6:
				iPal = 0x20;
				break;
			case 7:
				{
				if ((n2 = thruIn.Read4Bits()) < 0) 
					return false;

				iPal = n2;
				break;
				}
			}
			int nDuration = 1;
			if (n1 & 8) {
				nDuration = ReadDuration(thruIn);
				if (nDuration < 0)
					return false;
			}
			//_RPT2(_CRT_WARN, "%X,%5u\n", iPal&15, nDuration);

			if (iPal & 0x10) {
				if (!pw.StillPixels(nDuration)) 
					return false;
			} else if (iPal & 0x20) {
				if (!pw.SpillPixels(nDuration)) 
					return false;
			} else {
				if (!pw.WritePixels(nDuration, (UI8)iPal)) 
					return false;
			}
		}
		return thruIn.GetPosition() == thruIn.GetLength();
	}
	// 
	bool ReadImgDM2C4towns(int i, DMGHLiIMGM &imgm);
	// 
	static int ReadDuration(DMGHLiL2R4Bits &thruIn)
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
					int v4[4];
					v4[0] = thruIn.Read4Bits();
					v4[1] = thruIn.Read4Bits();
					v4[2] = thruIn.Read4Bits();
					v4[3] = thruIn.Read4Bits();

					if (v4[0] < 0 || v4[1] < 0 || v4[2] < 0 || v4[3] < 0) return -1;

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
				if (v1 < 0) return -1;

				nDuration = 17 + v * 16 + v1;
			}
		} else { // ?
			nDuration = v + 2;
		}
		return nDuration;
	}

protected:
	// 
	enum ImgTyp {
		typeNo,
		typeExtDM1,
		typeExtDM1Sega,
		typeExtDM1towns,
		typeDM2PCC8,
		typeU4,
		typeU4towns,
		typeU8,
	};

	// 
	ImgTyp DecideType(int i)
	{
		bool fLE = IsLE(), fCanC8U8 = IsC8U8Allowed();

		SizeBuff cSrc;
		if (!ReadEntry(i, cSrc)) return typeNo;
		if (cSrc.GetSize() < 4) return typeNo;
		const UI8 *pbPos = cSrc.GetData();
		int cx = fLE ? (pbPos[0] | pbPos[1] * 256) : (pbPos[0] * 256 | pbPos[1]);
		int cy = fLE ? (pbPos[2] | pbPos[3] * 256) : (pbPos[2] * 256 | pbPos[3]);

		if (!fCanC8U8) {
			switch (cy & 0xFE00) {
			case 0x8000:
				if (IsFMTowns())
					return typeU4towns;
				return typeU4;
			default:
				if (IsFMTowns())
					return typeExtDM1towns;
				if (IsSega())
					return typeExtDM1Sega;
				return typeExtDM1;
			}
		} else {
			switch (cy & 0xFE00) {
			case 0x8000:
				return typeU8;
			case 0x7C00:
				return typeDM2PCC8;
			default:
				return typeExtDM1;
			}
		}
		return typeNo;
	}
	// 
	bool ReadImgx(int i, DMGHLiIMGM &imgm)
	{
		ImgTyp it = DecideType(i);

		switch (it) {
		case typeExtDM1:
		{
			int nImgs[DMGHLi_MAX_TRACK];
			Track(i, nImgs);
			int iTrack = 0;
			for (; nImgs[iTrack] != 0; iTrack++) {
				int iImg = nImgs[iTrack];
				if (!ReadImgExtDM1(iImg, imgm, iTrack == 0)) return false;
			}
			return true;
		}
		case typeExtDM1Sega:
		{
			int nImgs[DMGHLi_MAX_TRACK];
			Track(i, nImgs);
			int iTrack = 0;
			for (; nImgs[iTrack] != 0; iTrack++) {
				int iImg = nImgs[iTrack];
				if (!ReadImgSegaC4(iImg, imgm, iTrack == 0)) return false;
			}
			return true;
		}
		case typeExtDM1towns:
		{
			if (!ReadImgDM2C4towns(i, imgm)) return false;
			return true;
		}
		case typeU4:
		{
			if (!ReadImgU4(i, imgm)) return false;
			return true;
		}
		case typeU4towns:
		{
			if (!ReadImgU4towns(i, imgm)) return false;
			return true;
		}
		case typeDM2PCC8:
		{
			if (!ReadImgDM2PCC8(i, imgm)) return false;
			return true;
		}
		case typeU8:
		{
			if (!ReadImgU8(i, imgm)) return false;
			return true;
		}
		}
		return false;
	}
};

#endif // #define _DMGHLI_H_
