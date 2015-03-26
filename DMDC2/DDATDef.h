
#pragma once

#include "rn.h"
#include "SizeVuff.h"
#include "Serialized.h"

//CArchive &readByteArray(CArchive &ar, CByteArray &s);
//CArchive &writeByteArray(CArchive &ar, const CByteArray &s);
//CArchive &readWordArray(CArchive &ar, CWordArray &s);
//CArchive &writeWordArray(CArchive &ar, const CWordArray &s);

class DBIPos_t
{
public:
	// 
	static DBIPos_t END, NIL;
	// 
	union {
		// 
		WORD w;
		// 
		struct {
			// 
			WORD idx:10;
			// 
			WORD db:4;
		} bf;
	} r;
	// 
	DBIPos_t() { r.w = 0xffff; }
	// 
	static DBIPos_t From(UINT w) { DBIPos_t x; x.r.w = w; return x; }
	// 
	explicit DBIPos_t(RN k)
	{
		if (IsThisAsIs(k)) {
			r.w = k.r.w;
		} else {
			r.w = 0; r.bf.db = k.db(); r.bf.idx = k.idx();
		}
	}
	// 
	DBIPos_t(UINT db, UINT idx) { r.w = 0; r.bf.db = db; r.bf.idx = idx; }
	// 
	inline void db(UINT x) { r.bf.db = x; }
	// 
	inline void idx(UINT x) { r.bf.idx = x; }
	// 
	inline UINT db() const { return r.bf.db; }
	// 
	inline UINT idx() const { return r.bf.idx; }
	// 
	inline bool operator <(const DBIPos_t &s) const { return r.w < s.r.w; }
	// 
	inline bool operator ==(const DBIPos_t &s) const { return r.w == s.r.w; }
	// 
	inline bool operator !=(const DBIPos_t &s) const { return r.w != s.r.w; }
	// 
	friend CArchive &operator <<(CArchive &ar, const DBIPos_t &s)
	{
		ar << s.r.w; return ar;
	}
	// 
	friend CArchive &operator >>(CArchive &ar, DBIPos_t &s)
	{
		ar >> s.r.w; return ar;
	}
};

struct DMDBX
{
	// 
	RN next;
};

struct DMDB0 : public DMDBX
{
	// 
	WORD w2;
};

struct DMDB1 : public DMDBX
{
	// 
	WORD w2, w4;
};

struct DMDB2 : public DMDBX
{
	// 
	WORD w2;
};

struct DMDB3 : public DMDBX
{
	// 
	WORD w2, w4, w6;
};

struct DMDB4 : public DMDBX
{
	// 
	RN stolen;
	// 
	BYTE b4, b5;
	// 
	WORD w6[4], w14;
};

struct DMDB4XMAP : public DMDBX
{
	// 
	RN stolen;
	// 
	BYTE b4, b5;
	// 
	WORD w6[4], w14;
	// 
	WORD w16, w18;
};

struct DMDB5 : public DMDBX
{
	// 
	WORD w2;
};

struct DMDB6 : public DMDBX
{
	// 
	WORD w2;
};

struct DMDB7 : public DMDBX
{
	// 
	WORD w2;
};

struct DMDB8 : public DMDBX
{
	// 
	WORD w2;
};

struct DMDB9 : public DMDBX
{
	// 
	RN stolen;
	// 
	WORD w4;
	// 
	BYTE b6[2];

};

struct DMDB9XMAP : public DMDBX
{
	// 
	RN stolen;
	// 
	WORD w4;
	// 
	BYTE b6[2];
	// 
	WORD w8, w10;

};

struct DMDB10 : public DMDBX
{
	// 
	WORD w2;
};

struct DMDB11 : public DMDB3
{

};

struct DMDB12 : public DMDB3
{

};

struct DMDB13 : public DMDB3
{

};

struct DMDB14 : public DMDBX
{
	// 
	RN stolen;
	// 
	BYTE b4[2];
	// 
	WORD w6;
};

