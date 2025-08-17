#include <dm2data.h> // timdat.gametick
#include <util.h>
#include <startend.h>
#include <sktimer.h>

#include <dm2debug.h>

c_timerdata timdat;

void c_timerdata::init(void)
{
	SPX_DEBUG_PRINT("TIMERDATA:INIT\n");
  timerarray = NULL;
  timer_indices = NULL;
  num_timer_indices = 0;
  num_timers = 0;
  available_timeridx = 0;
  max_timers = 0;
  timer_unk = 0;
  gametick = 0;
}

void c_tim::init(void)
{
  l_00 = 0;
  ttype = 0;
  actor = 0;
  wvalueA = 0;
  wvalueB = 0;
  dummya = 0;
}

// was SKW_3a15_0381
static bool DM2_cmp_timers(c_tim* eaxtimp, c_tim* edxtimp)
{
  if (eaxtimp->getticks() < edxtimp->getticks())
    return true;
  if (eaxtimp->getticks() != edxtimp->getticks())
    return false;
  if (eaxtimp->gettype() > edxtimp->gettype())
    return true;
  if (!eaxtimp->is_type(edxtimp->gettype()))
    return false;
  if (eaxtimp->getactor() > edxtimp->getactor())
    return true;
  if (eaxtimp->getactor() != edxtimp->getactor())
    return false;
  if (eaxtimp > edxtimp)
    return false;
  return true;
}

static void DM2_timer_3a15_0486(i16 eaxw)
{
  timdat.timer_unk = -1;

  i16 vw_04 = timdat.num_timers - 1;
  if (vw_04 == 0)
    return;

  i16 RG3w = eaxw;
  i16 vw_08 = timdat.timer_indices[RG3w];
  c_tim* RG5p = &timdat.timerarray[vw_08];
  bool vbool_00 = false;

  //m_4623A:
  while (RG3w != 0)
  {
    i16 RG21w = (RG3w - 1) / 2;
    if (!DM2_cmp_timers(RG5p, &timdat.timerarray[timdat.timer_indices[RG21w]]))
      break;
    timdat.timer_indices[RG3w] = timdat.timer_indices[RG21w];
    RG3w = RG21w;
    vbool_00 = true;
  }

  //m_46294:
  if (!vbool_00)
  {
    vw_04 = (vw_04 - 1) / 2;
    //m_462AC:
    while (RG3w <= vw_04)
    {
      i16 RG22w = 2 * RG3w + 1;
      if (   (RG22w + 1) < timdat.num_timers
          && DM2_cmp_timers(&timdat.timerarray[timdat.timer_indices[RG22w + 1]], &timdat.timerarray[timdat.timer_indices[RG22w]])
         )
        RG22w++;
      if (!DM2_cmp_timers(&timdat.timerarray[timdat.timer_indices[RG22w]], RG5p))
        break;
      timdat.timer_indices[RG3w] = timdat.timer_indices[RG22w];
      RG3w = RG22w;
    }
  }

  //m_46361:
  timdat.timer_indices[RG3w] = vw_08;
}

void DM2_REARRANGE_TIMERLIST(void)
{
  i16 lastn; // cannot be requested before being set! but compiler throws...
  timdat.num_timer_indices = 0;
  timdat.available_timeridx = -1;

  //m_4615B:
  for (i16 n = 0; n < timdat.max_timers; n++)
  {
    //m_460FB:
    if (!timdat.timerarray[n].is_notype())
      //m_46150:
      timdat.num_timer_indices = n + 1;
    else
    {
      if (timdat.available_timeridx != -1)
        //m_46128:
        timdat.timerarray[lastn].setdataw(n);
      else
        timdat.available_timeridx = n;
      //m_46136:
      timdat.timerarray[n].setdataw(-1);
      lastn = n;
    }
    //m_4615A:
  }
}

