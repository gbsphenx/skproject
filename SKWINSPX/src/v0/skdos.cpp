//------------------------------------------------------------------------------
// SKDOS
//	Specific DOS-renderer for Skullkeep
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

#include <skdos.h>
#include <skver.h>
#include <sktypes.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//..............................................................................

#define VIDEO_INT					0x10
#define DOS_INT_VGA_16_COLOR_MODE	0x12
#define DOS_INT_VGA_256_COLOR_MODE	0x13
#define DOS_INT_TEXT_MODE			0x03		// 80x25
#define DOS_SCREEN_WIDTH			320
#define DOS_SCREEN_HEIGHT			200


void SKDOS_RESET_VIDEO_MODE();
void SKDOS_SET_VIDEO_MODE_VGA_16();
void SKDOS_SET_VIDEO_MODE_VGA();

void SKDOS_UPDATE_VRAM_STATIC();
void SKDOS_UPDATE_VRAM_TEST_TITLE();

//..............................................................................

#ifndef __DJGPP__
#define __djgpp_conventional_base 0
#endif // not __DJGPP__

//..............................................................................

#ifdef __DJGPP__
#include <unistd.h>

#undef inportb
#undef outportb

#include <dos.h>
#include <go32.h>
#include <sys/nearptr.h>
#include <sys/farptr.h>

#endif // __DJGPP__
//..............................................................................

#define DOS_VGA_SEGMENT	0xA0000

//------------------------------------------------------------------------------

void SKDOS_SET_VIDEO_MODE_VGA_16()
{
#ifdef __DJGPP__
    union REGS regs;
    regs.h.ah = 0x00;							// Function: Set Video Mode
    regs.h.al = DOS_INT_VGA_16_COLOR_MODE;     // Mode 0x12 = 640x480 16-color VGA
    int86(VIDEO_INT, &regs, &regs);
#endif // __DJGPP__
}

void SKDOS_SET_VIDEO_MODE_VGA()
{
#ifdef __DJGPP__
    union REGS regs;

    // Enable access to VGA memory => copy area directly into VRAM for fast display
    if (__djgpp_nearptr_enable() == 0) {
        return; // Failed to enable access to VGA memory
    }

    regs.h.ah = 0x00;					// Function: Set Video Mode
    regs.h.al = DOS_INT_VGA_256_COLOR_MODE; 
    int86(VIDEO_INT, &regs, &regs);
#endif // __DJGPP__
}

void SKDOS_RESET_VIDEO_MODE()
{
#ifdef __DJGPP__
	union REGS regs;
    regs.h.ah = 0x00;					// Function: Set Video Mode
    regs.h.al = DOS_INT_TEXT_MODE;		// Mode: 3 (80x25 text mode)
    int86(VIDEO_INT, &regs, &regs);		// Call BIOS interrupt 0x10
#endif // __DJGPP__
}

//..............................................................................
/*
void SKDOS_UPDATE_VRAM_STATIC()
{
	X8* xVideoData = NULL;
	X8* pVideoRAM = (X8*) (DOS_VGA_SEGMENT + __djgpp_conventional_base);	// DOS VRAM address
	UINT iNbPixels = DOS_SCREEN_HEIGHT * DOS_SCREEN_WIDTH;
	UINT ip = 0;
	UINT ix = 0;
	UINT iy = 0;
	UINT z = 0;

	xVideoData = (X8*) calloc(320*200, sizeof(X8));

    // Fill screen with a gradient
    for (int y = 0; y < 200; y++) {
		z = y * 320;
        for (int x = 0; x < 320; x++) {
			xVideoData[z] = (X8)((x + y) % 256);
			z++;
        }
    }

	memcpy(pVideoRAM, xVideoData, 320*200);	// this works when __djgpp_nearptr_enable is enabled

	free(xVideoData);
}
*/
void SKDOS_SET_VGA_PALETTE(X8 *xVGAPalette) {
#ifdef __DJGPP__
    outp(0x3C8, 0);  // Start writing at color index 0

    for (int i = 0; i < 256; i++) {
        outp(0x3C9, xVGAPalette[i * 4 + 2] >> 2);  // Red
        outp(0x3C9, xVGAPalette[i * 4 + 1] >> 2);  // Green
        outp(0x3C9, xVGAPalette[i * 4 + 0] >> 2);  // Blue
    }
#endif // __DJGPP__
}

