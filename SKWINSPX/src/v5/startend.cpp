#include <types.h>
#include <skwinapp.h>
//#include <SKALEGRO.h>
#include <dm2globl.h>
#include <uikeybd.h>
#include <uitmouse.h>
#include <dm2data.h>
#include <skrandom.h>
#include <gfxstr.h>
#include <gfxpal.h>
#include <util.h>
#include <startend.h>
#include <skhero.h>
#include <gfxmain.h>
#include <bcreatur.h>
#include <SK1031.h>
#include <sksvgame.h>
#include <skmovrec.h>
#include <skeventq.h>
#include <skimage.h>
#include <sfx.h>
#include <uiinput.h>
#include <sfxsnd.h>
#include <sklodlvl.h>
#include <skmcursr.h>
#include <skstr.h>
#include <skguidrw.h>
#include <skgdtqdb.h>
//#include <UIKEYBD.h>
#include <skxrect.h>
#include <SK1C9A.h>
#include <skmap.h>
#include <bitem.h>
#include <skmove.h>
#include <sklight.h>
#include <sktimer.h>
#include <skguivwp.h>
#include <sktimprc.h>
#include <skweath5.h>
#include <skevents.h>
//#include <skmove.h>
#include <skengage.h>
#include <SK0AAF.h>
#include <bgdat.h>

#include <emu.h>
#include <regs.h>

#include <dm2debug.h>

#include <stdio.h>
#include <dosvideo.h>

#include <skparam.h>
#include <skdefine.h>

extern SkWinApp* skWinApplication;	// in GFXMAIN.cpp

ui8 table_1dfac4[0x14] =
{
  0x00, 0x09, 0x01, 0x01, 0x0b, 0x06, 0x04, 0x05,
  0x05, 0x05, 0x02, 0x03, 0xff, 0xff, 0x07, 0x08,
  0x06, 0x08, 0x01, 0xff
};

// id: 0x58B3F
// errcode comes as two bytes in one word
// called only by fileio-functions
// converts an error-code and writes it via SKW_MARK_ERROR
i16 DM2_WATCOM_ERRHANDLING(i16 nw)
{
  i32 code = unsignedlong(nw);
  ui8 ub_04 = CUTLX8(code);
  ui8 ub_02 = CUTLX8(code);
  printf("DM2_WATCOM_ERRHANDLING %d\n", nw);
  ddat.errhandling = unsignedlong(ub_04);
  if (nw >= wcon(0x100))
    code = unsignedlong(CUTLX8(nw >> 8));
  else
  {
    if (ddat.errversion >= 3) // ever true!
    {
      if (ub_04 != 80)
      {
        if (ub_04 >= 34)
          ub_02 = 19;
        else if (ub_04 >= 32)
          ub_02 = 5;
      }
      else
        ub_02 = 14;
    }
    if (ub_02 > 19)
      ub_02 = 19; // limits ub_02
    code = unsignedlong(table_1dfac4[ub_02]);
  }
  DM2_WATCOM_MARK_ERROR(code);
  return -1;
}

// id: 0x58BCB
// writes an errorcode into a memory - location
// (which was returned formerly by WATCOM_590A2)
// was WATCOM_58BC8
void DM2_WATCOM_MARK_ERROR(i32 nl)
{
	printf("DM2_WATCOM_MARK_ERROR %d\n", nl);
  ddat.markerror = nl;
}

// function doesn't do anything anymore.
#if 0
static void DM2_1031_096a(void)
{
  for (i16 wi = 0; wi < 18; wi++)
  {
    c_clickrectnode* xptr = &table1d32d8[wi];
    R_4BFF(); // now an empty function
  }
}
#endif

// a lot deleted herein
// TODO: condition is surely wrong, but works (for now)!
void DM2_PREPARE_EXIT(void)
{
  // DM2_1031_096a();  does nothing anymore
  sysrestart = Tmouse.is_visible() ? RESTART_QUIT : RESTART_RESUME;
  //alg.stop_timer(); // SPX: REMOVED ALLEGRO
  throw(THROW_PREPARE_EXIT);
}

// id: 0x58A37
// used only on toplevels
// no value - return necessary, because exit follows everytime
// calls closing functions stored in exittable, which isn't
// available anymore
#if 0
void WATCOM_PRE_EXIT(ui8 ua, ui8 ub)
{
  for (;;)
  {
    s_exittableentry* eptr5 = adress_of(EXITTABLE1);
    s_exittableentry* eptr6 = adress_of(EXITTABLE2);
    s_exittableentry* eptr2 = ptr6;
    ui8 b1 = ua;
    while (eptr5 < eptr6)
    {
      if ((eptr5->v1 != 2) && (b1 <= eptr5->v2))
      {
        eptr2 = eptr5;
        b1 = eptr5->v2;
      }
      eptr5++;
    }
    if (eptr2 == eptr6)
      return;
    if (b1 <= ub && eptr2->func != 0)
    {
      // RG1L = call32(eptr2->func);
    }
    eptr2->v1 = 2;
  }
}
#endif

// 0x21/0x4c
void SYS_exit(i8 exitcode)
{
  // DOS - 2+ - QUIT WITH EXIT CODE (EXIT)
  // exit code was alb
  throw(THROW_SYS_EXIT);
  return; // TODO noreturn
}

// id: 0x5260C
// top - level - ending routine
// had one parameter - makes problems, works without
// id: 0x52624
// top - level - ending routine - never returns
// routine gets no parameter anymore (had one - but there are problems then)
static void DM2_FIRE_EXIT(void)
{
//   call32(VARL(SHUTDOWNVEC2));
//   call32(VARL(SHUTDOWNVEC3));
  // WATCOM_PRE_EXIT(0, lcon(0xff));
  SYS_exit(2); // parameter set by me
}

static void DM2_FIRE_SHUTDOWN(void)
{
//   call32(VARL(SHUTDOWNVEC1));
//   call32(VARL(SHUTDOWNVEC2));
  DM2_FIRE_EXIT();
}

