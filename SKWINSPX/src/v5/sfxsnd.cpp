// FILE c_sound.cpp

#ifndef __SK_NO_ALLEGRO__
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#else
#include <skalegro.h>
#endif // __SK_NO_ALLEGRO__

#include <util.h>
#include <sfxmidi.h>
#include <skalloc.h>
#include <skalocdb.h>
#include <dm2data.h>
#include <startend.h> // THROW...
#include <sfxsnd.h>

// ---------------
// function is obsolete
//i8* getsind2(i32 n, ui32 ofs)
//{
//  return &memory[0x143000 + table1d83ec[n] + ofs];
//}
// ---------------

void c_lw::init(void)
{
  l_00 = 0;
  w_04 = 0;
}

// ################################################################

// this table has an overlap. at some points 0x1d14f2 is adressed,
// but the index from 0x1d14e2 runs into this. EVIL adjusted.
ui16 table1d14e2[24] =
{
  0x25a0, 0x11f0, 0x0b00, 0x0730, 0x0490, 0x0290, 0x00d0, 0x0000,
  0x0800, 0x1700, 0x2600, 0x3500, 0x4400, 0x5300, 0x6200, 0x7100,
  0x8f00, 0x9e00, 0xad00, 0xbc00, 0xcb00, 0xda00, 0xe900, 0xf800
};

// ################################################################

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
  i16 i = 0;
  v1d2698 = 0;
  v1d269a = 0;
  v1e0ace = 0;
  v1d269c = 0;
  v1dd1d1 = 90;
  for (i = 0; i < 8; i++) v1da334[i] = 0;
  for (i = 0; i < 8; i++) v1da374[i].init();
  v1d14c2 = DOWNCAST(special, 0x1);
  v1d14da = true;
  v1d14cc = false;
  v1d14d4 = false;
  v1d14d0 = false;
  v1dff86 = 5;
  for (i = 0; i < 8; i++) v1dd1d2[i] = 0x7f;
  v1dff2c = 0;
  v1d14ca = 0;
  v1d14d8 = -1;
  v1d14be = true;
  v1dff88 = 7;
  v1dff8a = 0;
  for (i = 0; i < 64; i++) v1dfda4[i] = -1;
  v1d1410 = 0x1000;
  v1d2694 = 0;
  v1d2696 = 0;
  v1d140c = 0x7fff;
  v1d1512 = 0xff;
  v1d13f8 = NULL;
  v1d143c = false;
  v1d1438 = 0;
  v1d1420 = 0;
  v1d1400 = 0;

  if (!init_sfx())
    return false;
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
extern bool DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(i8 eaxb, i8 edxb, i8 ebxb, i8 ecxb);

// MIDI -------------------------------

static void R_5044A(i8 eaxb)
{
  i8 vb_08 = eaxb;
  dm2sound.v1dd1d1 = vb_08;
  i32 vl_04 = 0;
  while (vl_04 < 8)
  {
    if (dm2sound.v1da334[vl_04] != 0) // TODO: what for? all entries are 0 and stay so
    {
      // R_520C3(vl_04, unsignedlong(CUTLX8(dm2sound.v1dd1d2[vl_04]))); // DM's MIDI stuff extracted
    }
    vl_04++;
  }
}

static void R_51AF6(i16 edxw)
{
// write is obsolete, because never read again
// mov16(&memory[0x143000 + 0x12f8 + 0x4], edxw);
}

static void R_4FF39(i32 eaxl)
{
  if (mkul(eaxl) < 8)
  {
    dm2sound.v1da374[eaxl].w_04 = 0;
    dm2sound.v1da374[eaxl].l_00 = 0;
  }
}

static void R_B65(void)
{
  if (dm2sound.v1d14c2 != NULL && dm2sound.v1d14d0)
  {
    // R_50012(dm2sound.v1dff2c); // DM's MIDI extracted
    R_4FF39(dm2sound.v1dff2c);
    dm2sound.v1d14d0 = false;
    dm2sound.v1d14cc = false;
    dm2sound.v1d14ca = 0;
  }
}


