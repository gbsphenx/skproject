// DM2RPtr.h: CDM2RPtr クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM2RPTR_H__E80296AC_7BED_472B_B9D9_8723E197E995__INCLUDED_)
#define AFX_DM2RPTR_H__E80296AC_7BED_472B_B9D9_8723E197E995__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template<class T> class CDM2SPtr
{
	// 
	T *p;

public:
	// 
	CDM2SPtr(): p(NULL)
	{

	}
	// 
	CDM2SPtr(T *p): p(p)
	{
		if (p) {
			p->AddRef();
		}
	}
	// 
	~CDM2SPtr()
	{
		if (p) {
			p->Release();
		}
	}
	// 
	const CDM2SPtr &operator =(const CDM2SPtr &r)
	{
		if (p) {
			p->Release();
		}
		if (p = r.p) {
			p->AddRef();
		}
		return *this;
	}
	// 
	void Attach(T *p)
	{
		this->p = p;
	}
	// 
	T& operator *()
	{
		if (p == NULL) throw bad_cast();
		return *p;
	}
	// 
	T* operator ->()
	{
		return p;
	}
	// 
	operator T*()
	{
		return p;
	}
};

template<class T> class CDM2RPtr
{
	// 
	template<class T> class Data
	{
		// 
		T *p;
		// 
		LONG nLocked;

	public:
		// 
		Data(T *p): p(p)
		{
			nLocked = 1;
		}
		// 
		~Data()
		{
			delete p;
		}
		// 
		void AddRef()
		{
			InterlockedIncrement(&nLocked);
		}
		// 
		void Release()
		{
			LONG x = InterlockedDecrement(&nLocked);
			if (!x) {
				delete this;
			}
		}
		// 
		T *_Ptr()
		{
			return p;
		}
	};

	// 
	Data<T> *z;

public:
	// 
	CDM2RPtr()
	{
		z = NULL;
	}
	// 
	CDM2RPtr(T *p)
	{
		z = new Data<T>(p);
	}
	// 
	CDM2RPtr(const CDM2RPtr<T> &r): z(r.z)
	{
		z->AddRef();
	}
	// 
	~CDM2RPtr()
	{
		if (z) {
			z->Release();
		}
	}
	// 
	const CDM2RPtr<T> &operator =(const CDM2RPtr<T> &r)
	{
		if (z) {
			z->Release();
		}
		if (z = r.z) {
			z->AddRef();
		}
		return *this;
	}
	// 
	T& operator *()
	{
		if (z == NULL) throw bad_cast();
		return *z->_Ptr();
	}
	// 
	T* operator ->()
	{
		return (z != NULL) ? z->_Ptr() : NULL;
	}
	// 
	operator T*()
	{
		return (z != NULL) ? z->_Ptr() : NULL;
	}

};

#endif // !defined(AFX_DM2RPTR_H__E80296AC_7BED_472B_B9D9_8723E197E995__INCLUDED_)
