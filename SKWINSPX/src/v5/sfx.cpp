#ifndef __SK_NO_ALLEGRO__
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#else
#include <skalegro.h>
#endif // __SK_NO_ALLEGRO__

#include <stdlib.h>	// abs
#include <util.h>
#include <startend.h> // THROW...
#include <dm2data.h>
#include <skalocdb.h>
#include <sfxsnd.h>
#include <skmap.h>
#include <sktimer.h>
#include <sfx.h>

// TODO needs:
i16 DM2_QUEUE_TIMER(c_tim* eaxtimp);

static ALLEGRO_SAMPLE* thesamples[MAXSAMPLES] = {
       NULL, NULL, NULL, NULL,
       NULL, NULL, NULL, NULL,
       NULL, NULL, NULL, NULL,
       NULL, NULL, NULL, NULL
     };

static i8* samplebuffs[MAXSAMPLES] = {
       NULL, NULL, NULL, NULL,
       NULL, NULL, NULL, NULL,
       NULL, NULL, NULL, NULL,
       NULL, NULL, NULL, NULL
     };

bool c_sfx::init_sfx(void)
{
  al_init_acodec_addon();
  if (!al_install_audio()) return false;
  if (!al_reserve_samples(MAXSAMPLES)) return false;
  return true;
}

void c_sfx::stop_sample(int i)
{
  al_destroy_sample(thesamples[i]);
  thesamples[i] = NULL;
  delete [] samplebuffs[i];
  samplebuffs[i] = NULL;
}

