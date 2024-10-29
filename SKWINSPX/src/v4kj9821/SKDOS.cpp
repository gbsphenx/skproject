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
//    unsigned char* video_mem = (unsigned char*)0xA0000;
    unsigned char *video_mem = (unsigned char *)0xA0000 + __djgpp_conventional_base;
	int ix = 0;
	int iy = 0;
    for (iy = 0; iy < SCREEN_HEIGHT; ++iy) {
        for (ix = 0; ix < SCREEN_WIDTH; ++ix) {
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
	usleep(millisecs * 1000);
#endif
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
	printf("\nDungeon Master II PC-9821/PC-DOS\n");
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