extern int __GlobalOption_DM2Skull_MusicMode;	// from main.cpp
//R_A0E renamed DM2_PLAY_MUSIC
/*static*/ void DM2_PLAY_MUSIC(i8 eaxb, bool bForceMusic)
{
  unk* xparp00;

  i8 vb_00 = eaxb;
  //if (dm2sound.v1d14c2 != NULL && vb_00 != 0 && dm2sound.v1dff86 > 0)
  if (dm2sound.v1d14c2 != NULL && (vb_00 != 0 || bForceMusic == true) && dm2sound.v1dff86 > 0)	// if bForceMusic, then don't test vb_00
  {
    R_B65();
    if (dm2sound.sndptr6 == NULL)
      return;
	//i32 longrg1 = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x4), vb_00, CUTLX8(lcon(0x3)), 0) ? 1 : 0;
    i32 longrg1 = 1;	// default to 1 to make it pass the 0 test
if (__GlobalOption_DM2Skull_MusicMode == 0 && bForceMusic == false)	// check if HMP exists only when music mode is for HMP
	longrg1 = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_MUSICS, vb_00, dtHMP, 0) ? 1 : 0;
#if 1 // TODO
    dm2sound.stop_music();
#endif
    if (longrg1 == 0)
      return;
#if 1 // TODO
    dm2sound.do_music(unsignedlong(vb_00));
    return;
#else
  $  i32 parl01;
  $  i32 longrg6 = unsignedlong(DM2_QUERY_GDAT_ENTRY_DATA_LENGTH(lcon(0x4), vb_00, lcon(0x3), 0));
  $  DM2_COPY_MEMORY(dm2sound.sndptr6, DM2_QUERY_GDAT_ENTRY_DATA_PTR(lcon(0x4), vb_00, lcon(0x3), 0), longrg6);
  $  ddat.v1dfea8 = DSZERO;
  $  ddat.v1dfea4 = dm2sound.sndptr6;
  $  ddat.v1dfeb0 = 0;
  $  ddat.v1dfeac = 0;
  $  parl01 = DSZERO;
  $  parp00 = &dm2sound.v1dff2c;
  $  RG2P = ddat.v1d1378;
  $  RG1P = &ddat.v1dfea4;
  $  RG1L = 0; // was calling R_4F75E, result 0x0 or 0xe
  $  ddat.v1dff34 = RG1L;
  $  if (RG1L != 0) return;
  $  RG1L = dm2sound.v1dff2c;
  $  RG1L = 0; // TODO old call of R_4FF83, result can be lcon(0xb) to, removed, MIDI!!
  $  ddat.v1dff34 = RG1L;
  $  if (RG1L != 0) return;
  $  RG1L = dm2sound.v1dff2c;
  $  // R_520C3(RG1L, lcon(0x7f)); // DM's MIDI stuff extracted
  $  RG1L = dm2sound.v1dff2c;
  $  // R_51F94(RG1L); // DM's MIDI stuff extracted
  $  dm2sound.v1d14d4 = true;
  $  dm2sound.v1d14d0 = true;
#endif
  }
//M_B27:
#if 0
$  dm2sound.v1d14d8 = unsignedword(vb_00);
#endif
//M_A05:
}

// was R_7E7
void DM2_SOUND3(i16 eaxw, i16 edxw)
{
  i16 wordrg2 = eaxw;
  if (eaxw > 7)
    wordrg2 = 7;
  if (wordrg2 < 0)
    wordrg2 = 0;
  if (edxw != 0)
  {
    if (edxw != lcon(0xa))
      return;
    if (dm2sound.v1d14c2 != NULL)
    {
      R_5044A(2 * CUTX8(wordrg2) | CUTX8(wordrg2) << 4);
      if (dm2sound.v1d14d4)
      {
        if (wordrg2 != 0)
        {
          if (dm2sound.v1dff86 == 0 && dm2sound.v1d14d8 >= 0)
          {
            dm2sound.v1dff86 = wordrg2;
            DM2_PLAY_MUSIC(CUTX8(dm2sound.v1d14d8));
          }
        }
        else
          R_B65();
      }
    }
    dm2sound.v1dff86 = wordrg2;
    dm2sound.set_midi_volume(36 * dm2sound.v1dff86);
    return;
  }
  if (dm2sound.v1d14be)
  {
    // i32 longrg1 = signedlong(wordrg2);
    // R_51AF6(CUTX16((8 * longrg1) | (longrg1 << 6) | (longrg1 << bcon(0x9)) | (longrg1 << bcon(0xc)) | longrg1));
    dm2sound.v1dff88 = wordrg2;
  }
}