static void R_C470(c_pixel* srcgfx, i16 edxw, i16 ebxw, t_palette* palette)
{
  c_rect rc_00;

  i16 srcx = 0;
  i16 srcy = 0;
  if (DM2_QUERY_BLIT_RECT(BMPCAST(srcgfx), &rc_00, edxw, IO srcx, IO srcy) != NULL)
  {
    blit_toscreen(
      srcgfx,
      &rc_00,
      srcx,
      srcy,
      getbmpheader(BMPCAST(srcgfx))->width,
      /*TAG alphamask*/ ebxw,
      BLITMODE0,
      getbmpheader(BMPCAST(srcgfx))->res,
      BPP_8,
      palette);
  }
}

static void DM2_DRAW_TITLE_MENU_SCREEN(void)
{
  DM2_SELECT_PALETTE_SET(0);
  if (ddat.v1d66f8 == 0)
    //m_2877D:
    R_C470(ddat.glbImageMenuScreen, 2, -1, paldat.glbl_pal1);
	// = DRAW_GAMELOAD_DIALOGUE_TO_SCREEN(glbImageMenuScreen, 2, -1, _4976_52ba);
  {
    blit_toscreen(
      ddat.glbImageMenuScreen,
      tmprects.alloc_origin_tmprect(ORIG_SWIDTH, ORIG_SHEIGHT),
      0,
      0,
      ORIG_SWIDTH,
      NOALPHA,
      BLITMODE0,
      BPP_8,
      BPP_8);
  }

  DM2_SELECT_PALETTE_SET(1);
}

// TODO: optimize temporaries
#include <regs.h>
#include <emu.h>

//extern int __GlobalOption_DM2Skull_NewGameMode; // main.cpp

// id: 0x287A6
// was SKW_2481_007d, SHOW_MENU_SCREEN
static void DM2_SHOW_MENU_SCREEN(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
_DEBUG_SHOW_BIGPOOL("DM2_SHOW_MENU_SCREEN");
	printf("DM2_SHOW_MENU_SCREEN\n");

  ddat.glbImageCreditScreen = DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(5, 0, 1);
 //SPX_DEBUG_PRINT("DM2_QUERY_GDAT_ENTRY_DATA_INDEX(5, 0, 7, 4)\n");
  if (DM2_QUERY_GDAT_ENTRY_DATA_INDEX(5, 0, 7, 4) == -1)
  {
    //m_28802:
    ddat.glbImageMenuScreen = DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(5, 0, 4);
    t_resolution res = getbmpheader(BMPCAST(ddat.glbImageMenuScreen))->res;
//	SPX_DEBUG_PRINT_WAIT("res = %d\n", res);
    if (res != BPP_8)
    {
      if (res != BPP_4)
        //m_28837:
        RG1W = getbmpheader(BMPCAST(ddat.glbImageMenuScreen))->width;
      else
        RG1W = MK_EVEN(getbmpheader(BMPCAST(ddat.glbImageMenuScreen))->width) / 2;
      //m_28840:
      paldat.glbl_pal1 = CHGCAST(t_palette, ddat.glbImageMenuScreen + RG1W * getbmpheader(BMPCAST(ddat.glbImageMenuScreen))->height);
    }
  }
  else
  {
    ddat.v1d66f8 = 1;
    ddat.glbImageMenuScreen = QUERY_pixel(DM2_QUERY_GDAT_ENTRY_DATA_PTR(5, 0, 7, 4));
  }
_DEBUG_SHOW_BIGPOOL("BEFORE PART 2");
  //m_28860:
  t_resolution res = getbmpheader(BMPCAST(ddat.glbImageCreditScreen))->res;
  if (res != BPP_8)
  {
    //m_28879:
    if (res != BPP_4)
      //m_28890:
      RG4W = getbmpheader(BMPCAST(ddat.glbImageCreditScreen))->width;
    else
      RG4W = MK_EVEN(getbmpheader(BMPCAST(ddat.glbImageCreditScreen))->width) / 2;
    //m_28896:
    paldat.glbl_pal2 = CHGCAST(t_palette, ddat.glbImageCreditScreen + RG4W * getbmpheader(BMPCAST(ddat.glbImageCreditScreen))->height);
  }
  else
    paldat.glbl_pal2 = NULL;
_DEBUG_SHOW_BIGPOOL("BEFORE DATA CLOSE");
  //m_288B7:
  DM2_GRAPHICS_DATA_CLOSE();
 _DEBUG_SHOW_BIGPOOL("AFTER DATA CLOSE");
  DM2_1031_0541(0);
_DEBUG_SHOW_BIGPOOL("BEFORE LOOP");
  //m_288C8:
  for (;;)
  {
_DEBUG_SHOW_BIGPOOL("BEFORE SHOW_MOUSE");
    DM2_DRAW_TITLE_MENU_SCREEN();
    DM2_SHOW_MOUSE();
//SPX_DEBUG_PRINT("event_1031_098e...\n");
    eventqueue.event_1031_098e();
    ddat.cd.mo.glbSpecialScreen = _MENU_SCREEN__TITLE_MENU; // x63 = title screen
// SPX
	if (SkCodeParam::bOptionNewGame > 0)
	{	
		if (SkCodeParam::bOptionNewGame == 1)
			ddat.cd.mo.glbSpecialScreen = _MENU_SCREEN__LOAD_NEW_GAME;
		else if (SkCodeParam::bOptionNewGame == 2)
			ddat.cd.mo.glbSpecialScreen = _MENU_SCREEN__RESUME_GAME_SELECT;
		else if (SkCodeParam::bOptionNewGame == 3)
			ddat.cd.mo.glbSpecialScreen = _MENU_SCREEN__SHOW_CREDITS;
	}
    for (;;)
    {
//SPX_DEBUG_PRINT("BEFORE DM2_EVENT_LOOP...\n");
      DM2_EVENT_LOOP();
      if (ddat.cd.mo.glbSpecialScreen != lcon(_MENU_SCREEN__TITLE_MENU))
      {
_DEBUG_SHOW_BIGPOOL();
        if (ddat.cd.mo.glbSpecialScreen != lcon(_MENU_SCREEN__SHOW_CREDITS))
        {
          DM2_HIDE_MOUSE();
_DEBUG_SHOW_BIGPOOL();
          if (ddat.v1d66f8 == 0)
            //m_28910:
            DM2_FREE_PICT_ENTRY(ddat.glbImageMenuScreen); // glbImageMenuScreen
          else
            dm2_dballochandler.DM2_DEALLOC_BIGPOOL_STRUCT_BEFORE(CHGCAST(s_malloctail, ddat.glbImageMenuScreen)); // glbImageMenuScreen
          //m_2891A:
          DM2_FREE_PICT_ENTRY(ddat.glbImageCreditScreen); // glbImageCreditScreen
          return;
        }
        break;
      }
    } // for DM2_EVENT_LOOP();
  } // for DM2_DRAW_TITLE_MENU_SCREEN();
}

