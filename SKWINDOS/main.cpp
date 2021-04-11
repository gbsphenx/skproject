#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#pragma warning (disable:4312)


#include <allegro.h>
#include <allegro5.h>

#include "src/dm2data.h"
#include "src/startend.h"
#include "src/c_midi.h"
#include "src/fileio.h"
#include "src/c_sound.h"
#include "src/c_allegro.h"
#include "src/c_alloc.h"
#include "src/c_dballoc.h"

#include "dm2global.h"


#include "driver.h"
#include "addon.h"
#include "util.h" // for checks only


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
//        (but that was the same in Atari-DM and CSB-source!)
//        (search for EVIL). 
//        a few datafiles are there now, didn't hardcode them yet.

//        Multithreading alive. (but - not nice to debug now...)

//        Resuming the game or restarting
//        it from the menu screen will not work yet!!!
//        Have to check the different ends!!!
//        (But reinitializations should work already)

//        zillions of TODO :P

// known bugs:
//   there are some things that MAY cause troubles,
//   but the current bugfix-list is handled at the moment
// 
// SKW_QUEUE_NOISE_GEN1 failed near /*@005FF3*/ bug still there?
// some int stop=1; spent to set breakpoints at critical things
// TODO: win - close button in the menu - screen

// compilerbugs and traps:
//   comparing i16 vs negative i16 - consts - KEEP LONG CONSTANTS AS POSSIBLE
// BAD TRAP! signed 0xffffffff >> 1 = 0xffffffff, but 0xffffffff / 2 = 0
// that one is really a pain!!! one can never be sure...
// ------------------------------------------------------------------

#if 0
#define BIGMEMPATH "../new_bigmem.bin"
#endif

#define PMDATAPATH "./pmdata.bin"

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

  for (int i=0; i<MAX_FILEHANDLES; i++)
    if (filehandle[i]!=NULL)
    {
      fclose(filehandle[i]);
      filehandle[i]=NULL;
    }
}

void* thread_func(ALLEGRO_THREAD *thr, void *arg)
{
//  ThreadInfo *info = (ThreadInfo *) arg;

//   while (!al_get_thread_should_stop(thr))
//  al_lock_mutex(info->mutex);

  do
  {
    try
    {
      alg.restart();

#if 0
      memset(memory,0,MEGA16);

      FILE* f = fopen(BIGMEMPATH, "rb");
      if (f==NULL) { cleanup(); exit(2); }
      if (fread(&memory[0x14],MEGA16 - 0x14,1,f) != 1) { cleanup(); exit(2); } // DOSbox started the dump here at byte 20
      fclose(f);

      FILE* fi = fopen("blitb.dat","wb");
      fwrite(memory+0x1d1554, 0x1000, 0x1, fi);
      fclose(fi);

      FILE* fi1 = fopen("mouse1.dat","wb");
      fwrite(memory+0x1d255e, 0x60, 0x1, fi1);
      fclose(fi1);

      FILE* fi2 = fopen("mouse2.dat","wb");
      fwrite(memory+0x1d25be, 0x90, 0x1, fi2);
      fclose(fi2);

      FILE* fi3 = fopen("img1.dat","wb");
      fwrite(memory+0x1e1054, sizeof(c_image), 0x1, fi3);
      fclose(fi3);

      FILE* fi4 = fopen("v1d7108.dat","wb");
      fwrite(memory+0x1d7108, 0x80, 0x1, fi4);
      fclose(fi4);

      FILE* fi5 = fopen("v1d39bc.dat","wb");
      fwrite(memory+0x1d39bc, 0x1e4, 0x1, fi5);
      fclose(fi5);

      FILE* fi6 = fopen("v1d338c.dat","wb");
      fwrite(memory+0x1d338c, 0x108*6, 0x1, fi6);
      fclose(fi6);

      FILE* fi7 = fopen("v1d653c.dat","wb");
      fwrite(memory+0x1d653c, 0x37*8, 0x1, fi7);
      fclose(fi7);

      FILE* fi8 = fopen("v1d6802.dat","wb");
      fwrite(memory+0x1d6802, 0x110, 0x1, fi8);
      fclose(fi8);

      FILE* fi9 = fopen("v1d296c.dat","wb");
      fwrite(memory+0x1d296c, 0x3f*0x24, 0x1, fi9);
      fclose(fi9);
#else
#if 0
      FILE* f = fopen(PMDATAPATH, "rb");
      if (f==NULL)
        throw(THROW_DMABORT);
      if (fread(&memory[0x1d065c], 0x1128c, 1, f) != 1)
        throw(THROW_DMABORT);
      fclose(f);

      f = fopen(MUSICDATAPATH, "rb");
      if (f==NULL)
        throw(THROW_DMABORT);
      if (fread(&memory[0x1410be], 0xc0330, 1, f) != 1)
        throw(THROW_DMABORT);
#endif
#endif

      ddata.init();

      driver_init(DOWNCAST(t_gfxdata, ddata.screenptr));

      DM2_SETUP_MEMORYALLOCATION();
      DM2_SETUP_DB_ALLOCATION();

      // route pointers to driver - variables
      ddata.drvxmouserectptr = &DRVR_rect1;
      ddata.drvmouseentryptr = &DRV_mouseentry;

      restart = RESTART_NOTHING;

      alg.hide_mouse();

      alg.set_mouse(ORIG_SWIDTH/2, ORIG_SHEIGHT/2);

      alg.start_timer();

      DRVW_mousevisicnt = 1;
      ddata.mouse_visibility = 1;
      WATCOM_STARTUP();
    }
    catch(unsigned int n)
    {
SPX_DEBUG_POP_ALL();
      if (    n == THROW_CLOSEWIN
           || n == THROW_DMABORT
//           || n == THROW_PREPARE_EXIT
         )
         {
           DM2_DTOR_MEMORYALLOCATION();
           break;
         }
      if (n & THROW_SYS_EXIT)
        restart = RESTART_NOTHING;
    }
    DM2_DTOR_MEMORYALLOCATION();
  } while (restart != RESTART_QUIT);

  dm2sound.stop_all_sound();
  main_exit = true;

//  al_unlock_mutex(info->mutex);

  return NULL;
}

