//------------------------------------------------------------------------------
// SK-CORE
//	Main base core code for running DM2
//------------------------------------------------------------------------------

#include <skver.h>

#include <skparam.h>
#include <skcore.h>
//#include <skcnsole.h>

#include <stdlib.h>
#include <string.h>

//------------------------------------------------------------------------------

extern int SKWIN_V5BITDOS_MAIN(SkWinApp* skWinApplication);

//------------------------------------------------------------------------------

#if defined(SKDOSV5) && !defined(SKWINSPX)
SkWinCore::SkWinCore() 
{
	skWinApp = NULL;
}
#endif // SKDOSV5 + !SKWINSPX
// With SKWINSPX, we take the CoreV4 constructor

UINT SkWinCore::SKWINV5_MAIN_START()
{
	return SKWIN_V5BITDOS_MAIN(skWinApp);
}