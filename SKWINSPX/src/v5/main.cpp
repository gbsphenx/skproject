//#include <process.h>	// what was that for ?
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <skwinapp.h>
#include <skver.h>

#pragma warning (disable:4312)

//#include <allegro.h>
#ifndef __SK_NO_ALLEGRO__
//#include <allegro5/allegro5.h>
#else
	#define ALLEGRO_THREAD	void
#endif // __SK_NO_ALLEGRO__

#include <dm2data.h>
#include <startend.h>
#include <sfxmidi.h>
#include <fileio.h>
#include <sfxsnd.h>
//#include <SKALEGRO.h>
#include <skalloc.h>
#include <skalocdb.h>
#include <dm2globl.h>
#include <skrecord.h>
#include <skmap.h>
#include <bcreatur.h>
#include <uiinput.h>
#include <skimage.h>
#include <uikeybd.h>
#include <skeventq.h>
#include <skmcursr.h>
#include <gfxblit.h>
#include <gfxdec.h>
#include <skrandom.h>
#include <gfxpal.h>
#include <gfxstr.h>
#include <gfxmain.h>
#include <skxrect.h>
#include <skhero.h>
#include <sktimer.h>
#include <gfxblit.h>
#include <SK0350.h>
#include <ints.h>
#include <bgdat.h>
#include <uitmouse.h>
#include <skaddon.h>

#include <dm2debug.h>

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
 // alg.dtor(); // SPX: REMOVED ALLEGRO

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

void DM2_THREAD_MAIN_LOOP_01()
{
  i16 i = 0;
  do
  {
    try
    {
      //alg.restart();  // SPX: REMOVED ALLEGRO

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

	SPX_DEBUG_PRINT("DM2_DBALLOC START...\n");
      dm2_dballochandler.DM2_SETUP_MEMORYALLOCATION();
	SPX_DEBUG_PRINT("DM2_DBALLOC 2 ...\n");
      dm2_dballochandler.DM2_SETUP_DB_ALLOCATION();
	SPX_DEBUG_PRINT("DM2_DBALLOC DONE\n");

      sysrestart = RESTART_NOTHING;

 // SPX: REMOVED ALLEGRO
//      alg.hide_mouse(); 
//      alg.set_mouse(ORIG_SWIDTH / 2, ORIG_SHEIGHT / 2);
//      alg.start_timer();

      gamethread_initialized = true;

      //DM2_HIDE_MOUSE();

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

  return;
}

void* thread_func(ALLEGRO_THREAD* thr, void* vparg)
{
	DM2_THREAD_MAIN_LOOP_01();
	return NULL;
}

int __GlobalOption_DM2Skull_MusicMode = 0;
int __GlobalOption_DM2Skull_NewGameMode = 0; // 0 = wait title menu / 1 = new game


extern SkWinApp* skWinApplication;	// in GFXMAIN.cpp

int SKWIN_V5BITDOS_MAIN(SkWinApp* skArgWinApplication)
{
  logfile = fopen("logdm2.txt", FILEMODE_WT);
  printf("SKWIN_V5BITDOS_MAIN\n");
	skWinApplication = skArgWinApplication;
  DM2_THREAD_MAIN_LOOP_01();

  //alg.start();  // SPX: REMOVED ALLEGRO

  do
  {
    try
    {
      while (!gamethread_initialized && !main_exit);

      while (!main_exit && gamethread_initialized)
      {
		//printf("event_loop_T1 START\n");
        event_loop_T1();
		//printf("event_loop_T1 END\n");
		//printf("al_rest START\n");
        //al_rest(0.001);
		skWinApplication->skwin_Sleep(1);
		//printf("al_rest END\n");
      }
    }
    catch(...)
    {
		printf("MAIN CATCH!\n");
      close_win = true;
    }
  } while (!main_exit);

  dm2sound.dtor();
  cleanup();
  return 0;
}
