#ifndef DM2DEBUG_H
#define DM2DEBUG_H

#include <stdlib.h>	// SPX: just for exit, else not needed.
#include "types.h"

#define LOGX(X) SPX_DEBUG_LOGFILE_INFO X
#ifdef NDEBUG
	#define LOGX(X) ;;
#endif

void DEBUG_DISPLAY_GDAT_MAIN_INFO();
void DEBUG_DUMP_BIN_DATA(unsigned char* ptr, unsigned int iDumpSize, unsigned int iBytesPerRow);
void DEBUG_DUMP_ULP();
const char* DEBUG_SKLOADENT(ui8* xSkLoadEnt);
const char* DEBUG_SKGDATENT(s_gdat* xSkGDATEnt);


#endif
