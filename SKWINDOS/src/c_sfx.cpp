#include <allegro5.h>
#include <allegro_audio.h>
#include <allegro_acodec.h>

#include "util.h"
#include "startend.h" // THROW...
#include "dm2data.h"
#include "c_dballoc.h"
#include "c_sound.h"
#include "c_sfx.h"

// SPX 2021-03-07
#include "types_allegro.h"

// needs:
x16 SKW_QUERY_SND_ENTRY_INDEX(x8 eaxb, x8 ebxb, x8 edxb); // c_dballocx.cpp
x16 SKW_QUEUE_TIMER(c_tim* eaxptim);

static ALLEGRO_SAMPLE* thesamples[MAXSAMPLES] = {
                           NULL, NULL, NULL, NULL,
                           NULL, NULL, NULL, NULL,
                           NULL, NULL, NULL, NULL,
                           NULL, NULL, NULL, NULL,
                         };

static char* samplebuffs[MAXSAMPLES] = {
                           NULL, NULL, NULL, NULL,
                           NULL, NULL, NULL, NULL,
                           NULL, NULL, NULL, NULL,
                           NULL, NULL, NULL, NULL,
                         };

bool c_sfx::init_sfx(void)
{
  al_init_acodec_addon();
  if (!al_install_audio()) return false;
  if (!al_reserve_samples(MAXSAMPLES)) return false;
  return true;
}

void c_sfx::stop_sample(int n)
{
  al_destroy_sample(thesamples[n]);
  thesamples[n] = NULL;
  delete [] samplebuffs[n];
  samplebuffs[n] = NULL;
}

