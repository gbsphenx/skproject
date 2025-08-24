//------------------------------------------------------------------------------
// SKRENDERER
//	Basic class for implementing different renderers
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

#include <skver.h>
#include <sktypes.h>

#include <skrender.h>

//..............................................................................

//------------------------------------------------------------------------------


SkRendererGeneric::SkRendererGeneric()
{
	;
}


SkRendererGeneric::~SkRendererGeneric()
{
	;
}


UINT SkRendererGeneric::Init(SkVRAM* xVRAM)
{
	this->xVRAM = xVRAM;
	iScreenWidth = xVRAM->GET_VRAM_WIDTH();
	iScreenHeight = xVRAM->GET_VRAM_HEIGHT();
	return 0;
}


UINT SkRendererGeneric::SetVGAPaletteRGB(X8 *xVGAPalette)
{
	return 0;
}



UINT SkRendererGeneric::Render()
{
	return 0;
}


UINT SkRendererGeneric::Close()
{
	return 0;
}

UINT SkRendererGeneric::ResizeWindow()
{
	return 0;
}

UINT SkRendererGeneric::StartAudioSample(const char* sSampleName)
{
	return 0;
}

//------------------------------------------------------------------------------