// belongs to DM2_098d_1208
static void DM2_LOAD_RECTS_AND_COMPRESS(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG51l;
  i32 RG52l;
  void* RG6p;
  c_nreg _RG7;
  i32 vl_00;
  i32 vl_04;
  unk* xparp00;

  vl_04 = unsignedlong(RG2Blo);
  RG7L = unsignedlong(RG4Blo);
  RG51l = unsignedlong(RG1Blo);
  vl_00 = RG51l;
  RG3L = vl_04;
  RG2L = 4;
  RG4L = RG7L;
  RG1L = DM2_QUERY_GDAT_ENTRY_DATA_LENGTH(CUTLX8(RG51l), RG4Blo, RG2Blo, RG3Blo);
  RG52l = unsignedlong(RG1W);
  RG2L = lcon(0x400);
  RG4L = 2;
  RG6p = VCAST(dm2_dballochandler.DM2_ALLOC_HIBIGPOOL_MEMORY(RG52l));
  xparp00 = CHGCAST(unk, RG6p);
  DM2_LOAD_GDAT_ENTRY_DATA_TO(CUTLX8(vl_00), RG7Blo, lcon(0x4), CUTLX8(vl_04), xparp00);
  RG2P = DOWNCAST(s_rnode*, &xrectdat.rnodep_rectanglelist);
  DM2_COMPRESS_RECTS(RG6p, UPCAST(s_rnode, RG2P));
  dm2_dballochandler.DM2_DEALLOC_HIBIGPOOL(RG52l);
}

static void DM2_098d_1208(void)
{
  DM2_LOAD_RECTS_AND_COMPRESS(1, 0, 0);
  ddat.v1e01d4 = 1;
}

static t_bmp* R_D2C(i16 w, i16 h)
{
  t_bmp* bmp = DRV_screen256.pixel; // did call R_4A986 here
  s_dm2bmpheader* sbmp = getbmpheader(bmp);
  sbmp->res = BPP_8;
  sbmp->unused = 0; // SPX: better for proper init
  sbmp->width = w;
  sbmp->height = h;
  return bmp;
}

// belongs to DM2_INIT
static void DM2_38c8_00c8(void)
{
  gfxsys.bitmapptr = R_D2C(gfxsys.backbuffer_w, gfxsys.backbuffer_h);
  dm2_dballochandler.v1e0a90 = 100000;
}

// this one used one time by an unreferenced routine
// else it belongs to DM2_3929_0e16
/*static*/ void DM2_3929_07e1(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;

  RG2L = RG4L;
  if (RG1W < 0)
    RG1L = 0;
  RG4W = ddat.v1d2744;
  RG3W = strdat.gfxstrw4;
  RG4L -= RG3L;
  RG4L = signedlong(RG4W);
  RG1L *= RG3L;
  RG1L = signedlong(DM2_MIN(RG1W, RG4W));
  ddat.v1d70ea = RG1W;
  if (RG2W >= 0)
  {
    //m_457F7:
    RG1L = signedlong(RG2W);
    if (RG1L >= lcon(0x3))
      RG2L = 2;
  }
  else
    RG2L = 0;
  //m_45804:
  ddat.v1e141e = RG2W;
  ddat.v1e1420 = wcon(0x0);
}

// belongs to DM2_INIT
static i32 DM2_3929_0e16(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_rect rc_00;
  unk* xparp00;

  DM2_3929_07e1(0, 2);
  gfxsys.pictbuff = DM2_ALLOC_PICT_BUFF(ddat.v1d2744, ddat.v1d2746, FREEPOOL, lcon(0x8));
  RG2L = lcon(0x400);
  RG4L = 1;
  RG1P = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(lcon(0x300));
  strdat.strptr = UPCAST(t_text, RG1P);
  xparp00 = RG1P;
  RG3L = 0;
  DM2_LOAD_GDAT_ENTRY_DATA_TO(1, 0, lcon(0x7), RG3Blo, xparp00);
  RG1L = 0;
  RG2L = lcon(0xffffffff);
  //m_45D6B:
  for (;;)
  {
    RG4L = unsignedlong(RG1W);
    if (RG4L >= lcon(0x3))
    {
      RG4R = &rc_00;
      DM2_QUERY_EXPANDED_RECT(15, RG4R);
      RG1L = signedlong(strdat.strxplus);
      RG4L = 3 * RG1L - signedlong(ddat.v1d2736);
      RG1L = (signedlong(rc_00.h) - RG4L) / 2;
      ddat.v1e141c = RG1W;
      ddat.v1d70dc = 1;
      return RG1L;
    }
    //m_45D63:
    ddat.v1e1408[RG4L] = RG2L;
    RG1L++;
  }
  FOREND
}

// belongs to DM2_INIT
static void DM2_2405_0009(void)
{
  ddat.savegamewpc.bmp = DM2_ALLOC_PICT_BUFF(ddat.v1d271a, ddat.v1d271c, FREEPOOL, 4);
}

