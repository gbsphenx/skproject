#include <allegro5.h>
#include <allegro_audio.h>

#include "util.h"
#include "c_midi.h"
#include "c_alloc.h"
#include "c_dballoc.h"
#include "dm2data.h"
#include "startend.h" // THROW...
#include "c_sound.h"

// ---------------
// function is obsolete
//i8* getsind2(i32 n, ui32 ofs)
//{
//  return &memory[0x143000 + table1d83ec[n] + ofs];
//}
// ---------------

c_sound dm2sound;

c_sound::c_sound(void)
:
  sndptr1(NULL),
  xsndptr2(NULL),
  xsndptr3(NULL),
  sndptr4(NULL),
  xsndptr5(NULL),
  sndptr6(NULL)
{
}

bool c_sound::init(void)
{
  v1d2698 = 0;
  v1d269a = 0;
  v1e0ace = 0;
  v1d269c = 0;

  if (!init_sfx()) return false;
  init_midi();
  return true;
}

void c_sound::stop_all_sound(void)
{
  stop_sfx();
  stop_music();
}

void c_sound::dtor(void)
{
  al_uninstall_audio();
}

// ###########################################

// TODO needed externals:
//extern bool SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(x8 eaxb, x8 ebxb, x8 ecxb, x8 edxb);
extern bool SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(x8 eaxb, ui16 ebxb, ui16 ecxb, ui16 edxb);

// MIDI -------------------------------

static void R_5044A(x8 eaxb)
{
  //x32 vl_00;
  x32 vl_04;
  x8 vb_08;

  vb_08 = eaxb;
  ddata.v1dd1d1 = vb_08;
  vl_04 = con(0x0);
  while (vl_04 < con(0x8))
  {
    if (ddata.longv1da334[vl_04] != con(0x0)) // TODO: what for? all entries are 0 and stay so
    {
      // R_520C3(vl_04, unsignedlong(CUTLX8(ddata.longv1dd1d2[vl_04]))); // DM's MIDI stuff extracted
    }
    vl_04++;
  }
}

static void R_51AF6(x16 edxw)
{
// write is obsolete, because never read again
// mov16(&memory[0x143000 + 0x12f8 + 0x4], edxw);
}

static void R_4FF39(x16 eaxw)
{
  if (eaxw < con(0x8))
  {
    ddata.v1da374[eaxw].w4 = con(0x0);
    ddata.v1da374[eaxw].l0 = con(0x0);
  }
}

static void R_B65(void)
{
  if (ddata.v1d14c2 && ddata.v1d14d0)
  {
    // R_50012(ddata.v1dff2c); // DM's MIDI extracted
    R_4FF39(ddata.v1dff2c);
    ddata.v1d14d0 = false;
    ddata.v1d14cc = false;
    ddata.v1d14ca = con(0x0);
  }
}

static void R_A0E(x8 eaxb)
{
  x8 vb_00;

  vb_00 = eaxb;
  if (ddata.v1d14c2 && vb_00 != 0 && ddata.v1dff86 > con(0x0))
  {
    R_B65();
    if (dm2sound.sndptr6 == NULL)
      return;
    x32 longrg1 = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0x4), CUTLX8(con(0x3)), con(0x0), vb_00) ? 1 : 0;
#if 1 // TODO
    dm2sound.stop_music();
#endif
    if (longrg1 == con(0x0))
      return;
#if 1 // TODO
    dm2sound.do_music(unsignedword(vb_00));
    return;