static void R_928(s_sfx* eaxp_s60)
{
  i32 vl_00;

  i16 wordrg5 = signedword(eaxp_s60->ub_06);
  i16 wordrg2 = signedword(eaxp_s60->ub_07);
  s_bw* s59prg3 = &eaxp_s60->s59_08;
  vl_00 = mkl(wordrg5 * wordrg5);
  i32 longrg6 = mkul(wordrg2 * wordrg2 + CUTX16(vl_00)) + lcon(0x8);
  s59prg3->ub_00 = CUTLX8((unsignedlong(eaxp_s60->ub_05) << bcon(0x8)) / longrg6 >> 5);
  if (wordrg5 == 0)
  {
    s59prg3->w_01 = wcon(0x8000);
    return;
  }
  if (wordrg2 == 0)
  {
    i16 wordrg11;
    if (wordrg5 >= 0)
      wordrg11 = table1d14e2[0xf + 0x8]; // EVIL: shifted by index 0x8
    else
      wordrg11 = table1d14e2[0x8]; // EVIL: shifted by index 0x8
    s59prg3->w_01 = wordrg11;
    return;
  }

  bool boolrg6 = true;
  if (wordrg5 < 0)
  {
    wordrg5 = -wordrg5;
    boolrg6 = false;
  }
  if (wordrg2 < 0)
    wordrg2 = -wordrg2;
  i32 longrg2 = unsignedlong(wordrg2);
  ui16 uwordrg5 = CUTX16((unsignedlong(wordrg5) << bcon(0xb)) / longrg2); // BUGFIX 2016
  i16 wordrg4 = 0;
  for (;;)
  {
    i32 longrg1 = 2 * unsignedlong(wordrg4);
    if (uwordrg5 >= table1d14e2[longrg1 / 2]) // BUGFIX 2016
    {
      i16 wordrg1;
      if (!boolrg6)
        wordrg1 = table1d14e2[0x8 + longrg1 / 2]; // EVIL: shifted by index 0x8
      else
        wordrg1 = table1d14e2[0x8 + lcon(0xf) - unsignedlong(wordrg4)]; // EVIL: shifted by index 0x8
      s59prg3->w_01 = wordrg1;
      return;
    }
    wordrg4++;
  }
}

static bool R_8FE(s_sfx* eaxp_s60, s_sfx* edxp_s60)
{
  ui8 ubyterg21 = eaxp_s60->ub_04;
  ui8 ubyterg22 = edxp_s60->ub_04;
  if (ubyterg21 > ubyterg22)
    return true;
  else if (ubyterg21 == ubyterg22)
    return eaxp_s60->s59_08.ub_00 >= edxp_s60->s59_08.ub_00;
  return false;
}

static i32 R_5096A(i32 n)
{
  i32 vl_04;
  i32 vl_08;

  vl_04 = n;
  if (mkul(vl_04) >= mkul(lcon(0x20)))
  {
    vl_08 = 10;
    return vl_08;
  }
  i32 longrg4;
  // all tableentries are 0!
//  if ((word_at(getsind2(vl_04, lcon(0x30))) & lcon(0x8000)) != 0)
//    vl_08 = 0;
//  else
    vl_08 = 1;
  return vl_08;
}