struct DMDB14XMAP : public DMDBX
{
	// 
	RN stolen;
	// 
	BYTE b4[2];
	// 
	WORD w6;
	// 
	WORD w8, w10;
};

struct DMDB15 : public DMDBX
{
	// 
	WORD w2;
};

union DMDBU {
	// 
	DMDB0 r0;
	// 
	DMDB1 r1;
	// 
	DMDB2 r2;
	// 
	DMDB3 r3;
	// 
	DMDB4 r4;
	// 
	DMDB4XMAP r4xmap;
	// 
	DMDB5 r5;
	// 
	DMDB6 r6;
	// 
	DMDB7 r7;
	// 
	DMDB8 r8;
	// 
	DMDB9 r9;
	// 
	DMDB9XMAP r9xmap;
	// 
	DMDB10 r10;
	// 
	DMDB11 r11;
	// 
	DMDB12 r12;
	// 
	DMDB13 r13;
	// 
	DMDB14 r14;
	// 
	DMDB14XMAP r14xmap;
	// 
	DMDB15 r15;

	class Ar {
		CArchive &ar;

	public:
		Ar(CArchive &ar): ar(ar) { }

		// 
		CArchive &operator <<(const DMDBU &s)
		{
			ar.Write(&s, sizeof(s));
			return ar;
		}
		// 
		CArchive &operator >>(DMDBU &s)
		{
			ar.Read(&s, sizeof(s));
			return ar;
		}
	};
};

struct DMDBS {
	// 
	DMDB0 r0;
	// 
	DMDB1 r1;
	// 
	DMDB2 r2;
	// 
	DMDB3 r3;
	// 
	DMDB4 r4;
	// 
	DMDB5 r5;
	// 
	DMDB6 r6;
	// 
	DMDB7 r7;
	// 
	DMDB8 r8;
	// 
	DMDB9 r9;
	// 
	DMDB10 r10;
	// 
	DMDB11 r11;
	// 
	DMDB12 r12;
	// 
	DMDB13 r13;
	// 
	DMDB14 r14;
	// 
	DMDB15 r15;
};

struct AllMapDesc_t
{
	// 
	UINT nRandomGraphicsSeed;
	// 
	BYTE nStartX, nStartY, nStartDir;
	// 
	WORD nMinObjectListSize;
	// 
	SizeBuff fvTextData;
	// 
	AllMapDesc_t() { }
	// 
	void Init()
	{
		nRandomGraphicsSeed = 0;
		nStartX = nStartY = nStartDir = 0;
		nMinObjectListSize = 32;
		fvTextData.Free();
	}
	// 
	const AllMapDesc_t &operator =(const AllMapDesc_t &src)
	{
		nRandomGraphicsSeed = src.nRandomGraphicsSeed;
		nStartX = src.nStartX;
		nStartY = src.nStartY;
		nStartDir = src.nStartDir;
		nMinObjectListSize = src.nMinObjectListSize;
		fvTextData = src.fvTextData;
		return *this;
	}
	// 
	friend CArchive &operator <<(CArchive &ar, const AllMapDesc_t &src)
	{
		ar	<< src.nRandomGraphicsSeed
			<< src.nStartX << src.nStartY << src.nStartDir 
			<< src.nMinObjectListSize
			<< Serialized::SizeBuffW(src.fvTextData);
		return ar;
	}
	// 
	friend CArchive &operator >>(CArchive &ar, AllMapDesc_t &dst)
	{
		ar	>> dst.nRandomGraphicsSeed
			>> dst.nStartX >> dst.nStartY >> dst.nStartDir 
			>> dst.nMinObjectListSize
			>> Serialized::SizeBuffR(dst.fvTextData);
		return ar;
	}
};

class Atlas_t
{
public:
	// 
	BYTE c[32][32];
	// 
	RN r[32][32];

};

