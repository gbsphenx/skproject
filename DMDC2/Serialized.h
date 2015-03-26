
#ifndef _SERIALIZED_H_
#define _SERIALIZED_H_

#include "SizeVuff.h"

namespace Serialized
{
	// 
	struct SizeBuffR {
		// 
		::SizeBuff &obj;

		// 
		SizeBuffR(::SizeBuff &obj)
			: obj(obj)
		{

		}
		// 
		friend CArchive &operator >>(CArchive &ar, Serialized::SizeBuffR &w)
		{
			::SizeBuff &x = w.obj;

			DWORD n;
			ar >> n;
			if (!x.Alloc(n))
				AfxThrowMemoryException();
			if (ar.Read(x.GetData(), n) != n)
				AfxThrowArchiveException(CArchiveException::endOfFile, NULL);

			return ar;
		}
	};
	// 
	struct SizeBuffW {
		// 
		const ::SizeBuff &obj;

		// 
		SizeBuffW(const ::SizeBuff &obj)
			: obj(obj)
		{

		}
		// 
		friend CArchive &operator <<(CArchive &ar, const Serialized::SizeBuffW &w)
		{
			const ::SizeBuff &x = w.obj;

			DWORD n = x.GetSize();
			ar << n;
			ar.Write(x.GetData(), n);

			return ar;
		}
	};
};

#endif // _SERIALIZED_H_