#else
  $  x32 longrg6 = unsignedlong(SKW_QUERY_GDAT_ENTRY_DATA_LENGTH(con(0x4), con(0x3), con(0x0), vb_00));
  $  SKW_COPY_MEMORY(SKW_QUERY_GDAT_ENTRY_DATA_PTR(con(0x4), con(0x3), con(0x0), vb_00), longrg6, dm2sound.sndptr6);
  $  ddata.v1dfea8 = DSZERO;
  $  ddata.v1dfea4 = dm2sound.sndptr6;
  $  ddata.v1dfeb0 = con(0x0);
  $  ddata.v1dfeac = con(0x0);
  $  parl01 = DSZERO;
  $  parp00 = &ddata.v1dff2c;
  $  RG2P = ddata.v1d1378;
  $  RG1P = &ddata.v1dfea4;
  $  RG1L = con(0x0); // was calling R_4F75E, result 0x0 or 0xe
  $  ddata.v1dff34 = RG1L;
  $  if (RG1L != con(0x0)) return;
  $  RG1L = ddata.v1dff2c;
  $  RG1L = con(0x0); // TODO old call of R_4FF83, result can be con(0xb) to, removed, MIDI!!
  $  ddata.v1dff34 = RG1L;
  $  if (RG1L != con(0x0)) return;
  $  RG1L = ddata.v1dff2c;
  $  // R_520C3(RG1L, con(0x7f)); // DM's MIDI stuff extracted
  $  RG1L = ddata.v1dff2c;
  $  // R_51F94(RG1L); // DM's MIDI stuff extracted
  $  ddata.v1d14d4 = true;
  $  ddata.v1d14d0 = true;
#endif
  }
//M_B27:
#if 0
$  ddata.v1d14d8 = unsignedword(vb_00);
#endif
//M_A05:
}

void R_7E7(x16 eaxw, x16 edxw)
{
  x16 wordrg2 = eaxw;
  if (eaxw > con(0x7))
    wordrg2 = con(0x7);
  if (wordrg2 < con(0x0))
    wordrg2 = con(0x0);
  if (edxw != 0)
  {
    if (edxw != con(0xa))
      return;
    if (ddata.v1d14c2)
    {
      R_5044A((2 * CUTX8(wordrg2)) | (CUTX8(wordrg2) << con(0x4)));
      if (ddata.v1d14d4)
      {
        if (wordrg2 != con(0x0))
        {
          if (ddata.v1dff86 == con(0x0) && ddata.v1d14d8 >= con(0x0))
          {
            ddata.v1dff86 = wordrg2;
            R_A0E(CUTX8(ddata.v1d14d8));
          }
        }
        else
          R_B65();
      }
    }
    ddata.v1dff86 = wordrg2;
    dm2sound.set_midi_volume(36 * ddata.v1dff86);
    return;
  }
  if (ddata.v1d14be)
  {
    // x32 longrg1 = signedlong(wordrg2);
    // R_51AF6(CUTX16((8 * longrg1) | (longrg1 << con(0x6)) | (longrg1 << con(0x9)) | (longrg1 << con(0xc)) | longrg1));
    ddata.v1dff88 = wordrg2;
  }
}

static void R_928(s_sfx* eaxp)
{
  x32 vl_00;

  x16 wordrg5 = signedword(eaxp->b6);
  x16 wordrg2 = signedword(eaxp->b7);
  s_bw* sptrrg3 = &eaxp->s8;
  vl_00 = lextended(wordrg5 * wordrg5);
  x32 longrg6 = UI32CAST(wordrg2 * wordrg2 + CUTX16(vl_00)) + con(0x8);
  sptrrg3->b0 = CUTLX8(((unsignedlong(eaxp->b5) << con(0x8)) / longrg6) >> con(0x5));
  if (wordrg5 == con(0x0))
  {
    sptrrg3->w1 = con(0xffff8000);
    return;
  }
  if (wordrg2 == con(0x0))
  {
    x16 wordrg11;
    if (wordrg5 >= con(0x0))
      wordrg11 = table1d14e2[0xf+0x8]; // EVIL: shifted by index 0x8
    else
      wordrg11 = table1d14e2[0x8]; // EVIL: shifted by index 0x8
    sptrrg3->w1 = wordrg11;
    return;
  }

  bool boolrg6 = true;
  if (wordrg5 < con(0x0))
  {
    wordrg5 = -wordrg5;
    boolrg6 = false;
  }
  if (wordrg2 < con(0x0))
    wordrg2 = -wordrg2;
  x32 longrg2 = unsignedlong(wordrg2);
  wordrg5 = CUTX16((unsignedlong(wordrg5) << con(0xb)) / longrg2);
  x16 wordrg4 = con(0x0);
  for(;;)
  {
    x32 longrg1 = 2 * unsignedlong(wordrg4);
    if (wordrg5 >= table1d14e2[longrg1 / 2]) // TODO: BUG! gives endless loops!!!
    {
      x16 wordrg1;
      if (!boolrg6)
        wordrg1 = table1d14e2[0x8 + longrg1 / 2]; // EVIL: shifted by index 0x8
      else
        wordrg1 = table1d14e2[0x8 + (con(0xf) - unsignedlong(wordrg4))]; // EVIL: shifted by index 0x8
      sptrrg3->w1 = wordrg1;
      return;
    }
    wordrg4++;
  }
}