// belongs to DM2_INIT
static void DM2_1c9a_3c30(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;

  RG4L = 0;
  ddat.v1e08a0 = RG4W;
  RG4RP = recordptr[4];
  RG2P = DOWNCAST(s_sgwords, ddat.savegamep4);
  RG2W = word_at(RG2P, lcon(0x14));
  //m_243E6:
  for (;;)
  {
    RG2L--;
    if (RG2W == lcon(0xffffffff))
    {
      RG1P = DOWNCAST(s_sgwords, ddat.savegamep4);
      RG4L = signedlong(word_at(RG1P, lcon(0x14)));
      RG1W = ddat.v1e08a0;
      RG1L += lcon(0x64);
      RG1L = signedlong(DM2_MIN(RG1W, RG4W));
      ddat.v1e08a0 = RG1W;
      creatures = XUPCAST(c_creature, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(sizeof(c_creature) * unsignedlong(RG1W)));
      ddat.v1e08cc = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(lcon(0x400));
      ddat.v1e08c8 = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(lcon(0x400));
      return;
    }
    //m_243C7:
    if (word_at(RG4P) != lcon(0xffffffff))
    {
      RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG4P, lcon(0x4)))));
      if (jz_test8(location(RG1P), 1))
        ddat.v1e08a0++;
    }
    RG4P += lcon(0x10);
  }
  FOREND
}

// id: 0x453C1
// was SKW_38c8_04aa
static void DM2_INIT(void)
{
  c_nreg _RG1;
  ui8* RG51p;
  i32 RG52l;

SPX_DEBUG_PRINT("DM2_INIT...\n");
SPX_DEBUG_PRINT("DM2_dballoc_2636_03d4...\n");
  dm2_dballochandler.DM2_dballoc_2636_03d4();
// SK4 => INIT_CRAM_EMS_MEM + 2 vars + 2 FUNC

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
  DM2_LOAD_GDAT_ENTRY_DATA_TO(GDAT_CATEGORY_INTERFACE_GENERAL, GDAT_INTERFACE_SUBCAT_BASE_DATA, dtPalIRGB, 0xFE, RG1P); // 1, 0, 9, 0xFE

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
}

// belongs to DM2_GAME_LOOP
// was SKW_2e62_0d82
void DM2_GLOBAL_UPDATE_UNKNOW1(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;

  RG4W = ddat.savegamewpc.w_00;
  if (RG4W != lcon(0xffffffff))
  {
    RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG4W), 0);
    RG1W &= lcon(0x8000);
    RG1L = unsignedlong(RG1W);
    if (RG1L != 0)
    {
      RG1Blo = DM2_guidraw_2405_014a(unsignedlong(ddat.savegamewpc.w_00), lcon(0xffffffff), lcon(0x1));
      RG4Blo = RG1Blo;
      if (RG1Blo != ddat.savegamewpc.b_16)
      {
        DM2_DRAW_ITEM_IN_HAND(&ddat.savegamewpc);
        DM2_events_443c_0434();
        ddat.savegamewpc.b_16 = RG4Blo;
      }
    }
  }
}

// belongs to DM2_GAME_LOOP
static void DM2_2759_12e6(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG5l;
  c_wlreg _RG61;
  i32 RG62l;
  c_o32 vo_00; // poke 16 peek 32 inc16
  c_o32 vo_04; // poke 16 peek 16 32 inc16
  i16 vw_08;

  vw_08 = -1;
  RG2L = 0;
  vo_04.poke16(RG2W);
  //m_310B0:
  for (;;)
  {
    RG1L = signedlong(vo_04.peek16());
    RG4L = unsignedlong(party.heros_in_party);
    if (RG1L >= RG4L)
    {
      RG1W = party.curacthero;
      vo_04.poke16(RG1W);
      if (RG1W != 0)
      {
        if (vw_08 >= wcon(0x0))
        {
          RG1L = signedlong(vw_08);
          if (RG1L <= lcon(0x1))
          {
            RG2L = signedlong(party.curactmode);
            RG61W = vo_04.peek16() - 1;
            vo_04.poke16(RG61W);
            RG4L = signedlong(vo_04.peek16());
            RG4L = unsignedlong(party.hero[RG4L].item[RG2L]);
            RG1L = unsignedlong(RG61W);
            DM2_IS_ITEM_HAND_ACTIVABLE(RG1L, RG4L, RG2L);
          }
        }
      }
      return;
    }
    //m_30FAE:
    RG1L = 0;
    vo_00.poke16(RG1W);
    //m_3109B:
    for (;;)
    {
      RG2L = signedlong(vo_00.peek16());
      if (RG2L > lcon(0x1))
      {
        vo_04.inc16();
        break;
      }
      //m_30FB9:
      RG62l = signedlong(vo_04.peek16());
      RG1P = &ddat.v1e0adc[16 * RG62l];
      RG3P = RG1P + 8 * RG2L;
      if (word_at(RG3P + 2) != 0)
      {
        bool skip00705 = false;
        c_hero* hero = &party.hero[RG62l];
        if (hero->curHP == 0)
          skip00705 = true;
        else
        {
          RG4W = word_at(RG3P, lcon(0x6));
          if (RG4W == hero->item[RG2L])
          {
            //m_31011:
            if (byte_at(RG3P + lcon(0x4)) == 0)
            {
              //m_31020:
              dec16(location(RG3P + 2));
              vw_08 = party.curactmode;
              RG1W = vo_00.peek16();
              party.curactmode = RG1W;
              RG4L = unsignedlong(word_at(RG3P, lcon(0x6)));
              RG5l = unsignedlong(vo_04.peek16());
              DM2_IS_ITEM_HAND_ACTIVABLE(RG5l, RG4L, RG2L);
              RG1W = unsignedword(RG1Blo);
              RG1Blo = byte_at(RG3P, lcon(0x5));
              RG1Bhi |= lcon(0xffffff80);
              RG4L = unsignedlong(RG1W);
              RG1L = RG5l;
              DM2_ENGAGE_COMMAND(RG1L, RG4L);
              RG1W = DM2_RAND16(mkuw(word_at(RG3P) + 1));
              if (RG1W == 0)
              {
                RG1L = DM2_events_30DEA(RG62l);
                if (RG1L != 0)
                {
                  mov16(location(RG3P), 0);
                  RG1W = word_at(RG3P);
                  mov16(location(RG3P + 2), RG1W);
                  mov8(location(RG3P + lcon(0x4)), RG1Blo);
                }
              }
              RG1W = vw_08;
              party.curactmode = RG1W;
            }
            else
              mov8(location(RG3P + lcon(0x4)), 0);
          }
          else
            skip00705 = true;
        }

        if (skip00705)
          //m_31006:
          mov16(location(RG3P + 2), 0);
      }
      //m_31097:
      vo_00.inc16();
    }
  }
  FOREND
}

