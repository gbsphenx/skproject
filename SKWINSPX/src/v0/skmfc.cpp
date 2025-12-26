#if (_MSC_VER >= 1200) || defined(_USE_MFC80) || defined(_USE_MFC60)
	#include <stdafx.h>	// Required for MSVC6 compilation
	//#include <windows.h>
	#include <afxwin.h>
	#include <objbase.h>
	#pragma comment(lib, "ole32.lib")
#elif !defined (__NO_MFC__)
	#define __NO_MFC__
#endif

#include <skver.h>
#include <skmfc.h>
#include <skwinmfc.h>
#include <skparam.h>

#include <skmfcres.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

SkRendererMFC::SkRendererMFC()
{
#if !defined (__NO_MFC__)
#if (_MSC_VER >= 1200) || defined(_USE_MFC80) || defined(_USE_MFC60)
	skwin.xRendererMFC = this;
#endif // _MSC_VER / _USE_MFC80 / _USE_MFC60
#endif // not __NO_MFC__
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
			CRect rc(0, 0, 320 * iScreenScale, 200 * iScreenScale * SkCodeParam::fVideoYScale);
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
	if (((CSkWinMFC*)&skwin) != NULL)	// crash here when exiting directly from window
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


bool SkRendererMFC::ML()
{
#if !defined(__NO_MFC__) && ((_MSC_VER >= 1200) || defined(_USE_MFC80) || defined(_USE_MFC60))
	skwin.ML();
#endif //
	return false;
}

//..............................................................................

UINT SkRendererMFC::AudioPlaySound(const U8 *xSoundBuffer, U32 iBufferSize, i8 iSoundVolume, U16 iPlaybackFrequency)
{
#if !defined(__NO_MFC__) && ((_MSC_VER >= 1200) || defined(_USE_MFC80) || defined(_USE_MFC60))
	U32 nSamples = iBufferSize;
#pragma pack(push, 1)
	struct {
		DWORD riff;
		DWORD n1;
		DWORD wave;
		DWORD fmt;
		DWORD n2;

		WORD wFormatTag, nChannels;
		DWORD nSamplesPerSec, nAvgBytesPerSec;
		WORD nBlockAlign, wBitsPerSample, cbSize;

		DWORD fact;
		DWORD n3;
		DWORD n4;

		DWORD data;
		DWORD n5;
	}	sPackedRiffWav = {
		*(DWORD *)"RIFF",
		4+(8)+(2+2+4+4+2+2+2)+(8)+(4)+(8)+(nSamples),
		*(DWORD *)"WAVE",
		*(DWORD *)"fmt ",
		(2+2+4+4+2+2+2),
		1,
		1,
		iPlaybackFrequency,
		iPlaybackFrequency,
		1,
		8,
		0,
		*(DWORD *)"fact",
		4,
		nSamples,
		*(DWORD *)"data",
		nSamples,
	};
#pragma pack(pop)

	static int cnt = 0;
	static BYTE buffInt[65536];	// because sound data from GDAT is 64K max

	// MFC stuff
	if (cnt != 0) PlaySound(NULL, NULL, SND_PURGE);
	CMemFile f;
	f.Write(&sPackedRiffWav, sizeof(sPackedRiffWav));
	i8 c;
	for (U32 x = 0; x < iBufferSize; x++) {
		c = xSoundBuffer[x];
		c = max(-127, min(127, i16(c) * iSoundVolume / 16));
		c += (i8)0x80;
		f.Write(&c, 1);
	}
	f.SeekToBegin();
	f.Read(buffInt, 65536);
	PlaySound((LPCSTR)buffInt, NULL, SND_ASYNC|SND_MEMORY);	// 
	cnt |= 1;
#endif //
	return 0;
}

//..............................................................................