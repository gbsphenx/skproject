// DM2ImageComp.cpp: CDM2ImageComp クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2ImageComp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CDM2ImageComp

//////////////////////////////////////////////////////////////////////
// CDM2ImageComp::IdxScatter

bool CDM2ImageComp::IdxScatter::MakeFrom(DM2BMP4 &img)
{
	UINT cx = img.cx(), cy = img.cy();
	for (UINT i = 0; i < 16; i++)
		nScatter[i] = 0;
	for (UINT y = 0; y < cy; y++) {
		for (UINT x = 0; x < cx; x++) {
			BYTE c = img.GetPixel(x, y);
			if (c < 0x10) {
				nScatter[c]++;
			} else {
				return false;
			}
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// CDM2ImageComp::DM2Pal4Loc16

void CDM2ImageComp::DM2Pal4Loc16::MakeFrom(const DMGHLciPAL &rPal)
{
	memcpy(iColorIdx, rPal.p.f.index, 16);
}

//////////////////////////////////////////////////////////////////////
// CDM2ImageComp::DM2Pal4Loc6

void CDM2ImageComp::DM2Pal4Loc6::MakeFrom(const IdxScatter &rScatter, const DM2Pal4Loc16 &rPal)
{
	typedef multimap<UI32, BYTE, greater<UI32> > Cnt2IdxMMap;
	Cnt2IdxMMap mmap;
	UINT iIdx;
	for (iIdx = 0; iIdx < 16; iIdx++) {
		mmap.insert(Cnt2IdxMMap::value_type(rScatter.nScatter[iIdx], (BYTE)iIdx));
	}

	Cnt2IdxMMap::iterator iterPos;
	for (iIdx = 0, iterPos = mmap.begin(); iIdx < 6; iIdx++, iterPos++) {
		iLocalIdx[iIdx] = iterPos->second;
	}
}

//////////////////////////////////////////////////////////////////////
// CDM2ImageComp

bool CDM2ImageComp::MakeC4(DM2BMP4 &rBMP, DM2Pal4Loc16 *pPal16, DM2Pal4Loc6 &rPal6, SizeBuff &fv)
{
	UINT cx = rBMP.cx(), cy = rBMP.cy();

	if (cx < 1 || cy < 1 || cx > DMGHLi_MAX_WIDTH || cy > DMGHLi_MAX_HEIGHT) return false;

	if (!fv.Alloc(65536)) return false;
	MemDestIo io((BYTE *)fv.GetData(), (BYTE *)fv.GetData() + fv.GetSize());

	BYTE bLocalPixMap[16];
	UINT iIdx;
	for (iIdx = 0; iIdx < 16; iIdx++) {
		UINT iLoc;
		for (iLoc = 0; iLoc < 6; iLoc++) {
			if (rPal6.iLocalIdx[iLoc] == iIdx)
				break;
		}
		if (iLoc == 6) {
			bLocalPixMap[iIdx] = 6 + iIdx;
		} else {
			bLocalPixMap[iIdx] = iLoc;
		}
	}

	BYTE bPrefix[7];
	BYTE bSuffix[16];

	bPrefix[0] = m_fLE ? LOBYTE(cx) : HIBYTE(cx);
	bPrefix[1] = m_fLE ? HIBYTE(cx) : LOBYTE(cx);
	bPrefix[2] = m_fLE ? LOBYTE(cy) : HIBYTE(cy);
	bPrefix[3] = m_fLE ? HIBYTE(cy) : LOBYTE(cy);
	bPrefix[4] = (rPal6.iLocalIdx[0] << 4) | (rPal6.iLocalIdx[1]);
	bPrefix[5] = (rPal6.iLocalIdx[2] << 4) | (rPal6.iLocalIdx[3]);
	bPrefix[6] = (rPal6.iLocalIdx[4] << 4) | (rPal6.iLocalIdx[5]);
	if (!rBMP.GetIPF().SaveToC4((UI8 *)bPrefix, m_fLE)) return false;
	if (!io.Write(bPrefix, 7)) return false;
	//
	DM2C4Wio wio(io);
	UINT iPixel = 0, nPixels = cx * cy;
	while (iPixel < nPixels) {
		BYTE iPix = rBMP.GetPixel(iPixel % cx, iPixel / cx);
		ASSERT(iPix < 16);
		int iIdenticalPixel = iPixel + 1;
		for (; iIdenticalPixel < nPixels && iPix == rBMP.GetPixel(iIdenticalPixel % cx, iIdenticalPixel / cx); iIdenticalPixel++);
		int iEmptyPixel = iPixel;
		if (cx <= iPixel) {
			for (; iEmptyPixel < nPixels && rBMP.GetPixel(iEmptyPixel % cx, iEmptyPixel / cx - 1) == rBMP.GetPixel(iEmptyPixel % cx, iEmptyPixel / cx); iEmptyPixel++);
		}
		if (iIdenticalPixel <= iEmptyPixel) {
			iEmptyPixel -= iPixel;
			if (!wio.WritePixels(0x16, iEmptyPixel))
				return false;
			iPixel += iEmptyPixel;
		} else {
			iIdenticalPixel -= iPixel;
			if (!wio.WritePixels(bLocalPixMap[iPix], iIdenticalPixel))
				return false;
			iPixel += iIdenticalPixel;
		}
	}
	wio.Flush();
	//
	if (pPal16 != NULL) {
		memcpy(bSuffix, pPal16->iColorIdx, 16);
		if (!io.Write(bSuffix, 16))
			return false;
	}
	if (!fv.Alloc(io.GetPosition()))
		return false;

	return true;
}

bool CDM2ImageComp::MakeC8(DM2BMP8 &rBMP, SizeBuff &fv)
{
	return MakeC8(rBMP, fv, ProgressHint());
}

bool CDM2ImageComp::MakeC8(DM2BMP8 &rBMP, SizeBuff &fv, ProgressHint &rPH)
{
	UINT cx = rBMP.cx(), cy = rBMP.cy();

	if (!fv.Alloc(65536)) return false;
	MemDestIo io((BYTE *)fv.GetData(), (BYTE *)fv.GetData() + fv.GetSize());

	PHImpl aPH(rPH, 2);

	if (MakeC8As(rBMP, io, true, aPH)) {
		SizeBuff fvType3;
		UINT cbType3 = io.GetPosition();
		if (!fvType3.Alloc(cbType3, fv.GetData()))
			return false;
		io.Rewind();
		if (MakeC8As(rBMP, io, false, aPH)) {
			UINT cbType2 = io.GetPosition();
			if (cbType2 < cbType3) {
				if (!fv.Alloc(cbType2))
					return false;
				return true;
			}
			if (!fv.Assign(fvType3))
				return false;
			return true;
		}
	}
	return false;
}

bool CDM2ImageComp::MakeC8As(DM2BMP8 &rBMP, MemDestIo &io, bool fIsType3, ProgressHint &rPH)
{
	UINT cx = rBMP.cx();
	UINT cy = rBMP.cy();

	if (cx < 1 || cy < 1 || cx > DMGHLi_MAX_WIDTH || cy > DMGHLi_MAX_HEIGHT) return false;

	rPH.Set_Max(cy);

	BYTE bPrefix[8];

	bPrefix[0] = m_fLE ? LOBYTE(cx) : HIBYTE(cx);
	bPrefix[1] = m_fLE ? HIBYTE(cx) : LOBYTE(cx);
	bPrefix[2] = m_fLE ? LOBYTE(cy) : HIBYTE(cy);
	bPrefix[3] = m_fLE ? HIBYTE(cy) : LOBYTE(cy);
	bPrefix[4] = 0;
	bPrefix[5] = 0;
	bPrefix[6] = fIsType3 ? 3 : 2;
	bPrefix[7] = 0;

	if (!rBMP.GetIPF().SaveToC8((UI8 *)bPrefix, m_fLE)) return false;

	bPrefix[m_fLE ? 3 : 2] |= 0x7C;

	// Type3: XXXX XXXX XXXY YYYY {X: 11-bits (0-2047), Y: 5-bits (0-31)}
	// Type2: XXXX XXXX XXXX YYYY {X: 12-bits (0-4095), Y: 4-bits (0-15)}

	io.Write(bPrefix, 8);
	//
	UINT iPixel = 0, nPixels = cx * cy;
	BYTE iPix;
	C8CH aCmds[8];
	UINT iCmd = 0;
	const UINT nMaxLength = fIsType3 ? (31 + 3) : (15 + 3);
	const UINT nMaxOffset = fIsType3 ? 2047 : 4095;

	while (iPixel < nPixels) {
		rPH.Set_Pos(iPixel / cx);

		UINT iMinLength = 3;
		UINT iMaxLength = __min(nMaxLength, nPixels - iPixel);
		UINT iMinOffset = 1;
		UINT iMaxOffset = __min(nMaxOffset, iPixel);

		UINT iBestOffset = 0;
		UINT iBestLength = 0;

		UINT iChkOffset;
		for (iChkOffset = iMinOffset; iChkOffset <= iMaxOffset; iChkOffset++)
		{
			int iOffset;
			for (iOffset = 0; iOffset < iMaxLength; iOffset++)
			{
				int iOffset1 = iPixel + iOffset;
				int iOffset2 = iPixel + iOffset - iChkOffset;
				if (rBMP.GetPixel(iOffset1 % cx, iOffset1 / cx) != rBMP.GetPixel(iOffset2 % cx, iOffset2 / cx))
					break;
			}
			if (iMinLength <= iOffset && iBestLength < iOffset)
			{
				iBestOffset = iChkOffset;
				iBestLength = iOffset;

				if (iBestLength == nMaxLength) break;
			}
		}
		if (iBestLength == 0) {
			iPix = rBMP.GetPixel(iPixel % cx, iPixel / cx);

			aCmds[iCmd] = C8CH(iPix);
			iCmd++;
			iPixel++;
		} else {
			aCmds[iCmd] = C8CH(iBestOffset, iBestLength);
			iCmd++;
			iPixel += iBestLength;
		}

		if (iCmd == 8 || iPixel == nPixels)
		{
			int nCmds = iCmd;
			BYTE bCmdCtrl = 0;
			static BYTE bTbl8[8] = {1,2,4,8,16,32,64,128};
			for (iCmd = 0; iCmd < nCmds; iCmd++)
				bCmdCtrl |= aCmds[iCmd].fIsPixel ? bTbl8[iCmd] : 0;
			if (!io.WriteByte(bCmdCtrl))
				return false;
			for (iCmd = 0; iCmd < nCmds; iCmd++)
			{
				if (aCmds[iCmd].fIsPixel)
				{
					if (!io.WriteByte(aCmds[iCmd].iPixel))
						return false;
				}
				else
				{
					BYTE v1, v2;
					if (fIsType3)
					{
						v1 = ((aCmds[iCmd].nOffset & 0x0007) << 5)|(aCmds[iCmd].nLength - 3);
						v2 = ((aCmds[iCmd].nOffset & 0xFFF8) >> 3);
					}
					else
					{
						v1 = ((aCmds[iCmd].nOffset & 0x000F) << 4)|(aCmds[iCmd].nLength - 3);
						v2 = ((aCmds[iCmd].nOffset & 0xFFF0) >> 4);
					}
					if (false
						|| !io.WriteByte(v1)
						|| !io.WriteByte(v2)
					) {
						return false;
					}
				}
			}
			iCmd = 0;
		}
	}
	//
	return true;
}

bool CDM2ImageComp::MakeU8(DM2BMP8 &rBMP, SizeBuff &fv)
{
	UINT cx = rBMP.cx(), cy = rBMP.cy();

	if (cx < 1 || cy < 1 || cx > DMGHLi_MAX_WIDTH || cy > DMGHLi_MAX_HEIGHT) return false;

	if (!fv.Alloc(65536)) return false;
	MemDestIo io((BYTE *)fv.GetData(), (BYTE *)fv.GetData() + fv.GetSize());

	BYTE bPrefix[10];

	bPrefix[0] = m_fLE ? LOBYTE(cx) : HIBYTE(cx);
	bPrefix[1] = m_fLE ? HIBYTE(cx) : LOBYTE(cx);
	bPrefix[2] = m_fLE ? LOBYTE(cy) : HIBYTE(cy);
	bPrefix[3] = m_fLE ? HIBYTE(cy) : LOBYTE(cy);
	bPrefix[4] = 8;
	bPrefix[5] = 0;
	bPrefix[6] = bPrefix[0];
	bPrefix[7] = bPrefix[1];
	bPrefix[8] = bPrefix[2];
	bPrefix[9] = bPrefix[3];
	bPrefix[m_fLE ? 1 : 0] |= 0x00;
	bPrefix[m_fLE ? 3 : 2] |= 0x80;
	io.Write(bPrefix, 10);
	//
	for (UINT y = 0; y < cy; y++) {
		for (UINT x = 0; x < cx; x ++) {
			BYTE bPix = rBMP.GetPixel(x, y);
			if (!io.WriteByte(bPix))
				return false;
		}
	}
	//
	if (!fv.Alloc(io.GetPosition()))
		return false;
	return true;
}

bool CDM2ImageComp::MakeU4(DM2BMP4 &rBMP, DM2Pal4Loc16 &rPal16, SizeBuff &fv)
{
	UINT cx = rBMP.cx(), cy = rBMP.cy();

	if (cx < 1 || cy < 1 || cx > DMGHLi_MAX_WIDTH || cy > DMGHLi_MAX_HEIGHT) return false;

	if (!fv.Alloc(65536)) return false;
	MemDestIo io((BYTE *)fv.GetData(), (BYTE *)fv.GetData() + fv.GetSize());

	BYTE bPrefix[10] = {0,0,0,0,0,0,0,0,0,0};
	BYTE bSuffix[16];

	BYTE bPixMap[256];
	UINT iPal;
	for (iPal = 0; iPal < 256; iPal++) {
		UINT iIdx;
		for (iIdx = 0; iIdx < 16; iIdx++) {
			if (iPal == rPal16.iColorIdx[iIdx])
				break;
		}
		if (iIdx == 16) {
			bPixMap[iPal] = 255;
		} else {
			bPixMap[iPal] = iIdx;
		}
	}

	bPrefix[0] = m_fLE ? LOBYTE(cx) : HIBYTE(cx);
	bPrefix[1] = m_fLE ? HIBYTE(cx) : LOBYTE(cx);
	bPrefix[2] = m_fLE ? LOBYTE(cy) : HIBYTE(cy);
	bPrefix[3] = m_fLE ? HIBYTE(cy) : LOBYTE(cy);
	bPrefix[4] = 4;
	bPrefix[5] = 0;
	bPrefix[6] = bPrefix[0];
	bPrefix[7] = bPrefix[1];
	bPrefix[8] = bPrefix[2];
	bPrefix[9] = bPrefix[3];
	bPrefix[m_fLE ? 1 : 0] |= 0x80;
	bPrefix[m_fLE ? 3 : 2] |= 0x80;
	io.Write(bPrefix, 10);
	//
	for (UINT y = 0; y < cy; y++) {
		for (UINT x = 0; x < cx; x += 2) {
			BYTE bPix[2] = {
				bPixMap[rBMP.GetPixel(x  , y)],
				bPixMap[rBMP.GetPixel(x+1, y)],
			};
			BYTE iPix = 0
				| (bPix[0] << 4)
				| (bPix[1] << 0)
				;
			// MUST be byte aligned per scanline
			if (!io.WriteByte(iPix))
				return false;
		}
	}
	//
	memcpy(bSuffix, rPal16.iColorIdx, 16);
	if (!io.Write(bSuffix, 16))
		return false;
	if (!fv.Alloc(io.GetPosition()))
		return false;
	return true;
}

bool CDM2ImageComp::DecompC4(const SizeBuff &cSrc, DMGHLiIMGM &imgm, const DMGHLciDM2Pal &DM2Pal)
{
	bool fLE = m_fLE, fPrimaryPlane = true;

	int cbSrc = cSrc.GetSize();
	if (cbSrc < 7+16) return false;
	const UI8 *pbPos = cSrc.GetData();
	const UI8 *pbEnd = pbPos + cbSrc;
	int rcx = fLE ? (pbPos[0] | pbPos[1] * 256) : (pbPos[0] * 256 | pbPos[1]);
	int rcy = fLE ? (pbPos[2] | pbPos[3] * 256) : (pbPos[2] * 256 | pbPos[3]);
	int cx = (rcx) & 0x03FF;
	int cy = (rcy) & 0x03FF;
	if (cx > DMGHLi_MAX_WIDTH || cx < 1 || cy > DMGHLi_MAX_HEIGHT || cy < 1) return false;
	imgm.cx = cx;
	imgm.cy = cy;
	imgm.cPal.UseDM2From16Bytes(pbEnd - 16, DM2Pal);
	imgm.ipf.LoadFromC4(pbPos, fLE);
	imgm.fIs8bpp = false;
	DMGHLiIMGMPixelWriter pw(imgm);
	DMGHLiL2R4Bits thruIn(cSrc.GetData() +4, cSrc.GetSize() -4 -16);
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

		if (iPal == 0x20) {
			if (!pw.SpillPixels(nDuration)) return false;
		} else {
			if (!pw.WritePixels(nDuration, (UI8)iPal)) return false;
		}
	}
	return thruIn.GetPosition() == thruIn.GetLength();
}

int CDM2ImageComp::ReadDuration(DMGHLiL2R4Bits &thruIn)
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

bool CDM2ImageComp::DecompC8(const SizeBuff &cSrc, DMGHLiIMGM &imgm)
{
	bool fLE = m_fLE;

	int cbSrc = cSrc.GetSize();
	if (cbSrc < 8) return false;
	const UI8 *pbPos = cSrc.GetData();
	const UI8 *pbEnd = pbPos + cbSrc;
	int rcx = fLE ? (pbPos[0] | pbPos[1] * 256) : (pbPos[0] * 256 | pbPos[1]);
	int rcy = fLE ? (pbPos[2] | pbPos[3] * 256) : (pbPos[2] * 256 | pbPos[3]);
	int cx = (rcx) & 0x03FF;
	int cy = (rcy) & 0x03FF;
	if (cx > DMGHLi_MAX_WIDTH || cx < 1 || cy > DMGHLi_MAX_HEIGHT || cy < 1) return false;
	imgm.cx = cx;
	imgm.cy = cy;
	imgm.ipf.LoadFromC8(pbPos, fLE);
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

bool CDM2ImageComp::MakeC4Auto(const DMGHLiIMGM &imgm, SizeBuff &fv)
{
	DM2BMPImg4 aBmp4(imgm);
	DM2Pal4Loc16 aPal16;
	DM2Pal4Loc6 aPal6;
	IdxScatter aScatter;
	aScatter.MakeFrom(aBmp4);
	aPal16.MakeFrom(imgm.cPal);
	aPal6.MakeFrom(aScatter, aPal16);
	if (MakeC4(aBmp4, aPal16, aPal6, fv)) {
		return true;
	}
	return false;
}

bool CDM2ImageComp::MakeC8Auto(const DMGHLiIMGM &imgm, SizeBuff &fv)
{
	DM2BMPImg8 aBmp8(imgm);
	if (MakeC8(aBmp8, fv)) {
		return true;
	}
	return false;
}

bool CDM2ImageComp::MakeC4(DM2BMP4 &rBMP, DM2Pal4Loc16 &rPal16, DM2Pal4Loc6 &rPal6, SizeBuff &fv)
{
	return MakeC4(rBMP, &rPal16, rPal6, fv);
}

bool CDM2ImageComp::MakeC4(DM2BMP4 &rBMP, DM2Pal4Loc6 &rPal6, SizeBuff &fv)
{
	return MakeC4(rBMP, NULL, rPal6, fv);
}