// belongs to DM2_GAME_LOOP
static void DM2_3929_086f(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;
  c_rect rc_00;

  RG4R = &rc_00;
  DM2_QUERY_EXPANDED_RECT(15, RG4R);
  RG5w = rc_00.y; // adj
  RG3L = 0;
  //m_458E2:
  for (;;)
  {
    RG2L = unsignedlong(RG3W);
    if (RG2L >= lcon(0x3))
      return;
    //m_45887:
    RG2L *= 4;
    RG1L = ddat.v1e1408[RG2L / 4];
    if (RG1L != lcon(0xffffffff) && mkul(RG1L) <= mkul(timdat.gametick))
    {
      RG1W = strdat.strxplus * RG3W + RG5w; // adj
      RG4W = ddat.v1e141c + RG1W;
      rc_00.y = RG4W;
      rc_00.h = strdat.gfxstrw1;
      DM2_HIDE_MOUSE();
      RG4L = unsignedlong(palettecolor_to_ui8(paldat.palette[E_COL00]));
      RG1R = &rc_00;
      DM2_FILL_FULLSCREEN(/*TAG BLITRECT*/ RG1R, ui8_to_pixel(RG4UBlo));
      DM2_SHOW_MOUSE();
      ddat.v1e1408[RG2L / 4] = lcon(0xffffffff);
    }
    //m_458E1:
    RG3L++;
  }
  FOREND
}

