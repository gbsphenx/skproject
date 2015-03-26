// DM2ImageComp.h: CDM2ImageComp クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM2IMAGECOMP_H__0006633C_1291_4A41_9633_C6D9A277230C__INCLUDED_)
#define AFX_DM2IMAGECOMP_H__0006633C_1291_4A41_9633_C6D9A277230C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DMGHLci.h"

class CDM2ImageComp
{
	// 
	struct MemDestIo {
		// 
		BYTE *pbBegin, *pbPos, *pbEnd;

		// 
		MemDestIo(BYTE *pbBegin, BYTE *pbEnd): pbBegin(pbBegin), pbPos(pbBegin), pbEnd(pbEnd) { }
		// 
		bool WriteByte(UINT x)
		{
			if (pbPos < pbEnd) {
				*pbPos = x;
				pbPos++;
				return true;
			}
			return false;
		}
		// 
		bool Write(const void *pv, UINT cb)
		{
			if (pbPos + cb <= pbEnd) {
				memcpy(pbPos, pv, cb);
				pbPos += cb;
				return true;
			}
			return false;
		}
		// 
		void Rewind()
		{
			pbPos = pbBegin;
		}
		// 
		DWORD GetPosition() { return pbPos - pbBegin; }
	};
	// 
	class MemDestIoL2R4Bit
	{
		// 
		MemDestIo& io;
		// 
		BYTE nVal;
		// 
		bool fOdd;

	public:
		// 
		MemDestIoL2R4Bit(MemDestIo &io): io(io), fOdd(false) { }
		// 
		bool Write4Bit(BYTE x)
		{
			x = x & 0x0F;
			if (fOdd) {
				nVal = nVal | x;
				fOdd = false;
				if (!io.WriteByte(nVal))
					return false;
			} else {
				nVal = x << 4;
				fOdd = true;
			}
			return true;
		}
		// 
		bool Flush()
		{
			if (fOdd) {
				if (!io.WriteByte(nVal))
					return false;
				fOdd = false;
			}
			return true;
		}

	};
	// 
	class DM2C4Wio : public MemDestIoL2R4Bit
	{
	public:
		// 
		DM2C4Wio(MemDestIo& io): MemDestIoL2R4Bit(io)
		{

		}
		// 
		bool WritePixels(BYTE iPixel, UINT nDistance)
		{
			// 0x00 - 0x05 : Most Used 6
			// 0x06 - 0x15 : Selected 16 Colors
			// 0x16        : Empty Colour
			BYTE nFlag = (nDistance > 1) ? 0x8 : 0x0;
			switch (iPixel) {
			case 0x0:
			case 0x1:
			case 0x2:
			case 0x3:
			case 0x4:
			case 0x5:
				if (!Write4Bit(nFlag | iPixel))
					return false;
				break;
			case 0x06:
			case 0x07:
			case 0x08:
			case 0x09:
			case 0x0A:
			case 0x0B:
			case 0x0C:
			case 0x0D:
			case 0x0E:
			case 0x0F:
			case 0x10:
			case 0x11:
			case 0x12:
			case 0x13:
			case 0x14:
			case 0x15:
				if (!Write4Bit(nFlag | 0x7))
					return false;
				if (!Write4Bit(iPixel - 6))
					return false;
				break;
			case 0x16:
				if (!Write4Bit(nFlag | 0x6))
					return false;
				break;
			}
			if (nFlag == 0x8) {
				if (!WriteDistance(nDistance))
					return false;
			}
			return true;
		}
		// 
		bool WriteDistance(UINT nDistance)
		{
			if (nDistance < 2) {
				ASSERT(FALSE);
				return false;
			} else if (nDistance <= 16) {
				if (!Write4Bit(nDistance - 2))
					return false;
			} else if (nDistance < 272) {
				if (false
					|| !Write4Bit(0xF)
					|| !Write4Bit((nDistance - 17) / 16)
					|| !Write4Bit((nDistance - 17) % 16)
				) {
					return false;
				}
			} else if (nDistance < 65536) {
				if (false
					|| !Write4Bit(0xF)
					|| !Write4Bit(0xF)
					|| !Write4Bit(0xF)
					|| !Write4Bit((nDistance >>12) & 0xF)
					|| !Write4Bit((nDistance >> 8) & 0xF)
					|| !Write4Bit((nDistance >> 4) & 0xF)
					|| !Write4Bit((nDistance >> 0) & 0xF)
				) {
					return false;
				}
			} else {
				ASSERT(FALSE);
				return false;
			}
			return true;
		}
	};
	// 
	class C8CH
	{
	public:
		// 
		bool fIsPixel;
		// 
		BYTE iPixel;
		// 
		UINT nOffset, nLength;

		// 
		C8CH() { }
		// 
		C8CH(BYTE iPixel): fIsPixel(true), iPixel(iPixel)
		{

		}
		// 
		C8CH(UINT nOffset, UINT nLength): fIsPixel(false), nOffset(nOffset), nLength(nLength)
		{

		}
	};