int main(int argc,char *argv[])
{
  // memory = new i8[MEGA16];

/*
ui8 uvar8 = 0xff;
  if (uvar8 == ucon8(0xff)) // CORRECT!
    printf("true");
  else
    printf("false");
  if (unsignedlong(uvar8) == ucon8(0xff))
    printf("true");
  else
    printf("false");

x8 var8 = 0xff;
  if (var8 == ucon8(0xff))
    printf("true");
  else
    printf("false"); // ERROR!
  if (unsignedlong(var8) == ucon8(0xff))
    printf("true");
  else
    printf("false");

i8 ivar8 = 0xff;
  if (ivar8 == icon8(0xff))
    printf("true"); // CORRECT!
  else
    printf("false");
  if (ivar8 == ucon8(0xff))
    printf("true");
  else
    printf("false"); // ERROR!
*/
// x16 intvar = con(0xff); // okay, intvar gets the unextended value 0x000000ff
/*
x16 intvar = con(0x1111);
    intvar &= con(0xff); // okay, the highbyte will be masked away
x16 intvar2 = con(0x1111);
    intvar2 |= con(0xff); // okay, the highbyte will not be affected
*/
/*
x8 bytevar = 0xff;
ui8 ubytevar = 0xff;
if (bytevar == con(0xffffffff))
  printf("okay"); // comes here
if (bytevar == con(0xff))
  printf("okay"); // doesn't come here!

if (ubytevar == con(0xffffffff))
  printf("okay"); // doesn't come here!
if (ubytevar == con(0xff))
  printf("okay"); // but here

// conclusion: use the expanded value for signed ones
//             use the unexpanded value for unsigned ones!
*/
// don't expand that one:
// if ((sbbwptrrg5->b0 & con(0x80)) != con(0x80))
// the temporary seems to be an unsigned one then - SILLY!
/*
x16 wordvar = con(0xff);
x16 uwordvar = con(0xff);
// correct, both get the value unextended
*/

/*
x16 wordvar = 0xff;
ui16 uwordvar = 0xff;
if (wordvar == con(0xffffffff))
  printf("okay"); // no
if (wordvar == con(0xff))
  printf("okay"); // yes
if (uwordvar == con(0xffffffff))
  printf("okay"); // no
if (uwordvar == con(0xff))
  printf("okay"); // yes
ui8 ubyterg8; ubyterg8 = 0xff;
ui16 uwordrg16;
  uwordrg16 = ubrshift(ubyterg8, con(0x5));
*/

  logfile = fopen("logfile1.txt","wt");

  alg.start();

  do
  {
    try
    {
      do
      {
          event_loop_T1();
 //       al_rest(0.001);
      } while (!main_exit);
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
