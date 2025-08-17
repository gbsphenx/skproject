#include <string.h> // memset

#include <util.h>
#include <gfxbmp.h>

#include <dm2debug.h>

// TODO: I don't like this negative offset

s_dm2bmpheader* getbmpheader(t_bmp* bmp)
{
  return UPCAST(s_dm2bmpheader, DOWNCAST(t_bmp, bmp) - sizeof(s_dm2bmpheader));
}

s_screen256bmp DRV_screen256; // 0x2cae

void init_bitmaps(void)
{
	SPX_DEBUG_PRINT("INIT_BITMAPS:INIT\n");
  memset(&DRV_screen256, sizeof(DRV_screen256), 0);
}

i32 DM2_CALC_IMAGE_BYTE_LENGTH(t_bmp* bmp)
{
  i16 w;
  s_dm2bmpheader* bmpheader = getbmpheader(bmp);

  if (bmpheader->res != BPP_4)
    w = bmpheader->width;
  else
    w = MK_EVEN(unsignedlong(bmpheader->width)) >> 1;

  return unsignedlong(bmpheader->height) * unsignedlong(w);
}
