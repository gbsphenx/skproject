#include "../driver.h" // driver_userint
#include "dm2data.h"
#include "c_gfx_fill.h"
#include "c_gfx_pal.h"

void SKW_FIRE_SELECT_PALETTE_SET(x8 index)
{
  userinterrupt(con(0x106), unsignedword(index));
}

void SKW_FIRE_FADE_SCREEN(bool eaxbool)
{
  if (!ddata.dialog2)
    return;
  if (!eaxbool)
    SKW_FIRE_SELECT_PALETTE_SET(1);
  else
  {
    SKW_FIRE_SELECT_PALETTE_SET(0);
    SKW_FIRE_FILL_SCREEN_RECT(con(0x2), con(0x0));
  }
}
