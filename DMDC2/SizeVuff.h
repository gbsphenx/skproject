
// +--------------------------------------------------
// |
// | SizeVuff.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |
// | DMDC2 : Copyright (c) 2004-2005, kentaro-k.21
// |

#ifndef _SIZEVUFF_H_
#define _SIZEVUFF_H_

#include <malloc.h>

class SizeBuff
{
	// 
	void *pvData;
	// 
	int cbData;

	// 
	static void *Realloc(void *pv, int cb)
	{
		if (pv && cb)
			return realloc(pv, cb);
		else if (pv)
			free(pv);
		else if (cb)
			return realloc(pv, cb);
		return NULL;
	}

public:
	// 
	SizeBuff(): pvData(NULL), cbData(NULL)
	{

	}
	// 
	SizeBuff(int cb): pvData(NULL), cbData(NULL)
	{
		Alloc(cb);
	}
	// 
	SizeBuff(const SizeBuff &s): pvData(NULL), cbData(NULL)
	{
		*this = s;
	}
	// 
	~SizeBuff()
	{
		Free();
	}

	// 
	void Free()
	{
		Realloc(pvData, 0);
		pvData = NULL;
		cbData = 0;
	}
	// 
	bool Alloc(int cb, const void *pv = NULL)
	{
		void *pvNew = Realloc(pvData, cb);
		if (pvNew == NULL && cb)
			return false;
		if (pvNew) {
			pvData = pvNew;
			cbData = cb;
			if (pv) memcpy(pvNew, pv, cb);
			return true;
		}
		if (cb == 0) {
			pvData = NULL;
			cbData = 0;
			return true;
		}
		return false;
	}

	// 
	BYTE *GetData() { return (BYTE *)pvData; }
	// 
	const BYTE *GetData() const { return (const BYTE *)pvData; }
	// 
	int GetSize() const { return cbData; }

	// 
	bool Copy(const SizeBuff &s)
	{
		Free();
		return Alloc(s.GetSize(), s.GetData());
	}
	// 
	const SizeBuff &operator =(const SizeBuff &s)
	{
		Copy(s);
		return *this;
	}
};

#endif // #define _SIZEVUFF_H_