// was SKW_47eb_014f
void DM2_PLAY_SOUND(i16 eaxw, s_sfx* edxp_s60)
{
  c_ql32 vql_00;
  i32 vl_04;
  i16 vw_08;
  i16 vw_0c;

  vql_00.poke16(eaxw);
  s_sfx* s60prg6 = edxp_s60;
  if (!dm2sound.v1d14be || dm2sound.v1dff88 == 0 || eaxw == wcon(0x0))
    return;
  ui8 ubyterg3 = 0;
  s_sfx* s60prg23 = s60prg6;
  while (ubyterg3 < mkub(vql_00.peek8()))
  {
    R_928(s60prg23);
    s60prg23->ub_0b = ubyterg3;
    ubyterg3++;
    s60prg23++;
  }
  bool boolrg7;
  do
  {
    boolrg7 = false;
    vw_0c = unsignedword(s60prg6->ub_0b);
    ui16 uwordrg32 = 1;
    s_sfx* s60prg22 = s60prg6 + 1;
    while (uwordrg32 < mkuw(vql_00.peek16()))
    {
      vw_08 = unsignedword(s60prg22->ub_0b);
      if (R_8FE(&s60prg6[signedlong(vw_0c)], &s60prg6[unsignedlong(s60prg22->ub_0b)]))
        vw_0c = vw_08;
      else
      {
        s_sfx* s60p = s60prg22 - 1; // EVIL
        s60p->ub_0b = CUTX8(vw_08);
        s60prg22->ub_0b = CUTX8(vw_0c);
        boolrg7 = true;
      }
      uwordrg32++;
      s60prg22++;
    }
  } while (boolrg7);
  vl_04 = 0;
  s_pwwp** s58pprg7;
  i32 longrg5;
  for (;;)
  {
    if (mkuw(vl_04) >= mkuw(vql_00.peek16()))
      return;
    i32 longrg43 = unsignedlong(CUTX16(vl_04));
    vl_04 = longrg43 + 1;
    s_pwwp* s58prg2 = CHGCAST(s_pwwp, s60prg6 + unsignedlong(s60prg6[longrg43].ub_0b));
    s_bwb* s57prg7 = &s58prg2->s57_08;
    longrg5 = lcon(0xffffffff);
    ui16 uwordrg3 = 0;

    i32 longrg1;
    while ((longrg1 = signedlong(uwordrg3)) < lcon(0x40))
    {
      i32 longrg42 = dm2sound.v1dfda4[longrg1];
      if (longrg42 == -1 || R_5096A(longrg42) == 1)
      {
        longrg5 = mkl(uwordrg3);
        break;
      }
      uwordrg3++;
    }

    i32 longrg41 = signedlong(CUTX16(longrg5));
    if (longrg41 == lcon(0xffffffff))
      return;
  #if 1 // TODO
    dm2sound.do_sound(CUTX8(dm2sound.v1dff88), DOWNCAST(s_pwwp, s58prg2)); // TODO
  #endif
    dm2sound.v1d1410 = signedlong(CUTX16(longrg5));
    dm2sound.v1d1400 = unsignedlong(s58prg2->w_04);
    dm2sound.v1d1420 = lcon(0x320);
    i32 longrg12 = unsignedlong(s57prg7->b_00);
    longrg5 = longrg12 << 7;
    longrg12 >>= bcon(0x1);
    longrg5 |= longrg12;
    dm2sound.v1d140c = longrg5;
    dm2sound.v1d1438 = unsignedlong(s57prg7->w_01);
    dm2sound.v1d143c = 0;
    // ddat.v1d13fc = DSZERO;
    dm2sound.v1d13f8 = s58prg2->xp_00;
    // RG1L = ddat.v1dff24;
    // RG2P = &dm2sound.v1d13f8;
    i16 wordrg13 = 0; // TODO: was a call, can return -1 as well. (if sample is still running?)
    dm2sound.v1dfda4[longrg41] = signedlong(wordrg13);
  }
}

// was R_B37
void DM2_SOUND1(void)
{
  if (dm2sound.v1d14c2 != NULL && dm2sound.v1d14d0)
  {
    // R_51BDF(dm2sound.v1dff2c, 1); // DM's MIDI stuff extracted. TODO: check what it wants to do here
    dm2sound.v1d14cc = true;
  }
}

// SPX: Added DM2_GET_MUSIC_INDEX_FROM_MODLIST to read MODLIST instead of SONGLIST
i8 DM2_GET_MUSIC_INDEX_FROM_MODLIST(i16 n)
{
	FILE* fModList = NULL;
	char iMusicIndex = 0;
	fModList = fopen("DATA/MODLIST.DAT", "rb");
	if (fModList != NULL)
	{
		fseek(fModList, n, SEEK_SET);
		fread(&iMusicIndex, 1, 1, fModList);
		fclose(fModList);
	}
	return iMusicIndex;
}

extern int __GlobalOption_DM2Skull_MusicMode;	// from main.cpp

