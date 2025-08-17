#include <skstr.h>

t_text fnum[5];

t_text DM2_SKCHR_TO_SCRIPTCHR(t_text a)
{
  if (a >= 'A' && a <= 'Z')
    return a - 'A';
  a = (a == '.') ? 0x1b : 0x1a;
  return a;
}

t_text* DM2_LTOA10(i32 a, t_text* dtp)
{
  t_text tarr_00[16];
  tarr_00[13] = '\0';
  t_text* tp = tarr_00 + 13;

  i32 lrg2 = a;
  if (lrg2 < 0)
    lrg2 = -lrg2;

  do
  {
    *--tp = CUTLX8(lrg2 % 10 + '0');
    lrg2 /= 10;
  } while (lrg2 != 0);

  if (a < 0)
    *--tp = '-';

  return DM2_STRCPY(dtp, tp);
}

t_text* DM2_FMT_NUM(i16 eaxw, i32 edxl, i16 ebxw)
{
  i16 vw_00;

  i32 longrg7 = edxl; // TODO: bool?
  vw_00 = ebxw;
  if (edxl != 0)
    DM2_FILL_STR(fnum, ' ', 1, 4);
  t_text* tprg2 = &fnum[4];
  *tprg2 = '\0';
  if (eaxw != 0)
  {
    for (;;)
    {
      i16 wordrg6 = eaxw;
      if (eaxw == 0)
        break;
      eaxw /= 10;
      *--tprg2 = CUTLX8(unsignedlong(wordrg6) + '0' - 10 * unsignedlong(eaxw));
    }
  }
  else
    *--tprg2 = '0';

  if (longrg7 == 0)
    return tprg2;
  return &fnum[4 - unsignedlong(vw_00)];
}

void DM2_FILL_STR(t_text* txtp, i8 entry, i16 step, i16 num)
{
  i16 idx = 0;
  i16 cnt = 0;

  while (cnt++ < num)
  {
    txtp[idx] = entry;
    idx += step;
  }
}

