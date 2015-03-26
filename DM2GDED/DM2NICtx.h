// DM2NICtx.h: CDM2NICtx クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM2NICTX_H__D13D74F4_6194_487F_8C11_D0BA57A123FD__INCLUDED_)
#define AFX_DM2NICTX_H__D13D74F4_6194_487F_8C11_D0BA57A123FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ExtBM.h"
#include "DMGHLci.h"
#include "DM2PIPDescList.h"

class CDM2NICtx  
{
public:
	// 
	enum {
		psNo,
		psForce,
		psShould,
	};

	// 
	CExtBM bm;
	// 
	DMGHLiIMGM imgm[4];
	// 
	int iStage;
	// 
	CSize size;
	// 
	bool fUse8;
	// 
	bool fForce4;
	// 
	int iColorKey;
	// 
	int iColorKey2;
	// 
	CGDEntry posColorPal;

	// 
	int iColorSrcKey;
	// 
	int iColorSrcKey2;
	// 
	DMGHLciPAL cPal;
	// 
	int ps;

	// 
	bool fKeepPal;

	// 
	DMGHLciDM2Pal DM2Pal;

	// 
	CDM2NICtx()
	{
		iStage = -1;
		fUse8 = false;
		fForce4 = true;
		iColorKey = -1;
		iColorKey2 = -1;

		iColorSrcKey = -1;
		iColorSrcKey2 = -1;
		ps = psNo;

		fKeepPal = true;
	}
	// 
	bool Enter(int iStage);
	// 
	bool DoStep(int iStage);

};

#endif // !defined(AFX_DM2NICTX_H__D13D74F4_6194_487F_8C11_D0BA57A123FD__INCLUDED_)
