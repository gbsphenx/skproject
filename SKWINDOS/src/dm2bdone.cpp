#include "util.h"
#include "..\calls.h"
#include "..\driver.h"
#include "dm2data.h"

// unreferenced
x16 R_1456(void)
{
  userinterrupt(con(0x12), con(0x0));
  return *P16CAST(ddata.drvinterfaceptr);
}

// special plugin - parameter never used
void R_148D(x16 dummy)
{
}

x16 SKW_476d_05b6(x16 n)
{
  userinterrupt(con(0x1b), n);
  return *P16CAST(ddata.drvinterfaceptr);
}

// was SKW_2405_00c9
void SKW_DISPLAY_TAKEN_ITEM_NAME(x16 n)
{
  SKW_DISPLAY_HINT_NEW_LINE();
  SKW_DISPLAY_HINT_TEXT(con(0xd), SKW_GET_ITEM_NAME(n));
}

// buffer is a bytebuffer
// was S KW_476d_05e3
void SKW_COPY_TO_DRIVER(x8* buffer, x16 amount)
{
  SKW_COPY_MEMORY(buffer, amount, DOWNCAST(i8, ddata.drvinterfaceptr));
  userinterrupt(con(0x0), con(0x0));
}

void SKW_443c_087c(void)
{
  userinterrupt(con(0x4), con(0x0));
}

void SKW_443c_0889(void)
{
  userinterrupt(con(0x5), con(0x0));
}
