#ifndef _STDAFX_H_
#define _STDAFX_H_
#if (_MSC_VER >= 1200) || defined(_USE_MFC80) || defined(_USE_MFC60)
// MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>
#pragma once

//#define WINVER 0x0410
#define DIRECTSOUND_VERSION 0x800

#include <afx.h>
#include <afxwin.h>

#ifdef _USE_MFC80
# include <atlcore.h>
# include <atlcoll.h>
# include <atlstr.h>
# include <atlcom.h>
#else
# include <atlbase.h>
extern CComModule _Module;
# include <atlcom.h>
# define ATLVERIFY VERIFY // use mfc's one (which is ASSERT)
#endif // _USE_MFC80

#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <setjmp.h>
#include <time.h>
#include <direct.h>
#include <io.h>
#include <fcntl.h>
#include <mmsystem.h>
#include <io.h>
#include <sys/stat.h>
#include <stack>
#include <afxcmn.h>

// You require: Microsoft DirectX 9.0 SDK Update (October 2004)
#include <dsound.h>

#endif // _USE_MFC80 || _USE_MFC60
#endif // _STDAFX_H_
