// DM2NICtx.cpp: CDM2NICtx クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2NICtx.h"
#include "DM2ImageExch.h"
#include "DM2Palo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

bool CDM2NICtx::Enter(int iStage)
{
	for (; this->iStage < iStage; this->iStage++) {
		if (!DoStep(this->iStage +1)) return false;
	}

	this->iStage = iStage;
	return true;
}

bool CDM2NICtx::DoStep(int iStage)
{
	switch (iStage) {
	case 0:
		{
			if (fForce4) {
				DMGHLiIMGM img4;
				if (!CDM2ImageConv().EncodeBMTo4(bm, img4, DM2Pal))
					return false;

				switch (ps) {
				case psNo:
				case psShould:
					{
						imgm[0] = img4;
						break;
					}
				case psForce:
					{
						if (!CDM2ImageConv().CCS4(img4, imgm[0], cPal.p.f.index, DM2Pal))
							return false;
					}
				}
			} else {
				if (!CDM2ImageConv().EncodeBMTo8(bm, imgm[0], DM2Pal))
					return false;
			}

			this->iStage = iStage;
			size.cx = imgm[0].cx;
			size.cy = imgm[0].cy;
			return true;
		}
	case 1:
		{
			if (fUse8) {
				imgm[1] = imgm[0];
			} else {
				VERIFY(CDM2ImageConv().CCS4(imgm[0], imgm[1], DM2Pal));
			}
			return true;
		}
	case 2:
		{
			imgm[2] = imgm[1];
			if (0 <= iColorKey) {
				BYTE k = iColorKey, ka = iColorSrcKey;
				BYTE kaa = (ka & 8) ? (ka -1) : (ka +1);
				if (imgm[2].fIs8bpp) {
					for (UINT y = 0; y < imgm[2].cy; y++) {
						for (UINT x = 0; x < imgm[2].cx; x++) {
							BYTE c = imgm[2].GetPixel(x, y);
							if (c == k) {
								c = ka;
							} else if (c == ka) {
								c = kaa;
							} else {
								continue;
							}
							imgm[2].SetPixel(x, y, c);
						}
					}
				} else {
					for (UINT y = 0; y < imgm[2].cy; y++) {
						for (UINT x = 0; x < imgm[2].cx; x++) {
							BYTE c = imgm[2].GetPixel(x, y);
							if (c == k) {
								c = ka;
							} else if (c == ka) {
								c = k;
							} else {
								continue;
							}
							imgm[2].SetPixel(x, y, c);
						}
					}

					swap(imgm[2].cPal.p.f.index[k], imgm[2].cPal.p.f.index[ka]);
					swap(imgm[2].cPal.p.f.color[k], imgm[2].cPal.p.f.color[ka]);

					//imgm[2].cPal.p.f.index[ka] = 0;
					//imgm[2].cPal.p.f.color[ka] = 0;
				}
			}
			return true;
		}
	case 3:
		{
			if (fKeepPal && ps == psShould) {
				VERIFY(CDM2ImageConv().CCS4(imgm[2], imgm[3], cPal.p.f.index, DM2Pal));
			} else {
				imgm[3] = imgm[2];
			}
			return true;
		}
	}
	return false;
}
