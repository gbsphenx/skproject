// DM2RawContConv.h: CDM2RCConv クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM2RAWCONTCONV_H__56588C6B_84E6_4F18_8AA9_F7D3C83D5ED4__INCLUDED_)
#define AFX_DM2RAWCONTCONV_H__56588C6B_84E6_4F18_8AA9_F7D3C83D5ED4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SizeVuff.h"
#include "FM.h"

class CDM2RCConvMallocIf
{
public:
	// 
	virtual ~CDM2RCConvMallocIf() { }
	// 
	virtual void *Alloc(UINT cb) = NULL;
};

class CDM2RCConvMIfSizeBuff : public CDM2RCConvMallocIf
{
	// 
	SizeBuff &fv;

public:
	// 
	CDM2RCConvMIfSizeBuff(SizeBuff &fv): fv(fv)
	{
		fv.Free();
	}
	// 
	virtual void *Alloc(UINT cb)
	{
		if (fv.GetData() == NULL && fv.Alloc(cb)) {
			return fv.GetData();
		}
		return NULL;
	}
};

class CDM2RCConvMIfGHND : public CDM2RCConvMallocIf
{
	// 
	HGLOBAL hMem;

public:
	// 
	CDM2RCConvMIfGHND()
	{
		hMem = NULL;
	}
	// 
	virtual ~CDM2RCConvMIfGHND()
	{
		if (hMem) {
			GlobalUnlock(hMem);
			GlobalFree(hMem);
		}
	}
	// 
	virtual void *Alloc(UINT cb)
	{
		if (!hMem && (hMem = GlobalAlloc(GMEM_MOVEABLE, cb)) != NULL) {
			return GlobalLock(hMem);
		}
		return NULL;
	}
	// 
	HGLOBAL Detach()
	{
		HGLOBAL hMemRet = hMem;
		hMem = NULL;
		return hMemRet;
	}
	
};

class CDM2RCConv
{
	// 
	class MemSrcIoLE;
	// 
	class MemDestIo;

	// 
	bool fLE;
	// 
	UINT nGDRSDT;

public:
	// 
	CDM2RCConv(bool fLE, UINT nGDRSDT): fLE(fLE), nGDRSDT(nGDRSDT) { }
	// 
	bool DecodeRIFFWave(const BYTE *pbPos, UINT cb, CDM2RCConvMallocIf &rMalloc);
	// 
	bool EncodeRIFFWave(const BYTE *pbPos, UINT cb, CDM2RCConvMallocIf &rMalloc);

};

class CDM2RCConv::MemSrcIoLE
{
	// 
	const BYTE *pbBegin, *pbPos, *pbEnd;

public:
	// 
	MemSrcIoLE(const BYTE *pbBegin, const BYTE *pbEnd): pbBegin(pbBegin), pbPos(pbBegin), pbEnd(pbEnd)
	{

	}
	// 
	int ReadWord()
	{
		int r = -1;
		if (pbPos + 1 < pbEnd) {
			r = *(WORD *)pbPos;
			pbPos += 2;
		}
		return r;
	}
	// 
	int ReadDWord()
	{
		int r = -1;
		if (pbPos + 3 < pbEnd) {
			r = *(DWORD *)pbPos;
			pbPos += 4;
		}
		return r;
	}
	// 
	int SeekForward(int n)
	{
		if (n < 0) {
			if (pbPos + n < pbBegin)
				n = pbBegin -pbPos;
		} else {
			if (pbEnd < pbPos + n)
				n = pbEnd -pbPos;
		}
		pbPos += n;
		return pbPos - pbBegin;
	}
	// 
	UINT GetPosition() const { return pbPos - pbBegin; }

};

class CDM2RCConv::MemDestIo
{
public:
	// 
	BYTE *pbPos, *pbEnd;

	// 
	MemDestIo(BYTE *pbBegin, BYTE *pbEnd): pbPos(pbBegin), pbEnd(pbEnd) { }
	// 
	void WriteByte(UINT x)
	{
		if (pbPos < pbEnd) {
			*pbPos = x;
			pbPos++;
		}
	}
	// 
	void WriteWord(UINT x)
	{
		if (pbPos + 1 < pbEnd) {
			pbPos[0] = LOBYTE(x);
			pbPos[1] = HIBYTE(x);
			pbPos += 2;
		}
	}
	// 
	void WriteDWord(UI32 x)
	{
		if (pbPos + 3 < pbEnd) {
			pbPos[0] = LOBYTE(LOWORD(x));
			pbPos[1] = HIBYTE(LOWORD(x));
			pbPos[2] = LOBYTE(HIWORD(x));
			pbPos[3] = HIBYTE(HIWORD(x));
			pbPos += 4;
		}
	}
};

#endif // !defined(AFX_DM2RAWCONTCONV_H__56588C6B_84E6_4F18_8AA9_F7D3C83D5ED4__INCLUDED_)
