
#ifndef _SKCODE_SDL2_H_
#define _SKCODE_SDL2_H_

#include <sktypes.h>
#include <skrender.h>

//..............................................................................

#if defined (__LINUX__) && !defined (__NO_SDL__)
	#include <SDL2/SDL.h>
#elif defined (__DJGPP__)	// not using SDL on DOS
	#define __NO_SDL__
#elif !defined (__DJGPP__) && !defined (__NO_SDL__)
	#include <SDL.h>
#endif // __LINUX__

#if defined (__NO_SDL__)
	typedef void SDL_Window;
	typedef void SDL_Renderer;
	typedef void SDL_Texture;
	typedef X8 SDL_Color;
#endif // __NO_SDL__

//..............................................................................

class SkVRAM;
class SkWinApp;

int SKSDL_INIT_TEST(SkWinApp* skWinApplication);


class SkRendererSDL : public SkRendererGeneric
{
private:
//	SkVRAM* xVRAM;

//	UINT iScreenWidth;
//	UINT iScreenHeight;
	SDL_Window* sdlWindow;
	SDL_Renderer* sdlRenderer;
	SDL_Texture* sdlTexture;
	SDL_Color sdlPalette[256];

	uint32_t* xRGBBuffer;

public:
	SkRendererSDL();
	virtual ~SkRendererSDL();

	UINT Init(SkVRAM* xVRAM);
	UINT ResizeWindow();
	UINT SetVGAPaletteRGB(X8 *xVGAPalette);
	UINT Render();
	UINT Close();

private:
	UINT ConvertVRAMToRGB();
};


#endif // _SKCODE_SDL2_H_ 