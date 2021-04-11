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
	for (int iCursor = 0; iCursor < iDumpSize; iCursor++)
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
		u_lp* p = dm2_ulp.getadr(iItemNumber);
		if (p != NULL && (void*) p < (void*) 0x70000000)	// assume else it is not valid
		{
			LOGX(("p %04d: %08x x32 = %05d ptr %08X\n", iItemNumber, p, p->l_00, p->p_00)); 
				
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
// End of SPX