static bool R_8FE(s_sfx* eaxp, s_sfx* edxp)
{
  ui8 ubyterg21 = eaxp->b4;
  ui8 ubyterg22 = edxp->b4;
  if (ubyterg21 > ubyterg22)
    return true;
  else if (ubyterg21 == ubyterg22)
    return eaxp->s8.b0 >= edxp->s8.b0;
  return false;
}

static x16 R_5096A(x16 edxw)
{
  x16 vw_04;
  x16 vw_08;

  vw_04 = edxw;
  if (vw_04 >= con(0x20))
    return con(0xa);
  // all tableentries are 0!
//  if ((word_at(getsind2(vl_04, con(0x30))) & con(0x8000)) != 0)
//    vl_08 = con(0x0);
//  else
    vw_08 = con(0x1);
  return vw_08;
}

// was SKW_47eb_014f
// TODO: vw_00 is requested one time as a byte only, which makes it in fact byterange only
void SKW_PLAY_SOUND(ui16 eaxw, s_sfx* edxp)
{
  ui16 vw_00;
  x32 vl_04;
  x16 vw_08;
  x16 vw_0c;

  vw_00 = eaxw;
  s_sfx* sptrrg6 = edxp;
  if (!ddata.v1d14be || ddata.v1dff88 == con(0x0) || eaxw == con(0x0))
    return;
  ui8 ubyterg3 = con(0x0);
  s_sfx* sptrrg23 = sptrrg6;
  while (ubyterg3 < CUTX8(vw_00))
  {
    R_928(sptrrg23);
    sptrrg23->bb = ubyterg3;
    ubyterg3++;
    sptrrg23++;
  }
  bool boolrg7;
  do
  {
    boolrg7 = false;
    vw_0c = unsignedword(sptrrg6->bb);
    ui16 uwordrg32 = con(0x1);
    s_sfx* sptrrg22 = sptrrg6 + 1;
    while (uwordrg32 < vw_00)
    {
      vw_08 = unsignedword(sptrrg22->bb);
      if (R_8FE(&sptrrg6[vw_0c], &sptrrg6[sptrrg22->bb]))
        vw_0c = vw_08;
      else
      {
        s_sfx* sptr = sptrrg22 - 1; // EVIL
        sptr->bb = CUTX8(vw_08);
        sptrrg22->bb = CUTX8(vw_0c);
        boolrg7 = true;
      }
      uwordrg32++;
      sptrrg22++;
    }
  } while (boolrg7);
  vl_04 = con(0x0);
  x32 longrg5;
  for(;;)
  {
    if (UI16CAST(vl_04) >= vw_00)
      return;
    x32 longrg43 = unsignedlong(CUTX16(vl_04));
    vl_04 = longrg43 + 1;
    s_pwwp* sptrrg2 = UPCAST(s_pwwp, sptrrg6 + unsignedlong(sptrrg6[longrg43].bb));
    s_bwb* sptrrg7 = &sptrrg2->u.s8;
    longrg5 = con(0xffffffff);
    ui16 uwordrg3 = con(0x0);

    x32 longrg1;
    while ((longrg1 = signedlong(uwordrg3)) < con(0x40))
    {
      x32 wordrg42 = ddata.v1dfda4[longrg1];
      if (wordrg42 == con(0xffffffff) || R_5096A(wordrg42) == con(0x1))
      {
        longrg5 = lextended(uwordrg3);
        break;
      }
      uwordrg3++;
    }

    x32 longrg41 = signedlong(CUTX16(longrg5));
    if (longrg41 == con(0xffffffff))
      return;
  #if 1 // TODO
    dm2sound.do_sound(CUTX8(ddata.v1dff88), P8CAST(sptrrg2)); // TODO
  #endif
    ddata.v1d1410 = CUTX16(longrg5);
    ddata.v1d1400 = sptrrg2->w4;
    ddata.v1d1420 = con(0x320);
    x32 longrg12 = unsignedlong(sptrrg7->b0);
    longrg5 = longrg12 << con(0x7);
    longrg12 >>= con(0x1);
    longrg5 |= longrg12;
    ddata.longv1d140c = longrg5;
    ddata.v1d1438 = sptrrg7->w1;
    ddata.longv1d143c = con(0x0);
    // ddata.v1d13fc = DSZERO;
    ddata.v1d13f8 = sptrrg2->p0;
    // RG1L = ddata.longv1dff24;
    // RG2P = &ddata.v1d13f8;
    x16 wordrg13 = con(0x0); // TODO: was a call, can return -1 as well. (if sample is still running?)
    ddata.v1dfda4[longrg41] = wordrg13;
  }
}


