// RCT2.h: CRCT2 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCT2_H__9DD492B0_AC2C_4B4A_8A60_C01130B5356D__INCLUDED_)
#define AFX_RCT2_H__9DD492B0_AC2C_4B4A_8A60_C01130B5356D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRCT2 : public CRectTracker  
{
	// 
	int R2_24(int x)
	{
		return ((x + 12) / 24) * 24;
	}
	// 
	CPoint m_ptOrg;

public:
	// 
	CSize m_sizeNew;
	// 
	CPoint m_ptNew;

	// 
	void Draw(CDC* pDC) const;
	// 
	BOOL SetCursor(CScrollView* pWnd, UINT nHitTest) const;
	// 
	BOOL Track(CScrollView* pWnd, CPoint point, BOOL bAllowInvert, CWnd* pWndClipTo);
	// 
	BOOL TrackHandle(int nHandle, CScrollView* pWnd, CPoint point, CWnd* pWndClipTo);
	// 
	virtual void AdjustRect(int nHandle, LPRECT lpRect);
	// 
	void DrawTrackerRecto(LPCRECT lpRect, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd);

};

#endif // !defined(AFX_RCT2_H__9DD492B0_AC2C_4B4A_8A60_C01130B5356D__INCLUDED_)