class AtlasBit_t
{
public:
	// 
	BYTE c;
	// 
	RN r;
	// 
	friend CArchive &operator <<(CArchive &ar, const AtlasBit_t &s)
	{
		ar << s.c << s.r;
		return ar;
	}
	// 
	friend CArchive &operator >>(CArchive &ar, AtlasBit_t &s)
	{
		ar >> s.c >> s.r;
		return ar;
	}
};

class AtlasPos_t
{
public:
	// 
	union {
		// 
		struct {
			// 
			BYTE x:8;
			// 
			BYTE y:8;
			// 
			BYTE z:8;
		} r;
		// 
		DWORD i;
	} k;
	// 
	AtlasPos_t(BYTE _z, BYTE _x, BYTE _y) { k.i = 0, k.r.x = _x, k.r.y = _y, k.r.z = _z; }
	// 
	AtlasPos_t() { k.i = -1; }
	// 
	inline BYTE X() const { return k.r.x & 0x1f; }
	// 
	inline BYTE Y() const { return k.r.y & 0x1f; }
	// 
	inline BYTE Z() const { return k.r.z; }
	// 
	bool operator <(const AtlasPos_t &s) const { return k.i < s.k.i; }
	// 
	bool operator ==(const AtlasPos_t &s) const { return k.i == s.k.i; }
	// 
	friend CArchive &operator <<(CArchive &ar, const AtlasPos_t &s) { ar << s.k.i; return ar; }
	// 
	friend CArchive &operator >>(CArchive &ar, AtlasPos_t &s) { ar >> s.k.i; return ar; }
};

class ATP_t
{
public:
	// 
	union {
		// 
		struct {
			// 
			BYTE x:8;
			// 
			BYTE y:8;
			// 
			BYTE z:8;
			// 
			BYTE room:8;
		} r;
		// 
		DWORD i;
	} k;

	// 
	ATP_t() { k.i = -1; }
	// 
	ATP_t(BYTE z, BYTE x, BYTE y, BYTE room) { k.i = 0, k.r.x = x, k.r.y = y, k.r.z = z; k.r.room = room & 3; }
	// 
	explicit ATP_t(AtlasPos_t s) { k.r.x = s.X(); k.r.y = s.Y(); k.r.z = s.Z(); k.r.room = 0; }
	// 
	ATP_t(AtlasPos_t s, BYTE room) { k.r.x = s.X(); k.r.y = s.Y(); k.r.z = s.Z(); k.r.room = room & 3; }
	// 
	inline BYTE X() const { return k.r.x & 0x1f; }
	// 
	inline BYTE Y() const { return k.r.y & 0x1f; }
	// 
	inline BYTE Z() const { return k.r.z; }
	// 
	inline BYTE room() const { return k.r.room; }
	// 
	AtlasPos_t T() const { return AtlasPos_t(Z(), X(), Y()); }
	// 
	bool operator <(const ATP_t &s) const { return k.i < s.k.i; }
	// 
	bool operator ==(const ATP_t &s) const { return k.i == s.k.i; }
};

class OptimizeTester_t
{
	// 
	BYTE bUsingTbl[16][1024/8];
	// 
	int iOld2New[16][1024];
	// 
	UINT nTblRec[16];

public:
	// 
	void Init()
	{
		ZeroMemory(bUsingTbl, sizeof(bUsingTbl));
	}
	// 
	void MarkAsUsing(UINT db, UINT idx)
	{
		bUsingTbl[db % 16][idx / 8] |= 1 << (idx % 8);
	}
	// 
	BOOL IsMarkedAsUsing(UINT db, UINT idx)
	{
		return (bUsingTbl[db % 15][idx / 8] & (1 << (idx % 8))) ? TRUE : FALSE;
	}
	// 
	void Optz(UINT z, UINT cx);
	// 
	int TrackNewPos(UINT z, UINT x)
	{
		return iOld2New[z % 16][x % 1024];
	}
	// 
	UINT GetNewTblLen(UINT z)
	{
		return nTblRec[z % 16];
	}
};
