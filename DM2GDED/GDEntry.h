// GDEntry.h: CGDEntry クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GDENTRY_H__7D76CDD8_A2C4_4818_A3A6_32FA9DD37633__INCLUDED_)
#define AFX_GDENTRY_H__7D76CDD8_A2C4_4818_A3A6_32FA9DD37633__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct CGDEntry
{
	// 
	BYTE c[6];
	// 
	static CGDEntry ERR, Min, Max;

	// 
	CGDEntry() { }
	// 
	CGDEntry(BYTE c1, BYTE c2, BYTE c3, BYTE c4, BYTE c5, BYTE c6)
	{
		c[0] = c1, c[1] = c2, c[2] = c3, c[3] = c4, c[4] = c5, c[5] = c6;
	}
	// 
	inline BYTE c1() const { return c[0]; }
	// 
	inline BYTE c2() const { return c[1]; }
	// 
	inline BYTE c3() const { return c[2]; }
	// 
	inline BYTE c4() const { return c[3]; }
	// 
	inline BYTE c5() const { return c[4]; }
	// 
	inline BYTE c6() const { return c[5]; }
	// 
	void c1(BYTE x) { c[0] = x; }
	// 
	void c2(BYTE x) { c[1] = x; }
	// 
	void c3(BYTE x) { c[2] = x; }
	// 
	void c4(BYTE x) { c[3] = x; }
	// 
	void c5(BYTE x) { c[4] = x; }
	// 
	void c6(BYTE x) { c[5] = x; }
	// 
	static LPCSTR GetFriendlyTypeName(BYTE c);
	// 
	static LPCSTR GetFriendlyClass1Name(BYTE c);

	// 
	int CompareTo(const CGDEntry &s) const
	{
		return memcmp(c, s.c, 6);
	}
	// 
	bool operator ==(const CGDEntry &s) const
	{
		return CompareTo(s) == 0;
	}
	// 
	bool operator !=(const CGDEntry &s) const
	{
		return CompareTo(s) != 0;
	}
	// 
	bool operator <(const CGDEntry &s) const
	{
		return CompareTo(s) < 0;
	}
	// 
	bool operator <=(const CGDEntry &s) const
	{
		return CompareTo(s) <= 0;
	}

};

class CGDEntryMatch
{
public:
	// 
	virtual bool operator ()(const CGDEntry &pos) = NULL;
};

#endif // !defined(AFX_GDENTRY_H__7D76CDD8_A2C4_4818_A3A6_32FA9DD37633__INCLUDED_)
