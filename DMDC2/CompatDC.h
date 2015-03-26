// CompatDC.h: CCompatDC クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPATDC_H__DFE1A9A7_9DFC_4344_B603_B202B3433938__INCLUDED_)
#define AFX_COMPATDC_H__DFE1A9A7_9DFC_4344_B603_B202B3433938__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCompatDC : public CDC
{
public:
	// 
	CCompatDC()
	{
		CClientDC dc(NULL);
		if (!CreateCompatibleDC(&dc)) AfxThrowResourceException();
	}
	// 
	CCompatDC(CWnd *pParentWnd)
	{
		CClientDC dc(pParentWnd);
		if (!CreateCompatibleDC(&dc)) AfxThrowResourceException();
	}
	// 
	CCompatDC(CDC *pDC)
	{
		if (!CreateCompatibleDC(pDC)) AfxThrowResourceException();
	}

};

#endif // !defined(AFX_COMPATDC_H__DFE1A9A7_9DFC_4344_B603_B202B3433938__INCLUDED_)
