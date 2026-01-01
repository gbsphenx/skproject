//------------------------------------------------------------------------------
// SKRENDERER
//	Basic class for implementing different renderers
//------------------------------------------------------------------------------

#ifndef _SKCODE_RENDERER_H_
#define _SKCODE_RENDERER_H_

#include <sktypes.h>
#include <skvram.h>

//..............................................................................
//------------------------------------------------------------------------------

class SkWinApp;

class SkRendererGeneric
{
protected:
	SkVRAM* xVRAM;

	UINT iScreenWidth;
	UINT iScreenHeight;
public:
	SkWinApp* xMasterWinApp;

public:
	SkRendererGeneric();
	virtual ~SkRendererGeneric();

	
	UINT InitWinApp(SkWinApp* xWinApp);
	virtual UINT Init(SkVRAM* xVRAM);
	virtual UINT SetVGAPaletteRGB(X8 *xVGAPalette);
	virtual UINT Render();
	virtual UINT Close();

	virtual bool ML();

	virtual UINT ResizeWindow();	// depending on screen scale

	virtual UINT StartAudioSample(const char* sSampleName);

	virtual UINT AudioPlaySound(const U8 *xSoundBuffer, U32 iBufferSize, i8 iSoundVolume, U16 iPlaybackFrequency);
	virtual UINT AudioPlayFile(const char* sFilename, i8 iSoundVolume);
	virtual UINT AudioStop();

};

//------------------------------------------------------------------------------

#endif // _SKCODE_RENDERER_H_