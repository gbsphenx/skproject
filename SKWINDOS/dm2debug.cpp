#include <stdio.h>

// SPX: special code for displaying structure info and values
#include "addon.h"
#include "driver.h" // DRVW_mousevisicnt
#include "dm2global.h"
#include "dm2data.h"
#include "mdata.h"
#include "src/c_alloc.h"
#include "src/c_dballoc.h"
#include "src/c_mouse.h"
#include "src/c_gfx_pal.h"
#include "src/c_gfx_stretch.h"
#include "src/c_gfx_misc.h"
#include "src/c_gfx_blit.h"
#include "src/c_gfx_str.h"
#include "src/c_gfx_fill.h"
#include "src/c_gfx_decode.h"
#include "src/c_savegame.h"
#include "calls.h"


#define LOGX(X) SPX_DEBUG_LOGFILE_INFO X
#ifdef NDEBUG
	#define LOGX(X) ;;
#endif

//------------------------------------------------------------------------------

#define glbGDatNumberOfData			ddata.gdatentries


#define MAX(a,b) ( (a > b) ? a : b )
#define MIN(a,b) ( (a < b) ? a : b )

//------------------------------------------------------------------------------


// SPX: Add function to detail important GDAT values, in order to compare to other SkWin versions
void DEBUG_DISPLAY_GDAT_MAIN_INFO()
{
	unsigned int iItemIndex = 0;
	LOGX(("DEBUG_DISPLAY_GDAT_MAIN_INFO\n"));
	LOGX(("#Items: %04d\n", glbGDatNumberOfData));
	for (iItemIndex = 0; iItemIndex < glbGDatNumberOfData; iItemIndex++) 
	{
		//LOGX(("ShelfMemTab: %04d => Val = %08X [%d] (%05d)", iItemIndex, glbShelfMemoryTable[iItemIndex].val, glbShelfMemoryTable[iItemIndex].Absent(), (glbShelfMemoryTable[iItemIndex].val & 0x7FFFFFF) ));
		LOGX(("ShelfMemTab: %04d => Val = %08X [%d] (%05d)\n", iItemIndex, 
			dm2_ulp.getv(iItemIndex), dm2_ulp.Absent(iItemIndex), (dm2_ulp.getv(iItemIndex) & 0x7FFFFFF) ));
	}
	// Max of each category
	unsigned int iCategoryIndex = 0;
	for (iCategoryIndex = 0; iCategoryIndex < 26; iCategoryIndex++) 
	{
		unsigned int iMaxItemsForCategory = 0;
		iMaxItemsForCategory = SKW_3e74_2439(iCategoryIndex, 0x0B);
		LOGX(("Category: %02d => # %03d\n", iCategoryIndex, iMaxItemsForCategory ));
	}
	LOGX(("============================\n"));

}

void DEBUG_DUMP_BIN_DATA(unsigned char* ptr, unsigned int iDumpSize, unsigned int iBytesPerRow)
{
	for (unsigned int iCursor = 0; iCursor < iDumpSize; iCursor++)
	{
		LOGX(("%02X ", ptr[iCursor]));
		if (iCursor%iBytesPerRow == (iBytesPerRow-1) )
			LOGX(("\n"));
	}
}


void DEBUG_DUMP_ULP()
{
	unsigned int iItemNumber = 0;
LOGX(("============================\n"));
LOGX(("ULP >> \n"));
	for (iItemNumber = 0; iItemNumber < glbGDatNumberOfData; iItemNumber++)
	{
		char sInfoPointer[8];
		char sData[128];
		memset(sInfoPointer, 0, 8);
		memset(sData, 0, 128);
		u_lp* p = dm2_ulp.getadr(iItemNumber);
		if (p != NULL && (void*) p < (void*) 0x70000000)	// assume else it is not valid
		{
			unsigned int iLength = 0;
			iLength = dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(iItemNumber);
			iLength= iLength & 0x7FFFFFFF;
			
			if (p->l_00 & 0x80000000) // not a direct pointer
			{
				sprintf(sInfoPointer, "(np)");
			}
			else // this is a pointer, then we try to display the first data
			{
				unsigned int iMaxBytesToDisplay = MIN(16, iLength);
				unsigned char* pData;
				char* sWriteData = (char*) sData;
				pData = (unsigned char*) p->p_00;
				for (unsigned int iByte = 0; iByte < iMaxBytesToDisplay; iByte++)
				{
					sprintf(sWriteData, "%02X ", pData[iByte]);
					pData++;
					sWriteData+=3;
				}

			}
			
			LOGX(("u_lp #%04d: %08x || x32 = %08x %4s -> len = %05d (%04X) || i8* = %08X  || Data = %s\n", iItemNumber, p, p->l_00, sInfoPointer, iLength, iLength, p->p_00, sData)); 
				
		}
		else
			LOGX(("p %04d: %08x \n", iItemNumber, p));
	}
LOGX(("============================\n"));
}
/*
  union u_lp
  {
    x32 l_00;
    xxx* p_00;
  };
*/

char xStaticDebugLoadEnt[32];

const char* DEBUG_SKLOADENT(ui8* xSkLoadEnt)
{
	unsigned int iCls1Category = 0;
	unsigned int iCls2 = 0;
	unsigned int iCls3 = 0;
	unsigned int iCls4 = 0;
	memset(xStaticDebugLoadEnt, 0, 32);

	iCls1Category = xSkLoadEnt[2];
	iCls2 = xSkLoadEnt[3];
	iCls3 = xSkLoadEnt[4];
	iCls4 = xSkLoadEnt[5];


	sprintf(xStaticDebugLoadEnt, "%02X-%02X-%02X-%02X", iCls1Category, iCls2, iCls3, iCls4);
	return xStaticDebugLoadEnt;
}

char xStaticDebugGDATEnt[128];

const char* DEBUG_SKGDATENT(s_gdat* xSkGDATEnt)
{
	char sLoadEntFrom[32];
	char sLoadEntTo[32];
	unsigned int iValue32 = 0;
	unsigned char* xRawPointer = NULL;

	memset(sLoadEntFrom, 0, 32);
	memset(sLoadEntTo, 0, 32);
	memset(xStaticDebugGDATEnt, 0, 128);


	iValue32 = xSkGDATEnt->l_00;
	xRawPointer = (unsigned char*) xSkGDATEnt->p_10;
	sprintf(sLoadEntFrom, "%s", DEBUG_SKLOADENT((ui8*)&xSkGDATEnt->s_04));
	sprintf(sLoadEntTo, "%s", DEBUG_SKLOADENT((ui8*)&xSkGDATEnt->s_0a));


	sprintf(xStaticDebugGDATEnt, "%s to %s -=- x32 = %08x / pRaw = %08x", sLoadEntFrom, sLoadEntTo, iValue32, xRawPointer);
	return xStaticDebugGDATEnt;

}

// End of SPX
