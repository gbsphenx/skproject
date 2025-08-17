//#include <stdafx.h>
#if (_MSC_VER >= 1200)
	#include <winatl.h>
#endif

#include <skver.h>
#include <stdio.h>
#include <stdarg.h> // va_start, etc ...
#include <skdebug.h>

//--- SkwinDEBUG ---------------------------------------------------------------

#ifdef _USE_SDL
void SkwinDEBUG (int lv, const char *psz, ...) {
	if (lv < DLV_BUGHERE) return;

	va_list va;
	va_start(va, psz);
	vfprintf(stderr, psz, va);
	va_end(va);
}
#endif // _USE_SDL


#if defined(__MINGW__) || defined(__DJGPP__) || defined(__LINUX__) || (_MSC_VER >= 1200)
void SkwinDEBUG (int lv, const char *psz, ...) {
	if (lv < DLV_BUGHERE) return;

	va_list va;
	va_start(va, psz);
	vfprintf(stderr, psz, va);
	va_end(va);
}
#endif // __DJGPP__


//--- Unreachable --------------------------------------------------------------

#if (_MSC_VER >= 1200)
void Unreachable(LPCSTR funct, LPCSTR file, int line) {
	SkD((DLV_BUGHERE,"BUG? Unreachable funct %s at %s(%u)\n"
		, funct, file, line));
	ATLASSERT(false);
}
#endif // defined(_MSVC6_) || defined(_MSVC8_)

#if defined(_USE_SDL) || defined(__DJGPP__) || defined(__MINGW__) || defined(__LINUX__)
void Unreachable(const char *funct, const char *file, int line) {
	SkD((DLV_BUGHERE,"BUG? Unreachable funct %s at %s(%u)\n"
		, funct, file, line));
	ATLASSERT(false);
}
#endif // _USE_SDL

//------------------------------------------------------------------------------


/*
void Write2LOGX(LPCTSTR pszFormat, ...) {
#if DLV_USE_LOGX
	CStringA str;
	va_list va;
	va_start(va, pszFormat);
	str.FormatV(pszFormat, va);
	va_end(va);

	++g_cntx;

	static FILE *f = NULL;
	if (f == NULL)
		f = fopen("LOGX.txt", "wt");
	if (f != NULL) {
		fprintf(f, "%u %s\n"
			, g_cntx
			, static_cast<LPCSTR>(str)
			);
		fflush(f);
	}
#endif
}
*/

// SPX: my version
void Write2LOGX(const char *message, ...) {
#if DLV_USE_LOGX

	va_list args;
	va_start (args, message);

	static FILE *flog = NULL;
	if (flog == NULL)
		flog = fopen("LOGX.txt", "wt");
	if (flog != NULL)
	{
		vfprintf (flog, message, args);
		fprintf (flog, "\n");
		fflush(flog);
	}
	va_end(args);
#endif
}











//------------------------------------------------------------------------------