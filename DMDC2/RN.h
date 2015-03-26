
#pragma once

#pragma pack(push, 1)

class RNBF
{
public:
	// 
	WORD idx:10;
	// 
	WORD db:4;
	// 
	WORD room:2;
};

class RNRP
{
public:
	// 
	WORD rid:14;
	// 
	WORD room:2;
};

class RN
{
public:
	// 
	union { 
		// 
		WORD w;
		// 
		RNBF bf;
		// 
		RNRP rp;
	} r;
	// 
	BYTE db() const { return r.bf.db; }
	// 
	WORD idx() const { return r.bf.idx; }
	// 
	BYTE room() const { return r.bf.room; }
	// 
	void db(BYTE x) { r.bf.db = x; }
	// 
	void idx(WORD x) { r.bf.idx = x; }
	// 
	void room(BYTE x) { r.bf.room = x; }
	// 
	bool operator <(const RN &s) const { return r.w < s.r.w; }
	// 
	bool operator ==(const RN &s) const { return r.w == s.r.w; }
	// 
	bool operator !=(const RN &s) const { return r.w != s.r.w; }
	// 
	friend CArchive &operator <<(CArchive &ar, const RN &s) { ar << s.r.w; return ar; }
	// 
	friend CArchive &operator >>(CArchive &ar, RN &s) { ar >> s.r.w; return ar; }
};

class RNco : public RN
{
public:
	// 
	RNco() { r.w = 0xffff; }
	// 
	RNco(WORD w) { r.w = w; }
	// 
	RNco(UINT db, UINT idx, UINT room) { r.bf.idx = idx; r.bf.db = db; r.bf.room = room; }
};

extern RN RN_NIL;
extern RN RN_END;
extern RN RNff80;

extern bool IsThisAsIs(const RN &rn);

#pragma pack(pop)
