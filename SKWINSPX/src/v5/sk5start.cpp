

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

#include <skcore.h>

//extern void DM2_INIT(void);

UINT SkWinCore::SKW5_QUICK_START()
{
	SKW5_STARTUP_THREAD_MAIN_LOOP_01();
	SKW5_SYS_EXIT(0);
	return 0;
}

void SkWinCore::SKW5_MAIN_STARTUP()
{
	// DM2_FIRE_BOOTSTRAP
	SKW5_INIT();
	SKW5_GAME_LOOP(); // TODO: doesn't return!
	//SKW5_END_GAME(CUTX16(ddat.v1e0250));
	SKW5_FIRE_SHUTDOWN();
	// WATCOM_PRE_EXIT
	printf("BEFORE SYS EXIT\n");
	SKW5_SYS_EXIT(0);
}

void SkWinCore::SKW5_SYS_EXIT(i8 iExitCode)
{
  throw(THROW_SYS_EXIT);
  return; // TODO noreturn
}

void SkWinCore::SKW5_FIRE_SHUTDOWN()
{
	;
}

void SkWinCore::SKW5_END_GAME()
{
	;
}

void SkWinCore::SKW5_GAME_LOOP()
{
	;
}

void SkWinCore::SKW5_STARTUP_THREAD_MAIN_LOOP_01()
{
  i16 i = 0;
  do
  {
    try
    {

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

	  printf("ALLOC 1\n");
      dm2_dballochandler.DM2_SETUP_MEMORYALLOCATION();
	  printf("ALLOC 2\n");
      dm2_dballochandler.DM2_SETUP_DB_ALLOCATION();
	  printf("ALLOC END\n");

      sysrestart = RESTART_NOTHING;

      bSK5GameThreadInitialized = true;
	  printf("ALL INIT DONE\n");

      //DM2_HIDE_MOUSE();

      SKW5_MAIN_STARTUP();
    }
    catch(unsigned int cn)
    {
      if (   cn == THROW_CLOSEWIN
          || cn == THROW_DMABORT
          || (cn & THROW_SYS_EXIT)
         )
        sysrestart = RESTART_QUIT;
    }
    bSK5GameThreadInitialized = false;
    //dm2_dballochandler.DM2_DTOR_MEMORYALLOCATION();
  } while (sysrestart != RESTART_QUIT);

  //dm2sound.stop_all_sound();
  main_exit = true;

  return;
}




