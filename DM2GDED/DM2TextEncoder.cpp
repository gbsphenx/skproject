// DM2TextEncoder.cpp: CDM2TextEncoder クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2TextEncoder.h"
#include "DM2TextEncImpl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 

namespace
{

CDM2TextEncoderEn g_textEncEn(false);
CDM2TextEncoderEn g_textEncEnPlain(true);
CDM2TextEncoderJa g_textEncJa;
CDM2TextEncoderJa_PSV g_textEncJa2;

};

CDM2TextEncoder *GetDM2TextEncoder(int nDM2TET)
{
	switch (nDM2TET) {
	case DM2TET_EN:
		return &g_textEncEn;
	case DM2TET_JA1:
		return &g_textEncJa;
	case DM2TET_JA2:
		return &g_textEncJa2;
	}
	return &g_textEncEnPlain;
}
