// DM1ImageComp.cpp: CDM1ImageComp クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "DM1ImageComp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace
{

int CalcStrokeLen(const CDM1Pic &rPic, UINT iPixel, UINT nPixels, UINT cx)
{
	int nFinalStrokeLen = 0;
	for (int nBaseCost = 3; nBaseCost < 5; nBaseCost++) {
		//
		int nStrokeLen = 0;
		int nStrokeNewLen = 0;
		int nCost = 0;
		while (iPixel < nPixels) {
			BYTE iPix = rPic.GetPix(iPixel);
			ASSERT(iPix < 16);
			int iIdenticalPixel = iPixel + 1;
			for (; iIdenticalPixel < nPixels && iPix == rPic.GetPix(iIdenticalPixel); iIdenticalPixel++);
			int iEmptyPixel = iPixel;
			if (cx <= iPixel) {
				for (; iEmptyPixel < nPixels && rPic.GetPix(iEmptyPixel - cx) == rPic.GetPix(iEmptyPixel); iEmptyPixel++);
			}
			if (iIdenticalPixel < iEmptyPixel && iEmptyPixel + 1 <= nPixels) {
				int nLen = iEmptyPixel - iPixel;
				nStrokeNewLen += nLen + 1;
				//
				if (nLen == 0) {
					ASSERT(false);
				} else if (nLen <= 256) {
					nCost += 2;
				} else if (nLen <= 65536) {
					nCost += 3;
				} else {
					ASSERT(false);
					break;
				}
				//
				iPixel = iEmptyPixel + 1;
			} else if (iPixel < iIdenticalPixel) {
				int nLen = iIdenticalPixel - iPixel;
				nStrokeNewLen += nLen;
				//
				if (nLen == 0) {
					ASSERT(false);
				} else if (nLen <= 8) {
					nCost += 1;
				} else if (nLen <= 256) {
					nCost += 2;
				} else if (nLen <= 65536) {
					nCost += 3;
				} else {
					ASSERT(false);
					break;
				}
				//
				iPixel = iIdenticalPixel;
			} else {
				ASSERT(false);
			}
			if (256 < nStrokeNewLen)
				break;
			// 1 ( 4) -- [ 9][p0][ 0][ 1]
			// 2 ( 6) -- [ 9][--][ 0][ 2][p0][p1]
			// 3 ( 6) -- [ 9][p0][ 0][ 2][p1][p2]
			// 4 ( 8) -- [ 9][--][ 0][ 2][p0][p1][p2][p3]
			// 5 ( 8) -- [ 9][p0][ 0][ 2][p1][p2][p3][p4]
			// 6 (10) -- [ 9][--][ 0][ 2][p0][p1][p2][p3][p4][p5]

			// 1: 4 -- 1->4+2*0
			// 2: 6 -- 2->4+2*1
			// 3: 6 -- 3->4+2*1
			// 4: 8 -- 4->4+2*2
			// 5: 8 -- 5->4+2*2
			// 6:10 -- 6->4+2*3

			int nCostAdd = __max(0, nBaseCost - nStrokeNewLen);

			int nStrokeCost = 4 + (nStrokeNewLen & (~1));

			if (nStrokeCost / 2 > nCost + nCostAdd) {
				break;
			}

			if (nStrokeCost / 2 < nCost) {
				nStrokeLen = nStrokeNewLen;
			}
		}
		//
		if (nStrokeLen != 0) nFinalStrokeLen = __min(nFinalStrokeLen, nStrokeLen);
	}
	return nFinalStrokeLen;
}

};

//////////////////////////////////////////////////////////////////////
// CDM1ImageComp::DM1Pal4Loc6

