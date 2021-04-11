#include "dm2data.h"
#include "c_timer.h"

// was an interruptroutine, ended with iret
// old name: R_D73
// gets called by the driver to execute a timertick
// calling thread depends on flags - can be called by
// both threads, but won't overlap.
void queue_int2d_T1(void)
{
  ddata.longgametime += signedlong(ddata.tickconst);

  i16 val = ddata.ticksum + ddata.tickconst;
  ddata.ticksum = val;
  if (val >= ddata.ticktrig)
    ddata.tickevent = true;
}
