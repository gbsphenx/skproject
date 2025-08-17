#if (_MSC_VER >= 1200) || defined(_USE_MFC80) || defined(_USE_MFC60)
#include <stdafx.h>	// Required for MSVC6 compilation
//#include <windows.h>
#include <afxwin.h>
#include <objbase.h>
#pragma comment(lib, "ole32.lib")
#else
	#define __NO_MFC__
#endif

#include <skver.h>
#include <skmfc.h>
#include <skwinmfc.h>
#include <skmfcres.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <skparam.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

SkRendererMFC::SkRendererMFC()
{
	;
}

SkRendererMFC::~SkRendererMFC()
{
	;
}

UINT SkRendererMFC::Init(SkVRAM* xVRAM)
{
	char sMFCWindowName[128];
	
	SkRendererGeneric::Init(xVRAM);

	memset(sMFCWindowName, 0, 128);
	sprintf(sMFCWindowName, "%s <%s> [MFC]", __SKWIN_PROGNAME__ __SKULL_SUBVER__, __SKWIN_SYSTEM__);

	this->xVRAM = xVRAM;
	iScreenWidth = 320;
	iScreenHeight = 200;

#if !defined (__NO_MFC__)
#if (_MSC_VER >= 1200) || defined(_USE_MFC80) || defined(_USE_MFC60)
	HRESULT hr;
	if (AfxWinInit(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOW)) {
		if (SUCCEEDED(hr = CoInitialize(NULL))) {
			int iScreenScale = SkCodeParam::iVideoScale;
			CRect rc(0, 0, 320 * iScreenScale, 200 * iScreenScale);
			AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, true, 0);
			rc.OffsetRect(-rc.left, -rc.top);
			printf("Initializing windows screen (%d,%d)\n", -rc.left, -rc.top);
			skwin.SetVRAM(xVRAM);
			skwin.sxfact = iScreenScale;
			if (skwin.CreateEx(0, AfxRegisterWndClass(0, AfxGetApp()->LoadCursor(IDC_POINTER), (HBRUSH)GetStockObject(BLACK_BRUSH), NULL), sMFCWindowName, WS_OVERLAPPEDWINDOW, rc, NULL, 0, NULL)) {
				skwin.ShowWindow(SW_SHOWDEFAULT);
			}
		}
	}
#endif // _MSC_VER / _USE_MFC80 / _USE_MFC60
#endif // not __NO_MFC__
	return 0;
}

UINT SkRendererMFC::ResizeWindow()
{
	return 0;
}

UINT SkRendererMFC::SetVGAPaletteRGB(X8 *xVGAPalette)
{
#if !defined (__NO_MFC__)
	skwin.SetVGAPaletteRGB(xVGAPalette);
#endif // not __NO_MFC__
	return 0;
}



UINT SkRendererMFC::Render()
{
#if !defined (__NO_MFC__)
	SetVGAPaletteRGB(xVRAM->GET_PALETTE());
	skwin.RedrawWindow();	
#endif // not __NO_MFC__
	return 0;
}

UINT SkRendererMFC::Close()
{
#if !defined (__NO_MFC__)
	skwin.DestroyWindow();
#endif // not __NO_MFC__
	return 0;
}
