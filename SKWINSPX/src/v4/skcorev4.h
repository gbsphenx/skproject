//------------------------------------------------------------------------------
// SKCODE CORE V4
//	Main structure for common global variables
//------------------------------------------------------------------------------

#ifndef _SKCODE_CORE_V4_H_
#define _SKCODE_CORE_V4_H_

#include <sktypes.h>

//..............................................................................
//------------------------------------------------------------------------------

typedef struct {


} SKCoreGameState;

//..............................................................................

typedef struct {

	U8	glbCreaturesMaxCount;	// (_4976_4c56) Nb of different types of creatures within GDAT
	U8*	glbSomeCreatureTable;	// (_4976_4c58) 3 values per creature type

} SKCoreGameGeneral;


//..............................................................................

// cd.mo.
typedef struct {

	i16	glbSpecialScreen;
	X8* glbImageCreditScreen;		// (_4976_52ae)
	X8* glbImageMenuScreen;			// (_4976_52b2)

} SKCoreMenuOption;

//..............................................................................

// cd.mk.
typedef struct {

	U16	_04bf_17a8;	// _04bf_17a8

	U16 mice_btn;
	U16 mice_x;
	U16 mice_y;

	U16	glbMouseButtonState;		// (_04bf_17ee) mouse push state (latest); bit0=left, bit1=right
	U16	glbMouseXPos;		// (_04bf_03be) mouse x-pos
	U16	glbMouseYPos;		// (_04bf_03c0) mouse y-pos
	U16	glbMousePreviousXPos;		// (_04bf_03c2) prev xpos
	U16	glbMousePreviousYPos;		// (_04bf_03c4) prev ypos

} SKCoreMouseKeyboard;

// cd.sc.
typedef struct {

	U16	glbSoundBlasterBasePort;		// (_04bf_0288) sblaster base port #
	U16	glbSoundCardType;		// (_04bf_028a) sndcard type: sblaster=6
	U16	sndSoundInBuffer;		// (_04bf_028c) =1
	U16	sndSoundBufferSize;		// (_04bf_028e)
	X8* sndSoundBuffer;	// (_04bf_0290) sound buffer
	U16	sndSoundPlaybackFrequency;		// (_04bf_17a6) playback speed

	X16	_04bf_04f1;

} SKCoreSoundCard;

//..............................................................................
//------------------------------------------------------------------------------

typedef struct {

	SKCoreGameGeneral gg;
	SKCoreGameState	gs;
	SKCoreMenuOption mo;

	SKCoreSoundCard sc;
	SKCoreMouseKeyboard mk;

} SKCoreData;

//------------------------------------------------------------------------------

#endif // _SKCODE_CORE_V4_H_