void SkWinCore::SKW5_INIT(void)
{
//  c_nreg _RG1;
  ui8* RG51p;
  i32 RG52l;
printf("DM2 init\n");
  
SPX_DEBUG_PRINT("DM2_INIT...\n");
SPX_DEBUG_PRINT("DM2_dballoc_2636_03d4...\n");
  dm2_dballochandler.SKW5_ALLOC_ZSTRINGS_BUFFER();
// SK4 => INIT_CRAM_EMS_MEM + 2 vars + 2 FUNC
/*
 SPX_DEBUG_PRINT("DM2_READ_GRAPHICS_STRUCTURE...\n");
  DM2_READ_GRAPHICS_STRUCTURE();
SPX_DEBUG_PRINT("DM2_SOUND6...\n");
  DM2_SOUND6();
SPX_DEBUG_PRINT("DM2_ALLOC_HIBIGPOOL_MEMORY...\n");
  RG1P = dm2_dballochandler.DM2_ALLOC_HIBIGPOOL_MEMORY(lcon(0x400));
  RG51p = UPCAST(ui8, RG1P);
DEBUG_DUMP_ULP();


SPX_DEBUG_PRINT("DM2_LOAD_GDAT_ENTRY_DATA_TO...\n");
//  DM2_LOAD_GDAT_ENTRY_DATA_TO(1, 0, lcon(0x9), lcon(0xfe), RG1P); // 1, 0, 9, 0xFE
  DM2_LOAD_GDAT_ENTRY_DATA_TO(GDAT_CATEGORY_x01_INTERFACE_GENERAL, GDAT_INTERFACE_SUBCAT_x00_BASE_DATA, dtPalIRGB, 0xFE, RG1P); // 1, 0, 9, 0xFE

 //DEBUG_DUMP_BIN_DATA((unsigned char*) RG1P, 1024, 16);	// SPX: dump the IRGB palette
  DM2_CONVERT_DRIVERPALETTE(RG51p); // should come from RG1P

  dm2_dballochandler.DM2_DEALLOC_HIBIGPOOL(lcon(0x400));	// SK4 DEALLOC_LOWER_MEMORY
SPX_DEBUG_PRINT("DM2_LOAD_GDAT_INTERFACE_00_02...\n");
  DM2_LOAD_GDAT_INTERFACE_00_02();
  paldat.palette = XUPCAST(t_palette, DM2_QUERY_GDAT_ENTRY_DATA_PTR(1, 0, 13, lcon(0xfe)));
  DM2_098d_1208();
  //SK4 LOAD_GDAT_INTERFACE_00_00, not used in V5
  DM2_38c8_00c8();
  DM2_3929_0e16();


  ddat.v1e0984 = mkul(ddat.v1e0ad0 & 0x8);
SPX_DEBUG_PRINT("DM2_2405_0009...\n");
  DM2_2405_0009();
  DM2_INITBASICCURSORS();
  ddat.v1d70cb = 1;	// _4976_4748
  DM2_1031_07d6();


SPX_DEBUG_PRINT("DM2_dballoc_3e74_2439...\n");
  RG1Blo = dm2_dballochandler.DM2_dballoc_3e74_2439(15, 11);	// glbCreaturesMaxCount = _3e74_2439_GET_ENTRIES_NUMBER(0xf, 0xb);
  ddat.cd.gg.glbCreaturesMaxCount = RG1Blo;	// (ddat.v1e0414[17]) glbCreaturesMaxCount
  RG1L = unsignedlong(RG1Blo);
  RG1L++;
  RG1L = 3 * RG1L;
  RG52l = RG1L;
  // X16 si = (glbCreaturesMaxCount +1) * 3;
  
  RG1P = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(RG52l);
  ddat.v1e02ec = RG1P;
  DM2_FILL_STR(RG1T, 0xff, 1, CUTX16(RG52l));
  // FILL_STR(glbSomeCreatureTable, si, 0xff, 1);


  //m_454CA:
_DEBUG_SHOW_BIGPOOL();
SPX_DEBUG_PRINT("DM2_PLAY_MUSIC\n");
DM2_PLAY_MUSIC(0, true);	// SPX: to get some music in the menu screen
  for (;;)
  {
	SPX_DEBUG_PRINT("BEGIN_LOOP...\n");
	DM2_SHOW_MENU_SCREEN();
	SPX_DEBUG_PRINT("BEGIN_LOOP OUT\n");
	
	skWinApplication->skwin_Sleep(1000);
	
	_DEBUG_SHOW_BIGPOOL();
	if (DM2_GAME_LOAD() == 1)
		break;
    DM2_GRAPHICS_DATA_OPEN();
  }

  //m_454E1:
  DM2_GRAPHICS_DATA_OPEN();
  DM2_events_5BFB(unsignedlong(ddat.v1d26a0), 0);
  DM2_events_5BFB(unsignedlong(ddat.v1d26a2), 10);
  DM2_1c9a_3c30();
  dm2_dballochandler.DM2_ALLOC_CPX_SETUP(table1d70b4);
  DM2__INIT_GAME_38c8_03ad();
  DM2_GRAPHICS_DATA_CLOSE();
  if (ddat.cd.mo.glbSpecialScreen != _MENU_SCREEN__RESUME_GAME_SELECT)
    DM2_MOVE_RECORD_TO(lcon(0xffff), lcon(0xffffffff), 0, signedlong(ddat.v1e0270), ddat.v1e0272);
  //m_4554D:
  DM2_SHOW_MOUSE();
  eventqueue.event_1031_098e();
  */
printf("SKW5_INIT DONE!\n");
}