void CDM1ImageComp::DM1Pal4Loc6::MakeFrom(const IdxScatter &rScatter)
{
	typedef multimap<WORD, BYTE, greater<WORD> > Cnt2IdxMMap;

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
// CDM1ImageComp::IdxScatter

bool CDM1ImageComp::IdxScatter::MakeFrom(const CDM1Pic &rPic)
{
	const UINT cx = rPic.cx(), cy = rPic.cy();

	for (UINT i = 0; i < 16; i++)
		nScatter[i] = 0;
	for (UINT y = 0; y < cy; y++) {
		for (UINT x = 0; x < cx; x++) {
			BYTE c = rPic.GetPixel(x, y);
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
// CDM1ImageComp

bool CDM1ImageComp::MakeI3(const CDM1Pic &rPic, SizeBuff &sbNew)
{
	const UINT cx = rPic.cx(), cy = rPic.cy();

	if (320U < cx || 200U < cy) return false;

	if (!sbNew.Alloc(65536)) return false;

	CMemVuff mv;
	mv.pData = sbNew.GetData();
	mv.iData = 0;
	mv.nData = sbNew.GetSize();
	mv.fLE = fLE;

	IdxScatter cis;
	cis.MakeFrom(rPic);
	DM1Pal4Loc6 cl6;
	cl6.MakeFrom(cis);

	BYTE bLocalPixMap[16];
	UINT iIdx;
	for (iIdx = 0; iIdx < 16; iIdx++) {
		UINT iLoc;
		for (iLoc = 0; iLoc < 6; iLoc++) {
			if (cl6.iLocalIdx[iLoc] == iIdx)
				break;
		}
		if (iLoc == 6) {
			bLocalPixMap[iIdx] = 6 + iIdx;
		} else {
			bLocalPixMap[iIdx] = iLoc;
		}
	}

	DM1I3I4Wio cw(mv);

	mv.WriteWord(cx);
	mv.WriteWord(cy);

	UINT i;
	for (i = 0; i < 6; i++) cw.Write4Bit(cl6.iLocalIdx[i]);

	UINT iPixel = 0, nPixels = cx * cy;
	while (iPixel < nPixels) {
		BYTE iPix = rPic.GetPixel(iPixel % cx, iPixel / cx);
		ASSERT(iPix < 16);
		int iIdenticalPixel = iPixel + 1;
		for (; iIdenticalPixel < nPixels && iPix == rPic.GetPix(iIdenticalPixel); iIdenticalPixel++);
		int iEmptyPixel = iPixel;
		if (cx <= iPixel) {
			for (; iEmptyPixel < nPixels && rPic.GetPix(iEmptyPixel - cx) == rPic.GetPix(iEmptyPixel); iEmptyPixel++);
		}
		if (iIdenticalPixel <= iEmptyPixel) {
			iEmptyPixel -= iPixel;
			if (!cw.WritePixels(iEmptyPixel, 0x16))
				return false;
			iPixel += iEmptyPixel;
		} else {
			iIdenticalPixel -= iPixel;
			if (!cw.WritePixels(iIdenticalPixel, bLocalPixMap[iPix]))
				return false;
			iPixel += iIdenticalPixel;
		}
	}
	cw.Flush();
	if (!sbNew.Alloc(mv.iData))
		return false;
	return true;
}

bool CDM1ImageComp::MakeI1(const CDM1Pic &rPic, SizeBuff &sbNew)
{
	const UINT cx = rPic.cx(), cy = rPic.cy();

	if (320U < cx || 200U < cy) return false;

	if (!sbNew.Alloc(65536)) return false;

	CMemVuff mv;
	mv.pData = sbNew.GetData();
	mv.iData = 0;
	mv.nData = sbNew.GetSize();
	mv.fLE = fLE;

	DM1I1I2Wio cw(mv);

	mv.WriteWord(cx);
	mv.WriteWord(cy);

	UINT iPixel = 0, nPixels = cx * cy;
	while (iPixel < nPixels) {
		BYTE iPix = rPic.GetPix(iPixel);
		ASSERT(iPix < 16);
		int iIdenticalPixel = iPixel + 1;
		for (; iIdenticalPixel < nPixels && iPix == rPic.GetPix(iIdenticalPixel); iIdenticalPixel++);
		int iEmptyPixel = iPixel;
		if (cx <= iPixel) {
			for (; iEmptyPixel < nPixels && rPic.GetPix(iEmptyPixel - cx) == rPic.GetPix(iEmptyPixel); iEmptyPixel++);
		}
		if (iIdenticalPixel < iEmptyPixel && iEmptyPixel + 1 <= nPixels) {
			int nLen = iEmptyPixel - iPixel;
			if (!cw.SpillPixels(nLen, rPic.GetPix(iEmptyPixel)))
				return false;
			iPixel = iEmptyPixel + 1;
		} else if (iIdenticalPixel == iPixel + 1) {
			//
			int nLen = CalcStrokeLen(rPic, iPixel, nPixels, cx);
			if (nLen == 0) {
				if (!cw.WritePixels(1, iPix))
					return false;
				iPixel++;
			} else {
				if (!cw.StrokeBegin(nLen))
					return false;
				while (nLen != 0) {
					if (!cw.StrokeNext(rPic.GetPix(iPixel)))
						return false;
					iPixel++; nLen--;
				}
			}
			//
		} else {
			int nLen = iIdenticalPixel - iPixel;
			if (!cw.WritePixels(nLen, iPix))
				return false;
			iPixel = iIdenticalPixel;
		}
	}
/*
	0 -+
	1  |
	2  |
	3  | Write [1 to 8] pixels
	4  |
	5  |
	6  |
	7 -+
	8 -- Write [1 to 256] pixels
	9 -- Write [1] pixel + Stroke [0, 2, 4, ... 254] pixels
	     or
		 Stroke [2, 4, ... 256] pixels
	A -- Transparent [1 to 16] pixels
	B -- Spill [1 to 256] pixels + Write [1] pixel
	C -- Write [1 to 65536] pixels
	D -- ?
	E -+ 0 -+
	   | 1  |
	   | 2  |
	   | 3  |
	   | 4  |
	   | 5  | Transparent [17 to 29] pixels
	   | 6  |
	   | 7  |
	   | 8  |
	   | 9  |
	   | A  |
	   | B  |
	   | C -+
	   | D -- Transparent [1 to 256] pixels
	   | E -- Transparent [257 to 512] pixels
	   | F -- Transparent [1 to 65536] pixels
	F -- Spill [1 to 65536] pixels + Write [1] pixel
*/
/*
	Write [1 to 8]
	Write [1 to 256]
	Write [1 to 65536]

	Write [1] + Stroke [0, 2, 4, ... 254] ---> Stroke [1, 3, 5, ... 255]
	Stroke [2, 4, 6, ... 256]

	Spill [1 to 256] + Write [1]
	Spill [1 to 65536] + Write [1]
*/
	cw.Flush();
	if (!sbNew.Alloc(mv.iData))
		return false;
	return true;
}
