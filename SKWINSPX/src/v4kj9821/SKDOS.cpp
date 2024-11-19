//--- SkDOS -------------------------------------
//SPX: SkDOS replaces SkMFC when compiled with DOS/DJGPP

#include <StdAfx.h>
#include <DME.h>	// defining types for skwin
#include <SKDOS.h>
#include <SKGlobal.h>

#ifndef __DJGPP__
#define __djgpp_conventional_base 0
#endif

#include <stdlib.h>
#include <time.h>

#ifdef __DJGPP__

#include <unistd.h>

#undef inportb
#undef outportb

#include <dos.h>
#include <go32.h>
#include <sys/nearptr.h>

#endif // __DJGPP__


#define VIDEO_INT 0x10
#define VGA_256_COLOR_MODE 0x13
#define TEXT_MODE 0x03
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

CSkWinDOS::CSkWinDOS()
{
	sxfact = 1;
	sblast = 1;
	sbdsbf = 0;
	sbdsaf = 0;
	spfact = 0;	// for DOS, put it directly to MAX, even at "normal (2)" it is way to slow
	lang = _GDAT_LANG_ENGLISH_;
	dung = _OPTION_DUNGEON_DM2_SK_;
	sksave = -1;
	newgame = 0;
}

CSkWinDOS::~CSkWinDOS()
{
	;
}


// Fake methods

void CSkWinDOS::OnPaint()
{
	;
}

bool CSkWinDOS::ML() {
	return true;
}

void CSkWinDOS::ProcessArgs(int argc, char** argv)
{
	unsigned int iArgIndex = 0;

	sCustomGraphicsDatFilename = NULL;
	sCustomDungeonDatFilename = NULL;
	sCustomDataFolder = NULL;

	while (iArgIndex < argc)
	{
		if (argv[iArgIndex] == NULL)
			break;

		if (!strcmp(argv[iArgIndex], "-en"))
			lang = _GDAT_LANG_ENGLISH_;	// english
		else if (!strcmp(argv[iArgIndex], "-jp"))
			lang = _GDAT_LANG_JAPANESE_;	// japanese
		else if (!strcmp(argv[iArgIndex], "-de"))
			lang = _GDAT_LANG_GERMAN_;	// german
		else if (!strcmp(argv[iArgIndex], "-fr"))
			lang = _GDAT_LANG_FRENCH_;	// french
		else if (!strcmp(argv[iArgIndex], "-es"))
			lang = _GDAT_LANG_SPANISH_;	// spanish
		else if (!strcmp(argv[iArgIndex], "-it"))
			lang = _GDAT_LANG_ITALIAN_;	// italian

		else if (!strcmp(argv[iArgIndex], "-r0"))	// resume game sksave0
		{
			sksave = 0;
			newgame = 2;
		}
		else if (!strcmp(argv[iArgIndex], "-resume"))	// go to resume screen
			newgame = 2;
		else if (!strcmp(argv[iArgIndex], "-new"))	// directly start new game
			newgame = 1;


		else if (!strcmp(argv[iArgIndex], "-dm1_kid"))
			dung = _OPTION_DUNGEON_DM1_KID_;
		else if (!strcmp(argv[iArgIndex], "-dm1_dm"))
			dung = _OPTION_DUNGEON_DM1_DM_;
		else if (!strcmp(argv[iArgIndex], "-dm1_csb"))
			dung = _OPTION_DUNGEON_DM1_CSB_;
		else if (!strcmp(argv[iArgIndex], "-dm1_tq"))
			dung = _OPTION_DUNGEON_DM1_TQ_;
		else if (!strcmp(argv[iArgIndex], "-dm2_dm"))
			dung = _OPTION_DUNGEON_DM2_DM_;
		else if (!strcmp(argv[iArgIndex], "-dm2_csb"))
			dung = _OPTION_DUNGEON_DM2_CSB_;
		else if (!strcmp(argv[iArgIndex], "-dm2_tq"))
			dung = _OPTION_DUNGEON_DM2_TQ_;
		else if (!strcmp(argv[iArgIndex], "-dm2_beta"))
			dung = _OPTION_DUNGEON_DM2_SK_BETA_;
		else if (!strcmp(argv[iArgIndex], "-dm2_demo"))
			dung = _OPTION_DUNGEON_DM2_SK_DEMO_;
		else if (!strcmp(argv[iArgIndex], "-dm2_sk"))
			dung = _OPTION_DUNGEON_DM2_SK_;
		else if (!strcmp(argv[iArgIndex], "-dm2_skx"))
			dung = _OPTION_DUNGEON_DM2_SK_EXT_;

#ifndef __SKWIN_PUBLIC_VERSION__
		else if (!strcmp(argv[iArgIndex], "-bwy_bw"))
			dung = _OPTION_DUNGEON_BWY_BW_;
		else if (!strcmp(argv[iArgIndex], "-eob_eob1"))
			dung = _OPTION_DUNGEON_EOB_EOB1_;
		else if (!strcmp(argv[iArgIndex], "-dmx"))
			dung = _OPTION_DUNGEON_DMX_;
#endif

		else if (!strcmp(argv[iArgIndex], "-fhz6"))
			freq = 6;
		else if (!strcmp(argv[iArgIndex], "-fhz11"))
			freq = 11;

		// specify any graphics.dat filename
		else if (!strcmp(argv[iArgIndex], "-gdat"))
		{
			iArgIndex++;
			if (argv[iArgIndex] != NULL)
			{
				unsigned int iArgLength = 0;
				iArgLength = strlen(argv[iArgIndex]);
				if (iArgLength > 0)
				{
					sCustomGraphicsDatFilename = (char*) calloc(iArgLength+1, sizeof(char));
					if (sCustomGraphicsDatFilename != NULL)
						strcpy(sCustomGraphicsDatFilename, argv[iArgIndex]);
				}
			}
		}
		// specify any dungeon.dat filename
		else if (!strcmp(argv[iArgIndex], "-dungeon"))
		{
			iArgIndex++;
			if (argv[iArgIndex] != NULL)
			{
				unsigned int iArgLength = 0;
				iArgLength = strlen(argv[iArgIndex]);
				if (iArgLength > 0)
				{
					sCustomDungeonDatFilename = (char*) calloc(iArgLength+1, sizeof(char));
					if (sCustomDungeonDatFilename != NULL)
						strcpy(sCustomDungeonDatFilename, argv[iArgIndex]);
				}
			}
		}

		// specify any data folder (by default, dungeon.dat and graphics.dat will be taken from that folder)
		else if (!strcmp(argv[iArgIndex], "-data"))
		{
			iArgIndex++;
			if (argv[iArgIndex] != NULL)
			{
				unsigned int iArgLength = 0;
				iArgLength = strlen(argv[iArgIndex]);
				if (iArgLength > 0)
				{
					sCustomDataFolder = (char*) calloc(iArgLength+1, sizeof(char));
					if (sCustomDataFolder != NULL)
						strcpy(sCustomDataFolder, argv[iArgIndex]);
				}
			}
		}

		iArgIndex++;
	}

}