void c_sfx::do_sound(ui8 vol, i8* adr)
{
  if (++sampleindex >= MAXSAMPLES)
    sampleindex=0;
  if (thesamples[sampleindex] != NULL)
    stop_sample(sampleindex);

  i32 i1 = *(i32*)adr;
  i8 vx = *(i8*)(adr + 6);
  i8 vy = *(i8*)(adr + 7);
  i32 i2 = *(i32*)i1;
  int length = *(i16*)(i1 + 4) & 0xffff;
  void* src = VCAST(i2);
  char* dest = new char[length];
  memcpy(dest, src, length);
  samplebuffs[sampleindex] = dest;

  thesamples[sampleindex] =
    al_create_sample(dest, length, SAMPLE_REC_FREQ,
                     ALLEGRO_AUDIO_DEPTH_UINT8,
                     ALLEGRO_CHANNEL_CONF_1, false);

  i16 distance = (abs(vx) + abs(vy)) / 2;
  i32 volume = (255 - 36 * (7 - vol)) / (distance + 1); // ad hoc formula

  i16 pan = 0x80 + 4 * vx; // TODO ad hoc formula, x/y->angle would be better
  if (pan<0) pan=0;
  if (pan>0xff) pan=0xff;

  float fvolume = volume/255.0;
  float fpan = 2 * pan/255.0 - 1.0;

  al_play_sample(thesamples[sampleindex], fvolume, fpan, 1.0 /* freq */, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void c_sfx::stop_sfx(void)
{
  for (int i=0; i<MAXSAMPLES; i++)
    if (thesamples[i] != NULL)
      stop_sample(i);
}

// #########################################################

static x32 R_1FB7D(x16 eaxw, x16 edxw)
{
  x16 vw_00;
  x16 vw_04;

  vw_00 = edxw;
  x16 wordrg1 = ddata.v1d3248;
  xxx* ptrrg7;
  x16 wordrg2;
  if (wordrg1 != ddata.v1d62a4)
  {
    if (wordrg1 != ddata.v1d62a6)
      return con(0xffffffff);
    ptrrg7 = DOWNCAST(s_32bytes, ddata.v1e08c8);
    wordrg2 = ddata.v1e08d0;
  }
  else
  {
    ptrrg7 = DOWNCAST(s_32bytes, ddata.v1e08cc);
    wordrg2 = ddata.v1e08d2;
  }
  vw_04 = wordrg2;
  if (eaxw >= vw_04)
    return con(0xffffffff);
  ui8 byterg3 = ptrrg7[(signedlong(eaxw) << con(0x5)) + signedlong(vw_00)];
  if ((byterg3 & con(0xffffff80)) != con(0x0))
  {
    byterg3 = con(0x7f);
    x16 wordrg2 = con(0x0);
    x32 longrg11;
    while ((longrg11 = signedlong(wordrg2)) <= con(0x3))
    {
      x16 wordrg4 = table1d27fc[longrg11] + eaxw;
      x16 wordrg14 = vw_00 + table1d2804[longrg11];
      if (wordrg4 >= 0 && wordrg4 < vw_04)
      {
        if (wordrg14 >= 0 && wordrg14 < ddata.savegamewb)
        {
          ui8 ubyterg1 = ptrrg7[(signedlong(wordrg4) << con(0x5)) + signedlong(wordrg14)];
          if (ubyterg1 != con(0x0) && byterg3 > ubyterg1)
            byterg3 = ubyterg1;
        }
      }
      wordrg2++;
    }
  }
  return unsignedlong(byterg3) - 1;
}

void SKW_QUEUE_NOISE_GEN1(x8 eaxb, x8 ebxb, x16 ecxw, x8 edxb, x16 argw0, x16 argw1, x16 argw2, x16 argw3)
{
  c_tim vs_00;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_14;
  x16 vw_18;
  x16 vw_1c;
  x16 vw_24;
  x8 vb_28;
  x8 vb_2c;
  x8 vb_30;

  vb_2c = eaxb;
  vb_30 = edxb;
  vb_28 = ebxb;
  vw_24 = ecxw;
  if (argw3 > con(0x0))
  {
    if (ddata.v1d3248 != ddata.v1e0282)
    {
      if (ddata.v1d3248 != ddata.v1e027c)
        return;
    }
  }
  if (ddata.v1d2694 == con(0x14))
    return;
  x16 wordrg19 = SKW_QUERY_SND_ENTRY_INDEX(vb_2c, CUTLX8(unsignedlong(vb_28)), vb_30);
  vw_10 = wordrg19;
  if (wordrg19 == con(0x0))
    return;
  if (ddata.v1e0238)
    argw0 >>= con(0x1);
  if (argw3 <= con(0x1))
  {
    s_size10* sptrrg3 = UPCAST(s_size10, dm2sound.sndptr4 + (unsignedlong(dm2sound.xsndptr2[vw_10 - con(0x1)].w0) << con(0x4)));
    vw_14 = argw1;
    vw_0c = argw2;
    if (argw3 > con(0x0))
    {
      s_size10* ptrrg1 = &ddata.v1e03c8[ddata.v1e0266];
      argw1 += unsignedword(ddata.v1e03c0->b4[0x2]) - unsignedword(ptrrg1->b4[0x2]);
      argw2 += unsignedword(ddata.v1e03c0->b4[0x3]) - unsignedword(ptrrg1->b4[0x3]);
    }
    if (ddata.v1e0258 <= UI16CAST(con(0x3)))
    {
      x16 wordrg21 = ddata.v1e0272 - argw2;
      x16 wordrg6 = argw1 - ddata.v1e0270;
      x16 wordrg5 = argw2 - ddata.v1e0272;
      x16 wordrg41 = ddata.v1e0270 - argw1;
      switch (ddata.v1e0258)
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
    ui16 uwordrg61;
    s_sfx* sptrrg2;
    if (argw3 >= con(0x0))
    {
      sptrrg2 = dm2sound.xsndptr5;
      uwordrg61 = ddata.v1d2694;
    }
    else
    {
      if (ddata.v1d2696 == con(0x6))
        return;
      sptrrg2 = dm2sound.xsndptr3;
      uwordrg61 = ddata.v1d2696;
    }
    ui16 uwordrg13 = con(0x0);
    while (uwordrg13 < uwordrg61)
    {
      s_size10* sptrrg4 = UPCAST(s_size10, sptrrg2 + signedlong(uwordrg13));
      s_size10* sptrrg5 = sptrrg4->u0.p;
      if (sptrrg3 == sptrrg5 || sptrrg5->u0.p == sptrrg3->u0.p) // Attention!
      {
        sptrrg4 = UPCAST(s_size10, sptrrg2 + signedlong(uwordrg13));
        if (signedword(sptrrg4->b4[2]) == vw_18)
        {
          if (signedword(sptrrg4->b4[3]) == vw_1c)
            return;
        }
      }
      uwordrg13++;
    }
    x16 wordrg3 = signedlong(vw_18);
    x16 vw_20 = DM2_ABS(wordrg3) + DM2_ABS(vw_1c);
    x32 longrg5 = signedlong(vw_20);
    if (longrg5 > con(0x1))
    {
      x16 wordrg18 = CUTX16(R_1FB7D(vw_14, vw_0c));
      if (wordrg18 < con(0x0))
        return;
      if (wordrg18 > vw_20)
      {
        x16 wordrg1 = (signedlong(wordrg18) << con(0xa)) / longrg5;
        x16 wordrg5 = vw_18;
        if (wordrg5 >= con(0x0))
        {
          if (wordrg5 > con(0x0))
          {
            wordrg3 *= wordrg1;
            wordrg3 += con(0x200);
            wordrg3 >>= con(0xa);
            vw_18 = wordrg3;
          }
        }
        else
          vw_18 = -((-wordrg3 * wordrg1 + con(0x200)) >> con(0xa));
        if (vw_1c >= con(0x0))
        {
          if (vw_1c > con(0x0))
          {
            wordrg1 *= vw_1c;
            wordrg1 += con(0x200);
            wordrg1 >>= con(0xa);
            vw_1c = wordrg1;
          }
        }
        else
        {
          wordrg1 *= -vw_1c;
          wordrg1 += con(0x200);
          wordrg1 >>= con(0xa);
          vw_1c = -wordrg1;
        }
      }
    }
    sptrrg2 += signedlong(uwordrg61);
    s_size10* sptr = UPCAST(s_size10, sptrrg2);
    sptr->u0.p = UPCAST(s_size10, dm2sound.sndptr4 + (unsignedlong(dm2sound.xsndptr2[vw_10 - con(0x1)].w0) << con(0x4)));
    sptr->b4[0] = CUTX8(vw_24);
    sptr->b4[1] = CUTX8(argw0);
    sptr->b4[2] = CUTX8(vw_18);
    sptr->b4[3] = CUTX8(vw_1c);
    if (argw3 != con(0x0))
    {
      if (argw3 <= con(0x0))
        ddata.v1d2696++;
      else
        ddata.v1d2694++;
    }
    else
      SKW_PLAY_SOUND(con(0x1), sptrrg2);
    return;
  }
  bool boolrg5 = false;
  x16 wordrg1 = con(0x0);
  x32 longrg4;
  while ((longrg4 = signedlong(wordrg1)) < con(0x8))
  {
    x32 longrg22 = 14 * longrg4;
    if (dm2sound.sndptr1[longrg4].u0.p != NULL)
      wordrg1++;
    else
    {
      boolrg5 = true;
      break;
    }
  }
  if (!boolrg5)
    return;
  s_size10* sptrrg4 = &dm2sound.sndptr1[wordrg1];
  sptrrg4->u0.u.l = con(0x1);
  sptrrg4->b4[0] = vb_2c;
  sptrrg4->b4[1] = vb_30;
  sptrrg4->b4[2] = vb_28;
  sptrrg4->wa = vw_24;
  sptrrg4->wc = argw0;
  sptrrg4->b4[3] = CUTX8(ddata.v1d3248);
  sptrrg4->b4[4] = CUTX8(argw1);
  sptrrg4->b4[5] = CUTX8(argw2);
  vs_00.un.l_00 = (ddata.longmallocx + signedlong(argw3) - con(0x1)) | (signedlong(ddata.v1d3248) << con(0x18));
  vs_00.b_04 = con(0x15);
  vs_00.b_05 = CUTX8(vw_24);
  vs_00.u.w_06 = wordrg1;
  SKW_QUEUE_TIMER(&vs_00);
}

// stacksize was 0x18
void SKW_QUEUE_NOISE_GEN2(x8 eaxb, x8 ebxb, x8 ecxb, x8 edxb, x16 argw0, x16 argw1, x16 argw2, x16 argw3, x16 argw4)
{
  x8 byterg3 = ecxb;
  if (SKW_QUERY_SND_ENTRY_INDEX(eaxb, ebxb, edxb) != con(0x0))
    byterg3 = edxb;
  SKW_QUEUE_NOISE_GEN1(eaxb, ebxb, argw3, byterg3, argw4, argw0, argw1, argw2);
}

static x16 SKW_QUERY_SND_ENTRY_INDEX(x8 eaxb, x8 ebxb, x8 edxb)
{
  for (ui16 uwordrg4 = con(0x0); uwordrg4 < dm2sound.v1d2698; uwordrg4++)
  {
    s_size7* ptrrg1 = &dm2sound.xsndptr2[uwordrg4];
    if (eaxb == ptrrg1->b2 && edxb == ptrrg1->b3 && ebxb == ptrrg1->b4)
      return uwordrg4 + 1;
  }
  return con(0x0);
}

void SKW_482b_0624(x8 eaxb, x8 ebxb, x8 edxb)
{
  if (SKW_QUERY_SND_ENTRY_INDEX(eaxb, ebxb, edxb) != con(0x0))
    return;
  if (dm2sound.v1d2698 >= dm2_dballochandler.v1e0ad6)
    return;
  s_size7* ptrrg1 = &dm2sound.xsndptr2[dm2sound.v1d2698];
  ptrrg1->b2 = eaxb;
  ptrrg1->b3 = edxb;
  ptrrg1->b4 = ebxb;
  ptrrg1->w5 = con(0xffffffff);
  dm2sound.v1d2698++;
}
