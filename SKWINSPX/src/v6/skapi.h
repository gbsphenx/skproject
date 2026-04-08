#ifndef _SKWIN_API_H_
#define _SKWIN_API_H_

//------------------------------------------------------------------------------

#include <skcore.h>

#ifdef __cplusplus
extern "C" {
#endif

//------------- Export / Import macro ------------------------------------------

#if defined(_WIN32) || defined(_WIN64)
    #if defined (SKDLL_EXPORTS) || defined (LIBSKWINDLL_EXPORTS)
		// LIBSKWINDLL_EXPORTS is default MSVC6
        #define SK_API __declspec(dllexport)
    #else
    //    #define SK_API __declspec(dllimport)
		#define SK_API
    #endif
#else
    #define SK_API
#endif

//------------- Public API -----------------------------------------------------

typedef struct
{
	SkWinCore* xSkCore;
} tSKWinContext;


SK_API int SK_MainFromCommandLine(const char* sCommandLine);
SK_API int SK_MainStartHeadless(tSKWinContext* xSKWinContext, const char* sCommandLine);
SK_API int SK_GameLoad(tSKWinContext* xSKWinContext);
SK_API int SK_StartGameLoop(tSKWinContext* xSKWinContext);
SK_API int SK_ProceedOneGameStep(tSKWinContext* xSKWinContext);
SK_API int SK_SendUIEvent(tSKWinContext* xSKWinContext, UINT iUIEventCode);
//


SK_API int SK_GetContext(tSKWinContext* xSKWinContext);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------

#endif // _SKWIN_API_H_
