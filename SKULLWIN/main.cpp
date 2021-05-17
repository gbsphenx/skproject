#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#pragma warning (disable:4312)

//#include <allegro.h>
#include "allegro5/allegro5.h"

#include "dm2data.h"
#include "startend.h"
#include "c_midi.h"
#include "fileio.h"
#include "c_sound.h"
#include "c_allegro.h"
#include "c_alloc.h"
#include "c_dballoc.h"
#include "dm2global.h"
#include "c_record.h"
#include "c_map.h"
#include "c_creature.h"
#include "c_input.h"
#include "c_image.h"
#include "c_keybd.h"
#include "c_eventqueue.h"
#include "c_mcursor.h"
#include "c_gfx_blit.h"
#include "c_gfx_decode.h"
#include "c_random.h"
#include "c_gfx_pal.h"
#include "c_gfx_str.h"
#include "c_gfx_main.h"
#include "c_xrect.h"
#include "c_hero.h"
#include "c_timer.h"
#include "c_gfx_blit.h"
#include "c_350.h"
#include "ints.h"
#include "c_gdatfile.h"
#include "c_tmouse.h"
#include "c_addon.h"

//Note: void R_1583(i32 eaxl) removed - unreferenced
//Note: R_4A986 exploded, delivers DRVP_screen16ptr
//Note: R_1456 removed, unreferenced
//Note: R_52DC0 removed, obsolete
//Note: bool SKW_476d_04af(i32 eaxl) removed, obsolete
//Note: void R_148D(i16 wdummy) removed, obsolete
// Note: SKW_443c_0889 exploded (T1_execmousefifo + driver_unblockmouseinput)
// Note: function SKW_443c_087c exploded, it's driver_blockmouseinput
// was driver function 4
// unreferenced - calls user interrupt (TODO: lost here, see older files)
// void R_4D44(i16 wa, i16 wb);

// ==================================================================
// state: new driver interface connected - dispatched from memory
//        memory free pool reorganized - first variable: freepoollist_end
//        alive and divides the 'new' memory now to the program
//        all allocation stuff should work now this way.
//        old 'memory' isn't needed at all anymore.

//        All files except dm2bya.cpp (the core todo-file) don't need the emu
//        anymore - but aren't optimized. Also still some special types for
//        local variables needed for the damned Watcom-long-int-handling.
//        They should vanish later.

//        For the data-sequence read dm2data.txt. Those datas aren't
//        created and initialized in original sequence - as it should be -
//        each variable should be independent now.
//        'bigmem', 'pmdata' and 'musicdata' aren't needed
//        anymore, all data are replaced - still, sizes of variables
//        and structures may be not save, and some things may join
//        together to higher structures.
//        There have been some evil dataoffsets that pointed to a structure
//        that wasn't really meant, but in fact an extra offset had to
//        be added to point to the real thing - pretty demotivating.
//        (but that was the same in Atari-DM2 and CSB-source!)
//        (search for EVIL).
//        a few datafiles are there now, didn't hardcode them yet.

//        Multithreading alive. (but - not nice to debug now...)

//        Resuming the game or restarting
//        it from the menu screen will not work yet!!!
//        Have to check the different ends!!!
//        (But reinitializations should work already)

//        zillions of TODO :P

// known bugs: button RESTART cannot work!
//   there are some things that MAY cause troubles,
//   but the current bugfix-list is handled at the moment
//
// DM2_QUEUE_NOISE_GEN1 failed near (oops? TODO)  bug still there?
// some int stop = 1; spent to set breakpoints at critical things
// TODO: win - close button in the menu - screen

// compilerbugs and traps:
//   comparing i16 vs negative i16 - consts - KEEP LONG CONSTANTS AS POSSIBLE
// BAD TRAP! signed 0xffffffff >> 1 = 0xffffffff, but 0xffffffff / 2 = 0
// that one is really a pain!!! one can never be sure...
// ------------------------------------------------------------------

bool gamethread_initialized = false;

i16 test = 0; // TODO DEBUG

void cleanup(void)
{
  if (logfile)
  {
    fflush(logfile);
    fclose(logfile);
    logfile = NULL;
  }

  // shutdown Allegro4
  dm2sound.stop_all_sound();
  // shutdown Allegro5
  alg.dtor();

  for (int i = 0; i < MAX_FILEHANDLES; i++)
    if (filehandle[i] != NULL)
    {
      fclose(filehandle[i]);
      filehandle[i] = NULL;
    }
}

#ifdef THIRDTHREAD // activate for special debugging purposes
void* thread_func2(ALLEGRO_THREAD* thr, void* vparg)
{
  do
  {
    if (gfxsys.bitmapptr)
    {
      if (   gfxsys.bitmapptr[ORIG_SWIDTH * 85 + 212] == 0
          && gfxsys.bitmapptr[ORIG_SWIDTH * 85 + 12] == 0)
        int stop=1;

      if (   gfxsys.bitmapptr[ORIG_SWIDTH * 85 + 212] != 0
          && gfxsys.bitmapptr[ORIG_SWIDTH * 85 + 12] == 0)
        int stop=1;
    }
  } while (!main_exit);

  return NULL;
}
#endif