// was SKW_3a15_008e
void DM2_SORT_TIMERS(void)
{
  if (timdat.num_timers != 0)
  {
    i16 RG2w = 0;
    i16 RG32w = timdat.num_timers;
    //m_45EB5:
    for (i16 vw_04 = 0; ; vw_04++)
    {
      if (RG32w <= vw_04)
      {
        i16 RG14w = RG32w;
        if (RG14w != 1)
        {
          RG14w -= 2;
          RG14w >>= 1;
          //m_45FDC:
          for (i16 vw_00 = RG14w; vw_00 >= 0; vw_00--)
          {
            //m_45ED7:
            vw_04 = vw_00;
            c_tim* RG51p = &timdat.timerarray[timdat.timer_indices[vw_00]];
            //m_45F00:
            for (;;)
            {
              RG2w = 2 * vw_04 + 1;
              if (RG2w >= timdat.num_timers)
                break;
              c_tim* RG31p = &timdat.timerarray[timdat.timer_indices[RG2w]];
              if ((RG2w + 1) >= timdat.num_timers)
              {
                //m_45F92:
                if (DM2_cmp_timers(RG51p, RG31p))
                  break; // RG32w undefined then
              }
              else
              {
                i16 vw_10 = RG2w + 1;
                c_tim* RG61p = &timdat.timerarray[timdat.timer_indices[vw_10]];
                if (DM2_cmp_timers(RG51p, RG31p))
                {
                  if (DM2_cmp_timers(RG51p, RG61p))
                    break; // RG32w undefined then
                  RG2w = vw_10;
                }
                else
                {
                  //m_45F83:
                  if (DM2_cmp_timers(RG61p, RG31p))
                    RG2w = vw_10;
                }
              }

              //m_45F9F:
              i16 vw_08 = timdat.timer_indices[vw_04];
              timdat.timer_indices[vw_04] = timdat.timer_indices[RG2w];
              timdat.timer_indices[RG2w] = vw_08;
              vw_04 = RG32w = RG2w;
            }
            //m_45FD9:
          }
        }
        break;
      }

      //m_45E96:
      timdat.timer_indices[vw_04] = vw_04;
    }
  }

  //m_45FE8:
  timdat.timer_unk = -1;
  DM2_REARRANGE_TIMERLIST();
}

// was SKW_3a15_0447
i16 DM2_GET_TIMER_NEW_INDEX(i16 eaxw)
{
  i16* RG1p = timdat.timer_indices;
  i16 n = 0;
  //m_461C3:
  while (n < timdat.num_timer_indices && eaxw != *RG1p++)
    n++;
  //m_461CC:
  if (n >= timdat.num_timer_indices)
    DM2_RAISE_SYSERR(0x46);
  return n;
}

// was SKW_3a15_061a
void DM2_DELETE_TIMER(i16 eaxw)
{
  if (timdat.timer_unk >= 0)
    DM2_timer_3a15_0486(timdat.timer_unk);
  timdat.timerarray[eaxw].clrtype();
  timdat.timerarray[eaxw].setdataw(timdat.available_timeridx);
  timdat.available_timeridx = eaxw;
  timdat.num_timers--;
  if (timdat.num_timers != -1)
  {
    i16 idx = DM2_GET_TIMER_NEW_INDEX(eaxw);
    if (idx != timdat.num_timers)
    {
      timdat.timer_indices[idx] = timdat.timer_indices[timdat.num_timers];
      timdat.timer_unk = idx;
    }
  }
}

i16 DM2_QUEUE_TIMER(c_tim* eaxtimp)
{
  if (eaxtimp->is_notype())
    return -1;

  if (timdat.num_timers == timdat.max_timers)
    DM2_RAISE_SYSERR(0x2d);

  i16 ti = timdat.available_timeridx;
  c_tim* dest = &timdat.timerarray[ti];
  timdat.available_timeridx = dest->getdataw();
  *dest = *eaxtimp;
  if (ti >= timdat.num_timer_indices)
    timdat.num_timer_indices = ti + 1;

  i16 tw = timdat.timer_unk;
  if (tw < 0)
    tw = timdat.num_timers;
  timdat.timer_unk = -1;
  timdat.num_timers++;
  timdat.timer_indices[tw] = ti;
  DM2_timer_3a15_0486(tw);
  return ti;
}

// was SKW_3a15_072f
void DM2_GET_AND_DELETE_NEXT_TIMER(c_tim* eaxtimp)
{
  i16 idx = timdat.timer_indices[0];
  *eaxtimp = timdat.timerarray[idx];
  DM2_DELETE_TIMER(idx);
}

// was SKW_3a15_0763
bool DM2_IS_TIMER_TO_PROCEED(void)
{
  if (timdat.timer_unk >= 0)
    DM2_timer_3a15_0486(timdat.timer_unk);

  if (timdat.num_timers == 0)
    return false;

  ui32 RG1ul = timdat.timerarray[timdat.timer_indices[0]].getticks();
  return RG1ul <= mkul(timdat.gametick);
}

void DM2_timer_3a15_05f7(i16 eaxw)
{
  if (timdat.timer_unk >= 0)
    DM2_timer_3a15_0486(timdat.timer_unk);

  DM2_timer_3a15_0486(DM2_GET_TIMER_NEW_INDEX(eaxw));
}
