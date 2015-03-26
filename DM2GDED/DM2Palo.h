
#ifndef _DM2PALO_H_
#define _DM2PALO_H_

class CDM2Palo
{
public:
	// 
	static void GetColor(BYTE x, COLORREF &clr);
	// 
	static void GetColor(BYTE x, RGBQUAD &rq);

	// 
	static inline void Conv(const COLORREF clr, RGBQUAD &rq)
	{
		rq.rgbBlue = GetBValue(clr),
		rq.rgbGreen = GetGValue(clr),
		rq.rgbRed = GetRValue(clr),
		rq.rgbReserved = 0;
	}
	// 
	static inline void Conv(const RGBQUAD rq, COLORREF &clr)
	{
		clr = RGB(rq.rgbRed, rq.rgbGreen, rq.rgbBlue);
	}
};

#endif