unsigned char CSkWinDOS::GetLang()
{
	return (unsigned char)lang;
}

void CSkWinDOS::SndPlayLo(const U8 *buff, U32 buffSize, i8 dX, i8 dY)
{
	;
}

void CSkWinDOS::SndPlayHi(const U8 *buff, U32 buffSize, i8 vol)
{
	;
}

void CSkWinDOS::SndPlayFile(const char* sFilename, i8 vol)
{
	;
}


void CSkWinDOS::SndSetFrequency(U16 iSoundPlaybackFrequency)
{
	;
}

void CSkWinDOS::ShowMessage(const char *psz) {
	;
}

void CSkWinDOS::UpdateRect(i16 x, i16 y, i16 cx, i16 cy)
{
    unsigned char *video_mem = (unsigned char *)0xA0000 + __djgpp_conventional_base;
	int ix = 0;
	int iy = 0;
    for (iy = 0; iy < SCREEN_HEIGHT; iy++) {
        for (ix = 0; ix < SCREEN_WIDTH; ix++) {
            video_mem[iy * SCREEN_WIDTH + ix] = vram[iy * SCREEN_WIDTH + ix];  // Copy pixel data
			//video_mem[iy * SCREEN_WIDTH + ix] = rand()%256; // random static (video test)
        }
    }
    //printf("UpdateRect done.");
}

bool CSkWinDOS::IsAvail() 
{
	return true;
}

U32 CSkWinDOS::GetTickCount() 
{
    return (U32)(clock() * 1000 / CLOCKS_PER_SEC);
}
void CSkWinDOS::Sleep(U32 millisecs)
{
#ifdef __DJGPP__
	usleep(millisecs); // * 1000
#endif
}

#define KEYBOARD_INT 0x16

void CSkWinDOS::PollKeyboard()
{
#ifdef __DJGPP__
    union REGS regs;
    static unsigned char lastScanCode = 0;
    static int isKeyPressed = 0;

    // Call BIOS interrupt to check keyboard status
    regs.h.ah = 0x01;
    int86(0x16, &regs, &regs);

	isKeyPressed = regs.x.cflag & 1;

    regs.h.ah = 0x00; // current key
    int86(0x16, &regs, &regs);

    unsigned char scanCode = regs.h.ch; // High byte contains the scan code
    unsigned char asciiCode = regs.h.cl; // Low byte contains the ASCII code
	unsigned char ah = regs.h.ah; // Low byte contains the ASCII code
	unsigned char al = regs.h.al; // Low byte contains the ASCII code
	//printf("Scan = %02X %02X %02X %02X, pressed = %d\n", scanCode, asciiCode, ah, al, isKeyPressed);
	scanCode = ah;
    if (scanCode != 0) // A key is being pressed
    {
        if (!isKeyPressed || scanCode != lastScanCode)
        {
       //     printf("Key pressed: ASCII=0x%02X, ScanCode=0x%02X\n", asciiCode, scanCode);
        }
        isKeyPressed = 1;
        lastScanCode = scanCode;

		processKinput(scanCode, false);
    }
    else if (isKeyPressed) // No key is being pressed now
    {
        //printf("Key released: ScanCode=0x%02X\n", lastScanCode);
        isKeyPressed = 0;
        lastScanCode = 0;
    }
#endif // __DJGPP__


}