/*
void SKDOS_UPDATE_VRAM_TEST_TITLE()
{
	X8* xVideoData = NULL;
	X8* pVideoRAM = (X8*) (DOS_VGA_SEGMENT + __djgpp_conventional_base);	// DOS VRAM address
	UINT iNbPixels = DOS_SCREEN_HEIGHT * DOS_SCREEN_WIDTH;
	X8 xPalette[4*256];

    {
		FILE *file = fopen("dm2sk.raw", "rb");
		if (!file) {
			perror("Error opening file");
			return;
		}

		fseek(file, 0x32, SEEK_SET);

		// Read palette (256 x RGB)
		if (fread(xPalette, 1, 4*256, file) != (4*256)) {
			fprintf(stderr, "Error reading palette data.\n");
			fclose(file);
			return;
		}
		SKDOS_SET_VGA_PALETTE(xPalette);

		xVideoData = (X8*) calloc(320*200, sizeof(X8));

		if (fread(xVideoData, 1, 320 * 175, file) != (320 * 175)) {
			fprintf(stderr, "Error reading image data.\n");
			fclose(file);
			return;
		}
	}

	memcpy(pVideoRAM, xVideoData, 320*200);

	free(xVideoData);
}
*/
//------------------------------------------------------------------------------


SkRendererDOS::SkRendererDOS()
{
	;
}


SkRendererDOS::~SkRendererDOS()
{
	;
}


UINT SkRendererDOS::Init(SkVRAM* xVRAM)
{
	SkRendererGeneric::Init(xVRAM);
	SetModeVGA();
	this->xVRAM = xVRAM;
//	iScreenWidth = 320;
//	iScreenHeight = 200;
//	ATLASSERT(iScreenWidth == 320);
//	ATLASSERT(iScreenHeight == 200);
	return 0;
}

UINT SkRendererDOS::ResizeWindow()
{
	return 0;
}

UINT SkRendererDOS::SetModeVGA()
{
	SKDOS_SET_VIDEO_MODE_VGA();
	return 0;
}

UINT SkRendererDOS::SetModeVGA16()
{
	SKDOS_SET_VIDEO_MODE_VGA_16();
	return 0;
}

UINT SkRendererDOS::SetVGAPaletteRGB(X8 *xVGAPalette)
{
#ifdef __DJGPP__
    outp(0x3C8, 0);  // Start writing at color index 0

    for (int i = 0; i < 256; i++) {
        outp(0x3C9, xVGAPalette[i * 4 + 0] >> 2);  // Red
        outp(0x3C9, xVGAPalette[i * 4 + 1] >> 2);  // Green
        outp(0x3C9, xVGAPalette[i * 4 + 2] >> 2);  // Blue
    }
#endif // __DJGPP__
	return 0;
}


UINT SkRendererDOS::ResetTextCursor() {
#ifdef __DJGPP__
    union REGS regs;
    regs.h.ah = 0x02;  // Set cursor position
    regs.h.bh = 0x00;  // Page number
    regs.h.dh = 0x00;  // Row (Y)
    regs.h.dl = 0x00;  // Column (X)
    int86(0x10, &regs, &regs);
#endif // __DJGPP__
	return 0;
}


UINT SkRendererDOS::Render()
{
	X8* xVRAMData = NULL;
	X8* pVideoRAM = (X8*) (DOS_VGA_SEGMENT + __djgpp_conventional_base);	// DOS VRAM address

	if (xVRAM != NULL) {
		xVRAMData = xVRAM->GET_VIDEO_ARRAY();
		SetVGAPaletteRGB(xVRAM->GET_PALETTE());
		memcpy(pVideoRAM, xVRAMData, 320*200);
	}
	ResetTextCursor();
	return 0;
}


UINT SkRendererDOS::Close()
{
	SKDOS_RESET_VIDEO_MODE();
	return 0;
}


//------------------------------------------------------------------------------