static void DM2_GAME_LOOP(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 parw00;
  i16 parw01;
  bool skipinloop = false;
	skWinApplication->skwin_Sleep(1000);
	{
		char message[128]; memset(message, 0, 128);
		char sExtraInfo[64]; memset(sExtraInfo, 0, 64);
//		if (SkCodeParam::bDM2V5Mode)
//			strcpy(sExtraInfo, " V5-GFX");
		sprintf((char*)message, "%s (%s) [%s] <%s>%s\n", __SKWIN_PROGNAME__, strVersionNumber, __SKWIN_RELEASE_DATE__, __SKWIN_SYSTEM__, sExtraInfo);
		DM2_DISPLAY_HINT_TEXT(COLOR_YELLOW, (t_text*)message);
//		SkD((DLV_DBG_INIT, "%s\n", message));
	}

  ddat.ticktrig = lcon(0x8);	// stdTickBalance ?
  ddat.dialog2 = 0;
  DM2_FILL_ORPHAN_CAII();
  //m_1475D:
  for (;;)
  {
    RG2L = 0;
    ddat.ticksum = RG2W;
    RG1L = signedlong(ddat.v1d27f8);
    if (RG1L == lcon(0xffffffff))
      skipinloop = true;
    //m_14773:
    for (;;)
    {
      if (!skipinloop)
      {
		  // v1d27f8 = glbMapToLoad
		  // DM2_LOAD_NEWMAP has different signature from LOAD_NEWMAP
        DM2_LOAD_NEWMAP(signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), signedlong(ddat.v1d27f8), lcon(0x1));
        RG3L = 0;
        ddat.v1e025c = 0;
        RG1L = signedlong(ddat.v1e0272);	// glbPlayerPosY
        put16(parw00, RG1W);	// glbPlayerPosY
        RG3L = signedlong(ddat.v1e0270);	// glbPlayerPosX
        RG2L = 0;
        DM2_MOVE_RECORD_TO(lcon(0xffff), lcon(0xffffffff), RG2L, RG3L, parw00);
        ddat.v1d27f8 = lcon(0xffffffff);	// v1d27f8 = glbMapToLoad
      }
      //m_147CC:
      skipinloop = false;
     // DM2_PROCEED_TIMERS();
      RG1L = signedlong(ddat.v1d27f8);	// RG1L = glbMapToLoad
      if (RG1L != lcon(0xffffffff))
        continue;
      DM2_UPDATE_WEATHER(0);
      if (ddat.v1e0238 == 0)	// glbIsPlayerSleeping
      {
        if (ddat.v1e0288 == 0)	// glbNextChampionNumber
          DM2_events_38c8_0060();
        if (ddat.v1e0976 == 0)	// glbChampionInventory
        {
          if (ddat.v1e0390.l_00 != 0)	// glbDoLightCheck
            RG1L = DM2_CHECK_RECOMPUTE_LIGHT();
          if (ddat.v1e025c == 0)	// !glbIsPlayerMoving ?
          {	// DISPLAY_VIEWPORT(glbPlayerDir, glbPlayerPosX, glbPlayerPosY); ?
            //m_1483C:
            RG2L = signedlong(ddat.v1e0272);
            RG4L = signedlong(ddat.v1e0270);
            RG1L = signedlong(ddat.v1e0258);
          }
          else	// DISPLAY_VIEWPORT(_4976_4c40, _4976_4c32, _4976_4c34); ?
          {
            RG2L = signedlong(ddat.v1e0268);
            RG4L = signedlong(ddat.v1e026a);
            RG1L = signedlong(ddat.v1e0278);
          }
          //m_14853:
          DM2_DISPLAY_VIEWPORT(RG1L, RG4L, RG2L);
          DM2_DRAWINGS_COMPLETED();
          ddat.v1e01d8 = false;
        }
        //m_1486D:
        if (ddat.v1e025c != 0)	// glbIsPlayerMoving
        {
          RG1L = signedlong(ddat.v1e025c);
          if (RG1L == lcon(0x1)) // glbIsPlayerMoving
            DM2_PERFORM_MOVE(ddat.v1e0256);	// PERFORM_MOVE(glbPlayerLastMove);
          RG1L = signedlong(ddat.v1d27f8);
          ddat.v1e025c--;
          if (RG1L != lcon(0xffffffff))
            continue;
        }
        //m_148A9:
        ddat.v1e048c = 0;	// _4976_4e64
        if (ddat.v1e0254 != 0)	// glbShowMousePointer
        {
          ddat.v1e0254 = wcon(0x0);
          DM2_events_443c_0434();
        }
        DM2_IBMIO_USER_INPUT_CHECK();
      }
      //m_148CE:
      DM2_move_12b4_0092();	// for SKWin I have CHOOSE_HIGHLIGHT_ARROW_PANEL instead ?
      DM2_SOUND8(false);
      DM2_PROCESS_PLAYERS_DAMAGE();
      RG1Bhi = ddat.savegames1.b_04;
      if (RG1Bhi != 0)	// glbGlobalSpellEffects.AuraOfSpeed ?
      {
        RG4Blo = RG1Bhi - 1;
        ddat.savegames1.b_04 = RG4Blo;
      }
      if (ddat.v1e0238 == 0)
        //m_14903:
        RG1L = lcon(0x3f);
      else
        RG1L = lcon(0xf);
      //m_14908:
      RG4L = unsignedlong(CUTX16(timdat.gametick));
	// 		if ((X16(glbGameTick) & ((glbIsPlayerSleeping != 0) ? 15 : 0x3f)) == 0)
      if ((RG1L & RG4L) == 0)
        DM2_UPDATE_CHAMPIONS_STATS();
      DM2_GLOBAL_UPDATE_UNKNOW1();
      RG1L = 1;
      DM2_events_2e62_0cfa(RG1L);
      if (ddat.v1e0250 != 0)	// glbPlayerDefeated ?
      {
        //m_14A81:
        DM2_10777();
        return;
      }
      timdat.gametick++;
      RG1W = CUTX16(timdat.gametick);
      RG1Bhi &= 0x1;
      RG1L = unsignedlong(RG1W);
      if (RG1L == 0)
        DM2_BURN_PLAYER_LIGHTING_ITEMS();
      RG4Bhi = ddat.savegames1.b_03;
      if (RG4Bhi != 0)	// glbGlobalSpellEffects.FreezeCounter
      {
        RG2Blo = RG4Bhi - 1;
        ddat.savegames1.b_03 = RG2Blo;
      }
      RG1W = ddat.v1e026e;
      if (RG1W != 0)	// _4976_4c00
      {
        RG4L = RG1L - 1;
        ddat.v1e026e = RG4W;
      }
      RG2W = ddat.v1e025e;
      if (RG2W != 0)	// glbPlayerThrowCounter
      {
        RG3L = RG2L - 1;
        ddat.v1e025e = RG3W;
      }
      DM2_3929_086f();
      ddat.tickevent = false;	// glbTickStepReached?
      DM2_move_12b4_0092();	// CHOOSE_HIGHLIGHT_ARROW_PANEL ????
      //m_149A3:
      for (;;)	// MessageLoop
      {
//		  skWinApplication->skwin_Sleep(1000);
		printf("MESSAGE LOOP\n");
		skWinApplication->skwin_Sleep(500);
		skWinApplication->renderScreen((X8*) gfxsys.dm2screen, 0, 0, 320, 200); // does not work here
        if (ddat.vcapture1)
        {
          bool skip00242 = false;
          RG1L = signedlong(ddat.v1e0272);
          put16(parw01, RG1W);
          RG3L = signedlong(ddat.v1e0270);
          RG1L = signedlong(ddat.v1e0258);
          RG2L = 0;
          RG1L = DM2_map_0cee_04e5(RG1L, lcon(0x1), RG2L, RG3L, parw01);
          RG4L = unsignedlong(RG1Blo);
          RG1L = unsignedlong(RG1Blo) >> 5;
          RG1L = unsignedlong(RG1Blo);
          if (RG1L != lcon(0x6))
            skip00242 = true;
          else
          {
            RG1L = RG4L;
            RG1Bhi ^= RG4Bhi;
            RG1Blo &= 0x1;
            RG1L = signedlong(RG1W);
            if (RG1L == 0)
              skip00242 = true;
            else
            {
              RG4W &= 0x4;
              RG1L = signedlong(RG4W);
              if (RG1L != 0)
                skip00242 = true;
            }
          }

          if (skip00242)
          {
            //m_149FF:
            RG3L = 0;
            ddat.vcapture1 = false;
            DM2_MOUSE_RELEASE_CAPTURE();
            Tmouse.hide();
            DM2_SHOW_MOUSE();
          }
        }
        //m_14A1A:
        DM2_EVENT_LOOP();	// MAIN_LOOP ????
        if (ddat.tickevent && ddat.v1e0248 != 0)
        {
          RG4W = ddat.v1d27f8;	// glbMapToLoad
          RG1L = signedlong(RG4W);
          if (RG1L != lcon(0xffffffff))
          {
            RG4W = ddat.v1d3248;	// iLocalMap = glbCurrentMapIndex;
            DM2_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1d27f8));
          }
          //m_14A5D:
          DM2_2759_12e6();
          RG1L = signedlong(RG4W);
          if (RG1L != lcon(0xffffffff))
            DM2_CHANGE_CURRENT_MAP_TO(RG1L);
          DM2_SOUND2(ddat.v1e0266);
          break;
        }
      }
      break;
    }
  }
  FOREND
}

// id: 0x14A8C
// arguments not needed anymore
static void DM2_MAIN(void)
{
  DM2_INIT();
  DM2_GAME_LOOP(); // TODO: doesn't return!
SPX_DEBUG_PRINT_WAIT("DM2_END_GAME\n");
  DM2_END_GAME(CUTX16(ddat.v1e0250));
}

