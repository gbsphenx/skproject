
#ifndef _DM2SELCOLORKEYCB_
#define _DM2SELCOLORKEYCB_

#include "DMGHLci.h"

// CDM2SelColorKeyCB

class CDM2SelColorKeyCB : public CComboBox
{
protected:
	// 
	DECLARE_DYNAMIC(CDM2SelColorKeyCB)

protected:
	// 
	COLORREF m_clrs[256];

public:
	CDM2SelColorKeyCB();
	virtual ~CDM2SelColorKeyCB();

	// 
	void Set4(const DMGHLciPAL &a, int iColorSrcKey, const DMGHLciDM2Pal &DM2Pal);
	// 
	void Set8(int iColorSrcKey, const DMGHLciDM2Pal &DM2Pal);
	// 
	void Init();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};

#endif // _DM2SELCOLORKEYCB_