// was R_BA7
void DM2_SOUND2(i16 n)
{
  if (dm2sound.v1d14c2 == NULL || !dm2sound.v1d14da)
    return;
  if (dm2sound.v1d14cc)
  {
    // R_51D42(dm2sound.v1dff2c);  DM's MIDI stuff extracted. TODO: check what it wants to do here
    dm2sound.v1d14cc = false;
  }
  i16 rg1 = dm2sound.v1d14ca;
  if (rg1 <= 1)
  {
    if (rg1 == 1)
    {
      dm2sound.v1d1512 = dm2sound.v1dff8a;
      DM2_PLAY_MUSIC(dm2sound.v1d1512);
      dm2sound.v1d14ca = 0;
      return;
    }
  }
  else
  {
    // R_520C3(dm2sound.v1dff2c, unsignedlong(CUTX8(dm2sound.v1d14ca)); // DM's MIDI stuff extracted TODO: check what it wants to do here
    dm2sound.v1d14ca--;
  }
  dm2sound.v1dff8a = tblMusicsMap[n];
  if (__GlobalOption_DM2Skull_MusicMode == 1) // SPX: added this for using MOD MAP table list instead of SONGLIST for HMP
	dm2sound.v1dff8a = DM2_GET_MUSIC_INDEX_FROM_MODLIST(n);

  if (dm2sound.v1d1512 != mkw(dm2sound.v1dff8a))
  {
    if (!dm2sound.v1d14d0 || dm2sound.v1d14ca != 0)
      DM2_PLAY_MUSIC(dm2sound.v1dff8a);
    else
      dm2sound.v1d14ca = 127;
    dm2sound.v1d1512 = dm2sound.v1dff8a;
  }
}

// more SFX ------------------------

static void R_51083(ui32 n)
{
  ui32 vl_04;

  vl_04 = n;
  if (vl_04 >= mkul(32))
    return;
  // all table-entries are 0!
//  if ((word_at(getsind2(vl_04, lcon(0x30))) & lcon(0x8000)) == 0)
//    return;  // so this return is chosen
//  if (jz_test8(getsind2(vl_04, lcon(0x31)), lcon(0x10)))
//  {
//    and8(getsind2(vl_04, lcon(0x31)), lcon(0x7f));
//    mov16(getsind2(vl_04, lcon(0x34)), 0);
//  }
}

static i32 R_51B56(void)
{
  i32 vl_08 = 0;
  i32 vl_0c = 0;

  while (mkul(vl_0c) < mkul(lcon(0x20)))
  {
// all entries are 0, so condition is always no
//    if ((word_at(getsind2(vl_0c, lcon(0x30))) & lcon(0x8000)) != 0)
//      vl_08++;
    vl_0c++;
  }
  return vl_08; // returns initial value then, which is 0
}

static void R_8E6(void) // obsolete, because...
{
  if (dm2sound.v1d14be)
    while (R_51B56() != 0); // ...return is always 0, so call not needed
}

// was SKW_482b_060e
void DM2_SOUND4(void)
{
  // R_8E6();
  dm2sound.v1d2694 = 0;
}

static void R_8AF(void)
{
  if (dm2sound.v1d14be)
  {
    i32 rg2 = 0;
    while (rg2 < 64)
    {
      if (dm2sound.v1dfda4[rg2] != -1)
        R_51083(dm2sound.v1dfda4[rg2]);
      rg2++;
    }
  }
}

static void R_5F7(s_pwwp* eaxp_s58)
{
  R_8AF();
  s_pwwp* s58prg1 = dm2_dballochandler.v1dff38;
  while (s58prg1 != NULL && eaxp_s58 != s58prg1)
    s58prg1 = s58prg1->s58p_08;
  s_pwwp* s58prg5 = s58prg1;
  if (eaxp_s58 == s58prg5)
    s58prg1 = s58prg5->s58p_08;
}

