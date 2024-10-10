#include <StdAfx.h>

#include <stdio.h>
#include <stdarg.h> // va_start, etc ...
#include <SkDebug.h>
#include <SkGlobal.h> // for LPCSTR

//--- SkwinDEBUG ---------------------------------------------------------------

#ifdef _USE_MFC60 // Vc98(Vc6)
void SkwinDEBUG (int lv, LPCSTR psz, ...) {
	if (lv < DLV_BUGHERE) return;

	CString str;
	va_list va;
	va_start(va, psz);
	str.FormatV(psz, va);
	va_end(va);
	fprintf(stderr, "%s", (LPCSTR)str);
}
#endif // _USE_MFC60


#ifdef _USE_MFC80 // Vs2005 or later
void SkwinDEBUG (int lv, LPCSTR psz, ...) {
	if (lv < DLV_BUGHERE) return;

	CStringA str;
	va_list va;
	va_start(va, psz);
	str.FormatV(psz, va);
	va_end(va);
	fprintf(stderr, "%s", (LPCSTR)str);
}
#endif // _USE_MFC80

#ifdef _USE_SDL
void SkwinDEBUG (int lv, const char *psz, ...) {
	if (lv < DLV_BUGHERE) return;

	va_list va;
	va_start(va, psz);
	vfprintf(stderr, psz, va);
	va_end(va);
}
#endif // _USE_SDL


#ifdef __DJGPP__
void SkwinDEBUG (int lv, const char *psz, ...) {
	if (lv < DLV_BUGHERE) return;

	va_list va;
	va_start(va, psz);
	vfprintf(stderr, psz, va);
	va_end(va);
}
#endif // __DJGPP__


//--- Unreachable --------------------------------------------------------------

#if defined(_USE_MFC60) || defined(_USE_MFC80)
void Unreachable(LPCSTR funct, LPCSTR file, int line) {
	SkD((DLV_BUGHERE,"BUG? Unreachable funct %s at %s(%u)\n"
		, funct, file, line));
	ATLASSERT(false);
}
#endif // defined(_USE_MFC80) || defined(_USE_MFC60)

#if defined(_USE_SDL) || defined(__DJGPP__)
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


// SPX: I rewrite this in more C-ish form
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
		//vfprintf (stdout, message, args);
		//fprintf (stdout, "\n");
		fflush(flog);
	}
	va_end(args);
#endif
}











//------------------------------------------------------------------------------