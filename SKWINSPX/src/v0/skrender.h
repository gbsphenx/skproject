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

class SkRendererGeneric
{
protected:
	SkVRAM* xVRAM;

	UINT iScreenWidth;
	UINT iScreenHeight;

public:
	SkRendererGeneric();
	virtual ~SkRendererGeneric();

	virtual UINT Init(SkVRAM* xVRAM);
	virtual UINT SetVGAPaletteRGB(X8 *xVGAPalette);
	virtual UINT Render();
	virtual UINT Close();

	virtual UINT ResizeWindow();	// depending on screen scale

	virtual UINT StartAudioSample(const char* sSampleName);
};

//------------------------------------------------------------------------------

#endif // _SKCODE_RENDERER_H_