#include <skapi.h>

#include <skwinapp.h>

#include <stdio.h>

//------------------------------------------------------------------------------

SK_API int SK_GetContext(tSKWinContext* xSKWinContext)
{
	//xSKWinContext->xSkCore = a;
	printf("CALL CONTEXT\n");
	return 0;
}

SK_API int SK_ProceedOneGameStep(tSKWinContext* xSKWinContext)
{
	int iResult = 0;
	return xSKWinContext->xSkCore->SK_GAME_STEP();
}


SK_API int SK_SendUIEvent(tSKWinContext* xSKWinContext, UINT iUIEventCode)
{
	MousePosition xUIHandleData;
	xUIHandleData.XPos = 0;
	xUIHandleData.YPos = 0;
	xUIHandleData.event = iUIEventCode;
	return xSKWinContext->xSkCore->SK_HANDLE_UI_EVENT(&xUIHandleData);
}

SK_API int SK_SetMousePointer(tSKWinContext* xSKWinContext, UINT iXPos, UINT iYPos)
{
	xSKWinContext->xSkCore->SKExt_ForceMousePointer(iXPos, iYPos);
	return 0;
}


//------------------------------------------------------------------------------