// DM2_MAIN was called with the arguments
// pointer_at(MAIN_ARGC) and pointer_at(MAIN_ARGV)
// not needed anymore
static void DM2_FIRE_BOOTSTRAP(void)
{
  DM2_MAIN(); // TODO: doesn't return!
  DM2_FIRE_SHUTDOWN();
}

// id: 0x52334
void DM2_WATCOM_STARTUP(void) // ~IBMIO_EXEC
{
  DM2_FIRE_BOOTSTRAP(); // TODO: doesn't return!
  // WATCOM_PRE_EXIT(0, lcon(0xff));
  SYS_exit(0);
}

void DM2_RAISE_SYSERR(i16 eaxw)
{
  t_text tarr_00[0x80];
  t_text* par_tp00;

SPX_DEBUG_PRINT_WAIT("DM2_RAISE_SYSERR %02X\n", eaxw);

  ddat.v1e0218 = eaxw;
  if (ddat.v1d70d3 != 0)
    DM2_SELECT_PALETTE_SET(1);
  DM2_FORMAT_SKSTR(table1d27e8[signedlong(ddat.v1d674c)], tarr_00);
  if (ddat.v1d70dc == 0 || ddat.v1e01d4 == 0)
  {
    //m_C67E:
    // did send 256 Bytes from tarr_00 to the driver
    // DM2_COPY_TO_DRIVER(XDOWNCAST(t_text, tarr_00), lcon(0x100));
  }
  else
  {
    par_tp00 = tarr_00;
    DM2_PRINT_SYSERR_TEXT(0, lcon(0x10), lcon(0xf), 0, par_tp00);
  }
  //m_C685:
  if (ddat.v1d70cf == 0)
  {
    //m_C695:
    DM2_SLEEP_SEVERAL_TIME(300);
  }
  else
    DM2_GETKEY_TRANSLATED();
  //m_C69F:
  DM2_PREPARE_EXIT();
}

static void DM2_RESET_CAII(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;

  RG4L = 0;
  ddat.v1d4020 = RG4W;
  //m_2437A:
  for (;;)
  {
    if (RG4UW >= mkuw(ddat.v1e08a0))
    {
      RG1RP = recordptr[4];
      RG4P = DOWNCAST(s_sgwords, ddat.savegamep4);
      RG4W = word_at(RG4P, lcon(0x14));
      break;
    }
    //m_24361:
    RG2L = unsignedlong(RG4W);
    RG1L = 17 * RG2L;
    RG2P = DOWNCAST(c_creature, creatures);
    mov16(location(RG2P + 2 * RG1L), lcon(0xffffffff));
    RG4L++;
  }
  //m_2439B:
  for (;;)
  {
    RG4L--;
    if (RG4W == lcon(0xffffffff))
    {
      DM2_FILL_ORPHAN_CAII();
      return;
    }
    //m_24394:
    mov8(location(RG1P + lcon(0x5)), lcon(0xffffffff));
    RG1P += lcon(0x10);
  }
  FOREND
}

static void DM2_2f3f_0789(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;

  if (ddat.cd.mo.glbSpecialScreen == _MENU_SCREEN__RESUME_GAME_SELECT)
  {
    RG4W = ddat.savegamewpc.w_00;
    if (RG4W != lcon(0xffffffff))
    {
      //m_39E25:
      RG2W = eventqueue.event_heroidx;
      eventqueue.event_heroidx = E_NOHERO;
      RG1L = unsignedlong(RG4W);
      DM2_TAKE_OBJECT(RG1L, 1);
      eventqueue.event_heroidx = (e_hero)RG2W;
    }
    else
    {
      ddat.savegamewpc.b_16 = lcon(0xffffffff);
      DM2_events_443c_0434();
    }
    //m_39E4B:
    c_hero* hero = &party.hero[0];
    RG2L = 0;
    //m_39EA8:
    for (;;)
    {
      RG1L = signedlong(RG2W);
      RG4L = unsignedlong(party.heros_in_party);
      if (RG1L >= RG4L)
      {
        party.set_hero_flags();
        RG1L = signedlong(eventqueue.event_heroidx);
        if (RG1L == -1)
          return;
        eventqueue.event_heroidx = E_NOHERO;
        DM2_SELECT_CHAMPION_LEADER(RG1L);
        return;
      }
      //m_39E59:
      hero->heroflag &= 0x3ff;
      party.handitems.barr_00[RG1L] = -1;
      RG4L = RG1L;
      ddat.v1e0b1c[RG1L] = 0;
      v1d67be[RG1L] = lcon(0xffffffff);
      RG1L = 3 * RG4L;
      ddat.v1e0b34[RG4L][1] = 0;
      party.handitems.warr2d_00[RG4L + 1][1] = -1;
      ddat.v1e0b34[RG4L][0] = 0;
      party.handitems.warr2d_00[RG4L + 1][0] = -1;
      RG4W = unsignedword(RG4Blo);
      ddat.v1e0b34[RG1L / 3][2] = RG4Bhi;
      RG2L++;
      hero++; // struct advance
    }
    FOREND
  }
  //m_39EE8:
  ddat.savegamewpc.w_00 = lcon(0xffffffff);
  ddat.savegamewpc.b_16 = lcon(0xffffffff);
  RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(0, 0));
  //m_39F01:
  for (;;)
  {
    RG4L = RG1L;
    if (RG4W == lcon(0xfffffffe))
      return;
    RG1L = RG4L;
    RG1Blo ^= RG4Blo;
    RG1Bhi &= lcon(0x3c);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    if (RG1L == lcon(0x3))
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
      RG1W = word_at(RG1P, 2) & lcon(0x7f);
      if (RG1W == lcon(0x7e))
      {
        RG4L = 1;
        ddat.v1d6a2d = RG4L;
        DM2_SELECT_CHAMPION(0, RG4L, 0, signedlong(ddat.v1e0266));
        DM2_events_2f3f_04ea(0, 1, 0, signedlong(ddat.v1e0266), lcon(0x92));
        ddat.v1d6a2d = 0;
        RG1Blo = CUTX8(ddat.v1e0258);
        party.hero[0].absdir = party.hero[0].partypos = RG1Blo;
        party.set_hero_flags();
        return;
      }
    }
    //m_39F9A:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
  }
  FOREND
}

