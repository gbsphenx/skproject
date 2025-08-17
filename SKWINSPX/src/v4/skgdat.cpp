//------------------------------------------------------------------------------
// SK-GRAPHICS.DAT
//	Graphics.dat entries/items/data management
//------------------------------------------------------------------------------

//#include <StdAfx.h>	// Required for MVC6 compilation
#include <skcore.h>

#include <skglobal.h>
//#if defined(_USE_MFC80) || defined(_USE_MFC60)
//#include <SKMFC.h>
//#include <resource.h>
//#endif // defined(_USE_MFC80) || defined(_USE_MFC60)

//#include <DME.h>

//^3E74:1562
SkEnt4 SkWinCore::MAKE_ENT4(U8 cls1, U8 cls2, U8 cls3, U8 cls4)
{
	//^3E74:1562
	ENTER(4);
	//^3E74:1566
	SkEnt4 b4;
	b4.cls1(cls1);
	b4.cls2(cls2);
	b4.cls3(cls3);
	b4.cls4(cls4);
	return b4;
}
//^482B:0624
void SkWinCore::_482b_0624(U8 cls1, U8 cls2, U8 cls4)
{
	//^482B:0624
	//printf("_482b_0624 GDAT %d/%d/%d\n", cls1, cls2, cls4); getch();
	ENTER(4);
	//^482B:0628
	//printf("_482b_0624 GDAT %d/%d/%d QUERY_SND_ENTRY_INDEX\n", cls1, cls2, cls4); getch();
#ifndef __DJGPP__
	if (QUERY_SND_ENTRY_INDEX(cls1, cls2, cls4) == 0 && _4976_49d4[0][0] < _4976_5d58) {
		sk5f06 *bp04 = &_4976_5f06[_4976_49d4[0][0]];
		bp04->category = cls1;
		bp04->index = cls2;
		bp04->entry = cls4;
		bp04->w5 = 0xffff;
		//printf("_482b_0624 GDAT %d/%d/%d QUERY_SND_ENTRY_INDEX BLOCK\n", cls1, cls2, cls4); getch();
		_4976_49d4[0][0]++;
	}
#endif	
	//^482B:0682
	//printf("_482b_0624 GDAT %d/%d/%d END\n", cls1, cls2, cls4); getch();
	return;
}