void CSkWinDOS::processKinput(unsigned char nChar, bool press)
{
	CSkKinput *p = allocKinput();
	//printf("Kinput p = %08x\n", (void*)p);
	if (p != NULL) {
		unsigned char v = 0;
		switch (nChar) {
		case DK_ESCAPE: v = 1; break;
		case DK_1: v = 2; break;
		case DK_2: v = 3; break;
		case DK_3: v = 4; break;
		case DK_4: v = 5; break;
		case DK_5: v = 6; break;
		case DK_6: v = 7; break;
		case DK_7: v = 8; break;
		case DK_8: v = 9; break;
		case DK_9: v = 10; break;
		case DK_0: v = 11; break;

		case DK_BACKSPACE: v = 14; break;
		case DK_TAB: v = 15; break;

		case DK_ENTER: v = 28; break;

		case DK_NUMPAD_7: v = 71; break;
		case DK_NUMPAD_8: v = 72; break;
		case DK_NUMPAD_9: v = 73; break;
		case DK_NUMPAD_4: v = 75; break;
		case DK_NUMPAD_5: v = 76; break;
		case DK_NUMPAD_6: v = 77; break;
		case DK_NUMPAD_1: v = 79; break;
		case DK_NUMPAD_2: v = 80; break;
		case DK_NUMPAD_3: v = 81; break;

		case DK_Q: v = 16; break;
		case DK_W: v = 17; break;
		case DK_E: v = 18; break;
		case DK_R: v = 19; break;
		case DK_T: v = 20; break;
		case DK_Y: v = 21; break;
		case DK_U: v = 22; break;
		case DK_I: v = 23; break;
		case DK_O: v = 24; break;
		case DK_P: v = 25; break;

		case DK_A: v = 30; break;
		case DK_S: v = 31; break;
		case DK_D: v = 32; break;
		case DK_F: v = 33; break;
		case DK_G: v = 34; break;
		case DK_H: v = 35; break;
		case DK_J: v = 36; break;
		case DK_K: v = 37; break;
		case DK_L: v = 38; break;

		case DK_Z: v = 44; break;
		case DK_X: v = 45; break;
		case DK_C: v = 46; break;
		case DK_V: v = 47; break;
		case DK_B: v = 48; break;
		case DK_N: v = 49; break;
		case DK_M: v = 50; break;

		case DK_SPACE: v = 57; break;
		}
		p->raw = (press) ? (v) : (v | 0x80);
	}
}



void CSkWinDOS::GetMousePosButtons(U16 *x, U16 *y, U16 *buttons) 
{
#ifdef __DJGPP__	
    union REGS regs;
    regs.x.ax = 0x0003;  // Get mouse position and button status
    int86(0x33, &regs, &regs);
    *x = regs.x.cx;  // X-coordinate of the mouse
    *y = regs.x.dx;  // Y-coordinate of the mouse
    *buttons = regs.x.bx;  // Mouse button status
    //printf("GetMousePosButtons (%d,%d,%d)\n", *x, *y, *buttons);
#endif // __DJGPP__
}

#ifdef __DJGPP__
void init_mouse() {
    union REGS regs;
    regs.x.ax = 0x0000;  // Initialize the mouse driver
    int86(0x33, &regs, &regs);

    if (regs.x.ax == 0) {
        printf("Mouse not installed\n");
    } else {
        printf("Mouse is installed with %d buttons\n", regs.x.bx);
    }
}
#endif // __DJGPP__

void CSkWinDOS::VersionMessage()
{
	printf("\nDungeon Master II PC-9821/PC-DOS for DOS\n");
	printf("Unofficial Port by Kentaro-k.21 + Sphenx (%s - build %s).\n", strVersionNumber, __SKWIN_RELEASE_DATE__);
}


void CSkWinDOS::StartMessage()
{
	VersionMessage();
}

void CSkWinDOS::InitVideo()
{
#ifdef __DJGPP__
    union REGS regs;

    init_mouse();

    // Enable access to VGA memory
    if (__djgpp_nearptr_enable() == 0) {
        return; // Failed to enable access to VGA memory
    }

    regs.h.ah = 0x00; // Function 0: Set Video Mode
    regs.h.al = VGA_256_COLOR_MODE;
    int86(VIDEO_INT, &regs, &regs);

#endif // __DJGPP__
}


void CSkWinDOS::ResetVideo()
{
#ifdef __DJGPP__
	union REGS regs;
    regs.h.ah = 0x00;  // Function: Set Video Mode
    regs.h.al = 0x03;  // Mode: 3 (80x25 text mode)
    int86(VIDEO_INT, &regs, &regs);  // Call BIOS interrupt 0x10
#endif // __DJGPP__
}

void CSkWinDOS::ExitMessage()
{
	VersionMessage();
	printf("\nbye !\n");
}