// was SKW_38c8_03ad
i32 DM2__INIT_GAME_38c8_03ad(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  bool skip00980 = false;

  RG4L = 0;
  ddat.vcapture3 = false;
  ddat.vcapture2 = false;
  RG3L = 0;
  ddat.vcapture1 = false;
  ddat.v1e03a8 = 0;
  ddat.v1e0238 = 0;
  ddat.v1e13f0 = 0;
  DM2_ZERO_MEMORY(&ddat.v1e0ffa[2], lcon(0x10)); // EVIL offset shifted, was 0x1e0ffc
  DM2_ZERO_MEMORY(ddat.v1e0adc, lcon(0x40));
  RG4L = 0;
  party.curacthero = RG4W;
  RG2L = 0;
  ddat.v1e00b8 = RG2W;
  DM2_1031_0541(5);
  DM2_LOAD_NEWMAP(signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), signedlong(ddat.v1e0266), RG3L);
  DM2_RESET_CAII();
  dm2_dballochandler.v1e0200 = false;
  if (ddat.cd.mo.glbSpecialScreen != _MENU_SCREEN__RESUME_GAME_SELECT)
  {
    RG4L = ddat.savegamel2 ? 1 : 0;
    if (RG4L == 0)
    {
      //m_45386:
      DM2_WAIT_SCREEN_REFRESH();
      ddat.dialog2 = 0;
      skip00980 = true;
    }
  }

  if (!skip00980)
  {
    //m_45329:
    RG4L = 0;
    RG1L = 2;
    DM2_FILL_SCREEN_RECT(/*TAG QUERY*/ RG1W, ui8_to_pixel(RG4UBlo));
    RG1L = DM2_0aaf_02f8(lcon(0x6), 0) & lcon(0xff);
    DM2_0aaf_0067(RG1L);
    RG4W = gdat.fileopencounter;
    if (RG4W != 0)
    {
      gdat.fileopencounter = 1;
      DM2_GRAPHICS_DATA_CLOSE();
    }
    //m_45365:
    RG1L = 1; // SKW_2066_03e0 solved here
    if (RG4W != 0)
    {
      DM2_GRAPHICS_DATA_OPEN();
      gdat.fileopencounter = RG4W;
    }
    //m_4537D:
    DM2_WAIT_SCREEN_REFRESH();
    RG4L = 0;
  }

  //m_45393:
  RG1L = 2;
  DM2_FILL_SCREEN_RECT(/*TAG QUERY*/ RG1W, ui8_to_pixel(RG4UBlo));
  DM2_guidraw_29ee_000f();
  DM2_2f3f_0789();
  RG3L = 1;
  ddat.v1e13f4 = RG3L;
  ddat.v1e0248 = RG3L;
  return DM2_CHECK_RECOMPUTE_LIGHT();
}

// was SKW_101b_0001
void DM2_END_GAME(i16 eaxw)
{
  DM2_HIDE_MOUSE();
  if (eaxw != 0)
  {
    if (ddat.v1e0240 == 0)
    {
      //m_FFFB:
      DM2_QUEUE_NOISE_GEN2(22, (party.heros_in_party <= 0) ? -2 : party.hero[0].herotype, bcon(0x87), bcon(0xfe), ddat.v1e0270, ddat.v1e0272, 0, lcon(0xff), lcon(0xff));
      DM2_SLEEP_SEVERAL_TIME(240);
    }
  }
  //m_10013:
  ddat.dialog2 = 1;
  if (eaxw != 0)
  {
    DM2_FILL_SCREEN_RECT(/*TAG QUERY*/ 2, c_pixel(E_COL00));
    if (ddat.v1e13f4 != 0)
    {
      ddat.savegamel2 = DM2_0aaf_0067(DM2_0aaf_02f8(16, 0) & lcon(0xff)) == 0;
      if (ddat.savegamel2)
      {
        if (DM2_GAME_LOAD() != -1)
        {
          DM2__INIT_GAME_38c8_03ad();
          ddat.savegamel2 = false;
          DM2_SHOW_MOUSE();
          eventqueue.event_1031_098e();
          return;
        }
      }
    }
  }
  //m_1008D:
  DM2_FILL_SCREEN_RECT(/*TAG QUERY*/ 2, c_pixel(E_COL00));
  DM2_SELECT_PALETTE_SET(0);
  if (ddat.v1e08fc != 0)
    DM2_PREPARE_EXIT();
  if (eaxw != 0)
  {
    ddat.v1e0438 = 1;
    DM2_PREPARE_EXIT();
  }
  DM2_DRAW_TRANSPARENT_STATIC_PIC(6, 0, 1, 2, NOALPHA);
  DM2_SELECT_PALETTE_SET(1);
  DM2_1031_0541(1);
  //m_100EE:
  do
  {
    DM2_EVENT_LOOP();
  } while (eventqueue.event_unk0a != lcon(0xef));
  close_win = true;
  throw(THROW_CLOSEWIN); //DM2_PREPARE_EXIT();
}

// belongs to DM2_HANDLE_UI_EVENT
void DM2_SHOW_CREDITS(void)
{
  DM2_SELECT_PALETTE_SET(0);
  DM2_HIDE_MOUSE();
  R_C470(ddat.glbImageCreditScreen, 2, -1, paldat.glbl_pal2);
  DM2_SELECT_PALETTE_SET(1);
  DM2_SHOW_MOUSE();
  DM2_1031_0541(1);
  //m_28970:
  i16 n = 1800;
  do
  {
    DM2_EVENT_LOOP();
    if (eventqueue.event_unk0a == lcon(0xef))
      break;
    DM2_SLEEP_SEVERAL_TIME(1);
  } while (--n != 0);
  //m_28992:
  ddat.cd.mo.glbSpecialScreen = lcon(_MENU_SCREEN__SHOW_CREDITS);
  DM2_1031_0541(0);
  DM2_HIDE_MOUSE();
}
