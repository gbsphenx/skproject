#ifndef DM2DEBUG_H
#define DM2DEBUG_H


#ifndef C_GDATFILE_H
	#include <types.h>
	#include <skalocdb.h>
	#include <bgdat.h>	// requires includes above
#endif

//#define NO_TRACE

void SPX_DEBUG_POP_ALL();
void SPX_DEBUG_FUNCTION_INFO(const char* sFunctionName);
void SPX_DEBUG_PUSH_FUNCTION_INFO(const char* sFunctionName);
void SPX_DEBUG_LOGFILE_INFO(const char *sMessage, ...);
void SPX_DEBUG_MESSAGE_INFO(const char *sMessage, ...);
void SPX_DEBUG_PRINT_WAIT(const char *sMessage, ...);
void SPX_DEBUG_PRINT(const char *sMessage, ...);

#define LOGX(X) SPX_DEBUG_LOGFILE_INFO X
#ifdef NO_TRACE
	#define LOGX(X) ;;
#endif

void DEBUG_DISPLAY_GDAT_MAIN_INFO();
void DEBUG_DUMP_BIN_DATA(unsigned char* ptr, unsigned int iDumpSize, unsigned int iBytesPerRow);
void DEBUG_DUMP_ULP();
const char* DEBUG_SKLOADENT(ui8* xSkLoadEnt);
const char* DEBUG_SKGDATENT(s_gdat* xSkGDATEnt);	// requires c_gdatfile.h


#define _DEBUG_OPTION_BREAK_DOOR_	1

#endif
