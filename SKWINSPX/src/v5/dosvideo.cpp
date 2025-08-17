#include <dosvideo.h>

#include <stdio.h>
#include <stdlib.h>
#include <dm2debug.h>

#ifdef __DJGPP__

#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#undef inportb
#undef outportb

#include <dos.h>
#include <go32.h>
#include <sys/nearptr.h>

#endif // __DJGPP__

#define DOS_VIDEO_INT			0x10
#define DOS_VGA_256_COLOR_MODE	0x13
#define DOS_TEXT_MODE			0x03
#define DOS_SCREEN_WIDTH		320
#define DOS_SCREEN_HEIGHT		200


//------------------------------------------------------------------------------

static bool bDOSVideoVGAMode = false;


//------------------------------------------------------------------------------



void DOS_VIDEO_STATIC_TEST()
{
	unsigned char *video_mem = (unsigned char *)0xA0000 + __djgpp_conventional_base;
	int ix = 0;
	int iy = 0;
	int iScreenWidth = DOS_SCREEN_WIDTH;
	int iScreenHeight = DOS_SCREEN_HEIGHT;
//SPX_DEBUG_PRINT("DOS_VIDEO_STATIC_TEST: %d * %d.\n", iScreenWidth, iScreenHeight);
	if (!bDOSVideoVGAMode)
		return;

	for (iy = 0; iy < iScreenHeight; ++iy) {
		for (ix = 0; ix < iScreenWidth; ++ix) {
			video_mem[iy * iScreenWidth + ix] = rand()%128; // random static (video test)
		}
	}
//SPX_DEBUG_PRINT("DOS_VIDEO_STATIC_TEST DONE.\n\n");

}

void DOS_VIDEO_INIT_VGA_MODE()
{
#ifdef __DJGPP__
    union REGS regs;

SPX_DEBUG_PRINT("DOS_VIDEO_INIT_VGA_MODE\n");
    // Enable access to VGA memory
    if (__djgpp_nearptr_enable() == 0) {
SPX_DEBUG_PRINT_WAIT("DOS_VIDEO_INIT_VGA_MODE FAILED\n");
        return; // Failed to enable access to VGA memory
    }

    regs.h.ah = 0x00; // Function 0: Set Video Mode
    regs.h.al = DOS_VGA_256_COLOR_MODE;
    int86(DOS_VIDEO_INT, &regs, &regs);
	bDOSVideoVGAMode = true;
SPX_DEBUG_PRINT("DOS_VIDEO_INIT_VGA_MODE DONE\n");
	DOS_VIDEO_STATIC_TEST();
#endif // __DJGPP__
}


void DOS_VIDEO_RESET_MODE()
{
#ifdef __DJGPP__
	union REGS regs;
    regs.h.ah = 0x00;  // Function: Set Video Mode
    regs.h.al = DOS_TEXT_MODE;  // Mode: 3 (80x25 text mode)
    int86(DOS_VIDEO_INT, &regs, &regs);  // Call BIOS interrupt 0x10
	bDOSVideoVGAMode = false;
#endif // __DJGPP__
}

void DOS_VIDEO_UPDATE_SCREEN(unsigned char* bptr)
{
    unsigned char *video_mem = (unsigned char *)0xA0000 + __djgpp_conventional_base;
	int ix = 0;
	int iy = 0;
	if (!bDOSVideoVGAMode)
		return;
    for (iy = 0; iy < DOS_SCREEN_HEIGHT; ++iy) {
        for (ix = 0; ix < DOS_SCREEN_WIDTH; ++ix) {
            video_mem[iy * DOS_SCREEN_WIDTH + ix] = bptr[iy * DOS_SCREEN_WIDTH + ix];
        }
    }
//	SPX_DEBUG_PRINT("DOS_VIDEO_UPDATE_SCREEN\n");
}

//------------------------------------------------------------------------------