	// 
	static int ReadDuration(DMGHLiL2R4Bits &thruIn);
	// 
	static BYTE M_IF_6B(int x)
	{
		if (x < 0)
			return 0x20 | (x & 0x1F);
		return x & 0x1F;
	}
	// 
	static BYTE M_IF_8B(int x)
	{
		if (x < 0)
			return 0x80 | (x & 0x7F);
		return x & 0x7F;
	}
	// 
	static int D_IF_6B(BYTE x)
	{
		if (x & 0x20)
			return (int)x - 0x40;
		return x;
	}
	// 
	static int D_IF_8B(BYTE x)
	{
		if (x & 0x80)
			return (int)x - 0x100;
		return x;
	}

public:
	// 
	struct DM2BMP4;
	// 
	struct IdxScatter {
		// 
		UINT nScatter[16];

		// for identical complete recompression
		bool MakeFrom(DM2BMP4 &img);
	};
	// 
	struct DM2Pal4Loc16 {
		// from 0 to 255. index to DM2 system palette
		BYTE iColorIdx[16];

		// 
		void MakeFrom(const DMGHLciPAL &rPal);
	};
	// 
	struct DM2Pal4Loc6 {
		// from 0 to 15. index to local 16 colors palette
		BYTE iLocalIdx[6];

		// 
		void MakeFrom(const IdxScatter &rScatter, const DM2Pal4Loc16 &rPal);
	};
	// 
	struct DM2BMP4 {
		// 
		virtual UINT cx() = NULL;
		// 
		virtual UINT cy() = NULL;
		// 
		virtual BYTE GetPixel(UINT x, UINT y) = NULL;
		// 
		virtual DMGHLiPicIPF GetIPF() = NULL;

	};
	// 
	struct DM2BMPImg4 : DM2BMP4 {
		// 
		virtual UINT cx() { return r.cx; }
		// 
		virtual UINT cy() { return r.cy; }
		// 
		virtual BYTE GetPixel(UINT x, UINT y) { return r.GetPixel(x, y); }
		// 
		virtual DMGHLiPicIPF GetIPF() { return r.ipf; }

		// 
		DM2BMPImg4(const DMGHLiIMGM &r): r(r) { }

	protected:
		// 
		const DMGHLiIMGM &r;

	};
	// 
	struct DM2BMP8 {
		// 
		virtual UINT cx() = NULL;
		// 
		virtual UINT cy() = NULL;
		// 
		virtual DMGHLiPicIPF GetIPF() = NULL;
		// 
		virtual BYTE GetPixel(UINT x, UINT y) = NULL;

	};
	// 
	struct DM2BMPImg8 : DM2BMP8 {
		// 
		virtual UINT cx() { return r.cx; }
		// 
		virtual UINT cy() { return r.cy; }
		// 
		virtual BYTE GetPixel(UINT x, UINT y) { return r.GetPixel(x, y); }
		// 
		virtual DMGHLiPicIPF GetIPF() { return r.ipf; }

		// 
		DM2BMPImg8(const DMGHLiIMGM &r): r(r) { }

	protected:
		// 
		const DMGHLiIMGM &r;

	};
	// 
	struct ProgressHint {
		// 
		virtual void Set_Max(int iMax) { }
		// 
		virtual void Set_Pos(int iPos) { }
	};

	// 
	bool m_fLE;

	// 
	CDM2ImageComp(bool fLE): m_fLE(fLE) { }
	// 
	bool MakeC4(DM2BMP4 &rBMP, DM2Pal4Loc16 &rPal16, DM2Pal4Loc6 &rPal6, SizeBuff &fv);
	// 
	bool MakeC4(DM2BMP4 &rBMP, DM2Pal4Loc6 &rPal6, SizeBuff &fv);
	// 
	bool MakeC8(DM2BMP8 &rBMP, SizeBuff &fv);
	// 
	bool MakeC8(DM2BMP8 &rBMP, SizeBuff &fv, ProgressHint &rPH);
	// 
	bool MakeC8As(DM2BMP8 &rBMP, MemDestIo &io, bool fIsType3, ProgressHint &rPH);
	// 
	bool MakeU8(DM2BMP8 &rBMP, SizeBuff &fv);
	// 
	bool MakeU4(DM2BMP4 &rBMP, DM2Pal4Loc16 &rPal16, SizeBuff &fv);
	// 
	bool DecompC4(const SizeBuff &fv, DMGHLiIMGM &img4, const DMGHLciDM2Pal &DM2Pal);
	// 
	bool DecompC8(const SizeBuff &fv, DMGHLiIMGM &img8);
	// 
	bool MakeC4Auto(const DMGHLiIMGM &img4, SizeBuff &fv);
	// 
	bool MakeC8Auto(const DMGHLiIMGM &img8, SizeBuff &fv);

protected:
	// 
	struct PHImpl : ProgressHint {
		// 
		PHImpl(ProgressHint &rPH, int nStages): rPH(rPH), nStages(nStages), iStage(0) { }
		// 
		virtual void Set_Max(int iMax)
		{
			rPH.Set_Max(iMax * nStages);
			rPH.Set_Pos(iBase = iMax * iStage);

			iStage++;
		}
		// 
		virtual void Set_Pos(int iPos)
		{
			rPH.Set_Pos(iBase + iPos);
		}

	private:
		// 
		ProgressHint &rPH;
		// 
		int nStages, iStage, iBase;
	};

	// 
	bool MakeC4(DM2BMP4 &rBMP, DM2Pal4Loc16 *pPal16, DM2Pal4Loc6 &rPal6, SizeBuff &fv);

};

#endif // !defined(AFX_DM2IMAGECOMP_H__0006633C_1291_4A41_9633_C6D9A277230C__INCLUDED_)
