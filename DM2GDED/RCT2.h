// RCT2.h: CRCT2 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCT2_H__36BE0ABD_9F69_4523_9348_4C6FECE5BB1C__INCLUDED_)
#define AFX_RCT2_H__36BE0ABD_9F69_4523_9348_4C6FECE5BB1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRCT2 : public CRectTracker  
{
public:
	// 
	CRCT2() { }
	// 
	void CRCT2::Draw(CDC* pDC) const;

};

#endif // !defined(AFX_RCT2_H__36BE0ABD_9F69_4523_9348_4C6FECE5BB1C__INCLUDED_)