void* thread_func(ALLEGRO_THREAD* thr, void* vparg)
{
  i16 i = 0;
  do
  {
    try
    {
      alg.restart();

      for (i=0; i<62; i++)
        table1d3d23[i] = _table1d3d23[i];
      for (i=0; i<4; i++)
        v1d67be[i] = _v1d67be[i];
      for (i=0; i<17; i++)
        table1d70b4[i] = _table1d70b4[i];
      for (i=0; i<10; i++)
        table1d3ed5[i] = _table1d3ed5[i];
      for (i=0; i<5; i++)
        table1d674e[i] = _table1d674e[i];
      for (i=0; i<76; i++)
        table1d3ba0[i] = _table1d3ba0[i];
      // dialogbox still missing!

      filedat.init();
      dm2keybd.init();
      randdat.init();
      tmprects.init();
      glblrects.init();
      xrectdat.init();
      strdat.init();
      ddat.init();
      timdat.init();
      init_global_records(); // TODO
      init_global_images(); // TODO
      init_global_buttongroups(); // TODO
      init_mousecursors(); // TODO
      init_bitmaps(); // TODO
      mapdat.init();
      gfxsys.init();
      party.init();
      s350.init();
      gdat.init();
      eventdata.init();
      init_clickrecttable(); // TODO
      eventqueue.init();
      Tmouse.init();
      cqueue.flush();
      init_creatures();

      dm2_dballochandler.DM2_SETUP_MEMORYALLOCATION();
      dm2_dballochandler.DM2_SETUP_DB_ALLOCATION();

      sysrestart = RESTART_NOTHING;

      alg.hide_mouse();

      alg.set_mouse(ORIG_SWIDTH / 2, ORIG_SHEIGHT / 2);

      alg.start_timer();

      gamethread_initialized = true;

      DM2_HIDE_MOUSE();

      DM2_WATCOM_STARTUP();
    }
    catch(unsigned int cn)
    {
      if (   cn == THROW_CLOSEWIN
          || cn == THROW_DMABORT
          || (cn & THROW_SYS_EXIT)
         )
        sysrestart = RESTART_QUIT;
    }
    gamethread_initialized = false;
    dm2_dballochandler.DM2_DTOR_MEMORYALLOCATION();
  } while (sysrestart != RESTART_QUIT);

  dm2sound.stop_all_sound();
  main_exit = true;

  return NULL;
}

int __GlobalOption_DM2Skull_MusicMode = 0;


// SPX: added to handle options from command line
void ProcessArgs(int argc, char** argv)
{
	unsigned int iArgIndex = 0;

	unsigned int lang = 0;
	unsigned int gdat_vers = 0;
	unsigned int dung = 0;
	unsigned int music = 0;

	while (iArgIndex < argc)
	{
		if (argv[iArgIndex] == NULL)
			break;

		if (!strcmp(argv[iArgIndex], "-en"))
			lang = 0x10;	// english
		else if (!strcmp(argv[iArgIndex], "-en"))
			lang = 0x10;	// english
		else if (!strcmp(argv[iArgIndex], "-jp"))
			lang = 0x20;	// japanese
		else if (!strcmp(argv[iArgIndex], "-de"))
			lang = 0x30;	// german
		else if (!strcmp(argv[iArgIndex], "-fr"))
			lang = 0x40;	// french
		else if (!strcmp(argv[iArgIndex], "-es"))
			lang = 0x50;	// spanish
		else if (!strcmp(argv[iArgIndex], "-it"))
			lang = 0x60;	// italian

		else if (!strcmp(argv[iArgIndex], "-vx"))
			gdat_vers = 0;
		else if (!strcmp(argv[iArgIndex], "-v1"))
			gdat_vers = 1;
		else if (!strcmp(argv[iArgIndex], "-v2"))
			gdat_vers = 2;
		else if (!strcmp(argv[iArgIndex], "-v3"))
			gdat_vers = 3;
		else if (!strcmp(argv[iArgIndex], "-v4"))
			gdat_vers = 4;
		else if (!strcmp(argv[iArgIndex], "-v5"))
			gdat_vers = 5;
		else if (!strcmp(argv[iArgIndex], "-cartoon"))
			gdat_vers = 6;	// v5 + cartoon

		else if (!strcmp(argv[iArgIndex], "-dm1"))
			dung = 1;
		else if (!strcmp(argv[iArgIndex], "-csb"))
			dung = 2;
//		else if (!strcmp(argv[iArgIndex], "-tq"))
//			dung = 3;
		else if (!strcmp(argv[iArgIndex], "-beta"))
			dung = 4;
		else if (!strcmp(argv[iArgIndex], "-dm2"))
			dung = 5;


		else if (!strcmp(argv[iArgIndex], "-hmp"))
			music = 0;	// use converted hmp to midi music
		else if (!strcmp(argv[iArgIndex], "-mod"))
			music = 1;	// use converted mod to ogg music
		else if (!strcmp(argv[iArgIndex], "-ogg"))
			music = 1;	// use ogg
		else if (!strcmp(argv[iArgIndex], "-segacd"))
			music = 2;	// use converted audio track to ogg music

		
		iArgIndex++;
	}

	__GlobalOption_DM2Skull_MusicMode = music;
}


int main(int argc, char* argv[])
{
  logfile = fopen("logfile1.txt", FILEMODE_WT);

  ProcessArgs(argc, argv);
  alg.start();

  do
  {
    try
    {
      while (!gamethread_initialized && !main_exit);

      while (!main_exit && gamethread_initialized)
      {
        event_loop_T1();
        al_rest(0.001);
      }
    }
    catch(...)
    {
      close_win = true;
    }
  } while (!main_exit);

  dm2sound.dtor();

  cleanup();
  return 0;
}
