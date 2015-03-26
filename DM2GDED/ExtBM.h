// ExtBM.h: CExtBM クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXTBM_H__F8A059E0_0624_4FE9_857A_0E98F0B2854E__INCLUDED_)
#define AFX_EXTBM_H__F8A059E0_0624_4FE9_857A_0E98F0B2854E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SizeVuff.h"

class CExtBMx
{
public:
	// 
	class CompatDC : public CDC
	{
	public:
		// 
		CompatDC(CWnd *pParentWnd)
		{
			CClientDC dc(pParentWnd);
			if (!CreateCompatibleDC(&dc)) AfxThrowResourceException();
		}
	};
	// 
	class SaveToMem
	{
	public:
		// 
		virtual void *Alloc1ce(UINT nSize) = NULL;
	};
	// 
	class SaveToGlobalMem : public SaveToMem
	{
		// 
		HGLOBAL m_hMem;

	public:
		// 
		SaveToGlobalMem()
		{
			m_hMem = NULL;
		}
		// 
		~SaveToGlobalMem()
		{
			if (m_hMem != NULL) {
				VERIFY(GlobalUnlock(m_hMem) == NULL);
				VERIFY(GlobalFree(m_hMem) == NULL);
			}
		}
		// 
		virtual void *Alloc1ce(UINT nSize)
		{
			if (m_hMem != NULL || (m_hMem = GlobalAlloc(GMEM_MOVEABLE, nSize)) == NULL) return NULL;

			void *pv = GlobalLock(m_hMem);
			ASSERT(pv != NULL);
			return pv;
		}
		// 
		HGLOBAL Detach()
		{
			if (m_hMem == NULL) return NULL;

			VERIFY(GlobalUnlock(m_hMem) == NULL);

			HGLOBAL hMem = m_hMem;
			m_hMem = NULL;
			return hMem;
		}
	};
	// 
	class SaveToFixedVuffer : public SaveToMem
	{
		// 
		SizeBuff &fv;

	public:
		// 
		SaveToFixedVuffer(SizeBuff &fv): fv(fv) { fv.Free(); }
		// 
		virtual void *Alloc1ce(UINT nSize)
		{
			if (fv.GetData() != NULL) return NULL;
			if (!fv.Alloc(nSize)) return NULL;

			return fv.GetData();
		}
	};
	// 
	class SaveToFM : public SaveToMem
	{
		// 
		HANDLE hFile, hFM;
		// 
		LPVOID pvMem;
		// 
		CString strPath;

	public:
		// 
		SaveToFM(LPCTSTR psz): strPath(psz), hFile(NULL), hFM(NULL), pvMem(NULL)
		{

		}
		// 
		~SaveToFM()
		{
			Close();
		}
		// 
		void Close()
		{
			if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
				return;
			if (pvMem) {
				VERIFY(UnmapViewOfFile(pvMem));
			}
			if (hFM) {
				VERIFY(CloseHandle(hFM));
			}
			VERIFY(CloseHandle(hFile));

			hFile = NULL;
			hFM = NULL;
			pvMem = NULL;
		}
		// 
		virtual void *Alloc1ce(UINT nSize)
		{
			if (hFile != NULL) return NULL;

			hFile = CreateFile(
				strPath, 
				0 |GENERIC_READ |GENERIC_WRITE,
				0 |FILE_SHARE_READ,
				NULL,
				CREATE_ALWAYS,
				0,
				NULL
				);
			if (hFile == INVALID_HANDLE_VALUE) {
				// It's ok that 'hFile == INVALID_HANDLE_VALUE'.
				return NULL;
			}
			hFM = CreateFileMapping(
				hFile,
				NULL,
				0 |PAGE_READWRITE |SEC_COMMIT,
				0,
				nSize,
				NULL
				);
			if (hFM == NULL)
				return NULL;
			pvMem = MapViewOfFile(
				hFM,
				0 |FILE_MAP_READ |FILE_MAP_WRITE,
				0,
				0,
				nSize
				);
			if (pvMem == NULL)
				return NULL;
			return pvMem;
		}
	};
	// 
	class LoadFromMem
	{
	public:
		// 
		virtual const void *ReadNextLock(UINT nSize) = NULL;
	};
	// 
	class LoadFromRangedMem : public LoadFromMem
	{
		// 
		const BYTE *m_pbPos;
		// 
		const BYTE *m_pbEnd;

