
#ifndef _skwin_h_
#define _skwin_h_

#include <StdAfx.h>

struct CSkMinput {
	i16 btn, x, y;
};
struct CSkKinput {
	U8 raw;
};

#define MAXMICEIN 16U
#define MAXKEYBIN 16U

class CSkWin {
public:
	CSkMinput xMiceInput[MAXMICEIN];
	int cntMiceIn;
	CSkKinput xKeybInput[MAXKEYBIN];
	int cntKeybIn;
	int curKeybIn;
	bool enableDoubleStepMove;

public:
	CSkWin() {
		enableDoubleStepMove = true;
	}
	CSkKinput *DequeueKinput() {
		CSkKinput *p = NULL;
		if (cntKeybIn != 0) {
			p = &xKeybInput[curKeybIn];
			curKeybIn = (curKeybIn +1) % MAXKEYBIN;
			cntKeybIn--;
		}
		return p;
	}

protected:
	CSkMinput *allocMinput() {
		if (cntMiceIn < MAXMICEIN) {
			CSkMinput *p = &xMiceInput[cntMiceIn];
			cntMiceIn++;
			return p;
		}
		return NULL;
	}
	CSkKinput *allocKinput() {
		if (cntKeybIn < MAXKEYBIN) {
			CSkKinput *p = &xKeybInput[(curKeybIn +cntKeybIn) % MAXKEYBIN];
			cntKeybIn++;
			return p;
		}
		return NULL;
	}
};

#endif // _skwin_h_
