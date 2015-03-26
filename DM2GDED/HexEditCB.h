// HexEditCB.h: CHexEditCB クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEXEDITCB_H__4E3D0187_D44C_4BB2_B639_4296DCF79B2B__INCLUDED_)
#define AFX_HEXEDITCB_H__4E3D0187_D44C_4BB2_B639_4296DCF79B2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SizeVuff.h"

struct CHexEditCB;

struct CHexEditSink {
	// 
	virtual void OnHexEditApply(CHexEditCB *pSender) = 0;
};

struct CHexEditCB {
	// 
	virtual void SetRawData(LPCVOID pvAny, UINT cbAny) = 0;
	// 
	virtual void GetRawData(SizeBuff &fv) = 0;
	// 
	virtual void SetSink(CHexEditSink *pUniSink) = 0;
};

#endif // !defined(AFX_HEXEDITCB_H__4E3D0187_D44C_4BB2_B639_4296DCF79B2B__INCLUDED_)