// was SKW_482b_07c2
void DM2_SOUND5(void)
{
  ui16 uwordrg2 = 0;
  for (;;)
  {
    if (dm2sound.v1d2698-- <= uwordrg2)
      break;
    i16 wordrg1 = dm2sound.xsndptr2[unsignedlong(dm2sound.v1d2698)].w_05;
    if (wordrg1 != -1)
      if (DM2_SOUND7(wordrg1) == 0)
      {
        R_5F7(XUPCAST(s_pwwp, dm2sound.sndptr4 + 16 * unsignedlong(dm2sound.xsndptr2[unsignedlong(dm2sound.v1d2698)].w_00)));
        dm2sound.v1d269a--;
      }
  }
  dm2sound.v1d2698 = uwordrg2;
  dm2sound.v1d2694 = 0;
}

// was SKW_482b_0004
void DM2_SOUND6(void)
{
  dm2sound.sndptr4 = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(16 * dm2sound.v1e0ace, true);
  dm2sound.xsndptr2 = XUPCAST(s_ssound, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(7 * unsignedlong(dm2_dballochandler.v1e0ad6), true));
  dm2sound.xsndptr5 = XUPCAST(s_sfx, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(lcon(0xf0), true));
  dm2sound.sndptr1 = XUPCAST(s_sizee, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(lcon(0x70), true));
  DM2_ZERO_MEMORY(DOWNCAST(s_sizee, dm2sound.sndptr1), lcon(0x70));
  dm2sound.xsndptr3 = XUPCAST(s_sfx, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(lcon(0x48), true));
  dm2sound.sndptr6 = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(unsignedlong(dm2_dballochandler.v1e0ad4));
}

// was SKW_482b_015c
ui16 DM2_SOUND7(i16 n)
{
  ui16 urg4 = 0;
  do
  {
    if (urg4 >= dm2sound.v1d2698)
      return 0;
  } while (n != dm2sound.xsndptr2[urg4++].w_05);
  return urg4;
}

// was SKW_482b_04e9
void DM2_PROCESS_SOUND(i16 n)
{
  i16 wrg1 = signedword(dm2sound.sndptr1[n].barr_04[3]);
  if (wrg1 == ddat.v1e0282 || wrg1 == ddat.v1e027c)
  {
    s_sizee* s54prg1 = &dm2sound.sndptr1[n];
    DM2_QUEUE_NOISE_GEN1(s54prg1->barr_04[0], s54prg1->barr_04[1], s54prg1->barr_04[2], s54prg1->w_0a, s54prg1->w_0c, signedword(s54prg1->barr_04[4]), signedword(s54prg1->barr_04[5]), 1);
  }
  // i32 longrg1 = 7 * signedlong(n); // returns, but never needed
  dm2sound.sndptr1[n].s54p_00 = NULL;
}

// was SKW_482b_05bf
void DM2_SOUND8(bool flag)
{
  if (flag)
  {
    if (dm2sound.v1d2696 > 0)
      DM2_PLAY_SOUND(dm2sound.v1d2696, dm2sound.xsndptr3);
    dm2sound.v1d2696 = 0;
  }
  else
  {
    if (dm2sound.v1d2694 > 0)
      DM2_PLAY_SOUND(dm2sound.v1d2694, dm2sound.xsndptr5);
    dm2sound.v1d2694 = 0;
  }
}

// was SKW_482b_0624
void DM2_SOUND9(i8 eaxb, i8 edxb, i8 ebxb)
{
  if (DM2_QUERY_SND_ENTRY_INDEX(eaxb, edxb, ebxb) != 0)
    return;
  if (dm2sound.v1d2698 >= dm2_dballochandler.v1e0ad6)
    return;
  s_ssound* s55prg1 = &dm2sound.xsndptr2[unsignedlong(dm2sound.v1d2698)];
  s55prg1->b_02 = eaxb;
  s55prg1->b_03 = edxb;
  s55prg1->b_04 = ebxb;
  s55prg1->w_05 = lcon(0xffffffff);
  dm2sound.v1d2698++;
}

i16 DM2_QUERY_SND_ENTRY_INDEX(i8 eaxb, i8 edxb, i8 ebxb)
{
  for (ui16 uwordrg4 = 0; uwordrg4 < dm2sound.v1d2698; uwordrg4++)
  {
    s_ssound* s55prg1 = &dm2sound.xsndptr2[unsignedlong(uwordrg4)];
    if (eaxb == s55prg1->b_02 && edxb == s55prg1->b_03 && ebxb == s55prg1->b_04)
      return uwordrg4 + 1;
  }
  return 0;
}
