
#ifndef _SIZEVUFF_H_
#define _SIZEVUFF_H_

#include "DMGHLt.h"
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
	bool Assign(const SizeBuff &fv)
	{
		if (!Alloc(fv.GetSize(), fv.GetData()))
			return false;
		return true;
	}

	// 
	UI8 *GetData() { return (UI8 *)pvData; }
	// 
	const UI8 *GetData() const { return (const UI8 *)pvData; }
	// 
	int GetSize() const { return cbData; }

	// 
	const SizeBuff &operator =(const SizeBuff &s)
	{
		Free();
		Alloc(s.GetSize(), s.GetData());
		return *this;
	}
	// 
	bool operator ==(const SizeBuff &s) const
	{
		int n = GetSize();
		if (n != s.GetSize()) return false;
		return memcmp(GetData(), s.GetData(), n) == 0;
	}
	// 
	bool operator !=(const SizeBuff &s) const
	{
		return !(*this == s);
	}
};

#endif // #define _SIZEVUFF_H_
