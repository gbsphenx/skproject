// DM1ImageComp.h: CDM1ImageComp クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM1IMAGECOMP_H__ABE27865_B97D_4D7C_8C45_297AB37F4BE6__INCLUDED_)
#define AFX_DM1IMAGECOMP_H__ABE27865_B97D_4D7C_8C45_297AB37F4BE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MemVuff.h"
#include "DM2ImageComp.h"

class CDM1Pic  
{
public:
	// 
	CDM2ImageComp::DM2BMP4 &ref;

	// 
	CDM1Pic(CDM2ImageComp::DM2BMP4 &ref): ref(ref) { }
	// 
	BYTE GetPixel(UINT x, UINT y) const {
		return ref.GetPixel(x, y);
	}
	// 
	BYTE GetPix(UINT x) const {
		return ref.GetPixel(x % ref.cx(), x / ref.cx());
	}
	// 
	UINT cx() const { return ref.cx(); }
	// 
	UINT cy() const { return ref.cy(); }

};

class CDM1ImageComp  
{
	// 
	bool fLE;

public:
	// 
	class MemDestIoL2R4Bit
	{
		// 
		CMemVuff& io;
		// 
		BYTE nVal;
		// 
		bool fOdd;

	public:
		// 
		MemDestIoL2R4Bit(CMemVuff &io): io(io), fOdd(false) { }
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
	class DM1I3I4Wio : public MemDestIoL2R4Bit
	{
	public:
		// 
		DM1I3I4Wio(CMemVuff& io): MemDestIoL2R4Bit(io)
		{

		}
		// 
		bool WritePixels(UINT nDistance, BYTE iPixel)
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
	struct DM1I1I2Wio : public MemDestIoL2R4Bit
	{
		// 
		UINT nStrokeLen;

	public:
		// 
		DM1I1I2Wio(CMemVuff& io): MemDestIoL2R4Bit(io)
		{
			nStrokeLen = 0;
		}
		// 
		bool WritePixels(UINT nDuration, BYTE iPixel)
		{
			if (nDuration == 0) {
				return true;
			} else if (nDuration <= 8) {
				// Write [1 to 8]
				nDuration--;
				if (false
					|| !Write4Bit(nDuration)
					|| !Write4Bit(iPixel)
				) {
					return false;
				}
			} else if (nDuration <= 256) {
				// Write [1 to 256]
				nDuration--;
				if (false
					|| !Write4Bit(8)
					|| !Write4Bit(iPixel)
					|| !Write4Bit((nDuration >> 4) % 16)
					|| !Write4Bit((nDuration >> 0) % 16)
				) {
					return false;
				}
			} else if (nDuration <= 65536) {
				// Write [1 to 65536]
				nDuration--;
				if (false
					|| !Write4Bit(12)
					|| !Write4Bit(iPixel)
					|| !Write4Bit((nDuration >>12) % 16)
					|| !Write4Bit((nDuration >> 8) % 16)
					|| !Write4Bit((nDuration >> 4) % 16)
					|| !Write4Bit((nDuration >> 0) % 16)
				) {
					return false;
				}
			} else {
				return false;
			}
			return true;
		}
		// 
		bool StrokeBegin(UINT nDuration)
		{
			if (256 < nDuration)
				return false;
			nStrokeLen = nDuration;
			return true;
		}
		// 
		bool StrokeNext(BYTE iPixel)
		{
			if (nStrokeLen != 0) {
				if (nStrokeLen & 1) {
					nStrokeLen--;
					if (false
						|| !Write4Bit(9)
						|| !Write4Bit(iPixel)
						|| !Write4Bit((nStrokeLen >> 4) % 16)
						|| !Write4Bit((nStrokeLen >> 0) % 16)
					) {
						return false;
					}
				} else {
					nStrokeLen--;
					if (false
						|| !Write4Bit(9)
						|| !Write4Bit(0)
						|| !Write4Bit((nStrokeLen >> 4) % 16)
						|| !Write4Bit((nStrokeLen >> 0) % 16)
						|| !Write4Bit(iPixel)
					) {
						return false;
					}
				}
				nStrokeLen = 0;
				return true;
			}
			if (!Write4Bit(iPixel))
				return false;
			return true;
		}
		// 
		bool SpillPixels(UINT nDuration, BYTE iAppendPixel)
		{
			if (nDuration == 0) {
				return false;
			} else if (nDuration <= 256) {
				nDuration--;
				if (false
					|| !Write4Bit(11)
					|| !Write4Bit(iAppendPixel)
					|| !Write4Bit((nDuration >> 4) % 16)
					|| !Write4Bit((nDuration >> 0) % 16)
				) {
					return false;
				}
			} else if (nDuration <= 65536) {
				nDuration--;
				if (false
					|| !Write4Bit(15)
					|| !Write4Bit(iAppendPixel)
					|| !Write4Bit((nDuration >>12) % 16)
					|| !Write4Bit((nDuration >> 8) % 16)
					|| !Write4Bit((nDuration >> 4) % 16)
					|| !Write4Bit((nDuration >> 0) % 16)
				) {
					return false;
				}
			} else {
				return false;
			}
			return true;
		}
	};
	// 
	struct IdxScatter {
		// 
		UINT nScatter[16];

		// 
		bool MakeFrom(const CDM1Pic &rPic);
	};
	// 
	struct DM1Pal4Loc6 {
		// 
		BYTE iLocalIdx[6];

		// 
		void MakeFrom(const IdxScatter &rScatter);
	};

	// 
	CDM1ImageComp(bool fLE)
		: fLE(fLE)
	{

	}
	// 
	bool MakeI3(const CDM1Pic &rPic, SizeBuff &sbNew);
	// 
	bool MakeI1(const CDM1Pic &rPic, SizeBuff &sbNew);

};

#endif // !defined(AFX_DM1IMAGECOMP_H__ABE27865_B97D_4D7C_8C45_297AB37F4BE6__INCLUDED_)
