// EditPCMCB.h: CEditPCMCB クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITPCMCB_H__6A15BC7F_5F8D_4C68_AD54_6DABF279F682__INCLUDED_)
#define AFX_EDITPCMCB_H__6A15BC7F_5F8D_4C68_AD54_6DABF279F682__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SizeVuff.h"

class CEditPCMCB
{
public:
	// 
	virtual bool CreateRIFFWave(SizeBuff &fv) = 0;
	// 
	virtual bool SetPCM11025(BYTE *pbWav, UINT cbWav) = 0;

};

#endif // !defined(AFX_EDITPCMCB_H__6A15BC7F_5F8D_4C68_AD54_6DABF279F682__INCLUDED_)
