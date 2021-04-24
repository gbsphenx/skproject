#include <stdio.h>

#include "dm2global.h"

int restart;
bool close_win = false;
bool main_exit = false;
bool enable_video = true;
bool enable_drawing = true;

FILE* logfile = NULL;


// SPX: some block to trace stacked calls
static int iDebugCallDepth = 0;
static char sLocalTabSpace[64];
#include <string.h>
#include <stdarg.h>	// va_list
void SPX_DEBUG_INCREASE_DEPTH() { iDebugCallDepth++;}
void SPX_DEBUG_DECREASE_DEPTH() { iDebugCallDepth--;}
#define SPX_DEBUG_PUSH		SPX_DEBUG_INCREASE_DEPTH();
#define SPX_DEBUG_POP		SPX_DEBUG_DECREASE_DEPTH();


#define FFLUSH(X) fflush(X);
#ifdef NDEBUG
	#define FFLUSH(X) ;;
#endif

#define NO_TRACE
#ifdef NO_TRACE
	#define NO_LOG_RETURN	return;
#else
	#define NO_LOG_RETURN	;
#endif

void SPX_DEBUG_POP_ALL()	// To be used for top-level catch (after throw)
{
	iDebugCallDepth = 0;	
}

void SPX_DEBUG_FUNCTION_INFO(const char* sFunctionName)
{
NO_LOG_RETURN;
	memset(sLocalTabSpace, ' ', 64);
	sLocalTabSpace[iDebugCallDepth] = 0;
	printf("%s%d)%s\n", sLocalTabSpace, iDebugCallDepth, sFunctionName);
	fprintf(logfile, "%s%d)%s\n", sLocalTabSpace, iDebugCallDepth, sFunctionName);
	FFLUSH(logfile);
}

void SPX_DEBUG_PUSH_FUNCTION_INFO(const char* sFunctionName)
{
NO_LOG_RETURN;
	SPX_DEBUG_PUSH;
	SPX_DEBUG_FUNCTION_INFO(sFunctionName);
}
void SPX_DEBUG_MESSAGE_INFO(const char *sMessage, ...)
{
NO_LOG_RETURN;
	memset(sLocalTabSpace, ' ', 64);
	sLocalTabSpace[iDebugCallDepth] = 0;

	va_list args;
	va_start (args, sMessage);
	printf("%s%d)", sLocalTabSpace, iDebugCallDepth);
	vfprintf (stdout, sMessage, args);
	fprintf(logfile, "%s%d)", sLocalTabSpace, iDebugCallDepth);
	va_end(args);
	FFLUSH(logfile);

}


void SPX_DEBUG_LOGFILE_INFO(const char *sMessage, ...)
{
NO_LOG_RETURN;
	//char sLocalMessage[1024];
	//const char* sLocalMessage = sMessage;

	va_list args;
	//sprintf(sLocalMessage, "%s\n", sMessage);
	va_start (args, sMessage);
	vfprintf (logfile, sMessage, args);
	va_end(args);
	FFLUSH(logfile);

}

const char* __sDataTypes[16] = {
"raw0",	"img ",
"snd ",	"hmpm",
"rect",	"text",
"raw6",	"raw7",

"raw8",	"IRGB",
"rawA",	"word",
"ioff",	"pal ",
"rawE",	"rawF"
};

const char* SPX_STR_DATATYPE(unsigned int iType)
{
	if (iType >= 16)
		return "<<>>";
	return (const char*) __sDataTypes[iType];
}

// end of SPX