	public:
		// 
		LoadFromRangedMem(const BYTE *pbPos, UINT cbPos)
		{
			m_pbEnd = (m_pbPos = pbPos) + cbPos;
		}
		// 
		virtual const void *ReadNextLock(UINT nSize)
		{
			const BYTE *pbRet = NULL;
			if (m_pbPos + nSize <= m_pbEnd) {
				pbRet = m_pbPos;
				m_pbPos += nSize;
			}
			return pbRet;
		}
	};
	// 
	class LoadFromGlobalMem : public LoadFromMem
	{
		// 
		HGLOBAL m_hMem;
		// 
		BYTE *m_pbPos;
		// 
		BYTE *m_pbEnd;

	public:
		// 
		LoadFromGlobalMem(HGLOBAL hMem): m_hMem(NULL)
		{
			if (hMem != NULL && (m_pbPos = (BYTE *)GlobalLock(hMem)) != NULL) {
				m_hMem = hMem;
				m_pbEnd = m_pbPos + GlobalSize(hMem);
			}
		}
		// 
		~LoadFromGlobalMem()
		{
			if (m_hMem != NULL) GlobalUnlock(m_hMem);
		}
		// 
		virtual const void *ReadNextLock(UINT nSize)
		{
			BYTE *pbRet = NULL;
			if (m_pbPos + nSize <= m_pbEnd) {
				pbRet = m_pbPos;
				m_pbPos += nSize;
			}
			return pbRet;
		}
	};
};

class CExtBM : public CBitmap
{
	// 
	typedef CExtBMx::CompatDC CompatDC;
	// 
	typedef CExtBMx::SaveToMem SaveToMem;
	// 
	typedef CExtBMx::SaveToGlobalMem SaveToGlobalMem;
	// 
	typedef CExtBMx::SaveToFixedVuffer SaveToFixedVuffer;
	// 
	typedef CExtBMx::LoadFromMem LoadFromMem;
	// 
	typedef CExtBMx::LoadFromRangedMem LoadFromRangedMem;
	// 
	typedef CExtBMx::LoadFromGlobalMem LoadFromGlobalMem;

	// 
	CPalette m_Pal;
	// 
	BITMAP m_bm;

public:
	// 
	CExtBM();
	// 
	virtual ~CExtBM();
	// 
	BOOL Attach(HBITMAP hBM);
	// 
	HBITMAP Detach();
	// 
	BOOL SaveTo(HGLOBAL &hMem);
	// 
	BOOL SaveTo(SizeBuff &fv, bool fToFile = false);
	// 
	BOOL SaveTo(SaveToMem &rMem, bool fToFile);
	// 
	BOOL SaveToFile(LPCTSTR pszFileInto);
	// 
	BOOL Create(CSize size, int nBitsPixel, const RGBQUAD *p4, int n4 = 0);
	// 
	BOOL LoadFrom(HGLOBAL hMem);
	// 
	BOOL LoadFrom(const void *pvPos, UINT cbPos, bool fFromFile);
	// 
	BOOL LoadFrom(LoadFromMem &rMem, bool fFromFile = false);
	// 
	BOOL DeleteObject();
	// 
	BYTE *GetPointer();
	// 
	BOOL GetBitmap(BITMAP &bm);
	// 
	int BitsPixel();
	// 
	CPalette &GetPalette();
	// 
	CSize GetSize();

};

#endif // !defined(AFX_EXTBM_H__F8A059E0_0624_4FE9_857A_0E98F0B2854E__INCLUDED_)
