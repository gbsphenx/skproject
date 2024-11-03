//------------------------------------------------------------------------------

#ifndef _SKWIN_DEBUG_H_
#define _SKWIN_DEBUG_H_

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#define SkD(X) SkwinDEBUG X
#ifdef _DEBUG
	#define LOGX(X) Write2LOGX X
#else
	#define LOGX(X) ;;
#endif

#define DLV_BUGHERE 1
#define DLV_CPX 0
#define DLV_FYI 0
#define DLV_GUI 0
#define DLV_MEM 0
#define DLV_GLD 0
#define DLV_DBM 0
#define DLV_DYN 0
#define DLV_SYS 0
#define DLV_RCT 0
#define DLV_FIL 0	// Track file open (useful for custom GDAT)
#define DLV_DBC 0
#define DLV_CAI 0
#define DLV_TWEET 0
#define DLV_MOVE 0	// Player moves
#define DLV_EUI 0	// Event from UI code
#define DLV_MOUSE 0	// Mouse
#define DLV_CCM 0	// Creature command
#define DLV_FSM 0	// Find special marker (for x teleport and void fall)
#define DLV_XP	1	// Display info about XP gain
#define DLV_DBG_GETPIC 0
#define DLV_DBG_CELL 0
#define DLV_DBG_EXTRACT 0
#define DLV_DBG_TIMER 0
#define DLV_DBG_CAI 0
#define DLV_DBG_RAND 0
#define DLV_DBG_CEL2 0
#define DLV_DBG_SEED 0
#define DLV_DBG_SED2 0
#define DLV_DBG_SED3 0
#define DLV_DBG_RAIN 0		// Rain/Storm information
#define DLV_DBG_DIST 0		// Distant element
#define DLV_DBG_THUNDER	0
#define DLV_DBG_SND_CRE 0	// Sound requested by creature
#define DLV_DBG_SND_OUT 0	// Sound played
#define DLV_DBG_TICK	0	// Tick information
#define DLV_DBG_DOOR	0

#define DLV_USE_LOGX 1

//------------------------------------------------------------------------------

//// Check if it is the only version ? (this for 80 and 60 only?)
#if defined(_USE_MFC80) || defined(_USE_MFC60)
void SkwinDEBUG (int lv, LPCSTR psz, ...);
#elif defined(_USE_SDL)
void SkwinDEBUG (int lv, const char* psz, ...);
#elif defined(__DJGPP__) || defined(__MINGW__) || defined(__LINUX__)
void SkwinDEBUG (int lv, const char* psz, ...);
#endif //

#ifdef _USE_MFC60
	#define Unr() Unreachable("?", __FILE__, __LINE__)
	void Unreachable(LPCSTR funct, LPCSTR file, int line);
#elif defined(_USE_MFC80)
	#define Unr() Unreachable(__FUNCTION__, __FILE__, __LINE__)
	void Unreachable(LPCSTR funct, LPCSTR file, int line);
#elif defined(_USE_SDL)
	#define Unr() Unreachable(__FUNCTION__, __FILE__, __LINE__)
	void Unreachable(const char *funct, const char *file, int line);
#elif defined(__DJGPP__) || defined(__MINGW__) || defined(__LINUX__)
	#define Unr() Unreachable(__FUNCTION__, __FILE__, __LINE__)
	void Unreachable(const char *funct, const char *file, int line);
#endif



void Write2LOGX(const char *message, ...);

//------------------------------------------------------------------------------

#endif // _SKWIN_DEBUG_H_

//------------------------------------------------------------------------------

