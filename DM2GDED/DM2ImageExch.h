// DM2ImageExch.h: CDM2ImageExch クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM2IMAGEEXCH_H__B024CC9D_345F_410B_9509_7CEDBA2E245E__INCLUDED_)
#define AFX_DM2IMAGEEXCH_H__B024CC9D_345F_410B_9509_7CEDBA2E245E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DMGHLci.h"

class CDM2ImageConv
{
	// 
	class ColorDOM
	{
		// 
		void **ppv;

		// 
		void **Mk()
		{
			size_t n = sizeof(void *) * 2 * 2 * 2;
			void **ppv = (void **)malloc(n);
			ZeroMemory(ppv, n);
			return ppv;
		}
		// 
		void Unmk(void **ppv, int z)
		{
			if (z == 8) return;

			for (int i = 0; i < 8; i++)
				if (ppv[i])
					Unmk((void **)ppv[i], z+1);

			free(ppv);
		}
		// 
		void Scan1(void **ppv, int i = 0, BYTE v1 = 0, BYTE v2 = 0, BYTE v3 = 0)
		{
			if (i == 8) {
				CString s; s.Format("%3d;%3d;%3d;%3d\n", (UINT)ppv, 0U+v1, 0U+v2, 0U+v3);
				TRACE0(s);
				return;
			}
			int t = 7 - i;
			for (int x = 0; x < 8; x++) {
				if (ppv[x]) {
					Scan1((void **)ppv[x], i+1
						, v1 |((x&1) ? (1 << t) : 0)
						, v2 |((x&2) ? (1 << t) : 0)
						, v3 |((x&4) ? (1 << t) : 0)
						);
				}
			}
		}

	public:
		// 
		ColorDOM()
		{
			ppv = Mk();
		}
		// 
		~ColorDOM()
		{
			Unmk(ppv, 0);
		}
		// 
		void Scan()
		{
			void **ppv = this->ppv;

			Scan1(ppv);
		}
		// 
		void AddColor(COLORREF clr, BYTE z)
		{
			BYTE v1 = GetRValue(clr);
			BYTE v2 = GetGValue(clr);
			BYTE v3 = GetBValue(clr);

			void **ppv = this->ppv;

			for (int i = 0; i < 8; i++) {
				BOOL f1 = (v1 & 128);
				BOOL f2 = (v2 & 128);
				BOOL f3 = (v3 & 128);
				int x = 0
					|(f1 ? 1 : 0)
					|(f2 ? 2 : 0)
					|(f3 ? 4 : 0)
					;
				if (i == 7) {
					ppv[x] = (void **)z;
				} else {
					if (ppv[x] == NULL)
						ppv[x] = Mk();
				}
				ppv = (void **)ppv[x];
				v1 <<= 1;
				v2 <<= 1;
				v3 <<= 1;
			}
		}
		// 
		BYTE Search(COLORREF clr)
		{
			BYTE v1 = GetRValue(clr);
			BYTE v2 = GetGValue(clr);
			BYTE v3 = GetBValue(clr);

			void **ppv = this->ppv;

			for (int i = 0; i < 8; i++) {
				BOOL f1 = (v1 & 128);
				BOOL f2 = (v2 & 128);
				BOOL f3 = (v3 & 128);
				int x = 0
					|(f1 ? 1 : 0)
					|(f2 ? 2 : 0)
					|(f3 ? 4 : 0)
					;
				int z = 0;
				for (; z < 8; z++) {
					if (ppv[x ^ z] != NULL) {
						ppv = (void **)ppv[x ^ z];
						break;
					}
				}
				if (z == 8)
					return 0;
				v1 <<= 1;
				v2 <<= 1;
				v3 <<= 1;
			}
			return (BYTE)(ppv);
		}
	};

	// 
	static HBITMAP CreateDIB(int cx, int cy, const DMGHLciPAL &rPal, bool fBM8bpp, void *&ppv, int &nPitch, const DMGHLciDM2Pal &DM2Pal);

public:
	// 
	CDM2ImageConv() { }
	// 
	bool EncodeBMTo4(HBITMAP hbm, DMGHLiIMGM &imgm, const DMGHLciDM2Pal &DM2Pal);
	// 
	bool EncodeBMTo8(HBITMAP hbm, DMGHLiIMGM &imgm, const DMGHLciDM2Pal &DM2Pal);
	// 
	bool EncodeToBM4(DMGHLiIMGM &imgm, HBITMAP &hbm, const DMGHLciDM2Pal &DM2Pal);
	// 
	bool EncodeToBM8(DMGHLiIMGM &imgm, HBITMAP &hbm, const DMGHLciDM2Pal &DM2Pal);
	// 
	bool EncodeToBM(DMGHLiIMGM &imgm, HBITMAP &hbm, const DMGHLciDM2Pal &DM2Pal);
	// 
	bool CCS4(DMGHLiIMGM &rSrc, DMGHLiIMGM &rDst, const DMGHLciDM2Pal &DM2Pal);
	// 
	bool CCS4(DMGHLiIMGM &rSrc, DMGHLiIMGM &rDst, COLORREF clr[16], const DMGHLciDM2Pal &DM2Pal);
	// 
	bool CCS4(DMGHLiIMGM &rSrc, DMGHLiIMGM &rDst, const BYTE bNewPal[16], const DMGHLciDM2Pal &DM2Pal);
	// 
	bool CCS4(const COLORREF clr[16], DMGHLciPAL &rPal, const DMGHLciDM2Pal &DM2Pal);
	// 
	bool CCS4(const RGBQUAD rq[16], DMGHLciPAL &rPal, const DMGHLciDM2Pal &DM2Pal);

};

#endif // !defined(AFX_DM2IMAGEEXCH_H__B024CC9D_345F_410B_9509_7CEDBA2E245E__INCLUDED_)
