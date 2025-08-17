//------------------------------------------------------------------------------
// SKDOS
//	Specific DOS-renderer for Skullkeep
//------------------------------------------------------------------------------

#ifndef _SKCODE_DOS_H_
#define _SKCODE_DOS_H_

#include <sktypes.h>
#include <skvram.h>
#include <skrender.h>

//..............................................................................

class SkVRAM;

//------------------------------------------------------------------------------

class SkRendererDOS : public SkRendererGeneric
{
private:
//	SkVRAM* xVRAM;

//	UINT iScreenWidth;
//	UINT iScreenHeight;

public:
	SkRendererDOS();
	virtual ~SkRendererDOS();

	UINT Init(SkVRAM* xVRAM);
	UINT ResizeWindow(); // useless
	UINT SetModeVGA();
	UINT SetModeVGA16();

	UINT SetVGAPaletteRGB(X8 *xVGAPalette);
	UINT ResetTextCursor();
	UINT Render();
	UINT Close();

};

//------------------------------------------------------------------------------

#endif // _SKCODE_DOS_H_