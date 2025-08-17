#ifndef _WINATL_H_
#if (_MSC_VER >= 1200)

#pragma once

//#ifndef WINVER
//#define WINVER 0x0410
//#endif
#include <stdafx.h>

#define DIRECTSOUND_VERSION 0x800

//#include <afx.h>
//#include <afxwin.h>

#ifdef _USE_MFC80
# include <atlcore.h>
# include <atlcoll.h>
# include <atlstr.h>
# include <atlcom.h>
#else
# include <atlbase.h>
extern CComModule _Module;
# include <atlcom.h>
# define ATLVERIFY VERIFY // use mfc's one
#endif // _USE_MFC80

//#include <afxcmn.h>

// You require: Microsoft DirectX 9.0 SDK Update (October 2004)
//#include <dsound.h>

#endif // _MSC_VER
#endif // _WINATL_H_