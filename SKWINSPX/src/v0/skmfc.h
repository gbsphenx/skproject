
#ifndef _SKCODE_MFC_H_
#define _SKCODE_MFC_H_

#include <skvram.h>
#include <skrender.h>

//class SkWinCore;

#include <skwinmfc.h>

//..............................................................................
//------------------------------------------------------------------------------

class SkRendererMFC : public SkRendererGeneric
{
private:
#if !defined(__NO_MFC__) && ((_MSC_VER >= 1200) || defined(_USE_MFC80) || defined(_USE_MFC60))
	CSkWinMFC skwin;
#endif


public:
	SkRendererMFC();
	virtual ~SkRendererMFC();

	UINT Init(SkVRAM* xVRAM);
	UINT ResizeWindow();
	UINT SetModeVGA();
	UINT SetModeVGA16();

	bool ML();

	UINT SetVGAPaletteRGB(X8 *xVGAPalette);
	UINT ResetTextCursor();
	UINT Render();
	UINT Close();

	UINT AudioPlaySound(const U8 *xSoundBuffer, U32 iBufferSize, i8 iSoundVolume, U16 iPlaybackFrequency);

};

//------------------------------------------------------------------------------

#endif // _SKCODE_MFC_H_
