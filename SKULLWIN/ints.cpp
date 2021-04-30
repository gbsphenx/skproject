#include "dm2data.h"
#include "ints.h"

// bool DRVb_timer_disabled; // ADR511W(0x430)
// needs to be 0 for the timer to tick. if not 0, 'driver_forceint2d' is able to queue_int2d_T1
// value in fact never changes
// was set by driver_init only

// bool DRVb_int2d_enabled; // ADR511W(0x432)
// if 0 queue_int2d_T1 cannot be called
// was set by driver_init only, then never reset again

// was sub_3FC3
void driver_timerint_T1(void)
{
  static ui8 DRVB_staticsubcounter = 0;

// Note: because there is still nothing that sets DRVb_timer_disabled or
// resets DRVb_int2d_enabled, the condition is ever true.
  if ((++DRVB_staticsubcounter & 0x3) == 0) // each 4th tick no call
  {
    // old queue_int2d_T1 exploded here
    // was an interruptroutine, ended with iret
    // was R_D73
    // gets called by the driver to execute a timertick
    // calling thread depends on flags - can be called by
    // both threads, but won't overlap.
    ddat.gametime += signedlong(ddat.tickconst);

    ddat.ticksum += ddat.tickconst;
    if (ddat.ticksum >= ddat.ticktrig)
      ddat.tickevent = true;
  }

  // original programs the timer here with a one shot trigger
}