void R_B37(void)
{
  if (ddata.v1d14c2 && ddata.v1d14d0)
  {
    // R_51BDF(ddata.v1dff2c, con(0x1)); // DM's MIDI stuff extracted. TODO: check what it wants to do here
    ddata.v1d14cc = true;
  }
}

void R_BA7(x16 eaxw)
{
  if (!ddata.v1d14c2 || !ddata.v1d14da)
    return;
  if (ddata.v1d14cc)
  {
    // R_51D42(ddata.v1dff2c);  DM's MIDI stuff extracted. TODO: check what it wants to do here
    ddata.v1d14cc = false;
  }
  x32 longrg1 = signedlong(ddata.v1d14ca);
  if (longrg1 <= con(0x1))
  {
    if (longrg1 == con(0x1))
    {
      ddata.v1d1512 = ddata.v1dff8a;
      R_A0E(ddata.v1d1512);
      ddata.v1d14ca = con(0x0);
      return;
    }
  }
  else
  {
    // R_520C3(ddata.v1dff2c, unsignedlong(CUTX8(ddata.v1d14ca)); // DM's MIDI stuff extracted TODO: check what it wants to do here
    ddata.v1d14ca--;
  }
  ddata.v1dff8a = table1410ec[eaxw];
  if (ddata.v1d1512 != ddata.v1dff8a)
  {
    if (!ddata.v1d14d0 || ddata.v1d14ca != con(0x0))
      R_A0E(ddata.v1dff8a);
    else
      ddata.v1d14ca = con(0x7f);
    ddata.v1d1512 = ddata.v1dff8a;
  }
}

// SFX -------------------------------

static void R_51083(x16 edxw)
{
  x16 vw_04;
  //x32 vl_08;

  vw_04 = edxw;
  if (vw_04 >= con(0x20))
    return;
  // all table-entries are 0!
//  if ((word_at(getsind2(vw_04, con(0x30))) & con(0x8000)) == 0)
//    return;  // so this return is chosen
//  if (jz_test8(getsind2(vw_04, con(0x31)), con(0x10)))
//  {
//    and8(getsind2(vw_04, con(0x31)), con(0x7f));
//    mov16(getsind2(vw_04, con(0x34)), con(0x0));
//  }
}

static x32 R_51B56(void)
{
  x32 vl_08;
  x32 vl_0c;

  vl_08 = con(0x0);
  vl_0c = con(0x0);
  while (UI32CAST(vl_0c) < UI32CAST(con(0x20)))
  {
// all entries are 0, so condition is always no
//    if ((word_at(getsind2(vl_0c, con(0x30))) & con(0x8000)) != 0)
//      vl_08++;
    vl_0c++;
  }
  return vl_08; // returns initial value then, which is 0
}

static void R_8E6(void) // obsolete, because...
{
  if (ddata.v1d14be)
    while (R_51B56() != 0); // ...return is always 0, so call not needed
}

void SKW_482b_060e(void)
{
  // R_8E6();
  ddata.v1d2694 = con(0x0);
}

static void R_8AF(void)
{
  if (!ddata.v1d14be)
    return;
  x16 wordrg2 = con(0x0);
  x16 wordrg1;
  while ((wordrg1 = wordrg2) < con(0x40))
  {
    if (ddata.v1dfda4[wordrg1] != con(0xffffffff))
      R_51083(ddata.v1dfda4[wordrg1]);
    wordrg2++;
  }
}