void c_sfx::do_sound(ui8 vol, i8* adr)
{
  if (++sampleindex >= MAXSAMPLES)
    sampleindex = 0;
  if (thesamples[sampleindex] != NULL)
    stop_sample(sampleindex);

  i32 i1 = *UPCAST(i32, adr);
  i8 vx = *(adr + 6);
  i8 vy = *(adr + 7);
  i32 i2 = *CHGCAST(i32, i1);
  int length = mkl(*CHGCAST(i16, i1 + 4)) & lcon(0xffff);
  void* vpsrc = VCAST(i2);
  i8* bpdest = new i8[length];
  memcpy(bpdest, vpsrc, length);
  samplebuffs[sampleindex] = bpdest;

  thesamples[sampleindex] = al_create_sample((char*)bpdest, length, SAMPLE_REC_FREQ, ALLEGRO_AUDIO_DEPTH_UINT8, ALLEGRO_CHANNEL_CONF_1, false);

  i16 distance = (abs(vx) + abs(vy)) / 2;
  i32 volume = (255 - 36 * (7 - vol)) / (distance + 1); // ad hoc formula

  i16 pan = 0x80 + 4 * vx; // TODO ad hoc formula, x/y->angle would be better
  if (pan < 0) pan = 0;
  if (pan > wcon(0xff)) pan = 0xff;

  float fvolume = volume / 255.0;
  float fpan = 2 * pan / 255.0 - 1.0;

  al_play_sample(thesamples[sampleindex], fvolume, fpan, 1.0 /* freq */, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void c_sfx::stop_sfx(void)
{
  for (int i = 0; i < MAXSAMPLES; i++)
    if (thesamples[i] != NULL)
      stop_sample(i);
}

// #########################################################

static i32 R_1FB7D(i16 eaxw, i16 edxw)
{
  i16 vw_00;
  i16 vw_04;

  vw_00 = edxw;
  i16 wordrg1 = ddat.v1d3248;
  unk* xptrrg7;
  i16 wordrg2;
  if (wordrg1 != ddat.v1d62a4)
  {
    if (wordrg1 != ddat.v1d62a6)
      return lcon(0xffffffff);
    xptrrg7 = ddat.v1e08c8;
    wordrg2 = ddat.v1e08d0;
  }
  else
  {
    xptrrg7 = ddat.v1e08cc;
    wordrg2 = ddat.v1e08d2;
  }
  vw_04 = wordrg2;
  if (eaxw >= vw_04)
    return lcon(0xffffffff);
  ui8 ubyterg3 = xptrrg7[(signedlong(eaxw) << 5) + signedlong(vw_00)];
  if ((ubyterg3 & ubcon(0x80)) != 0)
  {
    ubyterg3 = lcon(0x7f);
    i16 wordrg2 = 0;
    i32 longrg11;
    while ((longrg11 = signedlong(wordrg2)) <= lcon(0x3))
    {
      i16 wordrg4 = table1d27fc[longrg11] + eaxw;
      i16 wordrg14 = vw_00 + table1d2804[longrg11];
      if (wordrg4 >= 0 && wordrg4 < vw_04)
      {
        if (wordrg14 >= 0 && wordrg14 < mapdat.map_height)
        {
          ui8 ubyterg1 = xptrrg7[(signedlong(wordrg4) << 5) + signedlong(wordrg14)];
          if (ubyterg1 != ubcon(0x0) && ubyterg3 > ubyterg1)
            ubyterg3 = ubyterg1;
        }
      }
      wordrg2++;
    }
  }
  return unsignedlong(ubyterg3) - 1;
}

// stacksize was 0x40
void DM2_QUEUE_NOISE_GEN1(i8 eaxb, i8 edxb, i8 ebxb, i16 ecxw, i16 argw0, i16 argw1, i16 argw2, i16 argw3)
{
  c_tim c38_00;
  i16 vw_0c;
  i16 vw_10;
  i16 vw_14;
  i16 vw_18;
  i16 vw_1c;
  i16 vw_20;
  i16 vw_24;
  i8 vb_28;
  i8 vb_2c;
  i8 vb_30;

  vb_2c = eaxb;
  vb_30 = edxb;
  vb_28 = ebxb;
  vw_24 = ecxw;
  if (argw3 > wcon(0x0))
  {
    if (ddat.v1d3248 != ddat.v1e0282)
    {
      if (ddat.v1d3248 != ddat.v1e027c)
        return;
    }
  }
  if (dm2sound.v1d2694 == 20)
    return;
  i16 wordrg19 = DM2_QUERY_SND_ENTRY_INDEX(vb_2c, vb_30, vb_28);
  vw_10 = wordrg19;
  if (wordrg19 == 0)
    return;
  if (ddat.v1e0238 != 0)
    argw0 >>= bcon(0x1);
  if (argw3 <= wcon(0x1))
  {
    s_sizee* s54prg3 = XUPCAST(s_sizee, dm2sound.sndptr4 + (unsignedlong(dm2sound.xsndptr2[unsignedlong(vw_10) - 1].w_00) << 4));
    vw_14 = argw1;
    vw_0c = argw2;
    if (argw3 > wcon(0x0))
    {
      s_sizee* s54prg1 = XUPCAST(s_sizee, ddat.v1e03c8 + 16 * signedlong(ddat.v1e0266));
      argw1 += unsignedword(ddat.v1e03c0->barr_04[0x2]) - unsignedword(s54prg1->barr_04[0x2]);
      argw2 += unsignedword(ddat.v1e03c0->barr_04[0x3]) - unsignedword(s54prg1->barr_04[0x3]);
    }
    if (mkuw(ddat.v1e0258) <= mkuw(lcon(0x3)))
    {
      i16 wordrg21 = ddat.v1e0272 - argw2;
      i16 wordrg6 = argw1 - ddat.v1e0270;
      i16 wordrg5 = argw2 - ddat.v1e0272;
      i16 wordrg41 = ddat.v1e0270 - argw1;
      switch (ddat.v1e0258)
      {
        case 0:
          vw_18 = wordrg6;
          vw_1c = wordrg21;
        break;
        case 1:
          vw_18 = wordrg5;
          vw_1c = wordrg6;
        break;
        case 2:
          vw_18 = wordrg41;
          vw_1c = wordrg5;
        break;
        case 3:
          vw_18 = wordrg21;
          vw_1c = wordrg41;
        break;
      }
    }
    i16 wordrg61;
    s_sfx* s60prg2;
    if (argw3 >= wcon(0x0))
    {
      s60prg2 = dm2sound.xsndptr5;
      wordrg61 = dm2sound.v1d2694;
    }
    else
    {
      if (dm2sound.v1d2696 == 6)
        return;
      s60prg2 = dm2sound.xsndptr3;
      wordrg61 = dm2sound.v1d2696;
    }
    i16 wordrg13 = 0;
    while (wordrg13 < wordrg61)
    {
      s_sizee* s54prg4 = CHGCAST(s_sizee, s60prg2 + signedlong(wordrg13));
      s_sizee* s54prg5 = s54prg4->s54p_00;
      if (s54prg3 == s54prg5 || s54prg5->s54p_00 == s54prg3->s54p_00) // Attention!
      {
        s54prg4 = CHGCAST(s_sizee, s60prg2 + signedlong(wordrg13));
        if (signedword(s54prg4->barr_04[2]) == vw_18)
        {
          if (signedword(s54prg4->barr_04[3]) == vw_1c)
            return;
        }
      }
      wordrg13++;
    }
    i16 wordrg3 = signedlong(vw_18);
    vw_20 = DM2_ABS(wordrg3) + DM2_ABS(vw_1c);
    i32 longrg5 = signedlong(vw_20);
    if (longrg5 > 1)
    {
      i16 wordrg18 = CUTX16(R_1FB7D(vw_14, vw_0c));
      if (wordrg18 < 0)
        return;
      if (wordrg18 > vw_20)
      {
        i16 wordrg1 = (signedlong(wordrg18) << bcon(0xa)) / longrg5;
        i16 wordrg5 = vw_18;
        if (wordrg5 >= 0)
        {
          if (wordrg5 > 0)
          {
            wordrg3 *= wordrg1;
            wordrg3 += lcon(0x200);
            wordrg3 >>= bcon(0xa);
            vw_18 = wordrg3;
          }
        }
        else
          vw_18 = -((-wordrg3 * wordrg1 + wcon(0x200)) >> 10);
        if (vw_1c >= 0)
        {
          if (vw_1c > 0)
          {
            wordrg1 *= vw_1c;
            wordrg1 += lcon(0x200);
            wordrg1 >>= bcon(0xa);
            vw_1c = wordrg1;
          }
        }
        else
        {
          wordrg1 *= -vw_1c;
          wordrg1 += lcon(0x200);
          wordrg1 >>= bcon(0xa);
          vw_1c = -wordrg1;
        }
      }
    }
    s60prg2 += signedlong(wordrg61);
    s_sizee* s54p = CHGCAST(s_sizee, s60prg2); // TODO
    s54p->s54p_00 = XUPCAST(s_sizee, dm2sound.sndptr4 + (unsignedlong(dm2sound.xsndptr2[unsignedlong(vw_10) - 1].w_00) << 4));
    s54p->barr_04[0] = CUTX8(vw_24);
    s54p->barr_04[1] = mkb(argw0);
    s54p->barr_04[2] = CUTX8(vw_18);
    s54p->barr_04[3] = CUTX8(vw_1c);
    if (argw3 != 0)
    {
      if (argw3 <= 0)
        dm2sound.v1d2696++;
      else
        dm2sound.v1d2694++;
    }
    else
      DM2_PLAY_SOUND(1, s60prg2);
    return;
  }
  bool boolrg5 = false;
  i16 wordrg1 = 0;
  i32 longrg4;
  while ((longrg4 = signedlong(wordrg1)) < lcon(0x8))
  {
    i32 longrg22 = 14 * longrg4;
    if (dm2sound.sndptr1[longrg4].l_00 != 0)
      wordrg1++;
    else
    {
      boolrg5 = true;
      break;
    }
  }
  if (!boolrg5)
    return;
  s_sizee* s54prg4 = &dm2sound.sndptr1[signedlong(wordrg1)];
  s54prg4->l_00 = 1;
  s54prg4->barr_04[0] = vb_2c;
  s54prg4->barr_04[1] = vb_30;
  s54prg4->barr_04[2] = vb_28;
  s54prg4->w_0a = vw_24;
  s54prg4->w_0c = argw0;
  s54prg4->barr_04[3] = CUTX8(ddat.v1d3248);
  s54prg4->barr_04[4] = mkb(argw1);
  s54prg4->barr_04[5] = mkb(argw2);
  c38_00.setmticks(ddat.v1d3248, timdat.gametick + signedlong(argw3) - 1);
  c38_00.settype(0x15);
  c38_00.setactor(CUTX8(vw_24));
  c38_00.setA(wordrg1);
  DM2_QUEUE_TIMER(&c38_00);
}

// stacksize was 0x18
void DM2_QUEUE_NOISE_GEN2(i8 eaxb, i8 edxb, i8 ebxb, i8 ecxb, i16 argw0, i16 argw1, i16 argw2, i16 argw3, i16 argw4)
{
  i8 vb_00;
  i8 vb_04;
  vb_04 = eaxb;
  vb_00 = edxb;

  i8 byterg3 = ecxb;
  if (DM2_QUERY_SND_ENTRY_INDEX(vb_04, vb_00, ebxb) != 0)
    byterg3 = vb_00;
  DM2_QUEUE_NOISE_GEN1(vb_04, byterg3, ebxb, argw3, argw4, argw0, argw1, argw2);
}