static void R_5F7(s_pwwp* eaxp)
{
  R_8AF();
  s_pwwp* sptrrg1 = dm2_dballochandler.v1dff38;
  for(;;)
  {
    if ((sptrrg1 == NULL) || (eaxp == sptrrg1))
      break;
    sptrrg1 = sptrrg1->u.p8;
  }
  s_pwwp* sptrrg5 = sptrrg1;
  if (eaxp == sptrrg5)
    sptrrg1 = sptrrg5->u.p8;
}

void SKW_482b_07c2(void)
{
  ui16 uwordrg2 = con(0x0);
  for(;;)
  {
    if (dm2sound.v1d2698-- <= uwordrg2)
      break;
    x16 wordrg1 = dm2sound.xsndptr2[dm2sound.v1d2698].w5;
    if (wordrg1 != con(0xffffffff))
    {
      if (SKW_482b_015c(wordrg1) == con(0x0))
      {
        R_5F7(UPCAST(s_pwwp, dm2sound.sndptr4 + 16 * unsignedlong(dm2sound.xsndptr2[dm2sound.v1d2698].w0)));
        dm2sound.v1d269a--;
      }
    }
  }
  dm2sound.v1d2698 = uwordrg2;
  ddata.v1d2694 = con(0x0);
}

void SKW_482b_0004(void)
{
	SPX_DEBUG_FUNCTION_INFO("SKW_482b_0004");
  dm2sound.sndptr4 = DM2_ALLOC_FREEPOOL_MEMORY(16 * unsignedlong(dm2sound.v1e0ace), true);
  dm2sound.xsndptr2 = UPCAST(s_size7, DM2_ALLOC_FREEPOOL_MEMORY(7 * unsignedlong(dm2_dballochandler.v1e0ad6), true));
  dm2sound.xsndptr5 = UPCAST(s_sfx, DM2_ALLOC_FREEPOOL_MEMORY(con(0xf0), true));
  dm2sound.sndptr1 = UPCAST(s_size10, DM2_ALLOC_FREEPOOL_MEMORY(con(0x70), true));
  DM2_ZERO_MEMORY(DOWNCAST(s_size10, dm2sound.sndptr1), con(0x70));
  dm2sound.xsndptr3 = UPCAST(s_sfx, DM2_ALLOC_FREEPOOL_MEMORY(con(0x48), true));
  dm2sound.sndptr6 = DM2_ALLOC_FREEPOOL_MEMORY(unsignedlong(dm2_dballochandler.v1e0ad4));
	SPX_DEBUG_POP;
}

ui16 SKW_482b_015c(x16 eaxw)
{
  ui16 uwordrg4 = con(0x0);
  x32 longrg2;
  do
  {
    if (uwordrg4 >= dm2sound.v1d2698)
      return con(0x0);
    longrg2 = 7 * unsignedlong(uwordrg4);
    uwordrg4++;
  } while (eaxw != dm2sound.xsndptr2[longrg2 / 7].w5);
  return uwordrg4;
}

// was SKW_482b_04e9
void SKW_PROCESS_SOUND(x16 eaxw)
{
  x16 wordrg1 = signedword(dm2sound.sndptr1[eaxw].b4[3]);
  if (wordrg1 == ddata.v1e0282 || wordrg1 == ddata.v1e027c)
  {
    s_size10* sptrrg1 = &dm2sound.sndptr1[eaxw];
    x32 longrg3 = signedlong(sptrrg1->wa);
    x32 longrg2 = unsignedlong(sptrrg1->b4[2]);
    x32 longrg4 = unsignedlong(sptrrg1->b4[1]);
    SKW_QUEUE_NOISE_GEN1(sptrrg1->b4[0], CUTLX8(longrg2), CUTX16(longrg3), CUTLX8(longrg4), sptrrg1->wc, signedword(sptrrg1->b4[4]), signedword(sptrrg1->b4[5]), con(0x1));
  }
  dm2sound.sndptr1[eaxw].u0.p = NULL;
}

void SKW_482b_05bf(bool eaxbool)
{
  if (eaxbool)
  {
    if (ddata.v1d2696 > con(0x0))
      SKW_PLAY_SOUND(ddata.v1d2696, dm2sound.xsndptr3);
    ddata.v1d2696 = con(0x0);
  }
  else
  {
    if (ddata.v1d2694 > con(0x0))
      SKW_PLAY_SOUND(ddata.v1d2694, dm2sound.xsndptr5);
    ddata.v1d2694 = con(0x0);
  }
}
